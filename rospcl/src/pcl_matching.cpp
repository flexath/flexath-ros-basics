#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/registration/icp.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>

class cloudMatching{
protected:
  ros::NodeHandle nh;
  ros::Subscriber pcl_sub;
  ros::Publisher pcl_pub;
  ros::Timer timer;
  pcl::visualization::CloudViewer viewer;

public:
  cloudMatching() : viewer("Cloud Viewer Matching")
  {
    pcl_sub = nh.subscribe("pcl_downsampling",1,&cloudMatching::pclCallback,this);
    pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_matching",10);
    timer = nh.createTimer(ros::Duration(0.1),&cloudMatching::timerCallback,this);
  }

  void pclCallback(const sensor_msgs::PointCloud2 &input)
  {
    pcl::PointCloud<pcl::PointXYZ> cloud_in;
    pcl::PointCloud<pcl::PointXYZ> cloud_fixed;
    pcl::PointCloud<pcl::PointXYZ> cloud_out;                     // Output Cloud
    sensor_msgs::PointCloud2 output;

    pcl::fromROSMsg(input,cloud_in);

    cloud_fixed = cloud_in;

    for (size_t i = 0; i < cloud_in.points.size (); ++i)
    {
        cloud_fixed.points[i].x = cloud_in.points[i].x + 0.7f;
    }

    pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
    icp.setInputSource(cloud_in.makeShared());
    icp.setInputTarget(cloud_fixed.makeShared());
    // Set the maximum distance threshold between two correspondent points in source <-> target. If the distance is larger than this threshold, the points will be ignored in the alignment process.
    icp.setMaxCorrespondenceDistance(5);
    // Set the maximum number of iterations the internal optimization should run for.
    icp.setMaximumIterations(100);
    // Set the transformation epsilon (maximum allowable difference between two consecutive transformations) in order for an optimization
    icp.setTransformationEpsilon (1e-12);
    // Set the maximum allowed Euclidean error between two consecutive steps in the ICP loop, before the algorithm is considered to have converged.
    icp.setEuclideanFitnessEpsilon(0.1);
    icp.align(cloud_out);


    pcl::toROSMsg(cloud_out,output);
    pcl_pub.publish(output);
    viewer.showCloud(cloud_out.makeShared());

  }

  void timerCallback(const ros::TimerEvent&){
    if(viewer.wasStopped()){
      ros::shutdown();
    }
  }

};

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_matching_node");
  cloudMatching visualizer;
  ros::spin();

}
