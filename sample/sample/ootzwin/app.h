#pragma once


#include <memory> // for std::unique_ptr
#include <unordered_map> // for std::unordered_map


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


namespace ootzwin
{


/* interface Updatable */
class Updatable
{
public:
    virtual ~Updatable() = default;


    /* @Parameter
     * In - delta time (unit: seconds) e.g. 0.001f
     * Out - success(true) or failure(false), if you want to exit the program, return false.
     */
    virtual bool update(const float deltaTime) = 0;
};


class App
{
    using UpdatableKeyType = std::string;


public:

    App(const char* appName, 
        const bool isFullScreen, 
        const int width, const int height);

    ~App();

    /* Noncopyconstructible */
    App(const App&) = delete;

    /* Nonmoveconstructible */
    App(App&&) = delete;

    /* Noncopyassignable */
    App& operator=(const App&) = delete;

    /* Nonmoveassignable */
    App& operator=(App&&) = delete;


    template<typename T, 
        typename Deleter = std::default_delete<T>,
        std::enable_if_t<std::is_base_of_v<Updatable, T>, int> = 0>
    bool addUpdatable(const std::string& key, T* updatable, Deleter&& deleter = Deleter())
    {
        // argument is null
        if (nullptr == updatable)
        {
            return false;
        }

        // the key is in use
        const auto result = _updatables.find(key);
        if (result != _updatables.end())
        {
            return false;
        }

        _updatables.emplace(key, std::unique_ptr<T>(updatable, deleter));
        return true;
    }


    void removeUpdatable(const std::string& key);


    int run();


    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);


private:

    void exitError(const char* reason);
    void exitSystemError(const char* functionName);


    HWND _hWnd;
    HINSTANCE _hInstance;
    std::string _appName;
    bool _isFullScreen;
    std::unordered_map<UpdatableKeyType, std::unique_ptr<Updatable>> _updatables;


    // use to allow only one instance
    static bool _isInstantiated;
};


} // namespace ootzwin