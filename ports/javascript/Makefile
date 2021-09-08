SHELL := /bin/bash
$(info ************  lvgl_javascript version ************)
# customizations for lvgl
WASM_FILE_API = 1
FROZEN_DIR ?= modules

#LV_CFLAGS:= -DLV_FONT_CUSTOM_DECLARE="LV_FONT_DECLARE(lv_font_heb_16)"

include ../../py/mkenv.mk

CROSS = 0

QSTR_DEFS = qstrdefsport.h

include $(TOP)/py/py.mk

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

#LDFLAGS ?= -m32 -Wl,--gc-sections
#CFLAGS ?= -m32

#default debug options for browser debugger
JSFLAGS ?= --source-map-base http://localhost:8000

CFLAGS += -std=gnu99 -Wall -Werror -Wdouble-promotion -Wfloat-conversion
CFLAGS += -O3 -DNDEBUG -g4
LDFLAGS += -g4
CFLAGS += $(INC)

CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += $(CFLAGS_MOD)

CC = emcc
LD = emcc
# TODO: figure out why this hack is needed
CPP := emcc -x c -E -D__CPP__ -D__EMSCRIPTEN__


SRC_SHARED = $(addprefix shared/,\
	runtime/interrupt_char.c \
	runtime/stdout_helpers.c \
	runtime/pyexec.c \
	readline/readline.c \
	timeutils/timeutils.c \
	)



LIB_SRC_C = $(addprefix lib/,\
    lv_bindings/driver/SDL/SDL_monitor.c \
    lv_bindings/driver/SDL/SDL_mouse.c \
	lv_bindings/driver/SDL/modSDL.c \
	$(LIB_SRC_C_EXTRA) \
	)

SRC_C = \
	main.c \
	mphalport.c \
	modutime.c \
	lv_font_heb_16.c \

ifdef WASM_FILE_API
	SRC_C += file.c wasm_file_api.c
endif

ifneq ($(FROZEN_DIR),)
# To use frozen source modules, put your .py files in a subdirectory (eg scripts/)
# and then invoke make with FROZEN_DIR=scripts (be sure to build from scratch).
CFLAGS += -DMICROPY_MODULE_FROZEN_STR
endif

ifneq ($(FROZEN_MPY_DIR),)
# To use frozen bytecode, put your .py files in a subdirectory (eg frozen/) and
# then invoke make with FROZEN_MPY_DIR=frozen (be sure to build from scratch).
CFLAGS += -DMICROPY_QSTR_EXTRA_POOL=mp_qstr_frozen_const_pool
CFLAGS += -DMICROPY_MODULE_FROZEN_MPY
endif

SRC_C += $(SRC_MOD)

SRC_QSTR += $(SRC_C) $(SRC_LIB) $(LIB_SRC_C)

OBJ += $(PY_O)
OBJ += $(addprefix $(BUILD)/, $(SRC_SHARED:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(SRC_C:.c=.o))
OBJ += $(addprefix $(BUILD)/, $(LIB_SRC_C:.c=.o))

JSFLAGS += -s ASYNCIFY -s 'ASYNCIFY_IMPORTS=["mp_js_sleep_with_intr"]' -s WASM=1 -s USE_SDL=2
JSFLAGS += -s EXPORTED_FUNCTIONS="['_mp_js_init', '_mp_js_init_repl', '_mp_js_do_str', '_mp_js_process_char', '_mp_hal_get_interrupt_char', '_mp_sched_keyboard_interrupt', '_mp_handle_pending']" -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -s --memory-init-file 0 --js-library library.js

all: $(BUILD)/micropython.js

$(BUILD)/micropython.js: $(OBJ) library.js wrapper.js
	$(ECHO) "LINK $(BUILD)/firmware.js"
	$(Q)emcc $(LDFLAGS) -o $(BUILD)/firmware.js $(OBJ) $(JSFLAGS)
	cat wrapper.js $(BUILD)/firmware.js > $@
	$(Q)mkdir -p $(BUILD)/../bundle_out
	cp $@ $(BUILD)/../bundle_out/lvgl_mp.js
	([ -e $(BUILD)/firmware.wasm ] && cp $(BUILD)/firmware.wasm $(BUILD)/../bundle_out/firmware.wasm); true
	([ -e $(BUILD)/firmware.wasm.map ] && cp $(BUILD)/firmware.wasm.map $(BUILD)/../bundle_out/firmware.wasm.map); true

min: $(BUILD)/micropython.js
	uglifyjs $< -c -o $(BUILD)/micropython.min.js

test: $(BUILD)/micropython.js $(TOP)/tests/run-tests.py
	$(eval DIRNAME=ports/$(notdir $(CURDIR)))
	cd $(TOP)/tests && MICROPY_MICROPYTHON=../ports/javascript/node_run.sh ./run-tests.py -j1

# Disable errors for array-bounds warnings on "sp[-MP_OBJ_ITER_BUF_NSLOTS + 2]" access.
$(BUILD)/py/vm.o: CFLAGS += -Wno-error=array-bounds

include $(TOP)/py/mkrules.mk
