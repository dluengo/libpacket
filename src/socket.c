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
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

#include "libpacket/socket.h"
#include "libpacket/packet.h"

Socket_t * Socket_create(const char *ifname) {
    Socket_t *sock = NULL;
    int desc, err, ok = 0;
    struct ifreq ifindex;
    struct sockaddr *addr;

    if (ifname == NULL) {
        printf("%s: ifname is NULL\n", __FUNCTION__);
        goto end;
    }

    sock = malloc(sizeof(Socket_t));
    if (sock == NULL) {
        perror("malloc()\n");
        goto end;
    }

    desc = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (desc == -1) {
        perror("socket()");
        goto end;
    }

    strncpy(ifindex.ifr_name, ifname, IFNAMSIZ-1);
    err = ioctl(desc, SIOCGIFINDEX, &ifindex);
    if (err) {
        perror("ioctl()");
        goto end;
    }

    sock->desc = desc;
    memset(&sock->addr, 0, sizeof(sock->addr));
    sock->addr.sll_family = AF_PACKET;
    sock->addr.sll_ifindex = ifindex.ifr_ifindex;
    err = bind(desc,
            (const struct sockaddr *)(&sock->addr),
            sizeof(sock->addr));
    if (err) {
        perror("bind()");
        goto end;
    }

    ok = 1;

end:
    if (!ok && sock != NULL) {
        free(sock);
        sock = NULL;
    }

    return sock;
}

void Socket_delete(Socket_t *sock) {
    if (sock != NULL) {
        close(sock->desc);
    }

    free(sock);
}

#define WRITE_RETRIES (3)

int Socket_inject(const Socket_t *sock, const Packet_t *pack) {
    int ret = -1, tries, sent;
    uint8_t *bitstream = NULL;
    unsigned int bs_length;

    if (sock == NULL || pack == NULL) {
        goto end;
    }

    bs_length = Packet_getSize(pack);
    if (bs_length == 0) {
        goto end;
    }

    bitstream = malloc(sizeof(uint8_t) * bs_length);
    if (bitstream == NULL) {
        goto end;
    }

    Packet_getBitstream(pack, bitstream, bs_length);

    tries = sent = 0;
    do {
        sent = sendto(sock->desc,
                bitstream + sent,
                bs_length - sent,
                0,
                (struct sockaddr *)(&sock->addr),
                sizeof(sock->addr));
    } while (sent < bs_length && tries++ < WRITE_RETRIES);

    if (tries >= WRITE_RETRIES) {
        ret = -1;
        goto end;
    }

end:
    free(bitstream);
    return ret;
}

