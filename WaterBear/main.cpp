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

class RenderingApp : public WaterBear::Core::Application {
public:
    
    void OnStart() {
        mProgram = glCreateProgram();
        
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        CompileShader(fs, "texture_map_basic_fs.glsl");
        
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(vs, "texture_map_basic_vs.glsl");
        
        glAttachShader(mProgram, fs);
        glAttachShader(mProgram, vs);
        
        glLinkProgram(mProgram);
        glDeleteShader(fs);
        glDeleteShader(vs);
        
        mModelViewLoc = glGetUniformLocation(mProgram, "mv_matrix");
        mProjLoc = glGetUniformLocation(mProgram, "proj_matrix");
        mTexLoc = glGetUniformLocation(mProgram, "tex_object");
        
        mTriangle = new WaterBear::Renderables::TriangleRO("../assets/ktx/pattern1.ktx");
        
        // cube vertex array object
        glGenVertexArrays(1, &mVao);
        glBindVertexArray(mVao);
        
        WaterBear::Core::WBVert_Pos_Tex cube_verts[] = {
            {{-1, -1, 1}, {0, 0}},
            {{1, -1, 1}, {1, 0}},
            {{1, 1, 1}, {1, 1}},
            {{-1, 1, 1}, {0, 1}},
            
            {{1, -1, 1}, {0, 0}},
            {{1, -1, -1}, {1, 0}},
            {{1, 1, -1}, {1, 1}},
            {{1, 1, 1}, {0, 1}},
            
            {{1, -1, -1}, {0, 0}},
            {{-1, -1, -1}, {1, 0}},
            {{-1, 1, -1}, {1, 1}},
            {{1, 1, -1}, {0, 1}},
            
            {{-1, -1, -1}, {0, 0}},
            {{-1, -1, 1}, {1, 0}},
            {{-1, 1, 1}, {1, 1}},
            {{-1, 1, -1}, {0, 1}},
            
            {{-1, 1, 1}, {0, 0}},
            {{1, 1, 1}, {1, 0}},
            {{1, 1, -1}, {1, 1}},
            {{-1, 1, -1}, {0, 1}},
            
            {{1, -1, -1}, {0, 0}},
            {{1, -1, 1}, {1, 0}},
            {{-1, -1, 1}, {1, 1}},
            {{-1, -1, -1}, {0, 1}}
        };
    
        GLushort cube_indices[] = {
            0, 1, 2,
            2, 3, 0,
            
            4, 5, 6,
            6, 7, 4,
            
            8, 9, 10,
            10, 11, 8,
            
            12, 13, 14,
            14, 15, 12,
            
            16, 17, 18,
            18, 19, 16,
            
            20, 21, 22,
            22, 23, 20,
        };
        
        glGenBuffers (1, &mPositionBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_verts), cube_verts, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WaterBear::Core::WBVert_Pos_Tex), NULL);
        glEnableVertexAttribArray(0);
        
        glGenBuffers(1, &mIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
        
        // texture
        glGenTextures(1, &mTexObject);
        
        char* rootDir = WaterBear::Core::Application::GetRootDir();
        std::string brickTexturePath = std::string(rootDir) + "../assets/ktx/baboon.ktx";
        sb7::ktx::file::load(brickTexturePath.c_str(), mTexObject);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(WaterBear::Core::WBVert_Pos_Tex), BUFFER_OFFSET(12));
        glEnableVertexAttribArray(1);
        
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
        glUseProgram(mProgram);
        
        // CUBE RENDER
        glBindVertexArray(mVao);
        glBindTexture(GL_TEXTURE_2D, mTexObject);
        
        // projection matrix
        vmath::mat4 projMatrix = vmath::perspective(50.0f, (float)config.windowWidth / (float)config.windowHeight, 0.1f, 1000.0f);
        glUniformMatrix4fv(mProjLoc, 1, GL_FALSE, projMatrix);
        
        // model-view matrix
        vmath::mat4 mvMatrix = vmath::translate(0.0f, 0.0f, -7.0f) *
            vmath::rotate((float)currentTime * 45.0f, 0.0f, 1.0f, 0.0f) *
            vmath::rotate((float)currentTime * 81.0f, 1.0f, 0.0f, 0.0f);
        
        glUniformMatrix4fv(mModelViewLoc, 1, GL_FALSE, mvMatrix);
        
        // textures
        glUniform1i(mTexLoc, 0);
        
        // draw
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }
    
    void OnShutDown() {
        Application::OnShutDown();
        
        glDeleteVertexArrays(1, &mVao);
        glDeleteProgram(mProgram);
        
        glDeleteBuffers(1, &mPositionBuffer);
        glDeleteBuffers(1, &mIndexBuffer);
        
        delete mTriangle;
    }
    
    void OnWindowResize(int w, int h) {
        Application::OnWindowResize(w, h);
        glViewport(0, 0, config.windowWidth, config.windowHeight);
    }
    
private:
    GLuint mProgram;
    GLuint mVao;
    GLuint mModelViewLoc;
    GLuint mProjLoc;
    GLuint mPositionBuffer;
    GLuint mIndexBuffer;
    GLuint mTexObject;
    GLuint mTexLoc;
    
    WaterBear::Renderables::TriangleRO *mTriangle;
};

DECLARE_MAIN(RenderingApp)
