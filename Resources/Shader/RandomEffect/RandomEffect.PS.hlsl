#include "RandomEffect.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSample : register(s0);

//hlsliでは絶対に日本語を使わないでね
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};


//rand2To1dないやんけ・・
//調べてみたらこのページにあった！！
//https://www.ronja-tutorials.com/post/024-white-noise/
float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233))
{
    float2 smallValue = sin(value);
    float random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return random;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float random = rand2dTo1d(input.texcoord);
    //色にする
    output.color = float4(random, random, random, 1.0f);
    
    return output;
}