//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// Game.xaml.cpp  v17.1.21
//=============================================================================

#include "pch.h"
#include "MainPage.xaml.h"
#include <string>
using namespace Microsoft::Graphics::Canvas::UI;

using namespace GameEngine2;
using namespace Platform;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::System::Threading;
using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::Effects;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;


//-----------------------------------------------------------------------------
// Game class constructor.
GameEngine2::MainPage::MainPage()
{
    // Initialize the Windows components. This is not initializing the game.
    InitializeComponent();

    // Tell Windows about our OnKeyDown and OnKeyUp event handlers.
    Window::Current->CoreWindow->KeyDown += ref new TypedEventHandler
        <CoreWindow^, KeyEventArgs^>(this, &MainPage::OnKeyDown);
    Window::Current->CoreWindow->KeyUp += ref new TypedEventHandler
        <CoreWindow^, KeyEventArgs^>(this, &MainPage::OnKeyUp);

	// Tell Windows about our OnCharReceived event handler.
	Window::Current->CoreWindow->CharacterReceived += ref new TypedEventHandler
		<CoreWindow^, CharacterReceivedEventArgs^>(this, &MainPage::OnCharReceived);

    // Tell Windows about our OnPointerPressed event handler.
    Window::Current->CoreWindow->PointerPressed += ref new TypedEventHandler
        <CoreWindow^, PointerEventArgs^>(this, &MainPage::OnPointerPressed);

	// Tell Windows about our OnPointerWheelChanged event handler.
	Window::Current->CoreWindow->PointerWheelChanged += ref new TypedEventHandler
		<CoreWindow^, PointerEventArgs^>(this, &MainPage::OnPointerWheelChanged);

	// Tell Windows about our OnPointerMoved event handler.
	Window::Current->CoreWindow->PointerMoved += ref new TypedEventHandler
		<CoreWindow^, PointerEventArgs^>(this, &MainPage::OnPointerMoved);

	// Tell Windows about or GamepadAdded event handler.
	Windows::Gaming::Input::Gamepad::GamepadAdded +=
		ref new Windows::Foundation::EventHandler
		<Windows::Gaming::Input::Gamepad ^>(this, &MainPage::OnGamepadAdded);
} 


//-----------------------------------------------------------------------------
void GameEngine2::MainPage::CreateResources(Xaml::CanvasAnimatedControl^ sender,
    CanvasCreateResourcesEventArgs^ args)
{
    using namespace Utilities;

    TimeSpan period;
    period.Duration = 10000000/DESIRED_UPS; // Set Updates/Second
    sender->TargetElapsedTime = period;

    // Lambda function for timer
    ThreadPoolTimer^ periodicTimer = 
        ThreadPoolTimer::CreatePeriodicTimer(
            ref new TimerElapsedHandler([&](ThreadPoolTimer^ source) {}),
            period);

	/*ApplicationView^ view = ApplicationView::GetForCurrentView();
	view->TryEnterFullScreenMode();*/

    // Initialize all game items.
    game.initialize(sender, args);
}


//-----------------------------------------------------------------------------
void GameEngine2::MainPage::Update(Xaml::CanvasAnimatedControl^ sender,
    Xaml::CanvasAnimatedUpdateEventArgs^ args)
{
    game.update(args);  // Update all game items.
}


//-----------------------------------------------------------------------------
void GameEngine2::MainPage::Draw(Xaml::CanvasAnimatedControl^ sender,
    Xaml::CanvasAnimatedDrawEventArgs^ args)
{
    game.draw(args);    // Draw all game items.
}


//-----------------------------------------------------------------------------
void GameEngine2::MainPage::OnKeyDown(CoreWindow ^sender, KeyEventArgs ^args)
{
    game.onKeyDown(sender, args);
    // Exit application if the Esc key is pressed
	if (args->VirtualKey == VirtualKey::Escape)
	{
        //ApplicationView^ view = ApplicationView::GetForCurrentView();
        //    view->ExitFullScreenMode();
        ApplicationView::GetForCurrentView()->ExitFullScreenMode();
    }

    // F11 toggles full screen mode
    if (args->VirtualKey == VirtualKey::F11)
    {
        ApplicationView^ view = ApplicationView::GetForCurrentView();
        if (view->IsFullScreenMode)
            view->ExitFullScreenMode();
        else
            view->TryEnterFullScreenMode();
    }
}

//-----------------------------------------------------------------------------
void GameEngine2::MainPage::OnKeyUp(CoreWindow^ sender, KeyEventArgs^ args)
{
    game.onKeyUp(sender, args);
}

//-----------------------------------------------------------------------------
void GameEngine2::MainPage::OnCharReceived(CoreWindow^ sender, CharacterReceivedEventArgs^ args)
{
    game.onCharReceived(sender, args);
}

//-----------------------------------------------------------------------------
void GameEngine2::MainPage::OnPointerPressed(CoreWindow^ sender, 
    PointerEventArgs^ args)
{
    game.onPointerPressed(sender, args);
}

//-----------------------------------------------------------------------------
void GameEngine2::MainPage::OnPointerWheelChanged(CoreWindow^ sender,
    PointerEventArgs^ args)
{
    game.onPointerWheelChanged(sender, args);
}

//-----------------------------------------------------------------------------
void GameEngine2::MainPage::OnPointerMoved(CoreWindow^ sender,
	PointerEventArgs^ args)
{
	game.onPointerMoved(sender, args);
}

//-----------------------------------------------------------------------------
//OnGamepadAdded(Windows::Gaming::Input::Gamepad^ sender, PointerEventArgs^ args)

void GameEngine2::MainPage::OnGamepadAdded(
	Platform::Object ^sender,
	Windows::Gaming::Input::Gamepad ^args)
{
	game.onGamepadAdded(sender, args);
}