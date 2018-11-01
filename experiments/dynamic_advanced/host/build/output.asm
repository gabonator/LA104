
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08008000 <g_pfnVectors>:
 8008000:	00 27 00 20 a1 82 00 08 5f 82 00 08 5f 82 00 08     .'. ...._..._...
 8008010:	5f 82 00 08 5f 82 00 08 5f 82 00 08 00 00 00 00     _..._..._.......
	...
 800802c:	5d 82 00 08 5d 82 00 08 00 00 00 00 5d 82 00 08     ]...].......]...
 800803c:	5d 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     ]..._..._..._...
 800804c:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 800805c:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 800806c:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 800807c:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 800808c:	5d 82 00 08 5d 82 00 08 5d 82 00 08 5d 82 00 08     ]...]...]...]...
 800809c:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 80080ac:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 80080bc:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 80080cc:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 80080dc:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 80080ec:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 80080fc:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 800810c:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 800811c:	5f 82 00 08 5f 82 00 08 5f 82 00 08 5f 82 00 08     _..._..._..._...
 800812c:	5f 82 00 08                                         _...

08008130 <random>:
 8008130:	4b07      	ldr	r3, [pc, #28]	; (8008150 <random+0x20>)
 8008132:	b510      	push	{r4, lr}
 8008134:	6819      	ldr	r1, [r3, #0]
 8008136:	4a07      	ldr	r2, [pc, #28]	; (8008154 <random+0x24>)
 8008138:	1c4c      	adds	r4, r1, #1
 800813a:	601c      	str	r4, [r3, #0]
 800813c:	4b06      	ldr	r3, [pc, #24]	; (8008158 <random+0x28>)
 800813e:	6810      	ldr	r0, [r2, #0]
 8008140:	440b      	add	r3, r1
 8008142:	4906      	ldr	r1, [pc, #24]	; (800815c <random+0x2c>)
 8008144:	fb01 3000 	mla	r0, r1, r0, r3
 8008148:	6010      	str	r0, [r2, #0]
 800814a:	f3c0 000e 	ubfx	r0, r0, #0, #15
 800814e:	bd10      	pop	{r4, pc}
 8008150:	20000004 	.word	0x20000004
 8008154:	20000000 	.word	0x20000000
 8008158:	002490f3 	.word	0x002490f3
 800815c:	007df121 	.word	0x007df121

08008160 <Set_Posi>:
 8008160:	b510      	push	{r4, lr}
 8008162:	460c      	mov	r4, r1
 8008164:	4601      	mov	r1, r0
 8008166:	200b      	movs	r0, #11
 8008168:	f7f7 ffca 	bl	8000100 <__Bios>
 800816c:	4621      	mov	r1, r4
 800816e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8008172:	200c      	movs	r0, #12
 8008174:	f7f7 bfc4 	b.w	8000100 <__Bios>

08008178 <Set_Pixel>:
 8008178:	4601      	mov	r1, r0
 800817a:	200d      	movs	r0, #13
 800817c:	f7f7 bfc0 	b.w	8000100 <__Bios>

08008180 <PutPixel>:
 8008180:	b510      	push	{r4, lr}
 8008182:	4614      	mov	r4, r2
 8008184:	f7ff ffec 	bl	8008160 <Set_Posi>
 8008188:	4620      	mov	r0, r4
 800818a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800818e:	f7ff bff3 	b.w	8008178 <Set_Pixel>

08008192 <WriteMem>:
 8008192:	b510      	push	{r4, lr}
 8008194:	4401      	add	r1, r0
 8008196:	1a12      	subs	r2, r2, r0
 8008198:	4288      	cmp	r0, r1
 800819a:	eb00 0302 	add.w	r3, r0, r2
 800819e:	d100      	bne.n	80081a2 <WriteMem+0x10>
 80081a0:	bd10      	pop	{r4, pc}
 80081a2:	f810 4b01 	ldrb.w	r4, [r0], #1
 80081a6:	701c      	strb	r4, [r3, #0]
 80081a8:	e7f6      	b.n	8008198 <WriteMem+0x6>

080081aa <WriteDword>:
 80081aa:	6001      	str	r1, [r0, #0]
 80081ac:	4770      	bx	lr

080081ae <ExecEntry>:
 80081ae:	4700      	bx	r0

080081b0 <experiment>:
 80081b0:	b570      	push	{r4, r5, r6, lr}
 80081b2:	b0bc      	sub	sp, #240	; 0xf0
 80081b4:	22a8      	movs	r2, #168	; 0xa8
 80081b6:	491c      	ldr	r1, [pc, #112]	; (8008228 <experiment+0x78>)
 80081b8:	a812      	add	r0, sp, #72	; 0x48
 80081ba:	f000 f87b 	bl	80082b4 <memcpy>
 80081be:	4a1b      	ldr	r2, [pc, #108]	; (800822c <experiment+0x7c>)
 80081c0:	21a8      	movs	r1, #168	; 0xa8
 80081c2:	a812      	add	r0, sp, #72	; 0x48
 80081c4:	f7ff ffe5 	bl	8008192 <WriteMem>
 80081c8:	aa06      	add	r2, sp, #24
 80081ca:	4615      	mov	r5, r2
 80081cc:	4b18      	ldr	r3, [pc, #96]	; (8008230 <experiment+0x80>)
 80081ce:	f103 0630 	add.w	r6, r3, #48	; 0x30
 80081d2:	4614      	mov	r4, r2
 80081d4:	6818      	ldr	r0, [r3, #0]
 80081d6:	6859      	ldr	r1, [r3, #4]
 80081d8:	3308      	adds	r3, #8
 80081da:	c403      	stmia	r4!, {r0, r1}
 80081dc:	42b3      	cmp	r3, r6
 80081de:	4622      	mov	r2, r4
 80081e0:	d1f7      	bne.n	80081d2 <experiment+0x22>
 80081e2:	4a14      	ldr	r2, [pc, #80]	; (8008234 <experiment+0x84>)
 80081e4:	4628      	mov	r0, r5
 80081e6:	2130      	movs	r1, #48	; 0x30
 80081e8:	f7ff ffd3 	bl	8008192 <WriteMem>
 80081ec:	aa01      	add	r2, sp, #4
 80081ee:	4615      	mov	r5, r2
 80081f0:	4b11      	ldr	r3, [pc, #68]	; (8008238 <experiment+0x88>)
 80081f2:	f103 0610 	add.w	r6, r3, #16
 80081f6:	4614      	mov	r4, r2
 80081f8:	6818      	ldr	r0, [r3, #0]
 80081fa:	6859      	ldr	r1, [r3, #4]
 80081fc:	3308      	adds	r3, #8
 80081fe:	c403      	stmia	r4!, {r0, r1}
 8008200:	42b3      	cmp	r3, r6
 8008202:	4622      	mov	r2, r4
 8008204:	d1f7      	bne.n	80081f6 <experiment+0x46>
 8008206:	6818      	ldr	r0, [r3, #0]
 8008208:	2114      	movs	r1, #20
 800820a:	6020      	str	r0, [r4, #0]
 800820c:	4a0b      	ldr	r2, [pc, #44]	; (800823c <experiment+0x8c>)
 800820e:	4628      	mov	r0, r5
 8008210:	f7ff ffbf 	bl	8008192 <WriteMem>
 8008214:	4b0a      	ldr	r3, [pc, #40]	; (8008240 <experiment+0x90>)
 8008216:	4a0b      	ldr	r2, [pc, #44]	; (8008244 <experiment+0x94>)
 8008218:	480b      	ldr	r0, [pc, #44]	; (8008248 <experiment+0x98>)
 800821a:	601a      	str	r2, [r3, #0]
 800821c:	4a0b      	ldr	r2, [pc, #44]	; (800824c <experiment+0x9c>)
 800821e:	605a      	str	r2, [r3, #4]
 8008220:	f7ff ffc5 	bl	80081ae <ExecEntry>
 8008224:	b03c      	add	sp, #240	; 0xf0
 8008226:	bd70      	pop	{r4, r5, r6, pc}
 8008228:	080083a0 	.word	0x080083a0
 800822c:	20005000 	.word	0x20005000
 8008230:	08008448 	.word	0x08008448
 8008234:	20005140 	.word	0x20005140
 8008238:	08008478 	.word	0x08008478
 800823c:	200051e0 	.word	0x200051e0
 8008240:	200051ec 	.word	0x200051ec
 8008244:	08008131 	.word	0x08008131
 8008248:	20005001 	.word	0x20005001
 800824c:	08008181 	.word	0x08008181

08008250 <main>:
 8008250:	b508      	push	{r3, lr}
 8008252:	f7ff ffad 	bl	80081b0 <experiment>
 8008256:	2000      	movs	r0, #0
 8008258:	bd08      	pop	{r3, pc}
	...

0800825c <Dummy_Handler>:
 800825c:	4770      	bx	lr

0800825e <Default_Handler>:
 800825e:	e7fe      	b.n	800825e <Default_Handler>

08008260 <__Init_Data>:
 8008260:	4a0a      	ldr	r2, [pc, #40]	; (800828c <__Init_Data+0x2c>)
 8008262:	4b0b      	ldr	r3, [pc, #44]	; (8008290 <__Init_Data+0x30>)
 8008264:	429a      	cmp	r2, r3
 8008266:	d003      	beq.n	8008270 <__Init_Data+0x10>
 8008268:	490a      	ldr	r1, [pc, #40]	; (8008294 <__Init_Data+0x34>)
 800826a:	3a04      	subs	r2, #4
 800826c:	428b      	cmp	r3, r1
 800826e:	d305      	bcc.n	800827c <__Init_Data+0x1c>
 8008270:	2100      	movs	r1, #0
 8008272:	4b09      	ldr	r3, [pc, #36]	; (8008298 <__Init_Data+0x38>)
 8008274:	4a09      	ldr	r2, [pc, #36]	; (800829c <__Init_Data+0x3c>)
 8008276:	4293      	cmp	r3, r2
 8008278:	d305      	bcc.n	8008286 <__Init_Data+0x26>
 800827a:	4770      	bx	lr
 800827c:	f852 0f04 	ldr.w	r0, [r2, #4]!
 8008280:	f843 0b04 	str.w	r0, [r3], #4
 8008284:	e7f2      	b.n	800826c <__Init_Data+0xc>
 8008286:	f843 1b04 	str.w	r1, [r3], #4
 800828a:	e7f4      	b.n	8008276 <__Init_Data+0x16>
 800828c:	0800848c 	.word	0x0800848c
 8008290:	20000000 	.word	0x20000000
 8008294:	20000004 	.word	0x20000004
 8008298:	20000004 	.word	0x20000004
 800829c:	20000008 	.word	0x20000008

080082a0 <Reset_Handler>:
 80082a0:	4668      	mov	r0, sp
 80082a2:	f020 0107 	bic.w	r1, r0, #7
 80082a6:	468d      	mov	sp, r1
 80082a8:	b508      	push	{r3, lr}
 80082aa:	f7ff ffd9 	bl	8008260 <__Init_Data>
 80082ae:	f7ff ffcf 	bl	8008250 <main>
 80082b2:	e7fe      	b.n	80082b2 <Reset_Handler+0x12>

080082b4 <memcpy>:
 80082b4:	4684      	mov	ip, r0
 80082b6:	ea41 0300 	orr.w	r3, r1, r0
 80082ba:	f013 0303 	ands.w	r3, r3, #3
 80082be:	d149      	bne.n	8008354 <memcpy+0xa0>
 80082c0:	3a40      	subs	r2, #64	; 0x40
 80082c2:	d323      	bcc.n	800830c <memcpy+0x58>
 80082c4:	680b      	ldr	r3, [r1, #0]
 80082c6:	6003      	str	r3, [r0, #0]
 80082c8:	684b      	ldr	r3, [r1, #4]
 80082ca:	6043      	str	r3, [r0, #4]
 80082cc:	688b      	ldr	r3, [r1, #8]
 80082ce:	6083      	str	r3, [r0, #8]
 80082d0:	68cb      	ldr	r3, [r1, #12]
 80082d2:	60c3      	str	r3, [r0, #12]
 80082d4:	690b      	ldr	r3, [r1, #16]
 80082d6:	6103      	str	r3, [r0, #16]
 80082d8:	694b      	ldr	r3, [r1, #20]
 80082da:	6143      	str	r3, [r0, #20]
 80082dc:	698b      	ldr	r3, [r1, #24]
 80082de:	6183      	str	r3, [r0, #24]
 80082e0:	69cb      	ldr	r3, [r1, #28]
 80082e2:	61c3      	str	r3, [r0, #28]
 80082e4:	6a0b      	ldr	r3, [r1, #32]
 80082e6:	6203      	str	r3, [r0, #32]
 80082e8:	6a4b      	ldr	r3, [r1, #36]	; 0x24
 80082ea:	6243      	str	r3, [r0, #36]	; 0x24
 80082ec:	6a8b      	ldr	r3, [r1, #40]	; 0x28
 80082ee:	6283      	str	r3, [r0, #40]	; 0x28
 80082f0:	6acb      	ldr	r3, [r1, #44]	; 0x2c
 80082f2:	62c3      	str	r3, [r0, #44]	; 0x2c
 80082f4:	6b0b      	ldr	r3, [r1, #48]	; 0x30
 80082f6:	6303      	str	r3, [r0, #48]	; 0x30
 80082f8:	6b4b      	ldr	r3, [r1, #52]	; 0x34
 80082fa:	6343      	str	r3, [r0, #52]	; 0x34
 80082fc:	6b8b      	ldr	r3, [r1, #56]	; 0x38
 80082fe:	6383      	str	r3, [r0, #56]	; 0x38
 8008300:	6bcb      	ldr	r3, [r1, #60]	; 0x3c
 8008302:	63c3      	str	r3, [r0, #60]	; 0x3c
 8008304:	3040      	adds	r0, #64	; 0x40
 8008306:	3140      	adds	r1, #64	; 0x40
 8008308:	3a40      	subs	r2, #64	; 0x40
 800830a:	d2db      	bcs.n	80082c4 <memcpy+0x10>
 800830c:	3230      	adds	r2, #48	; 0x30
 800830e:	d30b      	bcc.n	8008328 <memcpy+0x74>
 8008310:	680b      	ldr	r3, [r1, #0]
 8008312:	6003      	str	r3, [r0, #0]
 8008314:	684b      	ldr	r3, [r1, #4]
 8008316:	6043      	str	r3, [r0, #4]
 8008318:	688b      	ldr	r3, [r1, #8]
 800831a:	6083      	str	r3, [r0, #8]
 800831c:	68cb      	ldr	r3, [r1, #12]
 800831e:	60c3      	str	r3, [r0, #12]
 8008320:	3010      	adds	r0, #16
 8008322:	3110      	adds	r1, #16
 8008324:	3a10      	subs	r2, #16
 8008326:	d2f3      	bcs.n	8008310 <memcpy+0x5c>
 8008328:	320c      	adds	r2, #12
 800832a:	d305      	bcc.n	8008338 <memcpy+0x84>
 800832c:	f851 3b04 	ldr.w	r3, [r1], #4
 8008330:	f840 3b04 	str.w	r3, [r0], #4
 8008334:	3a04      	subs	r2, #4
 8008336:	d2f9      	bcs.n	800832c <memcpy+0x78>
 8008338:	3204      	adds	r2, #4
 800833a:	d008      	beq.n	800834e <memcpy+0x9a>
 800833c:	07d2      	lsls	r2, r2, #31
 800833e:	bf1c      	itt	ne
 8008340:	f811 3b01 	ldrbne.w	r3, [r1], #1
 8008344:	f800 3b01 	strbne.w	r3, [r0], #1
 8008348:	d301      	bcc.n	800834e <memcpy+0x9a>
 800834a:	880b      	ldrh	r3, [r1, #0]
 800834c:	8003      	strh	r3, [r0, #0]
 800834e:	4660      	mov	r0, ip
 8008350:	4770      	bx	lr
 8008352:	bf00      	nop
 8008354:	2a08      	cmp	r2, #8
 8008356:	d313      	bcc.n	8008380 <memcpy+0xcc>
 8008358:	078b      	lsls	r3, r1, #30
 800835a:	d0b1      	beq.n	80082c0 <memcpy+0xc>
 800835c:	f010 0303 	ands.w	r3, r0, #3
 8008360:	d0ae      	beq.n	80082c0 <memcpy+0xc>
 8008362:	f1c3 0304 	rsb	r3, r3, #4
 8008366:	1ad2      	subs	r2, r2, r3
 8008368:	07db      	lsls	r3, r3, #31
 800836a:	bf1c      	itt	ne
 800836c:	f811 3b01 	ldrbne.w	r3, [r1], #1
 8008370:	f800 3b01 	strbne.w	r3, [r0], #1
 8008374:	d3a4      	bcc.n	80082c0 <memcpy+0xc>
 8008376:	f831 3b02 	ldrh.w	r3, [r1], #2
 800837a:	f820 3b02 	strh.w	r3, [r0], #2
 800837e:	e79f      	b.n	80082c0 <memcpy+0xc>
 8008380:	3a04      	subs	r2, #4
 8008382:	d3d9      	bcc.n	8008338 <memcpy+0x84>
 8008384:	3a01      	subs	r2, #1
 8008386:	f811 3b01 	ldrb.w	r3, [r1], #1
 800838a:	f800 3b01 	strb.w	r3, [r0], #1
 800838e:	d2f9      	bcs.n	8008384 <memcpy+0xd0>
 8008390:	780b      	ldrb	r3, [r1, #0]
 8008392:	7003      	strb	r3, [r0, #0]
 8008394:	784b      	ldrb	r3, [r1, #1]
 8008396:	7043      	strb	r3, [r0, #1]
 8008398:	788b      	ldrb	r3, [r1, #2]
 800839a:	7083      	strb	r3, [r0, #2]
 800839c:	4660      	mov	r0, ip
 800839e:	4770      	bx	lr
 80083a0:	b087b5f0 	.word	0xb087b5f0
 80083a4:	4b21466c 	.word	0x4b21466c
 80083a8:	0007e893 	.word	0x0007e893
 80083ac:	0007e884 	.word	0x0007e884
 80083b0:	330c2400 	.word	0x330c2400
 80083b4:	0007e893 	.word	0x0007e893
 80083b8:	e903ab06 	.word	0xe903ab06
 80083bc:	25000007 	.word	0x25000007
 80083c0:	03a0f1a4 	.word	0x03a0f1a4
 80083c4:	f603fb03 	.word	0xf603fb03
 80083c8:	f1a5b2a7 	.word	0xf1a5b2a7
 80083cc:	fb020278 	.word	0xfb020278
 80083d0:	02126202 	.word	0x02126202
 80083d4:	f402b2a9 	.word	0xf402b2a9
 80083d8:	46384278 	.word	0x46384278
 80083dc:	f0003501 	.word	0xf0003501
 80083e0:	2df0f88f 	.word	0x2df0f88f
 80083e4:	3401d1f1 	.word	0x3401d1f1
 80083e8:	7fa0f5b4 	.word	0x7fa0f5b4
 80083ec:	2714d1e7 	.word	0x2714d1e7
 80083f0:	2603240a 	.word	0x2603240a
 80083f4:	f0002502 	.word	0xf0002502
 80083f8:	fb90f87b 	.word	0xfb90f87b
 80083fc:	eb03f3f6 	.word	0xeb03f3f6
 8008400:	aa060343 	.word	0xaa060343
 8008404:	eb021ac3 	.word	0xeb021ac3
 8008408:	f8530383 	.word	0xf8530383
 800840c:	f8532c18 	.word	0xf8532c18
 8008410:	44143c0c 	.word	0x44143c0c
 8008414:	fb94441f 	.word	0xfb94441f
 8008418:	fb97f4f5 	.word	0xfb97f4f5
 800841c:	0040f7f5 	.word	0x0040f7f5
 8008420:	b2b9b282 	.word	0xb2b9b282
 8008424:	f000b2a0 	.word	0xf000b2a0
 8008428:	e7e4f86b 	.word	0xe7e4f86b
 800842c:	20005090 	.word	0x20005090
 8008430:	0000000a 	.word	0x0000000a
 8008434:	00000136 	.word	0x00000136
 8008438:	0000008c 	.word	0x0000008c
 800843c:	00000014 	.word	0x00000014
 8008440:	00000028 	.word	0x00000028
 8008444:	000000dc 	.word	0x000000dc
 8008448:	f8dfb500 	.word	0xf8dfb500
 800844c:	44fee008 	.word	0x44fee008
 8008450:	ff08f85e 	.word	0xff08f85e
 8008454:	00000094 	.word	0x00000094
 8008458:	0c90f240 	.word	0x0c90f240
 800845c:	0c00f2c0 	.word	0x0c00f2c0
 8008460:	f8dc44fc 	.word	0xf8dc44fc
 8008464:	bf00f000 	.word	0xbf00f000
 8008468:	0c84f240 	.word	0x0c84f240
 800846c:	0c00f2c0 	.word	0x0c00f2c0
 8008470:	f8dc44fc 	.word	0xf8dc44fc
 8008474:	bf00f000 	.word	0xbf00f000
 8008478:	200050a8 	.word	0x200050a8
	...
 8008484:	20005140 	.word	0x20005140
 8008488:	20005140 	.word	0x20005140
