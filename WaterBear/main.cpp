#include "WaterBearApplication.h"

class RenderingApp : public WaterBear::Core::Application {
public:
    RenderingApp() {}
    
    void OnRun() {
        glClearColor(0.f, 0.3f, 0.5f, 1.0f);
    }
};

int main(void) {
    RenderingApp *app = new RenderingApp();
    app->Initialize(640, 480, "WaterBear");
    app->Run();
    app->Shutdown();
    
    return 0;
}
