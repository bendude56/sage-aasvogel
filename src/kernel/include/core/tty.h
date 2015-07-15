#ifndef CORE_TTY_H
#define CORE_TTY_H

#include <core/console.h>
#include <lock/spinlock.h>
#include <stdarg.h>

#define TTY_NUM_VCS 5
#define TTY_NUM_SERIAL 4

typedef struct tty_base
{
    spinlock lock;
    
    uint16 width, height;
    
    void (*flush)(struct tty_base*);
    void (*clear)(struct tty_base*);
    void (*write)(struct tty_base*, char);
    
    bool supports_color;
    console_color back_color;
    console_color fore_color;
    
    bool supports_cursor;
    uint16 cursor_x, cursor_y;
    bool cursor_hidden;
} tty_base;

typedef struct
{
    tty_base base;
    
    console_char* buffer;
    uint16 buffer_line;
    
    bool is_active;
} tty_vc;

typedef struct
{
    tty_base base;
    
    uint16 io_port;
} tty_serial;

extern tty_vc tty_virtual_consoles[TTY_NUM_VCS];
extern tty_serial tty_serial_consoles[TTY_NUM_SERIAL];

extern void tty_init(void);
extern void tty_init_vc(tty_vc* tty, console_char* buffer, uint16 width, uint16 height);
extern void tty_init_serial(tty_serial* tty, uint16 io_port);

extern void tty_write(tty_base* tty, const char* msg);

extern void tty_switch_vc(tty_vc* tty);

extern void tprintf(tty_base* tty, const char* format, ...);
extern void tvprintf(tty_base* tty, const char* format, va_list vararg);

#endif
