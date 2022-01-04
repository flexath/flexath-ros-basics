#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>

class cloudDownsampling{
protected:
  ros::NodeHandle nh;
  ros::Subscriber pcl_sub;
  ros::Publisher pcl_pub;
  ros::Timer timer;
  pcl::visualization::CloudViewer viewer;

public:
  cloudDownsampling() : viewer("Cloud Viewer Downsampling")
  {
    pcl_sub = nh.subscribe("pcl_reading",1,&cloudDownsampling::pclCallback,this);
    pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_downsampling",10);
    timer = nh.createTimer(ros::Duration(0.1),&cloudDownsampling::timerCallback,this);
  }

  void pclCallback(const sensor_msgs::PointCloud2 &input)
  {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromROSMsg(input,cloud);

    pcl::PointCloud<pcl::PointXYZ> cloud_downsampled;

    pcl::VoxelGrid<pcl::PointXYZ> voxelSampler;
    voxelSampler.setInputCloud(cloud.makeShared());
    voxelSampler.setLeafSize(0.01f, 0.01f, 0.01f);
    voxelSampler.filter(cloud_downsampled);

    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(cloud_downsampled,output);
    pcl_pub.publish(output);
    viewer.showCloud(cloud_downsampled.makeShared());
  }

  void timerCallback(const ros::TimerEvent&){
    if(viewer.wasStopped()){
      ros::shutdown();
    }
  }

};

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_downsampling_node");
  cloudDownsampling visualizer;
  ros::spin();

}
