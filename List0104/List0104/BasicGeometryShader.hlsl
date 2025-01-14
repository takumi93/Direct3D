#include "BasicShader.hlsli"

// 平行光源の向き（w = 0.0f）or 点光源の位置座標（w = 1.0f）
static const float4 lightPosition = float4(1.0f, 2.0f, -2.0f, 0.0f);

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
        float3 worldNormal = normalize(mul(input[i].normal, (float3x3) WorldViewProjection));
        // ライトへのベクトル
        float4 light = lightPosition -input[i].position * lightPosition.w;
		// light と worldNormal を使って計算する
        float diffuse = max(dot(normalize(light.xyz), worldNormal), 0.0f);
		
        element.color = float4(diffuse, diffuse, diffuse, 1.0f);
		
        output.Append(element);
    }
}