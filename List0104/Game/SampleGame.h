#pragma once

#include <GameLibrary.h>
#include <memory>

class SampleGame : public GameLibrary::Game
{
public:
	SampleGame(const GameLibrary::ProjectSettings& settings);

protected:
	void OnInitialize() override;
	void OnUpdate() noexcept override;
	void OnRender() noexcept override;

private:
	std::unique_ptr<GameLibrary::ContentManager> contentManager;

	// メイン カメラ
	DirectX::XMFLOAT3 cameraPosition = { 0, 1, -10 };
	DirectX::XMFLOAT4 cameraRotation = { 0, 0, 0, 1 };
	// プロジェクション
	float fieldOfView = 60.0f;
	float clipPlaneNear = 0.3f;
	float clipPlaneFar = 1000;
	bool orthographic = false;
	float orthographicSize = 10;

	struct ConstantsPerFrame
	{
		DirectX::XMFLOAT4X4 MatrixView;
		DirectX::XMFLOAT4X4 MatrixProjection;
		DirectX::XMFLOAT4X4 MatrixViewProjection;
	};
	ConstantsPerFrame constantsPerFrame = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerFrame;

	struct ConstantsPerDraw
	{
		DirectX::XMFLOAT4X4 MatrixWorld;
	};
	ConstantsPerDraw constantsPerDraw = {};
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerDraw;

	// オブジェクト
	DirectX::XMVECTOR localScale = { 1, 1, 1, };
	DirectX::XMVECTOR localRotation = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR localPosition = { 0, 0, 0, };
	DirectX::XMFLOAT4 albedoColor = { 1, 1, 1, 1 };

	// 頂点バッファー
	std::shared_ptr<GameLibrary::StaticVertexBuffer> vertexBuffer;
	UINT vertexOffset = 0;

	// インデックスバッファー
	std::shared_ptr<GameLibrary::IndexBuffer> indexBuffer;
	UINT indexOffset = 0;

	UINT startIndexLocation = 0;
	INT baseVertexLocation = 0;

	// 入力レイアウト
	std::shared_ptr<GameLibrary::InputLayout> inputLayout;

	// シェーダーを参照
	GameLibrary::VertexShader* vertexShader = nullptr;
	GameLibrary::GeometryShader* geometryShader = nullptr;
	GameLibrary::PixelShader* pixelShader = nullptr;

	// テクスチャー
	std::shared_ptr<GameLibrary::Texture2D> texture;

	// マテリアル
	struct ConstantsPerMaterial
	{
		DirectX::XMFLOAT4 Albedo;
	};
	ConstantsPerMaterial constantsPerMaterial = {};

	// 定数バッファー
	std::shared_ptr<GameLibrary::ConstantBuffer> constantBufferPerMaterial;
};
