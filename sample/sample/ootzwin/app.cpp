#include "app.h"


#include <algorithm>


#include "ootzwin/helper.h"


namespace ootzwin
{


bool App::_isInstantiated = false;


void App::removeUpdatable(const std::string& key)
{
    _updatables.erase(key);
}


int App::run()
{
    MSG msg;
    memset(&msg, 0, sizeof msg);

    while (true)
    {
        // handle window message
        if (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // escape loop
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        else
        {
            bool isSuccess = true;
            for (const auto& kv : _updatables)
            {
                const auto& updatable = kv.second;

                if (false == updatable ||
                    false == updatable->update(0.0f))
                {
                    isSuccess = false;
                }
            }

            if (false == isSuccess)
            {
                break;
            }
        }
    }


    // clear updatables
    _updatables.clear();


    // clear display settings
    if (_isFullScreen)
    {
        ChangeDisplaySettingsA(nullptr, 0);
    }


    // destroy window
    DestroyWindow(_hWnd);
    _hWnd = nullptr;


    // unregister instance
    UnregisterClassA(_appName.c_str(), _hInstance);
    _hInstance = nullptr;


    // exit app
    return 0;
}


LRESULT CALLBACK App::WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    switch (uMessage)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }

        default:
        {
            return DefWindowProc(hWnd, uMessage, wParam, lParam);
        }
    }
}


App::App(const char* appName, 
         const bool isFullScreen, 
         const int width, const int height)
    : _hWnd(nullptr)
    , _hInstance(nullptr)
    , _appName()
    , _updatables()
{
    // check duplicate
    if (false == _isInstantiated)
    {
        _isInstantiated = true;
    }
    else
    {
        exitError("Launcher is duplicated.");
    }


    // set hInstance
    HINSTANCE hInstance = GetModuleHandleA(nullptr);
    if (nullptr == hInstance)
    {
        exitSystemError("Launcher initialization");
    }

    _hInstance = hInstance;


    // set app name
    _appName = appName;


    // register window class
    WNDCLASSEXA wc;
    const size_t wcSize = sizeof wc;
    memset(&wc, 0, wcSize);

    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInstance;
    wc.hIcon = LoadIconA(nullptr, IDI_WINLOGO);
    wc.hIconSm = wc.hIcon;
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = appName;
    wc.cbSize = wcSize;

    RegisterClassExA(&wc);


    // read monitor resolution
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int posX = 0;
    int posY = 0;

    if (isFullScreen)
    {
        // set resolution to monitor resolution
        DEVMODEA dmScreenSettings;
        const size_t dmSize = sizeof dmScreenSettings;
        memset(&dmScreenSettings, 0, dmSize);

        dmScreenSettings.dmSize = dmSize;
        dmScreenSettings.dmPelsWidth = static_cast<DWORD>(screenWidth);
        dmScreenSettings.dmPelsHeight = static_cast<DWORD>(screenHeight);
        dmScreenSettings.dmBitsPerPel = 32;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        ChangeDisplaySettingsA(&dmScreenSettings, CDS_FULLSCREEN);
    }
    else
    {
        // set center
        posX = (screenWidth - width) / 2;
        posY = (screenHeight - height) / 2;

        screenWidth = width;
        screenHeight = height;
    }


    // set "is full screen"
    _isFullScreen = isFullScreen;


    // create window handle
    HWND hWnd = CreateWindowExA(
        WS_EX_APPWINDOW,
        appName, appName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        posX, posY, screenWidth, screenHeight, 
        nullptr, nullptr, _hInstance, nullptr);

    if (nullptr == hWnd)
    {
        exitSystemError("Launcher initialization");
    }


    // set "window handle"
    _hWnd = hWnd;


    // set focus
    ShowWindow(_hWnd, SW_SHOW);
    SetForegroundWindow(_hWnd);
    SetFocus(_hWnd);
}


App::~App()
{
    _isInstantiated = false;
}


void App::exitError(const char* reason)
{
    MessageBoxA(nullptr, reason, "Error", MB_OK);
    ExitProcess(0);
}


void App::exitSystemError(const char* functionName)
{
    const DWORD dwErrorCode = GetLastError();

    std::string displayBuf(functionName);
    displayBuf += " failed with error ";
    displayBuf += ErrorCodeToString(dwErrorCode);

    MessageBoxA(nullptr, displayBuf.c_str(), "Error", MB_OK);
    ExitProcess(dwErrorCode);
}


} // namespace ootzwin