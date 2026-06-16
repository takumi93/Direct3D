#pragma once

#include <GameLibrary.h>

class SampleGame : public GameLibrary::Game
{
public:
	SampleGame(const GameLibrary::ProjectSettings& settings);

protected:
	void OnInitialize() override;
	void OnUpdate() noexcept override;
	void OnRender() noexcept override;

	// 定数バッファーを介してシェーダーに毎フレーム送る行列データを表します。
	struct MatricesPerFrame {
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4X4 projectionMatrix;
		DirectX::XMFLOAT4X4 worldViewProjectionMatrix;

		// カメラの位置座標
		DirectX::XMFLOAT4 viewPosition = DirectX::XMFLOAT4(0, 1, -10, 1);

		// ライトの位置座標(平行光源 w = 0, 点光源 w = 1)
		DirectX::XMFLOAT4 lightPosition = DirectX::XMFLOAT4(1.0f, 2.0f, -2.0f, 1.0f);

		// マテリアルの表面カラー
		DirectX::XMFLOAT4 materialDiffuse = DirectX::XMFLOAT4(1, 1, 0, 1);

		// 鏡面反射の色(r, g, b) = (x, y, z)
		DirectX::XMFLOAT3 materialSpecularColor = DirectX::XMFLOAT3(1, 1, 1);
		// 鏡面反射の強さ(float) = w
		float materialSpecularPower = 1;
	};
	MatricesPerFrame matricesPerFrame = {};

	// バッファー
	std::unique_ptr<GameLibrary::VertexBuffer> vertexBuffer;
	std::unique_ptr<GameLibrary::IndexBuffer> indexBuffer;
	// インデックスの数
	UINT indexCount = 0;
	// 定数バッファー
	std::unique_ptr<GameLibrary::ConstantBuffer> constantBuffer;
	// シェーダー
	std::unique_ptr<GameLibrary::BasicVertexShader> vertexShader;
	std::unique_ptr<GameLibrary::BasicGeometryShader> geometryShader;
	std::unique_ptr<GameLibrary::BasicPixelShader> pixelShader;
	// 入力レイアウト
	std::unique_ptr<GameLibrary::InputLayout> inputLayout;
	// テクスチャー
	std::unique_ptr<GameLibrary::Texture2D> texture;
};
