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
// #define debugAxis	// Debug Axis Direction
#define debugChange		// Debug Change in Maped Values

// Include Libraies:
#include "Arduino.h"			// Default Arduino 
#include "FutabaSBUS.h"			// Provides S-Bus Capability
#include <TaskScheduler.h>		// Adds Task Scheduler Support
#include "boards.h"				// Defines Board Specific Peramators
#include "AW-UE150-Codes.h"	    // Provides RS422 Serial and HTTP IP Packages 



// ||=======================================================||
// ||                    Global Varibles                    ||
// ||=======================================================||

// Number of sbus channels
int sBusCh[16];

// SBUS Status states
bool failsafeState = true;
bool sBusError = true;

// Define analog calibrations varibles
int analogCenter = 1010;
int analogMax = 1800;
int analogMin = 200;
int analogDeadzone = 50;
int rangeMap = 50;

// Define axis
int pan = 0; 			// Channel 1
int tilt = 0;			// Channel 2
int zoom = 0;			// Channel 3
int focus = 0;			// Channel 4

// Store Mapped Axis Values
int panMap = 0; 		// Channel 1
int tiltMap = 0;		// Channel 2
int zoomMap = 0;		// Channel 3
int focusMap = 0;		// Channel 4

// Store Last Axis Direction
int panMapOld = 0; 		// Channel 1
int tiltMapOld = 0;		// Channel 2
int zoomMapOld = 0;		// Channel 3
int focusMapOld = 0;	// Channel 4

// Store Axis Direction
String panDir = "Stop";
String tiltDir = "Stop";
String zoomDir = "Stop";
String focusDir = "Stop";

// Callback methods prototypes for timers
void t1Callback();
void t2Callback();

// Create Tasks
Task t1(10, TASK_FOREVER, &t1Callback);
Task t2(100, TASK_FOREVER, &t2Callback);
Scheduler runner;



// ||=======================================================||
// ||                       Functions                       ||
// ||=======================================================||

// Cap max and min analog value acording to settings
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

// Get the axis direction
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

// Map analog axis to match speed settings on PTZ
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

// Detect if analog axis has moved since last message was send
bool detectChange() {
	bool changed = false;

	if (panMapOld != panMap) { changed = true; }
	if (tiltMapOld != tiltMap) { changed = true; }
	if (zoomMapOld != zoomMap) { changed = true; }
	if (focusMapOld != focusMap) { changed = true; }

	#ifdef debugChange
	if (changed == true) {
		Serial.print("Update Detected: | ");
		Serial.print(" Pan: " + panDir + " " + panMap + " " + panMapOld + " |");
		Serial.print(" Tilt: " + tiltDir + " " + tiltMap + " " + tiltMapOld + " |");
		Serial.print(" Zoom: " + zoomDir + " " + zoomMap + " " + zoomMapOld + " |");
		Serial.println(" Focus: " + focusDir + " " + focusMap + " " + focusMapOld + " |");
	}
	#endif

	panMapOld = panMap;
	tiltMapOld = tiltMap;
	zoomMapOld = zoomMap;
	focusMapOld = focusMap;

	if (changed == false) {	return false; }	
	else if (changed == true) {	return true; }
	return false;	
}



// ||=======================================================||
// ||                    S-Bus Functions                    ||
// ||=======================================================||

// Setup S-Bus:
FutabaSBUS sbus;

// Delivers Channel Data For Use:
void dataReceived(ChannelData data) {
	// Set sBUS state
	failsafeState = false;
	sBusError = false;

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
}

// Notify When a Failsafe Orcures:
void failsafe() {
	failsafeState = true;
	#ifdef debug
		Serial.println("Failsafe");
	#endif
}

// Notify When a Frame Error Orcures:
void frameError() {
	sBusError = true;
	#ifdef debug
	 	Serial.println("Frame Error");
	#endif
}



// ||=======================================================||
// ||                      Main Tasks                       ||
// ||=======================================================||

// Update/Recive Sbus Data
void t1Callback() {
	sbus.receive();  // Update S-Bus, need to call receive() regularly. Can be done by a timer interrupt too.
	getAnalog();
	findCommand();
}

// Send Camera Data
void t2Callback() {

  	if (detectChange() == true) {
		actions();
	}
	else {
		/* Code */
	}
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
	
	// Start Scheduler
	runner.init();
	Serial.println("Initialized scheduler");
	
	// Add Tasks
	runner.addTask(t1);
	Serial.println("added t1");	
	runner.addTask(t2);
	Serial.println("added t2");

	delay(5000);

	// Enable Tasks	
	t1.enable();
	Serial.println("Enabled t1");
	t2.enable();
	Serial.println("Enabled t2");

	Serial.println("Ready");
}



// ||=======================================================||
// ||                       Main Loop                       ||
// ||=======================================================||

void loop() {
	runner.execute(); // Update Runner
}

