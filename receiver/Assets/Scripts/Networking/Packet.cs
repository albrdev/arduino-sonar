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
    }
}
