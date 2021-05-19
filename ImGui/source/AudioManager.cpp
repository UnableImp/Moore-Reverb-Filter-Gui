/******************************************************************************/
/*!
\file   AudioManager.cpp
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include "AudioManager.h"
#include "ReverbHandler.h"

AudioManager::AudioManager()
{
    Pa_Initialize();

    m_IsInit = true;

    // output stream parameters
    m_output_params.device = Pa_GetDefaultOutputDevice();
    m_output_params.channelCount = 1;
    m_output_params.sampleFormat = paFloat32;
    m_output_params.suggestedLatency = 2*Pa_GetDeviceInfo(m_output_params.device)
                                        ->defaultLowOutputLatency;
    m_output_params.hostApiSpecificStreamInfo = 0;
}

AudioManager::~AudioManager()
{
    if(m_IsInit)
        stop();
}

void AudioManager::stop()
{
        m_IsInit = false;
        
        Pa_StopStream(m_stream);
        Pa_CloseStream(m_stream);
        Pa_Terminate();
}

void AudioManager::start(void * data)
{
    Pa_OpenStream(&m_stream,0,&m_output_params,RATE,0,paClipOff,&AudioManager::onWrite,data);
    Pa_StartStream(m_stream);
}

int AudioManager::onWrite(const void *input, void *output, unsigned long frameCount,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags, void* userData)
{
    ReverbHandler* reverb = reinterpret_cast<ReverbHandler*>(userData);
    reverb->onWrite(reinterpret_cast<float*>(output), frameCount);
    return paContinue;
}