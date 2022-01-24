#ifndef _PLAYER_H_
#define _PLAYER_H_
#define WIN32_LEAN_AND_MEAN

#include <vector>
#include "Character.h"

class Player : public Character
{
protected:
	std::vector<Character> teamList;


public:
	Player();

	int attack() {
		return 0;
	}
};
#endif // !_PLAYER_H_