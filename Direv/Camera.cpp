#include "Camera.h"
#include "Include.h"
#include "RenderClass.h"

Camera::Camera(RenderClass* rend, RECT si)
{
	Render = rend;

	Render->GetDevice(&Device);
	Render->GetDeviceContext(&DeviceContext);
	size = si;

	ZeroMemory(&Viewport, sizeof(D3D11_VIEWPORT));
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	Viewport.Width = size.right;
	Viewport.Height = size.bottom;
	Viewport.MinDepth = 0.0;
	Viewport.MaxDepth = 1.0;

	DeviceContext->RSSetViewports(1, &Viewport);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Device->CreateBuffer(&bd, nullptr, ConstBuffer.GetAddressOf());

	//initialize the matrixes
	EyePos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	FocusPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	UpDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	View = XMMatrixLookAtLH(EyePos, FocusPos, UpDir);

	Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, size.right / (FLOAT)size.bottom, 0.001f, 200.0f);

	DeviceContext->VSSetConstantBuffers(0, 1, ConstBuffer.GetAddressOf());

	SetPosition(0.0f, 5.0f, 0.0f);
}

void Camera::UpdateConstBuffer()
{
	ConstantBuffer cb;

	cb.WorldRot = XMMatrixIdentity();
	cb.WorldPos = XMMatrixIdentity();
	cb.View = XMMatrixTranspose(View);
	cb.Projection = XMMatrixTranspose(Projection);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	DeviceContext->Map(ConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	memcpy(MappedResource.pData, &cb, sizeof(cb));

	DeviceContext->Unmap(ConstBuffer.Get(), 0);
}

void Camera::SetRotation(float R, float P, float Y)
{
	Roll = BalanceAndLock(R);
	Pitch = Balance(P);
	Yawn = Balance(Y);

	UpdateViewMatrix();
}

void Camera::AddRotation(float R, float P, float Y)
{
	Roll	= BalanceAndLock(Roll + R);
	Pitch	= Balance(Pitch + P);
	Yawn	= Balance(Yawn + Y);

	UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
	pos = XMVectorSet(x, y, z, 1);

	UpdateViewMatrix();
}

void Camera::AddPosition(float x, float y, float z)
{
	XMVECTOR BufferVector = XMVectorSet(x, y, z, 1);
	pos += BufferVector;

	UpdateViewMatrix();
}

void inline Camera::UpdateViewMatrix()
{
	TurnRadiantsIntoXandZComponentSin(Pitch, &Rx, &Rz);

	EyePos = pos;

	XMVECTOR BufferVector = XMVectorSet(Rx, Roll / -XM_PIDIV2, Rz, 1);
	FocusPos = pos + BufferVector;

	View = XMMatrixLookAtLH(EyePos, FocusPos, UpDir);
}

Camera::~Camera()
{
}
