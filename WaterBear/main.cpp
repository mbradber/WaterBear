#include "ext/gl/gl3w.h"
#include "WaterBearApplication.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "ext/math/vmath.h"
#include "ext/sb/sb7ktx.h"
#include "WaterBearGeometry.h"
#include "WaterBearTriangleRO.h"
#include "WaterBearCubeRO.h"

class RenderingApp : public WaterBear::Core::Application {
public:
    
    void OnStart() {
        mTriangle = new WaterBear::Renderables::TriangleRO("../assets/ktx/pattern1.ktx");
        mCube = new WaterBear::Renderables::CubeRO("../assets/ktx/baboon.ktx");
        
        glGenFramebuffers(1, &mFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
        
        glGenTextures(1, &mFboColorTexture);
        glBindTexture(GL_TEXTURE_2D, mFboColorTexture);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glGenTextures(1, &mFboDepthTexture);
        glBindTexture(GL_TEXTURE_2D, mFboDepthTexture);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 512, 512);
        
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mFboColorTexture, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mFboDepthTexture, 0);
        
        static const GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, drawBuffers);
        
        // rendering settings
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
    
    void OnRender(double currentTime) {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        static const GLfloat blue[] = { 0.0f, 0.20f, 0.5f, 1.0f };
        static const float one = 1.0f;
        
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
        glViewport(0, 0, 512, 512);
        
        glClearBufferfv(GL_COLOR, 0, green);
        glClearBufferfv(GL_DEPTH, 0, &one);
        
        // draw tri
        vmath::mat4 mvMatrixTri = vmath::translate(0.0f, 0.0f, -7.0f);
        vmath::mat4 projMatrixTri = vmath::perspective(50.0f, (float)config.windowWidth / (float)config.windowHeight, 0.1f, 1000.0f);
        
        mTriangle->Render(currentTime, mvMatrixTri, projMatrixTri);
        
        // restore default framebuffer & viewport
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glViewport(0, 0, config.windowWidth, config.windowHeight);
        glClearBufferfv(GL_COLOR, 0, blue);
        glClearBufferfv(GL_DEPTH, 0, &one);
        
        // draw cube
        vmath::mat4 projMatrixCube = vmath::perspective(50.0f, (float)config.windowWidth / (float)config.windowHeight, 0.1f, 1000.0f);
        
        vmath::mat4 mvMatrixCube = vmath::translate(0.0f, 0.0f, -7.0f) *
            vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
            vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
        
        mCube->Render(currentTime, mvMatrixCube, projMatrixCube, mFboColorTexture);
    }
    
    void OnShutDown() {
        Application::OnShutDown();
        
        delete mTriangle;
        delete mCube;
    }
    
    void OnWindowResize(int w, int h) {
        Application::OnWindowResize(w, h);
        glViewport(0, 0, config.windowWidth, config.windowHeight);
    }
    
private:
    WaterBear::Renderables::TriangleRO *mTriangle;
    WaterBear::Renderables::CubeRO *mCube;
    
    GLuint mFbo;
    GLuint mFboColorTexture;
    GLuint mFboDepthTexture;
};

DECLARE_MAIN(RenderingApp)
