#include "WaterBearApplication.h"
#include <stdlib.h>
#include <stdio.h>

namespace WaterBear {
namespace Core {
    
    
    void Application::Initialize(int windowWidth, int windowHeight, const std::string& windowTitle, bool vSync)
    {
        if(!glfwInit()) {
            exit(EXIT_FAILURE);
        }
        
        mWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
        mWindowSize = SizeI(windowWidth, windowHeight);
        
        if(!mWindow) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        
        glfwMakeContextCurrent(mWindow);
        
        if(vSync) {
            glfwSwapInterval(1);
        }
    }
    
    void Application::Shutdown() {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
    }
    
    void Application::Run() {
        while(!glfwWindowShouldClose(mWindow)) {
            int windowWidth, windowHeight;
            glfwGetFramebufferSize(mWindow, &windowWidth, &windowHeight);
            
            glViewport(0, 0, windowWidth, windowHeight);
            
            glClear(GL_COLOR_BUFFER_BIT);
            
            OnRun();
            
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
        }
    }
        
}
    
}
