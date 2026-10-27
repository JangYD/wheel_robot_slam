#include "robot_keyboard_teleop.h"
#include <map>
#include <vector>

std::map<char, std::vector<float>>MoveCmd
{
  {'w', {1,0,0,0}},
  {'s', {-1,0,0,0}},
  {'a', {0,0,0,1}},
  {'d', {0,0,0,-1}},

  {'W', {1,0,0,0}},
  {'S', {-1,0,0,0}},
  {'A', {0,0,0,1}},
  {'D', {0,0,0,-1}}
};

static const char* msg = R"(

    Moving Command :
          w
    a     s     d

    CTRL-C quit
                  )";

robot_keyboard_teleop::robot_keyboard_teleop() :
  linear_(0.5), angular_(1.0), l_scale(2.0), a_scale(2.0), key(' '), x(0), y(0), z(0), th(0)
{
  nh_.param("scale_angular", a_scale, a_scale);
  nh_.param("scale_linearr", l_scale, l_scale);


}

int robot_keyboard_teleop::keyloop()
{
  int ch;
  struct termios oldt;
  struct termios newt;

  // Store old settings, and copy to new settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  // Make required changes and apply the settings
  newt.c_lflag &= ~(ICANON | ECHO);
  newt.c_iflag |= IGNBRK;
  newt.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
  newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
  newt.c_cc[VMIN] = 1;
  newt.c_cc[VTIME] = 0;
  tcsetattr(fileno(stdin), TCSANOW, &newt);

  // Get the current character
  ch = getchar();

  // Reapply old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

int main(int argc, char **argv)
{
  ros::init(argc,argv, "robot_keyboard_teleop");
  ros::NodeHandle nh;
  robot_keyboard_teleop key_teleop;
  key_teleop.twist_pub = key_teleop.nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

  geometry_msgs::Twist twist;

  ROS_INFO("%s/n" , msg);

  ROS_INFO_STREAM("linear : " << key_teleop.linear_ << " angular : " <<  key_teleop.angular_ );

  

  while(ros::ok())
  {
    key_teleop.key = key_teleop.keyloop();
    if(MoveCmd.count(key_teleop.key) == 1)
    {
      key_teleop.x = MoveCmd[key_teleop.key][0];
      key_teleop.y = MoveCmd[key_teleop.key][1];
      key_teleop.z = MoveCmd[key_teleop.key][2];
      key_teleop.th = MoveCmd[key_teleop.key][3];

     ROS_INFO_STREAM("linear : " << key_teleop.linear_ << " angular : " <<  key_teleop.angular_  << " key : " << key_teleop.key);
    }
    else
    {
      key_teleop.x = 0;
      key_teleop.y = 0;
      key_teleop.z = 0;
      key_teleop.th = 0;
    }

    if(key_teleop.key == '\x03')
    {
      break;
    }
    twist.linear.x = key_teleop.x;
    twist.linear.y = key_teleop.y;
    twist.linear.z = key_teleop.z;

    twist.angular.x = 0;
    twist.angular.y = 0;
    twist.angular.z = key_teleop.th;
    key_teleop.twist_pub.publish(twist);
    ros::spinOnce();
  }
  return 0;
}

