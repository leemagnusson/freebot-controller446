#include "actuator.h"
#include "util.h"
#include "param.h"
#include "fast_loop.h"
#include "main_loop.h"
#include <cstdio>
#include "parameter_api.h"
#include "system.h"

void Actuator::set_param(FastLoopParam &fast_loop_param, MainLoopParam &main_loop_param) {
    fast_loop_.set_param(fast_loop_param);
    main_loop_.set_param(main_loop_param);
}

void Actuator::start() {
     // zero current sensors in voltage mode to try to eliminate bias from pwm noise, could also do open mode
    fast_loop_.voltage_mode();
    uint32_t t_start = get_clock();
    while ((get_clock() - t_start)/170e6 < 2) {
        fast_loop_.zero_current_sensors();
    }

    if (param()->startup_param.do_phase_lock) {
        fast_loop_.phase_lock_mode(param()->startup_param.phase_lock_current);
        ms_delay(1000*param()->startup_param.phase_lock_duration);
    }
    fast_loop_.maintenance();  // TODO better way than calling this to update zero pos
    //main_loop_.set_mode(param()->startup_param.startup_mode);
    fast_loop_.current_mode();
    fast_loop_.set_iq_des(0);
}

void Actuator::maintenance() {
    fast_loop_.maintenance();
}