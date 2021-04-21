public _flash_unlock
public _flash_lock
public _get_flash_lock_status
public _flash_sequence


_flash_unlock:
	ld	bc,$24
	ld	a,$8c
	call	write_port
	ld	bc,$06
	call	read_port
	or	a,4
	call	write_port
	ld	bc,$28
	ld	a,$4
	jp	write_port

_flash_lock:
	ld	bc,$28
	xor	a,a
	call	write_port
	ld	bc,$06
	call	read_port
	res	2,a
	call	write_port
	ld	bc,$24
	ld	a,$88
	jp	write_port

_get_flash_lock_status:
    ld  bc,$28
    jp  read_port

_flash_sequence:
    pop de
    pop hl
    push    hl
    push    de
    jp  (hl)

extern read_port
extern write_port
