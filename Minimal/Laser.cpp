#include "Laser.h"
#define LASER_PATH "laser.obj"
Laser::Laser(): Model(LASER_PATH)
{
}

Laser::~Laser()
{
}

void Laser::update() {

}

void Laser::Draw(Shader shader) {
	Model::Draw(shader);
}