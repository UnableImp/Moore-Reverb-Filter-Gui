/******************************************************************************/
/*!
\file   utils.cpp
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include "utils.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>


#include <windows.h>
#include <Commdlg.h>

namespace utils
{

bool ReadWave(const char* filename, std::vector<float>& fSamples)
{
    std::fstream in(filename , std::ios_base::binary | std::ios_base::in);
    if (!in)
        return false;

    char header[44];
    in.read(header,44);

    fileheader cHeader = *reinterpret_cast<fileheader*>(header);
    
    if(cHeader.channel_count != 1)
    {
        std::cout << "Channel count of " << cHeader.channel_count << " not supported"  << std::endl; 
       return false;
    }

    if(cHeader.bytes_per_sample != 2)
    {
        std::cout << "Byters per sample of " << cHeader.bytes_per_sample << " not supported" << std::endl;
        return false;
    }
    unsigned size = cHeader.data_size;
    unsigned count = size / 2;
            
    char *data = new char[size];
  

    in.read(data,size);
    short *samples = reinterpret_cast<short*>(data);

    //std::cout << cHeader.data_size << " " << count << " " << cHeader.riff_size << std::endl;

    // Convert to range [-1,1]
    fSamples.reserve(count);
    for(unsigned i = 0; i < count; ++i)
    {
        fSamples.push_back(samples[i] / static_cast<float>((1 << 15)));
    }
    
    delete [] data;

    return true;
}

void saveData(std::vector<float>& list, const char* name)
{
    fileheader header = { {'R','I','F','F'},             
                        static_cast<unsigned>(36 + (static_cast<int>(list.size()) * 2)),
                        {'W','A','V','E'},
                        {'f','m','t',' '},
                        16,
                        1,             
                        static_cast<unsigned short>(1),
                        44100,
                        44100*2,
                        static_cast<unsigned short>(2),
                        static_cast<unsigned short>(16),
                        {'d','a','t','a'},
                        static_cast<unsigned>(static_cast<int>(list.size()) * 2)
                    };

    short *data = new short[list.size()];
    for(unsigned i = 0; i < list.size(); ++i) 
        data[i] = static_cast<short>(list[i] * (1 << 15)) ;

    std::fstream out(name, std::ios_base::binary|std::ios_base::out);
    out.write(reinterpret_cast<char*>(&header), 44);
    out.write(reinterpret_cast<char*>(data),list.size() * 2);
    out.close();

    
    delete [] data;
}

void normalize(std::vector<double>& samples, double level)
{   
    // float max = std::numeric_limits<float>::lowest();
    // for(const double& x : samples)
    //     max = std::max(max, std::abs(x));


    // for(double& x : samples)
    //     x *= level/max;
    

}

void removeDCOffset(std::vector<double>& samples)
{
    double offset = 0;
    for(auto const& sample : samples)
        offset += sample;

    offset /= samples.size();

    for(auto& sample : samples)
        sample += offset;
}


std::string AskForPath(const char* filter)
{
    /*Code based off of: https://www.daniweb.com/programming/software-development/code/217307/a-simple-getopenfilename-example */
    OPENFILENAME ofn;

    char szFile[1024];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    
    ofn.lpstrFilter = filter;

    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    GetOpenFileName(&ofn);

    std::string ws(szFile);
    return ws;
}
}