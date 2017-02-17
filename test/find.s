	.seg	"data"
	.align	8
	.global	a
a:
	.word	9
	.word	8
	.word	7
	.word	6
	.word	5
	.word	4
	.word	3
	.word	2
	.word	1
	.word	0
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.1_i = -4
.1_key = -8
	save	%sp,(-104),%sp
! source statement 7
	sethi	%hi(.L16),%o0
	add	%o0,%lo(.L16),%o0
	call	printf,1
	nop
! source statement 8
	sethi	%hi(.L18),%o0
	add	%o0,%lo(.L18),%o0
	add	%fp,.1_key,%o1
	call	scanf,2
	nop
! source statement 9
	st	%g0,[%fp + .1_i]
.L21:
! source statement 10
	ld	[%fp + .1_i],%o0
	sll	%o0,2,%o0
	sethi	%hi(a),%o1
	add	%o1,%lo(a),%o1
	ld	[%o0 + %o1],%o0
	ld	[%fp + .1_key],%o1
	cmp	%o0,%o1
	bne	.L22
	nop
	ba,a	.L20
.L22:
! source statement 12
	ld	[%fp + .1_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_i]
! source statement 12
	ld	[%fp + .1_i],%o0
	cmp	%o0,10
	bge	.L23
	nop
	ba,a	.L21
.L23:
.L20:
! source statement 12
	ld	[%fp + .1_i],%o0
	cmp	%o0,10
	bne	.L24
	nop
! source statement 13
	sethi	%hi(.L25),%o0
	add	%o0,%lo(.L25),%o0
	ld	[%fp + .1_key],%o1
	call	printf,2
	nop
	ba,a	.L26
.L24:
! source statement 15
	sethi	%hi(.L27),%o0
	add	%o0,%lo(.L27),%o0
	ld	[%fp + .1_key],%o1
	ld	[%fp + .1_i],%o2
	call	printf,3
	nop
.L26:
	ret
	restore
	.seg	"data"
.L27:
	.ascii "position of key %d is %d\12\0"
.L25:
	.ascii "key %d not found\12\0"
.L18:
	.ascii "%d\0"
.L16:
	.ascii "enter a key\12\0"
