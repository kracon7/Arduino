/*
   rosserial::std_msgs::Float64 Test
   Receives a Float64 input, subtracts 1.0, and publishes it
*/
// On keyboard, v = 118, b = 98, n = 110
// 'b': left pump
// 'a': left deflate
// 'c': right pump
// 'd': right deflate

#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Char.h>
#include <Servo.h>

//the relays connect to
int IN1 = 32;
int IN2 = 33;
int IN3 = 34;
int IN4 = 35;
#define ON 0
#define OFF 1
#define Pump_R 32
#define Valve_R 33
#define Pump_L 34
#define Valve_L 35

ros::NodeHandle nh_2;

float Dist;
float thresh = 10;  // threshold to tell when the gripper stop
char Mode = 's';
int pos_center = 110;
int pos = pos_center;
// four flags as indicator of relay status
int flag_0 = 0;
int flag_1 = 0;
int flag_2 = 0;
int flag_3 = 0;

Servo myservo;  // create servo object to control a servo
std_msgs::Int32 test;

ros::Publisher p("my_topic", &test);

unsigned long int timeToToggle = 0;

void messageCb( const std_msgs::Int32& msg) {
  Dist = msg.data;
}

void modeCb(const std_msgs::Int32& msg) {
  int data = msg.data;
  Mode = char('a'+ (data - 97));
  test.data = msg.data;
  p.publish(&test);
//  char report[80];
//  sprintf(report, "pressure:   position:\n");
//  Serial.println(report);

}

void resetAll() {
  myservo.write(pos_center);
  digitalWrite(Pump_R, OFF);
  digitalWrite(Pump_L, OFF);
  digitalWrite(Valve_R, ON);
  digitalWrite(Valve_L, ON);
  delay(6000);
}

// step the pump, turn it on for [interval]
// amount of microseconds and then turn it off
void pump_step(int pin, unsigned int interval) { 
  nh_2.loginfo("Pump one step");
  digitalWrite(pin, ON);
  delay(interval);
  digitalWrite(pin, OFF);
  delay(80);
}


ros::Subscriber<std_msgs::Int32> s("D_pub", &messageCb);
ros::Subscriber<std_msgs::Int32> mode_sub("keyboard_pub", &modeCb);

void setup()
{
  digitalWrite(Pump_R, OFF);
  digitalWrite(Pump_L, OFF);
  digitalWrite(Valve_R, OFF);
  digitalWrite(Valve_L, OFF);
  pinMode(Pump_R, OUTPUT);
  pinMode(Pump_L, OUTPUT);
  pinMode(Valve_R, OUTPUT);
  pinMode(Valve_L, OUTPUT);
  nh_2.initNode();
  nh_2.advertise(p);
  nh_2.subscribe(s);
  nh_2.subscribe(mode_sub);
  myservo.attach(9);
  myservo.write(pos_center);
  Mode == 's';
}

void loop()
{
  if (Mode == 'g') {
    nh_2.loginfo("Gripping mode");
    int T = 30;
    if (Dist < thresh) {
      pump_step(Pump_R, 0.8*T);
      pump_step(Pump_L, 30*T);
    }
    else if (Dist >= thresh) {
      Mode = 's';
    }
  }
  else if (Mode == 't') {
    nh_2.loginfo("Twisting mode");
    if (abs(pos - pos_center) < 200) {
      pos++;
      myservo.write(pos);
    }
    else if (abs(pos - pos_center) >= 200) {
      Mode = 's';
    }
  }
  else if (Mode == 's') {
    nh_2.loginfo("All units in stand-by");
    digitalWrite(Pump_R, OFF);
    digitalWrite(Pump_L, OFF);
    digitalWrite(Valve_R, OFF);
    digitalWrite(Valve_L, OFF);
  }
  else if (Mode == 'r') {
    nh_2.loginfo("Reset mode");
    resetAll();
    Mode = 's';
    nh_2.loginfo("Done Reset");
  }

  nh_2.spinOnce();
}
