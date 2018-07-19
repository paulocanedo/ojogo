#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texCoord;
/* out float reflectedAlpha; */

uniform mat4 model;
uniform mat4 projection;

uniform bool reflected;

void main()
{
    texCoord = aTex;

    vec2 position = aPos.xy;
    /* reflectedAlpha = 1.0; */
    /* if(reflected) { */
      /* position.y = 1.0 - position.y; */
      /* reflectedAlpha = position.y; */
    /* } */
    gl_Position = projection * model * vec4(position, 1.0, 1.0);
}
