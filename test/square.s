	.seg	"data"
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.1_x = -8
	save	%sp,(-104),%sp
! source statement 5
	sethi	%hi(.L01),%o0
	ldd	[%o0 + %lo(.L01)],%f0
	std	%f0,[%fp + .1_x]
! source statement 6
	sethi	%hi(.L15),%o0
	ldd	[%fp + .1_x],%f0
	ldd	[%fp + .1_x],%f2
	add	%o0,%lo(.L15),%o0
	ldd	[%fp + .1_x],%f4
	std	%f4,[%sp + 72]
	ld	[%sp + 72],%o1
	ld	[%sp + 76],%o2
	fmuld	%f0,%f2,%f0
	std	%f0,[%sp + 80]
	ld	[%sp + 80],%o3
	ld	[%sp + 84],%o4
	call	printf,5
	nop
	ret
	restore
	.seg	"data"
	.align	8
.L01:	.double	0r3.00000000000000000000000000000000e+00
.L15:
	.ascii "x = %f, x**2 = %f\12\0"
