/*
 * yolo_obstacle_detector_node.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: Marko Bjelonic
 *   Institute: ETH Zurich, Robotic Systems Lab
 */

#include <ros/ros.h>
#include <stdio.h>
#include <string.h>
#include <darknet_ros/YoloObjectDetector.hpp>

using namespace std;

int main(int argc, char** argv) {
  string device_name, Node_name;
  device_name = getenv("HOSTNAME");
  Node_name = "darknet_ros_"+device_name;
  ros::init(argc, argv, "darknet_ros" );
  ros::NodeHandle nodeHandle("~");
  darknet_ros::YoloObjectDetector yoloObjectDetector(nodeHandle);

  ros::spin();
  return 0;
}
