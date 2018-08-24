struct Vertex								//���_�p�\����
{
    float4 position : POSITION;			//�I�u�W�F�N�g��ԓ��̒��_�ʒu�B
    float3 normal : NORMAL;				//�@���x�N�g��
    float2 uv : TEXCOORD;				//�e�N�X�`���[���W
};
struct Light
{
    int type;
    float3 position;
    float3 direction;
    float range;
    float4 color;
};
struct Pixel								//�s�N�Z���p�\����
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
     float3 worldPosition : TEXCOORD1;
};

cbuffer Light : register(b1)
{
    Light lights[100];
};
cbuffer Camera : register(b0)	//�萔�o�b�t�@
{
    matrix view;
    matrix projection;
};
cbuffer Object : register(b5)
{
    matrix world;
};
Texture2D texture0 : register(t0);			//�󂯎�����e�N�X�`���[�����̃��W�X�^�[�Ɋ��蓖�Ă�
SamplerState sampler0 : register(s0);		//�󂯎�����T���v���[�����̃��W�X�^�[�Ɋ��蓖�Ă�

Pixel VS(Vertex vertex)					//���_�o�͗p�֐�
{
    Pixel output;
    output.position = mul(vertex.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.normal = mul(vertex.normal, (float3x3)world);
    output.uv = vertex.uv;
    output.worldPosition = mul(vertex.position, world).xyz;
    return output;
}
float4 PS(Pixel pixel) : SV_TARGET	//�����_�����O�^�[�Q�b�g�ɏo��
{
     float3 diffuseColor = texture0.Sample(sampler0, pixel.uv).rgb;
    float3 normal = normalize(pixel.normal);
    float3 lightColor = float3(0.0, 0.0, 0.0);
    for (int i = 0; i < 100; i++)
    {
        if (lights[i].type == 0)
        {
            float shade = max(0, dot(-lights[i].direction, normal));
            lightColor += lights[i].color.xyz * shade;
        }
        if (lights[i].type == 1)
        {
            float3 lightDirection = normalize(lights[i].position.xyz - pixel.worldPosition);
            float shade = max(0, dot(normal, lightDirection));
            float attenuation = max(0, (lights[i].range - length(lights[i].position.xyz - pixel.worldPosition)) / lights[i].range);
            lightColor += lights[i].color.xyz * shade * attenuation;
        }
    }
    float3 ambientColor = float3(1.0, 1.0, 1.0) * 0.1;
    return float4(diffuseColor * (lightColor + ambientColor), 1);
}