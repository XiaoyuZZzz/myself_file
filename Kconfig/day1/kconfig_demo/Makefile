.DEFAULT_GOAL := all

include .config

CC = gcc
CFLAGS = -I.

ifeq ($(OS),Windows_NT)
    RM = del /F /Q
    EXE = .exe
else
    RM = rm -f
    EXE =
endif

TARGET = app$(EXE)
CONFIG_H = config.h

SRCS = main.c

ifeq ($(CONFIG_UART),y)
    SRCS += drivers/uart.c
endif

ifeq ($(CONFIG_BLE),y)
    SRCS += drivers/ble.c
endif

$(CONFIG_H): .config genconfig.py
	python genconfig.py

$(TARGET): $(CONFIG_H) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

all: $(TARGET)

.PHONY: all clean

clean:
	$(RM) $(TARGET) $(CONFIG_H)
