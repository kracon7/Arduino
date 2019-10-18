/************************************************
 Name：_2_channel_relay
 Description: control the 2 channel relay module to ON or OFF
 Website: www.handsontec.com
 Email: techsupport@handsontec.com
*****************************************************/

#include <ros.h>
#include <std_msgs/Int32.h>

std_msgs::Int32 test;

ros::NodeHandle nh;
ros::Publisher p("my_topic", &test);

//the relays connect to
int IN1 = 4;
int IN2 = 5;
#define ON 0
#define OFF 1
#define Relay_1 4
#define Relay_2 5

void setup()
{
   digitalWrite(IN1, OFF);
   digitalWrite(IN2, OFF);
   pinMode(Relay_1, OUTPUT);
   pinMode(Relay_2, OUTPUT);
   nh.initNode();
   nh.advertise(p);
}


void loop() {
   digitalWrite(IN1, ON);
   digitalWrite(IN2, ON);
   delay(2000);//delay 2s
   digitalWrite(IN1, OFF);
   digitalWrite(IN2, OFF);
   delay(2000);//delay 2s
   test.data = 100;
   p.publish(&test);
   nh.spinOnce();
}
