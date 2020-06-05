#include <loadbalancer/loadbalancer.h>

using namespace std;

namespace LoadBalancer {

LoadControl::LoadControl(ros::NodeHandle nh)
  : nodeHandle_(nh), pubList_(0), nodeList_(0) {
  ROS_INFO("[LoadControl] Node started.");

  init();
}

LoadControl::~LoadControl(){
  ROS_INFO("[LoadControl] Node shutdown.");
}
void LoadControl::init(void) {
  ROS_INFO("[LoadControl] init().");

  string cameraTopicName = "/usb_cam/image_raw";
  int cameraQueueSize = 1;
  string LDServerTopicName = "/LD_set";
  string LBServerTopicName = "/LB_command";
  imageSub_ = nodeHandle_.subscribe(cameraTopicName, cameraQueueSize, &LoadControl::cameraCallback, this);
  LDServer_ = nodeHandle_.advertiseService(LDServerTopicName, &LoadControl::connectionCallback, this);
  LBServer_ = nodeHandle_.advertiseService(LBServerTopicName, &LoadControl::commandCallback, this);
  cnt_ = 0;
  flag_ = 0;
}
    
void LoadControl::spin(void) {
  block_device::LD_data msg;
  ros::Rate loop_rate(5);
  int i;

  while(nodeHandle_.ok()){
    if((cnt_ != 0) && (flag_ == 1)) {
      msg.id = i%cnt_;
      msg.stamp = ros::Time::now();
      msg.data = i++;
      ROS_INFO("%s :: send msg = %d",nodeList_.at(msg.id).c_str(), msg.data);
      pubList_.at(msg.id).publish(msg);
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
}

bool LoadControl::connectionCallback(block_device::LD_connect::Request &req, block_device::LD_connect::Response &res){
  ++cnt_;
  res.id = cnt_;
  ROS_INFO("[%d] %s request connection...", res.id, req.node_name.c_str());
  ros::Publisher LDPub = nodeHandle_.advertise<block_device::LD_data>(req.msg_name.c_str(), 10);
  pubList_.push_back(LDPub);
  nodeList_.push_back(req.node_name);
  ROS_INFO("register LD node : Connected %d nodes", cnt_);
  return true;
}

bool LoadControl::commandCallback(block_device::LB_command::Request &req, block_device::LB_command::Response &res){
  if(!req.command.compare("start")){
    res.result = "[LoadControl] start sending";
    flag_ = 1;
    ROS_INFO("[Command] start sending");
    return true;
  }
  else if(!req.command.compare("stop")){
    res.result = "[LoadControl] stop sending";
    flag_ = 0;
    ROS_INFO("[Command] stop sendig");
    return true;
  } else {
    res.result = "[Command Error] " + req.command;
    ROS_INFO("[Command Error] %s", req.command.c_str());
  }
  return false;
}

void LoadControl::cameraCallback(const sensor_msgs::ImageConstPtr& msg) {
  ROS_DEBUG("[LoadControl] USB image received");

  cv_bridge::CvImagePtr cam_image;

  try {
    cam_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  } catch (cv_bridge::Exception& e) {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }

  if(cam_image) {
    ;
  }
  return;
}

}
