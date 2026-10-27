#ifndef MW_AHRSV2U_H
#define MW_AHRSV2U_H

#include <tuple>
#include <iostream>

#include "serial/serial.h"
#include "robot_mw_ahrsv2u/const.h"
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/MagneticField.h>
#include <sensor_msgs/Imu.h>
#include <tf/transform_broadcaster.h>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <time.h>

using namespace  std;

typedef struct
{
  double acc_value[3] = {0,};
  double gyr_value[3] = {0,};
  double deg_value[3] = {0,};
  double mag_value[3] = {0,};

}IMUMsg;

enum axis
{
  x = 0,
  y = 1,
  z = 2
};

class MW_AHRSv2U
{
public:
  MW_AHRSv2U();
  ~MW_AHRSv2U();
  void reset_imu();
  void speed_setup();
  void start_data_stream();
  void parse_ss_data(IMUMsg &msg_in);
  void pub_msg();
  void receive_data();
  tf2::Quaternion Euler2Quaternion(float roll, float pitch, float yaw);


private:

  int device_id = 0;

  char buffer[128];

  int ang_count;

  bool publish_tf = true;

  ros::NodeHandle nh;
  ros::Publisher imu_pub;
  ros::Publisher imu_data_raw_pub;
  ros::Publisher imu_data_pub;
  ros::Publisher imu_mag_pub;
  ros::Publisher imu_yaw_pub;
  ros::Publisher odom_pub;

  ros::Timer time_;

  sensor_msgs::Imu imu_msg;
  tf::TransformBroadcaster broadcaster_;
  string parent_frame_id_;
  string frame_id_;

  double linear_acceleration_stddev;
  double angular_velocity_stddev;
  double magnetic_field_stddev;
  double orientation_stddev;
  double ahrs_yaw;
  ros::Time current_time;
  ros::Time last_time;

  tf::TransformBroadcaster odom_broadcaster;

  IMUMsg imu_data;
};

#endif // MW_AHRSV2U_H
