#include "Include.h"
#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "AllShips.h"
#include "RenderClass.h"
#include "ModelLoader.h"
#include "Player.h"
#include "Sky.h"

RenderClass::RenderClass(HWND wndhandle)
{
	hwnd = wndhandle;
	GetClientRect(hwnd, &size);
	HRESULT hr;

	DXGI_RATIONAL RefreshRateRat; //A rational (Numerator/Denominator) describing the framerate and it will get bound into the displaymode and eventually into the SwapChain
	RefreshRateRat.Numerator = 120;
	RefreshRateRat.Denominator = 1;

	DXGI_MODE_DESC ModeDesc; //A displaymode in order to be bound into the SwapChain
	ZeroMemory(&ModeDesc, sizeof(ModeDesc));
	ModeDesc.Width = size.right;
	ModeDesc.Height = size.bottom;
	ModeDesc.RefreshRate = RefreshRateRat;
	ModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //The way our pixel are definded
	ModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE; //If we think about the screen as an array of pixels, every horizontal row of pixels is a scanline in the big array
	ModeDesc.Scaling = DXGI_MODE_SCALING_CENTERED; //We dont scale

	DXGI_SAMPLE_DESC dMultiSampleing; //Determinting the MultiSampleing
	dMultiSampleing.Count = 1;
	dMultiSampleing.Quality = 0;

	DXGI_SWAP_CHAIN_DESC SwapChainDesc; //Determinting the properties of the SwapChain that will be created with the Device, it is not the SwapChain itself
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferDesc = ModeDesc;
	SwapChainDesc.SampleDesc = dMultiSampleing;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = hwnd;
	SwapChainDesc.Windowed = true;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL DirectXFeatureLvl[] = { D3D_FEATURE_LEVEL_11_0 };

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, DirectXFeatureLvl, ARRAYSIZE(DirectXFeatureLvl), D3D11_SDK_VERSION, &SwapChainDesc, SwapChain.GetAddressOf(), Device.GetAddressOf(), NULL, DeviceContext.GetAddressOf());

	//create a render target view
	ID3D11Texture2D* BackBufferRessource;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBufferRessource);

	Device->CreateRenderTargetView(BackBufferRessource, NULL, Backbuffer.GetAddressOf());

	D3D11_TEXTURE2D_DESC BackBufferDesc;
	BackBufferRessource->GetDesc(&BackBufferDesc);

	BackBufferRessource->Release();

	ID3D11Texture2D* DepthStencilTexture;
	D3D11_TEXTURE2D_DESC DepthDesc;
	DepthDesc.Width = BackBufferDesc.Width;
	DepthDesc.Height = BackBufferDesc.Height;
	DepthDesc.MipLevels = 1;
	DepthDesc.ArraySize = 1;
	DepthDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	DepthDesc.SampleDesc = dMultiSampleing;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthDesc.CPUAccessFlags = 0;
	DepthDesc.MiscFlags = 0;
	Device->CreateTexture2D(&DepthDesc, NULL, &DepthStencilTexture);

	D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
	// Depth test parameters
	DepthStencilDesc.DepthEnable = true;
	DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	DepthStencilDesc.StencilEnable = true;
	DepthStencilDesc.StencilReadMask = 0xFF;
	DepthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	hr = Device->CreateDepthStencilState(&DepthStencilDesc, DepthStencilState.GetAddressOf());

	DeviceContext->OMSetDepthStencilState(DepthStencilState.Get(), 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = Device->CreateDepthStencilView(DepthStencilTexture, &DepthStencilViewDesc, DepthStencilView.GetAddressOf());

	DeviceContext->OMSetRenderTargets(1, Backbuffer.GetAddressOf(), DepthStencilView.Get());

	//compiling and setting the shaders
	ID3D10Blob *ErrorBlob;
	ID3D10Blob *VertexShaderBlob, *PixelShaderBlob;

	D3DCompileFromFile(L"shader.HLSL", NULL, NULL, "VS", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL1 || D3DCOMPILE_WARNINGS_ARE_ERRORS, NULL, &VertexShaderBlob, &ErrorBlob);

	D3DCompileFromFile(L"shader.HLSL", NULL, NULL, "PS", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL1 || D3DCOMPILE_WARNINGS_ARE_ERRORS, NULL, &PixelShaderBlob, &ErrorBlob);

	Device->CreateVertexShader(VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), NULL, VertexShader.GetAddressOf());
	Device->CreatePixelShader(PixelShaderBlob->GetBufferPointer(), PixelShaderBlob->GetBufferSize(), NULL, PixelShader.GetAddressOf());

	//defining an input layout
	D3D11_INPUT_ELEMENT_DESC InputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXPOS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	Device->CreateInputLayout(InputElementDesc, ARRAYSIZE(InputElementDesc), VertexShaderBlob->GetBufferPointer(), VertexShaderBlob->GetBufferSize(), InputLayout.GetAddressOf());
	DeviceContext->IASetInputLayout(InputLayout.Get());

	VertexShaderBlob->Release();
	PixelShaderBlob->Release();

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Device->CreateSamplerState(&samplerDesc, SamplerState.GetAddressOf());
	DeviceContext->PSSetSamplers(0, 1, SamplerState.GetAddressOf());

	SetFillMode(D3D11_FILL_SOLID);

	UpdateWindow(hwnd);

	RECT rect;
	GetWindowRect(hwnd, &rect);

	ModLoad = new ModelLoader(this);
	//ModLoad->LoadAllModels();
	cam = new Camera(this, size);
	inp = new Input(this, rect);
	player = new Player(this);
	SkyClouds = new Sky(this);
}

void RenderClass::AddModel(int ModTyp, void** parent)
{
	void* newModel = nullptr;

	switch (ModTyp)
	{
	case(Model_Type_Lebigrand):
		newModel = new Lebigrand(this);
		ModelVector.push_back(static_cast<Lebigrand*>(newModel)->mod);
		*parent = newModel;
		break;	
	
	case(Model_Type_Cooge):
		newModel = new Cooge(this);
		ModelVector.push_back(static_cast<Cooge*>(newModel)->mod);
		*parent = newModel;
		break;	
	case(Model_Type_Nosu):
		newModel = new Nosu(this);
		ModelVector.push_back(static_cast<Nosu*>(newModel)->mod);
		*parent = newModel;
		break;
	}

	ModelVector.shrink_to_fit();
}

void RenderClass::RenderFrame()
{
	float Colour[4] = { 0.5f, 0.1f, 0.7f, 1.0f };
	DeviceContext->ClearRenderTargetView(Backbuffer.Get(), Colour);

	DeviceContext->ClearDepthStencilView(DepthStencilView.Get(), D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

	cam->UpdateConstBuffer();

	DeviceContext->VSSetShader(VertexShader.Get(), nullptr, 0);
	DeviceContext->PSSetShader(PixelShader.Get(), nullptr, 0);

	for (unsigned short i = 0; i < ModelVector.size(); i++)
	{
		ModelVector[i]->Draw();
	}

	SwapChain->Present(0, 0);
}

void RenderClass::ChangeFullscreenState() //THIS STUFF IS NOT WORKING!!
{
	GetClientRect(hwnd, &size);
	Fullscreen = !Fullscreen;

	if (Fullscreen)
	{
		SwapChain->ResizeBuffers(1, size.right, size.bottom, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_FULLSCREEN_VIDEO || DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	}
	else
	{
		SwapChain->ResizeBuffers(1, size.right, size.bottom, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	}

	SwapChain->SetFullscreenState(Fullscreen, NULL);
}

void RenderClass::SetFillMode(D3D11_FILL_MODE fillmode)
{
	D3D11_RASTERIZER_DESC RasDesc;
	ZeroMemory(&RasDesc, sizeof(D3D11_RASTERIZER_DESC));
	RasDesc.FillMode = fillmode;
	RasDesc.CullMode = D3D11_CULL_BACK;
	RasDesc.DepthClipEnable = true;
	RasDesc.ScissorEnable = false;
	RasDesc.FrontCounterClockwise = true;

	Device->CreateRasterizerState(&RasDesc, RasState.GetAddressOf());

	DeviceContext->RSSetState(RasState.Get());
}

bool RenderClass::CollisionMovement(XMVECTOR* pos, XMVECTOR* move, XMVECTOR** outputPos)
{
	float dx, dy, dz;
	int ModTyp;

	XMVECTOR FirstHit = XMVectorSet(0, 0, 0, 1);
	XMVECTOR* Buffer;

	for (unsigned int i = 0; i < ModelVector.size(); i++)
	{
		ModTyp = ModelVector[i]->GetModelType();

		dx = XMVectorGetX(*(ModelVector[i]->GetPosition())) - XMVectorGetX(*pos);
		dy = XMVectorGetY(*(ModelVector[i]->GetPosition())) - XMVectorGetY(*pos);
		dz = XMVectorGetZ(*(ModelVector[i]->GetPosition())) - XMVectorGetZ(*pos);

		if (((dx < ModelVector[i]->GetBounding(0) || dx > ModelVector[i]->GetBounding(1)) &&
			(dy < ModelVector[i]->GetBounding(2) || dy > ModelVector[i]->GetBounding(3)) &&
			(dz < ModelVector[i]->GetBounding(4) || dz > ModelVector[i]->GetBounding(5))) ||
			((dx + XMVectorGetX(*pos) < ModelVector[i]->GetBounding(0) || dx + XMVectorGetX(*pos) > ModelVector[i]->GetBounding(1)) && 
			(dy + XMVectorGetX(*pos) < ModelVector[i]->GetBounding(2) || dy + XMVectorGetX(*pos) > ModelVector[i]->GetBounding(3)) &&
			(dz + XMVectorGetX(*pos) < ModelVector[i]->GetBounding(4) || dz + XMVectorGetX(*pos) > ModelVector[i]->GetBounding(5))))

		{

			ModelVector[i]->CheckForCollision(pos, move, &Buffer);
			if (abs(XMVectorGetX(*Buffer) - XMVectorGetX(*pos)) < abs(XMVectorGetX(FirstHit) - XMVectorGetX(*pos)))
			{
				FirstHit = *Buffer;
			}
		}
	}
	return false;
}

RenderClass::~RenderClass()
{
	SwapChain->SetFullscreenState(false, NULL);

	delete inp;
	delete cam;
	delete ModLoad;
	delete player;

	for (unsigned short i = 0; i < ModelVector.size(); i++)
	{
		delete ModelVector[i];
	}
}