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

#include "libpacket/ipv4.h"

Ipv4Proto_t * Ipv4Proto_create() {
    Ipv4Proto_t *proto;
    int ok;

    proto = malloc(sizeof(Ipv4Proto_t));
    if (proto == NULL) {
        goto end;
    }

    proto->proto_base = Protocol_create();
    if (proto->proto_base == NULL) {
        free(proto);
        proto = NULL;
        goto end;
    }

    //TODO: Use macros for these constants. Don't access Protocol_t members, use setters/getters.
    proto->version = 4;
    proto->hdr_length = 5;
    proto->tos = 0;
    proto->length = 20;
    proto->id = 42;
    proto->flags = 0;
    proto->frag_off = 0;
    proto->ttl = 64;
    proto->checksum = 0x1234;
    proto->saddr = 0x11223344;
    proto->daddr = 0x55667788;
    proto->opts_padding = NULL;

    proto->proto_base->getSize =
        (Protocol_getSizeFunc_t)Ipv4Proto_getSize;
    proto->proto_base->getBitstream =
        (Protocol_getBitstreamFunc_t)Ipv4Proto_getBitstream;
    //TODO: Create Protocol_create(void *owner)
    //TODO: Follow same approach as ether.
    ok = Protocol_setOwner(proto->proto_base, proto);
    if (!ok) {
        Protocol_delete(proto->proto_base);
        proto->proto_base = NULL;
        free(proto);
        proto = NULL;
    }

end:
    return proto;
}

void Ipv4Proto_delete(Ipv4Proto_t *proto) {
    if (proto != NULL) {
        free(proto->proto_base);
    }

    free(proto);
}

unsigned int Ipv4Proto_getSize(const Ipv4Proto_t *proto) {
    //TODO: This really needs to change!
    return 20;
}

int Ipv4Proto_getBitstream(
        const Ipv4Proto_t *proto,
        uint8_t *buf,
        unsigned int size) {
    uint16_t length, id;
    uint16_t checksum;
    uint32_t saddr;
    uint32_t daddr;

    //TODO: Pointer checking?!
    length = htons(proto->length);
    id = htons(proto->id);
    checksum = htons(proto->checksum);
    saddr = htonl(proto->saddr);
    daddr = htonl(proto->daddr);

    buf[0] = proto->version << 4 | proto->hdr_length;
    buf[1] = proto->tos;
    memcpy(&buf[2], &length, 2);
    memcpy(&buf[4], &id, 2);
    buf[6] = proto->flags << 5 | proto->frag_off >> 11;
    buf[7] = proto->frag_off & 0x1f;
    buf[8] = proto->ttl;
    buf[9] = proto->proto;
    memcpy(&buf[10], &checksum, sizeof(checksum));
    memcpy(&buf[12], &saddr, sizeof(saddr));
    memcpy(&buf[16], &daddr, sizeof(daddr));
    return 20;
}

Protocol_t * Ipv4Proto_getProtoBase(const Ipv4Proto_t *proto) {
    return proto->proto_base;
}

int Ipv4Proto_setLength(Ipv4Proto_t *proto, uint16_t length) {
    int res = -1;

    if (proto != NULL) {
        proto->length = length;
        res = 0;
    }

    return res;
}

uint16_t Ipv4Proto_getLength(const Ipv4Proto_t *proto) {
    return proto != NULL? proto->length: 0;
}

int Ipv4Proto_setProtocol(Ipv4Proto_t *proto, uint8_t proto_num) {
    int res = -1;

    if (proto != NULL) {
        proto->proto = proto_num;
        res = 0;
    }

    return res;
}

uint8_t Ipv4Proto_getProtocol(const Ipv4Proto_t *proto) {
    return proto != NULL? proto->proto: 0;
}

