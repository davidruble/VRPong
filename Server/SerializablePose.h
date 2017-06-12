#ifndef SERIALIZABLE_POSE_H
#define SERIALIZABLE_POSE_H

#include <LibOVR/OVR_CAPI.h>
#include <rpc.h>

// shared defines for RPC parameters
#define OCULUS 0
#define LEAP 1
#define HEAD 0
#define HAND 1
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

// serializable pose object
struct s_Pose
{
	float pos_x, pos_y, pos_z;
	float rot_x, rot_y, rot_z, rot_w;

	MSGPACK_DEFINE_MAP(pos_x, pos_y, pos_z,
		rot_x, rot_y, rot_z, rot_w);
};

// serializes an ovr pose for the server
s_Pose serializePose(ovrPosef poseIn)
{
	s_Pose retPose;

	retPose.pos_x = poseIn.Position.x;
	retPose.pos_y = poseIn.Position.y;
	retPose.pos_z = poseIn.Position.z;

	retPose.rot_x = poseIn.Orientation.x;
	retPose.rot_y = poseIn.Orientation.y;
	retPose.rot_z = poseIn.Orientation.z;
	retPose.rot_w = poseIn.Orientation.w;

	return retPose;
}

// deserializes a server pose for ovr
ovrPosef deserializePose(s_Pose poseIn)
{
	ovrVector3f pos;
	pos.x = poseIn.pos_x;
	pos.y = poseIn.pos_y;
	pos.z = poseIn.pos_z;

	ovrQuatf rot;
	rot.x = poseIn.rot_x;
	rot.y = poseIn.rot_y;
	rot.z = poseIn.rot_z;
	rot.w = poseIn.rot_w;

	ovrPosef retPose;
	retPose.Position = pos;
	retPose.Orientation = rot;
	return retPose;
}

#endif
