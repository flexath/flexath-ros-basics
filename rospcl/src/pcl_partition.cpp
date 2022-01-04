#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <pcl_ros/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/octree/octree.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>

class cloudPartition{
protected:
  ros::NodeHandle nh;
  ros::Subscriber pcl_sub;
  ros::Publisher pcl_pub;
  ros::Timer timer;
  pcl::visualization::CloudViewer viewer;

public:
  cloudPartition() : viewer("Cloud Viewer Partition")
  {
    pcl_sub = nh.subscribe("pcl_downsampling",10,&cloudPartition::pclCallback,this);
    pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_partition",10);
    timer = nh.createTimer(ros::Duration(0.1),&cloudPartition::timerCallback,this);
  }

  void pclCallback(const sensor_msgs::PointCloud2 &input)
  {
    pcl::PointCloud<pcl::PointXYZ> cloud;
    pcl::PointCloud<pcl::PointXYZ> cloud_partitioned;
    pcl::fromROSMsg(input,cloud);

    float resolution = 128.0f;
    pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree (resolution);

    octree.setInputCloud (cloud.makeShared());
    octree.addPointsFromInputCloud ();

    pcl::PointXYZ center_point;
    center_point.x = 0 ;
    center_point.y = 0.4;
    center_point.z = -1.4;

    float radius = 0.5;
    std::vector<int> radiusIdx;
    std::vector<float> radiusSQDist;
    if (octree.radiusSearch (center_point, radius, radiusIdx, radiusSQDist) > 0)
    {
        for (size_t i = 0; i < radiusIdx.size (); ++i)
        {
            cloud_partitioned.points.push_back(cloud.points[radiusIdx[i]]);
        }
    }

    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(cloud_partitioned,output);
    output.header.frame_id = "fixed_frame";
    pcl_pub.publish(output);
    viewer.showCloud(cloud_partitioned.makeShared());

  }

  void timerCallback(const ros::TimerEvent&){
    if(viewer.wasStopped()){
      ros::shutdown();
    }
  }

};

int main(int argc,char **argv){
  ros::init(argc,argv,"pcl_partitioning_node");
  cloudPartition visualizer;
  ros::spin();

}
