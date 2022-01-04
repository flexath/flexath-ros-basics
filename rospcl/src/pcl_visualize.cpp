#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>

class cloudVisualization{
protected:
  ros::NodeHandle nh;
  ros::Subscriber pcl_sub;
  ros::Timer timer;
  pcl::visualization::CloudViewer viewer;

public:
  cloudVisualization() : viewer("Cloud Viewer")
  {
    pcl_sub = nh.subscribe("pcl_reading",1,&cloudVisualization::pclCallback,this);
    timer = nh.createTimer(ros::Duration(0.1),&cloudVisualization::timerCallback,this);
  }

  void pclCallback(const sensor_msgs::PointCloud2 &input)
  {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromROSMsg(input,cloud);
    viewer.showCloud(cloud.makeShared());
  }

  void timerCallback(const ros::TimerEvent&){
    if(viewer.wasStopped()){
      ros::shutdown();
    }
  }

};

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_writing_node");
  cloudVisualization visualizer;
  ros::spin();

}
