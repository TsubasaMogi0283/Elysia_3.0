#include "FullScreen.hlsli"
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
    output.color = gTexture.Sample(gSample, input.texcoord);
    float32_t value = dot(output.color.rgb, float32_t3(0.2125f, 0.7154f, 0.0721f));
    
    output.color.rgb = float32_t3(value, value, value);
    //セピア
    //output.color.rgb = value*float32_t3(1.0f,74.0f/107.0f,43.0f/107.0f);
    
    
    return output;
}