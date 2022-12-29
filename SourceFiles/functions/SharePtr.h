#pragma once
#include "Player.h"

class SharePtr
{
private:
	static Player* player;

public:
	static void SetPlayer(Player* player_) { player = player_; }
	static Player* GetPlayer() { return player; }
};