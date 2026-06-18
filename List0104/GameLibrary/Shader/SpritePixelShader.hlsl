#include "SpriteShader.hlsli"

float4 main(PixelShaderInput_Sprite input) : SV_TARGET
{
    return AlbedoColor * MainTexture.Sample(MainTextureSampler, input.texCoord);
}
