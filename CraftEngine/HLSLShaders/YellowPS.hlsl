struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// texture
Texture2D diffuseTexture : register(t0);

// sampler
SamplerState diffuseSampler : register(s0);

// vertex shader output = pixel shader input
float4 main(VSOutput input) : SV_TARGET
{
    float4 diffuseColor = diffuseTexture.Sample(diffuseSampler, input.uv);
	return diffuseColor;
}
