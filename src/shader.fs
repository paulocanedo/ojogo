#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform bool reflected;
uniform sampler2D ourTexture1;
/* uniform sampler2D ourTexture2; */

const float offset = 1.0 / 300.0;

void main()
{
	vec2 offsets[9] = vec2[](
      vec2(-offset,  offset), // top-left
      vec2( 0.0f,    offset), // top-center
      vec2( offset,  offset), // top-right
      vec2(-offset,  0.0f),   // center-left
      vec2( 0.0f,    0.0f),   // center-center
      vec2( offset,  0.0f),   // center-right
      vec2(-offset, -offset), // bottom-left
      vec2( 0.0f,   -offset), // bottom-center
      vec2( offset, -offset)  // bottom-right
  );

  float kernel[9] = float[](
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
  );

  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++)
  {
      sampleTex[i] = vec3(texture(ourTexture1, texCoord.st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for(int i = 0; i < 9; i++)
      col += sampleTex[i] * kernel[i];


	/* if(reflected && finalColor.a > 0.0) {
		finalColor.a = (1.0 - texCoord.t) * 0.7;
	} */

	float reflectionAlpha = 1.0;
	vec2 coord = vec2(texCoord.s, texCoord.t * 2.0);
	if(texCoord.t < 0.5) {
		coord.t = 1.0 - coord.t;
	}

	vec4 finalColor = texture(ourTexture1, coord);
	if(texCoord.t < 0.5 && finalColor.a > 0) {
		finalColor.a = texCoord.t * 2.0 * 0.7;
	}
	FragColor = finalColor;
}
