//�O���[�o��

Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global
{
	matrix g_mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
};

//�\����
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	//UV���W�����_�V�F�[�_�[�ɓ����Ă���
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;		//���̂܂܃s�N�Z���V�F�[�_�[�ɗ���

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	return g_texColor.Sample(g_samLinear, input.Tex);	//�e�N�X�`���[����s�N�Z�������o��
}