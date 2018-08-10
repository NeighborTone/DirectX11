cbuffer Camera : register(b0)	//定数バッファ
{
    matrix view;
    matrix projection;
};
cbuffer Object : register(b1)
{
    matrix world;
};
Texture2D texture0 : register(t0);			//受け取ったテクスチャーを特定のレジスターに割り当てる
SamplerState sampler0 : register(s0);		//受け取ったサンプラーを特定のレジスターに割り当てる
struct Vertex								//頂点用構造体
{
    float4 position : POSITION;			//オブジェクト空間内の頂点位置。
    float3 normal : NORMAL;				//法線ベクトル
    float2 uv : TEXCOORD;				//テクスチャー座標
};
struct Pixel								//ピクセル用構造体
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};
Pixel VS(Vertex vertex)					//頂点出力用関数
{
    Pixel output;
    output.position = mul(vertex.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.normal = mul(vertex.normal, (float3x3)world);
    output.uv = vertex.uv;
    return output;
}
float4 PS(Pixel pixel) : SV_TARGET	//レンダリングターゲットに出力
{
    float3 normal = normalize(pixel.normal);
    float3 lightDirection = normalize(float3(0.25, -1.0, 0.5));			//ライトの向き
    float3 lightColor = float3(1.0, 1.0, 1.0);							//ライトの色
    float4 diffuseColor = texture0.Sample(sampler0, pixel.uv);			//拡散光
    float3 diffuseIntensity = dot(-lightDirection, normal) * lightColor;	//拡散光の強さ
    float3 ambientIntensity = lightColor * 0.5;							//環境光の強さ
    return diffuseColor * float4(diffuseIntensity + ambientIntensity, 1);	
}