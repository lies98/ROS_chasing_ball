cmake_minimum_required(VERSION 2.8.3)
project(beginner_tutorials)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  image_transport
  cv_bridge
  geometry_msgs
  message_generation
)
find_package(OpenCV COMPONENTS opencv_highgui opencv_features2d opencv_xfeatures2d opencv_calib3d)
if(NOT OPENCV_FOUND)
	find_package(OpenCV REQUIRED COMPONENTS opencv_highgui opencv_features2d opencv_calib3d)
	add_definitions(-DNO_CV_XFEATURES2D)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_message_files(
  FILES
  valueMatrix.msg
 )

generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
 )

catkin_package(
  LIBRARIES tiago_opencv_tutorial
  CATKIN_DEPENDS roscpp rospy image_transport cv_bridge pal_detection_msgs  geometry_msgs message_generation
  DEPENDS system_lib
)
include_directories(include)
include_directories(SYSTEM
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(beginner_tutorials src/track_sequential.cpp)
  target_link_libraries(beginner_tutorials ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
  add_dependencies(beginner_tutorials ${PROJECT_NAME}_generate_messages_cpp)


catkin_install_python(PROGRAMS scripts/talker.py scripts/avoidObstacle.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
