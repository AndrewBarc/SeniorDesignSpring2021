#include <Adafruit_NeoPixel.h>
     
    #define PIN      6
    #define N_LEDS 30
     
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
    uint32_t Red = strip.Color(255,0,0);
    uint32_t Orange = strip.Color(255,128,0);
    uint32_t Yellow = strip.Color(255,255,0);
    uint32_t Green = strip.Color(0,255,0);
    uint32_t Blue = strip.Color(0,0,255);
    uint32_t Purple = strip.Color(130,0,255);
	uint32_t White = strip.Color(255,255,255);

	uint32_t RedD0 = strip.ColorHSV(0,255,224);
	uint32_t RedD1 = strip.ColorHSV(0,255,192);
	uint32_t RedD2 = strip.ColorHSV(0,255,160);
	uint32_t RedD3 = strip.ColorHSV(0,255,128);
	uint32_t RedD4 = strip.ColorHSV(0,255,96);
	uint32_t RedD5 = strip.ColorHSV(0,255,64);
	uint32_t RedD6 = strip.ColorHSV(0,255,32);


	int r = 255;
	int g = 0;
	int b = 0;
    void setup() {
      strip.begin();
      strip.show();
      strip.setBrightness(150);
    }
     
    void loop() {
	//rainbowCascade();
    //blinking();

    pulse();
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
    delay(250);
  }
}

void rainbowCascade(){
	for (int i = 0; i <= N_LEDS; i++){
      	strip.setPixelColor(i,r,g,b);
        strip.show();
        if(r == 255 && g != 255 && b == 0){//becoming yellow
          //g++;
          g = g+5;
        }
        else if(r != 0 && g == 255 && b == 0){//becoming Green
          //r = r - 1;
          r = r - 5;
        }
        else if(r == 0 && g == 255 && b != 255){//becoming L Blue
          //b++;
          b = b + 5;
        }
        else if(r == 0 && g != 0 && b == 255){//becoming D Blue
         // g = g - 1;
          g = g - 5;
        }
        else if(r != 255 && g == 0 && b == 255){//becoming bri Purple
          //r++;
          r = r + 5;
        }
        else if(r == 255 && g == 0 && b != 0){//becoming red
          //b = b - 1;
          b = b - 5;
        }
        else {
          //cry
        }
      }
}
void blinking() {
      strip.fill(Red,0,30);
      strip.show();
      delay(250);
      strip.clear();
      strip.fill(Orange,0,30);
      strip.show();
      delay(250);
      strip.clear();
      strip.fill(Yellow,0,30);
      strip.show();
      delay(250);
      strip.clear();
      strip.fill(Green,0,30);
      strip.show();
      delay(250);
      strip.clear();
      strip.fill(Blue,0,30);
      strip.show();
      delay(250);
      strip.clear();
      strip.fill(Purple,0,30);
      strip.show();
      delay(250);
      strip.clear();
}
    