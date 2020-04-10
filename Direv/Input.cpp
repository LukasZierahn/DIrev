#include "Input.h"
#include "Model.h"
#include "Camera.h"
#include "RenderClass.h"
#include "Include.h"
#include "Ship.h"

Input::Input(RenderClass* RndCls, RECT rect)
{
	Render = RndCls;
	WndRect = rect;
}

void Input::Key(bool down, WPARAM mes)
{
	switch (mes)
	{
	case(KEY_W):
		keyW = down;
		break;

	case(KEY_A):
		keyA = down;
		break;

	case(KEY_S):
		keyS = down;
		break;

	case(KEY_D):
		keyD = down;
		break;

	case(KEY_C):
		keyC = down;
		break;

	case(KEY_V):
		keyV = down;
		break;

	case(KEY_E):
		EnterShipInfront();
		break;

	case(KEY_P):
		if (down)
		{
			D3D11_RASTERIZER_DESC RasDesc;
			Render->GetRasDesc(&RasDesc);

			if (RasDesc.FillMode == D3D11_FILL_SOLID)
				Render->SetFillMode(D3D11_FILL_WIREFRAME);
			else
				Render->SetFillMode(D3D11_FILL_SOLID);
		}
		break;
	}
}

void Input::Mouse(WPARAM wParam, int x, int y)
{
	int dx = x - (WndRect.right / 2) + WndRect.left;
	int dy = y - (WndRect.bottom / 2) + WndRect.top;

	Render->cam->AddRotation(dy / MouseSense, dx / MouseSense, 0.0f);

	SetCursorPos((WndRect.right / 2) + WndRect.left, (WndRect.bottom / 2) + WndRect.top);
}

void Input::EnterShipInfront()
{
	float camX, camY, dx, dy, Pitch, delta;

	camX = XMVectorGetX(*Render->cam->GetPosition());
	camY = XMVectorGetY(*Render->cam->GetPosition());

	Pitch = Render->cam->GetPitch();

	for (int i = 0; i < Render->ModelCount(); i++)
	{
		dx = XMVectorGetX(*Render->GetModel(i)->GetPosition()) - camX;
		dy = XMVectorGetY(*Render->GetModel(i)->GetPosition()) - camY;

		delta = fabsf(dx / dy);
	}
}

void Input::RunTick(int t)
{
	x = 0;
	y = 0;
	z = 0; 

	//determin how much we go in what direction regarding the camera rotation
	float Pitch = Render->cam->GetPitch();

	TurnRadiantsIntoXandZComponent(Pitch, &Rx, &Rz);

	//adding movement from the keys together
	if (keyW)
	{
		z += speed * t * Rz;
		x += speed * t * Rx;
	}

	if (keyA)
	{
		z += speed * t * Rx;
		x -= speed * t * Rz;
	}

	if (keyS)
	{
		z -= speed * t * Rz;
		x -= speed * t * Rx;
	}

	if (keyD)
	{
		z -= speed * t * Rx;
		x += speed * t * Rz;
	}

	if (keyC)
	{
		y += speed * t;
	}

	if (keyV)
	{
		y -= speed * t;
	}

	//if shift is pressed, we want to go faster
	if (HIWORD(GetKeyState(VK_SHIFT)))
	{
		x *= 2;
		y *= 2;
		z *= 2;
	}

	//keeping the Character moves at a constant speed
	//the character will slow down however if, for example, A, D and W are pressed at the same time but i honestly dont care, that would be their own fault
	if ((keyW || keyS) && (keyA || keyD))
		Render->cam->AddPosition(x / 2, y, z / 2);
	else
		Render->cam->AddPosition(x, y, z);
}

void Input::ShipTick(int)
{
}

void Input::SetShip(Model* Model, Ship* newShip)
{
	mod = Model;
	CurrentShip = newShip;
}
