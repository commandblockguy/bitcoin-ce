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

#include "flash.h"
#include "ports.h"

// todo: check for 5.5+
int main(void)
{
	gfx_Begin();
	unlock_flash_full();

	flash_lock();
	reset_priv();
	gfx_End();
    return 0;
}
