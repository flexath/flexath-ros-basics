#!/usr/bin/env python

import rospy
from rosbasic.srv import *

def callback(request):
	addition  = request.first_number + request.second_number
	subtraction = request.first_number - request.second_number
	multiplication = request.first_number * request.second_number
	division = request.first_number / request.second_number
	print(addition,subtraction,multiplication,division)
	
	return CalculationResponse(addition,subtraction,multiplication,division)
	

rospy.init_node("server_node")
rospy.Service("/calculation_service",Calculation,callback)
print("The server is ready")
rospy.spin()