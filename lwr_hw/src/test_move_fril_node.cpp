// SYS
#include <sys/mman.h>
#include <cmath>
#include <time.h>
#include <signal.h>
#include <stdexcept>

// ROS headers
#include <ros/ros.h>

// FRI headers
#include <FastResearchInterface.h>
#include <TypeIRML.h>
#include <OSAbstraction.h> // need for WaitForKBCharacter

#ifndef PI
	#define PI 3.1415926535897932384626433832795
#endif

#ifndef RAD
	#define RAD(A)	((A) * PI / 180.0 )
#endif

#ifndef DEG
	#define DEG(A)	((A) * 180.0 / PI )
#endif

#ifndef NUMBER_OF_JOINTS
	#define NUMBER_OF_JOINTS 7
#endif

void raz(float *tab, const int nb)
{
	memset(tab,0x0,nb*sizeof(float));
}

void moveToPosition(FastResearchInterface *FRI, TypeIRML *RML, TypeIRMLInputParameters *IP, TypeIRMLOutputParameters *OP)
{
	int resultValue = TypeIRML::RML_WORKING;
	
	float jointValuesInRad[NUMBER_OF_JOINTS];

    while ((FRI->IsMachineOK()) && (resultValue != TypeIRML::RML_FINAL_STATE_REACHED))
    {
        FRI->WaitForKRCTick();

        resultValue	=	RML->GetNextMotionState_Position(*IP,OP);

        if ((resultValue != TypeIRML::RML_WORKING) && (resultValue != TypeIRML::RML_FINAL_STATE_REACHED))
        {
            std::cout << "MoveToReasonableStartPosition(): ERROR during trajectory generation " << resultValue << std::endl;
        }

        for ( int i = 0; i < NUMBER_OF_JOINTS; i++)
        {
            jointValuesInRad[i] = RAD((double)(OP->NewPosition->VecData[i]));
        }

        FRI->SetCommandedJointPositions(jointValuesInRad);

        *(IP->CurrentPosition) = *(OP->NewPosition);
        *(IP->CurrentVelocity) = *(OP->NewVelocity);
    }
	
}

int main( int argc, char** argv )
{
  // initialize ROS
  ros::init(argc, argv, "test_move_fril_node", ros::init_options::NoSigintHandler);

  // ros spinner
  ros::AsyncSpinner spinner(1);
  spinner.start();

  // create a node
  ros::NodeHandle lwr_nh;

  // get params or give default values
  std::string file;
  std::string name;
  lwr_nh.param("/fri_driver_file", file, std::string(""));
  lwr_nh.param("/name", name, std::string("lwr"));

  ROS_INFO("fril node -> driver file = %s, name = %s",file.c_str(),name.c_str());
  

  FastResearchInterface	*FRI;
	int resultValue	= 0;
	char c;

	FRI = new FastResearchInterface(file.c_str());

	std::cout << "\nRobotJointImpedanceController object created..." << std::endl;
	
	std::cout << "\nPlease press any key..." << std::endl;

	c = WaitForKBCharacter(NULL);

	unsigned int controlScheme = FastResearchInterface::JOINT_POSITION_CONTROL;

	std::cout << "Control strategy set to joint position control." << std::endl;

	std::cout << "Start Robot !" << std::endl;

	resultValue = FRI->StartRobot(controlScheme);
	
	if (resultValue != EOK)
	{
		std::cout << "\nAn error occurred during starting up the robot..." << std::endl;
	}

	std::cout << "Result of Start Robot = " << resultValue << std::endl;
	
	
	float jointValuesInRad[NUMBER_OF_JOINTS];

    double cycleTime = 0.002;

    TypeIRML *RML = NULL;

    TypeIRMLInputParameters *IP = NULL;

    TypeIRMLOutputParameters *OP=NULL;

    RML = new TypeIRML(NUMBER_OF_JOINTS,cycleTime);

    IP = new TypeIRMLInputParameters(NUMBER_OF_JOINTS);

    OP = new TypeIRMLOutputParameters(NUMBER_OF_JOINTS);

    raz(jointValuesInRad,NUMBER_OF_JOINTS);
    
   std::cout << "Moving to the a reasonable start position.." << std::endl;

    double targetJointPosition[7];
    
    for (int j=0; j<4; ++j)
    {
   
		FRI->GetMeasuredJointPositions(jointValuesInRad);
		 
		targetJointPosition[0] = 0.0;
		targetJointPosition[1] = 27;
		targetJointPosition[2] = 0.0;
		targetJointPosition[3] = -100;
		targetJointPosition[4] = 0.0;
		targetJointPosition[5] = -27.5;
		targetJointPosition[6] = 0.0;
		

		for ( int i = 0; i < NUMBER_OF_JOINTS; i++)
		{
			IP->CurrentPosition->VecData[i] = (double)DEG(jointValuesInRad[i]);
			IP->TargetPosition->VecData[i]	= (double)targetJointPosition[i];
			IP->MaxVelocity->VecData[i] = (double)50.0;
			IP->MaxAcceleration->VecData[i] = (double)20.0;
			IP->SelectionVector->VecData[i] = true;
		}

		moveToPosition(FRI, RML, IP, OP);
		
		FRI->GetMeasuredJointPositions(jointValuesInRad);
		
		targetJointPosition[0] = 0.0;
		targetJointPosition[1] = 1.55;
		targetJointPosition[2] = 0.0;
		targetJointPosition[3] = 0.0;
		targetJointPosition[4] = 0.0;
		targetJointPosition[5] = 0.0;
		targetJointPosition[6] = 0.0;
		
		for ( int i = 0; i < NUMBER_OF_JOINTS; i++)
		{
			IP->CurrentPosition->VecData[i] = (double)DEG(jointValuesInRad[i]);
			IP->TargetPosition->VecData[i]	= (double)DEG(targetJointPosition[i]);
			IP->MaxVelocity->VecData[i] = (double)75.0;
			IP->MaxAcceleration->VecData[i] = (double)50.0;
			IP->SelectionVector->VecData[i] = true;
		}
		
		moveToPosition(FRI, RML, IP, OP);
	}
    
    if (!FRI->IsMachineOK())
    {
        std::cout << "MoveToReasonableStartPosition(): ERROR, machine is not ready." << std::endl;
    }
    
	resultValue	= FRI->StopRobot();	

	if (resultValue != EOK)
	{
		std::cout << "\nAn error occurred during stopping the robot..." << std::endl;
	}
	
	delete	RML;
    delete	IP;
    delete	OP;
	
	std::cout << "\nPlease press any key..." << std::endl;

	c = WaitForKBCharacter(NULL);
	
	std::cout << "\nDeleting the object..." << std::endl;
	
	delete FRI;
	
	std::cout << "\nObject deleted..." << std::endl;
  
  
  return 0;
}
