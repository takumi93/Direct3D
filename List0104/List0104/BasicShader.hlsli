// 毎フレーム更新する定数バッファー
cbuffer ConstantBufferPerFrame
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WorldViewProjection;
    float4 MaterialColor;
    float4 LightPosition;
};

// 頂点シェーダーの入力データ
struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
};

// 頂点シェーダーの出力データ
struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

// ジオメトリーシェーダーの入力データ
typedef VSOutput GSInput;

// ジオメトリーシェーダーの出力データ
struct GSOutput
{
    float4 position : SV_POSITION;      // WVP変換
    float4 worldPosition : POSITION;    // 法線ベクトル(ワールド空間)
    float3 worldNormal : NORMAL;        // 位置座標（ワールド空間）
};

// ピクセルシェーダーの入力データ(ピクセルシェーダと頂点シェーダの型が一緒のため)
typedef GSOutput PSInput;