#include "FullScreen.hlsli"
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSample : register(s0);


struct Effect{
    int32_t type;
};

//ConstantBuffer<構造体>変数名:register(b0);
ConstantBuffer<Effect> gEffect : register(b0);
struct PixelShaderOutput{
    float32_t4 color : SV_TARGET0;
};

struct SepiaColor{
    float32_t r;
    float32_t g;
    float32_t b;
};


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSample, input.texcoord);
    float32_t r = 0.2125f;
    float32_t g = 0.7154f;
    float32_t b = 0.0721f;
        
        
    float32_t value = dot(output.color.rgb, float32_t3(r, g, b));
    //モノクロ(グレースケール)
    if (gEffect.type==1){
       
        output.color.rgb = float32_t3(value, value, value);
    }
    else if (gEffect.type==2){
        SepiaColor sepiaColor = { 1.0f, 74.0f / 107.0f, 43.0f / 107.0f };
        //セピア
        output.color.rgb = value * float32_t3(sepiaColor.r, sepiaColor.g, sepiaColor.b);
        
    }
        
    
    
    
    
    return output;
}