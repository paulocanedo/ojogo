#version 330 core

uniform vec3 uColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	vec3 lightDir = normalize(uLightPos - fragPos);
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uLightColor;

	vec3 resultColor = (ambient + diffuse) * uColor;
	FragColor = vec4(resultColor, 1.0f);
	// FragColor = vec4(uColor, 1.0f);
}