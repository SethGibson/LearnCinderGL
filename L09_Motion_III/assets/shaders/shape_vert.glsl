#version 150
uniform mat4 ciModelMatrix;
uniform mat4 ciViewMatrix;
uniform mat4 ciProjectionMatrix;
uniform mat4 ciNormalMatrix;

in vec4 ciPosition;
in vec4 ciNormal;

in vec3		iPosition;
in vec3		iColor;
in float	iSize;

out vec3 Normal;
out vec3 WorldPos;
out vec3 Emissive;

void main()
{
	Emissive = iColor;
	mat4 normalMatrix = transpose(inverse(ciModelMatrix));
	vec4 scaled = vec4(ciPosition.xyz*iSize,1.0);
	WorldPos = vec3(ciModelMatrix*scaled);
	Normal = vec3(normalMatrix*ciNormal);
	gl_Position = ciProjectionMatrix*ciViewMatrix*ciModelMatrix * (scaled+vec4(iPosition,1.0));
}