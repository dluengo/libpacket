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

OBJDIR := objs
LIBDIR := ../lib

OBJLIST := stack.o \
           ether.o \
           ipv4.o \
		   udpv4.o \
           packet.o \
           socket.o

OBJS := $(addprefix $(OBJDIR)/,$(OBJLIST))
LIBS := $(addprefix $(LIBDIR)/,libpacket.so)

INCLUDE := ../include

CC := gcc
LD := gcc
CFLAGS := -ggdb -fPIC -I$(INCLUDE)
LDFLAGS := -ggdb -shared

all: libpacket

libpacket: $(LIBS)

$(LIBS): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS): | $(OBJDIR) $(LIBDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(LIBDIR):
	mkdir $(LIBDIR)

clean:
	rm -fr *.o *.so objs
