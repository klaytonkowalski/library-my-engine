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

#version 460 core

////////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////////

#define MY_BINDING_CAMERA 0

#define MY_ATTRIBUTE_SPRITE_POSITION 0
#define MY_ATTRIBUTE_SPRITE_TEXTURE 1
#define MY_ATTRIBUTE_SPRITE_TRANSFORM 2

////////////////////////////////////////////////////////////////////////////////
// Inputs
////////////////////////////////////////////////////////////////////////////////

layout (location = MY_ATTRIBUTE_SPRITE_POSITION) in vec3 myAttributeSpritePosition;
layout (location = MY_ATTRIBUTE_SPRITE_TEXTURE) in vec2 myAttributeSpriteTexture;
layout (location = MY_ATTRIBUTE_SPRITE_TRANSFORM) in mat4 myAttributeSpriteTransform;

////////////////////////////////////////////////////////////////////////////////
// Forwards
////////////////////////////////////////////////////////////////////////////////

out vec2 myForwardSpriteTexture;

////////////////////////////////////////////////////////////////////////////////
// Uniforms
////////////////////////////////////////////////////////////////////////////////

layout (std140, binding = MY_BINDING_CAMERA) uniform MyCamera
{
    mat4 view;
    mat4 projection;
}
myUniformCamera;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

void main()
{
    gl_Position = myUniformCamera.projection * myUniformCamera.view * myAttributeSpriteTransform * vec4(myAttributeSpritePosition, 1.0f);
    myForwardSpriteTexture = myAttributeSpriteTexture;
}