#include <Windows.h>
#pragma comment(lib,"winmm.lib")
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	MessageBox(NULL, "hello", L"TestBox", 0);
	return 0;
}