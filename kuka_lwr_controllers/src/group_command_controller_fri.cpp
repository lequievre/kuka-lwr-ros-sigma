/*
 *  Laurent LEQUIEVRE
 *  Institut Pascal UMR6602
 *  laurent.lequievre@univ-bpclermont.fr
 * 
*/

#include <group_command_controller_fri.h>

// For plugin
#include <pluginlib/class_list_macros.h>

#include <algorithm>


namespace kuka_lwr_controllers 
{
    GroupCommandControllerFRI::GroupCommandControllerFRI() {}
    GroupCommandControllerFRI::~GroupCommandControllerFRI() 
    {
			delete RML_;
			delete IP_;
			delete OP_;	
    }

    bool GroupCommandControllerFRI::init(hardware_interface::PositionJointInterface *robot, ros::NodeHandle &n)
    {
		#if TRACE_GroupCommandController_ACTIVATED
			ROS_INFO("GroupCommandControllerFRI: Start init of robot %s !",robot_namespace_.c_str());
		#endif
	
		robot_namespace_ = n.getNamespace();
			
        if( !(KinematicChainControllerBase<hardware_interface::PositionJointInterface>::init(robot, n)) )
        {
            ROS_ERROR("GroupCommandControllerFRI: Couldn't initilize GroupCommandController controller of robot %s!",robot_namespace_.c_str());
            return false;
        }
        
        cycleTime_ = 0.002;
        
        RML_ = new TypeIRML(joint_handles_.size(),cycleTime_);
	IP_ = new TypeIRMLInputParameters(joint_handles_.size());
	OP_ = new TypeIRMLOutputParameters(joint_handles_.size());

	sub_command_ = nh_.subscribe("command", 1, &GroupCommandControllerFRI::commandCB, this);

	cmd_flag_ = 0;  // set this flag to 0 to not run the update method

	return true;
	}
	
    void GroupCommandControllerFRI::starting(const ros::Time& time)
    {
		#if TRACE_GroupCommandController_ACTIVATED
			ROS_INFO("GroupCommandControllerFRI: Starting of robot %s !",robot_namespace_.c_str());
		#endif
		
		cmd_flag_ = 0;  // set this flag to 0 to not run the update method
		
    }
    
    void GroupCommandControllerFRI::stopping(const ros::Time& time)
    {
		#if TRACE_GroupCommandController_ACTIVATED
			ROS_INFO_NAMED("GroupCommandControllerFRI: Stopping of robot %s !",robot_namespace_.c_str());
		#endif
		
		cmd_flag_ = 0;  // set this flag to 0 to not run the update method
    }
	
    void GroupCommandControllerFRI::update(const ros::Time& time, const ros::Duration& period)
    {	
		if (cmd_flag_)
		{
			if (resultValue_ != TypeIRML::RML_FINAL_STATE_REACHED)
			{
				resultValue_ = RML_->GetNextMotionState_Position(*IP_,OP_);
				
				if ((resultValue_ != TypeIRML::RML_WORKING) && (resultValue_ != TypeIRML::RML_FINAL_STATE_REACHED))
				{
					ROS_INFO("GroupCommandControllerFRI::update : ERROR during trajectory generation err n°%d",resultValue_);
				}
				
				// set control command for joints
				for (int i = 0; i < joint_handles_.size(); i++)
					joint_handles_[i].setCommand(RAD((double)(OP_->NewPosition->VecData[i])));
				
				*(IP_->CurrentPosition) = *(OP_->NewPosition);
				*(IP_->CurrentVelocity) = *(OP_->NewVelocity);
				
				#if TRACE_GroupCommandController_ACTIVATED
					ROS_INFO("GroupCommandControllerFRI: resultValue_ = %d", resultValue_);
					ROS_INFO("GroupCommandControllerFRI: of robot %s -> j0=%f, j1=%f, j2=%f, j3=%f, j4=%f, j5=%f, j6=%f",robot_namespace_.c_str(),joint_des_states_.q(0), joint_des_states_.q(1), joint_des_states_.q(2), joint_des_states_.q(3), joint_des_states_.q(4), joint_des_states_.q(5), joint_des_states_.q(6));
				#endif
			}
			else
			{
				cmd_flag_=0; // all the joint values derired are reached, so set this flag to 0 to not run the update method
				ROS_INFO("GroupCommandControllerFRI: GOAL of robot %s !!!!!!!!!!",robot_namespace_.c_str());
			}
		}
	}
	
	void GroupCommandControllerFRI::commandCB(const std_msgs::Float64MultiArrayConstPtr& msg)
    	{
		#if TRACE_GroupCommandController_ACTIVATED
			ROS_INFO("GroupCommandControllerFRI: Start commandCB of robot %s!",robot_namespace_.c_str());
		#endif


		if(msg->data.size()!=joint_handles_.size())
		{ 
			ROS_ERROR_STREAM("GroupCommandControllerFRI: Dimension (of robot " << robot_namespace_.c_str() << ") of command (" << msg->data.size() << ") does not match number of joints (" << joint_handles_.size() << ")! Not executing!");
			cmd_flag_ = 0;
			return; 
		}

		for (size_t i=0; i<joint_handles_.size(); ++i)
		{
			IP_->CurrentPosition->VecData[i] = (double)DEG(joint_handles_[i].getPosition());  // set current position (transfrom to degrees) with current position of joint handles
			IP_->TargetPosition->VecData[i]	= (double)DEG(msg->data[i]); // set desired position (get it from msg data of topic)
			IP_->MaxVelocity->VecData[i] = (double)50.0;
			IP_->MaxAcceleration->VecData[i] = (double)20.0;
			IP_->SelectionVector->VecData[i] = true;
		}

		resultValue_ = TypeIRML::RML_WORKING;

		cmd_flag_ = 1; // set this flag to 1 to run the update method

		#if TRACE_GroupCommandController_ACTIVATED
			ROS_INFO("GroupCommandControllerFRI: Finish commandCB of robot %s !",robot_namespace_.c_str());
			ROS_INFO("GroupCommandControllerFRI: of robot %s -> j0=%f, j1=%f, j2=%f, j3=%f, j4=%f, j5=%f, j6=%f",robot_namespace_.c_str(),msg->data[0],msg->data[1],msg->data[2],msg->data[3],msg->data[4],msg->data[5],msg->data[6]);
		#endif
		 
	}
	
}

PLUGINLIB_EXPORT_CLASS(kuka_lwr_controllers::GroupCommandControllerFRI, controller_interface::ControllerBase)
