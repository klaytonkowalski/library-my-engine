#version 460 core

#define MY_ATTRIBUTE_MESH_POSITION 0
#define MY_ATTRIBUTE_MESH_TEXTURE 1
#define MY_ATTRIBUTE_MESH_NORMAL 2
#define MY_ATTRIBUTE_MESH_TRANSFORM 3

layout (location = MY_ATTRIBUTE_MESH_POSITION) in vec3 inMeshPosition;
layout (location = MY_ATTRIBUTE_MESH_TEXTURE) in vec2 inMeshTexture;
layout (location = MY_ATTRIBUTE_MESH_NORMAL) in vec2 inMeshNormal;
layout (location = MY_ATTRIBUTE_MESH_TRANSFORM) in mat4 inMeshTransform;

out vec2 forwardMeshTexture;

void main()
{
    gl_Position = inMeshTransform * vec4(inMeshPosition, 1.0f);
    forwardMeshTexture = inMeshTexture;
}