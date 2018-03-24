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

#ifndef __LIBPACKET_PACKET
#define __LIBPACKET_PACKET

/**
 * @file packet.h
 * @author David "Ole" Luengo
 * @date 10 March 2018
 * @brief File implementing the Packet class, the cornerstone of libpacket.
 */

#include <stdint.h>

#include "libpacket/stack.h"

/*-------------------------------- Protocol ---------------------------------*/

/**
 * @class Protocol
 * @brief Generic abstraction of any kind of network protocol.
 *
 * The class Protocol is a generic interface to implement real network
 * protocols (i.e. IPv4, Ethernet, DNS...).
 *
 * Ideally other developers will develop their own protocols for libpacket.
 * Here is how you do it:
 *   - Declaring one of the extending class members of type Protocol_t *.
 *   - Implementing a getSize() method.
 *   - Implementing a getBitstream() method.
 */
typedef struct Protocol Protocol_t;

/**
 * @typedef unsigned int (*Protocol_getSizeFunc_t)(Protocol_t *)
 *
 * This is the signature of the methods implementing the required getSize()
 * behavior.
 *
 * Classes extending Protocol need to implement a method that receives a single
 * pointer to its instance (self/this) and returns an unsigned integer that
 * indicates the number of bytes that Protocol occupies in the wire.
 */
typedef unsigned int (*Protocol_getSizeFunc_t)(Protocol_t *);

/**
 * @typedef int (*Protocol_getBitstreamFunc_t)(Protocol_t *, uint8_t *, unsigned int)
 *
 * This is the signature of the methods implementing the required
 * getBitstream() behavior.
 *
 * Classes extending Protocol need to implement a method that receives a
 * pointer to its instance (self/this), a pointer to a uint8_t buffer and an
 * unsigned integer indicating the size of the buffer. This method writes into
 * the buffer the wire representation of the Protocol instance up to its
 * maximum size. Returns the number of bytes written into the buffer, that
 * should be the same as the value returned by getSize() or the maximum size
 * of the buffer.
 */
//TODO: This method must return an unsigned int and its implementation in all
//the protocols must change too.
typedef int (*Protocol_getBitstreamFunc_t)(
        Protocol_t *,
        uint8_t *,
        unsigned int);

typedef struct Protocol {
    Protocol_getSizeFunc_t getSize;
    Protocol_getBitstreamFunc_t getBitstream;
    StackItem_t *item;
    void *owner;
} Protocol_t;

/**
 * @memberof Protocol
 *
 * Class constructor with parameters. Allocates and initializes a new instance
 * of Protocol.
 * 
 * @param getSize Pointer to the getSize() function of the implementing
 * protocol.
 * @param getBitstream Pointer to the getBitstream() function of the
 * implementing Protocol.
 * @param item The StackItem instance hold by this Protocol.
 * @param owner A pointer to an instance of an extending class.
 * @return A pointer to the newly allocated Protocol or NULL.
 */
Protocol_t * Protocol_createWithParams(
        Protocol_getSizeFunc_t getSize,
        Protocol_getBitstreamFunc_t getBitstream,
        StackItem_t *item,
        void *owner);

/**
 * @memberof Protocol
 * 
 * Class constructor. Allocates and initializes a new Protocol instance with
 * its member with default values.
 *
 * @return A pointer to the newly allocated Protocol or NULL.
 */
Protocol_t * Protocol_create(void);

/**
 * @memberof Protocol
 *
 * Class destructor. Frees all the resources associated to a Protocol instance.
 * 
 * @param proto A pointer to the instance of Protocol to be freed.
 */
void Protocol_delete(Protocol_t *proto);

/**
 * @memberof Protocol
 *
 * Setter of the member item.
 * 
 * @param proto Pointer to the Protocol instance to set its StackItem member.
 * @param item Pointer to the StackItem instance to be set.
 * @return 0 on success, -1 otherwise.
 */
int Protocol_setItem(Protocol_t *proto, StackItem_t *item);

/**
 * @memberof Protocol
 *
 * Getter of the member item.
 * 
 * @param proto Pointer to the Protocol instance from where to get the
 * StackItem member.
 * @return A pointer to the StackItem member or NULL;
 */
StackItem_t * Protocol_getItem(const Protocol_t *proto);

/**
 * @memberof Protocol
 *
 * Setter of the member owner.
 * 
 * @param proto Pointer to the Protocol instance to set its owner member.
 * @param owner Pointer to an instance of an extending class that owns this
 * Protocol.
 * @return 0 on success, -1 otherwise.
 */
int Protocol_setOwner(Protocol_t *proto, void *owner);

/**
 * @memberof Protocol
 *
 * Getter of the member owner.
 * 
 * @param proto Pointer to the Protocol instance from where to get the owner
 * member.
 * @return A pointer to an instance of an extending class that owns this
 * Protocol.
 */
void * Protocol_getOwner(const Protocol_t *proto);

/**
 * @memberof Protocol
 *
 * Setter of the member getSize.
 * 
 * @param proto Pointer to the Protocol instance to set its getSize member.
 * @param func Pointer to the method of an extending class that implements
 * the getSize() behavior.
 * @return 0 if sucess, -1 otherwise.
 */
int Protocol_setGetSize(Protocol_t *proto, Protocol_getSizeFunc_t func);

/**
 * @memberof Protocol
 *
 * Getter of the member getSize.
 * 
 * @param proto Pointer to the Protocol instance from where to get the getSize
 * member.
 * @return A pointer to the getSize method of this Protocol.
 */
Protocol_getSizeFunc_t Protocol_getGetSize(const Protocol_t *proto);

/**
 * @memberof Protocol
 *
 * Setter of the member getBitstream.
 * 
 * @param proto Pointer to the Protocol instance to set its getBitstream
 * member.
 * @param func Pointer to the method of an extending class that implements the
 * getBitstream() behavior.
 * @return 0 if sucess, -1 otherwise.
 */
int Protocol_setGetBitstream(
        Protocol_t *proto,
        Protocol_getBitstreamFunc_t func);

/**
 * @memberof Protocol
 *
 * Getter of the member getBitstream.
 * 
 * @param Pointer to the Protocol instance from where to get the getBitstream
 * member.
 * @return A pointer to the getBitstream method of this Protocol.
 */
Protocol_getBitstreamFunc_t Protocol_getGetBitstream(const Protocol_t *proto);

/*--------------------------------- Packet ----------------------------------*/

/**
 * @class Packet
 * @brief Class that abstract a bunch of network protocols stacked together.
 *
 * A Packet is a extension of a Stack and is aimed to hold multiple intance of
 * protocols one on top of each other. To give an example, a common DNS query
 * would be Ethernet - IPv4 - UDPv4 - DNS, with Ethernet being the bottom of
 * the Packet/Stack and DNS at the top.
 */
typedef struct Packet Packet_t;

typedef struct Packet {
    Stack_t *stack;
} Packet_t;

/**
 * @memberof Packet
 *
 * Class constructor with parameters.
 *
 * @param stack Pointer to an instance of Stack that will be the internal stack
 * maintained by the Packet.
 * @return A pointer to the newly allocated Packet or NULL.
 */
Packet_t * Packet_createWithParams(Stack_t *stack);

/**
 * @memberof Packet
 *
 * Class constructor. Allocates a new Packet with an empty Stack.
 *
 * @return A pointer to the newly allocated Packet or NULL.
 */
Packet_t * Packet_create(void);

/**
 * @memberof Packet
 *
 * Class destructor. Frees all the resources associated to a Packet.
 *
 * @param pack Pointer to the Packet instance to be destroyed.
 */
void Packet_delete(Packet_t *pack);

/**
 * @memberof Packet
 *
 * Stacks a Protocol on top of the Packet.
 *
 * @param pack Pointer to the instance of Packet that will stack the protocol.
 * @param layer Pointer to an instance of a Protocol that will be stacked on
 * this Packet.
 * @return 0 on success, -1 otherwise.
 */
int Packet_stack(Packet_t *pack, Protocol_t *layer);

/**
 * @memberof Packet
 *
 * Returns the size in bytes of the Packet.
 *
 * @param pack Pointer to the instance of Packet to want its size.
 * @return The size in bytes of the Packet.
 */
unsigned int Packet_getSize(const Packet_t *pack);

/**
 * @memberof Packet
 *
 * This method writes into buf the wire representation of the Packet up to
 * size bytes.
 *
 * @param pack Pointer to the Packet instance.
 * @param buf The buffer where to put the bytes.
 * @param size The length of buffer.
 * @return The number of bytes written into the buffer.
 */
int Packet_getBitstream(const Packet_t *pack, uint8_t *buf, unsigned int size);

#endif

