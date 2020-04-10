#pragma once
#include "Include.h"

#ifndef __H__Camera
#define __H__Camera

class RenderClass;

class Camera
{
public:
	Camera(RenderClass*, RECT);
	void UpdateConstBuffer();

	void SetRotation(float, float, float);
	void AddRotation(float, float, float);
	void SetPosition(float, float, float);
	void AddPosition(float, float, float);

	XMVECTOR* GetPosition() { return &pos; }

	void inline UpdateViewMatrix();

	inline float GetRoll() { return Roll; }
	inline float GetPitch() { return Pitch; }
	inline float GetYawn() { return Yawn; }

	~Camera();

private:
	RenderClass* Render;

	D3D11_VIEWPORT Viewport;
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;
	ComPtr<ID3D11Buffer> ConstBuffer;

	RECT size;

	XMVECTOR pos;

	float Roll = 0.0f;
	float Pitch = 0.0f;
	float Yawn = 0.0f;

	float Rx, Rz;

	XMMATRIX View;
	XMMATRIX Projection;

	XMVECTOR EyePos;
	XMVECTOR FocusPos;
	XMVECTOR UpDir;
};

#endif