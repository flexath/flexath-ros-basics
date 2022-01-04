#! /usr/bin/env python

import actionlib
import rospy

from rosbasic.msg import CounterFeedback, CounterResult, CounterAction, CounterGoal

number = 1

def feedback_callback(a):
	global number 
	print("[Feedback] number {} received".format(number))
	number += 1

client = actionlib.SimpleActionClient('/counter', CounterAction)

rospy.init_node('action_client_node')
	
client.wait_for_server()

while not rospy.is_shutdown():
	
	goal = CounterGoal()
	goal.count = input('Enter a number: ')
	
	client.send_goal(goal, feedback_cb = feedback_callback)
	
	client.wait_for_result()
	print("The  result is ",client.get_result())
	print("[Result] State: {}".format(client.get_state()))
	print '---'
	number = 1