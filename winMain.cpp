#include <Windows.h>
#define  WINDOW_WIDTH 800
#define  WINDOW_HEIGHT 600
#define  WINDOW_TITLE L"DXdev"

//func declare
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//#pragma comment(lib,"winmm.lib")	//PlaySound()
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//PlaySound(L"game.wav", NULL, SND_FILENAME | SND_ASYNC);
	//MessageBox(NULL, L"hello", L"TestBox", 0);	//L means LPCWSTR
	
	//1.design wndClass
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);	
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//what style?
	wndClass.lpfnWndProc = WndProc;	
	wndClass.cbClsExtra = 0;	//ig
	wndClass.cbWndExtra = 0;	//ig
	wndClass.hInstance = hInstance;	
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"gir.ico",IMAGE_ICON,
		0, 0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE);	//what icon?
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);	//ig
	wndClass.lpszMenuName = NULL;	//ig
	wndClass.lpszClassName = L"Game DX DEV";

	//2.register wndClass
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	//3.create wnd
	HWND hwnd = CreateWindow(L"GAME DX DEV", WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//4.show wnd
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	//5.message cycle
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//6.unReg out wnd
	UnregisterClass(L"GAME DX DEV", wndClass.hInstance);

	return 0;
}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		ValidateRect(hwnd, NULL);		//??
		break;
	case WM_KEYDOWN:
		if (wParam = VK_ESCAPE)
			DestroyWindow(hwnd);	//send WM_DESTROY
		break;
	case WM_DESTROY:
		PostQuitMessage(0);		//important(end thread)
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

