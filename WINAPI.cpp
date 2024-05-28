// WINAPI.cpp : Defines the entry point for the application.
//
#include "framework.h"
#include "WINAPI.h"
#include <commdlg.h>
#include <string>
#include <Windows.h>

using namespace std;

#define MAX_LOADSTRING 100
#define MAX_TRAIL 20
#define P_WIDTH 15
#define P_HEIGHT 60
#define DIA 16


// Global Variables:

static DWORD rgbCurrent;
static int BALL_POS_X = 50;
static int BALL_POS_Y = 50;
int BALL_DIRECTION_X = 1;
int BALL_DIRECTION_Y = 1;
static int hitCount = 0;
static int loseCount = 0;
static int paddlePos=0;
RECT hitCountRect;
RECT loseCountRect;


bool bitmapClicked = false;
bool stretchChoosen = false;

static int windowHeight; 
static int windowWidth;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                   // the main window class name
WCHAR szBallClass[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];   
WCHAR szPaddleClass[MAX_LOADSTRING];
HWND hPaddle;
HWND hWnd;
HWND hBall;
HBITMAP hBitmap;

// Forward declarations of functions included in this code module:

ATOM                RegisterBall(HINSTANCE hInstance);
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                RegisterPaddle(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    WndProcBall(HWND hb, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK    WndProcPaddle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI, szWindowClass, MAX_LOADSTRING);
    LoadStringW(hInstance, 104, szBallClass, MAX_LOADSTRING);
    LoadStringW(hInstance, 105, szPaddleClass, MAX_LOADSTRING);
    RegisterBall(hInstance);
    RegisterPaddle(hInstance);

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(0, 255, 64));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

ATOM RegisterBall(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcBall;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 0, 0)));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szBallClass;
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

ATOM RegisterPaddle(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcPaddle;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(128, 128, 128)));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szPaddleClass;
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
  
   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   RECT rc;
   GetClientRect(hWnd, &rc);

   SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
   SetLayeredWindowAttributes(hWnd, 0, (255 * 80) / 100, LWA_ALPHA);
   
   hBall = CreateWindow(szBallClass, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
       BALL_POS_X, BALL_POS_Y, DIA, DIA, hWnd, nullptr, hInst, nullptr);

   hPaddle = CreateWindow(szPaddleClass, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
       rc.right - P_WIDTH , paddlePos, P_WIDTH, P_HEIGHT, hWnd, nullptr, hInst, nullptr);

   HRGN reg = CreateEllipticRgn(0, 0, DIA, DIA);
   SetWindowRgn(hBall, reg, true);
 
   if (!hWnd)
   {
      return FALSE;
   }

  
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProcBall(HWND hb, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: {

        SetTimer(hb, 1, 50, NULL);

        }
    break;
    
    case WM_TIMER: {
      
        if (wParam == 1) {
            BALL_POS_X += 5 * (BALL_DIRECTION_X);
            BALL_POS_Y += 5 * (BALL_DIRECTION_Y);
           
            if (BALL_POS_X + DIA >= windowWidth) {
                BALL_POS_X = windowWidth - DIA;
                BALL_DIRECTION_X = -BALL_DIRECTION_X;
                hitCount++;
                InvalidateRect(hWnd,NULL, TRUE);
               
            }

            else if (BALL_POS_X + DIA >= windowWidth - P_WIDTH && BALL_POS_Y<=paddlePos + P_HEIGHT && BALL_POS_Y>=paddlePos) {
                BALL_DIRECTION_X *= -1;
                ++loseCount;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            if (BALL_POS_Y + DIA >= windowHeight) {
                BALL_POS_Y = windowHeight - DIA;
                BALL_DIRECTION_Y = -BALL_DIRECTION_Y;

            }

            if (BALL_POS_X <= 0) {
                BALL_POS_X = 0;
                BALL_DIRECTION_X = -BALL_DIRECTION_X;

            }

            if (BALL_POS_Y <= 0) {
                BALL_POS_Y = 0;
                BALL_DIRECTION_Y = -BALL_DIRECTION_Y;
            }
         
        }
            
            MoveWindow(hb, BALL_POS_X, BALL_POS_Y, DIA, DIA, TRUE);
        }

 break;
            
   case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hb, &ps);
        // TODO: Add any drawing code that uses hdc here...

        EndPaint(hb, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hb, message, wParam, lParam);         

         }
    return 0;
}      
        
LRESULT CALLBACK WndProcPaddle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
       
    case WM_CREATE:
        paddlePos = (windowHeight - P_HEIGHT) / 2;
        break;
  
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: {
        RECT rc;
        GetClientRect(hWnd, &rc);
       
        int  screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int centerX = (screenWidth + 1) / 2;
        int centerY = (screenHeight + 1) / 2;
        windowWidth = rc.right - rc.left;
        windowHeight = rc.bottom - rc.top;

        int x = centerX - (windowWidth / 2) + 1;
        int y = centerY - (windowHeight / 2) + 1;

        hitCountRect.left = windowWidth/4+200;
        hitCountRect.top = windowHeight/2;
       
        loseCountRect.left = windowWidth*3/4+300;
        loseCountRect.top = windowHeight/2;
        
        MoveWindow(hWnd, x, y, windowWidth, windowHeight, FALSE);

    }
                
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:

            switch (wmId)
            {
            case ID_COLOR:{
                CHOOSECOLOR cc;
                COLORREF acrCustClr[16];
               
                ZeroMemory(&cc, sizeof(cc));
                cc.lStructSize = sizeof(cc);
                cc.hwndOwner = hWnd;
                HBRUSH hbrush;
                cc.lpCustColors = (LPDWORD)acrCustClr;
                cc.rgbResult = rgbCurrent;
                cc.Flags = CC_FULLOPEN | CC_RGBINIT;
                if (ChooseColor(&cc) == TRUE)
                {
                    hbrush = CreateSolidBrush(cc.rgbResult);
                    SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hbrush);
                    InvalidateRect(hWnd, NULL, TRUE);
                    hBitmap = NULL;
                    EnableMenuItem(GetMenu(hWnd), ID_TILE,MF_DISABLED );
                    EnableMenuItem(GetMenu(hWnd), ID_STRECH, MF_DISABLED);
                }

            }
                   break;

            case ID_NEW_GAME:
                BALL_POS_X = 50;
                BALL_POS_Y = 50;
                BALL_DIRECTION_X = 1;
                BALL_DIRECTION_Y = 1;
                hitCount = 0;
                loseCount = 0;              
                InvalidateRect(hWnd, NULL, TRUE);
                break;

            case ID_BITMAP:
                OPENFILENAME ofn;      
                char szFile[260];        
                HANDLE hf;        

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = (LPWSTR)szFile;

                ofn.lpstrFile[0] = '\0';
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = L".bmp extentions\0*.BMP\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn) == TRUE) {
                    hBitmap = (HBITMAP)LoadImage(NULL, (LPWSTR)szFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                    bitmapClicked = true;
                   
                        InvalidateRect(hWnd, NULL, TRUE);
                        CheckMenuItem(GetMenu(hWnd), ID_TILE, MF_CHECKED);
                        EnableMenuItem(GetMenu(hWnd), ID_TILE,MF_ENABLED);
                        EnableMenuItem(GetMenu(hWnd), ID_STRECH, MF_ENABLED);                  
                }
                break;
            
            case ID_STRECH: 
                
                if (bitmapClicked ) {
                  
                    stretchChoosen = true;
                    InvalidateRect(hWnd, NULL, TRUE);
                }

                break;
           
            case ID_TILE:

                if (bitmapClicked) {

                    stretchChoosen = false;
                    InvalidateRect(hWnd, NULL, TRUE);
                }
                break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_MOUSEMOVE: {
       
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hWnd, &pt);

        paddlePos = pt.y - P_HEIGHT / 2;

        if (paddlePos < 0) {
            paddlePos = 0;
        }
        else if (paddlePos > windowHeight - P_HEIGHT) {
            paddlePos = windowHeight - P_HEIGHT;
        }
       
        MoveWindow(hPaddle, windowWidth - P_WIDTH, paddlePos, P_WIDTH, P_HEIGHT, TRUE);
               
    }
    case WM_PAINT:
        {       
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
        
            RECT rc;
            GetClientRect(hWnd, &rc);
           
             if (hBitmap) {
                 HDC hdcM= CreateCompatibleDC(hdc);
              
                HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcM, hBitmap);
                BITMAP bm;
                GetObject(hBitmap, sizeof(bm), &bm);
                
                if(stretchChoosen){
                    CheckMenuItem(GetMenu(hWnd), ID_STRECH, MF_CHECKED);
                    CheckMenuItem(GetMenu(hWnd), ID_TILE, MF_UNCHECKED);
                    EnableMenuItem(GetMenu(hWnd), ID_STRECH, MF_ENABLED);
                    StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcM, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);                 
                }
                else {
                    CheckMenuItem(GetMenu(hWnd), ID_TILE, MF_CHECKED);
                    CheckMenuItem(GetMenu(hWnd), ID_STRECH, MF_UNCHECKED);
                    EnableMenuItem(GetMenu(hWnd), ID_TILE, MF_ENABLED);
                    BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdcM, 0, 0, SRCCOPY);                                
                }
                SelectObject(hdcM, hBitmapOld);
                DeleteDC(hdcM);

            }

             TCHAR countStr[32];
             TCHAR loseStr[32];

             HFONT font = CreateFont(-MulDiv(48, GetDeviceCaps(hdc, LOGPIXELSY), 72), 0, 0, 0,
                 FW_DEMIBOLD, false, FALSE, 0, EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                 DEFAULT_PITCH | FF_SWISS, _T("Verdana"));
             HFONT oldFont = (HFONT)SelectObject(hdc, font);
             SetBkMode(hdc, TRANSPARENT);
             SetTextColor(hdc, RGB(255, 20, 147));
             _stprintf_s(countStr, _T("%d"), loseCount);
             _stprintf_s(loseStr, _T("%d"), hitCount);
             TextOut(hdc, windowWidth / 4, 32, loseStr, (int)_tcslen(loseStr));
             TextOut(hdc, windowWidth*3/4, 32, countStr, (int)_tcslen(countStr));
           /*  DrawText(hdc, countStr, (int)_tcslen(countStr), &hitCountRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
             DrawText(hdc, loseStr, (int)_tcslen(loseStr), &loseCountRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);*/
             SelectObject(hdc, oldFont);

            EndPaint(hWnd, &ps);
               
        }
        break;
    case WM_GETMINMAXINFO: {
        MINMAXINFO* m = (MINMAXINFO*)lParam;
        m->ptMaxSize.x = m->ptMaxTrackSize.x = 500;
        m->ptMaxSize.y = m->ptMaxTrackSize.y = 350;
        m->ptMinTrackSize.x = 500;
        m->ptMinTrackSize.y = 350;

    }
     break;                    
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

