/******************************************************************************/
/*!
\file   OptionsWindow.h
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
namespace utils
{
template<typename T>
struct reverbInfo;
}

/**
 * @brief Handles setting and displaying all settings 
 * 
 */
class OptionsWindow
{
public:
    /**
     * @brief Construct a new Options Window object
     * 
     * @param info Were settings info will be stored
     */
    OptionsWindow(utils::reverbInfo<float>* info);

    /**
     * @brief Draw the settings window
     * 
     */
    void draw();

private:

    void updateG(); // Updates G based on R and M
    void updateR(); // Updates R based on G and M
    void updateM(); // Updates M based on G and R

    utils::reverbInfo<float>* data; // Setting data

};