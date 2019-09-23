;
; Harrin2_Assign1B.asm
;
; Created: 9/20/2019 3:04:21 PM
; Author : Nick
;


; Replace with your application code
start:
    lds r20, 0x02
	lds r21, 0x00
	ldi r22, 5
	ldi r23, 255
	ldi r24, 5
L1:	cp r22, r23
	brne D1
D1:	cp r22, r24
	brne D2
	st X, r22
	sts 0x300, X+
	adiw r24, 5
	inc r22 
	jmp L1
D2:	st X, r22
	sts 0x500, X+
	inc r22
	jmp L1
	
	





