/*

Arduino IDE library for managing single-color and RGB LEDs across multiple platforms using a consistent API

Copyright 2017 Rob Redford
This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 (CC BY-SA 4.0) International License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-sa/4.0
*/

#ifndef _LEDManager_H_
#define _LEDManager_H_

#if defined(ARDUINO) && ARDUINO >= 100
   #include <arduino.h>
#else
   #error Older versions of Arduino IDE not supported
#endif

#include <SysTimer.h>

// type for RGB LEDs
enum class LEDType:uint8_t { ANODE, CATHODE, SINGLE };

// colors for RGB LED
enum class LEDColor:uint32_t {
   NONE     =  0x000000,
   RED		=	0xFF0000,
   GREEN    =  0x00FF00,
   BLUE		=	0x0000FF,
   MAGENTA	=	0xFF00F0,
   YELLOW	=	0xFFFF00,
   CYAN		=	0x00FFFF,
   WHITE	   =  0xFFFFFF,
   ORANGE	=	0xFFA500,
   PURPLE	=	0x800080,
};

#define MAX_RGB_COLORS         6

typedef LEDColor LEDColorArray[MAX_RGB_COLORS];

// LED states
enum class LEDState:uint8_t { OFF, ON, BLINK_OFF, BLINK_ON, ALTERNATE };


// common base class - not used alone
class LEDCommon {
public:
   LEDCommon() {}

   LEDState   getState(void) const { return _state; }
   
protected:
   LEDType    _type = LEDType::SINGLE;              // single LED -or- common anode or cathode for RGB LEDs
   LEDState   _state = LEDState::OFF;
   volatile   bool _illuminated = false;            // for blinking  - illumination state
   bool       _timerArmed = false;
   SysTimer   _timer;                               // multi-platform hardware timer object
};

// single-color (2-lead) LED
class SingleLED : public LEDCommon {
public:
   SingleLED (const uint8_t pin);
   
   bool begin(void);

   void       setState(const LEDState ledState, const uint32_t interval = 500);

private:
   void       _illuminate(const bool mode);
   void       _toggleState(void);

   friend     void _SingleToggle(SingleLED* led);

   uint8_t    _LEDpin = 0;
};

// 3-color LED, common cathode or anode
class RGBLED : public LEDCommon {
public:
   RGBLED (const uint8_t redPin, const uint8_t greenPin, const uint8_t bluePin, LEDType ledType = LEDType::CATHODE);
   
   bool begin(void);

   void       setState(const LEDState ledState, const uint32_t interval = 500);
   void       setColor(const LEDColor color1, const LEDColor color2 = LEDColor::NONE,  const LEDColor color3 = LEDColor::NONE,
                       const LEDColor color4 = LEDColor::NONE, const LEDColor color5 = LEDColor::NONE, const LEDColor color6 = LEDColor::NONE);
   const      LEDColorArray& getColor(void) const { return _color; }

private:
   void       _illuminate(const LEDColor color);
   void       _toggleState(void);
   void       _alternateRGB(void);

   friend     void _RGBToggle(RGBLED* led);
   friend     void _RGBAlternate(RGBLED* led);

   uint8_t       _redPin = 0, _greenPin = 0, _bluePin = 0;
   LEDColorArray _color = {LEDColor::NONE};              // 1 or more colors to display
   volatile      uint8_t _colorIndex = 0;                // current LED color to display
};

#endif
