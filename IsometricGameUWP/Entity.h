#pragma once
//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// Entity.h v17.2.3
//
// Entity objects inherit from the Image class. Entities add the ability to
// collide with other entities. Three collision types are supported: CIRCLE,
// BOX and ROTATED_BOX. Entities also have mass and velocity properties which
// are used by the physics calculations.
//
// The following code uses functions contained in the
// Windows::Foundation::Numerics:: namespace. That namespace is
// included in Utilities.h which means it is not necessary to prefix
// the function calls with the namespace, however I have added the
// prefix to this code to clearly identify where the functions are located.
//=============================================================================

#include <D2d1.h>
#include <WindowsNumerics.h>
#include "Utilities.h"
#include "Image.h"
//#include "Input.h"
#include "Audio.h"
//#include "Game.h"
#include "common.h"
using namespace Microsoft::Graphics::Canvas;

namespace GameEngine2
{
    enum COLLISION_TYPE { CIRCLE, BOX, ROTATED_BOX };
    //enum DIRECTION { STRAIGHT, LEFT, RIGHT };   // rotation direction
	enum DIRECTION { NORTH, SOUTH, EAST, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST };   // rotation direction
	enum WALK_DIRECTION { N, NE, E, SE, S, SW, W, NW };

    const float GRAVITY = 6.67428e-11f;         // gravitational constant
    // How attracted entities are to each other.
    // Between 0 and 1.0f where 0 = no cohesion and 1.0 = glue.
    const float COHESION = 0.6f;                

    class Entity;               // Forward declaration

    class Entity : public Image
    {
    private:
        Vector2 cv;
        Vector2 Vdiff;          // = velocity - ent.velocity;
        Vector2 cUV;            // collision unit vector
        bool collide;
        float cUVdotVdiff;
        float   radius;         // Radius of collision circle.
        float   scaledRadius;   // Scaled version of radius.
        // edge specifies the collision box relative to the center of the entity.
        // left and top are typically negative numbers
        RECT    edge;           // For BOX and ROTATED_BOX collision detection
        RECT    scaledEdge;     // Scaled version of edge.

        // Entity properties
    protected:
        COLLISION_TYPE  collisionType;
        Vector2 oldCenter;      // The location in the previous frame.
        Vector2 distSquared;    // Used for calculating circle collision.
        float   sumRadiiSquared;
        Vector2 corners[4];     // for ROTATED_BOX collision detection
        Vector2 edge01, edge03;  // edges used for projection
        // min and max projections for this entity
        float   entA01min, entA01max, entA03min, entA03max;
        // min and max projections for other entity
        float   entB01min, entB01max, entB03min, entB03max;
        // projection overlaps 
        float overlap01, overlap03, minOverlap;
        Vector2 velocity;       // velocity
        Vector2 deltaV;         // added to velocity during next call to update()
        float   rotationRate;   // current rotation rate (radians/second)
        DIRECTION direction;    // direction of rotation
		WALK_DIRECTION walkDirection;
        float   mass;           // Mass of entity
        float   health;         // health 0 to 100
        float   rr;             // Radius squared variable
        float   force;          // Force of gravity
        float   gravity;        // gravitational constant of the game universe
        //Input   *input;         // pointer to the input system
        Audio   *audio;         // pointer to audio system
        bool    active;         // only active entities may collide
        bool    rotatedBoxReady;    // true when rotated collision box is ready
        bool    intersecting;   // true when this entity is intersecting another entity
        // The bounce function will perform an extra move of the entity when embedded is true.
        bool    embedded;       // true when this entity is completely contained within the collision area of another entity.
        float   bounciness;     // how bouncy is this entity 0 (none) through 1 (max)
        float   cohesion;       // How attracted is this entity to other entities.
        bool    noBounce;       // true indicates this entity does not move as a result of a collision

        //=====================================================================
        // --- The following functions are protected because they are not
        // intended to be called from outside the class.

        // Circular collision detection method.
        // Called by collision(), this is the default collision detection method.
        // Pre:  &ent = The other Entity.
        //       &collisionVector = Set if colliding.
        // Post: Returns true if collision, false otherwise.
        //       Sets collisionVector if collision. The collisionVector points in the
        //         direction of force that would be applied to this entity as a result
        //         of the collision. The magnitude of the collision vector is the
        //         distance the entities are overlapping.
        virtual bool collideCircle(Entity &ent, Vector2 &collisionVector);

        // Axis aligned bounding box collision detection method
        // Called by collision()
        // Pre:  &ent = The other Entity.
        //       &collisionVector = Set if colliding.
        // Post: Returns true if collision, false otherwise.
        //       Sets collisionVector if collision. The collisionVector points in the
        //         direction of force that would be applied to this entity as a result
        //         of the collision. The magnitude of the collision vector is the
        //         distance the entities are overlapping.
        virtual bool collideBox(Entity &ent, Vector2 &collisionVector);

        // Rotated Box collision detection method
        // Called by collision()
        // Uses Separating Axis Test to detect collision. 
        // The separating axis test:
        //   Two boxes are not colliding if their projections onto a line do not overlap.
        // The current entity is A the other entity is B
        // Post: returns true if collision, false otherwise
        //       Sets collisionVector if collision. The collisionVector points in the
        //         direction of force that would be applied to this entity as a result
        //         of the collision. The magnitude of the collision vector is the
        //         distance the entities are overlapping.
        virtual bool collideRotatedBox(Entity &ent, Vector2 &collisionVector);

        // Rotated Box and Circle collision detection method
        // Called by collision()
        // Uses separating axis test on edges of box and radius of circle.
        // If the circle center is outside the lines extended from the collision box
        // edges (also known as the Voronoi region) then the nearest box corner is checked
        // for collision using a distance check.
        // The nearest corner is determined from the overlap tests.
        //
        //   Voronoi0 |   | Voronoi1
        //         ---0---1---
        //            |   |
        //         ---3---2---
        //   Voronoi3 |   | Voronoi2
        //
        // Pre: This entity (entA) must be rotated box and other entity (entB) must be circle.
        // Post: Returns true if collision, false otherwise.
        //       Sets collisionVector if collision. The collisionVector points in the
        //         direction of force that would be applied to this entity as a result
        //         of the collision. The magnitude of the collision vector is the
        //         distance the entities are overlapping.
        virtual bool collideRotatedBoxCircle(Entity &ent, Vector2 &collisionVector);

        // Separating axis collision detection helper functions

        // Compute corners of rotated box, projection edges and min and max projections
        // 0---1  corner numbers
        // |   |
        // 3---2
        void computeRotatedBox();

        // Projects other box onto this edge01 and edge03.
        // Called by collideRotatedBox()
        // The current entity is A the other entity is B
        // Post: returns true if projections overlap, false otherwise
        bool projectionsOverlap(Entity &ent, Vector2 &collisionVector);

        // The box corner is checked for collision with circle using a distance check.
        // Called by collideRotatedBoxCircle()
        // Post: returns true if collision, false otherwise
        //       Sets collisionVector if collision. The collisionVector points in the
        //         direction of force that would be applied to this entity as a result
        //         of the collision. The magnitude of the collision vector is the
        //         distance the entities are overlapping.
        bool collideCornerCircle(Vector2 corner, Entity &ent, Vector2 &collisionVector);

    public:
        // Constructor
        Entity();
        // Destructor
        virtual ~Entity();

        //=====================================================================
        // Initialize Entity
        // Pre: *audioPtr = pointer to audio object.
        //      w = width of Image in pixels  (0 = use full texture width).
        //      h = height of Image in pixels (0 = use full texture height).
        //      ncols = number of columns in texture (1 to n) (0 same as 1).
        //      textures = Texture images.
        //=====================================================================
        virtual bool initialize(Audio *audioPtr, int w, int h, int ncols,
            CanvasBitmap^ textures);

        //=====================================================================
        // Update the animation. frameTime is used to regulate the speed.
        //=====================================================================
        virtual void update(float frameTime);

        //=====================================================================
        // Activate Entity. Only active etities may collide.
        //=====================================================================
        virtual void activate()
        {
            active = true;
        }

        //=====================================================================
        // ai (artificial intelligence)
        // Typically called once per frame.
        // Performs AI calculations.
        // Pre: frameTime = Elapsed time of the prevous frame in seconds.
        //      ent = The other Entity. 
        //      Interaction with multiple entities should be handled with multiple
        //      calls to this function.
        //=====================================================================
        virtual void ai(float frameTime, Entity &ent);

        ////////////////////////////////////////
        //           Get functions            //
        ////////////////////////////////////////

        // Return radius of collision circle.
        virtual float getRadius() const { return scaledRadius; }

        // Return RECT structure used for BOX and ROTATED_BOX collision detection.
        virtual const RECT& getEdge() const { return scaledEdge; }

        // Return corner c of ROTATED_BOX
        virtual const Vector2* getCorner(UINT c) const
        {
            if (c >= 4)
                c = 0;
            return &corners[c];
        }

        // Return corners array
        virtual Vector2* getCorners() { return corners; }

        // Return projection overlaps used in rotated box collision
        virtual float getOverlap01() { return overlap01; }
        virtual float getOverlap03() { return overlap03; }
        virtual float getMinOverlap() { return minOverlap; }

        // Return velocity vector.
        virtual const Vector2 getVelocity() const { return velocity; }

        // Returns rotation rate
        virtual float getRotationRate() { return rotationRate; }

        // Return active.
        // Only active entities may collide.
        virtual bool  getActive()   const { return active; }

        // Return intersecting. 
        // Intersecting is true if this entity is intersecting another entity.
        virtual bool getIntersecting() const { return intersecting; }

        // Return embedded
        // embedded is true if this entity is completely contained within the collision area of another entity.
        virtual bool getEmbedded() const { return embedded; }

        // Return mass.
        virtual float getMass()     const { return mass; }

        // Return bounciness
        virtual float getBounciness()   const { return bounciness; }

        // Return noBounce
        virtual bool getNoBounce()   const { return noBounce; }

        // Get cohesion
        // How attracted entities are to each other.
        // Between 0 and 1.0f where 0 = glue and 1.0f = no cohesion.
        virtual float getCohesion()
        {
            return cohesion;
        }

        // Return gravitational constant.
        virtual float getGravity()  const { return gravity; }

        // Return health;
        virtual float getHealth()   const { return health; }

        // Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT)
        virtual COLLISION_TYPE getCollisionType() { return collisionType; }

        // Return oldX
        virtual float getOldX()     const { return oldCenter.x; }

        // Return oldY
        virtual float getOldY()     const { return oldCenter.y; }

        // Return old center
        virtual Vector2 getOldCenter()  const { return oldCenter; }

        ////////////////////////////////////////
        //           Set functions            //
        ////////////////////////////////////////

        // Set radius of collision circle.
        virtual void setRadius(float r)  
        { 
            if (r > 0)
            {
                radius = r;
                scaledRadius = radius * getScale();
            }

        }

        // Set velocity.
        virtual void  setVelocity(Vector2 &v) { velocity = v; }

        // Set delta velocity. Added to velocity in update().
        virtual void  setDeltaV(Vector2 dv) { deltaV = dv; }

        // Set rotation rate, radians/second
        virtual void setRotationRate(float r) { rotationRate = r; }

        // Set active. Only active entities may collide.
        virtual void  setActive(bool a) { active = a; }

        // Set health.
        virtual void setHealth(float h) { health = h; }

        // Set mass.
        virtual void  setMass(float m) { mass = m; }

        // Set bounciness
        // 0 = no bounce, 1 = max bounce.
        virtual void setBounciness(float b)
        {
            if (b < 0)
                b = 0;
            else if (b > 1)
                b = 1;
            bounciness = b;
        }

        // Set noBounce
        // true indicates this entity does not move as a result of a collision
        virtual void setNoBounce(bool no) { noBounce = no; }

        // Set cohesion
        // How attracted entities are to each other.
        // Between 0 and 1.0f where 0 = glue and 1.0f = no cohesion.
        virtual void setCohesion(float c)
        {
            if (c >= 0 && c <= 1.0f)
                cohesion = c;
        }

        // Set gravitational constant. Default is 6.67428e-11
        virtual void  setGravity(float g) { gravity = g; }

        // Use this function to set the scale property of an Entity.
        virtual void setScale(float s) override
        {
            Image::setScale(s);
            // Adjust scale of collision boundaries
            scaledRadius = radius * s;  // For Circle collision
            scaledEdge.left = edge.left * s;    // For Box and Rotated Box collision
            scaledEdge.top = edge.top * s;
            scaledEdge.right = edge.right * s;
            scaledEdge.bottom = edge.bottom * s;
        }

        // Set radius of collision circle.
        virtual void setCollisionRadius(float r) 
        { 
            radius = r; 
            scaledRadius = r * getScale();
        }

        // Set collision type (NONE, CIRCLE, BOX, ROTATED_BOX, PIXEL_PERFECT)
        // PIXEL_PERFECT is much slower than the other methods. It calls CIRCLE collision
        // to set the collision vector.
        virtual void setCollisionType(COLLISION_TYPE ctype)
        {
            collisionType = ctype;
        }

        // Set embedded
        // embedded is true if this entity is completely contained within the collision area of another entity.
        virtual void setEmbedded(bool e) { embedded = e; }

        // Set RECT structure used for BOX and ROTATED_BOX collision detection.
        // edge specifies the collision box relative to the center of the entity.
        // left and top are typically negative numbers.
        virtual void setEdge(RECT e) 
        { 
            edge.left = e.left;
            edge.top = e.top;
            edge.right = e.right;
            edge.bottom = e.bottom;
            scaledEdge.left = e.left * getScale();
            scaledEdge.top = e.top * getScale();
            scaledEdge.right = e.right * getScale();
            scaledEdge.bottom = e.bottom * getScale();
        }

        // Set rotatedBoxReady. Set to false to force recalculation.
        virtual void setRotatedBoxReady(bool r) { rotatedBoxReady = r; }

        ////////////////////////////////////////
        //          Move functions            //
        ////////////////////////////////////////

        //=====================================================================
        // Rotate using specified rotation rate
        // Pre: frameTime = Elapsed time in seconds
        //      rotateRate = radians/second, + clockwise, - counterclockwise
        // Post: Current rotation rate is unchanged (no change in momentum)
        //=====================================================================
        virtual void rotate(float frameTime, float rotateRate)
        {
            angle += frameTime * rotateRate;   // apply rotation
        }

        //=====================================================================
        // Rotate using current rotationRate
        // Pre: frameTime = Elapsed time in seconds
        // Post: Current rotation rate is unchanged (no change in momentum)
        // Call this from the game's update method to give the Entity momentum.
        //=====================================================================
        virtual void rotate(float frameTime)
        {
            angle += frameTime * rotationRate;   // apply rotation
        }

        //=====================================================================
        // Change rotationRate (angular momentum)
        // Pre: frameTime = Elapsed time in seconds
        //      rotateRate = radians/second, + clockwise, - counterclockwise
        // Post: Current rotation rate is changed (change in momentum)
        // The Entity does not turn until the rotate method is called.
        //=====================================================================
        virtual void turn(float frameTime, float rotateRate)
        {
            rotationRate += frameTime * rotateRate;
        }

        //=====================================================================
        // Angle to target
        // Pre:  x,y = Target center
        // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
        //=====================================================================
        virtual float angleToTarget(float x, float y);

        //=====================================================================
        // Angle to target
        // Pre:  target = Target center
        // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
        //=====================================================================
        virtual float angleToTarget(Vector2 target)
        {
            return angleToTarget(target.x, target.y);
        }

        //=====================================================================
        // Aim at target
        // Pre: frameTime = Elapsed time in seconds
        //      rotationRate = radians/second
        //      x,y = Target center
        // Post: Current rotation rate is unchanged (no change in momentum)
        // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
        //=====================================================================
        virtual float aimAtTarget(float frameTime, float rotationRate, float x, float y);

        //=====================================================================
        // Aim at target
        // Pre: frameTime = Elapsed time in seconds
        //      rotationRate = radians/second
        //      target = Target center
        // Post: Current rotation rate is unchanged (no change in momentum)
        // Returns: Angle to target in radians. + target is clockwise, - target is counterclockwise
        //=====================================================================
        virtual float aimAtTarget(float frameTime, float rotationRate, Vector2 target)
        {
            return aimAtTarget(frameTime, rotationRate, target.x, target.y);
        }

        //=====================================================================
        // Move in X direction using current velocity
        // Pre: frameTime = Elapsed time in seconds
        // Post: Current velocity vector is unchanged (no change in momentum)
        //=====================================================================
        virtual void moveX(float frameTime)
        {
            center.x += frameTime * velocity.x;     // move X 
        }

        //=====================================================================
        // Move in X direction
        // Pre: frameTime = Elapsed time in seconds
        //      speed = Pixels/second (+ right, - left)
        // Post: Current velocity vector is unchanged (no change in momentum)
        //=====================================================================
        virtual void moveX(float frameTime, float speed)
        {
            center.x += frameTime * speed;          // move X 
        }

        //=====================================================================
        // Move in Y direction using current velocity
        // Pre: frameTime = Elapsed time in seconds
        // Post: Current velocity vector is unchanged (no change in momentum)
        //=====================================================================
        virtual void moveY(float frameTime)
        {
            center.y += frameTime * velocity.y;     // move Y
        }

        //=====================================================================
        // Move in Y direction
        // Pre: frameTime = Elapsed time in seconds
        //      speed = Pixels/second (+ down, - up)
        // Post: Current velocity vector is unchanged (no change in momentum)
        //=====================================================================
        virtual void moveY(float frameTime, float speed)
        {
            center.y += frameTime * speed;          // move Y
        }

        //=====================================================================
        // Move in X,Y direction using current velocity
        // Pre: frameTime = Elapsed time in seconds
        // Post: Current velocity vector is unchanged (no change in momentum)
        // Call this from the game's update method to give the Entity momentum.
        //=====================================================================
        virtual void move(float frameTime)
        {
            moveX(frameTime, velocity.x);
            moveY(frameTime, velocity.y);
        }

        //=====================================================================
        // Move in X,Y direction using specified velocity
        // Pre: frameTime = Elapsed time in seconds
        //      vel = Velocity vector
        // Post: Current velocity vector is unchanged (no change in momentum)
        //=====================================================================
        virtual void move(float frameTime, Vector2 vel)
        {
            moveX(frameTime, vel.x);
            moveY(frameTime, vel.y);
        }

        //=====================================================================
        // MoveTo X,Y at speed. Ignores current velocity.
        // Pre: frameTime = Elapsed time in seconds
        //      speed = Movement speed in pixels/second
        //      x,y = Destination location (center)
        // Post: Current velocity vector is unchanged (no change in momentum)
        //=====================================================================
        virtual void moveTo(float frameTime, float speed, 
            float destX, float destY)
        {
            float distX = destX - center.x;     // destination X - current X
            float distY = destY - center.y;     // destination Y - current Y
            float distance = sqrt(distX*distX + distY*distY);
            float moveTime = distance / speed;  // time required to move
            moveX(frameTime, distX / moveTime); // move in X direction
            moveY(frameTime, distY / moveTime); // move in Y direction
        }

        //=====================================================================
        // Change the velocity
        // Pre: frameTime = Elapsed time in seconds
        //      speed = Movement speed in pixels/second
        //      vel = Velocity vector
        // Post: Current velocity vector is changed (change in momentum)
        //=====================================================================
        virtual void accelerate(float frameTime, float speed, Vector2 deltaV)
        {
            velocity += deltaV * speed * frameTime;
        }

        ////////////////////////////////////////
        //         Other functions            //
        ////////////////////////////////////////

        //=====================================================================
        // Is this Entity outside the specified rectangle
        // Post: returns true if outside rect, false otherwise
        //=====================================================================
        virtual bool outsideRect(RECT rect);

        //=====================================================================
        // Does this entity intersect with ent?
        // Each entity must use a single collision type. Complex shapes that
        // require multiple collision types may be done by treating each part
        // as a separate entity.
        // Pre:  &ent = The other Entity.
        //       &collisionVector = Set by this function.
        // Post: Returns true if the entities are intersecting.
        //       Sets the collision vector if intersecting.
        //       Sets intersecting true if this entity is intersecting another
        //       entity.
        //   The collisionVector points in the direction of force that should
        //   be applied to this entity as a result of the collision. (e.g. If
        //   this entity is a ball that is dropped onto a box, the collision
        //   vector would point up (-Y).
        //=====================================================================
        virtual bool intersects(Entity &ent, Vector2 &collisionVector);

        //=====================================================================
        // Perform collision detection between this entity and the other Entity.
        // Calls intersects function.
        // Each entity must use a single collision type. Complex shapes that require
        // multiple collision types may be done by treating each part as a separate
        // entity or by using PIXEL_PERFECT collision.  PIXEL_PERFECT collision detection
        // is slower and returns a less accurate collision vector so it is not good for
        // realistic physics. 
        // Typically called once per frame.
        // The collision types: CIRCLE, BOX, ROTATED_BOX or PIXEL_PERFECT.
        // Pre:  &ent = The other Entity.
        //       &collisionVector = Set by this function.
        // Post: Returns true if collision, false otherwise. A collision is occurring if
        //       the entities are intersecting and moving toward each other. If the
        //       entities are intersecting and moving apart it is assumed they are
        //       intersecting as the result of a previous collision; false is returned.
        //       Sets intersecting true if collision.
        //       Sets collisionVector if collision. The collisionVector points in the
        //         direction of force that would be applied to this entity as a result
        //         of the collision. (e.g. If this entity is a ball that is dropped
        //         onto a box, the collision vector would point up (-Y).
        //=====================================================================
        virtual bool collidesWith(Entity &ent, Vector2 &collisionVector);

        //=====================================================================
        // damage
        // This entity has been damaged by a weapon.
        // Override this function in the inheriting class.
        //=====================================================================
        virtual void damage(int weapon)
        {}

        //=====================================================================
        // Entity bounces after collision with another entity
        // Pre: ent is the other entity.
        //      The collisionVector has been set with direction of force that should be
        //      applied to this entity. The magnitude of collisionVector is the
        //      distance the two entities are overlapping.
        // Post: The deltaV of the current entity is set. The deltaV will be applied
        //      to this entity in the update function.
        //      This entity is moved in the direction of the collison vector by a
        //      distance proportional to the mass ratio with the other entity.
        //=====================================================================
        void bounceOffEntity(Vector2 &collisionVector, Entity &ent);

        //=====================================================================
        // Entity bounces off walls of rectangle defined by width & height.
        // Pre: width = Screen width.
        //      height = Screen height.
        // Post: The velocity of this entity is reversed according to which
        //      wall is hit. The position of this entity is moved to
        //      the edge of the rectangle.
        //=====================================================================
        void bounceOffWalls(int width, int height);

        //=====================================================================
        // Move this entity to its oldX, oldY
        //=====================================================================
        void moveToOldXY()
        {
            setX(oldCenter.x);
            setY(oldCenter.y);
        }

        //=====================================================================
        // Force of gravity on this entity from other entity
        // Adds the gravitational force to the velocity vector of this entity
        // force = GRAVITY * m1 * m2 / r*r
        //                    2              2
        //  r*r  =   (Ax - Bx)   +  (Ay - By)
        //=====================================================================
        void gravityForce(Entity *other, float frameTime);

        //=====================================================================
        // Normalize the vector.
        static void Vector2Normalize(Vector2 *v)
        {
            *v = Windows::Foundation::Numerics::normalize(*v);
            // If normalize function returns NaN (Not a Number) for X or Y
            // component in vector.
            if (v->x != v->x)   // If v.x is Nan
                v->x = 0;       // Force to 0
            if (v->y != v->y)   // If v.y is Nan
               v->y = 0;        // Force to 0
        }

        //=====================================================================
        // Return Dot product of 2 vectors.
        static float Vector2Dot(const Vector2 *v1, const Vector2 *v2)
        {
            return Windows::Foundation::Numerics::dot(*v1, *v2);
        }

		// Return length of vector v.
		static float Vector2Length(const Vector2 *v)
		{
			return Windows::Foundation::Numerics::length(*v);
		}

		float3x2 *m;

		// Transform vector v with matrix m.
		static float2 Vector2Transform(float2 *v, float3x2 *m)
		{	
			return Windows::Foundation::Numerics::transform(*v, *m);
		}

    };
}

