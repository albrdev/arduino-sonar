#include "custom_packets.h"
#include "crc.h"

void packet_mksonardata(struct _packet_sonardata *const pkt, const float distance, const float angle)
{
    memcpy(&pkt->distance, &distance, sizeof(pkt->distance));
    memcpy(&pkt->angle, &angle, sizeof(pkt->angle));

    packet_mkheader(&pkt->header, sizeof(*pkt), PT_SONARDATA);
}
