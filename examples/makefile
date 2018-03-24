# This file is part of libpacket.
#
# libpacket is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libpacket is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with libpacket.  If not, see <http://www.gnu.org/licenses/>

CC := gcc
CFLAGS := -ggdb

LD := gcc
LDFLAGS := -Wl,-rpath=$(shell pwd)/../lib

SOURCES := p1.c
#TODO: I have no idea how to write this. HELP PLEASE!!!
BINS := $($(SOURCES)%.c=%)
INCLUDES := ../include
LIBSDIR := ../lib

#example: $(BINS)
#
#$(BINS): $(SOURCES)
#	$(CC) $(CFLAGS) -I$(INCLUDES) -L$(LIBSDIR) $(LDFLAGS) -o $@ $< -lpacket
#	sudo setcap "cap_net_raw,cap_net_admin+eip" $@
#
#clean:
#	rm -f $(BINS)

all: example

example:
	$(CC) $(CFLAGS) -I$(INCLUDES) -L$(LIBSDIR) $(LDFLAGS) -o p1 p1.c -lpacket

clean:
	rm -f p1
