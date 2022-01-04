#!/usr/bin/env python

import rospy
from std_msgs.msg import String

def callback(msg):
	print("I receive >> ",msg.data)

rospy.init_node("subscriber_hello_node")
sub = rospy.Subscriber("/hello",String,callback)
rospy.spin()