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

/* The default value for pass-through is 'false' here. we do the updates and sending manually. */
FutabaSBUS sbus;

void setup() {
        sbus.begin(Serial);
}

void loop() {
        ChannelData channels;
        
        sbus.receive();
        channels = sbus.getChannels();
        
        /* Alter the digi channels for this example */
        channels.channels.channel17 = 1;
        channels.channels.channel18 = 0;
        
        /* Update the data */
        sbus.updateChannels(channels);
        
        /* Send the data */
        sbus.send();
}