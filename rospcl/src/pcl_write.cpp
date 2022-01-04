#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>

void pclCallback(const sensor_msgs::PointCloud2 &input){
  pcl::PointCloud<pcl::PointXYZ> cloud;
  pcl::fromROSMsg(input,cloud);
  pcl::io::savePCDFileASCII("test_write_pcd.pcd",cloud);
}

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_writing_node");
  ros::NodeHandle nh;
  ros::Subscriber pcl_sub = nh.subscribe("pcl_reading",1,pclCallback);
  ros::spin();

}
