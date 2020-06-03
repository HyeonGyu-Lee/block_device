#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ros/ros.h>
#include <darknet_ros/YoloObjectDetector.hpp>
#include <block_device/block_device.h>
#include "block_device/LD_data.h"
#include "block_device/LD_set.h"
#include "block_device/LD_connect.h"

using namespace std;

namespace block_device {

class LoadDeviceNode
{
  public:
    ros::NodeHandle nh_;
    ros::Subscriber LD_sub_;
    ros::Publisher LD_pub_;
    ros::ServiceClient LD_client_;
    LoadDevice device_;
    string Node_name_, Msg_name_;

    void init(string Node_str, string Msg_str){
      Node_name_ = Node_str;
      Msg_name_ = Msg_str;
    }

    bool spin(void) {
      LD_client_ = nh_.serviceClient<block_device::LD_connect>("LD_set");
      block_device::LD_connect check;

      check.request.node_name = Node_name_;
      check.request.msg_name = Msg_name_;

      if(LD_client_.call(check)) {
        ROS_INFO("%s wake up ..., setting msg : %s",check.request.node_name.c_str(),check.request.msg_name.c_str());
	ROS_INFO("You are node [%d]", check.response.id);
	LD_sub_ = nh_.subscribe(Msg_name_,1000, &LoadDeviceNode::msgCallback, this);
        ros::Rate loop_rate(500);	    
        while(nh_.ok())
        {
          ros::spinOnce();
	  loop_rate.sleep();
        }
      }else{
        ROS_ERROR("Failed to connect");
	return -1;
      }

      return 0;
    }

    void msgCallback(const block_device::LD_data::ConstPtr& msg) {
      ROS_INFO("recieve msg = %d", msg->data);
    }
   
};

}
  
int main(int argc, char **argv)
{
  string device_name, Node_name, Msg_name;
  device_name = getenv("HOSTNAME");
  Node_name = "block_device_" + device_name;
  Msg_name = device_name + "_msg";
   
  ros::init(argc, argv, Node_name);
  ROS_INFO("SETTING NODE : %s", Node_name.c_str());
 
  ros::NodeHandle nodeHandle("~");
  darknet_ros::YoloObjectDetector yoloObjectDetector(nodeHandle);
  block_device::LoadDeviceNode nh;

  nh.init(Node_name, Msg_name);
  nh.spin();

  return EXIT_SUCCESS;
}
