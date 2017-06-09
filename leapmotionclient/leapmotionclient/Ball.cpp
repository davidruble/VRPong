#include "Ball.h"



Ball::Ball(): Model (BALL_PATH)
{
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		meshes[i].toWorld = glm::scale(meshes[i].toWorld, glm::vec3(0.1f, 0.1f, 0.1f));
	}
	velocity = glm::vec3(0.0f, 0.0f, 0.15f);
	released = true;
	lastPlayer = 0;
}

void Ball::update() {
	if (released) {
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			meshes[i].toWorld = glm::translate(meshes[i].toWorld, velocity);
		}
	}
}

glm::vec3 Ball::calcCenterPoint()
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

void Ball::Draw(Shader shader)
{

	Model::Draw(shader);

}

Ball::~Ball()
{
}
