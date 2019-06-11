#pragma once
#include <D2d1.h>
#include "Entity.h"

namespace GameEngine2
{
	class Player;				 // Foward Declaration

	class Player : public Entity
	{
	private:
		// Player properties
		//WALK_DIRECTION  direction;              // direction to move
		Vector2     oldCenter;                  // Previous location
		float		speed;						// Movement Speed
	public:
		// Constructor
		Player();

		// Player Properties
		const int   WIDTH = 196;                 // image width (each frame)
		const int   HEIGHT = 151;                // image height
		const int   TEXTURE_COLS = 16;           // texture has 8 columns
		const float X = 0;					// location on screen
		const float Y = 0;
		const float ROTATION_RATE = (float)PI;  // radians per second
		const float FRICTION = 0.9f;            // 10% energy loss
		const float MASS = 300.0f;              // mass
		const float WALK_ANIMATION_DELAY = 0.06f;    // time between frames
		const int   S_START_FRAME = 20;		    // Walk South start frame.
		const int   S_END_FRAME = 29;			// Walk South end frame.
		const int   SW_START_FRAME = 10;	    // Walk South West start frame.
		const int   SW_END_FRAME = 19;	        // Walk South West end frame.
		const int   W_START_FRAME = 0;		    // Walk West start frame.
		const int   W_END_FRAME = 9;		    // Walk West end frame.
		const int   NW_START_FRAME = 70;	    // Walk North West start frame.
		const int   NW_END_FRAME = 79;		    // Walk North West end frame.
		const int   N_START_FRAME = 60;	        // Walk North start frame.
		const int   N_END_FRAME = 69;			// Walk North end frame.
		const int   NE_START_FRAME = 50;	    // Walk North East start frame.
		const int   NE_END_FRAME = 59;	        // Walk North East end frame.
		const int   E_START_FRAME = 40;		    // Walk East start frame.
		const int   E_END_FRAME = 49;		    // Walk East end frame.
		const int   SE_START_FRAME = 30;        // Walk South East start frame.
		const int   SE_END_FRAME = 39;	        // Walk South East end frame.
		const int	NE_ATTACK_START_FRAME = 80;
		const int	NE_ATTACK_END_FRAME = 89;
		const int	N_ATTACK_START_FRAME = 90;
		const int	N_ATTACK_END_FRAME = 99;
		const int	NW_ATTACK_START_FRAME = 100;
		const int	NW_ATTACK_END_FRAME = 109;
		const int	W_ATTACK_START_FRAME = 110;
		const int	W_ATTACK_END_FRAME = 119;
		const int	SW_ATTACK_START_FRAME = 120;
		const int	SW_ATTACK_END_FRAME = 129;
		const int	S_ATTACK_START_FRAME = 130;
		const int	S_ATTACK_END_FRAME = 139;
		const int	SE_ATTACK_START_FRAME = 140;
		const int	SE_ATTACK_END_FRAME = 149;
		const int	E_ATTACK_START_FRAME = 150;
		const int	E_ATTACK_END_FRAME = 159;
		const int	NE_DEATH_START_FRAME = 160;
		const int	NE_DEATH_END_FRAME = 175;
		const int	N_DEATH_START_FRAME = 176;
		const int	N_DEATH_END_FRAME = 191;
		const int	NW_DEATH_START_FRAME = 192;
		const int	NW_DEATH_END_FRAME = 207;
		const int	W_DEATH_START_FRAME = 208;
		const int	W_DEATH_END_FRAME = 223;
		const int	SW_DEATH_START_FRAME = 224;
		const int	SW_DEATH_END_FRAME = 239;
		const int	S_DEATH_START_FRAME = 240;
		const int	S_DEATH_END_FRAME = 255;
		const int	SE_DEATH_START_FRAME = 256;
		const int	SE_DEATH_END_FRAME = 271;
		const int	E_DEATH_START_FRAME = 272;
		const int	E_DEATH_END_FRAME = 287;
		const float RUN_SPEED = 100;
		const float WALK_SPEED = 70;            // Pixels/second speed
		bool		attack;
		bool		isWalking;
		int			lives;
		bool		isDead;
		float timer;
		bool tmrOn;
		float tmr;
		
		//---------------------------------------------------------------------
		// Inherited member functions

		// Initialize Image
		// Pre: audioPtr = pointer to audio object.
		//      w = width of Image in pixels  (0 = use full texture width).
		//      h = height of Image in pixels (0 = use full texture height).
		//      ncols = number of columns in texture (1 to n) (0 same as 1).
		//      textures = Texture images.
		virtual bool initialize(Audio *audioPtr, 
			CanvasBitmap^ textures);

		// Update the animation. frameTime is used to regulate the speed.
		virtual void update(float frameTime);

		////////////////////////////////////////
		//           Set functions            //
		////////////////////////////////////////

		// Use this function to set the animation property of a Player if walking
		virtual void setWalking(bool b) 
		{ 
			/*if (b == true)
				animComplete = false;
			else
				animComplete = true;*/
			isWalking = b;
		}
		
		//=====================================================================
		// Set player direction {N, NE, E, SE, S, SW, W, NW}
		//=====================================================================
		virtual void setDirection(WALK_DIRECTION dir);

		//=====================================================================
		// Set player walk speed
		//=====================================================================
		virtual void setSpeedWalk() {
			speed = WALK_SPEED; }

		//=====================================================================
		// Set player run speed
		//=====================================================================
		virtual void setSpeedRun() {
			speed = RUN_SPEED; }

		//=====================================================================
		// Set player speed
		//=====================================================================
		virtual void setSpeed(int s) {
			speed = s; }

		//=====================================================================
		// Set player attack (T or F)
		//=====================================================================
		void setAttack(bool b);

		//=====================================================================
		// Set player death (T or F)
		//=====================================================================
		void setDeath(bool b);

		//=====================================================================
		// Set player walking status (T or F)
		//=====================================================================
		void setIsWalking(bool b) {
			isWalking = b; }

		////////////////////////////////////////
		//           Get functions            //
		////////////////////////////////////////

		//=====================================================================
		// Get player direction {N, NE, E, SE, S, SW, W, NW}
		//=====================================================================
		WALK_DIRECTION getDirection() const {
			return walkDirection; }

		//=====================================================================
		// Get player walking status (T or F)
		//=====================================================================
		bool getWalking() const {
			return isWalking; }

		//=====================================================================
		// Get player attack status (T or F)
		//=====================================================================
		bool getAttack() const {
			return attack; }

		//=====================================================================
		// Get player death status (T or F)
		//=====================================================================
		bool getDeath() const {
			return isDead; }

		//=====================================================================
		// Get player lives
		//=====================================================================
		int getLives() const { return lives; }

		////////////////////////////////////////
		//           Other functions          //
		////////////////////////////////////////
		
		//=====================================================================
		// Damage player with WEAPON parameter when called
		//=====================================================================
		void damage(WEAPON);

		//=====================================================================
		// Called when player health <= 0, subtracts a life and sets animation
		//=====================================================================
		void die();

		//=====================================================================
		// Respawn player back to full health and reset frames when called
		//=====================================================================
		void respawn();
	};
}