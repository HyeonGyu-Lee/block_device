#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ros/ros.h>
#include <block_device/block_device.h>
#include "block_device/LD_data.h"

/*namespace block_device {

class LoadDeviceNode
{
  public:
    ros::NodeHandle node_;
    ros::Subscriber LD_sub_;
    LoadDevice device_;
    int result_;
    void init(void){
      LD_sub_ = node_.subscribe("LD_data_msg",10,LoadDeviceNode::msgCallback);
    };
    bool spin() {
      while(node_.ok())
      {
        ros::spinOnce();
      }
      return true;
    }
   void msgCallback(const block_device::LD_data::ConstPtr& msg) {
     ROS_INFO("recieve msg = %d", msg->data);
   }
   
};

}*/

void msgCallback(const block_device::LD_data::ConstPtr& msg) {
  ROS_INFO("recieve msg = %d", msg->data);
}
  
int main(int argc, char **argv)
{
  char Node_name[20], Msg_name[20];
  char* device_name = getenv("HOSTNAME");
  strcpy(Node_name, "block_device_");
  strcpy(Msg_name, device_name);
  strcat(Node_name,device_name);
  strcat(Msg_name, "_msg");
  ros::init(argc, argv, Node_name);
  ROS_INFO("ROS init Node : %s",Node_name);
  ros::NodeHandle nh;
  ros::Subscriber LD_sub = nh.subscribe(Msg_name,100,msgCallback);
  ros::spin();
//  block_device::LoadDeviceNode nh;
//  nh.init(device_name);
//  nh.spin();
  return EXIT_SUCCESS;
}
