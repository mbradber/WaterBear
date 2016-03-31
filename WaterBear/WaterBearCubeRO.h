#ifndef WaterBearCubeRO_h
#define WaterBearCubeRO_h

#include "ext/gl/gl3w.h"
#include <string>
#include "ext/math/vmath.h"

namespace WaterBear {
namespace Renderables {
    
    class CubeRO {
    public:
        CubeRO(const std::string& texture);
        ~CubeRO();
        
        void Render(double currentTime, const vmath::mat4& mvMatrix, const vmath::mat4& projMatrix);
        
    private:
        GLuint mVao;
        GLuint mVbo;
        GLuint mIbo;
        GLuint mTexObject;
        
        GLuint mProgram;
        GLuint mModelViewLoc;
        GLuint mProjLoc;
        GLuint mTexLoc;
    };
    
}
}


#endif /* WaterBearCubeRO_h */
