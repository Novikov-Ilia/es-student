#pragma once
#include <stdint.h>

#define DEVICE_NAME "es-led-module"
#define FIRMWARE_VERSION "1.2.0"
#define DEVICE_PROJECT "221-command-time"
#define DEVICE_REPO "https://github.com/Novikov-Ilia/es-student"

#ifndef DEVICE_BOARD
#define DEVICE_BOARD "unknown"
#endif

void device_info(void);


// До перестановки (revision, version, name) sizeof = 24, padding = 6.
// После: 20 байт, padding = 2; значения полей сохранены.
struct info_t
{
    uint32_t version;
    char name[13];
    uint8_t revision;
};

extern struct info_t device_card;
void dev_info(void);
