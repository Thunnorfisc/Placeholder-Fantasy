//===========================================================================================
//	Module		: Game Programming Assignment 2
//	Assignment	: Placeholder Fantasy?	
//	Name		: Jason Teo Jun Hao
//	S/N			: S10194978
//	Description	: This header file is for the player object which is a child of Character.
//				  This is the player character that will be checked against. Will also contain
//				  teamList that could be used in the future.
//===========================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include "Character.h"
#include "audio.h"

class Player : public Character
{
protected:
	std::vector<Character> teamList;
	bool centeredX;
	bool centeredY;

public:
	Player();

	virtual void update(float frameTime);

	int attack() {
		return 0;
	}

	void playerInputs(float velocity, float frameTime);

	void isCenterX(bool iscenterX) { centeredX = iscenterX; }
	void isCenterY(bool iscenterY) { centeredY = iscenterY; }

	bool ifCenterX() { return centeredX; }
	bool ifCenterY() { return centeredY; }
};
#endif // !_PLAYER_H_