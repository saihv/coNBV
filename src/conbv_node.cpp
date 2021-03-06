#pragma once

#include "libcmaes/cmaes.h"
#include <iostream>
#include "conbv/rosUtils.hpp"
#include "conbv/Optimizer.hpp"
#include "conbv/visionUtils.hpp"
#include "ros/ros.h"

using namespace libcmaes;

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "conbv_node");
  	ros::NodeHandle nh;

	int nDrones = 2;

  	ROSUtils rosUtils(nDrones);

  	ros::Subscriber sub = nh.subscribe<PointCloud>("map", 1, &ROSUtils::mapCallback, &rosUtils);

	for (int droneIdx = 0; droneIdx < nDrones; droneIdx++) {
		std::string topicName = "coloc/drone" + std::to_string(droneIdx) + "/pose";
		ros::Subscriber sub = nh.subscribe<geometry_msgs::PoseStamped>(topicName, 1, boost::bind(&ROSUtils::poseCallback, &rosUtils, _1, droneIdx));
	}

  	rosUtils.mapAvailable = false;
	rosUtils.posesAvailable = false;

  	while(!rosUtils.mapAvailable)
    	ros::spinOnce();

	Optimizer optimizer(nDrones, rosUtils.mapPoints);

	optimizer.setupProblem();
  	optimizer.run();
}