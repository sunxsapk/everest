/*
 * =========== time =============
 * Author: Sunil Sapkota
 * Description: Provides all the time-related functionalities for the engine.
 * relative clock value, scaled and unscaled delta-time,
 * frame-stepping, etc.
 */

#pragma once

namespace Everest {
    class Time {
        public:
            static void init();
            static void tick();

            /* sets the game time-scale to max(0.f, provided_value) */
            static void setTimeScale(f32 scale);

            /*returns the unscaled time since the beginning of the application.*/
            static inline f32 getUnscaledTime(){return glfwGetTime();}
            /*returns unscaled delta-time between frames*/
            static inline f32 getUnscaledDeltaTime(){return u_deltaTime;}
            /*returns frame-rate of the game */
            static inline i32 getFrameRate() {return g_fps;}
            /*returns frame count*/
            static inline i32 getFrameCount(){return g_frameCount;}

            /*returns scaled time, used for physics system calculation*/
            static inline f32 getScaledTime(){return s_time;}
            /*returns scaled delta-time between frames*/
            static inline f32 getDeltaTime(){return s_deltaTime;}
            /*retuns current timescale*/
            static inline f32 getTimeScale(){return g_timeScale;}
        private:
            // unscaled times
            static f32 u_curTime, u_deltaTime;
            static i32 g_fps;

            // scaled times
            static f32 s_time, s_deltaTime, g_timeScale;
            static u32 g_frameCount;
    };
}
