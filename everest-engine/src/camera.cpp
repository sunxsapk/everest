#include "scene/camera.h"

namespace Everest {

    camera_c::camera_c()
    :_type(Orthographic){
        recalc();
    }


    camera_c::camera_c(OrthographicData data)
    :_orthoData(data), _type(Orthographic){
        recalc();
    }

    camera_c::camera_c(PerspectiveData data)
    :_perspData(data), _type(Perspective){
        recalc();
    }
}
