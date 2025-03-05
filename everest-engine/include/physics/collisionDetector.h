/*
 * ========== Contacts ==============
 * Author: Sunil Sapkota
 * Description: Provides data and methods for Contact detection and generation for physics objects 
 */

#pragma once


#include "physics/colliders2d.h"
#include "physics/collisionResolver.h"

namespace Everest {

    class CollisionDetector2D {
        using result_t = std::vector<body_contact2d_t>;
        public:

            static bool checkForContacts(ref<collider2d_t>& body1, ref<collider2d_t>& body2, result_t& results);

        private:

            static bool circle_circle(collider2d_t& body1, collider2d_t& body2, result_t& results);
            static bool box_circle(collider2d_t& body1, collider2d_t& body2, result_t& results);
            static bool box_box(collider2d_t& body1, collider2d_t& body2, result_t& results);

            static inline bool circle_box(collider2d_t& body1, collider2d_t& body2, result_t& results){
                return box_circle(body2, body1, results);
            }
    };

}
