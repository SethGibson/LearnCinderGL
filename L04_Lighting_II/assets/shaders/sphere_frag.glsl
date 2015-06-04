#version 150
uniform vec3	uCameraPos;
uniform vec3	uLightPos;
uniform vec3	uColor;
uniform float	uSpecularStrength;
uniform float	uSpecularPower;
uniform float	uAmbientStrength;
 
in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(uLightPos-WorldPos);
	vec3 viewDirection = normalize(uCameraPos-WorldPos);
	vec3 viewReflect = reflect(viewDirection, normal);
	
	vec3 diffuse = uColor*max(dot(normal,lightDirection), 0.0);
	vec3 ambient = uColor*uAmbientStrength;
	float specular = pow(max(dot(-lightDirection, viewReflect),0.0), uSpecularPower)*uSpecularStrength;

	FragColor = vec4(diffuse+specular+ambient,1.0);
}