#pragma once

// For 3DS homebrew, we always use static linking
// No DLL semantics needed


// Alternative approach if you want to keep the original structure:
/*
#if defined(_3DS) || defined(__3DS__)
    // 3DS homebrew - always static linking
    #define MCLIB_API
#elif defined(_MSC_VER)
    #ifdef MCLIB_EXPORTS
        #define MCLIB_API __declspec(dllexport)
    #else
        #define MCLIB_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__)
    #ifdef MCLIB_EXPORTS
        #define MCLIB_API __attribute__((visibility("default")))
    #else
        #define MCLIB_API
    #endif
#else
    #define MCLIB_API  // Default to empty for unknown platforms
#endif
*/