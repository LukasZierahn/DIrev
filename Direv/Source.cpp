#include "Include.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"
#include "AllShips.h"
#include "RenderClass.h"
#include "Player.h"
#include "Sky.h"


//global variables
LPTSTR WndClassName = "Direv";
HWND hwnd;

RenderClass* Render;

long long milliseconds_now() {
	LARGE_INTEGER frequency;
	BOOL use_qpc = QueryPerformanceFrequency(&frequency);

	if (use_qpc) 
	{
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / frequency.QuadPart;
	}
	else 
	{
		return GetTickCount();
	}
}

LRESULT CALLBACK WndProc(HWND inpHwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	hwnd = inpHwnd;
	//handeling Windows messages
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		Render->inp->Key(true, wParam);
		break;

	case WM_KEYUP:
		Render->inp->Key(false, wParam);
		break;

	case WM_MOUSEMOVE:
		Render->inp->Mouse(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	//case WM_SIZE:
	//	Render->Resize(LOWORD(lParam) , HIWORD(lParam));
	//	break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	RECT Desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &Desktop);

	//Creating a WndClass with all the properties of the later window
	WNDCLASSEX WndClass;
	ZeroMemory(&WndClass, sizeof(WNDCLASSEX));

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.hInstance = hInstance;
	WndClass.lpszClassName = WndClassName;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//setting a window style, it redraws if moved
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hIcon = NULL;
	WndClass.hCursor = NULL;

	RegisterClassEx(&WndClass);

	RECT WindowRect = Desktop;
	AdjustWindowRectEx(&WindowRect, WS_POPUP, false, NULL);

	//Creating the Window
	HWND hwnd = CreateWindowEx(NULL, WndClassName, "Direv", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE || SWP_SHOWWINDOW);

	ShowCursor(false);

	Render = new RenderClass(hwnd);

	Lebigrand* leb;
	Nosu* nos;
	Cooge* cog;

	void* B = nullptr;

	Render->AddModel(Model_Type_Lebigrand, &B);
	leb = static_cast<Lebigrand*>(B);

	Render->AddModel(Model_Type_Nosu, &B);
	nos = static_cast<Nosu*>(B);

	Render->AddModel(Model_Type_Cooge, &B);
	cog = static_cast<Cooge*>(B);

	cog->mod->SetPosition(0.0f, 3.0f, 5.0f);
	nos->mod->SetPosition(0.0f, 0.0f, 5.0f);
	leb->mod->SetPosition(0.0f, 3.0f, 0.0f);

	//Initalising the Message loop
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));

	long long t = milliseconds_now();

	while (true)
	{
		long long NewTime = milliseconds_now();
		int DeltaTime = NewTime - t;

		if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			if (Msg.message == WM_QUIT || (Msg.message == WM_KEYDOWN && Msg.wParam == VK_ESCAPE))
				break;
		}

		Render->player->tick(DeltaTime);
		Render->SkyClouds->MoveSky(DeltaTime);
		Render->RenderFrame();

		t = NewTime;
	}

	delete Render;

	return 0;
}