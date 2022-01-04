#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>

class cloudFiltering{
protected:
  ros::NodeHandle nh;
  ros::Subscriber pcl_sub;
  ros::Publisher pcl_pub;
  ros::Timer timer;
  pcl::visualization::CloudViewer viewer;

public:
  cloudFiltering() : viewer("Cloud Viewer Filtering")
  {
    pcl_sub = nh.subscribe("pcl_reading",1,&cloudFiltering::pclCallback,this);
    pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_filtering",10);
    timer = nh.createTimer(ros::Duration(0.1),&cloudFiltering::timerCallback,this);
  }

  void pclCallback(const sensor_msgs::PointCloud2 &input)
  {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::fromROSMsg(input,cloud);

    pcl::PointCloud<pcl::PointXYZ> cloud_filtered;

    // To understand statistical outlier removal filer , watch these youtube links :
    // https://www.youtube.com/watch?v=jbCxoUUQC3k&t=400s&fbclid=IwAR0ljLlArA-W5N6Fk4xqulWsp-HCS8Qilh2RgA79V6vQpzgPgIpcJSuWFF4
    // https://www.mathsisfun.com/data/standard-deviation.html?fbclid=IwAR0NCI5DfFKallcCEt03zuWL1yPOqDhj0Z8_LHqT9srFikXV15Ybm1n84R8

    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> statRemover;
    statRemover.setInputCloud(cloud.makeShared());
    statRemover.setMeanK(10);                                                    // The number of neighbors to analyze for each point is set to 10
    statRemover.setStddevMulThresh(0.2);                                         // the standard deviation multiplier
    statRemover.filter(cloud_filtered);

    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(cloud_filtered,output);
    pcl_pub.publish(output);
    viewer.showCloud(cloud_filtered.makeShared());

  }

  void timerCallback(const ros::TimerEvent&){
    if(viewer.wasStopped()){
      ros::shutdown();
    }
  }

};

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_filtering_node");
  cloudFiltering visualizer;
  ros::spin();

}
