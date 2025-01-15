//=============================================================================
// Game.cpp
// 
//=============================================================================
#include <DirectXMath.h>	// DirectXの算術ライブラリー
#include <comdef.h>
#include <exception>
#include "Game.h"
#include "BasicVertexShader.h"
#include "BasicGeometryShader.h"
#include "BasicPixelShader.h"

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
		//InitGraphicsDevice();
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

	// 一つの頂点に含まれるデータの型
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 position;	// 位置座標
		DirectX::XMFLOAT4 color;	// 頂点カラー
	};
	// 一つの頂点に含まれるデータの型
	struct VertexPositionNormal
	{
		DirectX::XMFLOAT3 position;	// 位置座標, POSITION
		DirectX::XMFLOAT3 normal;	// 法線ベクトル, NORMAL
	};

	// 頂点データの配列
	constexpr VertexPositionNormal vertices[] = {
		// Top
		{ {-0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ {-0.5f, 0.5f,-0.5f }, { 0.0f, 1.0f, 0.0f } },
		{ { 0.5f, 0.5f,-0.5f }, { 0.0f, 1.0f, 0.0f } },
		// Bottom
		{ { 0.5f,-0.5f, 0.5f }, { 0.0f,-1.0f, 0.0f } },
		{ {-0.5f,-0.5f, 0.5f }, { 0.0f,-1.0f, 0.0f } },
		{ { 0.5f,-0.5f,-0.5f }, { 0.0f,-1.0f, 0.0f } },
		{ {-0.5f,-0.5f,-0.5f }, { 0.0f,-1.0f, 0.0f } },
		// Front
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		{ {-0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		{ { 0.5f,-0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		{ {-0.5f,-0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
		// Back
		{ {-0.5f, 0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f } },
		{ { 0.5f, 0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f } },
		{ {-0.5f,-0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f } },
		{ { 0.5f,-0.5f,-0.5f }, { 0.0f, 0.0f,-1.0f } },
		// Left
		{ {-0.5f, 0.5f, 0.5f }, {-1.0f, 0.0f, 0.0f } },
		{ {-0.5f, 0.5f,-0.5f }, {-1.0f, 0.0f, 0.0f } },
		{ {-0.5f,-0.5f, 0.5f }, {-1.0f, 0.0f, 0.0f } },
		{ {-0.5f,-0.5f,-0.5f }, {-1.0f, 0.0f, 0.0f } },
		// Right
		{ { 0.5f, 0.5f,-0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f,-0.5f,-0.5f }, { 1.0f, 0.0f, 0.0f } },
		{ { 0.5f,-0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f } },
	};

	constexpr UINT32 indices[] = {
		0, 1, 2, 3, 2, 1,
		4, 5, 6, 7, 6, 5,
		8, 9, 10, 11, 10, 9,
		12, 13, 14, 15, 14, 13,
		16, 17, 18, 19, 18, 17,
		20, 21, 22, 23, 22, 21,
	};
	constexpr UINT indexCount = _countof(indices);
	
	
	// 頂点バッファーを作成
	ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof vertices;	// 作成するバッファーのサイズ(bytes) x:4bytes,y:4bytes,z:4bytes total 12bytes
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;	// バッファーの使用方法
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// Vertex Bufferとして利用する
	bufferDesc.CPUAccessFlags = 0;			// CPUからの読み書きに使わない場合は0
	bufferDesc.MiscFlags = 0;				// オプションのフラグ
	bufferDesc.StructureByteStride = 0;		// 頂点バッファーとして使うなら0
	hr = graphicsDevice->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);
	if (FAILED(hr) || vertexBuffer == nullptr) {
		OutputDebugString(L"頂点バッファーを作成できませんでした。");
		return -1;
	}
	// バッファーにデータを転送
	immediateContext->UpdateSubresource(vertexBuffer.Get(), 0, NULL, vertices, 0, 0);

	// インデックスバッファーを作成
	ComPtr<ID3D11Buffer> indexBuffer;
	{
		// 作成するインデックスバッファーについての記述
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof indices;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		// バッファーを作成
		hr = graphicsDevice->CreateBuffer(&bufferDesc, NULL, &indexBuffer);
		if (FAILED(hr) || indexBuffer == nullptr) {
			OutputDebugString(L"インデックスバッファーを作成できませんでした。");
			return -1;
		}
		// バッファーにデータを転送
		immediateContext->UpdateSubresource(indexBuffer.Get(), 0, NULL, indices, 0, 0);
	}

	// 定数バッファーを介してシェーダーに毎フレーム送るデータを表します。
	struct ConstantBufferPerFrame
	{
		DirectX::XMFLOAT4X4 worldMatrix;		// ワールド変換行列(スケール回転移動を統合)
		DirectX::XMFLOAT4X4 viewMatrix;			// ビュー変換行列
		DirectX::XMFLOAT4X4 projectionMatrix;	// プロジェクション変換行列
		DirectX::XMFLOAT4X4 wvpMatrix;			// ワールド×ビュー×プロジェクション変換行列
		DirectX::XMFLOAT4 materialColor;		// カラー
		DirectX::XMFLOAT4 lightPosition;		// ライト位置
	};
	ConstantBufferPerFrame constantBufferPerFrame = {};
	// 定数バッファーを作成
	ComPtr<ID3D11Buffer> constantBuffer;
	{
		// 作成するバッファーについての記述
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = sizeof(ConstantBufferPerFrame);	// 作成するバッファーのサイズ(bytes)
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		// Constant Bufferとして利用する
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		// バッファーを作成
		auto hr = graphicsDevice->CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
		if (FAILED(hr)) {
			OutputDebugString(L"定数バッファーを作成できませんでした。");
			return -1;
		}
	}

	// 定数バッファーを更新
	XMStoreFloat4x4(&constantBufferPerFrame.worldMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&constantBufferPerFrame.viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&constantBufferPerFrame.projectionMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&constantBufferPerFrame.wvpMatrix, XMMatrixIdentity());
	constantBufferPerFrame.materialColor = XMFLOAT4(1, 238 / 255.0f, 0, 1);
	constantBufferPerFrame.lightPosition = XMFLOAT4(1, 2, -2, 1);
	immediateContext->UpdateSubresource(constantBuffer.Get(), 0, NULL, &constantBufferPerFrame, 0, 0);

	// 頂点シェーダーの作成
	ComPtr<ID3D11VertexShader> vertexShader;
	hr = graphicsDevice->CreateVertexShader(
		g_BasicVertexShader,
		ARRAYSIZE(g_BasicVertexShader),
		NULL,
		&vertexShader);
	if (FAILED(hr)) {
		OutputDebugString(L"頂点シェーダーの作成に失敗しました。");
		return 0;
	}

	// ジオメトリーシェーダーの作成
	ComPtr<ID3D11GeometryShader> geometryShader;
	hr = graphicsDevice->CreateGeometryShader(
		g_BasicGeometryShader, ARRAYSIZE(g_BasicGeometryShader),
		NULL,
		&geometryShader);
	if (FAILED(hr)) {
		OutputDebugString(L"ジオメトリーシェーダーを作成できませんでした。");
	}

	// ピクセルシェーダーの作成
	ComPtr<ID3D11PixelShader> pixelShader;
	hr = graphicsDevice->CreatePixelShader(
		g_BasicPixelShader,
		ARRAYSIZE(g_BasicPixelShader),
		NULL,
		&pixelShader);
	if (FAILED(hr)) {
		OutputDebugString(L"ピクセルシェーダーを作成できませんでした。");
	}

	//D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// 入力レイアウトを作成
	ComPtr<ID3D11InputLayout> inputLayout;
	hr = graphicsDevice->CreateInputLayout(
		inputElementDescs,				// 入力要素についての記述
		ARRAYSIZE(inputElementDescs),	// inputElementDescs配列の数
		g_BasicVertexShader,	// 入力を受け取る頂点シェーダーのバイトコード
		ARRAYSIZE(g_BasicVertexShader),		// バイトコードのサイズ
		&inputLayout);
	if (FAILED(hr)) {
		OutputDebugString(L"入力レイアウトを作成できませんでした。");
		return 0;
	}

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

		// 定数バッファーを更新
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
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get()};
		UINT strides[1] = { sizeof(VertexPositionNormal) };
		UINT offsets[1] = { 0 };
		immediateContext->IASetVertexBuffers(
			0,
			std::size(vertexBuffers),
			vertexBuffers, strides, offsets);

		// インデックスバッファーを設定
		immediateContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		// 頂点バッファーと頂点シェーダーの組合せに対応した入力レイアウトを設定
		immediateContext->IASetInputLayout(inputLayout.Get());
		// プリミティブトポロジーとしてトライアングルを設定
		immediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 定数バッファーを更新
		immediateContext->UpdateSubresource(constantBuffer.Get(), 0, NULL, &constantBufferPerFrame, 0, 0);

		// シェーダーを設定
		immediateContext->VSSetShader(vertexShader.Get(), NULL, 0);
		immediateContext->GSSetShader(geometryShader.Get(), NULL, 0);
		immediateContext->PSSetShader(pixelShader.Get(), NULL, 0);

		
		// シェーダーに定数バッファーを設定
		ID3D11Buffer* constantBuffers[] = { constantBuffer.Get() };
		immediateContext->VSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);
		immediateContext->GSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);
		immediateContext->PSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);

		

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
