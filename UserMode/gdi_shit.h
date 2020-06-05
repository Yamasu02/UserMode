#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


HPEN BoxPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
HPEN BoxPen2 = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
HPEN BoxPen3 = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
RECT WBounds;
HWND EspHWND;


void Draw(HDC hdc, Vector1 foot, Vector1 head, HPEN BoxPen)
{
    float height = head.y - foot.y;
    float width = height / 2.4f;
    SelectObject(hdc, BoxPen);
    Rectangle(hdc, foot.x - (width / 2), head.y, head.x + (width / 2), foot.y);
}
/*
void DrawLineGDI(int i, int boneid1, int boneid2, HDC Memhdc, ViewMatrix vm)
{
    Vector xtt = WorldToScreen(GetBonePosEnemy(i, boneid1), vm);
    Vector xtt2 = WorldToScreen(GetBonePosEnemy(i, boneid2), vm);
    MoveToEx(Memhdc, xtt.x, xtt.y, 0);
    LineTo(Memhdc, xtt2.x, xtt2.y);
}

void Skeleton(int i, HDC Memhdc, ViewMatrix vm)
{
    DrawLineGDI(i, 0, 71, Memhdc, vm);
    DrawLineGDI(i, 78, 79, Memhdc, vm);
    DrawLineGDI(i, 77, 78, Memhdc, vm);
    DrawLineGDI(i, 71, 72, Memhdc, vm);
    DrawLineGDI(i, 0, 7, Memhdc, vm);
    DrawLineGDI(i, 7, 41, Memhdc, vm);
    DrawLineGDI(i, 7, 11, Memhdc, vm);
    DrawLineGDI(i, 41, 42, Memhdc, vm);
    DrawLineGDI(i, 42, 43, Memhdc, vm);
    DrawLineGDI(i, 11, 12, Memhdc, vm);
    DrawLineGDI(i, 12, 13, Memhdc, vm);
}
*/



void MoveWindowAlongOtherWindow(HWND Gamehwnd,HWND Overlayhwnd)
{
    RECT* rect = new RECT;
    GetWindowRect(Gamehwnd, rect);
    int width = rect->right - rect->left;
    int height = rect->bottom - rect->top;
    MoveWindow(Overlayhwnd, rect->left, rect->top, width, height, true);
    delete rect;
}