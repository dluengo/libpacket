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

#ifndef __LIBPACKET_ETHER
#define __LIBPACKET_ETHER

/**
 * @file ether.h
 * @author David "Ole" Luengo
 * @date 10 March 2018
 * @brief File implemening the Ethernet protocol.
 */

#include <stdint.h>

#include "libpacket/packet.h"

//TODO: Name needs to change as it can collide with other protocol
//implementations. ETHER_ADDR_LEN
#define ADDR_LEN (6)
/**
 * @class EtherProto
 * @brief Class implementing the Ethernet header.
 */
typedef struct EtherProto EtherProto_t;

typedef struct EtherProto {
    Protocol_t *proto_base;
    uint8_t daddr[ADDR_LEN];
    uint8_t saddr[ADDR_LEN];
    uint16_t type;
} EtherProto_t;

/**
 * @memberof EtherProto
 *
 * Class constructor. Allocates and initializes by default a EtherProto
 * instance. By default the destination address is 00:11:22:33:44:55, the
 * source address is 66:77:88:99:aa:bb and type is IPv4 (0x800).
 *
 * @return A pointer to the newly allocated EtherProto or NULL.
 */
EtherProto_t * EtherProto_create(void);

/**
 * @memberof EtherProto
 *
 * Class destructor. Frees the resources associated to a EtherProto instance.
 *
 * @param proto Pointer to the instance to destroy.
 */
void EtherProto_delete(EtherProto_t *proto);

/**
 * @memberof EtherProto
 *
 * Method implementing the getSize() behavior needed by Protocol.
 *
 * @param proto Pointer to an instance of EtherProto.
 * @return The size in bytes of this instance.
 */
unsigned int EtherProto_getSize(const EtherProto_t *proto);

/**
 * @memberof EtherProto
 *
 * Method implementing the getBitstream() behavior needed by Protocol.
 *
 * @param proto Pointer to an instance of EtherProto.
 * @param buf Pointer to a buffer where to write the bytes.
 * @param size Maximum size of the buffer.
 * @return The number of bytes written into the buffer.
 */
int EtherProto_getBitstream(
        const EtherProto_t *proto,
        uint8_t *buf,
        unsigned int size);

/**
 * @memberof EtherProto
 * 
 * Getter of the member proto_base.
 *
 * @param proto Ponter to an instance of EtherProto.
 * @return A pointer to the Protocol instance own by this EtherProto instance.
 */
Protocol_t * EtherProto_getProtoBase(const EtherProto_t *proto);

#endif
