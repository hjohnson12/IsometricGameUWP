#pragma once
//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// common.h v17.2.15
//=============================================================================
#include <D2d1.h>
#include <WindowsNumerics.h>
#include "Utilities.h"

//#include <windows.h>
namespace GameEngine2
{
    //=========================================================================
    //                  Constants
    //=========================================================================
    const bool FULLSCREEN = false;  // windowed or fullscreen
    //const UINT GAME_WIDTH = 1280;   // width of game in pixels
    //const UINT GAME_HEIGHT = 720;   // height of game in pixels
	//const int GAME_WIDTH = 1600;   // width of game in pixels
	//const int GAME_HEIGHT = 1000;   // height of game in pixels
	const UINT GAME_WIDTH = 1280;   // width of game in pixels
	const UINT GAME_HEIGHT = 720;   // height of game in pixels

    //const float FRAME_RATE = 240.0f;  // the target frame rate (frames/sec)
    //const float MIN_FRAME_RATE = 10.0f;   // the minimum frame rate
    //Minimum desired time for 1 frame.
    //const float MIN_FRAME_TIME = 1.0f / FRAME_RATE;
    //Maximum time used in calculations.
    //const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;
    const int DESIRED_UPS = 100;    // Desired Updates/Second
    const float PI = 3.141593f;

    // Redefine float2 as Vector2. Redefining makes it easier to change
    // Vector2 in the future.
    typedef Windows::Foundation::Numerics::float2 Vector2;     // A 2D vector

    //=========================================================================
    // Color defines for text.
	// ARGB numbers range from 0 (off) through 255 (full color)
    // A = Alpha channel transparency. 0(transparent) through 255(opaque).
    // R = Red, G = Green, B = Blue.
    //=========================================================================
    typedef Windows::UI::Color TEXT_COLOR;
	// TEXT_COLORS:: opens list of predefined text colors.
    typedef Windows::UI::Colors TEXT_COLORS;

    //=========================================================================
    // Color defines for graphics.
    // COLORS:: opens list of colors defined below.
    // RGBA numbers range from 0.0f (off) through 1.0f (full color).
    // R = Red, G = Green, B = Blue.
    // A = Alpha channel transparency. 0.0f(transparent) through 1.0f(opaque).
    // ===== SPECIAL EFFECTS =====
    // From MSDN website: "You can set the members of this structure to values
    // outside the range of 0 through 1 to implement some unusual effects.
    // Values greater than 1 produce strong lights that tend to wash out a
    // scene. Negative values produce dark lights that actually remove light
    // from a scene."
    //=========================================================================
    typedef _D3DCOLORVALUE COLOR_RGBA_F;
    // Some common colors.
    namespace COLORS
    {
        const COLOR_RGBA_F BLUE{ 0, 0, 1.0f, 1.0f };
        const COLOR_RGBA_F BLUE_BRIGHT{ 0, 0, 2.0f, 1.0f };
        const COLOR_RGBA_F BLUE50{ 0, 0, 1.0f, 0.5f };
        const COLOR_RGBA_F BLACK{ 0, 0, 0, 1.0f };
        const COLOR_RGBA_F BLACK50{ 0, 0, 0, 0.5f };
        const COLOR_RGBA_F BLACK25{ 0, 0, 0, 0.25f };
        const COLOR_RGBA_F BROWN{ 0.55f, 0.27f, 0.75f, 1.0f };
        const COLOR_RGBA_F BROWN50{ 0.55f, 0.27f, 0.75f, 0.5f };
        const COLOR_RGBA_F CYAN{ 0, 1.0f, 1.0f, 1.0f };
        const COLOR_RGBA_F CYAN_BRIGHT{ 0, 2.0f, 2.0f, 1.0f };
        const COLOR_RGBA_F CYAN50{ 0, 1.0f, 1.0f, 0.5f };
        const COLOR_RGBA_F GRAY{ 0.5f, 0.5f, 0.5f, 1.0f };
        const COLOR_RGBA_F GRAY50{ 0.5f, 0.5f, 0.5f, 0.5f };
        const COLOR_RGBA_F GREEN{ 0, 0.5f, 0, 1.0f };
        const COLOR_RGBA_F GREEN50{ 0, 0.5f, 0, 0.5f };
        const COLOR_RGBA_F LIME{ 0, 1.0f, 0, 1.0f };
        const COLOR_RGBA_F LIME_BRIGHT{ 0, 2.0f, 0, 1.0f };
        const COLOR_RGBA_F LIME50{ 0, 1.0f, 0, 0.5f };
        const COLOR_RGBA_F LTGRAY{ 0.75f, 0.75f, 0.75f, 1.0f };
        const COLOR_RGBA_F LTGRAY50{ 0.75f, 0.75f, 0.75f, 0.5f };
        const COLOR_RGBA_F MAGENTA{ 1.0f, 0, 1.0f, 1.0f };
        const COLOR_RGBA_F MAGENTA_BRIGHT{ 2.0f, 0, 2.0f, 1.0f };
        const COLOR_RGBA_F MAGENTA50{ 1.0f, 0, 1.0f, 0.5f };
        const COLOR_RGBA_F MAROON{ 0.5f, 0, 0, 1.0f };
        const COLOR_RGBA_F MAROON50{ 0.5f, 0, 0, 0.5f };
        const COLOR_RGBA_F NAVY{ 0, 0, 0.5f, 1.0f };
        const COLOR_RGBA_F NAVY50{ 0, 0, 0.5f, 0.5f };
        const COLOR_RGBA_F OLIVE{ 0.5f, 0.5f, 0, 1.0f };
        const COLOR_RGBA_F OLIVE50{ 0.5f, 0.5f, 0, 0.5f };
        const COLOR_RGBA_F ORANGE{ 1.0f, 0.65f, 0, 1.0f };
        const COLOR_RGBA_F ORANGE_BRIGHT{ 2.0f, 1.3f, 0, 1.0f };
        const COLOR_RGBA_F ORANGE50{ 1.0f, 0.65f, 0, 0.5f };
        const COLOR_RGBA_F PURPLE{ 0.5f, 0, 0.5f, 1.0f };
        const COLOR_RGBA_F PURPLE50{ 0.5f, 0, 0.5f, 0.5f };
        const COLOR_RGBA_F RED{ 1.0f, 0, 0, 1.0f };
        const COLOR_RGBA_F RED_BRIGHT{ 2.0f, 0, 0, 1.0f };
        const COLOR_RGBA_F RED50{ 1.0f, 0, 0, 0.5f };
        const COLOR_RGBA_F TEAL{ 0, 0.5f, 0.5f, 1.0f };
        const COLOR_RGBA_F TEAL50{ 0, 0.5f, 0.5f, 0.5f };
        const COLOR_RGBA_F WHITE{ 1.0f, 1.0f, 1.0f, 1.0f };
        const COLOR_RGBA_F WHITE_BRIGHT{ 2.0f, 2.0f, 2.0f, 1.0f };
        const COLOR_RGBA_F WHITE50{ 1.0f, 1.0f, 1.0f, 0.5f };
        const COLOR_RGBA_F YELLOW{ 1.0f, 1.0f, 0, 1.0f };
        const COLOR_RGBA_F YELLOW_BRIGHT{ 2.0f, 2.0f, 0, 1.0f };
        const COLOR_RGBA_F YELLOW50{ 1.0f, 1.0f, 0, 0.5f };
        // Use to specify drawing with colorFilter.
        const COLOR_RGBA_F FILTER{ 0, 0, 0, 0 };
    }

    // weapon types
    enum WEAPON { NPC_ATTACK, PLAYER_ATTACK, TORPEDO, SHIP, PLANET };

}