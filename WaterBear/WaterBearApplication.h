#include <string>
#include <memory>
#include "WaterBearGeometry.h"
#include <GLFW/glfw3.h>

namespace WaterBear {
namespace Core{
    
    class Application {
    public:
        Application() {}
        
        virtual void Initialize(int windowWidth, int windowHeight, const std::string& windowTitle, bool vSync = true);
        virtual void Shutdown();
        virtual void Run();
        
        virtual void OnRun() = 0;
        
        virtual ~Application() {};
        
    private:
        GLFWwindow *mWindow;
        SizeI mWindowSize;
    };
    
    
}
}
