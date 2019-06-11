#include "pch.h"
#include "Character.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GameEngine2::Character::Character() : Entity()
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
bool GameEngine2::Character::initialize(Audio * audioPtr, int w, int h, int ncols, CanvasBitmap ^ textureImage)
{
	return(Entity::initialize(audioPtr, w, h, ncols, textureImage));
}

//-----------------------------------------------------------------------------
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//-----------------------------------------------------------------------------
void GameEngine2::Character::update(float frameTime)
{
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
		setAnimationComplete(false);

	}
	if (timerOn)
	{
		timer -= frameTime;
		if (timer <= 0)
			setLoop(false);
	}
	Entity::update(frameTime);	
}

//=====================================================================
// Set NPC direction {N, NE, E, SE, S, SW, W, NW}
//=====================================================================
void GameEngine2::Character::setDirection(WALK_DIRECTION dir)
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
	aimDirection = dir;    // Movement direction
}

//=====================================================================
// Set NPC attack status (T or F)
//=====================================================================
void GameEngine2::Character::setAttack(bool b)
{
	attack = b;
	if (attack)
	{	
		switch (aimDirection)
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
		setAnimationComplete(false);
	}
}

//=====================================================================
// Set NPC death status (T or F)
//=====================================================================
void GameEngine2::Character::setDeath(bool b)
{
	isDead = b;
	timerOn = true;
	timer = 0.25;
}

//=====================================================================
// Damage NPC with WEAPON parameter when called
//=====================================================================
void GameEngine2::Character::damage(WEAPON weapon)
{
	switch (weapon)
	{
	case PLAYER_ATTACK:
		//audio->PlaySound(8);			// play hit sound
		health -= .7f;
		break;
	}
	if (health <= 0)
		die();
}

//=====================================================================
// Called when NPC health <= 0, sets death animation
//=====================================================================
void GameEngine2::Character::die()
{
	setAttack(false);
	health = 0;
	setDeath(true);
}

void GameEngine2::Character::respawn()
{
	health = 100;
}
