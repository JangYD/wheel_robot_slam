cmake_minimum_required(VERSION 3.0.2)
project(robot_mw_ahrsv2u)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  sensor_msgs
  std_msgs
  tf
  tf2
  tf2_msgs
  tf2_ros
  nav_msgs
)


#generate_messages(
#  DEPENDENCIES
#  geometry_msgs std_msgs sensor_msgs tf2_msgs
#)

catkin_package()

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  "${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}"
  "${PROJECT_SOURCE_DIR}/serial/"

)
add_executable(${PROJECT_NAME}_node src/mw_ahrsv2u.cpp serial/list_ports_linux.cc serial/serial.cc serial/unix.cc)
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES} )
target_include_directories(${PROJECT_NAME}_node PRIVATE "include/${PROJECT_NAME}/")
