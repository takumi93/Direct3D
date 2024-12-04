#include "BasicShader.hlsli"

// ピクセルシェーダーの入力データ
float4 main(PSInput input) : SV_TARGET
{
    return MaterialColor;
}