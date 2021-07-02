#include "framework.h"
#include "HOME_LAB.h"
#include <windows.h>
#include <commdlg.h>
#include <wingdi.h>

void RegisterPanel();
COLORREF ShowColorDialog(HWND hwnd);
#define MAX_LOADSTRING 100

COLORREF gColor = RGB(255, 255, 255);

typedef struct
{
    UINT MessageId;
    WPARAM wParam;
    LPARAM lParam;
} SMessage, * PSMessage;



HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                 
WCHAR szWindowClass[MAX_LOADSTRING];         
WCHAR childchar[] = L"CHILD";
WCHAR color[] = L"COLOR";

int missed = 0;
int wrong = 0;


bool PauzaFocus = false;
bool PauzaEsc = false;
int GameCounter = 1;

static int licznik = 0;

ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                MyRegisterClass_Child(HINSTANCE hInstance);
ATOM                MyRegisterClass_Color(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChildProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ColorProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PanelProc(HWND, UINT, WPARAM, LPARAM);


BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    PSMessage psMessage = (PSMessage)lParam;
    SendMessage(hwnd, psMessage->MessageId, psMessage->wParam, psMessage->lParam);
    return TRUE;
}
void SendMessageToAllChildren(HWND hParendWnd, UINT MessageId, WPARAM wParam, LPARAM lParam)
{
    SMessage sMessage;
    sMessage.MessageId = MessageId;
    sMessage.wParam = wParam;
    sMessage.lParam = lParam;

    EnumChildWindows(hParendWnd, EnumChildProc, (LPARAM)&sMessage);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
   

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_HOMELAB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    MyRegisterClass_Child(hInstance);
    //MyRegisterClass_Color(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMELAB));

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

    return (int)msg.wParam;
}
int dolny = 0;
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_HOMELAB));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(4);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HOMELAB);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
ATOM MyRegisterClass_Child(HINSTANCE hInstance)
{
    WNDCLASSEX child;

    child.cbSize = sizeof(WNDCLASSEX);

    child.style = CS_HREDRAW | CS_VREDRAW;
    child.lpfnWndProc = ChildProc;
    child.cbClsExtra = 0;
    child.cbWndExtra = 0;
    child.hInstance = hInstance;
    child.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_HOMELAB));
    child.hCursor = LoadCursor(nullptr, IDC_ARROW);
    child.hbrBackground = (HBRUSH)(10);
    child.lpszMenuName = MAKEINTRESOURCEW(IDC_HOMELAB);
    child.lpszClassName = childchar;
    child.hIconSm = LoadIcon(child.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&child);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;
    int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hWnd = CreateWindowW(szWindowClass, L"KeyboardMasterWinAPI_LAB", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        ScreenWidth / 4, ScreenHeight / 4, ScreenWidth / 2, ScreenHeight / 2, nullptr, nullptr, hInstance, nullptr);
    SetWindowPos(hWnd, HWND_TOPMOST, ScreenWidth / 4, ScreenHeight / 4, ScreenWidth / 2, ScreenHeight / 2, SWP_NOSIZE);
    if (!hWnd)
    {
        return FALSE;
    }
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
void create_child(HWND hWnd)
{
    if (licznik < 100)
    {
        licznik++;
        RECT Child;
        GetWindowRect(hWnd, &Child);
        int rand_x = rand() % (Child.right - Child.left);
        if (rand_x > Child.right + 25) {
            rand_x -= 25;
        }
        HWND hWnd_child = CreateWindowW(childchar, L"Dziecko", WS_CHILD | WS_VISIBLE, rand_x, 0, 25, 25, hWnd, nullptr, hInst, nullptr);
        UpdateWindow(hWnd_child);
        ShowWindow(hWnd_child, SW_SHOW);
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 300 + rand() % 700, NULL);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
        case ID_FILE_NEWGAME:
            wParam = VK_F2;
            SendMessageToAllChildren(hWnd, WM_KEYDOWN, wParam, lParam);
            break;
        case ID_FILE_PAUSE:
            PauzaEsc = !PauzaEsc;
            break;
        case ID_COLORCTRL:
        {
            CHOOSECOLORW cc;
            static COLORREF acrCust[16];
            static HBRUSH hBrush;
            ZeroMemory(&cc, sizeof(cc));
            cc.lStructSize = sizeof(cc);
            cc.lpCustColors = (LPDWORD)acrCust;
            cc.Flags = CC_FULLOPEN;
            if (ChooseColorW(&cc) == TRUE) {
                hBrush = CreateSolidBrush(cc.rgbResult);
                SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
                InvalidateRect(hWnd, nullptr, TRUE);
            }
            break;
            }
        case ID_BITMAP:
        {
        
            HBITMAP hBitmap01;
            wchar_t fname[MAX_PATH];
            HBRUSH Background;
            OPENFILENAME ofn;
            for (int i = 0; i < MAX_PATH; i++) {
                fname[i] = '\0';
            }
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
            //ofn.lpstrFilter = _T("(*.bmp) Bitmap Image File\0*.bmp\0");
            ofn.lpstrFilter = (LPCWSTR)("Bitmap Image File\0*.bmp");
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrFile = fname;
            ofn.lpstrTitle = _T("Select a File, yo!");
            ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
            if (GetOpenFileNameA((LPOPENFILENAMEA)&ofn) == TRUE)
            {
                hBitmap01 = (HBITMAP)LoadImage(NULL, L"C:/Users/Szymon/Downloads/HOME_LAB/HOME_LAB/HOME_LAB/brick.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                Background = CreatePatternBrush(hBitmap01);
                SetClassLongPtrW(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)Background);
                InvalidateRect(hWnd, nullptr, TRUE);
            }
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_TIMER:
    {
        TCHAR s[256];
        _stprintf_s(s, 256, _T("Keyboard Master WinApi_2021, Missed: %d Wrong: %d"), missed, wrong);
        SetWindowText(hWnd, s);
        if (GetActiveWindow() != hWnd) {
            PauzaFocus = true;
            PauzaEsc = false;
        }
        if (GetActiveWindow() == hWnd) {
            PauzaFocus = false;
        }
        if (!PauzaEsc && !PauzaFocus) {
            create_child(hWnd);
        }
    }
    break;
    case WM_KEYUP:
    {
        if (wParam == VK_SPACE)
        {
            SendMessageToAllChildren(hWnd, WM_KEYDOWN, wParam, lParam);
        }
        if (wParam == VK_F2) {
            SendMessageToAllChildren(hWnd, WM_KEYDOWN, wParam, lParam);
            GameCounter++;
            missed = 0;
           
        }
    }
    break;
    
    case WM_RBUTTONDOWN:
    {
        POINT point;
         point.x = LOWORD(lParam);
        point.y = HIWORD(lParam);
        HMENU hMenu;
        hMenu = CreatePopupMenu();
        ClientToScreen(hWnd, &point);

        AppendMenuW(hMenu, MF_STRING, ID_BITMAP, L"&Bitmap...  Ctrl + B");
        AppendMenuW(hMenu, MF_STRING, ID_TILE, L"&Tile  Ctrl + T");
        AppendMenuW(hMenu, MF_STRING, ID_STREACH, L"&Streach   Ctrl + S");
        AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        AppendMenuW(hMenu, MF_STRING, ID_COLORCTRL, L"Color Ctrl + C");

        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
        DestroyMenu(hMenu);
        break;
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
LRESULT CALLBACK ChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int stepSize = 10;
    
    int speed = 1;
    switch (message)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, 7, 1 + rand() % 20, NULL);
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_FILE_NEWGAME:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_TIMER:
    {
        if (wParam == 7) // check timer id
        {
            RECT rect, parent_rect;
            // get the center of the work area of the system
            // get current size of the window
            GetWindowRect(hWnd, &rect);
            GetWindowRect(GetParent(hWnd), &parent_rect);
            if (rect.bottom >= parent_rect.bottom || rect.right > parent_rect.right || rect.left < parent_rect.left)
            {
                licznik--;
                missed++;
                dolny = 0;
                DestroyWindow(hWnd);
            }
            else if (rect.bottom > dolny)
            {
                dolny = rect.bottom;
            }
            MapWindowPoints(HWND_DESKTOP, GetParent(hWnd), (LPPOINT)&rect, 2);
            int x = rect.left;
            int y = rect.top;
            // modify size of the window
            // currentSize += stepSize;
            if (!PauzaEsc && !PauzaFocus)
            {
                MoveWindow(hWnd, x, y + speed, 25, 25, TRUE);
            }
            else
            {
                MoveWindow(hWnd, x, y, 25, 25, TRUE);
            }
        }
    }
    break;
    case WM_KEYDOWN:
    {
        if (wParam == VK_SPACE)
        {
            RECT rect;
            GetWindowRect(hWnd, &rect);
            if (rect.bottom >= dolny && dolny > 0)
            {
                licznik--;
                dolny = 0;
                DestroyWindow(hWnd);
            }
        }
        if (wParam == VK_F2) {
            DestroyWindow(hWnd);
        }
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
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
//LRESULT CALLBACK ColorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//
//    static HWND hwndPanel;
//
//    switch (msg) {
//
//    case WM_CREATE:
//    {
//        CreateWindowW(L"button", L"Color",
//            WS_VISIBLE | WS_CHILD,
//            20, 30, 80, 25,
//            hwnd, (HMENU)1, NULL, NULL);
//
//        RegisterPanel();
//        hwndPanel = CreateWindowW(L"Panel", NULL,
//            WS_CHILD | WS_VISIBLE,
//            130, 30, 80, 80, hwnd, (HMENU)2, NULL, NULL);
//        break;
//    }
//
//    case WM_COMMAND:
//    {
//        gColor = ShowColorDialog(hwnd);
//        InvalidateRect(hwndPanel, NULL, TRUE);
//        break;
//    }
//
//    case WM_DESTROY:
//    {
//        PostQuitMessage(0);
//        break;
//    }
//    }
//
//    return DefWindowProcW(hwnd, msg, wParam, lParam);
//}
//
//LRESULT CALLBACK PanelProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//
//    HDC hdc;
//    PAINTSTRUCT ps;
//    RECT rect;
//
//    switch (msg) {
//
//    case WM_PAINT:
//    {
//        GetClientRect(hwnd, &rect);
//        hdc = BeginPaint(hwnd, &ps);
//        SetBkColor(hdc, gColor);
//        ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, L"", 0, NULL);
//        EndPaint(hwnd, &ps);
//        break;
//    }
//    }
//
//    return DefWindowProc(hwnd, msg, wParam, lParam);
//}
//
//
//COLORREF ShowColorDialog(HWND hwnd) {
//
//    CHOOSECOLOR cc;
//    static COLORREF crCustClr[16];
//    ZeroMemory(&cc, sizeof(cc));
//    cc.lStructSize = sizeof(cc);
//    cc.hwndOwner = hwnd;
//    cc.lpCustColors = (LPDWORD)crCustClr;
//    cc.rgbResult = RGB(0, 255, 0);
//    //cc.Flags = CC_FULLOPEN | CC_RGBINIT;
//    //ChooseColor(&cc);
//
//    return cc.rgbResult;
//}
//
//void RegisterPanel() {
//
//    WNDCLASSW rwc = { 0 };
//    rwc.lpszClassName = L"Panel";
//    rwc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
//    rwc.lpfnWndProc = PanelProc;
//    RegisterClassW(&rwc);
//}
//void OnBnClickedColorBtn()
//{
//    // TODO: Add your control notification handler code here
//    CColorDialog dlg;
//
//    COLORREF cRef[] = { RGB(0,     5,   5),
//                    RGB(0,    15,  55),
//                RGB(0,    25, 155),
//                RGB(0,    35, 255),
//                    RGB(10,    0,   5),
//                RGB(10,   20,  55),
//                RGB(10,   40, 155),
//                    RGB(10,   60, 255),
//                RGB(100,   5,   5),
//                RGB(100,  25,  55),
//                    RGB(100,  50, 155),
//                RGB(100, 125, 255),
//                RGB(200, 120,   5),
//                    RGB(200, 150,  55),
//                RGB(200, 200, 155),
//                RGB(200, 250, 255) };
//    dlg.m_cc.lpCustColors = cRef;
//    dlg.m_cc.Flags |= CC_FULLOPEN;
//
//    if (dlg.DoModal() == IDOK) {
//        return dlg.GetColor();
//    }
//       
//}