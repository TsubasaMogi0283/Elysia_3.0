#include "HSVFilter.hlsli"


//hlsliでは絶対に日本語を使わないでね
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};


//ConstantBuffer<RandomEngine> gRandomEngine : register(b0);

Texture2D<float4> gTexture : register(t0);
SamplerState gSample : register(s0);



//変換関数
//https://ja.wikipedia.org/wiki/HSV%E8%89%B2%E7%A9%BA%E9%96%93
//RGBからHSVへ
float3 RGBToHSV(float3 color){
    float red = color.r;
    float green = color.g;
    float blue = color.b;
    
    
    float max = red > green ? red : green;
    //if(red>green){
    //  red
    //}
    //else {
    //  green
    //}
    //
    max = max > blue ? max : blue;
    float min = red < green ? red : green;
    min = min < blue ? min : blue;
    float hue = max - min;
    if (hue > 0.0f)
    {
        if (max == red)
        {
            hue = (green - blue) / hue;
            if (hue < 0.0f)
            {
                hue += 6.0f;
            }
        }
        else if (max == green)
        {
            hue = 2.0f + (blue - red) / hue;
        }
        else
        {
            hue = 4.0f + (red - green) / hue;
        }
    }
    hue /= 6.0f;
    float saturate = (max - min);
    if (max != 0.0f)
        saturate /= max;
    float value = max;
    
    float3 hsv = { hue, saturate, value };
    
    
    return hsv;
}
//HSVからRGBへ
float3 HSVToRGB(float3 hsv)
{
    float red = hsv.b;
    float green = hsv.b;
    float blue = hsv.b;
    
    float hue = hsv.r;
    float saturate = hsv.g;
    
    
    if (saturate > 0.0f){
        hue *= 6.0f;
        int i = (int) hue;
        float f = hue - (float) i;
        switch (i){
            default:
            case 0:
                green *= 1 - saturate * (1 - f);
                blue *= 1 - saturate;
                break;
            case 1:
                red *= 1 - saturate * f;
                blue *= 1 - saturate;
                break;
            case 2:
                red *= 1 - saturate;
                blue *= 1 - saturate * (1 - f);
                break;
            case 3:
                red *= 1 - saturate;
                green *= 1 - saturate * f;
                break;
            case 4:
                red *= 1 - saturate * (1 - f);
                green *= 1 - saturate;
                break;
            case 5:
                green *= 1 - saturate;
                blue *= 1 - saturate * f;
                break;
        }
    }
    
    float3 rgb = { red,green,blue};
    return rgb;
}


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float4 textureColor = gTexture.Sample(gSample, input.texcoord);
    float3 hsv = RGBToHSV(textureColor.rgb);
    
    
    float3 rgb = HSVToRGB(hsv);
    
    output.color.rgb = rgb;
    output.color.a = textureColor.a;
    
    return output;
}