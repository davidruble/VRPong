#pragma once

#include "Model.h"

#define CO2_PATH "Assets/co2/co2.obj"
#define O2_PATH "Assets/o2/o2.obj"
#define BOUNDS_DIST 2.5f

static glm::vec3 boundsOrigin = glm::vec3(0.0f);

class Molecule : protected Model
{
public:
	Molecule(bool first);
	Molecule();
	~Molecule();

	void Draw(Shader shader);
	void update();
	void randomizePosition();
	void makeO2();

	glm::vec3 calcCenterPoint();

	static void cleanup();

	static bool modelLoaded;
	bool isO2;
	static vector<Mesh> model_meshes;
private:
	void initRands();

	//static vector<Mesh> model_meshes;

	float velocity;
	float spinX, spinY, spinZ, spinSpeed;

	static float randPosMin, randPosMax;

	// this is a really shitty thing to do
	static Model* o2Model;
};
