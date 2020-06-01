#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ros/ros.h>
#include <vector>
#include <pthread.h>
#include <loadbalancer/loadbalancer.h>
#include "block_device/LD_data.h"
#include "block_device/LD_set.h"
#include "block_device/LD_connect.h"

using namespace std;

namespace LoadBalancer {

class LoadControlNode
{
  public:
    ros::NodeHandle nh_;
    ros::Subscriber LD_sub_;
    ros::ServiceServer LD_server_;
    vector<ros::Publisher> pub_list_;
    vector<string> node_list_;
    int cnt_;
    
    bool spin(void) {
      LD_server_ = nh_.advertiseService("LD_set", &LoadControlNode::connection_check, this);
      cnt_ = 0;
      block_device::LD_data msg;
      ros::Rate loop_rate(5);
      int i;
      while(nh_.ok()){
	if(cnt_ != 0) {
          msg.id = i%cnt_;
	  msg.stamp = ros::Time::now();
	  msg.data = i++;
          ROS_INFO("%s :: send msg = %d",node_list_.at(msg.id).c_str(), msg.data);
          pub_list_.at(msg.id).publish(msg);
	}
	ros::spinOnce();
        loop_rate.sleep();
      }
    }

    bool connection_check(block_device::LD_connect::Request &req, block_device::LD_connect::Response &res){
      ++cnt_;
      res.id = cnt_;
      ROS_INFO("[%d] %s request connection...", res.id, req.node_name.c_str());
      ros::Publisher LD_pub = nh_.advertise<block_device::LD_data>(req.msg_name.c_str(), 10);
      pub_list_.push_back(LD_pub);
      node_list_.push_back(req.node_name);
      ROS_INFO("register LD node : Connected %d nodes", cnt_);

      return true;
    }

};

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "LoadBalancer");
  ROS_INFO("LoadBalancer operation");
 
  LoadBalancer::LoadControlNode nh;
  nh.spin();

  return EXIT_SUCCESS;
}
