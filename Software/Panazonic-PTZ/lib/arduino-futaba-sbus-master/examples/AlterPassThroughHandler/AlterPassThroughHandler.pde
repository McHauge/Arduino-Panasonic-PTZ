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

/* In this example we say 'true' for the pass-through and we register a data handler.
   The AlterPassThrough example uses 'false' for pass-through and handles the sending manually.
   Take a look at that example too. This is a more elegant way perhaps.
*/
FutabaSBUS sbus;

/* In this example we use the pass-through handler to alter the data before it leaves */
ChannelData handler(ChannelData data) {
        /* Alter the digi channels for this example */
        data.channels.channel17 = 1;
        data.channels.channel18 = 0;
        
        return data;
}

void setup() {
	sbus.begin(Serial, true);
        sbus.attachPassThroughHandler(handler);
}

void loop() {
        sbus.receive();
}
