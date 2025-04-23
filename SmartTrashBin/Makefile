PROJECT = trash_can_control
BUILD_DIR = bin

CFILES = main.c srf05.c servo.c delay.c button.c font.c i2c.c led.c sh1106.c uart.c vl53l0x.c smartbin.c 

DEVICE = stm32f411ceu6
OOCD_FILE = board/stm32f4discovery.cfg

VPATH += $(SHARED_DIR) src
INCLUDES += $(patsubst %,-I%, . inc $(SHARED_DIR))
OPENCM3_DIR = /home/hoang_buu/project_individual_of_buu/libopencm3_lib/libopencm3-template/libopencm3


include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
