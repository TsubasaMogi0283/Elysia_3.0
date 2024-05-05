#include "CopyImage.hlsli"

//三角形で覆われればOK
static const uint32_t NUM_VERTEX = 3;
static const float32_t4 POSITIONS[NUM_VERTEX] ={
    //左上
    { -1.0f, 1.0f, 0.0f, 1.0f },
    //右上
    { 3.0f, 1.0f, 0.0f, 1.0f },
    //左下
    { -1.0f, -3.0f, 0.0f, 1.0f },
    
};

static const float32_t2 TEXCOORDS[NUM_VERTEX] ={
    //左上
    { 0.0f, 0.0f },
    //右上
    { 2.0f, 0.0f },
    //左下
    { 0.0f, 2.0f },
};

struct CenterPosition
{
    float32_t4 position;
};

ConstantBuffer<CenterPosition> gPosition : register(b0);

VertexShaderOutput main(uint32_t vertexId : SV_VertexID)
{
    VertexShaderOutput output;
    
    //float32_t3 damyPosition = gPosition.position;
    output.position = POSITIONS[vertexId];
    output.texcoord = TEXCOORDS[vertexId];
    return output;
}