//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// image.cpp v17.1.21
//=============================================================================

#include "pch.h"
#include "Image.h"
using namespace GameEngine2;

//using namespace Platform;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::WRL;
using namespace Windows::Foundation::Numerics;
//using namespace Microsoft::Graphics::Canvas::Effects;

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
GameEngine2::Image::Image()
{
    initialized = false;            // set true when successfully initialized
    width = 2;
    height = 2;
    center.x = 0.0;
    center.y = 0.0;
    scale = 1.0;
    angle = 0.0;
    // Used to select one frame from multi-frame image.
    rect = Rect(0.0f, 0.0f, 
    static_cast<float>(width), static_cast<float>(height));
    flipHorizontal = false;
    flipVertical = false;
    cols = 1;
    startFrame = 0;
    endFrame = 0;
    currentFrame = 0;
    frameDelay = 1.0;               // default to 1 second per frame of animation
    animTimer = 0.0;
    visible = true;                 // the image is visible
    loop = true;                    // loop frames
    animComplete = false;
    colorFilter = COLORS::WHITE;            // WHITE for no change
}

// Destructor
GameEngine2::Image::~Image()
{}

//-----------------------------------------------------------------------------
// Select the individual texture from the multiple texture sheet to draw
// the current sprite of animation.
//-----------------------------------------------------------------------------
void GameEngine2::Image::setCurrentFrame(int c)
{
    if (c >= 0)
    {
        currentFrame = c;
        animComplete = false;
        setRect();          // Set rect to desired texture
    }
}

//-----------------------------------------------------------------------------
// Set rect to draw currentSprite of image.
//-----------------------------------------------------------------------------
inline void GameEngine2::Image::setRect()
{
    // Rect(left, top, width, height)
    rect = Rect(
        (currentFrame % cols) * static_cast<float>(width),
        (currentFrame / cols) * static_cast<float>(height),
        static_cast<float>(width), static_cast<float>(height));
}

//-----------------------------------------------------------------------------
// Initialize Image
// Pre: width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//      ^textures = pointer to textures.
// Initialize the Image.
// Post: returns true if successful, false if failed
//-----------------------------------------------------------------------------
bool GameEngine2::Image::initialize(int w, int h, int ncols,
    CanvasBitmap^ textureImage)
{
    try {
        if (w == 0)
            w = textureImage->SizeInPixels.Width;   // Use full width.
        width = w;
        if (h == 0)
            h = textureImage->SizeInPixels.Height;  // Use full height.
        height = h;
        cols = ncols;
        if (cols == 0)
            cols = 1;       // If 0 cols use 1.

        textures = textureImage;    // This image's textures
        setCurrentFrame(0); // Default to start at frame 0.
    }
    catch(...) { return false; }
    initialized = true;     // Successfully initialized.
    return true;
}

//-----------------------------------------------------------------------------
// Draw the image using color as filter.
// The color parameter is optional, WHITE is assigned as default in image.h
// The textureN parameter is optional, 0 is default.
// Pre : spriteBegin() is called
// Post: spriteEnd() is called
//-----------------------------------------------------------------------------
void GameEngine2::Image::draw(CanvasSpriteBatch^ spriteBatch, 
    COLOR_RGBA_F color, UINT spriteN)
{
    if (!visible)
        return;
    float2 origin;
    origin.x = width / 2.0f;
    origin.y = height / 2.0f;
    float4 tint(color.r, color.g, color.b, color.a);
    Microsoft::Graphics::Canvas::CanvasSpriteFlip spriteFlip;
    if (flipHorizontal && !flipVertical)        // if flip sprite horizontal
        spriteFlip = CanvasSpriteFlip::Horizontal;
    else if (!flipHorizontal && flipVertical)   // if flip sprite vertical
        spriteFlip = CanvasSpriteFlip::Vertical;
    else if (flipHorizontal && flipVertical)    // if flip sprite horizontal and vertical
        spriteFlip = CanvasSpriteFlip::Both;
    else                                        // don't flip
        spriteFlip = CanvasSpriteFlip::None;

    spriteBatch->DrawFromSpriteSheet(
        textures,
        center,
        rect,
        tint,
        origin,
        angle,
        float2(scale, scale),
        spriteFlip);
}

// Draw Image using color as filter. Default color is WHITE.
//void draw(COLOR_ARGB color = WHITE) { draw(color, 0); }

// Draw Image using default color filter.
// textureN is number of texture in textureManager 
//void draw(UINT textureN) { draw(WHITE, textureN); }

// Draw this image using the specified SpriteData.
// The current SpriteData.rect is used to select the texture.
// textureN is number of texture in textureManager 
//void draw(COLOR_ARGB color = WHITE, UINT textureN = 0);

//-----------------------------------------------------------------------------
// update
// typically called once per frame.
// deltaTime is used to regulate the speed of movement and animation.
//-----------------------------------------------------------------------------
void GameEngine2::Image::update(float deltaTime)
{
    if (endFrame - startFrame > 0)          // if animated sprite
    {
        animTimer += deltaTime;             // total elapsed time
        if (animTimer > frameDelay)
        {
            animTimer -= frameDelay;
            currentFrame++;
            if (currentFrame < startFrame || currentFrame > endFrame)
            {
                if (loop == true)            // if looping animation
                    currentFrame = startFrame;
                else                        // not looping animation
                {
                    currentFrame = endFrame;
                    animComplete = true;    // animation complete
                }
            }
            setRect();                      // set spriteData.rect
        }
    }
}
