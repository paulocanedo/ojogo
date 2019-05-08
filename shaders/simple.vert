#version 330 core

precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 randomVector;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float animation;

void main()
{
    // texCoord = aTex;

    // gl_Position = vec4(aPos.xy, 1.0, 1.0);
    // gl_Position = projection * vec4(aPos.xy, 1.0, 1.0);
    // gl_Position = model * vec4(aPos.xy, 1.0, 1.0);
    // vec3 pos = aPos + randomVector * ((animation + 1.0) / 2.0) * 100.0;
    vec3 pos = aPos;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
