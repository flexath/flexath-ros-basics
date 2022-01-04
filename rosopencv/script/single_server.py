#!/usr/bin/env python

from __future__ import print_function
from __future__ import division
import cv2
import numpy as np
import rospy
from rosopencv.srv import *
from rosopencv.msg import Angle
import math
import numpy as np
import time
from std_msgs.msg import Int32
import pyttsx3
import imutils

def single_obj_callback(request):
	
	rospy.loginfo("Single Request is started")

	X = request.singleX                                                    # desired x postion of end effector in cm
	Y = request.singleY                                                    # desired y postion of end effector in cm
	r1 = 0.0                                                     			 # to use in Equation 2
	a2 = 10                                                      			 # Link 2
	a4 = 8                                                       			 # Link 4

	# if elbow == 1:

	# 	# For Finding Theat 1
	# 	r1 = math.sqrt(X**2+Y**2)                                    # Equation 2
	# 	phil3 = math.atan(Y/X)                                       # Equation 1
	# 	#a4**2 = (a2**2)+(r1**2)-(2*a2*r1*(math.cos(phil1)))         # Equation 3 ( It isn't used)
	# 	phil1 = math.acos(((a2**2)+(r1**2)-(a4**2))/(2*a2*r1))       # Equation 7 ( It is derived from Equation 3)
	# 	theta1 = phil1+phil3                                         # Equation 4 ( In radians)

	# 	# For Finding Theta 2
	# 	#r1**2 = (a2**2)+(a4**2)-(2*a2*a4*(math.cos(phil2)))         # Equation 5 ( It isn't used)
	# 	phil2 = math.acos(((a2**2)+(a4**2)-(r1**2))/(2*a2*a4))       # Equation 8 ( It is derived from Equation 5)
	# 	theta2 = phil2-np.radians(180)                               # Equation 6 ( In radians)

	# 	theta1 = np.degrees(theta1)
	# 	theta2 = np.degrees(theta2)
	# 	print("First servo angle : " , theta1 , "Second Servo angle : " , theta2)

	# For Finding Theta 1 (This is elbow down)
	r1 = np.sqrt(X**2+Y**2)                                    # Equation 2
	phil3 = np.arctan(Y/X)                                       # Equation 1
	#a4**2 = (a2**2)+(r1**2)-(2*a2*r1*(math.cos(phil1)))         # Equation 3 ( It isn't used)

	up1 = float((a2**2+r1**2)-(a4**2))
	down1 = float(2*a2*r1)
	phil1 = np.arccos(up1/down1)     # Equation 7 ( It is derived from Equation 3)

	theta1 = phil1+phil3                                         # Equation 4 ( In radians)

	# For Finding Theta 2
	#r1**2 = (a2**2)+(a4**2)-(2*a2*a4*(math.cos(phil2)))         # Equation 5 ( It isn't used)
	up2 = float((a2**2+a4**2)-(r1**2))
	down2 = float(2*a2*a4)
	phil2 = np.arccos(up2/down2)       # Equation 8 ( It is derived from Equation 5)
	theta2 = phil2-np.radians(180)                               # Equation 6 ( In radians)

	theta1 = np.degrees(theta1)
	theta2 = np.degrees(abs(theta2))
	print("First servo angle : " , theta1)
	print("Second Servo angle : " , theta2)
	pub = rospy.Publisher("/angle" , Angle , queue_size=10)

	msg = Angle()
	msg.angle1 = theta1
	msg.angle2 = theta2
	pub.publish(msg)
	rospy.loginfo("One Request is done")
	print('----------')
	print("\n")

	return singleObjLocationResponse(theta1,theta2)

if __name__ == "__main__":
	rospy.init_node("single_server_node")
	rospy.Service("/single_detection_service",singleObjLocation,single_obj_callback)
	print("The single detection service is ready")
	rospy.spin()