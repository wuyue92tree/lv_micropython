/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
<<<<<<< Updated upstream:ports/mimxrt/modutime.c
 * Copyright (c) 2019 Damien P. George
 * Copyright (c) 2020 Jim Mussared
=======
 * Copyright (c) 2017-2018 Glenn Ruben Bakke
 * Copyright (c) 2018 Ayke van Laethem
>>>>>>> Stashed changes:ports/nrf/drivers/rng.c
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

<<<<<<< Updated upstream:ports/mimxrt/modutime.c
#include "extmod/utime_mphal.h"

STATIC const mp_rom_map_elem_t time_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },

    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&mp_utime_sleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&mp_utime_sleep_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&mp_utime_sleep_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_ms), MP_ROM_PTR(&mp_utime_ticks_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_us), MP_ROM_PTR(&mp_utime_ticks_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_cpu), MP_ROM_PTR(&mp_utime_ticks_cpu_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_add), MP_ROM_PTR(&mp_utime_ticks_add_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_diff), MP_ROM_PTR(&mp_utime_ticks_diff_obj) },
};

STATIC MP_DEFINE_CONST_DICT(time_module_globals, time_module_globals_table);

const mp_obj_module_t mp_module_utime = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&time_module_globals,
};
=======
#include "py/mpconfig.h"

#if MICROPY_HW_ENABLE_RNG

#include "nrf_rng.h"
#include "drivers/rng.h"

#if BLUETOOTH_SD
#include "nrf_soc.h"
#include "ble_drv.h"
#define BLUETOOTH_STACK_ENABLED() (ble_drv_stack_enabled())
#endif

static inline uint32_t generate_hw_random(void) {
    uint32_t retval = 0;
    uint8_t * p_retval = (uint8_t *)&retval;

    nrf_rng_event_clear(NRF_RNG, NRF_RNG_EVENT_VALRDY);
    nrf_rng_task_trigger(NRF_RNG, NRF_RNG_TASK_START);

    for (uint16_t i = 0; i < 4; i++) {
        while (!nrf_rng_event_check(NRF_RNG, NRF_RNG_EVENT_VALRDY)) {
            ;
        }

        nrf_rng_event_clear(NRF_RNG, NRF_RNG_EVENT_VALRDY);
        p_retval[i] = nrf_rng_random_value_get(NRF_RNG);
    }

    nrf_rng_task_trigger(NRF_RNG, NRF_RNG_TASK_STOP);

    return retval;
}

uint32_t rng_generate_random_word(void) {

#if BLUETOOTH_SD
    if (BLUETOOTH_STACK_ENABLED() == 1) {
        uint32_t retval = 0;
        uint32_t status;
        do {
            status = sd_rand_application_vector_get((uint8_t *)&retval, 4); // Extract 4 bytes
        } while (status != 0);

        return retval;
    }
#endif

    return generate_hw_random();
}

#endif // MICROPY_HW_ENABLE_RNG
>>>>>>> Stashed changes:ports/nrf/drivers/rng.c
