#ifndef LOADBALANCER_LOADBALANCER_H
#define LOADBALANCER_LOADBALANCER_N
#endif


// c++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

// ROS
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include "block_device/LD_data.h"
#include "block_device/LD_set.h"
#include "block_device/LD_connect.h"
#include "block_device/LB_command.h"

// OpenCV
#include <cv_bridge/cv_bridge.h>

using namespace std;

namespace LoadBalancer{

class LoadControl{
  public:
    explicit LoadControl(ros::NodeHandle nh);
    void spin();
    ~LoadControl();
  private:
    void init();
    bool connectionCallback(block_device::LD_connect::Request &req, block_device::LD_connect::Response &res);
    bool commandCallback(block_device::LB_command::Request &req, block_device::LB_command::Response &res);
    void cameraCallback(const sensor_msgs::ImageConstPtr& msg);
    double what_time_is_it_now();
    ros::NodeHandle nodeHandle_;
    ros::Subscriber imageSub_;
    ros::ServiceServer LDServer_; // /LD_connect
    ros::ServiceServer LBServer_; // /LB_command
    ros::Subscriber LDSub_; // /LD_data
    vector<ros::Publisher> pubList_;
    vector<string> nodeList_;
    int cnt_;
    int flag_;
    double fps_;
    double prevTime_;

};

}
