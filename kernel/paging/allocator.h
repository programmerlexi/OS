#pragma once

typedef char block_free;

block_free buddy0[1048576]; // 4K Blocks

void* allocate_blocks(int blocks);