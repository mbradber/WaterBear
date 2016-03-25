#include "ext/gl/gl3w.h"

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

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
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        
#if WATERBEAR_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
        
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        // todo: update this to be an argument for when we support multi-sampling
        glfwWindowHint(GLFW_SAMPLES, 0);
        
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
        
        gl3wInit();
        
        OnStart();
    }
    
    void Application::Shutdown() {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
        
        OnShutdown();
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
