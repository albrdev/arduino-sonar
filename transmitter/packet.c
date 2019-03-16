#include "packet.h"
#include "crc.h"

int packet_verify(const struct _packet_header *const pkt, const size_t size, const uint16_t checksum)
{
    return mkcrc16((uint8_t *)pkt + sizeof(pkt->checksum), size - sizeof(pkt->checksum)) == checksum;
}

void packet_mkheader(struct _packet_header *const pkt, const size_t size, const uint8_t type)
{
    pkt->type = type;

    // Checmsum has to be calculated last
    // Checksum can't be taken into account for the checksum itself, therefore start at an offset to avoid it
    // The length of the data to be calculated over is now total size minus the checksum size to avoid going outside allowed memory

    // Packet in bits: C = checksum, T = type, D = data (size may vary)
    // CCCCCCCCCCCCCCCCTTTTTTTTDDDDDDDDDDDDDDDD
    //                 ^---------------------->| size - sizeof(checksum)

    pkt->checksum = mkcrc16((uint8_t *)pkt + sizeof(pkt->checksum), size - sizeof(pkt->checksum));
}

void packet_mksonardata(struct _packet_sonardata *const pkt, const float distance, const float angle)
{
    memcpy(&pkt->distance, &distance, sizeof(pkt->distance));
    memcpy(&pkt->angle, &angle, sizeof(pkt->angle));

    packet_mkheader(&pkt->header, sizeof(*pkt), PT_SONARDATA);
}
