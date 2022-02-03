#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
// Useful macros

// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
//Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr) {if(ptr){delete [](ptr); ptr = NULL}}
//Safely call onRestDevice
#define SAFE_ON_RESET_DEVICE(ptr) {if(ptr){ptr->onResetDevice();}}
#define TRANSCOLOR SETCOLOR_ARGB(255,254,254,254)
//#define XTRANSCOLOR SETCOLOR_ARGB(0,255,0,255)
#define BACKGROUNDCOLOUR SETCOLOR_ARGB(200,20,20,20)

//Constants

// window
const char CLASS_NAME[] = "Placeholder Fantasy";
const char GAME_TITLE[] = "Placeholder Fantasy";
const bool FULLSCREEN = false;              // windowed or fullscreen
const int  GAME_WIDTH = 1280;              // width of game in pixels
const int  GAME_HEIGHT = 720;              // height of game in pixels

//game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;					//the target frame rate (frame/sec)
const float MIN_FRAME_RATE = 10.0f;					//the minimum frame rate
const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;		//minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE; //Maximum time used in calculations

const float ROTATION_RATE = 180.0f;             // degrees per second
const float SCALE_RATE = 0.2f;                  // % change per second
const float ANIMATE_SPEED = 100.0f;             // pixels per second
const float CURSOR_SCALE = 1.5f;                  // starting ship scale

// Asset files
const char MAINCHARA_IMAGE[] = "assets\\Ayame.png";
const char MAINCHARA_ANIMATION[] = "assets\\ayameRun.png";
const char OVERWORLD_IMAGE[] = "assets\\PlaceholderWorldMap.png";
const char Cursor[] = "assets\\cursor.png";
const char PLACEHOLDERRECT[] = "assets\\PlaceholderRect.png";
const char PLAYERCHARA_IMAGE[] = "assets\\blue_char.png";
const char BEDROOM_IMAGE[] = "assets\\Bedroom\\Bedroom.png";
const char BED_IMAGE[] = "assets\\Bedroom\\Bed.png";

// Sound files
const char MENUMUSIC[] = "sound\\music\\CASTLEVANIA_MENU_1.mp3";
const char OPTIONCHANGESFX[] = "sound\\sound_effects\\switch27.mp3";

//Asset Constants
//const int SHIP_START_FRAME = 0;
//const int SHIP_END_FRAME = 3;
//const float SHIP_ANIMATION_DELAY = 0.2f;
const int HPBAR_HEIGHT = 18;
const int HPBAR_WIDTH = 240;
const float CHARA_SCALE = 2;

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const BYTE ESC_KEY = VK_ESCAPE;					// escape key
const BYTE NAVI_RIGHT_KEY = VK_RIGHT;
const BYTE NAVI_LEFT_KEY = VK_LEFT;
const BYTE NAVI_UP_KEY = VK_UP;
const BYTE NAVI_DOWN_KEY = VK_DOWN;

const UCHAR CURSOR_UP_KEY = VK_UP;
const UCHAR CURSOR_DOWN_KEY = VK_DOWN;
const UCHAR CURSOR_LEFT_KEY = VK_LEFT;
const UCHAR CURSOR_RIGHT_KEY = VK_RIGHT;

const float MOVEMENTSPEED = 200;

const float ENEMYSECTION = GAME_WIDTH * 0.25;
const float TEAMSECTION = GAME_WIDTH * 0.75;

const float OFFSET = 10;
const float TRIGGEROFFSET = 20;

#endif // !_CONSTANTS_H_
