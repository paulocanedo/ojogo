#version 330 core

precision highp float;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    texCoord = aTex;

    gl_Position = projection * model * vec4(aPos.xy, 1.0, 1.0);
}
