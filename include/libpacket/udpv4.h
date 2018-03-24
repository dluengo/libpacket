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

#ifndef __LIBPACKET_UDPV4
#define __LIBPACKET_UDPV4

/**
 * @file ether.h
 * @author David "Ole" Luengo
 * @date 10 March 2018
 * @brief File implemening the UDPv4 protocol.
 */

#include <stdint.h>

#include "libpacket/packet.h"

#define UDPV4_HEADER_LEN (8)

/**
 * @class Udpv4Proto
 * @brief Class implementing the UDPv4 header.
 */
typedef struct Udpv4Proto Udpv4Proto_t;

typedef struct Udpv4Proto {
    uint16_t sport;
    uint16_t dport;
    uint16_t length;
    uint16_t checksum;
    Protocol_t *proto_base;
} Udpv4Proto_t;

/**
 * @memberof Udpv4Proto
 *
 * Class constructor. Creates a new instance of Udpv4Proto with members set at
 * the values passed by parameters.
 *
 * @param sport The source port.
 * @param dport The destination port.
 * @param length The length of the UDPv4 packet, that is, the length of the
 * header + length of they payload.
 * @param checksum The value of the checksum (usually disable in UDPv4).
 * @return A pointer to the newly allocated Udpv4Proto instance.
 */
Udpv4Proto_t * Udpv4Proto_createWithParams(
        uint16_t sport,
        uint16_t dport,
        uint16_t length,
        uint16_t checksum,
        Protocol_t *proto_base);

/**
 * @memberof Udpv4Proto
 *
 * Class constructor. Allocates a new Udpv4Proto instance with all its members
 * set to default values. Ports 0, length UDPV4_HEADER_LEN, checksum 0.
 *
 * @return A pointer to the newly allocated Udpv4Proto instance.
 */
Udpv4Proto_t * Udpv4Proto_create(void);

/**
 * @memberof Udpv4Proto
 *
 * Class destructor. Frees all the resources associated to this instance.
 *
 * @param proto Pointer to a Udpv4Proto instance to be freed.
 */
void Udpv4Proto_delete(Udpv4Proto_t *proto);

/**
 * @memberof Udpv4Proto
 *
 * Implements the getSize() behavior needed by the Protocol class.
 *
 * @param proto Pointer to the Udpv4Proto instance to get its size.
 * @return The length of this instance in bytes in the wire.
 */
unsigned int Udpv4Proto_getSize(const Udpv4Proto_t *proto);

/**
 * @memberof Udpv4Proto
 *
 * Implements the getBitstream() behavior needed by the Protocol class.
 *
 * @param proto Pointer to the Udpv4Proto instance to get its bitstream.
 * @param buf The buffer where to write the bitstream that represents this
 * instance in the wire.
 * @param size The maximum size of the buffer.
 * @return The number of bytes written into the buffer.
 */
int Udpv4Proto_getBitstream(
        Udpv4Proto_t *proto,
        uint8_t *buf,
        unsigned int size);

/**
 * @memberof Udpv4Proto
 *
 * Getter of the member proto_base.
 *
 * @param proto Pointer to the Udpv4Proto instance.
 * @return A pointer to the Protocol instance associated to this Udpv4Proto.
 */
Protocol_t * Udpv4Proto_getProtoBase(const Udpv4Proto_t *proto);

#endif
