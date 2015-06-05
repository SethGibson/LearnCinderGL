#version 150
uniform mat4 ciModelMatrix;
uniform mat4 ciViewMatrix;
uniform mat4 ciProjectionMatrix;

in vec4 ciPosition;
in vec4 ciNormal;

in vec3 iPosition;

out vec3 Normal;
out vec3 WorldPos;

void main()
{
	mat4 normalMatrix = transpose(inverse(ciModelMatrix));
	WorldPos = vec3(ciModelMatrix*ciPosition);
	Normal = vec3(normalMatrix*ciNormal);
	gl_Position = ciProjectionMatrix*ciViewMatrix*ciModelMatrix * (ciPosition+vec4(iPosition,1.0));
}