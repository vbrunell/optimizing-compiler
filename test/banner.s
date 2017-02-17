	.seg	"data"
	.align	8
	.global	btab
btab:
	.ascii "\10\10\10\10\10\0\10\24\24\24\0\0\0\0\24\24\177\24\177\24"
	.ascii "\24\10\34(\34\12\34\10qRt\10\27%G\34\"\24\10\25\"\35\4"
	.ascii "\10\20\0\0\0\0\1\2\4\4\4\2\1@ \20\20\20 @I*\34\177\34*"
	.ascii "I\0\10\10>\10\10\0\0\0\0\0\0 @\0\0\0<\0\0\0\0\0\0\0\0"
	.ascii "\0 \1\2\4\10\20 @\34\"EIQ\"\34\30\10\10\10\10\10\34\34"
	.ascii "\"\2\4\10\20>\34\"\2\4\2\"\34\2\6\12\22?\2\2>  <\2\"\34"
	.ascii "\2\4\10\36!!\36?\1\2\4\10\20 \34\"\"\34\"\"\34\36!!\36"
	.ascii "\4\10\20\0\0\0\10\0\10\0\0\0\0\10\0\10\20\2\4\10\20\10"
	.ascii "\4\2\0\0<\0<\0\0 \20\10\4\10\20 \34\"\2\4\10\0\10>AO"
	.ascii "IO@>\34\"A\177AAA~AA~AA~\34\"@@@\"\34|BAAAB|~@@x@@~~"
	.ascii "@@x@@@\36!@@G!\36AAA\177AAA\34\10\10\10\10\10\34\2\2\2"
	.ascii "\2\2\"\34BDHPhDB@@@@@@~AcUIAAAAaQIECA>AAAAA>~AA~@@@>"
	.ascii "AAIEB=~AA~DBA>A@>\1A>\177\10\10\10\10\10\10AAAAAA>AA"
	.ascii "AA\"\24\10AAAIUcAA\"\24\10\24\"AA\"\24\10\10\10\10\177\2"
	.ascii "\4\10\20 \177\7\4\4\4\4\4\7@ \20\10\4\2\1p\20\20\20\20"
	.ascii "\20p\10\24\"A\0\0\0\0\0\0\0\0\0\177\20\10\4\0\0\0\0\0"
	.ascii ">\2>\"?\0  <\"\"|\0\0\36   \36\0\2\2\36\"\"\37\0\0\36!"
	.ascii "? \36\0\10\24\20"
	.ascii "8\20\20\0\0\34\"\"\36\2\14  >!!!\0\10\0\10\10\10\34\0"
	.ascii "\2\0\2\2\2\22\14 $(0($\0\30\10\10\10\10\34\0\0vIIII\0"
	.ascii "\0>!!!!\0\0\36!!!\36\0\0<\"\"<  \0\36\"\"\36\2\3\0,\""
	.ascii "   \0\0\36 \36\1\36\0\20<\20\20\22\14\0\0!!!!\35\0\0\""
	.ascii "\"\24\24\10\0\0AAII6\0\0\"\24\10\24\"\0\0\"\"\"\36\2\14"
	.ascii "\0>\4\10\20>\0\3\4\4\30\4\4\3\10\10\10\10\10\10\10`\20"
	.ascii "\20\14\20\20`0I\6\0\0\0\0\177\177\177\177\177\177\177\0"
	.ascii "\0\0"
	.seg	"text"
	.align	8
	.global	mkline
	.proc	1
mkline:
.1_rs = -4
.1_rl = -8
.1_bit = -12
.1_i = -16
.1_j = -20
.1_m = -24
	save	%sp,(-88),%sp
.1_s = 68
	st	%i0,[%fp + .1_s]
.1_prch = 75
	stb	%i1,[%fp + .1_prch]
.1_line = 76
	st	%i2,[%fp + .1_line]
.1_space = 80
	st	%i3,[%fp + .1_space]
! source statement 122
	ld	[%fp + .1_space],%o0
	st	%o0,[%fp + .1_rl]
! source statement 123
	st	%g0,[%fp + .1_i]
.L89:
! source statement 124
	st	%g0,[%fp + .1_j]
! source statement 125
	ld	[%fp + .1_s],%o0
	st	%o0,[%fp + .1_rs]
	ba,a	.L92
.L93:
! source statement 126
	ld	[%fp + .1_rs],%o0
	add	%o0,1,%o1
	st	%o1,[%fp + .1_rs]
	ldsb	[%o0],%o0
	and	%o0,127,%o0
	sub	%o0,33,%o0
	st	%o0,[%fp + .1_bit]
	cmp	%o0,%g0
	bge	.L94
	nop
	mov	%g0,%o0
	ba,a	.L95
.L94:
	ld	[%fp + .1_bit],%o0
	smul	%o0,7,%o0
	sethi	%hi(btab),%o1
	add	%o1,%lo(btab),%o1
	add	%o0,%o1,%o0
	ld	[%fp + .1_i],%o1
	ldsb	[%o0 + %o1],%o0
.L95:
	st	%o0,[%fp + .1_bit]
! source statement 127
	mov	64,%o0
	st	%o0,[%fp + .1_m]
	ba,a	.L98
.L99:
! source statement 128
	ld	[%fp + .1_m],%o0
	ld	[%fp + .1_bit],%o1
	andcc	%o0,%o1,%g0
	be	.L100
	nop
	ldsb	[%fp + .1_prch],%o0
	cmp	%o0,%g0
	be	.L102
	nop
	ldsb	[%fp + .1_prch],%o0
	ba,a	.L103
.L102:
	ld	[%fp + .1_rs],%o0
	ldsb	[%o0 + (-1)],%o0
.L103:
	ba,a	.L101
.L100:
	mov	32,%o0
.L101:
	ld	[%fp + .1_rl],%o1
	add	%o1,1,%o2
	st	%o2,[%fp + .1_rl]
	stb	%o0,[%o1]
! source statement 129
	ld	[%fp + .1_bit],%o0
	ld	[%fp + .1_m],%o1
	andn	%o0,%o1,%o0
	st	%o0,[%fp + .1_bit]
! source statement 130
	ld	[%fp + .1_m],%o0
	sra	%o0,1,%o0
	st	%o0,[%fp + .1_m]
.L98:
! source statement 130
	ld	[%fp + .1_bit],%o0
	cmp	%o0,%g0
	bne	.L99
	nop
! source statement 131
	mov	9,%o0
	ld	[%fp + .1_rl],%o1
	add	%o1,1,%o2
	st	%o2,[%fp + .1_rl]
	stb	%o0,[%o1]
! source statement 132
	ld	[%fp + .1_j],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_j]
.L92:
! source statement 132
	ld	[%fp + .1_rs],%o0
	ldsb	[%o0],%o0
	cmp	%o0,%g0
	be	.L104
	nop
	ld	[%fp + .1_j],%o0
	ld	[%fp + .1_line],%o1
	cmp	%o0,%o1
	bge	.L104
	nop
	ba,a	.L93
.L104:
.L105:
! source statement 133
	ld	[%fp + .1_rl],%o0
	sub	%o0,1,%o0
	st	%o0,[%fp + .1_rl]
	ldsb	[%o0],%o0
	cmp	%o0,9
	bne	.L106
	nop
	ba,a	.L105
.L106:
! source statement 135
	ld	[%fp + .1_rl],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_rl]
! source statement 136
	mov	10,%o0
	ld	[%fp + .1_rl],%o1
	add	%o1,1,%o2
	st	%o2,[%fp + .1_rl]
	stb	%o0,[%o1]
! source statement 137
	ld	[%fp + .1_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_i]
! source statement 137
	ld	[%fp + .1_i],%o0
	cmp	%o0,7
	bge	.L107
	nop
	ba,a	.L89
.L107:
! source statement 138
	ld	[%fp + .1_rl],%o0
	stb	%g0,[%o0]
! source statement 139
	ld	[%fp + .1_space],%i0
	ret
	restore
	.seg	"data"
	.common	.L111,256,8
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.2_prch = -1
.2_space = -8
.2_line = -12
	save	%sp,(-104),%sp
.2_argc = 68
	st	%i0,[%fp + .2_argc]
.2_argv = 72
	st	%i1,[%fp + .2_argv]
! source statement 151
	mov	42,%o0
	stb	%o0,[%fp + .2_prch]
! source statement 152
	mov	10,%o0
	st	%o0,[%fp + .2_line]
.L113:
! source statement 153
	ld	[%fp + .2_argv],%o0
	add	%o0,4,%o0
	st	%o0,[%fp + .2_argv]
	ld	[%o0],%o0
	ldsb	[%o0],%o0
	cmp	%o0,45
	bne	.L114
	nop
! source statement 154
	ld	[%fp + .2_argc],%o0
	sub	%o0,1,%o0
	st	%o0,[%fp + .2_argc]
! source statement 155
	ld	[%fp + .2_argv],%o0
	ld	[%o0],%o1
	add	%o1,1,%o1
	st	%o1,[%o0]
	ldsb	[%o1],%o0
	ba,a	.L116
.L117:
! source statement 158
	ld	[%fp + .2_argv],%o0
	ld	[%o0],%o1
	add	%o1,1,%o1
	st	%o1,[%o0]
	ldsb	[%o1],%o0
	stb	%o0,[%fp + .2_prch]
	ba,a	.L115
.L118:
.L119:
! source statement 163
	ld	[%fp + .2_argv],%o0
	ld	[%o0],%o1
	add	%o1,1,%o1
	st	%o1,[%o0]
	mov	%o1,%o0
	call	myatoi,1
	nop
	sra	%o0,3,%o0
	st	%o0,[%fp + .2_line]
	ba,a	.L115
.L116:
	cmp	%o0,99
	be	.L117
	nop
	cmp	%o0,108
	be	.L118
	nop
	cmp	%o0,119
	be	.L119
	nop
.L115:
	ba,a	.L113
.L114:
! source statement 167
	ld	[%fp + .2_line],%o0
	sll	%o0,6,%o0
	call	malloc,1
	nop
	st	%o0,[%fp + .2_space]
! source statement 168
	sethi	%hi(.L121),%o0
	add	%o0,%lo(.L121),%o0
	call	printf,1
	nop
! source statement 169
	ld	[%fp + .2_argc],%o0
	sub	%o0,1,%o0
	st	%o0,[%fp + .2_argc]
	cmp	%o0,%g0
	be	.L122
	nop
.L125:
! source statement 171
	sethi	%hi(.L126),%l0
	ld	[%fp + .2_argv],%o0
	add	%o0,4,%o1
	st	%o1,[%fp + .2_argv]
	ld	[%o0],%o0
	ldsb	[%fp + .2_prch],%o1
	ld	[%fp + .2_line],%o2
	ld	[%fp + .2_space],%o3
	call	mkline,4
	nop
	mov	%o0,%o1
	add	%l0,%lo(.L126),%o0
	mov	%o1,%o1
	call	printf,2
	nop
! source statement 172
	ld	[%fp + .2_argc],%o0
	sub	%o0,1,%o0
	st	%o0,[%fp + .2_argc]
	cmp	%o0,%g0
	bne	.L125
	nop
	ba,a	.L127
.L122:
.L128:
! source statement 176
	sethi	%hi(.L111),%o0
	sethi	%hi(_iob),%o2
	add	%o0,%lo(.L111),%o0
	mov	256,%o1
	add	%o2,%lo(_iob),%o2
	call	fgets,3
	nop
	cmp	%o0,%g0
	be	.L129
	nop
! source statement 177
	sethi	%hi(.L130),%l0
	sethi	%hi(.L111),%o0
	add	%o0,%lo(.L111),%o0
	ldsb	[%fp + .2_prch],%o1
	ld	[%fp + .2_line],%o2
	ld	[%fp + .2_space],%o3
	call	mkline,4
	nop
	mov	%o0,%o1
	add	%l0,%lo(.L130),%o0
	mov	%o1,%o1
	call	printf,2
	nop
	ba,a	.L128
.L129:
.L127:
	ret
	restore
	.seg	"data"
.L130:
	.ascii "\12%s\0"
.L126:
	.ascii "\12%s\0"
.L121:
	.ascii "\12\0"
	.seg	"text"
	.align	8
	.global	myatoi
	.proc	1
myatoi:
.3_i = -4
.3_retval = -8
	save	%sp,(-72),%sp
.3_str = 68
	st	%i0,[%fp + .3_str]
! source statement 184
	st	%g0,[%fp + .3_i]
! source statement 185
	st	%g0,[%fp + .3_retval]
.L133:
! source statement 187
	ld	[%fp + .3_str],%o0
	ld	[%fp + .3_i],%o1
	ldsb	[%o0 + %o1],%o0
	cmp	%o0,48
	bl	.L134
	nop
	ld	[%fp + .3_str],%o0
	ld	[%fp + .3_i],%o1
	ldsb	[%o0 + %o1],%o0
	cmp	%o0,57
	bg	.L134
	nop
! source statement 188
	ld	[%fp + .3_retval],%o0
	smul	%o0,10,%o0
	st	%o0,[%fp + .3_retval]
! source statement 189
	ld	[%fp + .3_retval],%o0
	ld	[%fp + .3_str],%o1
	ld	[%fp + .3_i],%o2
	add	%o2,1,%o3
	st	%o3,[%fp + .3_i]
	ldsb	[%o1 + %o2],%o1
	sub	%o1,48,%o1
	add	%o0,%o1,%o0
	st	%o0,[%fp + .3_retval]
	ba,a	.L133
.L134:
! source statement 191
	ld	[%fp + .3_retval],%i0
	ret
	restore
	.seg	"data"
