#pragma once
#include "Model.h"
#include "Factory.h"
#include "Shader.h"
#include <OVR_CAPI.h>
#include <OVR_CAPI_GL.h>
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
	int handiness;
	Hand(ovrSession, long long, bool left);
	~Hand();
	bool update(ovrSession, long long);
	void calcAABB();
	void Draw(Shader shader);
private:
	double displayMidpointSeconds;
	ovrTrackingState trackState;
	
};
