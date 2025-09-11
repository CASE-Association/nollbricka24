/* rainbow_wave
 
By: Andrew Tuline
 
Date: Jan, 2020
 
A very simple rainbow wave and rainbow march using FastLED. That being said, I wouldn't normally use
the fill_rainbow function in production code. Rather, I would use palettes.
 
*/
 
 
#define FASTLED_ALLOW_INTERRUPTS 0                            // Used for ESP8266.
#include "FastLED.h"                                          // FastLED library.
 
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
// Fixed definitions cannot change on the fly.
#define LED_DT 0                                             // Serial data pin
#define LED_CK 11                                             // Clock pin for WS2801 or APA102
#define COLOR_ORDER GRB                                       // It's GRB for WS2812B and GBR for APA102
#define LED_TYPE WS2812                                       // What kind of strip are you using (APA102, WS2801 or WS2812B)?
#define NUM_LEDS 16                                        // Number of LED's
 
// Initialize changeable global variables.
uint8_t max_bright = 70;                                     // Overall brightness definition. It can be changed on the fly.
 
struct CRGB leds[NUM_LEDS];                                   // Initialize our LED array.

int x = 0;
int y = 0;
int mode = 0;
int z = 0;
 
void setup() {
 
 
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);         // For WS2812B
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801
 
  FastLED.setBrightness(max_bright);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);              // FastLED power management set at 5V, 500mA

  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);


 
} // setup()
 
 

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
 
void loop () {
 


  if (digitalRead(2) == 0){
    x++;
    if(x == 1){
      if(max_bright>200){
        max_bright = 20;
        FastLED.setBrightness(max_bright);
      }
      else{
        max_bright = max_bright * 1.88;
        FastLED.setBrightness(max_bright);
      }
    }
    else if(x > 200){
      x = 0;
    }
  }
  if (digitalRead(2) == 1){
    x = 0;
  }


  if (digitalRead(4) == 0){
      y++;
      if(y == 1){
        if(mode>7){
          mode = 0;
        }
        else{
          mode ++;
        }
      }
      else if(y > 200){
        y = 0;
      }
    }
    if (digitalRead(4) == 1){
      y = 0;
    }


  if (mode == 0){
    rainbow_wave(20, -15);                                      // Speed, delta hue values.
  }else
  if (mode == 2){
    z = (z + 1) % 600;
    int v = (int)((sin((float)z / 600.0f * 6.283f) / 2.0f + 0.5f) * 255.0f);
    for (int i = 0; i < 16; i++){
      fill_solid( leds, NUM_LEDS, CRGB(v, v, 0));
    }
  }else
  if (mode == 1){
    z = (z + 1) % 200;
    for (int i = 0; i < 16; i++){
      int v = (int)((sin(((float)z / 200.0f + (float)i / 16.0f) * 6.283f) / 2.0f + 0.5f) * 255.0f);
      leds[i] = CRGB(v, v, 0);
    }
  }else
  if (mode == 3){
    delay(1);
    z ++;

    if(z < 107){
    fill_solid( leds, NUM_LEDS, CRGB::Red);
    FastLED.show();
    } else if(z<214){
      fill_solid( leds, NUM_LEDS, CRGB::Green);
      FastLED.show();
    }else{
      fill_solid( leds, NUM_LEDS, CRGB::Blue);
      FastLED.show();
      if(z>321){
        z = 0;
      }
    }
  }else
  if (mode == 4){
    fill_solid( leds, NUM_LEDS, CRGB::Red);
  }else
  if (mode == 5){
    fill_solid( leds, NUM_LEDS, CRGB::Green);
  }else
  if (mode == 6){
    fill_solid( leds, NUM_LEDS, CRGB::Blue);
  }else
  if (mode == 7){
    fill_solid( leds, NUM_LEDS, CRGB::White);
  }else
  if (mode == 8){
    fill_solid( leds, NUM_LEDS, CRGB::Yellow);
  }
  
  FastLED.show();
  
} // loop()
 





 
void rainbow_wave(uint8_t thisSpeed, uint8_t deltaHue) {     // The fill_rainbow call doesn't support brightness levels.
 
// uint8_t thisHue = beatsin8(thisSpeed,0,255);                // A simple rainbow wave.
 uint8_t thisHue = beat8(thisSpeed,255);                     // A simple rainbow march.
  
 fill_rainbow(leds, NUM_LEDS, thisHue, deltaHue);            // Use FastLED's fill_rainbow routine.
 
} // rainbow_wave()
