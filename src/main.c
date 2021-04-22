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
#include <tice.h>

#include "flash.h"
#include "miner.h"
#include "ports.h"


void miner(void) {
	// Check for new hashes, do some graphical stuff, call assembly miner
	timer_Disable(1);
	timer_Set(1, 5*32768);
	timer_SetReload(1, 5*32768);
	timer_AckInterrupt(1, TIMER_RELOADED);
	timer_Enable(1, TIMER_32K, TIMER_0INT, TIMER_DOWN);
	while(!kb_IsDown(kb_KeyClear)) {
		bool result = mine_loop();
		printf("%lx\n", block.nonce);
		if(result) {
			printf("Found nonce %lx\n", block.nonce);
			block.nonce = 0;
		}
		kb_Scan();
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
