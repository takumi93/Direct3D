#include "BasicShader.hlsli"

// 平行光源の向き(w = 0.0f) or 点光源の位置座標(w = 1.0f)
static const float4 lightPosition = float4(1.0f, 2.0f, -2.0f, 1.0f);

float4 main(PSInput input) : SV_TARGET
{
    // ライトへのベクトル
    float4 light = lightPosition - input.worldPosition * lightPosition.w;
    // light と worldNormal を使って計算する
    float diffuse = max(dot(normalize(light.xyz), input.worldNormal), 0.0f);
    
    return diffuse * MaterialColor;
}