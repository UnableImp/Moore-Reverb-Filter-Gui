/******************************************************************************/
/*!
\file   ReverbHandler.cpp
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include "ReverbHandler.h"
#include <iostream>
#include <thread>
#include <imgui.h>

ReverbHandler::ReverbHandler() : settingsWindow(&settings), 
                                 sourceWindow(&source, "Original Sound", &settings, utils::SongType::normal),
                                 impulseWindow(&impluse, "Impulse Response", & settings, utils::SongType::stop),
                                 reverbWindow(&reverbed, "Reverb", &settings, utils::SongType::reverb)
{
    // Default settings for moore reverb from `A Digital Signal Processing Primer` by Ken Steiglitz
    settings.g1 = (utils::lerp(0.24f, 0.46f, utils::lerpTime));
    settings.g2 = (utils::lerp(0.26f, 0.48f, utils::lerpTime));
    settings.g3 = (utils::lerp(0.28f, 0.50f, utils::lerpTime));
    settings.g4 = (utils::lerp(0.29f, 0.52f, utils::lerpTime));
    settings.g5 = (utils::lerp(0.30f, 0.53f, utils::lerpTime));
    settings.g6 = (utils::lerp(0.32f, 0.55f, utils::lerpTime));

    settings.R1 = ((1.0f - settings.g1) * 0.83f);
    settings.R2 = ((1.0f - settings.g2) * 0.83f);
    settings.R3 = ((1.0f - settings.g3) * 0.83f);
    settings.R4 = ((1.0f - settings.g4) * 0.83f);
    settings.R5 = ((1.0f - settings.g5) * 0.83f);
    settings.R6 = ((1.0f - settings.g6) * 0.83f);

    settings.L1 = (50);
    settings.L2 = (56);
    settings.L3 = (61); 
    settings.L4 = (68);
    settings.L5 = (72); 
    settings.L6 = (78);

    settings.m = (0.83f); 
    settings.m1 = settings.m;
    settings.m2 = settings.m;
    settings.m3 = settings.m;
    settings.m4 = settings.m;
    settings.m5 = settings.m;
    settings.m6 = settings.m;

    settings.a = (0.7f);
    settings.K = (1);

    settings.isDirty = (true);
    settings.getNewSong = (false);

    settings.ToPlay = utils::SongType::stop;

    settings.volume = 1;

    isUpdateing = false;
    settings.openSong = true;

    source.resize(1,0);
    reverbed.resize(1,0);
    onDirty();   

    updater = new std::thread(&ReverbHandler::updateReverb, this);

    audioManager.start(this);
}

ReverbHandler::~ReverbHandler()
{
    delete updater;
}

void ReverbHandler::update()
{   
    ImGuiWindowFlags flag = ImGuiCond_Once;
	ImGui::SetNextWindowSize(ImVec2(400, 675), flag);
	ImGui::SetNextWindowPos(ImVec2(0, 0), flag);
    settingsWindow.draw();
    
    if(settings.isDirty)
        onDirty();

    ImGui::SetNextWindowSize(ImVec2(840, 225), flag);
	ImGui::SetNextWindowPos(ImVec2(400, 0), flag);
    sourceWindow.draw();

    ImGui::SetNextWindowSize(ImVec2(840, 225), flag);
	ImGui::SetNextWindowPos(ImVec2(400, 225), flag);
    impulseWindow.draw();

    ImGui::SetNextWindowSize(ImVec2(840, 225), flag);
	ImGui::SetNextWindowPos(ImVec2(400, 450), flag);
    reverbWindow.draw();
}

void ReverbHandler::onDirty()
{
    if(settings.getNewSong)
    {
        source.clear();
        std::string name = (utils::AskForPath("Sound Files\0*.wav\0"));
        bool didOpen = utils::ReadWave(name.c_str(), source);
        settings.openSong = didOpen;
        settings.getNewSong = false;
        settings.index = 0;
    }

    if(settings.shouldSave)
    {
        utils::saveData(reverbed, "output.wav");
        settings.shouldSave = false;
    }

    
    settings.R1 = settings.m1 - (settings.m1 * settings.g1);
    settings.R2 = settings.m2 - (settings.m2 * settings.g2);
    settings.R3 = settings.m3 - (settings.m3 * settings.g3);
    settings.R4 = settings.m4 - (settings.m4 * settings.g4);
    settings.R5 = settings.m5 - (settings.m5 * settings.g5);
    settings.R6 = settings.m6 - (settings.m6 * settings.g6);

    settings.g1 = 1.0f - (1.0f/settings.m1 * settings.R1);
    settings.g2 = 1.0f - (1.0f/settings.m2 * settings.R2);
    settings.g3 = 1.0f - (1.0f/settings.m3 * settings.R3);
    settings.g4 = 1.0f - (1.0f/settings.m4 * settings.R4);
    settings.g5 = 1.0f - (1.0f/settings.m5 * settings.R5);
    settings.g6 = 1.0f - (1.0f/settings.m6 * settings.R6);

    updateImpuse();   

    isUpdateing = true;

    //updateReverb();
    settings.isDirty = false;
    isDirty = true;

}

// void ReverbHandler::updateRealTime()
// {
//     // rtReverb =         Reverb<float>(settings.L1, settings.g1, settings.R1,
//     //                         settings.L2, settings.g2, settings.R2,
//     //                         settings.L3, settings.g3, settings.R3,
//     //                         settings.L4, settings.g4, settings.R4,
//     //                         settings.L5, settings.g5, settings.R5,
//     //                         settings.L6, settings.g6, settings.R6,
//     //                         settings.a, settings.K);
//     rtReverb.update(settings.L1, settings.g1, settings.R1,
//                     settings.L2, settings.g2, settings.R2,
//                     settings.L3, settings.g3, settings.R3,
//                     settings.L4, settings.g4, settings.R4,
//                     settings.L5, settings.g5, settings.R5,
//                     settings.L6, settings.g6, settings.R6,
//                     settings.a, settings.K);
//     isDirty = false;
// }

int ReverbHandler::onWrite(float* out, unsigned long count)
{   
    switch (settings.ToPlay)
    {
    case utils::SongType::normal:
        for(unsigned i = 0; i < count; ++i, ++settings.index)
        {
            if(settings.index >= source.size())
                settings.index = 0;
            
            
            out[i] = source[settings.index] * settings.volume;
        }
        break;
    case utils::SongType::reverb:
        //if(isDirty)
        //   updateRealTime();
        for(unsigned i = 0; i < count; ++i, ++settings.index)
        {
            if(settings.index >= reverbed.size())
            {
                settings.index = 0;
            }
            out[i] = reverbed[settings.index];
            // if(settings.index < source.size())
            //     out[i] = rtReverb(source[settings.index]) * settings.volume;
            // else
            //     out[i] = rtReverb(0) *settings.volume;
        }
        break;
    default:
        for(unsigned i = 0; i < count; ++i)
        {
            out[i] = 0;
        }
    }
    return 0;
}

void ReverbHandler::updateReverb()
{
    while(true)
    {

        if(!isUpdateing)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            continue;
        }
    

        Reverb<float> reverb(settings.L1, settings.g1, settings.R1,
                            settings.L2, settings.g2, settings.R2,
                            settings.L3, settings.g3, settings.R3,
                            settings.L4, settings.g4, settings.R4,
                            settings.L5, settings.g5, settings.R5,
                            settings.L6, settings.g6, settings.R6,
                            settings.a, settings.K);

        reverbed.resize(source.size() + static_cast<unsigned>(RATE * 3), 0.0f);

        for(size_t i = 0; i < reverbed.size(); ++i)
        {
            if(i < source.size())
                reverbed[i] = reverb(source[i]);
            else
                reverbed[i] = reverb(0);
        }
        isUpdateing = false;
    }
}

void ReverbHandler::updateImpuse()
{
     Reverb<float> reverb(settings.L1, settings.g1, settings.R1,
                          settings.L2, settings.g2, settings.R2,
                          settings.L3, settings.g3, settings.R3,
                          settings.L4, settings.g4, settings.R4,
                          settings.L5, settings.g5, settings.R5,
                          settings.L6, settings.g6, settings.R6,
                          settings.a, settings.K);

    impluse.clear();
    impluse.push_back(reverb(1));

    for(int i = 0; i < 44100; ++i)
    {
        float value = reverb(0);
        impluse.push_back(value);
    }

}