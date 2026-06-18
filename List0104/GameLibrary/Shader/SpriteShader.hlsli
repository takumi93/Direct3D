#include "BasicShader.hlsli"

cbuffer ConstantBufferPerMaterial
{
    float4 AlbedoColor;
};

Texture2D MainTexture;
SamplerState MainTextureSampler;
