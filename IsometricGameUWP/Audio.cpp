//=============================================================================
// Programming 2D Games
// Copyright (c) 2016 by: 
// Charles Kelly
// audio.cpp v16.4.5
//=============================================================================
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Portions of this file:
// Copyright (c) Microsoft Corporation. All rights reserved

#include "pch.h"
#include "Audio.h"

namespace
{
    //
    // Handler for XAudio source voice callbacks to maintain playing state
    //
    class SoundCallbackHander : public IXAudio2VoiceCallback
    {
    public:
        SoundCallbackHander(bool* isPlayingHolder) :
            m_isPlayingHolder(isPlayingHolder)
        {}

        ~SoundCallbackHander()
        {
            m_isPlayingHolder = nullptr;
        }

        //
        // Voice callbacks from IXAudio2VoiceCallback
        //
        STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 bytesRequired);
        STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS);
        STDMETHOD_(void, OnStreamEnd) (THIS);
        STDMETHOD_(void, OnBufferStart) (THIS_ void* bufferContext);
        STDMETHOD_(void, OnBufferEnd) (THIS_ void* bufferContext);
        STDMETHOD_(void, OnLoopEnd) (THIS_ void* bufferContext);
        STDMETHOD_(void, OnVoiceError) (THIS_ void* bufferContext, HRESULT error);

    private:
        bool* m_isPlayingHolder;
    };

    //
    // Callback handlers, only implement the buffer events for maintaining play state
    //
    void SoundCallbackHander::OnVoiceProcessingPassStart(UINT32 /*bytesRequired*/)
    {
    }
    void SoundCallbackHander::OnVoiceProcessingPassEnd()
    {
    }
    void SoundCallbackHander::OnStreamEnd()
    {
    }
    void SoundCallbackHander::OnBufferStart(void* /*bufferContext*/)
    {
        *m_isPlayingHolder = true;
    }
    void SoundCallbackHander::OnBufferEnd(void* /*bufferContext*/)
    {
        *m_isPlayingHolder = false;
    }
    void SoundCallbackHander::OnLoopEnd(void* /*bufferContext*/)
    {
    }
    void SoundCallbackHander::OnVoiceError(void* /*bufferContext*/, HRESULT /*error*/)
    {
    }
}

//
// Per sound data required to play a sound
//
struct Audio::ImplData
{
    ImplData::ImplData(Platform::Array<byte>^ data) :
        sourceVoice(nullptr),
        playData(data),
        isPlaying(false),
        callbackHander(&isPlaying)
    {
    }

    ~ImplData()
    {
        if (sourceVoice)
        {
            sourceVoice->DestroyVoice();
            sourceVoice = nullptr;
        }
    }

    IXAudio2SourceVoice*    sourceVoice;
    Platform::Array<byte>^  playData;
    bool                    isPlaying;
    SoundCallbackHander     callbackHander;
};


//-----------------------------------------------------------------------------
// Name: Audio constructor
// Desc: Setup common XAudio2 objects:
//-----------------------------------------------------------------------------
Audio::Audio(uint32 sampleRate) :
    m_soundList()
{
    // Create the XAudio2 Engine
    UINT32 flags = 0;
    HRESULT hr;
    if (FAILED(hr = XAudio2Create(&m_audioEngine, flags)))
        throw(GameError(gameErrorNS::FATAL_ERROR,
            "Error creating XAudio2 Engine"));

    // Create the mastering voice
    if (FAILED(hr = m_audioEngine->CreateMasteringVoice(
        &m_masteringVoice,
        XAUDIO2_DEFAULT_CHANNELS,
        sampleRate
        )))
        throw(GameError(gameErrorNS::FATAL_ERROR,
            "Error creating XAudio2 mastering voice"));
}

//-----------------------------------------------------------------------------
// Name: Audio destructor
// Desc: Stop all playing sounds, cleanup per sound data, and destroy XAudio
//       object.
//-----------------------------------------------------------------------------
Audio::~Audio()
{
    if (m_masteringVoice != nullptr)
    {
        m_masteringVoice->DestroyVoice();
        m_masteringVoice = nullptr;
    }

    if (m_audioEngine != nullptr)
    {
        m_audioEngine->Release();
        m_audioEngine = nullptr;
    }
}

//-----------------------------------------------------------------------------
// Name: Audio::AddSound
// Desc: Create data, create voice, store data
//-----------------------------------------------------------------------------
size_t Audio::AddSound(
    _In_ WAVEFORMATEX*  format,
    _In_ Platform::Array<byte>^   data
    )
{
    //
    // Allocate the implementation data
    //
    std::shared_ptr<ImplData> implData(new ImplData(data));

    //
    // Create the source voice
    //
    HRESULT hr;
    if (FAILED(hr = m_audioEngine->CreateSourceVoice(
        &implData->sourceVoice,
        format,
        0,
        XAUDIO2_DEFAULT_FREQ_RATIO,
        reinterpret_cast<IXAudio2VoiceCallback*>(&implData->callbackHander),
        nullptr,
        nullptr
        )))
        throw(GameError(gameErrorNS::FATAL_ERROR,
            "Error creating XAudio2 source voice"));

    //
    // Add to the list and return its index
    //
    m_soundList.push_back(implData);
    return (m_soundList.size() - 1);
}

//-----------------------------------------------------------------------------
// Name: Audio::PlaySound
// Desc: Stop if already playing, setup XAudio2 Sound buffer and play
//-----------------------------------------------------------------------------
bool Audio::PlaySound(size_t index, byte loopCount)
{
    //
    // Setup buffer
    //
    XAUDIO2_BUFFER playBuffer = { 0 };
    std::shared_ptr<ImplData> soundData = m_soundList[index];
    playBuffer.AudioBytes = soundData->playData->Length;
    playBuffer.pAudioData = soundData->playData->Data;
    playBuffer.Flags = XAUDIO2_END_OF_STREAM;
    playBuffer.LoopCount = loopCount;

    //
    // In case it is playing, stop it and flush the buffers.
    //
    HRESULT hr = soundData->sourceVoice->Stop();
    if (SUCCEEDED(hr))
    {
        hr = soundData->sourceVoice->FlushSourceBuffers();
    }

    //
    // Submit the sound buffer and (re)start (ignore any 'stop' failures)
    //
    hr = soundData->sourceVoice->SubmitSourceBuffer(&playBuffer);
    if (SUCCEEDED(hr))
    {
        hr = soundData->sourceVoice->Start(0, XAUDIO2_COMMIT_NOW);
    }

    return SUCCEEDED(hr);
}

//-----------------------------------------------------------------------------
// Name: Audio::StopSound
// Desc: Stop if playing
//-----------------------------------------------------------------------------
bool Audio::StopSound(size_t index)
{
    if (index >= 0 && index < m_soundList.size())   // If valid index
    {
        std::shared_ptr<ImplData> soundData = m_soundList[index];

        HRESULT hr = soundData->sourceVoice->Stop(XAUDIO2_PLAY_TAILS);
        if (SUCCEEDED(hr))
        {
            hr = soundData->sourceVoice->FlushSourceBuffers();
        }
        return SUCCEEDED(hr);
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: Audio::SetVolume
// Desc: Adjust volume
//-----------------------------------------------------------------------------
void Audio::SetVolume(size_t index, float volume)
{
    if (index >= 0 && index < m_soundList.size())   // If valid index
    {
        if (volume >= -XAUDIO2_MAX_VOLUME_LEVEL &&
            volume <= XAUDIO2_MAX_VOLUME_LEVEL)
        {
            std::shared_ptr<ImplData> soundData = m_soundList[index];
            soundData->sourceVoice->SetVolume(volume);
        }
    }
}

//-----------------------------------------------------------------------------
// Name: Audio::GetVolume
// Desc: Return the volume
//-----------------------------------------------------------------------------
float Audio::GetVolume(size_t index)
{
    if (index >= 0 && index < m_soundList.size())   // If valid index
    {
        float volume;
        m_soundList[index]->sourceVoice->GetVolume(&volume);
        return volume;
    }
    return -1.0f;   // If invalid index
}

//-----------------------------------------------------------------------------
// Name: Audio::PauseSound
// Desc: Pause if playing
//-----------------------------------------------------------------------------
void Audio::PauseSound(size_t index)
{
    if (index >= 0 && index < m_soundList.size())   // If valid index
    {
        std::shared_ptr<ImplData> soundData = m_soundList[index];
        soundData->sourceVoice->Stop();
        m_soundList[index]->isPlaying = false;
    }
}

//-----------------------------------------------------------------------------
// Name: Audio::ResumeSound
// Desc: Resume paused sound.
//-----------------------------------------------------------------------------
void Audio::ResumeSound(size_t index)
{
    if (index >= 0 && index < m_soundList.size())   // If valid index
    {
        std::shared_ptr<ImplData> soundData = m_soundList[index];
        soundData->sourceVoice->Start(0);
        m_soundList[index]->isPlaying = true;
    }
}

//-----------------------------------------------------------------------------
// Name: Audio::IsSoundPlaying
// Desc: Returns the playing state of the sound at sent index
//-----------------------------------------------------------------------------
bool Audio::IsSoundPlaying(size_t index) const
{
    if (index >= 0 && index < m_soundList.size())   // If valid index
    {
        return m_soundList[index]->isPlaying;
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: Audio::GetSoundCount
// Desc: Returns the number of sounds in the sound list
//-----------------------------------------------------------------------------
size_t Audio::GetSoundCount() const
{
    return m_soundList.size();
}

//-----------------------------------------------------------------------------
// Name: Audio::Suspend
// Desc: Stops the XAudio2 Engine
//-----------------------------------------------------------------------------
void Audio::Suspend()
{
    m_audioEngine->StopEngine();
}

//-----------------------------------------------------------------------------
// Name: Audio::Resume
// Desc: Start the XAudio2 engine
//-----------------------------------------------------------------------------
void Audio::Resume()
{
    HRESULT hr;
    if (FAILED(hr = m_audioEngine->StartEngine()))
        throw(GameError(gameErrorNS::FATAL_ERROR,
            "Error calling Audio::Resume()"));
}
