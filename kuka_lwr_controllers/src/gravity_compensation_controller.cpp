/*
 *  Laurent LEQUIEVRE
 *  Institut Pascal UMR6602
 *  laurent.lequievre@univ-bpclermont.fr
 * 
*/

#include <gravity_compensation_controller.h>

// For plugin
#include <pluginlib/class_list_macros.h>

#include <algorithm>


namespace kuka_lwr_controllers 
{
    GravityCompensationController::GravityCompensationController() {}
    GravityCompensationController::~GravityCompensationController() {}

    bool GravityCompensationController::init(hardware_interface::KUKAJointInterface *robot, ros::NodeHandle &n)
    {
		#if TRACE_ACTIVATED
			ROS_INFO("GravityCompensationController: Start init of robot %s !",robot_namespace_.c_str());
		#endif
		
		robot_namespace_ = n.getNamespace();
			
        if( !(KinematicChainControllerBase<hardware_interface::KUKAJointInterface>::init(robot, n)) )
        {
            ROS_ERROR("GravityCompensationController: Couldn't initilize GravityCompensationController controller of robot %s!",robot_namespace_.c_str());
            return false;
        }

		return true;
	}
	
	void GravityCompensationController::starting(const ros::Time& time)
    {
        
		#if TRACE_ACTIVATED
			ROS_INFO("GravityCompensationController: Starting of robot %s !",robot_namespace_.c_str());
		#endif
    }
    
    void GravityCompensationController::stopping(const ros::Time& time)
	{
		#if TRACE_ACTIVATED
			ROS_INFO_NAMED("GravityCompensationController: Stopping of robot %s !",robot_namespace_.c_str());
		#endif
		
	}
	
    void GravityCompensationController::update(const ros::Time& time, const ros::Duration& period)
    {	
		
		// update the commanded position to the actual, so that the robot doesn't 
        // go back at full speed to the last commanded position when the stiffness 
        // is raised again
        for(size_t i=0; i<joint_handles_.size(); i++) 
        {
			joint_handles_[i].setCommandPosition(joint_handles_[i].getPosition());
            joint_handles_[i].setCommandTorque(0.0); // Set a value of torque to 0.0 for each joint.
            joint_handles_[i].setCommandStiffness(200.0);
            joint_handles_[i].setCommandDamping(0.7);
        }
        
	}
	
}

PLUGINLIB_EXPORT_CLASS(kuka_lwr_controllers::GravityCompensationController, controller_interface::ControllerBase)
