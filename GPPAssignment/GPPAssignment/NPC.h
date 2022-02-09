#ifndef _NPC_H_
#define _NPC_H_
#define WIN32_LEAN_AND_MEAN

#include "Character.h"
#include <vector>

class NPC : public Character
{
protected:
	bool friendly;
	bool interactable;

	std::vector<string> dialogLines;

public:
	NPC();

	bool initialize();

	virtual void update(float frameTime);

};
#endif // !_NPC_H_
