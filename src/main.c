/*
 *--------------------------------------
 * Program Name: MINER
 * Author: commandblockguy
 * License: MIT
 * Description: Bitcoin Miner
 *--------------------------------------
*/

#include <debug.h>
#include <graphx.h>
#include <keypadc.h>
#include <stdbool.h>
#include <string.h>
#include <tice.h>

#include "cursormem.h"
#include "flash.h"
#include "miner.h"
#include "ports.h"

char test_block[] = "\x00\x00\x00\x01\xab\x02\xcd\x81\x8b\x9e\x56\x7e\xe2\x17\x93\xcd\xde\xf2\x99\xfe\xb2\x9a\xd4\x44\xa4\x1b\x85\xb8\x00\x00\x08\xa3\x00\x00\x00\x00\xc2\xb6\x20\xe3\x75\x8d\xfc\xff\x8b\xdb\x23\x04\xae\x42\xb9\x1e\x1e\x95\x0e\x71\xaf\xf7\x97\xd7\xb0\x92\x88\xfc\x2b\x12\xfc\xf1\x4d\xd7\xf5\xc7\x1a\x44\xb9\xf2\x95\x46\xa1\x42";
char test_target[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xf2\xb9\x44\x00\x00\x00\x00\x00\x00";

void miner(void) {
	// Check for new hashes, do some graphical stuff, call assembly miner
	memcpy(&CURSORMEM->block, test_block, sizeof CURSORMEM->block);
	memcpy(&CURSORMEM->target, test_target, sizeof CURSORMEM->target);
	copy_miner();
	timer_Disable(1);
	timer_Set(1, 32768);
	timer_SetReload(1, 32768);
	timer_AckInterrupt(1, TIMER_RELOADED);
	timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
	while(!kb_IsDown(kb_KeyClear)) {
		bool result = MINER_LOOP();
		printf("%lx\n", CURSORMEM->block.nonce);
		if(result) {
			printf("Found nonce %lx\n", CURSORMEM->block.nonce);
			CURSORMEM->block.nonce = 0;
		}
		kb_Scan();
		return;
	}
	//printf("%x\n", hash);
}

// todo: check for 5.5+
int main(void)
{
	//gfx_Begin();
	os_ClrHome();
	unlock_flash_full();

	miner();

	flash_lock();
	reset_priv();
	//gfx_End();
    return 0;
}
