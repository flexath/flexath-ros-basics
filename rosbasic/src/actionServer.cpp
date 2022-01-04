#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <actionlib/server/simple_action_server.h>
#include "rosbasic/CounterAction.h"
#include <iostream>
#include <sstream>

class GhostMan{
protected:
	ros::NodeHandle nh;
	actionlib::SimpleActionServer<rosbasic::CounterAction> as;

	rosbasic::CounterFeedback feedback;
	rosbasic::CounterResult result;

	std::string action_name;
	int goal;
	int progress;
public:
	GhostMan(std::string name) : as(
		nh,
		name,
		boost::bind(&GhostMan::executeCB,this,_1),
		false),
	action_name(name)
	{

		as.registerPreemptCallback(boost::bind(&GhostMan::preemptCB,this));
		as.start();
	}

	void preemptCB()
	{
		ROS_WARN("%s got preempted!", action_name.c_str());
		result.final_count=progress;
		as.setPreempted(result,"I got Preempted");
	}
	void executeCB(const rosbasic::CounterGoalConstPtr &goal)
	{
		if(!as.isActive() || as.isPreemptRequested()) return;

		ros::Rate r(5);
		ROS_INFO("%s is processing the goal %d",action_name.c_str(),goal->count);

		for(progress =1;progress<=goal->count;progress++)
		{
			if(!ros::ok())
			{
				result.final_count = progress;
				as.setAborted(result,"I fail");
				ROS_INFO("%s shutting dodwn",action_name.c_str());
				break;
			}
			if(!as.isActive() || as.isPreemptRequested() ) return;

			if(goal->count<=progress)
			{
				result.final_count = progress;
				as.setSucceeded(result);

			}
			else{
				feedback.current_number = progress;
				as.publishFeedback(feedback);
			}
			r.sleep();
		}
	}// end exeCB

};


int main(int argc,char** argv)
{
	ros::init(argc,argv,"action_server");
	ROS_INFO("Starting Action server");
	//GhostMan gm(ros::this_node::getName());
	GhostMan gm("Counter_action");
	ros::spin();
	return 0;
}
