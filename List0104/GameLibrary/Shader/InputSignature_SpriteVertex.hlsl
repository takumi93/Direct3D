#include "BasicShader.hlsli"

float4 main(VertexShaderInput_Sprite input) : SV_POSITION
{
    return input.position;
}