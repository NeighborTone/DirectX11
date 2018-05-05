//�O���[�o��
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


//�o�[�e�b�N�X�V�F�[�_�[
PixelInputType TextureVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	//�K�؂ȍs��v�Z�̂��߂Ɉʒu�x�N�g����4�P�ʂɕύX
	input.position.w = 1.0f;
	
	//���[���h�A�r���[�A���e�s��ɑ΂��Ē��_�̈ʒu���v�Z
	output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	 //�s�N�Z���V�F�[�_�̃e�N�X�`�����W���i�[
    output.tex = input.tex;
    
    return output;

}





