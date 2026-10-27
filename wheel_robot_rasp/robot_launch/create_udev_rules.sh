#!/bin/bash

echo "remap the devices serial port(ttyUSBX, ttySX) to  ydlidar, AHRS, Motordriver, Bluetooth"
echo "devices usb connection as /dev/YDLIDAR, /dev/AHRS, /dev/MW, /dev/BT  , check it using the command : ls -l /dev|grep -e ttyUSB -e ttyS0"
echo "start copy robot.rules to  /etc/udev/rules.d/"
echo "`rospack find robot_launch`/robot.rules"
sudo cp `rospack find robot_launch`/robot.rules  /etc/udev/rules.d
echo " "
echo "Restarting udev"
echo ""
sudo udevadm trigger
echo "finish "
