#ifndef MEMORY_PHYS_H
#define MEMORY_PHYS_H

#include <typedef.h>
#include <lock.h>
#include <multiboot.h>

typedef enum
{
    MEM_BLOCK_FREE        = (1 << 0),
    MEM_BLOCK_HW_RESERVED = (1 << 1),
    MEM_BLOCK_LOCKED      = (1 << 2),
    MEM_BLOCK_KERNEL_ONLY = (1 << 3)
} mem_block_flags;

struct mem_region;
struct mem_block;

typedef struct mem_region
{
    spinlock lock;
    
    uint64 physical_address;
    
    uint16 num_blocks;
    uint16 first_free;
    
    struct mem_block* block_info;
    
    struct mem_region* next;
} mem_region;

typedef struct mem_block
{
    struct mem_region* region;
    uint16 next_free;
    
    uint16 ref_count;
    
    uint32 flags;
    uint32 owner_pid;
} mem_block;

void kmem_phys_init(multiboot_info* info);

uint64 kmem_block_address(mem_block* block);
mem_block* kmem_block_find(uint64 address);
uint32 kmem_block_find_range(mem_block** out, uint64 start, uint64 end);

mem_block* kmem_block_alloc(bool kernel);
void kmem_block_free(mem_block* block);

#endif