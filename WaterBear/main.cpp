#include "ext/gl/gl3w.h"
#include "WaterBearApplication.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

class RenderingApp : public WaterBear::Core::Application {
public:
    RenderingApp() {}
    
    void CompileShader(GLuint shader, std::string shaderName) {
        // load shader text from file
        char *rootDir = getenv("WATERBEAR_DIR");
        if(!rootDir) {
            std::cout << "Please define WATERBEAR_DIR" << std::endl;
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
            
            glDeleteShader(shader); // Don't leak the shader.
            return;
        }
    }
    
    void OnStart() {
        mProgram = glCreateProgram();
        
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        CompileShader(fs, "single_tri_fs.glsl");
        
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(vs, "single_tri_vs.glsl");
        
        glAttachShader(mProgram, fs);
        glAttachShader(mProgram, vs);
        
        glLinkProgram(mProgram);
//        glDeleteShader(fs);
//        glDeleteShader(vs);
        
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);
    }
    
    void OnRun() {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, green);
        
        glUseProgram(mProgram);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    
    void OnShutdown() {
        glDeleteVertexArrays(1, &mVao);
        glDeleteProgram(mProgram);
    }
    
private:
    GLuint mProgram;
    GLuint mVao;
};

int main(void) {
    RenderingApp *app = new RenderingApp();
    app->Initialize(640, 480, "WaterBear");
    app->Run();
    app->Shutdown();
    
    return 0;
}
