#pragma once
#include <stdint.h>
#include "../../utils/util.h"
#include "../../disk/disk.h"
#include "../rootfs/rootfs.h"
enum dev_type {
    NONE,
    DEV_BLOCK,
    DEV_STREAM,
    DEV_PORT
};

typedef struct dev_file {
    char* name;
    dev_type type;
    uint16_t device;
} dev_file_t;

dev_file_t* dev_files[255]; // 255 devices max.

void read_dev_bytes(dev_file_t* file, uint8_t* buffer, uint8_t size);
void write_dev_bytes(dev_file_t* file, uint8_t* buffer, uint8_t size);
dev_file_t* open_dev_file(const char* name);
void init_devfs();