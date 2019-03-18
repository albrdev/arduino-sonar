using System;
using System.Collections.Generic;
using System.Linq;
using System.IO.Ports;
using System.Threading;
using System.Runtime.InteropServices;
using UnityEngine;
using Assets.Scripts.Networking;
using Assets.Scripts.Cryptography.CRC;
using Assets.Scripts.ExtensionClasses;
using System.Diagnostics;
using System.Text;

[StructLayout(LayoutKind.Sequential, Pack = 1), Serializable]
public struct SonarInput
{
    public float Distance { get; }
    public float Angle { get; }

    public Quaternion Rotation
    {
        get { return Quaternion.AngleAxis(Angle, Vector3.back); }
    }

    public Vector3 Direction
    {
        get { return (Rotation * Vector3.up); }
    }

    public Vector3 Position
    {
        get { return Direction * Distance; }
    }

    private static float ConvToFloat(object obj)
    {
        int rawsize = Marshal.SizeOf(obj);
        IntPtr buffer = Marshal.AllocHGlobal(rawsize);
        byte[] rawdata = new byte[rawsize];
        Marshal.StructureToPtr(obj, buffer, true);
        Marshal.Copy(buffer, rawdata, 0, rawsize);
        Marshal.FreeHGlobal(buffer);
        return System.BitConverter.ToSingle(rawdata, 0);
    }

    public static unsafe implicit operator SonarInput(Packet.SonarData* packet)
    {
        return new SonarInput(ConvToFloat(packet->Distance), ConvToFloat(packet->Angle));
    }

    public override string ToString()
    {
        return string.Format("{0}({1}, {2})", nameof(SonarInput), Distance, Angle);
    }

    public SonarInput(float distance, float angle)
    {
        Distance = distance;
        Angle = angle;
    }
}

public struct SerialPortSettings
{
    public static readonly SerialPortSettings Default = new SerialPortSettings
    {
        BaudRate = 9600,
        Parity = Parity.None,
        DataBitSize = 8,
        StopBitCount = StopBits.One,
        HandshakeMode = Handshake.None,
        ReadTimeout = 500,
        WriteTimeout = 500,
        LoopDelay = 100
    };

    public int BaudRate { get; set; }
    public Parity Parity { get; set; }
    public int DataBitSize { get; set; }
    public StopBits StopBitCount { get; set; }
    public Handshake HandshakeMode { get; set; }
    public int ReadTimeout { get; set; }
    public int WriteTimeout { get; set; }
    public int LoopDelay { get; set; }
}

public static class SonarSerialReceiver
{
    private static SerialPort s_SerialPort;
    private static Thread s_ReceivingThread;
    private static bool s_IsActive = false;

    private static SerialPortSettings s_Settings;

    private static readonly object s_DataLock = new object();
    private static SonarInput s_Data;

    public static SonarInput Data
    {
        get
        {
            lock(s_DataLock)
            {
                return s_Data;
            }
        }

        private set
        {
            lock(s_DataLock)
            {
                s_Data = value;
            }
        }
    }

    public static event Action<SonarInput> OnUpdate = null;

    public static void Begin(string portName)
    {
        Begin(portName, SerialPortSettings.Default);
    }

    public static void Begin(string portName, SerialPortSettings settings)
    {
        s_ReceivingThread = new Thread(Read);

        s_SerialPort = new SerialPort();

        s_Settings = settings;

        s_SerialPort.PortName = portName;
        s_SerialPort.BaudRate = s_Settings.BaudRate;
        s_SerialPort.Parity = s_Settings.Parity;
        s_SerialPort.DataBits = s_Settings.DataBitSize;
        s_SerialPort.StopBits = s_Settings.StopBitCount;
        s_SerialPort.Handshake = s_Settings.HandshakeMode;
        //s_SerialPort.RtsEnable = true;//*
        //s_SerialPort.DtrEnable = true;//*
        //s_SerialPort.ReceivedBytesThreshold = Marshal.SizeOf(typeof(Packet.SonarData));//*

        s_SerialPort.ReadTimeout = s_Settings.ReadTimeout;
        s_SerialPort.WriteTimeout = s_Settings.WriteTimeout;

        //s_SerialPort.DataReceived += new SerialDataReceivedEventHandler(OnDataReceived);//*

        s_IsActive = true;
        s_SerialPort.Open();
        s_ReceivingThread.Start();//*
    }

    public static void End()
    {
        s_IsActive = false;
        s_ReceivingThread.Join();//*
        s_SerialPort.Close();
    }

    /*private static void OnDataReceived(object sender, SerialDataReceivedEventArgs e)
    {
    }*/

    private static void Read()
    {
        while(s_IsActive)
        {
            try
            {
                do
                {
                    //DebugStringBuilder debug = new DebugStringBuilder("RECV: ");//
                    int packetSize = Marshal.SizeOf(typeof(Packet.SonarData));

                    int packetCount = s_SerialPort.BytesToRead / packetSize;
                    //debug.Append("TotalSize={0}, PacketCount={1}", s_SerialPort.BytesToRead, packetCount);//
                    if(packetCount <= 0) break;

                    int packetChunkSize = packetCount * packetSize;
                    byte[] packetBuffer = new byte[packetChunkSize];
                    //debug.Append(", PacketsSize={0}", packetChunkSize);//

                    int readSize = s_SerialPort.Read(packetBuffer, 0, packetChunkSize);
                    //debug.AppendLine(", ReadSize={0}", readSize);//
                    if(readSize % packetSize != 0) break;

                    IEnumerable<IEnumerable<byte>> bytes = packetBuffer.Split(packetSize);
                    unsafe
                    {
                        foreach(var packet in bytes)
                        {
                            fixed(byte* pointer = packet.ToArray())
                            {
                                //int checksumSize = Marshal.SizeOf(((Packet.SonarData*)pointer)->Header.Checksum);//
                                //debug.Append("PacketCRC={0}, LocalCRC={1}", ((Packet.SonarData*)pointer)->Header.Checksum, CRC16.Generate((byte*)(pointer + checksumSize), packetSize - checksumSize));//
                                if(Packet.Verify((Packet.Header*)pointer, packetSize))
                                {
                                    Data = (Packet.SonarData*)pointer;
                                    OnUpdate?.Invoke(Data);
                                    //debug.AppendLine(", VALID");//
                                    //debug.AppendLine(", {0}", Data);//
                                }
                                /*else
                                {
                                    //debug.AppendLine(", FAIL");//
                                    //debug.LogMode = DebugLogMode.Warning;//
                                }*/
                            }
                        }
                    }

                    //debug.Print();//
                } while(false);
            } // try
            catch(TimeoutException) { }

            Thread.Sleep(s_Settings.LoopDelay);
        }
    }
}
