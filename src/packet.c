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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "libpacket/packet.h"

/*------------------------------ Protocol ------------------------------*/

Protocol_t * Protocol_createWithParams(
        Protocol_getSizeFunc_t getSize,
        Protocol_getBitstreamFunc_t getBitstream,
        StackItem_t *item,
        void *owner) {
    Protocol_t *proto;

    proto = malloc(sizeof(Protocol_t));
    if (proto == NULL) {
        goto end;
    }

    StackItem_setOwner(item, proto);
    proto->item = item;
    proto->getSize = NULL;
    proto->getBitstream = NULL;
    proto->owner = NULL;

end:
    return proto;

}

Protocol_t * Protocol_create() {
    Protocol_createWithParams(
            NULL,
            NULL,
            StackItem_create(),
            NULL);
}

void Protocol_delete(Protocol_t *proto) {
    if (proto != NULL) {
        StackItem_delete(proto->item);
    }

    free(proto);
}

int Protocol_setItem(Protocol_t *proto, StackItem_t *item) {
    int ok = 0;

    if (proto == NULL) {
        goto end;
    }

    proto->item = item;
    ok = 1;

end:
    return ok;
}

StackItem_t * Protocol_getItem(const Protocol_t *proto) {
    return proto != NULL? proto->item: NULL;
}

int Protocol_setOwner(Protocol_t *proto, void *owner) {
    int ok = 0;

    if (proto == NULL || owner == NULL) {
        goto end;
    }

    proto->owner = owner;
    ok = 1;

end:
    return ok;
}

void * Protocol_getOwner(const Protocol_t *proto) {
    return proto != NULL? proto->owner: NULL;
}

int Protocol_setGetSize(Protocol_t *proto, Protocol_getSizeFunc_t func) {
    int res = -1;

    if (proto != NULL && func != NULL) {
        proto->getSize = func;
        res = 0;
    }

    return res;
}

Protocol_getSizeFunc_t Protocol_getGetSize(const Protocol_t *proto) {
    return proto != NULL? proto->getSize: NULL;
}

int Protocol_setGetBitstream(
        Protocol_t *proto,
        Protocol_getBitstreamFunc_t func) {
    int res = -1;

    if (proto != NULL && func != NULL) {
        proto->getBitstream = func;
        res = 0;
    }

    return res;
}

Protocol_getBitstreamFunc_t Protocol_getGetBitstream(const Protocol_t *proto) {
    return proto != NULL? proto->getBitstream: NULL;
}

/*------------------------------ Packet ------------------------------*/


//TODO: Right now Packet is accessing to the members of Stack, that needs to change.

Packet_t * Packet_createWithParams(Stack_t *stack) {
    Packet_t *pack;

    pack = malloc(sizeof(Packet_t));
    if (pack == NULL) {
        goto end;
    }

    if (Stack_setOwner(stack, pack) != 0) {
        free(pack);
        pack = NULL;
        goto end;
    }

    pack->stack = stack;

end:
    return pack;
}

Packet_t * Packet_create() {
    return Packet_createWithParams(Stack_create());
}

void Packet_delete(Packet_t *pack) {
    if (pack != NULL) {
        Stack_delete(pack->stack);
    }

    free(pack);
}

int Packet_stack(Packet_t *pack, Protocol_t *proto) {
    int ok = 0;

    if (pack == NULL || pack->stack == NULL || proto == NULL) {
        goto end;
    }

    ok = Stack_push(pack->stack, Protocol_getItem(proto));

end:
    return ok;
}

unsigned int Packet_getSize(const Packet_t *pack) {
    StackItem_t *iter;
    Stack_t *stack;
    Protocol_t *proto;
    unsigned int total_size = 0;

    if (pack == NULL || pack->stack == NULL) {
        goto end;
    }

    stack = pack->stack;
    for (iter = stack->bottom; iter != NULL; iter = iter->next) {
        proto = StackItem_getOwner(iter);
        if (proto != NULL) {
            total_size += proto->getSize(proto);
        } else {
            printf("%s: an owner of a StackItem_t shouldn't be NULL\n", __FUNCTION__);
        }
    }

end:
    return total_size;
}

int Packet_getBitstream(
        const Packet_t *pack,
        uint8_t *buf,
        unsigned int size) {
    StackItem_t *iter;
    Stack_t *stack;
    Protocol_t *proto;
    unsigned int current_offset = 0;
    int ok = 0;

    if (pack == NULL 
            || pack->stack == NULL
            || buf == NULL
            || size < Packet_getSize(pack)) {
        goto end;
    }

    stack = pack->stack;
    for (iter = stack->bottom; iter != NULL; iter = iter->next) {
        proto = StackItem_getOwner(iter);
        if (proto != NULL) {
            current_offset += proto->getBitstream(
                    Protocol_getOwner(proto),
                    &buf[current_offset],
                    size - current_offset);
        } else {
            printf("%s: an owner of a StackItem_t shouldn't be NULL\n", __FUNCTION__);
        }
    }

end:
    return ok;
}

