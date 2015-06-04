#version 150
uniform mat4 ciModelViewProjection;
uniform mat4 ciNormalMatrix;
uniform mat4 ciModelMatrix;

in vec4 ciPosition;
in vec4 ciNormal;

out vec3 Normal;
out vec3 WorldPos;

void main()
{
	WorldPos = vec3(ciModelMatrix*ciPosition);
	Normal = vec3(ciModelMatrix*ciNormal);
	gl_Position = ciModelViewProjection * ciPosition;
}