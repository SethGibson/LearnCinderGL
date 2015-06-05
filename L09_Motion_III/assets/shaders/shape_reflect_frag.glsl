#version 150
uniform samplerCube	uCubemapSampler;
uniform vec3	uCameraPos;
uniform vec3	uLightPos;
uniform vec3	uColor;

in vec3 Normal;
in vec3 WorldPos;
in vec3 Emissive;

out vec4 FragColor;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(uLightPos-WorldPos);
	vec3 viewDirection = normalize(uCameraPos-WorldPos);
	vec3 reflectDirection = normalize(WorldPos-uCameraPos);
	vec3 specularLookup = reflect(viewDirection, normal);
	vec3 reflLookup = reflect(reflectDirection, normal);

	vec4 refl = texture(uCubemapSampler, reflLookup)*0.5;
	vec3 diffuse = uColor*max(dot(normal,lightDirection), 0.0);
	vec3 ambient = uColor*0.1;
	float specular = pow(max(dot(-lightDirection, specularLookup),0.0), 16.0);

	FragColor = vec4(diffuse+specular+ambient+refl.rgb+Emissive,1.0);
}