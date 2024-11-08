/*
 * =========== time =============
 * Author: Sunil Sapkota
 * Description: Provides all the time-related functionalities for the engine.
 * relative _clock value, scaled and unscaled delta-time,
 * frame-stepping, etc.
 */

#pragma once
#include <chrono>

namespace Everest {
    using hrclock = std::chrono::high_resolution_clock;
    using micro_t = std::chrono::microseconds;
    template<typename T1, typename T2>
    using duration_t = std::chrono::duration<T1, T2>;
    template<typename T>
    using duration_s = std::chrono::duration<T>;
    using time_t = hrclock::time_point;

    class Clock {
        friend class Time;
        public:
            Clock();

            /* starts the _clock for timing */
            void begin();
            /* updates/cycles the _clock */
            virtual void tick();

            /* returns the time elapsed since the creation of _clock */
            inline f64 getElapsedTime(){ return duration_s<f64>
                (hrclock::now()-_epoch) .count(); }
            /* returns time in milliseconds */
            inline f64 getElapsedTime_ms(){ return duration_t<f64, std::milli>
                (hrclock::now()-_epoch).count();}
            /* returns time in microseconds */
            inline f64 getElapsedTime_us(){ return duration_t<f64, std::micro>
                (hrclock::now()-_epoch).count();}
            /* returns the time elapsed since the last time the _clock was ticked */
            inline f64 getTimeSinceLastTick(){
                return duration_s<f64>(hrclock::now()-_lastTickTime) .count();
            }
            /* returns the time elapsed between latest two cycles/ticks */
            inline f64 getTimeBetweenTicks(){ return _timeBetweenTicks;}
            /* returns the clock's ticking rate */
            inline u32 getTickRate(){ return _tickRate;}
            /* returns the current number of cycles/ticks */
            inline u32 getTickCount(){ return _ticks;}

        protected:
            time_t _epoch, _lastTickTime;
            f64 _timeBetweenTicks;
            u32 _ticks, _tickRate;
    };

    class ScaledClock {
        friend class Time;
        public:
            ScaledClock(f32 scale = 1.f):_scale(scale){}

            void begin();
            void tick();
            void manualTick();

            inline void setScale(f32 scale){_scale = abs(scale);}
            inline f32 getScale(){return  _scale;}

            inline f64 getElapsedTime(){return _time;}
            inline f64 getTimeBetweenTicks(){return _timeBetweenTicks;}
            inline f64 getTickCount(){return _ticks;}

        private:
            time_t _lastTickTime;
            f64 _time, _timeBetweenTicks;
            f32 _scale;
            u32 _ticks, _tickRate;
    };

    class Time {

        static void init();
        inline static void quit(){/*maybe add something here*/}

        inline static void begin(){
            _clock->begin();
            _unscaledClock->begin();
        }
        static void tick();

        friend class Core;
        friend class Application;

        public:
            /* returns unscaled time since the beginning of program in seconds*/
            inline static f64 getUnscaledTime(){ return _unscaledClock->getElapsedTime();}
            /* returns unscaled time elapsed between the consecutive frames */
            inline static f64 getUnscaledDeltatime(){ return _unscaledClock->getTimeBetweenTicks();}

            /* returns the time elapsed since the beginning of the program */
            inline static f64 getTime(){ return _clock->getElapsedTime();}
            /* returns the scaled-time between the frames */
            inline static f64 getDeltatime(){ return _clock->getTimeBetweenTicks();}
            /* sets the current time-scale */
            inline static void setScale(f32 scale){_clock->setScale(scale);}
            /* returns the current time-scale */
            inline static f32 getScale(){return  _clock->getScale();}
            /* commands to tick scaled time manually when paused. No effect when
             * time is not paused*/
            inline static void tickManually(){_manualTick = true;}
            /* pauses the time to allow manual tick */
            inline static void setPauseState(bool pause){_paused = pause;}
            /* returns true if time is paused */
            inline static bool isPaused(){return _paused;}

            /* returns the current frame rate in frames-per-seconds */
            inline static int getFrameRate(){return _unscaledClock->getTickRate();}
            /* returns the number of frames rendered since the start of the program */
            inline static int getFrameCount(){return _clock->getTickCount();}

        private:
            static scope<Clock> _unscaledClock;
            static scope<ScaledClock> _clock;
            static bool _manualTick, _paused;
    };
}
