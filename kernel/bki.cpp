/* Bootloader Kernel Interaction */
#include "bki.h"

boot_info* get_boot_info() {
    return (boot_info*)0x5000;
}