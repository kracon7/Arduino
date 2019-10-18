#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <ros.h>
#include <rosserial_arduino/Adc.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>


ros::NodeHandle nh;

std_msgs::Int16MultiArray adc_msg;
ros::Publisher p("hall_sensor", &adc_msg);
int temp;
long previousMillis = 0;        // will store last time LED was updated
long interval = 100;           // interval at which to blink (milliseconds)

char dim0_label[] = "adc";
void setup()
{ 
  pinMode(13, OUTPUT);
  nh.initNode();
  adc_msg.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension) * 2);
//  adc_msg.layout.dim_length = 1;
  adc_msg.layout.dim[0].label = dim0_label;
  adc_msg.layout.dim[0].size = 10;
  adc_msg.layout.dim[0].stride = 1*10;
  adc_msg.layout.data_offset = 0;
  adc_msg.data_length = 10;
  adc_msg.data = (int *)malloc(sizeof(int)*10);
  nh.advertise(p);
}

void loop()
{
//  adc_msg.data.clear;
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {

    previousMillis = currentMillis; 
    //for loop, pushing data in the size of the array
    for (int i = 0; i < 10; i++)
    {
      //assign array a random number between 0 and 255.
      temp = analogRead(i);
      adc_msg.data[i] = temp;
    }
   
    p.publish(&adc_msg);
    nh.spinOnce();
  }
  
  
}
