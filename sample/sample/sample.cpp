#include <Windows.h>

#include "ootzwin/launcher.h"


int APIENTRY WinMain(_In_ HINSTANCE hInstance, 
                     _In_opt_ HINSTANCE hPrevInstance, 
                     _In_ LPSTR lpCmdLine, 
                     _In_ int nShowCmd)
{
    using namespace ootzwin;

    Launcher launcher("sampleApp", false, 800, 600);

    auto app = [](const float deltaTime) -> bool 
    {
        return true;
    };

    launcher.addApp(app);

    return launcher.run();
}