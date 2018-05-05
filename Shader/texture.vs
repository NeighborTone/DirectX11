//グローバル
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};


//Typedefs
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


//バーテックスシェーダー
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	//適切な行列計算のために位置ベクトルを4単位に変更
	input.position.w = 1.0f;
	
	//ワールド、ビュー、投影行列に対して頂点の位置を計算
	output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	 //ピクセルシェーダのテクスチャ座標を格納
    output.tex = input.tex;
    
    return output;

}





