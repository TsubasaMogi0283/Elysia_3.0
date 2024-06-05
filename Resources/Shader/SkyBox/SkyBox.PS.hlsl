#include "SkyBox.hlsli"


struct PixelShaderOutput{
    float4 color : SV_TARGET0;
};

struct Material{
    float4 color;
};


TextureCube<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutput main(VertexShaderOutput input){
    PixelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    output.color = textureColor * gMaterial.color;
    return output;
}