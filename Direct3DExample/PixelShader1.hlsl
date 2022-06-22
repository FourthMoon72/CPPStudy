Texture2D shaderTexture;
SamplerState Sampler;
//{  
//	Filter = MIN_MAG_MIP_LINEAR;		// ��������� / Ȯ�������� / 
//	AddressU = Wrap;					// rap ���δ°� / ���� ���°� ���� 
//	AddressV = Wrap;					// ������ ������ �ѹ� �� �׸��� 
//};

struct PIn
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PIn input) : SV_TARGET
{
	float4 pix = shaderTexture.Sample(Sampler, input.tex);
	//float gray = (pix[0] + pix[1] + pix[2]) / 3;

	//if (gray > 0.36)
	//{
	//	gray = 1.0f;
	//}
	//else
	//{
	//	gray = 0.0f;
	//}
	//pix[0] = pix[1] = pix[2] = gray;

	return pix;
	//return shaderTexture.Sample(Sampler, input.tex);
}