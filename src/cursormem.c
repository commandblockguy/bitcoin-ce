#include "cursormem.h"

#include <string.h>

void copy_miner(void) {
	memcpy(CURSORMEM->miner_func, mine_func_start, mine_func_end - mine_func_start);
}
