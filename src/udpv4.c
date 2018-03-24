/* 
 This file is part of libpacket.

 libpacket is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 libpacket is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with libpacket.  If not, see <http://www.gnu.org/licenses/>
*/

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "libpacket/udpv4.h"
#include "libpacket/packet.h"

#define UDPV4_HEADER_LEN (8)

Udpv4Proto_t * Udpv4Proto_createWithParams(
        uint16_t sport,
        uint16_t dport,
        uint16_t length,
        uint16_t checksum,
        Protocol_t *proto_base) {
    Udpv4Proto_t *proto;

    proto = malloc(sizeof(Udpv4Proto_t));
    if (proto == NULL) {
        goto end;
    }

    proto->proto_base = proto_base;
    if (proto_base != NULL) {
        Protocol_setOwner(proto_base, proto);
        Protocol_setGetSize(
                proto_base,
                (Protocol_getSizeFunc_t)Udpv4Proto_getSize);
        Protocol_setGetBitstream(
                proto_base,
                (Protocol_getBitstreamFunc_t)Udpv4Proto_getBitstream);
    }

    proto->sport = sport;
    proto->dport = dport;
    proto->length = length;
    proto->checksum = checksum;

end:
    return proto;
}

Udpv4Proto_t * Udpv4Proto_create() {
    return Udpv4Proto_createWithParams(
            0,
            0,
            UDPV4_HEADER_LEN,
            0,
            Protocol_create());
}

unsigned int Udpv4Proto_getSize(const Udpv4Proto_t *proto) {
    return UDPV4_HEADER_LEN;
}

int Udpv4Proto_getBitstream(
        Udpv4Proto_t *proto,
        uint8_t *buf,
        unsigned int size) {
    uint16_t sport;
    uint16_t dport;
    uint16_t length;
    uint16_t checksum;
    int res = -1;

    if (proto == NULL || buf == NULL || size < UDPV4_HEADER_LEN) {
        goto end;
    }

    sport = htons(proto->sport);
    dport = htons(proto->dport);
    length = htons(proto->length);
    checksum = htons(proto->checksum);

    memcpy(buf, &sport, 2);
    memcpy(buf+2, &dport, 2);
    memcpy(buf+4, &length, 2);
    memcpy(buf+6, &checksum, 2);

end:
    return res;
}

Protocol_t * Udpv4Proto_getProtoBase(const Udpv4Proto_t *proto) {
    return proto != NULL? proto->proto_base: NULL;
}

