#!/usr/bin/env python

import rospy
from rosbasic.msg import Hello

rospy.init_node("publisher_hello_node")
pub = rospy.Publisher("/hello",Hello,queue_size=1)

rate = rospy.Rate(1)

hello = Hello()
hello.data = "'Hello World , I am publisher node'"

while not rospy.is_shutdown():
	pub.publish(hello)
	rate.sleep()
	print("I send >> ",hello.data)
	rospy.signal_shutdown("Fuck !")

# def shutdown():
#    print "shutdown time!"

# rospy.on_shutdown(shutdown)

print("Greeting is end")