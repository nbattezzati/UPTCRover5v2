/*
 * rover_controllers.c
 *
 *  Created on: 3/06/2020
 *      Author: andre
 */

#include <math.h>
#include "rover_pose_controller.h"

////////////////////////////
///                      ///
///       DEFINES        ///
///                      ///
////////////////////////////

////////////////////////////
///                      ///
///   PUBLIC VARIABLES   ///
///                      ///
////////////////////////////

////////////////////////////
///                      ///
///   PRIVATE VARIABLES  ///
///                      ///
////////////////////////////

////////////////////////////
///                      ///
///    PUBLIC TYPES      ///
///                      ///
////////////////////////////

////////////////////////////
///                      ///
///    PRIVATE TYPES     ///
///                      ///
////////////////////////////

////////////////////////////
///                      ///
/// FUNCTION PROTOTYPES  ///
///                      ///
////////////////////////////

void PIDPoseControl(
	const rover_PID_const_t * params,
	const rover_pose_t * pose,
	const rover_pose_t * desired_pose,
	rover_pose_t * error_pose,
	rover_velocities_t * velocities
);

void LyapunovControl_Init (const rover_lyapunov_const_t * params);
void LyapunovControl (
	const rover_lyapunov_const_t * params,
	const rover_pose_t * pose,
	const rover_pose_t * desired_pose,
	rover_pose_t * error_pose,
	rover_velocities_t * velocities
);


////////////////////////////
///                      ///
///   PUBLIC FUNCTIONS   ///
///                      ///
////////////////////////////


void PoseController_Init(rover_pose_control_t type, const rover_pose_control_params_t * params)
{
	switch(type) {
	case RoverPoseControl_PID:
		PIDControl_Init(params->pid);
	break;

	case RoverPoseControl_Lyapunov:
		LyapunovControl_Init(params->lyapunov);
	break;

	default:
		// TODO: error
	break;
	}
}


void PoseController_Run(
	const rover_pose_control_t type,
	const rover_pose_control_params_t * params,
	const rover_pose_t * pose,
	const rover_pose_t * desired_pose,
	rover_pose_t * pose_error,
	rover_velocities_t * velocities
)
{
	switch(type) {
	case RoverPoseControl_PID:
		PIDPoseControl(params->pid, pose, desired_pose, pose_error, velocities);
	break;

	case RoverPoseControl_Lyapunov:
		LyapunovControl(params->lyapunov, pose, desired_pose, pose_error, velocities);
	break;

	default:
		// TODO: error
	break;
	}
}




////////////////////////////
///                      ///
///  PRIVATE FUNCTIONS   ///
///                      ///
////////////////////////////



void PIDPoseControl(
	const rover_PID_const_t * params,
	const rover_pose_t * pose,
	const rover_pose_t * desired_pose,
	rover_pose_t * error_pose,
	rover_velocities_t * velocities
)
{
	// TODO implement PID from pose to velocities
}





void LyapunovControl_Init (const rover_lyapunov_const_t * params)
{
	// nothing to init
}

void LyapunovControl (
	const rover_lyapunov_const_t * params,
	const rover_pose_t * pose,
	const rover_pose_t * desired_pose,
	rover_pose_t * error_pose,
	rover_velocities_t * velocities
)
{
	float l;
	float zeta;
	float psi;

	error_pose->x = desired_pose->x - pose->x;
	error_pose->y = desired_pose->y - pose->y;
	error_pose->theta = desired_pose->theta - pose->theta;

	l = sqrtf((powf((error_pose->x),2))+(powf((error_pose->y),2)));
	zeta = atan2f((error_pose->y),(error_pose->x))- pose->theta;
	psi = atan2f((error_pose->y),(error_pose->x))- desired_pose->theta;

	velocities->v = params->K1 * l * ((cosf(zeta))+(tanf(zeta)*tanf(zeta)*cos(zeta)));
	velocities->w = (params->K2 * zeta) + ((1 + ((params->Q2 * psi) / zeta)) * (params->K1 * sinf(zeta)) * ((cos(zeta))+(tan(zeta)*tanf(zeta)*cosf(zeta))));
}
