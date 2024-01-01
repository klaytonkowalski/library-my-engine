#version 460 core

#define MY_ATTRIBUTE_SPRITE_POSITION 0
#define MY_ATTRIBUTE_SPRITE_TEXTURE 1
#define MY_ATTRIBUTE_SPRITE_TRANSFORM 2

layout (location = MY_ATTRIBUTE_SPRITE_POSITION) in vec3 inSpritePosition;
layout (location = MY_ATTRIBUTE_SPRITE_TEXTURE) in vec2 inSpriteTexture;
layout (location = MY_ATTRIBUTE_SPRITE_TRANSFORM) in mat4 inSpriteTransform;

out vec2 forwardSpriteTexture;

void main()
{
    gl_Position = inSpriteTransform * vec4(inSpritePosition, 1.0f);
    forwardSpriteTexture = inSpriteTexture;
}