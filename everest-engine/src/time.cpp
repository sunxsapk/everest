#include "core/time.h"

namespace Everest {
    Clock::Clock(ClockResolution res):_resolution(res){
        _epoch = _clock::now();
    }

    void Clock::begin(){
        _lastTickTime = _clock::now();
        _ticks = 0;
        _timeBetweenTicks = 0;
    }

    void Clock::tick(){
        time_t t = _clock::now();
        duration_t<f64, std::micro> uc_del = t - _lastTickTime;
        _lastTickTime = t;

        _ticks++;
        _timeBetweenTicks = uc_del.count()/_resolution;
        _tickRate = 1.f/_timeBetweenTicks;
    }

    scope<Clock> Time::clock = NULL;
    void Time::init(){
        clock = createScope<Clock>();
    }

    void Time::tick(){ //TODO: scaled time and manual ticking
        clock->tick();
    }
}
