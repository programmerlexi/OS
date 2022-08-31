#include "rootfs.h"

void mount(char* name, void* read, void* write, void* open, void* create) {
    for (int i = 0; i < 255; i++) {
        if (!strcmp(mount_points[i].name,"UNUSED")) return;
        mount_points[i].name = name;
        mount_points[i].read = read;
        mount_points[i].write = write;
        mount_points[i].open = open;
        mount_points[i].create = create;
        return;
    }
}

void unmount(char* name) {
    for (int i = 0; i < 255; i++) {
        if (strcmp(mount_points[i].name,"UNUSED")) continue;
        if (!strcmp(mount_points[i].name,name)) continue;
        mount_points[i].name = (char*)"UNUSED";
        mount_points[i].read = NULL;
        mount_points[i].write = NULL;
        mount_points[i].open = NULL;
        mount_points[i].create = NULL;
        return;
    }
}

void init_rootfs() {
    for (int i = 0; i < 255; i++) {
        mount_points[i].name = (char*)"UNUSED";
        mount_points[i].read = NULL;
        mount_points[i].write = NULL;
        mount_points[i].open = NULL;
        mount_points[i].create = NULL;
    }
}