
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08008000 <g_pfnVectors>:
 8008000:	00 27 00 20 71 83 00 08 2f 83 00 08 2f 83 00 08     .'. q.../.../...
 8008010:	2f 83 00 08 2f 83 00 08 2f 83 00 08 00 00 00 00     /.../.../.......
	...
 800802c:	2d 83 00 08 2d 83 00 08 00 00 00 00 2d 83 00 08     -...-.......-...
 800803c:	2d 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     -.../.../.../...
 800804c:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 800805c:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 800806c:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 800807c:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 800808c:	2d 83 00 08 2d 83 00 08 2d 83 00 08 2d 83 00 08     -...-...-...-...
 800809c:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 80080ac:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 80080bc:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 80080cc:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 80080dc:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 80080ec:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 80080fc:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 800810c:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 800811c:	2f 83 00 08 2f 83 00 08 2f 83 00 08 2f 83 00 08     /.../.../.../...
 800812c:	2f 83 00 08                                         /...

08008130 <Set_Posi>:
 8008130:	b510      	push	{r4, lr}
 8008132:	460c      	mov	r4, r1
 8008134:	4601      	mov	r1, r0
 8008136:	200b      	movs	r0, #11
 8008138:	f7f7 ffe2 	bl	8000100 <__Bios>
 800813c:	4621      	mov	r1, r4
 800813e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8008142:	200c      	movs	r0, #12
 8008144:	f7f7 bfdc 	b.w	8000100 <__Bios>

08008148 <Set_Pixel>:
 8008148:	4601      	mov	r1, r0
 800814a:	200d      	movs	r0, #13
 800814c:	f7f7 bfd8 	b.w	8000100 <__Bios>

08008150 <Test>:
 8008150:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 8008154:	2500      	movs	r5, #0
 8008156:	f8df 8040 	ldr.w	r8, [pc, #64]	; 8008198 <Test+0x48>
 800815a:	2600      	movs	r6, #0
 800815c:	f1a5 03a0 	sub.w	r3, r5, #160	; 0xa0
 8008160:	fb03 f703 	mul.w	r7, r3, r3
 8008164:	fa1f f985 	uxth.w	r9, r5
 8008168:	f1a6 0078 	sub.w	r0, r6, #120	; 0x78
 800816c:	fb00 7400 	mla	r4, r0, r0, r7
 8008170:	b2b1      	uxth	r1, r6
 8008172:	4648      	mov	r0, r9
 8008174:	f7ff ffdc 	bl	8008130 <Set_Posi>
 8008178:	ea08 2004 	and.w	r0, r8, r4, lsl #8
 800817c:	f400 4078 	and.w	r0, r0, #63488	; 0xf800
 8008180:	3601      	adds	r6, #1
 8008182:	f7ff ffe1 	bl	8008148 <Set_Pixel>
 8008186:	2ef0      	cmp	r6, #240	; 0xf0
 8008188:	d1ee      	bne.n	8008168 <Test+0x18>
 800818a:	3501      	adds	r5, #1
 800818c:	f5b5 7fa0 	cmp.w	r5, #320	; 0x140
 8008190:	d1e3      	bne.n	800815a <Test+0xa>
 8008192:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8008196:	bf00      	nop
 8008198:	07fff800 	.word	0x07fff800

0800819c <WriteMem>:
 800819c:	b510      	push	{r4, lr}
 800819e:	4401      	add	r1, r0
 80081a0:	1a12      	subs	r2, r2, r0
 80081a2:	4288      	cmp	r0, r1
 80081a4:	eb00 0302 	add.w	r3, r0, r2
 80081a8:	d100      	bne.n	80081ac <WriteMem+0x10>
 80081aa:	bd10      	pop	{r4, pc}
 80081ac:	f810 4b01 	ldrb.w	r4, [r0], #1
 80081b0:	701c      	strb	r4, [r3, #0]
 80081b2:	e7f6      	b.n	80081a2 <WriteMem+0x6>

080081b4 <WriteDword>:
 80081b4:	6001      	str	r1, [r0, #0]
 80081b6:	4770      	bx	lr

080081b8 <ExecEntry>:
 80081b8:	4700      	bx	r0

080081ba <experiment>:
 80081ba:	b570      	push	{r4, r5, r6, lr}
 80081bc:	b0a2      	sub	sp, #136	; 0x88
 80081be:	aa01      	add	r2, sp, #4
 80081c0:	4615      	mov	r5, r2
 80081c2:	4b1e      	ldr	r3, [pc, #120]	; (800823c <experiment+0x82>)
 80081c4:	f103 0610 	add.w	r6, r3, #16
 80081c8:	4614      	mov	r4, r2
 80081ca:	6818      	ldr	r0, [r3, #0]
 80081cc:	6859      	ldr	r1, [r3, #4]
 80081ce:	3308      	adds	r3, #8
 80081d0:	c403      	stmia	r4!, {r0, r1}
 80081d2:	42b3      	cmp	r3, r6
 80081d4:	4622      	mov	r2, r4
 80081d6:	d1f7      	bne.n	80081c8 <experiment+0xe>
 80081d8:	6818      	ldr	r0, [r3, #0]
 80081da:	889b      	ldrh	r3, [r3, #4]
 80081dc:	6020      	str	r0, [r4, #0]
 80081de:	80a3      	strh	r3, [r4, #4]
 80081e0:	4628      	mov	r0, r5
 80081e2:	4a17      	ldr	r2, [pc, #92]	; (8008240 <experiment+0x86>)
 80081e4:	2116      	movs	r1, #22
 80081e6:	f7ff ffd9 	bl	800819c <WriteMem>
 80081ea:	2250      	movs	r2, #80	; 0x50
 80081ec:	4915      	ldr	r1, [pc, #84]	; (8008244 <experiment+0x8a>)
 80081ee:	a80e      	add	r0, sp, #56	; 0x38
 80081f0:	f000 f8c8 	bl	8008384 <memcpy>
 80081f4:	4a14      	ldr	r2, [pc, #80]	; (8008248 <experiment+0x8e>)
 80081f6:	2150      	movs	r1, #80	; 0x50
 80081f8:	a80e      	add	r0, sp, #56	; 0x38
 80081fa:	f7ff ffcf 	bl	800819c <WriteMem>
 80081fe:	aa07      	add	r2, sp, #28
 8008200:	4615      	mov	r5, r2
 8008202:	4b12      	ldr	r3, [pc, #72]	; (800824c <experiment+0x92>)
 8008204:	f103 0618 	add.w	r6, r3, #24
 8008208:	4614      	mov	r4, r2
 800820a:	6818      	ldr	r0, [r3, #0]
 800820c:	6859      	ldr	r1, [r3, #4]
 800820e:	3308      	adds	r3, #8
 8008210:	c403      	stmia	r4!, {r0, r1}
 8008212:	42b3      	cmp	r3, r6
 8008214:	4622      	mov	r2, r4
 8008216:	d1f7      	bne.n	8008208 <experiment+0x4e>
 8008218:	6818      	ldr	r0, [r3, #0]
 800821a:	211c      	movs	r1, #28
 800821c:	6020      	str	r0, [r4, #0]
 800821e:	4a0c      	ldr	r2, [pc, #48]	; (8008250 <experiment+0x96>)
 8008220:	4628      	mov	r0, r5
 8008222:	f7ff ffbb 	bl	800819c <WriteMem>
 8008226:	4b0b      	ldr	r3, [pc, #44]	; (8008254 <experiment+0x9a>)
 8008228:	4a0b      	ldr	r2, [pc, #44]	; (8008258 <experiment+0x9e>)
 800822a:	480c      	ldr	r0, [pc, #48]	; (800825c <experiment+0xa2>)
 800822c:	e9c2 3300 	strd	r3, r3, [r2]
 8008230:	e9c2 3302 	strd	r3, r3, [r2, #8]
 8008234:	f7ff ffc0 	bl	80081b8 <ExecEntry>
 8008238:	b022      	add	sp, #136	; 0x88
 800823a:	bd70      	pop	{r4, r5, r6, pc}
 800823c:	08008470 	.word	0x08008470
 8008240:	20005000 	.word	0x20005000
 8008244:	08008486 	.word	0x08008486
 8008248:	200050c0 	.word	0x200050c0
 800824c:	080084d6 	.word	0x080084d6
 8008250:	200051b8 	.word	0x200051b8
 8008254:	08008151 	.word	0x08008151
 8008258:	200051c4 	.word	0x200051c4
 800825c:	20005001 	.word	0x20005001

08008260 <PRNG>:
 8008260:	4b07      	ldr	r3, [pc, #28]	; (8008280 <PRNG+0x20>)
 8008262:	b510      	push	{r4, lr}
 8008264:	6819      	ldr	r1, [r3, #0]
 8008266:	4a07      	ldr	r2, [pc, #28]	; (8008284 <PRNG+0x24>)
 8008268:	1c4c      	adds	r4, r1, #1
 800826a:	601c      	str	r4, [r3, #0]
 800826c:	4b06      	ldr	r3, [pc, #24]	; (8008288 <PRNG+0x28>)
 800826e:	6810      	ldr	r0, [r2, #0]
 8008270:	440b      	add	r3, r1
 8008272:	4906      	ldr	r1, [pc, #24]	; (800828c <PRNG+0x2c>)
 8008274:	fb01 3000 	mla	r0, r1, r0, r3
 8008278:	6010      	str	r0, [r2, #0]
 800827a:	f3c0 000e 	ubfx	r0, r0, #0, #15
 800827e:	bd10      	pop	{r4, pc}
 8008280:	20000004 	.word	0x20000004
 8008284:	20000000 	.word	0x20000000
 8008288:	002490f3 	.word	0x002490f3
 800828c:	007df121 	.word	0x007df121

08008290 <Clear>:
 8008290:	b570      	push	{r4, r5, r6, lr}
 8008292:	2400      	movs	r4, #0
 8008294:	2500      	movs	r5, #0
 8008296:	b2a6      	uxth	r6, r4
 8008298:	b2a9      	uxth	r1, r5
 800829a:	4630      	mov	r0, r6
 800829c:	f7ff ff48 	bl	8008130 <Set_Posi>
 80082a0:	3501      	adds	r5, #1
 80082a2:	f245 5055 	movw	r0, #21845	; 0x5555
 80082a6:	f7ff ff4f 	bl	8008148 <Set_Pixel>
 80082aa:	2df0      	cmp	r5, #240	; 0xf0
 80082ac:	d1f4      	bne.n	8008298 <Clear+0x8>
 80082ae:	3401      	adds	r4, #1
 80082b0:	f5b4 7fa0 	cmp.w	r4, #320	; 0x140
 80082b4:	d1ee      	bne.n	8008294 <Clear+0x4>
 80082b6:	bd70      	pop	{r4, r5, r6, pc}

080082b8 <main>:
 80082b8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 80082bc:	b086      	sub	sp, #24
 80082be:	466c      	mov	r4, sp
 80082c0:	f04f 0814 	mov.w	r8, #20
 80082c4:	270a      	movs	r7, #10
 80082c6:	2603      	movs	r6, #3
 80082c8:	2502      	movs	r5, #2
 80082ca:	f7ff ffe1 	bl	8008290 <Clear>
 80082ce:	f7ff ff74 	bl	80081ba <experiment>
 80082d2:	4b15      	ldr	r3, [pc, #84]	; (8008328 <main+0x70>)
 80082d4:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 80082d8:	330c      	adds	r3, #12
 80082da:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 80082de:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 80082e2:	ab06      	add	r3, sp, #24
 80082e4:	e903 0007 	stmdb	r3, {r0, r1, r2}
 80082e8:	f7ff ffba 	bl	8008260 <PRNG>
 80082ec:	fb90 f3f6 	sdiv	r3, r0, r6
 80082f0:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 80082f4:	aa06      	add	r2, sp, #24
 80082f6:	1ac3      	subs	r3, r0, r3
 80082f8:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 80082fc:	f853 2c18 	ldr.w	r2, [r3, #-24]
 8008300:	f853 3c0c 	ldr.w	r3, [r3, #-12]
 8008304:	4417      	add	r7, r2
 8008306:	4498      	add	r8, r3
 8008308:	fb97 f7f5 	sdiv	r7, r7, r5
 800830c:	fb98 f8f5 	sdiv	r8, r8, r5
 8008310:	4604      	mov	r4, r0
 8008312:	fa1f f188 	uxth.w	r1, r8
 8008316:	b2b8      	uxth	r0, r7
 8008318:	f7ff ff0a 	bl	8008130 <Set_Posi>
 800831c:	0060      	lsls	r0, r4, #1
 800831e:	b280      	uxth	r0, r0
 8008320:	f7ff ff12 	bl	8008148 <Set_Pixel>
 8008324:	e7e0      	b.n	80082e8 <main+0x30>
 8008326:	bf00      	nop
 8008328:	080084f4 	.word	0x080084f4

0800832c <Dummy_Handler>:
 800832c:	4770      	bx	lr

0800832e <Default_Handler>:
 800832e:	e7fe      	b.n	800832e <Default_Handler>

08008330 <__Init_Data>:
 8008330:	4a0a      	ldr	r2, [pc, #40]	; (800835c <__Init_Data+0x2c>)
 8008332:	4b0b      	ldr	r3, [pc, #44]	; (8008360 <__Init_Data+0x30>)
 8008334:	429a      	cmp	r2, r3
 8008336:	d003      	beq.n	8008340 <__Init_Data+0x10>
 8008338:	490a      	ldr	r1, [pc, #40]	; (8008364 <__Init_Data+0x34>)
 800833a:	3a04      	subs	r2, #4
 800833c:	428b      	cmp	r3, r1
 800833e:	d305      	bcc.n	800834c <__Init_Data+0x1c>
 8008340:	2100      	movs	r1, #0
 8008342:	4b09      	ldr	r3, [pc, #36]	; (8008368 <__Init_Data+0x38>)
 8008344:	4a09      	ldr	r2, [pc, #36]	; (800836c <__Init_Data+0x3c>)
 8008346:	4293      	cmp	r3, r2
 8008348:	d305      	bcc.n	8008356 <__Init_Data+0x26>
 800834a:	4770      	bx	lr
 800834c:	f852 0f04 	ldr.w	r0, [r2, #4]!
 8008350:	f843 0b04 	str.w	r0, [r3], #4
 8008354:	e7f2      	b.n	800833c <__Init_Data+0xc>
 8008356:	f843 1b04 	str.w	r1, [r3], #4
 800835a:	e7f4      	b.n	8008346 <__Init_Data+0x16>
 800835c:	0800850c 	.word	0x0800850c
 8008360:	20000000 	.word	0x20000000
 8008364:	20000004 	.word	0x20000004
 8008368:	20000004 	.word	0x20000004
 800836c:	20000008 	.word	0x20000008

08008370 <Reset_Handler>:
 8008370:	4668      	mov	r0, sp
 8008372:	f020 0107 	bic.w	r1, r0, #7
 8008376:	468d      	mov	sp, r1
 8008378:	b508      	push	{r3, lr}
 800837a:	f7ff ffd9 	bl	8008330 <__Init_Data>
 800837e:	f7ff ff9b 	bl	80082b8 <main>
 8008382:	e7fe      	b.n	8008382 <Reset_Handler+0x12>

08008384 <memcpy>:
 8008384:	4684      	mov	ip, r0
 8008386:	ea41 0300 	orr.w	r3, r1, r0
 800838a:	f013 0303 	ands.w	r3, r3, #3
 800838e:	d149      	bne.n	8008424 <memcpy+0xa0>
 8008390:	3a40      	subs	r2, #64	; 0x40
 8008392:	d323      	bcc.n	80083dc <memcpy+0x58>
 8008394:	680b      	ldr	r3, [r1, #0]
 8008396:	6003      	str	r3, [r0, #0]
 8008398:	684b      	ldr	r3, [r1, #4]
 800839a:	6043      	str	r3, [r0, #4]
 800839c:	688b      	ldr	r3, [r1, #8]
 800839e:	6083      	str	r3, [r0, #8]
 80083a0:	68cb      	ldr	r3, [r1, #12]
 80083a2:	60c3      	str	r3, [r0, #12]
 80083a4:	690b      	ldr	r3, [r1, #16]
 80083a6:	6103      	str	r3, [r0, #16]
 80083a8:	694b      	ldr	r3, [r1, #20]
 80083aa:	6143      	str	r3, [r0, #20]
 80083ac:	698b      	ldr	r3, [r1, #24]
 80083ae:	6183      	str	r3, [r0, #24]
 80083b0:	69cb      	ldr	r3, [r1, #28]
 80083b2:	61c3      	str	r3, [r0, #28]
 80083b4:	6a0b      	ldr	r3, [r1, #32]
 80083b6:	6203      	str	r3, [r0, #32]
 80083b8:	6a4b      	ldr	r3, [r1, #36]	; 0x24
 80083ba:	6243      	str	r3, [r0, #36]	; 0x24
 80083bc:	6a8b      	ldr	r3, [r1, #40]	; 0x28
 80083be:	6283      	str	r3, [r0, #40]	; 0x28
 80083c0:	6acb      	ldr	r3, [r1, #44]	; 0x2c
 80083c2:	62c3      	str	r3, [r0, #44]	; 0x2c
 80083c4:	6b0b      	ldr	r3, [r1, #48]	; 0x30
 80083c6:	6303      	str	r3, [r0, #48]	; 0x30
 80083c8:	6b4b      	ldr	r3, [r1, #52]	; 0x34
 80083ca:	6343      	str	r3, [r0, #52]	; 0x34
 80083cc:	6b8b      	ldr	r3, [r1, #56]	; 0x38
 80083ce:	6383      	str	r3, [r0, #56]	; 0x38
 80083d0:	6bcb      	ldr	r3, [r1, #60]	; 0x3c
 80083d2:	63c3      	str	r3, [r0, #60]	; 0x3c
 80083d4:	3040      	adds	r0, #64	; 0x40
 80083d6:	3140      	adds	r1, #64	; 0x40
 80083d8:	3a40      	subs	r2, #64	; 0x40
 80083da:	d2db      	bcs.n	8008394 <memcpy+0x10>
 80083dc:	3230      	adds	r2, #48	; 0x30
 80083de:	d30b      	bcc.n	80083f8 <memcpy+0x74>
 80083e0:	680b      	ldr	r3, [r1, #0]
 80083e2:	6003      	str	r3, [r0, #0]
 80083e4:	684b      	ldr	r3, [r1, #4]
 80083e6:	6043      	str	r3, [r0, #4]
 80083e8:	688b      	ldr	r3, [r1, #8]
 80083ea:	6083      	str	r3, [r0, #8]
 80083ec:	68cb      	ldr	r3, [r1, #12]
 80083ee:	60c3      	str	r3, [r0, #12]
 80083f0:	3010      	adds	r0, #16
 80083f2:	3110      	adds	r1, #16
 80083f4:	3a10      	subs	r2, #16
 80083f6:	d2f3      	bcs.n	80083e0 <memcpy+0x5c>
 80083f8:	320c      	adds	r2, #12
 80083fa:	d305      	bcc.n	8008408 <memcpy+0x84>
 80083fc:	f851 3b04 	ldr.w	r3, [r1], #4
 8008400:	f840 3b04 	str.w	r3, [r0], #4
 8008404:	3a04      	subs	r2, #4
 8008406:	d2f9      	bcs.n	80083fc <memcpy+0x78>
 8008408:	3204      	adds	r2, #4
 800840a:	d008      	beq.n	800841e <memcpy+0x9a>
 800840c:	07d2      	lsls	r2, r2, #31
 800840e:	bf1c      	itt	ne
 8008410:	f811 3b01 	ldrbne.w	r3, [r1], #1
 8008414:	f800 3b01 	strbne.w	r3, [r0], #1
 8008418:	d301      	bcc.n	800841e <memcpy+0x9a>
 800841a:	880b      	ldrh	r3, [r1, #0]
 800841c:	8003      	strh	r3, [r0, #0]
 800841e:	4660      	mov	r0, ip
 8008420:	4770      	bx	lr
 8008422:	bf00      	nop
 8008424:	2a08      	cmp	r2, #8
 8008426:	d313      	bcc.n	8008450 <memcpy+0xcc>
 8008428:	078b      	lsls	r3, r1, #30
 800842a:	d0b1      	beq.n	8008390 <memcpy+0xc>
 800842c:	f010 0303 	ands.w	r3, r0, #3
 8008430:	d0ae      	beq.n	8008390 <memcpy+0xc>
 8008432:	f1c3 0304 	rsb	r3, r3, #4
 8008436:	1ad2      	subs	r2, r2, r3
 8008438:	07db      	lsls	r3, r3, #31
 800843a:	bf1c      	itt	ne
 800843c:	f811 3b01 	ldrbne.w	r3, [r1], #1
 8008440:	f800 3b01 	strbne.w	r3, [r0], #1
 8008444:	d3a4      	bcc.n	8008390 <memcpy+0xc>
 8008446:	f831 3b02 	ldrh.w	r3, [r1], #2
 800844a:	f820 3b02 	strh.w	r3, [r0], #2
 800844e:	e79f      	b.n	8008390 <memcpy+0xc>
 8008450:	3a04      	subs	r2, #4
 8008452:	d3d9      	bcc.n	8008408 <memcpy+0x84>
 8008454:	3a01      	subs	r2, #1
 8008456:	f811 3b01 	ldrb.w	r3, [r1], #1
 800845a:	f800 3b01 	strb.w	r3, [r0], #1
 800845e:	d2f9      	bcs.n	8008454 <memcpy+0xd0>
 8008460:	780b      	ldrb	r3, [r1, #0]
 8008462:	7003      	strb	r3, [r0, #0]
 8008464:	784b      	ldrb	r3, [r1, #1]
 8008466:	7043      	strb	r3, [r0, #1]
 8008468:	788b      	ldrb	r3, [r1, #2]
 800846a:	7083      	strb	r3, [r0, #2]
 800846c:	4660      	mov	r0, ip
 800846e:	4770      	bx	lr
 8008470:	f000b508 	.word	0xf000b508
 8008474:	f000f86d 	.word	0xf000f86d
 8008478:	f000f863 	.word	0xf000f863
 800847c:	f000f879 	.word	0xf000f879
 8008480:	2001f86f 	.word	0x2001f86f
 8008484:	b500bd08 	.word	0xb500bd08
 8008488:	e008f8df 	.word	0xe008f8df
 800848c:	f85e44fe 	.word	0xf85e44fe
 8008490:	00ecff08 	.word	0x00ecff08
 8008494:	f2400000 	.word	0xf2400000
 8008498:	f2c00ce8 	.word	0xf2c00ce8
 800849c:	44fc0c00 	.word	0x44fc0c00
 80084a0:	f000f8dc 	.word	0xf000f8dc
 80084a4:	f240bf00 	.word	0xf240bf00
 80084a8:	f2c00cdc 	.word	0xf2c00cdc
 80084ac:	44fc0c00 	.word	0x44fc0c00
 80084b0:	f000f8dc 	.word	0xf000f8dc
 80084b4:	f240bf00 	.word	0xf240bf00
 80084b8:	f2c00cd0 	.word	0xf2c00cd0
 80084bc:	44fc0c00 	.word	0x44fc0c00
 80084c0:	f000f8dc 	.word	0xf000f8dc
 80084c4:	f240bf00 	.word	0xf240bf00
 80084c8:	f2c00cc4 	.word	0xf2c00cc4
 80084cc:	44fc0c00 	.word	0x44fc0c00
 80084d0:	f000f8dc 	.word	0xf000f8dc
 80084d4:	5018bf00 	.word	0x5018bf00
 80084d8:	00002000 	.word	0x00002000
 80084dc:	00000000 	.word	0x00000000
 80084e0:	50c00000 	.word	0x50c00000
 80084e4:	50c02000 	.word	0x50c02000
 80084e8:	50c02000 	.word	0x50c02000
 80084ec:	50c02000 	.word	0x50c02000
 80084f0:	00002000 	.word	0x00002000
 80084f4:	0000000a 	.word	0x0000000a
 80084f8:	00000136 	.word	0x00000136
 80084fc:	0000008c 	.word	0x0000008c
 8008500:	00000014 	.word	0x00000014
 8008504:	00000028 	.word	0x00000028
 8008508:	000000dc 	.word	0x000000dc
