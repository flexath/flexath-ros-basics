#include <ros/ros.h>
#include <rosbasic/Calculation.h>

bool callback(rosbasic::Calculation::Request &req,rosbasic::Calculation::Response &res){

	res.addition_result = req.first_number + req.second_number;
	res.subtraction_result = req.first_number - req.second_number;
	res.multiplication_result = req.first_number * req.second_number;
	res.division_result = req.first_number / req.second_number;

	ROS_INFO("Addition Result is : %f" , (float)res.addition_result);
	ROS_INFO("subtraction_result Result is : %f" , (float)res.subtraction_result);
	ROS_INFO("Multiplication Result is : %f" , (float)res.multiplication_result);
	ROS_INFO("Division Result is : %f" , (float)res.division_result);
	ROS_INFO("----------");

	return true;
}

int main(int argc,char** argv){
	ros::init(argc,argv,"server_node");
	ros::NodeHandle nh;
	ros::ServiceServer server = nh.advertiseService("/calculation_service",callback);
	ROS_INFO("Service is ready !");
	ros::spin();
}