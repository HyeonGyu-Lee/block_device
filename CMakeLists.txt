cmake_minimum_required(VERSION 3.0.2)
project(block_device)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
  darknet_ros
)

################################################
## Declare ROS messages, services and actions ##
################################################

add_message_files(
  FILES
  LD_data.msg
  LD_check.msg
)

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

generate_messages(
  DEPENDENCIES
  std_msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  #CATKIN_DEPENDS roscpp rospy std_msgs
  #   DEPENDS system_lib
)

###########
## Build ##
###########

cmake_host_system_information(RESULT hostname_ QUERY HOSTNAME)

include_directories(include
  ${catkin_INCLUDE_DIRS}
)

## Build library
add_library(${PROJECT_NAME}
  src/block_device.cpp
  src/loadbalancer.cpp  
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

add_executable(${PROJECT_NAME}_${hostname_} nodes/block_device_node.cpp)
add_dependencies(${PROJECT_NAME}_${hostname_} ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(${PROJECT_NAME}_${hostname_}
  ${catkin_LIBRARIES}
)

add_executable(loadbalancer_node nodes/loadbalancer_node.cpp)
add_dependencies(loadbalancer_node ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(loadbalancer_node
  ${catkin_LIBRARIES}
)
