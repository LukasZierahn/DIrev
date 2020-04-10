#include "Include.h"
#include "Model.h"
#include "RenderClass.h"
#include "ModelLoader.h"
#include "Texture.h"

Model::Model(RenderClass* RndCls, int ModTyp, bool col)
{
	ModelType = ModTyp;
	int TexTyp = ModTyp;
	Render = RndCls;

	Render->GetDevice(&Device);
	Render->GetDeviceContext(&DeviceContext);
	Render->GetSamplerState(&SamplerState);

	Scale = XMMatrixScaling(ScaleByModelType[ModTyp], ScaleByModelType[ModTyp], ScaleByModelType[ModTyp]);

	ModBufClass.Position = XMMatrixIdentity();
	ModBufClass.Rotation = XMMatrixIdentity();
	ModBufClass.Scale = XMMatrixTranspose(Scale);

	Render->ModLoad->GetModel(ModTyp, &Data);
	Render->ModLoad->GetTexture(TexTyp, &tex);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ModelBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Device->CreateBuffer(&bd, nullptr, ModBuf.GetAddressOf());
}

Model::Model(RenderClass* RndCls, int ModTyp, float x, float y, float z, bool col)
{
	ModelType = ModTyp;
	int TexTyp = ModTyp;
	Render = RndCls;

	Render->GetDevice(&Device);
	Render->GetDeviceContext(&DeviceContext);
	Render->GetSamplerState(&SamplerState);

	Position = XMVectorSet(x, y, z, 1);
	Scale = XMMatrixScaling(ScaleByModelType[ModTyp], ScaleByModelType[ModTyp], ScaleByModelType[ModTyp]);

	ModBufClass.Position = XMMatrixIdentity();
	ModBufClass.Rotation = XMMatrixIdentity();
	ModBufClass.Scale = XMMatrixTranspose(Scale);

	Render->ModLoad->GetModel(ModTyp, &Data);
	Render->ModLoad->GetTexture(TexTyp, &tex);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ModelBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Device->CreateBuffer(&bd, nullptr, ModBuf.GetAddressOf());
}

void Model::Draw()
{
	//buffer stuff
	ModBufClass.Position = XMMatrixTranslationFromVector(Position);
	ModBufClass.Rotation = XMMatrixRotationRollPitchYaw(Roll, Pitch, Yawn);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	DeviceContext->Map(ModBuf.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	memcpy(MappedResource.pData, &ModBufClass, sizeof(ModBufClass));

	DeviceContext->Unmap(ModBuf.Get(), 0);

	DeviceContext->VSSetConstantBuffers(1, 1, ModBuf.GetAddressOf());

	//drawing stuff
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, Data->VertexBuffer.GetAddressOf(), &stride, &offset);

	DeviceContext->IASetIndexBuffer(Data->IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	DeviceContext->PSSetSamplers(0, 1, SamplerState.GetAddressOf());

	tex->SetTextureAsShaderRessource();

	DeviceContext->DrawIndexed(Data->IndexCount, 0, 0);
}

void Model::SetRotation(float R, float P, float Y)
{
	Roll = R;
	Pitch = P;
	Yawn = Y;
}

void Model::AddRotation(float R, float P, float Y)
{
	Roll += R;
	Pitch += P;
	Yawn += Y;
}

void Model::SetPosition(float x, float y, float z)
{
	Position = XMVectorSet(x, y, z, 1);
}

void Model::SetPosition(XMVECTOR newpos)
{
	Position = newpos;
}

void Model::AddPosition(float x, float y, float z, bool collision, bool forceCollision)
{
	if (collision && Data->HitBoxLoaded && Data->BoundingBoxLoaded || forceCollision)
	{
		XMVECTOR Movement = XMVectorSet(x, y, z, 1);
		XMVECTOR* NewPos;
		Render->CollisionMovement(&Position, &Movement, &NewPos);
		Position = *NewPos;
	}
	else
	{
		XMVECTOR BufferVector = XMVectorSet(x, y, z, 1);
		Position += BufferVector;
	}
}

float Model::GetBounding(int BoundingIndex)
{
	if (Data->BoundingBoxLoaded)
	{
		float PositionOffset = 0.0f;

		if (BoundingIndex < 2)
		{
			PositionOffset = XMVectorGetX(Position);
		}
		else if (BoundingIndex > 3)
		{
			PositionOffset = XMVectorGetZ(Position);
		}
		else
		{
			PositionOffset = XMVectorGetY(Position);
		}

		return Data->BoundingBox[BoundingIndex] + PositionOffset;
	}
	else
	{
		return 0.0f;
	}
}

bool Model::CheckForCollision(XMVECTOR* pos, XMVECTOR* move, XMVECTOR** outputPos)
{
	*outputPos = pos;
	return false;
}

Model::~Model()
{
}