;
; Harrin2_DA_2A.asm
;
; Created: 9/30/2019 1:22:36 AM
; Author : Nick
;
	ldi r16, 0x00 ;load 0 into r16
	ldi r17, 0xff ;loaf ff into r17
	out DDRC, r16 ;port c is input
	out DDRB, r17 ;port b is output
	sbi PORTC,3 ;set pull up on portc pin 3
CHECK: sbis PINC, 3 //check if pin c pin 3 is enable
	rjmp CHECK
	ldi r17, 0x02 //load 2 into r17
	out PORTB, r17 //enable port b pin 2
	call DELAY ;delay for 250ms
	call DELAY
	call DELAY
	call DELAY
	cbi PORTB,2  ;clear portb pin2
END: rjmp END

DELAY: 
	ldi  r18, 11
    ldi  r19, 38
    ldi  r20, 94
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1
    rjmp PC+1
