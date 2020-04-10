cbuffer ConstantBuffer : register(b0)
{
	matrix WorldPos;
	matrix WorldRot;
	matrix View;
	matrix Projection;
}

cbuffer ModelBuffer : register(b1)
{
	matrix Scale;
	matrix Position;
	matrix Rotation;
}

struct VertexInputType
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXPOS0;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXPOS0;
};


VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXPOS)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, Rotation);
	output.Pos = mul(Position, output.Pos);

	output.Pos = mul(WorldPos, output.Pos);
	output.Pos = mul(output.Pos, WorldRot);

	output.Pos = mul(output.Pos, Scale);

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Tex = Tex;
	return output;
}

Texture2D shaderTexture;
SamplerState SampleType;

float4 PS(float4 position : SV_POSITION, float2 Tex : TEXPOS) : SV_TARGET
{
	float4 TextureColour;
	
	TextureColour = shaderTexture.Sample(SampleType, Tex);

	return TextureColour;
}