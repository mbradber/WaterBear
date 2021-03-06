#include "ext/gl/gl3w.h"

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include "WaterBearApplication.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>

namespace WaterBear {
namespace Core {
    
    Application *Application::sApp = 0;
    
    void Application::Run(Application *app) {
        sApp = app;
        
        if(!glfwInit()) {
            exit(EXIT_FAILURE);
        }
        
        OnInit();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        
#if WATERBEAR_DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
        
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        // todo: update this to be an argument for when we support multi-sampling
        glfwWindowHint(GLFW_SAMPLES, config.samples);
        
        mWindow = glfwCreateWindow(config.windowWidth, config.windowHeight, config.windowTitle.c_str(), nullptr, nullptr);
        
        if(!mWindow) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        
        glfwMakeContextCurrent(mWindow);
        
        glfwSetWindowSizeCallback(mWindow, glfw_onResize);
        
        if(config.vsync) {
            glfwSwapInterval(1);
        }
        
        gl3wInit();
        
        OnStart();
        
        bool running = true;
        do {
            OnRender(glfwGetTime());
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
            
            running &= (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
            running &= (glfwWindowShouldClose(mWindow) != GL_TRUE);
        } while(running);
        
        OnShutDown();
    }
    
    void Application::OnInit() {
        config.windowTitle = "WaterBear";
        config.windowWidth = 800;
        config.windowHeight = 600;
        config.samples = 0;
        config.vsync = true;
    }
    
    void Application::OnStart() {
        
    }
    
    void Application::OnRender(double currentTime) {
        
    }
    
    void Application::OnWindowResize(int w, int h) {
        config.windowWidth = w;
        config.windowHeight = h;
    }
    
    void Application::OnShutDown() {
        glfwDestroyWindow(mWindow);
        glfwTerminate();
    }
    
    void Application::glfw_onResize(GLFWwindow *window, int w, int h) {
        sApp->OnWindowResize(w, h);
    }
    
    void Application::CompileShader(GLuint shader, const std::string& shaderName) {
        // load shader text from file
        char *rootDir = GetRootDir();
        if(!rootDir) {
            return;
        }
        
        std::string shaderPath = std::string(rootDir) + "shaders/" + shaderName;
        std::string shaderString;
        std::ifstream sourceFile(shaderPath.c_str());
        
        if(sourceFile) {
            shaderString.assign( ( std::istreambuf_iterator< char >( sourceFile ) ), std::istreambuf_iterator< char >() );
        }
        else {
            std::cout << "Error loading file contents from " << shaderPath << std::endl;
            return;
        }
        
        const GLchar* shaderSource = shaderString.c_str();
        
        // load/compile shader
        glShaderSource(shader, 1, (const GLchar**)&shaderSource, NULL);
        glCompileShader(shader);
        
        GLint isCompiled = 0;
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        
        if(isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            
            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
            std::string errorStr(errorLog.begin(), errorLog.end());
            
            std::cout << errorStr << std::endl;
        }
    }
    
    char* Application::GetRootDir() {
        char *rootDir = getenv("WATERBEAR_DIR");
        if(!rootDir) {
            std::cout << "Please define WATERBEAR_DIR" << std::endl;
            rootDir = NULL;
        }
        
        return rootDir;
    }
        
}
    
}
