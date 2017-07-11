#!/bin/bash

# Laurent LEQUIEVRE
# laurent.lequievre@uca.fr
# UMR 6602 - Institut Pascal


cd /home/pantilt/projects/ros_sigma_platform_fri_ws
source devel/setup.bash
export ROS_MASTER_URI=http://ifma-kuka-test:11311
roslaunch single_lwr_robot right_arm_rviz.launch





