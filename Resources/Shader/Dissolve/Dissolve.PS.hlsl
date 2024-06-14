#include "Dissolve.hlsli"


struct Dissolve{
    float therashold;
};

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);

SamplerState gSample : register(s0);

ConstantBuffer<Dissolve> gDissolve : register(b0);

//hlsliでは絶対に日本語を使わないでね


struct PixelShaderOutput{
    float4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input){
    PixelShaderOutput output;
    
    float mask = gMaskTexture.Sample(gSample, input.texcoord);
    //maskの値が設定したtherashold以下の場合はdiscard
    
    if (mask <= gDissolve.therashold)
    {
        discard;
        
    }
    
    float edge = 1.0f - smoothstep(0.5f, 0.53f, mask);
    output.color = gTexture.Sample(gSample, input.texcoord);
    
    
    //Edgeっぽいほど指定した色を加算
    output.color.rgb += edge * float3(1.0f, 1.0f, 1.0f);
    
    return output;
}