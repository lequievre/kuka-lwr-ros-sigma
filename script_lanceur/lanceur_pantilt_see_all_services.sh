#!/bin/bash

# Laurent LEQUIEVRE
# laurent.lequievre@uca.fr
# UMR 6602 - Institut Pascal


source /opt/ros/indigo/setup.bash
export ROS_MASTER_URI=http://ifma-kuka-test:11311
rosservice call /lwr/controller_manager/list_controllers

echo "Appuyer la touche <Entrée> pour continuer..."
read touche




