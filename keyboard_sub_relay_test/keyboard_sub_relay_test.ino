/* 
 * rosserial::std_msgs::Float64 Test
 * Receives a Float64 input, subtracts 1.0, and publishes it
 */
// On keyboard, v = 118, b = 98, n = 110
// 'd': left pump
// 'c': left deflate
// 'b': right pump
// 'a': right deflate
#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>

//the relays connect to
int IN1 = 32;
int IN2 = 33;
int IN3 = 34;
int IN4 = 35;
#define ON 0
#define OFF 1
#define Relay_1 32   // rv
#define Relay_2 33   // rp
#define Relay_3 34   // lv
#define Relay_4 35   // lp

ros::NodeHandle nh;

float x; 
// four flags as indicator of relay status
int flag_0 = 0;
int flag_1 = 0;
int flag_2 = 0;
int flag_3 = 0;


std_msgs::Int32 test;

ros::Publisher p("my_topic", &test);

unsigned long int timeToToggle = 0;

void messageCb( const std_msgs::Int32& msg){
  test.data = msg.data;
  x = msg.data;
  if(x == 97)             // keyboard a
  {
    flag_0 = 1;
    flag_1 = 0;
    flag_2 = 0;
    flag_3 = 0;
    
  }
  else if(x == 98)        // keyboard b
  {
    flag_0 = 0;
    flag_1 = 1;
    flag_2 = 0;
    flag_3 = 0;
    
  }
  else if(x == 99)        // keyboard c
  {
    flag_0 = 0;
    flag_1 = 0;
    flag_2 = 1;
    flag_3 = 0;
  }
  else if(x == 100)        // keyboard d
  {
    flag_0 = 0;
    flag_1 = 0;
    flag_2 = 0;
    flag_3 = 1;
  }
  else if(x == 101)        // keyboard e
  {
    flag_0 = 0;
    flag_1 = 0;
    flag_2 = 0;
    flag_3 = 0;
  }
  p.publish(&test);
}


// step the pump, turn it on for [interval] 
// amount of milliseconds and then turn it off
void pump_step(int pin, unsigned int interval){
  digitalWrite(pin, ON);
  delayMicroseconds(interval);
  digitalWrite(pin, OFF);
}


ros::Subscriber<std_msgs::Int32> s("keyboard_pub", &messageCb);


void setup()
{
  digitalWrite(IN1, OFF);
  digitalWrite(IN2, OFF);
  digitalWrite(IN3, OFF);
  digitalWrite(IN4, OFF);
  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  pinMode(Relay_3, OUTPUT);
  pinMode(Relay_4, OUTPUT);
  nh.initNode();
  nh.advertise(p);
  nh.subscribe(s);
}

void loop()
{
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

  if(flag_2 == 1)
  {
//    timeToToggle = currentTime + 200;
    digitalWrite(IN3, ON);
  }
  else
  {
    digitalWrite(IN3, OFF);
  }

  if(flag_3 == 1)
  {
//    timeToToggle = currentTime + 200;
    digitalWrite(IN4, ON);
  }
  else
  {
    digitalWrite(IN4, OFF);
  }

//  if(currentTime > timeToToggle)
//  {
//    digitalWrite(IN1, OFF);
//    digitalWrite(IN2, OFF);
//  }
  nh.spinOnce();
}
