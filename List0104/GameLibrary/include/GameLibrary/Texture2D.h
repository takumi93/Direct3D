#pragma once

#include <GameLibrary/Graphics.h>
#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_4.h>
#include <vector>

namespace GameLibrary
{
	/// <summary>
	/// 2D テクスチャとシェーダー リソース ビュー、サンプラー状態オブジェクトを管理します。
	/// </summary>
	class Texture2D final : public GraphicsResource
	{
	public:
		Texture2D(ID3D11Device5* graphicsDevice,
			UINT width, UINT height, DXGI_FORMAT format, const std::byte* source);
		Texture2D(ID3D11Device5* graphicsDevice,
			UINT width, UINT height, UINT mipLevels, DXGI_FORMAT format,
			const std::vector<D3D11_SUBRESOURCE_DATA>& initialData);

		/// <summary>
		/// テクスチャの幅を取得します。
		/// </summary>
		/// <returns>テクスチャの幅</returns>
		inline UINT GetWidth() const noexcept { return width; }
		/// <summary>
		/// テクスチャの高さを取得します。
		/// </summary>
		/// <returns>テクスチャの高さ</returns>
		inline UINT GetHeight() const noexcept { return height; }
		/// <summary>
		/// テクスチャ形式を取得します。
		/// </summary>
		/// <returns>DXGI フォーマット</returns>
		inline DXGI_FORMAT GetFormat() const noexcept { return format; }
		/// <summary>
		/// テクスチャ内のミップマップ レベルの最大数を取得します。
		/// </summary>
		/// <returns>ミップマップ レベル</returns>
		inline UINT GetMipLevels() const noexcept { return mipLevels; }
		/// <summary>
		/// テクスチャ形式を設定します。
		/// </summary>
		/// <param name="value">DXGI フォーマット</param>
		inline void SetFormat(DXGI_FORMAT value) noexcept { format = value; }

		/// <summary>
		/// ID3D11Texture2D1 のネイティブ ポインターを取得します。
		/// </summary>
		/// <returns>ネイティブ ポインター</returns>
		ID3D11Texture2D1* GetNativePointer() noexcept { return nativePointer.Get(); }
		/// <summary>
		/// このテクスチャのシェーダーリソースビューを取得します。
		/// </summary>
		/// <returns>シェーダーリソースビュー</returns>
		ID3D11ShaderResourceView1* GetView() noexcept { return view.Get(); }

		/// <summary>
		/// テクスチャをサンプリングするときに使用するフィルター方法を取得します。
		/// </summary>
		/// <returns>フィルター方法</returns>
		inline D3D11_FILTER GetFilter() const noexcept { return filter; }
		/// <summary>
		/// 0 ~ 1 の範囲外の u テクスチャ座標を解決するために使用するメソッドを取得します。
		/// </summary>
		/// <returns>テクスチャの境界外にあるテクスチャ座標を解決する手法</returns>
		inline D3D11_TEXTURE_ADDRESS_MODE GetAddressU() const noexcept { return addressU; }
		/// <summary>
		/// 0 ~ 1 の範囲外の v テクスチャ座標を解決するために使用するメソッドを取得します。
		/// </summary>
		/// <returns>テクスチャの境界外にあるテクスチャ座標を解決する手法</returns>
		inline D3D11_TEXTURE_ADDRESS_MODE GetAddressV() const noexcept { return addressV; }
		/// <summary>
		/// 0 ~ 1 の範囲外の w テクスチャ座標を解決するために使用するメソッドを取得します。
		/// </summary>
		/// <returns>テクスチャの境界外にあるテクスチャ座標を解決する手法</returns>
		inline D3D11_TEXTURE_ADDRESS_MODE GetAddressW() const noexcept { return addressW; }
		/// <summary>
		/// 計算されたミップマップ レベルからのオフセット値を取得します。
		/// </summary>
		/// <returns>オフセット値</returns>
		inline float GetMipLODBias() const noexcept { return mipLODBias; }
		/// <summary>
		/// Filter で D3D11_FILTER_ANISOTROPIC または D3D11_FILTER_COMPARISON_ANISOTROPIC が
		/// 指定された場合に使用されるクランプ値。有効な値は 1 から 16 の間です。
		/// </summary>
		/// <returns>オフセット値</returns>
		inline UINT GetMaxAnisotropy() const noexcept { return maxAnisotropy; }

		/// <summary>
		/// テクスチャをサンプリングするときに使用するフィルター方法を設定します。
		/// </summary>
		/// <param name="value">フィルター方法</param>
		inline void SetFilter(D3D11_FILTER value) noexcept { filter = value; }
		/// <summary>
		/// 0 ~ 1 の範囲外のテクスチャ座標を解決するために使用するメソッドを設定します。
		/// </summary>
		/// <param name="value">テクスチャの境界外にあるテクスチャ座標を解決する手法</param>
		inline void SetAddress(D3D11_TEXTURE_ADDRESS_MODE value) noexcept { addressU = value; addressV = value; addressW = value; }
		/// <summary>
		/// 0 ~ 1 の範囲外の u テクスチャ座標を解決するために使用するメソッドを設定します。
		/// </summary>
		/// <param name="value">テクスチャの境界外にあるテクスチャ座標を解決する手法</param>
		inline void SetAddressU(D3D11_TEXTURE_ADDRESS_MODE value) noexcept { addressU = value; }
		/// <summary>
		/// 0 ~ 1 の範囲外の v テクスチャ座標を解決するために使用するメソッドを設定します。
		/// </summary>
		/// <param name="value">テクスチャの境界外にあるテクスチャ座標を解決する手法</param>
		inline void SetAddressV(D3D11_TEXTURE_ADDRESS_MODE value) noexcept { addressV = value; }
		/// <summary>
		/// 0 ~ 1 の範囲外の w テクスチャ座標を解決するために使用するメソッドを設定します。
		/// </summary>
		/// <param name="value">テクスチャの境界外にあるテクスチャ座標を解決する手法</param>
		inline void SetAddressW(D3D11_TEXTURE_ADDRESS_MODE value) noexcept { addressW = value; }
		/// <summary>
		/// Offset from the calculated mipmap level.
		/// 計算されたミップマップ レベルからのオフセット値を設定します。
		/// </summary>
		/// <returns>オフセット値</returns>
		inline void SetMipLODBias(float value) noexcept { mipLODBias = value; }
		/// <summary>
		/// Filter で D3D11_FILTER_ANISOTROPIC または D3D11_FILTER_COMPARISON_ANISOTROPIC が
		/// 指定された場合に使用されるクランプ値。有効な値は 1 から 16 の間です。
		/// </summary>
		/// <returns>オフセット値</returns>
		inline void SetMaxAnisotropy(UINT value) noexcept { maxAnisotropy = value; }

		/// <summary>
		/// このテクスチャのサンプラーを取得します。
		/// </summary>
		/// <returns>サンプラー ステート</returns>
		ID3D11SamplerState* GetSamplerState() noexcept { return samplerState.Get(); }

		void Apply();

	private:
		UINT width = 0;
		UINT height = 0;
		DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		Microsoft::WRL::ComPtr<ID3D11Texture2D1> nativePointer;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> view;

		D3D11_FILTER filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		UINT mipLevels = 0;
		D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		D3D11_TEXTURE_ADDRESS_MODE addressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		float mipLODBias = 0.0f;
		UINT maxAnisotropy = 1;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	};
}
