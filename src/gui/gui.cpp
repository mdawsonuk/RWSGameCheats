#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include "gui/gui.h"
#include "gui/guiControl.h"

using namespace std;

#define SWITCH_BHOP_BUTTON 1
#define SWITCH_CHAMS_BUTTON 2
#define SWITCH_GLOW_BUTTON 3
#define SWITCH_NOFLAH_BUTTON 4
#define SWITCH_AIMBOT_AND_NORECOIL 5
#define SWTICH_ADAPTIVE_CHEAT_CONTROL 6

namespace Gui 
{
    vector<HWND> buttons, messages;
    vector<LPCWSTR> cheats{ L"bhop", L"chams", L"glow" , L"noFlash", L"aimbot and noRecoil" };

    HWND title, instructions, adaptiveCheatControl, adaptiveButton;
    HFONT defaultFont, titleFont;

    LPCWSTR cheatOn = L" cheat is turned on";
    LPCWSTR cheatOff = L" cheat is turned off";

    void SwitchBasedOnAdaptive(HWND hWnd) {
        for (size_t i = 0; i < buttons.size() - 1; i++) {
            wchar_t temp[150];
            GetWindowTextW(messages[i], temp, 150);
            if (wcsstr(temp, L"off") != 0) {
                SendMessage(hWnd, WM_COMMAND, (i + 1), 0);
            }
            EnableWindow(buttons[i], !isAdaptive);
        }
    }

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
        
        instructions = CreateWindowW(L"Static", L"Use the below switches to turn the cheats on and off", WS_VISIBLE | WS_CHILD, 100, 80, 350, 70, hWnd, NULL, NULL, NULL);
        
        adaptiveCheatControl = CreateWindowW(L"EDIT", L"adaptive cheat control is turned off", WS_VISIBLE | WS_CHILD, 230, 130, 250, 40, hWnd, NULL, NULL, NULL);
        adaptiveButton = CreateWindowW(L"Button", L"Switch", WS_VISIBLE | WS_CHILD | WS_BORDER, 80, 130, 100, 40, hWnd, (HMENU)SWTICH_ADAPTIVE_CHEAT_CONTROL, NULL, NULL);

        int y = 200;

        for (size_t i = 0; i < cheats.size(); i++) {
            wstring temp = wstring(cheats[i]) + cheatOff;
            messages.push_back(CreateWindowW(L"Edit", temp.c_str(), WS_VISIBLE | WS_CHILD, 230, y, 265, 40, hWnd, NULL, NULL, NULL));
            buttons.push_back(CreateWindowW(L"Button", L"Switch", WS_VISIBLE | WS_CHILD | WS_BORDER, 80, y, 100, 40, hWnd, (HMENU)(i + 1), NULL, NULL));

            y += 50;
        }

        buttons.push_back(adaptiveButton);
        messages.push_back(adaptiveCheatControl);
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
            case SWITCH_NOFLAH_BUTTON:
                SwitchCheats(isNoFlah, cheats[3], messages[3]);
                break;
            case SWITCH_AIMBOT_AND_NORECOIL:
                SwitchCheats(isAimbotAndNoRecoil, cheats[4], messages[4]);
                break;
            case SWTICH_ADAPTIVE_CHEAT_CONTROL:
                {
                    if (!isAdaptive) {
                        isAdaptive = true;
                        SetWindowTextW(adaptiveCheatControl, L"adaptive cheat control is turned on");
                    }
                    else {
                        isAdaptive = false;
                        SetWindowTextW(adaptiveCheatControl, L"adaptive cheat control is turned off");
                    }
                    SwitchBasedOnAdaptive(hWnd);
                }
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

        HWND hWnd = CreateWindowW(L"cheat", L"HackAndGO", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 550, NULL, NULL, NULL, NULL);

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