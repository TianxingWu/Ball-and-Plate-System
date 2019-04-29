# -*- coding: utf-8 -*-
"""
Created on Mon Jan 15 18:03:22 2018

@author: James Wu
"""

import cv2
import numpy as np
import serial

#==============================================================================
#   找中心函数
#       输入：视频帧
#       输出：小球中心坐标
#==============================================================================
def Detection(frame):
    frame = frame[0:480, 80:560] #操作ROI即region of image
    frame = cv2.GaussianBlur(frame, (15,15), 0) #高斯模糊
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    lower_orange = np.array([11,60,36])   #设置颜色阈值
    upper_orange = np.array([25,255,255])
    
    mask = cv2.inRange(hsv, lower_orange, upper_orange)
    
    #==============================================================================
    #     形态学操作
    #==============================================================================

    kernel = np.ones((3,3), np.uint8)

#    # 1.Erosion and Dilation 腐蚀与膨胀
#    erosion = cv2.erode(mask, kernel, iterations=1)     #先腐蚀：黑吞白
#    dilation = cv2.dilate(erosion, kernel, iterations=1)   #后膨胀：白吞黑
    
    # 2.Opening and Closing 开操作与闭操作
    opening = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernel)    #先开操作：去除背景白色噪点(False Positives)
    closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel)   #再闭操作：填补前景黑色孔洞(False Negatives)
    

    #==============================================================================
    #     找坐标
    #==============================================================================
    contours= cv2.findContours(closing, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[1]   # 提取轮廓
    
    # 找面积最大的contours，并作最小包围矩形
    if len(contours)>0:    
        max_contour = max(contours, key=cv2.contourArea) 
    
        rectangle = cv2.boundingRect(max_contour)   # 得到最小包围矩形信息
        x,y,w,h = rectangle
        
        rectangle_pts = np.array([[x,y],[x+w,y],[x+w,y+h],[x,y+h]], np.int32)   # 最小包围矩形各顶点
        cv2.polylines(frame, [rectangle_pts], True, (0,255,0), 3) # 绘制最小包围矩形
                    
        X = x+w//2
        Y = y+h//2
        center_pt=(X,Y)   # 小球中点坐标
        cv2.circle(frame, center_pt, 8, (0,0,255), -1)   # 绘制小球中点
    else:
         X = 10000
         Y = 10000
        
    #==============================================================================
    #     绘制参考线
    #==============================================================================
    x = 0;
    y = 0;
    w = 240;
    h = 240;
    
    rectangle_pts = np.array([[x,y],[x+w,y],[x+w,y+h],[x,y+h]], np.int32)   # 最小包围矩形各顶点
    cv2.polylines(frame, [rectangle_pts], True, (0,255,0), 2) # 绘制最小包围矩形
    
    x2 = 240;
    y2 = 240;
    rectangle_pts2 = np.array([[x2,y2],[x2+w,y2],[x2+w,y2+h],[x2,y2+h]], np.int32)   # 最小包围矩形各顶点
    cv2.polylines(frame, [rectangle_pts2], True, (0,255,0), 2) # 绘制最小包围矩形

    #==============================================================================
    #     显示
    #==============================================================================

    cv2.imshow('frame',frame)
    cv2.imshow('mask', mask)
#    cv2.imshow('erosion',erosion)
#    cv2.imshow('dilation',dilation)
    cv2.imshow('opening',opening)
    cv2.imshow('closing',closing)
    
    return X,Y
                
#==============================================================================
#   **************************主函数入口***********************************
#==============================================================================
# 设置串口参数
ser = serial.Serial()
ser.baudrate = 115200    # 设置比特率为115200bps
ser.port = 'COM4'      # 单片机接在哪个串口，就写哪个串口。这里默认接在"COM4"端口
ser.open()             # 打开串口

#先发送一个中心坐标保证初始化时板子水平
data = '#'+str('240')+'$'+str('240')+'\r\n'
ser.write(data.encode())        


cap = cv2.VideoCapture(1)

while(cap.isOpened()):
    _, frame = cap.read()
    
    X, Y = Detection(frame)
    
    if(X<10000):
        print('X = ')
        print(X)
        print('Y =')
        print(Y)
	#按照协议将形心坐标打包并发送至串口
        data = '#'+str(X)+'$'+str(Y)+'\r\n'
        ser.write(data.encode())
    

    k = cv2.waitKey(5) & 0xFF
    if k==27:   #按“Esc”退出
        break

ser.close()                                     # 关闭串口
cv2.destroyAllWindows()
cap.release()

       