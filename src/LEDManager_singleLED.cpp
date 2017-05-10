/*

Arduino IDE library for managing single-color and RGB LEDs across multiple platforms using a consistent API

Copyright 2017 Rob Redford
This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 (CC BY-SA 4.0) International License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0
*/

#include <LEDManager.h>

//#define DEBUG_INFO
//#define DEBUG_LOG

#include "DebugLib.h"

/*
Callback funciton to toggle the LED's state for blinking
Note that as you cannot get the address of a class member function, this has to exist outside the class
and we use a pointer to the object to allow this function access the LED object

Note that the SysTimer library disables interrupts before transfering control to our handler
*/
void _SingleToggle (SingleLED* led) {
   led->_toggleState();
}

// single pin LED constructor
SingleLED::SingleLED (const uint8_t pin) {
   _type = LEDType::SINGLE;
   _LEDpin = pin;
   pinMode(_LEDpin, OUTPUT);
}

// ensure timer object is valid
bool SingleLED::begin(void) {
   return _timer.begin();
}

// Single-color LED illumination
void SingleLED::_illuminate (const bool mode) {
   if ( mode ) {
      INFO(F("Solid LED"), F("ON"));
      digitalWrite(_LEDpin, HIGH);
   } else {
      INFO(F("Solid LED"), F("OFF"));
      digitalWrite(_LEDpin, LOW);
   }
}

/*
This ISR is called by the physical pin change "C" function to change the current (temporal) state for blinking
*/
void SingleLED::_toggleState (void) {
   _illuminated = !_illuminated;
   _illuminate(_illuminated);
}

/*
Set a new target state of the LED
For blinking, the state indicates the initial condition of the LED - this allows us to alternate between 2 LEDs
*/
void SingleLED::setState(const LEDState ledState, const uint32_t interval) {
   if ( _timerArmed ) {
      // reset the timer whenever we change the state
      _timer.disarm();
      _timerArmed = false;
   }
   INFO(F("State set to"), (int)ledState);
   INFO(F("Interval"), interval);
   _state = ledState;
   switch ( ledState ) {
   case LEDState::ON:
   case LEDState::BLINK_ON:
      _illuminated = true;
      SingleLED::_illuminate(true);

      if ( ledState == LEDState::BLINK_ON ) {
         // blink, with initial state ON
         _timer.attachInterrupt(reinterpret_cast<CallbackArg>(&_SingleToggle), reinterpret_cast<void*>(this));
         _timer.setInterval(interval);
         _timer.arm(true);
         _timerArmed = true;
      }
      break;

   case LEDState::OFF:
   case LEDState::BLINK_OFF:
      _illuminated = false;
      _illuminate(false);

      if ( ledState == LEDState::BLINK_OFF ) {
         // blinking, but initial state is OFF
         _timer.attachInterrupt(reinterpret_cast<CallbackArg>(&_SingleToggle), reinterpret_cast<void*>(this));
         _timer.setInterval(interval);
         _timer.arm(true);
         _timerArmed = true;
      }
      break;

   default:
      break;
   }
}
