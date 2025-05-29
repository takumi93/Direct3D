//=============================================================================
// Game.cpp
// 
//=============================================================================
#include <DirectXMath.h>	// DirectXの算術ライブラリー
#include <comdef.h>
#include <exception>
#include "Game.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// メッセージループを実行
int Game::Run(const WindowSettings& settings)
{
	try {
		// ウィンドウを作成
		window = std::make_shared<MainWindow>(settings);
		// グラフィックデバイスを作成
		graphics = std::make_shared<Graphics>();
		// スワップチェーンを作成
		swapChain.reset(new SwapChain(graphics, window));

		// ビューポート
		viewports[0].Width = static_cast<FLOAT>(window->GetWidth());
		viewports[0].Height = static_cast<FLOAT>(window->GetHeight());
		viewports[0].MinDepth = 0.0f;
		viewports[0].MaxDepth = 1.0f;
		viewports[0].TopLeftX = 0.0f;
		viewports[0].TopLeftY = 0.0f;
	}
	catch(const std::exception& error){
		OutputDebugStringA("ERROR: ");
		OutputDebugStringA(error.what());
		OutputDebugStringA("\n");
		MessageBoxW(NULL, L"ウィンドウを作成できませんでした。", L"メッセージ", MB_OK);
		return 0;
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBoxW(NULL, TEXT("グラフィックデバイスを初期化できませんでした。"), TEXT("エラー"), MB_OK);
		return 0;
	}

	HRESULT hr = S_OK;
	const auto graphicsDevice = graphics->GetDevice();
	const auto immediateContext = graphics->GetDeviceContext();

	// 頂点データの配列
	// 配列の左は座標で右は頂点の向きを指定
	constexpr VertexPositionNormalTexture vertices[] = {
		// Top
		{ {-0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},
		{ {-0.5f, 0.5f,-0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }},
		{ { 0.5f, 0.5f,-0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }},
		// Bottom
		{ { 0.5f,-0.5f, 0.5f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f }},
		{ {-0.5f,-0.5f, 0.5f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 0.0f }},
		{ { 0.5f,-0.5f,-0.5f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 1.0f }},
		{ {-0.5f,-0.5f,-0.5f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 1.0f }},
		// Front
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ {-0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { 0.5f,-0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ {-0.5f,-0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		// Back
		{ {-0.5f, 0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, 0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f } },
		{ {-0.5f,-0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 1.0f } },
		{ { 0.5f,-0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f } },
		// Left
		{ {-0.5f, 0.5f, 0.5f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {-0.5f, 0.5f,-0.5f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {-0.5f,-0.5f, 0.5f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
		{ {-0.5f,-0.5f,-0.5f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
		// Right
		{ { 0.5f, 0.5f,-0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 0.5f,-0.5f,-0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 0.5f,-0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
	};
	// インデックスデータの配列
	constexpr UINT32 indices[] = {
		0, 1, 2, 3, 2, 1,
		4, 5, 6, 7, 6, 5,
		8, 9, 10, 11, 10, 9,
		12, 13, 14, 15, 14, 13,
		16, 17, 18, 19, 18, 17,
		20, 21, 22, 23, 22, 21,
	};
	constexpr UINT indexCount = _countof(indices);

	// 画像データのダミー
	constexpr uint32_t source[16] = {
		0xFF0000FF, 0xFF000000, 0xFF0000FF, 0xFF000000,
		0xFF000000, 0xFF0000FF, 0xFF000000, 0xFF0000FF,
		0xFF0000FF, 0xFF000000, 0xFF0000FF, 0xFF000000,
		0xFF000000, 0xFF0000FF, 0xFF000000, 0xFF0000FF,
	};

	//// 画像データのダミー
	//constexpr uint8_t source[][4] = {
	//	{0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF},
	//	{0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF},
	//	{0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF},
	//	{0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF}, {0x00, 0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0x00, 0xFF},
	//};
	
	// 自分で作ったクラスはshared、既存のクラスはComptr
	// バッファー
	std::shared_ptr<VertexBuffer> vertexBuffer;
	// インデックスバッファー
	std::shared_ptr<IndexBuffer> indexBuffer;
	// 定数バッファー
	std::shared_ptr<ConstantBuffer> constantBuffer;
	// シェーダー
	std::shared_ptr<BasicVertexShader> vertexShader;
	std::shared_ptr<BasicGeometryShader> geometryShader;
	std::shared_ptr<BasicPixelShader> pixelShader;
	// 入力レイアウト
	std::shared_ptr<InputLayout> inputLayout;
	// テクスチャー
	std::shared_ptr<Texture2D> texture;

	try {
		// 頂点バッファーを作成
		vertexBuffer = std::make_shared<VertexBuffer>(graphics, sizeof vertices);
		// インデックスバッファーを作成
		indexBuffer = std::make_shared<IndexBuffer>(graphics, sizeof vertices);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("バッファーを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return 0;
	}

	// バッファーにデータを転送
	vertexBuffer->SetData(vertices);
	indexBuffer->SetData(indices);

	// 定数バッファーを介してシェーダーに毎フレーム送るデータを表します。
	struct ConstantBufferPerFrame
	{
		DirectX::XMFLOAT4X4 worldMatrix;		// ワールド変換行列(スケール回転移動を統合)
		DirectX::XMFLOAT4X4 viewMatrix;			// ビュー変換行列
		DirectX::XMFLOAT4X4 projectionMatrix;	// プロジェクション変換行列
		DirectX::XMFLOAT4X4 wvpMatrix;			// ワールド×ビュー×プロジェクション変換行列
		DirectX::XMFLOAT4 materialColor;		// カラー

		// カメラの位置座標
		DirectX::XMFLOAT4 viewPosition = DirectX::XMFLOAT4(0, 1, -10, 1);

		// ライトの位置座標(平行光源 w = 0, 点光源 w = 1)
		DirectX::XMFLOAT4 lightPosition = DirectX::XMFLOAT4(1.0f, 2.0f, -2.0f, 1.0f);

		// マテリアルの表面カラー
		DirectX::XMFLOAT4 materialDiffuse = DirectX::XMFLOAT4(1, 1, 0, 1);

		// 鏡面反射の色(r, g, b) = (x, y, z)
		DirectX::XMFLOAT3 materialSpecularColor = DirectX::XMFLOAT3(1, 1, 1);
		// 鏡面反射の強さ(float) = w
		float materialSpecularPower = 2;
	};
	
	ConstantBufferPerFrame constantBufferPerFrame = {};

	// バッファーを作成
	try {
		// 定数バッファーを作成
		constantBuffer = std::make_shared<ConstantBuffer>(graphics, sizeof constantBufferPerFrame);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("定数バッファーを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return 0;
	}

	// 定数バッファーを更新
	XMStoreFloat4x4(&constantBufferPerFrame.worldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&constantBufferPerFrame.viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&constantBufferPerFrame.wvpMatrix, XMMatrixIdentity());
	constantBufferPerFrame.materialColor = XMFLOAT4(1, 238 / 255.0f, 0, 1);
	constantBufferPerFrame.lightPosition = XMFLOAT4(1, 2, -2, 1);
	constantBuffer->SetData(&constantBufferPerFrame);

	try {
		// シェーダーを作成
		vertexShader = std::make_shared<BasicVertexShader>(graphics);
		geometryShader = std::make_shared<BasicGeometryShader>(graphics);
		pixelShader = std::make_shared<BasicPixelShader>(graphics);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("シェーダーを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return 0;
	}

	try {
		// 入力レイアウトを作成
		inputLayout = std::make_shared<InputLayout>(
			graphicsDevice,												// 使用するグラフィックデバイス
			VertexPositionNormalTexture::inputElementDescs, 			// 入力要素についての記述
			std::size(VertexPositionNormalTexture::inputElementDescs),	// inputElementDescs配列の数
			vertexShader->GetBytecode(),								// 入力を受け取る頂点シェーダーのバイトコード
			vertexShader->GetBytecodeLength()							// バイトコードのサイズ
			//&inputLayout
		);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("入力レイアウトを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return 0;
	}


	//ComPtr<ID3D11InputLayout> inputLayout;
	//hr = graphicsDevice->CreateInputLayout(
	//	VertexPositionNormalTexture::inputElementDescs, 				// 入力要素についての記述
	//	std::size(VertexPositionNormalTexture::inputElementDescs),		// inputElementDescs配列の数
	//	vertexShader->GetBytecode(),							// 入力を受け取る頂点シェーダーのバイトコード
	//	vertexShader->GetBytecodeLength(),						// バイトコードのサイズ
	//	&inputLayout);
	//if (FAILED(hr)) {
	//	OutputDebugString(L"入力レイアウトを作成できませんでした。");
	//	return 0;
	//}

	try {
		// テクスチャーを作成
		texture = std::make_shared<Texture2D>(
			graphicsDevice,
			4,
			4,
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			false
		);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("テクスチャを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return 0;
	}

	// ピクセルデータを更新
	texture->SetData(source);

	// 位置座標
	XMFLOAT3 position = { 0, 0, 0 };
	// 回転
	XMFLOAT4 rotation = {};
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	// スケール
	XMFLOAT3 scale = { 1, 1, 1 };

	// カメラの位置座標
	constexpr XMFLOAT3 eyePosition = { 0.0f, 1.0f, -5.0f };
	// カメラの回転
	XMFLOAT4 cameraRotation = {};
	XMStoreFloat4(&cameraRotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(15.0f), 0, 0));

	// ライトの位置座標
	// 平行光源(w = 0.0f) or 点光源(w = 1.0f)
	XMFLOAT4 lightPosition = { 1, 2, -2, 0 };

	float time = 0;

	// メッセージループを実行
	MSG msg = {};
	while (true) {
		time += 0.01f;
		
		// カメラのz軸回転
		float zAngle = 0.0f;

		// フレーム更新処理
		// 点光源
		if (GetAsyncKeyState(VK_SPACE)) {
			lightPosition.w = 1;
		}
		// 平行光源
		else {
			lightPosition.w = 0;
		}

		// オブジェクトのy軸回転
		XMStoreFloat4(
			&rotation,
			XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(0.5f), 0)));
		
		// 定数バッファーを更新
		// Scaling × Rotation × TranslationをCPU側で計算してシェーダーへ送る(シェーダーでやるより処理が軽いため)
		const auto worldMatrix =
			XMMatrixScalingFromVector(XMLoadFloat3(&scale)) *
			XMMatrixRotationQuaternion(XMLoadFloat4(&rotation)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		XMStoreFloat4x4(&constantBufferPerFrame.worldMatrix, XMMatrixTranspose(worldMatrix));

		// マテリアルの色を更新
		constantBufferPerFrame.materialColor = XMFLOAT4(1, 230 / 255.0f, 0, 1);

		// ライトのパラメーター
		constantBufferPerFrame.lightPosition = lightPosition;

		// カメラの前方ベクトル
		const auto eyeDirection =
			XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&cameraRotation));
		// カメラの上方ベクトル
		const auto eyeUpDirection =
			XMVector3Rotate(XMVectorSet(0, 1, 0, 0), XMLoadFloat4(&cameraRotation));

		// ビュー変換行列を更新
		const auto viewMatrix = XMMatrixLookToLH(
			XMLoadFloat3(&eyePosition), eyeDirection, eyeUpDirection);
		XMStoreFloat4x4(&constantBufferPerFrame.viewMatrix, XMMatrixTranspose(viewMatrix));

		//// 【正射影変換の場合】
		//// スクリーン画面のアスペクト比
		//const auto orthographicSize = 10.0f;	// ビュー空間の垂直方向のサイズ
		//const auto nearZ = 0.3f;	// nearクリップ面
		//const auto farZ = 1000.0f;	// farクリップ面
		//// 定数バッファーを更新
		//const auto projectionMatrix = XMMatrixOrthographicLH(
		//	orthographicSize * ScreenWidth / static_cast<float>(ScreenHeight),
		//	orthographicSize, nearZ, farZ);
		//XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixTranspose(projectionMatrix));


		// 【パースペクティブ射影変換の場合】
		// 視錐台の垂直方向の角度
		constexpr auto fieldOfView = XMConvertToRadians(60);
		// スクリーン画面のアスペクト比
		const auto aspectRatio = window->GetWidth() / static_cast<float>(window->GetHeight());
		const auto nearZ = 0.3f;	// nearクリップ面
		const auto farZ = 1000.0f;	// farクリップ面
		// 定数バッファーを更新
		const auto projectionMatrix = XMMatrixPerspectiveFovLH(
			fieldOfView, aspectRatio, nearZ, farZ);
		XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixTranspose(projectionMatrix));

		// World × View × ProjectionをCPU側で計算してシェーダーへ送る
		XMStoreFloat4x4(&constantBufferPerFrame.wvpMatrix, XMMatrixTranspose(
			worldMatrix* viewMatrix* projectionMatrix));


		//Direct3Dの描画処理

		// レンダーターゲットを設定
		ID3D11RenderTargetView* renderTargetViews[] = { swapChain->GetRenderTargetView() };
		immediateContext->OMSetRenderTargets(_countof(renderTargetViews), renderTargetViews, swapChain->GetDepthStencilView());
		// 画面をクリア
		immediateContext->ClearRenderTargetView(swapChain->GetRenderTargetView(), clearColor);
		immediateContext->ClearDepthStencilView(swapChain->GetDepthStencilView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		// ビューポートを設定
		immediateContext->RSSetViewports(1, viewports);


		// 頂点バッファーを設定
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer->GetNativePointer() };
		UINT strides[1] = { sizeof(VertexPositionNormalTexture) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(
			0,
			std::size(vertexBuffers),
			vertexBuffers, strides, offsets);

		// インデックスバッファーを設定
		immediateContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), DXGI_FORMAT_R32_UINT, 0);
		// 頂点バッファーと頂点シェーダーの組合せに対応した入力レイアウトを設定
		immediateContext->IASetInputLayout(inputLayout->GetNativePointer());
		// プリミティブトポロジーとしてトライアングルを設定
		immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 定数バッファーを更新
		constantBuffer->SetData(&constantBufferPerFrame);

		// シェーダーを設定
		immediateContext->VSSetShader(vertexShader->GetNativePointer(), NULL, 0);
		immediateContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
		immediateContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);

		
		// シェーダーに定数バッファーを設定
		ID3D11Buffer* constantBuffers[] = { constantBuffer->GetNativePointer() };
		immediateContext->VSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);
		immediateContext->GSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);
		immediateContext->PSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);

		// ピクセルシェーダーにテクスチャーを設定
		ID3D11ShaderResourceView* textureViews[] = { texture->GetShaderResourceView(),};
		immediateContext->PSSetShaderResources(0, _countof(textureViews), textureViews);
		ID3D11SamplerState* samplerStates[] = { texture->GetSamplerState(),};
		immediateContext->PSSetSamplers(0, _countof(samplerStates), samplerStates);

		// 描画
		immediateContext->DrawIndexed(indexCount, 0, 0);

		// バックバッファーに描画したイメージをディスプレイに表示
		try {
			swapChain->Present(1);
		}
		catch (const _com_error& error) {
			OutputDebugString(TEXT("ERROR: "));
			OutputDebugString(error.ErrorMessage());
			OutputDebugString(TEXT("\n"));
			MessageBox(window->GetHandle(),
				TEXT("グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。"),
				TEXT("エラー"), MB_OK);
			break;
		}

		// このウィンドウのメッセージが存在するかを確認
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			// メッセージを取得
			if (!GetMessage(&msg, NULL, 0, 0)) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
