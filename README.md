Remote PC : Ubuntu 20.04</br>
Remote PC ROS Version : noetic</br></br>
RaspberryPI Version : buster</br>
RaspberryPI ROS Version : noetic</br>

mobile robot : stella n2</br>
stella n2 git hub : https://github.com/ntrexlab</br>
stella n2 motor git hub : https://github.com/ntrexlab/STELLA_MotorControllers_Preferences</br>

YDLIDAR install </br>
----------------------
git clone https://github.com/YDLIDAR/YDLidar-SDK.git</br>
cd YDLidar-SDK</br>
mkdir build</br>
cd build</br>
cmake ..</br>
make</br>
sudo make install</br>

Remote PC Setting</br>
------------------
sudo nano ~/.bashrc</br>
export ROS_MASTER_URI=http:// your remote pc ip:11311</br>
export ROS_HOSTNAME=your remote pc ip</br>
ctrl+x</br>
y</br>

ROS Cartographer install</br>
https://google-cartographer-ros.readthedocs.io/en/latest/compilation.html</br>

Cartographer install error </br>
cartographer: [libabsl-dev] defined as "not available" for OS version [focal]</br>
file path ~/cartographer/package.xml -> delete <depend>libabsl-dev</depend></br>

Raspberry PI setting</br>
--------------------
IP Setting</br>
sudo nano ~/.bashrc</br>
export ROS_MASTER_URI=http:// your remote pc ip:11311</br>
export ROS_HOSTNAME=your raspberypi ip</br>
ctrl+x</br>
y</br>

Robot rules Setting</br>
cd catkin_ws/src/robot_launch</br>
sh create_udev_rules.sh </br>

robot start</br>
-----------
simulation</br> 
|Remote PC|Raspberry PI|
|:-|:-|
|1. roscore|2. roslaunch robot_launch robot_launch.launch|
|3. roslaunch robot_simulation robot_stella.launch</br>|
|4. rosrun robot_gui_template robot_gui_template_node</br>|

slam</br>
|Remote PC|Raspberry PI|
|:-|:-|
|1. roscore|2. roslaunch robot_launch robot_launch.launch|
|3. roslaunch robot_slam robot_slam_rviz.launch|
|4. rosrun robot_gui_template robot_gui_template_node</br>|

navigation</br>
|Remote PC|Raspberry PI|
|:-|:-|
|1. roscore|2. roslaunch robot_launch robot_launch.launch|
|3. roslaunch robot_navigation robot_navigation.launch|







