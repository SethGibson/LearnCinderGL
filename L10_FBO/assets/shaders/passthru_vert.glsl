#version 150
uniform mat4	ciModelViewProjection;
in vec4			ciPosition;
in vec2			ciTexCoord0;

out vec2		UV;

void main()
{
	UV = ciTexCoord0;
	gl_Position = ciModelViewProjection * ciPosition;
}