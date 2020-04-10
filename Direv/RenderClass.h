#pragma once
#include "Include.h"

#ifndef __H__RenderClass
#define __H__RenderClass

class Model;
class Camera;
class Input;
class ModelLoader;
class Player;
class Sky;

class RenderClass
{
public:
	RenderClass(HWND);

	void AddModel(int, void**);
	void RenderFrame();

	void ChangeFullscreenState();

	Model* GetModel(int index)	{ return ModelVector[index]; }
	void AddModel(Model* mod)	{ ModelVector.push_back(mod); 	ModelVector.shrink_to_fit();}

	void GetDevice(ComPtr<ID3D11Device>* dev)					{ Device.As(dev); }
	void GetDeviceContext(ComPtr<ID3D11DeviceContext>* devcon)	{ DeviceContext.As(devcon); }
	void GetRasDesc(D3D11_RASTERIZER_DESC* RasDesc)				{ RasState->GetDesc(RasDesc); }
	void GetSamplerState(ComPtr<ID3D11SamplerState>* Sample)	{ SamplerState.As(Sample); }

	Input* GetInput()			{ return inp; }
	int ModelCount()			{ return ModelVector.size(); }
	BOOL GetFullscreenState()	{ return Fullscreen; }

	void SetFillMode(D3D11_FILL_MODE);

	bool CollisionMovement(XMVECTOR*, XMVECTOR*, XMVECTOR** = nullptr); //Position, wanted Movement, resulting Position

	~RenderClass();

	Sky* SkyClouds;
	Camera* cam;
	Input* inp;
	ModelLoader* ModLoad;
	Player* player;

private:
	HWND hwnd;

	vector<Model*> ModelVector;
	
	BOOL Fullscreen = false;

	RECT size;
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;
	ComPtr<IDXGISwapChain> SwapChain;
	ComPtr<ID3D11RenderTargetView> Backbuffer;
	ComPtr<ID3D11DepthStencilState> DepthStencilState;
	ComPtr<ID3D11DepthStencilView> DepthStencilView;

	ComPtr<ID3D11InputLayout> InputLayout;

	ComPtr<ID3D11VertexShader> VertexShader;
	ComPtr<ID3D11PixelShader> PixelShader;

	ComPtr<ID3D11SamplerState> SamplerState;

	ComPtr<ID3D11RasterizerState> RasState;

};

#endif