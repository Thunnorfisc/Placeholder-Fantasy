#include "Overworld.h"

Overworld::Overworld(SceneManager* manager) 
{
	dxManager = manager;
	world = World();
	player = Player();
	//npc1 = NPC();
}

Overworld::~Overworld()
{
	dxManager = NULL;
}

void Overworld::initialize()
{
	entManagerV2.Initialize(MAXLAYERS);

	//initialize World
	world.initialize(dxManager,OVERWORLD_IMAGE);

	//Set Parameters
	world.setScale(OVERWORLD_SCALE_X, OVERWORLD_SCALE_Y);

	//initialize cliffs


}

void Overworld::update(float frameTime) 
{
	
}

void Overworld::ai() {}

void Overworld::collisions() {}

void Overworld::render() {}

void Overworld::releaseAll() {}

void Overworld::resetAll() {}

void Overworld::onMessage(const Mail& mail) {}