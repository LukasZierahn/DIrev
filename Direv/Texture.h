#include "Include.h"

#ifndef __H__Texture
#define __H__Texture

class RenderClass;

class Texture
{
public:
	Texture(RenderClass*, char[256]);
	bool hasSucceded() { return succeded; }
	void SetTextureAsShaderRessource();
	~Texture();

private:
	bool OpenTexture();

	RenderClass* Render;

	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;

	ComPtr<ID3D11Texture2D> TextureData;
	ComPtr<ID3D11ShaderResourceView> ShaderRessourceView;

	string fullFilename;

	int ImageSize;
	unsigned char* Data;

	TargaHeader* Header;

	bool succeded = false;
};

#endif