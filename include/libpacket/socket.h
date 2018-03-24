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

#ifndef __LIBPACKET_SOCKET
#define __LIBPACKET_SOCKET

/**
 * @file socket.h
 * @author David "Ole" Luengo
 * @date 10 March 2018
 * @brief File implementing an abstraction of a socket where we can inject any
 * kind of traffic.
 */

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#include "libpacket/packet.h"

/**
 * @class Socket "libpacket/socket.h"
 * @brief Class implementing a socket where we can inject any kind of traffic.
 *
 * The Socket class underneath works using the regular POSIX socket() call and
 * its family functions. It encapsulates all the ugly logic needed to open a
 * socket and write in it. Take into account that we are using PACKET_RAW
 * sockets in order to inject traffic. Because of that, sometimes it could be
 * kind of hard to properly build a program that works due to the many security
 * measures around this type of sockets. The simplest way to run a program that
 * uses these sockets is by building as root or running with sudo, however this
 * is not desiderable from the security point of view and is not mandatory. In
 * Linux, capabilities can be used, more specifically CAP_NET_RAW and
 * CAP_NET_ADMIN (you can find an example in examples/makefile). The problem
 * with this approach is that if libpacket is dynamically linked, when running
 * the program some systems strip LD_LIBRARY_PATH, leading us into a library-
 * cannot-be-found situation. I included a workaround for this while I was
 * developing libpacket (you can see it in examples/makefile), linking with the
 * option -Wl,-rpath so the resulting programs contain a path in the system
 * where to look for libraries. It's a dirty trick, but at least it lets me be
 * up and running with libpacket fast.
 */
typedef struct Socket Socket_t;

typedef struct Socket {
    int desc;
    struct sockaddr_ll addr;
} Socket_t;

/**
 * @memberof Socket
 * 
 * Class constructor. Creates a new socket and prepares it to be able to
 * inject traffic using it.
 *
 * @param ifname A string with the name of the interface to be used to inject
 * traffic into.
 * @return A pointer to the newly allocated socket or NULL.
 */
Socket_t * Socket_create(const char *ifname);

/**
 * @memberof Socket
 * 
 * Class destructor. Frees all the resources allocated for a socket.
 *
 * @param sock A pointer to the socket to be freed.
 */
void Socket_delete(Socket_t *sock);

/**
 * @memberof Socket
 * 
 * Injects a single packet into a socket. Underneath sendto() is used and, due
 * to its nature, it could happen that not all of the bytes of a packet are
 * written. Internally if this happens, we try again a couple of times but if
 * after some retries we haven't still injected everything we give up and
 * return the number of bytes written so far.
 *
 * @param sock A pointer to the socket where we want to inject the packet.
 * @param pack A pointer to a packet that contains the information to be
 * injected in the network.
 * @return The number of bytes written into the network.
 */
int Socket_inject(const Socket_t *sock, const Packet_t *pack);

#endif
