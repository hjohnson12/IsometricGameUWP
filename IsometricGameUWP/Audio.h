#pragma once
//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// audio.h v17.1.21
//=============================================================================
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
// Portions of this file:
// Copyright (c) Microsoft Corporation. All rights reserved


#include "mmreg.h"
#include <vector>
#include <memory>

class Audio
{
    struct ImplData;

    public:
        Audio(uint32 sampleRate);
        ~Audio();

        //---------------------------------------------------------------------
        size_t AddSound(_In_ WAVEFORMATEX* format, _In_ Platform::Array<byte>^ data);

        //---------------------------------------------------------------------
        // Play sound.
        // Pre: 
        //    index = Index of sound to play from the SOUND_FILES array.
        //    loopCount = 0, sound is played once.
        //               
        bool   PlaySound(size_t index, byte loopCount=0);

        //---------------------------------------------------------------------
        // Stops sound and empties sound buffers.
        bool   StopSound(size_t index);

        //---------------------------------------------------------------------
        // Set volume level
        // Valid ranges of volumes are -2^24 to 2^24, with a maximum gaing of 144.6 dB.
        // A volume level of 1.0 means no attenuation or gain and 0 means silence.
        // Negative volume levels invert the audio's phase.
        //---------------------------------------------------------------------
        void   SetVolume(size_t index, float volume=1.0f);

        //---------------------------------------------------------------------
        // Returns the most recently set volume level.
        //---------------------------------------------------------------------
        float  GetVolume(size_t index);

        //---------------------------------------------------------------------
        void   PauseSound(size_t index);

        //---------------------------------------------------------------------
        void   ResumeSound(size_t index);

        //---------------------------------------------------------------------
        bool   IsSoundPlaying(size_t index) const;

        //---------------------------------------------------------------------
        size_t GetSoundCount() const;

        void Suspend();
        void Resume();

    private:
        interface IXAudio2*                     m_audioEngine;
        interface IXAudio2MasteringVoice*       m_masteringVoice;
        std::vector<std::shared_ptr<ImplData>>  m_soundList;
};

