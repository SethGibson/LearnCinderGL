#version 150
uniform mat4	ciModelViewProjection;
uniform vec2	uBlurAxis;
uniform float	uBlurSize;
in vec4			ciPosition;
in vec2			ciTexCoord0;

out vec2		UV;
out vec2		BlurSamples[14];

void main()
{
	gl_Position = ciModelViewProjection * ciPosition;
	UV = ciTexCoord0;

	BlurSamples[0] = ciTexCoord0 + vec2(-0.014*uBlurAxis.x*uBlurSize, -0.014*uBlurAxis.y*uBlurSize);
    BlurSamples[1] = ciTexCoord0 + vec2(-0.012*uBlurAxis.x*uBlurSize, -0.012*uBlurAxis.y*uBlurSize);
    BlurSamples[2] = ciTexCoord0 + vec2(-0.010*uBlurAxis.x*uBlurSize, -0.010*uBlurAxis.y*uBlurSize);
    BlurSamples[3] = ciTexCoord0 + vec2(-0.008*uBlurAxis.x*uBlurSize, -0.008*uBlurAxis.y*uBlurSize);
    BlurSamples[4] = ciTexCoord0 + vec2(-0.006*uBlurAxis.x*uBlurSize, -0.006*uBlurAxis.y*uBlurSize);
    BlurSamples[5] = ciTexCoord0 + vec2(-0.004*uBlurAxis.x*uBlurSize, -0.004*uBlurAxis.y*uBlurSize);
    BlurSamples[6] = ciTexCoord0 + vec2(-0.002*uBlurAxis.x*uBlurSize, -0.002*uBlurAxis.y*uBlurSize);
    BlurSamples[7] = ciTexCoord0 + vec2(0.002*uBlurAxis.x*uBlurSize, 0.002*uBlurAxis.y*uBlurSize);
    BlurSamples[8] = ciTexCoord0 + vec2(0.004*uBlurAxis.x*uBlurSize, 0.004*uBlurAxis.y*uBlurSize);
    BlurSamples[9] = ciTexCoord0 + vec2(0.006*uBlurAxis.x*uBlurSize, 0.006*uBlurAxis.y*uBlurSize);
    BlurSamples[10] = ciTexCoord0 + vec2(0.008*uBlurAxis.x*uBlurSize, 0.008*uBlurAxis.y*uBlurSize);
    BlurSamples[11] = ciTexCoord0 + vec2(0.010*uBlurAxis.x*uBlurSize, 0.010*uBlurAxis.y*uBlurSize);
    BlurSamples[12] = ciTexCoord0 + vec2(0.012*uBlurAxis.x*uBlurSize, 0.012*uBlurAxis.y*uBlurSize);
    BlurSamples[13] = ciTexCoord0 + vec2(0.014*uBlurAxis.x*uBlurSize, 0.014*uBlurAxis.y*uBlurSize);
}