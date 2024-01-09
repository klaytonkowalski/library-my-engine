#version 460 core

#define MY_BINDING_CAMERA 0

#define MY_ATTRIBUTE_MESH_POSITION 0
#define MY_ATTRIBUTE_MESH_TEXTURE 1
#define MY_ATTRIBUTE_MESH_NORMAL 2
#define MY_ATTRIBUTE_MESH_TRANSFORM 3

layout (location = MY_ATTRIBUTE_MESH_POSITION) in vec3 myAttributeMeshPosition;
layout (location = MY_ATTRIBUTE_MESH_TEXTURE) in vec2 myAttributeMeshTexture;
layout (location = MY_ATTRIBUTE_MESH_NORMAL) in vec2 myAttributeMeshNormal;
layout (location = MY_ATTRIBUTE_MESH_TRANSFORM) in mat4 myAttributeMeshTransform;

out vec2 myForwardMeshTexture;

layout (std140, binding = MY_BINDING_CAMERA) uniform MyCamera
{
    mat4 view;
    mat4 projection;
}
myUniformCamera;

void main()
{
    gl_Position = myUniformCamera.projection * myUniformCamera.view * myAttributeMeshTransform * vec4(myAttributeMeshPosition, 1.0f);
    myForwardMeshTexture = myAttributeMeshTexture;
}