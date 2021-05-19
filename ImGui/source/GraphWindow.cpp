/******************************************************************************/
/*!
\file   GraphWindow.cpp
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include "Graphwindow.h"
#include <imgui.h>
#include "utils.h"

GraphWindow::GraphWindow(std::vector<float>* data, const char* name, utils::reverbInfo<float>* info,
                         utils::SongType type) : data(data), name(name), settings(info), type(type)
{

}

void GraphWindow::draw()
{
    ImGuiWindowFlags windowFlags = 0;

    if(!ImGui::Begin(name.c_str(), nullptr, windowFlags))
    {
        ImGui::End();
        return;
    }  

    // Force graph to take of full window size
    ImVec2 max = ImGui::GetWindowContentRegionMax();
    ImVec2 min = ImGui::GetWindowContentRegionMin();
    max.y -= 50;

    ImGui::PlotLines((std::string("##") + name).c_str(), &((*data)[0]), static_cast<int>(data->size()),
                   0, "", -1.0f, 1.0f, max);
    
    // ImGui::PlotHistogram((std::string("##") + name).c_str(), &((*data)[0]), static_cast<int>(data->size()),
    //                 0, "", -1.0f, 1.0f, max);
    if(type != utils::SongType::stop)
    {
        if(ImGui::Button("Play"))
        {
            settings->ToPlay = type;
            settings->isDirty = true;
            settings->index = 0;
        }
        ImGui::SameLine();
        if(ImGui::Button("Stop"))
        {
            settings->ToPlay = utils::SongType::stop;
            settings->isDirty = true;
            settings->index = 0;
        }
    }
    ImGui::End();
}