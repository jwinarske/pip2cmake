# pip2cmake

*Status* - Currently a work in progress

Currently the tool will enumerate a header file decorated with PIP meta data and dump the values.  It's ready for CMake generation stage.

Building on AppleClang 11.0, MSVC 2019, Linux GCC 7.4.


The goal of this project:  

1. Implement parsing of JUCE PIP meta data to directly generate CMake file
2. Test bed to implement module class for jucer2cmake
3. Merge module work into jucer2cmake


Example run on macOS Catalina

```
cartel:build joel.winarske$ ./pip2cmake -i ~/git/tracktion_engine/examples/PatternGeneratorDemo.h -v
Opening "/Users/joel.winarske/git/tracktion_engine/examples/PatternGeneratorDemo.h"
Module Base: /Users/joel.winarske/git/tracktion_engine/examples/../modules

** PIP **
[Exporters]
linux_make
vs2017
xcode_iphone
xcode_mac
[ModuleFlags]
JUCE_STRICT_REFCOUNTEDPOINTER=1
JUCE_PLUGINHOST_VST3=1
JUCE_PLUGINHOST_AU=1
TRACKTION_ENABLE_TIMESTRETCH_SOUNDTOUCH=1
Description: This example shows how to use the pattern generator to create MIDI content.
MainClass: PatternGeneratorComponent
Name: PatternGeneratorDemo
Type: Component
Vendor: Tracktion
Version: 0.0.1
Website: www.tracktion.com
Dependencies[]

** Module **
ID : juce_audio_basics
Vendor : juce
Version : 5.4.4
Name : JUCE audio and MIDI data classes
Description : Classes for audio buffer manipulation, midi message handling, synthesis, etc.
Website : http
License : ISC
<< dep >> juce_core
<< OSXFramework >> Accelerate
<< iOSFrameworks >> Accelerate

** Module **
ID : juce_audio_devices
Vendor : juce
Version : 5.4.4
Name : JUCE audio and MIDI I/O device classes
Description : Classes to play and record from audio and MIDI I/O devices
Website : http
License : ISC
<< dep >> juce_audio_basics
<< dep >> juce_events
<< OSXFramework >> CoreAudio
<< OSXFramework >> CoreMIDI
<< OSXFramework >> AudioToolbox
<< iOSFrameworks >> CoreAudio
<< iOSFrameworks >> CoreMIDI
<< iOSFrameworks >> AudioToolbox
<< iOSFrameworks >> AVFoundation
<< linuxPackages >> alsa
<< mingwLibs >> winmm

** Module **
ID : juce_audio_formats
Vendor : juce
Version : 5.4.4
Name : JUCE audio file format codecs
Description : Classes for reading and writing various audio file formats.
Website : http
License : GPL/Commercial
<< dep >> juce_audio_basics
<< OSXFramework >> CoreAudio
<< OSXFramework >> CoreMIDI
<< OSXFramework >> QuartzCore
<< OSXFramework >> AudioToolbox
<< iOSFrameworks >> AudioToolbox
<< iOSFrameworks >> QuartzCore

** Module **
ID : juce_audio_processors
Vendor : juce
Version : 5.4.4
Name : JUCE audio processor classes
Description : Classes for loading and playing VST, AU, LADSPA, or internally-generated audio processors.
Website : http
License : GPL/Commercial
<< dep >> juce_gui_extra
<< dep >> juce_audio_basics
<< OSXFramework >> CoreAudio
<< OSXFramework >> CoreMIDI
<< OSXFramework >> AudioToolbox
<< iOSFrameworks >> AudioToolbox

** Module **
ID : juce_audio_utils
Vendor : juce
Version : 5.4.4
Name : JUCE extra audio utility classes
Description : Classes for audio-related GUI and miscellaneous tasks.
Website : http
License : GPL/Commercial
<< dep >> juce_gui_extra
<< dep >> juce_audio_processors
<< dep >> juce_audio_formats
<< dep >> juce_audio_devices
<< OSXFramework >> CoreAudioKit
<< OSXFramework >> DiscRecording
<< iOSFrameworks >> CoreAudioKit

** Module **
ID : juce_core
Vendor : juce
Version : 5.4.4
Name : JUCE core classes
Description : The essential set of basic JUCE classes, as required by all the other JUCE modules. Includes text, container, memory, threading and i/o functionality.
Website : http
License : ISC
<< OSXFramework >> Cocoa
<< OSXFramework >> IOKit
<< iOSFrameworks >> Foundation
<< linuxLibs >> rt
<< linuxLibs >> dl
<< linuxLibs >> pthread
<< mingwLibs >> uuid
<< mingwLibs >> wsock32
<< mingwLibs >> wininet
<< mingwLibs >> version
<< mingwLibs >> ole32
<< mingwLibs >> ws2_32
<< mingwLibs >> oleaut32
<< mingwLibs >> imm32
<< mingwLibs >> comdlg32
<< mingwLibs >> shlwapi
<< mingwLibs >> rpcrt4
<< mingwLibs >> winmm

** Module **
ID : juce_data_structures
Vendor : juce
Version : 5.4.4
Name : JUCE data model helper classes
Description : Classes for undo/redo management, and smart data structures.
Website : http
License : GPL/Commercial
<< dep >> juce_events

** Module **
ID : juce_dsp
Vendor : juce
Version : 5.4.4
Name : JUCE DSP classes
Description : Classes for audio buffer manipulation, digital audio processing, filtering, oversampling, fast math functions etc.
Website : http
License : GPL/Commercial
<< dep >> juce_audio_basics
<< dep >> juce_audio_formats
<< OSXFramework >> Accelerate
<< iOSFrameworks >> Accelerate

** Module **
ID : juce_events
Vendor : juce
Version : 5.4.4
Name : JUCE message and event handling classes
Description : Classes for running an application's main event loop and sending/receiving messages, timers, etc.
Website : http
License : ISC
<< dep >> juce_core

** Module **
ID : juce_graphics
Vendor : juce
Version : 5.4.4
Name : JUCE graphics classes
Description : Classes for 2D vector graphics, image loading/saving, font handling, etc.
Website : http
License : GPL/Commercial
<< dep >> juce_events
<< OSXFramework >> Cocoa
<< OSXFramework >> QuartzCore
<< iOSFrameworks >> CoreGraphics
<< iOSFrameworks >> CoreImage
<< iOSFrameworks >> CoreText
<< iOSFrameworks >> QuartzCore
<< linuxPackages >> x11
<< linuxPackages >> xinerama
<< linuxPackages >> xext
<< linuxPackages >> freetype2

** Module **
ID : juce_gui_basics
Vendor : juce
Version : 5.4.4
Name : JUCE GUI core classes
Description : Basic user-interface components and related classes.
Website : http
License : GPL/Commercial
<< dep >> juce_graphics
<< dep >> juce_data_structures
<< OSXFramework >> Cocoa
<< OSXFramework >> Carbon
<< OSXFramework >> QuartzCore
<< iOSFrameworks >> UIKit
<< iOSFrameworks >> MobileCoreServices
<< linuxPackages >> x11
<< linuxPackages >> xinerama
<< linuxPackages >> xext

** Module **
ID : juce_gui_extra
Vendor : juce
Version : 5.4.4
Name : JUCE extended GUI classes
Description : Miscellaneous GUI classes for specialised tasks.
Website : http
License : GPL/Commercial
<< dep >> juce_gui_basics
<< OSXFramework >> WebKit

** Module **
ID : juce_osc
Vendor : juce
Version : 5.4.4
Name : JUCE OSC classes
Description : Open Sound Control implementation.
Website : http
License : GPL/Commercial
<< dep >> juce_core
<< dep >> juce_events

** Module **
ID : tracktion_engine
Vendor : Tracktion Corporation
Version : 1.0.0
Name : The Tracktion audio engine
Description : Classes for manipulating and playing Tracktion projects
Website : http
License : Proprietary
<< dep >> juce_audio_devices
<< dep >> juce_audio_utils
<< dep >> juce_gui_extra
<< dep >> juce_dsp
<< dep >> juce_osc


Created "/Users/joel.winarske/git/tracktion_engine/examples/CMakeLists.txt"
cartel:build joel.winarske$
```
