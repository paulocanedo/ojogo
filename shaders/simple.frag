#version 330 core

uniform vec3 uColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(uColor.rgb, 1.0f);
	// FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}