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
    
    output.color = gTexture.Sample(gSample, input.texcoord);
    
    return output;
}