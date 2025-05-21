#include "BasicShader.hlsli"

[maxvertexcount(3)]
void main(
	triangle GSInput input[3],
	inout TriangleStream< GSOutput > output)
{
    for (uint i = 0; i < 3; i++)
    {
        GSOutput element;
		// WVP変換
        element.position = mul(input[i].position, WorldViewProjection);
        // 法線ベクトル(ワールド空間)
        element.worldPosition = mul(input[i].position, World);
        // 位置座標（ワールド空間）
        element.worldNormal = normalize(mul(input[i].normal, (float3x3) World));
        element.texCoord = input[i].texCoord;
        output.Append(element);
    }
}