//  ||=======================================================||
//  ||                 Frontrow - Panasonic PTZ              ||
//  ||                        Rev 0.1                        ||
//  ||              Made By: Andreas Hauge Thomsen           ||
//  ||=======================================================||
//  ||                 Main Program & Setup                  ||
//  ||=======================================================||

/** 
 * @Author: McHauge 
 * @Date: 2020-04-29 05:03:25 
 * @Desc: Main Goal: Provide a way to directly control of a Panasonic PTZ Head with an Arduino and an X+Y encoder
 */

// Debug Mode:
#define debug

// Other Defines:
#define ENCODER_USE_INTERRUPTS // Force the use off interupts
#define ENCODER_OPTIMIZE_INTERRUPTS // Optimize interupts when using dual interupt pins

// Include Libraies:
#include "Arduino.h"
#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
//   avoid using pins with LEDs attached
//	 https://www.pjrc.com/teensy/td_libs_Encoder.html?utm_source=platformio&utm_medium=piohome

// Create Encoders:
Encoder encoderHori(5, 6);
Encoder encoderVert(7, 8);



// ||=======================================================||
// ||                      Main Setup                       ||
// ||=======================================================||

void setup() {
  Serial.begin(9600);
  Serial.println("TwoKnobs Encoder Test:");

}

// Define Encoder Start Pos:
long posEncoderHori  = -999;
long posEncoderVert = -999;

// ||=======================================================||
// ||                       Main Loop                       ||
// ||=======================================================||

void loop() {

  long newEncoderHori, newEncoderVert;
  newEncoderHori = encoderHori.read();
  newEncoderVert = encoderVert.read();
  
	if (newEncoderHori != posEncoderHori || newEncoderVert != posEncoderVert) {
    Serial.print("Left = ");
    Serial.print(newEncoderHori);
    Serial.print(", Right = ");
    Serial.print(newEncoderVert);
    Serial.println();
    posEncoderHori = newEncoderHori;
    posEncoderVert = newEncoderVert;
  }
  
	// if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    encoderHori.write(0);
    encoderVert.write(0);
  }
}
