#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view_proj;
} ubo;

layout(set = 0, binding = 1) uniform UniformDynamicBufferObject {
    mat4 model;
} uboDynamic;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main() {
    gl_Position = ubo.view_proj * uboDynamic.model* vec4(inPosition, 1.0);
    fragColor = inColor;
}