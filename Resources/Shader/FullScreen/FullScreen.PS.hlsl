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


static const float32_t2 INDEX3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

//9個あって平均にするので1/9
static const float32_t KERNEL3x3[3][3] =
{
    { { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f } },
    { { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f } },
    { { 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f } },
};


static const float32_t2 INDEX5x5[5][5] =
{
    { { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f } },
    { { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
    { { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
    { { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
    { { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } },
    
};

//9個あって平均にするので1/25
static const float32_t KERNEL5x5[5][5] =
{
    { { 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f } },
    { { 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f } },
    { { 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f } },
    { { 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f } },
    { { 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f } },
    
    
};

static const int NONE = 0;
static const int MONOCHROME = 1;
static const int SEPIA = 2;
static const int VIGNETTE = 3;
static const int BOX_FILTER3x3 = 4;
static const int BOX_FILTER5x5 = 5;




PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSample, input.texcoord);
    float32_t r = 0.2125f;
    float32_t g = 0.7154f;
    float32_t b = 0.0721f;
        
        
    float32_t value = dot(output.color.rgb, float32_t3(r, g, b));
    //モノクロ(グレースケール)
    if (gEffect.type == MONOCHROME){
        output.color.rgb = float32_t3(value, value, value);
    }
    //セピア
    else if (gEffect.type == SEPIA){
        SepiaColor sepiaColor = { 1.0f, 74.0f / 107.0f, 43.0f / 107.0f };
        output.color.rgb = value * float32_t3(sepiaColor.r, sepiaColor.g, sepiaColor.b);
    }
    //Vignette
    else if (gEffect.type == VIGNETTE){
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
    else if (gEffect.type == BOX_FILTER3x3){
        //uvStepSizeの算出
        uint32_t width, height;
        gTexture.GetDimensions(width, height);
        //rcp...逆数にする。正確では無いけど処理が速いよ
        float32_t2 uvStepSIze = float32_t2(rcp(width), rcp(height));
        output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
        output.color.a = 1.0f;
        
        for (int32_t x = 0; x < 3; ++x){
            for (int32_t y = 0; y < 3; ++y){
                float32_t2 texcoord = input.texcoord + INDEX3x3[x][y] * uvStepSIze;
                float32_t3 fetchColor = gTexture.Sample(gSample, texcoord).rgb;
                output.color.rgb += fetchColor * KERNEL3x3[x][y];

            }
        }
        

    }
    else if (gEffect.type == BOX_FILTER5x5)
    {
        //uvStepSizeの算出
        uint32_t width, height;
        gTexture.GetDimensions(width, height);
        //rcp...逆数にする。正確では無いけど処理が速いよ
        float32_t2 uvStepSIze = float32_t2(rcp(width), rcp(height));
        output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
        output.color.a = 1.0f;
        
        for (int32_t x = 0; x < 5; ++x)
        {
            for (int32_t y = 0; y < 5; ++y)
            {
                float32_t2 texcoord = input.texcoord + INDEX5x5[x][y] * uvStepSIze;
                float32_t3 fetchColor = gTexture.Sample(gSample, texcoord).rgb;
                output.color.rgb += fetchColor * KERNEL5x5[x][y];

            }
        }
        

    }
    
    
    return output;
}