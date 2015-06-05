#version 150
uniform samplerCube	uCubemapSampler;
uniform vec3	uCameraPos;
uniform vec3	uLightPos;
uniform vec3	uColor;
uniform float	uSpecularStrength;
uniform float	uSpecularPower;
uniform float	uAmbientStrength;
uniform float	uEnvStrength;
uniform float	uRefract;
 
in vec3 Normal;
in vec3 WorldPos;

out vec4 FragColor;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(uLightPos-WorldPos);
	vec3 viewDirection = normalize(uCameraPos-WorldPos);
	vec3 specularLookup = reflect(viewDirection, normal);
	vec3 refrLookup = refract(normal, viewDirection, 1.0/uRefract);
	

	vec4 refr = texture(uCubemapSampler, refrLookup)*uEnvStrength;
	vec3 diffuse = uColor*max(dot(normal,lightDirection), 0.0);
	vec3 ambient = uColor*uAmbientStrength;
	float specular = pow(max(dot(-lightDirection, specularLookup),0.0), uSpecularPower)*uSpecularStrength;

	FragColor = vec4(diffuse+specular+ambient+refr.rgb,1.0);
}