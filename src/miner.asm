sha_base = $E10000
sha_control = 0
sha_last = $C
sha_block = $10
sha_state = $60

sha_block_size = $40
sha_digest_size = 256/8

bitcoin_block_size = $50

macro block?
 macro end?.block?!
   .size := $-.raw_contents
   db $80
   rb sha_block_size - (.size + 1 + 4) mod sha_block_size
   dd (.size*8) bswap 4
   .num_blocks := (.size + 1 + 4 - 1) / sha_block_size + 1
  end virtual
  repeat .num_blocks * sha_block_size / 4
   load .raw_data:4 from .raw_contents_:((%-1)*4)
   dd .raw_data bswap 4
  end repeat
  purge end?.block?
 end macro
 virtual at 0
  .raw_contents_::
  .raw_contents:
end macro

public	_mine_loop
_mine_loop:
	ld	iy,sha_base

	scf
	sbc	hl,hl
	ld	(hl),2
.loop:
	ld	hl,_block
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
	ld	hl,_target + sha_digest_size - 1
.compare_loop:
	ld	a,(bc)
	cp	a,(hl)
	dec	bc
	dec	hl
	jq	z,.compare_loop
	jq	c,.success

	; Increment nonce
	ld	hl,(_block.nonce)
	inc	hl
	ld	(_block.nonce),hl
	jq	nz,.no_upper
	ld	hl,_block.nonce+3
	inc	(hl)
.no_upper:

	; Check timer
	ld	hl,$F20034
	bit	0,(hl)
	jq	z,.loop
	set	0,(hl)

	xor	a,a
	ret

.success:
	; will probably never get called
	ld	a,1
	ret

public	_block

_block:
; originally ends in 42a14695
block
	emit	bitcoin_block_size: $0100000081cd02ab7e569e8bcd9317e2fe99f2de44d49ab2b8851ba4a308000000000000e320b6c2fffc8d750423db8b1eb942ae710e951ed797f7affc8892b0f1fc122bc7f5d74df2b9441a42a14695 bswap bitcoin_block_size
end block
.nonce := . + 76

_target:
	emit	sha_digest_size: $44B9F2 shl (8*($1A-3))
