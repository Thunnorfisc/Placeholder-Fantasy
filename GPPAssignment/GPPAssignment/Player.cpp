#include "Player.h"

Player::Player()
{
	teamList = std::vector<Character>();
	centered = true;
}

void Player::update(float frameTime)
{
	if (collisionVector.x != 0)
	{
		if (velocity.x / collisionVector.x > 0)
		{
			setX(getX() + collisionVector.x * frameTime);
			collisionVector.x = 0;
		}
	}
	else {
		setX(getX() + velocity.x * frameTime);
	}

	if (collisionVector.y != 0)
	{
		if (velocity.y / collisionVector.y > 0)
		{
			setY(getY() + collisionVector.y * frameTime);
			collisionVector.y = 0;
		}
	}
	else {
		setY(getY() + velocity.y * frameTime);
	}

	return Character::update(frameTime);
}