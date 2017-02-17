	.seg	"data"
	.align	8
	.global	dayw
dayw:
	.ascii " S  M Tu  W Th  F  S\0\0\0\0"
	.align	8
	.global	smon
smon:
	.word	.L13
	.word	.L14
	.word	.L15
	.word	.L16
	.word	.L17
	.word	.L18
	.word	.L19
	.word	.L20
	.word	.L21
	.word	.L22
	.word	.L23
	.word	.L24
	.common	string,432,4
	.seg	"text"
	.align	8
	.global	main
	.proc	0
main:
.1_y = -4
.1_i = -8
.1_j = -12
.1_m = -16
	save	%sp,(-112),%sp
.1_argc = 68
	st	%i0,[%fp + .1_argc]
.1_argv = 72
	st	%i1,[%fp + .1_argv]
! source statement 17
	ld	[%fp + .1_argc],%o0
	cmp	%o0,2
	bge	.L29
	nop
! source statement 18
	sethi	%hi(.L31),%o0
	add	%o0,%lo(.L31),%o0
	call	printf,1
	nop
! source statement 19
	mov	%g0,%o0
	call	exit,1
	nop
.L29:
! source statement 21
	ld	[%fp + .1_argc],%o0
	cmp	%o0,2
	bne	.L33
	nop
	ba,a	.L34
.L33:
! source statement 28
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 4],%o0
	call	number,1
	nop
	st	%o0,[%fp + .1_m]
! source statement 29
	ld	[%fp + .1_m],%o0
	cmp	%o0,1
	bge	.L37
	nop
	ba,a	.L38
.L37:
	ld	[%fp + .1_m],%o0
	cmp	%o0,12
	ble	.L36
	nop
.L38:
	ba,a	.L39
.L36:
! source statement 31
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 8],%o0
	call	number,1
	nop
	st	%o0,[%fp + .1_y]
! source statement 32
	ld	[%fp + .1_y],%o0
	cmp	%o0,1
	bge	.L41
	nop
	ba,a	.L42
.L41:
	ld	[%fp + .1_y],%o0
	sethi	%hi(9999),%o1
	or	%o1,%lo(9999),%o1
	cmp	%o0,%o1
	ble	.L40
	nop
.L42:
	ba,a	.L39
.L40:
! source statement 34
	sethi	%hi(.L43),%o0
	ld	[%fp + .1_m],%o1
	sub	%o1,1,%o1
	sll	%o1,2,%o1
	sethi	%hi(smon),%o2
	add	%o2,%lo(smon),%o2
	add	%o0,%lo(.L43),%o0
	ld	[%o1 + %o2],%o1
	ld	[%fp + .1_y],%o2
	call	printf,3
	nop
! source statement 35
	sethi	%hi(.L44),%o0
	sethi	%hi(dayw),%o1
	add	%o0,%lo(.L44),%o0
	add	%o1,%lo(dayw),%o1
	call	printf,2
	nop
! source statement 36
	sethi	%hi(string),%o2
	ld	[%fp + .1_m],%o0
	ld	[%fp + .1_y],%o1
	add	%o2,%lo(string),%o2
	mov	24,%o3
	call	cal,4
	nop
! source statement 37
	st	%g0,[%fp + .1_i]
.L48:
! source statement 38
	ld	[%fp + .1_i],%o0
	sethi	%hi(string),%o1
	add	%o1,%lo(string),%o1
	add	%o0,%o1,%o0
	mov	24,%o1
	call	pstr,2
	nop
! source statement 38
	ld	[%fp + .1_i],%o0
	add	%o0,24,%o0
	st	%o0,[%fp + .1_i]
! source statement 38
	ld	[%fp + .1_i],%o0
	cmp	%o0,144
	bge	.L50
	nop
	ba,a	.L48
.L50:
! source statement 39
	mov	%g0,%o0
	call	exit,1
	nop
.L34:
! source statement 46
	ld	[%fp + .1_argv],%o0
	ld	[%o0 + 4],%o0
	call	number,1
	nop
	st	%o0,[%fp + .1_y]
! source statement 47
	ld	[%fp + .1_y],%o0
	cmp	%o0,1
	bge	.L52
	nop
	ba,a	.L53
.L52:
	ld	[%fp + .1_y],%o0
	sethi	%hi(9999),%o1
	or	%o1,%lo(9999),%o1
	cmp	%o0,%o1
	ble	.L51
	nop
.L53:
	ba,a	.L39
.L51:
! source statement 49
	sethi	%hi(.L54),%o0
	add	%o0,%lo(.L54),%o0
	call	printf,1
	nop
! source statement 50
	sethi	%hi(.L55),%o0
	add	%o0,%lo(.L55),%o0
	ld	[%fp + .1_y],%o1
	call	printf,2
	nop
! source statement 51
	sethi	%hi(.L56),%o0
	add	%o0,%lo(.L56),%o0
	call	printf,1
	nop
! source statement 52
	st	%g0,[%fp + .1_i]
.L59:
! source statement 53
	st	%g0,[%fp + .1_j]
.L62:
! source statement 54
	ld	[%fp + .1_j],%o0
	sethi	%hi(string),%o1
	add	%o1,%lo(string),%o1
	stb	%g0,[%o0 + %o1]
! source statement 54
	ld	[%fp + .1_j],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .1_j]
! source statement 54
	ld	[%fp + .1_j],%o0
	cmp	%o0,432
	bge	.L63
	nop
	ba,a	.L62
.L63:
! source statement 55
	sethi	%hi(.L64),%o0
	ld	[%fp + .1_i],%o1
	sll	%o1,2,%o1
	sethi	%hi(smon),%o2
	add	%o2,%lo(smon),%o2
	add	%o0,%lo(.L64),%o0
	ld	[%o1 + %o2],%o1
	call	printf,2
	nop
! source statement 56
	sethi	%hi(.L65),%o0
	ld	[%fp + .1_i],%o1
	add	%o1,1,%o1
	sll	%o1,2,%o1
	sethi	%hi(smon),%o2
	add	%o2,%lo(smon),%o2
	add	%o0,%lo(.L65),%o0
	ld	[%o1 + %o2],%o1
	call	printf,2
	nop
! source statement 57
	sethi	%hi(.L66),%o0
	ld	[%fp + .1_i],%o1
	add	%o1,2,%o1
	sll	%o1,2,%o1
	sethi	%hi(smon),%o2
	add	%o2,%lo(smon),%o2
	add	%o0,%lo(.L66),%o0
	ld	[%o1 + %o2],%o1
	call	printf,2
	nop
! source statement 58
	sethi	%hi(.L67),%o0
	sethi	%hi(dayw),%o1
	sethi	%hi(dayw),%o2
	sethi	%hi(dayw),%o3
	add	%o0,%lo(.L67),%o0
	add	%o1,%lo(dayw),%o1
	add	%o2,%lo(dayw),%o2
	add	%o3,%lo(dayw),%o3
	call	printf,4
	nop
! source statement 59
	ld	[%fp + .1_i],%o0
	sethi	%hi(string),%o2
	add	%o0,1,%o0
	ld	[%fp + .1_y],%o1
	add	%o2,%lo(string),%o2
	mov	72,%o3
	call	cal,4
	nop
! source statement 60
	ld	[%fp + .1_i],%o0
	sethi	%hi(string),%o2
	add	%o2,%lo(string),%o2
	add	%o0,2,%o0
	ld	[%fp + .1_y],%o1
	add	%o2,23,%o2
	mov	72,%o3
	call	cal,4
	nop
! source statement 61
	ld	[%fp + .1_i],%o0
	sethi	%hi(string),%o2
	add	%o2,%lo(string),%o2
	add	%o0,3,%o0
	ld	[%fp + .1_y],%o1
	add	%o2,46,%o2
	mov	72,%o3
	call	cal,4
	nop
! source statement 62
	st	%g0,[%fp + .1_j]
.L70:
! source statement 63
	ld	[%fp + .1_j],%o0
	sethi	%hi(string),%o1
	add	%o1,%lo(string),%o1
	add	%o0,%o1,%o0
	mov	72,%o1
	call	pstr,2
	nop
! source statement 63
	ld	[%fp + .1_j],%o0
	add	%o0,72,%o0
	st	%o0,[%fp + .1_j]
! source statement 63
	ld	[%fp + .1_j],%o0
	cmp	%o0,432
	bge	.L71
	nop
	ba,a	.L70
.L71:
! source statement 64
	ld	[%fp + .1_i],%o0
	add	%o0,3,%o0
	st	%o0,[%fp + .1_i]
! source statement 64
	ld	[%fp + .1_i],%o0
	cmp	%o0,12
	bge	.L72
	nop
	ba,a	.L59
.L72:
! source statement 65
	sethi	%hi(.L73),%o0
	add	%o0,%lo(.L73),%o0
	call	printf,1
	nop
! source statement 66
	mov	%g0,%o0
	call	exit,1
	nop
.L39:
! source statement 69
	sethi	%hi(.L74),%o0
	add	%o0,%lo(.L74),%o0
	call	printf,1
	nop
	ret
	restore
	.seg	"data"
.L74:
	.ascii "Bad argument\12\0"
.L73:
	.ascii "\12\12\12\0"
.L67:
	.ascii "%s   %s   %s\12\0"
.L66:
	.ascii "\11\11       %.3s\12\0"
.L65:
	.ascii "\11\11\11%.3s\0"
.L64:
	.ascii "\11 %.3s\0"
.L56:
	.ascii "\12\0"
.L55:
	.ascii "\11\11\11\11%u\12\0"
.L54:
	.ascii "\12\12\12\0"
.L44:
	.ascii "%s\12\0"
.L43:
	.ascii "   %s %u\12\0"
.L31:
	.ascii "usage: cal [month] year\12\0"
.L24:
	.ascii "December\0"
.L23:
	.ascii "November\0"
.L22:
	.ascii "October\0"
.L21:
	.ascii "September\0"
.L20:
	.ascii "August\0"
.L19:
	.ascii "July\0"
.L18:
	.ascii "June\0"
.L17:
	.ascii "May\0"
.L16:
	.ascii "April\0"
.L15:
	.ascii "March\0"
.L14:
	.ascii "February\0"
.L13:
	.ascii "January\0"
	.seg	"text"
	.align	8
	.global	number
	.proc	1
number:
.2_n = -4
.2_c = -8
.2_s = -12
	save	%sp,(-80),%sp
.2_str = 68
	st	%i0,[%fp + .2_str]
! source statement 78
	st	%g0,[%fp + .2_n]
! source statement 79
	ld	[%fp + .2_str],%o0
	st	%o0,[%fp + .2_s]
.L77:
! source statement 80
	ld	[%fp + .2_s],%o0
	add	%o0,1,%o1
	st	%o1,[%fp + .2_s]
	ldsb	[%o0],%o0
	st	%o0,[%fp + .2_c]
	cmp	%o0,%g0
	be	.L78
	nop
! source statement 81
	ld	[%fp + .2_c],%o0
	cmp	%o0,48
	bge	.L80
	nop
	ba,a	.L81
.L80:
	ld	[%fp + .2_c],%o0
	cmp	%o0,57
	ble	.L79
	nop
.L81:
! source statement 82
	mov	%g0,%i0
	ret
	restore
.L79:
! source statement 83
	ld	[%fp + .2_n],%o0
	smul	%o0,10,%o0
	ld	[%fp + .2_c],%o1
	add	%o0,%o1,%o0
	sub	%o0,48,%o0
	st	%o0,[%fp + .2_n]
	ba,a	.L77
.L78:
! source statement 85
	ld	[%fp + .2_n],%i0
	ret
	restore
	.seg	"data"
	.seg	"text"
	.align	8
	.global	pstr
	.proc	0
pstr:
.3_i = -4
.3_s = -8
	save	%sp,(-104),%sp
.3_str = 68
	st	%i0,[%fp + .3_str]
.3_n = 72
	st	%i1,[%fp + .3_n]
! source statement 94
	ld	[%fp + .3_str],%o0
	st	%o0,[%fp + .3_s]
! source statement 95
	ld	[%fp + .3_n],%o0
	st	%o0,[%fp + .3_i]
.L84:
! source statement 96
	ld	[%fp + .3_i],%o0
	sub	%o0,1,%o1
	st	%o1,[%fp + .3_i]
	cmp	%o0,%g0
	be	.L85
	nop
! source statement 97
	ld	[%fp + .3_s],%o0
	add	%o0,1,%o1
	st	%o1,[%fp + .3_s]
	ldsb	[%o0],%o0
	cmp	%o0,%g0
	bne	.L86
	nop
! source statement 98
	mov	32,%o0
	ld	[%fp + .3_s],%o1
	stb	%o0,[%o1 + (-1)]
.L86:
	ba,a	.L84
.L85:
! source statement 99
	ld	[%fp + .3_n],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .3_i]
.L87:
! source statement 100
	ld	[%fp + .3_i],%o0
	sub	%o0,1,%o1
	st	%o1,[%fp + .3_i]
	cmp	%o0,%g0
	be	.L88
	nop
! source statement 101
	ld	[%fp + .3_s],%o0
	sub	%o0,1,%o0
	st	%o0,[%fp + .3_s]
	ldsb	[%o0],%o0
	cmp	%o0,32
	be	.L89
	nop
	ba,a	.L88
.L89:
	ba,a	.L87
.L88:
! source statement 103
	ld	[%fp + .3_s],%o0
	stb	%g0,[%o0 + 1]
! source statement 104
	sethi	%hi(.L90),%o0
	add	%o0,%lo(.L90),%o0
	ld	[%fp + .3_str],%o1
	call	printf,2
	nop
	ret
	restore
	.seg	"data"
.L90:
	.ascii "%s\12\0"
	.align	8
	.global	mon
mon:
	.ascii "\0\37\35\37\36\37\36\37\37\36\37\36\37\0\0\0"
	.seg	"text"
	.align	8
	.global	cal
	.proc	0
cal:
.4_d = -4
.4_i = -8
.4_s = -12
	save	%sp,(-104),%sp
.4_m = 68
	st	%i0,[%fp + .4_m]
.4_y = 72
	st	%i1,[%fp + .4_y]
.4_p = 76
	st	%i2,[%fp + .4_p]
.4_w = 80
	st	%i3,[%fp + .4_w]
! source statement 120
	ld	[%fp + .4_p],%o0
	st	%o0,[%fp + .4_s]
! source statement 121
	ld	[%fp + .4_y],%o0
	call	jan1,1
	nop
	st	%o0,[%fp + .4_d]
! source statement 122
	mov	29,%o0
	sethi	%hi(mon),%o1
	add	%o1,%lo(mon),%o1
	stb	%o0,[%o1 + 2]
! source statement 123
	mov	30,%o0
	sethi	%hi(mon),%o1
	add	%o1,%lo(mon),%o1
	stb	%o0,[%o1 + 9]
! source statement 125
	ld	[%fp + .4_y],%o0
	add	%o0,1,%o0
	call	jan1,1
	nop
	add	%o0,7,%o0
	ld	[%fp + .4_d],%o1
	sub	%o0,%o1,%o0
	mov	7,%o1
	call	.rem,2
	nop
	mov	%o0,%o0
	ba,a	.L96
.L97:
! source statement 131
	mov	28,%o0
	sethi	%hi(mon),%o1
	add	%o1,%lo(mon),%o1
	stb	%o0,[%o1 + 2]
	ba,a	.L95
.L98:
! source statement 138
	mov	19,%o0
	sethi	%hi(mon),%o1
	add	%o1,%lo(mon),%o1
	stb	%o0,[%o1 + 9]
	ba,a	.L95
.L99:
	ba,a	.L95
.L96:
	cmp	%o0,1
	be	.L97
	nop
	cmp	%o0,2
	be	.L99
	nop
	ba,a	.L98
.L95:
! source statement 147
	mov	1,%o0
	st	%o0,[%fp + .4_i]
	ba,a	.L102
.L103:
! source statement 148
	ld	[%fp + .4_d],%o0
	ld	[%fp + .4_i],%o1
	sethi	%hi(mon),%o2
	add	%o2,%lo(mon),%o2
	ldsb	[%o1 + %o2],%o1
	add	%o0,%o1,%o0
	st	%o0,[%fp + .4_d]
! source statement 148
	ld	[%fp + .4_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .4_i]
.L102:
! source statement 148
	ld	[%fp + .4_i],%o0
	ld	[%fp + .4_m],%o1
	cmp	%o0,%o1
	bge	.L104
	nop
	ba,a	.L103
.L104:
! source statement 149
	ld	[%fp + .4_d],%o0
	mov	7,%o1
	call	.rem,2
	nop
	st	%o0,[%fp + .4_d]
! source statement 150
	ld	[%fp + .4_s],%o0
	ld	[%fp + .4_d],%o1
	smul	%o1,3,%o1
	add	%o0,%o1,%o0
	st	%o0,[%fp + .4_s]
! source statement 151
	mov	1,%o0
	st	%o0,[%fp + .4_i]
	ba,a	.L107
.L108:
! source statement 152
	ld	[%fp + .4_i],%o0
	cmp	%o0,3
	bne	.L109
	nop
	ld	[%fp + .4_m],%o0
	sethi	%hi(mon),%o1
	add	%o1,%lo(mon),%o1
	ldsb	[%o0 + %o1],%o0
	cmp	%o0,19
	bne	.L109
	nop
! source statement 153
	ld	[%fp + .4_i],%o0
	add	%o0,11,%o0
	st	%o0,[%fp + .4_i]
! source statement 154
	ld	[%fp + .4_m],%o0
	sethi	%hi(mon),%o1
	add	%o1,%lo(mon),%o1
	add	%o0,%o1,%o0
	ldsb	[%o0],%o1
	add	%o1,11,%o1
	stb	%o1,[%o0]
.L109:
! source statement 156
	ld	[%fp + .4_i],%o0
	cmp	%o0,9
	ble	.L110
	nop
! source statement 157
	ld	[%fp + .4_i],%o0
	mov	10,%o1
	call	.div,2
	nop
	add	%o0,48,%o0
	ld	[%fp + .4_s],%o1
	stb	%o0,[%o1]
.L110:
! source statement 158
	ld	[%fp + .4_s],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .4_s]
! source statement 159
	ld	[%fp + .4_i],%o0
	mov	10,%o1
	call	.rem,2
	nop
	add	%o0,48,%o0
	ld	[%fp + .4_s],%o1
	add	%o1,1,%o2
	st	%o2,[%fp + .4_s]
	stb	%o0,[%o1]
! source statement 160
	ld	[%fp + .4_s],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .4_s]
! source statement 161
	ld	[%fp + .4_d],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .4_d]
	cmp	%o0,7
	bne	.L111
	nop
! source statement 162
	st	%g0,[%fp + .4_d]
! source statement 163
	ld	[%fp + .4_p],%o0
	ld	[%fp + .4_w],%o1
	add	%o0,%o1,%o0
	st	%o0,[%fp + .4_s]
! source statement 164
	ld	[%fp + .4_s],%o0
	st	%o0,[%fp + .4_p]
.L111:
! source statement 166
	ld	[%fp + .4_i],%o0
	add	%o0,1,%o0
	st	%o0,[%fp + .4_i]
.L107:
! source statement 166
	ld	[%fp + .4_i],%o0
	ld	[%fp + .4_m],%o1
	sethi	%hi(mon),%o2
	add	%o2,%lo(mon),%o2
	ldsb	[%o1 + %o2],%o1
	cmp	%o0,%o1
	bg	.L112
	nop
	ba,a	.L108
.L112:
	ret
	restore
	.seg	"data"
	.seg	"text"
	.align	8
	.global	jan1
	.proc	1
jan1:
.5_y = -4
.5_d = -8
	save	%sp,(-104),%sp
.5_yr = 68
	st	%i0,[%fp + .5_yr]
! source statement 184
	ld	[%fp + .5_yr],%o0
	st	%o0,[%fp + .5_y]
! source statement 185
	ld	[%fp + .5_y],%l0
	add	%l0,4,%l0
	ld	[%fp + .5_y],%o0
	add	%o0,3,%o0
	mov	4,%o1
	call	.div,2
	nop
	add	%l0,%o0,%l0
	st	%l0,[%fp + .5_d]
! source statement 193
	ld	[%fp + .5_y],%o0
	cmp	%o0,1800
	ble	.L115
	nop
! source statement 194
	ld	[%fp + .5_d],%l0
	ld	[%fp + .5_y],%o0
	sub	%o0,1701,%o0
	mov	100,%o1
	call	.div,2
	nop
	sub	%l0,%o0,%l0
	st	%l0,[%fp + .5_d]
! source statement 195
	ld	[%fp + .5_d],%l0
	ld	[%fp + .5_y],%o0
	sub	%o0,1601,%o0
	mov	400,%o1
	call	.div,2
	nop
	add	%l0,%o0,%l0
	st	%l0,[%fp + .5_d]
.L115:
! source statement 202
	ld	[%fp + .5_y],%o0
	cmp	%o0,1752
	ble	.L116
	nop
! source statement 203
	ld	[%fp + .5_d],%o0
	add	%o0,3,%o0
	st	%o0,[%fp + .5_d]
.L116:
! source statement 205
	ld	[%fp + .5_d],%o0
	mov	7,%o1
	call	.rem,2
	nop
	ret
	restore	%o0,%g0,%o0
	ret
	restore
	.seg	"data"
