#ifndef H_MINER
#define H_MINER

#include <stdbool.h>
#include <stdint.h>

struct block {
	uint32_t version;
	char hashPrevBlock[32];
	char hashMerkleRoot[32];
	uint32_t time;
	uint32_t bits;
	uint32_t nonce;
};

extern struct block block;

// Returns true if block was mined
bool mine_loop(void);

#endif
