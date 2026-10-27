#ifndef ROBOT_MOTOR_DRIVER_H
#define ROBOT_MOTOR_DRIVER_H

#define M_PI 3.14159265358979323846
#define d2r (M_PI /180.0)
#define pulse_per_rotation 2000

#define LEFT 0
#define RIGHT 1

#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <tf2/LinearMath/Quaternion.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/JointState.h>

#include <thread>
#include <std_msgs/Float64.h>
#include <std_msgs/Int32.h>
#include <string>

using namespace std;

typedef struct
{
  int encoder_pulse[2];
}MOTORmsg;


class robot_motor_driver
{
  public:
    robot_motor_driver();
    ~robot_motor_driver();

    void vel_callback(const geometry_msgs::Twist::ConstPtr& cmd);
    void ahrs_yaw_data_callback(const std_msgs::Float64::ConstPtr& ahrs_msg);
    void receive_data();
    void send_motor_command(double linear_vel, double angular_vel);
    void read_encoder();
    void parse_encoder_data(MOTORmsg &msg_in);

  public:
    ros::NodeHandle nh;
    ros::Publisher vel_pub_;
    ros::Publisher odom_pub;
    ros::Subscriber d_out_sub;
    ros::Subscriber vel_sub;

    ros::Publisher joint_states_pub;
    ros::Publisher left_encoder_pub;
    ros::Publisher right_encoder_pub;

    ros::Subscriber ahrs_sub;
    ros::Time current_time;
    ros::Time last_time;

    double tread;
    double wheel_radius;
    double gear_ratio;

    double max_vel;

    double ahrs_yaw;

   int last_pulse[2];

    double left_prev_vel;
    double right_prev_vel;

    nav_msgs::Odometry odom;
    geometry_msgs::TransformStamped odom_tf;
    tf::TransformBroadcaster tf_broadcaster;
    sensor_msgs::JointState joint_states;
    string odom_header_frame_id;
    string odom_child_frame_id;
    double odom_pose[3];
    double odom_vel[3];
    std::string joint_states_name[2];
    double last_velocity[2];
    double last_position[2];
    double update_time_out;

    std_msgs::String result;

    double linear_tmp;
    double angular_tmp;

    double left_prev_pulse;
    double right_prev_pulse;

    double left_odom_pulse_data_prev;
    double right_odom_pulse_data_prev;

    MOTORmsg motor_data;
};

#endif // ROBOT_MOTOR_DRIVER_H

