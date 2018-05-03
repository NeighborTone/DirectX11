//グローバル

Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global
{
	matrix g_mWVP;//ワールドから射影までの変換行列
};

//構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	//UV座標が頂点シェーダーに入ってくる
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;		//そのままピクセルシェーダーに流す

	return output;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	return g_texColor.Sample(g_samLinear, input.Tex);	//テクスチャーからピクセルを取り出す
}

////グローバル
//cbuffer global
//{
//	matrix g_WVP; //ワールドから射影までの変換行列
//};
//
////
////
////バーテックスシェーダー
//float4 VS(float4 Pos : POSITION) : SV_POSITION
//{
//	Pos = mul(Pos,g_WVP);
//return Pos;
//}
//
////
////
////ピクセルシェーダー
//float4 PS(float4 Pos : SV_POSITION) : SV_Target
//{
//	return float4(1,1,1,1);
//}