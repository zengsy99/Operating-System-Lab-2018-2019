.data

msg: .ascii "It's a bootloader...\n"

.text
.global main

main:
	la	$a0, msg
	jal	0x8007b980			#printstr
	
	li	$a0, 0xa0800000
	li	$a1, 0x200
	lw	$a2, 0xa08001fc($0)

	li	$31, 0xa0800000
	j	0x8007b1cc			#read_sd_card
