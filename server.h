#ifndef _SERVER_H
#define _SERVER_H

#include <windows.h>

bool ServerStart(HWND hwndDlg);
void ServerStop();
bool ServerIsStarted();
const char* ServerLastError();

#endif // _SERVER_H
