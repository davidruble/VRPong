#pragma once
#include "Model.h"
#include "Shader.h"
class Laser: protected Model
{
public:
	Laser();
	~Laser();
	void update();
	void Draw(Shader);
private:
	float velocity;
};