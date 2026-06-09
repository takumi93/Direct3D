#include "SampleGame.h"
#include <iterator>
#include <comdef.h>
#include <wincodec.h>

using namespace GameLibrary;
using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace
{
	// 頂点データの配列
	VertexPositionNormalTexture vertices[] = {
		// Front
		{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { 1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ {-1.0f,-1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		// Back
		{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 0.0f } },
		{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 0.0f } },
		{ {-1.0f,-1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 0.0f, 1.0f } },
		{ { 1.0f,-1.0f,-1.0f }, { 0.0f, 0.0f,-1.0f }, { 1.0f, 1.0f } },
		// Right
		{ { 1.0f, 1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 1.0f,-1.0f,-1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 1.0f,-1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
		// Left
		{ {-1.0f, 1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {-1.0f, 1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {-1.0f,-1.0f, 1.0f }, {-1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
		{ {-1.0f,-1.0f,-1.0f }, {-1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
		// UP
		{ {-1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ {-1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 1.0f, 1.0f,-1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		// DOWN
		{ { 1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {-1.0f,-1.0f, 1.0f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ {-1.0f,-1.0f,-1.0f }, { 0.0f,-1.0f, 0.0f }, { 1.0f, 1.0f } },
	};
	// インデックスデータの配列
	UINT32 indices[] = {
		 0,  1,  2,  3,  2,  1,	// Front
		 4,  5,  6,  7,  6,  5,	// Back
		 8,  9, 10, 11, 10,  9,	// Right
		12, 13, 14, 15, 14, 13,	// Left
		16, 17, 18, 19, 18, 17,	// Top
		20, 21, 22, 23, 22, 21,	// Bottom
	};

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
}

SampleGame::SampleGame(const ProjectSettings& settings)
	: Game(settings)
{

}

void SampleGame::OnInitialize()
{
	HRESULT hr = S_OK;

	indexCount = static_cast<UINT>(std::size(indices));

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

	// バッファーを作成
	try {
		// 定数バッファーを作成
		//constantBuffer.reset(new ConstantBuffer(graphics, sizeof constantBufferPerFrame));
		constantBuffer = std::make_unique<ConstantBuffer>(graphics, sizeof(MatricesPerFrame));
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("定数バッファーを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return;
	}

	// シェーダーを作成
	try {
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
		return;
	}

	// 入力レイアウトを作成
	try {
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
			static_cast<UINT>(std::size(VertexPositionNormalTexture::inputElementDescs)),	// inputElementDescs配列の数
			vertexShader->GetBytecode(),								// 入力を受け取る頂点シェーダーのバイトコード
			vertexShader->GetBytecodeLength()							// バイトコードのサイズ
		);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("入力レイアウトを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return;
	}


	//// 入力レイアウトを作成
	//inputLayout.reset(new InputLayout(device.Get(),
	//	VertexPositionNormalTexture::inputElementDescs, static_cast<UINT>(std::size(VertexPositionNormalTexture::inputElementDescs)),
	//	vertexShader->GetBytecode(), vertexShader->GetBytecodeLength()));

	// 画像データの読み込み
	// WIC ファクトリーを作成
	ComPtr<IWICImagingFactory2> factory;
	hr = CoCreateInstance(CLSID_WICImagingFactory2, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	// BITMAP デコーダーを作成
	ComPtr<IWICBitmapDecoder> decorder;
	hr = factory->CreateDecoderFromFilename(
		L"Sample.png",
		NULL, GENERIC_READ, WICDecodeOptions::WICDecodeMetadataCacheOnDemand,
		&decorder);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	//ComPtr<IWICDdsDecoder> ddsDecoder;
	//hr = decoder.As(&ddsDecoder);

	// BITMAPフレームを取得
	ComPtr<IWICBitmapFrameDecode> bitmapFrame;
	hr = decorder->GetFrame(0, &bitmapFrame);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	//UINT arrayIndex = 16;
	//UINT mipLevel = 32;
	//UINT sliceIndex = 32;
	//hr = ddsDecoder->GetFrame(arrayIndex, mipLevel, sliceIndex, &frame);
	//if (FAILED(hr)) {
	//	throw _com_error(hr);
	//}

	// FormatConverter
	ComPtr<IWICFormatConverter> converter;
	hr = factory->CreateFormatConverter(&converter);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	hr = converter->Initialize(
		bitmapFrame.Get(),
		GUID_WICPixelFormat32bppRGBA,
		WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

	UINT width = 0;
	UINT height = 0;
	hr = converter->GetSize(&width, &height);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}

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

	// DXGI_FORMAT を検索
	auto dxgiFormat = DXGI_FORMAT{};
	for (auto& wic2dxgi_format : wic2dxgi_formats)
	{
		if (InlineIsEqualGUID(wicFormat, wic2dxgi_format.wic)) {
			dxgiFormat = wic2dxgi_format.dxgi;
		}
	}
	const auto bufferSize = width * height * 4;
	std::unique_ptr<BYTE[]> source = std::make_unique<BYTE[]>(bufferSize);
	converter->CopyPixels(nullptr, width * 4, bufferSize, source.get());


	// 出力ストリームを作成
	ComPtr<IWICStream> outputStream;
	hr = factory->CreateStream(&outputStream);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	// 出力するファイル名を設定
	hr = outputStream->InitializeFromFilename(L"Sample.dds", GENERIC_WRITE);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	// DDS対応のエンコーダーを作成
	ComPtr<IWICBitmapEncoder> encoder;
	hr = factory->CreateEncoder(GUID_ContainerFormatDds, NULL, &encoder);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	hr = encoder->Initialize(outputStream.Get(), WICBitmapEncoderCacheOption::WICBitmapEncoderNoCache);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	ComPtr<IWICBitmapFrameEncode> bitmapFrameEncode;
	ComPtr<IPropertyBag2> propertyBag;
	hr = encoder->CreateNewFrame(&bitmapFrameEncode, &propertyBag);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}


	// IWICDdsEncoder を取得
	ComPtr<IWICDdsEncoder> ddsEncoder;
	hr = encoder.As(&ddsEncoder);
	if (FAILED(hr)) {
		throw _com_error(hr);
	}
	auto ddsParameters = WICDdsParameters{
		.Width = 64,
		.Height = 64,
		.Depth = 1,
		.MipLevels = 7,
		.ArraySize = 1,
		.DxgiFormat = DXGI_FORMAT::DXGI_FORMAT_BC1_UNORM,
		.Dimension = WICDdsDimension::WICDdsTexture2D,
		.AlphaMode = WICDdsAlphaMode::WICDdsAlphaModeOpaque,
	};
	hr = ddsEncoder->SetParameters(&ddsParameters);
	if (FAILED(hr)) {
		throw hr;
	}
	//ComPtr<IWICBitmapFrameEncode> newFrame;
	//hr = ddsEncoder->CreateNewFrame(&newFrame, NULL, NULL, NULL);
	//if (FAILED(hr)) {
	//	throw _com_error(hr);
	//}

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
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			false
		);
	}
	catch (const _com_error& error) {
		OutputDebugString(TEXT("ERROR: "));
		OutputDebugString(error.ErrorMessage());
		OutputDebugString(TEXT("\n"));
		MessageBox(NULL, TEXT("テクスチャを作成できませんでした。"), TEXT("エラー"), MB_OK);
		return;
	}

	// ピクセルデータを更新
	texture->SetData(source.get());
}

void SampleGame::OnUpdate() noexcept
{
	// 定数バッファーへ転送するデータソースを準備
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix *= XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//XMVECTOR axis = XMVectorSet(1, 1, 0, 0);
	//worldMatrix *= XMMatrixRotationAxis(axis, time);
	worldMatrix *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	// ビュー行列を計算
	XMVECTOR eyePosition = XMVectorSet(0, 1, -10, 1);
	XMVECTOR focusPosition = XMVectorSet(0, 1, 0, 1);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	XMMATRIX viewMatrix =
		XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	// プロジェクション行列を計算
	float fovAngleY = 60.0f;
	float aspectRatio = 640 / 480.0f;
	float nearZ = 0.3f;
	float farZ = 1000.0f;
	XMMATRIX projectionMatrix =
		XMMatrixPerspectiveFovLH(XMConvertToRadians(fovAngleY), aspectRatio, nearZ, farZ);

	XMStoreFloat4x4(
		&matricesPerFrame.worldMatrix,
		XMMatrixTranspose(worldMatrix));
	XMStoreFloat4x4(
		&matricesPerFrame.viewMatrix,
		XMMatrixTranspose(viewMatrix));
	XMStoreFloat4x4(
		&matricesPerFrame.projectionMatrix,
		XMMatrixTranspose(projectionMatrix));
	XMStoreFloat4x4(
		&matricesPerFrame.worldViewProjectionMatrix,
		XMMatrixTranspose(worldMatrix * viewMatrix * projectionMatrix));
	// ライト
	matricesPerFrame.lightPosition = DirectX::XMFLOAT4(1.0f, 2.0f, -2.0f, 0.0f);
	// マテリアル
	if (GetAsyncKeyState('D')) {
		matricesPerFrame.materialDiffuse = DirectX::XMFLOAT4(1, 1, 1, 1);
	}
	else {
		matricesPerFrame.materialDiffuse = DirectX::XMFLOAT4(0, 0, 0, 1);
	}
	if (GetAsyncKeyState('S')) {
		matricesPerFrame.materialSpecularColor = DirectX::XMFLOAT3(1, 1, 1);
		matricesPerFrame.materialSpecularPower = 1;
	}
	else {
		matricesPerFrame.materialSpecularColor = DirectX::XMFLOAT3(0, 0, 0);
	}
}

void SampleGame::OnRender() noexcept
{
	constantBuffer->SetData(&matricesPerFrame);

	// 定数バッファーを更新
	constantBuffer->SetData(&matricesPerFrame);

	// ビューポートを設定
	D3D11_VIEWPORT viewports[] = { viewport, };
	deviceContext->RSSetViewports(static_cast<UINT>(std::size(viewports)), viewports);

	// 頂点バッファーを設定
	ID3D11Buffer* vertexBuffers[1] = { vertexBuffer->GetNativePointer() };
	UINT strides[1] = { sizeof(VertexPositionNormalTexture) };
	UINT offsets[1] = { 0 };
	deviceContext->IASetVertexBuffers(
		0, static_cast<UINT>(std::size(vertexBuffers)),
		vertexBuffers, strides, offsets);

	// シェーダーを設定
	deviceContext->VSSetShader(vertexShader->GetNativePointer(), NULL, 0);
	deviceContext->GSSetShader(geometryShader->GetNativePointer(), NULL, 0);
	deviceContext->PSSetShader(pixelShader->GetNativePointer(), NULL, 0);
	// シェーダーに定数バッファーを設定
	ID3D11Buffer* constantBuffers[1] = { constantBuffer->GetNativePointer() };
	deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(std::size(constantBuffers)), constantBuffers);
	deviceContext->GSSetConstantBuffers(0, static_cast<UINT>(std::size(constantBuffers)), constantBuffers);
	deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(std::size(constantBuffers)), constantBuffers);
	// ピクセルシェーダーにテクスチャーを設定
	ID3D11ShaderResourceView* textureViews[] = { texture->GetShaderResourceView(), };
	deviceContext->PSSetShaderResources(0, static_cast<UINT>(std::size(textureViews)), textureViews);
	ID3D11SamplerState* samplerStates[] = { texture->GetSamplerState(), };
	deviceContext->PSSetSamplers(0, static_cast<UINT>(std::size(samplerStates)), samplerStates);

	// 頂点バッファーと頂点シェーダーの組合せに対応した入力レイアウトを設定
	deviceContext->IASetInputLayout(inputLayout->GetNativePointer());
	// プリミティブトポロジーとしてトライアングルを設定
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// インデックスバッファーを設定
	deviceContext->IASetIndexBuffer(indexBuffer->GetNativePointer(), DXGI_FORMAT_R32_UINT, 0);

	// 描画
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
