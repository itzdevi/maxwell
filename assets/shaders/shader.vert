#version 450

layout(push_constant) uniform constants {
    mat4 mvp;
} pc;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

void main() {
    gl_Position = pc.mvp * vec4(position, 1);
}