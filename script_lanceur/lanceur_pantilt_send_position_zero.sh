#!/bin/bash

# Laurent LEQUIEVRE
# laurent.lequievre@uca.fr
# UMR 6602 - Institut Pascal


source /opt/ros/indigo/setup.bash
export ROS_MASTER_URI=http://ifma-kuka-test:11311
rostopic pub -1 /lwr/kuka_group_command_controller_fri/command std_msgs/Float64MultiArray "data: [0.0,0.0,0.0,0.0,0.0,0.0,0.0]"





