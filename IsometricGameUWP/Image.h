#pragma once
//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// image.h v17.1.21
//=============================================================================

#include <D2d1.h>
#include "Utilities.h"
#include "Common.h"
using namespace Microsoft::Graphics::Canvas;

namespace GameEngine2
{
    struct RECT {
        float left;
        float top;
        float right;
        float bottom;
    };

    class Image;

    class Image
    {
        // Image properties
    private:
        // Use setScale to change the scale of Entities that collide.
        float   scale;          // <1 smaller, >1 bigger.
    protected:
        int     width;          // Width of image in pixels.
        int     height;         // Height of image in pixels.
        Vector2 center;         // X,Y screen location of image center.
        float   angle;          // Rotation angle in radians.
        // The texture images, which normally contains multiple images,
        // used to draw this image.
        Microsoft::Graphics::Canvas::CanvasBitmap^ textures;
        Rect    rect;           // Used to select one image from texture sheet.
        bool    flipHorizontal; // true to flip sprite horizontally (mirror)
        bool    flipVertical;   // true to flip sprite vertically
        COLOR_RGBA_F colorFilter; // applied as a color filter (use WHITE for no change)
        int     cols;           // number of cols (1 to n) in multi-frame sprite
        int     startFrame;     // first frame of current animation
        int     endFrame;       // end frame of current animation
        int     currentFrame;   // current frame of animation
        float   frameDelay;     // how long between frames of animation
        float   animTimer;      // animation timer
        bool    loop;           // true to loop frames
        bool    visible;        // true when visible
        bool    initialized;    // true when successfully initialized
        bool    animComplete;   // true when loop is false and endFrame has finished displaying
        Rect    textureRect;    // The current texture to display.
        CanvasDrawingSession^ ds;

    public:
        // Constructor
        Image();
        // Destructor
        virtual ~Image();

        ////////////////////////////////////////
        //           Get functions            //
        ////////////////////////////////////////

        // Return visible parameter.
        virtual bool  getVisible() { return visible; }

        // Return X position (center of image).
        virtual float getX() { return center.x; }

        // Return Y position (center of image).
        virtual float getY() { return center.y; }

        // Return pointer to center of image.
        virtual Vector2* getCenter() { return &center; }

        // Return scale factor.
        virtual float getScale() { return scale; }

        // Return width.
        virtual int   getWidth() { return width; }

        // Return height.
        virtual int   getHeight() { return height; }

        // Return rotation angle in degrees.
        virtual float getDegrees() { return angle*(180.0f / (float)PI); }

        // Return rotation angle in radians.
        virtual float getRadians() { return angle; }

        // Return delay between frames of animation.
        virtual float getFrameDelay() { return frameDelay; }

        // Return number of starting frame.
        virtual int   getStartFrame() { return startFrame; }

        // Return number of ending frame.
        virtual int   getEndFrame() { return endFrame; }

        // Return number of current frame.
        virtual int   getCurrentFrame() { return currentFrame; }

        // Return RECT structure of Image.
        virtual Rect  getRect() { return rect; }

        // Return state of animation complete.
        virtual bool  getAnimationComplete() { return animComplete; }

        // Return colorFilter.
        virtual COLOR_RGBA_F getColorFilter() { return colorFilter; }

        // Return flip image horizontally (mirror).
        virtual bool getFlipHorizontal() { return flipHorizontal; }

        // Return flip image vertically.
        virtual bool getFlipVertical() { return flipVertical; }

        ////////////////////////////////////////
        //           Set functions            //
        ////////////////////////////////////////

        // Set X location (center of image).
        virtual void setX(float newX) { center.x = newX; }

        // Set Y location (center of image).
        virtual void setY(float newY) { center.y = newY; }

        // Set center of image.
        virtual void setCenter(Vector2 c) { center = c; }

        // Set scale.
        virtual void setScale(float s) { scale = s; }

        // Set width.
        virtual void setWidth(int w) { width = w; }

        // Set height.
        virtual void setHeight(int h) { height = h; }

        // Set rotation angle in degrees.
        // 0 degrees is up. Angles progress clockwise.
        virtual void setDegrees(float deg) { angle = deg*((float)PI / 180.0f); }

        // Set rotation angle in radians.
        // 0 radians is up. Angles progress clockwise.
        virtual void setRadians(float rad) { angle = rad; }

        // Set visible.
        virtual void setVisible(bool v) { visible = v; }

        // Set delay (seconds) between frames of animation.
        virtual void setFrameDelay(float d) { frameDelay = d; }

        // Set starting and ending frames of animation.
        virtual void setFrames(int s, int e) { startFrame = s; endFrame = e; }

        // Select the individual texture from the multiple texture sheet to draw
        // the current frame of animation.
        // Pre: c = Texture number. 0 is the top left texture in the sprite sheet.
        // Numbers advance from left to right and top to bottom.
        virtual void setCurrentFrame(int c);

        // Set rect to draw the current frame.
        virtual void setRect();

        // Set rect to n.
        virtual void setRectN(RECT n) 
        { 
            rect = Rect(n.left, n.top, n.left + n.right, n.top + n.bottom);
        }

        // Set animation loop, true = loop.
        virtual void setLoop(bool lp) { loop = lp; }

        // Set animation complete, true = complete.
        virtual void setAnimationComplete(bool a) { animComplete = a; };

        // Set color filter. (use WHITE for no change)
        virtual void setColorFilter(COLOR_RGBA_F color) { colorFilter = color; }

        // Set Animation Timer in seconds.
        virtual void setAnimTimer(float t) { animTimer = t; };

        // Flip image horizontally (mirror).
        virtual void setFlipHorizontal(bool flip) { flipHorizontal = flip; }

        // Flip image vertically.
        virtual void setFlipVertical(bool flip) { flipVertical = flip; }

        ////////////////////////////////////////
        //         Other functions            //
        ////////////////////////////////////////

        // Initialize Image
        // Pre: w = width of Image in pixels  (0 = use full texture width).
        //      h = height of Image in pixels (0 = use full texture height).
        //      ncols = number of columns in texture (1 to n) (0 same as 1).
        //      textures = Texture images.
        virtual bool initialize(int w, int h, int ncols, 
            CanvasBitmap^ textures);

        //=====================================================================
        // Draw spriteN from sprite sheet using color as filter. 
        // spriteN is the number of the sprite.
        // 0 is the top left sprite in the sprite sheet. Numbers advance from
        // left to right by rows.
        //=====================================================================
        virtual void draw(CanvasSpriteBatch^ spriteBatch,
            COLOR_RGBA_F color, UINT spriteN);

        //=====================================================================
        // Draw sprite 0 from sprite sheet using color as filter. 
        // 0 is the top left sprite in the sprite sheet. Numbers advance from
        // left to right by rows.
        //=====================================================================
        virtual void draw(CanvasSpriteBatch^ spriteBatch,
            COLOR_RGBA_F color = COLORS::WHITE)
        {
            draw(spriteBatch, color, 0);
        }

        //=====================================================================
        // Draw spriteN from sprite sheet with no color filter. 
        // spriteN is the number of the sprite.
        // 0 is the top left sprite in the sprite sheet. Numbers advance from
        // left to right by rows.
        //=====================================================================
        virtual void draw(CanvasSpriteBatch^ spriteBatch, UINT spriteN)
        {
            draw(spriteBatch, COLORS::WHITE, spriteN);
        }

        // Draw this image using the specified SpriteData.
        // The current SpriteData.rect is used to select the texture.
        // textureN is number of texture in textureManager 
        //virtual void draw(D2D1::ColorF color = WHITE, UINT textureN = 0);

        // Update the animation. deltaTime is used to regulate the speed.
        virtual void update(float deltaTime);
    };
}

