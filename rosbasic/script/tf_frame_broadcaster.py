#!/usr/bin/env python

import tf
import rospy

import roslib
roslib.load_manifest('rosbasic')

def tf_frame_broadcaster():
	""" This whole program is all same as static transform publisher ! """

	rospy.init_node("tf_frame_broadcaster_node")
	broadcaster = tf.TransformBroadcaster()
	rate = rospy.Rate(10)

	while not rospy.is_shutdown():

		#Broadcast the transformation from tf frame child to parent on ROS topic "/tf
		translation = (0.0,  -2.0 , 0.0 )
		rotation = 	  (0.0 , 0.0 , 0.0 , 1.0)
		time = rospy.Time(0)
		child = "babe"
		parent = "mother"

		# sendTransform(translation, rotation, time, child, parent)
		# rotation will be [0,0,0,1] matrix in this program because it is Quaternion matrix (rotation may be any rotational matrix)
		broadcaster.sendTransform(translation, rotation, time, child, parent)
		rate.sleep()

if __name__ == "__main__":
	tf_frame_broadcaster()
	print("Transform frame is broadcasting")