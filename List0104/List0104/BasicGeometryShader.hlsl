#include "BasicShader.hlsli"

[maxvertexcount(3)]
void main(
	triangle GSInput input[3],
	inout TriangleStream< GSOutput > output)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.position = mul(input[i].position, WorldViewProjection);
		element.color = input[i].color;
		output.Append(element);
	}
}