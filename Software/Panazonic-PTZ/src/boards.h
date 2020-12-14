//  ||=======================================================||
//  ||                 Frontrow - Panasonic PTZ              ||
//  ||                        Rev 0.1                        ||
//  ||              Made By: Andreas Hauge Thomsen           ||
//  ||=======================================================||
//  ||               Borad Diffinition and Pinout            ||
//  ||=======================================================||

//
// Arduino Mega
//
#ifdef MEGA2560
	#define DEBUG Serial 		// Use USB Serial for DEBUG
	#define RS422 Serial1 		// Use Serial 1 for RS422 (pin: 18 & 19)
	#define sbusPort Serial2  	// Use Serial 2 for S-Bus (pin: 16 & 17)
#endif

//
// Arduino DUE
//
#ifdef DUE
	#define DEBUG Serial 		// Use USB Serial for DEBUG
	#define RS422 Serial1 		// Use Serial 1 for RS422 (pin: 18 & 19)
	#define sbusPort Serial2  	// Use Serial 2 for S-Bus (pin: 16 & 17)
#endif

//
// Teensy v3.6
//
#ifdef TEENSY36
	#define DEBUG Serial 		// Use USB Serial for DEBUG
	#define RS422 Serial6 		// Use Serial 6 for RS422 (pin: 47 & 48)
	#define sbusPort Serial5  	// Use Serial 5 for S-Bus (pin: 34 & 33)
#endif

//
// M5Stack Core ESP32
//
#ifdef M5CORE
	#define DEBUG Serial 		// Use USB Serial for DEBUG
	#define RS422 Serial1 		// Use Serial 6 for RS422 (pin: 47 & 48)
	#define sbusPort Serial2  	// Use Serial 5 for S-Bus (pin: 34 & 33)
#endif
