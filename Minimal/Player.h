#pragma once
#include "Head.h"
#include "Hand.h"
class Player
{
public:
	Player(int playernum, Hand * phand);
	void Draw(Shader shader);
	void update(ovrSession, long long);
	~Player();
	int playerNum;
	Head * head;
	Hand * hand;
};

