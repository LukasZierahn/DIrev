#include "Include.h"

#ifndef __H__ModelLoader
#define __H__ModelLoader

class RenderClass;
class Texture;

class ModelLoader
{
public:
	ModelLoader(RenderClass*);
	void GetModel(int, ModelData**, bool = false);
	void GetTexture(int, Texture**);
	void LoadAllModels();

	~ModelLoader();

private:
	RenderClass* Render;

	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> DeviceContext;

	bool LoadModel(int, vector<Vertex>*, vector<WORD>*, bool);
	bool CreateModel(int);

	char ModelFiles[Total_Model_Count][256] = { {"Lebigrand"}, { "Cooge" }, {"Nosu"}, { "Cloud1" }, { "Cloud2" } };
	bool ModelLoadBBH[Total_Model_Count] = { true, false, false, false, false };

	ModelData Models[Total_Model_Count];

	Texture* TextureBuffer[Total_Texture_Count];

	Texture* ErrorText;
};

#endif
