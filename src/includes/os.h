/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    // define something for Windows (32-bit and 64-bit, this part is common)
    #include <Windows.h>
    #ifdef _WIN64
        #define OS "_WIN64" // define something for Windows (64-bit only)
    #else
        #define OS "_WIN32" // define something for Windows (32-bit only)
    #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        #define OS "MACOS"
        // Other kinds of Mac OS
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __ANDROID__
    // android
#elif __linux__
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
    #error "Unknown compiler"
#endif

#ifndef OS
#define OS "UNKOWN_OS"
#endif