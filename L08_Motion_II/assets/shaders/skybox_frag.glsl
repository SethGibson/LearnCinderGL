#version 150
uniform samplerCube uCubemapSampler;

in vec3 LookupVector;

out vec4 FragColor;

void main()
{
	FragColor = texture(uCubemapSampler, LookupVector);
}