#include "core/time.h"

#define MAX_DELTATIME 10.f

namespace Everest {
    Clock::Clock(){
        _epoch = hrclock::now();
    }

    void Clock::begin(){
        _lastTickTime = hrclock::now();
        _ticks = 0;
        _timeBetweenTicks = 0;
    }

    void Clock::tick(){
        time_t t = hrclock::now();
        duration_s<f64> uc_del = t - _lastTickTime;
        _lastTickTime = t;

        _ticks++;
        _timeBetweenTicks = uc_del.count();
        _tickRate = 1.f/_timeBetweenTicks;
    }

    void ScaledClock::begin(){
        _lastTickTime = hrclock::now();
        _ticks = 0;
        _timeBetweenTicks = 0;
    }

    void ScaledClock::manualTick(){
        _timeBetweenTicks = 1.f/60.f;
        _tickRate = 60.f;
        _time += _timeBetweenTicks;
        _ticks++;
    }

    void ScaledClock::tick(){
        time_t t = hrclock::now();
        duration_s<f64> uc_del = t - _lastTickTime;
        _lastTickTime = t;

        _timeBetweenTicks = uc_del.count() * _scale;
        if(_timeBetweenTicks > MAX_DELTATIME) _timeBetweenTicks = 1.f/60.f;
        _tickRate = 1.f/_timeBetweenTicks;
        _time += _timeBetweenTicks;
        _ticks++;
    }

    scope<ScaledClock> Time::_clock = NULL;
    scope<Clock> Time::_unscaledClock = NULL;
    bool Time::_manualTick = false;
    bool Time::_paused = false;

    void Time::init(){
        _clock = createScope<ScaledClock>();
        _unscaledClock = createScope<Clock>();
    }

    void Time::quit(){
        _unscaledClock.reset();
        _clock.reset();
    }

    void Time::tick(){
        _unscaledClock->tick();
        if(!_paused){
            _clock->tick();
        } else if(_manualTick){
            _clock->manualTick();
            _manualTick = false;
        } else {
            _clock->_lastTickTime = hrclock::now();
        }
    }
}
