#include "BasicShader.hlsli"

static const float4 MaterialColor = float4(1, 238 / 255.0f, 0, 1);

// ピクセルシェーダーの入力データ
float4 main(PSInput input) : SV_TARGET
{
    return input.color * MaterialColor;
}