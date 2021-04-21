#include <fileioc.h>
#include "flash.h"
#include "ports.h"
#include <graphx.h>
#include <string.h>

void *sequence = NULL;

/* Privileged code must be reset prior to running this */
void *getSequence(void) {
    static const uint8_t seq[] = {0x3E, 0x04, 0xF3, 0x18, 0x00, 0xF3, 0xED, 0x7E, 0xED, 0x56, 0xED, 0x39, 0x28, 0xED, 0x38, 0x28, 0xCB, 0x57, 0xC9};
    ti_CloseAll();
    /* todo: Error if file exists? */
    ti_var_t slot = ti_Open("TMP", "w");
    ti_Write(seq, sizeof(seq), 1, slot);
    ti_SetArchiveStatus(slot, true);
    ti_Rewind(slot);
    return ti_GetDataPtr(slot);
}

void unlock_flash_full(void) {
    if(!sequence) {
        if(priv_upper() == 0xFF) {
            reset_priv();
        }
        sequence = getSequence();
    }
    set_priv();
    if(get_flash_lock_status() == LOCKED)
        flash_unlock();
    if(get_flash_lock_status() == PARTIALLY_UNLOCKED)
        flash_sequence(sequence);
}
