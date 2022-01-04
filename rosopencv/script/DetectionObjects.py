#!/usr/bin/env python

from __future__ import print_function
import cv2
import numpy as np
import math
from Tkinter import *
from PIL import ImageTk , Image
import imutils
import time

cap = cv2.VideoCapture(0)

def single_detection():

    """ Add Homogeneous Matrix to match camera frame and base frame related to object location"""

    pixel_to_cm_x_axis = (11.3/640)
    pixel_to_cm_y_axis = (11.3/480)

    while cap.isOpened():
        switch = False

        _,frame = cap.read()
        crop = frame[92:295 , 109:451]

        gray = cv2.cvtColor(crop,cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray,(3,3),0)
        _,threshold = cv2.threshold(blur,150,255,cv2.THRESH_BINARY)

        kernel = np.ones((5, 5))
        opening = cv2.morphologyEx(threshold,cv2.MORPH_OPEN,kernel, iterations=1)

        cnts = cv2.findContours(opening,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)
        #print(len(contour))

        objects = []
        for cnt in cnts:
            (x,y,w,h) = cv2.boundingRect(cnt)

            area = cv2.contourArea(cnt)

            if area > 500 :
                #print(len(cnts))
                moments = cv2.moments(cnt)
                cx = int(moments["m10"] / moments["m00"])
                cy = int(moments["m01"] / moments["m00"])
                #print("Center : ",cx,cy)
                objects.append((cx,cy))
                cv2.circle(crop, (cx,cy) , 7 , (255,0,0) , -1)
                cv2.rectangle(crop , (x,y) , (x+w,y+h) , (0,0,255) , 2)

                _, labels = cv2.connectedComponents(opening, 4)        # For finding numbers of objects
                #print("Number of objects : ",np.amax(labels))                                 # to print numbers of objects

            cv2.putText(crop,str(area) , (x,y) , 1 , 1, (0,255,0))

        for x,y in objects:
            world_units_x_axis = x * pixel_to_cm_x_axis
            world_units_y_axis = y * pixel_to_cm_y_axis

        cv2.imshow("Original" , frame)
        cv2.imshow("Crop Object", crop)
        #cv2.imshow("Threshold Object", threshold)
        cv2.imshow("Opening" , opening)

        # switch = True
        # if switch == True:
        #    break
        key = cv2.waitKey(1)
        if key == 27:
            break

    return world_units_x_axis,world_units_y_axis

def multiple_detection():

    pixel_to_cm_x_axis = (11.3/640)
    pixel_to_cm_y_axis = (11.3/480)

    while cap.isOpened():
        switch = False

        _,frame = cap.read()
        crop = frame[92:295 , 109:451]

        gray = cv2.cvtColor(crop,cv2.COLOR_BGR2GRAY)
        blur = cv2.GaussianBlur(gray,(3,3),0)
        _,threshold = cv2.threshold(blur,100,255,cv2.THRESH_BINARY)

        kernel = np.ones((5, 5))
        opening = cv2.morphologyEx(threshold,cv2.MORPH_OPEN,kernel, iterations=1)

        cnts = cv2.findContours(opening,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)
        #print(len(contour))

        objects = []
        for cnt in cnts:
            (x,y,w,h) = cv2.boundingRect(cnt)

            area = cv2.contourArea(cnt)

            if area > 500 :
                #print(len(cnts))
                moments = cv2.moments(cnt)
                cx = int(moments["m10"] / moments["m00"])
                cy = int(moments["m01"] / moments["m00"])
                #print("Center : ",cx,cy)
                objects.append((cx,cy))
                cv2.circle(crop, (cx,cy) , 7 , (255,0,0) , -1)
                cv2.rectangle(crop , (x,y) , (x+w,y+h) , (0,0,255) , 2)

                _, labels = cv2.connectedComponents(opening, 4)        # For finding numbers of objects
                #print("Number of objects : ",np.amax(labels))                                 # to print numbers of objects

            cv2.putText(crop,str(area) , (x,y) , 1 , 1, (0,255,0))

        locations = []
        for x,y in objects:
            world_units_x_axis = x * pixel_to_cm_x_axis
            world_units_y_axis = y * pixel_to_cm_y_axis
            locations.append([world_units_x_axis,world_units_y_axis])

        #print("Obects Locations in Pixels : " , objects)
        print("Obects Locations in cm     : " , locations)
        cv2.imshow("Original" , frame)
        cv2.imshow("Crop Object", crop)
        #cv2.imshow("Threshold Object", threshold)
        cv2.imshow("Opening" , opening)

        # switch = True
        # if switch == True:
        #    break
        key = cv2.waitKey(1)
        if key == 27:
            break

    return locations

def color_detection():
    pixel_to_cm_x_axis = (11.3/640)
    pixel_to_cm_y_axis = (11.3/480)

    while cap.isOpened():
        switch = False

        _,frame = cap.read()
        crop = frame[92:295 , 109:451]

        hsv = cv2.cvtColor(crop,cv2.COLOR_BGR2HSV)

        def white_detection():
            gray = cv2.cvtColor(crop, cv2.COLOR_BGR2GRAY)
            blur = cv2.GaussianBlur(gray, (3, 3), 0)
            _, threshold = cv2.threshold(blur, 150, 255, cv2.THRESH_BINARY)

            kernel = np.ones((5, 5))
            opening = cv2.morphologyEx(threshold, cv2.MORPH_OPEN, kernel, iterations=1)

            cnts = cv2.findContours(opening, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            cnts = imutils.grab_contours(cnts)
            # print(len(contour))

            objects_white = []
            for cnt in cnts:
                white_switch = False
                (x, y, w, h) = cv2.boundingRect(cnt)

                area = cv2.contourArea(cnt)
                if area > 1000:
                    # print(len(cnts))
                    moments = cv2.moments(cnt)
                    cx = int(moments["m10"] / moments["m00"])
                    cy = int(moments["m01"] / moments["m00"])
                    #print("White Center : ", cx, cy)
                    objects_white.append([cx, cy])
                    cv2.circle(crop, (cx, cy), 4, (0, 0, 255), -1)
                    cv2.rectangle(crop, (x, y), (x + w, y + h), (0, 0, 255), 2)
                    cv2.putText(crop, "White", (x, y), 1, 1, (255, 255, 255))
                    white_switch = True
                    if white_switch == True:
                        break

            white_locations = []
            for x,y in objects_white:
                world_units_x_axis = x * pixel_to_cm_x_axis
                world_units_y_axis = y * pixel_to_cm_y_axis
                white_locations.append([world_units_x_axis,world_units_y_axis])

            return white_locations


        def blue_detection():

            low_blue = np.array([90,150,100],np.uint8)
            upper_blue = np.array([121,255,255],np.uint8)
            blue = cv2.inRange(hsv,low_blue,upper_blue)
            blue_cnts = cv2.findContours(blue, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            blue_cnts = imutils.grab_contours(blue_cnts)

            objects_blue = []
            for cnt1 in blue_cnts:
                blue_switch = False
                (x, y, w, h) = cv2.boundingRect(cnt1)

                area = cv2.contourArea(cnt1)
                if area > 500:
                    # print(len(cnts1))
                    moments = cv2.moments(cnt1)
                    cx = int(moments["m10"] / moments["m00"])
                    cy = int(moments["m01"] / moments["m00"])
                    #print("Blue Center : ", cx, cy)
                    objects_blue.append([cx, cy])
                    cv2.circle(crop, (cx, cy), 4, (0, 0, 255), -1)
                    cv2.rectangle(crop, (x, y), (x + w, y + h), (0, 0, 255), 2)
                    cv2.putText(crop, "Blue", (x, y), 1, 1, (255, 255, 255))
                    blue_switch = True
                    if blue_switch == True:
                        break

            blue_locations = []
            for x,y in objects_blue:
                world_units_x_axis = x * pixel_to_cm_x_axis
                world_units_y_axis = y * pixel_to_cm_y_axis
                blue_locations.append([world_units_x_axis,world_units_y_axis])

            return blue_locations

        def green_detection():

            low_green = np.array([40, 70, 80],np.uint8)
            upper_green = np.array([70, 255, 255],np.uint8)
            green = cv2.inRange(hsv, low_green, upper_green)
            green_cnts = cv2.findContours(green, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            green_cnts = imutils.grab_contours(green_cnts)

            objects_green = []
            for cnt2 in green_cnts:
                green_switch = False
                (x, y, w, h) = cv2.boundingRect(cnt2)

                area = cv2.contourArea(cnt2)
                if area > 500:
                    # print(len(cnts2))
                    moments = cv2.moments(cnt2)
                    cx = int(moments["m10"] / moments["m00"])
                    cy = int(moments["m01"] / moments["m00"])
                    #print("Green Center : ", cx, cy)
                    objects_green.append([cx, cy])
                    cv2.circle(crop, (cx, cy), 4, (0, 0, 255), -1)
                    cv2.rectangle(crop, (x, y), (x + w, y + h), (0, 0, 255), 2)
                    cv2.putText(crop, "Green", (x, y), 1, 1, (255, 255, 255))
                    green_switch = True
                    if green_switch == True:
                        break

            green_locations = []
            for x,y in objects_green:
                world_units_x_axis = x * pixel_to_cm_x_axis
                world_units_y_axis = y * pixel_to_cm_y_axis
                green_locations.append([world_units_x_axis,world_units_y_axis])

            return green_locations

        def red_detection():

            """ Alert :> Red Color is sensitive for detection , so don't use !"""

            low_red = np.array([0,50,120])
            upper_red = np.array([10,255,255])
            red = cv2.inRange(hsv,low_red,upper_red)
            red_cnts = cv2.findContours(red, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
            red_cnts = imutils.grab_contours(red_cnts)

            objects_red = []
            for cnt3 in red_cnts:
                red_switch =  False
                (x, y, w, h) = cv2.boundingRect(cnt3)

                area = cv2.contourArea(cnt3)
                if area > 500:
                    # print(len(cnts3))
                    moments = cv2.moments(cnt3)
                    cx = int(moments["m10"] / moments["m00"])
                    cy = int(moments["m01"] / moments["m00"])
                    #print("Red Center : ", cx, cy)
                    objects_red.append([cx, cy])
                    cv2.circle(crop, (cx, cy), 4, (0, 0, 0), -1)
                    cv2.rectangle(crop, (x, y), (x + w, y + h), (0, 0, 255), 2)
                    cv2.putText(crop, "Red", (x, y), 1, 1, (255, 255, 255))
                    red_switch = True
                    if red_switch == True:
                        break

            red_locations = []
            for x,y in objects_red:
                world_units_x_axis = x * pixel_to_cm_x_axis
                world_units_y_axis = y * pixel_to_cm_y_axis
                red_locations.append([world_units_x_axis,world_units_y_axis])

            return red_locations

        white = white_detection()
        green = green_detection()
        blue = blue_detection()
        #red = red_detection()
        locations = []
        for i in green,blue,white:
            for ii in i:
                locations.append(ii)

        #print(locations)
        
        cv2.imshow("Original Image", frame)
        #cv2.imshow("Detection Image" , crop)
        switch = True

        #time.sleep(5)
        # if switch == True:
        #    break
        key = cv2.waitKey(1)
        if key == 27:
            break
        #cap.release()
        #cv2.destroyAllWindows()

    return green,blue,white

def camera_shutdown():
    cap.release()
    cv2.destroyAllWindows()