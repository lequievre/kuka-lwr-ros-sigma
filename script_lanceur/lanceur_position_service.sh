#!/bin/bash

# Laurent LEQUIEVRE
# laurent.lequievre@uca.fr
# UMR 6602 - Institut Pascal

do_start()
{
	cd /home/ifma/git_project/kuka-lwr-ros-sigma
	source devel/setup.bash
	rosservice call /lwr/controller_manager/switch_controller "{start_controllers: ['kuka_group_command_controller_fri'], stop_controllers: [], strictness: 2}"
}

do_stop()
{
	cd /home/ifma/git_project/kuka-lwr-ros-sigma
	source devel/setup.bash
	rosservice call /lwr/controller_manager/switch_controller "{start_controllers: [], stop_controllers: ['kuka_group_command_controller_fri'], strictness: 1}"
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
