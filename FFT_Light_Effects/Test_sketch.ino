/* Stereo peak meter example, assumes Audio adapter but just uses terminal so no more parts required.

This example code is in the public domain
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Adafruit_NeoPixel.h>

#define PIN 24
#define N_LEDS 30

const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;

AudioInputI2S        audioInput;         // audio shield: mic or line-in
AudioAnalyzePeak     peak_L;
AudioAnalyzePeak     peak_R;
AudioOutputI2S       audioOutput;        // audio shield: headphones & line-out

AudioConnection c1(audioInput,0,peak_L,0);
AudioConnection c2(audioInput,1,peak_R,0);
AudioConnection c3(audioInput,0,audioOutput,0);
AudioConnection c4(audioInput,1,audioOutput,1);

AudioControlSGTL5000 audioShield;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);



void setup() {
  strip.begin();
  AudioMemory(20);
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(.5);
  Serial.begin(9600);
}

// for best effect make your terminal/monitor a minimum of 62 chars wide and as high as you can.

elapsedMillis fps;
uint8_t cnt=0;

void loop() {
   chase1(strip.Color(255, 0, 0)); // Red
   chase1(strip.Color(0, 255, 0)); // Green
   chase1(strip.Color(0, 0, 255)); // Blue
  
  chase2(strip.Color(255, 0, 0)); // Red
   chase2(strip.Color(0, 255, 0)); // Green
   chase2(strip.Color(0, 0, 255)); // Blue

  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
    
 // rainbow(20);  
    
  
    
}

static void chase1(uint32_t c) {
  for(uint16_t i=15; i<30+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
  }
}
static void chase2(uint32_t c) {
  for(uint16_t i=14; i>0-4; i--) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i+4, 0); // Erase pixel a few steps back
      strip.show();
      delay(25);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
 
