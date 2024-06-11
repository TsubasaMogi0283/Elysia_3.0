#include "Dissolve.hlsli"




Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);

SamplerState gSample : register(s0);

//hlsliでは絶対に日本語を使わないでね


struct PixelShaderOutput{
    float4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input){
    PixelShaderOutput output;
    
    float mask = gMaskTexture.Sample(gSample, input.texcoord);
    //maskの値が0.5以下の場合はdiscard
    if (mask <= 0.5f){
        discard;
        
    }
    
    output.color = gTexture.Sample(gSample, input.texcoord);
    
    return output;
}