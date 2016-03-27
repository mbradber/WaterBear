#include <string>
#include <memory>
#include "WaterBearGeometry.h"
#include <GLFW/glfw3.h>

namespace WaterBear {
namespace Core{
    
    class Application {
    public:
        Application() {}
        
        virtual void Run(Application *);
        
        
        virtual ~Application() {};
        
    protected:
        static Application *sApp;
        
        virtual void OnInit();
        virtual void OnStart();
        virtual void OnShutDown();
        virtual void OnRender(double currentTime) = 0;
        virtual void OnWindowResize(int w, int h);
        
        struct AppConfig {
            std::string windowTitle;
            int windowWidth;
            int windowHeight;
            int samples;
            bool vsync;
            
            AppConfig():vsync(false) {}
        };
        
        AppConfig config;
        
    private:
        GLFWwindow *mWindow;
        
        static void glfw_onResize(GLFWwindow *window, int w, int h);
    };
    
    
}
}

#define DECLARE_MAIN(a)                             \
int main(int argc, const char ** argv)              \
{                                                   \
    a *app = new a;                                 \
    app->Run(app);                                  \
    delete app;                                     \
    return 0;                                       \
}
