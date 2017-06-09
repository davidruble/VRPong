#include "Player.h"

Player::Player(int playernum, Hand * phand)
{
	playerNum = playernum;
	hand = phand;
	head = new Head();
}

void Player::Draw(Shader shader) {
	if (playerNum == 1) {
		head->Draw(shader);
	}
	hand->Draw(shader);
}

void Player::update(ovrSession _session, long long frame) {
	hand->update();
	if (hand->isLeap) {
		head->HeadPose = hand->HandPose;
		head->update(hand->isLeap);
	}else {
		head->update(false);
	}
}
Player::~Player()
{
}
