#version 460 core

#define MY_UNIFORM_MESH_TEXTURE 0

out vec4 outMeshColor;

in vec2 forwardMeshTexture;

layout (location = MY_UNIFORM_MESH_TEXTURE) uniform sampler2D uniformTexture;

void main()
{
    outMeshColor = texture(uniformTexture, forwardMeshTexture);
}