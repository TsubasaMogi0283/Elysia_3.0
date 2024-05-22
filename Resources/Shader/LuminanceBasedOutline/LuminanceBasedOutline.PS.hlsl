#include "LuminanceBasedOutline.hlsli"




Texture2D<float4> gTexture : register(t0);
SamplerState gSample : register(s0);

//hlsliでは絶対に日本語を使わないでね

struct Effect{
    int type;
};


//ConstantBuffer<構造体>変数名:register(b0);
ConstantBuffer<Effect> gEffect : register(b0);


struct PixelShaderOutput{
    float4 color : SV_TARGET0;
};




//円周率
static const float PI = 3.1415926535f;

float gauss(float x, float y, float sigma){
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    //exp...ネイピア数eを求める関数
    return exp(exponent) * rcp(denominator);
    
}

float Luminance(float3 v){
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));

}

//exp(exponent)は以下と同じ
//const float e=2.71828182f;
//pow(e exponent)

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSample, input.texcoord);
        
    //uvStepSizeの算出
    uint width, height;
    gTexture.GetDimensions(width, height);
    //rcp...逆数にする。正確では無いけど処理が速いよ
    float2 uvStepSIze = float2(rcp(width), rcp(height));
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    //畳み込みとkernelが重要
    for (int x = 0; x < 3; ++x){
        for (int y = 0; y < 3; ++y){
            float2 texcoord = input.texcoord + INDEX3x3[x][y] * uvStepSIze;
            float3 fetchColor = gTexture.Sample(gSample, texcoord).rgb;
            output.color.rgb += fetchColor * KERNEL3x3[x][y];

        }
    }
        
    
    
    return output;
}