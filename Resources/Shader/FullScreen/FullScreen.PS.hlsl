#include "FullScreen.hlsli"
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSample : register(s0);


struct Effect{
    int32_t type;
};

struct Vignette{
	//倍
    float scale;
    //乗
    float pow;
	
};

//ConstantBuffer<構造体>変数名:register(b0);
ConstantBuffer<Effect> gEffect : register(b0);
ConstantBuffer<Vignette> gVignette : register(b1);



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
    //セピア
    else if (gEffect.type==2){
        SepiaColor sepiaColor = { 1.0f, 74.0f / 107.0f, 43.0f / 107.0f };
        output.color.rgb = value * float32_t3(sepiaColor.r, sepiaColor.g, sepiaColor.b);
        
    }
    //Vignette
    else if (gEffect.type==3){
        //周囲を0に、中心になるほど明るくなるように計算で調整
        float32_t2 current = input.texcoord * (1.0f - input.texcoord.yx);
        //currentだけで計算すると中心の最大値が0.0625で暗すぎるのでScaleで調整。
        //この例では16倍にして1にしている
        float vignette = current.x * current.y * gVignette.scale;
        //とりあえず0.8乗でそれっぽくしてみた
        vignette = saturate(pow(vignette, gVignette.pow));
        //係数として乗算
        output.color.rgb *= vignette;

    }
        
    
    
    
    
    return output;
}