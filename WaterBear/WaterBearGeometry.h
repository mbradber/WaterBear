// todo: pragma once stuff

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
}
}
