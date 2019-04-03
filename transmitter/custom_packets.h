#ifndef __CUSTOM_PACKETS_H__
#define __CUSTOM_PACKETS_H__

#include <stdint.h> /* uint32_t */
#include <string.h> /* size_t, memcpy() */
#include "packet.h"

typedef struct _packet_sonardata packet_sonardata_t;

struct __attribute__((packed)) _packet_sonardata
{
    packet_header_t header;

    uint32_t distance;
    uint32_t angle;
};

#ifdef __cplusplus
extern "C"
{
#endif
    void packet_mksonardata(struct _packet_sonardata *const pkt, const float distance, const float angle);
#ifdef __cplusplus
} // extern "C"
#endif

#endif // __CUSTOM_PACKETS_H__
