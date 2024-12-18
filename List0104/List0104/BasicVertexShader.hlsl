#include "BasicShader.hlsli"

// 頂点シェーダーの出力データ
VSOutput main(VSInput input)
{
    VSOutput output;

    output.position = mul(input.position, WorldViewProjection);
    output.color = input.color;
    return output;
}
