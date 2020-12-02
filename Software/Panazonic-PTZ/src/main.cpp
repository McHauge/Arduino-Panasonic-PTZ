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
// #define debugRaw 	// Debug Raw sBus values
// #define debugAnalog	// Degug Calculated Analog Value
#define debugAxis		// Debug Axis Direction

// Include Libraies:
#include "Arduino.h"			// Default Arduino 
#include "FutabaSBUS.h"			// Provides S-Bus Capability
#include "boards.h"				// Defines Board Specific Peramators
#include "AW-UE150-Codes.h"	    // Provides RS422 Serial and HTTP IP Packages 



// ||=======================================================||
// ||                    Global Varibles                    ||
// ||=======================================================||

// Number of sbus channels
int sBusCh[16];

// Define analog calibrations varibles
int analogCenter = 1010;
int analogMax = 1800;
int analogMin = 200;
int analogDeadzone = 50;
int rangeMap = 50;

// Define axis
int pan = 0; 		// Channel 1
int tilt = 0;		// Channel 2
int zoom = 0;		// Channel 3
int focus = 0;		// Channel 4

int panMap = 0; 	// Channel 1
int tiltMap = 0;	// Channel 2
int zoomMap = 0;	// Channel 3
int focusMap = 0;	// Channel 4

// Store Axis Direction
String panDir = "Stop";
String tiltDir = "Stop";
String zoomDir = "Stop";
String focusDir = "Stop";



// ||=======================================================||
// ||                       Functions                       ||
// ||=======================================================||

int setMaxMin(int axis) {
	if (axis <= (analogCenter-analogMin)*(-1)) { return analogCenter-analogMin*(-1); }
	else if (axis >=analogMax-analogCenter) { return analogMax-analogCenter; }
	return axis;
}

// Gets a + or - value depending on the analog signal
void getAnalog() {

	if (sBusCh[0] <= analogCenter-analogDeadzone || sBusCh[0] >= analogCenter+analogDeadzone) { pan = sBusCh[0]-analogCenter; }
	else { pan = 0; }
	if (sBusCh[1] <= analogCenter-analogDeadzone || sBusCh[1] >= analogCenter+analogDeadzone) { tilt = sBusCh[1]-analogCenter; }
	else { tilt = 0; }
	if (sBusCh[2] <= analogCenter-analogDeadzone || sBusCh[2] >= analogCenter+analogDeadzone) { zoom = sBusCh[2]-analogCenter; }
	else { zoom = 0; }
	if (sBusCh[3] <= analogCenter-analogDeadzone || sBusCh[3] >= analogCenter+analogDeadzone) { focus = sBusCh[3]-analogCenter; }
	else { focus = 0; }

	pan = setMaxMin(pan);
	tilt = setMaxMin(tilt);
	zoom = setMaxMin(zoom);
	focus = setMaxMin(focus);

    // Print result in debug mode
	#ifdef debugAnalog
		Serial.print("Pan: ");
		Serial.print(pan);
		Serial.print(" Tilt: ");
		Serial.print(tilt);
		Serial.print(" Zoom: ");
		Serial.print(zoom);
		Serial.print(" Focus: ");
		Serial.println(focus);
	#endif

}

String getDirection(int axis, String negative, String positive) {
	if (axis < 0) {	
		return negative; 
	}
	else if (axis == 0) { 
		return "Stop"; 
	}
	else if (axis > 0) { 
		return positive; 
	}	
	return "0";
}

int getMap(int axis, String negative, String positive) {
	if (axis < 0) {	
		return map(axis, (analogCenter-analogMin)*(-1), 0, 1, 49);
	}
	else if (axis == 0) { 
		return 50;
	}
	else if (axis > 0) { 
		return map(axis, 0, analogMax-analogCenter, 51, 99);
	}	
	return 50;

}

// ||=======================================================||
// ||                Comunication Functions                 ||
// ||=======================================================||

void findCommand() {

	panDir = getDirection(pan, "Left", "Right");
	tiltDir = getDirection(tilt, "Down", "Up");
	zoomDir = getDirection(zoom, "Out", "In");
	focusDir = getDirection(focus, "Far", "Near");

	panMap = getMap(pan, "Left", "Right");
	tiltMap = getMap(tilt, "Down", "Up");
	zoomMap = getMap(zoom, "Out", "In");
	focusMap = getMap(focus, "Far", "Near");

    // Print result in debug mode
	#ifdef debugAxis
		Serial.print("Pan: " + panDir + " " + panMap);
		Serial.print(" Tilt: " + tiltDir + " " + tiltMap);
		Serial.print(" Zoom: " + zoomDir + " " + zoomMap);
		Serial.println(" Focus: " + focusDir + " " + focusMap);
	#endif
}



// ||=======================================================||
// ||                    S-Bus Functions                    ||
// ||=======================================================||

// Setup S-Bus:
FutabaSBUS sbus;

// Delivers Channel Data For Use:
void dataReceived(ChannelData data) {
    // Save channels to a short varible    
    sBusCh[0] = data.channels.channel1;
    sBusCh[1] = data.channels.channel2;
    sBusCh[2] = data.channels.channel3;
    sBusCh[3] = data.channels.channel4;
    sBusCh[4] = data.channels.channel5;
    sBusCh[5] = data.channels.channel6;
    sBusCh[6] = data.channels.channel7;
    sBusCh[7] = data.channels.channel8;
    sBusCh[8] = data.channels.channel9;
    sBusCh[9] = data.channels.channel10;
    sBusCh[10] = data.channels.channel11;
    sBusCh[11] = data.channels.channel12;
    sBusCh[12] = data.channels.channel13;
    sBusCh[13] = data.channels.channel14;
    sBusCh[14] = data.channels.channel15;
    sBusCh[15] = data.channels.channel16;

    // Print result in debug mode
	#ifdef debugRaw
		Serial.print("CH1: ");
		Serial.print(sBusCh[0]);
		Serial.print(" CH2: ");
		Serial.print(sBusCh[1]);
		Serial.print(" CH3: ");
		Serial.print(sBusCh[2]);
		Serial.print(" CH4: ");
		Serial.println(sBusCh[3]);
	#endif

	getAnalog();
	findCommand();
	actions();
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
	RS422.begin(9600);	    // Start RS422 Serial Port

	sbus.begin(sbusPort, false);				// Start S-Bus Serial Port
	sbus.attachDataReceived(dataReceived);	    // Attach Channel Data
	sbus.attachFailSafe(failsafe);				// Attach Failsafe Flag
	sbus.attachFrameError(frameError);			// Attach Frame Error Flag

	Serial.println("Ready");
}



// ||=======================================================||
// ||                       Main Loop                       ||
// ||=======================================================||

void loop() {
	sbus.receive();  // Update S-Bus, need to call receive() regularly. Can be done by a timer interrupt too.
}

