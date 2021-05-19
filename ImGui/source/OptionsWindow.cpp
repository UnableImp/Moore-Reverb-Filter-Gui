/******************************************************************************/
/*!
\file   OptionsWindow.cpp
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include "OptionsWindow.h"
#include "utils.h"
#include <imgui.h>
#include <portaudio.h>
#include <algorithm>
#include <iostream>

OptionsWindow::OptionsWindow(utils::reverbInfo<float>* info) : data(info)
{

}

void OptionsWindow::draw()
{   

    ImGuiWindowFlags windowFlags = 0;

    if(!ImGui::Begin("Settings", nullptr, windowFlags))
    {
        ImGui::End();
        return;
    }   
    
    // TODO one slider to change all values
    if(ImGui::CollapsingHeader("g Values"))
    {
        //ImGui::Text("g1:");
        if(ImGui::SliderFloat("g1", &data->g1, 0.001f, 1.0f ))
            data->isDirty = true;

        if(ImGui::SliderFloat("g2", &data->g2, 0.001f, 1.0f ))
            data->isDirty = true;

        if(ImGui::SliderFloat("g3", &data->g3, 0.001f, 1.0f ))
            data->isDirty = true;

        if(ImGui::SliderFloat("g4", &data->g4, 0.001f, 1.0f ))
            data->isDirty = true;

        if(ImGui::SliderFloat("g5", &data->g5, 0.001f, 1.0f ))
            data->isDirty = true;
        
        if(ImGui::SliderFloat("g6", &data->g6, 0.001f, 1.0f ))
            data->isDirty = true;

        if(data->isDirty)
            updateR();
        
        ImGui::NewLine();
    }
    // data->R1 = data->m - (data->m * data->g1);
    //    data->g1 = 1.0f - (1.0f/data->m * data->R1);
    //float gMin = std::max(1.0f - (1.0f/data->m * 1), 0);
    // float rMax = std::min(1.0f, data->m - (data->m * 0.001f));
    // float rmin = std::max(0.001f, data->m - (data->m * 1.0f));

    if(ImGui::CollapsingHeader("R Values"))
    {
        float rMax1 = std::min(1.0f, data->m1 - (data->m1 * 0.001f));
        float rmin1 = std::max(0.001f, data->m1 - (data->m1 * 1.0f));

        float rMax2 = std::min(1.0f, data->m2 - (data->m2 * 0.001f));
        float rmin2 = std::max(0.001f, data->m2 - (data->m2 * 1.0f));

        float rMax3 = std::min(1.0f, data->m3 - (data->m3 * 0.001f));
        float rmin3 = std::max(0.001f, data->m3 - (data->m3 * 1.0f));

        float rMax4 = std::min(1.0f, data->m4 - (data->m4 * 0.001f));
        float rmin4 = std::max(0.001f, data->m4 - (data->m4 * 1.0f));

        float rMax5 = std::min(1.0f, data->m5 - (data->m5 * 0.001f));
        float rmin5 = std::max(0.001f, data->m5 - (data->m5 * 1.0f));

        float rMax6 = std::min(1.0f, data->m6 - (data->m6 * 0.001f));
        float rmin6 = std::max(0.001f, data->m6 - (data->m6 * 1.0f));

        if(ImGui::SliderFloat("R1", &data->R1, rmin1, rMax1 ))
            data->isDirty = true;

        if(ImGui::SliderFloat("R2", &data->R2, rmin2, rMax2 ))
            data->isDirty = true;

        if(ImGui::SliderFloat("R3", &data->R3, rmin3, rMax3 ))
            data->isDirty = true;

        if(ImGui::SliderFloat("R4", &data->R4, rmin4, rMax4 ))
            data->isDirty = true;

        if(ImGui::SliderFloat("R5", &data->R5, rmin5, rMax5 ))
            data->isDirty = true;
        
        if(ImGui::SliderFloat("R6", &data->R6, rmin6, rMax6 ))
            data->isDirty = true;

        if(data->isDirty)
            updateG();
    
        ImGui::NewLine();
    }

    if(ImGui::CollapsingHeader("L Values (In Miliseconds)"))
    {
        if(ImGui::SliderInt("L1", &data->L1, 1, 300 ))
            data->isDirty = true;

        if(ImGui::SliderInt("L2", &data->L2, 1, 300 ))
            data->isDirty = true;

        if(ImGui::SliderInt("L3", &data->L3, 1, 300 ))
            data->isDirty = true;

        if(ImGui::SliderInt("L4", &data->L4, 1, 300 ))
            data->isDirty = true;

        if(ImGui::SliderInt("L5", &data->L5, 1, 300 ))
            data->isDirty = true;
        
        if(ImGui::SliderInt("L6", &data->L6, 1, 300 ))
            data->isDirty = true;
    }

    ImGui::NewLine();
    if(ImGui::SliderFloat("Allpass coeff a", &data->a, 0.0f, 1.0f))
        data->isDirty = true;

    ImGui::NewLine();
    if(ImGui::SliderFloat("R/(1-g) constant", &data->m, 0.001f, 1.0f))
    {
        updateM();
        data->isDirty = true;
    }

    if(ImGui::CollapsingHeader("R/(1-g) constsants"))
    {
        if(ImGui::SliderFloat("R/(1-g) 1", &data->m1, 0.001f, 1.0f))
            data->isDirty = true;

        if(ImGui::SliderFloat("R/(1-g) 2", &data->m2, 0.001f, 1.0f))
            data->isDirty = true;
        
        if(ImGui::SliderFloat("R/(1-g) 3", &data->m3, 0.001f, 1.0f))
            data->isDirty = true;
        
        if(ImGui::SliderFloat("R/(1-g) 4", &data->m4, 0.001f, 1.0f))
            data->isDirty = true;
        
        if(ImGui::SliderFloat("R/(1-g) 5", &data->m5, 0.001f, 1.0f))
            data->isDirty = true;
        
        if(ImGui::SliderFloat("R/(1-g) 6", &data->m6, 0.001f, 1.0f))
            data->isDirty = true;
    }

    ImGui::NewLine();
    if(ImGui::SliderFloat("Dry Precentage K", &data->K, 0.0f, 1.0f))
        data->isDirty = true;

    ImGui::NewLine();
    int volume = static_cast<int>(data->volume * 100);
    if(ImGui::SliderInt("Volume", &volume, 0, 100))
        data->volume = volume / 100.0f;

    ImGui::NewLine();
    if(ImGui::Button("Load Sound"))
    {
        data->isDirty = true;
        data->getNewSong = true;
    }

    ImGui::SameLine();
    if(ImGui::Button("Save Reverb"))
    {
        data->isDirty = true;
        data->shouldSave = true;
    }

    if(data->openSong == false)
    {
        ImGui::TextColored(ImVec4(1.0f, 0.f,0.f,0.5), "Failed to open sound!");
    }

    static int localSave = 0;
    if(data->shouldSave == true)
        localSave += 300;

    if(data->shouldSave || localSave)
    {
        --localSave;
        ImGui::Text("Reverb saved as output.wav");
    }

    ImGui::End();
}

void OptionsWindow::updateG()
{
    data->g1 = 1.0f - (1.0f/data->m1 * data->R1);
    data->g2 = 1.0f - (1.0f/data->m2 * data->R2);
    data->g3 = 1.0f - (1.0f/data->m3 * data->R3);
    data->g4 = 1.0f - (1.0f/data->m4 * data->R4);
    data->g5 = 1.0f - (1.0f/data->m5 * data->R5);
    data->g6 = 1.0f - (1.0f/data->m6 * data->R6);

}

void OptionsWindow::updateR()
{
    data->R1 = data->m1 - (data->m1 * data->g1);
    data->R2 = data->m2 - (data->m2 * data->g2);
    data->R3 = data->m3 - (data->m3 * data->g3);
    data->R4 = data->m4 - (data->m4 * data->g4);
    data->R5 = data->m5 - (data->m5 * data->g5);
    data->R6 = data->m6 - (data->m6 * data->g6);
}

void OptionsWindow::updateM()
{
    data->m1 = data->m;
    data->m2 = data->m;
    data->m3 = data->m;
    data->m4 = data->m;
    data->m5 = data->m;
    data->m6 = data->m;
}