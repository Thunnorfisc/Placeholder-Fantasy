#include "Player.h"

Player::Player()
{
	teamList = std::vector<Character>();
	centered = true;
}

void Player::update(float frameTime)
{
	if (collisionVector.x != 0 || worldCollisionVector.x != 0)
	{
		collisionVector.x += isinf(velocity.x / collisionVector.x) * -velocity.x;
		worldCollisionVector.x += isinf(velocity.x / worldCollisionVector.x) * -velocity.x;

		if (velocity.x / collisionVector.x > 0 || velocity.x / worldCollisionVector.x > 0)
		{
			setX(getX() + (velocity.x) * frameTime);
			collisionVector.x = 0;
			worldCollisionVector.x = 0;
		}
	}
	else {
		setX(getX() + velocity.x * frameTime);
	}

	if (collisionVector.y != 0 || worldCollisionVector.y != 0)
	{
		collisionVector.y += isinf(velocity.y / collisionVector.y) * -velocity.y;
		worldCollisionVector.y += isinf(velocity.y / worldCollisionVector.y) * -velocity.y;

		if (velocity.y / collisionVector.y > 0 || velocity.y / worldCollisionVector.y > 0)
		{
			setY(getY() + (velocity.y) * frameTime);
			collisionVector.y = 0;
			worldCollisionVector.y = 0;
		}
	}
	else {
		setY(getY() + velocity.y * frameTime);
	}
	return Character::update(frameTime);
}