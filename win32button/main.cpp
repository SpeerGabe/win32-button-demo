#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <string>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND hButton1, hButton2, hButton3, hStatic;
HFONT hFont;

COLORREF bgColor = RGB(255, 255, 255);
std::wstring displayText = L"";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Speer Buttons",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        // Buttons
        hButton1 = CreateWindow(
            L"BUTTON", L"One",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 50, 100, 40,
            hwnd, (HMENU)1, NULL, NULL
        );

        hButton2 = CreateWindow(
            L"BUTTON", L"Two",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            200, 50, 100, 40,
            hwnd, (HMENU)2, NULL, NULL
        );

        hButton3 = CreateWindow(
            L"BUTTON", L"Quit",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            350, 50, 100, 40,
            hwnd, (HMENU)3, NULL, NULL
        );

        // Static text area
        hStatic = CreateWindow(
            L"STATIC", L"",
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            50, 150, 500, 100,
            hwnd, NULL, NULL, NULL
        );

        // Large font for display text
        hFont = CreateFont(
            48, 0, 0, 0,
            FW_BOLD, FALSE, FALSE, FALSE,
            ANSI_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS,
            L"Arial"
        );

        SendMessage(hStatic, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    return 0;

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);

        if (id == 1)
        {
            displayText = L"ONE";
            bgColor = RGB(255, 200, 200); // change background
            SetWindowText(hStatic, displayText.c_str());
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (id == 2)
        {
            displayText = L"TWO";
            bgColor = RGB(200, 255, 200); // change background
            SetWindowText(hStatic, displayText.c_str());
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if (id == 3)
        {
            PostQuitMessage(0);
        }
    }
    return 0;

    case WM_MOUSEMOVE:
    {
        POINT pt;
        pt.x = LOWORD(lParam);
        pt.y = HIWORD(lParam);

        RECT rc;
        GetWindowRect(hButton3, &rc);

        // Convert screen coords to client coords
        ScreenToClient(hwnd, (POINT*)&rc.left);
        ScreenToClient(hwnd, (POINT*)&rc.right);

        // Make quit button run away
        if (pt.x > rc.left && pt.x < rc.right &&
            pt.y > rc.top && pt.y < rc.bottom)
        {
            // Move button to new random position
            SetWindowPos(hButton3, NULL, pt.x + 100, pt.y + 50, 100, 40, SWP_NOZORDER);
        }
    }
    return 0;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetBkColor(hdcStatic, bgColor);
        return (INT_PTR)GetStockObject(NULL_BRUSH);
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Fill background with current color
        HBRUSH hBrush = CreateSolidBrush(bgColor);
        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);

        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
