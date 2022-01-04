#!/usr/bin/env python

import rospy
from rosbasic.srv import *

rospy.init_node("client_node")
rospy.wait_for_service("/calculation_service")
client = rospy.ServiceProxy("/calculation_service",Calculation)

requesting = CalculationRequest()


while not rospy.is_shutdown():
	requesting.first_number = float(input("Enter first number - "))
  	requesting.second_number = float(input("Enter second number - "))
	client(requesting)