#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include <windows.h>
#include <iostream>
using namespace std;

int cnt_UpDown = 0;
int cnt_LeftRight = 0;

int cnt_UpDown_player2 = 100;
int cnt_LeftRight_player2 = 100;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    /// 方框
    HDC hdc = GetDC(hwnd);
    for(int i = 1 ; i <= 501 ; i += 2)
    {
        for(int j = 1 ; j <= 501 ; j += 50)
        {
            RECT rect = { i, j, i+2, j+2 };
            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        }
    }
    for(int i = 1 ; i <= 501 ; i += 2)
    {
        for(int j = 1 ; j <= 501 ; j += 50)
        {
            RECT rect = { j, i, j+2, i+2 };
            FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        }
    }

    wchar_t msg[32];
    switch (uMsg)
    {
        case WM_KEYDOWN:
        {
            swprintf_s(msg, L"WM_KEYDOWN: 0x%x\n", wParam);
            OutputDebugStringW(msg);
            cout << "WM_KEYDOWN" << endl;

            HBRUSH redBrush = CreateSolidBrush(RGB(255, 0, 0));
            HBRUSH blueBrush = CreateSolidBrush(RGB(0, 0, 255));

            RECT rect;
            switch (wParam)
            {
            case VK_UP:
                cout << "up" << endl;
                cnt_UpDown -= 50;
                rect = { 2 + cnt_LeftRight, 2 + cnt_UpDown, 52 + cnt_LeftRight, 52 + cnt_UpDown }; //紅色方框
                FillRect(hdc, &rect, redBrush);
                break;

            case VK_DOWN:
                cout << "down" << endl;
                cnt_UpDown += 50;
                rect = { 2 + cnt_LeftRight, 2 + cnt_UpDown, 52 + cnt_LeftRight, 52 + cnt_UpDown }; //紅色方框
                FillRect(hdc, &rect, redBrush);
                break;

            case VK_LEFT:
                cout << "left" << endl;
                cnt_LeftRight -= 50;
                rect = { 2 + cnt_LeftRight, 2 + cnt_UpDown, 52 + cnt_LeftRight, 52 + cnt_UpDown }; //紅色方框
                FillRect(hdc, &rect, redBrush);
                break;

            case VK_RIGHT:
                cout << "right" << endl;
                cnt_LeftRight += 50;
                rect = { 2 + cnt_LeftRight, 2 + cnt_UpDown, 52 + cnt_LeftRight, 52 + cnt_UpDown }; //紅色方框
                FillRect(hdc, &rect, redBrush);
                break;

            case 'W':
                cout << "W pressed" << endl;
                cnt_UpDown_player2 -= 50;
                rect = { 2 + cnt_LeftRight_player2, 2 + cnt_UpDown_player2, 52 + cnt_LeftRight_player2, 52 + cnt_UpDown_player2 }; //紅色方框
                FillRect(hdc, &rect, blueBrush);
                break;

            case 'S':
                cout << "S pressed" << endl;
                cnt_UpDown_player2 += 50;
                rect = { 2 + cnt_LeftRight_player2, 2 + cnt_UpDown_player2, 52 + cnt_LeftRight_player2, 52 + cnt_UpDown_player2 }; //紅色方框
                FillRect(hdc, &rect, blueBrush);
                break;

            case 'A':
                cout << "A pressed" << endl;
                cnt_LeftRight_player2 -= 50;
                rect = { 2 + cnt_LeftRight_player2, 2 + cnt_UpDown_player2, 52 + cnt_LeftRight_player2, 52 + cnt_UpDown_player2 }; //紅色方框
                FillRect(hdc, &rect, blueBrush);
                break;

            case 'D':
                cout << "D pressed" << endl;
                cnt_LeftRight_player2 += 50;
                rect = { 2 + cnt_LeftRight_player2, 2 + cnt_UpDown_player2, 52 + cnt_LeftRight_player2, 52 + cnt_UpDown_player2 }; //紅色方框
                FillRect(hdc, &rect, blueBrush);
                break;

            default:
                break;
            }
            ReleaseDC(hwnd, hdc);
            break;
        }

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int main()
{
    // 註冊視窗
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = L"SampleWindowClass";
    RegisterClassW(&wc);

    // 創建視窗
    HWND hwnd = CreateWindowExW(0, L"SampleWindowClass", L"Sample Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, GetModuleHandleW(NULL), NULL);

    if (hwnd == NULL)
    {
        cerr << "視窗創建失敗！" << endl;
        return 1;
    }

    // 顯示視窗
    ShowWindow(hwnd, SW_SHOWDEFAULT);

    // 進入消息循環
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 恢復編譯器警告設定
    #pragma GCC diagnostic pop

    return 0;
}
