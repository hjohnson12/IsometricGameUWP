#pragma once
// Programming 2D Games
// Copyright (c) 2017 by: 
// Charles Kelly
// TextWin2D.cpp v17.3.8
// Win2D text output.

//#define WIN32_LEAN_AND_MEAN
#include <string>
#include "Common.h"
using namespace Microsoft::Graphics::Canvas;
using namespace Windows::UI::Text;

namespace GameEngine2
{
    const float DEFAULT_FONT_SIZE = 24.0f;
    const wchar_t DEFAULT_FONT_FAMILY[] = L"Arial";
    const FontStyle DEFAULT_FONT_STYLE = FontStyle::Normal;

    class TextWin2D;

    class TextWin2D
    {
    private:
        TEXT_COLOR textColor;         // Text color in Alpha, Red, Green, Blue.
        //RECT        fontRect;       // text rectangle
        // Matrix to rotate the text.
        Windows::Foundation::Numerics::float3x2 *matrix;
        float       angle;          // rotation angle of text in radians
        Platform::String^ fontFamily;
        float fontSize;
        FontStyle fontStyle;    // Assign from FontStyle:: 
        FontWeight fontWeight;  // Assign from FontWeights::


    public:
        //=====================================================================
        // Constructor (sprite text)
        //=====================================================================
        TextWin2D();

        //=====================================================================
        // Print text at x,y. Six different parameter configurations are
        //      available to choose from.
        // pre: ^args = Display canvas.
        //      &str = wstring to display.
        //      x, y = Screen location.
        //      size = Font size.
        //      color = Color of text. TEXT_COLORS:: will provide a list of
        //              predefined colors. TEXT_COLORS::Transparent is the
        //              default. Using transparent as the text color will
        //              result in the textColor property being used as the text
        //              color. Use the setFontColor method to set textColor.  
        //      style = Style of text from FontStyle::. Default is Normal.
        //      weight = Weights of text from FontWeights::. Default is Normal.
        //      radians = Rotation angle of text in radians. Default is 0.0f. 
        // Call from draw method after creation of CanvasSpriteBatch^.
        //=====================================================================
        virtual void print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
            const std::wstring &str,
            int x, int y,
            float size,
            TEXT_COLOR color,
            FontStyle style,
            FontWeight weight,
            float radians
        );

        virtual void print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
            const std::wstring &str,
            int x, int y)
        {
            print(args, str, x, y, DEFAULT_FONT_SIZE, TEXT_COLORS::Transparent,
                FontStyle::Normal, FontWeights::Normal, 0.0f);
        }

        virtual void print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
            const std::wstring &str,
            int x, int y,
            float size)
        {
            print(args, str, x, y, size, TEXT_COLORS::Transparent,
                FontStyle::Normal, FontWeights::Normal, 0.0f);
        }

        virtual void print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
            const std::wstring &str,
            int x, int y,
            float size,
            TEXT_COLOR color )
        {
            print(args, str, x, y, size, color,
                FontStyle::Normal, FontWeights::Normal, 0.0f);
        }

        virtual void print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
            const std::wstring &str,
            int x, int y,
            float size,
            TEXT_COLOR color,
            FontStyle style )
        {
            print(args, str, x, y, size, color, style,
                FontWeights::Normal, 0.0f);
        }

        virtual void print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
            const std::wstring &str,
            int x, int y,
            float size,
            TEXT_COLOR color,
            FontStyle style,
            FontWeight weight )
        {
            print(args, str, x, y, size, color, style, weight, 0.0f);
        }


        //=====================================================================
        // Print text inside rectangle.
        // pre: ^args = Display canvas.
        //      &str = wstring to display.
        //      &rect = Rectangular region.
        //      size = Font size.
        //      color = Color of text. TEXT_COLORS:: will provide a list of
        //              predefined colors. TEXT_COLORS::Transparent is the
        //              default. Using transparent as the text color will
        //              result in the textColor property being used as the text
        //              color. Use the setFontColor method to set textColor.  
        //      style = Style of text from FontStyle::. Default is Normal.
        //      weight = Weights of text from FontWeights::. Default is Normal.
        // Call from draw method after creation of CanvasSpriteBatch^.
        //=====================================================================
        virtual void print(UI::Xaml::CanvasAnimatedDrawEventArgs^ args,
            const std::wstring &str,
            Windows::Foundation::Rect &rect,
            float size,
            // Transparent is used as a flag to use textColor as the color.
            TEXT_COLOR color = TEXT_COLORS::Transparent,
            FontStyle style = FontStyle::Normal,
            FontWeight weight = FontWeights::Normal,
            float radians = 0.0f
        );

        //=====================================================================
        // Determines width and height of string in pixels for current format.
        // Call from draw method after creation of CanvasSpriteBatch object.
        // Pre: &str = Text to measure.
        //      &width = Width in pixels returned.
        //      &height = Height in pixels returned.
        //      format = Text format specifier.
        //      ^args = Canvas draw event arguments.
        //=====================================================================
        virtual void TextWin2D::getWidthHeight(const std::wstring &str,
            UINT &width, UINT &height, UINT format,
            UI::Xaml::CanvasAnimatedDrawEventArgs^ args
        );

        //=====================================================================
        // Return rotation angle of text in degrees.
        //=====================================================================
        virtual float getDegrees() 
        { 
            return angle*(180.0f / (float)PI); 
        }

        //=====================================================================
        // Return rotation angle of text in radians.
        //=====================================================================
        virtual float getRadians() 
        { 
            return angle; 
        }

        //=====================================================================
        // Returns font color.
        //=====================================================================
        virtual TEXT_COLOR getFontColor() 
        { 
            return textColor;
        }

		//=====================================================================
        // Set rotation angle of text in degrees.
        // 0 degrees is up. Angles progress clockwise.
        // All items displayed in the current SpriteBatch will be affected!!!!
        //=====================================================================
        virtual void setDegrees(float deg) 
        { 
            angle = deg*((float)PI / 180.0f); 
        }

        //=====================================================================
        // Set rotation angle of text in radians.
        // 0 radians is up. Angles progress clockwise.
        // All items displayed in the current SpriteBatch will be affected!!!!
        //=====================================================================
        virtual void setRadians(float rad) 
        { 
            angle = rad; 
        }

        //=====================================================================
        // Set the font color. 
        // TEXT_COLORS:: will display a list of predefined colors.
        //=====================================================================
        virtual void setFontColor(TEXT_COLOR color)
        {
            textColor = color;
        }
    };
}

