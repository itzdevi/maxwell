#version 450

layout(push_constant) uniform constants {
    mat4 model;
    mat4 vp;
    vec4 viewPos;
} pc;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outFragPos;
layout(location = 2) out vec3 outViewPos;

void main() {
    gl_Position = pc.vp * pc.model * vec4(position, 1);
    outNormal = (pc.model * vec4(normal, 1.0)).xyz;
    outFragPos = (pc.model * vec4(position, 1)).xyz;
    outViewPos = pc.viewPos.xyz;
}