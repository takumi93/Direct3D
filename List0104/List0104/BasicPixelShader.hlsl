#include "BasicShader.hlsli"

Texture2D diffuseTexture;
SamplerState diffuseTextureSampler;

float4 main(PSInput input) : SV_TARGET
{
    // 面から光源を指す正規化ベクトルL(平行光源(w=0)と点光源(w=1))
    float3 light = normalize(LightPosition.xyz - LightPosition.w * input.worldPosition.xyz);
    // 鏡面反射
    // light と worldNormal を使って計算する
    float diffuse = max(dot(normalize(light.xyz), input.worldNormal), 0.0f);
    float3 diffuseColor = diffuse * MaterialDiffuse.rgb;
    
    // 鏡面反射(dotで内積を求める)
    //float3 reflectDir = 2 * input.worldNormal * dot(input.worldNormal, light) - light;
    float3 reflectDir = reflect(-light, input.worldNormal);
    float3 viewDir = normalize(ViewPosition - input.worldPosition).xyz;
    float specular = pow(saturate(dot(reflectDir, viewDir)), MaterialSpecularPower);
    float3 specularColor = specular * MaterialSpecularColor.rgb;
    
    float4 texel = diffuseTexture.Sample(diffuseTextureSampler, input.texCoord);

    return float4(texel.rgb * diffuseColor + specularColor, MaterialDiffuse.a * texel.a);
    //return float4(texel.rgb, MaterialDiffuse.a * texel.a);
}