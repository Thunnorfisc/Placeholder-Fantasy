#ifndef _Game_H
#define _Game_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameerror.h"
#include "textDX.h"
#include "postOffice.h"
#include "audio.h"

namespace gameNS
{
	const char FONT[] = "Terminal";  // font
	const char INFOFONT[] = "Calibri ";  // font
	const int POINT_SIZE = 30;          // point size
	const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255, 255, 15, 15);    // white
}

class Game
{
protected:
	//common game properties
	Graphics* graphics;             // pointer to Graphics
	Input* input;					// pointer to Input
	PostOffice* mailManager;        // Pointer to Post Office (AKA Mail Manager)
	Audio* audio;
	HWND    hwnd;                   // window handle
	HRESULT hr;                     // standard return type
	LARGE_INTEGER timeStart;        // Performance Counter start value
	LARGE_INTEGER timeEnd;          // Performance Counter end value
	LARGE_INTEGER timerFreq;        // Performance Counter frequency
	float   frameTime;              // time required for last frame
	float   fps;                    // frames per second
	bool    fpsOn;                  // true to display fps
	DWORD   sleepTime;              // number of milli-seconds to sleep between frames
	bool    paused;                 // true if game is paused
	bool    initialized;

public:
	Game();
	virtual ~Game();																//Constructor
	LRESULT messagehandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);		//Destructor
	virtual void initialize(HWND hwnd);												//Initialize the game
	virtual void run(HWND);															//Window message handler

	//Call when the graphics device was lost
	virtual void releaseAll();														//Call run repeatedly by the main message loop in WinMain
	virtual void resetAll();														//Recreate all surfaces and reset all entities
	virtual void deleteAll();														//Delete all reserve memory

	virtual void renderGame();														//Render Game items
	virtual void render() = 0;
	virtual void handleLostGraphicsDevice();										//Handle lost graphics device
	Graphics* getGraphics() { return graphics; }									//Return pointer to Graphics
	Input* getInput() { return input; }												//Return pointer to Input
	PostOffice* getPostOffice() { return mailManager; }                             //Return pointer to Post Office
	Audio& getAudio() { return *audio; }                                             //Return pointer to Audio
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }						//Exit the game

	//Pure virtual function declarations
	//These functions MUST be written in any class that inherits from Game
	virtual void update() = 0;														//Update game items
	virtual void ai() = 0;															//Perform AI calculations
	virtual void collisions() = 0;													//Check for collisions

	//Render graphics.
	//Call graphics->spriteBegin()
	//	draw sprites
	//Call graphics->spriteEnd()
	//	draw non-sprites

};

#endif // !_Game_H
