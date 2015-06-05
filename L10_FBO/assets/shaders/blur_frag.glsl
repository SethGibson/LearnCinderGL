#version 150
uniform sampler2D	uTextureSampler;
uniform float		uBlurStrength;
in vec2				UV;
in vec2				BlurSamples[14];

out vec4 FragColor;

void main()
{
	vec4 sample = vec4(0);

    sample += texture(uTextureSampler, BlurSamples[ 0])*0.0044299121055113265;
    sample += texture(uTextureSampler, BlurSamples[ 1])*0.00895781211794;
    sample += texture(uTextureSampler, BlurSamples[ 2])*0.0215963866053;
    sample += texture(uTextureSampler, BlurSamples[ 3])*0.0443683338718;
    sample += texture(uTextureSampler, BlurSamples[ 4])*0.0776744219933;
    sample += texture(uTextureSampler, BlurSamples[ 5])*0.115876621105;
    sample += texture(uTextureSampler, BlurSamples[ 6])*0.147308056121;
    sample += texture(uTextureSampler, UV         )*0.159576912161;
    sample += texture(uTextureSampler, BlurSamples[ 7])*0.147308056121;
    sample += texture(uTextureSampler, BlurSamples[ 8])*0.115876621105;
    sample += texture(uTextureSampler, BlurSamples[ 9])*0.0776744219933;
    sample += texture(uTextureSampler, BlurSamples[10])*0.0443683338718;
    sample += texture(uTextureSampler, BlurSamples[11])*0.0215963866053;
    sample += texture(uTextureSampler, BlurSamples[12])*0.00895781211794;
    sample += texture(uTextureSampler, BlurSamples[13])*0.0044299121055113265;

	FragColor = vec4(sample.rgb*uBlurStrength,sample.a);
}