// vertex shader input
struct VSInput
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

// vertex shader output
struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// transform constant buffer
cbuffer TransformBuffer : register(b0)
{
    // world matrix
    row_major matrix world;
};

// camera constant buffer
cbuffer CameraBuffer : register(b1)
{
    row_major matrix view;
    row_major matrix projection;
};

VSOutput main( VSInput input )
{
    VSOutput output;
    
    // WVP(World-View-Projection) Transformation
    output.position = mul(float4(input.position, 1), world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);

    // uv
    output.uv = input.uv;
    
    return output;
}