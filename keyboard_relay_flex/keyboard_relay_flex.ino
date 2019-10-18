/* 
 * rosserial::std_msgs::Float64 Test
 * Receives a Float64 input, subtracts 1.0, and publishes it
 */
// On keyboard, v = 118, b = 98, n = 110

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <rosserial_arduino/Adc.h>
#include <PololuLedStrip.h>
PololuLedStrip<12> ledStrip;
#define LED_COUNT 60
rgb_color colors[LED_COUNT];


//the relays connect to
int IN1 = 4;
int IN2 = 5;
#define ON 0
#define OFF 1
#define Relay_1 4
#define Relay_2 5

// To measure position, the sense line must be connected to a
// pin capable of reading analog voltages.  For pressure,
// the sense line and drive line 2 must be connected to pins
// capable of reading analog voltages.  See the FSLP guide for
// more information.
const int fslpSenseLine = A2;
const int fslpDriveLine1 = 8;
const int fslpDriveLine2 = A3;
const int fslpBotR0 = 9;

ros::NodeHandle nh;

rosserial_arduino::Adc adc_msg;
ros::Publisher p("adc", &adc_msg);

float x; 
// two flags as indicator of relay status
int flag_0 = 0;
int flag_1 = 0;

std_msgs::Int32 test;


unsigned long int timeToToggle = 0;

void messageCb( const std_msgs::Int32& msg){
  x = msg.data;
  if(x == 118)
  {
    flag_0 = 1;
    flag_1 = 0;
  }
  else if(x == 98)
  {
    flag_0 = 0;
    flag_1 = 1;
  }
  else if(x == 110)
  {
    flag_0 = 0;
    flag_1 = 0;
  }
}

ros::Subscriber<std_msgs::Int32> s("keyboard_pub", &messageCb);


void setup()
{
  Serial.begin(9600);
  digitalWrite(IN1, OFF);
  digitalWrite(IN2, OFF);
  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  nh.initNode();
  nh.advertise(p);
  nh.subscribe(s);
}

long adc_timer;

void loop()
{
  int pressure, position;

  pressure = fslpGetPressure();

  if (pressure == 0)
  {
    // There is no detectable pressure, so measuring
    // the position does not make sense.
    position = 0;
  }
  else
  {
    position = fslpGetPosition();  // Raw reading, from 0 to 1023.
  }

  char report[80];
  sprintf(report, "pressure: %5d   position: %5d\n",
    pressure, position);
  Serial.println(report);

  // Scale the position reading to be from 0 to the
  // number of LEDs.
  int adjustedPosition = (int32_t)position * LED_COUNT / 1000;

  // Scale the pressure reading to be from 0 to 255.
  int adjustedPressure = pressure * 8 / 10;
  if (adjustedPressure > 255)
  {
    adjustedPressure = 255;
  }

  for(int i = 0; i < LED_COUNT; i++)
  {
    if (i < adjustedPosition)
    {
      // Light up the LEDs with indices lower than the adjustedLocation.
      // Set the color of the LEDs based on the adjustedPressure
      colors[i] = (rgb_color){ adjustedPressure, 0, 255 - adjustedPressure };
    }
    else
    {
      // Turn off the other LEDs.
      colors[i] = (rgb_color){ 0, 0, 0 };
    }
  }

  // Update the LED strip with values from the above for loop.
  ledStrip.write(colors, LED_COUNT);

  delay(400);


  
  unsigned long int currentTime = millis(); 
  if(flag_0 == 1)
  {
//    timeToToggle = currentTime + 200;
    digitalWrite(IN1, ON);
  }
  else
  {
    digitalWrite(IN1, OFF);
  }

  if(flag_1 == 1)
  {
//    timeToToggle = currentTime + 200;
    digitalWrite(IN2, ON);
  }
  else
  {
    digitalWrite(IN2, OFF);
  }

//  adc_msg.adc0 = averageAnalog(0);
//  adc_msg.adc1 = averageAnalog(1);
// 
//  p.publish(&adc_msg);

  nh.spinOnce();
}

int averageAnalog(int pin){
  int v=0;
  for(int i=0; i<4; i++) v+= analogRead(pin);
  return v/4;
}

// This function follows the steps described in the FSLP
// integration guide to measure the position of a force on the
// sensor.  The return value of this function is proportional to
// the physical distance from drive line 2, and it is between
// 0 and 1023.  This function does not give meaningful results
// if fslpGetPressure is returning 0.
int fslpGetPosition()
{
  // Step 1 - Clear the charge on the sensor.
  pinMode(fslpSenseLine, OUTPUT);
  digitalWrite(fslpSenseLine, LOW);

  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, LOW);

  pinMode(fslpDriveLine2, OUTPUT);
  digitalWrite(fslpDriveLine2, LOW);

  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);

  // Step 2 - Set up appropriate drive line voltages.
  digitalWrite(fslpDriveLine1, HIGH);
  pinMode(fslpBotR0, INPUT);
  pinMode(fslpSenseLine, INPUT);

  // Step 3 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 4 - Take the measurement.
  analogReset();
  return analogRead(fslpSenseLine);
}

// This function follows the steps described in the FSLP
// integration guide to measure the pressure on the sensor.
// The value returned is usually between 0 (no pressure)
// and 500 (very high pressure), but could be as high as
// 32736.
int fslpGetPressure()
{
  // Step 1 - Set up the appropriate drive line voltages.
  pinMode(fslpDriveLine1, OUTPUT);
  digitalWrite(fslpDriveLine1, HIGH);

  pinMode(fslpBotR0, OUTPUT);
  digitalWrite(fslpBotR0, LOW);

  pinMode(fslpSenseLine, INPUT);

  pinMode(fslpDriveLine2, INPUT);

  // Step 2 - Wait for the voltage to stabilize.
  delayMicroseconds(10);

  // Step 3 - Take two measurements.
  analogReset();
  int v1 = analogRead(fslpDriveLine2);
  analogReset();
  int v2 = analogRead(fslpSenseLine);

  char report[80];
  sprintf(report, "v1 is: %5d   v2 is: %5d\n", v1, v2);
  Serial.print(report);
  Serial.println("\n");

  // Step 4 - Calculate the pressure.
  // Detailed information about this formula can be found in the
  // FSLP Integration Guide.
  if (v1 == v2)
  {
    // Avoid dividing by zero, and return maximum reading.
    return 32 * 1023;
  }
  return 32 * v2 / (v1 - v2);
}

// Performs an ADC reading on the internal GND channel in order
// to clear any voltage that might be leftover on the ADC.
// Only works on AVR boards and silently fails on others.
void analogReset()
{
#if defined(ADMUX)
#if defined(ADCSRB) && defined(MUX5)
    // Code for the ATmega2560 and ATmega32U4
    ADCSRB |= (1 << MUX5);
#endif
    ADMUX = 0x1F;

    // Start the conversion and wait for it to finish.
    ADCSRA |= (1 << ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
#endif
}
