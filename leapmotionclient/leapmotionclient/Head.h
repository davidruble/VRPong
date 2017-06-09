#pragma once
#include "Model.h"
#include "Shader.h"
#include "LibOVR/OVR_CAPI.h"
#include "LibOVR/OVR_CAPI_GL.h"
#define HEAD_PATH "Assets/head/untitled.obj"
class Head : protected Model
{
public:
	Head();
	bool update(ovrPosef, bool isleap );
	void Draw(Shader shader);
	~Head();
	glm::mat4 toWorld;
};

