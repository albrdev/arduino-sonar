#ifndef __PACKET_H__
#define __PACKET_H__

#include <stddef.h> /* size_t */
#include <stdint.h> /* uint8_t, uint16_t */

typedef struct _packet_header packet_header_t;

enum PacketType
{
    PT_FAILURE = 0,
    PT_SUCCESS = 1,
    PT_SONARDATA = 2
};

struct __attribute__((packed)) _packet_header
{
    uint16_t checksum;
    uint8_t type;
};

#ifdef __cplusplus
extern "C"
{
#endif
    int packet_verify(const struct _packet_header *const pkt, const size_t size, const uint16_t checksum);
    void packet_mkheader(struct _packet_header *const pkt, const size_t size, const uint8_t type);
#ifdef __cplusplus
} // extern "C"
#endif

#endif // __PACKET_H__
