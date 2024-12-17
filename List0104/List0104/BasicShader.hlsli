// 毎フレーム更新する定数バッファー
cbuffer ConstantBufferPerFrame
{
    float4x4 World;
    float4x4 View;
    float4 MaterialColor;
};

// 頂点シェーダーの入力データ
struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR0;
};

// 頂点シェーダーの出力データ
struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

// ピクセルシェーダーの入力データ(ピクセルシェーダと頂点シェーダの型が一緒のため)
typedef VSOutput PSInput;