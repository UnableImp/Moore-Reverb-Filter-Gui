/******************************************************************************/
/*!
\file   utils.h
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
#pragma once
namespace utils
{

constexpr float lerpTime = (44100-25000)/25000.0f;

/**
 * @brief Lerps
 * 
 * @param a starting point
 * @param b ending point
 * @param t time to lerp at
 * @return T lerp value
 */
template<typename T>
T lerp(T a, T b, float t)
{
    return a + t * ( b - a);
}

/**
 * @brief Type of songs
 * 
 */
enum SongType
{
	normal,
	reverb,
	stop
};

template<typename T>
struct reverbInfo
{
	SongType ToPlay;
	// ToPlay play;

    T    g1, g2, g3, g4, g5, g6,
         R1, R2, R3, R4, R5, R6,
		 m1, m2, m3, m4, m5, m6;
    int  L1, L2, L3, L4, L5, L6;
    T    a, m, K;
    bool isDirty, getNewSong, openSong, shouldSave;
	unsigned long index;
	float volume;
};

struct fileheader
{
	char riff_label[4]; // (00) = {'R','I','F','F'}
	unsigned riff_size; // (04) = 36 + data_size
	char file_tag[4]; // (08) = {'W','A','V','E'}
	char fmt_label[4]; // (12) = {'f','m','t',' '}
	unsigned fmt_size; // (16) = 16
	unsigned short audio_format; // (20) = 1
	unsigned short channel_count; // (22) = 1 or 2
	unsigned sampling_rate; // (24) = (anything)
	unsigned bytes_per_second; // (28) = (see above)
	unsigned short bytes_per_sample; // (32) = (see above)
	unsigned short bits_per_sample; // (34) = 8 or 16
	char data_label[4]; // (36) = {'d','a','t','a'}
	unsigned data_size; // (40) = # bytes of data
};

/**
 * @brief Reads a wav file from filesystem
 * 
 * @param filename path to file
 * @param fSamples were data will be sroed
 * @return true file was read
 * @return false failed to read file
 */
bool ReadWave(const char* filename, std::vector<float>& fSamples);

/**
 * @brief saves data into a wave file
 * 
 * @param list data to svae
 * @param name name of wav file
 */
void saveData(std::vector<float>& list, const char* name);

/**
 * @brief Normilizes data
 * 
 * @param samples data to normalize
 * @param level Peak dB
 */
void normalize(std::vector<double>& samples, double level);

/**
 * @brief Removes dc offset from data
 * 
 * @param samples data to remove dc offset from
 */
void removeDCOffset(std::vector<double>& samples);

/**
 * @brief asks user for a file path
 * 
 * @param filter limit on what type of files to accpet
 * @return std::string path the user choose
 */
std::string AskForPath(const char* filter);

}

