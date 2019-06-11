// Programming 2D Games
// Copyright (c) 2017 by: 
// Charles Kelly
// TextWin2D.cpp v17.3.8
// Win2D text output.

#include "pch.h"
#include "TextWin2D.h"

//=============================================================================
// Default constructor
//=============================================================================
GameEngine2::TextWin2D::TextWin2D()
{
    textColor = TEXT_COLORS::White;   // Default to white font

    // set font position
    //fontRect.top = 0;
    //fontRect.left = 0;
    //fontRect.right = GAME_WIDTH;
    //fontRect.bottom = GAME_HEIGHT;
    //dxFont = NULL;
    fontSize = DEFAULT_FONT_SIZE;
    fontStyle = DEFAULT_FONT_STYLE;
    fontWeight = FontWeights::Normal;
    fontFamily = ref new Platform::String(DEFAULT_FONT_FAMILY);
    angle  = 0;
}


//=============================================================================
// Print text at x,y 
// pre: &str = wstring to display.
//      x, y = Screen location.
//      ^args = Display canvas.
//      color = Color of text. TEXT_COLORS:: will provide a list of
//              predefined colors. TEXT_COLORS::Transparent is the
//              default. Using transparent as the text color will
//              result in the textColor property being used as the text
//              color. Use the setFontColor method to set textColor.  
//      style = Style of text from FontStyle::. Default is Normal.
//      weight = Weights of text from FontWeights::. Default is Normal.
//      radians = Rotation angle of text in radians. Default is 0.0f. 
// Call from draw method after creation of CanvasSpriteBatch^.
//=============================================================================
void GameEngine2::TextWin2D::print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
    const std::wstring &str,
    int x, int y,
    float size,
    TEXT_COLOR color,
    FontStyle style,
    FontWeight weight,
    float radians )
{
    if (size <= 0)      // Prevent illegal font size.
        size = 1.0f;

    Windows::Foundation::Numerics::float3x2 matrix2;
    matrix2 = Windows::Foundation::Numerics::make_float3x2_rotation(radians);
    args->DrawingSession->Transform = matrix2;

    Microsoft::Graphics::Canvas::Text::CanvasTextFormat^ format =
        ref new Microsoft::Graphics::Canvas::Text::CanvasTextFormat;
    format->FontFamily = fontFamily;
    format->FontSize = size;
    format->FontStyle = style;
    format->FontWeight = weight;
    Platform::String^ stringRef = ref new Platform::String(str.c_str());
    // If color parameter is set to Transparent then use textColor property.
    if( color.A == TEXT_COLORS::Transparent.A &&
        color.R == TEXT_COLORS::Transparent.R &&
        color.G == TEXT_COLORS::Transparent.G &&
        color.B == TEXT_COLORS::Transparent.B )
        args->DrawingSession->DrawText(stringRef, float(x), float(y), 
            textColor, format);
    else    // Use the color property specified in this function call.
        args->DrawingSession->DrawText(stringRef, float(x), float(y), color,
            format);
}


//=============================================================================
// Print text inside rectangle.
// pre: &str = wstring to display.
//      &rect = Rectangular region.
//      ^args = Display canvas.
//      color = Color of text. TEXT_COLORS:: will provide a list of
//              predefined colors. TEXT_COLORS::Transparent is the
//              default. Using transparent as the text color will
//              result in the textColor property being used as the text
//              color. Use the setFontColor method to set textColor.  
//      style = Style of text from FontStyle::. Default is Normal.
//      weight = Weights of text from FontWeights::. Default is Normal.
// Call from draw method after creation of CanvasSpriteBatch^.
//=============================================================================
void GameEngine2::TextWin2D::print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
    const std::wstring &str,
    Windows::Foundation::Rect &rect,
    float size,
    TEXT_COLOR color,
    FontStyle style,
    FontWeight weight,
    float radians)
{
    if (size <= 0)      // Prevent illegal font size.
        size = 1.0f;

    Windows::Foundation::Numerics::float3x2 matrix2;
    matrix2 = Windows::Foundation::Numerics::make_float3x2_rotation(radians);
    args->DrawingSession->Transform = matrix2;

    Microsoft::Graphics::Canvas::Text::CanvasTextFormat^ format =
        ref new Microsoft::Graphics::Canvas::Text::CanvasTextFormat;
    format->FontFamily = fontFamily;
    format->FontSize = size;
    format->FontStyle = style;
    format->FontWeight = weight;
    Platform::String^ stringRef = ref new Platform::String(str.c_str());
    // If color parameter is set to Transparent then use textColor property.
    if (color.A == TEXT_COLORS::Transparent.A &&
        color.R == TEXT_COLORS::Transparent.R &&
        color.G == TEXT_COLORS::Transparent.G &&
        color.B == TEXT_COLORS::Transparent.B)
        args->DrawingSession->DrawText(stringRef, rect, textColor, format);
    else    // Use the color property specified in this function call.
        args->DrawingSession->DrawText(stringRef, rect, color, format);

    return;
}

//=============================================================================
// Determines width and height of string in pixels for current point size.
//=============================================================================
void GameEngine2::TextWin2D::getWidthHeight(const std::wstring &str,
    UINT &width, UINT &height, UINT format,
    UI::Xaml::CanvasAnimatedDrawEventArgs^ args)
{
    //CanvasDrawingSession ^drawingSession = args->DrawingSession;
    //float xLoc = 100.0f;
    //float yLoc = 100.0f;
    //Text::CanvasTextFormat ^canvasFormat = ref new Text::CanvasTextFormat;
    //canvasFormat->FontSize = 30.0f;
    //canvasFormat->WordWrapping = Text::CanvasWordWrapping::NoWrap;
    //Text::CanvasTextLayout ^textLayout = ref new Text::CanvasTextLayout(
    //    drawingSession, "Hello All", canvasFormat, 0.0f, 0.0f);
    //Rect sizeRect = Rect(xLoc + textLayout->DrawBounds.X, yLoc +
    //    textLayout->DrawBounds.Y, textLayout->DrawBounds.Width,
    //    textLayout->DrawBounds.Height);
    //drawingSession->DrawRectangle(sizeRect, Windows::UI::Colors::Green, 1.0f);
    //width = UINT(sizeRect.Width);
    //height = UINT(sizeRect.Height);
}
