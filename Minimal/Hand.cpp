#include "Hand.h"
#include "OVRUTIL.h"
Hand::Hand(ovrSession _session, long long frame, bool isleft) : Model(HAND_PATH)
{
	if (isleft) {
		handiness = 0;
	}
	else {
		handiness = 1;
	}

	displayMidpointSeconds = ovr_GetPredictedDisplayTime(_session, frame);
	trackState = ovr_GetTrackingState(_session, displayMidpointSeconds, ovrTrue);
	HandPose = trackState.HandPoses[handiness].ThePose;

	bool HandHigh = false;
	if (HandPose.Position.y > 1.0f) {
		HandHigh = true;
	}
	playerNum = 1;
}

Hand::~Hand()
{
}
void Hand::calcAABB() {
	min = vec3(10e10f, 10e10f, 10e10f);
	max = vec3(-10e10f, -10e10f, -10e10f);
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		for (unsigned int j = 0; j < meshes[i].vertices.size(); ++j)
		{
			vec3 v = toWorld * vec4(meshes[i].vertices[j].Position, 1.0f);
			min = glm::min(v, min);
			max = glm::max(v, max);
		}
	}
	//cout << "min: " << min.x << min.y << min.z << endl;
	//cout << "max: " << max.x << max.y << max.z << endl;
}

bool Hand::update( ovrSession _session, long long frame, Factory * fac) {
	//transform hands
	displayMidpointSeconds = ovr_GetPredictedDisplayTime(_session, frame);
	trackState = ovr_GetTrackingState(_session, displayMidpointSeconds, ovrTrue);

	
	HandPose = trackState.HandPoses[handiness].ThePose;

	color = glm::vec3(0.0f, 0.9f, 0.5f);
	bool HandHigh = false;
	if (HandPose.Position.y > 1.0f) {
		HandHigh = true;
	}
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		meshes[i].toWorld  = glm::scale(ovr::toGlm(HandPose), glm::vec3(0.05f, 0.05f, 0.05f));
	}
	toWorld = glm::scale(ovr::toGlm(HandPose), glm::vec3(0.05f, 0.05f, 0.05f));
	bool HandTriggerPressed = false;
	calcAABB();
	if (ovr_GetInputState(_session, ovrControllerType_Touch, &inputState) >= 0) {

		if (inputState.HandTrigger[handiness] > 0.5f) {
			HandTriggerPressed = true;
			if (handiness == 0) {
				ovr_SetControllerVibration(_session, ovrControllerType_LTouch, 0.0f, 0.3f);
			}
			else {
				ovr_SetControllerVibration(_session, ovrControllerType_RTouch, 0.0f, 0.3f);
			}

		}
	}
	return HandTriggerPressed;
}
	//game logic
void Hand::Draw(Shader shader) {
	Model::Draw(shader);
}