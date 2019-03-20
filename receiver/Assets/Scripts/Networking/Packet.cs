using System;
using System.Runtime.InteropServices;
using Assets.Scripts.Cryptography.CRC;

namespace Assets.Scripts.Networking
{
    public static class Packet
    {
        public enum PacketType : byte
        {
            Failure = 0,
            Success = 1,
            SonarData = 2
        }

        public static unsafe bool Verify(Header* pkt, Int32 size)
        {
            return CRC16.Generate((byte*)pkt + sizeof(UInt16), size - sizeof(UInt16)) == pkt->Checksum;
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct Header
        {
            public UInt16 Checksum { get; }
            public byte Type { get; }

            public Header(UInt16 checksum, byte type) : this()
            {
                Checksum = checksum;
                Type = type;
            }
        }

        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct SonarData
        {
            public Header Header { get; private set; }

            public UInt32 Distance { get; }
            public UInt32 Angle { get; }

            public unsafe SonarData(UInt32 distance, UInt32 angle) : this()
            {
                Header = new Header(0, (byte)PacketType.SonarData);
                Distance = distance;
                Angle = angle;

                int size = Marshal.SizeOf(this);
                byte[] buf = new byte[size];
                IntPtr unmanagedPointer = Marshal.AllocHGlobal(size);
                Marshal.Copy(buf, 0, unmanagedPointer, size);
                typeof(Header).GetProperty("Checksum").SetValue(Header, CRC16.Generate((byte*)(unmanagedPointer + sizeof(UInt16)), size - sizeof(UInt16)), null);
                Marshal.FreeHGlobal(unmanagedPointer);
            }
        }
    }
}
