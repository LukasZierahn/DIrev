#include "Sky.h"
#include "RenderClass.h"
#include "Model.h"

Sky::Sky(RenderClass* rend)
{
	Render = rend;

	Render->GetDevice(&Device);
	Render->GetDeviceContext(&DeviceContext);

	for (int i = 0; i < SkySize; i++)
	{
		SkyList[i] = new Model(Render, (rand() % Model_Type_CloudLength) + Model_Type_Cloud, (i / SkySizeRoot) * 10, -3, (i % SkySizeRoot) * 10);
		Render->AddModel(SkyList[i]);
	}

	pos = XMVectorSet((SkySizeRoot * -5) + 5, height, (SkySizeRoot * -5) + 5, 1);
	UpdatePosition();
}

void Sky::MoveSky(int t)
{
	AddPosition(SkySpeed * t, SkySpeed * t);
}

void Sky::AddPosition(float x, float z)
{
	XMVECTOR BufferVector = XMVectorSet(x, height, z, 1);
	pos += BufferVector;
	UpdatePosition();
}

void Sky::SetPosition(float x, float z)
{
	pos = XMVectorSet(x, height, z, 1);
	UpdatePosition();
}

void Sky::UpdatePosition()
{
	for (int i = 0; i < SkySize; i++)
	{
		SkyList[i]->SetPosition(((i / SkySizeRoot) * 10) + XMVectorGetX(pos), height, ((i % SkySizeRoot) * 10) + XMVectorGetZ(pos));
	}
}

Sky::~Sky()
{
	for (int i = 0; i < SkySize; i++)
	{
		delete SkyList[i];
	}
}
