#pragma once
#include <D2d1.h>
#include "Entity.h"

namespace GameEngine2
{
	//enum WALK_DIRECTIONN { N, NE, E, SE, S, SW, W, NW };

	class Character;				 // Foward Declaration

	class Character : public Entity
	{
	private:
		// Character properties
		WALK_DIRECTION  aimDirection;              // direction to move
		Vector2     oldCenter;                  // Previous location

		// Character variables

	public:
		// Constructor
		Character();	

		// Character Properties
		const int   WIDTH = 48;                 // image width (each frame)
		const int   HEIGHT = 92;                // image height
		const int   TEXTURE_COLS = 9;           // texture has 8 columns
		const float X = 0;						// location on screen
		const float Y = 0;
		const float ROTATION_RATE = (float)PI;  // radians per second
		const float FRICTION = 0.9f;            // 10% energy loss
		const float MASS = 300.0f;              // mass
		const float WALK_ANIMATION_DELAY = 0.06f;    // time between frames

		// Archer Frames
		bool attack;
		bool isWalking;
		int lives;
		bool isDead;
		bool timerOn;
		double timer;
		// NPC Frames
		const int   S_START_FRAME = 90;		    // Walk South start frame.
		const int   S_END_FRAME = 104;			// Walk South end frame.
		const int   SW_START_FRAME = 75;	    // Walk South West start frame.
		const int   SW_END_FRAME = 89;	        // Walk South West end frame.
		const int   W_START_FRAME = 60;		    // Walk West start frame.
		const int   W_END_FRAME = 74;		    // Walk West end frame.
		const int   NW_START_FRAME = 45;	    // Walk North West start frame.
		const int   NW_END_FRAME = 59;		    // Walk North West end frame.
		const int   N_START_FRAME = 30;	        // Walk North start frame.
		const int   N_END_FRAME = 44;			// Walk North end frame.
		const int   NE_START_FRAME = 15;	    // Walk North East start frame.
		const int   NE_END_FRAME = 29;	        // Walk North East end frame.
		const int   E_START_FRAME = 0;		    // Walk East start frame.
		const int   E_END_FRAME = 14;		    // Walk East end frame.
		const int   SE_START_FRAME = 105;       // Walk South East start frame.
		const int   SE_END_FRAME = 119;	        // Walk South East end frame.
		const int	E_ATTACK_START_FRAME = 120;
		const int	E_ATTACK_END_FRAME = 135;
		const int	NE_ATTACK_START_FRAME = 136;
		const int	NE_ATTACK_END_FRAME = 151;
		const int	N_ATTACK_START_FRAME = 152;
		const int	N_ATTACK_END_FRAME = 167;
		const int	NW_ATTACK_START_FRAME = 168;
		const int	NW_ATTACK_END_FRAME = 183;
		const int	W_ATTACK_START_FRAME = 184;
		const int	W_ATTACK_END_FRAME = 199;
		const int	SW_ATTACK_START_FRAME = 200;
		const int	SW_ATTACK_END_FRAME = 215;
		const int	S_ATTACK_START_FRAME = 216;
		const int	S_ATTACK_END_FRAME = 231;
		const int	SE_ATTACK_START_FRAME = 232;
		const int	SE_ATTACK_END_FRAME = 247;
		const int	SE_DEATH_START_FRAME = 248;
		const int	SE_DEATH_END_FRAME = 260;
		const int	E_DEATH_START_FRAME = 261;
		const int	E_DEATH_END_FRAME = 273;
		const int	NE_DEATH_START_FRAME = 274;
		const int	NE_DEATH_END_FRAME = 286;
		const int	N_DEATH_START_FRAME = 287;
		const int	N_DEATH_END_FRAME = 299;
		const int   NW_DEATH_START_FRAME = 300;
		const int	NW_DEATH_END_FRAME = 312;
		const int	W_DEATH_START_FRAME = 313;
		const int	W_DEATH_END_FRAME = 325;
		const int	SW_DEATH_START_FRAME = 326;
		const int	SW_DEATH_END_FRAME = 338;
		const int	S_DEATH_START_FRAME = 339;
		const int	S_DEATH_END_FRAME = 351;
		const float WALK_SPEED = 70;            // Pixels/second speed

		//---------------------------------------------------------------------
		// Inherited member functions

		// Initialize Image
		// Pre: audioPtr = pointer to audio object.
		//      w = width of Image in pixels  (0 = use full texture width).
		//      h = height of Image in pixels (0 = use full texture height).
		//      ncols = number of columns in texture (1 to n) (0 same as 1).
		//      textures = Texture images.
		virtual bool initialize(Audio *audioPtr, int w, int h, int ncols,
			CanvasBitmap^ textures);

		// Draw Image using color as filter. Default color is WHITE.
		// textureN is number of texture in textureManager 
		//virtual void draw(CanvasSpriteBatch^ spriteBatch,
		//	float frameTime);

		// Update the animation. frameTime is used to regulate the speed.
		virtual void update(float frameTime);

		////////////////////////////////////////
		//           Set functions            //
		////////////////////////////////////////

		// Use this function to set the animation property of a Character if walking
		virtual void setWalking(bool b)
		{
			if (b == true)
				animComplete = false;
			else
				animComplete = true;
		}

		//=====================================================================
		// Set NPC direction {N, NE, E, SE, S, SW, W, NW}
		//=====================================================================
		virtual void setDirection(WALK_DIRECTION dir);

		//=====================================================================
		// Set NPC attack status (T or F)
		//=====================================================================
		void setAttack(bool b);

		//=====================================================================
		// Set NPC death status (T or F)
		//=====================================================================
		void setDeath(bool b);


		////////////////////////////////////////
		//           Get functions            //
		////////////////////////////////////////

		//=====================================================================
		// Get NPC direction {N, NE, E, SE, S, SW, W, NW}
		//=====================================================================
		WALK_DIRECTION getDirection() const
		{
			return walkDirection;
		}

		//=====================================================================
		// Get NPC attack status (T or F)
		//=====================================================================
		bool getAttack() const
		{
			return attack;
		}

		//=====================================================================
		// Get NPC death status (T or F)
		//=====================================================================
		bool getDeath() const
		{
			return isDead;
		}

		////////////////////////////////////////
		//           Other functions          //
		////////////////////////////////////////

		//=====================================================================
		// Damage NPC with WEAPON parameter when called
		//=====================================================================
		void damage(WEAPON);

		//=====================================================================
		// Called when NPC health <= 0, sets death animation
		//=====================================================================
		void die();


		void respawn();
	};
}