#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>

void callback(const std_msgs::String::ConstPtr &request){
	ROS_INFO("I receive this message : [%s]" , request->data.c_str());
}

int main(int argc,char** argv){
	ros::init(argc,argv,"subscriber_node");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/hello",1000,callback);
	ros::spin();
}