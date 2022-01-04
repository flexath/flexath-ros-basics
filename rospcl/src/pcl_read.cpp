#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>
#include <iostream>

using namespace std;

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_reading_node");
  ros::NodeHandle nh;
  ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_reading",1);

  sensor_msgs::PointCloud2 output;
  pcl::PointCloud<pcl::PointXYZI> cloud;

  string filepath = "/home/ath11/rosWorkspaces/ath_ws/src/rospcl/data/test_pcd.pcd";
  pcl::io::loadPCDFile(filepath,cloud);

  pcl::toROSMsg(cloud,output);
  output.header.frame_id = "fixed_frame";

  ros::Rate r(10);
  while(ros::ok()){
    pcl_pub.publish(output);
    ros::spinOnce();
    r.sleep();
  }

}
