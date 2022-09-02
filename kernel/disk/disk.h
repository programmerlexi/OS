#pragma once
#include "../utils/util.h"
#include "../services/timer.h"

#define STATUS_BSY      0x80
#define STATUS_RDY      0x40
#define STATUS_DRQ      0x08
#define STATUS_DF       0x20
#define STATUS_ERR      0x01

bool master_rdy = false;
bool slave_rdy = false;

bool identify_ata(uint8_t drive);
void wait_BSY();
void wait_DRQ();
uint16_t* LBA28_read_sectors(uint8_t drive, uint32_t LBA, uint8_t sector);
void LBA28_write_sectors(uint8_t drive, uint32_t LBA, uint32_t sector, uint32_t *buffer);