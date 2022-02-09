#include "Player.h"

Player::Player()
{
	teamList = std::vector<Character>();
	centeredX = true;
	centeredY = true;
}

void Player::update(float frameTime)
{
	if (!centeredX)
	{
		setCurrentFrame();
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

	}
	if (!centeredY)
	{
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
	}

	playerInputs(MOVEMENTSPEED, frameTime);

	//return Character::update(frameTime);
}

void Player::playerInputs(float velocity = MOVEMENTSPEED, float frameTime = 0)
{
	if (input->isKeyDown(VK_LEFT) || input->isKeyDown(VK_RIGHT) || input->isKeyDown(VK_UP) || input->isKeyDown(VK_DOWN))
	{
		Image::update(3 * frameTime);
	}
	else {
		setCurrentFrame(0);
	}
	// Player Movement Inputs
	if (input->isKeyDown(VK_LEFT))
	{
		flipHorizontal(false);
		setVelocityX(-MOVEMENTSPEED);
	}
	else if (input->isKeyDown(VK_RIGHT))
	{
		flipHorizontal(true);
		setVelocityX(MOVEMENTSPEED);
	}
	else { 
		setVelocityX(0);
	}

	if (input->isKeyDown(VK_UP))
	{
		setVelocityY(-MOVEMENTSPEED);
	}
	else if (input->isKeyDown(VK_DOWN))
	{
		setVelocityY(MOVEMENTSPEED);
	}
	else { 
		setVelocityY(0);
		
	}
}