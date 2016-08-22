#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Float64.h"
#include <stdio.h>
#include <math.h>

#define MIN_HEADING_ERROR 2.5 // measured in degrees

// Global variables
float currentHeading = 0.;

float targetHeading = 0.;
float targetLinearVel = 0.;

bool first = true;
bool waitingForCommand = true;

void CurrentHeadingCallback(const std_msgs::Float64::ConstPtr& msg){
  currentHeading = msg->data;
  if(first){
    targetHeading = currentHeading;
    first = false;
  }
}

void TargetHeadingCallback(const std_msgs::Float64::ConstPtr& msg){
  targetHeading = msg->data;
  waitingForCommand = false;
}

void TargetLinearVelCallback(const std_msgs::Float64::ConstPtr& msg){
  targetLinearVel = msg->data;
}

int main(int argc, char **argv){

  ros::init(argc, argv, "follow_heading");

  ros::NodeHandle n;
 
  ros::Publisher twist_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

  ros::Subscriber currentHeadingSub = n.subscribe("currentHeading", 1000, CurrentHeadingCallback);
  ros::Subscriber targetHeadingSub  = n.subscribe("targetHeading", 1000, TargetHeadingCallback);
  ros::Subscriber targetLinearVelSub = n.subscribe("targetLinearVelocity", 1000, TargetLinearVelCallback);
  
  ros::Rate loop_rate(25);

  bool keepGoing = true;
  
  while (ros::ok()){
    // Message sent to cmd_vel
    geometry_msgs::Twist msg;

    // Variables which store components of twist msg
    float v = 0.;
    float w = 0.;
    float t = -1.;   

    if(waitingForCommand){
       v = w = 0.;
    }
    else{
      // Calculate in degrees first
      float headingError = -1.*(targetHeading - currentHeading) ;
      
      if(fabs(headingError) > 180.){
	headingError = 180. - headingError;
      }
    
      // Test if error below threshold (degrees)
      if( fabs(headingError) <  MIN_HEADING_ERROR){
	headingError = 0;
	v = 0.1*targetLinearVel;
      }

      // Now convert to radians (per second)
      headingError = headingError * M_PI/180.;
    
      w = headingError;
      //v = 0.01*targetLinearVel;
    }
        
    msg.linear.x = v;
    msg.angular.z = w;
    msg.angular.y = t;
    
    twist_pub.publish(msg);

    ros::spinOnce();

    loop_rate.sleep();

  }


  return 0;
}
