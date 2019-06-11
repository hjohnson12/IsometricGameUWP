#pragma once
//=============================================================================
// Programming 2D Games
// Copyright (c) 2017 by: 
// Charles Kelly
// Strings.h  v17.2.9
// Place exception messages and game text here.
//=============================================================================

namespace GameEngine2
{
    namespace Strings
    {
        //=====================================================================
        // ===== Exception Messages =====
        // Messages for GameError class are type const char[] for compatibility
        // with std::exception class.
        //=====================================================================
        const char NoAudioFiles[] = "Alert, no audio files found.";

        //=====================================================================
        // ===== Input Text Strings =====
        // Strings of type const cstring. Keyboard input is returned as an
        // array of char. The text here is used to compare with user input.
        // Input text is not in Unicode format.
        //=====================================================================
        const char LLAP[] = "LLAP";     // User command LLAP

        //=====================================================================
        // ===== Text for Output =====
        // Game messages are type const wstring to support Unicode.
        //=====================================================================
        const std::wstring LLAPS = L"Live long and prosper.";
		const std::wstring Instructions = L"Move Horizontal/Vertically with W,A,S,D keys.\n"
			"Move Diagonally with Q,E,Z,C keys.\n"
			"Attack with F key.\n"
			"I,J,K,L keys scroll map. +Shift for fast scroll speed.\n"
			"M = Music Pause/Resume\nPageUp, PageDown Volume\n"
			"Mouse Wheel + to Zoom In, Mouse Wheel - to Zoom Out";
        const std::wstring FPS = L"FPS: ";
        const std::wstring UPS = L"UPS: ";
		const std::wstring TIMER = L"Elapsed Time: ";
		const std::wstring PLAYER_HEALTH = L"Player Health: ";
		const std::wstring PLAYER_LIVES = L"Lives: ";
    }
}
