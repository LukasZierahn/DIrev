#pragma once
#include <wrl/client.h>
#include <vector>
#include <Windowsx.h>
#include <d3d11_1.h>
#include <directxmath.h>
#include <D3Dcompiler.h>
#include <directxcolors.h>
#include <fstream>

#ifndef __H__VertConstMod
#define __H__VertConstMod

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

#define Total_Model_Count 5

#define Model_Type_Lebigrand	0
#define Model_Type_Cooge		1
#define Model_Type_Nosu			2

#define Model_Type_Cloud		3
#define Model_Type_CloudLength	2

#define Model_Type_Lebigrand_Collision_Radius 

#define Total_Texture_Count 5

#define KEY_A 0x41
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_P 0x50
#define KEY_S 0x53
#define KEY_V 0x56
#define KEY_W 0x57

const int ScaleByModelType[Total_Model_Count] = { 1,1,1,5,5 };

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT2 TexturePos;
};

struct ConstantBuffer
{
	XMMATRIX WorldPos;
	XMMATRIX WorldRot;
	XMMATRIX View;
	XMMATRIX Projection;
};

struct ModelBuffer
{
	XMMATRIX Scale;
	XMMATRIX Position;
	XMMATRIX Rotation;
};

struct ModelData
{
	bool Initialized = false;
	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;
	int VertexCount;
	int	IndexCount;

	bool BoundingBoxLoaded = false;
	float BoundingBox[6] = { 0,0, 0,0 ,0,0 };

	bool HitBoxLoaded = false;
	vector<XMFLOAT3> HitBox;
	vector<WORD> HitBoxIndex;
};

struct PositionAndType
{
	XMFLOAT3 Position;
	int Type;
};

struct TargaHeader
{
	unsigned char data1[12];
	unsigned short width;
	unsigned short height;
	unsigned char bpp;
	unsigned char data2;
};

float inline Balance(float x)
{

	while (x >= XM_2PI)
	{
		x -= XM_2PI;
	}

	while (x < 0)
	{
		x += XM_2PI;
	}

	return x;
}

float inline BalanceAndLock(float x)
{
	if (x > XM_PI)
		x = XM_PI;

	if (x < -XM_PI)
		x = -XM_PI;

	return x;
}

void inline TurnRadiantsIntoXandZComponent(float Rotation, float *x, float *z)
{
	if (Rotation <= XM_PIDIV2)
	{
		*z = (Rotation / -XM_PIDIV2) + 1;
		*x = 1 - *z;
	}
	else if (Rotation <= XM_PI)
	{
		*z = (Rotation / -XM_PIDIV2) + 1;
		*x = (Rotation / -XM_PIDIV2) + 2;
	}
	else if (Rotation <= XM_PIDIV2 * 3)
	{
		*z = (Rotation / XM_PIDIV2) - 3;
		*x = (Rotation / -XM_PIDIV2) + 2;
	}
	else
	{
		*z = (Rotation / XM_PIDIV2) - 3;
		*x = (Rotation / XM_PIDIV2) - 4;
	}
}

void inline TurnRadiantsIntoXandZComponentSin(float Rotation, float *x, float *z)
{
	*z = cos(Rotation);
	*x = sin(Rotation);
}
#endif