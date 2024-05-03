//#include "CopyImage.hlsli"
//Texture2D<float32_t4> gTexture : register(t0);
//SamplerState gSample : register(s0);
//
//
//struct PixelShaderOutput{
//    float32_t4 color : SV_TARGET0;
//};
//
////設定されたTextureの内容を出力するだけ
//PixelShaderOutput main(VertexShaderOutput input){
//    PixelShaderOutput output;
//    
//    output.color = gTexture.Sample(gSample, input.texcoord);
//    
//    return output;
//}


////PixelShader
//ここからの出力は色で、OutputManagerに送られて
//ブレンド処理も行い画面に色が打ち込まれる
 
 
//テクスチャを張り付けたり、ライティングを行ったり出来る。
//最も重要なShader

//ラスタライザー
//		↓VertexShadeOutput
//ピクセルシェーダー
//		↓PixelShadeOutput
//	 出力結合




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


PixelShaderOutput main()
{
    PixelShaderOutput output;
    output.color = gMaterial.color;

    return output;
}