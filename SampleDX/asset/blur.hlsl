
struct VSInput
{
    float2 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

Texture2D sourceTexture : register(t0);
SamplerState linearSampler : register(s0);

PSInput vs(VSInput input)
{
    PSInput output;
    
    output.position = float4(input.position, 0.0, 1.0);
    output.texcoord = input.texcoord;
    return output;
}

float4 ps(PSInput input) : SV_TARGET
{
    uint width, height;
    sourceTexture.GetDimensions(width, height);
    float2 texelSize = float2(1.0 / (float) width, 1.0 / (float) height);

    float2 uv = input.texcoord;
    
    float4 color = float4(0, 0, 0, 0);
    for (int x = -2; x <= 2; x++)
    {
		[unroll]	
        for (int y = -2; y <= 2; y++)
        {
            color += sourceTexture.Sample(linearSampler, uv + (float2(x, y) * texelSize));
        }
    }
    return color * 0.04;
}