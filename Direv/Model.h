#include "Include.h"

#ifndef __H__Model
#define __H__Model

class RenderClass;
class Texture;

//defining a Model that will be inherited
class Model
{
public:
	Model(RenderClass*, int, bool = false);
	Model(RenderClass*, int, float, float, float, bool = false);
	~Model();

	void SetRotation(float, float, float);
	void AddRotation(float, float, float);
	void SetPosition(float, float, float);
	void SetPosition(XMVECTOR);
	void AddPosition(float, float, float, bool = false, bool = false);

	int GetModelType() { return ModelType; };
	float GetBounding(int);
	bool CheckForCollision(XMVECTOR*, XMVECTOR*, XMVECTOR** = nullptr);

	XMVECTOR* GetPosition() { return &Position; }

	void Draw();

private:
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;
	ComPtr<ID3D11SamplerState> SamplerState;

	RenderClass* Render;

	ComPtr<ID3D11Buffer> ModBuf;

	ModelData* Data;

	Texture* tex;

	ModelBuffer ModBufClass;
	XMVECTOR Position;
	XMMATRIX Scale;

	float Roll = 0.0f;
	float Pitch = 0.0f;
	float Yawn = 0.0f;

	int ModelType = 0;

	void* parent = nullptr;
};

#endif