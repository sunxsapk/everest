#include "math/matrix.h"

namespace Everest {
    template<typename T>
    _tmat4<T> operator +(_tmat4<T> a,_tmat4<T> b){
        _tmat4<T> c;
        for(i32 i=0; i<4; i++){
            for(i32 j=0; j<4; j++){
                c[i][j] = a[i][j]+b[i][j];
            }
        }
        return c;
    }

    template<typename T>
    _tmat4<T> operator -(_tmat4<T> a,_tmat4<T> b){
        _tmat4<T> c;
        for(i32 i=0; i<4; i++){
            for(i32 j=0; j<4; j++){
                c[i][j] = a[i][j]-b[i][j];
            }
        }
        return c;
    }
}
