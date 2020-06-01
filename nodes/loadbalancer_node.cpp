#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ros/ros.h>
#include <vector>
#include <pthread.h>
#include <loadbalancer/loadbalancer.h>
#include "block_device/LD_data.h"

using namespace std;
/*namespace Load_Balancer {

class LoadControlNode
{
  public:
    ros::NodeHandle nh_;
};

}*/

int main(int argc, char **argv)
{
  ros::init(argc,argv,"LoadBalancer");
  ROS_INFO("LoadBalancer Start");
  ros::NodeHandle nh;
  ros::Publisher LD1_pub = nh.advertise<block_device::LD_data>("LD1_msg",100);
  ros::Publisher LD2_pub = nh.advertise<block_device::LD_data>("LD2_msg",100);
  ros::Publisher LD3_pub = nh.advertise<block_device::LD_data>("LD3_msg",100);
  ros::Publisher LD4_pub = nh.advertise<block_device::LD_data>("LD4_msg",100);
  vector<ros::Publisher> pub_list;
  pub_list.push_back(LD1_pub);
  pub_list.push_back(LD2_pub);
  pub_list.push_back(LD3_pub);
  pub_list.push_back(LD4_pub);
  
  block_device::LD_data msg;
  ros::Rate loop_rate(10);
  int cnt=0;

  while(ros::ok())
  {
    msg.id=cnt%4;
    msg.stamp=ros::Time::now();
    msg.data=cnt;

    ROS_INFO("LD%d :: send msg = %d (%d.%d)",msg.id+1, msg.data, msg.stamp.sec, msg.stamp.nsec);
//    pthread_create();
    pub_list.at(msg.id).publish(msg);
//    pthread_join();

    loop_rate.sleep();
    cnt++;
  }
  
  return EXIT_SUCCESS;
}
