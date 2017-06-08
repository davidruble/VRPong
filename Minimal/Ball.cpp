#include "Ball.h"



Ball::Ball(): Model (BALL_PATH)
{
	velocity = glm::vec3(0.0f, 0.0f, 0.001f);
}

void Ball::update() {
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		meshes[i].toWorld = glm::translate(meshes[i].toWorld, velocity);
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
