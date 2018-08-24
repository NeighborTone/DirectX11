cbuffer Camera : register(b0)
{
    matrix view;
    matrix projection;
};
cbuffer Object : register(b5)
{
    matrix world;
};
cbuffer Sprite : register(b6)
{
    float4 color;
};
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);
struct Vertex
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};
struct Pixel
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};
Pixel VS(Vertex vertex)
{
    Pixel output;
    output.position = mul(vertex.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.uv = vertex.uv;
    return output;
}
float4 PS(Pixel pixel) : SV_TARGET
{
	//pixel.uv.x += 0.3;
    float4 textureColor = texture0.Sample(sampler0, pixel.uv);
    if (textureColor.a <= 0)
        discard;					//アルファ値を有効にする
    return textureColor * color;
}