#include "ext/gl/gl3w.h"
#include "WaterBearApplication.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "ext/math/vmath.h"

class RenderingApp : public WaterBear::Core::Application {
public:
    
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
        CompileShader(fs, "spinny_cube_fs.glsl");
        
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(vs, "spinny_cube_vs.glsl");
        
        glAttachShader(mProgram, fs);
        glAttachShader(mProgram, vs);
        
        glLinkProgram(mProgram);
        glDeleteShader(fs);
        glDeleteShader(vs);
        
        mModelViewLoc = glGetUniformLocation(mProgram, "mv_matrix");
        mProjLoc = glGetUniformLocation(mProgram, "proj_matrix");
        
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);
        
        static const GLushort vertIndices[] =
        {
            0, 1, 2,
            2, 1, 3,
            2, 3, 4,
            4, 3, 5,
            4, 5, 6,
            6, 5, 7,
            6, 7, 0,
            0, 7, 1,
            6, 0, 2,
            2, 4, 6,
            7, 5, 3,
            7, 3, 1
        };
        
        static const GLfloat vertPositions[] =
        {
            -0.25f, -0.25f, -0.25f,
            -0.25f,  0.25f, -0.25f,
            0.25f, -0.25f, -0.25f,
            0.25f,  0.25f, -0.25f,
            0.25f, -0.25f,  0.25f,
            0.25f,  0.25f,  0.25f,
            -0.25f, -0.25f,  0.25f,
            -0.25f,  0.25f,  0.25f,
        };
        
        glGenBuffers (1, &mPositionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertPositions), vertPositions, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
        
        glGenBuffers(1, &mIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertIndices), vertIndices, GL_STATIC_DRAW);
        
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
    
    void OnRender(double currentTime) {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        static const float one = 1.0f;
        
        glViewport(0, 0, config.windowWidth, config.windowHeight);
        glClearBufferfv(GL_COLOR, 0, green);
        glClearBufferfv(GL_DEPTH, 0, &one);
        
        glUseProgram(mProgram);
        
        // projection matrix
        vmath::mat4 projMatrix = vmath::perspective(50.0f, (float)config.windowWidth / (float)config.windowHeight, 0.1f, 1000.0f);
        glUniformMatrix4fv(mProjLoc, 1, GL_FALSE, projMatrix);
        
        // model-view matrix
        float f = (float)currentTime * 0.3f;
        vmath::mat4 mvMatrix = vmath::translate(0.0f, 0.0f, -4.0f) *
        vmath::translate(sinf(2.1f * f) * 0.5f,
                         cosf(1.7f * f) * 0.5f,
                         sinf(1.3f * f) * cosf(1.5f * f) * 2.0f) *
        vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
        vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
        
        glUniformMatrix4fv(mModelViewLoc, 1, GL_FALSE, mvMatrix);
        
        // draw
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }
    
    void OnShutdown() {
        Application::OnShutDown();
        
        glDeleteVertexArrays(1, &mVao);
        glDeleteProgram(mProgram);
        
        glDeleteBuffers(1, &mPositionBuffer);
        glDeleteBuffers(1, &mIndexBuffer);
    }
    
    void OnWindowResize(int w, int h) {
        Application::OnWindowResize(w, h);
    }
    
private:
    GLuint mProgram;
    GLuint mVao;
    GLuint mModelViewLoc;
    GLuint mProjLoc;
    GLuint mPositionBuffer;
    GLuint mIndexBuffer;
};

DECLARE_MAIN(RenderingApp)
