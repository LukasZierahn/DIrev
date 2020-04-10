#include "Texture.h"
#include "RenderClass.h"

Texture::Texture(RenderClass* rend, char filename[256])
{
	Render = rend;

	Render->GetDevice(&Device);
	Render->GetDeviceContext(&DeviceContext);

	fullFilename = "Textures\\";
	fullFilename += filename;
	fullFilename += ".tga";

	Header = new TargaHeader();

	if (OpenTexture())
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		textureDesc.Height = Header->height;
		textureDesc.Width = Header->width;
		textureDesc.MipLevels = 0;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		Device->CreateTexture2D(&textureDesc, NULL, TextureData.GetAddressOf());

		int rowPitch = (Header->width * 4) * sizeof(unsigned char);

		DeviceContext->UpdateSubresource(TextureData.Get(), 0, NULL, Data, rowPitch, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC ShaderRessourceViewDesc;
		ShaderRessourceViewDesc.Format = textureDesc.Format;
		ShaderRessourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		ShaderRessourceViewDesc.Texture2D.MostDetailedMip = 0;
		ShaderRessourceViewDesc.Texture2D.MipLevels = -1;

		Device->CreateShaderResourceView(TextureData.Get(), &ShaderRessourceViewDesc, ShaderRessourceView.GetAddressOf());

		DeviceContext->GenerateMips(ShaderRessourceView.Get());

		succeded = true;
	}
}

bool Texture::OpenTexture()
{
	FILE* filePointer;

	if (fopen_s(&filePointer, fullFilename.c_str(), "rb") != 0)
		return false;

	fread(Header, sizeof(TargaHeader), 1, filePointer);

	if (Header->bpp != 32)
		return false;

	ImageSize = Header->width * Header->height * 4;
	unsigned char* Buffer;

	Buffer = new unsigned char[ImageSize];
	fread(Buffer, 1, ImageSize, filePointer);

	fclose(filePointer);

	Data = new unsigned char[ImageSize];

	int Index, j, k, i;
	Index = 0;
	k = ImageSize - (Header->width * 4);

	for (j = 0; j < Header->height; j++)
	{
		for (i = 0; i < Header->width; i++)
		{
			Data[Index + 0] = Buffer[k + 2];  // Red.
			Data[Index + 1] = Buffer[k + 1];  // Green.
			Data[Index + 2] = Buffer[k + 0];  // Blue
			Data[Index + 3] = Buffer[k + 3];  // Alpha

			k += 4;
			Index += 4;
		}

		k -= (Header->width * 8);
	}

	delete[] Buffer;
	return true;
}

void Texture::SetTextureAsShaderRessource()
{
	DeviceContext->PSSetShaderResources(0, 1, ShaderRessourceView.GetAddressOf());
}

Texture::~Texture()
{
	delete Header;
	delete[] Data;
}
