#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "Structs.h"
#include "Memory.h"
#include "MiscVisuals.h"
#include "Offsets.h"
#include "MainFuncs.h"
#include "gdi_shit.h"

using namespace std;
int local = GetIndex();
int localteam = GetPlayerTeam(local);

int width;
int height;

IDirect3D9Ex* d3dObject = NULL;
IDirect3DDevice9Ex* d3dDevice = NULL;
D3DPRESENT_PARAMETERS d3dparams;
ID3DXFont* d3dFont = 0;
RECT rect;

int d3D9Init(HWND hWnd)
{
    RECT rect;
    GetWindowRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dObject)))
    {
        exit(1);
    }

    ZeroMemory(&d3dparams, sizeof(d3dparams));

    d3dparams.BackBufferWidth = width;
    d3dparams.BackBufferHeight = height;
    d3dparams.Windowed = TRUE;
    d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dparams.hDeviceWindow = hWnd;
    d3dparams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dparams.EnableAutoDepthStencil = TRUE;
    d3dparams.AutoDepthStencilFormat = D3DFMT_D16;

    HRESULT res = d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dparams, 0, &d3dDevice);

    D3DXCreateFont(d3dDevice, 50, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Comic Sans", &d3dFont);

    return 0;

}

void drawText(char* String, int x, int y, int a, int r, int g, int b)
{
    RECT FontPos;
    FontPos.left = x;
    FontPos.top = y;
    d3dFont->DrawTextA(0, String, strlen(String), &FontPos, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}

int render(HWND hWnd)
{
    RECT rect;
    GetWindowRect(hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    if (d3dDevice == nullptr)
        return 1;
    d3dDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
    d3dDevice->BeginScene();

    // if (GameHWND == GetForegroundWindow())
     //{
    drawText((char*)"U got hacked broo", width / 10, height / 10, 255, 171, 0, 182);
    //}

    d3dDevice->EndScene();
    d3dDevice->PresentEx(0, 0, 0, 0, 0);

    return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {

    case WM_PAINT:
    {      
        render(hwnd);
    }

    case WM_ERASEBKGND:
        return 1;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main()
{
    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&GlowLoop, NULL, NULL, NULL);
    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&BunnyHop, NULL, NULL, NULL);
    //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&SpinbotLoop, NULL, NULL, NULL);
    HWND GameHWND = FindWindow(NULL, L"Counter-Strike: Global Offensive");
    const wstring class1= L"class1";
    GetClientRect(GameHWND, &WBounds);
    HINSTANCE Hinstance = NULL;
    WNDCLASSEX WClass;
    MSG Msg;
    WClass.cbSize = sizeof(WNDCLASSEX);
    WClass.style = NULL;
    WClass.lpfnWndProc = WndProc;
    WClass.cbClsExtra = NULL;
    WClass.cbWndExtra = NULL;
    WClass.hInstance = Hinstance; // reinterpret_cast<HINSTANCE>(GetWindowLong(GameHWND, GWL_HINSTANCE));
    WClass.hIcon = NULL;
    WClass.hCursor = NULL;
    WClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); // WHITE_BRUSH;
    WClass.lpszMenuName = L"";
    WClass.lpszClassName = L"class1";
    WClass.hIconSm = NULL;
    RegisterClassEx(&WClass);
    if (GameHWND) 
    {
        RECT rect;
        GetWindowRect(GameHWND, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }
    EspHWND = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, L"class1", L"", 0, 0,0,1920,1080, GameHWND, NULL, Hinstance, NULL); //WBounds.left, WBounds.top, WBounds.right - WBounds.left, WBounds.bottom + WBounds.left
    SetLayeredWindowAttributes(EspHWND, RGB(0, 0, 0), 255, LWA_COLORKEY);
    ShowWindow(EspHWND, 1);
    while (GetMessage(&Msg, 0, 0, 0)) //GetMessageA(&Msg, NULL, NULL, NULL) > 0    GetMessage(&Msg, nullptr, 0, 0)
    {
        if (GetAsyncKeyState(VK_RBUTTON))
        {
            int xt = GetClosestEnemy();
            vector3 uh = GetBonePosEnemy(xt, 8);
            AimAt(&uh);
            // handleTBot();
            Sleep(3);
        }
        TranslateMessage(&Msg);
        DispatchMessageA(&Msg);
        MoveWindowAlongOtherWindow(GameHWND, EspHWND);
        Sleep(3);
    }
    ExitThread(0);
    return 0;
}

