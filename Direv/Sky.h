#include "Include.h"
#include "Model.h"

#ifndef __H__Sky
#define __H__Sky

class RenderClass;

class Sky
{
public:
	Sky(RenderClass*);

	void MoveSky(int);
	void AddPosition(float, float);
	void SetPosition(float, float);

	~Sky();

private:
	void UpdatePosition();

	const static int SkySize = 25;
	int SkySizeRoot = static_cast<int>(sqrt(SkySize));
	float height = -3;
	float SkySpeed = 0.00005f;

	RenderClass* Render;

	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;

	Model* SkyList[SkySize];

	XMVECTOR pos;
};

#endif
