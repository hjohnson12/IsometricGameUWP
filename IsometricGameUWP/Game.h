#pragma once
#define DEBUG		// define DEBUG to display debug data
//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// game.h  v17.1.21
//
// This is the main game class.
//=============================================================================

#include <Xaudio2.h>
#include <iostream>
#include "Strings.h"
#include "Utilities.h"
#include <string>
#include <fstream>
#include "Common.h"
#include "Input.h"
#include "Audio.h"
#include "SoundFileReader.h"
#include "TextWin2D.h"
#include "Image.h"
#include "Player.h"
#include "Character.h"
#pragma comment(lib, "Xaudio2.lib")

using namespace Microsoft::Graphics::Canvas::UI;

namespace GameEngine2
{
    const uint32 SOUND_SAMPLE_RATE = 48000;

    const int BEEP1 = 0;
    const int BEEP2 = 1;
    const int BEEP3 = 2;
    const int BEEP4 = 3;
    const int HIT = 4;
    const int POW = 5;
    const int MUSIC1 = 6;
    const int MUSIC2 = 7;

    // Seconds to display instructions.
    const float INSTRUCTION_DISPLAY_TIME = 3.5;

    // Background color of game.
    const COLOR_RGBA_F BACK_COLOR(COLORS::NAVY);

    // Map textures
	const int TEXTURE_COLS_TERRAIN = 8;
	const int TEXTURE_SIZE_TERRAIN = 128;
    // Size of height change in pixels for current tile set
	const int HEIGHT_CHANGE = 32;
	const int MAP_SIZE = 68;	
	const int MAP_WIDTH = 68;
	const int MAP_HEIGHT = 50;
	const double SCROLL_SPEED = 200;    // Map scroll speed in pixels/second
	// SCREEN_X & SCREEN_Y set starting position of map on screen.
	const int SCREEN_X = GAME_WIDTH / 2;
	const int SCREEN_Y = GAME_HEIGHT / 8;		
	const float MAX_ZOOM = 1.2f;        // Maximum zoom.
	const float MIN_ZOOM = 0.3f;        // Minimum zoom.
	const float ZOOM_AMOUNT = 0.1f;     // Amount to add or subtract from zoom.
	const int GOAL1_ROW = 36;
	const int GOAL1_COL = 65;
	const int GOAL2_ROW = 36;
	const int GOAL2_COL = 66;
	
    // Object Texture Numbers
	const int TREE_0 = 1;
	const int TREE_1 = 2;
	const int FENCE1 = 3;
	const int FENCE3 = 4;
	const int FENCE5 = 5;
	const int FENCE7 = 6;
	const int FENCE9 = 7;
	const int FENCE11 = 8;
	const int FENCE13 = 9;
	const int FENCE15 = 10;
	const int TREE0 = 11;
	const int TREE1 = 12;
	const int TREE2 = 13;
	const int TREE3 = 14;
	const int TREE4 = 15;
	const int TREE5 = 16;
	const int TREE6 = 17;
	const int WELL0 = 18;
	const int WELL1 = 19;
	const int HUT_ORIGIN = 20;
	const int HUT = 21;
	const int TOWER0 = 22;
	const int BUILDING_ORIGIN = 23;
	const int BUILDING = 24;
	const int BUILDING2_ORIGIN = 25;
	const int BUILDING2 = 26;
	const int BUILDING3_ORIGIN = 27;
	const int BUILDING3 = 28;
	const int WATCHTOWER0_ORIGIN = 29;
	const int WATCHTOWER0 = 30;
	const int WATCHTOWER1_ORIGIN = 31;
	const int WATCHTOWER1 = 32;
	const int BUILDING4_ORIGIN = 33;
	const int BUILDING4 = 34;
	const int BUILDING5_ORIGIN = 35;
	const int BUILDING5 = 36;
	const int ROCK2 = 37;
	const int NPC01 = 38;
	const int KNIGHT = 39;
	const int PLAYER = 40;
	const int BARRICADE0 = 41;
	const int BARRICADE1 = 42;
	const int BARRICADE2 = 43;
	const int BARRICADE3 = 44;
	const int BARRICADE4 = 45;
	const int BARRICADE5 = 46;
	const int BARRICADE6 = 47;
	const int BARRICADE7 = 48;
	const int BARRICADE8 = 49;
	const int BARRICADE9 = 50;
	const int POND_ORIGIN = 51;
	const int POND = 52;
	const int BLACKSMITH_ORIGIN = 53;
	const int BLACKSMITH = 54;
	const int BARRACKS_ORIGIN = 55;
	const int BARRACKS = 56;
	const int ARCHERY_ORIGIN = 57;
	const int ARCHERY = 58;
	const int STABLE_ORIGIN = 59;
	const int STABLE = 60;
	const int BROKEN_TOWER_ORIGIN = 61;
	const int BROKEN_TOWER = 62;
	const int TIMBERED_HOUSE0_ORIGIN = 63;
	const int TIMBERED_HOUSE0 = 64;
	const int TIMBERED_HOUSE1_ORIGIN = 65;
	const int TIMBERED_HOUSE1 = 66;
	const int NPC02 = 67;
	const int NPC03 = 68;
	const int NPC04 = 69;
	const int NPC05 = 70;
	const int NPC06 = 71;
	const int NPC07 = 72;
	const int NPC08 = 73;
	const int NPC09 = 74;
	const int NPC10 = 75;
	const int NPC11 = 76;
	const int NPC12 = 77;
	const int NPC13 = 78;
	const int NPC14= 79;
	const int NPC15 = 80;
	const int NPC16 = 81;
	const int NPC17 = 82;
	const int NPC18 = 83;
	const int NPC19 = 84;
	const int NPC20 = 85;
	const int CHEST_ORIGIN = 86;
	const int CHEST = 87;
	const int NPC21 = 88;
	const int NPC22 = 89;
	const int NPC25 = 90;
	const int NPC26 = 91;
	const int NPC27 = 92;
	const int NPC28 = 93;
	const int NPC29 = 94;
	const int NPC30 = 95;
	const int NPC31 = 96;
	const int NPC32 = 97;
	const int NPC33 = 98;
	const int NPC34 = 99;
	const int NPC35 = 100;
	const int NPC36 = 101;
	const int NPC37 = 102;
	const int NPC38 = 103;
	const int NPC39 = 104;
	const int NPC40 = 105;
	const int NPC41 = 106;
	const int NPC42 = 107;
	const int NPC43 = 108;
	const int NPC44 = 109;
	const int NPC45 = 110;
	const int NPC46 = 111;
	const int NPC47 = 112;
	const int NPC48 = 113;
	const int NPC49 = 114;
	const int NPC50 = 115;
	const int NPC51 = 116;
	const int NPC52 = 117;
	const int NPC53 = 118;
	const int NPC54 = 119;
	const int NPC55 = 120;
	const int NPC56 = 121;
	const int NPC57 = 122;
	const int NPC58 = 123;
	const int NPC59 = 124;
	const int NPC60 = 125;
	const int NPC61 = 126;
	const int NPC62 = 127;
	const int NPC63 = 128;
	const int NPC64 = 129;
	const int NPC65 = 130;
	const int NPC66 = 131;
	const int NPC67 = 132;
	const int NPC68 = 133;
	const int NPC69 = 134;
	const int NPC70 = 135;
	const int NPC23 = 136;
	const int NPC24 = 137;
	const int NPC71 = 138;
	const int NPC72 = 139;
	const int NPC73 = 140;
	const int NPC74 = 141;
	const int NPC75 = 142;
	const int NPC76 = 143;
	const int NPC77 = 144;
	const int NPC78 = 145;
	const int NPC79 = 146;
	const int NPC80 = 147;


	// Object Texture Properties
	const int PLAYER_TEXTURE_COLS = 9;
	const int PLAYER_TEXTURE_WIDTH = 48;
	const int PLAYER_TEXTURE_HEIGHT = 92;
	const int PLAYER_START_ROW = 2;
	const int PLAYER_START_COL = 2;
	const int PLAYER_OFFSET_X = 0;	  // add to map X to get player X
	const int PLAYER_OFFSET_Y = 36;  // add to may Y to get player Y 
	const int PLAYER_HEIGHT = 75;	  // player's actual height
	const int NPC21_START_ROW = 28;
	const int NPC21_START_COL = 31;
	const int NPC22_START_ROW = 11;
	const int NPC22_START_COL = 35;
	const int NPC23_START_ROW = 16;
	const int NPC23_START_COL = 35;
	const int NPC24_START_ROW = 24;
	const int NPC24_START_COL = 36;
	const int NPC25_START_ROW = 28;
	const int NPC25_START_COL = 37;
	const int NPC26_START_ROW = 15;
	const int NPC26_START_COL = 40;
	const int NPC27_START_ROW = 27;
	const int NPC27_START_COL = 1;
	const int NPC28_START_ROW = 35;
	const int NPC28_START_COL = 4;
	const int NPC29_START_ROW = 33;
	const int NPC29_START_COL = 16;
	const int NPC30_START_ROW = 30;
	const int NPC30_START_COL = 22;
	const int NPC31_START_ROW = 34;
	const int NPC31_START_COL = 22;
	const int NPC32_START_ROW = 46;
	const int NPC32_START_COL = 8;
	const int NPC33_START_ROW = 47;
	const int NPC33_START_COL = 13;
	const int NPC34_START_ROW = 44;
	const int NPC34_START_COL = 16;
	const int NPC35_START_ROW = 37;
	const int NPC35_START_COL = 16;
	const int NPC36_START_ROW = 47;
	const int NPC36_START_COL = 31;
	const int NPC37_START_ROW = 43;
	const int NPC37_START_COL = 31;
	const int NPC38_START_ROW = 38;
	const int NPC38_START_COL = 31;
	const int NPC39_START_ROW = 33;
	const int NPC39_START_COL = 31;
	const int NPC40_START_ROW = 35;
	const int NPC40_START_COL = 43;
	const int NPC41_START_ROW = 29;
	const int NPC41_START_COL = 48;
	const int NPC42_START_ROW = 31;
	const int NPC42_START_COL = 54;
	const int NPC43_START_ROW = 36;
	const int NPC43_START_COL = 49;
	const int NPC44_START_ROW = 37;
	const int NPC44_START_COL = 52;
	const int NPC45_START_ROW = 25;
	const int NPC45_START_COL = 50;
	const int NPC46_START_ROW = 25;
	const int NPC46_START_COL = 42;
	const int NPC47_START_ROW = 17;
	const int NPC47_START_COL = 49;
	const int NPC48_START_ROW = 19;
	const int NPC48_START_COL = 45;
	const int NPC49_START_ROW = 18;
	const int NPC49_START_COL = 55;
	const int NPC50_START_ROW = 11;
	const int NPC50_START_COL = 58;
	const int NPC51_START_ROW = 46;
	const int NPC51_START_COL = 49;
	const int NPC52_START_ROW = 45;
	const int NPC52_START_COL = 40;
	const int NPC53_START_ROW = 43;
	const int NPC53_START_COL = 46;
	const int NPC54_START_ROW = 45;
	const int NPC54_START_COL = 56;
	const int NPC55_START_ROW = 48;
	const int NPC55_START_COL = 42;
	const int NPC56_START_ROW = 48;
	const int NPC56_START_COL = 54;
	const int NPC57_START_ROW = 37;
	const int NPC57_START_COL = 61;
	const int NPC58_START_ROW = 24;
	const int NPC58_START_COL = 49;
	const int NPC59_START_ROW = 24;
	const int NPC59_START_COL = 24;
	const int NPC60_START_ROW = 48;
	const int NPC60_START_COL = 2;
	const int NPC61_START_ROW = 42;
	const int NPC61_START_COL = 19;
	const int NPC62_START_ROW = 11;
	const int NPC62_START_COL = 47;
	const int NPC63_START_ROW = 20;
	const int NPC63_START_COL = 41;
	const int NPC64_START_ROW = 47;
	const int NPC64_START_COL = 37;
	const int NPC65_START_ROW = 43;
	const int NPC65_START_COL = 37;
	const int NPC66_START_ROW = 37;
	const int NPC66_START_COL = 37;
	const int NPC67_START_ROW = 33;
	const int NPC67_START_COL = 37;;
	const int NPC68_START_ROW = 29;
	const int NPC68_START_COL = 37;
	const int NPC69_START_ROW = 42;
	const int NPC69_START_COL = 24;
	const int NPC70_START_ROW = 14;
	const int NPC70_START_COL = 19;
	const int NPC71_START_ROW = 18;
	const int NPC71_START_COL = 352;
	const int NPC72_START_ROW = 21;
	const int NPC72_START_COL = 5;
	const int NPC73_START_ROW = 26;
	const int NPC73_START_COL = 17;
	const int NPC74_START_ROW = 23;
	const int NPC74_START_COL = 58;
	const int NPC75_START_ROW = 11;
	const int NPC75_START_COL = 43;
	const int NPC76_START_ROW = 41;
	const int NPC76_START_COL = 55;
	const int NPC77_START_ROW = 15;
	const int NPC77_START_COL = 37;
	const int NPC78_START_ROW = 42;
	const int NPC78_START_COL = 9;
	const int NPC79_START_ROW = 33;
	const int NPC79_START_COL = 5;
	const int NPC80_START_ROW = 41;
	const int NPC80_START_COL = 45;
	const int NPC20_START_ROW = 38;
	const int NPC20_START_COL = 66;
	const int NPC19_START_ROW = 16;
	const int NPC19_START_COL = 25;
	const int NPC18_START_ROW = 25;
	const int NPC18_START_COL = 21;
	const int NPC17_START_ROW = 26;
	const int NPC17_START_COL = 12;
	const int NPC16_START_ROW = 24;
	const int NPC16_START_COL = 5;
	const int NPC15_START_ROW = 19;
	const int NPC15_START_COL = 12;
	const int NPC14_START_ROW = 18;
	const int NPC14_START_COL = 28;
	const int NPC13_START_ROW = 11;
	const int NPC13_START_COL = 26;
	const int NPC12_START_ROW = 23;
	const int NPC12_START_COL = 17;
	const int NPC11_START_ROW = 16;
	const int NPC11_START_COL = 17;
	const int NPC10_START_ROW = 7;
	const int NPC10_START_COL = 6;
	const int NPC9_START_ROW = 7;
	const int NPC9_START_COL = 18;
	const int NPC8_START_ROW = 9;
	const int NPC8_START_COL = 7;
	const int NPC7_START_ROW = 12;
	const int NPC7_START_COL = 18;
	const int NPC6_START_ROW = 9;
	const int NPC6_START_COL = 16;
	const int NPC5_START_ROW = 9;
	const int NPC5_START_COL = 13;
	const int NPC4_START_ROW = 18;
	const int NPC4_START_COL = 3;
	const int NPC3_START_ROW = 12;
	const int NPC3_START_COL = 3;
	const int NPC2_START_ROW = 11;
	const int NPC2_START_COL = 5;
	const int NPC_START_ROW = 5;
	const int NPC_START_COL = 7;
	const int NPC_OFFSET_X = 0;
	const int NPC_OFFSET_Y = 0;
	const int NPC_HEIGHT = 42;
	const int NPC_TEXTURE_WIDTH = 145;
	const int NPC_TEXTURE_HEIGHT = 151;
	const int NPC_TEXTURE_COLS = 18;
    const int TREE_TEXTURE_COLS = 2;
    const int TREE_TEXTURE_SIZE = 128;
	const int TREE_OFFSET_X = 0;      // add to map X to get tree X
	const int TREE_OFFSET_Y = -83;    // add to map Y to get tree Y
	const int TREE0_SHADOW = 2;
	const int TREE1_SHADOW = 3;
	const int TREE0_FRAME = 0;
	const int TREE1_FRAME = 1;
	const float TREE_SHADOW_DEGREES = 335;
	const float TREE_SHADOW_X = -21;  // offset after rotation
	const float TREE_SHADOW_Y = 5;
	const int TREE2_TEXTURE_COLS = 7;
	const int TREE2_TEXTURE_SIZE = 189;
	const int TREE2_OFFSET_X = 0;     // add to map X to get tree2 X
	const int TREE2_OFFSET_Y = -100;  // add to map Y to get tree2 Y
	const int TREE2_FRAME0 = 0;
	const int TREE2_FRAME1 = 1;
	const int TREE2_FRAME2 = 2;
	const int TREE2_FRAME3 = 3;
	const int TREE2_FRAME4 = 4;
	const int TREE2_FRAME5 = 5;
	const int TREE2_FRAME6 = 6;
	const int FENCE_TEXTURE_COLS = 8;
	const int FENCE_TEXTURE_SIZE = 128;	
	const int FENCE_OFFSET_X = 0;     // add to map X to get fence X
	const int FENCE_OFFSET_Y = -62;   // add to map Y to get fence Y
	const int FENCE_CORNER_OFFSET_Y = -58;
	const int FENCE_FRAME1 = 1;
	const int FENCE_FRAME3 = 3;
	const int FENCE_FRAME5 = 5;
	const int FENCE_FRAME7 = 7;
	const int FENCE_FRAME9 = 9;
	const int FENCE_FRAME11 = 11;
	const int FENCE_FRAME13 = 13;
	const int FENCE_FRAME15 = 15;
	const int WELL_TEXTURE_SIZE = 128;
	const int WELL_TEXTURE_COLS = 2;
	const int WELL_OFFSET_X = 0;      // add to map X to get well X
	const int WELL_OFFSET_Y = -75;    // add to map Y to get well Y
	const int WELL_FRAME0 = 0;
	const int WELL_FRAME1 = 1;
	const int TOWER_TEXTURE_WIDTH = 256;
	const int TOWER_TEXTURE_HEIGHT = 128;
	const int TOWER_TEXTURE_COLS = 4;
	const int TOWER_OFFSET_Y = -75;   // add to map Y to get tower Y
	const int TOWER_OFFSET_X = 0;     // add to map X to get tower X
	const int TOWER_FRAME0 = 0;
	const int TOWER_FRAME7 = 7;
	const int HUT_TEXTURE_WIDTH = 430;
	const int HUT_TEXTURE_HEIGHT = 306;
	const int HUT_TEXTURE_COLS = 1;
	const int HUT_OFFSET_X = -10;       // add to map Y to get hut Y
	const int HUT_OFFSET_Y = -10;     // add to map X to get hut X
	const int HUT_FRAME0 = 0;
	const int BUILDING_TEXTURE_WIDTH = 570;
	const int BUILDING_TEXTURE_HEIGHT = 414;
	const int BUILDING_TEXTURE_COLS = 1;
	const int BUILDING_OFFSET_X = -70;       // add to map Y to get building Y
	const int BUILDING_OFFSET_Y = -77;     // add to map X to get building X
	const int BUILDING_FRAME0 = 0;
	const int BUILDING2_TEXTURE_WIDTH = 600;
	const int BUILDING2_TEXTURE_HEIGHT = 478;
	const int BUILDING2_TEXTURE_COLS = 1;
	const int BUILDING2_OFFSET_X = -25;       // add to map Y to get building2 Y
	const int BUILDING2_OFFSET_Y = -92;     // add to map X to get building2 X
	const int BUILDING2_FRAME0 = 0;
	const int BUILDING3_TEXTURE_WIDTH = 550;
	const int BUILDING3_TEXTURE_HEIGHT = 410;
	const int BUILDING3_TEXTURE_COLS = 1;
	const int BUILDING3_OFFSET_X = -40;       // add to map Y to get building3 Y
	const int BUILDING3_OFFSET_Y = -65;     // add to map X to get building3 X
	const int BUILDING3_FRAME0 = 0;
	const int BUILDING4_TEXTURE_WIDTH = 700;
	const int BUILDING4_TEXTURE_HEIGHT = 473;
	const int BUILDING4_TEXTURE_COLS = 1;
	const int BUILDING4_OFFSET_X = -35;       // add to map Y to building4 tower Y
	const int BUILDING4_OFFSET_Y = -25;     // add to map X to get building4 X
	const int BUILDING4_FRAME0 = 0;
	const int BUILDING5_TEXTURE_WIDTH = 800;
	const int BUILDING5_TEXTURE_HEIGHT = 600;
	const int BUILDING5_TEXTURE_COLS = 1;
	const int BUILDING5_OFFSET_X = -50;       // add to map Y to get building5 Y
	const int BUILDING5_OFFSET_Y = -70;     // add to map X to get building5 X
	const int BUILDING5_FRAME0 = 0;
	const int ROCK_TEXTURE_WIDTH = 128;
	const int ROCK_TEXTURE_HEIGHT = 191;
	const int ROCK_TEXTURE_COLS = 3;
	const int ROCK_OFFSET_X = 0;
	const int ROCK_OFFSET_Y = -92;
	const int ROCK_FRAME0 = 0;
	const int ROCK_FRAME2 = 2;
	const int ROCK_FRAME5 = 5;
	const int WATCHTOWER_TEXTURE_WIDTH = 900;
	const int WATCHTOWER_TEXTURE_HEIGHT = 676;
	const int WATCHTOWER_TEXTURE_COLS = 2;
	const int WATCHTOWER_OFFSET_X = 0;
	const int WATCHTOWER_OFFSET_Y = -232;
	const int WATCHTOWER_FRAME0 = 0;
	const int WATCHTOWER_FRAME1 = 1;
	const int BARRICADE_TEXTURE_SIZE = 128;
	const int BARRICADE_TEXTURE_COLS = 4;
	const int BARRICADE_OFFSET_X = 0;    // add to map X to get barricade X
	const int BARRICADE_OFFSET_Y = -60;    // add to map Y to get barricade Y
	const int BARRICADE_FRAME0 = 0;
	const int BARRICADE_FRAME1 = 1;
	const int BARRICADE_FRAME2 = 2;
	const int BARRICADE_FRAME3 = 3;
	const int BARRICADE_FRAME4 = 4;
	const int BARRICADE_FRAME5 = 5;
	const int BARRICADE_FRAME6 = 6;
	const int BARRICADE_FRAME7 = 7;
	const int BARRICADE_FRAME8 = 8;
	const int BARRICADE_FRAME9 = 9;
	const int POND_TEXTURE_WIDTH = 310;
	const int POND_TEXTURE_HEIGHT = 166;
	const int POND_TEXTURE_COLS = 1;
	const int POND_OFFSET_X = 0;
	const int POND_OFFSET_Y = 68;
	const int POND_FRAME0 = 0;
	const int BUILDING6_TEXTURE_WIDTH = 315;
	const int BUILDING6_TEXTURE_HEIGHT = 255;
	const int BUILDING6_TEXTURE_COLS = 2;
	const int BARRAKS_OFFSET_X = 0;
	const int BARRAKS_OFFSET_Y = -20;
	const int BARRACKS_FRAME1 = 1;
	const int BLACKSMITH_OFFSET_X = 0;
	const int BLACKSMITH_OFFSET_Y = -10;
	const int BLACKSMITH_FRAME0 = 0;
	const int ARCHERY_OFFSET_X = 0;
	const int ARCHER_OFFSET_Y = 0;
	const int ARCHERY_FRAME2 = 2;
	const int STABLE_OFFSET_Y = -10;
	const int STABLE_OFFSET_X = 0;
	const int STABLE_FRAME3 = 3;
	const int HOUSE0_FRAME4 = 4;
	const int HOUSE1_FRAME5 = 5;
	const int HOUSE2_FRAME6 = 6;
	const int BROKENTOWER_TEXTURE_SIZE = 256;
	const int BROKENTOWER_TEXTURE_COLS = 1;
	const int BROKENTOWER_OFFSET_X = 0;
	const int BROKENTOWER_OFFSET_Y = -58;
	const int BROKENTOWER_FRAME0 = 0;
	const int TIMBERED_HOUSE_TEXTURE_WIDTH = 350;
	const int TIMBERED_HOUSE_TEXTURE_HEIGHT = 297;
	const int TIMBERED_HOUSE_TEXTURE_COLS = 2;
	const int TIMBERED_HOUSE_OFFSET_X = 0;
	const int TIMBERED_HOUSE_OFFSET_Y = -30;
	const int TIMBERED_HOUSE_FRAME0 = 0;
	const int TIMBERED_HOUSE_FRAME1 = 1;
	const int CHEST_TEXTURE_WIDTH = 256;
	const int CHEST_TEXTURE_HEIGHT = 218;
	const int CHEST_TEXTURE_COLS = 1;
	const int CHEST_OFFSET_X = 0;
	const int CHEST_OFFSET_Y = -36;
	const int CHEST_FRAME0 = 0;

    //=============================================================================
    class Game
    {
    protected:
        // Game objects and variables.

        // The XAML control where our graphics are displayed. Our game window.
        Xaml::CanvasAnimatedControl^ XamlCanvas;
        Input   *input;     // Pointer to Input object.
        Audio   *audio;     // XAudio2 player object
        TextWin2D *textWin2D;

		// Object Texture Variables
        Microsoft::Graphics::Canvas::CanvasBitmap^ tileTextures;
        Microsoft::Graphics::Canvas::CanvasBitmap^ treeTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ tree2Textures;
        Microsoft::Graphics::Canvas::CanvasBitmap^ menuTexture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ fenceTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ wellTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ hutTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ buildingTexture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ building2Texture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ building3Texture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ building4Texture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ building5Texture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ building6Textures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ watchTowerTexture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ towerTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ rockTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ outpostTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ barricadeTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ pondTexture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ archerTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ brokenTowerTexture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ timberedHouseTexture;
		Microsoft::Graphics::Canvas::CanvasBitmap^ chestTexture;

		// Player/NPC Texture Variables
		Microsoft::Graphics::Canvas::CanvasBitmap^ playerTextures;
		Microsoft::Graphics::Canvas::CanvasBitmap^ npc01Textures;

		// Map File
		std::ifstream mapData;

		// Maps
		int tileMap[MAP_SIZE][MAP_SIZE];
		int objectMap[MAP_SIZE][MAP_SIZE];    // 0=empty, 1=Tree0, 2=Tree1, 3=Fence8, 4=Fence12, 5=Fence14, 6=Fence10, 7=Water4
		
		// Object textures
        Image   menu;
        Image   mapTile;
        Image   tree;
		Image	tree2;
		Image	well;
		Image	hut;
		Image	building;
		Image	building2;
		Image	building3;
		Image	building4;
		Image	building5;
		Image	building6;
		Image	rock;
		Image	fence;
		Image	tower;
		Image	watchTower;
		Image	barricade;
		Image	pond;
		Image	brokenTower;
		Image	timberedHouse;
		Image	chest;
		Player  player;				// player object
		Character npc01;
		Character npc02;
		Character npc03;
		Character npc04;
		Character npc05;
		Character npc06;
		Character npc07;
		Character npc08;
		Character npc09;
		Character npc10;
		Character npc11;
		Character npc12;
		Character npc13;
		Character npc14;
		Character npc15;
		Character npc16;
		Character npc17;
		Character npc18;
		Character npc19;
		Character npc20;
		Character npc21;
		Character npc22;
		Character npc23;
		Character npc24;
		Character npc25;
		Character npc26;
		Character npc27;
		Character npc28;
		Character npc29;
		Character npc30;
		Character npc31;
		Character npc32;
		Character npc33;
		Character npc34;
		Character npc35;
		Character npc36;
		Character npc37;
		Character npc38;
		Character npc39;
		Character npc40;
		Character npc41;
		Character npc42;
		Character npc43;
		Character npc44;
		Character npc45;
		Character npc46;
		Character npc47;
		Character npc48;
		Character npc49;
		Character npc50;
		Character npc51;
		Character npc52;
		Character npc53;
		Character npc54;
		Character npc55;
		Character npc56;
		Character npc57;
		Character npc58;
		Character npc59;
		Character npc60;
		Character npc61;
		Character npc62;
		Character npc63;
		Character npc64;
		Character npc65;
		Character npc66;
		Character npc67;
		Character npc68;
		Character npc69;
		Character npc70;
		Character npc71;
		Character npc72;
		Character npc73;
		Character npc74;
		Character npc75;
		Character npc76;
		Character npc77;
		Character npc78;
		Character npc79;
		Character npc80;
		
		// Other variables
		int cnt = 0;
		float   mapY;				// map's Y location
		float	mapX;				// map's X location
		float   scrollX;			// Scroll the map -left/+right this amount.
		float   scrollY;			// Scroll the map -up/+down this amount.
		float   mapZoom;			// Map zoom, 1.0 = no zoom.
		float   oldZoom;			// Previous zoom amount.
		int		mouseWheel;			// Used to hold mouseWheelDelta value 
		int     selectedCol,		// Selected tile column
				selectedRow;		// Selected tile row
		int     playerColumn,		// The column the player is in.
			    playerRow;			// The row the player is in.
		float   mouseXMap;          // Pointer locations relative to map.
		float   mouseYMap;
		float   pointerX;           // Raw pointer location.
		float   pointerY;
		int		playerHealth;
		int		npc01Health;
		int		playerLives;
		bool	showDebug;			// Displays DEBUG info when true
		bool	highlightTiles = true;
		float	playerX;  // save current position
		float	playerY;
		float	playerXMap,
				playerYMap;
		bool	attack = false;
		bool	playerFound = true;
		bool	menuOn;
		bool	timerOn;
		double  timer;
		int		timerMin;
		bool    respawnTimerOn;
		double	respawnTimer;
		bool	win, lose;
		int		npcRow, npcCol;
		int		npc2Row, npc2Col;
		int		npc3Row, npc3Col;
		int		npc4Row, npc4Col;
		int		npc5Row, npc5Col;
		int		npc6Row, npc6Col;
		int		npc7Row, npc7Col;
		int		npc8Row, npc8Col;
		int		npc9Row, npc9Col;
		int		npc10Row, npc10Col;
		int		npc11Row, npc11Col;
		int		npc12Row, npc12Col;
		int		npc13Row, npc13Col;
		int		npc14Row, npc14Col;
		int		npc15Row, npc15Col;
		int		npc16Row, npc16Col;
		int		npc17Row, npc17Col;
		int		npc18Row, npc18Col;
		int		npc19Row, npc19Col;
		int		npc20Row, npc20Col;
		int		npc21Row, npc21Col;
		int		npc22Row, npc22Col;
		int		npc23Row, npc23Col;
		int		npc24Row, npc24Col;
		int		npc25Row, npc25Col;
		int		npc26Row, npc26Col;
		int		npc27Row, npc27Col;
		int		npc28Row, npc28Col;
		int		npc29Row, npc29Col;
		int		npc30Row, npc30Col;
		int		npc31Row, npc31Col;
		int		npc32Row, npc32Col;
		int		npc33Row, npc33Col;
		int		npc34Row, npc34Col;
		int		npc35Row, npc35Col;
		int		npc36Row, npc36Col;
		int		npc37Row, npc37Col;
		int		npc38Row, npc38Col;
		int		npc39Row, npc39Col;
		int		npc40Row, npc40Col;
		int		npc41Row, npc41Col;
		int		npc42Row, npc42Col;
		int		npc43Row, npc43Col;
		int		npc44Row, npc44Col;
		int		npc45Row, npc45Col;
		int		npc46Row, npc46Col;
		int		npc47Row, npc47Col;
		int		npc48Row, npc48Col;
		int		npc49Row, npc49Col;
		int		npc50Row, npc50Col;
		int		npc51Row, npc51Col;
		int		npc52Row, npc52Col;
		int		npc53Row, npc53Col;
		int		npc54Row, npc54Col;
		int		npc55Row, npc55Col;
		int		npc56Row, npc56Col;
		int		npc57Row, npc57Col;
		int		npc58Row, npc58Col;
		int		npc59Row, npc59Col;
		int		npc60Row, npc60Col;
		int		npc61Row, npc61Col;
		int		npc62Row, npc62Col;
		int		npc63Row, npc63Col;
		int		npc64Row, npc64Col;
		int		npc65Row, npc65Col;
		int		npc66Row, npc66Col;
		int		npc67Row, npc67Col;
		int		npc68Row, npc68Col;
		int		npc69Row, npc69Col;
		int		npc70Row, npc70Col;
		int		npc71Row, npc71Col;
		int		npc72Row, npc72Col;
		int		npc73Row, npc73Col;
		int		npc74Row, npc74Col;
		int		npc75Row, npc75Col;
		int		npc76Row, npc76Col;
		int		npc77Row, npc77Col;
		int		npc78Row, npc78Col;
		int		npc79Row, npc79Col;
		int		npc80Row, npc80Col;

        bool    moveUp;
        bool    moveDown;
        bool    moveLeft;
        bool    moveRight;
		std::wstring debugString;	// Debug message string
        float   deltaTime;          // Time required for one game loop
        float   drawTime;           // Time required for one game loop
        double  runningTime;        // Total running time
        double  previousTime;       // Used to calculate draws per second
        float   instructionTimer;   // Timer for display of instruction.
        int     updateCount;
        int     drawCount;
        int     gameLoopCount;
        // Calculated ups is typically faster than displayed dps.
        double  ups;                // Updates per second. 
        double  dps;                // Draws per second.
        bool    initialized;	

    public:
        // Constructor
        Game();

        // Destructor
        virtual ~Game();

        //=====================================================================
        // Return XAML Canvas (our main display target).
        //=====================================================================
        Xaml::CanvasAnimatedControl^ getXamlCanvas() { return XamlCanvas; }

        //=====================================================================
        // Return game width.
        //=====================================================================
        int getWidth() 
        { 
            if (XamlCanvas)
                return static_cast<int>(XamlCanvas->Size.Width);
            else
                return 0;
        }

        //=====================================================================
        // Return game height.
        //=====================================================================
        int getHeight() 
        { 
            if (XamlCanvas)
                return static_cast<int>(XamlCanvas->Size.Height);
            else
                return 0;
        }

        //=====================================================================
        // Initialize the game resources
        //=====================================================================
        void initialize(Xaml::CanvasAnimatedControl^ sender,
            CanvasCreateResourcesEventArgs^ args);

        //=====================================================================
        // Return pointer to Audio.
        //=====================================================================
        Audio* getAudio() { return audio; }

        //=====================================================================
        // Draw one frame of the game.
        //=====================================================================
        void draw(Xaml::CanvasAnimatedDrawEventArgs^ args);

        //=====================================================================
        // Update the game items.
        //=====================================================================
        void update(Xaml::CanvasAnimatedUpdateEventArgs^ args);

        //=====================================================================
        // Called when a key is pressed down.
        //=====================================================================
        void onKeyDown(CoreWindow^ sender, KeyEventArgs^ args);

        //=====================================================================
        // Called when a key is released.
        //=====================================================================
        void onKeyUp(CoreWindow^ sender, KeyEventArgs^ args);

        //=====================================================================
        // Called when a char is received.
        //=====================================================================
        void onCharReceived(CoreWindow^ sender, CharacterReceivedEventArgs^ args);

        //=====================================================================
        // Called when a pointer is pressed.
        //=====================================================================
        void onPointerPressed(CoreWindow^ sender, PointerEventArgs^ args);

        //=====================================================================
        // Called when a pointer is pressed.
        //=====================================================================
        void onPointerWheelChanged(CoreWindow^ sender, PointerEventArgs^ args);

		//=====================================================================
		// Called when a pointer is moved.
		//=====================================================================
		void onPointerMoved(CoreWindow^ sender, PointerEventArgs^ args);

		//=====================================================================
		// Called when a gamepad is added.
		//=====================================================================
		void onGamepadAdded(Platform::Object ^sender,
			Windows::Gaming::Input::Gamepad ^args);
		
		//=====================================================================
        // Artificial Intelligence
        // Complete this function to add AI to your game.
        //=====================================================================
        void ai();

        //=====================================================================
        // Handle collisions
        // Complete this function to add collision detection to your game.
        //=====================================================================
        void collisions();

		////////////////////////////////////////
		//           Get functions            //
		////////////////////////////////////////
		
		//=====================================================================
		// Get Column
		// Returns the current column of the player.
		//=====================================================================
		int   getColumn(Player &player, int offsetX, int offsetY);

		//=====================================================================
		// Get Column
		// Returns the current column of the NPC.
		//=====================================================================
		int   getColumn(Character &npc, int offsetX, int offsetY);

		//=====================================================================
		// Get Selected Column
		// Returns the selected column from mouse pointer.
		//=====================================================================
		int	  getSelectedColumn(float mouseXMap, float mouseYMap);

		//=====================================================================
		// Get Row
		// Returns the current row of the NPC.
		//=====================================================================
		int   getRow(Character &npc, int offsetX, int offsetY);

		//=====================================================================
		// Get Row
		// Returns the current row of the player.
		//=====================================================================
		int   getRow(Player &player, int offsetX, int offsetY);

		//=====================================================================
		// Get Selected Row
		// Returns the selected row from mouse pointer.
		//=====================================================================
		int   getSelectedRow(float mouseXMap, float mouseYMap);

		////////////////////////////////////////
		//           Other functions          //
		////////////////////////////////////////

		//=====================================================================
		// Initialize NPC
		// Initialize NPC on start
		//=====================================================================
		void  initializeNPC(Character &npc, int npcRow, int npcCol);

		//=====================================================================
		// Position NPC
		// Positions the NPC to its start position.
		//=====================================================================
		void  positionNPC(Character &npc, int startRow, int startCol,
			int offsetX, int offsetY);

		//=====================================================================
		// Position Player
		// Positions the player to their start position.
		//=====================================================================
		void  positionPlayer(Player &npc, int startRow, int startCol,
			int offsetX, int offsetY);

		//=====================================================================
		// Draw NPC
		// Draws the NPC object.
		//=====================================================================
		void  drawNPC(Character &npc,CanvasSpriteBatch^ spriteBatch);

		//=====================================================================
		// Draw Player
		// Draws the Player object.
		//=====================================================================
		void  drawPlayer(Player &player, CanvasSpriteBatch^ spriteBatch);
		
		//=====================================================================
		// Draw Object
		// Draws the other objects within ObjectMap, such as trees,houses,etc.
		//=====================================================================
		void  drawObject(Image &imgObj, int row, int col, int offsetX,
			int offsetY, int frame, CanvasSpriteBatch^ spriteBatch);	

		//=====================================================================
		// Check Enemy Range
		// Checks the 8 tiles around the NPC for the player. Attacks when found
		//=====================================================================
		bool  checkEnemyRange(Character &obj, int npcRow, int npcCol);

		//=====================================================================
		// Check Player Range
		// Checks the 8 tiles around the player for NPC. Attacks when found
		//=====================================================================
		void  checkPlayerRange(Player &obj, int playerRow, int playerColumn, 
			int npcObjectNum, Character &npc);

		//=====================================================================
		// Zoom In
		// Zoom in on the map.
		//=====================================================================
		void  zoomIn(int selectedRow, int selectedCol, float mouseXMap,
			float mouseYMap, float pointerX, float pointerY);

		//=====================================================================
		// Zoom Out
		// Zoom Out on the map.
		//=====================================================================
		void  zoomOut();

		//=====================================================================
		// Update NPC Location
		// Update NPC location in Object Map
		//=====================================================================
		void  updateNpcLocation(Character &character, int &npcCol, 
			int &npcRow, int npcObjectNum);

		//=====================================================================
		// Update Player Location
		// Update player location in Object Map
		//=====================================================================
		void  updatePlayerLocation(Player &player, int &playerColumn,
			int &playerRow, int playerObjectNum);

		//=====================================================================
		// Find Player
		// Checks the 8 tiles around the NPC for the player. Attacks when found
		//=====================================================================
		void  findPlayer(Character &npc, int npcRow, int npcCol);;
    };
}