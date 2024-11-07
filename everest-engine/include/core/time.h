/*
 * =========== time =============
 * Author: Sunil Sapkota
 * Description: Provides all the time-related functionalities for the engine.
 * relative clock value, scaled and unscaled delta-time,
 * frame-stepping, etc.
 */

#pragma once
#include <chrono>

namespace Everest {
    enum ClockResolution {
        Micro = 1,
        Milli = 1000,
        Seconds = 1000000,
    };

    class Clock {
        using _clock = std::chrono::high_resolution_clock;
        using micro_t = std::chrono::microseconds;
        template<typename T1, typename T2>
        using duration_t = std::chrono::duration<T1, T2>;
        using time_t = _clock::time_point;

        public:
            Clock(ClockResolution resolution = Seconds);

            /* starts the clock for timing */
            void begin();
            /* updates/cycles the clock */
            void tick();

            /* returns the time elapsed since the creation of clock */
            inline f64 getElapsedTime(){
                return duration_t<f64, std::micro>(_clock::now()-_epoch)
                    .count()/static_cast<f64>(_resolution);
            }
            /* returns the time elapsed since the last time the clock was ticked */
            inline f64 getTimeSinceLastTick(){
                return duration_t<f64, std::micro>(_clock::now()-_lastTickTime)
                    .count()/static_cast<f64>(_resolution);
            }
            /* returns the time elapsed between latest two cycles/ticks */
            inline f64 getTimeBetweenTicks(){ return _timeBetweenTicks;}
            /* returns the clock's ticking rate */
            inline u32 getTickRate(){ return _tickRate;}
            /* returns the current number of cycles/ticks */
            inline u32 getTickCount(){ return _ticks;}
            /* returns clock's resolution */
            inline u32 getResolution(){return _resolution;}

        private:
            time_t _epoch, _lastTickTime;
            f64 _timeBetweenTicks;
            u32 _ticks, _tickRate, _resolution;

    };

    class Time {

        static void init();
        static void quit(){}

        inline static void begin(){clock->begin();}
        static void tick();

        friend class Core;
        friend class Application;

        public:
            /* returns unscaled time in milliseconds */
            inline static f64 getUnscaledTime(){ return clock->getElapsedTime();}
            /* returns the unscaled time elapsed between the consecutive frames */
            inline static f64 getUnscaledDeltatime(){ return clock->getTimeBetweenTicks();}

            /* returns the time elapsed since the beginning of the program */
            inline static f64 getTime(){ return clock->getElapsedTime();}

            /* returns the scaled-time between the frames */
            inline static f64 getDeltatime(){ return clock->getTimeBetweenTicks();}
            /* returns the current frame rate in frames-per-seconds */
            inline static int getFrameRate(){return clock->getTickRate();}

            /* returns the number of frames rendered since the start of the program */
            inline static int getFrameCount(){return clock->getTickCount();}

        private:
            static scope<Clock> clock;
    };
}
