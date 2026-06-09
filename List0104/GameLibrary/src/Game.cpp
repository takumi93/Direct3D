//=============================================================================
// Game.cpp
// 
//=============================================================================
#include <GameLibrary/Game.h>
#include <GameLibrary/Utility.h>
#include <DirectXMath.h>	// DirectXの算術ライブラリー
#include <wincodec.h>
#include "BasicVertexShader.h"
#include "BasicPixelShader.h"
#include "BasicGeometryShader.h"
#include <comdef.h>
#include <exception>
#include <iterator>
#include <new>
#include <comdef.h>

using namespace GameLibrary;
using namespace DirectX;
using namespace Microsoft::WRL;

Game::Game(const ProjectSettings& settings)
	: title(settings.Title), width(settings.Width), height(settings.Height)
{

}

void Game::Initialize(HWND hWnd)
{
	try {
		// ウィンドウを作成
		//window = std::make_shared<MainWindow>();
		window = hWnd;
		// グラフィックデバイスを作成
		graphics = std::make_shared<Graphics>();
		// スワップチェーンを作成
		swapChain = std::make_unique<SwapChain>(graphics, window, width, height);

		// ビューポート
		viewport = {
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<FLOAT>(swapChain->GetSwapChainDesc()->Width),
		.Height = static_cast<FLOAT>(swapChain->GetSwapChainDesc()->Height),
		.MinDepth = D3D11_MIN_DEPTH,
		.MaxDepth = D3D11_MAX_DEPTH,
		};
	}
	catch (const std::exception& error) {
		OutputDebugStringA("ERROR: ");
		OutputDebugStringA(error.what());
		OutputDebugStringA("\n");
		MessageBoxW(NULL, L"ウィンドウを作成できませんでした。", L"メッセージ", MB_OK);
		return;
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBoxW(NULL, TEXT("グラフィックデバイスを初期化できませんでした。"), TEXT("エラー"), MB_OK);
		return;
	}

	HRESULT hr = S_OK;
	const auto graphicsDevice = graphics->GetDevice();
	immediateContext = graphics->GetDeviceContext();

	try {
		// 頂点バッファーを作成
		//vertexBuffer.reset(new VertexBuffer(graphics, sizeof vertices));
		vertexBuffer = std::make_unique<VertexBuffer>(graphics, sizeof vertices);
		// インデックスバッファーを作成
		//indexBuffer.reset(new IndexBuffer(graphics, sizeof indices));
		indexBuffer = std::make_unique<IndexBuffer>(graphics, sizeof indices);

		// バッファーにデータを転送
		vertexBuffer->SetData(vertices);
		indexBuffer->SetData(indices);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("バッファーを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return;
	}

	constantBufferPerFrame.materialColor = XMFLOAT4(1, 238 / 255.0f, 0, 1);
	constantBufferPerFrame.lightPosition = XMFLOAT4(1, 2, -2, 1);

	
}

void Game::Update() noexcept
{
	OnUpdate();
}

void Game::Render() noexcept
{
	// レンダーターゲットを設定
	ID3D11RenderTargetView* renderTargetViews[] = { swapChain->GetRenderTargetView() };
	immediateContext->OMSetRenderTargets(
		_countof(renderTargetViews), renderTargetViews, 
		swapChain->GetDepthStencilView());
	
	// 画面をクリアー
	immediateContext->ClearRenderTargetView(swapChain->GetRenderTargetView(), clearColor);
	immediateContext->ClearDepthStencilView(swapChain->GetDepthStencilView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	OnRender();

	// バックバッファーに描画したイメージをディスプレイに表示
	HRESULT hr = S_OK;
	try {
		swapChain->Present(1);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(window,
			TEXT("グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。"),
			TEXT("エラー"), MB_OK);
	}

	OnRender();

	UINT presentFlags = 0;
	if (allowTearing) {
		presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
	}
	const auto presentParameters = DXGI_PRESENT_PARAMETERS{
		.DirtyRectsCount = 0,
		.pDirtyRects = nullptr,
		.pScrollRect = nullptr,
		.pScrollOffset = nullptr,
	};
	const auto hr = swapChain->Present(0, presentFlags, &presentParameters);
	if (FAILED(hr)) {
		return;
	}

	//// 位置座標
	//XMFLOAT3 position = { 0, 0, 0 };
	//// 回転
	//XMFLOAT4 rotation = {};
	////XMStoreFloat4(&rotation, XMQuaternionIdentity());
	//XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(-10.0f), XMConvertToRadians(-1.0f), 0));
	//// スケール
	//XMFLOAT3 scale = { 1, 1, 1 };

	//// カメラの位置座標
	//constexpr XMFLOAT3 eyePosition = { 0.0f, 1.0f, -3.0f };
	//// カメラの回転
	//XMFLOAT4 cameraRotation = {};
	//XMStoreFloat4(&cameraRotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(20.0f), 0, 0));

	//// ライトの位置座標
	//// 平行光源(w = 0.0f) or 点光源(w = 1.0f)
	//XMFLOAT4 lightPosition = { 1, 2, -2, 0 };

	//float time = 0;

	//// メッセージループを実行
	//MSG msg = {};
	//while (true) {
	//	time += 0.01f;

	//	// カメラのz軸回転
	//	float zAngle = 0.0f;

	//	// フレーム更新処理
	//	// 点光源
	//	if (GetAsyncKeyState(VK_SPACE)) {
	//		lightPosition.w = 1;
	//	}
	//	// 平行光源
	//	else {
	//		lightPosition.w = 0;
	//	}

	//	// オブジェクトのy軸回転
	//	XMStoreFloat4(
	//		&rotation,
	//		//XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(0.1f), 0)));
	//		XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(0, 0, 0)));

	//	// 定数バッファーを更新
	//	// Scaling × Rotation × TranslationをCPU側で計算してシェーダーへ送る(シェーダーでやるより処理が軽いため)
	//	const auto worldMatrix =
	//		XMMatrixScalingFromVector(XMLoadFloat3(&scale)) *
	//		XMMatrixRotationQuaternion(XMLoadFloat4(&rotation)) *
	//		XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	//	XMStoreFloat4x4(&constantBufferPerFrame.worldMatrix, XMMatrixTranspose(worldMatrix));

	//	// マテリアルの色を更新
	//	constantBufferPerFrame.materialColor = XMFLOAT4(1, 230 / 255.0f, 0, 1);

	//	// ライトのパラメーター
	//	constantBufferPerFrame.lightPosition = lightPosition;

	//	// カメラの前方ベクトル
	//	const auto eyeDirection =
	//		XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&cameraRotation));
	//	// カメラの上方ベクトル
	//	const auto eyeUpDirection =
	//		XMVector3Rotate(XMVectorSet(0, 1, 0, 0), XMLoadFloat4(&cameraRotation));

	//	// ビュー変換行列を更新
	//	const auto viewMatrix = XMMatrixLookToLH(
	//		XMLoadFloat3(&eyePosition), eyeDirection, eyeUpDirection);
	//	XMStoreFloat4x4(&constantBufferPerFrame.viewMatrix, XMMatrixTranspose(viewMatrix));

	//	//// 【正射影変換の場合】
	//	//// スクリーン画面のアスペクト比
	//	//const auto orthographicSize = 10.0f;	// ビュー空間の垂直方向のサイズ
	//	//const auto nearZ = 0.3f;	// nearクリップ面
	//	//const auto farZ = 1000.0f;	// farクリップ面
	//	//// 定数バッファーを更新
	//	//const auto projectionMatrix = XMMatrixOrthographicLH(
	//	//	orthographicSize * ScreenWidth / static_cast<float>(ScreenHeight),
	//	//	orthographicSize, nearZ, farZ);
	//	//XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixTranspose(projectionMatrix));


	//	// 【パースペクティブ射影変換の場合】
	//	// 視錐台の垂直方向の角度
	//	constexpr auto fieldOfView = XMConvertToRadians(60);
	//	// スクリーン画面のアスペクト比
	//	const auto aspectRatio = window->GetWidth() / static_cast<float>(window->GetHeight());
	//	const auto nearZ = 0.3f;	// nearクリップ面
	//	const auto farZ = 1000.0f;	// farクリップ面
	//	// 定数バッファーを更新
	//	const auto projectionMatrix = XMMatrixPerspectiveFovLH(
	//		fieldOfView, aspectRatio, nearZ, farZ);
	//	XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixTranspose(projectionMatrix));

	//	// World × View × ProjectionをCPU側で計算してシェーダーへ送る
	//	XMStoreFloat4x4(&constantBufferPerFrame.wvpMatrix, XMMatrixTranspose(
	//		worldMatrix * viewMatrix * projectionMatrix));


	//	//Direct3Dの描画処理

	//	// レンダーターゲットを設定
	//	ID3D11RenderTargetView* renderTargetViews[] = { swapChain->GetRenderTargetView() };
	//	immediateContext->OMSetRenderTargets(_countof(renderTargetViews), renderTargetViews, swapChain->GetDepthStencilView());
	//	// 画面をクリア
	//	immediateContext->ClearRenderTargetView(swapChain->GetRenderTargetView(), clearColor);
	//	immediateContext->ClearDepthStencilView(swapChain->GetDepthStencilView(),
	//		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//	// ビューポートを設定
	//	immediateContext->RSSetViewports(1, viewports);


	//	// 頂点バッファーを設定
	//	ID3D11Buffer* vertexBuffers[1] = { vertexBuffer->GetNativePointer() };
	//	UINT strides[1] = { sizeof(VertexPositionNormalTexture) };
	//	UINT offsets[1] = { 0 };
	//	immediateContext->IASetVertexBuffers(
	//		0,
	//		std::size(vertexBuffers),
	//		vertexBuffers, strides, offsets);

	//	// インデックスバッファーを設定
	//	immediateContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), DXGI_FORMAT_R32_UINT, 0);
	//	// 頂点バッファーと頂点シェーダーの組合せに対応した入力レイアウトを設定
	//	immediateContext->IASetInputLayout(inputLayout->GetNativePointer());
	//	// プリミティブトポロジーとしてトライアングルを設定
	//	immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//	// 定数バッファーを更新
	//	constantBuffer->SetData(&constantBufferPerFrame);

	//	// シェーダーを設定
	//	immediateContext->VSSetShader(vertexShader->GetNativePointer(), NULL, 0);
	//	immediateContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
	//	immediateContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);


	//	// シェーダーに定数バッファーを設定
	//	ID3D11Buffer* constantBuffers[] = { constantBuffer->GetNativePointer() };
	//	immediateContext->VSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);
	//	immediateContext->GSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);
	//	immediateContext->PSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);

	//	// ピクセルシェーダーにテクスチャーを設定
	//	ID3D11ShaderResourceView* textureViews[] = { texture->GetShaderResourceView(), };
	//	immediateContext->PSSetShaderResources(0, _countof(textureViews), textureViews);
	//	ID3D11SamplerState* samplerStates[] = { texture->GetSamplerState(), };
	//	immediateContext->PSSetSamplers(0, _countof(samplerStates), samplerStates);

	//	// 描画
	//	immediateContext->DrawIndexed(indexCount, 0, 0);

	//	// バックバッファーに描画したイメージをディスプレイに表示
	//	try {
	//		swapChain->Present(1);
	//	}
	//	catch (const _com_error& error) {
	//		OutputDebugString(TEXT("ERROR: "));
	//		OutputDebugString(error.ErrorMessage());
	//		OutputDebugString(TEXT("\n"));
	//		MessageBox(window->GetHandle(),
	//			TEXT("グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。"),
	//			TEXT("エラー"), MB_OK);
	//		break;
	//	}

	//	// このウィンドウのメッセージが存在するかを確認
	//	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
	//		// メッセージを取得
	//		if (!GetMessage(&msg, NULL, 0, 0)) {
	//			break;
	//		}
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	//OnRender();
}

void Game::Release() noexcept
{
	OnRelease();
}

const std::wstring& Game::GetTitle() const
{
	return title;
}

int Game::GetWidth() const
{
	return width;
}

int Game::GetHeight() const
{
	return height;
}
