unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned random() {
    bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
    return lfsr = (lfsr >> 1) | (bit << 15);
}

unsigned char random_byte() {
    return random() & 0xFF;
}