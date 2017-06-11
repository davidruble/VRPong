#include <rpc/server.h>
#include <LibOVR/OVR_CAPI.h>
#include <iostream>
#include <string>

#include "SerializablePose.h"
using namespace std;

static s_Pose oculus_headPose;
static s_Pose oculus_handPose;
static s_Pose leap_headPose;
static s_Pose leap_handPose;

static int lastPlayer;

// setter for head and hand pose
void setPose(int player, int whichPose, s_Pose pose)
{
	if (player == LEAP)
	{
		//cout << "Setting leap ";
		
		if (whichPose == HEAD)
		{
			//cout << "head pose..." << endl;
			leap_headPose = pose;
		}
		else
		{
			//cout << "hand pose..." << endl;
			leap_handPose = pose;
		}
	}
	else
	{
		//cout << "Setting oculus ";
		
		if (whichPose == HEAD)
		{
			//cout << "head pose..." << endl;
			oculus_headPose = pose;
		}
		else
		{
			//cout << "hand pose..." << endl;
			oculus_handPose = pose;
		}
	}
}

// getter for head and hand pose
s_Pose getPose(int player, int whichPose)
{
	if (player == LEAP)
	{
		//cout << "Getting leap ";
		
		if (whichPose == HEAD)
		{
			//cout << "head pose..." << endl;
			return leap_headPose;
		}
		else
		{
			//cout << "hand pose..." << endl;
			return leap_handPose;
		}
	}
	else
	{
		//cout << "Getting oculus ";
		
		if (whichPose == HEAD)
		{
			//cout << "head pose..." << endl;
			return oculus_headPose;
		}
		else
		{
			//cout << "hand pose..." << endl;
			return oculus_handPose;
		}
	}
}

// setter for setting the last player to hit the ball
void setLastPlayer(int playerNum)
{
	lastPlayer = playerNum;
}

// getter for last player to hit the ball
int getLastPlayer()
{
	return lastPlayer;
}

int main(int argc, char* argv[])
{
	// start the server
	cout << "Starting server..." << endl;
	rpc::server srv(rpc::constants::DEFAULT_PORT);
	
	// bind the funtions so they can be called remotely
	srv.bind("setPose", &setPose);
	srv.bind("getPose", &getPose);
	srv.bind("setLastPlayer", &setLastPlayer);
	srv.bind("getLastPlayer", &getLastPlayer);

	cout << "Waiting for RPC calls..." << endl;
	srv.run();
	return 0;
}