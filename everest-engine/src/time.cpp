#include "core/time.h"

namespace Everest {
    f32 Time::u_curTime     = -1.f;
    f32 Time::u_deltaTime   = -1.f;

    f32 Time::s_time        = -1.f;
    f32 Time::s_deltaTime   = -1.0f;
    f32 Time::g_timeScale     =  1.0f;

    i32 Time::g_fps           = 0;
    u32 Time::g_frameCount    = 0;

    void Time::init(){
        u_curTime = 0.f;

        s_time = 0.f;

        g_timeScale = 1.0f;
        g_fps = 0;
        glfwSetTime(0.0);
    }

    void Time::tick(){
        f32 prevTime = u_curTime;
        u_curTime = glfwGetTime();

        if(g_fps){
            u_deltaTime = u_curTime - prevTime;
        }else{
            s_time = u_curTime;
            u_deltaTime = 1.f/60.f;
        }

        s_deltaTime = u_deltaTime * g_timeScale;
        s_time += s_deltaTime;

        g_fps = 1.f/u_deltaTime;
        g_frameCount++;
    }

    void Time::setTimeScale(f32 scale){
        g_timeScale = std::max(0.f, scale);
    }
}
