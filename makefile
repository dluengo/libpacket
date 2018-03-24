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

MAKE := make

all: libpacket example doc

libpacket:
	$(MAKE) -C src/ libpacket

example:
	$(MAKE) -C examples/ example

doc:
	$(MAKE) -C doc/ doc

clean:
	$(MAKE) -C src/ clean
	$(MAKE) -C examples/ clean
	rm -rf libs/

