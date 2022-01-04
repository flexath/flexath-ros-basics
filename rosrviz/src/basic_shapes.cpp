#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main(int argc,char **argv){
  ros::init(argc,argv,"basic_shape_node");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<visualization_msgs::Marker>("/basic_shape",10);
  ros::Rate r(10);

  visualization_msgs::Marker marker;
  marker.header.frame_id = "/shaper";
  marker.header.stamp = ros::Time::now();
  marker.ns = "shapes";
  marker.id = 0;
  marker.type = visualization_msgs::Marker::CUBE;
  marker.action = visualization_msgs::Marker::ADD;

  marker.pose.position.x = 0;
  marker.pose.position.y = 0;
  marker.pose.position.z = 3;
  marker.pose.orientation.x = 0;
  marker.pose.orientation.y = 0;
  marker.pose.orientation.z = 0;
  marker.pose.orientation.w = 1;

  marker.scale.x = 1;
  marker.scale.y = 1;
  marker.scale.z = 4;
  marker.color.r = 1;
  marker.color.g = 0;
  marker.color.b = 0;
  marker.color.a = 1;

  while (ros::ok()){
    pub.publish(marker);
    r.sleep();
  }
}
