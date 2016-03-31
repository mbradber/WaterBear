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
        
        // rendering settings
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
    
    void OnRender(double currentTime) {
        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        static const float one = 1.0f;
        
        glClearBufferfv(GL_COLOR, 0, green);
        glClearBufferfv(GL_DEPTH, 0, &one);
        
        // draw quad
        vmath::mat4 mvMatrixQuad = vmath::translate(2.5f, 1.5f, -7.0f);
        vmath::mat4 projMatrixQuad = vmath::perspective(50.0f, (float)config.windowWidth / (float)config.windowHeight, 0.1f, 1000.0f);
        
        mTriangle->Render(currentTime, mvMatrixQuad, projMatrixQuad);
        
        // draw cube
        vmath::mat4 projMatrixCube = vmath::perspective(50.0f, (float)config.windowWidth / (float)config.windowHeight, 0.1f, 1000.0f);
        
        vmath::mat4 mvMatrixCube = vmath::translate(0.0f, 0.0f, -7.0f) *
            vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
            vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
        
        mCube->Render(currentTime, mvMatrixCube, projMatrixCube);
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
};

DECLARE_MAIN(RenderingApp)
