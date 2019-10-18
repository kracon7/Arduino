/* 
 * rosserial::std_msgs::Float64 Test
 * Receives a Float64 input, subtracts 1.0, and publishes it
 */
// On keyboard, v = 118, b = 98, n = 110
// a = 97 z = 122

#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 115;    // variable to store the servo position

//the relays connect to
int IN1 = 4;
int IN2 = 5;
#define ON 0
#define OFF 1
#define Relay_1 4
#define Relay_2 5

ros::NodeHandle nh;

float x; 
int flag_0 = 0;   // two flags as indicator of relay status
int flag_1 = 0;
int flag_servo = 0; // control parameter for servo, 
                    // 0 neutral; 1 add 1 degree; -1 decrease 1 degree

std_msgs::Int32 test;
std_msgs::Int32 servo_angle;

ros::Publisher p("my_topic", &test);
ros::Publisher servo_pub("servo_angle", &servo_angle);

unsigned long int timeToToggle = 0;

void messageCb( const std_msgs::Int32& msg){
  test.data = msg.data;
  x = msg.data;
  if(x == 97)               // keyboard a 
    flag_0 = 1;
  else if(x == 98)          // keyboard b
    flag_0 = 0;
  else if(x == 99)          // keyboard c
    flag_1 = 1;
  else if(x == 100)         // keyboard d
    flag_1 = 0;
  else if(x == 101)         // keyboard e
    flag_servo = 1;
  else if(x == 102)         // keyboard f
    flag_servo = -1;
  p.publish(&test);
  servo_pub.publish(&servo_angle);
}



ros::Subscriber<std_msgs::Int32> s("keyboard_pub", &messageCb);


void setup()
{
  digitalWrite(IN1, OFF);
  digitalWrite(IN2, OFF);
  pinMode(Relay_1, OUTPUT);
  pinMode(Relay_2, OUTPUT);
  nh.initNode();
  nh.advertise(p);
  nh.advertise(servo_pub);
  nh.subscribe(s);
  myservo.attach(9);
  myservo.write(pos);
}

void loop()
{
  unsigned long int currentTime = millis(); 
  
  if(flag_0 == 1)
    digitalWrite(IN1, ON);
  else if(flag_0 == 0)
    digitalWrite(IN1, OFF);

  if(flag_1 == 1)
    digitalWrite(IN2, ON);
  else if(flag_1 == 0)
    digitalWrite(IN2, OFF);

  if(flag_servo == 1){
    pos++;
    myservo.write(pos);
    flag_servo = 0;
  }
  else if(flag_servo == -1){
    pos--;
    myservo.write(pos);
    flag_servo = 0;
  }

  servo_angle.data = pos;

//  if(currentTime > timeToToggle)
//  {
//    digitalWrite(IN1, OFF);
//    digitalWrite(IN2, OFF);
//  }
  nh.spinOnce();
}
