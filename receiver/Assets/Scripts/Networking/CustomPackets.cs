using System;
using System.Runtime.InteropServices;
using Assets.Scripts.Cryptography.CRC;

namespace Assets.Scripts.Networking
{
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SonarData
    {
        public Packet.Header Header { get; private set; }

        public UInt32 Distance { get; }
        public UInt32 Angle { get; }

        public unsafe SonarData(UInt32 distance, UInt32 angle) : this()
        {
            Header = new Packet.Header(0, (byte)Packet.PacketType.SonarData);
            Distance = distance;
            Angle = angle;

            int size = Marshal.SizeOf(this);
            byte[] buf = new byte[size];
            IntPtr unmanagedPointer = Marshal.AllocHGlobal(size);
            Marshal.Copy(buf, 0, unmanagedPointer, size);
            typeof(Packet.Header).GetProperty("Checksum").SetValue(Header, CRC16.Generate((byte*)(unmanagedPointer + sizeof(UInt16)), size - sizeof(UInt16)), null);
            Marshal.FreeHGlobal(unmanagedPointer);
        }
    }
}
