#version 330 core

uniform vec3 uColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec3 viewPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * uLightColor;

	vec3 lightDir = normalize(uLightPos - fragPos);
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uLightColor;

	vec3 lightColor = vec3(0.0, 1.0, 1.0);
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 resultColor = (ambient + diffuse + specular) * uColor;
	FragColor = vec4(resultColor, 1.0f);
	// FragColor = vec4(uColor, 1.0f);
}