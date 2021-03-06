/*
 Example sketch for LEDManager library
 
 Demonstration of the different modes, for both RGB and solid LEDs
 
 For common anode implementations, attach the common lead to Vcc and attach each RGB lead to the color pin through a current limiting resistor (current sink)
 For common cathode implementations, attach the common lead to ground, and attach each RGB lead to the color pin through a current limiting resistor (current source)
 
 Copyright 2017 Rob Redford
   This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 (CC BY-SA 4.0) International License.
   To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0
*/

#include <LEDManager.h>

// RGB LED
#define RED_PIN   14			// ESP8266 pin assignments. Change to match your board & confiruation
#define GREEN_PIN	12			// make sure there is a current limiting resistor attached to each color lead
#define BLUE_PIN	13

// single-color LED
#define LED_PIN    2       

#define COLORS 8
LEDColor colors[] = { LEDColor::RED, LEDColor::GREEN, LEDColor::BLUE, LEDColor::MAGENTA, LEDColor::CYAN, LEDColor::WHITE, LEDColor::ORANGE, LEDColor::PURPLE };


void setup ( void ) {
   Serial.begin(115200);
   while (!Serial);
}

String colorName (const LEDColor color) {
   String name;

   switch ( color ) {
   case LEDColor::RED:
      name = String("RED");
      break;
   case LEDColor::GREEN:
      name = String("GREEN");
      break;
   case LEDColor::BLUE:
      name = String("BLUE");
      break;
   case LEDColor::MAGENTA:
      name = String("MAGENTA");
      break;
   case LEDColor::YELLOW:
      name = String("YELLOW");
      break;
   case LEDColor::CYAN:
      name = String("CYAN");
      break;
   case LEDColor::WHITE:
      name = String("WHITE");
      break;
   case LEDColor::ORANGE:
      name = String("ORANGE");
      break;
   case LEDColor::PURPLE:
      name = String("PURPLE");
      break;
   case LEDColor::NONE:                   // avoid compiler warning
   default:
      break;
   }
   return name;
}

void loop ( void ) {
   RGBLED		rgb(RED_PIN, GREEN_PIN, BLUE_PIN);
   //RGBLED		rgb(RED_PIN, GREEN_PIN, BLUE_PIN, LEDType::ANODE);						// constructor for common anode

   SingleLED   led(LED_PIN);
   
   if (!rgb.begin()) {
      Serial.println(F("ERROR: could not allocate RGB LED object <<<<"));
      while (true) delay (500);
   }
   if (!led.begin()) {
      Serial.println(F("ERROR: could not allocate single LED object <<<<"));
      while (true) delay (500);
   }
   
   // run through the solid colors, solid LED o
   Serial.println(F("\n\n... Starting cycle ... SOLID COLORS ..."));
   led.setState(LEDState::ON);
   for ( int i = 0; i < COLORS; i++ ) {
      LEDColor color = colors[i];
      rgb.setColor(color);                      // after setting color, must set to ON before it is illuminated
      rgb.setState(LEDState::ON);
      Serial.print(F("Color: ")); Serial.print(colorName(rgb.getColor()[0]));       // just to demo getColor() function
      Serial.print(F(" ")); Serial.println((uint32_t)color, HEX);
      delay(5000);
      rgb.setState(LEDState::OFF);
   }	
   delay(5000);

   // same colors, but now blinking RGB LED on/off in tandem with solid LED
   led.setState(LEDState::BLINK_ON, 500);
   Serial.println(F("\n\n ... BLINKING IN TANDEM ..."));
   for ( auto color : colors ) {
      rgb.setColor(color);
      rgb.setState(LEDState::BLINK_ON, 500);
      Serial.print(F("Color: ")); Serial.print(colorName(rgb.getColor()[0]));
      Serial.print(F(" ")); Serial.println((uint32_t)color, HEX);
      delay(5000);
      rgb.setState(LEDState::OFF);
   }
   led.setState(LEDState::OFF);
   delay(5000);

   // alternate between RGB and solid LED
   Serial.println(F("\n\n... ALTERNATING BETWEEN RGB & SOLID ..."));
   Serial.println(F("RGB: RED"));
   rgb.setColor(LEDColor::RED);
   rgb.setState(LEDState::BLINK_ON, 500);             // LEDs will alternate when blinking initiated in opposite states
   led.setState(LEDState::BLINK_OFF, 500);
   delay(5000);
   Serial.println(F("RGB: GREEN"));
   rgb.setColor(LEDColor::GREEN);
   rgb.setState(LEDState::BLINK_ON, 125);            // faster blinking
   led.setState(LEDState::BLINK_OFF, 125);
   delay(5000);
   led.setState(LEDState::OFF);
   rgb.setState(LEDState::OFF);

   // alternating RGB colors
   Serial.println(F("\n\n... ALTERNATING COLORS on RGB ONLY ..."));
   Serial.println(F("RED/GREEN"));
   rgb.setColor(LEDColor::GREEN, LEDColor::RED);
   rgb.setState(LEDState::ALTERNATE, 500);
   delay(5000);
   Serial.println(F("ORANGE/PURPLE"));
   rgb.setColor(LEDColor::ORANGE, LEDColor::PURPLE);
   rgb.setState(LEDState::ALTERNATE, 1000);
   delay(10000);
   Serial.println(F("BLUE/MAGENTA"));
   rgb.setColor(LEDColor::BLUE, LEDColor::MAGENTA);
   rgb.setState(LEDState::ALTERNATE, 125);
   delay(5000);
   Serial.println(F("RED/GREEN/BLUE/ORANGE/CYAN/PURPLE"));
   rgb.setColor(LEDColor::RED, LEDColor::GREEN, LEDColor::BLUE, LEDColor::ORANGE, LEDColor::CYAN, LEDColor::PURPLE);
   rgb.setState(LEDState::ALTERNATE, 250);
   delay(10000);
   
   Serial.println(F("\n\n... Cycle complete ...\n"));
   rgb.setState(LEDState::OFF);
   while (true) delay(500);
}