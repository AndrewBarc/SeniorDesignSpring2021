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

    uint32_t Blank = strip.Color(0,0,0); 
    uint32_t Orange = strip.Color(255,69,0);
    uint32_t Yellow = strip.Color(255,255,0);
    uint32_t Purple = strip.Color(130,0,255);
    int count1 = 0;
    uint32_t colorArray1[29];
    
const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT1024    myFFT;
AudioAnalyzeFFT1024    myFFT1;
AudioOutputI2S         audioOutput;        // audio shield: headphones & line-out

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
AudioConnection patchCord2(audioInput, 0, audioOutput,0);
AudioConnection patchCord4(audioInput, 1, myFFT1,0);
AudioConnection patchCord3(audioInput, 1, audioOutput,1);

//AudioConnection patchCord1(sinewave, 0, myFFT, 0);

AudioControlSGTL5000 audioShield;

int stereoIntensity(float subbass, float bass, int direction,uint32_t color,int count);
uint32_t Color4Raj();
void easybpm(float subbass, float bass);
void whiteblack(float subbass,float bass, float lower_midrange, float midrange, float higher_midrange, float presence,float brillance);
void wave(float subbass,float bass, float lower_midrange, float midrange, float higher_midrange, float presence,float brillance);

void setup() {
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(24);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);

  // Configure the window algorithm to use
  myFFT.windowFunction(AudioWindowHanning1024);
  myFFT1.windowFunction(AudioWindowHanning1024);
  //myFFT.windowFunction(NULL);

  // Create a synthetic sine wave, for testing
  // To use this, edit the connections above
  sinewave.amplitude(0.8);
  sinewave.frequency(1034.007);
  
  //LEDS
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  Serial.begin(9600);
}

void loop() {
  veryfastft(myFFT,myFFT1);
  }

void veryfastft(AudioAnalyzeFFT1024 myFFT,AudioAnalyzeFFT1024 myFFT1){
  uint32_t color;
  float n;
  float m;
  int j;
  int i;
  float fft [500];
  float subbass = 0;
  float subbass2 = 0;
  float bass = 0;
  float bass2 = 0;
  float lower_midrange = 0;
  float lower_midrange2 = 0;
  float midrange = 0;
  float midrange2 = 0;
  float higher_midrange = 0;
  float higher_midrange2 = 0;
  float presence = 0;
  float presence2 = 0;
  float brillance = 0;
  float brillance2 = 0;
  
  if (myFFT.available()) {
    Serial.print("FFT: ");
    for (i=0; i<500; i++) {
      n = myFFT.read(i);
      
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
        else if( i >=13 and i <49){
          midrange = n + midrange;
        }
        else if( i >=49 and i <100){
          higher_midrange = n + higher_midrange;
        }
        else if( i >=100 and i <150){
          presence = n + presence;
        }
        else if( i >=150 and i <501){
          brillance = n + brillance;
        }
      
    }
  if (myFFT1.available()){
  for(j=0; j<500; j++){
    m = myFFT1.read(j);
      if(j == 1){
        subbass2 = m;
      }
      else if (j >= 2 and j < 7){
        bass2 = m + bass2;
      }
      else if(j >= 7 and j < 13){
        lower_midrange2 = m + lower_midrange2;
      }
      else if(j >= 13 and j < 49){
        midrange2 = midrange2 + m;
      }
      else if (j >= 49 and j < 100){
        higher_midrange2 = higher_midrange2 + m;
      }
      else if (j >= 100 and j <150){
        presence2 = presence2 + m;
      }
      else if (j >= 150 and j < 501){
        brillance2 = brillance2 + m;
    }

  }
  }
  if(count1 >= 0 and count1< 5000){
    color = Red;
  }
  else if (count1 >= 5000 and count1<10000){
    color = Green;
  }
  else if (count1 >=10000){
    color = Blue;
  }
  
 // count1 = stereoIntensity(subbass,bass,0,color,count1);
 // count1 = stereoIntensity(subbass2,bass2,1,color, count1);
  Serial.println();
  Serial.print(count1);  
  //easybpm(subbass,bass);
  //range5(subbass2,bass2,lower_midrange2,midrange2,higher_midrange2,presence2,brillance2);
  //whiteback(subbass,bass,lower_midrange,midrange,higher_midrange,presence,brillance);
  //delay(100);
  wave(subbass,bass,lower_midrange,midrange,higher_midrange,presence,brillance);
  }
}

void whiteback(float subbass,float bass, float lower_midrange, float midrange, float higher_midrange,float presence, float brillance){
  uint32_t White = strip.Color(255,255,255);
  //strip.fill(White,0,29);
  bass = subbass + bass;
  if(bass >= lower_midrange and bass >= midrange and bass >= higher_midrange and 
    bass >= presence and bass >= brillance){
    strip.fill(Red,0,29);
    //pulse();  
    }
  else if(midrange >= bass and midrange >= lower_midrange and midrange >= higher_midrange and
    midrange >= presence and midrange >= brillance){
    //strip.fill(Green,0,29);
    //pulse2();
    }
  else{
    strip.clear();
    //strip.fill(White,0,29);
  }
  strip.show();
}

void easybpm(float subbass,float bass ){
  float bpm;
  bpm = subbass + bass;
  if (bpm >= 0.3){
    pulse();
  }
}

uint32_t Color4Raj(){
  uint32_t Orange = strip.Color(255,128,0);
  uint32_t Yellow = strip.Color(255,255,0);
  uint32_t Purple = strip.Color(130,0,255);
  uint32_t Red  = strip.Color(225,0,0);
  uint32_t Green  = strip.Color(0,255,0);
  uint32_t Blue   = strip.Color(0,0,255);
  int rand;
  rand = random(0,5);
  if (rand == 0){
    return Red;
  }
  else if (rand == 1){
    return Orange;
  }
  else if (rand == 2){
    return Yellow;
  }
  else if (rand == 3){
    return Green;
  }
  else if (rand == 4){
    return Blue;
  }
  else if (rand == 5){
    return Purple;
  }
}

void wave(float subbass,float bass, float lower_midrange, float midrange, 
  float higher_midrange, float presence,float brillance){
  int i = 0;
  uint32_t temp1;
  uint32_t temp2;
  bass = subbass + bass;
  midrange = lower_midrange + higher_midrange + midrange;
  presence = presence + brillance;
  if(bass >= midrange and bass >= presence){
    for(i = 29; i >=3; i--){
      colorArray1[i] = colorArray1[i-3];
    }
    colorArray1[0] = Red;
    colorArray1[1] = Red;
    colorArray1[2] = Red;
    delay(50);
  }
  else if(midrange >= bass and midrange >= presence){
    for(i = 29; i >=3; i--){
      colorArray1[i] = colorArray1[i-3];
    }
    colorArray1[0] = Green;
    colorArray1[1] = Green;
    colorArray1[2] = Green;
    delay(50);
  }
  else if(presence >= midrange and presence >= bass){
    for(i = 29; i >=3; i--){
      colorArray1[i] = colorArray1[i-3];
    }
    colorArray1[0] = Blue;
    colorArray1[1] = Blue;
    colorArray1[2] = Blue;
    delay(50);
  }
  else{
    for(i = 29; i >=3; i--){
      colorArray1[i] = colorArray1[i-3];
    }
    colorArray1[0] = Blank;
    colorArray1[1] = Blank;
    colorArray1[2] = Blank;
  }
  for(i = 0; i < 30; i++){
    strip.setPixelColor(i,colorArray1[i]);
  }
  
  strip.show();
}

int stereoIntensity(float subbass, float bass, int direction,uint32_t color,int count){
  bass = bass + subbass;
  count++;
  if (count > 15000){
    count = 0;
  }
  if(direction == 0){
  
    if (bass >= 0.05 and bass < 0.2){
      strip.setPixelColor(14,color);
    }
    else if (bass >= 0.2 and bass < 0.3){
      strip.setPixelColor(14,color);
      strip.setPixelColor(13,color);
      strip.setPixelColor(12,color);
    }
    else if (bass >= 0.3 and bass < 0.4){
      strip.setPixelColor(14,color);
      strip.setPixelColor(13,color);
      strip.setPixelColor(12,color);
      strip.setPixelColor(11,color);
      strip.setPixelColor(10,color);
    }
    else if (bass >= 0.4 and bass < 0.5){
      strip.setPixelColor(14,color);
      strip.setPixelColor(13,color);
      strip.setPixelColor(12,color);
      strip.setPixelColor(11,color);
      strip.setPixelColor(10,color);
      strip.setPixelColor(9,color);
      strip.setPixelColor(8,color);
    }
    else if (bass >= 0.5 and bass < 0.6){
      strip.setPixelColor(14,color);
      strip.setPixelColor(13,color);
      strip.setPixelColor(12,color);
      strip.setPixelColor(11,color);
      strip.setPixelColor(10,color);
      strip.setPixelColor(9,color);
      strip.setPixelColor(8,color);
      strip.setPixelColor(7,color);
      strip.setPixelColor(6,color);
    }
    else if (bass >= 0.6 and bass < 0.7){
      strip.setPixelColor(14,color);
      strip.setPixelColor(13,color);
      strip.setPixelColor(12,color);
      strip.setPixelColor(11,color);
      strip.setPixelColor(10,color);
      strip.setPixelColor(9,color);
      strip.setPixelColor(8,color);
      strip.setPixelColor(7,color);
      strip.setPixelColor(6,color);
      strip.setPixelColor(5,color);
      strip.setPixelColor(4,color);
    }
    else if (bass >= 0.7 and bass < 0.8){
      strip.setPixelColor(14,color);
      strip.setPixelColor(13,color);
      strip.setPixelColor(12,color);
      strip.setPixelColor(11,color);
      strip.setPixelColor(10,color);
      strip.setPixelColor(9,color);
      strip.setPixelColor(8,color);
      strip.setPixelColor(7,color);
      strip.setPixelColor(6,color);
      strip.setPixelColor(5,color);
      strip.setPixelColor(4,color);
      strip.setPixelColor(3,color);
      strip.setPixelColor(2,color);
    }
    else if (bass >= 0.8){
      strip.setPixelColor(14,color);
      strip.setPixelColor(13,color);
      strip.setPixelColor(12,color);
      strip.setPixelColor(11,color);
      strip.setPixelColor(10,color);
      strip.setPixelColor(9,color);
      strip.setPixelColor(8,color);
      strip.setPixelColor(7,color);
      strip.setPixelColor(6,color);
      strip.setPixelColor(5,color);
      strip.setPixelColor(4,color);
      strip.setPixelColor(3,color);
      strip.setPixelColor(2,color);
      strip.setPixelColor(1,color);
      strip.setPixelColor(0,color);
    }
  }
  else if(direction == 1){
  
    if (bass >= 0.05 and bass < 0.2){
      strip.setPixelColor(15,color);
    }
    else if (bass >= 0.2 and bass < 0.3){
      strip.setPixelColor(15,color);
      strip.setPixelColor(16,color);
      strip.setPixelColor(17,color);
    }
    else if (bass >= 0.3 and bass < 0.4){
      strip.setPixelColor(15,color);
      strip.setPixelColor(16,color);
      strip.setPixelColor(17,color);
      strip.setPixelColor(18,color);
      strip.setPixelColor(19,color);
    }
    else if (bass >= 0.4 and bass < 0.5){
      strip.setPixelColor(15,color);
      strip.setPixelColor(16,color);
      strip.setPixelColor(17,color);
      strip.setPixelColor(18,color);
      strip.setPixelColor(19,color);
      strip.setPixelColor(20,color);
      strip.setPixelColor(21,color);
    }
    else if (bass >= 0.5 and bass < 0.6){
      strip.setPixelColor(15,color);
      strip.setPixelColor(16,color);
      strip.setPixelColor(17,color);
      strip.setPixelColor(18,color);
      strip.setPixelColor(19,color);
      strip.setPixelColor(20,color);
      strip.setPixelColor(21,color);
      strip.setPixelColor(22,color);
      strip.setPixelColor(23,color);
    }
    else if (bass >= 0.6 and bass < 0.7){
      strip.setPixelColor(15,color);
      strip.setPixelColor(16,color);
      strip.setPixelColor(17,color);
      strip.setPixelColor(18,color);
      strip.setPixelColor(19,color);
      strip.setPixelColor(20,color);
      strip.setPixelColor(21,color);
      strip.setPixelColor(22,color);
      strip.setPixelColor(23,color);
      strip.setPixelColor(24,color);
      strip.setPixelColor(25,color);
    }
    else if (bass >= 0.7 and bass < 0.8){
      strip.setPixelColor(15,color);
      strip.setPixelColor(16,color);
      strip.setPixelColor(17,color);
      strip.setPixelColor(18,color);
      strip.setPixelColor(19,color);
      strip.setPixelColor(20,color);
      strip.setPixelColor(21,color);
      strip.setPixelColor(22,color);
      strip.setPixelColor(23,color);
      strip.setPixelColor(24,color);
      strip.setPixelColor(25,color);
      strip.setPixelColor(26,color);
      strip.setPixelColor(27,color);
    }
    else if (bass >= 0.8){
      strip.setPixelColor(15,color);
      strip.setPixelColor(16,color);
      strip.setPixelColor(17,color);
      strip.setPixelColor(18,color);
      strip.setPixelColor(19,color);
      strip.setPixelColor(20,color);
      strip.setPixelColor(21,color);
      strip.setPixelColor(22,color);
      strip.setPixelColor(23,color);
      strip.setPixelColor(24,color);
      strip.setPixelColor(25,color);
      strip.setPixelColor(26,color);
      strip.setPixelColor(27,color);
      strip.setPixelColor(28,color);
      strip.setPixelColor(29,color);
    }
  }
  strip.show();
  strip.clear();
  return count;
}

void mainpulse(float subbass, float bass, float lower_midrange, float midrange){
      
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

void range(float subbass, float bass, float lower_midrange, float midrange, float higher_midrange,float presence,float brillance){
  bass = bass+subbass;
  if(bass > .1 and bass < .3){
    strip.setPixelColor(2,Red);
  }
  else if(bass >= .3 and bass < .6){
    strip.setPixelColor(1,Red);
    strip.setPixelColor(2,Red);
    strip.setPixelColor(3,Red);
  }
  else if(bass >= .6){
    strip.setPixelColor(0,Red);
    strip.setPixelColor(1,Red);
    strip.setPixelColor(2,Red);
    strip.setPixelColor(3,Red);
    strip.setPixelColor(4,Red);
  }
  if(lower_midrange > .1 and lower_midrange < .3){
    strip.setPixelColor(7,Orange);
  }
  else if(lower_midrange >= .3 and lower_midrange < .45){
    strip.setPixelColor(6,Orange);
    strip.setPixelColor(7,Orange);
    strip.setPixelColor(8,Orange);
  }
  else if(lower_midrange >= .45){
    strip.setPixelColor(5,Orange);
    strip.setPixelColor(6,Orange);
    strip.setPixelColor(7,Orange);
    strip.setPixelColor(8,Orange);
    strip.setPixelColor(9,Orange);
  }
  if(midrange > .1 and midrange <.3){
    strip.setPixelColor(12,Yellow);
  }
  else if(midrange >= .3 and midrange < .45){
    strip.setPixelColor(11,Yellow);
    strip.setPixelColor(12,Yellow);
    strip.setPixelColor(13,Yellow);
  }
  else if(midrange >= .45){
    strip.setPixelColor(10,Yellow);
    strip.setPixelColor(11,Yellow);
    strip.setPixelColor(12,Yellow);
    strip.setPixelColor(13,Yellow);
    strip.setPixelColor(14,Yellow);
  }
    if(higher_midrange > .1 and higher_midrange <.3){
    strip.setPixelColor(17,Green);
  }
  else if(higher_midrange >= .3 and higher_midrange < .45){
    strip.setPixelColor(16,Green);
    strip.setPixelColor(17,Green);
    strip.setPixelColor(18,Green);
  }
  else if(higher_midrange >= .45){
    strip.setPixelColor(15,Green);
    strip.setPixelColor(16,Green);
    strip.setPixelColor(17,Green);
    strip.setPixelColor(18,Green);
    strip.setPixelColor(19,Green);
  }
      if(presence > .1 and presence <.3){
    strip.setPixelColor(22,Blue);
  }
  else if(presence >= .3 and presence < .45){
    strip.setPixelColor(21,Blue);
    strip.setPixelColor(22,Blue);
    strip.setPixelColor(23,Blue);
  }
  else if(presence >= .45){
    strip.setPixelColor(20,Blue);
    strip.setPixelColor(21,Blue);
    strip.setPixelColor(22,Blue);
    strip.setPixelColor(23,Blue);
    strip.setPixelColor(24,Blue);
  }
    if(brillance > .1 and brillance < .3){
    strip.setPixelColor(27,Purple);
  }
  else if(brillance >= .3 and presence < .46){
    strip.setPixelColor(26,Purple);
    strip.setPixelColor(27,Purple);
    strip.setPixelColor(28,Purple);
  }
  else if(brillance >= .46){
    strip.setPixelColor(25,Purple);
    strip.setPixelColor(26,Purple);
    strip.setPixelColor(27,Purple);
    strip.setPixelColor(28,Purple);
    strip.setPixelColor(29,Purple);
  }
  strip.show();
  strip.clear();
}

void range5(float subbass, float bass, float lower_midrange, float midrange, float higher_midrange,float presence,float brillance){
  bass = bass+subbass;
  midrange = midrange + lower_midrange;
  if(bass > .1 and bass < .3){
    strip.setPixelColor(2,Red);
  strip.setPixelColor(3,Red);
  }
  else if(bass >= .3 and bass < .6){
    strip.setPixelColor(1,Red);
    strip.setPixelColor(2,Red);
    strip.setPixelColor(3,Red);
  strip.setPixelColor(4,Red);
  }
  else if(bass >= .6){
    strip.setPixelColor(0,Red);
    strip.setPixelColor(1,Red);
    strip.setPixelColor(2,Red);
    strip.setPixelColor(3,Red);
    strip.setPixelColor(4,Red);
  strip.setPixelColor(5,Red);
  }

  if(midrange > .1 and midrange <.3){
    strip.setPixelColor(8,Yellow);
  strip.setPixelColor(9,Yellow);
  }
  else if(midrange >= .3 and midrange < .45){
    strip.setPixelColor(7,Yellow);
    strip.setPixelColor(8,Yellow);
    strip.setPixelColor(9,Yellow);
  strip.setPixelColor(10,Yellow);
  }
  else if(midrange >= .45){
    strip.setPixelColor(6,Yellow);
    strip.setPixelColor(7,Yellow);
    strip.setPixelColor(8,Yellow);
    strip.setPixelColor(9,Yellow);
    strip.setPixelColor(10,Yellow);
  strip.setPixelColor(11,Yellow);
  }
    if(higher_midrange > .1 and higher_midrange <.3){
    strip.setPixelColor(14,Green);
  strip.setPixelColor(15,Green);
  }
  else if(higher_midrange >= .3 and higher_midrange < .45){
    strip.setPixelColor(13,Green);
    strip.setPixelColor(14,Green);
    strip.setPixelColor(15,Green);
  strip.setPixelColor(16,Green);
  }
  else if(higher_midrange >= .45){
    strip.setPixelColor(12,Green);
    strip.setPixelColor(13,Green);
    strip.setPixelColor(14,Green);
    strip.setPixelColor(15,Green);
    strip.setPixelColor(16,Green);
  strip.setPixelColor(17,Green);
  }
      if(presence > .1 and presence <.3){
    strip.setPixelColor(21,Blue);
  strip.setPixelColor(20,Blue);
  }
  else if(presence >= .3 and presence < .45){
    strip.setPixelColor(19,Blue);
  strip.setPixelColor(20,Blue);
  strip.setPixelColor(21,Blue);
    strip.setPixelColor(22,Blue);
  }
  else if(presence >= .45){
  strip.setPixelColor(18,Blue);
  strip.setPixelColor(19,Blue);  
    strip.setPixelColor(20,Blue);
    strip.setPixelColor(21,Blue);
    strip.setPixelColor(22,Blue);
    strip.setPixelColor(23,Blue);
  }
    if(brillance > .1 and brillance < .3){
    strip.setPixelColor(26,Purple);
  strip.setPixelColor(27,Purple);
  }
  else if(brillance >= .3 and presence < .46){
    strip.setPixelColor(25,Purple);
  strip.setPixelColor(26,Purple);
    strip.setPixelColor(27,Purple);
    strip.setPixelColor(28,Purple);
  }
  else if(brillance >= .46){
    strip.setPixelColor(24,Purple);
  strip.setPixelColor(25,Purple);
    strip.setPixelColor(26,Purple);
    strip.setPixelColor(27,Purple);
    strip.setPixelColor(28,Purple);
    strip.setPixelColor(29,Purple);
  }
  strip.show();
  strip.clear();
}

void range3(float subbass, float bass, float lower_midrange, float midrange, float higher_midrange, float presence, float brillance){
  bass = subbass + bass;
  midrange = lower_midrange + midrange + higher_midrange;
  presence = presence + brillance;
  if (bass >= .1 and bass < .3){
    strip.setPixelColor(4,Red);
    strip.setPixelColor(5,Red);
  }
  else if(bass >= .3 and bass < .5){
    strip.setPixelColor(3,Red);
    strip.setPixelColor(4,Red);
    strip.setPixelColor(5,Red);
    strip.setPixelColor(6,Red);
  }
  else if(bass >= .5 and bass< .7){
    strip.setPixelColor(2,Red);
    strip.setPixelColor(3,Red);
    strip.setPixelColor(4,Red);
    strip.setPixelColor(5,Red);
    strip.setPixelColor(6,Red);
    strip.setPixelColor(7,Red);
  }
  else if(bass >= .7 and bass <.9){
    strip.setPixelColor(1,Red);
    strip.setPixelColor(2,Red);
    strip.setPixelColor(3,Red);
    strip.setPixelColor(4,Red);
    strip.setPixelColor(5,Red);
    strip.setPixelColor(6,Red);
    strip.setPixelColor(7,Red);
    strip.setPixelColor(8,Red);
  }
  else if(bass >= .9){
    strip.setPixelColor(0,Red);
    strip.setPixelColor(1,Red);
    strip.setPixelColor(2,Red);
    strip.setPixelColor(3,Red);
    strip.setPixelColor(4,Red);
    strip.setPixelColor(5,Red);
    strip.setPixelColor(6,Red);
    strip.setPixelColor(7,Red);
    strip.setPixelColor(8,Red);
    strip.setPixelColor(9,Red);
  }
    if (midrange >= .1 and midrange < .3){
    strip.setPixelColor(14,Yellow);
    strip.setPixelColor(15,Yellow);
  }
  else if(midrange >= .3 and midrange < .5){
    strip.setPixelColor(13,Yellow);
    strip.setPixelColor(14,Yellow);
    strip.setPixelColor(15,Yellow);
    strip.setPixelColor(16,Yellow);
  }
  else if(midrange >= .5 and midrange< .7){
    strip.setPixelColor(12,Yellow);
    strip.setPixelColor(13,Yellow);
    strip.setPixelColor(14,Yellow);
    strip.setPixelColor(15,Yellow);
    strip.setPixelColor(16,Yellow);
    strip.setPixelColor(17,Yellow);
  }
  else if(midrange >= .7 and midrange <.9){
    strip.setPixelColor(11,Yellow);
    strip.setPixelColor(12,Yellow);
    strip.setPixelColor(13,Yellow);
    strip.setPixelColor(14,Yellow);
    strip.setPixelColor(15,Yellow);
    strip.setPixelColor(16,Yellow);
    strip.setPixelColor(17,Yellow);
    strip.setPixelColor(18,Yellow);
  }
  else if(midrange >= .9){
    strip.setPixelColor(10,Yellow);
    strip.setPixelColor(11,Yellow);
    strip.setPixelColor(12,Yellow);
    strip.setPixelColor(13,Yellow);
    strip.setPixelColor(14,Yellow);
    strip.setPixelColor(15,Yellow);
    strip.setPixelColor(16,Yellow);
    strip.setPixelColor(17,Yellow);
    strip.setPixelColor(18,Yellow);
    strip.setPixelColor(19,Yellow);
  }
      if (presence >= .1 and presence < .3){
    strip.setPixelColor(24,Green);
    strip.setPixelColor(25,Green);
  }
  else if(presence >= .3 and presence < .5){
    strip.setPixelColor(23,Green);
    strip.setPixelColor(24,Green);
    strip.setPixelColor(25,Green);
    strip.setPixelColor(26,Green);
  }
  else if(presence >= .5 and presence< .7){
    strip.setPixelColor(22,Green);
    strip.setPixelColor(23,Green);
    strip.setPixelColor(24,Green);
    strip.setPixelColor(25,Green);
    strip.setPixelColor(26,Green);
    strip.setPixelColor(27,Green);
  }
  else if(presence >= .7 and presence <.9){
    strip.setPixelColor(21,Green);
    strip.setPixelColor(22,Green);
    strip.setPixelColor(23,Green);
    strip.setPixelColor(24,Green);
    strip.setPixelColor(25,Green);
    strip.setPixelColor(26,Green);
    strip.setPixelColor(27,Green);
    strip.setPixelColor(28,Green);
  }
  else if(presence >= .9){
    strip.setPixelColor(20,Green);
    strip.setPixelColor(21,Green);
    strip.setPixelColor(22,Green);
    strip.setPixelColor(23,Green);
    strip.setPixelColor(24,Green);
    strip.setPixelColor(25,Green);
    strip.setPixelColor(26,Green);
    strip.setPixelColor(27,Green);
    strip.setPixelColor(28,Green);
    strip.setPixelColor(29,Green);
  }
  strip.show();
  strip.clear();
}
void pulse(){
    for (int i = 0; i < N_LEDS+5; i++){
    strip.setPixelColor(i, Red);
    strip.setPixelColor(i-1,RedD0);
    strip.setPixelColor(i-2,RedD1);
    strip.setPixelColor(i-3,RedD2);
    strip.setPixelColor(i-4,RedD3);
    //strip.setPixelColor(i-5,RedD4);
    //strip.setPixelColor(i-6,RedD5);
    //strip.setPixelColor(i-7,RedD6); 
    strip.show();
    strip.clear();
    delay(5);
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
