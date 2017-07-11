#!/bin/bash

# Laurent LEQUIEVRE
# laurent.lequievre@uca.fr
# UMR 6602 - Institut Pascal


cd /home/ifma/git_project/kuka-lwr-ros-sigma
source devel/setup.bash
rostopic pub -1 /lwr/kuka_group_command_controller_fri/command std_msgs/Float64MultiArray "data: [0.9,0.9,0.5,0.5,0.3,0.5,0.8]"





