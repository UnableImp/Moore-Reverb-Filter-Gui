/******************************************************************************/
/*!
\file   ReverbHandler.h
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include "utils.h"
#include "OptionsWindow.h"
#include "GraphWindow.h"
#include <portaudio.h>
#include "AudioManager.h"
#include <atomic>
#include "Reverb.h"
#include <thread>

#pragma once

/**
 * @brief Handles the generation of reverb data
 * 
 */
class ReverbHandler
{    
public:
    /**
     * @brief Construct a new Reverb Handler object
     * 
     */
    ReverbHandler();
    /**
     * @brief Destroy the Reverb Handler object
     * 
     */
    ~ReverbHandler();

    /**
     * @brief Draws all windows
     * 
     */
    void update();

    /**
     * @brief Callback to give data to AudioManager for port audio
     * 
     * @param out Data will be stored here
     * @param count Number of frames to write
     * @return 0 - Success 
     */
    int onWrite(float* out, unsigned long count);

private:

    void onDirty();       // updates inpmulse and flags for reverb to be regerated
    
    void updateReverb();  // Threaded function to update reverb
    void updateImpuse();  // Updates impuse 
    //void updateRealTime();

    OptionsWindow settingsWindow; 
    GraphWindow sourceWindow;
    GraphWindow impulseWindow;
    GraphWindow reverbWindow;
    AudioManager audioManager;
    utils::reverbInfo<float> settings;

    std::vector<float> source;     // Source sound
    std::vector<float> impluse;    // Impulse wave
    std::vector<float> reverbed;   // Reverb wave
    std::atomic<bool> isUpdateing; // is reverb being generated

    std::thread* updater;          // Regenerates reverb when its dirty

    bool isDirty;                  // Is the data dirty
};