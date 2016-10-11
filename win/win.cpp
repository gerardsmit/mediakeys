//////////////////////////////////////////
//                                      //
// SIMPLEST WINDOWS PROGRAM             //
//                                      //
// You found this at bobobobo's weblog, //
// http://bobobobo.wordpress.com        //
//                                      //
// Creation date:  Jan 31/08            //
// Last modified:  Feb 9/08             //
//                                      //
//////////////////////////////////////////

#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <windows.h>
#include <stdio.h>
#include <string.h>

// Function prototypes.
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow );

typedef BOOL (CALLBACK* SetMMShellHookType)(HWND);
typedef BOOL (CALLBACK* UnSetMMShellHookType)(HWND);

HINSTANCE utilHandle;
HINSTANCE dllHandle;

int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR szCmdLine,
                    int iCmdShow )
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;  // ignore for now
    wc.cbWndExtra = 0;  // ignore for now
    wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );   // I want the window to have a white background
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );            // I want it to have an arrow for a cursor
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );        // I want it to have that envelope like icon
    wc.hInstance = hInstance;           // INSTANCE HANDLE -- see the GLOSSARY PART of this file for an explanation of what HINSTANCE is
    wc.lpfnWndProc = WndProc;           // Give name of WndProc function here.
    wc.lpszClassName = TEXT("Philip");
    wc.lpszMenuName = 0;    // no menu - ignore
    RegisterClass( &wc );
    HWND hwnd = CreateWindowEx(0, TEXT("Philip"), "dummy_name", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
    ShowWindow(hwnd, SW_HIDE);
    UpdateWindow(hwnd);

    dllHandle = LoadLibrary("MMShellHook.dll");
    if (dllHandle != NULL) {

        MINIMIZEDMETRICS mmm;
        mmm.cbSize = sizeof( MINIMIZEDMETRICS );
        SystemParametersInfo( SPI_SETMINIMIZEDMETRICS, sizeof( MINIMIZEDMETRICS ), &mmm, 0 );

        SetMMShellHookType SetMMShellHook = (SetMMShellHookType) GetProcAddress(dllHandle, "SetMMShellHook");
        int statshook = SetMMShellHook(hwnd);
    }

    setvbuf(stdout, NULL, _IONBF, 0);
    printf("{\"action\":\"connected\"}\n");

    MSG msg;
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    return msg.wParam;    // return from WinMain
}

LRESULT CALLBACK WndProc(   HWND hwnd,      // "handle" to the window that this message is for
                            UINT message,   // TYPE of message (e.g. WM_PAINT is a message asking to paint the window)
                            WPARAM wparam,  // information about the actual message
                            LPARAM lparam ) // MORE info about the message
{
    short AppCommand = 0;

    switch( message )
    {
    case WM_CREATE:
        return 0;

    case WM_PAINT:
        return 0;

    case WM_APPCOMMAND:
        AppCommand = GET_APPCOMMAND_LPARAM(lparam);
        switch (AppCommand)
        {
        case APPCOMMAND_MEDIA_NEXTTRACK:
            printf("{\"action\":\"next\"}\n");
            break;
        case APPCOMMAND_MEDIA_PLAY_PAUSE:
            printf("{\"action\":\"play\"}\n");
            break;
        case APPCOMMAND_MEDIA_PREVIOUSTRACK:
            printf("{\"action\":\"back\"}\n");
            break;
        case APPCOMMAND_MEDIA_STOP:
            printf("{\"action\":\"stop\"}\n");
            break;
        case APPCOMMAND_BROWSER_HOME:
            printf("{\"action\":\"home\"}\n");
            break;
        case APPCOMMAND_LAUNCH_MEDIA_SELECT:
            printf("{\"action\":\"media\"}\n");
            break;
        case APPCOMMAND_MEDIA_REWIND:
            printf("{\"action\":\"rewind\"}\n");
            break;
        case APPCOMMAND_MEDIA_FAST_FORWARD:
            printf("{\"action\":\"forward\"}\n");
            break;
        }
        return 0;


    case WM_DESTROY:
        if (dllHandle != NULL) {
            UnSetMMShellHookType UnSetMMShellHook = (UnSetMMShellHookType) GetProcAddress(dllHandle, "UnSetMMShellHook");
            UnSetMMShellHook(hwnd);
        }

        PostQuitMessage( 0 ) ;
        return 0;
    }

    return DefWindowProc( hwnd, message, wparam, lparam );
}