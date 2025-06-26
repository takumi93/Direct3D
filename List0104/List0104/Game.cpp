//=============================================================================
// Game.cpp
// 
//=============================================================================
#include <DirectXMath.h>	// DirectXの算術ライブラリー
#include <wincodec.h>
#include <comdef.h>
#include <exception>
#include <memory>
#include "Game.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// メッセージループを実行
int Game::Run(const WindowSettings& settings)
{
	// COMライブラリを初期化
	if (FAILED(CoInitialize(NULL))) {
		return EXIT_FAILURE;
	}
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
	
	// 自分で作ったクラスはunique or shared、既存のクラスはComptr
	// バッファー
	std::unique_ptr<VertexBuffer> vertexBuffer;
	// インデックスバッファー
	std::unique_ptr<IndexBuffer> indexBuffer;
	// 定数バッファー
	std::unique_ptr<ConstantBuffer> constantBuffer;
	// シェーダー
	std::unique_ptr<BasicVertexShader> vertexShader;
	std::unique_ptr<BasicGeometryShader> geometryShader;
	std::unique_ptr<BasicPixelShader> pixelShader;
	// 入力レイアウト
	std::unique_ptr<InputLayout> inputLayout;
	// テクスチャー
	std::unique_ptr<Texture2D> texture;

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
		return 0;
	}

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
		//constantBuffer.reset(new ConstantBuffer(graphics, sizeof constantBufferPerFrame));
		constantBuffer = std::make_unique<ConstantBuffer>(graphics, sizeof constantBufferPerFrame);
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
		//vertexShader.reset(new BasicVertexShader(graphics));
		//geometryShader.reset(new BasicGeometryShader(graphics));
		//pixelShader.reset(new BasicPixelShader(graphics));
		vertexShader = std::make_unique<BasicVertexShader>(graphics);
		geometryShader = std::make_unique<BasicGeometryShader>(graphics);
		pixelShader = std::make_unique<BasicPixelShader>(graphics);
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
		//inputLayout.reset(new InputLayout(
		//	graphicsDevice,												// 使用するグラフィックデバイス
		//	VertexPositionNormalTexture::inputElementDescs, 			// 入力要素についての記述
		//	std::size(VertexPositionNormalTexture::inputElementDescs),	// inputElementDescs配列の数
		//	vertexShader->GetBytecode(),								// 入力を受け取る頂点シェーダーのバイトコード
		//	vertexShader->GetBytecodeLength()							// バイトコードのサイズ
		//	)
		//);

		inputLayout = std::make_unique<InputLayout>(
			graphicsDevice,												// 使用するグラフィックデバイス
			VertexPositionNormalTexture::inputElementDescs, 			// 入力要素についての記述
			std::size(VertexPositionNormalTexture::inputElementDescs),	// inputElementDescs配列の数
			vertexShader->GetBytecode(),								// 入力を受け取る頂点シェーダーのバイトコード
			vertexShader->GetBytecodeLength()							// バイトコードのサイズ
		);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("入力レイアウトを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return 0;
	}

	// 画像データの読み込み
	// ファクトリーの作成
	ComPtr<IWICImagingFactory2> factory;
	hr = CoCreateInstance(
		CLSID_WICImagingFactory2,
		NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&factory));
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// デコードの作成
	ComPtr<IWICBitmapDecoder> decoder;
	hr = factory->CreateDecoderFromFilename(
		L"Sample.png",
		NULL, GENERIC_READ, WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		&decoder);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// BitMapフレームを取得
	//UINT frameCount = 0;
	//hr = decoder->GetFrameCount(&frameCount);
	//if (FAILED(hr)) {
	//	throw _com_error(hr);
	//}
	ComPtr<IWICBitmapFrameDecode> frame;
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// FormatConverter
	ComPtr<IWICFormatConverter> converter;
	hr = factory->CreateFormatConverter(&converter);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// 出力ストリームを作成
	ComPtr<IWICStream> outputStream;
	hr = factory->CreateStream(&outputStream);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	// 保存先を指定
	hr = outputStream->InitializeFromFilename(L"Sample.dds", GENERIC_WRITE);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// DDS対応のエンコーダを作成
	ComPtr<IWICBitmapEncoder> encoder;
	hr = factory->CreateEncoder(GUID_ContainerFormatDds, NULL, &encoder);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	hr = encoder->Initialize(outputStream.Get(), WICBitmapEncoderCacheOption::WICBitmapEncoderNoCache);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	ComPtr<IWICBitmapFrameEncode> bitmapEncode;
	ComPtr<IPropertyBag2> propertyBag;
	hr = encoder->CreateNewFrame(&bitmapEncode,&propertyBag);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// IWICDdsEncoderを取得
	ComPtr<IWICDdsEncoder> ddsEncoder;
	hr = encoder.As(&ddsEncoder);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	//auto ddsparameters = WICDdsParameters{
	//	.Width = 4,
	//	.Height = 4,
	//	//.Depth = 1,
	//	//.MipLevels = 3,
	//	.DxgiFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
	//	//.Dimension = WICDdsDimension::WICDdsTexture2D,
	//};

	//hr = ddsEncoder->SetParameters(&ddsparameters);
	//if (FAILED(hr)) {
	//	throw _com_error(hr);
	//}

	UINT width = 0;
	UINT height = 0;
	hr = converter->GetSize(&width, &height);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// 画像フォーマットを取得
	WICPixelFormatGUID wicFormat = {};
	hr = converter->GetPixelFormat(&wicFormat);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	struct WIC2DXGI_Format
	{
		const GUID& wic;
		DXGI_FORMAT dxgi;
	};

	// 画像フォーマットからdxgiフォーマットに変換する一覧を作成
	WIC2DXGI_Format wic2dxgi_formats[] = {
		{GUID_WICPixelFormat128bppRGBAFloat, DXGI_FORMAT_R32G32B32A32_FLOAT},
		{GUID_WICPixelFormat64bppRGBAHalf, DXGI_FORMAT_R16G16B16A16_FLOAT},
		{GUID_WICPixelFormat64bppRGBA, DXGI_FORMAT_R16G16B16A16_UNORM},
		{GUID_WICPixelFormat32bppRGBA, DXGI_FORMAT_R8G8B8A8_UNORM},
		{GUID_WICPixelFormat32bppBGRA, DXGI_FORMAT_B8G8R8A8_UNORM},
		{GUID_WICPixelFormat32bppBGR, DXGI_FORMAT_B8G8R8X8_UNORM},
		{GUID_WICPixelFormat24bppBGR, DXGI_FORMAT_B8G8R8X8_UNORM},
		{GUID_WICPixelFormat32bppRGBA1010102XR, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM},
		{GUID_WICPixelFormat32bppRGBA1010102, DXGI_FORMAT_R10G10B10A2_UNORM},
		{GUID_WICPixelFormat32bppRGBE, DXGI_FORMAT_R9G9B9E5_SHAREDEXP},
		{GUID_WICPixelFormat16bppBGRA5551, DXGI_FORMAT_B5G5R5A1_UNORM},
		{GUID_WICPixelFormat16bppBGR565, DXGI_FORMAT_B5G6R5_UNORM},
		{GUID_WICPixelFormat32bppGrayFloat, DXGI_FORMAT_R32_FLOAT},
		{GUID_WICPixelFormat16bppGrayHalf, DXGI_FORMAT_R16_FLOAT},
		{GUID_WICPixelFormat16bppGray, DXGI_FORMAT_R16_UNORM},
		{GUID_WICPixelFormat8bppGray, DXGI_FORMAT_R8_UNORM},
		{GUID_WICPixelFormat8bppAlpha, DXGI_FORMAT_A8_UNORM},
		{GUID_WICPixelFormat96bppRGBFloat, DXGI_FORMAT_R32G32B32_FLOAT},
	};

	// dxgiフォーマットを宣言
	auto dxgiFormat = DXGI_FORMAT{};

	// dxgiフォーマット一覧から取得した画像フォーマットと同じものを探す
	for (auto& wic2dxgi_format : wic2dxgi_formats)
	{
		if (InlineIsEqualGUID(wicFormat, wic2dxgi_format.wic)) {
			dxgiFormat = wic2dxgi_format.dxgi;
		}
		// ない場合はUNKNOWNとする
		//dxgiFormat = DXGI_FORMAT_UNKNOWN;
	}

	const auto bufferSize = width * height * 4;
	std::unique_ptr<BYTE[]> source = std::make_unique<BYTE[]>(bufferSize);
	converter->CopyPixels(nullptr, width * 4, width * height * 4, source.get());

	try {
		// テクスチャーを作成
		//texture.reset(new Texture2D(
		//	graphicsDevice,
		//	width,
		//	height,
		//	dxgiFormat,
		//	false
		//	)
		//);
		texture = std::make_unique<Texture2D>(
			graphicsDevice,
			width,
			height,
			dxgiFormat,
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
	texture->SetData(source.get());

	// 位置座標
	XMFLOAT3 position = { 0, 0, 0 };
	// 回転
	XMFLOAT4 rotation = {};
	//XMStoreFloat4(&rotation, XMQuaternionIdentity());
	XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(-10.0f), XMConvertToRadians(-5.0f), 0));
	// スケール
	XMFLOAT3 scale = { 1, 1, 1 };

	// カメラの位置座標
	constexpr XMFLOAT3 eyePosition = { 0.0f, 1.0f, -3.0f };
	// カメラの回転
	XMFLOAT4 cameraRotation = {};
	XMStoreFloat4(&cameraRotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(20.0f), 0, 0));

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
			//XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(0, XMConvertToRadians(1.0f), 0)));
			XMQuaternionMultiply(XMLoadFloat4(&rotation), XMQuaternionRotationRollPitchYaw(0, 0, 0)));
		
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
