#include "server.h"
#include "mongoose.h"
#include "ssl_cert.h"
#include <cyassl/openssl/ssl.h>

static struct mg_context* ctx=0;

static int begin_request_handler(struct mg_connection *conn)
{
    const struct mg_request_info *ri = mg_get_request_info(conn);
    //puts(ri->uri);
    if(!strcmp(ri->request_method, "POST"))
    {
        char post_data[65536]="",response[65536]="";
        mg_read(conn, post_data, sizeof(post_data));
        //puts(post_data);
        if(strstr(post_data, "CreateAccount"))
            strcpy(response, "{\"id\":1,\"result\":\"created\",\"error\":null}");
        else if(strstr(post_data, "ValidateEmailValidationCode"))
            strcpy(response, "{\"id\":1,\"result\":\"valid\",\"error\":null}");
        else if(strstr(post_data, "RegisterFullProduct") or strstr(post_data, "RegisterTrialProduct"))
        {
            SYSTEMTIME st;
            GetSystemTime(&st);
            sprintf(response, "{\"id\":1,\"result\":{\"key_status\":\"valid\",\"register_status\":\"success\",\"server_date\":\"%.4d-%.2d-%.2d\"\"user_password_token\":\"+08bcdda0c383e62a91e2e816308d99b3\"},\"error\":null}", st.wYear, st.wMonth, st.wDay);
        }
        else if(strstr(post_data, "CheckOrMigrateAccountExt"))
            strcpy(response, "{\"id\":1,\"result\":{\"status\":\"success\",\"first_name\":\"RNDD\",\"last_name\":\"\1\"},\"error\":null}");
        mg_printf(conn, "HTTP/1.0 200 OK\r\n"
                  "Content-Length: %d\r\n"
                  "Content-Type: text/html\r\n\r\n%s",
                  (int) strlen(response), response);
    }
    else
    {
        mg_printf(conn, "HTTP/1.0 200 OK\r\n"
                  "Content-Length: 2\r\n"
                  "Content-Type: text/html\r\n\r\nStop sniffing around dude...");
    }
    //puts(" ------------------------------------------------------------------------------");
    return 1;  // Mark request as processed
}

bool ServerStart(HWND hwndDlg)
{
    HANDLE hFile=CreateFileA("ssl_cert.pem", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFile==INVALID_HANDLE_VALUE)
    {
        MessageBoxA(hwndDlg, "Could not create ssl_cert.pem", "Cause unknown", MB_ICONERROR);
        return false;
    }
    SetEndOfFile(hFile);
    DWORD written=0;
    if(!WriteFile(hFile, SSL_CERT, strlen(SSL_CERT), &written, 0))
    {
        MessageBoxA(hwndDlg, "Could not write ssl_cert.pem", "Cause unknown", MB_ICONERROR);
        CloseHandle(hFile);
        return false;
    }
    CloseHandle(hFile);
    const char *options[] = {"listening_ports", "443s", "ssl_certificate", "ssl_cert.pem", NULL};
    struct mg_callbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = begin_request_handler;
    ctx = mg_start(&callbacks, NULL, options);
    DeleteFileA("ssl_cert.pem");
    if(!ctx)
        return false;
    return true;
}

bool ServerIsStarted()
{
    if(!ctx)
        return false;
    return true;
}

void ServerStop()
{
    if(ctx)
        mg_stop(ctx);
}

const char* ServerLastError()
{
    return lastcry();
}
