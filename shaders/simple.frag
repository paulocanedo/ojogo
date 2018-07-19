#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D ourTexture1;

void main()
{
	FragColor = texture(ourTexture1, texCoord);
}
