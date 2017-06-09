#include "Head.h"
#include "OVRUTIL.h"


Head::Head() : Model(HEAD_PATH)
{
}


bool Head::update(bool isLeap) {
	cout << "starting update" << endl;
	//transform hands
	if (!isLeap) {
		//get headPose for oculus
		cout << "update oc head" << endl;
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			meshes[i].toWorld = glm::translate(glm::scale(ovr::toGlm(HeadPose), glm::vec3(0.2f, 0.2f, 0.2f)), glm::vec3(0.0f, 1.0f, 10.0f));
		}
		toWorld = glm::translate(glm::scale(ovr::toGlm(HeadPose), glm::vec3(0.5f, 0.5f, 0.5f)), glm::vec3(0.0f, -1.0f, -3.0f));
		return false;
	}
	else {
		cout << "update lp head" << endl;
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			meshes[i].toWorld = glm::translate(glm::scale(ovr::toGlm(HeadPose), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, -1.0f, -3.0f));
		}
		toWorld = glm::translate(glm::scale(ovr::toGlm(HeadPose), glm::vec3(0.05f, 0.05f, 0.05f)), glm::vec3(0.0f, -1.0f, -3.0f));
		return false;
	}
}

void Head::Draw(Shader shader)
{

	Model::Draw(shader);

}

Head::~Head()
{
}
