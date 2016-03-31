#ifndef WaterBearTriangleRO_h
#define WaterBearTriangleRO_h

#include "ext/gl/gl3w.h"
#include <string>
#include "ext/math/vmath.h"

namespace WaterBear {
namespace Renderables {
    
    class TriangleRO {
    public:
        
        TriangleRO(const std::string& texture);
        ~TriangleRO();
        
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



#endif /* WaterBearTriangleRO_h */
