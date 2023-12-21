////////////////////////////////////////////////////////////////////////////////
// License
////////////////////////////////////////////////////////////////////////////////

// Copyright (c) 2023 Klayton Kowalski
// 
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.

////////////////////////////////////////////////////////////////////////////////

#ifndef MY_GUARD_EXPORT
#define MY_GUARD_EXPORT

#ifdef MY_SHARED

    #ifdef MY_EXPORT
        #ifdef _WIN32
            #define MY_API __declspec(dllexport)
        #else
            #define MY_API __attribute__((visibility("default")))
        #endif

    #else
        #ifdef _WIN32
            #define MY_API __declspec(dllimport)
        #else
            #define MY_API __attribute__((visibility("default")))
        #endif
    #endif

#else
    #define MY_API
#endif

#endif