.data

msg: .ascii "It's a bootloader....\n"

.text
.global main

main:
	la	$a0, msg
	jal	0x8007b980			#printstr

	li	$a0, 0xa0800200
	li	$a1, 0x200
	lw	$a2, 0xa08001fc($0)

	jal	0x8007b1cc			#read_sd_card
	 
	jal	0xa0800200
	
stop:
	j	stop
