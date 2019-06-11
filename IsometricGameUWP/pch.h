//
// pch.h
// Header for standard system include files.
// v16.4.15

#pragma once

#include <collection.h>
#include <ppltasks.h>

#include "App.xaml.h"

#include <memory>
#include <vector>
#include <wrl.h>
#include <WindowsNumerics.h>

#include "gameError.h"

// DirectX headers.
#if WINVER > _WIN32_WINNT_WINBLUE
#include <d2d1_3.h>
#endif
#include <d2d1_2.h>
#include <d2d1helper.h>
#include <DirectXMath.h>

// DirectX Tool Kit headers.
//#include <GeometricPrimitive.h>
#include <Effects.h>

// Win2D headers.
#include <Microsoft.Graphics.Canvas.native.h>

//#if WINVER <= _WIN32_WINNT_WINBLUE
//#include <Microsoft.Graphics.Canvas.DirectX.Direct3D11.interop.h>
//#else
#include <Windows.Graphics.DirectX.Direct3D11.interop.h>
//#endif

#include "common.h"

// Sound
#include <xaudio2.h>
#include <wincodec.h>
