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

// start and end addresses for the miner function
extern uint8_t mine_func_start[];
extern uint8_t mine_func_end[];

#endif
