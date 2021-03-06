/*
 *  Laurent LEQUIEVRE
 *  Institut Pascal UMR6602
 *  laurent.lequievre@univ-bpclermont.fr
 * 
*/

#ifndef LWR_CONTROLLERS_GRAVITY_COMPENSATION_CONTROLLER_H
#define LWR_CONTROLLERS_GRAVITY_COMPENSATION_CONTROLLER_H

// Controller base
#include "kinematic_chain_controller_base.h"

// hardware_interface 
#include <lwr_hw/lwr_kuka_interface.h> // contains definition of KUKAJointInterface

// msgs Float64MultiArray
#include <std_msgs/Float64MultiArray.h>

#include <string>

#define TRACE_ACTIVATED 0

namespace kuka_lwr_controllers
{
	class GravityCompensationController: 
	public controller_interface::KinematicChainControllerBase<hardware_interface::KUKAJointInterface>
	{
		public:
			GravityCompensationController();
			~GravityCompensationController();

			bool init(hardware_interface::KUKAJointInterface *robot, ros::NodeHandle &n); // Init the controller
			void starting(const ros::Time& time);  // Start the controller
			void stopping(const ros::Time& time);  // Stop the controller
			void update(const ros::Time& time, const ros::Duration& period);  // Update the controller
			
		private:
			std::string robot_namespace_;
	};
}


#endif
