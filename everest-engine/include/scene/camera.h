/*
 * ============= camera ==============
 * Author: Sunil Sapkota
 * Description: camera_c system provides all the required functionalities required
 * for calculating view-frustum, projection, coordinates mapping, etc.
 */

#pragma once
#include "math/types.h"
#include "component_type.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#define __str__(x) #x

namespace Everest {
    struct camera_c : public component_t {
        bool isPrimary = false;
        bool fixedAspect = false;


        camera_c(bool is2d = true){ _isOrtho = is2d; recalc(); }
        camera_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const camera_c& other, Entity ent) {
            *this = other;
            entity = ent;
            active = other.active;
        }

        inline mat4 getProjection(){ return _projection;}

        inline f32 get_lenssize() const {return _u_size_fov;}
        inline f32 get_fov() const {return _u_size_fov;}
        inline f32 get_aspect() const {return _aspect;}
        inline f32 get_near() const {return _near;}
        inline f32 get_far() const {return _far;}

        inline void set_fov(f32 fov){ _u_size_fov = fov; recalc();}
        inline void set_near(f32 near_) { _near = near_; recalc();}
        inline void set_far(f32 far_) { _far = far_; recalc(); }
        inline void set_lenssize(f32 size){ _u_size_fov = size; recalc();}
        inline void set_aspect(f32 aspect){ _aspect = aspect; recalc();}
        

        inline bool is2d() const {return _isOrtho;}
        inline bool is3d() const {return !_isOrtho;}
        inline void set2d(){
            _isOrtho = true;
            _u_size_fov = 10.f;
            _near = -1000.f;
            _far = 1000.f;
            recalc();
        }
        inline void set3d(){
            _isOrtho = false;
            _u_size_fov = 60.f;
            _near = 0.01f;
            _far = 1000.f;
            recalc();
        }

        protected:
        mat4 _projection;
        f32 _u_size_fov = 8.f, _aspect = 16.f/9.f, _near = 0.01f, _far=100.f;
        bool _isOrtho = true;

        private:
        inline void recalc(){
            _projection = _isOrtho?
                glm::ortho(-_u_size_fov * _aspect, _u_size_fov * _aspect, -_u_size_fov, _u_size_fov, _near, _far) :
                glm::perspective(glm::radians(_u_size_fov), _aspect, _near, _far);
        }
    };
}
