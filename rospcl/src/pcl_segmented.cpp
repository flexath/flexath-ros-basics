#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>

class cloudSegmentation{
protected:
  ros::NodeHandle nh;
  ros::Subscriber pcl_sub;
  ros::Publisher pcl_pub, ind_pub, coef_pub;
  ros::Timer timer;
  pcl::visualization::CloudViewer viewer;

public:
  cloudSegmentation() : viewer("Cloud Viewer Segmentation")
  {
    pcl_sub = nh.subscribe("pcl_downsampling",10,&cloudSegmentation::pclCallback,this);
    pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_segmentation",10);
    ind_pub = nh.advertise<pcl_msgs::PointIndices>("point_indices", 1);
    coef_pub = nh.advertise<pcl_msgs::ModelCoefficients>("planar_coef", 1);
    timer = nh.createTimer(ros::Duration(0.1),&cloudSegmentation::timerCallback,this);
  }

  void pclCallback(const sensor_msgs::PointCloud2 &input)
  {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZ> cloud_segmented;
    pcl::fromROSMsg(input,cloud);

    pcl::ModelCoefficients coefficients;
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices());

    pcl::SACSegmentation<pcl::PointXYZ> segmentation;
    segmentation.setModelType(pcl::SACMODEL_PLANE);
    segmentation.setMethodType(pcl::SAC_RANSAC);
    segmentation.setMaxIterations(1000);
    segmentation.setDistanceThreshold(0.01);
    segmentation.setInputCloud(cloud.makeShared());
    segmentation.segment(*inliers, coefficients);

    // Publish the model coefficients
    pcl_msgs::ModelCoefficients ros_coefficients;             // this is ros message
    pcl_conversions::fromPCL(coefficients, ros_coefficients);
    coef_pub.publish(ros_coefficients);

    // Publish the Point Indices
    pcl_msgs::PointIndices ros_inliers;                       // this is ros message
    pcl_conversions::fromPCL(*inliers, ros_inliers);
    ind_pub.publish(ros_inliers);

    // Create the filtering object
    pcl::ExtractIndices<pcl::PointXYZ> extract;
    extract.setInputCloud(cloud.makeShared());
    extract.setIndices(inliers);
    extract.setNegative(false);
    extract.filter(cloud_segmented);

    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(cloud_segmented,output);
    pcl_pub.publish(output);
    viewer.showCloud(cloud_segmented.makeShared());

  }

  void timerCallback(const ros::TimerEvent&){
    if(viewer.wasStopped()){
      ros::shutdown();
    }
  }

};

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_segmentation_node");
  cloudSegmentation visualizer;
  ros::spin();

}
