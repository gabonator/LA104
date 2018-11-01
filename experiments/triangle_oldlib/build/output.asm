
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08008000 <_vectors>:
 8008000:	20009a00 	.word	0x20009a00
 8008004:	080082b9 	.word	0x080082b9
 8008008:	080082cf 	.word	0x080082cf
 800800c:	080082cf 	.word	0x080082cf
 8008010:	080082cf 	.word	0x080082cf
 8008014:	080082cf 	.word	0x080082cf
 8008018:	080082cf 	.word	0x080082cf
	...
 800802c:	080082cd 	.word	0x080082cd
 8008030:	080082cd 	.word	0x080082cd
 8008034:	00000000 	.word	0x00000000
 8008038:	080082cd 	.word	0x080082cd
 800803c:	080082cd 	.word	0x080082cd
 8008040:	080082cf 	.word	0x080082cf
 8008044:	080082cf 	.word	0x080082cf
 8008048:	080082cf 	.word	0x080082cf
 800804c:	080082cf 	.word	0x080082cf
 8008050:	080082cf 	.word	0x080082cf
 8008054:	080082cf 	.word	0x080082cf
 8008058:	080082cf 	.word	0x080082cf
 800805c:	080082cf 	.word	0x080082cf
 8008060:	080082cf 	.word	0x080082cf
 8008064:	080082cf 	.word	0x080082cf
 8008068:	080082cf 	.word	0x080082cf
 800806c:	080082cf 	.word	0x080082cf
 8008070:	080082cf 	.word	0x080082cf
 8008074:	080082cf 	.word	0x080082cf
 8008078:	080082cf 	.word	0x080082cf
 800807c:	080082cf 	.word	0x080082cf
 8008080:	080082cf 	.word	0x080082cf
 8008084:	080082cf 	.word	0x080082cf
 8008088:	080082cf 	.word	0x080082cf
 800808c:	080082cd 	.word	0x080082cd
 8008090:	080082cd 	.word	0x080082cd
 8008094:	080082cd 	.word	0x080082cd
 8008098:	080082cd 	.word	0x080082cd
 800809c:	080082cf 	.word	0x080082cf
 80080a0:	080082cf 	.word	0x080082cf
 80080a4:	080082cf 	.word	0x080082cf
 80080a8:	080082cf 	.word	0x080082cf
 80080ac:	080082cf 	.word	0x080082cf
 80080b0:	080082cf 	.word	0x080082cf
 80080b4:	080082cf 	.word	0x080082cf
 80080b8:	080082cf 	.word	0x080082cf
 80080bc:	080082cf 	.word	0x080082cf
 80080c0:	080082cf 	.word	0x080082cf
 80080c4:	080082cf 	.word	0x080082cf
 80080c8:	080082cf 	.word	0x080082cf
 80080cc:	080082cf 	.word	0x080082cf
 80080d0:	080082cf 	.word	0x080082cf
 80080d4:	080082cf 	.word	0x080082cf
 80080d8:	080082cf 	.word	0x080082cf
 80080dc:	080082cf 	.word	0x080082cf
 80080e0:	080082cf 	.word	0x080082cf
 80080e4:	080082cf 	.word	0x080082cf
 80080e8:	080082cf 	.word	0x080082cf
 80080ec:	080082cf 	.word	0x080082cf
 80080f0:	080082cf 	.word	0x080082cf
 80080f4:	080082cf 	.word	0x080082cf
 80080f8:	080082cf 	.word	0x080082cf
 80080fc:	080082cf 	.word	0x080082cf
 8008100:	080082cf 	.word	0x080082cf
 8008104:	080082cf 	.word	0x080082cf
 8008108:	080082cf 	.word	0x080082cf
 800810c:	080082cf 	.word	0x080082cf
 8008110:	080082cf 	.word	0x080082cf
 8008114:	080082cf 	.word	0x080082cf
 8008118:	080082cf 	.word	0x080082cf
 800811c:	080082cf 	.word	0x080082cf
 8008120:	080082cf 	.word	0x080082cf
 8008124:	080082cf 	.word	0x080082cf
 8008128:	080082cf 	.word	0x080082cf
 800812c:	080082cf 	.word	0x080082cf

08008130 <Set_Posi>:
 8008130:	4603      	mov	r3, r0
 8008132:	b510      	push	{r4, lr}
 8008134:	200b      	movs	r0, #11
 8008136:	460c      	mov	r4, r1
 8008138:	4619      	mov	r1, r3
 800813a:	f7f7 ffe1 	bl	8000100 <__Bios>
 800813e:	200c      	movs	r0, #12
 8008140:	4621      	mov	r1, r4
 8008142:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8008146:	f7f7 bfdb 	b.w	8000100 <__Bios>

0800814a <Set_Pixel>:
 800814a:	4601      	mov	r1, r0
 800814c:	200d      	movs	r0, #13
 800814e:	f7f7 bfd7 	b.w	8000100 <__Bios>

08008152 <PRNG>:
 8008152:	4a08      	ldr	r2, [pc, #32]	; (8008174 <PRNG+0x22>)
 8008154:	4808      	ldr	r0, [pc, #32]	; (8008178 <PRNG+0x26>)
 8008156:	6811      	ldr	r1, [r2, #0]
 8008158:	4b08      	ldr	r3, [pc, #32]	; (800817c <PRNG+0x2a>)
 800815a:	b530      	push	{r4, r5, lr}
 800815c:	6804      	ldr	r4, [r0, #0]
 800815e:	4d08      	ldr	r5, [pc, #32]	; (8008180 <PRNG+0x2e>)
 8008160:	18cb      	adds	r3, r1, r3
 8008162:	fb05 3304 	mla	r3, r5, r4, r3
 8008166:	3101      	adds	r1, #1
 8008168:	6003      	str	r3, [r0, #0]
 800816a:	0458      	lsls	r0, r3, #17
 800816c:	6011      	str	r1, [r2, #0]
 800816e:	0c40      	lsrs	r0, r0, #17
 8008170:	bd30      	pop	{r4, r5, pc}
 8008172:	bf00      	nop
 8008174:	20000004 	.word	0x20000004
 8008178:	20000000 	.word	0x20000000
 800817c:	002490f3 	.word	0x002490f3
 8008180:	007df121 	.word	0x007df121

08008184 <main>:
 8008184:	b5f0      	push	{r4, r5, r6, r7, lr}
 8008186:	4b3a      	ldr	r3, [pc, #232]	; (8008270 <main+0xec>)
 8008188:	b087      	sub	sp, #28
 800818a:	ac03      	add	r4, sp, #12
 800818c:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 8008190:	330c      	adds	r3, #12
 8008192:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 8008196:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 800819a:	466c      	mov	r4, sp
 800819c:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 80081a0:	4934      	ldr	r1, [pc, #208]	; (8008274 <main+0xf0>)
 80081a2:	2004      	movs	r0, #4
 80081a4:	f7f7 ffac 	bl	8000100 <__Bios>
 80081a8:	4932      	ldr	r1, [pc, #200]	; (8008274 <main+0xf0>)
 80081aa:	2006      	movs	r0, #6
 80081ac:	f7f7 ffa8 	bl	8000100 <__Bios>
 80081b0:	f44f 4100 	mov.w	r1, #32768	; 0x8000
 80081b4:	2001      	movs	r0, #1
 80081b6:	f7f7 ffa3 	bl	8000100 <__Bios>
 80081ba:	492e      	ldr	r1, [pc, #184]	; (8008274 <main+0xf0>)
 80081bc:	2005      	movs	r0, #5
 80081be:	f7f7 ff9f 	bl	8000100 <__Bios>
 80081c2:	2132      	movs	r1, #50	; 0x32
 80081c4:	2005      	movs	r0, #5
 80081c6:	f7f7 ff9b 	bl	8000100 <__Bios>
 80081ca:	492a      	ldr	r1, [pc, #168]	; (8008274 <main+0xf0>)
 80081cc:	2015      	movs	r0, #21
 80081ce:	f7f7 ff97 	bl	8000100 <__Bios>
 80081d2:	4928      	ldr	r1, [pc, #160]	; (8008274 <main+0xf0>)
 80081d4:	2012      	movs	r0, #18
 80081d6:	f7f7 ff93 	bl	8000100 <__Bios>
 80081da:	4926      	ldr	r1, [pc, #152]	; (8008274 <main+0xf0>)
 80081dc:	2011      	movs	r0, #17
 80081de:	f7f7 ff8f 	bl	8000100 <__Bios>
 80081e2:	2011      	movs	r0, #17
 80081e4:	f44f 51fa 	mov.w	r1, #8000	; 0x1f40
 80081e8:	f7f7 ff8a 	bl	8000100 <__Bios>
 80081ec:	2400      	movs	r4, #0
 80081ee:	e013      	b.n	8008218 <main+0x94>
 80081f0:	f1a5 0678 	sub.w	r6, r5, #120	; 0x78
 80081f4:	fb06 7606 	mla	r6, r6, r6, r7
 80081f8:	b2a9      	uxth	r1, r5
 80081fa:	b2a0      	uxth	r0, r4
 80081fc:	f7ff ff98 	bl	8008130 <Set_Posi>
 8008200:	08f0      	lsrs	r0, r6, #3
 8008202:	f000 001f 	and.w	r0, r0, #31
 8008206:	3501      	adds	r5, #1
 8008208:	f7ff ff9f 	bl	800814a <Set_Pixel>
 800820c:	2df0      	cmp	r5, #240	; 0xf0
 800820e:	d1ef      	bne.n	80081f0 <main+0x6c>
 8008210:	3401      	adds	r4, #1
 8008212:	f5b4 7fa0 	cmp.w	r4, #320	; 0x140
 8008216:	d004      	beq.n	8008222 <main+0x9e>
 8008218:	f1a4 07a0 	sub.w	r7, r4, #160	; 0xa0
 800821c:	437f      	muls	r7, r7
 800821e:	2500      	movs	r5, #0
 8008220:	e7e6      	b.n	80081f0 <main+0x6c>
 8008222:	9d03      	ldr	r5, [sp, #12]
 8008224:	9c00      	ldr	r4, [sp, #0]
 8008226:	f7ff ff94 	bl	8008152 <PRNG>
 800822a:	2303      	movs	r3, #3
 800822c:	fb90 f3f3 	sdiv	r3, r0, r3
 8008230:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 8008234:	aa06      	add	r2, sp, #24
 8008236:	1ac3      	subs	r3, r0, r3
 8008238:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 800823c:	f853 2c0c 	ldr.w	r2, [r3, #-12]
 8008240:	f853 3c18 	ldr.w	r3, [r3, #-24]
 8008244:	18ad      	adds	r5, r5, r2
 8008246:	18e4      	adds	r4, r4, r3
 8008248:	eb05 75d5 	add.w	r5, r5, r5, lsr #31
 800824c:	eb04 74d4 	add.w	r4, r4, r4, lsr #31
 8008250:	106d      	asrs	r5, r5, #1
 8008252:	1064      	asrs	r4, r4, #1
 8008254:	4606      	mov	r6, r0
 8008256:	b2a1      	uxth	r1, r4
 8008258:	b2a8      	uxth	r0, r5
 800825a:	f7ff ff69 	bl	8008130 <Set_Posi>
 800825e:	0076      	lsls	r6, r6, #1
 8008260:	f64f 70fe 	movw	r0, #65534	; 0xfffe
 8008264:	ea06 0000 	and.w	r0, r6, r0
 8008268:	f7ff ff6f 	bl	800814a <Set_Pixel>
 800826c:	e7db      	b.n	8008226 <main+0xa2>
 800826e:	bf00      	nop
 8008270:	080082d0 	.word	0x080082d0
 8008274:	ffff0000 	.word	0xffff0000

08008278 <__Init_Data>:
 8008278:	4a0a      	ldr	r2, [pc, #40]	; (80082a4 <__Init_Data+0x2c>)
 800827a:	4b0b      	ldr	r3, [pc, #44]	; (80082a8 <__Init_Data+0x30>)
 800827c:	429a      	cmp	r2, r3
 800827e:	d105      	bne.n	800828c <__Init_Data+0x14>
 8008280:	4b0a      	ldr	r3, [pc, #40]	; (80082ac <__Init_Data+0x34>)
 8008282:	e00a      	b.n	800829a <__Init_Data+0x22>
 8008284:	f852 1b04 	ldr.w	r1, [r2], #4
 8008288:	f843 1b04 	str.w	r1, [r3], #4
 800828c:	4908      	ldr	r1, [pc, #32]	; (80082b0 <__Init_Data+0x38>)
 800828e:	428b      	cmp	r3, r1
 8008290:	d3f8      	bcc.n	8008284 <__Init_Data+0xc>
 8008292:	e7f5      	b.n	8008280 <__Init_Data+0x8>
 8008294:	2200      	movs	r2, #0
 8008296:	f843 2b04 	str.w	r2, [r3], #4
 800829a:	4a06      	ldr	r2, [pc, #24]	; (80082b4 <__Init_Data+0x3c>)
 800829c:	4293      	cmp	r3, r2
 800829e:	d3f9      	bcc.n	8008294 <__Init_Data+0x1c>
 80082a0:	4770      	bx	lr
 80082a2:	bf00      	nop
 80082a4:	080082e8 	.word	0x080082e8
 80082a8:	20000000 	.word	0x20000000
 80082ac:	20000004 	.word	0x20000004
 80082b0:	20000004 	.word	0x20000004
 80082b4:	20000008 	.word	0x20000008

080082b8 <Reset_Handler>:
 80082b8:	4668      	mov	r0, sp
 80082ba:	f020 0107 	bic.w	r1, r0, #7
 80082be:	468d      	mov	sp, r1
 80082c0:	b501      	push	{r0, lr}
 80082c2:	f7ff ffd9 	bl	8008278 <__Init_Data>
 80082c6:	f7ff ff5d 	bl	8008184 <main>
 80082ca:	e7fe      	b.n	80082ca <Reset_Handler+0x12>

080082cc <Dummy_Handler>:
 80082cc:	4770      	bx	lr

080082ce <Default_Handler>:
 80082ce:	e7fe      	b.n	80082ce <Default_Handler>

080082d0 <C.0.6060>:
 80082d0:	000a 0000 0136 0000 008c 0000               ....6.......

080082dc <C.1.6061>:
 80082dc:	0014 0000 0028 0000 00dc 0000               ....(.......
