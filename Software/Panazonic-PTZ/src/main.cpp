//  ||=======================================================||
//  ||                Frontrow - Panasonic PTZ               ||
//  ||                       Rev 0.1                         ||
//  ||             Made By: Andreas Hauge Thomsen            ||
//  ||=======================================================||
//  ||         To Provide control for Panasonic PTZ          ||
//  ||      via an DJI Force Pro or a S-Bus Controller       ||
//  ||=======================================================||



// Debug Mode:
#define debug

// Include Libraies:
#include "Arduino.h"				// Default Arduino 
#include "FutabaSBUS.h"			// Provides S-Bus Capability
#include "boards.h"					// Defines Board Specific Peramators
#include "AW-UE150-Codes.h"	// Provides RS422 Serial Packages 


// ||=======================================================||
// ||                    S-Bus Functions                    ||
// ||=======================================================||

// Setup S-Bus:
FutabaSBUS sbus;

// Delivers Channel Data For Use:
void dataReceived(ChannelData channels) {
  // do something with the data
	#ifdef debug
		Serial.print("CH1: ");
		Serial.print(channels.channels.channel1);
		Serial.print(" CH2: ");
		Serial.print(channels.channels.channel2);
		Serial.print(" CH3: ");
		Serial.print(channels.channels.channel3);
		Serial.print(" CH4: ");
		Serial.println(channels.channels.channel4);
	#endif
}

// Notify When a Failsafe Orcures:
void failsafe() {
	#ifdef debug
		Serial.println("Failsafe");
	#endif
}

// Notify When a Frame Error Orcures:
void frameError() {
	#ifdef debug
	 	Serial.println("Frame Error");
	#endif
}




// ||=======================================================||
// ||                      Main Setup                       ||
// ||=======================================================||

void setup() {
	Serial.begin(115200);	// Start Debug Serial Port (USB)

	RS422.begin(9600);	// Start RS422 Serial Port

	sbus.begin(sbusPort, false);						// Start S-Bus Serial Port
	sbus.attachDataReceived(dataReceived);	// Attach Channel Data
	sbus.attachFailSafe(failsafe);					// Attach Failsafe Flag
	sbus.attachFrameError(frameError);			// Attach Frame Error Flag

}



// ||=======================================================||
// ||                       Main Loop                       ||
// ||=======================================================||

void loop() {
	sbus.receive();  // Update S-Bus, need to call receive() regularly. Can be done by a timer interrupt too.
}

