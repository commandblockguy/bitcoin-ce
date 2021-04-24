sha_base = $E10000
sha_control = 0
sha_last = $C
sha_block = $10
sha_state = $60

sha_block_size = $40
sha_digest_size = 256/8

bitcoin_block_size = $50

cursormem = $E30800
block = cursormem
block.nonce := block + 76
target = cursormem + bitcoin_block_size

public	_mine_func_start
_mine_func_start:
	ld	iy,sha_base

	scf
	sbc	hl,hl
	ld	(hl),2
.loop:
	ld	hl,block
	lea	de,iy+sha_block
	ld	bc,sha_block_size
	ldir
	ld	a,$a
	ld	(iy+sha_control),a

	lea	de,iy+sha_block
	ld	c,bitcoin_block_size-sha_block_size
	ldir
	xor	a,a
	sbc	hl,hl
	add	hl,de	; hl = de
	ld	(de),a
	inc	de
	ld	c,sha_block_size-(bitcoin_block_size-sha_block_size)
	ldir
	ld	de,$280 ; de = bitcoin_block_size * 8, e = $80
	ld	(iy+sha_block+bitcoin_block_size-sha_block_size+3),e
	ld	(iy+sha_block + sha_block_size - 4),de
	ld	a,$e
	ld	(iy+sha_control),a

	lea	hl,iy+sha_state
	lea	de,iy+sha_block
	ld	c,sha_digest_size
	ldir
	ld	a,$80
	ld	(iy+sha_block+sha_digest_size+3),a
	ld	b,1 ; $bc = $100 = sha_digest_size * 8
	ld	(iy+sha_block + sha_block_size - 4),bc
	ld	a,$a
	ld	(iy+sha_control),a

	; Compare to target
	; No bounds check, because
	lea	bc,iy+sha_state + sha_digest_size - 1
	ld	hl,target + sha_digest_size - 1
.compare_loop:
	ld	a,(bc)
	cp	a,(hl)
	dec	bc
	dec	hl
	jr	z,.compare_loop
	jr	c,.success

	; Increment nonce
	ld	hl,(block.nonce)
	inc	hl
	ld	(block.nonce),hl
	jr	nz,.no_upper
	ld	hl,block.nonce+3
	inc	(hl)
.no_upper:

	; Check timer
	ld	hl,$F20034
	bit	0,(hl)
	jr	z,.loop
	set	0,(hl)

	xor	a,a
	ret

.success:
	; will probably never get called
	ld	a,1
	ret
public	_mine_func_end
_mine_func_end:
