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
struct Vertex								//���_�p�\����
{
    float4 position : POSITION;			//�I�u�W�F�N�g��ԓ��̒��_�ʒu�B
    float3 normal : NORMAL;				//�@���x�N�g��
    float2 uv : TEXCOORD;				//�e�N�X�`���[���W
};
struct Pixel								//�s�N�Z���p�\����
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};
Pixel VS(Vertex vertex)					//���_�o�͗p�֐�
{
    Pixel output;
    output.position = mul(vertex.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.normal = mul(vertex.normal, (float3x3)world);
    output.uv = vertex.uv;
    return output;
}
float4 PS(Pixel pixel) : SV_TARGET	//�����_�����O�^�[�Q�b�g�ɏo��
{
    float3 normal = normalize(pixel.normal);
    float3 lightDirection = normalize(float3(0.25, -1.0, 0.5));			//���C�g�̌���
    float3 lightColor = float3(1.0, 1.0, 1.0);							//���C�g�̐F
    float4 diffuseColor = texture0.Sample(sampler0, pixel.uv);			//�g�U��
    float3 diffuseIntensity = dot(-lightDirection, normal) * lightColor;	//�g�U���̋���
    float3 ambientIntensity = lightColor * 0.5;							//�����̋���
    return diffuseColor * float4(diffuseIntensity + ambientIntensity, 1);	
}