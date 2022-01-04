#!/usr/bin/env python

from __future__ import print_function
import rospy
from rosopencv.srv import *
from rosopencv.msg import *
import cv2
import numpy as np
import math
import time
from Tkinter import *
from PIL import ImageTk , Image
import pyttsx3
import imutils
import sys
import os
from DetectionObjects import *

world_units_x_axis = 0
world_units_x_axis = 0

root = Tk()
def login_level():

        top = Toplevel()
        top.geometry("400x400")
        top.title("Pick and Place")
        top.configure(bg="#003153")

        label1 = Label(top,text="Username :" , font = {'Helevtica,10'})
        entry1 = Entry(top)
        label2 = Label(top,text="Password :" , font = {'Helevtica,10'})
        entry2 = Entry(top , show="*")

        def exit():
            top.destroy()
            root.destroy()
            sys.exit()

        button = Button(top,text = 'Cancel' , command = exit)

        def login(event):
            if entry1.get() == 'a' and entry2.get() == '1':
                engine = pyttsx3.init()
                engine.say(" Correct ")
                engine.runAndWait()
                root.deiconify()
                top.destroy()
            else:
                engine = pyttsx3.init()
                rate = engine.getProperty('rate')  # getting details of current speaking rate
                engine.setProperty('rate', 180)

                engine.say("Wrong. Try Again")
                engine.runAndWait()
                label = Label(top , text = "Wrong ! Try Again" , fg = "red").pack()

        entry2.bind('<Return>', login)

        label1.pack()
        entry1.pack()
        label2.pack()
        entry2.pack()
        button.pack()

login_level()
root.title("Pick and Place")
root.geometry("500x500")
root.configure(bg="#003153")
root.withdraw()

frame = LabelFrame(root,text = "Choose Modes" , padx=120 , pady=120 , bg = "#003153" , fg = "red")
frame.pack(padx=10 , pady=10)

rospy.init_node("obj_client_node")

def single():
    label = Label(frame,text="Single Object Detection")
    label.pack()

    rospy.wait_for_service("/single_detection_service")
    client_single = rospy.ServiceProxy("/single_detection_service",singleObjLocation)

    requesting_single = singleObjLocationRequest()
    rate = rospy.Rate(10)

    requesting_single.singleX , requesting_single.singleY = single_detection()
    camera_shutdown()
    print("Location is " , requesting_single.singleX , requesting_single.singleY)

    while not rospy.is_shutdown():
        rospy.loginfo("Object Single Client Node is started")
        client_single(requesting_single)
        time.sleep(3)
        rate.sleep()
        break
    print("End of Single Callback !")
    print("-------------------------------")
    return 0

def multiple():
    label = Label(frame,text="Multiple Object Detection")
    label.pack()

    rospy.wait_for_service("/multiple_detection_service")
    client_multiple = rospy.ServiceProxy("/multiple_detection_service",multipleObjLocation)

    requesting_multiple = multipleObjLocationRequest()
    rate = rospy.Rate(10)

    locations = multiple_detection()
    camera_shutdown()
    print("Locations are " , locations)

    count = 0
    while not rospy.is_shutdown():
        rospy.loginfo("Object Multiple Client Node is started")

        for x,y in locations:
            requesting_multiple.multipleX = x
            requesting_multiple.multipleY = y
            client_multiple(requesting_multiple)
            time.sleep(3)
            print("\n")
            count+=1
            if count == len(locations)+1:
                break
            rate.sleep()
        break
        # for i in locations:
        #     requesting_multiple.multipleX = i[0]
        #     requesting_multiple.multipleY = i[1]
        #     client_multiple(requesting_multiple)
        
        # for i in range(count,len(locations)):
        #     if i == count:
        #         requesting_multiple.multipleX = locations[count][0]
        #         requesting_multiple.multipleY = locations[count][1]
        #         client_multiple(requesting_multiple)
        #         break
        # count+=1
    print("End of Multiple Callback !")
    print("-------------------------------")
    return 0


def color():
    label = Label(frame,text="Colored Object Detection")
    label.pack()

    rospy.wait_for_service("/color_detection_service")
    client_color = rospy.ServiceProxy("/color_detection_service",colorObjLocation)

    rate = rospy.Rate(10)
    green,blue,white = color_detection()
    camera_shutdown()
    print("Green Location :" , green)
    print("Blue Location :" , blue)
    print("White Location :" , white)

    count = 0
    while not rospy.is_shutdown():
        rospy.loginfo("Object Color Client Node is started")
        requesting_color = colorObjLocationRequest()
        #camera_shutdown()
        #print("1 - White")
        print("1 - Green")
        print("2 - Blue")
        print("3 - White")
        color = int(input("Enter the color which you want to pick : "))

        if color == 1:
            requesting_color.colorX = green[0][0]
            requesting_color.colorY = green[0][1]
            print("Green Location in cm :",green[0][0])
            print("Green Location in cm :",green[0][1])
            client_color(requesting_color)
            rate.sleep()

        if color == 2:
            requesting_color.colorX = blue[0][0]
            requesting_color.colorY = blue[0][1]
            print("Blue Location in cm :",blue[0][0])
            print("Blue Location in cm :",blue[0][1])
            client_color(requesting_color)
            rate.sleep()

        if color == 3:
            requesting_color.colorX = white[0][0]
            requesting_color.colorY = white[0][1]
            print("Blue Location in cm ",white[0][0])
            print("Blue Location in cm ",white[0][1])
            client_color(requesting_color)
            rate.sleep()
        count+=1
        if count == 3:
            break
        print('---')
    print("End of Color Callback")
    print("-------------------------------")
    return 0

def clear():
    python = sys.executable
    os.execl(python, python, * sys.argv)

r = IntVar()

radio1 = Radiobutton(frame,text = "Single Object Detection" , padx=50 , pady=10 ,  bg = '#003153', variable=r , value=1 , command = single)
radio1.pack()

radio2 = Radiobutton(frame,text = "Multiple Object Detection" , padx=50 , pady=10 ,  bg = '#003153', variable=r , value=2 , command = multiple)
radio2.pack()

radio3 = Radiobutton(frame,text = "Colored Object Detection" , padx=50 , pady=10 ,  bg = '#003153', variable=r , value=3 , command = color)
radio3.pack()

button = Button(frame,text="Reset" , bg = "black" , fg = "red" , command=clear)
button.pack()

root.mainloop()