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
	~NPC();

	bool initialize();
};
#endif // !_NPC_H_
