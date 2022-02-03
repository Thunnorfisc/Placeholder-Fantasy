#include "Player.h"

Player::Player()
{
	teamList = std::vector<Character>();
	centered = true;
}

void Player::update(float frameTime)
{
	setX(getX() + velocity.x * frameTime);
	setY(getY() + velocity.y * frameTime);

	return Character::update(frameTime);
}