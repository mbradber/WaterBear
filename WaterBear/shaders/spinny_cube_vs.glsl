#version 410 core

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 tc;

out VS_OUT {
    vec2 tc;
} vs_out;

void main() {
    vs_out.tc = tc;
    
    vec4 pos4 = vec4(position.xyz, 1.0);
    gl_Position = proj_matrix * mv_matrix * position;
}