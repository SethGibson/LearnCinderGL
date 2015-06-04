#version 150
uniform vec3	uLightPosition = vec3(0.0,0.5,-0.5);
uniform vec3	uBaseColor = vec3(0.5,0.1,0.25);
uniform float	uAmbientStrength = 0.25;
 
in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

void main()
{
	vec3 lightDirection = normalize(uLightPosition-WorldPos);
	vec3 normal = normalize(Normal);
	float diffuse = max(dot(normal,lightDirection), 0.0);

	FragColor = vec4(uBaseColor*diffuse+uBaseColor*uAmbientStrength,1.0);
}