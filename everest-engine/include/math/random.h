/*
 * ========== Random Number Generators ============
 * Author: Different implementations of Pseudo Random numbers generators are
 * provided as a part of the engine.
 */

#pragma once
#include <random>
#include "types.h"

namespace Everest {
    class Random {
        public:
            inline static void init(){_engine.seed(std::random_device()());}
            inline static f32 r_f32(){
                return (f32)_dist(_engine)/(f32)std::numeric_limits<u32>::max();
            }

        private:
            static std::mt19937 _engine;
            static std::uniform_int_distribution<std::mt19937::result_type> _dist;
    };
}
