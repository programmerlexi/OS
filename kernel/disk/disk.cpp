#include "disk.h"
#include "../memory/heap.h"
bool identify_ata(uint8_t drive) {
    // 0xA0 for Master
	// 0xB0 for Slave

	outb(0x1F6, drive);
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);
	outb(0x1F7, 0xEC);
	uint8_t tmp = inb(0x1F7);
	sleep(2);
    if (tmp & STATUS_RDY) {
        switch (drive) {
            case 0xA0:
                print_string("Master Drive Ready\n\r");
                break;
            case 0xB0:
                print_string("Slave Drive Ready\n\r");
                break;
            default:
                print_string("Unknown Drive Ready\n\r");
                break;
        }
    } else {
        switch (drive) {
            case 0xA0:
                print_string("Master Drive Not Ready\n\r");
                break;
            case 0xB0:
                print_string("Slave Drive Not Ready\n\r");
                break;
            default:
                print_string("Unknown Drive Not Ready\n\r");
                break;
        }
    }
    return tmp & STATUS_RDY;
}

void wait_BSY(){
	while(inb(0x1F7) & STATUS_BSY);
}

void wait_DRQ(){
	while(!(inb(0x1F7) & STATUS_RDY));
}

uint16_t* LBA28_read_sectors(uint8_t drive, uint32_t LBA, uint8_t sector) {
    wait_BSY();
    outb(0x1F6, drive | ((LBA >> 24) & 0xF));
	outb(0x1F1, 0x00);
    outb(0x1F2,sector);
    outb(0x1F3, (uint8_t) LBA);
    outb(0x1F4, (uint8_t)(LBA >> 8));
	outb(0x1F5, (uint8_t)(LBA >> 16)); 
	outb(0x1F7, 0x20); // 0x20 = 'Read' Command
    uint16_t *addr = (uint16_t*)calloc(sizeof(uint16_t)*256*sector);
    for(int j = 0; j < sector; j++) {
        wait_BSY();
        wait_DRQ();
        for (int i = 0; i < 256; i++) {
            addr[i] = inw(0x1F0);
        }
        addr += 256;
    }
    return (uint16_t*) addr;
}

void LBA28_write_sectors(uint8_t drive, uint32_t LBA, uint32_t sector, uint32_t *buffer) {
    wait_BSY();
    outb(0x1F6, drive | ((LBA >> 24) & 0xF));
    outb(0x1F1, 0x00);
	outb(0x1F2, sector);
	outb(0x1F3, (uint8_t) LBA);
	outb(0x1F4, (uint8_t) (LBA >> 8));
	outb(0x1F5, (uint8_t) (LBA >> 16)); 
	outb(0x1F7,0x30); // 0x30 = 'Write' Command
    for (int j = 0; j < sector; j++){
		wait_BSY();
		wait_DRQ();

		for(int i = 0; i < 256; i++){
			outl(0x1F0, buffer[i]);
		}

		outb(0x1F7, 0xE7);
		sleep(1);
	}
}