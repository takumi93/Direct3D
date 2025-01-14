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

// グラフィックデバイスを作成します。
void Game::InitGraphicsDevice()
{
	// 関数の実行結果を受け取る変数
	HRESULT hr = S_OK;

	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11ShaderResourceView> renderTargetResourceView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	ComPtr<ID3D11ShaderResourceView> depthStencilResourceView;

	// 作成するスワップチェーンについての情報を格納
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = window->GetWidth();
	swapChainDesc.BufferDesc.Height = window->GetHeight();
	swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage =
		DXGI_USAGE_RENDER_TARGET_OUTPUT |
		DXGI_USAGE_SHADER_INPUT;	// シェーダーリソースとして使用することを設定
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = window->GetHandle();
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.Windowed = TRUE;

	// 画面サイズを変える際にスワップチェーンだけ変更する必要があるため
	//スワップチェーンを作成
	hr = graphics->GetDXGI_Factory()->CreateSwapChain(graphics->GetDevice(), &swapChainDesc, &swapChain);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// バックバッファーを取得
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// バックバッファーにアクセスするためのレンダーターゲット ビューを作成
		hr = graphics->GetDevice()->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// バックバッファーにシェーダーからアクセスするためのリソース ビューを作成
		hr = graphics->GetDevice()->CreateShaderResourceView(backBuffer.Get(), NULL, &renderTargetResourceView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
	}

	// テクスチャとシェーダーリソースビューのフォーマットを設定
	DXGI_FORMAT textureFormat = depthStencilFormat;
	DXGI_FORMAT resourceFormat = depthStencilFormat;
	//設定したフォーマットをテクスチャとリソース用に入れ替え
	switch (depthStencilFormat)
	{
	case DXGI_FORMAT_D16_UNORM:
		textureFormat = DXGI_FORMAT_R16_TYPELESS;
		resourceFormat = DXGI_FORMAT_R16_UNORM;
		break;
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
		textureFormat = DXGI_FORMAT_R24G8_TYPELESS;
		resourceFormat = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		break;
	case DXGI_FORMAT_D32_FLOAT:
		textureFormat = DXGI_FORMAT_R32_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT;
		break;
	case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		textureFormat = DXGI_FORMAT_R32G8X24_TYPELESS;
		resourceFormat = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		break;
	}
	// 深度ステンシルを作成
	{
		ComPtr<ID3D11Texture2D> depthStencil;
		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.Width = swapChainDesc.BufferDesc.Width;
		depthStencilDesc.Height = swapChainDesc.BufferDesc.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = textureFormat;
		depthStencilDesc.SampleDesc = swapChainDesc.SampleDesc;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE; // シェーダーリソースとして使用することを設定
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		hr = graphics->GetDevice()->CreateTexture2D(&depthStencilDesc, NULL, &depthStencil);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// 深度ステンシルにアクセスするためのビューを作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = depthStencilFormat;
		if (depthStencilDesc.SampleDesc.Count > 0) {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		}
		else {
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
		}
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		hr = graphics->GetDevice()->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &depthStencilView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
		// 深度ステンシルにシェーダーからアクセスするためのリソース ビューを作成
		D3D11_SHADER_RESOURCE_VIEW_DESC depthStencilResourceViewDesc = {};
		depthStencilResourceViewDesc.Format = resourceFormat;
		if (depthStencilDesc.SampleDesc.Count > 0) {
			depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		}
		else {
			depthStencilResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			depthStencilResourceViewDesc.Texture2D.MostDetailedMip = 0;
			depthStencilResourceViewDesc.Texture2D.MipLevels = 1;
		}
		hr = graphics->GetDevice()->CreateShaderResourceView(depthStencil.Get(), &depthStencilResourceViewDesc, &depthStencilResourceView);
		//失敗したら処理しない
		if (FAILED(hr)) {
			throw _com_error(hr);
		}
	}

	// ビューポートを指定
	viewports[0].Width = static_cast<FLOAT>(window->GetWidth());
	viewports[0].Height = static_cast<FLOAT>(window->GetHeight());
	viewports[0].MinDepth = 0.0f;
	viewports[0].MaxDepth = 1.0f;
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;

	// 初期化がすべて成功したらメンバー変数を更新する
	this->swapChain = swapChain;
	this->renderTargetView = renderTargetView;
	this->renderTargetResourceView = renderTargetResourceView;
	this->depthStencilView = depthStencilView;
	this->depthStencilResourceView = depthStencilResourceView;
}

// メッセージループを実行
int Game::Run(const WindowSettings& settings)
{
	try {
		// ウィンドウを作成
		window.reset(new MainWindow(settings));
		// グラフィックデバイスを作成
		graphics.reset(new Graphics());
		InitGraphicsDevice();
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
	// 頂点データの配列
	constexpr VertexPositionColor vertices[] = {
		// Top
		{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

		// Buttom
		{ {  0.5f,  -0.5f,  0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f,  -0.5f,  0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
		{ {  0.5f,  -0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f } },
		{ { -0.5f,  -0.5f, -0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f } },

		// Front
		{ {  0.5f,  0.5f, 0.5f }, { 0.25f, 0.25f, 0.25f, 1.0f } },
		{ { -0.5f,  0.5f, 0.5f }, { 0.25f, 0.25f, 0.25f, 1.0f } },
		{ {  0.5f, -0.5f, 0.5f }, { 0.25f, 0.25f, 0.25f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f }, { 0.25f, 0.25f, 0.25f, 1.0f } },

		// Back
		{ { -0.5f,  0.5f, -0.5f }, { 0.75f, 0.75f, 0.75f, 1.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.75f, 0.75f, 0.75f, 1.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.75f, 0.75f, 0.75f, 1.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.75f, 0.75f, 0.75f, 1.0f } },

		// Left
		{ { -0.5f,  0.5f,  0.5f }, { 0.15f, 0.15f, 0.15f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 0.15f, 0.15f, 0.15f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.15f, 0.15f, 0.15f, 1.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.15f, 0.15f, 0.15f, 1.0f } },

		// Right
		{ {  0.5f,  0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 0.5f, 0.5f, 0.5f, 1.0f } },
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
		DirectX::XMFLOAT4X4 worldMatrix;	// ワールド変換行列(スケール回転移動を統合)
		DirectX::XMFLOAT4X4 viewMatrix;		// ビュー変換行列
		DirectX::XMFLOAT4X4 projectionMatrix;	// プロジェクション変換行列
		DirectX::XMFLOAT4X4 wvpMatrix;	// ワールド×ビュー×プロジェクション変換行列
		DirectX::XMFLOAT4 materialColor;	// カラー
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

	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	 0,							   0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	float time = 0;

	// メッセージループを実行
	MSG msg = {};
	while (true) {
		time += 0.01f;
		
		// カメラのz軸回転
		float zAngle = 0.0f;

		// フレーム更新処理
		if (GetAsyncKeyState(VK_SPACE)) {
			XMStoreFloat4(
				&rotation,
				XMQuaternionRotationRollPitchYaw(0, 0, 0));
		}
		XMStoreFloat4(
			&rotation,
			XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(1.0f), 0)));
		
		// 定数バッファーを更新
		// Scaling × Rotation × TranslationをCPU側で計算してシェーダーへ送る(シェーダーでやるより処理が軽いため)
		const auto worldMatrix =
			XMMatrixScalingFromVector(XMLoadFloat3(&scale)) *
			XMMatrixRotationQuaternion(XMLoadFloat4(&rotation)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&position));

		XMStoreFloat4x4(&constantBufferPerFrame.worldMatrix, XMMatrixTranspose(worldMatrix));

		constantBufferPerFrame.materialColor = XMFLOAT4(1, 230 / 255.0f, 0, 1);

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
		ID3D11RenderTargetView* renderTargetViews[] = { renderTargetView.Get() };
		immediateContext->OMSetRenderTargets(_countof(renderTargetViews), renderTargetViews, depthStencilView.Get());
		// 画面をクリア
		immediateContext->ClearRenderTargetView(renderTargetViews[0], clearColor);
		immediateContext->ClearDepthStencilView(depthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		// ビューポートを設定
		immediateContext->RSSetViewports(1, viewports);


		// 頂点バッファーを設定
		ID3D11Buffer* vertexBuffers[1] = { vertexBuffer.Get()};
		UINT strides[1] = { sizeof(VertexPositionColor) };
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
		HRESULT hr = S_OK;
		hr = swapChain->Present(1, 0);
		if (FAILED(hr))
		{
			MessageBox(window->GetHandle(),
				L"グラフィックデバイスが物理的に取り外されたか、ドライバーがアップデートされました。",
				L"エラー", MB_OK);
			return -1;
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
