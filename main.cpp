#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "effect.h"
#include "nfoview.h"
#include "server.h"
#include "hosts.h"
#include "keygen.h"
#include "oldkeygen.h"
#include "acdsee16.h"

bool server_enabled=true;
bool hosts_enabled=true;

HINSTANCE hInst;
HBRUSH hBrush;
HCURSOR hCursor;

static char sinemsg[]= {" ACDSystems Multi-Keygen\n"\
                        " GFX...............zorke\n"\
                        " Music..............peak\n"\
                        " Keygen.............RNDD"
                       };

static char scrlmsg[]="                                              "
                      "Real Niggaz Dont Die are back again, flaunting today an ACDSystems Multi-Keygen!"
                      " ----------------------------------------- "
                      "Protection: MD5 + Custom CRC32 + Base32 + Server Check + Custom Algos"
                      " ----------------------------------------- "
                      "The sublime SFX was provided by the wonderful Peak^NERVE"
                      " ----------------------------------------- "
                      "Greetings to all our favourite niggaz in: CORE . Lz0 . LnDL . TSRh . NERVE . TPoDT . DVT . BRD"
                      " ----------------------------------------- "
                      "Niggaz can roll with us, or they can be rolled under us"
                      " ----------------------------------------- "
                      "whats it goin to be?"
                      "                                              ";

char productid[10]="";
char language[3]="";

bool oldmethod=false;
int oldproductid[3]= {0, 0, 0};
int oldlanguage=0;

HWND CreateChildWindow(HINSTANCE hInstance, int dlg_id, HWND father, int child_base, DLGPROC dlgproc)
{
    RECT base;
    GetWindowRect(GetDlgItem(father, child_base), &base);
    HRSRC hrsrc=FindResource(0, MAKEINTRESOURCE(dlg_id), RT_DIALOG);
    HGLOBAL hglb=LoadResource(hInstance, hrsrc);
    return CreateDialogIndirectParam(hInstance, (DLGTEMPLATE*)LockResource(hglb), father, dlgproc, (LPARAM)&base);
}

void oldcpy(int a, int b, int c)
{
    oldproductid[0]=a;
    oldproductid[1]=b;
    oldproductid[2]=c;
}

void ProductSel(HWND hwndDlg, int langsel, int cursel)
{
    EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_ACDSEE16), 0);
    bool disable_hosts=false;
    switch(langsel)
    {
    case 0: //English
    {
        oldmethod=false;
        switch(cursel)
        {
        case 0: //ACDSee Pro 6 x64/x32
            strcpy(productid, "ACPW06");
            break;

        case 1: //ACDSee 16
            disable_hosts=true;
            EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_ACDSEE16), 1);
            strcpy(productid, "ACSW16");
            break;

        case 2: //Photo Editor 6
            strcpy(productid, "PHEW06");
            break;

        case 3: //Canvas 14
            strcpy(productid, "CVPW14");
            break;

        case 4: //Fotoslate 4
            oldmethod=true;
            oldcpy(0x384, 0x3b5, 5);
            break;

        case 5: //Video Converter 3
            strcpy(productid, "VIDC03");
            break;

        case 6: //Video Converter Pro 3
            strcpy(productid, "VIDCP3");
            break;
        }
    }
    break;

    case 1: //German
    {
        oldmethod=false;
        switch(cursel)
        {
        case 0: //ACDSee Pro 6 x64/x32
            strcpy(productid, "ACPW06");
            break;

        case 1: //ACDSee 16
            disable_hosts=true;
            EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_ACDSEE16), 1);
            strcpy(productid, "ACSW16");
            break;

        case 2: //Photo Editor 4
            oldmethod=true;
            oldcpy(0x4b0, 0x513, 9);
            break;

        case 3: //Fotoslate 4
            oldmethod=true;
            oldcpy(0x384, 0x3b5, 5);
            break;

        case 4: //Video Converter 3
            strcpy(productid, "VIDC03");
            break;

        case 5: //Video Converter Pro 3
            strcpy(productid, "VIDCP3");
            break;
        }
    }
    break;

    case 2: //French
    {
        oldmethod=false;
        switch(cursel)
        {
        case 0: //ACDSee Pro 6 x64/x32
            strcpy(productid, "ACPW06");
            break;

        case 1: //ACDSee 16
            disable_hosts=true;
            EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_ACDSEE16), 1);
            strcpy(productid, "ACSW16");
            break;

        case 2: //Photo Editor 4
            oldmethod=true;
            oldcpy(0x4b0, 0x513, 9);
            break;

        case 3: //Fotoslate 4
            oldmethod=true;
            oldcpy(0x384, 0x3b5, 5);
            break;

        case 4: //Video Converter 3
            strcpy(productid, "VIDC03");
            break;

        case 5: //Video Converter Pro 3
            strcpy(productid, "VIDCP3");
            break;
        }
    }
    break;

    case 3: //Chinese Simplified
    {
        oldmethod=false;
        switch(cursel)
        {
        case 0: //ACDSee Pro 6 x64/x32
            strcpy(productid, "ACPW06");
            break;

        case 1: //ACDSee 15
            strcpy(productid, "ACSW15");
            break;
        }
    }
    break;

    case 4: //Chinese Traditional
    {
        oldmethod=false;
        switch(cursel)
        {
        case 0: //ACDSee Pro 6 x64/x32
            strcpy(productid, "ACPW06");
            break;

        case 1: //ACDSee 15
            strcpy(productid, "ACSW15");
            break;
        }
    }
    break;

    case 5: //Japanese
    {
        oldmethod=false;
        switch(cursel)
        {
        case 0: //ACDSee Pro 6 x64/x32
            strcpy(productid, "ACPW06");
            break;

        case 1: //ACDSee 15
            strcpy(productid, "ACSW15");
            break;

        case 2: //Canvas 14
            strcpy(productid, "CVPW14");
            break;
        }
    }
    break;

    case 6: //Spanish
    {
        oldmethod=true;
        switch(cursel)
        {
        case 0: //ACDSee Fotomanager 8
            oldcpy(0x2bc, 0x31f, 3);
            break;
        }
    }
    break;

    case 7: //Italian
    {
        oldmethod=true;
        switch(cursel)
        {
        case 0: //ACDSee Fotomanager 8
            oldcpy(0x2bc, 0x31f, 3);
            break;

        case 1: //ACDSee Pro 8
            oldcpy(0x3e8, 0x44b, 0x2e1);
            break;
        }
    }
    break;

    case 8: //Dutch
    {
        oldmethod=true;
        switch(cursel)
        {
        case 0: //ACDSee Fotomanager 9
            oldcpy(0x514, 0x577, 3);
            break;

        case 1: //ACDSee Photo Editor 4
            oldcpy(0x4b0, 0x513, 9);
            break;
        }
    }
    break;
    }
    bool en=true;
    if(disable_hosts or oldmethod)
        en=false;
    if(server_enabled)
        EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_STARTSERVER), en);
    if(hosts_enabled)
        EnableWindow(GetDlgItem(hwndDlg, IDC_BTN_PATCHHOSTS), en);
}

void LanguageSel(HWND hwndDlg, int cursel)
{
    HWND product=GetDlgItem(hwndDlg, IDC_COMBO_PRODUCT);
    SendMessageA(product, CB_RESETCONTENT, 0, 0);
    switch(cursel)
    {
    case 0: //English
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Pro 6 x64/x32");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee 16");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Photo Editor 6");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Canvas 14");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Fotoslate 4");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Video Converter 3");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Video Converter Pro 3");
        strcpy(language, "EN");
        oldlanguage=0;
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 1: //German
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Pro 6 x64/x32");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee 16");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Foto-Editor 4");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Fotoslate 4");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Video Converter 3");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Video Converter Pro 3");
        strcpy(language, "DE");
        oldlanguage=1;
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 2: //French
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Pro 6 x64/x32");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee 16");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Retouche Photo 4");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Fotoslate 4");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Video Converter 3");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Video Converter Pro 3");
        strcpy(language, "FR");
        oldlanguage=3;
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 3: //Chinese Simplified
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Pro 6 x64/x32");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee 15");
        strcpy(language, "ZA");
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 4: //Chinese Traditional
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Pro 6 x64/x32");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee 15");
        strcpy(language, "ZH");
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 5: //Japanese
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Pro 6 x64/x32");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee 15");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"Canvas 14");
        strcpy(language, "JA");
        oldlanguage=2;
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 6: //Spanish
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Fotomanager 8");
        oldlanguage=5;
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 7: //Italian
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Fotomanager 8");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Pro 8");
        oldlanguage=4;
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;

    case 8: //Dutch
    {
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Fotomanager 9");
        SendMessageA(product, CB_ADDSTRING, 0, (LPARAM)"ACDSee Photo Editor 4");
        oldlanguage=8;
        SendMessageA(product, CB_SETCURSEL, 0, 0);
        ProductSel(hwndDlg, cursel, 0);
    }
    break;
    }
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        hCursor=LoadCursorA(hInst, MAKEINTRESOURCE(IDI_CURSOR1));
        SendMessageA(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1)));
        SetWindowTextA(hwndDlg, "Keygen");
        EffectInit(hInst, MAKEINTRESOURCE(XM_MUSIC), 0, sinemsg, scrlmsg, 0, "[RNDD] - ACDSystems Multi-Keygen v1");
        CreateChildWindow(hInst, DLG_EFFECT, hwndDlg, IDC_STC_FRAME, DlgEffect);
        HWND language=GetDlgItem(hwndDlg, IDC_COMBO_LANGUAGE);
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"English"); //EN
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"German"); //DE
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"French"); //FR
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"CN (Simplified)"); //ZA
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"CN (Traditional)"); //ZH
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"Japanese"); //JP
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"Spanish"); //ES
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"Italian"); //IT
        SendMessageA(language, CB_ADDSTRING, 0, (LPARAM)"Dutch"); //NL
        SendMessageA(language, CB_SETCURSEL, 0, 0);
        LanguageSel(hwndDlg, 0);
        SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
    }
    return TRUE;

    case WM_SETCURSOR:
    {
        SetCursor(hCursor);
        SetWindowLong(hwndDlg, DWL_MSGRESULT, TRUE);
    }
    return TRUE;

    case WM_CLOSE:
    {
        ServerStop();
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_CTLCOLORSTATIC:
    {
        if(GetDlgCtrlID((HWND)lParam)!=IDC_EDT_SERIAL)
        {
            SetTextColor((HDC)wParam,RGB(255,255,255));
            SetBkMode((HDC)wParam,TRANSPARENT);
            return (BOOL)GetStockObject(NULL_BRUSH);
        }
        break;
    }

    case WM_CTLCOLORDLG:
    {
        HBRUSH g_hbrBackground=CreateSolidBrush(RGB(0,0,0));
        return (BOOL)g_hbrBackground;
    }

    case WM_LBUTTONDOWN:
    {
        SendMessage(hwndDlg, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDC_BTN_GENERATE:
        {
            const char* serial;
            if(oldmethod)
                serial=OldKeygenGenerate(oldproductid[0], oldproductid[1], oldlanguage, oldproductid[2]);
            else
            {
                char prodid[50]="";
                sprintf(prodid, "%s%s", productid, language);
                serial=KeygenGenerate(prodid);
            }
            SetDlgItemTextA(hwndDlg, IDC_EDT_SERIAL, serial);
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
        }
        return TRUE;

        case IDC_COMBO_PRODUCT:
        {
            switch(HIWORD(wParam))
            {
            case CBN_SELCHANGE:
            {
                ProductSel(hwndDlg, SendMessageA(GetDlgItem(hwndDlg, IDC_COMBO_LANGUAGE), CB_GETCURSEL, 0, 0), SendMessageA((HWND)lParam, CB_GETCURSEL, 0, 0));
                SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
            }
            break;
            }
        }
        return TRUE;

        case IDC_COMBO_LANGUAGE:
        {
            switch(HIWORD(wParam))
            {
            case CBN_SELCHANGE:
            {
                LanguageSel(hwndDlg, SendMessageA((HWND)lParam, CB_GETCURSEL, 0, 0));
                SendMessageA(hwndDlg, WM_COMMAND, IDC_BTN_GENERATE, 0);
            }
            break;
            }
        }
        return TRUE;

        case IDC_BTN_PATCHHOSTS:
        {
            if(HostsPatch(hwndDlg))
            {
                EnableWindow((HWND)lParam, 0);
                hosts_enabled=false;
            }
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
        }
        return TRUE;

        case IDC_BTN_STARTSERVER:
        {
            if(ServerIsStarted())
            {
                MessageBoxA(hwndDlg, "Server already started", "Derp", MB_ICONINFORMATION);
                return true;
            }
            if(!ServerStart(hwndDlg))
            {
                MessageBoxA(hwndDlg, ServerLastError(), "Could not start server on port 443, the program will exit", MB_ICONERROR);
                ExitProcess(0);
            }
            else
            {
                MessageBoxA(hwndDlg, "Server started on port 443", "YAY!", MB_ICONINFORMATION);
                EnableWindow((HWND)lParam, 0);
                server_enabled=false;
            }
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
        }
        return TRUE;

        case IDC_BTN_EXIT:
        {
            SendMessageA(hwndDlg, WM_CLOSE, 0, 0);
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
        }
        return TRUE;

        case IDC_BTN_NFO:
        {
            NfoInit(hInst, MAKEINTRESOURCE(NFO_FILE), RT_RCDATA);
            DialogBoxParam(hInst, MAKEINTRESOURCE(DLG_NFO), hwndDlg, DlgNfo, 0);
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
        }
        return TRUE;

        case IDC_BTN_ACDSEE16:
        {
            MemPatch(hwndDlg);
            SetFocus(GetDlgItem(hwndDlg, IDC_STC_FOCUS));
        }
        return TRUE;
        }
    }
    return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
