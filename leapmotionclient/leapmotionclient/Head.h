#pragma once
#include "Model.h"
#include "Shader.h"
#define HEAD_PATH "Assets/head/untitled.obj"
class Head : protected Model
{
public:
	Head();
	void Draw(Shader shader);
	~Head();
};

