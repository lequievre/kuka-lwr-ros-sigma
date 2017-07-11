#!/bin/bash

# Laurent LEQUIEVRE
# laurent.lequievre@uca.fr
# UMR 6602 - Institut Pascal

do_start()
{

	source /opt/ros/indigo/setup.bash
	export ROS_MASTER_URI=http://ifma-kuka-test:11311
	rosservice call /lwr/controller_manager/switch_controller "{start_controllers: ['kuka_one_task_inverse_kinematics'], stop_controllers: [], strictness: 2}"
}

do_stop()
{
	source /opt/ros/indigo/setup.bash
	export ROS_MASTER_URI=http://ifma-kuka-test:11311
	rosservice call /lwr/controller_manager/switch_controller "{start_controllers: [], stop_controllers: ['kuka_one_task_inverse_kinematics'], strictness: 1}"
}

case "$1" in
   start)
      do_start
      ;;
   stop)
      do_stop
      ;;
   *)
      echo "--> Usage: $0 {start|stop}"
      exit 1
esac

exit 0
