#include "BasicShader.hlsli"

// 平行光源の向き（の逆ベクトル）
static const float3 light = float3(1.0f, 2.0f, -2.0f);

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
		
		// ランバート反射
		
		// ワールド変換後の法線ベクトル
        float3 worldNormal = normalize(mul(input[i].normal, (float3x3)WorldViewProjection));
		
		// lightとworldNormalを使って計算する
        float diffuse = max(dot(normalize(light), worldNormal), 0.0f);
		
        element.color = float4(diffuse, diffuse, diffuse, 1.0f);
		output.Append(element);
	}
}