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
// Dependencies
////////////////////////////////////////////////////////////////////////////////

#include <myengine/myengine.h>

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    my_window_create(0, 0, 800, 800, "Example");
    my_sprite_create(1, 1);
    my_sprite_create(1, 1);
    my_entity_render(1, true);
    my_entity_render(2, true);
    while (my_window_prepare())
    {
        if (my_window_get_key_state(MY_KEY_W) == MY_KEY_STATE_DOWN)
        {
            my_entity_position(1, (MyVector) { 0.0f, 0.02f, 0.0f }, false);
        }
        if (my_window_get_key_state(MY_KEY_A) == MY_KEY_STATE_DOWN)
        {
            my_entity_position(1, (MyVector) { -0.02f, 0.0f, 0.0f }, false);
        }
        if (my_window_get_key_state(MY_KEY_S) == MY_KEY_STATE_DOWN)
        {
            my_entity_position(1, (MyVector) { 0.0f, -0.02f, 0.0f }, false);
        }
        if (my_window_get_key_state(MY_KEY_D) == MY_KEY_STATE_DOWN)
        {
            my_entity_position(1, (MyVector) { 0.02f, 0.0f, 0.0f }, false);
        }
        if (my_window_get_key_state(MY_KEY_Q) == MY_KEY_STATE_DOWN)
        {
            my_entity_rotation(1, (MyVector) { 0.0f, 0.0f, -2.0f }, false);
        }
        if (my_window_get_key_state(MY_KEY_E) == MY_KEY_STATE_DOWN)
        {
            my_entity_rotation(1, (MyVector) { 0.0f, 0.0f, 2.0f }, false);
        }
        if (my_window_get_key_state(MY_KEY_ESCAPE) == MY_KEY_STATE_PRESSED)
        {
            break;
        }
        my_window_render();
    }
    my_window_destroy();
    return 0;
}