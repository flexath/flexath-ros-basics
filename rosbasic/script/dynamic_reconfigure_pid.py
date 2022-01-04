#!/usr/bin/env python

import rospy
from dynamic_reconfigure.server import Server
from rosbasic.cfg import PIDConfig

def dynamic_callback(config, level):
	rospy.loginfo("""Reconfigure Request: {Kp} , {Ki} , {Kd} """.format(**config))
	Kp = config["Kp"]
	Ki = config["Ki"]
	Kd = config["Kd"]
	print(Kp , Ki , Kd)
	return config


rospy.init_node("pid_node", anonymous = False)
server = Server(PIDConfig, dynamic_callback)
rospy.spin()

#To set values with gui trackbar , type this command >> rosrun rqt_gui rqt_gui -s reconfigure