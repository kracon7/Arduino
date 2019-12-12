/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE

 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.
 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"

#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT_1 2
#define CLK_1  3
#define DOUT_2 4
#define CLK_2  5
#define DOUT_3 6
#define CLK_3  7


HX711 scale_1;
HX711 scale_2;
HX711 scale_3;


void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

  scale_1.begin(DOUT_1, CLK_1);
  scale_2.begin(DOUT_2, CLK_2);
  scale_3.begin(DOUT_3, CLK_3);
  
  scale_1.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_1.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  
  scale_2.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_2.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  
  scale_3.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_3.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
}

void loop() {
  Serial.print("Reading: ");
  Serial.print(scale_1.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();

  
  Serial.print(scale_2.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();

  
  Serial.print(scale_3.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
}
