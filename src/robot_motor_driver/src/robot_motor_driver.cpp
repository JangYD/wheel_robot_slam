#include "robot_motor_driver.h"

serial::Serial ser;
using namespace std;

robot_motor_driver::robot_motor_driver()
{
  ser.setPort("/dev/MW");
  ser.setBaudrate(115200);
  serial::Timeout to = serial::Timeout::simpleTimeout(10);
  ser.setTimeout(to);
  ser.open();

  vel_sub = nh.subscribe("cmd_vel", 1, &robot_motor_driver::vel_callback, this);
  ahrs_sub = nh.subscribe("/imu/yaw", 5, &robot_motor_driver::ahrs_yaw_data_callback, this);
  left_encoder_pub = nh.advertise<std_msgs::Int32>("left_encoder_pub", 1000);
  right_encoder_pub = nh.advertise<std_msgs::Int32>("right_encoder_pub", 1000);
}

robot_motor_driver::~robot_motor_driver()
{
  send_motor_command(0,0);
  ser.close();
}

void robot_motor_driver::ahrs_yaw_data_callback(const std_msgs::Float64::ConstPtr& ahrs_msg)
{
  ahrs_yaw = ahrs_msg->data;
}

void robot_motor_driver::vel_callback(const geometry_msgs::Twist::ConstPtr& cmd)
{
  last_time = ros::Time::now();
  linear_tmp = cmd->linear.x;
  angular_tmp = cmd->angular.z;
}

void robot_motor_driver::send_motor_command(double linear_vel, double angular_vel)
{
  std::string command_string = "mla=" + std::to_string(linear_vel) + "," +std::to_string(angular_vel) + "\r" + "\n";
  ser.write(command_string);
}

void robot_motor_driver::receive_data()
{
  while(!ser.available());
  result.data = ser.read(ser.available());
  ROS_INFO_STREAM("Read : " << result.data);
}

void robot_motor_driver::parse_encoder_data(MOTORmsg &msg_in)
{
  string line = ser.readline();
  line.erase(0,4);

  stringstream ss(line);
  char *rest;
  char *token;
  char *ptr = const_cast<char*>(line.c_str());

  int encoder_count = 0;

  while((token = strtok_r(ptr, ",", &rest)))
  {
    encoder_count++;
    if(encoder_count == 1)
    {
      msg_in.encoder_pulse[LEFT] = stoi(token);
    }
    else if (encoder_count == 2)
    {
      msg_in.encoder_pulse[RIGHT] = stoi(token);
    }

    ptr = rest;
  }
}

void robot_motor_driver::read_encoder()
{
  parse_encoder_data(motor_data);

  std_msgs::Int32 left_encoder_pub_data;
  std_msgs::Int32 right_encoder_pub_data;

  left_encoder_pub_data.data = motor_data.encoder_pulse[LEFT];
  right_encoder_pub_data.data = motor_data.encoder_pulse[RIGHT];

  left_encoder_pub.publish(left_encoder_pub_data);
  right_encoder_pub.publish(right_encoder_pub_data);
}

int main(int argc, char** argv)
{
  ros::init(argc,argv,"robot_motor_driver");
  robot_motor_driver rmd;
  ros::Rate rate(100);

  while(ros::ok())
  {
    rmd.read_encoder();
    rmd.send_motor_command(-rmd.linear_tmp, -rmd.angular_tmp);
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}



