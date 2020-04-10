#include "ModelLoader.h"
#include "RenderClass.h"
#include "Texture.h"
#include "Include.h"

ModelLoader::ModelLoader(RenderClass* rend)
{
	Render = rend;

	Render->GetDevice(&Device);
	Render->GetDeviceContext(&DeviceContext);

	ErrorText = new Texture(Render, "Error");
}

bool ModelLoader::LoadModel(int ModTyp, vector<Vertex> *outputVertex, vector<WORD> *outputIndex, bool forceRecreation)
{
	ifstream ModelStream;
	char input;

	string fullFilename = "Data\\";
	fullFilename += ModelFiles[ModTyp];
	fullFilename += ".cng";

	if (forceRecreation)
		CreateModel(ModTyp);

	//open the document
	ModelStream.open(fullFilename.c_str(), ifstream::in);

	if (ModelStream.fail())
	{
		ModelStream.close();
		if (CreateModel(ModTyp))
		{
			ModelStream.open(fullFilename.c_str(), ifstream::in);
			if (ModelStream.fail())
			{
				ModelStream.close();
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	//Load the Indexes and Vertexes
	Vertex	NewVertex;

	WORD NewIndex;

	short i;

	while (!ModelStream.eof())
	{
		//get the first char of the line
		ModelStream.get(input);

		//Vertecies
		if (input == 'v')
		{
			ModelStream.get(input);
			if (input == ' ')
			{
				ModelStream >> NewVertex.Position.x >> NewVertex.Position.y >> NewVertex.Position.z >> NewVertex.TexturePos.x >> NewVertex.TexturePos.y;
				outputVertex->push_back(NewVertex);

				ModelStream.ignore(1000, '\n');
			}
		}
		//Indecies
		else if (input == 'f')
		{
			ModelStream.get(input);
			if (input == ' ')
			{
				for (i = 0; i < 3; i++)
				{
					ModelStream >> NewIndex;
					outputIndex->push_back(NewIndex);

					ModelStream.get(input);
					while (input != ' ' && input != '\n')
					{
						ModelStream.get(input);
					}
				}
			}
		}
		//skip to the next line
		else if (input != '\n')
		{
			ModelStream.ignore(1000, '\n');

		}
	}

	Models[ModTyp].VertexCount = outputVertex->size();
	Models[ModTyp].IndexCount = outputIndex->size();

	outputVertex->shrink_to_fit();
	outputIndex->shrink_to_fit();

	//Loading in Hit and BoundingBox
	if (ModelLoadBBH[ModTyp])
	{
		ifstream bbhInput;

		fullFilename = "Data\\";
		fullFilename += ModelFiles[ModTyp];
		fullFilename += ".hbb";

		//open the document
		bbhInput.open(fullFilename.c_str(), ifstream::in);
		if (bbhInput.fail())
		{
			bbhInput.close();
			if (CreateModel(ModTyp))
			{
				bbhInput.open(fullFilename.c_str(), ifstream::in);
				if (bbhInput.fail())
				{
					bbhInput.close();
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		XMFLOAT3 NewPosVex;

		while (!bbhInput.eof())
		{
			//get the first char of the line
			bbhInput.get(input);

			//Bounding Box
			if (input == 'b')
			{
				bbhInput.get(input);
				if (input == 'b')
				{
					for (unsigned int i = 0; i < 6; i++)
					{
						bbhInput >> Models[ModTyp].BoundingBox[i];
					}
					Models[ModTyp].BoundingBoxLoaded = true;
				}
			}
			//Vertecies
			else if (input == 'v')
			{
				bbhInput.get(input);
				if (input == ' ')
				{
					bbhInput >> NewPosVex.x >> NewPosVex.y >> NewPosVex.z;
					Models[ModTyp].HitBox.push_back(NewPosVex);

					bbhInput.ignore(1000, '\n');
				}
			}
			//Indecies
			else if (input == 'f')
			{
				bbhInput.get(input);
				if (input == ' ')
				{
					for (i = 0; i < 3; i++)
					{
						bbhInput >> NewIndex;
						Models[ModTyp].HitBoxIndex.push_back(NewIndex);

						bbhInput.get(input);
						while (input != ' ' && input != '\n')
						{
							bbhInput.get(input);
						}
					}
				}
			}
			//skip to the next line
			else if (input != '\n')
			{
				bbhInput.ignore(1000, '\n');

			}
		}
		Models[ModTyp].HitBoxLoaded = true;
	}

	return true;
}

bool ModelLoader::CreateModel(int ModTyp)
{
	struct InstallVertex
	{
		int NewIndex = 0;
		vector<WORD> TexIndexes;
	};

	ifstream InputStream;
	ofstream OutputStream;
	vector<Vertex>	InputVertexVector, OutputVertexVector;
	vector<WORD>	InputIndexVector, InputTexIndexVector;
	vector<InstallVertex>	InstallVertexVector;
	vector<float>	TexVertex1, TexVertex2;
	Vertex	NewVertex;
	WORD NewIndex = 0, NewTexIndex = 0;
	float TexVex1, TexVex2;
	short i;
	char input;

	string fullFilenameInput = "Source\\";
	fullFilenameInput += ModelFiles[ModTyp];
	fullFilenameInput += ".obj";

	InputStream.open(fullFilenameInput.c_str(), ifstream::in);
	if (InputStream.fail())
		return false;

	while (!InputStream.eof())
	{
		InputStream.get(input);

		//Vertecies
		if (input == 'v')
		{
			InputStream.get(input);
			if (input == ' ')
			{
				InputStream >> NewVertex.Position.x >> NewVertex.Position.y >> NewVertex.Position.z;
				NewVertex.Position.z = NewVertex.Position.z * -1.0f;
				NewVertex.TexturePos.x = -1;
				NewVertex.TexturePos.y = -1;
				InputVertexVector.push_back(NewVertex);

				InputStream.ignore(1000, '\n');
			}
			else if (input == 't')
			{
				InputStream.get(input);

				InputStream >> TexVex1 >> TexVex2;
				TexVertex1.push_back(TexVex1);
				TexVertex2.push_back(1 - TexVex2);

				InputStream.ignore(1000, '\n');
			}
		}
		else if (input == 'f') //Indecies
		{
			InputStream.get(input);
			if (input == ' ')
			{
				for (i = 0; i < 3; i++)
				{
					InputStream >> NewIndex >> input >> NewTexIndex;
					InputIndexVector.push_back(NewIndex - 1);
					InputTexIndexVector.push_back(NewTexIndex - 1);

					InputStream.get(input);
					while (input != ' ' && input != '\n')
					{
						InputStream.get(input);
					}
				}
			}
		}
		else if (input != '\n') //skip to the next line
		{
			InputStream.ignore(1000, '\n');

		}
	}

	InputStream.close();

	InputVertexVector.shrink_to_fit();
	InputIndexVector.shrink_to_fit();
	InputTexIndexVector.shrink_to_fit();
	TexVertex1.shrink_to_fit();
	TexVertex2.shrink_to_fit();

	string fullFilenameOutput = "Data\\";
	fullFilenameOutput += ModelFiles[ModTyp];
	fullFilenameOutput += ".cng";

	OutputStream.open(fullFilenameOutput.c_str());

	int Indexes = InputIndexVector.size();
	int LastNewIndex = 0;


	if (OutputStream.fail())
		return false;

	for (unsigned int i = 0; i < InputVertexVector.size(); i++)
	{
		InstallVertex NewIndex;
		NewIndex.NewIndex = LastNewIndex;
		InstallVertexVector.push_back(NewIndex);

		for (int f = 0; f < Indexes; f++)
		{
			if (InputIndexVector[f] == i)
			{
				bool skip = false;

				for (unsigned int AP = 0; AP < InstallVertexVector[i].TexIndexes.size(); AP ++)
				{
					if (InputTexIndexVector[f] == InstallVertexVector[i].TexIndexes[AP])
					{
						skip = true;
					}
				}

				if (!skip)
				{
					OutputStream << "v " << InputVertexVector[i].Position.x << ' ' << InputVertexVector[i].Position.y << ' ' << InputVertexVector[i].Position.z
						<< ' ' << TexVertex1[InputTexIndexVector[f]] << ' ' << TexVertex2[InputTexIndexVector[f]] << endl;

					InstallVertexVector[i].TexIndexes.push_back(InputTexIndexVector[f]);
					LastNewIndex++;
				}
			}
		}
	}

	for (unsigned int f = 0; f < InputIndexVector.size(); f += 3)
	{
		OutputStream << 'f';
		for (int face = 0; face < 3; face++)
		{
			int IndexToLookAt = InputIndexVector[f + face];

			for (unsigned int i = 0; i < InstallVertexVector[IndexToLookAt].TexIndexes.size(); i++)
			{
				if (InstallVertexVector[IndexToLookAt].TexIndexes[i] == InputTexIndexVector[f + face])
				{
					int Print = InstallVertexVector[IndexToLookAt].NewIndex + i;
					OutputStream << ' ' << Print;
					continue;
				}
			}
		}
		OutputStream << endl;
	}

	OutputStream.close();

	bool rewriteHBB = false;

	//Loading in the Boundingbox
	fullFilenameInput = "Source\\";
	fullFilenameInput += ModelFiles[ModTyp];
	fullFilenameInput += "_BB.obj";

	ifstream BoundingInput;

	input = '#';
	float BoundingBox[6] = { 0,0, 0,0, 0,0 };
	float Buffer[3];

	BoundingInput.open(fullFilenameInput.c_str(), ifstream::in);
	if (!BoundingInput.fail())
	{
		while (!BoundingInput.eof())
		{
			BoundingInput.get(input);

			//Vertecies
			if (input == 'v')
			{
				BoundingInput.get(input);
				if (input == ' ')
				{
					BoundingInput >> Buffer[0] >> Buffer[1] >> Buffer[2];
					Buffer[2] = Buffer[2] * -1.0f;
					//getting the Positiv and Negativ X bounds in
					if (Buffer[0] > BoundingBox[0]) BoundingBox[0] = Buffer[0];
					if (Buffer[0] < BoundingBox[1]) BoundingBox[1] = Buffer[0];

					//getting the Positiv and Negativ Y bounds in
					if (Buffer[1] > BoundingBox[2]) BoundingBox[2] = Buffer[1];
					if (Buffer[1] < BoundingBox[3]) BoundingBox[3] = Buffer[1];

					//getting the Positiv and Negativ Z bounds in
					if (Buffer[2] > BoundingBox[4]) BoundingBox[4] = Buffer[2];
					if (Buffer[2] < BoundingBox[5]) BoundingBox[5] = Buffer[2];

					BoundingInput.ignore(1000, '\n');
				}
			}
			else if (input == 'f') break;
			else if (input != '\n') //skip to the next line
			{
				BoundingInput.ignore(1000, '\n');

			}
		}
		rewriteHBB = true;
	}
	BoundingInput.close();


	//Loading in the Hitbox
	fullFilenameInput = "Source\\";
	fullFilenameInput += ModelFiles[ModTyp];
	fullFilenameInput += "_Hitbox.obj";

	ifstream HitboxInput;

	vector<XMFLOAT3> HitboxVertexies;
	vector<DWORD> HitboxIndexies;

	XMFLOAT3 NewPosVex;

	input = '#';

	HitboxInput.open(fullFilenameInput.c_str(), ifstream::in);
	if (!HitboxInput.fail())
	{
		while (!HitboxInput.eof())
		{
			HitboxInput.get(input);

			//Vertecies
			if (input == 'v')
			{
				HitboxInput.get(input);
				if (input == ' ')
				{
					HitboxInput >> NewPosVex.x >> NewPosVex.y >> NewPosVex.z;
					NewPosVex.z = NewPosVex.z * -1.0f;
					HitboxVertexies.push_back(NewPosVex);

					HitboxInput.ignore(1000, '\n');
				}
			}
			//Indecies
			else if (input == 'f') 
			{
				HitboxInput.get(input);
				if (input == ' ')
				{
					for (i = 0; i < 3; i++)
					{
						HitboxInput >> NewIndex;
						HitboxIndexies.push_back(NewIndex - 1);

						HitboxInput.get(input);
						while (input != ' ' && input != '\n')
						{
							HitboxInput.get(input);
						}
					}
				}
			}
			//skip to the next line
			else if (input != '\n') 
			{
				HitboxInput.ignore(1000, '\n');

			}
		}
		rewriteHBB = true;
	}
	HitboxInput.close();

	if (rewriteHBB)
	{
		ofstream OutputHBBStream;

		fullFilenameOutput = "Data\\";
		fullFilenameOutput += ModelFiles[ModTyp];
		fullFilenameOutput += ".hbb";

		OutputHBBStream.open(fullFilenameOutput.c_str());

		if (OutputHBBStream.fail())
			return false;

		OutputHBBStream << "bb";
		for (unsigned int i = 0; i < 6; i++)
		{
			OutputHBBStream << ' ' << BoundingBox[i];
		}

		OutputHBBStream << endl;

		for (unsigned int i = 0; i < HitboxVertexies.size(); i++)
		{
			OutputHBBStream << "v " << HitboxVertexies[i].x << ' ' << HitboxVertexies[i].y << ' ' << HitboxVertexies[i].z << endl;
		}

		for (unsigned int f = 0; f < HitboxIndexies.size(); f += 3)
		{
			OutputHBBStream << "f " << HitboxIndexies[f] << ' ' << HitboxIndexies[f + 1] << ' ' << HitboxIndexies[f + 2] << endl;
		}

		OutputHBBStream.close();
	}
	return true;
}

void ModelLoader::GetModel(int ModTyp, ModelData** OutputData, bool Recreate)
{
	if (!Models[ModTyp].Initialized || Recreate)
	{
		vector<Vertex>	VertexVector;
		vector<WORD>	IndexVector;

		bool worked = LoadModel(ModTyp, &VertexVector, &IndexVector, Recreate);

		D3D11_SUBRESOURCE_DATA InitVertexData;
		ZeroMemory(&InitVertexData, sizeof(InitVertexData));
		InitVertexData.pSysMem = VertexVector.data();

		D3D11_SUBRESOURCE_DATA InitIndexData;
		ZeroMemory(&InitIndexData, sizeof(InitIndexData));
		InitIndexData.pSysMem = IndexVector.data();

		D3D11_BUFFER_DESC VertexBufferDesc;
		ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));
		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = sizeof(Vertex) * Models[ModTyp].VertexCount;
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;

		D3D11_BUFFER_DESC IndexBufferDesc;
		ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));
		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = sizeof(WORD) * Models[ModTyp].IndexCount;
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;

		Device->CreateBuffer(&VertexBufferDesc, &InitVertexData, Models[ModTyp].VertexBuffer.GetAddressOf());
		Device->CreateBuffer(&IndexBufferDesc, &InitIndexData, Models[ModTyp].IndexBuffer.GetAddressOf());
		Models[ModTyp].Initialized = true;

	}
	*OutputData = &Models[ModTyp];
}

void ModelLoader::GetTexture(int TextType, Texture** tex)
{
	if (TextureBuffer[TextType] == nullptr)
	{
		if (TextType < Total_Model_Count)
		{
			TextureBuffer[TextType] = new Texture(Render, ModelFiles[TextType]);
		}
		else
		{
			TextureBuffer[TextType] = new Texture(Render, ModelFiles[TextType]); //yes yes this is not done yet
		}

		if (!TextureBuffer[TextType]->hasSucceded())
		{
			delete TextureBuffer[TextType];
			TextureBuffer[TextType] = ErrorText;
		}
	}

	*tex = TextureBuffer[TextType];
}

void ModelLoader::LoadAllModels()
{
	//yes we will load everything and instantly throw it away but luckily the class itself holds copies of the ComPtrs to all the buffers
	ComPtr<ID3D11Buffer> VertexBuffer;
	ComPtr<ID3D11Buffer> IndexBuffer;

	int VertexCount, IndexCount;
}

ModelLoader::~ModelLoader()
{
	delete ErrorText;
}
