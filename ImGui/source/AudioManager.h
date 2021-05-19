/******************************************************************************/
/*!
\file   AudioManager.h
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include <portaudio.h>

#pragma once

constexpr float RATE = 44100.0f;

/**
 * @brief 
 *  Handles everything port audio
 * 
 */
class AudioManager
{
public:

    AudioManager();
    ~AudioManager();
  
    /**
     * @brief 
     *  Inits port audio
     * 
     * @param data 
     *  Custom class to get data samples from
     */
    void start(void* data);

    /**
     * @brief 
     *  Shuts down port audio
     */
    void stop();
    
private:

    /**
     * @brief 
     *  Port audio call back function
     * 
     * @param input not used
     * @param output Writing frames 
     * @param frameCount Number of frames to write
     * @param timeInfo not used 
     * @param statusFlags not used
     * @param userData custom class to get data samples
     * @return paContinue will always be returned
     */
    static int onWrite(const void *input, void *output, unsigned long frameCount,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags, void* userData);

    PaStreamParameters m_output_params; // Params
    PaStream *m_stream;                 // Stream
    bool m_IsInit;                      // is pa init
};