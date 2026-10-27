cmake_minimum_required(VERSION 3.0.2)
project(opencv_camera)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  std_msgs
)

find_package(OpenCV REQUIRED)

catkin_package(
#  CATKIN_DEPENDS
#  cv_bridge
#  image_transport
#  roscpp
#  sensor_msgs
#  std_msgs
#  DEPENDS system_lib
)

include_directories(
  #include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(opencv_camera_pub src/opencv_pub.cpp)
target_link_libraries(opencv_camera_pub ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_executable(opencv_camera_sub src/opencv_sub.cpp)
target_link_libraries(opencv_camera_sub ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
