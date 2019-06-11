#pragma once
//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// input.h v17.1.21
//=============================================================================

class Input;

#include <string>
//#include <XInput.h>
#include <Windows.h>
//#include <collection.h>     // Required for Platform::Collections::Vector
#include "Utilities.h"
//#include "common.h"
#include "gameError.h"

//using namespace Platform::Collections;


namespace GameEngine2
{

	const int KEYS_ARRAY_LEN = 256;     // Size of key arrays

										// Values for clear(), bit flag "what" argument
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;

	const float GAMEPAD_THUMBSTICK_DEADZONE = (0.20f);  // 20% of range
	const float GAMEPAD_TRIGGER_DEADZONE = 0.05f;   // 5% of range

	enum { MAX_CONTROLLERS = 4 };   // Maximum number of controllers.

									// Describes a game controller.
	struct ControllerDevice {
		Windows::Gaming::Input::Gamepad^            gamepad;
		Windows::Gaming::Input::GamepadReading      reading;
		Windows::Gaming::Input::GamepadVibration    vibration;
		// Vibration times in milli-seconds.
		float   vibrateTimeLeftBody;        // Left body motor.
		float   vibrateTimeRightBody;       // Right body motor.
		float   vibrateTimeLeftTrigger;     // Left trigger motor.
		float   vibrateTimeRightTrigger;    // Right trigger motor.
	};

	class Input
	{
	private:
		bool keysDown[KEYS_ARRAY_LEN];      // true if indexed key is down
		bool keysPressed[KEYS_ARRAY_LEN];   // true if indexed key was pressed
		std::string textIn;                 // user entered text
		char charIn;                        // last character entered
		bool newLine;                       // true on start of new line
		float  pointerX, pointerY;          // pointer screen coordinates
		int  mouseWheel;                    // mouse wheel movement

		bool mouseCaptured;                 // true if mouse captured
		bool mouseLButton;                  // true if left mouse button down
		bool mouseMButton;                  // true if middle mouse button down
		bool mouseRButton;                  // true if right mouse button down
		bool mouseX1Button;                 // true if X1 mouse button down
		bool mouseX2Button;                 // true if X2 mouse button down
		std::vector<ControllerDevice> controllers; // state of controllers
		float thumbstickDeadzone;
		float triggerDeadzone;

	public:
		// Constructor
		Input();

		// Destructor
		virtual ~Input();

		// Initialize mouse and controller input.
		// Throws GameError
		// Pre: hwnd = window handle
		//      capture = true to capture mouse.
		void initialize(HWND hwnd, bool capture);

		// Save key down state
		void keyDown(Windows::System::VirtualKey);

		// Save key up state
		void keyUp(Windows::System::VirtualKey);

		// Save the char just entered in textIn string
		void keyIn(unsigned int key);

		// Returns true if the specified VIRTUAL KEY is down, otherwise false.
		bool isKeyDown(Windows::System::VirtualKey) const;

		// Return true if the specified VIRTUAL KEY has been pressed in the most recent frame.
		// Key presses are erased at the end of each frame.
		bool wasKeyPressed(Windows::System::VirtualKey) const;

		// Return true if any key was pressed in the most recent frame.
		// Key presses are erased at the end of each frame.
		bool anyKeyPressed() const;

		// Clear the specified key press
		void clearKeyPress(UCHAR vkey);

		// Clear specified input buffers where what is any combination of
		// KEYS_DOWN, KEYS_PRESSED, MOUSE, TEXT_IN or KEYS_MOUSE_TEXT.
		// Use OR '|' operator to combine parameters.
		void clear(UCHAR what);

		// Clears key, mouse and text input data
		void clearAll() { clear(KEYS_MOUSE_TEXT); }

		// Clear text input buffer
		void clearTextIn() { textIn.clear(); }

		// Clear last character entered
		void clearCharIn() { charIn = 0; }

		// Return text input as a string
		std::string getTextIn() { return textIn; }

		// Set text input string
		void setTextIn(std::string str) { textIn = str; }

		// Return last character entered
		char getCharIn() { return charIn; }

		//=====================================================================
		// Save pointer position into pointerX, pointerY
		//=====================================================================
		void setPointerXY(float x, float y)
		{
			pointerX = x;
			pointerY = y;
		}

		// Reads mouse wheel movement.
		void mouseWheelIn(int);

		// Save state of mouse button
		void setMouseLButton(bool b) { mouseLButton = b; }

		// Save state of mouse button
		void setMouseMButton(bool b) { mouseMButton = b; }

		// Save state of mouse button
		void setMouseRButton(bool b) { mouseRButton = b; }

		// Return pointer X position
		float  getPointerX()        const { return pointerX; }

		// Return pointer Y position
		float  getPointerY()        const { return pointerY; }

		// Return mouse wheel movement relative to previous position.
		// Forward is >0, Backward is <0, movement is in multiples of WHEEL_DATA (120).
		int  getMouseWheel()
		{
			int wheel = mouseWheel;
			mouseWheel = 0;
			return wheel;
		}

		// Return state of left mouse button.
		bool getMouseLButton()  const { return mouseLButton; }

		// Return state of middle mouse button.
		bool getMouseMButton()  const { return mouseMButton; }

		// Return state of right mouse button.
		bool getMouseRButton()  const { return mouseRButton; }

		// Return state of X1 mouse button.
		bool getMouseX1Button() const { return mouseX1Button; }

		// Return state of X2 mouse button.
		bool getMouseX2Button() const { return mouseX2Button; }

		// Update connection status of game controllers.
		//void checkControllers();

		// Save input from connected game controllers.
		void readControllers();

		// A game controller was connected.
		void gamepadAdded(Windows::Gaming::Input::Gamepad ^gamepad);

		// A game controller was disconnected.
		void gamepadRemoved(Windows::Gaming::Input::Gamepad ^gamepad);

		// Set thumbstick deadzone
		void setThumbstickDeadzone(float dz) { thumbstickDeadzone = dz; }

		// Set trigger deadzone
		void setTriggerDeadzone(float dz) { triggerDeadzone = dz; }

		// Get thumbstick deadzone
		float getThumbstickDeadzone() { return thumbstickDeadzone; }

		// Get trigger deadzone
		float getTriggerDeadzone() { return triggerDeadzone; }

		// Return state of specified game controller.
		//const ControllerDevice* getControllerState(UINT n)
		//{
		//    if (n > MAX_CONTROLLERS - 1)
		//        n = MAX_CONTROLLERS - 1;
		//    return &controllers[n];
		//}

		// Return connection state of specified game controller
		//bool getGamepadConnected(UINT n)
		//{
		//    if (n > MAX_CONTROLLERS - 1)
		//        n = MAX_CONTROLLERS - 1;
		//    return controllers[n].connected;
		//}

		// Return state of controller n buttons.
		//Windows::Gaming::Input::GamepadButtons getGamepadButtons(UINT n)
		//{
		//    if (n > MAX_CONTROLLERS - 1)
		//        n = MAX_CONTROLLERS - 1;
		//    return controllers[n].state.Buttons;
		//}

		//=========================================================================
		// Return state of controllers[n] A button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadA(UINT n)
		{
			// If at least one controller is connected then controllers.size()
			// should always be > n.
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::A ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::A))
					return true;
			}
			return false;	// Button not pressed or invalid n or no controllers
		}

		//=========================================================================
		// Return state of controllers[n] B button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadB(UINT n)
		{
			// If at least one controller is connected then controllers.size()
			// should always be > n.
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::B ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::B))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] X button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadX(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::X ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::X))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] Y button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadY(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::Y ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::Y))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] D-pad Up.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadDPadUp(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::DPadUp ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::DPadUp))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] D-pad Down.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadDPadDown(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::DPadDown ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::DPadDown))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] D-pad Left.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadDPadLeft(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::DPadLeft ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::DPadLeft))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] D-pad Right.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadDPadRight(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::DPadRight ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::DPadRight))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] View button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadView(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::View ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::View))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] Menu button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadMenu(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::Menu ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::Menu))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] Left stick button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadLeftStickButton(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::LeftThumbstick ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::LeftThumbstick))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] Right stick button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadRightStickButton(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::RightThumbstick ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::RightThumbstick))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] Left shoulder (bumper) button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadLeftShoulderButton(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::LeftShoulder ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::LeftShoulder))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return state of controllers[n] Right shoulder (bumper) button.
		// Pre: The controllers vector contains only connected controllers.
		// Post: Returns true if pressed, false otherwise.
		//=========================================================================
		bool getGamepadRightShoulderButton(UINT n)
		{
			if (controllers.size() > n)
			{
				if (Windows::Gaming::Input::GamepadButtons::RightShoulder ==
					(controllers.at(n).reading.Buttons &
						Windows::Gaming::Input::GamepadButtons::RightShoulder))
					return true;
			}
			return false;
		}

		//=========================================================================
		// Return controllers[n] Left Trigger as a float between 0.0 and 1.0.
		// A value of 0.0 is fully released and a value of 1.0 is fully depressed.
		// Trigger movement less than GAMEPAD_TRIGGER_DEADZONE returns 0.
		// A value of 0 is returned if controller n is not connected.
		//=========================================================================
		float getGamepadLeftTrigger(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)		// if valid n
			{
				float x = getGamepadLeftTriggerUndead(n);
				if (x > triggerDeadzone) // if +x outside dead zone
										 // Adjust magnitude relative to the end of the dead zone.
					x = x - triggerDeadzone;
				else if (x < -triggerDeadzone)   // if -x outside dead zone
												 // Adjust magnitude relative to the end of the dead zone.
					x = x + triggerDeadzone;
				else        // else, x inside dead zone
					x = 0;  // return 0
				return x;
			}
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return controllers[n] Left Trigger as a float between 0.0 and 1.0.
		// A value of 0.0 is fully released and a value of 1.0 is fully depressed.
		// A value of 0 is returned if no controller is connected.
		// Deadzone is not applied.
		//=========================================================================
		float getGamepadLeftTriggerUndead(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // If valid n.
				return (float)controllers.at(n).reading.LeftTrigger;
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return controllers[n] Right Trigger as a float between 0.0 and 1.0.
		// A value of 0.0 is fully released and a value of 1.0 is fully depressed.
		// A value of 0 is returned if no controller is connected.
		// Trigger movement less than GAMEPAD_TRIGGER_DEADZONE returns 0.
		//=========================================================================
		float getGamepadRightTrigger(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // if valid n
			{
				float x = getGamepadRightTriggerUndead(n);
				if (x > triggerDeadzone) // if +x outside dead zone
										 // Adjust magnitude relative to the end of the dead zone.
					x = x - triggerDeadzone;
				else if (x < -triggerDeadzone)   // if -x outside dead zone
												 // Adjust magnitude relative to the end of the dead zone.
					x = x + triggerDeadzone;
				else        // else, x inside dead zone
					x = 0;  // return 0
				return x;
			}
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return controllers[n] Right Trigger as a float between 0.0 and 1.0.
		// A value of 0.0 is fully released and a value of 1.0 is fully depressed.
		// A value of 0 is returned if no controller is connected.
		// Deadzone is not applied.
		//=========================================================================
		float getGamepadRightTriggerUndead(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // If valid n.
				return (float)controllers.at(n).reading.RightTrigger;
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return value of controllers[n] Left Thumbstick X as -1.0 to +1.0.
		// A value of 0 is returned if no controller is connected.
		// Stick movement less than GAMEPAD_THUMBSTICK_DEADZONE returns 0.
		//=========================================================================
		float getGamepadThumbLX(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // if valid n
			{
				float x = getGamepadThumbLXUndead(n);
				if (x > thumbstickDeadzone) // if +x outside dead zone
											// Adjust magnitude relative to the end of the dead zone.
					x = x - thumbstickDeadzone;
				else if (x < -thumbstickDeadzone)   // if -x outside dead zone
													// Adjust magnitude relative to the end of the dead zone.
					x = x + thumbstickDeadzone;
				else        // else, x inside dead zone
					x = 0;  // return 0
				return x;
			}
			return 0;	// Controller n is not connected
		}

		//=========================================================================
		// Return value of controllers[n] Left Thumbstick X as -1.0 to +1.0.
		// A value of 0 is returned if no controller is connected.
		// Deadzone is not applied.
		//=========================================================================
		float getGamepadThumbLXUndead(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // If valid n.
				return (float)controllers.at(n).reading.LeftThumbstickX;
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return value of controller n Left Thumbstick Y as a float -1.0 to +1.0.
		// A value of 0 is returned if no controller is connected.
		// Stick movement less than GAMEPAD_THUMBSTICK_DEADZONE returns 0.
		//=========================================================================
		float getGamepadThumbLY(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // if valid n
			{
				float y = getGamepadThumbLYUndead(n);
				if (y > thumbstickDeadzone) // if +y outside dead zone
											// Adjust magnitude relative to the end of the dead zone.
					y = y - thumbstickDeadzone;
				else if (y < -thumbstickDeadzone)   // if -y outside dead zone
													// Adjust magnitude relative to the end of the dead zone.
					y = y + thumbstickDeadzone;
				else        // else, y inside dead zone
					y = 0;  // return 0
				return y;
			}
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return value of controller n Left Thumbstick Y as a float -1.0 to +1.0.
		// A value of 0 is returned if no controller is connected.
		// Deadzone is not applied.
		//=========================================================================
		float getGamepadThumbLYUndead(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // If valid n.
				return (float)controllers.at(n).reading.LeftThumbstickY;
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return value of controllers[n] Right Thumbstick X as -1.0 to +1.0.
		// A value of 0 is returned if no controller is connected.
		// Stick movement less than GAMEPAD_THUMBSTICK_DEADZONE returns 0.
		//=========================================================================
		float getGamepadThumbRX(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // if valid n
			{
				float x = getGamepadThumbRXUndead(n);
				if (x > thumbstickDeadzone) // if +x outside dead zone
											// Adjust magnitude relative to the end of the dead zone.
					x = x - thumbstickDeadzone;
				else if (x < -thumbstickDeadzone)   // if -x outside dead zone
													// Adjust magnitude relative to the end of the dead zone.
					x = x + thumbstickDeadzone;
				else        // else, x inside dead zone
					x = 0;  // return 0
				return x;
			}
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return value of controllers[n] Right Thumbstick X as -1.0 to +1.0.
		// A value of 0 is returned if no controller is connected.
		// Deadzone is not applied.
		//=========================================================================
		float getGamepadThumbRXUndead(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // If valid n.
				return (float)controllers.at(n).reading.RightThumbstickX;
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return value of controller n Right Thumbstick Y as a float -1.0 to +1.0.
		// A value of 0 is returned if no controller is connected.
		// Stick movement less than GAMEPAD_THUMBSTICK_DEADZONE returns 0.
		//=========================================================================
		float getGamepadThumbRY(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() >= n)    // if valid n
			{
				float y = getGamepadThumbRYUndead(n);
				if (y > thumbstickDeadzone) // if +y outside dead zone
											// Adjust magnitude relative to the end of the dead zone.
					y = y - thumbstickDeadzone;
				else if (y < -thumbstickDeadzone)   // if -y outside dead zone
													// Adjust magnitude relative to the end of the dead zone.
					y = y + thumbstickDeadzone;
				else        // else, y inside dead zone
					y = 0;  // return 0
				return y;
			}
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Return value of controller n Right Thumbstick Y as a float -1.0 to +1.0.
		// A value of -1.0 is returned if no controller is connected.
		// Deadzone is not applied.
		//=========================================================================
		float getGamepadThumbRYUndead(UINT n)
		{
			if (controllers.size() == 0)	// If no controllers connected
				return 0;
			if (controllers.size() > n)     // If valid n.
				return (float)controllers.at(n).reading.RightThumbstickY;
			return 0;	// Controller n is not connected.
		}

		//=========================================================================
		// Vibrate controllers[n] left motor.
		// Left is low frequency vibration.
		// Speed 0.0 to 1.0.
		// sec is time to vibrate in seconds
		//=========================================================================
		void gamePadVibrateLeftBody(UINT n, float speed, float sec)
		{
			if (n > controllers.size())     // If invalid n.
				n = 0;                      // force valid
			controllers.at(n).vibration.LeftMotor = speed;
			controllers.at(n).vibrateTimeLeftBody = sec;
		}

		//=========================================================================
		// Vibrate controllers[n] right motor.
		// Right is high frequency vibration.
		// Speed 0.0 to 1.0.
		// sec is time to vibrate in seconds
		//=========================================================================
		void gamePadVibrateRightBody(UINT n, float speed, float sec)
		{
			if (n > controllers.size())     // If invalid n.
				n = 0;                      // force valid
			controllers.at(n).vibration.RightMotor = speed;
			controllers.at(n).vibrateTimeRightBody = sec;
		}

		//=========================================================================
		// Vibrate controllers[n] left trigger.
		// Speed 0.0 to 1.0.
		// sec is time to vibrate in seconds
		//=========================================================================
		void gamePadVibrateLeftTrigger(UINT n, float speed, float sec)
		{
			if (n > controllers.size())     // If invalid n.
				n = 0;                      // force valid
			controllers.at(n).vibration.LeftTrigger = speed;
			controllers.at(n).vibrateTimeLeftTrigger = sec;
		}

		//=========================================================================
		// Vibrate controllers[n] right trigger.
		// Speed 0.0 to 1.0.
		// sec is time to vibrate in seconds
		//=========================================================================
		void gamePadVibrateRightTrigger(UINT n, float speed, float sec)
		{
			if (n > controllers.size())     // If invalid n.
				n = 0;                      // force valid
			controllers.at(n).vibration.RightTrigger = speed;
			controllers.at(n).vibrateTimeRightTrigger = sec;
		}

		//=========================================================================
		// Vibrates the connected controllers for the desired time.
		//=========================================================================
		void vibrateControllers(float frameTime);
	};
}