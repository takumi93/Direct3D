#include "SpriteShader.hlsli"

[maxvertexcount(3)]
void main(
	triangle GeometryShaderInput_Sprite input[3],
	inout TriangleStream<GeometryShaderOutput_Sprite> output)
{
	[unroll]
    for (uint i = 0; i < 3; i++)
    {
        GeometryShaderOutput_Sprite element;
        element.position = mul(input[i].position, matrixWorldViewProjection);
        element.texCoord = input[i].texCoord;
        output.Append(element);
    }
}
