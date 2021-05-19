Windows only and requires cmake 3.8 or newer.
To complie:
    cmake -G "Visual Studio 16 2019" -A x64 -B build
    cmake --build ./build/ --config Release

To run:
    ./MoorFilter.exe
    Requires a 64bit version of a portaudio dll. 
    Sound is loaded for use in reverb in GUI with button in the settings box.
    The play button under the graphs will play the sound looping infinately untill stopped.
    Reverb is updated in real time when playing.  