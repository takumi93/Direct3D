#include "BasicShader.hlsli"

// 頂点シェーダーの出力データ
VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = input.position;
    output.normal = input.normal;
    output.texCoord = input.texCoord;
    return output;
}
