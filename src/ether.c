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

//TODO: Get rid of this library
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "libpacket/ether.h"

#define IPV4_TYPE (0x0800)
#define ETHER_HEADER_LEN (14)

EtherProto_t * EtherProto_create() {
    EtherProto_t *proto;
    int ok;
    proto = malloc(sizeof(EtherProto_t));
    if (proto == NULL) {
        goto end;
    }

    proto->proto_base = Protocol_create();
    if (proto->proto_base == NULL) {
        free(proto);
        proto = NULL;
        goto end;
    }

    //TODO: Don't access members, create setters/getters.
    proto->proto_base->getSize =
        (Protocol_getSizeFunc_t)EtherProto_getSize;
    proto->proto_base->getBitstream =
        (Protocol_getBitstreamFunc_t)EtherProto_getBitstream;
    ok = Protocol_setOwner(proto->proto_base, proto);
    if (!ok) {
        Protocol_delete(proto->proto_base);
        proto->proto_base = NULL;
        //TODO: Don't repeat code.
        free(proto);
        proto = NULL;
        goto end;
    }

    //TODO: This have to change.
    memcpy(proto->daddr, "\x00\x01\x02\x03\x04\x05", ADDR_LEN);
    memcpy(proto->saddr, "\x06\x07\x08\x09\x0a\x0b", ADDR_LEN);
    proto->type = IPV4_TYPE;

end:
    return proto;
}

void EtherProto_delete(EtherProto_t *proto) {
    if (proto != NULL) {
        Protocol_delete(proto->proto_base);
    }

    free(proto);
}

unsigned int EtherProto_getSize(const EtherProto_t *proto) {
    return ETHER_HEADER_LEN;
}

int EtherProto_getBitstream(
        const EtherProto_t *proto,
        uint8_t *buf,
        unsigned int size) {
    uint16_t type;

    //TODO: Pointer checking?!
    if (size < ETHER_HEADER_LEN) {
        goto end;
    }

    type = htons(proto->type);
    memcpy(buf, proto->daddr, ADDR_LEN);
    memcpy(buf + ADDR_LEN, proto->saddr, ADDR_LEN);
    memcpy(&buf[ADDR_LEN*2], &type, sizeof(proto->type));

end:
    return ETHER_HEADER_LEN;
}

Protocol_t * EtherProto_getProtoBase(const EtherProto_t *proto) {
    return proto != NULL? proto->proto_base: NULL;
}

