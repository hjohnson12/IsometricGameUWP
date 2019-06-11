#include "pch.h"
#include "Player.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GameEngine2::Player::Player() : Entity()
{
	width = WIDTH;						// size of person
	height = HEIGHT;
	center.x = X;						// location on screen
	center.y = Y;
	oldCenter = center;
	frameDelay = WALK_ANIMATION_DELAY;
	startFrame = SE_START_FRAME;		    // First frame of animation.
	endFrame = SE_END_FRAME;             // Last frame of animation.
	currentFrame = startFrame;
	loop = false;
	setBounciness(0.5f);
	health = 100;
	lives = 4;
	tmrOn = false;
}

//-----------------------------------------------------------------------------
// Initialize Image
// Pre: audioPtr = pointer to audio object.
//      width = width of Image in pixels  (0 = use full texture width).
//      height = height of Image in pixels (0 = use full texture height).
//      ncols = number of columns in texture (1 to n) (0 same as 1).
//      ^textures = pointer to textures.
// Initialize the Image.
// Post: returns true if successful, false if failed
//-----------------------------------------------------------------------------
bool GameEngine2::Player::initialize(Audio * audioPtr, CanvasBitmap ^ textureImage)
{
	return(Entity::initialize(audioPtr, WIDTH, HEIGHT, TEXTURE_COLS, textureImage));
}

//-----------------------------------------------------------------------------
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//-----------------------------------------------------------------------------
void GameEngine2::Player::update(float frameTime)
{
	if (attack)
	{
		WALK_DIRECTION dir = getDirection();

		switch (dir)
		{
		case N:
			setFrames(N_ATTACK_START_FRAME, N_ATTACK_END_FRAME);
			setLoop(true);
			break;
		case NE:
			setFrames(NE_ATTACK_START_FRAME, NE_ATTACK_END_FRAME);
			setLoop(true);
			break;
		case E:
			setFrames(E_ATTACK_START_FRAME, E_ATTACK_END_FRAME);
			setLoop(true);
			break;
		case SE:
			setFrames(SE_ATTACK_START_FRAME, SE_ATTACK_END_FRAME);
			setLoop(true);
			break;
		case S:
			setFrames(S_ATTACK_START_FRAME, S_ATTACK_END_FRAME);
			setLoop(true);
			break;
		case SW:
			setFrames(SW_ATTACK_START_FRAME, SW_ATTACK_END_FRAME);
			setLoop(true);
			break;
		case W:
			setFrames(W_ATTACK_START_FRAME, W_ATTACK_END_FRAME);
			setLoop(true);
			break;
		case NW:
			setFrames(NW_ATTACK_START_FRAME, NW_ATTACK_END_FRAME);
			setLoop(true);
			break;
		}
		setAnimationComplete(true); 
	}


	if (tmrOn)
	{
		tmr -= frameTime;
		if (tmr <= 0)
			tmrOn = false;
	}

	if (isDead)
	{
		WALK_DIRECTION dir = getDirection();
		switch (dir)
		{
		case N:
			setFrames(N_DEATH_START_FRAME, N_DEATH_END_FRAME);
			setLoop(true);
			break;
		case NE:
			setFrames(NE_DEATH_START_FRAME, NE_DEATH_END_FRAME);
			setLoop(true);
			break;
		case E:
			setFrames(E_DEATH_START_FRAME, E_DEATH_END_FRAME);
			setLoop(true);
			break;
		case SE:
			setFrames(SE_DEATH_START_FRAME, SE_DEATH_END_FRAME);
			setLoop(true);
			break;
		case S:
			setFrames(S_DEATH_START_FRAME, S_DEATH_END_FRAME);
			setLoop(true);
			break;
		case SW:
			setFrames(SW_DEATH_START_FRAME, SW_DEATH_END_FRAME);
			setLoop(true);
			break;
		case W:
			setFrames(W_DEATH_START_FRAME, W_DEATH_END_FRAME);
			setLoop(true);
			break;
		case NW:
			setFrames(NW_DEATH_START_FRAME, NW_DEATH_END_FRAME);
			setLoop(true);
			break;
		}
		setAnimationComplete(true); // (stops it from moving some while dying)
	}	

	Entity::update(frameTime);

	if (getAnimationComplete() == false)     // If player is moving?
	{

		switch (walkDirection)
		{
		case N:
			center.y -= frameTime * speed * 0.5f;  // move player
			break;
		case NE:
			center.x += frameTime * speed;         // move player
			center.y -= frameTime * speed * 0.5f;  // move player
			break;
		case E:
			center.x += frameTime * speed;         // move player
			break;
		case SE:
			center.x += frameTime * speed;         // move player
			center.y += frameTime * speed * 0.5f;  // move player
			break;
		case S:
			center.y += frameTime * speed * 0.5f;  // move player
			break;
		case SW:
			center.x -= frameTime * speed;         // move player
			center.y += frameTime * speed * 0.5f;  // move player
			break;
		case W:
			center.x -= frameTime * speed;         // move player
			break;
		case NW:
			center.x -= frameTime * speed;         // move player
			center.y -= frameTime * speed * 0.5f;  // move player
			break;
		}
	}

}

//=====================================================================
// Set player direction {N, NE, E, SE, S, SW, W, NW}
//=====================================================================
void GameEngine2::Player::setDirection(WALK_DIRECTION dir)
{
	switch (dir)
	{
	case N:
		setFrames(N_START_FRAME, N_END_FRAME);
		setLoop(true);
		break;
	case NE:
		setFrames(NE_START_FRAME, NE_END_FRAME);
		setLoop(true);
		break;
	case E:
		setFrames(E_START_FRAME, E_END_FRAME);
		setLoop(true);
		break;
	case SE:
		setFrames(SE_START_FRAME, SE_END_FRAME);
		setLoop(true);
		break;
	case S:
		setFrames(S_START_FRAME, S_END_FRAME);
		setLoop(true);
		break;
	case SW:
		setFrames(SW_START_FRAME, SW_END_FRAME);
		setLoop(true);
		break;
	case W:
		setFrames(W_START_FRAME, W_END_FRAME);
		setLoop(true);
		break;
	case NW:
		setFrames(NW_START_FRAME, NW_END_FRAME);
		setLoop(true);
		break;
	}
	setAnimationComplete(false);
	walkDirection = dir;    // Movement direction
}

//=====================================================================
// Set player attack (T or F)
//=====================================================================
void GameEngine2::Player::setAttack(bool b)
{
	attack = b;	
}

//=====================================================================
// Set player death (T or F)
//=====================================================================
void GameEngine2::Player::setDeath(bool b)
{
	isDead = b;

	if (b == false)
	{
		setFrames(SE_START_FRAME, SE_END_FRAME);
		setLoop(false);
	}
}

//=====================================================================
// Damage player with WEAPON parameter when called
//=====================================================================
void GameEngine2::Player::damage(WEAPON weapon)
{
	switch (weapon)
	{
	case NPC_ATTACK:
		if (!tmrOn)
		{
			audio->PlaySound(9);			// play hit sound
			tmrOn = true;
			tmr = 1;
		}
		health -= .155;
		break;
	}
	if (health <= 0)
		die();
}

//=====================================================================
// Called when player health <= 0, subtracts a life and sets animation
//=====================================================================
void GameEngine2::Player::die()
{
	health = 0;
	lives -= 1;
	if (lives <= 0)
		lives = 0;
	setDeath(true);	
}

//=====================================================================
// Respawn player back to full health and reset frames when called
//=====================================================================
void GameEngine2::Player::respawn()
{
	health = 100;
	setDeath(false);
}
