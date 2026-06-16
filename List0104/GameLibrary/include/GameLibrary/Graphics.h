#pragma once

#include <Windows.h>
#include <iterator>
#include <wrl/client.h>
#include <d3d11_4.h>
#include <DirectXMath.h>

namespace GameLibrary
{
	struct Vertex_Sprite
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT2 TexCoord;

		static constexpr UINT GetSize() { return static_cast<UINT>(sizeof(Vertex_Sprite)); }

		static constexpr D3D11_INPUT_ELEMENT_DESC InputElementDescs[] = {
			{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0,    DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		static constexpr UINT NumElements = static_cast<UINT>(std::size(InputElementDescs));
	};

	class GraphicsResource
	{
	public:
		// noexceptで例外が出ない事を指している（作った人以外が分かりやすいようにnoexceptを入れている）
		GraphicsResource(ID3D11Device5* graphicsDevice) noexcept;
		virtual ~GraphicsResource() = default;

		ID3D11Device5* GetDevice() noexcept;

	private:
		// グラフィックデバイス
		Microsoft::WRL::ComPtr<ID3D11Device5> graphicsDevice;
	};
}