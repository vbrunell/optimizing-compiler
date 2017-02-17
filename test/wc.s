	.seg	"data"
	.align	8
sccsid:
	.word	.L12
	.common	linect,4,4
	.common	wordct,4,4
	.common	charct,4,4
	.common	pagect,4,4
	.common	tlinect,4,4
	.common	twordct,4,4
	.common	tcharct,4,4
	.common	tpagect,4,4
	.align	8
	.global	wd
wd:
	.word	.L94
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.1_i = -4
.1_token = -8
.1_fp = -12
.1_c = -16
	save	%sp,(-112),%sp
.1_argc = 68
	st	%i0,[%fp + .1_argc]
.1_argv = 72
	st	%i1,[%fp + .1_argv]
.L98:
! source statement 16
	ld	[%fp + .1_argc],%o0
	cmp	%o0,1
	ble	.L99
	nop
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 4],%o0
	ldsb	[%o0],%o0
	cmp	%o0,45
	bne	.L99
	nop
! source statement 17
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 4],%o0
	ldsb	[%o0 + 1],%o0
	ba,a	.L101
.L102:
.L103:
.L104:
! source statement 19
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 4],%o0
	add	%o0,1,%o0
	sethi	%hi(wd),%o1
	st	%o0,[%o1 + %lo(wd)]
	ba,a	.L100
.L105:
! source statement 23
	sethi	%hi(_iob),%o0
	add	%o0,%lo(_iob),%o0
	sethi	%hi(.L107),%o1
	add	%o0,32,%o0
	add	%o1,%lo(.L107),%o1
	call	fprintf,2
	nop
! source statement 24
	mov	1,%o0
	call	exit,1
	nop
	ba,a	.L100
.L101:
	cmp	%o0,99
	be	.L104
	nop
	cmp	%o0,108
	be	.L102
	nop
	cmp	%o0,119
	be	.L103
	nop
	ba,a	.L105
.L100:
! source statement 26
	ld	[%fp + .1_argc],%o0
	sub	%o0,1,%o0
	st	%o0,[%fp + .1_argc]
! source statement 27
	ld	[%fp + .1_argv],%o0
	add	%o0,4,%o0
	st	%o0,[%fp + .1_argv]
	ba,a	.L98
.L99:
! source statement 30
	mov	1,%o0
	st	%o0,[%fp + .1_i]
! source statement 31
	sethi	%hi(_iob),%o0
	add	%o0,%lo(_iob),%o0
	st	%o0,[%fp + .1_fp]
.L111:
! source statement 33
	ld	[%fp + .1_argc],%o0
	cmp	%o0,1
	ble	.L113
	nop
	ld	[%fp + .1_argv],%o0
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	sethi	%hi(.L112),%o2
	ld	[%o0 + %o1],%o0
	add	%o2,%lo(.L112),%o1
	call	fopen,2
	nop
	st	%o0,[%fp + .1_fp]
	cmp	%o0,%g0
	bne	.L113
	nop
! source statement 34
	ld	[%fp + .1_argv],%o0
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	ld	[%o0 + %o1],%o0
	call	perror,1
	nop
	ba,a	.L110
.L113:
! source statement 37
	sethi	%hi(linect),%o0
	st	%g0,[%o0 + %lo(linect)]
! source statement 38
	sethi	%hi(wordct),%o0
	st	%g0,[%o0 + %lo(wordct)]
! source statement 39
	sethi	%hi(charct),%o0
	st	%g0,[%o0 + %lo(charct)]
! source statement 40
	st	%g0,[%fp + .1_token]
	ba,a	.L116
.L117:
! source statement 42
	ld	[%fp + .1_fp],%o0
	ld	[%o0],%o1
	sub	%o1,1,%o1
	st	%o1,[%o0]
	cmp	%o1,%g0
	bge	.L118
	nop
	ld	[%fp + .1_fp],%o0
	call	_filbuf,1
	nop
	ba,a	.L119
.L118:
	ld	[%fp + .1_fp],%o0
	add	%o0,4,%o0
	ld	[%o0],%o1
	add	%o1,1,%o2
	st	%o2,[%o0]
	ldub	[%o1],%o0
.L119:
	st	%o0,[%fp + .1_c]
! source statement 43
	ld	[%fp + .1_c],%o0
	cmp	%o0,(-1)
	bne	.L120
	nop
	ba,a	.L115
.L120:
! source statement 45
	sethi	%hi(charct),%o0
	ld	[%o0 + %lo(charct)],%o0
	add	%o0,1,%o0
	sethi	%hi(charct),%o1
	st	%o0,[%o1 + %lo(charct)]
! source statement 46
	ld	[%fp + .1_c],%o0
	cmp	%o0,32
	ble	.L121
	nop
	ld	[%fp + .1_c],%o0
	cmp	%o0,127
	bge	.L121
	nop
! source statement 47
	ld	[%fp + .1_token],%o0
	cmp	%o0,%g0
	bne	.L122
	nop
! source statement 48
	sethi	%hi(wordct),%o0
	ld	[%o0 + %lo(wordct)],%o0
	add	%o0,1,%o0
	sethi	%hi(wordct),%o1
	st	%o0,[%o1 + %lo(wordct)]
! source statement 49
	ld	[%fp + .1_token],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_token]
.L122:
	ba,a	.L114
.L121:
! source statement 53
	ld	[%fp + .1_c],%o0
	cmp	%o0,10
	bne	.L123
	nop
! source statement 54
	sethi	%hi(linect),%o0
	ld	[%o0 + %lo(linect)],%o0
	add	%o0,1,%o0
	sethi	%hi(linect),%o1
	st	%o0,[%o1 + %lo(linect)]
	ba,a	.L124
.L123:
! source statement 56
	ld	[%fp + .1_c],%o0
	cmp	%o0,32
	be	.L125
	nop
	ld	[%fp + .1_c],%o0
	cmp	%o0,9
	be	.L125
	nop
	ba,a	.L114
.L125:
.L124:
! source statement 58
	st	%g0,[%fp + .1_token]
.L114:
.L116:
! source statement 59
	ba,a	.L117
.L115:
! source statement 61
	sethi	%hi(wd),%o0
	sethi	%hi(charct),%o1
	sethi	%hi(wordct),%o2
	sethi	%hi(linect),%o3
	ld	[%o0 + %lo(wd)],%o0
	ld	[%o1 + %lo(charct)],%o1
	ld	[%o2 + %lo(wordct)],%o2
	ld	[%o3 + %lo(linect)],%o3
	call	wcp,4
	nop
! source statement 62
	ld	[%fp + .1_argc],%o0
	cmp	%o0,1
	ble	.L127
	nop
! source statement 63
	sethi	%hi(.L128),%o0
	ld	[%fp + .1_argv],%o1
	ld	[%fp + .1_i],%o2
	sll	%o2,2,%o2
	add	%o0,%lo(.L128),%o0
	ld	[%o1 + %o2],%o1
	call	printf,2
	nop
	ba,a	.L129
.L127:
! source statement 65
	sethi	%hi(.L130),%o0
	add	%o0,%lo(.L130),%o0
	call	printf,1
	nop
.L129:
! source statement 66
	ld	[%fp + .1_fp],%o0
	call	fclose,1
	nop
! source statement 67
	sethi	%hi(tlinect),%o0
	ld	[%o0 + %lo(tlinect)],%o0
	sethi	%hi(linect),%o1
	ld	[%o1 + %lo(linect)],%o1
	add	%o0,%o1,%o0
	sethi	%hi(tlinect),%o1
	st	%o0,[%o1 + %lo(tlinect)]
! source statement 68
	sethi	%hi(twordct),%o0
	ld	[%o0 + %lo(twordct)],%o0
	sethi	%hi(wordct),%o1
	ld	[%o1 + %lo(wordct)],%o1
	add	%o0,%o1,%o0
	sethi	%hi(twordct),%o1
	st	%o0,[%o1 + %lo(twordct)]
! source statement 69
	sethi	%hi(tcharct),%o0
	ld	[%o0 + %lo(tcharct)],%o0
	sethi	%hi(charct),%o1
	ld	[%o1 + %lo(charct)],%o1
	add	%o0,%o1,%o0
	sethi	%hi(tcharct),%o1
	st	%o0,[%o1 + %lo(tcharct)]
.L110:
! source statement 70
	ld	[%fp + .1_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_i]
	ld	[%fp + .1_argc],%o1
	cmp	%o0,%o1
	bge	.L131
	nop
	ba,a	.L111
.L131:
! source statement 71
	ld	[%fp + .1_argc],%o0
	cmp	%o0,2
	ble	.L132
	nop
! source statement 72
	sethi	%hi(wd),%o0
	sethi	%hi(tcharct),%o1
	sethi	%hi(twordct),%o2
	sethi	%hi(tlinect),%o3
	ld	[%o0 + %lo(wd)],%o0
	ld	[%o1 + %lo(tcharct)],%o1
	ld	[%o2 + %lo(twordct)],%o2
	ld	[%o3 + %lo(tlinect)],%o3
	call	wcp,4
	nop
! source statement 73
	sethi	%hi(.L133),%o0
	add	%o0,%lo(.L133),%o0
	call	printf,1
	nop
.L132:
! source statement 75
	mov	%g0,%o0
	call	exit,1
	nop
	ret
	restore
	.seg	"data"
.L133:
	.ascii " total\12\0"
.L130:
	.ascii "\12\0"
.L128:
	.ascii " %s\12\0"
.L112:
	.ascii "r\0"
.L107:
	.ascii "Usage: wc [-lwc] [files]\12\0"
.L94:
	.ascii "lwc\0"
.L12:
	.ascii "@(#)wc.c\11"
	.ascii "4.6 (Berkeley) 6/1/83\0"
	.seg	"text"
	.align	8
	.global	wcp
	.proc	0
wcp:
	save	%sp,(-96),%sp
.2_wd = 68
	st	%i0,[%fp + .2_wd]
.2_charct = 72
	st	%i1,[%fp + .2_charct]
.2_wordct = 76
	st	%i2,[%fp + .2_wordct]
.2_linect = 80
	st	%i3,[%fp + .2_linect]
.L136:
! source statement 82
	ld	[%fp + .2_wd],%o0
	ldsb	[%o0],%o0
	cmp	%o0,%g0
	be	.L137
	nop
! source statement 82
	ld	[%fp + .2_wd],%o0
	add	%o0,1,%o1
	st	%o1,[%fp + .2_wd]
	ldsb	[%o0],%o0
	ba,a	.L139
.L140:
! source statement 84
	ld	[%fp + .2_linect],%o0
	call	ipr,1
	nop
	ba,a	.L138
.L142:
! source statement 88
	ld	[%fp + .2_wordct],%o0
	call	ipr,1
	nop
	ba,a	.L138
.L143:
! source statement 92
	ld	[%fp + .2_charct],%o0
	call	ipr,1
	nop
	ba,a	.L138
.L139:
	cmp	%o0,99
	be	.L143
	nop
	cmp	%o0,108
	be	.L140
	nop
	cmp	%o0,119
	be	.L142
	nop
.L138:
	ba,a	.L136
.L137:
	ret
	restore
	.seg	"data"
	.seg	"text"
	.align	8
	.global	ipr
	.proc	0
ipr:
	save	%sp,(-96),%sp
.3_num = 68
	st	%i0,[%fp + .3_num]
! source statement 101
	sethi	%hi(.L146),%o0
	add	%o0,%lo(.L146),%o0
	ld	[%fp + .3_num],%o1
	call	printf,2
	nop
	ret
	restore
	.seg	"data"
.L146:
	.ascii " %7ld\0"
