#!/bin/bash

# Laurent LEQUIEVRE
# laurent.lequievre@uca.fr
# UMR 6602 - Institut Pascal

cd /home/ifma/projects/ros_sigma_platform_fri_ws
source devel/setup.bash
roslaunch single_lwr_robot right_arm.launch
