# Ball-and-Plate-System
A Ball &amp; Plate System using Noise-immune PD Control

<p align="center">
    <img src="./results/test.gif">
</p>

## Introduction
This project's goal is to build a 2-DOF ball and plate system, which can balance a ping-pong ball on a flat plate.

To detect the position of the ball, We use a low-cost USB camera as the image sensor, and process the video in real time with OpenCV-Python on the PC.The coordinate is then transmitted to STM32 microcontroller where a noise-immune PD control loop is implemented, actuating 2 servos to control the pitch and roll of the plate to keep the ball in its centre.To reduce the oscillation caused by large noises, a noise-robust differentiator[1] is used to improved the PD control algorithm. Experiment results show that the system using this method has better robustness and rapidness. 

## Software Environment
* python 3.6 
- numpy 1.14.0
* opencv-python 3.4.0

## Hardware
* a USB camera
- 2X analog servos
* STM32F103ZET6 microcontroller

## Other Materials
* an acrylic plate
- several plastic sticks
* 3X universal joints
- 2X metal sticks with rod ends
* others

## Reference
[1] http://www.holoborodko.com/pavel/numerical-methods/numerical-derivative/smooth-low-noise-differentiators