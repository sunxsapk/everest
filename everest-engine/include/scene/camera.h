/*
 * ============= camera ==============
 * Author: Sunil Sapkota
 * Description: camera_c system provides all the required functionalities required
 * for calculating view-frustum, projection, coordinates mapping, etc.
 */

#pragma once
#include "math/types.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#define __str__(x) #x

namespace Everest {
    class camera_c {
        public:
            bool isPrimary = false;
            bool fixedAspect = false;


            camera_c(bool is2d = true);

            inline mat4 getProjection(){ return _projection;}

            inline f32 get_lenssize() const {return u_size_fov;}
            inline f32 get_fov() const {return u_size_fov;}
            inline f32 get_aspect() const {return aspect;}
            inline f32 get_near() const {return near;}
            inline f32 get_far() const {return far;}

            inline void set_fov(f32 fov){ u_size_fov = fov; recalc();}
            inline void set_lenssize(f32 size){ u_size_fov = size; recalc();}
            inline void set_aspect(f32 aspect_){ aspect = aspect_;recalc(); }
            inline void set_near(f32 near){ near = near; recalc();}
            inline void set_far(f32 far){ far = far; recalc();}

            inline bool is2d() const {return _isOrtho;}
            inline bool is3d() const {return !_isOrtho;}
            inline void set2d(){_isOrtho = true;}
            inline void set3d(){_isOrtho = false;}

        protected:
            f32 u_size_fov = 5.f, aspect = 16.f/9.f, near = 0.01f, far=100.f;
            bool _isOrtho = true;
            mat4 _projection;

        private:
            inline void recalc(){
                if(_isOrtho){
                    f32 lw = u_size_fov * aspect;
                    _projection = glm::ortho(-lw, lw, -u_size_fov, u_size_fov, near, far);
                } else {
                    _projection = glm::perspective(glm::radians(u_size_fov), aspect, near, far);
                }
            }
    };
}
