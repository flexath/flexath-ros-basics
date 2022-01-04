#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

using namespace std;

int main(int argc,char **argv){
  ros::init(argc,argv,"broadcaster_node");
  ros::NodeHandle nh;

  tf::TransformBroadcaster br;
  tf::Transform transform;

  ros::Rate rate(10.0);

  while(ros::ok()){
    transform.setOrigin(tf::Vector3(7.0,5.0,0.0));
    transform.setRotation(tf::Quaternion(0,0,0,1));
    ros::Time time = ros::Time::now();
    string parent_frame = "/parent";
    string child_frame = "/child";
    br.sendTransform(tf::StampedTransform(transform,time,parent_frame,child_frame));
    rate.sleep();
  }
  return 0;
};
