    #include <Adafruit_NeoPixel.h>
     
    #define PIN      27
    #define N_LEDS 30
     
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
      uint32_t Red = strip.Color(255,0,0);
      uint32_t Orange = strip.Color(255,128,0);
      uint32_t Yellow = strip.Color(255,255,0);
      uint32_t Green = strip.Color(0,255,0);
      uint32_t Blue = strip.Color(0,0,255);
      uint32_t Purple = strip.Color(130,0,255);
    void setup() {
      strip.begin();
      strip.show();
      strip.setBrightness(40);
    }
     
    void loop() {
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
