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

#include <stdio.h>

#include "libpacket/socket.h"
#include "libpacket/packet.h"
#include "libpacket/ether.h"
#include "libpacket/ipv4.h"
#include "libpacket/udpv4.h"

int main() {
    Socket_t *sock;
    Packet_t *pack;
    EtherProto_t *ether;
    Ipv4Proto_t *ipv4;
    Udpv4Proto_t *udpv4;
    int res;

    sock = Socket_create("lo");
    if (sock == NULL) {
        perror("Socket_create()");
        return -1;
    }

    pack = Packet_create();
    if (pack == NULL) {
        perror("Packet_create()");
        return -1;
    }

    ether = EtherProto_create();
    if (ether == NULL) {
        perror("EtherProto_create()");
        return -1;
    }

    ipv4 = Ipv4Proto_create();
    if (ipv4 == NULL) {
        perror("Ipv4Proto_create()");
        return -1;
    }

    udpv4 = Udpv4Proto_create();
    if (udpv4 == NULL) {
        perror("Udpv4_create()");
        return -1;
    }

    Packet_stack(pack, EtherProto_getProtoBase(ether));
    Packet_stack(pack, Ipv4Proto_getProtoBase(ipv4));
    Packet_stack(pack, Udpv4Proto_getProtoBase(udpv4));

    Ipv4Proto_setProtocol(ipv4, 17);
    Ipv4Proto_setLength(ipv4, 28);

    printf("Packet size is %d\n", Packet_getSize(pack));
    res = Socket_inject(sock, pack);
    return 0;
}

