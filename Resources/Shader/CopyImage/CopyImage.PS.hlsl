#include "CopyImage.hlsli"
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSample : register(s0);


struct PixelShaderOutput{
    float32_t4 color : SV_TARGET0;
};

//設定されたTextureの内容を出力するだけ
PixelShaderOutput main(VertexShaderOutput input){
    PixelShaderOutput output;
    
    output.color = gTexture.Sample(gSample, input.texcoord);
    
    return output;
}
