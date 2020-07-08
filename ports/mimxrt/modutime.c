/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Damien P. George
 * Copyright (c) 2020 Jim Mussared
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

<<<<<<< Updated upstream:extmod/nimble/modbluetooth_nimble.h
#ifndef MICROPY_INCLUDED_EXTMOD_NIMBLE_MODBLUETOOTH_NIMBLE_H
#define MICROPY_INCLUDED_EXTMOD_NIMBLE_MODBLUETOOTH_NIMBLE_H

#include "extmod/modbluetooth.h"
=======
#include "extmod/utime_mphal.h"
>>>>>>> Stashed changes:ports/mimxrt/modutime.c

STATIC const mp_rom_map_elem_t time_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_utime) },

<<<<<<< Updated upstream:extmod/nimble/modbluetooth_nimble.h
typedef struct _mp_bluetooth_nimble_root_pointers_t {
    // Characteristic (and descriptor) value storage.
    mp_gatts_db_t gatts_db;

    // Pending service definitions.
    size_t n_services;
    struct ble_gatt_svc_def *services[MP_BLUETOOTH_NIMBLE_MAX_SERVICES];
} mp_bluetooth_nimble_root_pointers_t;

enum {
    MP_BLUETOOTH_NIMBLE_BLE_STATE_OFF,
    MP_BLUETOOTH_NIMBLE_BLE_STATE_STARTING,
    MP_BLUETOOTH_NIMBLE_BLE_STATE_ACTIVE,
    MP_BLUETOOTH_NIMBLE_BLE_STATE_STOPPING,
=======
    { MP_ROM_QSTR(MP_QSTR_sleep), MP_ROM_PTR(&mp_utime_sleep_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_ms), MP_ROM_PTR(&mp_utime_sleep_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_sleep_us), MP_ROM_PTR(&mp_utime_sleep_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_ms), MP_ROM_PTR(&mp_utime_ticks_ms_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_us), MP_ROM_PTR(&mp_utime_ticks_us_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_cpu), MP_ROM_PTR(&mp_utime_ticks_cpu_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_add), MP_ROM_PTR(&mp_utime_ticks_add_obj) },
    { MP_ROM_QSTR(MP_QSTR_ticks_diff), MP_ROM_PTR(&mp_utime_ticks_diff_obj) },
>>>>>>> Stashed changes:ports/mimxrt/modutime.c
};

STATIC MP_DEFINE_CONST_DICT(time_module_globals, time_module_globals_table);

<<<<<<< Updated upstream:extmod/nimble/modbluetooth_nimble.h
void mp_bluetooth_nimble_port_preinit(void);
void mp_bluetooth_nimble_port_postinit(void);
void mp_bluetooth_nimble_port_deinit(void);
void mp_bluetooth_nimble_port_start(void);

#endif // MICROPY_INCLUDED_EXTMOD_NIMBLE_MODBLUETOOTH_NIMBLE_H
=======
const mp_obj_module_t mp_module_utime = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&time_module_globals,
};
>>>>>>> Stashed changes:ports/mimxrt/modutime.c
