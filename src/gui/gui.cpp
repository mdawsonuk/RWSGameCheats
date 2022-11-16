#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include "../../include/gui/gui.h"
#include "../../include/gui/guiControl.h"

using namespace std;

#define SWITCH_BHOP_BUTTON 1
#define SWITCH_CHAMS_BUTTON 2
#define SWITCH_GLOW_BUTTON 3

namespace Gui 
{
    vector<HWND> buttons;
    vector<HWND> messages;
    vector<LPCWSTR> cheats{ L"bhop", L"chams", L"glow" };

    HWND title, instructions;
    HFONT defaultFont, titleFont;

    LPCWSTR cheatOn = L" cheat is turned on";
    LPCWSTR cheatOff = L" cheat is turned off";

    void CreateAndSetFonts() {
        defaultFont = CreateFont(18, 0, 0, 0, 500, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        titleFont = CreateFont(40, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
        SendMessage(title, WM_SETFONT, (LPARAM)titleFont, TRUE);
        SendMessage(instructions, WM_SETFONT, (LPARAM)defaultFont, TRUE);

        for (size_t i = 0; i < messages.size(); i++) {
            SendMessage(messages[i], WM_SETFONT, (LPARAM)defaultFont, TRUE);
            SendMessage(buttons[i], WM_SETFONT, (LPARAM)defaultFont, TRUE);
        }
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
        title = CreateWindowW(L"Static", L"HackAndGO", WS_VISIBLE | WS_CHILD, 20, 20, 350, 70, hWnd, NULL, NULL, NULL);
        instructions = CreateWindowW(L"Static", L"Use the below switches to turn the cheats on and off", WS_VISIBLE | WS_CHILD, 70, 90, 350, 70, hWnd, NULL, NULL, NULL);

        int y = 130;

        for (size_t i = 0; i < cheats.size(); i++) {
            wstring temp = wstring(cheats[i]) + cheatOff;
            messages.push_back(CreateWindowW(L"Edit", temp.c_str(), WS_VISIBLE | WS_CHILD | WS_BORDER, 250, y, 182, 40, hWnd, NULL, NULL, NULL));
            buttons.push_back(CreateWindowW(L"Button", L"Switch", WS_VISIBLE | WS_CHILD | WS_BORDER, 100, y, 100, 40, hWnd, (HMENU)(i + 1), NULL, NULL));

            y += 50;
        }
    }

    LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
        switch (msg)
        {
        case WM_COMMAND:
            switch (wp)
            {
            case SWITCH_BHOP_BUTTON:
                SwitchCheats(isBhop, cheats[0], messages[0]);
                break;
            case SWITCH_CHAMS_BUTTON:
                SwitchCheats(isChams, cheats[1], messages[1]);
                break;
            case SWITCH_GLOW_BUTTON:
                SwitchCheats(isGlow, cheats[2], messages[2]);
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