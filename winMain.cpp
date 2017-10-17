#include <Windows.h>
#include <time.h>
#define  WINDOW_WIDTH 800
#define  WINDOW_HEIGHT 600
#define  WINDOW_TITLE L"DXdev"

//global vars
HDC g_hdc = NULL;

HPEN g_hPen[7] = { 0 };
HBRUSH g_hBrush[7] = { 0 };
int g_iPenStyle[7] = { PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT,
PS_NULL,PS_INSIDEFRAME };
int g_iBrushStyle[6] = { HS_VERTICAL,HS_HORIZONTAL,HS_CROSS,HS_DIAGCROSS,
HS_FDIAGONAL,HS_BDIAGONAL };
HFONT g_hFont = NULL;

//func declare
bool Game_Init(HWND hwnd);
void Game_Paint(HWND hwnd);
bool Game_CleanUp(HWND hwnd);
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

	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"Init failed.", L"Message window", 0);
		return false;
	}

	//PlaySound();


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

bool Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);
	srand((unsigned)time(NULL));

	//randomly select pen & brush style
	for (int i = 0; i <= 6; i++)
	{
		g_hPen[i] = CreatePen(g_iPenStyle[i], 1,
			RGB(rand() % 256, rand() % 256, rand() % 256));
		if (i == 6)
			g_hBrush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		else
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i], RGB(rand() % 256, rand() % 256, rand() % 256));
	}

	Game_Paint(hwnd);
	ReleaseDC(hwnd, g_hdc);
	return true;
}

void Game_Paint(HWND hwnd)
{
	g_hFont = CreateFont(45, 0, 0, 0, 0, 0, 0, 0, GB2312_CHARSET, 0, 0, 0, 0, TEXT("sfs"));	//no use
	//choose font
	SelectObject(g_hdc, g_hFont);		// initial
	//set text background
	SetBkMode(g_hdc, TRANSPARENT);		
	//text out 
	wchar_t text1[] = L"Written by a doubi2";
	SetTextColor(g_hdc, RGB(255, 0, 0));
	TextOut(g_hdc, 20, 10, text1, wcslen(text1));
	//DrawText()

	//paint line
	int y = 0;
	for (int i = 0; i <= 6; i++)
	{
		y = (i + 1) * 70;	//don't forget

		SelectObject(g_hdc, g_hPen[i]);		//don't forget g_hPen[i]
		MoveToEx(g_hdc, 30, y, NULL);
		LineTo(g_hdc, 100, y);
	}

	//paint rectangle
	int x1 = 120;
	int x2 = 190;

	for (int i = 0; i <= 6; i++)
	{
		SelectObject(g_hdc, g_hBrush[i]);	//don't forget g_hBrush[i]
		Rectangle(g_hdc, x1, 70, x2, y);
		x1 += 90;
		x2 += 90;
	}
}

bool Game_CleanUp(HWND hwnd)
{
	for (int i = 0; i <= 6; i++)
	{
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}
	return true;
}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;	//paintStruct
	switch (message)
	{
	case WM_PAINT:
		g_hdc = BeginPaint(hwnd, &paintStruct);
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintStruct);
		ValidateRect(hwnd, NULL);		//??update clientArea ,show 
		break;
	case WM_KEYDOWN:
		if (wParam = VK_ESCAPE)
			DestroyWindow(hwnd);	//send WM_DESTROY
		break;
	case WM_DESTROY:
		Game_CleanUp(hwnd);
		PostQuitMessage(0);		//important(end thread)
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

