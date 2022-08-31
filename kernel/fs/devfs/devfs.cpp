#include "devfs.h"
void read_dev_bytes(dev_file_t* file, uint8_t* buffer, uint8_t size) {
    switch (file->type) {
        case DEV_PORT:
        {
            for (int i = 0; i < size; i++) {
                buffer[i] = inb(file->device);
            }
            break;
        }
        case DEV_BLOCK:
        {
            int mod = size % 512;
            int sectors = ((size-mod)/512)+1;
            uint8_t* data = (uint8_t*)LBA28_read_sectors((uint8_t)(file->device&0xff),0,sectors);
            for (int i = 0; i < size; i++) {
                buffer[i] = data[i];
            }
            break;
        }
        case DEV_STREAM:
        {
            for (int i = 0; i < size; i++) {
                buffer[i] = *(uint8_t*)(file->device+i);
            }
            break;
        }
        default:
        {
            regs_t* regs = get_regs();
            kpanic("broken device",regs);
            break;
        }
    }
}

void write_dev_bytes(dev_file_t* file, uint8_t* buffer, uint8_t size) {
    switch (file->type) {
        case DEV_PORT:
        {
            for (int i = 0; i < size; i++) {
                outb(file->device,buffer[i]);
            }
            return;
        }
        case DEV_BLOCK:
        {
            int mod = size % 512;
            int sectors = ((size-mod)/512)+1;
            uint8_t* data = (uint8_t*)LBA28_read_sectors((uint8_t)(file->device&0xff),0,sectors);
            for (int i = 0; i < size; i++) {
                data[i] = buffer[i];
            }
            LBA28_write_sectors((uint8_t)(file->device&0xff),0,sectors,(uint32_t*)data);
            return;
        }
        case DEV_STREAM:
        {
            for (int i = 0; i < size; i++) {
                *(uint8_t*)(file->device+i) = buffer[i];
            }
            return;
        }
        default:
        {
            regs_t* regs = get_regs();
            kpanic("broken device",regs);
        }
    }
}

dev_file_t* open_dev_file(const char* name) {
    for (int i = 0; i < 255; i++) {
        if (dev_files[i]==NULL) continue;
        if (strcmp(dev_files[i]->name,name)==0) continue;
        return dev_files[i];
    }
    return NULL;
}

void create_dev_file(char* name, uint16_t device, dev_type type) {
    for (int i = 0; i < 255; i++) {
        if (dev_files[i]!=NULL) continue;
        dev_files[i] = (dev_file_t*)malloc(sizeof(dev_file_t));
        dev_files[i]->device = device;
        dev_files[i]->name = name;
        dev_files[i]->type = type;
        return;
    }
}

void init_devfs() {
    mount((char*)"/dev",read_dev_bytes,write_dev_bytes,open_dev_file,create_dev_file);
    for (int i = 0; i < 255; i++) {
        dev_files[i] = NULL;
    }
}