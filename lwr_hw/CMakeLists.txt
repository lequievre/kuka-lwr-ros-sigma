cmake_minimum_required(VERSION 2.8.3)
project(lwr_hw)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  control_toolbox
  controller_interface
  controller_manager
  hardware_interface
  realtime_tools
  joint_limits_interface
  roscpp
  tf
  urdf
  cmake_modules
  pluginlib
  kdl_parser
  transmission_interface
  gazebo_ros
  std_msgs
)

add_definitions (-fpermissive -std=c++11)

catkin_package(
  CATKIN_DEPENDS
    control_toolbox
    controller_interface
    controller_manager
    hardware_interface
    realtime_tools
    joint_limits_interface
    roscpp
    tf
    urdf
    cmake_modules
    pluginlib
    kdl_parser
    transmission_interface
    gazebo_ros
    std_msgs
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
)

###########
## Build ##
###########

include_directories(include
  ${Boost_INCLUDE_DIR}
  ${catkin_INCLUDE_DIRS}
)

## LWR HW BASE CLASS TO RULE'EM ALL
add_library(${PROJECT_NAME}
  include/${PROJECT_NAME}/lwr_hw.h
  include/${PROJECT_NAME}/lwr_hw_fril.hpp
  include/${PROJECT_NAME}/lwr_kuka_interface.h
  src/lwr_hw.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

# Standford FRILibrary support in PROGRESS

message(WARNING "ATTENTION: This project supports the fork of the Standford library found in https://github.com/lequievre/FRI_IP, and install the libraries in your catkin workspace, not in the system")
include(ExternalProject)

ExternalProject_Add(FRILibrary
	GIT_REPOSITORY https://github.com/lequievre/FRI_IP.git
	GIT_TAG master
	BUILD_COMMAND make
	BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND cmake -DCMAKE_INSTALL_PREFIX=${CATKIN_DEVEL_PREFIX}
	INSTALL_COMMAND make install
)

set(FRIL_DEFS -D__LINUX__)
add_definitions(${FRIL_DEFS})

# lwr hw fril node
include_directories(${CATKIN_DEVEL_PREFIX}/include/fril ${CATKIN_DEVEL_PREFIX}/include/RML)
link_directories(${CATKIN_DEVEL_PREFIX}/lib/x86_64-linux-gnu)
add_executable(lwr_hw_fril_node  src/lwr_hw_fril_node.cpp
  include/${PROJECT_NAME}/lwr_hw_fril.hpp
)

add_dependencies(lwr_hw_fril_node
  FRILibrary
  ${PROJECT_NAME}
)

target_link_libraries(lwr_hw_fril_node
  FastResearchInterfaceLibrary
  ${PROJECT_NAME}
  ${catkin_LIBRARIES}
)

#target_link_libraries(lwr_hw_fril_node
#  FastResearchInterfaceLibrary_rt.a
#  ${PROJECT_NAME}
#  ${catkin_LIBRARIES}
#)

 #set_target_properties( lwr_hw_fril_node PROPERTIES
 #                    COMPILE_FLAGS "-DRTNET -D_GNU_SOURCE -D_REENTRANT -D__XENO__" 
 #                    LINK_FLAGS "-Wl,@/usr/xenomai_2_6_5/lib/posix.wrappers")

# test fril move node
#add_executable(test_move_fril_node  src/test_move_fril_node.cpp)
#target_link_libraries(test_move_fril_node
#  FastResearchInterfaceLibrary_rt
#  TypeIRML
#  ${PROJECT_NAME}
#  ${catkin_LIBRARIES}
#)

#set_target_properties( test_move_fril_node PROPERTIES
#                     COMPILE_FLAGS "-DRTNET -D_GNU_SOURCE -D_REENTRANT -D__XENO__" 
#                     LINK_FLAGS "-Wl,@/usr/xenomai_2_6_5/lib/posix.wrappers")


