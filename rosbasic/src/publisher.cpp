#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>

int main(int argc,char** argv){
	ros::init(argc,argv,"publisher_node");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<std_msgs::String>("/hello" , 1000);
	ros::Rate rate(1);

	while(ros::ok()){
		std_msgs::String msg;
		std::stringstream ss;
		ss << "'Hello World , I am publisher node'";
		msg.data = ss.str();

		ROS_INFO(msg.data.c_str());
		//ROS_INFO_STREAM("Publisher Node sends : " << msg.data.c_str());
		pub.publish(msg);
		ros::spinOnce();
		rate.sleep();
	}
	return 0;

}
