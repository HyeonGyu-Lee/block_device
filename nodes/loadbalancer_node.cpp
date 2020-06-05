#include <ros/ros.h>
#include <loadbalancer/loadbalancer.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "LoadBalancer");
  ROS_INFO("LoadBalancer operation");
 
  ros::NodeHandle nodeHandle("~");
  LoadBalancer::LoadControl LB(nodeHandle);
  
  LB.spin();
  return EXIT_SUCCESS;
}
