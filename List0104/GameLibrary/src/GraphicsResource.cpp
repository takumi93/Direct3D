#include <GameLibrary/Graphics.h>

using namespace GameLibrary;

/// <summary>
/// このクラスの新しいインスタンスを初期化します。
/// </summary>
/// <param name="graphicsDevice">グラフィックス デバイス</param>
GraphicsResource::GraphicsResource(ID3D11Device5* graphicsDevice) noexcept
	: graphicsDevice(graphicsDevice)
{

}

/// <summary>
/// このリソースを作成したグラフィックス デバイスを取得します。
/// </summary>
/// <returns>グラフィックス デバイス</returns>
ID3D11Device5* GraphicsResource::GetDevice() noexcept
{
	return graphicsDevice.Get();
}
