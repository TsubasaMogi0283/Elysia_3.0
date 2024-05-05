#include "CopyImage.hlsli"
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSample : register(s0);

//Material...色など三角形の表面の材質をけっていするもの
struct Material
{
    float32_t4 color;
};

//ConstantBuffer<構造体>変数名:register(b0);
ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput
{

    float32_t4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float32_t4 textureColor = gTexture.Sample(gSample, input.texcoord);
    output.color = gMaterial.color * textureColor;

    return output;
}