#version 330 core

precision highp float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
// layout (location = 2) in vec3 randomVector;

// out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float animation;

void main()
{
    // texCoord = aTex;

    // vec3 pos = aPos + randomVector * ((animation + 1.0) / 2.0) * 100.0;
    vec3 pos = aPos;

    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = aNormal;

    gl_Position = projection * view * model * vec4(pos, 1.0);
}
