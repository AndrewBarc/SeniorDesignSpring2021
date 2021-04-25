// FFT Test
//
// Compute a 1024 point Fast Fourier Transform (spectrum analysis)
// on audio connected to the Left Line-In pin.  By changing code,
// a synthetic sine wave can be input instead.
//
// The first 40 (of 512) frequency analysis bins are printed to
// the Arduino Serial Monitor.  Viewing the raw data can help you
// understand how the FFT works and what results to expect when
// using the data to control LEDs, motors, or other fun things!
//
// This example code is in the public domain.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Adafruit_NeoPixel.h>

#define PIN 24
#define N_LEDS 30

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS,PIN, NEO_GRB + NEO_KHZ800);

  uint32_t Red = strip.Color(255,0,0);
  uint32_t RedD0 = strip.ColorHSV(0,255,224);
  uint32_t RedD1 = strip.ColorHSV(0,255,192);
  uint32_t RedD2 = strip.ColorHSV(0,255,128);
  uint32_t RedD3 = strip.ColorHSV(0,255,64);
  uint32_t RedD4 = strip.ColorHSV(0,255,32);
  uint32_t RedD5 = strip.ColorHSV(0,255,16);
  uint32_t RedD6 = strip.ColorHSV(0,255,8);

  uint32_t Green = strip.Color(0,255,0);
  uint32_t GreenD0 = strip.ColorHSV(21845,255,224);
  uint32_t GreenD1 = strip.ColorHSV(21845,255,192);
  uint32_t GreenD2 = strip.ColorHSV(21845,255,128);
  uint32_t GreenD3 = strip.ColorHSV(21845,255,64);
  uint32_t GreenD4 = strip.ColorHSV(21845,255,32);
  uint32_t GreenD5 = strip.ColorHSV(21845,255,16);
  uint32_t GreenD6 = strip.ColorHSV(21845,255,8);  

  uint32_t Blue = strip.Color(0,0,255);
  uint32_t BlueD0 = strip.ColorHSV(43690,255,224);
  uint32_t BlueD1 = strip.ColorHSV(43690,255,192);
  uint32_t BlueD2 = strip.ColorHSV(43690,255,128);
  uint32_t BlueD3 = strip.ColorHSV(43690,255,64);
  uint32_t BlueD4 = strip.ColorHSV(43690,255,32);
  uint32_t BlueD5 = strip.ColorHSV(43690,255,16);
  uint32_t BlueD6 = strip.ColorHSV(43690,255,8);    

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT1024    myFFT;
AudioOutputI2S         audioOutput;        // audio shield: headphones & line-out

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
AudioConnection patchCord2(audioInput, 0, audioOutput,0);
//AudioConnection patchCord1(sinewave, 0, myFFT, 0);

AudioControlSGTL5000 audioShield;

void setup() {
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);

  // Configure the window algorithm to use
  myFFT.windowFunction(AudioWindowHanning1024);
  //myFFT.windowFunction(NULL);

  // Create a synthetic sine wave, for testing
  // To use this, edit the connections above
  sinewave.amplitude(0.8);
  sinewave.frequency(1034.007);
  
  //LEDS
  strip.begin();
  strip.show();
  strip.setBrightness(45);

  Serial.begin(9600);
}

void loop() {
  //pulse();
  float n;
  int i;
  float fft [40];
  float subbass = 0;
  float bass = 0;
  float lower_midrange = 0;
  float midrange = 0;
  float higher_midrange = 0;
  float presence = 0;
  float brillance = 0;

  if (myFFT.available()) {
    // each time new FFT data is available
    // print it all to the Arduino Serial Monitor
    Serial.print("FFT: ");
    for (i=0; i<40; i++) {
      n = myFFT.read(i);
      if (n >= 0.01) {
        fft[i] = n;
        if (i == 1){
          subbass = n;
        }
        else if(i >=2 and i <7){
          bass = n + bass;
        }
        else if( i >= 7 and i <13){
          lower_midrange = n + lower_midrange;
        }
        else if( i >=13 and i <41){
          midrange = n + midrange;
        }
        //Serial.print(n);
        //Serial.print(" ");
      } 
      else {
        fft[i] = 0;
        if (i == 1){
          subbass = n;
        }
        else if(i >=2 and i <7){
          bass = n + bass;
        }
        else if( i >= 7 and i <13){
          lower_midrange = n + lower_midrange;
        }
        else if( i >=13 and i <41){
          midrange = n + midrange;
        }
        //Serial.print("  -  "); // don't print "0.00"
      }
      
    }
    mainpulse(subbass,bass,lower_midrange,midrange);
  }
    Serial.println();
  }
void mainpulse(float subbass, float bass, float lower_midrange, float midrange){
      Serial.println();
      Serial.print("SubBass: ");
      Serial.print(subbass);
      Serial.println();
      Serial.print("Bass: ");
      Serial.print(bass);
      Serial.println();
      Serial.print("Lower Midrange: ");
      Serial.print(lower_midrange);
      Serial.println();
      Serial.print("Midrange: ");
      Serial.print(midrange);
      Serial.println();
          if (subbass < bass and subbass > lower_midrange and subbass > midrange){
      pulse();
      delay(10);
    }
    else if (midrange > bass and midrange > lower_midrange and subbass < midrange){
      pulse2();
      delay(10);
    }    
    else if(midrange <= 0.10){
      strip.clear();
      Serial.println();
      Serial.print("AAAAAAAAAAAAAAAAAAAAAAAA");
      Serial.println();
      strip.show();
    }
    else if (lower_midrange > bass and lower_midrange > subbass and lower_midrange > midrange){
      pulse3();
      delay(10);
    }    
    else{
      pulse();
    }
}

void range(float fft[40]){
  
}

void pulse(){
    for (int i = 0; i < N_LEDS+8; i++){
    strip.setPixelColor(i, Red);
    strip.setPixelColor(i-1,RedD0);
    strip.setPixelColor(i-2,RedD1);
    strip.setPixelColor(i-3,RedD2);
    strip.setPixelColor(i-4,RedD3);
    strip.setPixelColor(i-5,RedD4);
    strip.setPixelColor(i-6,RedD5);
    strip.setPixelColor(i-7,RedD6); 
    strip.show();
    strip.clear();
    delay(8);
  }
}

void pulse2(){
    for (int i = 0; i < N_LEDS+8; i++){
    strip.setPixelColor(i, Green);
    strip.setPixelColor(i-1,GreenD0);
    strip.setPixelColor(i-2,GreenD1);
    strip.setPixelColor(i-3,GreenD2);
    strip.setPixelColor(i-4,GreenD3);
    strip.setPixelColor(i-5,GreenD4);
    strip.setPixelColor(i-6,GreenD5);
    strip.setPixelColor(i-7,GreenD6); 
    strip.show();
    strip.clear();
    delay(8);
  }
}
void pulse3(){
    for (int i = 0; i < N_LEDS+8; i++){
    strip.setPixelColor(i, Blue);
    strip.setPixelColor(i-1,BlueD0);
    strip.setPixelColor(i-2,BlueD1);
    strip.setPixelColor(i-3,BlueD2);
    strip.setPixelColor(i-4,BlueD3);
    strip.setPixelColor(i-5,BlueD4);
    strip.setPixelColor(i-6,BlueD5);
    strip.setPixelColor(i-7,BlueD6); 
    strip.show();
    strip.clear();
    delay(8);
  }
}
