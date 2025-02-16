/*
 * ========= Collision Resolver =============
 * Description: Provides things for collision/contact resolution
 */

#pragma once

#include "physics/rigidbody.h"
#include "scene/components.h"

namespace Everest {

    struct contact_body_t {
        transform_c* transform;
        rigidbody_c* rigidbody;
        vec3 relativeContactPoint;

        // calculated inside
        vec3 angularInertia;

        operator bool(){return rigidbody != nullptr;}
    };

    struct body_contact_t {
        contact_body_t body1;
        contact_body_t body2;

        vec3 contactNormal;
        f32 penetration = 0.f;
        f32 restitution = 1.f;
        f32 friction = 0.3f;

        void resolve(f32 duration);
        f32 getSeparationVelocity() const;

        private:
        void resolvePenetration(f32 duration);
        void resolveVelocity(f32 duration);
        vec3 getRelativeVelocity() const;
    };


    struct body_contact2d_t {
        transform_c* transformA;
        transform_c* transformB;

        rigidbody2d_c* rigidbody2dA;
        rigidbody2d_c* rigidbody2dB;

        vec2 contactNormal;
        vec2 relativeContactPointA;
        vec2 relativeContactPointB;

        f32 angularInertiaA = 0.f;
        f32 angularInertiaB = 0.f;

        f32 penetration = 0.f;
        f32 restitution = 1.f;
        f32 friction = 0.3f;

        void prepareContacts();
        void resolvePenetration();
        void resolveVelocity();
        f32 getSeparationVelocity() const;
        private:
        vec2 getRelativeVelocity() const;
    };

    class contact_resolver_t {

        public:
            contact_resolver_t(u32 iterations = 0);

            void resolveContacts(std::vector<body_contact_t>& contactRegistry, f32 duration);

        protected:
            u32 _iterations, _iterationsUsed;

    };


    class contact2d_resolver_t {

        public:
            contact2d_resolver_t(u32 iterations = 0);
            void resolveContacts(std::vector<body_contact2d_t>& contactRegistry);

        protected:
            u32 _iterations, _iterationsUsed;
            void prepareContacts(std::vector<body_contact2d_t>& contactRegistry);
            void resolvePenetration(std::vector<body_contact2d_t>& contactRegistry);
            void resolveVelocities(std::vector<body_contact2d_t>& contactRegistry);

    };
}
    struct AABB;
    struct AABB2D;

