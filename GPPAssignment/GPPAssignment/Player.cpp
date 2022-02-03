#include "Player.h"

Player::Player()
{
	teamList = std::vector<Character>();
	centered = true;
}

void Player::update(float frameTime)
{
	return Character::update(frameTime);
}