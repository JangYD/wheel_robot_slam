#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "geometry_msgs/Twist.h"

class JoyControl {
public:
    JoyControl() {
        joy_sub = nh.subscribe<sensor_msgs::Joy>("joy", 10, &JoyControl::joyCallback, this);
        twist_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
    }

    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy_msg) {
        geometry_msgs::Twist twist_cmd;
        if(joy_msg->axes[4] != 0 || joy_msg->axes[5] != 0)
        {
          twist_cmd.linear.x = joy_msg->axes[4];  // Left stick vertical axis for linear velocity
          twist_cmd.angular.z = joy_msg->axes[5]*(-1);  // Left stick horizontal axis for angular velocite

        }
        else if(joy_msg->buttons[0] != 0 || joy_msg->buttons[1] != 0 || joy_msg->buttons[2] != 0 || joy_msg->buttons[3] != 0 )
        {
          if(joy_msg->buttons[2] == 1)
          {
            twist_cmd.linear.x = joy_msg->buttons[2];
          }
          else if(joy_msg->buttons[1] == 1)
          {
            twist_cmd.linear.x = joy_msg->buttons[1]*(-1);
          }

          if(joy_msg->buttons[0] == 1)
          {
            twist_cmd.angular.z = joy_msg->buttons[0];
          }
          else if(joy_msg->buttons[3] == 1)
          {
            twist_cmd.angular.z = joy_msg->buttons[3] *(-1);
          }
        }
        ROS_INFO_STREAM("twist_cmd.linear.x : " <<twist_cmd.linear.x  <<  " twist_cmd.angular.z : " << twist_cmd.angular.z);

        twist_pub.publish(twist_cmd);
    }

    void spin() {
        ros::Rate rate(10);  
        while (ros::ok()) {
            ros::spinOnce();
            rate.sleep();
        }
    }

private:
    ros::NodeHandle nh;
    ros::Subscriber joy_sub;
    ros::Publisher twist_pub;
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "robot_joy_teleop");
    JoyControl joy_control;
    joy_control.spin();
    return 0;
}
