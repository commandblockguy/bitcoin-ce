#ifndef H_CURSORMEM
#define H_CURSORMEM

#include <stdint.h>

#include "miner.h"

// probably should be using the linker for this but I'm lazy

struct cursormem {
	struct block block;
	uint8_t target[32];
	char miner_func[];
};

#define CURSORMEM ((struct cursormem *const)0xE30800)

// Returns true if block was mined
#define MINER_LOOP ((bool (*const)(void))CURSORMEM->miner_func)

void copy_miner(void);

#endif