#include <Windows.h>


#include "ootzwin/app.h"

class Foo : public ootzwin::Updatable
{
public:
    Foo() {}
    virtual ~Foo() {}

    // Inherited via Updatable
    virtual bool update(const float deltaTime) override
    {
        return true;
    }
};


int APIENTRY WinMain(_In_ HINSTANCE hInstance, 
                     _In_opt_ HINSTANCE hPrevInstance, 
                     _In_ LPSTR lpCmdLine, 
                     _In_ int nShowCmd)
{
    using namespace ootzwin;

    App app("sampleApp", false, 800, 600);

    app.addUpdatable("Foo", new Foo());

    return app.run();
}