#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ros/ros.h>
#include <block_device/block_device.h>

namespace block_device {

class LoadDeviceNode
{
  public:
    ros::NodeHandle node_;
    LoadDevice device_;
    int result_;
    bool spin() {
      while(node_.ok())
      {
        ros::spinOnce();
      }
      return true;
    };
};

}

int main(int argc, char **argv)
{
  char Node_name[20];
  char* device_name = getenv("HOSTNAME");
  strcpy(Node_name, "block_device_");
  strcat(Node_name,device_name);
  ros::init(argc, argv, Node_name);
  ROS_INFO("ROS init Node : %s",Node_name);

  block_device::LoadDeviceNode nh;
  nh.spin();
  return EXIT_SUCCESS;
}
