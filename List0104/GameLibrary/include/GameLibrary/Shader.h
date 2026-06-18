#pragma once

#include <GameLibrary/Graphics.h>
#include <wrl/client.h>
#include <d3d11_4.h>

namespace GameLibrary
{
	/// <summary>
	/// シェーダーのネイティブ ポインターを管理するクラスを作成するテンプレートを表します。
	/// </summary>
	/// <typeparam name="T">ネイティブ ポインターの型</typeparam>
	template<typename T>
	class BaseShader : public GraphicsResource
	{
	public:
		BaseShader(ID3D11Device5* graphicsDevice);
		virtual ~BaseShader() = default;

		T* GetNativePointer() const noexcept;

	protected:
		Microsoft::WRL::ComPtr<T> shader;
	};

	template<typename T>
	inline BaseShader<T>::BaseShader(ID3D11Device5* graphicsDevice)
		: GraphicsResource(graphicsDevice)
	{

	}

	template<typename T>
	inline T* BaseShader<T>::GetNativePointer() const noexcept
	{
		return shader.Get();
	}

	/// <summary>
	/// 頂点シェーダーを表します。
	/// </summary>
	class VertexShader final : public BaseShader<ID3D11VertexShader>
	{
	public:
		VertexShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~VertexShader() = default;

		void Apply(ID3D11DeviceContext4* deviceContext) noexcept;
	};

	/// <summary>
	/// ジオメトリ シェーダーを表します。
	/// </summary>
	class GeometryShader final : public BaseShader<ID3D11GeometryShader>
	{
	public:
		GeometryShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~GeometryShader() = default;

		void Apply(ID3D11DeviceContext4* deviceContext) noexcept;
	};

	/// <summary>
	/// ピクセル シェーダーを表します。
	/// </summary>
	class PixelShader final : public BaseShader<ID3D11PixelShader>
	{
	public:
		PixelShader(ID3D11Device5* graphicsDevice, const void* shaderBytecode, size_t bytecodeLength);
		~PixelShader() = default;

		void Apply(ID3D11DeviceContext4* deviceContext) noexcept;
	};
}
