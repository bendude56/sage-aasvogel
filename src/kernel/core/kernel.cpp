#include <typedef.hpp>
#include <multiboot.hpp>

#include <core/gdt.hpp>
#include <core/idt.hpp>

extern "C" void kernel_main(multiboot_info* mb_info)
{
    gdt::init();
    idt::init();
    
    hang();
}
