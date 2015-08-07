#include <windows.h>
#include <iostream>
#include <cstdlib>

using namespace std;

LRESULT CALLBACK MouseHookLL (int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeybdHookLL (int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InitThread ();

int main()
{
	system("title CSGO Helper 1.0");
	system("cls");
	HMODULE hDll = LoadLibraryW(L"helper.dll");
	if ((InitThread() == 0) ||
		(SetWindowsHookEx(WH_KEYBOARD_LL, KeybdHookLL, hDll, 0) == 0) || 
		(SetWindowsHookEx(WH_MOUSE_LL, MouseHookLL, hDll, 0) == 0)) {
			cout << "An error occurred during initialization!\n";
			system("pause");
			return 1;
	}
	cout << "CSGO Helper 1.0 by Jeffrey\n\n";
	cout << "Press F9 to toggle on/off\n";
	cout << "Press F12 to exit\n";
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}