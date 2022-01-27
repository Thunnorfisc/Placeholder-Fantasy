#include "game.h"
#include "battleScene.h"

//Constructor
Game::Game()
{
    input = new Input();	//initializing keyboard input immediately
    mailManager = new PostOffice();
    audio = new Audio();
    paused = false;			//Game not paused
    graphics = NULL;
    initialized = false;
}

//Destuctor
Game::~Game()
{
    deleteAll();			//free all reserved memory
    ShowCursor(true);		//show cursor

}

//All Error within function is due to the missing functions from the input class
LRESULT Game::messagehandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (initialized)
    {
        switch (msg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);        //tell Windows to kill this program
            return 0;
        case WM_KEYDOWN: case WM_SYSKEYDOWN:    // key down
            input->keyDown(wParam);
            return 0;
        case WM_KEYUP: case WM_SYSKEYUP:        // key up
            input->keyUp(wParam);
            return 0;
        case WM_CHAR:                           // character entered
            input->keyIn(wParam);
            return 0;
        case WM_MOUSEMOVE:                      // mouse moved
            input->mouseIn(lParam);
            return 0;
        case WM_INPUT:                          // raw mouse data in
            input->mouseRawIn(lParam);
            return 0;
        case WM_LBUTTONDOWN:                    // left mouse button down
            input->setMouseLButton(true);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_LBUTTONUP:                      // left mouse button up
            input->setMouseLButton(false);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_MBUTTONDOWN:                    // middle mouse button down
            input->setMouseMButton(true);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_MBUTTONUP:                      // middle mouse button up
            input->setMouseMButton(false);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_RBUTTONDOWN:                    // right mouse button down
            input->setMouseRButton(true);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_RBUTTONUP:                      // right mouse button up
            input->setMouseRButton(false);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_XBUTTONDOWN: case WM_XBUTTONUP: // mouse X button down/up
            input->setMouseXButton(wParam);
            input->mouseIn(lParam);             // mouse position
            return 0;
        case WM_DEVICECHANGE:                   // check for controller insert
            input->checkControllers();
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);    // let Windows handle it
}

void Game::initialize(HWND hw) {
    hwnd = hw;

    //initialize graphics
    graphics = new Graphics;
    //throw GameError
    graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);
    //initialize input, do not capture mouse
    input->initialize(hwnd, false);

    //attempt to set up high resolution timer
    if (QueryPerformanceFrequency(&timerFreq) == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));
    QueryPerformanceCounter(&timeStart);

    initialized = true;
}

void Game::handleLostGraphicsDevice() {
    hr = graphics->getDeviceState();
    if (FAILED(hr)) {
        if (hr == D3DERR_DEVICELOST) {
            Sleep(100);                     //Yield cpu time (100 ms)
            return;
        }
        else if (hr == D3DERR_DEVICENOTRESET) {
            releaseAll();
            hr = graphics->reset();        //Attempted to reset graphics device
            if (FAILED(hr)) return;        //if reset failed
            resetAll();
        }
        else {
            return;                        //other device error
        }
    }
}

void Game::renderGame() {
    //start rendering
    if (SUCCEEDED(graphics->beginScene())) {
        render();                                   //A pure virtual class that must be provided in the parent class
        graphics->endScene();                       //Stop rendering
    }
    handleLostGraphicsDevice();

    graphics->showBackbuffer();                     //Display the back buffer on the screen

}

void Game::run(HWND hwnd) {
    if (graphics == NULL) return;

    QueryPerformanceCounter(&timeEnd);
    frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

    if (frameTime < MIN_FRAME_TIME) {
        sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
        timeBeginPeriod(1);     //Request 1ms resulutionn for windows timer
        Sleep(sleepTime);       //release cpu for sleeping
        timeEndPeriod(1);       //End 1ms timer resolution
    }

    if (frameTime > 0.0) fps = (fps * 0.99f) + (0.01f / frameTime); // average fps

    if (frameTime > MAX_FRAME_TIME) frameTime = MAX_FRAME_TIME; //if frame rate is very slow, limit the max frame time.

    timeStart = timeEnd;

    if (!paused) {
        update();
        ai();
        collisions();
        input->vibrateControllers(frameTime);
        renderGame();
        input->readControllers();
        //Clear input
        //Call after al key checks are done.
        input->clear(inputNS::KEYS_PRESSED);
        mailManager->dispatchMessages();
    }
}

void Game::releaseAll() {}

void Game::resetAll() {}

void Game::deleteAll() {
    releaseAll();               //call onLostDevice() for evert graphics items
    SAFE_DELETE(graphics);
    SAFE_DELETE(input);
    SAFE_DELETE(mailManager);
    SAFE_DELETE(audio);
    initialized = false;
}