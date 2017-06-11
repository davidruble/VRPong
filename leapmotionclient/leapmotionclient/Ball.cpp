#include "Ball.h"



Ball::Ball() : Model(BALL_PATH)
{
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		meshes[i].toWorld = glm::scale(meshes[i].toWorld, glm::vec3(0.1f, 0.1f, 0.1f));
	}
	velocity = glm::vec3(0.0f, 0.0f, 0.25f);
	released = true;
	lastPlayer = 0;
}

void Ball::update() {
	glm::vec3 cent = calcCenterPoint();
	if (cent.z > 3.0f || cent.z < -3.0f)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			meshes[i].toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f)), glm::vec3(0.0f, 0.0f, 0.25f));
		}
		velocity = glm::vec3(0.0f, 0.0f, 0.25f);
		lastPlayer = 0;
		outOfBounds = true;
	}
	if (cent.x > 1.0f || cent.x < -1.0f)
	{
		velocity = glm::reflect(velocity, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (cent.y > 0.7f || cent.y < -1.0f)
	{

		velocity = glm::reflect(velocity, glm::vec3(0.0f, 1.0f, 0.0f));
	}
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
