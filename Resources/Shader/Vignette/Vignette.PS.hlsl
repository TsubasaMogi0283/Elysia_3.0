#include "Vignette.hlsli"

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

struct Vignette
{
	//倍
    float scale;
    //乗
    float pow;
	//色
    float3 color; 
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSample : register(s0);
ConstantBuffer<Vignette> gVignette : register(b0);
//hlsliでは絶対に日本語を使わないでね



PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSample, input.texcoord);
    //周囲を0に、中心になるほど明るくなるように計算で調整
    float2 current = input.texcoord * (1.0f - input.texcoord.yx);
    //currentだけで計算すると中心の最大値が0.0625で暗すぎるのでScaleで調整。
    float vignette = current.x * current.y * gVignette.scale;
    vignette = saturate(pow(vignette, gVignette.pow));
    //係数として乗算
    output.color.rgb *= vignette;

    return output;
}