
main.elf:     file format elf32-littlearm


Disassembly of section .text:

20005000 <main>:
20005000:	b5f0      	push	{r4, r5, r6, r7, lr}
20005002:	b087      	sub	sp, #28
20005004:	466c      	mov	r4, sp
20005006:	4b21      	ldr	r3, [pc, #132]	; (2000508c <main+0x8c>)
20005008:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
2000500c:	e884 0007 	stmia.w	r4, {r0, r1, r2}
20005010:	2400      	movs	r4, #0
20005012:	330c      	adds	r3, #12
20005014:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
20005018:	ab06      	add	r3, sp, #24
2000501a:	e903 0007 	stmdb	r3, {r0, r1, r2}
2000501e:	2500      	movs	r5, #0
20005020:	f1a4 03a0 	sub.w	r3, r4, #160	; 0xa0
20005024:	fb03 f603 	mul.w	r6, r3, r3
20005028:	b2a7      	uxth	r7, r4
2000502a:	f1a5 0278 	sub.w	r2, r5, #120	; 0x78
2000502e:	fb02 6202 	mla	r2, r2, r2, r6
20005032:	0212      	lsls	r2, r2, #8
20005034:	b2a9      	uxth	r1, r5
20005036:	f402 4278 	and.w	r2, r2, #63488	; 0xf800
2000503a:	4638      	mov	r0, r7
2000503c:	3501      	adds	r5, #1
2000503e:	f000 f88f 	bl	20005160 <PutPixel@plt>
20005042:	2df0      	cmp	r5, #240	; 0xf0
20005044:	d1f1      	bne.n	2000502a <main+0x2a>
20005046:	3401      	adds	r4, #1
20005048:	f5b4 7fa0 	cmp.w	r4, #320	; 0x140
2000504c:	d1e7      	bne.n	2000501e <main+0x1e>
2000504e:	2714      	movs	r7, #20
20005050:	240a      	movs	r4, #10
20005052:	2603      	movs	r6, #3
20005054:	2502      	movs	r5, #2
20005056:	f000 f87b 	bl	20005150 <random@plt>
2000505a:	fb90 f3f6 	sdiv	r3, r0, r6
2000505e:	eb03 0343 	add.w	r3, r3, r3, lsl #1
20005062:	aa06      	add	r2, sp, #24
20005064:	1ac3      	subs	r3, r0, r3
20005066:	eb02 0383 	add.w	r3, r2, r3, lsl #2
2000506a:	f853 2c18 	ldr.w	r2, [r3, #-24]
2000506e:	f853 3c0c 	ldr.w	r3, [r3, #-12]
20005072:	4414      	add	r4, r2
20005074:	441f      	add	r7, r3
20005076:	fb94 f4f5 	sdiv	r4, r4, r5
2000507a:	fb97 f7f5 	sdiv	r7, r7, r5
2000507e:	0040      	lsls	r0, r0, #1
20005080:	b282      	uxth	r2, r0
20005082:	b2b9      	uxth	r1, r7
20005084:	b2a0      	uxth	r0, r4
20005086:	f000 f86b 	bl	20005160 <PutPixel@plt>
2000508a:	e7e4      	b.n	20005056 <main+0x56>
2000508c:	20005090 	.word	0x20005090
20005090:	0000000a 	.word	0x0000000a
20005094:	00000136 	.word	0x00000136
20005098:	0000008c 	.word	0x0000008c
2000509c:	00000014 	.word	0x00000014
200050a0:	00000028 	.word	0x00000028
200050a4:	000000dc 	.word	0x000000dc

Disassembly of section .plt:

20005140 <.plt>:
20005140:	b500      	push	{lr}
20005142:	f8df e008 	ldr.w	lr, [pc, #8]	; 2000514c <.plt+0xc>
20005146:	44fe      	add	lr, pc
20005148:	f85e ff08 	ldr.w	pc, [lr, #8]!
2000514c:	00000094 	.word	0x00000094

20005150 <random@plt>:
20005150:	f240 0c90 	movw	ip, #144	; 0x90
20005154:	f2c0 0c00 	movt	ip, #0
20005158:	44fc      	add	ip, pc
2000515a:	f8dc f000 	ldr.w	pc, [ip]
2000515e:	bf00      	nop

20005160 <PutPixel@plt>:
20005160:	f240 0c84 	movw	ip, #132	; 0x84
20005164:	f2c0 0c00 	movt	ip, #0
20005168:	44fc      	add	ip, pc
2000516a:	f8dc f000 	ldr.w	pc, [ip]
2000516e:	bf00      	nop
