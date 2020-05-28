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
  ros::Publisher LD1_pub = nh.advertise<block_device::LD_data>("LD1_msg",100);
  ros::Publisher LD2_pub = nh.advertise<block_device::LD_data>("LD2_msg",100);
  ros::Publisher LD3_pub = nh.advertise<block_device::LD_data>("LD3_msg",100);
  ros::Publisher LD4_pub = nh.advertise<block_device::LD_data>("LD4_msg",100);
  block_device::LD_data msg;
  ros::Rate loop_rate(10);
  int count=0;

  while(ros::ok())
  {
    msg.id=count%4+1;
    msg.stamp=ros::Time::now();
    msg.data=count;

    ROS_INFO("%d :: send msg = %d (%d.%d)",msg.id, msg.data, msg.stamp.sec, msg.stamp.nsec);
    switch(msg.id) {
    case 1:
      LD1_pub.publish(msg);
      break;
    case 2:
      LD2_pub.publish(msg);
      break;
    case 3:
      LD3_pub.publish(msg);
      break;
    case 4:
      LD4_pub.publish(msg);
      break;
    default:
      ROS_INFO("ERROR");
      break;
    }

    loop_rate.sleep();

    ++count;
  }
  
  return EXIT_SUCCESS;
}
