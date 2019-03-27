#include "Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float random(float2 p)
{
	// ランダムの公式
	// ランダムの値を作る
	// dot = 内積
	// frac = 小数点部を取り出す
	return frac(sin(dot(p, float2(12.9898f, 78.233f)))* 43758.5453f);
}

float random2(float2 st)
{
	st = float2(dot(st, float2(127.1f, 311.7f)), dot(st, float2(269.5f, 183.3f)));
	return (float)(-1.0 + 2.0f * frac(sin(st) * 43758.5453123f));
}

float perlinNoise(float2 st)
{
	float2 p = floor(st);
	float2 f = frac(st);
	float2 u = f * f * (3.0f - 2.0f * f);

	float v00 = random2(p + float2(0, 0));
	float v10 = random2(p + float2(1, 0));
	float v01 = random2(p + float2(0, 1));
	float v11 = random2(p + float2(1, 1));

	return lerp(
		lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
		lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
		u.y) + 0.5f;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	// ベースとなるメロンの画像
	float4 base = tex.Sample(samLinear, input.Tex);

	// ディゾルブをする為の「ルール画像」
	float4 dis = tex2.Sample(samLinear, input.Tex);

	// ルール画像をグレーにする
	float gray = dis.r * 0.3 + dis.g * 0.6 + dis.b * 0.1;

	 /*グレーの値より描画時のdiffuse.xの値が大きい時はbaseの色を出す*/
	if (gray < diffuse.x + 0.01f) {
		base = base + float4(0.6f, 0.6f, 0.6f, 0.0f);
	}

	// グレーの値より描画時のdiffuse.xの値が小さい時は透明度を0にする。
	if (gray < diffuse.x) {
		base.a = 0;
	}

	return base;
}