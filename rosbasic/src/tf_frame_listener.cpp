#include <ros/ros.h>
#include <tf/transform_listener.h>

using namespace std;

int main(int argc,char **argv){
  ros::init(argc,argv,"listener_node");
  ros::NodeHandle nh;

  tf::TransformListener lis;
  tf::Transform transform;

  ros::Rate rate(10.0);
  lis.waitForTransform("/child","/parent",ros::Time(0),ros::Duration(5.0));

  while(ros::ok()){
    tf::StampedTransform stamp_transform;
    try{
      string target = "/child";
      string source = "/parent";
      ros::Time time = ros::Time::now();
      lis.lookupTransform(target,source,time,stamp_transform);
    }
    catch(tf::ExtrapolationException &error){
      ROS_INFO("%s",error.what());
      ros::Duration(1.0).sleep();
      //continue;
  }
  rate.sleep();
}
return 0;
}
