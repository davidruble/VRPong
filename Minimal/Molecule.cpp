#include "Molecule.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

bool Molecule::modelLoaded = false;
vector<Mesh> Molecule::model_meshes;

float Molecule::randPosMin = -50.0f;
float Molecule::randPosMax = 50.0f;

Model* Molecule::o2Model;

const float SPIN_LO = 0.2f, SPIN_HI = 0.8f;
const float SPIN_DIR_LO = 0.0f, SPIN_DIR_HI = 1.0f;
const float VEL_LO = 0.05f, VEL_HI = 0.5f;

// Used for creating first molecule and loading the meshes
Molecule::Molecule(bool first) : Model(CO2_PATH)
{
	cout << "\nCreating first CO2 Molecule..." << endl;
	isO2 = false;
	if (!modelLoaded)
	{
		modelLoaded = true;
		o2Model = new Model(O2_PATH);

		// scale the molecules down a bit
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			meshes[i].toWorld = glm::translate(glm::scale(meshes[i].toWorld, glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, 1.0f, -30.0f));
			meshes[i].color = glm::vec3(0.0f, 0.6f, 0.0f);
		}
		model_meshes = meshes;

		// generate a seed for the velocity and spin generation
		srand(static_cast <unsigned> (time(0)));

		initRands();
	}
	else
	{
		cerr << "Attempting to respawn initial molecule and reload the meshes!" << endl;
	}
}

Molecule::Molecule() : Model()
{
	isO2 = false;
	cout << "\nCreating CO2 molecule..." << endl;
	meshes = model_meshes;
	initRands();
}

Molecule::~Molecule()
{
	Model::~Model();
}

void Molecule::cleanup()
{
	model_meshes.clear();
	delete o2Model;
	o2Model = NULL;
}

void Molecule::initRands()
{
	// assign initial random upwards velocity and spin
	this->spinSpeed = SPIN_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (SPIN_HI - SPIN_LO)));
	this->spinX = SPIN_DIR_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (SPIN_DIR_HI - SPIN_DIR_LO)));
	this->spinY = SPIN_DIR_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (SPIN_DIR_HI - SPIN_DIR_LO)));
	this->spinZ = SPIN_DIR_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (SPIN_DIR_HI - SPIN_DIR_LO)));
	this->velocity = VEL_LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (VEL_HI - VEL_LO)));

	cout << "Spin speed: " << spinSpeed << "     Velocity: " << velocity << endl;
	cout << "Spin X: " << spinX << "     Spin Y: " << spinY << "     Spin Z: " << spinZ << endl << endl;
}

void Molecule::Draw(Shader shader)
{
	// draw all meshes associated with this model
	for (GLuint i = 0; i < meshes.size(); i++)
	{
		
		meshes[i].Draw(shader);
	}
}

void Molecule::update()
{
	// spin and translate the molecule
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		meshes[i].toWorld = glm::translate(meshes[i].toWorld, glm::vec3(0.0f, velocity, 0.0f));
		meshes[i].toWorld = meshes[i].toWorld * glm::rotate(glm::mat4(1.0f), spinSpeed / 180.0f * glm::pi<float>(), glm::vec3(spinX, spinY, spinZ));
	}

	// get the distance from the center of the molecule to the bounding box border
	float dist = glm::abs(glm::distance(boundsOrigin, calcCenterPoint()));

	// "bounce" the molecules back when they reach the bounding box
	if (dist > BOUNDS_DIST)
		velocity = -velocity;
}

// Called when the game has been lost and molecules should be spawned in random locations
void Molecule::randomizePosition()
{
	float x = ((static_cast<float> (rand()) / (static_cast<float> (RAND_MAX))) * (randPosMax - randPosMin)) + randPosMin;
	float y = ((static_cast<float> (rand()) / (static_cast<float> (RAND_MAX))) * (randPosMax - randPosMin)) + randPosMin;
	float z = ((static_cast<float> (rand()) / (static_cast<float> (RAND_MAX))) * (randPosMax - randPosMin)) + randPosMin;

	// move the molecule to the new position
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		meshes[i].toWorld = glm::translate(meshes[i].toWorld, glm::vec3(x, y, z));
	}
}

void Molecule::makeO2()
{
	vector<Mesh> oldMeshes = meshes;
	isO2 = true;
	// turns the CO2 molecule into an O2
	meshes.clear();
	meshes = o2Model->getMeshes();
	for (int i = 0; i < meshes.size(); ++i)
		meshes[i].toWorld = oldMeshes[i].toWorld;
}

glm::vec3 Molecule::calcCenterPoint()
{
	glm::vec3 center = glm::vec3(0.0f);

	// calculate the average (center) of all the mesh points
	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		glm::vec4 pos = meshes[i].toWorld[3];
		center += glm::vec3(pos.x, pos.y, pos.z);
	}
	center /= (float)meshes.size();

	return center;
}