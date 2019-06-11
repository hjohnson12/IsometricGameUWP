
/*
Name: IsometricGameUWP
Programmer: Hunter Johnson
Date: 3/20/17
Class: CIS 267 Beginning Game Programming
*/

#include "pch.h"
#include "Game.h"
using namespace GameEngine2;
using namespace Windows::System;
using namespace Microsoft::Graphics::Canvas;
//using namespace Microsoft::Graphics::Canvas::Effects;

//=============================================================================
// Constructor
//=============================================================================
GameEngine2::Game::Game()
{
    input = new Input();    // Initialize keyboard input.
    updateCount = 0;
    drawCount = 0;
    gameLoopCount = 0;
    ups = 30;
    dps = 30;
    previousTime = 1;
    instructionTimer = INSTRUCTION_DISPLAY_TIME;
    audio = nullptr;
	scrollX = 0;            // Scroll map -left/+right.
	scrollY = 0;            // Scroll map -up/+down.
	mapZoom = 1.0f;			// Map zoom, 1.0 = no zoom.
	playerRow = PLAYER_START_ROW;
	playerColumn = PLAYER_START_COL;
	npcRow = NPC_START_ROW;
	npcCol = NPC_START_COL;
	menuOn = true;
	timerOn = false;
	respawnTimerOn = false;
}

//=============================================================================
// Destructor
//=============================================================================
GameEngine2::Game::~Game()
{
    delete audio;
    audio = nullptr;
}

//=============================================================================
// Initialize the game resources.
//=============================================================================
void Game::initialize(Xaml::CanvasAnimatedControl^ sender,
    CanvasCreateResourcesEventArgs^ args)
{
    XamlCanvas = sender;    // Save the game's Xaml Canvas

    // Attempt to initialize audio devics.
    // This can fail when there are no audio devices
    try
    {
        audio = new Audio(SOUND_SAMPLE_RATE);
    }
    catch (Platform::Exception^)
    {
        audio = nullptr;
    }

	textWin2D = new TextWin2D();  // Win2D text object

    // Load texture images from file
    try {
		// Menu Texture
		menuTexture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\menuBackground.png"));

		// Object Textures
        tileTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
            "Textures\\groundTextures.png"));
        treeTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
            "Textures\\treeTextures.png"));
		tree2Textures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\tree2Textures.png"));
		fenceTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\fenceTextures.png"));				
		wellTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\wellTextures.png"));
		hutTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\hutTexture.png"));
		towerTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\towerTextures.png"));
		watchTowerTexture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\watchTowerTextures.png"));
		buildingTexture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\buildingTexture.png"));
		building2Texture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\building2Texture.png"));
		building3Texture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\building3Texture.png"));
		building4Texture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\building4Texture.png"));
		building5Texture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\building5Texture.png"));
		rockTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\rockTextures.png"));
		barricadeTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\barricadeTextures.png"));
		pondTexture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\pondTexture.png"));
		building6Textures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\building6Textures.png"));
		brokenTowerTexture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\brokenTowerTexture.png"));
		timberedHouseTexture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\timberedHouseTexture.png"));
		chestTexture = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\chestTexture.png"));

		// Player/NPC Archer texture images
		playerTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\playerTextures.png"));
		archerTextures = Utilities::WaitExecution(CanvasBitmap::LoadAsync(sender,
			"Textures\\archerTexturess.png"));
    }
    catch (...) 
    {
        //throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing texture images."));
        exit(1);
    }

    // Menu image
    if (!menu.initialize(0, 0, 1, menuTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu image."));
    menu.setCenter(Vector2(GAME_WIDTH / 2, GAME_HEIGHT / 2));

    // Load map file
    try {
		std::string str;
		mapData.open("Maps\\map1.txt");
		if (!mapData)                            // if file error
			throw(GameError(gameErrorNS::FATAL_ERROR,
				"Error loading map data"));

		// Load tileMap
		std::getline(mapData, str);              // skip 1st comment line
		for (int row = 0; row<MAP_HEIGHT; row++)
			for (int col = 0; col<MAP_WIDTH; col++)
				mapData >> tileMap[row][col];   // read Tile map

		// Load objectMap
		std::getline(mapData, str);             // skip newline
		std::getline(mapData, str);             // skip empty row
		std::getline(mapData, str);             // skip comment line
		for (int row = 0; row<MAP_HEIGHT; row++)
			for (int col = 0; col<MAP_WIDTH; col++)
				mapData >> objectMap[row][col]; // read Object map
    }
    catch (...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error loading map data"));
    }

	// Map tile image
	if (!mapTile.initialize(TEXTURE_SIZE_TERRAIN, TEXTURE_SIZE_TERRAIN,
		TEXTURE_COLS_TERRAIN, tileTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile image."));

	// Tree image
    if (!tree.initialize(TREE_TEXTURE_SIZE, TREE_TEXTURE_SIZE
		, TREE_TEXTURE_COLS, treeTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tree image."));
    tree.setFrames(TREE0_FRAME, TREE1_FRAME);
    tree.setCurrentFrame(TREE0_FRAME);

	// Tree2 image
	if (!tree2.initialize(TREE2_TEXTURE_SIZE, TREE2_TEXTURE_SIZE,
		TREE2_TEXTURE_COLS, tree2Textures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tree2 image."));
	tree2.setFrames(TREE2_FRAME0, TREE2_FRAME6);

	// Fence image
	if (!fence.initialize(FENCE_TEXTURE_SIZE, FENCE_TEXTURE_SIZE,
		FENCE_TEXTURE_COLS, fenceTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing fence image."));
	fence.setFrames(FENCE_FRAME1, FENCE_FRAME15);

	// Well image
	if (!well.initialize(WELL_TEXTURE_SIZE, WELL_TEXTURE_SIZE,
		WELL_TEXTURE_COLS, wellTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	well.setFrames(WELL_FRAME0, WELL_FRAME1);

	// Hut image
	if (!hut.initialize(HUT_TEXTURE_WIDTH, HUT_TEXTURE_HEIGHT,
		HUT_TEXTURE_COLS, hutTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	hut.setFrames(HUT_FRAME0, HUT_FRAME0);

	// Building image
	if (!building.initialize(BUILDING_TEXTURE_WIDTH, BUILDING_TEXTURE_HEIGHT,
		BUILDING_TEXTURE_COLS, buildingTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
		building.setFrames(BUILDING_FRAME0, BUILDING_FRAME0);

	// Building2 image
	if (!building2.initialize(BUILDING2_TEXTURE_WIDTH, BUILDING2_TEXTURE_HEIGHT,
		BUILDING2_TEXTURE_COLS, building2Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	building2.setFrames(BUILDING2_FRAME0, BUILDING2_FRAME0);

	// Building3 image
	if (!building3.initialize(BUILDING3_TEXTURE_WIDTH, BUILDING3_TEXTURE_HEIGHT,
		BUILDING3_TEXTURE_COLS, building3Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	building3.setFrames(BUILDING3_FRAME0, BUILDING3_FRAME0);

	// Building4 image
	if (!building4.initialize(BUILDING4_TEXTURE_WIDTH, BUILDING4_TEXTURE_HEIGHT,
		BUILDING4_TEXTURE_COLS, building4Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	building4.setFrames(BUILDING4_FRAME0, BUILDING4_FRAME0);

	// Building5 image
	if (!building5.initialize(BUILDING5_TEXTURE_WIDTH, BUILDING5_TEXTURE_HEIGHT,
		BUILDING5_TEXTURE_COLS, building5Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	building5.setFrames(BUILDING5_FRAME0, BUILDING5_FRAME0);

	// Tower image
	if (!tower.initialize(TOWER_TEXTURE_WIDTH, TOWER_TEXTURE_HEIGHT,
		TOWER_TEXTURE_COLS, towerTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	tower.setFrames(TOWER_FRAME0, TOWER_FRAME7);

	// Rock image
	if (!rock.initialize(ROCK_TEXTURE_WIDTH, ROCK_TEXTURE_HEIGHT,
		ROCK_TEXTURE_COLS, rockTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	rock.setFrames(ROCK_FRAME0, ROCK_FRAME5);

	// watchTower image
	if (!watchTower.initialize(WATCHTOWER_TEXTURE_WIDTH, WATCHTOWER_TEXTURE_HEIGHT,
		WATCHTOWER_TEXTURE_COLS, watchTowerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	watchTower.setFrames(WATCHTOWER_FRAME0, WATCHTOWER_FRAME1);

	// Barricade image
	if (!barricade.initialize(BARRICADE_TEXTURE_SIZE, BARRICADE_TEXTURE_SIZE,
		BARRICADE_TEXTURE_COLS, barricadeTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	barricade.setFrames(BARRICADE_FRAME0, BARRICADE_FRAME9);

	// Pond image
	if (!pond.initialize(POND_TEXTURE_WIDTH, POND_TEXTURE_HEIGHT,
		POND_TEXTURE_COLS, pondTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	pond.setFrames(POND_FRAME0, POND_FRAME0);

	// building6 image
	if (!building6.initialize(BUILDING6_TEXTURE_WIDTH, BUILDING6_TEXTURE_HEIGHT,
		BUILDING6_TEXTURE_COLS, building6Textures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	building6.setFrames(BLACKSMITH_FRAME0, 6);

	// brokenTower image
	if (!brokenTower.initialize(BROKENTOWER_TEXTURE_SIZE, BROKENTOWER_TEXTURE_SIZE,
		BROKENTOWER_TEXTURE_COLS, brokenTowerTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing well image."));
	brokenTower.setFrames(BROKENTOWER_FRAME0, BROKENTOWER_FRAME0);

	// timberedHouse image
	if (!timberedHouse.initialize(TIMBERED_HOUSE_TEXTURE_WIDTH, TIMBERED_HOUSE_TEXTURE_HEIGHT,
		TIMBERED_HOUSE_TEXTURE_COLS, timberedHouseTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing timbered house image."));
	timberedHouse.setFrames(TIMBERED_HOUSE_FRAME0, TIMBERED_HOUSE_FRAME1);

	// chest image
	if (!chest.initialize(CHEST_TEXTURE_WIDTH, CHEST_TEXTURE_HEIGHT,
		CHEST_TEXTURE_COLS, chestTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing timbered house image."));
	chest.setFrames(CHEST_FRAME0, CHEST_FRAME0);
	
	// Player image
	if (!player.initialize(audio, playerTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player image."));
	// Position player on start
	positionPlayer(player, PLAYER_START_ROW, PLAYER_START_COL, PLAYER_OFFSET_X,
		PLAYER_OFFSET_Y);

	// Initialize NPC texture image and position NPC
	initializeNPC(npc01, NPC_START_ROW, NPC_START_COL);
	initializeNPC(npc02, NPC2_START_ROW, NPC2_START_COL);
	initializeNPC(npc03, NPC3_START_ROW, NPC3_START_COL);
	initializeNPC(npc04, NPC4_START_ROW, NPC4_START_COL);
	initializeNPC(npc05, NPC5_START_ROW, NPC5_START_COL);
	initializeNPC(npc06, NPC6_START_ROW, NPC6_START_COL);
	initializeNPC(npc07, NPC7_START_ROW, NPC7_START_COL);
	initializeNPC(npc08, NPC8_START_ROW, NPC8_START_COL);
	initializeNPC(npc09, NPC9_START_ROW, NPC9_START_COL);
	initializeNPC(npc10, NPC10_START_ROW, NPC10_START_COL);
	initializeNPC(npc11, NPC11_START_ROW, NPC11_START_COL);
	initializeNPC(npc12, NPC12_START_ROW, NPC12_START_COL);
	initializeNPC(npc13, NPC13_START_ROW, NPC13_START_COL);
	initializeNPC(npc14, NPC14_START_ROW, NPC14_START_COL);
	initializeNPC(npc15, NPC15_START_ROW, NPC15_START_COL);
	initializeNPC(npc16, NPC16_START_ROW, NPC16_START_COL);
	initializeNPC(npc17, NPC17_START_ROW, NPC17_START_COL);
	initializeNPC(npc18, NPC18_START_ROW, NPC18_START_COL);
	initializeNPC(npc19, NPC19_START_ROW, NPC19_START_COL);
	initializeNPC(npc20, NPC20_START_ROW, NPC20_START_COL);
	initializeNPC(npc21, NPC21_START_ROW, NPC21_START_COL);
	initializeNPC(npc22, NPC22_START_ROW, NPC22_START_COL);
	initializeNPC(npc23, NPC23_START_ROW, NPC23_START_COL);
	initializeNPC(npc24, NPC24_START_ROW, NPC24_START_COL);
	initializeNPC(npc25, NPC25_START_ROW, NPC25_START_COL);
	initializeNPC(npc26, NPC26_START_ROW, NPC26_START_COL);
	initializeNPC(npc27, NPC27_START_ROW, NPC27_START_COL);
	initializeNPC(npc28, NPC28_START_ROW, NPC28_START_COL);
	initializeNPC(npc29, NPC29_START_ROW, NPC29_START_COL);
	initializeNPC(npc30, NPC30_START_ROW, NPC30_START_COL);
	initializeNPC(npc31, NPC31_START_ROW, NPC31_START_COL);
	initializeNPC(npc32, NPC32_START_ROW, NPC32_START_COL);
	initializeNPC(npc33, NPC33_START_ROW, NPC33_START_COL);
	initializeNPC(npc34, NPC34_START_ROW, NPC34_START_COL);
	initializeNPC(npc35, NPC35_START_ROW, NPC35_START_COL);
	initializeNPC(npc36, NPC36_START_ROW, NPC36_START_COL);
	initializeNPC(npc37, NPC37_START_ROW, NPC37_START_COL);
	initializeNPC(npc38, NPC38_START_ROW, NPC38_START_COL);
	initializeNPC(npc39, NPC39_START_ROW, NPC39_START_COL);
	initializeNPC(npc40, NPC40_START_ROW, NPC40_START_COL);
	initializeNPC(npc41, NPC41_START_ROW, NPC41_START_COL);
	initializeNPC(npc42, NPC42_START_ROW, NPC42_START_COL);
	initializeNPC(npc43, NPC43_START_ROW, NPC43_START_COL);
	initializeNPC(npc44, NPC44_START_ROW, NPC44_START_COL);
	initializeNPC(npc45, NPC45_START_ROW, NPC45_START_COL);
	initializeNPC(npc46, NPC46_START_ROW, NPC46_START_COL);
	initializeNPC(npc47, NPC47_START_ROW, NPC47_START_COL);
	initializeNPC(npc48, NPC48_START_ROW, NPC48_START_COL);
	initializeNPC(npc49, NPC49_START_ROW, NPC49_START_COL);
	initializeNPC(npc50, NPC50_START_ROW, NPC50_START_COL);
	initializeNPC(npc51, NPC51_START_ROW, NPC51_START_COL);
	initializeNPC(npc52, NPC52_START_ROW, NPC52_START_COL);
	initializeNPC(npc53, NPC53_START_ROW, NPC53_START_COL);
	initializeNPC(npc54, NPC54_START_ROW, NPC54_START_COL);
	initializeNPC(npc55, NPC55_START_ROW, NPC55_START_COL);
	initializeNPC(npc56, NPC56_START_ROW, NPC56_START_COL);
	initializeNPC(npc57, NPC57_START_ROW, NPC57_START_COL);
	initializeNPC(npc58, NPC58_START_ROW, NPC58_START_COL);
	initializeNPC(npc59, NPC59_START_ROW, NPC59_START_COL);
	initializeNPC(npc60, NPC60_START_ROW, NPC60_START_COL);
	initializeNPC(npc61, NPC61_START_ROW, NPC61_START_COL);
	initializeNPC(npc62, NPC62_START_ROW, NPC62_START_COL);
	initializeNPC(npc63, NPC63_START_ROW, NPC63_START_COL);
	initializeNPC(npc64, NPC64_START_ROW, NPC64_START_COL);
	initializeNPC(npc65, NPC65_START_ROW, NPC65_START_COL);
	initializeNPC(npc66, NPC66_START_ROW, NPC66_START_COL);
	initializeNPC(npc67, NPC67_START_ROW, NPC67_START_COL);
	initializeNPC(npc68, NPC68_START_ROW, NPC68_START_COL);
	initializeNPC(npc69, NPC69_START_ROW, NPC69_START_COL);
	initializeNPC(npc70, NPC70_START_ROW, NPC70_START_COL);
	initializeNPC(npc71, NPC71_START_ROW, NPC71_START_COL);
	initializeNPC(npc72, NPC72_START_ROW, NPC72_START_COL);
	initializeNPC(npc73, NPC73_START_ROW, NPC73_START_COL);
	initializeNPC(npc74, NPC74_START_ROW, NPC74_START_COL);
	initializeNPC(npc75, NPC75_START_ROW, NPC75_START_COL);
	initializeNPC(npc76, NPC76_START_ROW, NPC76_START_COL);
	initializeNPC(npc77, NPC77_START_ROW, NPC77_START_COL);
	initializeNPC(npc78, NPC78_START_ROW, NPC78_START_COL);
	initializeNPC(npc79, NPC79_START_ROW, NPC79_START_COL);
	initializeNPC(npc80, NPC80_START_ROW, NPC80_START_COL);

    updateCount = 0;
    drawCount = 0;

    // XAudio2 initialize
    Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
    HRESULT hr;
    if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        throw Platform::Exception::CreateException(hr);

    IXAudio2MasteringVoice* pMasterVoice = NULL;
    if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
        return;

    // Audio File Names
    const char16* SOUND_FILES[] = {
        L"Wavs\\beep1_adpcm.wav",
        L"Wavs\\beep2_adpcm.wav",
        L"Wavs\\beep3_adpcm.wav",
        L"Wavs\\beep4_adpcm.wav",
        L"Wavs\\hit_adpcm.wav",
        L"Wavs\\pow_adpcm.wav",
        L"Wavs\\musicmono_adpcm.wav",
        L"Wavs\\Battle_adpcm.wav",
		L"Wavs\\swordSwoosh_adpcm.wav",
		L"Wavs\\arrowShot_adpcm.wav",
        nullptr
    };

    // Read the sound files and add them to the XAudio2 sound player.
    if (audio == nullptr)   // If no audio player
        return; // The game will play without sound

    // Continue initializing the audio
    for (size_t index = 0; SOUND_FILES[index]; ++index)
    {
        try
        {
            SoundFileReader nextSound(ref new Platform::String(SOUND_FILES[index]));
            (void)audio->AddSound(nextSound.GetSoundFormat(), nextSound.GetSoundData());
        }
        catch (Platform::FailureException^)
        {
            // If a sound fails to load just skip it.
        }
    }

    // We should have at least one sound to play
    if (audio->GetSoundCount() == 0)
    {
        audio = nullptr;    // Disable audio if no sounds loaded.
        //throw ref new Platform::FailureException();
        throw(GameError(gameErrorNS::WARNING,
            Strings::NoAudioFiles));
    }

    // Play sound then pause immediately to configure player.
    audio->PlaySound(MUSIC2, XAUDIO2_LOOP_INFINITE);
    audio->PauseSound(MUSIC2);
}

//=============================================================================
// Draw one frame of the game.
//=============================================================================
void GameEngine2::Game::draw(UI::Xaml::CanvasAnimatedDrawEventArgs^ args)
{
    // All drawing is performed in a SpriteBatch.
    CanvasSpriteBatch^ spriteBatch = args->DrawingSession->CreateSpriteBatch();

    // Calcualte total running time in seconds.
    runningTime = static_cast<double>(args->Timing.TotalTime.Duration / 10000000.0f);
    // Calcualte drawTime in seconds.
    drawTime = static_cast<float>(runningTime - previousTime);
    previousTime = runningTime;

    // Use a Moving Average to calculate average Draws Per Second (dps).
    if (drawTime > 0.0)
        dps = (dps*0.99f) + (0.01f / drawTime);  // Average dps

    // Calculate deltaTime in seconds.
    deltaTime = args->Timing.ElapsedTime.Duration / 10000000.0f;

	// Draw map in Isometric Diamond. 
	// Map is scrolled by scrollX, scrollY amount.
	// Map is zoomed by mapZoom amount.
	// Tile at top of diamond is location 0,0.
	// Negative pointer X is left of center, positive X is right of center.
	// Y = 0 is above the top of the window. The SCREEN_Y constant may be
	// changed to adjust the vertical position of the playfield.
	mapTile.setScale(mapZoom);
    for (int row = 0; row<MAP_HEIGHT; row++)
    {
        for (int col = 0; col<MAP_WIDTH; col++)
        {
			mapTile.setCurrentFrame(tileMap[row][col]);
			mapTile.setX((float)(SCREEN_X)+mapZoom * (
				-row * (TEXTURE_SIZE_TERRAIN / 2) +
				col * (TEXTURE_SIZE_TERRAIN / 2) + scrollX));
			mapTile.setY((float)(SCREEN_Y)+mapZoom * (
				row * (TEXTURE_SIZE_TERRAIN / 4) +
				col * (TEXTURE_SIZE_TERRAIN / 4) +
				// move down 1/2 tile height because tiles are drawn from
				// center x,y
				TEXTURE_SIZE_TERRAIN / 2 + scrollY));
			// Highlight Pointer Position if it's in the drawing tile
			if (row == selectedRow && col == selectedCol)
			{
				mapTile.setCurrentFrame(0);					 // Tile to draw
				mapTile.draw(spriteBatch, COLORS::LIME50);   // Highlight Lime 50%
			}
			// Highlight NPC tile locations if they're in the drawing tile 
			else if ((row == npcRow && col == npcCol) || (row == npc2Row && col == npc2Col) ||
				(row == npc3Row && col == npc3Col) || (row == npc4Row && col == npc4Col) ||
				(row == npc5Row && col == npc5Col) || (row == npc6Row && col == npc6Col) ||
				(row == npc7Row && col == npc7Col) || (row == npc8Row && col == npc8Col) ||
				(row == npc9Row && col == npc9Col) || (row == npc10Row && col == npc10Col) ||
				(row == npc11Row && col == npc11Col) || (row == npc12Row && col == npc12Col) ||
				(row == npc13Row && col == npc13Col) || (row == npc14Row && col == npc14Col) ||
				(row == npc15Row && col == npc15Col) || (row == npc16Row && col == npc16Col) ||
				(row == npc17Row && col == npc17Col) || (row == npc18Row && col == npc18Col) ||
				(row == npc19Row && col == npc19Col) || (row == npc20Row && col == npc20Col) ||
				(row == npc21Row && col == npc21Col) || (row == npc22Row && col == npc21Col) ||
				(row == npc23Row && col == npc23Col) || (row == npc24Row && col == npc24Col) ||
				(row == npc25Row && col == npc25Col) || (row == npc26Row && col == npc26Col) ||
				(row == npc27Row && col == npc27Col) || (row == npc28Row && col == npc28Col) ||
				(row == npc29Row && col == npc29Col) || (row == npc30Row && col == npc30Col) ||
				(row == npc31Row && col == npc31Col) || (row == npc32Row && col == npc32Col) ||
				(row == npc33Row && col == npc33Col) || (row == npc34Row && col == npc34Col) ||
				(row == npc35Row && col == npc35Col) || (row == npc36Row && col == npc36Col) ||
				(row == npc37Row && col == npc37Col) || (row == npc38Row && col == npc38Col) ||
				(row == npc39Row && col == npc39Col) || (row == npc40Row && col == npc40Col) ||
				(row == npc41Row && col == npc41Col) || (row == npc42Row && col == npc42Col) ||
				(row == npc43Row && col == npc43Col) || (row == npc44Row && col == npc44Col) ||
				(row == npc45Row && col == npc45Col) || (row == npc46Row && col == npc46Col) ||
				(row == npc47Row && col == npc47Col) || (row == npc48Row && col == npc48Col) ||
				(row == npc49Row && col == npc49Col) || (row == npc50Row && col == npc50Col) ||
				(row == npc51Row && col == npc51Col) || (row == npc52Row && col == npc52Col) ||
				(row == npc53Row && col == npc53Col) || (row == npc54Row && col == npc54Col) ||
				(row == npc55Row && col == npc55Col) || (row == npc56Row && col == npc56Col) ||
				(row == npc57Row && col == npc57Col) || (row == npc58Row && col == npc58Col) ||
				(row == npc59Row && col == npc59Col) || (row == npc60Row && col == npc60Col) ||
				(row == npc61Row && col == npc61Col) || (row == npc62Row && col == npc62Col) ||
				(row == npc63Row && col == npc63Col) || (row == npc64Row && col == npc64Col) ||
				(row == npc65Row && col == npc65Col) || (row == npc66Row && col == npc66Col) ||
				(row == npc67Row && col == npc67Col) || (row == npc68Row && col == npc68Col) ||
				(row == npc69Row && col == npc69Col) || (row == npc70Row && col == npc70Col) ||
				(row == npc71Row && col == npc71Col) || (row == npc72Row && col == npc72Col) ||
				(row == npc73Row && col == npc73Col) || (row == npc74Row && col == npc74Col) ||
				(row == npc75Row && col == npc75Col) || (row == npc76Row && col == npc76Col) ||
				(row == npc77Row && col == npc77Col) || (row == npc78Row && col == npc78Col) ||
				(row == npc79Row && col == npc79Col) || (row == npc80Row && col == npc80Col))
			{
				mapTile.setCurrentFrame(0);					 // Tile to draw
				mapTile.draw(spriteBatch, COLORS::RED50);    // Highlight Red 50%
			}
			// Highlight Player tile location if they're in the drawing tile
			else if (row == playerRow && col == playerColumn)
			{
				mapTile.draw(spriteBatch, COLORS::ORANGE50); // Highlight Orange 50%.
			}
			else	// Normal Tile draw.
				mapTile.draw(spriteBatch);			
        }
    }

	// Draw Objects, 0=empty, 1=TREE_0, 2=TREE_1, 3=FENCE1, etc.
    float treeX = 0, treeY = 0;
	float fenceX = 0, fenceY = 0;
	float tree2X = 0, tree2Y = 0;
	tree.setScale(mapZoom);
	tree2.setScale(mapZoom);
	fence.setScale(mapZoom);
	well.setScale(mapZoom);
	hut.setScale(mapZoom);
	tower.setScale(mapZoom);
	watchTower.setScale(mapZoom);
	building.setScale(mapZoom);
	building2.setScale(mapZoom);
	building3.setScale(mapZoom);
	building4.setScale(mapZoom);
	building5.setScale(mapZoom);
	rock.setScale(mapZoom);
	barricade.setScale(mapZoom);
	pond.setScale(mapZoom);
	building6.setScale(mapZoom);
	brokenTower.setScale(mapZoom);
	timberedHouse.setScale(mapZoom);
	chest.setScale(mapZoom);
    for (int row = 0; row<MAP_HEIGHT; row++)
    {
        for (int col = 0; col<MAP_WIDTH; col++)
        {			
            switch (objectMap[row][col])
            {
            case TREE_0:     // Tree - Large (Frame 0)
				tree.setX((float)(SCREEN_X)+mapZoom * (
					-row * (TEXTURE_SIZE_TERRAIN / 2) +
					col * (TEXTURE_SIZE_TERRAIN / 2) + TREE_OFFSET_X + scrollX));
				tree.setY((float)(SCREEN_Y)+mapZoom * (
					row * (TEXTURE_SIZE_TERRAIN / 4) +
					col * (TEXTURE_SIZE_TERRAIN / 4) +
					// Move down 1/2 tile height because tiles are drawn from
					// center x,y.
					TEXTURE_SIZE_TERRAIN / 2 + TREE_OFFSET_Y + scrollY));               
				if (col % 2)
                    tree.setFlipHorizontal(true);
                // draw shadow
                tree.setCurrentFrame(TREE0_SHADOW);
                tree.setDegrees(TREE_SHADOW_DEGREES);
                treeX = tree.getX();
                treeY = tree.getY();
                tree.setX(treeX + TREE_SHADOW_X * mapZoom);
                tree.setY(treeY + TREE_SHADOW_Y);
                tree.draw(spriteBatch,COLORS::BLACK25);
                tree.setX(treeX);   // restore X
                tree.setY(treeY);   // restore Y
                // draw tree
                tree.setDegrees(0);
                tree.setCurrentFrame(TREE0_FRAME);
                tree.draw(spriteBatch);
                tree.setFlipHorizontal(false);
                break;
            case TREE_1:     // Tree - Small (Frame 1)
				tree.setX((float)(SCREEN_X)+mapZoom * (
					-row * (TEXTURE_SIZE_TERRAIN / 2) +
					col * (TEXTURE_SIZE_TERRAIN / 2) + TREE_OFFSET_X + scrollX));
				tree.setY((float)(SCREEN_Y)+mapZoom * (
					row * (TEXTURE_SIZE_TERRAIN / 4) +
					col * (TEXTURE_SIZE_TERRAIN / 4) +
					// Move down 1/2 tile height because tiles are drawn from
					// center x,y.
					TEXTURE_SIZE_TERRAIN / 2 + TREE_OFFSET_Y + scrollY));
                if (col % 2)
                    tree.setFlipHorizontal(true);
                // draw shadow
                tree.setCurrentFrame(TREE1_SHADOW);
                tree.setDegrees(TREE_SHADOW_DEGREES);    // rotate shadow
                treeX = tree.getX();                     // save tree x,y
                treeY = tree.getY();
                tree.setX(treeX + TREE_SHADOW_X * mapZoom);        // position shadow
                tree.setY(treeY + TREE_SHADOW_Y);
                tree.draw(spriteBatch, COLORS::BLACK25); // draw shadow
                tree.setX(treeX);                        // restore tree x,y
                tree.setY(treeY);
                // draw tree
				tree.setDegrees(0);
                tree.setCurrentFrame(TREE1_FRAME);
                tree.draw(spriteBatch);
				tree.setFlipHorizontal(false);
                break;
			case FENCE1:     // Fence Bottom Right Corner - (Frame 1)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_CORNER_OFFSET_Y,
					FENCE_FRAME1, spriteBatch);
				break;
			case FENCE3:     // Fence Top Left Corner Back - (Frame 3)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_CORNER_OFFSET_Y,
					FENCE_FRAME3, spriteBatch);
				break;
			case FENCE5:     // Fence Top Right Corner - (Frame 5)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_CORNER_OFFSET_Y,
					FENCE_FRAME5, spriteBatch);
				break;
			case FENCE7:     // Fence Top Left Corner - (Frame 7)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_CORNER_OFFSET_Y,
					FENCE_FRAME7, spriteBatch);
				break;
			case FENCE9:     // Fence Back - (Frame 9)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_OFFSET_Y,
					FENCE_FRAME9, spriteBatch);
				break;
			case FENCE11:     // Fence Left Side - (Frame 11)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_OFFSET_Y,
					FENCE_FRAME11, spriteBatch);
				break;
			case FENCE13:    // Fence Front - (Frame 13)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_OFFSET_Y,
					FENCE_FRAME13, spriteBatch);
				break; 
			case FENCE15:   // Fence Right Side - (Frame 15)
				drawObject(fence, row, col, FENCE_OFFSET_X, FENCE_OFFSET_Y,
					FENCE_FRAME15, spriteBatch);
				break;
			case TREE0:    // Large Green Tree - (Frame 0)
				drawObject(tree2, row, col, TREE2_OFFSET_X, TREE2_OFFSET_Y,
					TREE2_FRAME0, spriteBatch);
				break;
			case TREE1:    // Medium Green Tree - (Frame 1)
				drawObject(tree2, row, col, TREE2_OFFSET_X, TREE2_OFFSET_Y,
					TREE2_FRAME1, spriteBatch);
				break;
			case TREE2:    // Small Green Tree - (Frame 2)
				drawObject(tree2, row, col, TREE2_OFFSET_X, TREE2_OFFSET_Y,
					TREE2_FRAME2, spriteBatch);
				break;
			case TREE3:    // Extra Small Green Tree - (Frame 3)
				drawObject(tree2, row, col, TREE2_OFFSET_X, TREE2_OFFSET_Y,
					TREE2_FRAME3, spriteBatch);
				break;
			case TREE4:    // Tropical Green Tree - (Frame 4)
				drawObject(tree2, row, col, TREE2_OFFSET_X, TREE2_OFFSET_Y,
					TREE2_FRAME4, spriteBatch);
				break;
			case TREE5:    // Green Pine Tree - (Frame 5)
				drawObject(tree2, row, col, TREE2_OFFSET_X, TREE2_OFFSET_Y,
					TREE2_FRAME5, spriteBatch);
				break;
			case TREE6:    // Medium Green Bush Tree - (Frame 6)
				drawObject(tree2, row, col, TREE2_OFFSET_X, TREE2_OFFSET_Y,
					TREE2_FRAME6, spriteBatch);
				break;
			case WELL0:    // Well Left Side - (Frame 0)
				drawObject(well, row, col, WELL_OFFSET_X, WELL_OFFSET_Y,
					WELL_FRAME0, spriteBatch);
				break;
			case WELL1:    // Well Right Side - (Frame 1)
				drawObject(well, row, col, WELL_OFFSET_X, WELL_OFFSET_Y,
					WELL_FRAME1, spriteBatch);
				break;
			case HUT_ORIGIN:    // Hut Origin - (Frame 0)
				drawObject(hut, row, col, HUT_OFFSET_X, HUT_OFFSET_Y,
					HUT_FRAME0, spriteBatch);
				break;
			case TOWER0:    // Tower - (Frame 0)
				drawObject(tower, row, col, TOWER_OFFSET_X, TOWER_OFFSET_Y,
					TOWER_FRAME0, spriteBatch);
				break;
			case BUILDING_ORIGIN:    // Medieval Building - (Frame 0)
				drawObject(building, row, col, BUILDING_OFFSET_X, BUILDING_OFFSET_Y,
					BUILDING_FRAME0, spriteBatch);
				break;
			case BUILDING2_ORIGIN:    // Medieval Building2 - (Frame 0)
				drawObject(building2, row, col, BUILDING2_OFFSET_X, BUILDING2_OFFSET_Y,
					BUILDING2_FRAME0, spriteBatch);
				break;
			case BUILDING3_ORIGIN:    // Medieval Building3 - (Frame 0)
				drawObject(building3, row, col, BUILDING3_OFFSET_X, BUILDING3_OFFSET_Y,
					BUILDING3_FRAME0, spriteBatch);
				break;
			case WATCHTOWER0_ORIGIN:    // Watchtower0 Stone - (Frame 1)
				drawObject(watchTower, row, col, WATCHTOWER_OFFSET_X, WATCHTOWER_OFFSET_Y,
					WATCHTOWER_FRAME0, spriteBatch);
				break;
			case WATCHTOWER1_ORIGIN:    // Watchtower1 Wooden - (Frame 1)
				drawObject(watchTower, row, col, WATCHTOWER_OFFSET_X, WATCHTOWER_OFFSET_Y,
					WATCHTOWER_FRAME1, spriteBatch);
				break;
			case BUILDING4_ORIGIN:    // Medieval Building4 - (Frame 0)
				drawObject(building4, row, col, BUILDING4_OFFSET_X, BUILDING4_OFFSET_Y,
					BUILDING4_FRAME0, spriteBatch);
				break;
			case BUILDING5_ORIGIN:    // Medieval Building5 - (Frame 0)
				drawObject(building5, row, col, BUILDING5_OFFSET_X, BUILDING5_OFFSET_Y,
					BUILDING5_FRAME0, spriteBatch);
				break;
			case ROCK2:    // Rock - (Frame 2)
				drawObject(rock, row, col, ROCK_OFFSET_X, ROCK_OFFSET_Y,
					ROCK_FRAME2, spriteBatch);
				break;
			case NPC01:		// NPC01
				drawNPC(npc01, spriteBatch);
				break;
			case PLAYER:	// Player
				drawPlayer(player, spriteBatch);
				break;
			case BARRICADE0:    // Barricade Left, one-sided  - (Frame 0)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME0, spriteBatch);
				break;
			case BARRICADE1:    // Barricade Right, one-sided - (Frame 1)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME1, spriteBatch);
				break;
			case BARRICADE2:    // Barricade Left, one-sided  - (Frame 2)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME2, spriteBatch);
				break;
			case BARRICADE3:    // Barricade Right, one-sided - (Frame 3)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME3, spriteBatch);
				break;			
			case BARRICADE4:    // Barricade Right Corner - (Frame 4)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME3, spriteBatch);
				break;
			case BARRICADE5:    // Barricade Bottom Corner - (Frame 5)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME5, spriteBatch);
				break;
			case BARRICADE6:    // Barricade Left Corner - (Frame 6)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME6, spriteBatch);
				break;
			case BARRICADE7:    // Barricade Top Corner - (Frame 7)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME7, spriteBatch);
				break;
			case BARRICADE8:    // Barricade Right, two-sided - (Frame 8)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME8, spriteBatch);
				break;
			case BARRICADE9:    // Barricade Right, two-sided - (Frame 0)
				drawObject(barricade, row, col, BARRICADE_OFFSET_X, BARRICADE_OFFSET_Y,
					BARRICADE_FRAME9, spriteBatch);
				break;
			case POND_ORIGIN:    // Pond Origin - (Frame 0)
				drawObject(pond, row, col, POND_OFFSET_X, POND_OFFSET_Y,
					POND_FRAME0, spriteBatch);
				break;
			case BLACKSMITH_ORIGIN:    // Building6 Blacksmith - (Frame 0)
				drawObject(building6, row, col, BLACKSMITH_OFFSET_X, BLACKSMITH_OFFSET_Y,
					BLACKSMITH_FRAME0, spriteBatch);
				break;
			case BARRACKS_ORIGIN:    // Building6 Barracks - (Frame 1)
				drawObject(building6, row, col, BARRAKS_OFFSET_X, BARRAKS_OFFSET_Y,
					BARRACKS_FRAME1, spriteBatch);
				break;
			case ARCHERY_ORIGIN:    // Building6 Archery - (Frame 2)
				drawObject(building6, row, col, ARCHERY_OFFSET_X, ARCHER_OFFSET_Y,
					ARCHERY_FRAME2, spriteBatch);
				break;
			case STABLE_ORIGIN:    // Building6 Stable - (Frame 3)
				drawObject(building6, row, col, STABLE_OFFSET_X, STABLE_OFFSET_Y,
					STABLE_FRAME3, spriteBatch);
				break;
			case BROKEN_TOWER_ORIGIN:    // Building6 Broken Tower - (Frame 0)
				drawObject(brokenTower, row, col, BROKENTOWER_OFFSET_X, BROKENTOWER_OFFSET_Y,
					BROKENTOWER_FRAME0, spriteBatch);
				break;
			case TIMBERED_HOUSE0_ORIGIN:    // Timbered House Right Side - (Frame 0)
				drawObject(timberedHouse, row, col, TIMBERED_HOUSE_OFFSET_X, TIMBERED_HOUSE_OFFSET_Y,
					TIMBERED_HOUSE_FRAME0, spriteBatch);
				break;
			case TIMBERED_HOUSE1_ORIGIN:    // Timbered House Left Side - (Frame 1)
				drawObject(timberedHouse, row, col, TIMBERED_HOUSE_OFFSET_X, TIMBERED_HOUSE_OFFSET_Y,
					TIMBERED_HOUSE_FRAME1, spriteBatch);
				break;
			case NPC02:		// NPC02
				drawNPC(npc02, spriteBatch);
				break;
			case NPC03:		// NPC03
				drawNPC(npc03, spriteBatch);			
				break;
			case NPC04:		// NPC04
				drawNPC(npc04, spriteBatch);
				break;
			case NPC05:		// NPC05
				drawNPC(npc05, spriteBatch);
				break;
			case NPC06:		// NPC06
				drawNPC(npc06, spriteBatch);
				break;
			case NPC07:		// NPC07
				drawNPC(npc07, spriteBatch);
				break;
			case NPC08:		// NPC08
				drawNPC(npc08, spriteBatch);
				break;
			case NPC09:		// NPC09
				drawNPC(npc09, spriteBatch);
				break;
			case NPC10:		// NPC10
				drawNPC(npc10, spriteBatch);
				break;
			case NPC11:		// NPC11
				drawNPC(npc11, spriteBatch);
				break;
			case NPC12:		// NPC12
				drawNPC(npc12, spriteBatch);
				break;
			case NPC13:		// NPC13
				drawNPC(npc13, spriteBatch);
				break;
			case NPC14:		// NPC14
				drawNPC(npc14, spriteBatch);
				break;
			case NPC15:		// NPC15
				drawNPC(npc15, spriteBatch);
				break;
			case NPC16:		// NPC16
				drawNPC(npc16, spriteBatch);
				break;
			case NPC17:		// NPC17
				drawNPC(npc17, spriteBatch);
				break;
			case NPC18:		// NPC18
				drawNPC(npc18, spriteBatch);
				break;
			case NPC19:		// NPC19
				drawNPC(npc19, spriteBatch);
				break;
			case NPC20:		// NPC20
				drawNPC(npc20, spriteBatch);
				break;
			case CHEST_ORIGIN:	// Chest - (Frame 0)
				drawObject(chest, row, col, CHEST_OFFSET_X, CHEST_OFFSET_Y,
					CHEST_FRAME0, spriteBatch);
				break;
			case NPC21:		// NPC21
				drawNPC(npc21, spriteBatch);
				break;
			case NPC22:		// NPC22
				drawNPC(npc22, spriteBatch);
				break;
			case NPC23:		// NPC23
				drawNPC(npc23, spriteBatch);
				break;
			case NPC24:		// NPC24
				drawNPC(npc24, spriteBatch);
				break;
			case NPC25:		// NPC25
				drawNPC(npc25, spriteBatch);
				break;
			case NPC26:		// NPC16
				drawNPC(npc26, spriteBatch);
				break;
			case NPC27:		// NPC27
				drawNPC(npc27, spriteBatch);
				break;
			case NPC28:		// NPC28
				drawNPC(npc28, spriteBatch);
				break;
			case NPC29:		// NPC29
				drawNPC(npc29, spriteBatch);
				break;
			case NPC30:		// NPC30
				drawNPC(npc30, spriteBatch);
				break;
			case NPC31:		// NPC31
				drawNPC(npc31, spriteBatch);
				break;
			case NPC32:		// NPC32
				drawNPC(npc32, spriteBatch);
				break;
			case NPC33:		// NPC33
				drawNPC(npc33, spriteBatch);
				break;
			case NPC34:		// NPC34
				drawNPC(npc34, spriteBatch);
				break;
			case NPC35:		// NPC35
				drawNPC(npc35, spriteBatch);
				break;
			case NPC36:		// NPC36
				drawNPC(npc36, spriteBatch);
				break;
			case NPC37:		// NPC37
				drawNPC(npc37, spriteBatch);
				break;
			case NPC38:		// NPC38
				drawNPC(npc38, spriteBatch);
				break;
			case NPC39:		// NPC39
				drawNPC(npc39, spriteBatch);
				break;
			case NPC40:		// NPC40
				drawNPC(npc40, spriteBatch);
				break;
			case NPC41:		// NPC41
				drawNPC(npc41, spriteBatch);
				break;
			case NPC42:		// NPC42
				drawNPC(npc42, spriteBatch);
				break;
			case NPC43:		// NPC43
				drawNPC(npc43, spriteBatch);
				break;
			case NPC44:		// NPC44
				drawNPC(npc44, spriteBatch);
				break;
			case NPC45:		// NPC45
				drawNPC(npc45, spriteBatch);
				break;
			case NPC46:		// NPC46
				drawNPC(npc46, spriteBatch);
				break;
			case NPC47:		// NPC47
				drawNPC(npc47, spriteBatch);
				break;
			case NPC48:		// NPC48
				drawNPC(npc48, spriteBatch);
				break;
			case NPC49:		// NPC49
				drawNPC(npc49, spriteBatch);
				break;
			case NPC50:		// NPC50
				drawNPC(npc50, spriteBatch);
				break;		
			case NPC51:		// NPC51
				drawNPC(npc51, spriteBatch);
				break;
			case NPC52:		// NPC52
				drawNPC(npc52, spriteBatch);
				break;
			case NPC53:		// NPC53
				drawNPC(npc53, spriteBatch);
				break;
			case NPC54:		// NPC54
				drawNPC(npc54, spriteBatch);
				break;
			case NPC55:		// NPC55
				drawNPC(npc55, spriteBatch);
				break;
			case NPC56:		// NPC56
				drawNPC(npc56, spriteBatch);
				break;
			case NPC57:		// NPC57
				drawNPC(npc57, spriteBatch);
				break;
			case NPC58:		// NPC58
				drawNPC(npc58, spriteBatch);
				break;
			case NPC59:		// NPC59
				drawNPC(npc59, spriteBatch);
				break;
			case NPC60:		// NPC60
				drawNPC(npc60, spriteBatch);
				break;
			case NPC61:		// NPC61
				drawNPC(npc61, spriteBatch);
				break;
			case NPC62:		// NPC62
				drawNPC(npc62, spriteBatch);
				break;
			case NPC63:		// NPC63
				drawNPC(npc63, spriteBatch);
				break;
			case NPC64:		// NPC64
				drawNPC(npc64, spriteBatch);
				break;
			case NPC65:		// NPC15
				drawNPC(npc65, spriteBatch);
				break;
			case NPC66:		// NPC66
				drawNPC(npc66, spriteBatch);
				break;
			case NPC67:		// NPC67
				drawNPC(npc67, spriteBatch);
				break;
			case NPC68:		// NPC68
				drawNPC(npc68, spriteBatch);
				break;
			case NPC69:		// NPC69
				drawNPC(npc69, spriteBatch);
				break;
			case NPC70:		// NPC70
				drawNPC(npc70, spriteBatch);
				break;
			case NPC71:		// NPC71
				drawNPC(npc71, spriteBatch);
				break;
			case NPC72:		// NPC72
				drawNPC(npc72, spriteBatch);
				break;
			case NPC73:		// NPC73
				drawNPC(npc73, spriteBatch);
				break;
			case NPC74:		// NPC74
				drawNPC(npc74, spriteBatch);
				break;
			case NPC75:		// NPC75
				drawNPC(npc75, spriteBatch);
				break;
			case NPC76:		// NPC76
				drawNPC(npc76, spriteBatch);
				break;
			case NPC77:		// NPC77
				drawNPC(npc77, spriteBatch);
				break;
			case NPC78:		// NPC78
				drawNPC(npc78, spriteBatch);
				break;
			case NPC79:		// NPC79
				drawNPC(npc79, spriteBatch);
				break;
			case NPC80:		// NPC80
				drawNPC(npc80, spriteBatch);
				break;
			}
        }
    }

	// Delete SpriteBatch when finished rendering.
    delete spriteBatch;

    // Use a separate SpriteBatch so text will render on top of images.
    CanvasSpriteBatch^ spriteBatch2 = args->DrawingSession->CreateSpriteBatch();

    // Display menu.
	if (menuOn)
		menu.draw(spriteBatch2);

	// Display Timer
	if (timer < 10)		// Display 0:00, 0:01, etc.
	{
		textWin2D->print(
			args,									// Current display canvas
			// Convert dps to wstring and concatentate to UPS.
			Strings::TIMER + timerMin.ToString()->Data() +
			L":0" + timer.ToString()->Data(),
			int(XamlCanvas->Size.Width) - 250,			// X position
			40, 24, TEXT_COLORS::Black, FontStyle::Normal,FontWeights::Bold
		);
	}
	else                // Display 0:10, 0:11, etc.
	{
		textWin2D->print(
			args,									// Current display canvas
			// Convert dps to wstring and concatentate to UPS.
			Strings::TIMER + timerMin.ToString()->Data() +
			L":" + timer.ToString()->Data(),
			int(XamlCanvas->Size.Width) - 250,			// X position
			40, 24, TEXT_COLORS::Black, FontStyle::Normal, FontWeights::Bold
		);
	}

	// Display Player Health
	playerHealth = player.getHealth();
	textWin2D->print(args,
		Strings::PLAYER_HEALTH + playerHealth.ToString()->Data() + L"/100", 50, 40, 24,
		TEXT_COLORS::DarkBlue, FontStyle::Normal, FontWeights::Bold);

	// Display Player Lives
	playerLives = player.getLives();
	textWin2D->print(args,
		Strings::PLAYER_LIVES + playerLives.ToString()->Data() + L"/4", 50, 70, 24,
		TEXT_COLORS::DarkBlue, FontStyle::Normal, FontWeights::Bold);
		
	// If player has 0 health and 0 lives, they lose.
	if (playerHealth <= 0 && playerLives <= 0)
	{
		textWin2D->print(args, L"You lose!!",
			getWidth() / 2 - 150, getHeight() / 2 - 20, 50, TEXT_COLORS::Black,
			FontStyle::Oblique, FontWeights::Bold
		);
		textWin2D->print(args, L"Press Y to restart, Press N to exit.",
			getWidth() / 2 - 450, getHeight() / 2 + 25, 55, TEXT_COLORS::Black,
			FontStyle::Oblique, FontWeights::Bold
		);
		lose = true;
		input->clearAll();
		timerOn = false;
	}

    // If player steps on a Goal Tile, they win.
	if ((playerRow == GOAL1_ROW && playerColumn == GOAL1_COL) ||
		(playerRow == GOAL2_ROW && playerColumn == GOAL2_COL))
	{
		textWin2D->print(args, L"You Win!!",
			getWidth() / 2 - 150, getHeight() / 2 - 20, 50, TEXT_COLORS::Black,
			FontStyle::Oblique, FontWeights::Bold
		);
		textWin2D->print(args, L"Press Y to restart, Press N to exit.",
			getWidth() / 2 - 450, getHeight() / 2 + 25, 55, TEXT_COLORS::Black,
			FontStyle::Oblique, FontWeights::Bold
		);
		win = true;
		input->clearAll();
		timerOn = false;
		
	}
	
	// Debugging Region
	#pragma region DebugRegion
	// Displays debug data if true
	if (showDebug)
	{
		//=============================================================================
		// Define DEBUG in Game.h to enable display of debug data.
		//=============================================================================
#ifdef DEBUG
		//// Display mouse data for debug purposes.
		double num;
		bool status;

		debugString += L"Col = ";
		num = selectedCol;
		// Convert numbers to wstrings
		debugString += num.ToString()->Data();


		debugString += L"\nRow = ";
		num = selectedRow;
		debugString += num.ToString()->Data();

		num = pointerX;     // pointer X
		debugString += L"\nPointerX = ";
		debugString += num.ToString()->Data(),

		debugString += L"\nMouseX = ";
		num = mouseXMap;    // pointer X
		debugString += num.ToString()->Data(),

			debugString += L"\nPointerY = ";
		num = pointerY;     // pointer Y
		debugString += num.ToString()->Data(),

			debugString += L"\nMouseY = ";
		num = mouseYMap;     // pointer Y
		debugString += num.ToString()->Data(),
	
			debugString += L"\nPlayer Col = ";
		num = playerColumn;
		debugString += num.ToString()->Data(),

			debugString += L"\nPlayer Row = ";
		num = playerRow;
		debugString += num.ToString()->Data(),

			debugString += L"\nPlayerX = ";
		num = player.getX();     // player X
		debugString += num.ToString()->Data(),

			debugString += L"\nPlayerY = ";
		num = player.getY();     // player Y
		debugString += num.ToString()->Data(),

			debugString += L"\nZoom = ";
		num = mapZoom;
		debugString += num.ToString()->Data(),

			debugString += L"\nPlayer Attack = ";
		status = player.getAttack();
		debugString += status.ToString()->Data(),

			debugString += L"\nPlayer Death = ";
		status = player.getDeath();
		debugString += status.ToString()->Data(),

			debugString += L"\nPlayer Found = ";
		status = playerFound;
		debugString += status.ToString()->Data(),

			debugString += L"\nnpc01 Health = ";
		num = npc01.getHealth();
		debugString += num.ToString()->Data(),

			debugString += L"\nnpc01 Attack = ";
		status = npc01.getAttack();
		debugString += status.ToString()->Data(),

			debugString += L"\nnpc01 Death = ";
		status = npc01.getDeath();
		debugString += status.ToString()->Data(),

			textWin2D->print(
				args,
				debugString,
				10,	    // X position
				100,     // Y position
				12,     // size
				TEXT_COLORS::Black,
				FontStyle::Normal,
				FontWeights::Bold
			);
		debugString.clear();
#endif  // DEBUG
		//=============================================================================
	}
#pragma endregion

    // Display Easter Egg
	if (input->getTextIn() == Strings::LLAP)
        textWin2D->print(args, Strings::LLAPS, 400, 40, 24,
            TEXT_COLORS::SkyBlue, FontStyle::Italic);

    // Delete SpriteBatch when finished rendering.
    delete spriteBatch2;
}

//=============================================================================
// Update the game items.
//=============================================================================
void GameEngine2::Game::update(Xaml::CanvasAnimatedUpdateEventArgs^ args)
{
    updateCount++;

    // Calculate deltaTime in seconds.
    deltaTime = args->Timing.ElapsedTime.Duration / 10000000.0f;
    // Calcualte total running time in seconds.
    runningTime = static_cast<double>(args->Timing.TotalTime.Duration / 10000000.0f);

    // Use a Moving Average to calculate average Updates Per Second (ups).
    if (deltaTime > 0.0)
        ups = (ups*0.99f) + (0.01f / deltaTime);  // Average ups

    if (instructionTimer > 0)
        instructionTimer -= deltaTime;      // Instruction display.
	if (input->wasKeyPressed(VirtualKey::Help) || input->wasKeyPressed(VirtualKey::F1))
		instructionTimer = INSTRUCTION_DISPLAY_TIME;

	// Wait for key pressed to start game
	if (menuOn)
	{
		if (input->anyKeyPressed())
		{
			menuOn = false;
			timerOn = true;
			input->clearAll();
		}
	}

	// Count elapsed time since game start
	if (timerOn)
	{
		timer += deltaTime;
		if (timer >= 60)
		{
			timer = 0;
			timerMin++;
			// Regenerate 50pts health every minute
			int h = player.getHealth();
			h += 50;
			if (h >= 100)
				h = 100;
			player.setHealth(h);
		}		
	}

	// Count down respawn of player when dead
	// Position player back to start on death
	if (respawnTimerOn)
	{
		respawnTimer -= deltaTime;
		if (respawnTimer <= 0 && (!win && !lose))
		{
			respawnTimerOn = false;
			player.respawn();
			// position player back at start after respawn
			positionPlayer(player, PLAYER_START_ROW, PLAYER_START_COL,
				PLAYER_OFFSET_X, PLAYER_OFFSET_Y);
		}
	}

	// If player won
	if (win)
	{
		// If user doesn't want to play again
		if (input->getTextIn() == "N" || input->getTextIn() == "n") {
			exit(1);
		}
		else if (input->getTextIn() == "Y" || input->getTextIn() == "y") {

		}
	}
	// If player lost
	if (lose)
	{
		// If user doesn't want to play again
		if (input->getTextIn() == "N" || input->getTextIn() == "n") {
			exit(1);
		}
		else if (input->getTextIn() == "Y" || input->getTextIn() == "y") {

		}
	}

	// Enable/Disable Debug Info when pressed
	if (input->wasKeyPressed(VirtualKey::F2))
	{
		if (showDebug)
			showDebug = false;
		else
			showDebug = true;
	}

	// Exit game on ESC key press.
	if (input->wasKeyPressed(VirtualKey::Escape))
	{
		exit(1);
	}

	// Save input from connected game controllers.
	input->readControllers();

	// Player Movement
	// W,A,S,D is Horizontal/Vertical movement
	// Q,E,Z,C is Diagonal movement
	// Gamepad Left Thumbstick controls movement.
	// Gamepad Right Trigger controls players speed.
	player.setLoop(false);	
	if (win == false && lose == false)
	{
		if (input->isKeyDown(VirtualKey::W) ||
			(input->getGamepadThumbLY(0) > 0 && input->getGamepadThumbLX(0) == 0))
			player.setDirection(N);
		else if (input->isKeyDown(VirtualKey::E) ||
			(input->getGamepadThumbLY(0) > 0 && input->getGamepadThumbLX(0) > 0))
			player.setDirection(NE);
		else if (input->isKeyDown(VirtualKey::D) ||
			(input->getGamepadThumbLY(0) == 0 && input->getGamepadThumbLX(0) > 0))
			player.setDirection(E);
		else if (input->isKeyDown(VirtualKey::C) ||
			(input->getGamepadThumbLY(0) < 0 && input->getGamepadThumbLX(0) > 0))
			player.setDirection(SE);
		else if (input->isKeyDown(VirtualKey::S) ||
			(input->getGamepadThumbLY(0) < 0 && input->getGamepadThumbLX(0) == 0))
			player.setDirection(S);
		else if (input->isKeyDown(VirtualKey::Z) ||
			(input->getGamepadThumbLY(0) < 0 && input->getGamepadThumbLX(0) < 0))
			player.setDirection(SW);
		else if (input->isKeyDown(VirtualKey::A) ||
			(input->getGamepadThumbLY(0) == 0 && input->getGamepadThumbLX(0) < 0))
			player.setDirection(W);
		else if (input->isKeyDown(VirtualKey::Q) ||
			(input->getGamepadThumbLY(0) > 0 && input->getGamepadThumbLX(0) < 0))
			player.setDirection(NW);
		if (input->getGamepadRightTrigger(0) > 0)
			player.setSpeedRun();
		else
			player.setSpeedWalk();
	}

	// Player Attack, allow if no win or lose
	if ((input->isKeyDown(VirtualKey::F) || (input->getGamepadX(0))) && (!win && !lose))
	{
		audio->PlaySound(8);	// Play sword swoosh sound
		player.setAttack(true);
		// Check tiles around the player for any NPC's
		checkPlayerRange(player, playerRow, playerColumn, NPC01, npc01);
		checkPlayerRange(player, playerRow, playerColumn, NPC02, npc02);
		checkPlayerRange(player, playerRow, playerColumn, NPC03, npc03);
		checkPlayerRange(player, playerRow, playerColumn, NPC04, npc04);
		checkPlayerRange(player, playerRow, playerColumn, NPC05, npc05);
		checkPlayerRange(player, playerRow, playerColumn, NPC06, npc06);
		checkPlayerRange(player, playerRow, playerColumn, NPC07, npc07);
		checkPlayerRange(player, playerRow, playerColumn, NPC08, npc08);
		checkPlayerRange(player, playerRow, playerColumn, NPC09, npc09);
		checkPlayerRange(player, playerRow, playerColumn, NPC10, npc10);
		checkPlayerRange(player, playerRow, playerColumn, NPC11, npc11);
		checkPlayerRange(player, playerRow, playerColumn, NPC12, npc12);
		checkPlayerRange(player, playerRow, playerColumn, NPC13, npc13);
		checkPlayerRange(player, playerRow, playerColumn, NPC14, npc14);
		checkPlayerRange(player, playerRow, playerColumn, NPC15, npc15);
		checkPlayerRange(player, playerRow, playerColumn, NPC16, npc16);
		checkPlayerRange(player, playerRow, playerColumn, NPC17, npc17);
		checkPlayerRange(player, playerRow, playerColumn, NPC18, npc18);
		checkPlayerRange(player, playerRow, playerColumn, NPC19, npc19);
		checkPlayerRange(player, playerRow, playerColumn, NPC20, npc20);
		checkPlayerRange(player, playerRow, playerColumn, NPC21, npc21);
		checkPlayerRange(player, playerRow, playerColumn, NPC22, npc22);
		checkPlayerRange(player, playerRow, playerColumn, NPC23, npc23);
		checkPlayerRange(player, playerRow, playerColumn, NPC24, npc24);
		checkPlayerRange(player, playerRow, playerColumn, NPC25, npc25);
		checkPlayerRange(player, playerRow, playerColumn, NPC26, npc26);
		checkPlayerRange(player, playerRow, playerColumn, NPC27, npc27);
		checkPlayerRange(player, playerRow, playerColumn, NPC28, npc28);
		checkPlayerRange(player, playerRow, playerColumn, NPC29, npc29);
		checkPlayerRange(player, playerRow, playerColumn, NPC30, npc30);
		checkPlayerRange(player, playerRow, playerColumn, NPC31, npc31);
		checkPlayerRange(player, playerRow, playerColumn, NPC32, npc32);
		checkPlayerRange(player, playerRow, playerColumn, NPC33, npc33);
		checkPlayerRange(player, playerRow, playerColumn, NPC34, npc34);
		checkPlayerRange(player, playerRow, playerColumn, NPC35, npc35);
		checkPlayerRange(player, playerRow, playerColumn, NPC36, npc36);
		checkPlayerRange(player, playerRow, playerColumn, NPC37, npc37);
		checkPlayerRange(player, playerRow, playerColumn, NPC38, npc38);
		checkPlayerRange(player, playerRow, playerColumn, NPC39, npc39);
		checkPlayerRange(player, playerRow, playerColumn, NPC40, npc40);
		checkPlayerRange(player, playerRow, playerColumn, NPC41, npc41);
		checkPlayerRange(player, playerRow, playerColumn, NPC42, npc42);
		checkPlayerRange(player, playerRow, playerColumn, NPC43, npc43);
		checkPlayerRange(player, playerRow, playerColumn, NPC44, npc44);
		checkPlayerRange(player, playerRow, playerColumn, NPC45, npc45);
		checkPlayerRange(player, playerRow, playerColumn, NPC46, npc46);
		checkPlayerRange(player, playerRow, playerColumn, NPC47, npc47);
		checkPlayerRange(player, playerRow, playerColumn, NPC48, npc48);
		checkPlayerRange(player, playerRow, playerColumn, NPC49, npc49);
		checkPlayerRange(player, playerRow, playerColumn, NPC50, npc50);
		checkPlayerRange(player, playerRow, playerColumn, NPC51, npc51);
		checkPlayerRange(player, playerRow, playerColumn, NPC52, npc52);
		checkPlayerRange(player, playerRow, playerColumn, NPC53, npc53);
		checkPlayerRange(player, playerRow, playerColumn, NPC54, npc54);
		checkPlayerRange(player, playerRow, playerColumn, NPC55, npc55);
		checkPlayerRange(player, playerRow, playerColumn, NPC56, npc56);
		checkPlayerRange(player, playerRow, playerColumn, NPC57, npc57);
		checkPlayerRange(player, playerRow, playerColumn, NPC58, npc58);
		checkPlayerRange(player, playerRow, playerColumn, NPC59, npc59);
		checkPlayerRange(player, playerRow, playerColumn, NPC60, npc60);
		checkPlayerRange(player, playerRow, playerColumn, NPC61, npc61);
		checkPlayerRange(player, playerRow, playerColumn, NPC62, npc62);
		checkPlayerRange(player, playerRow, playerColumn, NPC63, npc63);
		checkPlayerRange(player, playerRow, playerColumn, NPC64, npc64);
		checkPlayerRange(player, playerRow, playerColumn, NPC65, npc65);
		checkPlayerRange(player, playerRow, playerColumn, NPC66, npc66);
		checkPlayerRange(player, playerRow, playerColumn, NPC67, npc67);
		checkPlayerRange(player, playerRow, playerColumn, NPC68, npc68);
		checkPlayerRange(player, playerRow, playerColumn, NPC69, npc69);
		checkPlayerRange(player, playerRow, playerColumn, NPC70, npc70);
		checkPlayerRange(player, playerRow, playerColumn, NPC71, npc71);
		checkPlayerRange(player, playerRow, playerColumn, NPC72, npc72);
		checkPlayerRange(player, playerRow, playerColumn, NPC73, npc73);
		checkPlayerRange(player, playerRow, playerColumn, NPC74, npc74);
		checkPlayerRange(player, playerRow, playerColumn, NPC75, npc75);
		checkPlayerRange(player, playerRow, playerColumn, NPC76, npc76);
		checkPlayerRange(player, playerRow, playerColumn, NPC77, npc77);
		checkPlayerRange(player, playerRow, playerColumn, NPC78, npc78);
		checkPlayerRange(player, playerRow, playerColumn, NPC79, npc79);
		checkPlayerRange(player, playerRow, playerColumn, NPC80, npc80);
	}
	else
		player.setAttack(false);

	npc01.setLoop(false);
	npc02.setLoop(false);
	npc03.setLoop(false);
	npc04.setLoop(false);
	npc05.setLoop(false);
	npc06.setLoop(false);
	npc07.setLoop(false);
	npc08.setLoop(false);
	npc09.setLoop(false);
	npc10.setLoop(false);
	npc11.setLoop(false);
	npc12.setLoop(false);
	npc13.setLoop(false);
	npc14.setLoop(false);
	npc15.setLoop(false);
	npc16.setLoop(false);
	npc17.setLoop(false);
	npc18.setLoop(false);
	npc19.setLoop(false);
	npc20.setLoop(false);
	npc21.setLoop(false);
	npc22.setLoop(false);
	npc23.setLoop(false);
	npc24.setLoop(false);
	npc25.setLoop(false);
	npc26.setLoop(false);
	npc27.setLoop(false);
	npc28.setLoop(false);
	npc29.setLoop(false);
	npc30.setLoop(false);
	npc31.setLoop(false);
	npc32.setLoop(false);
	npc33.setLoop(false);
	npc34.setLoop(false);
	npc35.setLoop(false);
	npc36.setLoop(false);
	npc37.setLoop(false);
	npc38.setLoop(false);
	npc39.setLoop(false);
	npc40.setLoop(false);
	npc41.setLoop(false);
	npc42.setLoop(false);
	npc43.setLoop(false);
	npc44.setLoop(false);
	npc45.setLoop(false);
	npc46.setLoop(false);
	npc47.setLoop(false);
	npc48.setLoop(false);
	npc49.setLoop(false);
	npc50.setLoop(false);
	npc51.setLoop(false);
	npc52.setLoop(false);
	npc53.setLoop(false);
	npc54.setLoop(false);
	npc55.setLoop(false);
	npc56.setLoop(false);
	npc57.setLoop(false);
	npc58.setLoop(false);
	npc59.setLoop(false);
	npc60.setLoop(false);
	npc61.setLoop(false);
	npc62.setLoop(false);
	npc63.setLoop(false);
	npc64.setLoop(false);
	npc65.setLoop(false);
	npc66.setLoop(false);
	npc67.setLoop(false);
	npc68.setLoop(false);
	npc69.setLoop(false);
	npc70.setLoop(false);
	npc71.setLoop(false);
	npc72.setLoop(false);
	npc73.setLoop(false);
	npc74.setLoop(false);
	npc75.setLoop(false);
	npc76.setLoop(false);
	npc77.setLoop(false);
	npc78.setLoop(false);
	npc79.setLoop(false);
	npc80.setLoop(false);

	// If player is not dead, check tiles around NPCs for the player to attack
	if (!(player.getDeath()))
	{
		// Check tiles around NPCs for player
		// Attack player when found
		findPlayer(npc01, npcRow, npcCol);
		findPlayer(npc02, npc2Row, npc2Col);
		findPlayer(npc03, npc3Row, npc3Col);
		findPlayer(npc04, npc4Row, npc4Col);
		findPlayer(npc05, npc5Row, npc5Col);
		findPlayer(npc06, npc6Row, npc6Col);
		findPlayer(npc07, npc7Row, npc7Col);
		findPlayer(npc08, npc8Row, npc8Col);
		findPlayer(npc09, npc9Row, npc9Col);
		findPlayer(npc10, npc10Row, npc10Col);
		findPlayer(npc11, npc11Row, npc11Col);
		findPlayer(npc12, npc12Row, npc12Col);
		findPlayer(npc13, npc13Row, npc13Col);
		findPlayer(npc14, npc14Row, npc14Col);
		findPlayer(npc15, npc15Row, npc15Col);
		findPlayer(npc16, npc16Row, npc16Col);
		findPlayer(npc17, npc17Row, npc17Col);
		findPlayer(npc18, npc18Row, npc18Col);
		findPlayer(npc19, npc19Row, npc19Col);
		findPlayer(npc20, npc20Row, npc20Col);
		findPlayer(npc21, npc21Row, npc21Col);
		findPlayer(npc22, npc22Row, npc22Col);
		findPlayer(npc23, npc23Row, npc23Col);
		findPlayer(npc24, npc24Row, npc24Col);
		findPlayer(npc25, npc25Row, npc25Col);
		findPlayer(npc26, npc26Row, npc26Col);
		findPlayer(npc27, npc27Row, npc27Col);
		findPlayer(npc28, npc28Row, npc28Col);
		findPlayer(npc29, npc29Row, npc29Col);
		findPlayer(npc30, npc30Row, npc30Col);
		findPlayer(npc31, npc31Row, npc31Col);
		findPlayer(npc32, npc32Row, npc32Col);
		findPlayer(npc33, npc33Row, npc33Col);
		findPlayer(npc34, npc34Row, npc34Col);
		findPlayer(npc35, npc35Row, npc35Col);
		findPlayer(npc36, npc36Row, npc36Col);
		findPlayer(npc37, npc37Row, npc37Col);
		findPlayer(npc38, npc38Row, npc38Col);
		findPlayer(npc39, npc39Row, npc39Col);
		findPlayer(npc40, npc40Row, npc40Col);
		findPlayer(npc41, npc41Row, npc41Col);
		findPlayer(npc42, npc42Row, npc42Col);
		findPlayer(npc43, npc43Row, npc43Col);
		findPlayer(npc44, npc44Row, npc44Col);
		findPlayer(npc45, npc45Row, npc45Col);
		findPlayer(npc46, npc46Row, npc46Col);
		findPlayer(npc47, npc47Row, npc47Col);
		findPlayer(npc48, npc48Row, npc48Col);
		findPlayer(npc49, npc49Row, npc49Col);
		findPlayer(npc50, npc50Row, npc50Col);
		findPlayer(npc51, npc51Row, npc51Col);
		findPlayer(npc52, npc52Row, npc52Col);
		findPlayer(npc53, npc53Row, npc53Col);
		findPlayer(npc54, npc54Row, npc54Col);
		findPlayer(npc55, npc55Row, npc55Col);
		findPlayer(npc56, npc56Row, npc56Col);
		findPlayer(npc57, npc57Row, npc57Col);
		findPlayer(npc58, npc58Row, npc58Col);
		findPlayer(npc59, npc59Row, npc59Col);
		findPlayer(npc60, npc60Row, npc60Col);
		findPlayer(npc61, npc61Row, npc61Col);
		findPlayer(npc62, npc62Row, npc62Col);
		findPlayer(npc63, npc63Row, npc63Col);
		findPlayer(npc64, npc64Row, npc64Col);
		findPlayer(npc65, npc65Row, npc65Col);
		findPlayer(npc66, npc66Row, npc66Col);
		findPlayer(npc67, npc67Row, npc67Col);
		findPlayer(npc68, npc68Row, npc68Col);
		findPlayer(npc69, npc69Row, npc69Col);
		findPlayer(npc70, npc70Row, npc70Col);
		findPlayer(npc71, npc71Row, npc71Col);
		findPlayer(npc72, npc72Row, npc72Col);
		findPlayer(npc73, npc73Row, npc73Col);
		findPlayer(npc74, npc74Row, npc74Col);
		findPlayer(npc75, npc75Row, npc75Col);
		findPlayer(npc76, npc76Row, npc76Col);
		findPlayer(npc77, npc77Row, npc77Col);
		findPlayer(npc78, npc78Row, npc78Col);
		findPlayer(npc79, npc79Row, npc79Col);
		findPlayer(npc80, npc80Row, npc80Col);
	}

	// Update player/npc's while there's no win/lose
	if (!win && !lose)
	{
		player.update(deltaTime);
		npc01.update(deltaTime);
		npc02.update(deltaTime);
		npc03.update(deltaTime);
		npc04.update(deltaTime);
		npc05.update(deltaTime);
		npc06.update(deltaTime);
		npc07.update(deltaTime);
		npc08.update(deltaTime);
		npc09.update(deltaTime);
		npc10.update(deltaTime);
		npc11.update(deltaTime);
		npc12.update(deltaTime);
		npc13.update(deltaTime);
		npc14.update(deltaTime);
		npc15.update(deltaTime);
		npc16.update(deltaTime);
		npc17.update(deltaTime);
		npc18.update(deltaTime);
		npc19.update(deltaTime);
		npc20.update(deltaTime);
		npc21.update(deltaTime);
		npc22.update(deltaTime);
		npc23.update(deltaTime);
		npc24.update(deltaTime);
		npc25.update(deltaTime);
		npc26.update(deltaTime);
		npc27.update(deltaTime);
		npc28.update(deltaTime);
		npc29.update(deltaTime);
		npc30.update(deltaTime);
		npc31.update(deltaTime);
		npc32.update(deltaTime);
		npc33.update(deltaTime);
		npc34.update(deltaTime);
		npc35.update(deltaTime);
		npc36.update(deltaTime);
		npc37.update(deltaTime);
		npc38.update(deltaTime);
		npc39.update(deltaTime);
		npc40.update(deltaTime);
		npc41.update(deltaTime);
		npc42.update(deltaTime);
		npc43.update(deltaTime);
		npc44.update(deltaTime);
		npc45.update(deltaTime);
		npc46.update(deltaTime);
		npc47.update(deltaTime);
		npc48.update(deltaTime);
		npc49.update(deltaTime);
		npc50.update(deltaTime);
		npc51.update(deltaTime);
		npc52.update(deltaTime);
		npc53.update(deltaTime);
		npc54.update(deltaTime);
		npc55.update(deltaTime);
		npc56.update(deltaTime);
		npc57.update(deltaTime);
		npc58.update(deltaTime);
		npc59.update(deltaTime);
		npc60.update(deltaTime);
		npc61.update(deltaTime);
		npc62.update(deltaTime);
		npc63.update(deltaTime);
		npc64.update(deltaTime);
		npc65.update(deltaTime);
		npc66.update(deltaTime);
		npc67.update(deltaTime);
		npc68.update(deltaTime);
		npc69.update(deltaTime);
		npc70.update(deltaTime);
		npc71.update(deltaTime);
		npc72.update(deltaTime);
		npc73.update(deltaTime);
		npc74.update(deltaTime);
		npc75.update(deltaTime);
		npc76.update(deltaTime);
		npc77.update(deltaTime);
		npc78.update(deltaTime);
		npc79.update(deltaTime);
		npc80.update(deltaTime);
	}
	
	// Set respawn timer when dead
		if (player.getHealth() <= 0 && !lose)
		{
			player.setHealth(100);
			respawnTimerOn = true;
			respawnTimer = 0.95;
		}

	// Update player location in objectMap
	updatePlayerLocation(player, playerColumn, playerRow, PLAYER);	

	// Update npc locations in objectMap
	updateNpcLocation(npc01, npcCol, npcRow, NPC01);
	updateNpcLocation(npc02, npc2Col, npc2Row, NPC02);
	updateNpcLocation(npc03, npc3Col, npc3Row, NPC03);
	updateNpcLocation(npc04, npc4Col, npc4Row, NPC04);
	updateNpcLocation(npc05, npc5Col, npc5Row, NPC05);
	updateNpcLocation(npc06, npc6Col, npc6Row, NPC06);
	updateNpcLocation(npc07, npc7Col, npc7Row, NPC07);
	updateNpcLocation(npc08, npc8Col, npc8Row, NPC08);
	updateNpcLocation(npc09, npc9Col, npc9Row, NPC09);
	updateNpcLocation(npc10, npc10Col, npc10Row, NPC10);
	updateNpcLocation(npc11, npc11Col, npc11Row, NPC11);
	updateNpcLocation(npc12, npc12Col, npc12Row, NPC12);
	updateNpcLocation(npc13, npc13Col, npc13Row, NPC13);
	updateNpcLocation(npc14, npc14Col, npc14Row, NPC14);
	updateNpcLocation(npc15, npc15Col, npc15Row, NPC15);
	updateNpcLocation(npc16, npc16Col, npc16Row, NPC16);
	updateNpcLocation(npc17, npc17Col, npc17Row, NPC17);
	updateNpcLocation(npc18, npc18Col, npc18Row, NPC18);
	updateNpcLocation(npc19, npc19Col, npc19Row, NPC19);
	updateNpcLocation(npc20, npc20Col, npc20Row, NPC20);
	updateNpcLocation(npc21, npc21Col, npc21Row, NPC21);
	updateNpcLocation(npc22, npc22Col, npc22Row, NPC22);
	updateNpcLocation(npc23, npc23Col, npc23Row, NPC23);
	updateNpcLocation(npc24, npc24Col, npc24Row, NPC24);
	updateNpcLocation(npc25, npc25Col, npc25Row, NPC25);
	updateNpcLocation(npc26, npc26Col, npc26Row, NPC26);
	updateNpcLocation(npc27, npc27Col, npc27Row, NPC27);
	updateNpcLocation(npc28, npc28Col, npc28Row, NPC28);
	updateNpcLocation(npc29, npc29Col, npc29Row, NPC29);
	updateNpcLocation(npc30, npc30Col, npc30Row, NPC30);
	updateNpcLocation(npc31, npc31Col, npc31Row, NPC31);
	updateNpcLocation(npc32, npc32Col, npc32Row, NPC32);
	updateNpcLocation(npc33, npc33Col, npc33Row, NPC33);
	updateNpcLocation(npc34, npc34Col, npc34Row, NPC34);
	updateNpcLocation(npc35, npc35Col, npc35Row, NPC35);
	updateNpcLocation(npc36, npc36Col, npc36Row, NPC36);
	updateNpcLocation(npc37, npc37Col, npc37Row, NPC37);
	updateNpcLocation(npc38, npc38Col, npc38Row, NPC38);
	updateNpcLocation(npc39, npc39Col, npc39Row, NPC39);
	updateNpcLocation(npc40, npc40Col, npc40Row, NPC40);
	updateNpcLocation(npc41, npc41Col, npc41Row, NPC41);
	updateNpcLocation(npc42, npc42Col, npc42Row, NPC42);
	updateNpcLocation(npc43, npc43Col, npc43Row, NPC43);
	updateNpcLocation(npc44, npc44Col, npc44Row, NPC44);
	updateNpcLocation(npc45, npc45Col, npc45Row, NPC45);
	updateNpcLocation(npc46, npc46Col, npc46Row, NPC46);
	updateNpcLocation(npc47, npc47Col, npc47Row, NPC47);
	updateNpcLocation(npc48, npc48Col, npc48Row, NPC48);
	updateNpcLocation(npc49, npc49Col, npc49Row, NPC49);
	updateNpcLocation(npc50, npc50Col, npc50Row, NPC50);
	updateNpcLocation(npc51, npc51Col, npc51Row, NPC51);
	updateNpcLocation(npc52, npc52Col, npc52Row, NPC52);
	updateNpcLocation(npc53, npc53Col, npc53Row, NPC53);
	updateNpcLocation(npc54, npc54Col, npc54Row, NPC54);
	updateNpcLocation(npc55, npc55Col, npc55Row, NPC55);
	updateNpcLocation(npc56, npc56Col, npc56Row, NPC56);
	updateNpcLocation(npc57, npc57Col, npc57Row, NPC57);
	updateNpcLocation(npc58, npc58Col, npc58Row, NPC58);
	updateNpcLocation(npc59, npc59Col, npc59Row, NPC59);
	updateNpcLocation(npc60, npc60Col, npc60Row, NPC60);
	updateNpcLocation(npc61, npc61Col, npc61Row, NPC61);
	updateNpcLocation(npc62, npc62Col, npc62Row, NPC62);
	updateNpcLocation(npc63, npc63Col, npc63Row, NPC63);
	updateNpcLocation(npc64, npc64Col, npc64Row, NPC64);
	updateNpcLocation(npc65, npc65Col, npc65Row, NPC65);
	updateNpcLocation(npc66, npc66Col, npc66Row, NPC66);
	updateNpcLocation(npc67, npc67Col, npc67Row, NPC67);
	updateNpcLocation(npc68, npc68Col, npc68Row, NPC68);
	updateNpcLocation(npc69, npc69Col, npc69Row, NPC69);
	updateNpcLocation(npc70, npc70Col, npc70Row, NPC70);
	updateNpcLocation(npc71, npc71Col, npc71Row, NPC71);
	updateNpcLocation(npc72, npc72Col, npc72Row, NPC72);
	updateNpcLocation(npc73, npc73Col, npc73Row, NPC73);
	updateNpcLocation(npc74, npc74Col, npc74Row, NPC74);
	updateNpcLocation(npc75, npc75Col, npc75Row, NPC75);
	updateNpcLocation(npc76, npc76Col, npc76Row, NPC76);
	updateNpcLocation(npc77, npc77Col, npc77Row, NPC77);
	updateNpcLocation(npc78, npc78Col, npc78Row, NPC78);
	updateNpcLocation(npc79, npc79Col, npc79Row, NPC79);
	updateNpcLocation(npc80, npc80Col, npc80Row, NPC80);
	mapTile.update(deltaTime);
    collisions();

	// Resume/Pause Sound
    if (input->wasKeyPressed(VirtualKey::M))
    {
        if (audio->IsSoundPlaying(MUSIC2) == true)
            audio->PauseSound(MUSIC2);
        else
            audio->ResumeSound(MUSIC2);
    }
	
	// Increse Volume
    if (input->wasKeyPressed(VirtualKey::PageUp))
    {
        if (audio->IsSoundPlaying(MUSIC2) == true)
        {
            float volume = audio->GetVolume(MUSIC2);
            audio->SetVolume(MUSIC2, volume*2.0f);
        }
    }

	// Decrease Volume
    if (input->wasKeyPressed(VirtualKey::PageDown))
    {
        if (audio->IsSoundPlaying(MUSIC2) == true)
        {
            float volume = audio->GetVolume(MUSIC2);
            audio->SetVolume(MUSIC2, volume*0.5f);
        }
    }

	// If no win or lose, then allow scrolling of map
	if (!win && !lose)
	{
		// Map Scroll with W,A,S,D keys or Gamepad Right Thumbstick
		// Shift for fast scroll
		float scrollSpeed;
		if (input->isKeyDown(VirtualKey::Shift))    // if Shift
			scrollSpeed = SCROLL_SPEED * 4;          // Fast scroll speed.
		else
			scrollSpeed = SCROLL_SPEED;
		if (input->isKeyDown(VirtualKey::J))       // if 'A' key
			scrollX += scrollSpeed * deltaTime;     // Scroll map right.
		if (input->isKeyDown(VirtualKey::L))       // if 'D' key
			scrollX -= scrollSpeed * deltaTime;     // Scroll map left.
		if (input->isKeyDown(VirtualKey::I))       // if 'W' key
			scrollY += scrollSpeed * deltaTime;     // Scroll map down.
		if (input->isKeyDown(VirtualKey::K))       // if 'S' key
			scrollY -= scrollSpeed * deltaTime;     // Scroll map up.

		// Gamepad Left Trigger for fast scroll
		if (input->getGamepadLeftTrigger(0) > 0)   // if Left Trigger down
		{
			scrollSpeed = SCROLL_SPEED * 4;			// Fast scroll speed.
			input->gamePadVibrateLeftTrigger(0, 0.1, 0.1);
		}
		else
			scrollSpeed = SCROLL_SPEED;
		if ((input->getGamepadThumbRY(0) == 0 && input->getGamepadThumbRX(0) < 0))    // if Thumbstick Right is right.
			scrollX += scrollSpeed * deltaTime;     // Scroll map right.
		if ((input->getGamepadThumbRY(0) == 0 && input->getGamepadThumbRX(0) > 0))    // if Thumbstick Right is left.
			scrollX -= scrollSpeed * deltaTime;     // Scroll map left.
		if ((input->getGamepadThumbRY(0) > 0 && input->getGamepadThumbRX(0) == 0))    // if Thumbstick Right is up.
			scrollY += scrollSpeed * deltaTime;     // Scroll map down.
		if ((input->getGamepadThumbRY(0) < 0 && input->getGamepadThumbRX(0) == 0))    // if Thumbstick Right is down.
			scrollY -= scrollSpeed * deltaTime;     // Scroll map up.
		if (input->getGamepadThumbRY(0) < 0 && input->getGamepadThumbRX(0) < 0)		  // if Thumbstick Right is in SW corner.
		{
			scrollX += scrollSpeed * deltaTime;		// Scroll map SW.
			scrollY -= scrollSpeed * deltaTime;
		}
		if (input->getGamepadThumbRY(0) > 0 && input->getGamepadThumbRX(0) < 0)		  // if Thumbstick Right is in NW corner.
		{
			scrollX += scrollSpeed * deltaTime;		// Scroll map NW.
			scrollY += scrollSpeed * deltaTime;
		}
		if ((input->getGamepadThumbRY(0) < 0 && input->getGamepadThumbRX(0) > 0))	  // if Thumbstick Right is in SE corner.
		{
			scrollX -= scrollSpeed * deltaTime;		// Scroll map SE.
			scrollY -= scrollSpeed * deltaTime;
		}
		if (input->getGamepadThumbRY(0) > 0 && input->getGamepadThumbRX(0) > 0)		  // if Thumbstick Right is in NE corner.
		{
			scrollX -= scrollSpeed * deltaTime;		// Scroll map NE.
			scrollY += scrollSpeed * deltaTime;
		}
	}

	// Calculate location of pointer relative to isometric grid.
	// This treats the top corner of the board as location 0,0.
	// Pointer position relative to map
	pointerX = input->getPointerX();
	mouseXMap = (pointerX - SCREEN_X) / mapZoom - scrollX;
	pointerY = input->getPointerY();
	mouseYMap = (pointerY - SCREEN_Y) / mapZoom - scrollY;

	// Calculate selected column
	selectedCol = getSelectedColumn(mouseXMap, mouseYMap);
	// Calculate selected row
	selectedRow = getSelectedRow(mouseXMap, mouseYMap);

	// Keep map centered on mouse cursor when zooming in/out
	mouseWheel = input->getMouseWheel();
	if (!win && !lose) {
		oldZoom = mapZoom;
		if (mouseWheel > 0)                         // If zoom in
			zoomIn(selectedRow, selectedCol, mouseXMap, mouseYMap, pointerX, pointerY);
		else if (mouseWheel < 0)                    // If zoom out
			zoomOut();

		// Zoom in/out on players position with Gamepad D-Pad 
		oldZoom = mapZoom;
		if (input->getGamepadDPadUp(0))			    // If zoom in
			zoomIn(playerRow, playerColumn, playerX, playerY, playerXMap, playerYMap);
		else if (input->getGamepadDPadDown(0))		// If zoom out
			zoomOut();
	}

	// Zoom mapTile
	mapTile.setScale(mapZoom);
	// Zoom player
	player.setScale(mapZoom);
	npc01.setScale(mapZoom);
	npc02.setScale(mapZoom);
	npc03.setScale(mapZoom);
	npc04.setScale(mapZoom);
	npc05.setScale(mapZoom);
	npc06.setScale(mapZoom);
	npc07.setScale(mapZoom);
	npc08.setScale(mapZoom);
	npc09.setScale(mapZoom);
	npc10.setScale(mapZoom);
	npc11.setScale(mapZoom);
	npc12.setScale(mapZoom);
	npc13.setScale(mapZoom);
	npc14.setScale(mapZoom);
	npc15.setScale(mapZoom);
	npc16.setScale(mapZoom);
	npc17.setScale(mapZoom);
	npc18.setScale(mapZoom);
	npc19.setScale(mapZoom);
	npc20.setScale(mapZoom);
	npc21.setScale(mapZoom);
	npc22.setScale(mapZoom);
	npc23.setScale(mapZoom);
	npc24.setScale(mapZoom);
	npc25.setScale(mapZoom);
	npc26.setScale(mapZoom);
	npc27.setScale(mapZoom);
	npc28.setScale(mapZoom);
	npc29.setScale(mapZoom);
	npc30.setScale(mapZoom);
	npc31.setScale(mapZoom);
	npc32.setScale(mapZoom);
	npc33.setScale(mapZoom);
	npc34.setScale(mapZoom);
	npc35.setScale(mapZoom);
	npc36.setScale(mapZoom);
	npc37.setScale(mapZoom);
	npc38.setScale(mapZoom);
	npc39.setScale(mapZoom);
	npc40.setScale(mapZoom);
	npc41.setScale(mapZoom);
	npc42.setScale(mapZoom);
	npc43.setScale(mapZoom);
	npc44.setScale(mapZoom);
	npc45.setScale(mapZoom);
	npc46.setScale(mapZoom);
	npc47.setScale(mapZoom);
	npc48.setScale(mapZoom);
	npc49.setScale(mapZoom);
	npc50.setScale(mapZoom);
	npc51.setScale(mapZoom);
	npc52.setScale(mapZoom);
	npc53.setScale(mapZoom);
	npc54.setScale(mapZoom);
	npc55.setScale(mapZoom);
	npc56.setScale(mapZoom);
	npc57.setScale(mapZoom);
	npc58.setScale(mapZoom);
	npc59.setScale(mapZoom);
	npc60.setScale(mapZoom);
	npc61.setScale(mapZoom);
	npc62.setScale(mapZoom);
	npc63.setScale(mapZoom);
	npc64.setScale(mapZoom);
	npc65.setScale(mapZoom);
	npc66.setScale(mapZoom);
	npc67.setScale(mapZoom);
	npc68.setScale(mapZoom);
	npc69.setScale(mapZoom);
	npc70.setScale(mapZoom);
	npc71.setScale(mapZoom);
	npc72.setScale(mapZoom);
	npc73.setScale(mapZoom);
	npc74.setScale(mapZoom);
	npc75.setScale(mapZoom);
	npc76.setScale(mapZoom);
	npc77.setScale(mapZoom);
	npc78.setScale(mapZoom);
	npc79.setScale(mapZoom);
	npc80.setScale(mapZoom);

	input->vibrateControllers(deltaTime);

    // Clear input keys pressed
    // Call this after all key checks are done
    input->clear(KEYS_PRESSED);
}

//=============================================================================
// Called when a key is pressed down.
//=============================================================================
void GameEngine2::Game::onKeyDown(CoreWindow ^sender, KeyEventArgs ^args)
{
    input->keyDown(args->VirtualKey);
}

//=============================================================================
// Called when a key is released.
//=============================================================================
void GameEngine2::Game::onKeyUp(CoreWindow^ sender, KeyEventArgs^ args)
{
    input->keyUp(args->VirtualKey);
}

//=============================================================================
// Called when a char is received.
//=============================================================================
void GameEngine2::Game::onCharReceived(CoreWindow^ sender,
    CharacterReceivedEventArgs^ args)
{
    input->keyIn(args->KeyCode);
}

//=============================================================================
// Called when a pointer is pressed.
//=============================================================================
void GameEngine2::Game::onPointerPressed(CoreWindow^ sender,
    PointerEventArgs^ args)
{
	input->setPointerXY(args->CurrentPoint->Position.X,
		args->CurrentPoint->Position.Y);
	if (args->CurrentPoint->Properties->IsLeftButtonPressed)
		input->setMouseLButton(true);   // Left mouse or pointer pressed.
	if (args->CurrentPoint->Properties->IsRightButtonPressed)
		input->setMouseRButton(true);   // Right mouse or pointer pressed.
	if (args->CurrentPoint->Properties->IsMiddleButtonPressed)
		input->setMouseMButton(true);   // Middle mouse or pointer pressed.
}

//=============================================================================
// Called when the pointer wheel is changed.
//=============================================================================
void GameEngine2::Game::onPointerWheelChanged(CoreWindow^ sender,
    PointerEventArgs^ args)
{
	input->mouseWheelIn(args->CurrentPoint->Properties->MouseWheelDelta);
	input->setPointerXY(args->CurrentPoint->Position.X,
		args->CurrentPoint->Position.Y);
}

//=============================================================================
// Called when the pointer is moved.
//=============================================================================
void GameEngine2::Game::onPointerMoved(CoreWindow ^ sender, PointerEventArgs ^ args)
{
	input->setPointerXY(args->CurrentPoint->Position.X,
		args->CurrentPoint->Position.Y);
}

//=====================================================================
// Called when a gamepad is added.
//=====================================================================
void GameEngine2::Game::onGamepadAdded(
	Platform::Object ^sender, Windows::Gaming::Input::Gamepad ^gamepad)
{
	input->gamepadAdded(gamepad);
}

//=============================================================================
// Artificial Intelligence
// Complete this function to add AI to your game.
//=============================================================================
void GameEngine2::Game::ai()
{}

//=============================================================================
// Handle collisions
// Complete this function to add collision detection to your game.
//=============================================================================
void GameEngine2::Game::collisions()
{
	Vector2 collisionVector;
	if (player.collidesWith(npc01, collisionVector))
	{
		player.bounceOffEntity(collisionVector, npc01);  // Bounce off npc.
		audio->PlaySound(HIT);  // Play HIT sound effect.

		// Change the direction of the collisionVector for ship2.
		collisionVector.x *= -1;
		collisionVector.y *= -1;

		npc01.bounceOffEntity(collisionVector, player);   // Bounce off player.
	}
}

//=====================================================================
// Check Enemy Range
// Checks the 8 tiles around the NPC for the player. Attacks when found
//=====================================================================
bool GameEngine2::Game::checkEnemyRange(Character &obj, int npcRow, int npcCol)
{
	bool found = false;

	if (objectMap[npcRow - 1][npcCol] == PLAYER) {
		found = true;
		obj.setDirection(NE); }
	else if (objectMap[npcRow + 1][npcCol] == PLAYER) {
		found = true;
		obj.setDirection(SW); }
	else if (objectMap[npcRow][npcCol - 1] == PLAYER) {
		found = true;
		obj.setDirection(NW); }
	else if (objectMap[npcRow][npcCol + 1] == PLAYER) {
		found = true;
		obj.setDirection(SE); }
	else if (objectMap[npcRow - 1][npcCol - 1] == PLAYER) {
		found = true;
		obj.setDirection(N); }
	else if (objectMap[npcRow - 1][npcCol + 1] == PLAYER) {
		found = true;
		obj.setDirection(E); }
	else if (objectMap[npcRow + 1][npcCol - 1] == PLAYER) {
		found = true;
		obj.setDirection(W); }
	else if (objectMap[npcRow + 1][npcCol + 1] == PLAYER) {
		found = true;
		obj.setDirection(S); }
	return found;
}

//=====================================================================
// Check Player Range
// Checks the 8 tiles around the player for NPC. Attacks when found
//=====================================================================
void GameEngine2::Game::checkPlayerRange(Player & obj, int playerRow, int playerColumn,
	int npcObjectNum, Character &npc)
{
	bool found;
	if ((objectMap[playerRow - 1][playerColumn] == npcObjectNum))
		found = true;
	else if (objectMap[playerRow + 1][playerColumn] == npcObjectNum)
		found = true;
	else if (objectMap[playerRow][playerColumn - 1] == npcObjectNum)
		found = true;
	else if (objectMap[playerRow][playerColumn + 1] == npcObjectNum)
		found = true;
	else if (objectMap[playerRow - 1][playerColumn - 1] == npcObjectNum)
		found = true;
	else if (objectMap[playerRow - 1][playerColumn + 1] == npcObjectNum)
		found = true;
	else if (objectMap[playerRow + 1][playerColumn - 1] == npcObjectNum)
		found = true;
	else if (objectMap[playerRow + 1][playerColumn + 1] == npcObjectNum)
		found = true;
	else
		found = false;
	
	// If NPC found and not dead, damage it
	if (found && !(npc.getDeath()))
		npc.damage(PLAYER_ATTACK);
}

//=====================================================================
// Get Column
// Returns the current column of the player.
//=====================================================================
int GameEngine2::Game::getColumn(Player &player, int offsetX, int offsetY)
{
	int col;
	// Calculate player column
	col = (int)(player.getX() / TEXTURE_SIZE_TERRAIN +
		(player.getY() + offsetY) / (TEXTURE_SIZE_TERRAIN / 2));
	return col;
}

//=====================================================================
// Get Column
// Returns the current column of the NPC.
//=====================================================================
int GameEngine2::Game::getColumn(Character &npc, int offsetX, int offsetY)
{
	int col;
	// Calculate npc column
	col = (int)(npc.getX() / TEXTURE_SIZE_TERRAIN +
		(npc.getY() + NPC_OFFSET_Y) / (TEXTURE_SIZE_TERRAIN / 2));
	return col;
}

//=====================================================================
// Get Selected Column
// Returns the selected column from mouse pointer.
//=====================================================================
int GameEngine2::Game::getSelectedColumn(float mouseXMap, float mouseYMap)
{
	int selectedCol = (int)(mouseXMap / TEXTURE_SIZE_TERRAIN + mouseYMap /
		(TEXTURE_SIZE_TERRAIN / 2));
	return selectedCol;
}

//=====================================================================
// Get Row
// Returns the current row of the player.
//=====================================================================
int GameEngine2::Game::getRow(Player &player, int offsetX, int offsetY)
{
	int row;
	// Calculate player row
	row = (int)((player.getY() + offsetY) / (TEXTURE_SIZE_TERRAIN / 2) -
		player.getX() / TEXTURE_SIZE_TERRAIN);
	return row;
}

//=====================================================================
// Get Selected Row
// Returns the selected row from mouse pointer.
//=====================================================================
int GameEngine2::Game::getSelectedRow(float mouseXMap, float mouseYMap)
{
	int selectedRow = (int)(mouseYMap / (TEXTURE_SIZE_TERRAIN / 2) - mouseXMap /
		TEXTURE_SIZE_TERRAIN);
	return selectedRow;
}

//=====================================================================
// Draw NPC
// Draws the NPC object.
//=====================================================================
void GameEngine2::Game::drawNPC(Character & npc, CanvasSpriteBatch ^ spriteBatch)
{
	float x = npc.getX();	// Save current position
	float y = npc.getY();
	// calculate zoomed and scrolled position
	npc.setX((float)(SCREEN_X)+mapZoom * (x + scrollX));   // Position npc
	npc.setY((float)(SCREEN_Y)+mapZoom * (y + scrollY));
	// Draw npc
	npc.draw(spriteBatch);
	npc.setX(x);       // restore unzoomed position
	npc.setY(y);
}

//=====================================================================
// Draw Player
// Draws the Player object.
//=====================================================================
void GameEngine2::Game::drawPlayer(Player & player, CanvasSpriteBatch ^ spriteBatch)
{
	playerX = player.getX();  // save current position
	playerY = player.getY();
	// calculate zoomed and scrolled position
	playerXMap = (float)(SCREEN_X)+mapZoom * (playerX + scrollX);
	playerYMap = (float)(SCREEN_Y)+mapZoom * (playerY + scrollY);
	player.setX(playerXMap);   // Position player
	player.setY(playerYMap);
	// Draw player
	player.draw(spriteBatch);
	player.setX(playerX);       // restore unzoomed position
	player.setY(playerY);
}

//=====================================================================
// Draw Object
// Draws the other objects within ObjectMap, such as trees,houses,etc.
//=====================================================================
void GameEngine2::Game::drawObject(Image & imgObj, int row, int col, int offsetX,
	int offsetY, int frame, CanvasSpriteBatch ^ spriteBatch)
{
	// Position object
	imgObj.setX((float)(SCREEN_X)+mapZoom * (
		-row * (TEXTURE_SIZE_TERRAIN / 2) +
		col * (TEXTURE_SIZE_TERRAIN / 2) + offsetX + scrollX));
	imgObj.setY((float)(SCREEN_Y)+mapZoom * (
		row * (TEXTURE_SIZE_TERRAIN / 4) +
		col * (TEXTURE_SIZE_TERRAIN / 4) +
		// Move down 1/2 tile height because tiles are drawn from
		// center x,y.
		TEXTURE_SIZE_TERRAIN / 2 + (offsetY) + scrollY));
	imgObj.setCurrentFrame(frame);
	// Draw image
	imgObj.draw(spriteBatch);
}

//=====================================================================
// Position NPC
// Positions the NPC to its start position.
//=====================================================================
void GameEngine2::Game::positionNPC(Character & npc, int startRow, int startCol, int offsetX, int offsetY)
{
	// Position npc
	npc.setX((float)(
		-startRow * (TEXTURE_SIZE_TERRAIN / 2) +
		startCol * (TEXTURE_SIZE_TERRAIN / 2) + offsetX));
	npc.setY((float)(
		startRow * (TEXTURE_SIZE_TERRAIN / 4) +
		startCol * (TEXTURE_SIZE_TERRAIN / 4) +
		// Move down 1/2 tile height because tiles are drawn from
		// center x,y.
		TEXTURE_SIZE_TERRAIN / 2 + (offsetY)) - (NPC_HEIGHT));
}

//=====================================================================
// Position Player
// Positions the player to their start position.
//=====================================================================
void GameEngine2::Game::positionPlayer(Player & player, int startRow, int startCol, int offsetX, int offsetY)
{
	// Position player
	player.setX((float)(
		-startRow * (TEXTURE_SIZE_TERRAIN / 2) +
		startCol * (TEXTURE_SIZE_TERRAIN / 2) + offsetX));
	player.setY((float)(
		startRow * (TEXTURE_SIZE_TERRAIN / 4) +
		startCol * (TEXTURE_SIZE_TERRAIN / 4) +
		// Move down 1/2 tile height because tiles are drawn from
		// center x,y.
		TEXTURE_SIZE_TERRAIN / 2 + offsetY) - (PLAYER_HEIGHT));
}
//=====================================================================
// Zoom In
// Zoom in on the map.
//=====================================================================
void GameEngine2::Game::zoomIn(int selectedRow, int selectedCol, float mouseXMap, float mouseYMap, float pointerX, float pointerY)
{
	mapZoom += ZOOM_AMOUNT;
	if (mapZoom > MAX_ZOOM)
		mapZoom = MAX_ZOOM;

	float currentX = ((float)(SCREEN_X)+oldZoom * (
	-selectedRow * (TEXTURE_SIZE_TERRAIN / 2) +
	selectedCol * (TEXTURE_SIZE_TERRAIN / 2) + scrollX));
	float newX = ((float)(SCREEN_X)+mapZoom * (
	-selectedRow * (TEXTURE_SIZE_TERRAIN / 2) +
	selectedCol * (TEXTURE_SIZE_TERRAIN / 2) + scrollX));

	// Center zoom in at current pointer position.
	scrollX = (pointerX - SCREEN_X) / mapZoom - mouseXMap;
	scrollY = (pointerY - SCREEN_Y) / mapZoom - mouseYMap;
}

//=====================================================================
// Zoom Out
// Zoom Out on the map.
//=====================================================================
void GameEngine2::Game::zoomOut()
{
	mapZoom -= ZOOM_AMOUNT;
	if (mapZoom < MIN_ZOOM)
		mapZoom = MIN_ZOOM;

	// Center zoom out at window center.
	mouseXMap = (XamlCanvas->Size.Width / 2.0f - SCREEN_X) / oldZoom -
		scrollX;
	mouseYMap = (XamlCanvas->Size.Height / 2.0f - SCREEN_Y) / oldZoom -
		scrollY;
	scrollY = XamlCanvas->Size.Height / 2.0f;
	scrollY = (XamlCanvas->Size.Height / 2.0f - SCREEN_Y) / mapZoom -
		mouseYMap;
}

//=====================================================================
// Update NPC Location
// Update NPC location in Object Map
//=====================================================================
void GameEngine2::Game::updateNpcLocation(Character & npc, int &npcCol, int &npcRow, int npcObjectNum)
{
	objectMap[npcRow][npcCol] = 0;		// Clear current npc position

	// Calcuate location of NPC's relative to Isometric Grid.
	// This treats the top corner of the board as location 0,0 for each.
	npcCol = getColumn(npc, NPC_OFFSET_X, NPC_OFFSET_Y);
	npcRow = getRow(npc, NPC_OFFSET_X, NPC_OFFSET_Y);
	// Update npc location in objectMap.
	if (objectMap[npcRow][npcCol] == 0)    // If this location is empty?
		objectMap[npcRow][npcCol] = npcObjectNum;   // Save location to map.	
}

//=====================================================================
// Update Player Location
// Update player location in Object Map
//=====================================================================
void GameEngine2::Game::updatePlayerLocation(Player & player, int & playerColumn, int & playerRow, int playerObjectNum)
{
	objectMap[playerRow][playerColumn] = 0;		// Clear current player position

	// Calcuate location of the player relative to Isometric Grid.
	// This treats the top corner of the board as location 0,0.
	playerColumn = getColumn(player, PLAYER_OFFSET_X, PLAYER_OFFSET_Y);
	playerRow = getRow(player, PLAYER_OFFSET_X, PLAYER_OFFSET_Y);

	// If person row column is blocked by something on map.
	if (objectMap[playerRow][playerColumn] > 0)
	{
		player.moveToOldXY();   // prevent move
								// Calculate player column/row
		playerColumn = getColumn(player, PLAYER_OFFSET_X, PLAYER_OFFSET_Y);
		playerRow = getRow(player, PLAYER_OFFSET_X, PLAYER_OFFSET_Y);
	}
	else {  // Else, person moved.
			// Make sure person Row, Col is inside map range.
		if (playerRow < 0)
			playerRow = 0;
		else if (playerRow >= MAP_HEIGHT)
			playerRow = MAP_HEIGHT - 1;
		if (playerColumn < 0)
			playerColumn = 0;
		else if (playerColumn >= MAP_WIDTH)
			playerColumn = MAP_WIDTH - 1;
	}

	// Update npc location in objectMap.
	if (objectMap[playerRow][playerColumn] == 0)    // If this location is empty?
		objectMap[playerRow][playerColumn] = playerObjectNum;   // Save location to map.
}

//=====================================================================
// Find Player
// Checks the 8 tiles around the NPC for the player. Attacks when found
//=====================================================================
void GameEngine2::Game::findPlayer(Character & npc, int npcRow, int npcCol)
{
	if (npc.getDeath() == false)
	{
		// Check for player in range
		if (checkEnemyRange(npc, npcRow, npcCol)) {
			npc.setAttack(true);
			player.damage(NPC_ATTACK);	// Attack player when found
		}
		else
			npc.setAttack(false);
	}
}

//=====================================================================
// Initialize NPC
// Initialize NPC on start
//=====================================================================
void GameEngine2::Game::initializeNPC(Character & npc, int npcStartRow, int npcStartCol)
{
	// npc image
	if (!npc.initialize(audio, NPC_TEXTURE_WIDTH, NPC_TEXTURE_HEIGHT, NPC_TEXTURE_COLS, archerTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing npc image."));
	// Position npc on start
	positionNPC(npc, npcStartRow, npcStartCol, NPC_OFFSET_X,
		NPC_OFFSET_Y);
}

//=====================================================================
// Get Row
// Returns the current row of the NPC.
//=====================================================================
int GameEngine2::Game::getRow(Character & npc, int offsetX, int offsetY)
{
	int row;
	// Calculate npc row
	row = (int)((npc.getY() + offsetY) / (TEXTURE_SIZE_TERRAIN / 2) -
		npc.getX() / TEXTURE_SIZE_TERRAIN);
	return row;
}

