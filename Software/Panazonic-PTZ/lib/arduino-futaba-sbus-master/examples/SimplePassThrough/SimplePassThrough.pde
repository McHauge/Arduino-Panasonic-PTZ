/*
    FutabaSBUS - Futaba S-BUS protocol library for Arduino

    Copyright (C) 2017 Szabolcs Gyurko

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License, but not any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <FutabaSBUS.h>


FutabaSBUS sbus;

void setup() {
	sbus.begin(Serial, true);
}

void loop() {
        // You need to call receive() regularly. Can be done by a timer interrupt too.
        sbus.receive();
}
