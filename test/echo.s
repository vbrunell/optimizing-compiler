	.seg	"data"
	.align	8
sccsid:
	.word	.L12
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.1_i = -4
.1_nflg = -8
	save	%sp,(-104),%sp
.1_argc = 68
	st	%i0,[%fp + .1_argc]
.1_argv = 72
	st	%i1,[%fp + .1_argv]
! source statement 10
	st	%g0,[%fp + .1_nflg]
! source statement 11
	ld	[%fp + .1_argc],%o0
	cmp	%o0,1
	ble	.L88
	nop
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 4],%o0
	ldsb	[%o0],%o0
	cmp	%o0,45
	bne	.L88
	nop
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 4],%o0
	ldsb	[%o0 + 1],%o0
	cmp	%o0,110
	bne	.L88
	nop
! source statement 12
	ld	[%fp + .1_nflg],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_nflg]
! source statement 13
	ld	[%fp + .1_argc],%o0
	sub	%o0,1,%o0
	st	%o0,[%fp + .1_argc]
! source statement 14
	ld	[%fp + .1_argv],%o0
	add	%o0,4,%o0
	st	%o0,[%fp + .1_argv]
.L88:
! source statement 16
	mov	1,%o0
	st	%o0,[%fp + .1_i]
	ba,a	.L91
.L92:
! source statement 17
	ld	[%fp + .1_argv],%o0
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	sethi	%hi(_iob),%o2
	add	%o2,%lo(_iob),%o2
	ld	[%o0 + %o1],%o0
	add	%o2,16,%o1
	call	fputs,2
	nop
! source statement 18
	ld	[%fp + .1_i],%o0
	ld	[%fp + .1_argc],%o1
	sub	%o1,1,%o1
	cmp	%o0,%o1
	bge	.L93
	nop
! source statement 19
	sethi	%hi(_iob),%o0
	add	%o0,%lo(_iob),%o0
	add	%o0,16,%o0
	ld	[%o0],%o1
	sub	%o1,1,%o1
	st	%o1,[%o0]
	cmp	%o1,%g0
	bge	.L94
	nop
	sethi	%hi(_iob),%o1
	add	%o1,%lo(_iob),%o1
	mov	32,%o0
	add	%o1,16,%o1
	call	_flsbuf,2
	nop
	ba,a	.L95
.L94:
	mov	32,%o0
	sethi	%hi(_iob),%o1
	add	%o1,%lo(_iob),%o1
	add	%o1,20,%o1
	ld	[%o1],%o2
	add	%o2,1,%o3
	st	%o3,[%o1]
	stb	%o0,[%o2]
	and	%o0,255,%o0
.L95:
.L93:
! source statement 20
	ld	[%fp + .1_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_i]
.L91:
! source statement 20
	ld	[%fp + .1_i],%o0
	ld	[%fp + .1_argc],%o1
	cmp	%o0,%o1
	bge	.L96
	nop
	ba,a	.L92
.L96:
! source statement 21
	ld	[%fp + .1_nflg],%o0
	cmp	%o0,%g0
	bne	.L97
	nop
! source statement 22
	sethi	%hi(_iob),%o0
	add	%o0,%lo(_iob),%o0
	add	%o0,16,%o0
	ld	[%o0],%o1
	sub	%o1,1,%o1
	st	%o1,[%o0]
	cmp	%o1,%g0
	bge	.L98
	nop
	sethi	%hi(_iob),%o1
	add	%o1,%lo(_iob),%o1
	mov	10,%o0
	add	%o1,16,%o1
	call	_flsbuf,2
	nop
	ba,a	.L99
.L98:
	mov	10,%o0
	sethi	%hi(_iob),%o1
	add	%o1,%lo(_iob),%o1
	add	%o1,20,%o1
	ld	[%o1],%o2
	add	%o2,1,%o3
	st	%o3,[%o1]
	stb	%o0,[%o2]
	and	%o0,255,%o0
.L99:
.L97:
! source statement 23
	mov	%g0,%o0
	call	exit,1
	nop
	ret
	restore
	.seg	"data"
.L12:
	.ascii "@(#)echo.c\11"
	.ascii "4.1 (Berkeley) 10/1/80\0"
