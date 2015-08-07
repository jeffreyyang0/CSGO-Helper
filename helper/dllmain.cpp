#include <windows.h>

LRESULT CALLBACK MouseHookLL (int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeybdHookLL (int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InitThread ();
DWORD WINAPI BotThread(LPVOID param);

// TODO use something more thread-safe
bool isOn = false;
HANDLE bot;
bool quitLoop = false;

BOOL APIENTRY DllMain (HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

LRESULT CALLBACK MouseHookLL (int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || (!isOn)) {
		return CallNextHookEx(0, nCode, wParam, lParam);
	} else {
		// check origin of click
		PMSLLHOOKSTRUCT hs = (PMSLLHOOKSTRUCT) lParam;
		if (hs->dwExtraInfo == 0x1337) // simulated, let pass
			return CallNextHookEx(0, nCode, wParam, lParam);

		// start/stop macro
		if (wParam == WM_LBUTTONDOWN) { // start
			quitLoop = false;
			ResumeThread(bot);
			return 1;
		} else if (wParam == WM_LBUTTONUP) { // stop
			quitLoop = true;
			Sleep(10);
			SuspendThread(bot);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0x1337);
			//keybd_event(0x41, MapVirtualKey(0x41, 0), KEYEVENTF_KEYUP, 0);
			//keybd_event(0x44, MapVirtualKey(0x44, 0), KEYEVENTF_KEYUP, 0);
			keybd_event(0x57, MapVirtualKey(0x57, 0), KEYEVENTF_KEYUP, 0);
			keybd_event(0x53, MapVirtualKey(0x53, 0), KEYEVENTF_KEYUP, 0);
			return 1;
		} else {
			return CallNextHookEx(0, nCode, wParam, lParam);
		}
	}
}

// For toggling bot on/off
LRESULT CALLBACK KeybdHookLL (int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(0, nCode, wParam, lParam);

	if (wParam == WM_KEYDOWN) {
		PKBDLLHOOKSTRUCT hs = (PKBDLLHOOKSTRUCT) lParam;
		if (hs->vkCode == VK_F9) {
			isOn = !isOn;
		} else if (hs->vkCode == VK_F12) {
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

DWORD WINAPI BotThread(LPVOID param)
{
	while (true) {
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0x1337);
		// Sleep(100); // ensure that this finishes; use alternate method (might not be nec)
		for (int i=0; i < 10; i++) {
			//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0x1337);
			Sleep(10);
			//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0x1337);
			if (quitLoop)
				break;
		}
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0x1337);

		/*
		keybd_event(0x41, MapVirtualKey(0x41, 0), 0, 0); // A key
		Sleep(40);
		keybd_event(0x41, MapVirtualKey(0x41, 0), KEYEVENTF_KEYUP, 0);
		keybd_event(0x44, MapVirtualKey(0x44, 0), 0, 0); // D key
		Sleep(40);
		keybd_event(0x44, MapVirtualKey(0x44, 0), KEYEVENTF_KEYUP, 0);
		*/

		// Use W and S   seems to be better
		
		keybd_event(0x57, MapVirtualKey(0x57, 0), 0, 0); // W key
		Sleep(40);
		keybd_event(0x57, MapVirtualKey(0x57, 0), KEYEVENTF_KEYUP, 0);
		keybd_event(0x53, MapVirtualKey(0x53, 0), 0, 0); // S key
		Sleep(40);
		keybd_event(0x53, MapVirtualKey(0x53, 0), KEYEVENTF_KEYUP, 0);
		
	}
	return 0;
}

LRESULT CALLBACK InitThread ()
{
	bot = CreateThread(0, 0, BotThread, 0, CREATE_SUSPENDED, 0);
	//loopthread = CreateThread(0, 0, MsgLoop, 0, 0, 0);
	//kh = SetWindowsHookEx(WH_KEYBOARD_LL, KeybdHookLL, thisDLL, 0);
	//mh = SetWindowsHookEx(WH_MOUSE_LL, MouseHookLL, thisDLL, 0);
	return bot != 0;
}