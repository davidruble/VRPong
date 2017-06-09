#include "Head.h"



Head::Head() : Model(HEAD_PATH)
{
}

void Head::Draw(Shader shader)
{

	Model::Draw(shader);

}

Head::~Head()
{
}
