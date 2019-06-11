//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// input.cpp v17.1.21
//=============================================================================

#include "pch.h"
#include "input.h"
using namespace GameEngine2;

//=============================================================================
// default constructor
//=============================================================================
GameEngine2::Input::Input()
{
	// clear key down array
	for (size_t i = 0; i < KEYS_ARRAY_LEN; i++)
		keysDown[i] = false;
	// clear key pressed array
	for (size_t i = 0; i < KEYS_ARRAY_LEN; i++)
		keysPressed[i] = false;
	newLine = true;                     // start new line
	textIn = "";                        // clear textIn
	charIn = 0;                         // clear charIn

										// Mouse data.
	pointerX = 0;                       // screen X
	pointerY = 0;                       // screen Y
	mouseWheel = 0;                     // mouse wheel position
	mouseLButton = false;               // true if left mouse button is down
	mouseMButton = false;               // true if middle mouse button is down
	mouseRButton = false;               // true if right mouse button is down
	mouseX1Button = false;              // true if X1 mouse button is down
	mouseX2Button = false;              // true if X2 mouse button is down

	thumbstickDeadzone = GAMEPAD_THUMBSTICK_DEADZONE;    // default
	triggerDeadzone = GAMEPAD_TRIGGER_DEADZONE;          // default
}

//=============================================================================
// destructor
//=============================================================================
GameEngine2::Input::~Input()
{
	//if (mouseCaptured)
	//    ReleaseCapture();               // release mouse
}

//=============================================================================
// Initialize mouse and controller input
// Set capture=true to capture mouse
// Throws GameError
//=============================================================================
void GameEngine2::Input::initialize(HWND hwnd, bool capture)
{
	try {
		mouseCaptured = capture;
		//if (mouseCaptured)
		//    SetCapture(hwnd);           // capture mouse

		// Copy all connected gamepads to our controllers Vector.
		for (auto gamepad : Windows::Gaming::Input::Gamepad::Gamepads)
		{
			ControllerDevice cd;
			cd.gamepad = gamepad;
			cd.vibrateTimeLeftBody = 0;
			cd.vibrateTimeLeftTrigger = 0;
			cd.vibrateTimeRightBody = 0;
			cd.vibrateTimeRightTrigger = 0;
			cd.reading = gamepad->GetCurrentReading();
			controllers.push_back(cd);
		}
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing input system"));
	}
}

//=============================================================================
// Set true in the keysDown and keysPessed array for this key
// Pre: vkey contains the virtual key code (0--255)
//=============================================================================
void GameEngine2::Input::keyDown(Windows::System::VirtualKey vkey)
{
	int key = static_cast<int>(vkey);
	// make sure key code is within buffer range
	if (key < KEYS_ARRAY_LEN)
	{
		keysDown[key] = true;    // update keysDown array
								 // Key has been "pressed, erased by clear().
		keysPressed[key] = true; // update keysPressed array
	}
}

//=============================================================================
// Set false in the keysDown array for this key
// Pre: vkey contains the virtual key code (0--255)
//=============================================================================
void GameEngine2::Input::keyUp(Windows::System::VirtualKey vkey)
{
	int key = static_cast<int>(vkey);
	// make sure key code is within buffer range
	if (key < KEYS_ARRAY_LEN)
		// update state table
		keysDown[key] = false;
}

//=============================================================================
// Save the char just entered in textIn string
// Pre: key contains the unicode representation of the key.
//=============================================================================
void GameEngine2::Input::keyIn(unsigned int key)
{
	char ch = static_cast<char>(key);

	if (newLine)    // if start of new line
	{
		textIn.clear();
		newLine = false;
	}

	if (ch == '\b')							// if backspace
	{
		if (textIn.length() > 0)            // if characters exist
			textIn.erase(textIn.size() - 1);  // erase last character entered
	}
	else
	{
		textIn += ch;						// add character to textIn
		charIn = ch;						// save last char entered
	}

	if (key == '\r')						// if return    
		newLine = true;                     // start new line

}

//=============================================================================
// Returns true if the specified VIRTUAL KEY is down, otherwise false.
//=============================================================================
bool GameEngine2::Input::isKeyDown(Windows::System::VirtualKey vkey) const
{
	int key = static_cast<int>(vkey);
	if (key < KEYS_ARRAY_LEN)
		return keysDown[key];
	else
		return false;
}

//=============================================================================
// Return true if the specified VIRTUAL KEY has been pressed in the most recent
// frame. Key presses are erased at the end of each frame.
//=============================================================================
bool GameEngine2::Input::wasKeyPressed(Windows::System::VirtualKey vkey) const
{
	int key = static_cast<int>(vkey);
	if (key < KEYS_ARRAY_LEN)
		return keysPressed[key];
	else
		return false;
}

//=============================================================================
// Return true if any key was pressed in the most recent frame.
// Key presses are erased at the end of each frame.
//=============================================================================
bool GameEngine2::Input::anyKeyPressed() const
{
	for (size_t i = 0; i < KEYS_ARRAY_LEN; i++)
		if (keysPressed[i] == true)
			return true;
	return false;
}

//=============================================================================
// Clear the specified key press
//=============================================================================
void GameEngine2::Input::clearKeyPress(UCHAR vkey)
{
	if (vkey < KEYS_ARRAY_LEN)
		keysPressed[vkey] = false;
}

//=============================================================================
// Clear specified input buffers
// See input.h for what values
//=============================================================================
void GameEngine2::Input::clear(UCHAR what)
{
	if (what & KEYS_DOWN)       // if clear keys down
	{
		for (size_t i = 0; i < KEYS_ARRAY_LEN; i++)
			keysDown[i] = false;
	}
	if (what & KEYS_PRESSED)    // if clear keys pressed
	{
		for (size_t i = 0; i < KEYS_ARRAY_LEN; i++)
			keysPressed[i] = false;
	}
	if (what & MOUSE)           // if clear mouse
	{
		pointerX = 0;
		pointerY = 0;
		mouseWheel = 0;
	}
	if (what & TEXT_IN)
	{
		clearTextIn();
		clearCharIn();
	}
}

//=============================================================================
// Reads mouse wheel movement expressed in multiples of WHEEL_DELTA, which
// is 120. A positive value indicates that the wheel was rotated away from the
// user, a negative value indicates that the wheel was rotated toward the user.
//=============================================================================
void GameEngine2::Input::mouseWheelIn(int wheelDelta)
{
	mouseWheel = wheelDelta;
}

//=============================================================================
// Check for connected controllers
//=============================================================================
//void GameEngine2::Input::checkControllers()
//{
//    DWORD result;
//    for (DWORD i = 0; i <MAX_CONTROLLERS; i++)
//    {
//        result = XInputGetState(i, &controllers[i].state);
//        if (result == ERROR_SUCCESS)
//            controllers[i].connected = true;
//        else
//            controllers[i].connected = false;
//    }
//}

//=============================================================================
// A game controller was connected.
//=============================================================================
void GameEngine2::Input::gamepadAdded(Windows::Gaming::Input::Gamepad ^gamepad)
{
	ControllerDevice cd;
	cd.gamepad = gamepad;
	cd.vibrateTimeLeftBody = 0;
	cd.vibrateTimeLeftTrigger = 0;
	cd.vibrateTimeRightBody = 0;
	cd.vibrateTimeRightTrigger = 0;
	cd.reading = gamepad->GetCurrentReading();
	controllers.push_back(cd);  // Add the gampad to the list of controllers.
}

//=============================================================================
// A game controller was disconnected.
//=============================================================================
void GameEngine2::Input::gamepadRemoved(Windows::Gaming::Input::Gamepad^ gamepad)
{
	int index;
	ControllerDevice cd;
	cd.gamepad = gamepad;
	for (index = 0; index < controllers.size(); index++)
	{
		if (controllers.at(index).gamepad == gamepad)
			// Remove the gamepad from the list of controllers.
			controllers.erase(controllers.begin() + index);
	}
}

//=============================================================================
// Read state of connected gamepads.
// Pre: The controllers vector contains only connected controllers.
// Post: The data in the controlers vector is updated for all controllers.
//=============================================================================
void GameEngine2::Input::readControllers()
{
	for (int i = 0; i < controllers.size(); i++)    // for all controllers
	{
		// Read current state of controller.
		controllers.at(i).reading =
			controllers.at(i).gamepad->GetCurrentReading();
	}
}

//=============================================================================
// Vibrate connected controllers
//=============================================================================
void GameEngine2::Input::vibrateControllers(float frameTime)
{
	for (int i = 0; i < controllers.size(); i++)
	{
		controllers[i].vibrateTimeLeftBody -= frameTime;
		if (controllers[i].vibrateTimeLeftBody < 0)
		{
			controllers[i].vibrateTimeLeftBody = 0;
			controllers[i].vibration.LeftMotor = 0;
		}
		controllers[i].vibrateTimeRightBody -= frameTime;
		if (controllers[i].vibrateTimeRightBody < 0)
		{
			controllers[i].vibrateTimeRightBody = 0;
			controllers[i].vibration.RightMotor = 0;
		}

		controllers[i].vibrateTimeLeftTrigger -= frameTime;
		if (controllers[i].vibrateTimeLeftTrigger < 0)
		{
			controllers[i].vibrateTimeLeftTrigger = 0;
			controllers[i].vibration.LeftTrigger = 0;
		}
		controllers[i].vibrateTimeRightTrigger -= frameTime;
		if (controllers[i].vibrateTimeRightTrigger < 0)
		{
			controllers[i].vibrateTimeRightTrigger = 0;
			controllers[i].vibration.RightTrigger = 0;
		}

		controllers.at(i).gamepad->Vibration = controllers.at(i).vibration;
	}
}

