/* 
 * rosserial::std_msgs::Float64 Test
 * Receives a Float64 input, subtracts 1.0, and publishes it
 */

#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>


ros::NodeHandle nh;

float x; 
int y;



std_msgs::Int32 test;

ros::Publisher p("my_topic", &test);


void messageCb( const std_msgs::Int32& msg){
  test.data = msg.data;
  p.publish(&test);
}


ros::Subscriber<std_msgs::Int32> s("keyboard_pub", &messageCb);


void setup()
{
  nh.initNode();
  nh.advertise(p);
  nh.subscribe(s);
}

void loop()
{
  nh.spinOnce();
  delay(10);
}
