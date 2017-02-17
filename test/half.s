	.seg	"data"
	.seg	"text"
	.align	8
	.global	half
	.proc	7
half:
.1_d = -8
	save	%sp,(-80),%sp
.1_i = 68
	st	%i0,[%fp + .1_i]
! source statement 6
	ld	[%fp + .1_i],%o0
	st	%o0,[%sp + -12]
	ld	[%sp + -12],%f0
	fitod	%f0,%f0
	std	%f0,[%fp + .1_d]
! source statement 7
	ldd	[%fp + .1_d],%f0
	sethi	%hi(.L01),%o0
	ldd	[%o0 + %lo(.L01)],%f2
	fdivd	%f0,%f2,%f0
	std	%f0,[%fp + .1_d]
! source statement 8
	ldd	[%fp + .1_d],%f0
	ret
	restore
	ret
	restore
	.seg	"data"
	.align	8
.L01:	.double	0r2.00000000000000000000000000000000e+00
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.2_n = -4
	save	%sp,(-96),%sp
! source statement 15
	mov	3,%o0
	st	%o0,[%fp + .2_n]
! source statement 16
	sethi	%hi(.L18),%l0
	ld	[%fp + .2_n],%o0
	call	half,1
	nop
	add	%l0,%lo(.L18),%o0
	ld	[%fp + .2_n],%o1
	fmovs	%f0,%f0
	fmovs	%f1,%f1
	st	%f0,[%sp + 76]
	st	%f1,[%sp + 80]
	ld	[%sp + 76],%o2
	ld	[%sp + 80],%o3
	call	printf,4
	nop
	ret
	restore
	.seg	"data"
.L18:
	.ascii "n = %d, n/2 = %f\12\0"
