;
; Harrin2_DA_2B.asm
;
; Created: 10/5/2019 11:22:23 PM
; Author : Nick
;DA2B
	.INCLUDE <M328DEF.INC>
	jmp MAIN 
	.ORG 0x02 ;reset location
	jmp DA_TWO
MAIN: 
	ldi r16, 0x00 ;set portc as input
	ldi r17, 0x03 ;set port b as output
	out DDRC, r16
	out DDRB, r17
	sbi PORTC,3 ;set portc.3
	ldi r20, 0x2 ;falling edge triggered
	sts EICRA, r20
	ldi r20,1<<INT0 ;enable int0
	out EIMSK, r20 
	sei ;set interrupts
HERE:jmp HERE

DA_TWO:
	ldi r18, 11
	ldi r19, 38
	ldi r21, 94
L1:
	dec r21
	brne L1
	dec r19
	brne L1
	dec r18
	brne L1
	ret
	

