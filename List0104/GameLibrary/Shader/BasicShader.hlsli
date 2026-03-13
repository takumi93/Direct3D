// 毎フレーム更新する定数バッファー
cbuffer ConstantBufferPerFrame
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WorldViewProjection;
    float4 MaterialColor;
    
    // カメラの位置座標
    float4 ViewPosition;
	// ライトの位置座標(平行光源 w = 0, 点光源 w = 1)
    float4 LightPosition;
	// マテリアルの表面カラー
    float4 MaterialDiffuse;

	// マテリアルの鏡面反射カラー
    float3 MaterialSpecularColor;
	// マテリアルの鏡面反射の強さ
    float MaterialSpecularPower;
};

// 頂点シェーダーの入力データ
struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// 頂点シェーダーの出力データ
struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

// ジオメトリーシェーダーの入力データ
typedef VSOutput GSInput;

// ジオメトリーシェーダーの出力データ
struct GSOutput
{
    float4 position : SV_POSITION;      // WVP変換
    float4 worldPosition : POSITION;    // 法線ベクトル(ワールド空間)
    float3 worldNormal : NORMAL;        // 位置座標（ワールド空間）
    float2 texCoord : TEXCOORD;
};

// ピクセルシェーダーの入力データ(ピクセルシェーダと頂点シェーダの型が一緒のため)
typedef GSOutput PSInput;