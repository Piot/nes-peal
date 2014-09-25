.export _wait_for_that_vsync
.export _zero_screen_position
.export _draw_avatar


.proc _wait_for_that_vsync: near

.code
	lda #$00
	rts

.endproc


.proc	_zero_screen_position: near

.code

	lda     #$00
	sta     $2005
	sta     $2005
	rts

.endproc


.proc 	_draw_avatar: near

.segment	"DATA"

temp:
	.byte	$00

.code
	tya
	lsr     a
	lsr     a
	lsr     a
	clc
	adc     #$20
	sta     $2006

	tya
	asl     a
	asl     a
	asl     a
	asl     a
	asl     a
	sta     temp
	txa
	clc
	adc		temp
	sta     $2006


	lda     #$78
	sta     $2007
	rts

.endproc
