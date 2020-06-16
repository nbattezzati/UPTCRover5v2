/*
 * rover_controllers.h
 *
 *  Created on: 3/06/2020
 *      Author: andre
 */

#ifndef ROVER_POSE_CONTROLLER_H_
#define ROVER_POSE_CONTROLLER_H_


#include "rover_pid.h"


/* Mobile robot pose */
typedef struct {
	float x;
	float y;
	float theta;
} rover_pose_t;

/* Mobile robot velocities */
typedef struct {
	float v;
	float w;
} rover_velocities_t;


typedef enum {
	RoverPoseControl_PID,
	RoverPoseControl_Lyapunov
} rover_pose_control_t;





typedef struct {
	/* Controller gains */
	float K1;
	float K2;
	float Q1;
	float Q2;
} rover_lyapunov_const_t;


typedef union {
	rover_lyapunov_const_t * lyapunov;
	rover_PID_const_t * pid;
} rover_pose_control_params_t;


void PoseController_Init(const rover_pose_control_t type, const rover_pose_control_params_t * params);
void PoseController_Run(
	const rover_pose_control_t type,
	const rover_pose_control_params_t * params,
	const rover_pose_t * pose,
	const rover_pose_t * desired_pose,
	rover_pose_t * pose_error,
	rover_velocities_t * velocities);


#endif /* ROVER_POSE_CONTROLLER_H_ */
