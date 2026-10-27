#include "robot_mw_ahrsv2u/const.h"
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/MagneticField.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/String.h>

#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>
#include "robot_mw_ahrsv2u/mw_ahrsv2u.h"

#include <sstream>
#include <thread>


using namespace std;
serial::Serial ser;

MW_AHRSv2U::MW_AHRSv2U()
{
  ser.setPort("/dev/AHRS");
  ser.setBaudrate(115200);
  serial::Timeout to = serial::Timeout::simpleTimeout(1000);
  ser.setTimeout(to);
  ser.open();
  
  nh.param<std::string>("frame_id", frame_id_, "imu_link");
  nh.param<std::string>("parent_frame_id", parent_frame_id_, "base_link");
  imu_pub = nh.advertise<sensor_msgs::Imu>("imu", 10);
  imu_data_raw_pub = nh.advertise<sensor_msgs::Imu>("imu/data_raw", 10);
  imu_data_pub = nh.advertise<sensor_msgs::Imu>("imu/data", 10);
  imu_mag_pub = nh.advertise<sensor_msgs::MagneticField>("imu/mag", 10);
  imu_yaw_pub = nh.advertise<std_msgs::Float64>("imu/yaw", 10);
  odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 5);
}

MW_AHRSv2U::~MW_AHRSv2U()
{
  ser.close();
}

void MW_AHRSv2U::receive_data()
{
  std_msgs::String result;

  result.data = ser.read(ser.available());
  ROS_INFO_STREAM("read : " << result.data);
}

void MW_AHRSv2U::reset_imu()
{
  ser.write(reset_cmd);
  ROS_INFO_STREAM("reset imu done");
}

void MW_AHRSv2U::speed_setup()
{
  ser.write(speed_cmd);
  ROS_INFO_STREAM("speed setup done");
}

void MW_AHRSv2U::start_data_stream()
{
  ser.write(ros_data_cmd);
}


void MW_AHRSv2U::parse_ss_data(IMUMsg &msg_in)
{
  string line = ser.readline();
  stringstream ss(line);

  if(int(line[0]) < 97)
  {
    char *rest;
    char *token;
    char *ptr = const_cast<char*>(line.c_str());

    ang_count = 0;

    while((token = strtok_r(ptr, " ", &rest)))
    {
      ang_count++;
      if(ang_count == 1)
      {
        msg_in.acc_value[axis::x] = atof(token) / 1000.0;
      }
      else if (ang_count == 2)
      {
        msg_in.acc_value[axis::y] = atof(token) / 1000.0;
      }
      else if (ang_count == 3)
      {
        msg_in.acc_value[axis::z] = atof(token) / 1000.0;
      }
      else if (ang_count == 4)
      {
        msg_in.gyr_value[axis::x] = atof(token) / 10.0;
      }
      else if (ang_count == 5)
      {
        msg_in.gyr_value[axis::y] = atof(token) / 10.0;
      }
      else if (ang_count == 6)
      {
        msg_in.gyr_value[axis::z] = atof(token) / 10.0;
      }
      else if (ang_count == 7)
      {
        msg_in.deg_value[axis::x] = atof(token) / 100.0;
      }
      else if (ang_count == 8)
      {
        msg_in.deg_value[axis::y] = atof(token) / 100.0;
      }
      else if (ang_count == 9)
      {
        msg_in.deg_value[axis::z] = atof(token) / 100.0;
      }
      else if (ang_count == 10)
      {
        msg_in.mag_value[axis::x] = atof(token) / 10.0;
      }
      else if (ang_count == 11)
      {
        msg_in.mag_value[axis::y] = atof(token) / 10.0;
      }
      else if (ang_count == 12)
      {
        msg_in.mag_value[axis::z] = atof(token) / 10.0;
      }
      ptr = rest;
    }
  }
}

void MW_AHRSv2U::pub_msg()
{
  parse_ss_data(imu_data);

  auto imu_data_raw_msg = sensor_msgs::Imu();
  auto imu_data_msg = sensor_msgs::Imu();
  auto imu_magnetic_msg = sensor_msgs::MagneticField();
  auto imu_yaw_msg = std_msgs::Float64();


  double linear_acceleration_cov = linear_acceleration_stddev * linear_acceleration_stddev;
  double angular_velocity_cov = angular_velocity_stddev * angular_velocity_stddev;
  double magnetic_field_cov = magnetic_field_stddev * magnetic_field_stddev;
  double orientation_cov = orientation_stddev * orientation_stddev;

  imu_data_raw_msg.linear_acceleration_covariance[0] =
  imu_data_raw_msg.linear_acceleration_covariance[4] =
  imu_data_raw_msg.linear_acceleration_covariance[8] =
  imu_data_msg.linear_acceleration_covariance[0] =
  imu_data_msg.linear_acceleration_covariance[4] =
  imu_data_msg.linear_acceleration_covariance[8] =
  linear_acceleration_cov;

  imu_data_raw_msg.angular_velocity_covariance[0] =
  imu_data_raw_msg.angular_velocity_covariance[4] =
  imu_data_raw_msg.angular_velocity_covariance[8] =
  imu_data_msg.angular_velocity_covariance[0] =
  imu_data_msg.angular_velocity_covariance[4] =
  imu_data_msg.angular_velocity_covariance[8] =
  angular_velocity_cov;

  imu_data_msg.orientation_covariance[0] =
  imu_data_msg.orientation_covariance[4] =
  imu_data_msg.orientation_covariance[8] =
  orientation_cov;

  imu_magnetic_msg.magnetic_field_covariance[0] =
  imu_magnetic_msg.magnetic_field_covariance[4] =
  imu_magnetic_msg.magnetic_field_covariance[8] =
  magnetic_field_cov;

  double roll, pitch, yaw;

  roll = imu_data.deg_value[axis::x] * d2r;
  pitch = imu_data.mag_value[axis::y] * d2r;
  yaw = imu_data.deg_value[axis::z] * d2r;

  tf2::Quaternion tf_orientation = Euler2Quaternion(roll, pitch, yaw);

  current_time = ros::Time::now();

  imu_data_raw_msg.header.stamp = imu_data_msg.header.stamp =
      imu_magnetic_msg.header.stamp = current_time;
        
  imu_data_raw_msg.header.frame_id = imu_data_msg.header.frame_id =
      imu_magnetic_msg.header.frame_id = imu_msg.header.frame_id = "lmu_link";

  imu_data_msg.orientation.x = tf_orientation.x();
  imu_data_msg.orientation.y = tf_orientation.y();
  imu_data_msg.orientation.z = tf_orientation.z();
  imu_data_msg.orientation.w = tf_orientation.w();

  imu_data_raw_msg.linear_acceleration.x = imu_data_msg.linear_acceleration.x =
      imu_data.acc_value[axis::x] * g2a;
  imu_data_raw_msg.linear_acceleration.y = imu_data_msg.linear_acceleration.y =
      imu_data.acc_value[axis::y] * g2a;
  imu_data_raw_msg.linear_acceleration.z = imu_data_msg.linear_acceleration.z =
      imu_data.acc_value[axis::z] * g2a;

  imu_data_raw_msg.angular_velocity.x = imu_data_msg.angular_velocity.x =
      imu_data.gyr_value[axis::x] * d2r;
  imu_data_raw_msg.angular_velocity.y = imu_data_msg.angular_velocity.y =
      imu_data.gyr_value[axis::y] * d2r;
  imu_data_raw_msg.angular_velocity.z = imu_data_msg.angular_velocity.z =
      imu_data.gyr_value[axis::z] * d2r;

  imu_magnetic_msg.magnetic_field.x = imu_data.mag_value[axis::x] / ut2t;
  imu_magnetic_msg.magnetic_field.y = imu_data.mag_value[axis::y] / ut2t;
  imu_magnetic_msg.magnetic_field.z = imu_data.mag_value[axis::z] / ut2t;

  imu_yaw_msg.data = imu_data.deg_value[axis::z];

  imu_data_raw_pub.publish(std::move(imu_data_raw_msg));
  imu_data_pub.publish(std::move(imu_data_msg));
  imu_mag_pub.publish(std::move(imu_magnetic_msg));
  imu_yaw_pub.publish(std::move(imu_yaw_msg));

  imu_msg.header.frame_id = "imu_link";
  imu_msg.header.stamp = ros::Time::now();
  imu_pub.publish(imu_msg);

  if (publish_tf)
  {
    geometry_msgs::TransformStamped tf;
    tf.header.stamp = current_time;
    tf.header.frame_id = parent_frame_id_;
    tf.child_frame_id = frame_id_;
    tf.transform.translation.x = 0;
    tf.transform.translation.y = 0.0;
    tf.transform.translation.z = 0.063;
    tf.transform.rotation = imu_data_msg.orientation;

    odom_broadcaster.sendTransform(tf);
  }
}

tf2::Quaternion MW_AHRSv2U::Euler2Quaternion(float roll, float pitch, float yaw)
{
  float qx = (sin(roll / 2) * cos(pitch / 2) * cos(yaw / 2)) -
             (cos(roll / 2) * sin(pitch / 2) * sin(yaw / 2));
  float qy = (cos(roll / 2) * sin(pitch / 2) * cos(yaw / 2)) +
             (sin(roll / 2) * cos(pitch / 2) * sin(yaw / 2));
  float qz = (cos(roll / 2) * cos(pitch / 2) * sin(yaw / 2)) -
             (sin(roll / 2) * sin(pitch / 2) * cos(yaw / 2));
  float qw = (cos(roll / 2) * cos(pitch / 2) * cos(yaw / 2)) +
             (sin(roll / 2) * sin(pitch / 2) * sin(yaw / 2));

  tf2::Quaternion q(qx, qy, qz, qw);
  return q;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv,"robot_mw_ahrsv2u");

    MW_AHRSv2U ahrs_obj;
    ros::Rate rate(10);
    ahrs_obj.reset_imu();
    ahrs_obj.speed_setup();
    ahrs_obj.start_data_stream();

    while(ros::ok())
    {
      ahrs_obj.pub_msg();
      rate.sleep();
    }
    

    return 0;
}
