#pragma once
//=========================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// Game.xaml.h  v17.1.21
//=========================================================================

#include "MainPage.g.h"
#include "Utilities.h"
#include "Game.h"
#include <vector>

namespace GameEngine2
{
    using namespace Microsoft::Graphics::Canvas::UI;

    /// <summary>
    /// The main class of the game.
    /// </summary>

    //-------------------------------------------------------------------------
    // The Game class
    // A sealed class may not be used as a base class.
    public ref class MainPage sealed 
    {
    public:
        MainPage();

    private:
        //---------------------------------------------------------------------
        // Create dynamic game resources.
        void CreateResources(Xaml::CanvasAnimatedControl^ sender,
                             CanvasCreateResourcesEventArgs^ args);

        //---------------------------------------------------------------------
        // Update the game items.
        void Update(Xaml::CanvasAnimatedControl^ sender,
                    Xaml::CanvasAnimatedUpdateEventArgs^ args);

        //---------------------------------------------------------------------
        // Draw one frame of the game.
        void Draw(Xaml::CanvasAnimatedControl^ sender, 
                  Xaml::CanvasAnimatedDrawEventArgs^ args);

        //---------------------------------------------------------------------
        void OnKeyDown(CoreWindow^ sender, KeyEventArgs^ args);

        //---------------------------------------------------------------------
        void OnKeyUp(CoreWindow^ sender, KeyEventArgs^ args);

        //---------------------------------------------------------------------
        void OnCharReceived(CoreWindow^ sender, CharacterReceivedEventArgs^ args);

        //---------------------------------------------------------------------
        void OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args);

		//---------------------------------------------------------------------
        void OnPointerWheelChanged(CoreWindow^ sender, PointerEventArgs^ args);

		//---------------------------------------------------------------------
		void OnPointerMoved(CoreWindow ^ sender, PointerEventArgs ^ args);

		//---------------------------------------------------------------------
		void OnGamepadAdded(Platform::Object ^sender,
			Windows::Gaming::Input::Gamepad ^args);

        //---------------------------------------------------------------------
        // The game object.
        Game game;
    };
}
