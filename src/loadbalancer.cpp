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
  i_ = 0;
  prevTime_ = what_time_is_it_now();
}
    
void LoadControl::spin(void) {
  block_device::LD_data msg;
  ros::Rate loop_rate(60);
  int i;

  while(nodeHandle_.ok()){
    ros::spinOnce();
    loop_rate.sleep();
  }
}

bool LoadControl::connectionCallback(block_device::LD_connect::Request &req, block_device::LD_connect::Response &res){
  ++cnt_;
  res.id = cnt_;
  ROS_INFO("[%d] %s request connection...", res.id, req.node_name.c_str());
  ros::Publisher LDPub = nodeHandle_.advertise<sensor_msgs::Image>(req.msg_name.c_str(), 1, true);
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
  cv_bridge::CvImage cvImage;

  try {
    cam_image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  } catch (cv_bridge::Exception& e) {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }


  if(cam_image) {
    fps_= 1./(what_time_is_it_now() - prevTime_);
    prevTime_ = what_time_is_it_now();
    printf("\033[2J");
    printf("\033[1;1H");
    printf("\nFPS:%.1f\n", fps_);
    if(pubList_.empty())
      printf("[Warning]Node is Not connected\n");
    else { 
      cvImage.header.stamp = ros::Time::now();
      cvImage.header.frame_id = nodeList_.at(i_%pubList_.size()).c_str();
      cvImage.encoding = sensor_msgs::image_encodings::BGR8;
      cvImage.image = cam_image->image;
      pubList_.at(i_%pubList_.size()).publish(*cvImage.toImageMsg());
      i_++;
    }
  }
  else {
    printf("Wait for image\n");
  }
  return;
}

double LoadControl::what_time_is_it_now(){
  struct timeval time;
  if(gettimeofday(&time,NULL)){
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

}
