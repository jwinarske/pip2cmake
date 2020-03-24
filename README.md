# pip2cmake

*Status* - Currently a work in progress

Currently the tool will enumerate a header file decorated with PIP meta data and dump the values.  It's ready for CMake generation stage.

Building on AppleClang 11.0, MSVC 2019, Linux GCC 7.4.


The goal of this project:  

1. Implement parsing of JUCE PIP meta data to directly generate CMake file
2. Test bed to implement module class for jucer2cmake
3. Merge module work into jucer2cmake
