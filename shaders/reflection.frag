#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D ourTexture1;

void main()
{
	vec4 finalColor;
	vec2 coord;

	float reflectionAlpha = 1.0;
	float reflectionReduceRate = 0.7;

	bool reflectedArea = texCoord.t < 0.5;
	bool isVisible;

	//take half sized texture, if 2 pixels become 1, the size reduces
	coord = vec2(texCoord.s, texCoord.t * 2.0);
	if(reflectedArea) {
		coord.t = 1.0 - coord.t;
	}

	finalColor = texture(ourTexture1, coord);
	isVisible = finalColor.a > 0; //ignore transparent fragments
	if(reflectedArea && isVisible) {
		//t axis is from 0 to 0.5 here, multiply by 2 to normalize it
		finalColor.a = texCoord.t * 2.0 * reflectionReduceRate;
	}
	FragColor = finalColor;
}
