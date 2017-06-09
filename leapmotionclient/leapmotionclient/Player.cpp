#include "Player.h"



Player::Player(int playernum, Hand * phand)
{
	playerNum = playernum;
	hand = phand;
	head = new Head();
}

void Player::Draw(Shader shader) {
	//head->Draw(shader);
	hand->Draw(shader);
}

void Player::update(ovrSession _session, long long frame) {
	hand->update(_session, frame);
}
Player::~Player()
{
}
