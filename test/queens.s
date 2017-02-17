	.seg	"data"
	.common	up,64,4
	.common	down,64,4
	.common	rows,36,4
	.common	x,36,4
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.1_i = -4
	save	%sp,(-96),%sp
! source statement 9
	st	%g0,[%fp + .1_i]
.L20:
! source statement 10
	mov	%g0,%o0
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	sethi	%hi(down),%o2
	add	%o2,%lo(down),%o2
	st	%o0,[%o1 + %o2]
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	sethi	%hi(up),%o2
	add	%o2,%lo(up),%o2
	st	%o0,[%o1 + %o2]
! source statement 10
	ld	[%fp + .1_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_i]
! source statement 10
	ld	[%fp + .1_i],%o0
	cmp	%o0,16
	bge	.L21
	nop
	ba,a	.L20
.L21:
! source statement 11
	st	%g0,[%fp + .1_i]
.L24:
! source statement 12
	mov	%g0,%o0
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	sethi	%hi(x),%o2
	add	%o2,%lo(x),%o2
	st	%o0,[%o1 + %o2]
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	sethi	%hi(rows),%o2
	add	%o2,%lo(rows),%o2
	st	%o0,[%o1 + %o2]
! source statement 12
	ld	[%fp + .1_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_i]
! source statement 12
	ld	[%fp + .1_i],%o0
	cmp	%o0,9
	bge	.L25
	nop
	ba,a	.L24
.L25:
! source statement 13
	mov	1,%o0
	call	queens,1
	nop
	ret
	restore
	.seg	"data"
	.seg	"text"
	.align	8
	.global	queens
	.proc	0
queens:
.2_r = -4
	save	%sp,(-96),%sp
.2_c = 68
	st	%i0,[%fp + .2_c]
! source statement 21
	mov	1,%o0
	st	%o0,[%fp + .2_r]
.L31:
! source statement 22
	ld	[%fp + .2_r],%o0
	sll	%o0,2,%o0
	sethi	%hi(rows),%o1
	add	%o1,%lo(rows),%o1
	ld	[%o0 + %o1],%o0
	cmp	%o0,%g0
	bne	.L32
	nop
	ld	[%fp + .2_r],%o0
	ld	[%fp + .2_c],%o1
	sub	%o0,%o1,%o0
	add	%o0,8,%o0
	sll	%o0,2,%o0
	sethi	%hi(up),%o1
	add	%o1,%lo(up),%o1
	ld	[%o0 + %o1],%o0
	cmp	%o0,%g0
	bne	.L32
	nop
	ld	[%fp + .2_r],%o0
	ld	[%fp + .2_c],%o1
	add	%o0,%o1,%o0
	sub	%o0,1,%o0
	sll	%o0,2,%o0
	sethi	%hi(down),%o1
	add	%o1,%lo(down),%o1
	ld	[%o0 + %o1],%o0
	cmp	%o0,%g0
	bne	.L32
	nop
! source statement 23
	mov	1,%o0
	ld	[%fp + .2_r],%o1
	ld	[%fp + .2_c],%o2
	add	%o1,%o2,%o1
	sub	%o1,1,%o1
	sll	%o1,2,%o1
	sethi	%hi(down),%o2
	add	%o2,%lo(down),%o2
	st	%o0,[%o1 + %o2]
	ld	[%fp + .2_r],%o1
	ld	[%fp + .2_c],%o2
	sub	%o1,%o2,%o1
	add	%o1,8,%o1
	sll	%o1,2,%o1
	sethi	%hi(up),%o2
	add	%o2,%lo(up),%o2
	st	%o0,[%o1 + %o2]
	ld	[%fp + .2_r],%o1
	sll	%o1,2,%o1
	sethi	%hi(rows),%o2
	add	%o2,%lo(rows),%o2
	st	%o0,[%o1 + %o2]
! source statement 24
	ld	[%fp + .2_r],%o0
	ld	[%fp + .2_c],%o1
	sll	%o1,2,%o1
	sethi	%hi(x),%o2
	add	%o2,%lo(x),%o2
	st	%o0,[%o1 + %o2]
! source statement 25
	ld	[%fp + .2_c],%o0
	cmp	%o0,8
	bne	.L33
	nop
! source statement 26
	sethi	%hi(x),%o0
	add	%o0,%lo(x),%o0
	call	print,1
	nop
	ba,a	.L35
.L33:
! source statement 28
	ld	[%fp + .2_c],%o0
	add	%o0,1,%o0
	call	queens,1
	nop
.L35:
! source statement 29
	mov	%g0,%o0
	ld	[%fp + .2_r],%o1
	ld	[%fp + .2_c],%o2
	add	%o1,%o2,%o1
	sub	%o1,1,%o1
	sll	%o1,2,%o1
	sethi	%hi(down),%o2
	add	%o2,%lo(down),%o2
	st	%o0,[%o1 + %o2]
	ld	[%fp + .2_r],%o1
	ld	[%fp + .2_c],%o2
	sub	%o1,%o2,%o1
	add	%o1,8,%o1
	sll	%o1,2,%o1
	sethi	%hi(up),%o2
	add	%o2,%lo(up),%o2
	st	%o0,[%o1 + %o2]
	ld	[%fp + .2_r],%o1
	sll	%o1,2,%o1
	sethi	%hi(rows),%o2
	add	%o2,%lo(rows),%o2
	st	%o0,[%o1 + %o2]
.L32:
! source statement 31
	ld	[%fp + .2_r],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .2_r]
! source statement 31
	ld	[%fp + .2_r],%o0
	cmp	%o0,8
	bg	.L36
	nop
	ba,a	.L31
.L36:
	ret
	restore
	.seg	"data"
	.seg	"text"
	.align	8
	.global	print
	.proc	0
print:
.3_k = -4
	save	%sp,(-96),%sp
.3_x = 68
	st	%i0,[%fp + .3_x]
! source statement 38
	mov	1,%o0
	st	%o0,[%fp + .3_k]
.L41:
! source statement 39
	sethi	%hi(.L43),%o0
	ld	[%fp + .3_x],%o1
	ld	[%fp + .3_k],%o2
	sll	%o2,2,%o2
	add	%o0,%lo(.L43),%o0
	ld	[%o1 + %o2],%o1
	call	printf,2
	nop
! source statement 39
	ld	[%fp + .3_k],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .3_k]
! source statement 39
	ld	[%fp + .3_k],%o0
	cmp	%o0,8
	bg	.L44
	nop
	ba,a	.L41
.L44:
! source statement 40
	sethi	%hi(.L45),%o0
	add	%o0,%lo(.L45),%o0
	call	printf,1
	nop
	ret
	restore
	.seg	"data"
.L45:
	.ascii "\12\0"
.L43:
	.ascii " %d\0"
