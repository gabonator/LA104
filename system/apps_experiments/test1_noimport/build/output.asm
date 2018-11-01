
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08020000 <main>:
 8020000:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8020004:	2500      	movs	r5, #0
 8020006:	f8df 80c0 	ldr.w	r8, [pc, #192]	; 80200c8 <main+0xc8>
 802000a:	b087      	sub	sp, #28
 802000c:	2600      	movs	r6, #0
 802000e:	f1a5 03a0 	sub.w	r3, r5, #160	; 0xa0
 8020012:	fb03 f703 	mul.w	r7, r3, r3
 8020016:	fa1f f985 	uxth.w	r9, r5
 802001a:	f1a6 0078 	sub.w	r0, r6, #120	; 0x78
 802001e:	fb00 7400 	mla	r4, r0, r0, r7
 8020022:	b2b1      	uxth	r1, r6
 8020024:	4648      	mov	r0, r9
 8020026:	f000 f853 	bl	80200d0 <_Z8Set_Positt>
 802002a:	ea08 2004 	and.w	r0, r8, r4, lsl #8
 802002e:	f400 4078 	and.w	r0, r0, #63488	; 0xf800
 8020032:	3601      	adds	r6, #1
 8020034:	f000 f858 	bl	80200e8 <_Z9Set_Pixelt>
 8020038:	2ef0      	cmp	r6, #240	; 0xf0
 802003a:	d1ee      	bne.n	802001a <main+0x1a>
 802003c:	3501      	adds	r5, #1
 802003e:	f5b5 7fa0 	cmp.w	r5, #320	; 0x140
 8020042:	d1e3      	bne.n	802000c <main+0xc>
 8020044:	466c      	mov	r4, sp
 8020046:	f241 4bcb 	movw	fp, #5323	; 0x14cb
 802004a:	f04f 0a14 	mov.w	sl, #20
 802004e:	260a      	movs	r6, #10
 8020050:	f04f 0803 	mov.w	r8, #3
 8020054:	2702      	movs	r7, #2
 8020056:	4b1a      	ldr	r3, [pc, #104]	; (80200c0 <main+0xc0>)
 8020058:	f8df 9070 	ldr.w	r9, [pc, #112]	; 80200cc <main+0xcc>
 802005c:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 8020060:	330c      	adds	r3, #12
 8020062:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 8020066:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 802006a:	ab06      	add	r3, sp, #24
 802006c:	e903 0007 	stmdb	r3, {r0, r1, r2}
 8020070:	4c14      	ldr	r4, [pc, #80]	; (80200c4 <main+0xc4>)
 8020072:	f000 f83d 	bl	80200f0 <_Z6AnyKeyv>
 8020076:	b9f8      	cbnz	r0, 80200b8 <main+0xb8>
 8020078:	fb09 4b0b 	mla	fp, r9, fp, r4
 802007c:	f3cb 050e 	ubfx	r5, fp, #0, #15
 8020080:	fb95 f3f8 	sdiv	r3, r5, r8
 8020084:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 8020088:	aa06      	add	r2, sp, #24
 802008a:	1aeb      	subs	r3, r5, r3
 802008c:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 8020090:	f853 2c18 	ldr.w	r2, [r3, #-24]
 8020094:	f853 3c0c 	ldr.w	r3, [r3, #-12]
 8020098:	4416      	add	r6, r2
 802009a:	449a      	add	sl, r3
 802009c:	fb96 f6f7 	sdiv	r6, r6, r7
 80200a0:	fb9a faf7 	sdiv	sl, sl, r7
 80200a4:	b2b0      	uxth	r0, r6
 80200a6:	fa1f f18a 	uxth.w	r1, sl
 80200aa:	f000 f811 	bl	80200d0 <_Z8Set_Positt>
 80200ae:	0068      	lsls	r0, r5, #1
 80200b0:	f000 f81a 	bl	80200e8 <_Z9Set_Pixelt>
 80200b4:	3401      	adds	r4, #1
 80200b6:	e7dc      	b.n	8020072 <main+0x72>
 80200b8:	2000      	movs	r0, #0
 80200ba:	b007      	add	sp, #28
 80200bc:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 80200c0:	0802010c 	.word	0x0802010c
 80200c4:	002490f3 	.word	0x002490f3
 80200c8:	07fff800 	.word	0x07fff800
 80200cc:	007df121 	.word	0x007df121

080200d0 <_Z8Set_Positt>:
 80200d0:	b510      	push	{r4, lr}
 80200d2:	460c      	mov	r4, r1
 80200d4:	4601      	mov	r1, r0
 80200d6:	200b      	movs	r0, #11
 80200d8:	f7e0 f812 	bl	8000100 <__Bios>
 80200dc:	4621      	mov	r1, r4
 80200de:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80200e2:	200c      	movs	r0, #12
 80200e4:	f7e0 b80c 	b.w	8000100 <__Bios>

080200e8 <_Z9Set_Pixelt>:
 80200e8:	4601      	mov	r1, r0
 80200ea:	200d      	movs	r0, #13
 80200ec:	f7e0 b808 	b.w	8000100 <__Bios>

080200f0 <_Z6AnyKeyv>:
 80200f0:	b508      	push	{r3, lr}
 80200f2:	4905      	ldr	r1, [pc, #20]	; (8020108 <_Z6AnyKeyv+0x18>)
 80200f4:	2006      	movs	r0, #6
 80200f6:	f7e0 f803 	bl	8000100 <__Bios>
 80200fa:	43c0      	mvns	r0, r0
 80200fc:	f010 000f 	ands.w	r0, r0, #15
 8020100:	bf18      	it	ne
 8020102:	2001      	movne	r0, #1
 8020104:	bd08      	pop	{r3, pc}
 8020106:	bf00      	nop
 8020108:	fffc0000 	.word	0xfffc0000
