#include <Adafruit_NeoPixel.h>
#define PIXEL_PIN 4
#define PIXEL_COUNT 1

const int FSR_line = A0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
}

void loop() {
//  // put your main code here, to run repeatedly:
//    int PixelColorBlue = strip.Color( 0, 0, 128);
//    int PixelColorRed = strip.Color( 80, 0, 4);
//    int PixelColorGold = strip.Color( 60, 150, 5);
//    
//    //Set first pixel to blue
//    strip.setPixelColor(0, PixelColorBlue);
//    strip.show();
//    delay(1000); //wait 1sec
//
//    //set second and third pixel to Gopher Gold! and Red, respectively
//    strip.setPixelColor(0, PixelColorRed);
//    strip.show();
//    delay(1000); 
//    
//    strip.setPixelColor(0, PixelColorGold);
//    strip.show();
//    delay(1000); //wait 1sec}

    int V = int(double(analogRead(FSR_line)) * 255 / 1000) ; // scale FSR reading to 0-255
    int PixelColorFSR = strip.Color(30, V/1.2 +20,V/2.5+30);
    strip.setPixelColor(0, PixelColorFSR);
    strip.show();
    delay(50); //wait 1sec
    
    Serial.println("V is :");
    Serial.println(V);
    
    Serial.println("FSR is :");
    Serial.println(analogRead(FSR_line));
}
