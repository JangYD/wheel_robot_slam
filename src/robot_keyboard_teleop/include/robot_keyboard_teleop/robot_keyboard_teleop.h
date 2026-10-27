#ifndef ROBOT_KEYBOARD_TELEOP_H
#define ROBOT_KEYBOARD_TELEOP_H

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <termios.h>

class robot_keyboard_teleop
{
public:
  robot_keyboard_teleop();
  int keyloop();

public:
  ros::NodeHandle nh_;
  ros::Publisher twist_pub;
  float linear_;
  float angular_;
  float l_scale;
  float a_scale;
  char key;
  float x, y, z, th;





private:

};

#endif // ROBOT_KEYBOARD_TELEOP_H
