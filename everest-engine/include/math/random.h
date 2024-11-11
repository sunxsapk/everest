/*
 * ========== Random Number Generators ============
 * Author: Different implementations of Pseudo Random numbers generators are
 * provided as a part of the engine.
 */

#pragma once
#include <random>
#include "types.h"

namespace Everest {
    class PRNumGenerator {
        public:
            PRNumGenerator(u64 seed){_engine.seed(seed);}

            /*returns the random integer in specified data-type*/
            template<typename T>
            inline T randomInt_t(){
                return (T)_dist(_engine);
            }

            /* returns random number between [0,1] in floating point precision*/
            inline f32 r_f32(){
                return (u32)_dist(_engine)/((f32)std::numeric_limits<u32>::max());
            }
            /* returns random number between [0,1] in double precision*/
            inline f64 r_f64(){
                return _dist(_engine)/((f64)std::numeric_limits<u64>::max());
            }
        private:
            PRNumGenerator(){}
            friend class Random;
        private:
            std::mt19937 _engine;
            std::uniform_int_distribution<std::mt19937::result_type> _dist;
    };


    class Random {
        public:
            inline static void init(){}
            /* returns random number between [0,1] in floating point precision*/
            inline static f32 r_f32(){return _gen.r_f32();}
            /* returns random number between [0,1] in double precision*/
            inline static f32 r_f64(){return _gen.r_f64();}

            /*returns the random integer in specified data-type*/
            template<typename T>
            inline T randomInt_t(){
                return _gen.randomInt_t<T>();
            }

        private:
            static PRNumGenerator _gen;
    };
}
