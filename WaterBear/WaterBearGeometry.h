#ifndef WaterBearGeometry_h
#define WaterBearGeometry_h

#include "ext/math/vmath.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace WaterBear {
namespace Core {
    
    template<class T>
    class Size {
    public:
        
        Size<T>(T w, T h): width(w), height(h) {}
        Size<T>():width(0), height(0) {}
        
        T width;
        T height;
    };
    
    typedef Size<int> SizeI;
    
    // VERTEX TYPES
    typedef struct WBVert_Pos_Tex {
        vmath::vec3 pos;
        vmath::vec2 tc;
    } WBVert_Pos_Tex;
    
}
}

#endif /* WaterBearGeometry_h */
