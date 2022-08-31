#pragma once
#include "../devfs/devfs.h"

typedef struct mount_point {
    char* name;
    void* read;
    void* write;
    void* open;
    void* create;
} mount_point_t;

mount_point_t mount_points[255]; // 255 mount points max

void mount(char* name, void* read, void* write, void* open, void* create);
void unmount(char* name);
void init_rootfs();