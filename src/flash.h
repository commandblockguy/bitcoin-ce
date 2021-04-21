#ifndef H_FLASH
#define H_FLASH

#include <stdint.h>

void unlock_flash_full(void);

void flash_unlock(void);
void flash_lock(void);
void flash_sequence(void *sequence);

enum flash_lock_status {
    LOCKED = 0x00,
    PARTIALLY_UNLOCKED = 0x04,
    FULLY_UNLOCKED = 0x0C
};

uint8_t get_flash_lock_status(void);

#endif //H_FLASH