#ifndef _NFOVIEW_H
#define _NFOVIEW_H

#include <Windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

BOOL CALLBACK DlgNfo(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void NfoInit(HINSTANCE hInstance, const char* nfo_id, const char* nfo_resid);

#ifdef __cplusplus
}
#endif

#endif