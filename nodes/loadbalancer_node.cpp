#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ros/ros.h>
#include <loadbalancer/loadbalancer.h>
#include "block_device/LD_data.h"
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
  ros::Publisher LD_pub = nh.advertise<block_device::LD_data>("LD_data_msg",100);
  block_device::LD_data msg;
  ros::Rate loop_rate(10);
  int count=0;

  while(ros::ok())
  {
    msg.id=count%4+1;
    msg.stamp=ros::Time::now();
    msg.data=count;

    ROS_INFO("%d :: send msg = %d (%d.%d)",msg.id, msg.data, msg.stamp.sec, msg.stamp.nsec);
    LD_pub.publish(msg);

    loop_rate.sleep();

    ++count;
  }
  
  return EXIT_SUCCESS;
}
