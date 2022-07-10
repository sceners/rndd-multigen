#ifndef _EFFECT_H
#define _EFFECT_H

#include <Windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

BOOL CALLBACK DlgEffect(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
void EffectInit(HINSTANCE hInstance, const char* music_id, const char* music_resid, char* sine_msg, char* scroll_msg, int pos_add, const char* kgtitle);
void SetNfoFlag(int val);

#ifdef __cplusplus
}
#endif

#endif
