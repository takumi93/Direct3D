#include "SampleGame.h"
#include <iterator>
#include <comdef.h>
#include <wincodec.h>

using namespace GameLibrary;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace
{
	//// 頂点データの配列
	//VertexPositionNormalTexture vertices[] = {
	//	// Front
	//	{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
	//	{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
	//	{ { 1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
	//	{ {-1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
	//	// Back
	//	{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f } },
	//	{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f } },
	//	{ {-1.0f,-1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 1.0f } },
	//	{ { 1.0f,-1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f } },
	//	// Right
	//	{ { 1.0f, 1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	//	{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	//	{ { 1.0f,-1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	//	{ { 1.0f,-1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	//	// Left
	//	{ {-1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
	//	{ {-1.0f, 1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
	//	{ {-1.0f,-1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
	//	{ {-1.0f,-1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	//	// UP
	//	{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
	//	{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
	//	{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
	//	{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
	//	// DOWN
	//	{ { 1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f } },
	//	{ {-1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 0.0f } },
	//	{ { 1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 1.0f } },
	//	{ {-1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 1.0f } },
	//};
	//// インデックスデータの配列
	//UINT32 indices[] = {
	//	 0,  1,  2,  3,  2,  1,	// Front
	//	 4,  5,  6,  7,  6,  5,	// Back
	//	 8,  9, 10, 11, 10,  9,	// Right
	//	12, 13, 14, 15, 14, 13,	// Left
	//	16, 17, 18, 19, 18, 17,	// Top
	//	20, 21, 22, 23, 22, 21,	// Bottom
	//};

	//// 画像データのダミー
	//constexpr uint32_t source[16] = {
	//	0xFF0000FF, 0xFF000000, 0xFF0000FF, 0xFF000000,
	//	0xFF000000, 0xFF0000FF, 0xFF000000, 0xFF0000FF,
	//	0xFF0000FF, 0xFF000000, 0xFF0000FF, 0xFF000000,
	//	0xFF000000, 0xFF0000FF, 0xFF000000, 0xFF0000FF,
	//};

	//// 画像データのダミー
	//constexpr uint8_t source[][4] = {
	//	{0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF},
	//	{0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF},
	//	{0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF},
	//	{0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF},
	//};

	constexpr Vertex_Sprite quadVertices[] = {
	{ { -0.5f, +0.5f, +0.0f, }, { 0.0f, 0.0f, }, },
	{ { +0.5f, +0.5f, +0.0f, }, { 1.0f, 0.0f, }, },
	{ { -0.5f, -0.5f, +0.0f, }, { 0.0f, 1.0f, }, },
	{ { +0.5f, -0.5f, +0.0f, }, { 1.0f, 1.0f, }, },
	};
	constexpr uint32_t quadIndices[] = {
		0, 1, 2,
		3, 2, 1,
	};
}

SampleGame::SampleGame(const ProjectSettings& settings)
	: Game(settings)
{

}

void SampleGame::OnInitialize()
{
	contentManager = std::make_unique<ContentManager>(graphics->GetDevice());

	constantBufferPerFrame = std::make_shared<ConstantBuffer>(graphics->GetDevice(),
		static_cast<UINT>(sizeof constantsPerFrame));

	constantBufferPerDraw = std::make_shared<ConstantBuffer>(graphics->GetDevice(),
		static_cast<UINT>(sizeof constantsPerDraw));

	// マテリアル
	albedoColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 頂点バッファー
	vertexBuffer = std::make_shared<StaticVertexBuffer>(
		graphics->GetDevice(),
		Vertex_Sprite::GetSize(),
		static_cast<UINT>(std::size(quadVertices)),
		quadVertices);
	vertexOffset = 0;

	// インデックス バッファー
	indexBuffer = std::make_shared<IndexBuffer>(
		graphics->GetDevice(),
		IndexFormat::UInt32, static_cast<UINT>(std::size(quadIndices)),
		quadIndices);
	indexOffset = 0;

	// シェーダー
	vertexShader = spriteVertexShader.get();
	geometryShader = spriteGeometryShader.get();
	pixelShader = spritePixelShader.get();

	// マテリアル
	constantBufferPerMaterial = std::make_shared<ConstantBuffer>(graphics->GetDevice(),
		static_cast<UINT>(sizeof constantsPerMaterial));

	// テクスチャ
	texture = contentManager->LoadTexture2D(L"Sample.png");
	texture->SetFilter(D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_POINT);
	texture->Apply();

	// 入力レイアウト
	inputLayout = std::make_shared<InputLayout_Sprite>(graphics->GetDevice());

	startIndexLocation = 0;
	baseVertexLocation = 0;
}

void SampleGame::OnUpdate() noexcept
{
	
}

void SampleGame::OnRender() noexcept
{
	OutputDebugStringA("SampleGame::OnRender\n");

	// メイン カメラ
	const auto cameraWorldMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&cameraRotation));
	const auto cameraForward = cameraWorldMatrix.r[2];
	const auto cameraUp = cameraWorldMatrix.r[1];
	auto matrixView = XMMatrixLookToLH(XMLoadFloat3(&cameraPosition), cameraForward, cameraUp);
	XMStoreFloat4x4(&constantsPerFrame.MatrixView, XMMatrixTranspose(matrixView));

	// プロジェクション行列を計算
	const auto aspectRatio = GetWidth() / static_cast<float>(GetHeight());
	auto matrixProjection = XMMatrixIdentity();
	if (orthographic) {
		matrixProjection = XMMatrixOrthographicLH(
			orthographicSize * aspectRatio, orthographicSize, clipPlaneNear, clipPlaneFar);
	}
	else {
		matrixProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(fieldOfView), aspectRatio, clipPlaneNear, clipPlaneFar);
	}
	XMStoreFloat4x4(&constantsPerFrame.MatrixProjection, XMMatrixTranspose(matrixProjection));

	XMStoreFloat4x4(&constantsPerFrame.MatrixViewProjection, XMMatrixTranspose(matrixView * matrixProjection));
	constantBufferPerFrame->UpdateSubresource(&constantsPerFrame);

	// ゲーム オブジェクト
	const auto matrixWorld = XMMatrixTransformation(
		XMVectorZero(), XMQuaternionIdentity(), localScale,
		XMVectorZero(), localRotation,
		localPosition);
	XMStoreFloat4x4(&constantsPerDraw.MatrixWorld, XMMatrixTranspose(matrixWorld));
	constantBufferPerDraw->UpdateSubresource(&constantsPerDraw);

	// Mesh
	ID3D11Buffer* const vertexBuffers[] = { vertexBuffer->GetNativePointer(), };
	const UINT strides[] = { vertexBuffer->GetStride(), };
	const UINT offsets[] = { vertexOffset, };
	graphics->GetDeviceContext()->IASetVertexBuffers(0, static_cast<UINT>(std::size(vertexBuffers)), vertexBuffers, strides, offsets);
	graphics->GetDeviceContext()->IASetInputLayout(inputLayout->GetNativePointer());
	graphics->GetDeviceContext()->IASetIndexBuffer(indexBuffer->GetNativePointer(), indexBuffer->GetFormat(), indexOffset);

	// マテリアル
	XMStoreFloat4(&constantsPerMaterial.Albedo, XMColorSRGBToRGB(XMLoadFloat4(&albedoColor)));
	constantBufferPerMaterial->UpdateSubresource(&constantsPerMaterial);
	// Constant buffer
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerDraw->GetNativePointer(),
			constantBufferPerFrame->GetNativePointer(),
		};
		//deviceContext->VSSetConstantBuffers(
		//	0, static_cast<UINT>(std::size(constantBuffers)), constantBuffers);
	}
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerDraw->GetNativePointer(),
			constantBufferPerFrame->GetNativePointer(),
		};
		graphics->GetDeviceContext()->GSSetConstantBuffers(
			0, static_cast<UINT>(std::size(constantBuffers)), constantBuffers);
	}
	{
		ID3D11Buffer* const constantBuffers[] = {
			constantBufferPerMaterial->GetNativePointer(),
		};
		graphics->GetDeviceContext()->PSSetConstantBuffers(
			0, static_cast<UINT>(std::size(constantBuffers)), constantBuffers);
	}

	// Texture
	ID3D11ShaderResourceView* const textures[] = { texture->GetView(), };
	graphics->GetDeviceContext()->VSSetShaderResources(0, static_cast<UINT>(std::size(textures)), textures);
	graphics->GetDeviceContext()->GSSetShaderResources(0, static_cast<UINT>(std::size(textures)), textures);
	graphics->GetDeviceContext()->PSSetShaderResources(0, static_cast<UINT>(std::size(textures)), textures);
	ID3D11SamplerState* const samplerStates[] = { texture->GetSamplerState(), };
	graphics->GetDeviceContext()->VSSetSamplers(0, static_cast<UINT>(std::size(samplerStates)), samplerStates);
	graphics->GetDeviceContext()->GSSetSamplers(0, static_cast<UINT>(std::size(samplerStates)), samplerStates);
	graphics->GetDeviceContext()->PSSetSamplers(0, static_cast<UINT>(std::size(samplerStates)), samplerStates);

	// Shaders
	vertexShader->Apply(graphics->GetDeviceContext());
	geometryShader->Apply(graphics->GetDeviceContext());
	pixelShader->Apply(graphics->GetDeviceContext());

	graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphics->GetDeviceContext()->DrawIndexed(indexBuffer->GetCount(), startIndexLocation, baseVertexLocation);
}
