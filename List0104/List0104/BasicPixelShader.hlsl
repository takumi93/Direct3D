#include "BasicShader.hlsli"

float4 main(PSInput input) : SV_TARGET
{
    // ライトへのベクトル
    float4 light = LightPosition - input.worldPosition * LightPosition.w;
    // light と worldNormal を使って計算する
    float diffuse = max(dot(normalize(light.xyz), input.worldNormal), 0.0f);
    
    return diffuse * MaterialColor;
}