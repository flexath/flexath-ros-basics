#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <sensor_msgs/PointCloud2.h>

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_creation_node");
  ros::NodeHandle nh;
  ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_creation",1);

  sensor_msgs::PointCloud2 output;


  pcl::PointCloud<pcl::PointXYZI> cloud;
  cloud.height = 1;
  cloud.width = 100;
  cloud.points.resize(cloud.height*cloud.width);

  for(size_t i=0;i < cloud.points.size();i++){
    cloud.points[i].x = (1024*rand())/(RAND_MAX + 1.0f);
    cloud.points[i].y = (1024*rand())/(RAND_MAX + 1.0f);
    cloud.points[i].z = (1024*rand())/(RAND_MAX + 1.0f);
  }

  pcl::toROSMsg(cloud,output);
  output.header.frame_id = "fixed_frame";

  ros::Rate r(10);
  while(ros::ok()){
    pcl_pub.publish(output);
    ros::spinOnce();
    r.sleep();
  }

}
