#version 460 core

#define MY_UNIFORM_SPRITE_TEXTURE 0

out vec4 outSpriteColor;

in vec2 forwardSpriteTexture;

layout (location = MY_UNIFORM_SPRITE_TEXTURE) uniform sampler2D uniformTexture;

void main()
{
    outSpriteColor = texture(uniformTexture, forwardSpriteTexture);
}