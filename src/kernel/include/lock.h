#ifndef LOCK_H
#define LOCK_H

typedef struct
{
    uint32 taken;
    uint32 old_eflags;
} spinlock;

void spinlock_init(spinlock* lock);

void spinlock_acquire(spinlock* lock);
void spinlock_release(spinlock* lock);

#endif
