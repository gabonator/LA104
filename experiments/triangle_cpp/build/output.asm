
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08008000 <_vectors>:
 8008000:	20009a00 	.word	0x20009a00
 8008004:	080082c9 	.word	0x080082c9
 8008008:	080082df 	.word	0x080082df
 800800c:	080082df 	.word	0x080082df
 8008010:	080082df 	.word	0x080082df
 8008014:	080082df 	.word	0x080082df
 8008018:	080082df 	.word	0x080082df
	...
 800802c:	080082dd 	.word	0x080082dd
 8008030:	080082dd 	.word	0x080082dd
 8008034:	00000000 	.word	0x00000000
 8008038:	080082dd 	.word	0x080082dd
 800803c:	080082dd 	.word	0x080082dd
 8008040:	080082df 	.word	0x080082df
 8008044:	080082df 	.word	0x080082df
 8008048:	080082df 	.word	0x080082df
 800804c:	080082df 	.word	0x080082df
 8008050:	080082df 	.word	0x080082df
 8008054:	080082df 	.word	0x080082df
 8008058:	080082df 	.word	0x080082df
 800805c:	080082df 	.word	0x080082df
 8008060:	080082df 	.word	0x080082df
 8008064:	080082df 	.word	0x080082df
 8008068:	080082df 	.word	0x080082df
 800806c:	080082df 	.word	0x080082df
 8008070:	080082df 	.word	0x080082df
 8008074:	080082df 	.word	0x080082df
 8008078:	080082df 	.word	0x080082df
 800807c:	080082df 	.word	0x080082df
 8008080:	080082df 	.word	0x080082df
 8008084:	080082df 	.word	0x080082df
 8008088:	080082df 	.word	0x080082df
 800808c:	080082dd 	.word	0x080082dd
 8008090:	080082dd 	.word	0x080082dd
 8008094:	080082dd 	.word	0x080082dd
 8008098:	080082dd 	.word	0x080082dd
 800809c:	080082df 	.word	0x080082df
 80080a0:	080082df 	.word	0x080082df
 80080a4:	080082df 	.word	0x080082df
 80080a8:	080082df 	.word	0x080082df
 80080ac:	080082df 	.word	0x080082df
 80080b0:	080082df 	.word	0x080082df
 80080b4:	080082df 	.word	0x080082df
 80080b8:	080082df 	.word	0x080082df
 80080bc:	080082df 	.word	0x080082df
 80080c0:	080082df 	.word	0x080082df
 80080c4:	080082df 	.word	0x080082df
 80080c8:	080082df 	.word	0x080082df
 80080cc:	080082df 	.word	0x080082df
 80080d0:	080082df 	.word	0x080082df
 80080d4:	080082df 	.word	0x080082df
 80080d8:	080082df 	.word	0x080082df
 80080dc:	080082df 	.word	0x080082df
 80080e0:	080082df 	.word	0x080082df
 80080e4:	080082df 	.word	0x080082df
 80080e8:	080082df 	.word	0x080082df
 80080ec:	080082df 	.word	0x080082df
 80080f0:	080082df 	.word	0x080082df
 80080f4:	080082df 	.word	0x080082df
 80080f8:	080082df 	.word	0x080082df
 80080fc:	080082df 	.word	0x080082df
 8008100:	080082df 	.word	0x080082df
 8008104:	080082df 	.word	0x080082df
 8008108:	080082df 	.word	0x080082df
 800810c:	080082df 	.word	0x080082df
 8008110:	080082df 	.word	0x080082df
 8008114:	080082df 	.word	0x080082df
 8008118:	080082df 	.word	0x080082df
 800811c:	080082df 	.word	0x080082df
 8008120:	080082df 	.word	0x080082df
 8008124:	080082df 	.word	0x080082df
 8008128:	080082df 	.word	0x080082df
 800812c:	080082df 	.word	0x080082df

08008130 <_Z8Set_Positt>:
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

0800814a <_Z9Set_Pixelt>:
 800814a:	4601      	mov	r1, r0
 800814c:	200d      	movs	r0, #13
 800814e:	f7f7 bfd7 	b.w	8000100 <__Bios>

08008152 <_Z6HwInitv>:
 8008152:	b510      	push	{r4, lr}
 8008154:	2004      	movs	r0, #4
 8008156:	4913      	ldr	r1, [pc, #76]	; (80081a4 <_Z6HwInitv+0x52>)
 8008158:	f7f7 ffd2 	bl	8000100 <__Bios>
 800815c:	2006      	movs	r0, #6
 800815e:	4911      	ldr	r1, [pc, #68]	; (80081a4 <_Z6HwInitv+0x52>)
 8008160:	f7f7 ffce 	bl	8000100 <__Bios>
 8008164:	2001      	movs	r0, #1
 8008166:	f44f 4100 	mov.w	r1, #32768	; 0x8000
 800816a:	f7f7 ffc9 	bl	8000100 <__Bios>
 800816e:	2005      	movs	r0, #5
 8008170:	490c      	ldr	r1, [pc, #48]	; (80081a4 <_Z6HwInitv+0x52>)
 8008172:	f7f7 ffc5 	bl	8000100 <__Bios>
 8008176:	2132      	movs	r1, #50	; 0x32
 8008178:	2005      	movs	r0, #5
 800817a:	f7f7 ffc1 	bl	8000100 <__Bios>
 800817e:	2015      	movs	r0, #21
 8008180:	4908      	ldr	r1, [pc, #32]	; (80081a4 <_Z6HwInitv+0x52>)
 8008182:	f7f7 ffbd 	bl	8000100 <__Bios>
 8008186:	2012      	movs	r0, #18
 8008188:	4906      	ldr	r1, [pc, #24]	; (80081a4 <_Z6HwInitv+0x52>)
 800818a:	f7f7 ffb9 	bl	8000100 <__Bios>
 800818e:	2011      	movs	r0, #17
 8008190:	4904      	ldr	r1, [pc, #16]	; (80081a4 <_Z6HwInitv+0x52>)
 8008192:	f7f7 ffb5 	bl	8000100 <__Bios>
 8008196:	2011      	movs	r0, #17
 8008198:	f44f 51fa 	mov.w	r1, #8000	; 0x1f40
 800819c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80081a0:	f7f7 bfae 	b.w	8000100 <__Bios>
 80081a4:	ffff0000 	.word	0xffff0000

080081a8 <main>:
 80081a8:	b5f0      	push	{r4, r5, r6, r7, lr}
 80081aa:	b089      	sub	sp, #36	; 0x24
 80081ac:	a806      	add	r0, sp, #24
 80081ae:	f000 f853 	bl	8008258 <_ZN5CPRNGC1Ev>
 80081b2:	2400      	movs	r4, #0
 80081b4:	f7ff ffcd 	bl	8008152 <_Z6HwInitv>
 80081b8:	e013      	b.n	80081e2 <main+0x3a>
 80081ba:	f1a5 0678 	sub.w	r6, r5, #120	; 0x78
 80081be:	fb06 7606 	mla	r6, r6, r6, r7
 80081c2:	b2a9      	uxth	r1, r5
 80081c4:	b2a0      	uxth	r0, r4
 80081c6:	f7ff ffb3 	bl	8008130 <_Z8Set_Positt>
 80081ca:	00b0      	lsls	r0, r6, #2
 80081cc:	f400 7078 	and.w	r0, r0, #992	; 0x3e0
 80081d0:	3501      	adds	r5, #1
 80081d2:	f7ff ffba 	bl	800814a <_Z9Set_Pixelt>
 80081d6:	2df0      	cmp	r5, #240	; 0xf0
 80081d8:	d1ef      	bne.n	80081ba <main+0x12>
 80081da:	3401      	adds	r4, #1
 80081dc:	f5b4 7fa0 	cmp.w	r4, #320	; 0x140
 80081e0:	d004      	beq.n	80081ec <main+0x44>
 80081e2:	f1a4 07a0 	sub.w	r7, r4, #160	; 0xa0
 80081e6:	437f      	muls	r7, r7
 80081e8:	2500      	movs	r5, #0
 80081ea:	e7e6      	b.n	80081ba <main+0x12>
 80081ec:	4b19      	ldr	r3, [pc, #100]	; (8008254 <main+0xac>)
 80081ee:	ac03      	add	r4, sp, #12
 80081f0:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 80081f4:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 80081f8:	330c      	adds	r3, #12
 80081fa:	466c      	mov	r4, sp
 80081fc:	9d03      	ldr	r5, [sp, #12]
 80081fe:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 8008202:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 8008206:	4604      	mov	r4, r0
 8008208:	a806      	add	r0, sp, #24
 800820a:	f000 f82b 	bl	8008264 <_ZN5CPRNG3getEv>
 800820e:	2303      	movs	r3, #3
 8008210:	fb90 f3f3 	sdiv	r3, r0, r3
 8008214:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 8008218:	aa08      	add	r2, sp, #32
 800821a:	1ac3      	subs	r3, r0, r3
 800821c:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 8008220:	f853 2c14 	ldr.w	r2, [r3, #-20]
 8008224:	f853 3c20 	ldr.w	r3, [r3, #-32]
 8008228:	18ad      	adds	r5, r5, r2
 800822a:	18e4      	adds	r4, r4, r3
 800822c:	eb05 75d5 	add.w	r5, r5, r5, lsr #31
 8008230:	eb04 74d4 	add.w	r4, r4, r4, lsr #31
 8008234:	106d      	asrs	r5, r5, #1
 8008236:	1064      	asrs	r4, r4, #1
 8008238:	4606      	mov	r6, r0
 800823a:	b2a1      	uxth	r1, r4
 800823c:	b2a8      	uxth	r0, r5
 800823e:	f7ff ff77 	bl	8008130 <_Z8Set_Positt>
 8008242:	0076      	lsls	r6, r6, #1
 8008244:	f64f 70fe 	movw	r0, #65534	; 0xfffe
 8008248:	ea06 0000 	and.w	r0, r6, r0
 800824c:	f7ff ff7d 	bl	800814a <_Z9Set_Pixelt>
 8008250:	e7da      	b.n	8008208 <main+0x60>
 8008252:	bf00      	nop
 8008254:	080082e0 	.word	0x080082e0

08008258 <_ZN5CPRNGC1Ev>:
 8008258:	f241 42cb 	movw	r2, #5323	; 0x14cb
 800825c:	6002      	str	r2, [r0, #0]
 800825e:	2200      	movs	r2, #0
 8008260:	6042      	str	r2, [r0, #4]
 8008262:	4770      	bx	lr

08008264 <_ZN5CPRNG3getEv>:
 8008264:	6842      	ldr	r2, [r0, #4]
 8008266:	4b06      	ldr	r3, [pc, #24]	; (8008280 <_ZN5CPRNG3getEv+0x1c>)
 8008268:	b510      	push	{r4, lr}
 800826a:	6801      	ldr	r1, [r0, #0]
 800826c:	4c05      	ldr	r4, [pc, #20]	; (8008284 <_ZN5CPRNG3getEv+0x20>)
 800826e:	18d3      	adds	r3, r2, r3
 8008270:	fb04 3301 	mla	r3, r4, r1, r3
 8008274:	3201      	adds	r2, #1
 8008276:	6003      	str	r3, [r0, #0]
 8008278:	6042      	str	r2, [r0, #4]
 800827a:	0458      	lsls	r0, r3, #17
 800827c:	0c40      	lsrs	r0, r0, #17
 800827e:	bd10      	pop	{r4, pc}
 8008280:	002490f3 	.word	0x002490f3
 8008284:	007df121 	.word	0x007df121

08008288 <__Init_Data>:
 8008288:	4a0a      	ldr	r2, [pc, #40]	; (80082b4 <__Init_Data+0x2c>)
 800828a:	4b0b      	ldr	r3, [pc, #44]	; (80082b8 <__Init_Data+0x30>)
 800828c:	429a      	cmp	r2, r3
 800828e:	d105      	bne.n	800829c <__Init_Data+0x14>
 8008290:	4b0a      	ldr	r3, [pc, #40]	; (80082bc <__Init_Data+0x34>)
 8008292:	e00a      	b.n	80082aa <__Init_Data+0x22>
 8008294:	f852 1b04 	ldr.w	r1, [r2], #4
 8008298:	f843 1b04 	str.w	r1, [r3], #4
 800829c:	4908      	ldr	r1, [pc, #32]	; (80082c0 <__Init_Data+0x38>)
 800829e:	428b      	cmp	r3, r1
 80082a0:	d3f8      	bcc.n	8008294 <__Init_Data+0xc>
 80082a2:	e7f5      	b.n	8008290 <__Init_Data+0x8>
 80082a4:	2200      	movs	r2, #0
 80082a6:	f843 2b04 	str.w	r2, [r3], #4
 80082aa:	4a06      	ldr	r2, [pc, #24]	; (80082c4 <__Init_Data+0x3c>)
 80082ac:	4293      	cmp	r3, r2
 80082ae:	d3f9      	bcc.n	80082a4 <__Init_Data+0x1c>
 80082b0:	4770      	bx	lr
 80082b2:	bf00      	nop
 80082b4:	080082f8 	.word	0x080082f8
 80082b8:	20000000 	.word	0x20000000
 80082bc:	20000000 	.word	0x20000000
 80082c0:	20000000 	.word	0x20000000
 80082c4:	20000000 	.word	0x20000000

080082c8 <Reset_Handler>:
 80082c8:	4668      	mov	r0, sp
 80082ca:	f020 0107 	bic.w	r1, r0, #7
 80082ce:	468d      	mov	sp, r1
 80082d0:	b501      	push	{r0, lr}
 80082d2:	f7ff ffd9 	bl	8008288 <__Init_Data>
 80082d6:	f7ff ff67 	bl	80081a8 <main>
 80082da:	e7fe      	b.n	80082da <Reset_Handler+0x12>

080082dc <Dummy_Handler>:
 80082dc:	4770      	bx	lr

080082de <Default_Handler>:
 80082de:	e7fe      	b.n	80082de <Default_Handler>
