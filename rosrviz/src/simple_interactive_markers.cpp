#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/InteractiveMarker.h>
#include <visualization_msgs/InteractiveMarkerControl.h>
#include <interactive_markers/interactive_marker_server.h>

void processFeedback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr &feedback )
{
  ROS_INFO_STREAM( feedback->marker_name << " is now at "
      << feedback->pose.position.x << ", " << feedback->pose.position.y
      << ", " << feedback->pose.position.z );
}

int main(int argc,char **argv){
  ros::init(argc,argv,"simple_interactive_node");
  ros::NodeHandle nh;

  interactive_markers::InteractiveMarkerServer server("interactive_server");

  visualization_msgs::InteractiveMarker inter_marker;
  inter_marker.header.frame_id = "/base_marker";
  inter_marker.header.stamp = ros::Time::now();
  inter_marker.name = "/interactive_marker_simple";
  inter_marker.description = "Simple Interactive 1-DOF";

  visualization_msgs::Marker marker;
  marker.header.frame_id = "/base_marker";
  marker.header.stamp = ros::Time::now();
  marker.ns = "shapes";
  marker.id = 0;
  marker.type = visualization_msgs::Marker::CUBE;
  marker.action = visualization_msgs::Marker::ADD;
  marker.pose.position.x = 0;
  marker.pose.position.y = 0;
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0;
  marker.pose.orientation.y = 0;
  marker.pose.orientation.z = 0;
  marker.pose.orientation.w = 1;
  marker.scale.x = 0.45;
  marker.scale.y = 0.45;
  marker.scale.z = 1.0;
  marker.color.r = 0;
  marker.color.g = 1;
  marker.color.b = 1;
  marker.color.a = 1;

  visualization_msgs::InteractiveMarkerControl marker_control;
  marker_control.always_visible = true;

  visualization_msgs::InteractiveMarkerControl rotate_control;
  rotate_control.name = "rotate";
  rotate_control.name = "move";
  rotate_control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;
  rotate_control.orientation_mode = visualization_msgs::InteractiveMarkerControl::MOVE_ROTATE_3D;

  // add the control to the interactive marker
  marker_control.markers.push_back(marker);
  inter_marker.controls.push_back(marker_control);
  inter_marker.controls.push_back(rotate_control);

  server.insert(inter_marker,&processFeedback);
  server.applyChanges();
  ros::spin();

}
