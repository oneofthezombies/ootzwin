#pragma once


#include <cassert> // for assert
#include <functional> // for std::function


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace ootzwin
{


class Launcher
{
public:

    Launcher(const char* appName, 
             const bool isFullScreen, 
             const int width, const int height);

    ~Launcher();

    /* Noncopyconstructable */
    Launcher(const Launcher&) = delete;

    /* Nonmoveconstructable */
    Launcher(Launcher&&) = delete;

    /* Noncopyassignable */
    Launcher& operator=(const Launcher&) = delete;

    /* Nonmoveassignable */
    Launcher& operator=(Launcher&&) = delete;


    void addApp(const std::function<bool(float)>& app)
    {
        assert(app && "app is empty");
        _app = app;
    }

    int run();


    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);


private:

    void exitError(const char* reason);
    void exitSystemError(const char* functionName);


    HWND _hWnd;
    HINSTANCE _hInstance;
    std::string _appName;
    bool _isFullScreen;


    std::function<bool(float)> _app;


    static bool _isInstantiated;
};


} // namespace ootzwin