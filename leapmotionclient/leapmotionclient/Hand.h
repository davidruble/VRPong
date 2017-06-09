#pragma once
#include "Model.h"
#include "Shader.h"
#include "LibOVR/OVR_CAPI.h"
#include "LibOVR/OVR_CAPI_GL.h"
#define HAND_PATH "Assets/paddle/paddle.obj"
class Hand : protected Model
{
public:
	GLuint vbo, vao;
	vector<GLfloat> vertices;
	glm::vec3 color;
	glm::mat4 toWorld;
	glm::vec3 min;
	glm::vec3 max;
	ovrInputState inputState;
	ovrPosef HandPose;
	bool isLeap = false;
	int handiness = 0;
	Hand(ovrSession = NULL, long long = 0, bool left = false);
	Hand(bool isleap = false);
	~Hand();
	bool update(ovrSession = NULL, long long = 0);
	void calcAABB();
	void Draw(Shader shader);
private:
	double displayMidpointSeconds;
	ovrTrackingState trackState;
	
};
