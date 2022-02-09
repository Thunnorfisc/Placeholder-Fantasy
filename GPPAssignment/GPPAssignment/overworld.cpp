#include "Overworld.h"

Overworld::Overworld(SceneManager* manager) 
{
	dxManager = manager;
	world = World();
	player = Player();
	npc1 = NPC();
	cliff_1 = Interactable();
	cliff_2 = Interactable();
	cliff_3 = Interactable();
	wagon = Interactable();
	waterfall = Interactable();

	WorldX = 0;
	WorldY = 0;

	screenHeight = GAME_HEIGHT;
	screenWidth = GAME_WIDTH;
}

Overworld::~Overworld()
{
	dxManager = NULL;
}

void Overworld::initialize()
{
	//Initialize WorldX and Y
	setWorldPos(0, 0);

	//Initialize Layering System
	if (!entManagerV2.Initialize(MAXLAYERS)) throw(GameError(gameErrorNS::FATAL_ERROR, "Invalid Max Layers"));

	//initialize World
	if(!world.initialize(dxManager,OVERWORLD_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR,"Failed to initialize world"));

	//Set Parameters
	world.setScale(OVERWORLD_SCALE);
	world.setPos(getWorldX(), getWorldY());
	world.BoundsSetup(getWorldX(), getWorldY(), getWorldX() + world.getWidth(), getWorldY() + world.getHeight());
	
	//initialize cliffs
	if (!cliff_1.initialize(dxManager, CLIFF_1_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Cliff_1"));
	if (!cliff_2.initialize(dxManager, CLIFF_2_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Cliff_2"));
	if (!cliff_3.initialize(dxManager, CLIFF_3_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Cliff_3"));

	//Set Cliff Parameters
	cliff_1.setPos(getWorldX() + CLIFF_1_POSX * OVERWORLD_SCALE, getWorldY() + CLIFF_1_POSY * OVERWORLD_SCALE);
	cliff_2.setPos(getWorldX() + CLIFF_2_POSX * OVERWORLD_SCALE, getWorldY() + CLIFF_2_POSY * OVERWORLD_SCALE);
	cliff_3.setPos(getWorldX() + CLIFF_3_POSX * OVERWORLD_SCALE, getWorldY() + CLIFF_3_POSY * OVERWORLD_SCALE);

	//initialize wagon
	if (!wagon.initialize(dxManager, WAGON_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Wagon"));
	
	//Set Wagon Parameters
	wagon.setPos(getWorldX() + WAGON_POSX * OVERWORLD_SCALE, getWorldY() + WAGON_POSY * OVERWORLD_SCALE);

	//initialize waterfall
	if (!waterfall.initialize(dxManager, WATERFALL_IMAGE)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize Waterfall"));
	
	//Set Waterfall Parameters
	waterfall.setPos(getWorldX() + WATERFALL_POSX * OVERWORLD_SCALE, getWorldY() + WATERFALL_POSY * OVERWORLD_SCALE);

	//initialize Player
	if (!player.initialize(dxManager, MAINCHARA_ANIMATION, MAINCHARA_IMAGE, 32, 32, 0)) throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize player"));
	player.setScale(CHARA_SCALE);
	player.setPos(screenWidth/2 - player.getWidth(), screenHeight / 2 - player.getHeight() / 2);

	//Add Game Objects to Layers
	entManagerV2.AddToLayer(&cliff_1,MAXLAYERS - 1);
	entManagerV2.AddToLayer(&cliff_2,MAXLAYERS - 1);
	entManagerV2.AddToLayer(&cliff_3,MAXLAYERS - 1);
	entManagerV2.AddToLayer(&wagon,MAXLAYERS - 2);
	entManagerV2.AddToLayer(&waterfall,MAXLAYERS - 2);
	entManagerV2.AddToLayer(&player,MAXLAYERS - 2);

	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			if(layer->at(i)->getClass() == "Interactable")
			layer->at(i)->setScale(OVERWORLD_SCALE);
		}
	}

}

void Overworld::update(float frameTime) 
{
	if (dxManager->getInput()->isKeyDown(VK_LEFT))
	{
		setWorldPos(getWorldX() + 200 * frameTime, getWorldY());
		world.setX(getWorldX());
	}	
	if (dxManager->getInput()->isKeyDown(VK_RIGHT))
	{
		setWorldPos(getWorldX() - 200 * frameTime, getWorldY());
		world.setX(getWorldX());
	}	
	if (dxManager->getInput()->isKeyDown(VK_UP))
	{
		setWorldPos(getWorldX(), getWorldY() + 200 * frameTime);
		world.setY(getWorldY());
	}	
	if (dxManager->getInput()->isKeyDown(VK_DOWN))
	{
		setWorldPos(getWorldX(), getWorldY() - 200 * frameTime);
		world.setY(getWorldY());
	}

	//V2 TriggerLayers
	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			
			if (layer->at(i)->getClass() == "Interactable")
			{
				layer->at(i)->update(frameTime);
				updateWorldObj(layer->at(i));
			}
		}
	}
}

void Overworld::ai() {}

void Overworld::collisions() 
{
	//VECTOR2 collisionVector;

	//world.CheckBoundsCollision(&player);

	//V2 TriggerLayers
	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			if (layer->at(i)->getClass() == "Interactable")
			{
				Interactable* interactable = dynamic_cast<Interactable*> (layer->at(i));
				interactable->triggerLayerV2(&player, &entManagerV2);
				//interactable->collideBox(&player, collisionVector);
				//interactable->triggerBox(&player, collisionVector);
			}
		}
	}
}

void Overworld::render() 
{
	dxManager->getGraphics()->spriteBegin();

	//Draw the world map
	world.draw(TRANSCOLOR);

	//Here is where the layer system go
	//V2 System
	for (std::vector<Entity*>* layer : entManagerV2.GetLayers())
	{
		for (int i = 0; i < layer->size(); i++)
		{
			layer->at(i)->draw(TRANSCOLOR);
		}
	}

	dxManager->getGraphics()->spriteEnd();
}

void Overworld::releaseAll() {}

void Overworld::resetAll() {}

void Overworld::onMessage(const Mail& mail) {}

void Overworld::updateWorldObj(Entity* ent)
{
	ent->setX(getWorldX() + ent->getOriX());
	ent->setY(getWorldY() + ent->getOriY());
}