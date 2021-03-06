/*
 Example sketch for LEDManager library
 
 Reads a color code in hex from the serial port and displays that color
 
 For common anode implementations, attach the common lead to +Vcc and attach each RGB lead to the color pin through a current limiting resistor (current sink)
 For common cathode implementations, attach the common lead to ground, and attach each RGB lead to the color pin through a current limiting resistor (current source)
 
 Copyright 2017 Rob Redford
   This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
*/

#include <LEDManager.h>

// RGB LED
#define RED_PIN   14			// pin assignments for ESP-12E. Change to match your confiruation
#define GREEN_PIN	12			// make sure there is a current limiting resistor attached to each color lead
#define BLUE_PIN	13


RGBLED		led(RED_PIN, GREEN_PIN, BLUE_PIN);
//RGBLED		led(RED_PIN, GREEN_PIN, BLUE_PIN, LEDType::ANODE);						// common anode constructor


void setup ( void ) {
   Serial.begin(115200);
   while (!Serial);
   if (!led.begin()) {
      Serial.println(F("ERROR: cannot create led object. <<<"));
      while (true) delay(500);
   }
	Serial.println("\n\nInput complete 6-digit hexadecimal color values (RRGGBB)\nYou must eneter all 6 digits\n");
}

void loop ( void ) {
   uint32_t colorValue = 0;
   int 		bytePosition = 5;
   
   Serial.print("Input Color: ");
   while ( bytePosition >= 0 ) {
      byte inChar;
      
      // ignore non-conforming inputs
      if ( Serial.available() ) {
         byte inChar = Serial.read();
         if ( isDigit(inChar) ) {
            colorValue |= ((inChar - '0') << (bytePosition-- * 4));
         } else if ( isUpperCase(inChar) ) {
            colorValue |= ((inChar - 'A' + 10) << (bytePosition-- * 4));
         } else if ( isLowerCase(inChar) ) {
            colorValue |= ((inChar - 'a' + 10) << (bytePosition-- * 4));
         }
      }
   }
   Serial.println(colorValue, HEX);
   Serial.println();
   Serial.flush();
   led.setColor(static_cast<LEDColor>(colorValue));
   led.setState(LEDState::ON);                              // state must be set to ON each time the color changes
}
