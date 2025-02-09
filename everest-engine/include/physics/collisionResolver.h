/*
 * ========= Collision Resolver =============
 * Description: Provides things for collision resolution
 */

#pragma once

#include "physics/rigidbody.h"
#include "scene/components.h"

namespace Everest {

    struct BodyContact {
        transform_c* transform1;
        transform_c* transform2;
        rigidbody_c* body1;
        rigidbody_c* body2;

        vec3 contactNormal;
        f32 restitution;
        f32 penetration;

        void resolve(f32 duration);
        f32 calcSeparateVelocity() const;
        private:
        void resolvePenetration(f32 duration);
        void resolveVelocity(f32 duration);
    };

    struct BodyContact2D {
        transform_c* transform1;
        transform_c* transform2;
        rigidbody2d_c* body1 = nullptr;
        rigidbody2d_c* body2 = nullptr;

        vec2 contactNormal;
        vec2 ra, rb;
        f32 restitution = 1.f;
        f32 friction = 0.3f;
        f32 penetration = 0.f;

        void resolve(f32 duration);
        f32 calcSeparateVelocity() const;
        private:
        void resolvePenetration(f32 duration);
        void resolveVelocity(f32 duration);
        vec2 calcRelativeVelocity() const;
    };

    class ContactResolver {

        public:
            ContactResolver(u32 iterations = 0);

            void resolveContacts(std::vector<BodyContact>& contactRegistry, f32 duration);

        protected:
            u32 _iterations, _iterationsUsed;

    };


    class ContactResolver2D {

        public:
            ContactResolver2D(u32 iterations = 0);

            void resolveContacts(std::vector<BodyContact2D>& contactRegistry, f32 duration);

        protected:
            u32 _iterations, _iterationsUsed;

    };
}
