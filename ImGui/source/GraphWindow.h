/******************************************************************************/
/*!
\file   GraphWindow.h
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include <vector>
#include <string>
#include "utils.h"

class GraphWindow
{
public:
    /**
     * @brief Construct a new Graph Window object
     * 
     * @param data data that will be graphed
     * @param name name of the data for chart
     * @param info reverb info to send a play and stop command
     * @param type song to play on press play
     */
    GraphWindow(std::vector<float>* data, const char* name, utils::reverbInfo<float>* info,
                utils::SongType type);

    /**
     * @brief draws the graph window
     * 
     */
    void draw();

private:
    std::vector<float>* data;           // Data to graph
    std::string name;                   // Name of data
    utils::reverbInfo<float>* settings; // play and stop commands
    utils::SongType type;               // song to play on press play
};