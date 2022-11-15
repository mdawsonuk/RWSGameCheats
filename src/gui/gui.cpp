#include <stdio.h>
#include <windows.h>
#include <string>
#include "../../include/gui/gui.h"
#include "../gui/guiControl.h"

using namespace std;

#define SWITCH_BHOP_BUTTON 1
#define SWITCH_CHAMS_BUTTON 2

namespace Gui 
{
    HWND bhopMsg, chamsMsg, titleMsg, instrMsg, btn1, btn2;
    HFONT defaultFont, titleFont;

    LPCWSTR cheatOn = L" cheat is turned on!";
    LPCWSTR cheatOff = L" cheat is turned off!";

    void CreateAndSetFonts() {
        defaultFont = CreateFont(18, 0, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        titleFont = CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        SendMessage(titleMsg, WM_SETFONT, (LPARAM)titleFont, TRUE);
        SendMessage(instrMsg, WM_SETFONT, (LPARAM)defaultFont, TRUE);
        SendMessage(bhopMsg, WM_SETFONT, (LPARAM)defaultFont, TRUE);
        SendMessage(chamsMsg, WM_SETFONT, (LPARAM)defaultFont, TRUE);
        SendMessage(btn1, WM_SETFONT, (LPARAM)defaultFont, TRUE);
        SendMessage(btn2, WM_SETFONT, (LPARAM)defaultFont, TRUE);
    }

    void SwitchCheats(bool &cheatFlag, LPCWSTR cheat, HWND hWndMsg) {
        wstring tempOn = wstring(cheat) + cheatOn;
        wstring tempOff = wstring(cheat) + cheatOff;

        if (!cheatFlag) {
            cheatFlag = true;
            SetWindowTextW(hWndMsg, tempOn.c_str());
        }
        else {
            cheatFlag = false;
            SetWindowTextW(hWndMsg, tempOff.c_str());
        }
    }

    void AddSwitchControls(HWND hWnd) {
        titleMsg = CreateWindowW(L"Static", L"HackAndGO", WS_VISIBLE | WS_CHILD, 20, 20, 350, 70, hWnd, NULL, NULL, NULL);
        instrMsg = CreateWindowW(L"Static", L"Use the below switches to turn the cheats on and off", WS_VISIBLE | WS_CHILD, 70, 90, 350, 70, hWnd, NULL, NULL, NULL);
        btn1 = CreateWindowW(L"Button", L"Switch", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 130, 100, 40, hWnd, (HMENU)SWITCH_BHOP_BUTTON, NULL, NULL);
        bhopMsg = CreateWindowW(L"Edit", L"bhop cheat is turned off", WS_VISIBLE | WS_CHILD | WS_BORDER, 250, 130, 170, 40, hWnd, NULL, NULL, NULL);
        btn2 = CreateWindowW(L"Button", L"Switch", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, 180, 100, 40, hWnd, (HMENU)SWITCH_CHAMS_BUTTON, NULL, NULL);
        chamsMsg = CreateWindowW(L"Edit", L"chams cheat is turned off", WS_VISIBLE | WS_CHILD | WS_BORDER, 250, 180, 182, 40, hWnd, NULL, NULL, NULL);
    }

    LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg)
        {
        case WM_COMMAND:
            switch (wp)
            {
            case SWITCH_BHOP_BUTTON:
                SwitchCheats(isBhop, L"Bhop", bhopMsg);
                break;
            case SWITCH_CHAMS_BUTTON:
                SwitchCheats(isChams, L"Chams", chamsMsg);
                break;
            }
            break;
        case WM_CREATE:
            {
                AddSwitchControls(hWnd);
                CreateAndSetFonts();
            }
            break;
        case WM_DESTROY:
            DeleteObject(defaultFont);
            DeleteObject(titleFont);
            PostQuitMessage(0);
            break;
        }

        return DefWindowProcW(hWnd, msg, wp, lp);;
    }

    int WINAPI guiWindow(HINSTANCE hInst, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
        WNDCLASSW window = { 0 };

        window.hbrBackground = (HBRUSH)COLOR_WINDOW;
        window.hCursor = LoadCursor(NULL, IDC_ARROW);
        window.hInstance = hInst;
        window.lpszClassName = L"cheat";
        window.lpfnWndProc = WindowProcedure;
        window.hIcon = LoadIconA(NULL, IDI_ERROR);
        if (!RegisterClassW(&window)) {
            printf_s("FAIL!\n");
            return -1;
        }

        HWND hWnd = CreateWindowW(L"cheat", L"HackAndGO", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 350, NULL, NULL, NULL, NULL);

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        printf_s("GUI Window Created!\n");

        MSG msg = { 0 };

        while (GetMessage(&msg, NULL, 0, 0)) {
            DispatchMessage(&msg);
        }

        printf_s("GUI Window terminated!\n");

        return (int)msg.wParam;
    }
}