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

#ifndef __LIBPACKET_IPV4
#define __LIBPACKET_IPV4

/**
 * @file ether.h
 * @author David "Ole" Luengo
 * @date 10 March 2018
 * @brief File implemening the IPv4 protocol.
 */

#include <stdint.h>

#include "libpacket/packet.h"

/**
 * @class Ipv4Proto
 * @brief Class implementing the IPv4 header.
 */
typedef struct Ipv4Proto Ipv4Proto_t;

//TODO: Don't use literals, but macros instead.
typedef struct Ipv4Proto {
    uint8_t version: 4;
    uint8_t hdr_length: 4;
    uint8_t tos;
    uint16_t length;
    uint16_t id;
    uint8_t flags: 3;
    uint16_t frag_off: 13;
    uint8_t ttl;
    uint8_t proto;
    uint16_t checksum;
    uint32_t saddr;
    uint32_t daddr;
    uint32_t *opts_padding;
    Protocol_t *proto_base;
} Ipv4Proto_t;

//TODO: createWithParams().

/**
 * @memberof Ipv4Proto
 *
 * Class constructor. Creates an Ipv4Proto header and initializes all its
 * members with default values.
 *
 * @return A pointer to the newly allocated Ipv4Proto header.
 */
Ipv4Proto_t * Ipv4Proto_create(void);

/**
 * @memberof Ipv4Proto
 *
 * Class destructor. Frees all the resources associated to an Ipv4Proto header.
 *
 * @param proto Pointer to an Ipv4Proto instance to free its resources.
 */
void Ipv4Proto_delete(Ipv4Proto_t *proto);

/**
 * @memberof Ipv4Proto
 *
 * Implements the getSize() behavior (read Protocol for further details).
 *
 * @param proto Pointer to an Ipv4Proto instance to get the size from.
 * @return The size in bytes of this Ipv4Proto instance.
 */
unsigned int Ipv4Proto_getSize(const Ipv4Proto_t *proto);

/**
 * @memberof Ipv4Proto
 *
 * Implements the getBitstream() behavior (read Protocol for further details).
 *
 * @param proto Pointer to an Ipv4Proto instance to get its bitstream from.
 * @param buf Pointer to a buffer where the bitstream will be written into.
 * @param size The maximum size of the buffer.
 * @return The number of bytes written into buf.
 */
int Ipv4Proto_getBitstream(
        const Ipv4Proto_t *proto,
        uint8_t *buf,
        unsigned int size);

/**
 * @memberof Ipv4Proto
 *
 * Getter of the member proto_base.
 *
 * @param proto Pointer to the instance to get its proto_base.
 * @return A pointer to the member proto_base or NULL.
 */
Protocol_t * Ipv4Proto_getProtoBase(const Ipv4Proto_t *proto);

/**
 * @memberof Ipv4Proto
 *
 * Setter of the member length.
 *
 * The length in an IPv4 header represents the number of bytes of that specific
 * datagram, header + payload.
 *
 * Pay special attention when using this function as you can end up with an
 * inconsistent IPv4 header if you specify an incorrect length.
 *
 * @param proto Pointer to the instance to set its length.
 * @param length The value to set.
 * @return 0 on success, -1 otherwise.
 */
int Ipv4Proto_setLength(Ipv4Proto_t *proto, uint16_t length);

/**
 * @memberof Ipv4Proto
 *
 * Getter of the member length.
 *
 * @param proto Pointer to the instance to get its length from.
 * @return The value of the length member.
 */
uint16_t Ipv4Proto_getLength(const Ipv4Proto_t *proto);

/**
 * @memberof Ipv4Proto
 *
 * Setter of the member proto.
 *
 * The protocol field in an IPv4 header represents the upper protocol to which
 * the payload of the datagram must be passed, usually TCPv4 or UDPv4, but
 * could be others.
 *
 * @param proto Pointer to the instance to set its proto member.
 * @para proto_num The value to set.
 * @return 0 on success, -1 otherwise.
 */
//TODO: Create the list of proto_num or use it from one of the standard header
//files.
int Ipv4Proto_setProtocol(Ipv4Proto_t *proto, uint8_t proto_num);

/**
 * @memberof Ipv4Proto
 *
 * Getter of the member proto.
 *
 * @param proto Pointer to the instance to get its member proto from.
 * @return The value of the member proto.
 */
uint8_t Ipv4Proto_getProtocol(const Ipv4Proto_t *proto);

#endif

