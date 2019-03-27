#include "Particle.hlsli"


Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	// ��L�̕ω���base�摜��UV�ŃT���v�����O
	float4 base = tex.Sample(samLinear, input.Tex);

	return base;
}