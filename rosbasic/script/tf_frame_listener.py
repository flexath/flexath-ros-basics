#!/usr/bin/env python

import tf
import rospy

import roslib
roslib.load_manifest('rosbasic')


def tf_frame_listener():
	rospy.init_node('tf_frame_listener_node')
	listener = tf.TransformListener()

	transform = tf.TransformerROS()

	# waitForTransform(target_frame, source_frame, time, timeout, polling_sleep_duration = rospy.Duration(0.01))
	listener.waitForTransform("babe","mother",rospy.Time(0),rospy.Duration(10))
	rate = rospy.Rate(1)

	while not rospy.is_shutdown():

		# lookupTransform(target_frame, source_frame, time) returns tuple->(translation, orientation (may be quaternion))
		translation,rotation = listener.lookupTransform("babe","mother",rospy.Time(0))
		print(transform.fromTranslationRotation(translation,rotation))
		#print("The translation is {} \nThe rotation is {} \n".format(translation,rotation))
		rate.sleep()

if __name__ == "__main__":
	tf_frame_listener()
	print("Transform frame is listening")
