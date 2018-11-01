
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08008000 <g_pfnVectors>:
 8008000:	00 9a 00 20 75 81 00 08 01 b9 00 08 03 b9 00 08     ... u...........
 8008010:	05 b9 00 08 07 b9 00 08 09 b9 00 08 00 00 00 00     ................
	...
 800802c:	0d b9 00 08 0b b9 00 08 00 00 00 00 0f b9 00 08     ................
 800803c:	11 b9 00 08 b9 ba 00 08 bb ba 00 08 bd ba 00 08     ................
 800804c:	bf ba 00 08 c1 ba 00 08 c3 ba 00 08 c5 ba 00 08     ................
 800805c:	c7 ba 00 08 c9 ba 00 08 cb ba 00 08 cd ba 00 08     ................
 800806c:	cf ba 00 08 d1 ba 00 08 d3 ba 00 08 d5 ba 00 08     ................
 800807c:	d7 ba 00 08 d9 ba 00 08 db ba 00 08 dd ba 00 08     ................
 800808c:	df ba 00 08 e1 ba 00 08 33 81 00 08 33 81 00 08     ........3...3...
 800809c:	e9 ba 00 08 eb ba 00 08 ed ba 00 08 ef ba 00 08     ................
 80080ac:	f1 ba 00 08 f3 ba 00 08 f5 ba 00 08 f7 ba 00 08     ................
 80080bc:	f9 ba 00 08 fb ba 00 08 fd ba 00 08 ff ba 00 08     ................
 80080cc:	01 bb 00 08 03 bb 00 08 05 bb 00 08 07 bb 00 08     ................
 80080dc:	09 bb 00 08 0b bb 00 08 0d bb 00 08 0f bb 00 08     ................
 80080ec:	11 bb 00 08 13 bb 00 08 15 bb 00 08 17 bb 00 08     ................
 80080fc:	19 bb 00 08 1b bb 00 08 1d bb 00 08 1f bb 00 08     ................
 800810c:	21 bb 00 08 23 bb 00 08 25 bb 00 08 27 bb 00 08     !...#...%...'...
 800811c:	29 bb 00 08 2b bb 00 08 2d bb 00 08 2f bb 00 08     )...+...-.../...
 800812c:	31 bb 00 08                                         1...

08008130 <Dummy_Handler>:
 8008130:	4770      	bx	lr

08008132 <Default_Handler>:
 8008132:	e7fe      	b.n	8008132 <Default_Handler>

08008134 <__Init_Data>:
 8008134:	4a0a      	ldr	r2, [pc, #40]	; (8008160 <__Init_Data+0x2c>)
 8008136:	4b0b      	ldr	r3, [pc, #44]	; (8008164 <__Init_Data+0x30>)
 8008138:	429a      	cmp	r2, r3
 800813a:	d002      	beq.n	8008142 <__Init_Data+0xe>
 800813c:	490a      	ldr	r1, [pc, #40]	; (8008168 <__Init_Data+0x34>)
 800813e:	428b      	cmp	r3, r1
 8008140:	d305      	bcc.n	800814e <__Init_Data+0x1a>
 8008142:	2100      	movs	r1, #0
 8008144:	4b09      	ldr	r3, [pc, #36]	; (800816c <__Init_Data+0x38>)
 8008146:	4a0a      	ldr	r2, [pc, #40]	; (8008170 <__Init_Data+0x3c>)
 8008148:	4293      	cmp	r3, r2
 800814a:	d305      	bcc.n	8008158 <__Init_Data+0x24>
 800814c:	4770      	bx	lr
 800814e:	f852 0b04 	ldr.w	r0, [r2], #4
 8008152:	f843 0b04 	str.w	r0, [r3], #4
 8008156:	e7f2      	b.n	800813e <__Init_Data+0xa>
 8008158:	f843 1b04 	str.w	r1, [r3], #4
 800815c:	e7f4      	b.n	8008148 <__Init_Data+0x14>
 800815e:	bf00      	nop
 8008160:	08014878 	.word	0x08014878
 8008164:	20000000 	.word	0x20000000
 8008168:	2000023c 	.word	0x2000023c
 800816c:	20000240 	.word	0x20000240
 8008170:	200095d7 	.word	0x200095d7

08008174 <Reset_Handler>:
 8008174:	b508      	push	{r3, lr}
 8008176:	f7ff ffdd 	bl	8008134 <__Init_Data>
 800817a:	f000 f94d 	bl	8008418 <main>
 800817e:	e7fe      	b.n	800817e <Reset_Handler+0xa>

08008180 <Hardware_Init>:
 8008180:	b510      	push	{r4, lr}
 8008182:	4c20      	ldr	r4, [pc, #128]	; (8008204 <Hardware_Init+0x84>)
 8008184:	2004      	movs	r0, #4
 8008186:	4621      	mov	r1, r4
 8008188:	f7f7 ffba 	bl	8000100 <__Bios>
 800818c:	4621      	mov	r1, r4
 800818e:	2006      	movs	r0, #6
 8008190:	f7f7 ffb6 	bl	8000100 <__Bios>
 8008194:	f44f 4100 	mov.w	r1, #32768	; 0x8000
 8008198:	2001      	movs	r0, #1
 800819a:	f7f7 ffb1 	bl	8000100 <__Bios>
 800819e:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
 80081a2:	4b19      	ldr	r3, [pc, #100]	; (8008208 <Hardware_Init+0x88>)
 80081a4:	21f0      	movs	r1, #240	; 0xf0
 80081a6:	681b      	ldr	r3, [r3, #0]
 80081a8:	2005      	movs	r0, #5
 80081aa:	fbb3 f3f2 	udiv	r3, r3, r2
 80081ae:	4a17      	ldr	r2, [pc, #92]	; (800820c <Hardware_Init+0x8c>)
 80081b0:	3b01      	subs	r3, #1
 80081b2:	6053      	str	r3, [r2, #4]
 80081b4:	4b16      	ldr	r3, [pc, #88]	; (8008210 <Hardware_Init+0x90>)
 80081b6:	f883 1023 	strb.w	r1, [r3, #35]	; 0x23
 80081ba:	2300      	movs	r3, #0
 80081bc:	6093      	str	r3, [r2, #8]
 80081be:	2307      	movs	r3, #7
 80081c0:	4621      	mov	r1, r4
 80081c2:	6013      	str	r3, [r2, #0]
 80081c4:	f7f7 ff9c 	bl	8000100 <__Bios>
 80081c8:	2132      	movs	r1, #50	; 0x32
 80081ca:	2005      	movs	r0, #5
 80081cc:	f7f7 ff98 	bl	8000100 <__Bios>
 80081d0:	20c8      	movs	r0, #200	; 0xc8
 80081d2:	f003 fc5b 	bl	800ba8c <Beep_mS>
 80081d6:	4621      	mov	r1, r4
 80081d8:	2015      	movs	r0, #21
 80081da:	f7f7 ff91 	bl	8000100 <__Bios>
 80081de:	4621      	mov	r1, r4
 80081e0:	2012      	movs	r0, #18
 80081e2:	f7f7 ff8d 	bl	8000100 <__Bios>
 80081e6:	f004 f884 	bl	800c2f2 <DiskConfig>
 80081ea:	4621      	mov	r1, r4
 80081ec:	2011      	movs	r0, #17
 80081ee:	f7f7 ff87 	bl	8000100 <__Bios>
 80081f2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80081f6:	2164      	movs	r1, #100	; 0x64
 80081f8:	4b06      	ldr	r3, [pc, #24]	; (8008214 <Hardware_Init+0x94>)
 80081fa:	2011      	movs	r0, #17
 80081fc:	8e1b      	ldrh	r3, [r3, #48]	; 0x30
 80081fe:	4359      	muls	r1, r3
 8008200:	f7f7 bf7e 	b.w	8000100 <__Bios>
 8008204:	ffff0000 	.word	0xffff0000
 8008208:	2000011c 	.word	0x2000011c
 800820c:	e000e010 	.word	0xe000e010
 8008210:	e000ed00 	.word	0xe000ed00
 8008214:	2000000c 	.word	0x2000000c

08008218 <Show_Startup_Info>:
 8008218:	b537      	push	{r0, r1, r2, r4, r5, lr}
 800821a:	2400      	movs	r4, #0
 800821c:	4d1d      	ldr	r5, [pc, #116]	; (8008294 <Show_Startup_Info+0x7c>)
 800821e:	4623      	mov	r3, r4
 8008220:	4622      	mov	r2, r4
 8008222:	215a      	movs	r1, #90	; 0x5a
 8008224:	4620      	mov	r0, r4
 8008226:	9501      	str	r5, [sp, #4]
 8008228:	9400      	str	r4, [sp, #0]
 800822a:	f003 fce9 	bl	800bc00 <Disp_Str8x14>
 800822e:	4623      	mov	r3, r4
 8008230:	4622      	mov	r2, r4
 8008232:	2146      	movs	r1, #70	; 0x46
 8008234:	4620      	mov	r0, r4
 8008236:	e9cd 4500 	strd	r4, r5, [sp]
 800823a:	f003 fce1 	bl	800bc00 <Disp_Str8x14>
 800823e:	4b16      	ldr	r3, [pc, #88]	; (8008298 <Show_Startup_Info+0x80>)
 8008240:	f64f 72ff 	movw	r2, #65535	; 0xffff
 8008244:	e9cd 4300 	strd	r4, r3, [sp]
 8008248:	215a      	movs	r1, #90	; 0x5a
 800824a:	4623      	mov	r3, r4
 800824c:	2008      	movs	r0, #8
 800824e:	f003 fcd7 	bl	800bc00 <Disp_Str8x14>
 8008252:	4b12      	ldr	r3, [pc, #72]	; (800829c <Show_Startup_Info+0x84>)
 8008254:	f64f 72ff 	movw	r2, #65535	; 0xffff
 8008258:	e9cd 4300 	strd	r4, r3, [sp]
 800825c:	215a      	movs	r1, #90	; 0x5a
 800825e:	4623      	mov	r3, r4
 8008260:	20d8      	movs	r0, #216	; 0xd8
 8008262:	f003 fccd 	bl	800bc00 <Disp_Str8x14>
 8008266:	4b0e      	ldr	r3, [pc, #56]	; (80082a0 <Show_Startup_Info+0x88>)
 8008268:	2146      	movs	r1, #70	; 0x46
 800826a:	e9cd 4300 	strd	r4, r3, [sp]
 800826e:	f64f 72ff 	movw	r2, #65535	; 0xffff
 8008272:	4623      	mov	r3, r4
 8008274:	2008      	movs	r0, #8
 8008276:	f003 fcc3 	bl	800bc00 <Disp_Str8x14>
 800827a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
 800827e:	f004 f815 	bl	800c2ac <Delay_mS>
 8008282:	4908      	ldr	r1, [pc, #32]	; (80082a4 <Show_Startup_Info+0x8c>)
 8008284:	2010      	movs	r0, #16
 8008286:	f7f7 ff3b 	bl	8000100 <__Bios>
 800828a:	b900      	cbnz	r0, 800828e <Show_Startup_Info+0x76>
 800828c:	e7fe      	b.n	800828c <Show_Startup_Info+0x74>
 800828e:	b003      	add	sp, #12
 8008290:	bd30      	pop	{r4, r5, pc}
 8008292:	bf00      	nop
 8008294:	08012ba0 	.word	0x08012ba0
 8008298:	08012bc9 	.word	0x08012bc9
 800829c:	20000000 	.word	0x20000000
 80082a0:	08012be3 	.word	0x08012be3
 80082a4:	fffa0000 	.word	0xfffa0000

080082a8 <FPGA_Startup_Wave>:
 80082a8:	2200      	movs	r2, #0
 80082aa:	b510      	push	{r4, lr}
 80082ac:	4611      	mov	r1, r2
 80082ae:	2001      	movs	r0, #1
 80082b0:	f003 ff9c 	bl	800c1ec <FPGA_ReadWrite>
 80082b4:	229a      	movs	r2, #154	; 0x9a
 80082b6:	2101      	movs	r1, #1
 80082b8:	2004      	movs	r0, #4
 80082ba:	f003 ff97 	bl	800c1ec <FPGA_ReadWrite>
 80082be:	f44f 728c 	mov.w	r2, #280	; 0x118
 80082c2:	2102      	movs	r1, #2
 80082c4:	2005      	movs	r0, #5
 80082c6:	f003 ff91 	bl	800c1ec <FPGA_ReadWrite>
 80082ca:	f240 1219 	movw	r2, #281	; 0x119
 80082ce:	2100      	movs	r1, #0
 80082d0:	4828      	ldr	r0, [pc, #160]	; (8008374 <FPGA_Startup_Wave+0xcc>)
 80082d2:	f00a fc32 	bl	8012b3a <memset>
 80082d6:	f240 1219 	movw	r2, #281	; 0x119
 80082da:	2100      	movs	r1, #0
 80082dc:	4826      	ldr	r0, [pc, #152]	; (8008378 <FPGA_Startup_Wave+0xd0>)
 80082de:	4c27      	ldr	r4, [pc, #156]	; (800837c <FPGA_Startup_Wave+0xd4>)
 80082e0:	f00a fc2b 	bl	8012b3a <memset>
 80082e4:	4a23      	ldr	r2, [pc, #140]	; (8008374 <FPGA_Startup_Wave+0xcc>)
 80082e6:	f44f 718c 	mov.w	r1, #280	; 0x118
 80082ea:	2003      	movs	r0, #3
 80082ec:	f003 ff96 	bl	800c21c <FPGA_LCD_RW>
 80082f0:	2005      	movs	r0, #5
 80082f2:	f003 ffdb 	bl	800c2ac <Delay_mS>
 80082f6:	8ba2      	ldrh	r2, [r4, #28]
 80082f8:	4b21      	ldr	r3, [pc, #132]	; (8008380 <FPGA_Startup_Wave+0xd8>)
 80082fa:	8ae1      	ldrh	r1, [r4, #22]
 80082fc:	801a      	strh	r2, [r3, #0]
 80082fe:	4b21      	ldr	r3, [pc, #132]	; (8008384 <FPGA_Startup_Wave+0xdc>)
 8008300:	2019      	movs	r0, #25
 8008302:	8019      	strh	r1, [r3, #0]
 8008304:	8b21      	ldrh	r1, [r4, #24]
 8008306:	4b20      	ldr	r3, [pc, #128]	; (8008388 <FPGA_Startup_Wave+0xe0>)
 8008308:	8019      	strh	r1, [r3, #0]
 800830a:	4b20      	ldr	r3, [pc, #128]	; (800838c <FPGA_Startup_Wave+0xe4>)
 800830c:	8be1      	ldrh	r1, [r4, #30]
 800830e:	8019      	strh	r1, [r3, #0]
 8008310:	2102      	movs	r1, #2
 8008312:	f003 ff6b 	bl	800c1ec <FPGA_ReadWrite>
 8008316:	8ae2      	ldrh	r2, [r4, #22]
 8008318:	2102      	movs	r1, #2
 800831a:	201a      	movs	r0, #26
 800831c:	f003 ff66 	bl	800c1ec <FPGA_ReadWrite>
 8008320:	8b22      	ldrh	r2, [r4, #24]
 8008322:	2102      	movs	r1, #2
 8008324:	201b      	movs	r0, #27
 8008326:	f003 ff61 	bl	800c1ec <FPGA_ReadWrite>
 800832a:	8be2      	ldrh	r2, [r4, #30]
 800832c:	2102      	movs	r1, #2
 800832e:	2018      	movs	r0, #24
 8008330:	f003 ff5c 	bl	800c1ec <FPGA_ReadWrite>
 8008334:	2002      	movs	r0, #2
 8008336:	f003 ffb9 	bl	800c2ac <Delay_mS>
 800833a:	f240 123f 	movw	r2, #319	; 0x13f
 800833e:	23db      	movs	r3, #219	; 0xdb
 8008340:	2142      	movs	r1, #66	; 0x42
 8008342:	2028      	movs	r0, #40	; 0x28
 8008344:	f003 fc2e 	bl	800bba4 <Set_Block>
 8008348:	4911      	ldr	r1, [pc, #68]	; (8008390 <FPGA_Startup_Wave+0xe8>)
 800834a:	2008      	movs	r0, #8
 800834c:	f7f7 fed8 	bl	8000100 <__Bios>
 8008350:	2200      	movs	r2, #0
 8008352:	2006      	movs	r0, #6
 8008354:	4611      	mov	r1, r2
 8008356:	f003 ff49 	bl	800c1ec <FPGA_ReadWrite>
 800835a:	2200      	movs	r2, #0
 800835c:	4611      	mov	r1, r2
 800835e:	4610      	mov	r0, r2
 8008360:	f003 ff44 	bl	800c1ec <FPGA_ReadWrite>
 8008364:	05c3      	lsls	r3, r0, #23
 8008366:	d4f8      	bmi.n	800835a <FPGA_Startup_Wave+0xb2>
 8008368:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800836c:	4909      	ldr	r1, [pc, #36]	; (8008394 <FPGA_Startup_Wave+0xec>)
 800836e:	2008      	movs	r0, #8
 8008370:	f7f7 bec6 	b.w	8000100 <__Bios>
 8008374:	200009da 	.word	0x200009da
 8008378:	20000af3 	.word	0x20000af3
 800837c:	2000000c 	.word	0x2000000c
 8008380:	200009a2 	.word	0x200009a2
 8008384:	2000099e 	.word	0x2000099e
 8008388:	200009a0 	.word	0x200009a0
 800838c:	200009a4 	.word	0x200009a4
 8008390:	fff90000 	.word	0xfff90000
 8008394:	fffb0000 	.word	0xfffb0000

08008398 <Set_SysParam>:
 8008398:	b538      	push	{r3, r4, r5, lr}
 800839a:	2101      	movs	r1, #1
 800839c:	2300      	movs	r3, #0
 800839e:	2006      	movs	r0, #6
 80083a0:	250a      	movs	r5, #10
 80083a2:	4a15      	ldr	r2, [pc, #84]	; (80083f8 <Set_SysParam+0x60>)
 80083a4:	4c15      	ldr	r4, [pc, #84]	; (80083fc <Set_SysParam+0x64>)
 80083a6:	7011      	strb	r1, [r2, #0]
 80083a8:	7050      	strb	r0, [r2, #1]
 80083aa:	7093      	strb	r3, [r2, #2]
 80083ac:	4a14      	ldr	r2, [pc, #80]	; (8008400 <Set_SysParam+0x68>)
 80083ae:	8aa0      	ldrh	r0, [r4, #20]
 80083b0:	7013      	strb	r3, [r2, #0]
 80083b2:	7053      	strb	r3, [r2, #1]
 80083b4:	7093      	strb	r3, [r2, #2]
 80083b6:	4a13      	ldr	r2, [pc, #76]	; (8008404 <Set_SysParam+0x6c>)
 80083b8:	8010      	strh	r0, [r2, #0]
 80083ba:	8d60      	ldrh	r0, [r4, #42]	; 0x2a
 80083bc:	4a12      	ldr	r2, [pc, #72]	; (8008408 <Set_SysParam+0x70>)
 80083be:	7010      	strb	r0, [r2, #0]
 80083c0:	8da0      	ldrh	r0, [r4, #44]	; 0x2c
 80083c2:	4a12      	ldr	r2, [pc, #72]	; (800840c <Set_SysParam+0x74>)
 80083c4:	7010      	strb	r0, [r2, #0]
 80083c6:	4a12      	ldr	r2, [pc, #72]	; (8008410 <Set_SysParam+0x78>)
 80083c8:	8013      	strh	r3, [r2, #0]
 80083ca:	4b12      	ldr	r3, [pc, #72]	; (8008414 <Set_SysParam+0x7c>)
 80083cc:	7019      	strb	r1, [r3, #0]
 80083ce:	f004 f9e9 	bl	800c7a4 <ResetPowerOffTime>
 80083d2:	f894 0050 	ldrb.w	r0, [r4, #80]	; 0x50
 80083d6:	f004 fe5f 	bl	800d098 <PIO_Init>
 80083da:	f8b4 107e 	ldrh.w	r1, [r4, #126]	; 0x7e
 80083de:	2008      	movs	r0, #8
 80083e0:	4369      	muls	r1, r5
 80083e2:	f7f7 fe8d 	bl	8000100 <__Bios>
 80083e6:	f8b4 107c 	ldrh.w	r1, [r4, #124]	; 0x7c
 80083ea:	2005      	movs	r0, #5
 80083ec:	4369      	muls	r1, r5
 80083ee:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80083f2:	f7f7 be85 	b.w	8000100 <__Bios>
 80083f6:	bf00      	nop
 80083f8:	20004c12 	.word	0x20004c12
 80083fc:	2000000c 	.word	0x2000000c
 8008400:	20004c0f 	.word	0x20004c0f
 8008404:	200009ce 	.word	0x200009ce
 8008408:	200009a7 	.word	0x200009a7
 800840c:	200009a6 	.word	0x200009a6
 8008410:	200009b6 	.word	0x200009b6
 8008414:	200009d8 	.word	0x200009d8

08008418 <main>:
 8008418:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800841c:	f7ff feb0 	bl	8008180 <Hardware_Init>
 8008420:	f7ff fefa 	bl	8008218 <Show_Startup_Info>
 8008424:	f006 fd0e 	bl	800ee44 <ReadParameter>
 8008428:	2000      	movs	r0, #0
 800842a:	f003 fbca 	bl	800bbc2 <Clr_Scrn>
 800842e:	2700      	movs	r7, #0
 8008430:	f7ff ff3a 	bl	80082a8 <FPGA_Startup_Wave>
 8008434:	f7ff ffb0 	bl	8008398 <Set_SysParam>
 8008438:	f002 fa8e 	bl	800a958 <ShowAllMenu>
 800843c:	f006 f91a 	bl	800e674 <File_Num>
 8008440:	4cb6      	ldr	r4, [pc, #728]	; (800871c <main+0x304>)
 8008442:	f8df 8308 	ldr.w	r8, [pc, #776]	; 800874c <main+0x334>
 8008446:	f004 fbc3 	bl	800cbd0 <ShutdownTest>
 800844a:	f004 fb95 	bl	800cb78 <ShortcutBMP>
 800844e:	4bb3      	ldr	r3, [pc, #716]	; (800871c <main+0x304>)
 8008450:	781b      	ldrb	r3, [r3, #0]
 8008452:	b10b      	cbz	r3, 8008458 <main+0x40>
 8008454:	f004 fe88 	bl	800d168 <KeyQuickAct>
 8008458:	f8b8 3000 	ldrh.w	r3, [r8]
 800845c:	2b00      	cmp	r3, #0
 800845e:	f000 80b5 	beq.w	80085cc <main+0x1b4>
 8008462:	f5b3 6f00 	cmp.w	r3, #2048	; 0x800
 8008466:	4dae      	ldr	r5, [pc, #696]	; (8008720 <main+0x308>)
 8008468:	f000 80f1 	beq.w	800864e <main+0x236>
 800846c:	d827      	bhi.n	80084be <main+0xa6>
 800846e:	f5b3 7f00 	cmp.w	r3, #512	; 0x200
 8008472:	4eac      	ldr	r6, [pc, #688]	; (8008724 <main+0x30c>)
 8008474:	f8df a2c0 	ldr.w	sl, [pc, #704]	; 8008738 <main+0x320>
 8008478:	d049      	beq.n	800850e <main+0xf6>
 800847a:	f5b3 6f80 	cmp.w	r3, #1024	; 0x400
 800847e:	f000 80b3 	beq.w	80085e8 <main+0x1d0>
 8008482:	f5b3 7f80 	cmp.w	r3, #256	; 0x100
 8008486:	d123      	bne.n	80084d0 <main+0xb8>
 8008488:	f04f 0b00 	mov.w	fp, #0
 800848c:	2301      	movs	r3, #1
 800848e:	f8a8 b000 	strh.w	fp, [r8]
 8008492:	7023      	strb	r3, [r4, #0]
 8008494:	f886 b000 	strb.w	fp, [r6]
 8008498:	f88a b000 	strb.w	fp, [sl]
 800849c:	f004 fbf2 	bl	800cc84 <Process>
 80084a0:	4658      	mov	r0, fp
 80084a2:	f001 fc9b 	bl	8009ddc <ShowSmplStatus>
 80084a6:	4ba0      	ldr	r3, [pc, #640]	; (8008728 <main+0x310>)
 80084a8:	f893 9000 	ldrb.w	r9, [r3]
 80084ac:	f1b9 0f00 	cmp.w	r9, #0
 80084b0:	d04d      	beq.n	800854e <main+0x136>
 80084b2:	2032      	movs	r0, #50	; 0x32
 80084b4:	f883 b000 	strb.w	fp, [r3]
 80084b8:	f003 fae8 	bl	800ba8c <Beep_mS>
 80084bc:	e008      	b.n	80084d0 <main+0xb8>
 80084be:	f5b3 5f00 	cmp.w	r3, #8192	; 0x2000
 80084c2:	f000 8248 	beq.w	8008956 <main+0x53e>
 80084c6:	d806      	bhi.n	80084d6 <main+0xbe>
 80084c8:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 80084cc:	f000 835d 	beq.w	8008b8a <main+0x772>
 80084d0:	f04f 0900 	mov.w	r9, #0
 80084d4:	e03b      	b.n	800854e <main+0x136>
 80084d6:	f5b3 4f80 	cmp.w	r3, #16384	; 0x4000
 80084da:	f000 817f 	beq.w	80087dc <main+0x3c4>
 80084de:	f5b3 4f00 	cmp.w	r3, #32768	; 0x8000
 80084e2:	d1f5      	bne.n	80084d0 <main+0xb8>
 80084e4:	7823      	ldrb	r3, [r4, #0]
 80084e6:	2b00      	cmp	r3, #0
 80084e8:	f000 8201 	beq.w	80088ee <main+0x4d6>
 80084ec:	f895 9001 	ldrb.w	r9, [r5, #1]
 80084f0:	4b8e      	ldr	r3, [pc, #568]	; (800872c <main+0x314>)
 80084f2:	f1b9 0f02 	cmp.w	r9, #2
 80084f6:	f040 81e0 	bne.w	80088ba <main+0x4a2>
 80084fa:	488d      	ldr	r0, [pc, #564]	; (8008730 <main+0x318>)
 80084fc:	8b1a      	ldrh	r2, [r3, #24]
 80084fe:	8b00      	ldrh	r0, [r0, #24]
 8008500:	8ad9      	ldrh	r1, [r3, #22]
 8008502:	1a12      	subs	r2, r2, r0
 8008504:	428a      	cmp	r2, r1
 8008506:	bfac      	ite	ge
 8008508:	831a      	strhge	r2, [r3, #24]
 800850a:	8319      	strhlt	r1, [r3, #24]
 800850c:	e178      	b.n	8008800 <main+0x3e8>
 800850e:	f894 9000 	ldrb.w	r9, [r4]
 8008512:	f1b9 0f00 	cmp.w	r9, #0
 8008516:	d009      	beq.n	800852c <main+0x114>
 8008518:	78a0      	ldrb	r0, [r4, #2]
 800851a:	f04f 0900 	mov.w	r9, #0
 800851e:	f003 f939 	bl	800b794 <ShowPopItem>
 8008522:	2301      	movs	r3, #1
 8008524:	f884 9000 	strb.w	r9, [r4]
 8008528:	7033      	strb	r3, [r6, #0]
 800852a:	e010      	b.n	800854e <main+0x136>
 800852c:	7833      	ldrb	r3, [r6, #0]
 800852e:	b173      	cbz	r3, 800854e <main+0x136>
 8008530:	78a0      	ldrb	r0, [r4, #2]
 8008532:	2800      	cmp	r0, #0
 8008534:	d14f      	bne.n	80085d6 <main+0x1be>
 8008536:	4b7d      	ldr	r3, [pc, #500]	; (800872c <main+0x314>)
 8008538:	881b      	ldrh	r3, [r3, #0]
 800853a:	b143      	cbz	r3, 800854e <main+0x136>
 800853c:	4648      	mov	r0, r9
 800853e:	7829      	ldrb	r1, [r5, #0]
 8008540:	f001 fa26 	bl	8009990 <ShowSubItem>
 8008544:	2301      	movs	r3, #1
 8008546:	f886 9000 	strb.w	r9, [r6]
 800854a:	f88a 3000 	strb.w	r3, [sl]
 800854e:	f8b8 3000 	ldrh.w	r3, [r8]
 8008552:	4a78      	ldr	r2, [pc, #480]	; (8008734 <main+0x31c>)
 8008554:	421a      	tst	r2, r3
 8008556:	d002      	beq.n	800855e <main+0x146>
 8008558:	2032      	movs	r0, #50	; 0x32
 800855a:	f003 fa97 	bl	800ba8c <Beep_mS>
 800855e:	f001 fd75 	bl	800a04c <ShowTopTitle>
 8008562:	4b70      	ldr	r3, [pc, #448]	; (8008724 <main+0x30c>)
 8008564:	781b      	ldrb	r3, [r3, #0]
 8008566:	b133      	cbz	r3, 8008576 <main+0x15e>
 8008568:	f8b8 3000 	ldrh.w	r3, [r8]
 800856c:	2b10      	cmp	r3, #16
 800856e:	d002      	beq.n	8008576 <main+0x15e>
 8008570:	78a0      	ldrb	r0, [r4, #2]
 8008572:	f003 f90f 	bl	800b794 <ShowPopItem>
 8008576:	4b70      	ldr	r3, [pc, #448]	; (8008738 <main+0x320>)
 8008578:	781b      	ldrb	r3, [r3, #0]
 800857a:	b11b      	cbz	r3, 8008584 <main+0x16c>
 800857c:	78a0      	ldrb	r0, [r4, #2]
 800857e:	5c29      	ldrb	r1, [r5, r0]
 8008580:	f001 fa06 	bl	8009990 <ShowSubItem>
 8008584:	78a0      	ldrb	r0, [r4, #2]
 8008586:	5c29      	ldrb	r1, [r5, r0]
 8008588:	f004 fc8a 	bl	800cea0 <HW_Ctrl>
 800858c:	f001 feca 	bl	800a324 <ShowWindowPrecent>
 8008590:	78a3      	ldrb	r3, [r4, #2]
 8008592:	2b01      	cmp	r3, #1
 8008594:	d101      	bne.n	800859a <main+0x182>
 8008596:	f001 ff95 	bl	800a4c4 <ShowTimeBaseStr>
 800859a:	f002 f805 	bl	800a5a8 <ShowTimeMarkValue>
 800859e:	f004 f983 	bl	800c8a8 <ShowMeasureValue>
 80085a2:	f004 f8ff 	bl	800c7a4 <ResetPowerOffTime>
 80085a6:	2300      	movs	r3, #0
 80085a8:	f8a8 3000 	strh.w	r3, [r8]
 80085ac:	f1b9 0f00 	cmp.w	r9, #0
 80085b0:	d00c      	beq.n	80085cc <main+0x1b4>
 80085b2:	f240 1219 	movw	r2, #281	; 0x119
 80085b6:	4961      	ldr	r1, [pc, #388]	; (800873c <main+0x324>)
 80085b8:	4861      	ldr	r0, [pc, #388]	; (8008740 <main+0x328>)
 80085ba:	f00a fab3 	bl	8012b24 <memcpy>
 80085be:	f003 ff09 	bl	800c3d4 <ShowWaveToLCD>
 80085c2:	78a3      	ldrb	r3, [r4, #2]
 80085c4:	2b01      	cmp	r3, #1
 80085c6:	d101      	bne.n	80085cc <main+0x1b4>
 80085c8:	f003 fffc 	bl	800c5c4 <ZC_Scale>
 80085cc:	f004 f922 	bl	800c814 <StandbyAndPowerOff>
 80085d0:	f001 fcc4 	bl	8009f5c <ShowBattery>
 80085d4:	e737      	b.n	8008446 <main+0x2e>
 80085d6:	2804      	cmp	r0, #4
 80085d8:	d1b9      	bne.n	800854e <main+0x136>
 80085da:	4b54      	ldr	r3, [pc, #336]	; (800872c <main+0x314>)
 80085dc:	f8b3 3050 	ldrh.w	r3, [r3, #80]	; 0x50
 80085e0:	2b00      	cmp	r3, #0
 80085e2:	d1b4      	bne.n	800854e <main+0x136>
 80085e4:	7929      	ldrb	r1, [r5, #4]
 80085e6:	e7ab      	b.n	8008540 <main+0x128>
 80085e8:	f896 9000 	ldrb.w	r9, [r6]
 80085ec:	f1b9 0f00 	cmp.w	r9, #0
 80085f0:	d00a      	beq.n	8008608 <main+0x1f0>
 80085f2:	f04f 0901 	mov.w	r9, #1
 80085f6:	78a3      	ldrb	r3, [r4, #2]
 80085f8:	f884 9000 	strb.w	r9, [r4]
 80085fc:	2b01      	cmp	r3, #1
 80085fe:	bf04      	itt	eq
 8008600:	4a50      	ldreq	r2, [pc, #320]	; (8008744 <main+0x32c>)
 8008602:	7013      	strbeq	r3, [r2, #0]
 8008604:	2300      	movs	r3, #0
 8008606:	e78f      	b.n	8008528 <main+0x110>
 8008608:	f89a 3000 	ldrb.w	r3, [sl]
 800860c:	b1cb      	cbz	r3, 8008642 <main+0x22a>
 800860e:	f04f 0b01 	mov.w	fp, #1
 8008612:	4b46      	ldr	r3, [pc, #280]	; (800872c <main+0x314>)
 8008614:	7818      	ldrb	r0, [r3, #0]
 8008616:	f005 fb37 	bl	800dc88 <AnalyzeFrame>
 800861a:	f240 1219 	movw	r2, #281	; 0x119
 800861e:	4947      	ldr	r1, [pc, #284]	; (800873c <main+0x324>)
 8008620:	4847      	ldr	r0, [pc, #284]	; (8008740 <main+0x328>)
 8008622:	f00a fa7f 	bl	8012b24 <memcpy>
 8008626:	f003 fed5 	bl	800c3d4 <ShowWaveToLCD>
 800862a:	78a0      	ldrb	r0, [r4, #2]
 800862c:	f884 b000 	strb.w	fp, [r4]
 8008630:	f003 f8b0 	bl	800b794 <ShowPopItem>
 8008634:	f88a 9000 	strb.w	r9, [sl]
 8008638:	f886 b000 	strb.w	fp, [r6]
 800863c:	f884 9000 	strb.w	r9, [r4]
 8008640:	e785      	b.n	800854e <main+0x136>
 8008642:	2301      	movs	r3, #1
 8008644:	f886 9000 	strb.w	r9, [r6]
 8008648:	7023      	strb	r3, [r4, #0]
 800864a:	4699      	mov	r9, r3
 800864c:	e77f      	b.n	800854e <main+0x136>
 800864e:	4b35      	ldr	r3, [pc, #212]	; (8008724 <main+0x30c>)
 8008650:	781b      	ldrb	r3, [r3, #0]
 8008652:	2b00      	cmp	r3, #0
 8008654:	f000 80b2 	beq.w	80087bc <main+0x3a4>
 8008658:	78a3      	ldrb	r3, [r4, #2]
 800865a:	2b02      	cmp	r3, #2
 800865c:	d13e      	bne.n	80086dc <main+0x2c4>
 800865e:	78ab      	ldrb	r3, [r5, #2]
 8008660:	2b01      	cmp	r3, #1
 8008662:	d11b      	bne.n	800869c <main+0x284>
 8008664:	4931      	ldr	r1, [pc, #196]	; (800872c <main+0x314>)
 8008666:	8d4a      	ldrh	r2, [r1, #42]	; 0x2a
 8008668:	f012 09ff 	ands.w	r9, r2, #255	; 0xff
 800866c:	f43f af6f 	beq.w	800854e <main+0x136>
 8008670:	f1b9 0f05 	cmp.w	r9, #5
 8008674:	f43f af2c 	beq.w	80084d0 <main+0xb8>
 8008678:	f109 0907 	add.w	r9, r9, #7
 800867c:	b210      	sxth	r0, r2
 800867e:	fa42 f209 	asr.w	r2, r2, r9
 8008682:	fa03 f309 	lsl.w	r3, r3, r9
 8008686:	f012 0901 	ands.w	r9, r2, #1
 800868a:	b21b      	sxth	r3, r3
 800868c:	d003      	beq.n	8008696 <main+0x27e>
 800868e:	ea20 0303 	bic.w	r3, r0, r3
 8008692:	854b      	strh	r3, [r1, #42]	; 0x2a
 8008694:	e71c      	b.n	80084d0 <main+0xb8>
 8008696:	4303      	orrs	r3, r0
 8008698:	854b      	strh	r3, [r1, #42]	; 0x2a
 800869a:	e758      	b.n	800854e <main+0x136>
 800869c:	2b02      	cmp	r3, #2
 800869e:	f47f af17 	bne.w	80084d0 <main+0xb8>
 80086a2:	4922      	ldr	r1, [pc, #136]	; (800872c <main+0x314>)
 80086a4:	8d8b      	ldrh	r3, [r1, #44]	; 0x2c
 80086a6:	f013 09ff 	ands.w	r9, r3, #255	; 0xff
 80086aa:	f43f af50 	beq.w	800854e <main+0x136>
 80086ae:	f1b9 0f05 	cmp.w	r9, #5
 80086b2:	f43f af0d 	beq.w	80084d0 <main+0xb8>
 80086b6:	2201      	movs	r2, #1
 80086b8:	f109 0907 	add.w	r9, r9, #7
 80086bc:	b218      	sxth	r0, r3
 80086be:	fa43 f309 	asr.w	r3, r3, r9
 80086c2:	fa02 f209 	lsl.w	r2, r2, r9
 80086c6:	f013 0901 	ands.w	r9, r3, #1
 80086ca:	b212      	sxth	r2, r2
 80086cc:	d003      	beq.n	80086d6 <main+0x2be>
 80086ce:	ea20 0202 	bic.w	r2, r0, r2
 80086d2:	858a      	strh	r2, [r1, #44]	; 0x2c
 80086d4:	e6fc      	b.n	80084d0 <main+0xb8>
 80086d6:	4302      	orrs	r2, r0
 80086d8:	858a      	strh	r2, [r1, #44]	; 0x2c
 80086da:	e738      	b.n	800854e <main+0x136>
 80086dc:	2b04      	cmp	r3, #4
 80086de:	d105      	bne.n	80086ec <main+0x2d4>
 80086e0:	4b12      	ldr	r3, [pc, #72]	; (800872c <main+0x314>)
 80086e2:	f893 0050 	ldrb.w	r0, [r3, #80]	; 0x50
 80086e6:	f004 fa8d 	bl	800cc04 <PIO_SendData>
 80086ea:	e6f1      	b.n	80084d0 <main+0xb8>
 80086ec:	2b05      	cmp	r3, #5
 80086ee:	d13c      	bne.n	800876a <main+0x352>
 80086f0:	796b      	ldrb	r3, [r5, #5]
 80086f2:	bb6b      	cbnz	r3, 8008750 <main+0x338>
 80086f4:	f240 102b 	movw	r0, #299	; 0x12b
 80086f8:	21e1      	movs	r1, #225	; 0xe1
 80086fa:	f006 fad9 	bl	800ecb0 <Clear_File_ICO>
 80086fe:	4b0b      	ldr	r3, [pc, #44]	; (800872c <main+0x314>)
 8008700:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
 8008704:	f005 fffe 	bl	800e704 <Save_Bmp>
 8008708:	4607      	mov	r7, r0
 800870a:	4638      	mov	r0, r7
 800870c:	f006 fae0 	bl	800ecd0 <DispFileInfo>
 8008710:	2201      	movs	r2, #1
 8008712:	4b0d      	ldr	r3, [pc, #52]	; (8008748 <main+0x330>)
 8008714:	701a      	strb	r2, [r3, #0]
 8008716:	f001 fc21 	bl	8009f5c <ShowBattery>
 800871a:	e6d9      	b.n	80084d0 <main+0xb8>
 800871c:	20004c12 	.word	0x20004c12
 8008720:	20000244 	.word	0x20000244
 8008724:	20004c0f 	.word	0x20004c0f
 8008728:	200009d9 	.word	0x200009d9
 800872c:	2000000c 	.word	0x2000000c
 8008730:	0801360a 	.word	0x0801360a
 8008734:	fffff0f0 	.word	0xfffff0f0
 8008738:	20004c0c 	.word	0x20004c0c
 800873c:	20000af3 	.word	0x20000af3
 8008740:	200009da 	.word	0x200009da
 8008744:	20004c39 	.word	0x20004c39
 8008748:	200000d8 	.word	0x200000d8
 800874c:	200009b6 	.word	0x200009b6
 8008750:	2b01      	cmp	r3, #1
 8008752:	d1da      	bne.n	800870a <main+0x2f2>
 8008754:	21e1      	movs	r1, #225	; 0xe1
 8008756:	f240 102b 	movw	r0, #299	; 0x12b
 800875a:	f006 faa9 	bl	800ecb0 <Clear_File_ICO>
 800875e:	4bb2      	ldr	r3, [pc, #712]	; (8008a28 <main+0x610>)
 8008760:	f9b3 0066 	ldrsh.w	r0, [r3, #102]	; 0x66
 8008764:	f006 f884 	bl	800e870 <Save_Csv>
 8008768:	e7ce      	b.n	8008708 <main+0x2f0>
 800876a:	2b06      	cmp	r3, #6
 800876c:	f47f aeb0 	bne.w	80084d0 <main+0xb8>
 8008770:	f895 9006 	ldrb.w	r9, [r5, #6]
 8008774:	f1b9 0f00 	cmp.w	r9, #0
 8008778:	d10f      	bne.n	800879a <main+0x382>
 800877a:	f006 fad3 	bl	800ed24 <SaveParameter>
 800877e:	4607      	mov	r7, r0
 8008780:	79ab      	ldrb	r3, [r5, #6]
 8008782:	2b01      	cmp	r3, #1
 8008784:	f63f aee3 	bhi.w	800854e <main+0x136>
 8008788:	4638      	mov	r0, r7
 800878a:	f006 faa1 	bl	800ecd0 <DispFileInfo>
 800878e:	2201      	movs	r2, #1
 8008790:	4ba6      	ldr	r3, [pc, #664]	; (8008a2c <main+0x614>)
 8008792:	701a      	strb	r2, [r3, #0]
 8008794:	f001 fbe2 	bl	8009f5c <ShowBattery>
 8008798:	e6d9      	b.n	800854e <main+0x136>
 800879a:	f1b9 0f01 	cmp.w	r9, #1
 800879e:	d10a      	bne.n	80087b6 <main+0x39e>
 80087a0:	f006 fbf6 	bl	800ef90 <RestoreParameter>
 80087a4:	f006 fabe 	bl	800ed24 <SaveParameter>
 80087a8:	4ba0      	ldr	r3, [pc, #640]	; (8008a2c <main+0x614>)
 80087aa:	4607      	mov	r7, r0
 80087ac:	f883 9000 	strb.w	r9, [r3]
 80087b0:	f002 f8d2 	bl	800a958 <ShowAllMenu>
 80087b4:	e7e4      	b.n	8008780 <main+0x368>
 80087b6:	f04f 0900 	mov.w	r9, #0
 80087ba:	e7e1      	b.n	8008780 <main+0x368>
 80087bc:	f894 9000 	ldrb.w	r9, [r4]
 80087c0:	f1b9 0f00 	cmp.w	r9, #0
 80087c4:	f43f aec3 	beq.w	800854e <main+0x136>
 80087c8:	4999      	ldr	r1, [pc, #612]	; (8008a30 <main+0x618>)
 80087ca:	786a      	ldrb	r2, [r5, #1]
 80087cc:	7849      	ldrb	r1, [r1, #1]
 80087ce:	4291      	cmp	r1, r2
 80087d0:	d902      	bls.n	80087d8 <main+0x3c0>
 80087d2:	3201      	adds	r2, #1
 80087d4:	706a      	strb	r2, [r5, #1]
 80087d6:	e738      	b.n	800864a <main+0x232>
 80087d8:	706b      	strb	r3, [r5, #1]
 80087da:	e679      	b.n	80084d0 <main+0xb8>
 80087dc:	7823      	ldrb	r3, [r4, #0]
 80087de:	2b00      	cmp	r3, #0
 80087e0:	d03c      	beq.n	800885c <main+0x444>
 80087e2:	f895 9001 	ldrb.w	r9, [r5, #1]
 80087e6:	4b90      	ldr	r3, [pc, #576]	; (8008a28 <main+0x610>)
 80087e8:	f1b9 0f01 	cmp.w	r9, #1
 80087ec:	d111      	bne.n	8008812 <main+0x3fa>
 80087ee:	4891      	ldr	r0, [pc, #580]	; (8008a34 <main+0x61c>)
 80087f0:	8ada      	ldrh	r2, [r3, #22]
 80087f2:	8ac0      	ldrh	r0, [r0, #22]
 80087f4:	8b19      	ldrh	r1, [r3, #24]
 80087f6:	4402      	add	r2, r0
 80087f8:	428a      	cmp	r2, r1
 80087fa:	bfb4      	ite	lt
 80087fc:	82da      	strhlt	r2, [r3, #22]
 80087fe:	82d9      	strhge	r1, [r3, #22]
 8008800:	f240 1219 	movw	r2, #281	; 0x119
 8008804:	498c      	ldr	r1, [pc, #560]	; (8008a38 <main+0x620>)
 8008806:	488d      	ldr	r0, [pc, #564]	; (8008a3c <main+0x624>)
 8008808:	f00a f98c 	bl	8012b24 <memcpy>
 800880c:	f003 fde2 	bl	800c3d4 <ShowWaveToLCD>
 8008810:	e65e      	b.n	80084d0 <main+0xb8>
 8008812:	488b      	ldr	r0, [pc, #556]	; (8008a40 <main+0x628>)
 8008814:	f109 020a 	add.w	r2, r9, #10
 8008818:	f830 0012 	ldrh.w	r0, [r0, r2, lsl #1]
 800881c:	f833 1012 	ldrh.w	r1, [r3, r2, lsl #1]
 8008820:	4288      	cmp	r0, r1
 8008822:	bf81      	itttt	hi
 8008824:	4883      	ldrhi	r0, [pc, #524]	; (8008a34 <main+0x61c>)
 8008826:	f830 0012 	ldrhhi.w	r0, [r0, r2, lsl #1]
 800882a:	1809      	addhi	r1, r1, r0
 800882c:	f823 1012 	strhhi.w	r1, [r3, r2, lsl #1]
 8008830:	f1b9 0f00 	cmp.w	r9, #0
 8008834:	d102      	bne.n	800883c <main+0x424>
 8008836:	f003 fec5 	bl	800c5c4 <ZC_Scale>
 800883a:	e688      	b.n	800854e <main+0x136>
 800883c:	f1b9 0f05 	cmp.w	r9, #5
 8008840:	d103      	bne.n	800884a <main+0x432>
 8008842:	8a9a      	ldrh	r2, [r3, #20]
 8008844:	4b7f      	ldr	r3, [pc, #508]	; (8008a44 <main+0x62c>)
 8008846:	801a      	strh	r2, [r3, #0]
 8008848:	e7da      	b.n	8008800 <main+0x3e8>
 800884a:	f1b9 0f03 	cmp.w	r9, #3
 800884e:	d1d7      	bne.n	8008800 <main+0x3e8>
 8008850:	4b7d      	ldr	r3, [pc, #500]	; (8008a48 <main+0x630>)
 8008852:	2201      	movs	r2, #1
 8008854:	701a      	strb	r2, [r3, #0]
 8008856:	f003 feb5 	bl	800c5c4 <ZC_Scale>
 800885a:	e639      	b.n	80084d0 <main+0xb8>
 800885c:	4a7b      	ldr	r2, [pc, #492]	; (8008a4c <main+0x634>)
 800885e:	f892 9000 	ldrb.w	r9, [r2]
 8008862:	f1b9 0f00 	cmp.w	r9, #0
 8008866:	d00b      	beq.n	8008880 <main+0x468>
 8008868:	78a2      	ldrb	r2, [r4, #2]
 800886a:	f815 9002 	ldrb.w	r9, [r5, r2]
 800886e:	f1b9 0f00 	cmp.w	r9, #0
 8008872:	f43f ae6c 	beq.w	800854e <main+0x136>
 8008876:	f109 39ff 	add.w	r9, r9, #4294967295	; 0xffffffff
 800887a:	f805 9002 	strb.w	r9, [r5, r2]
 800887e:	e627      	b.n	80084d0 <main+0xb8>
 8008880:	4b73      	ldr	r3, [pc, #460]	; (8008a50 <main+0x638>)
 8008882:	781b      	ldrb	r3, [r3, #0]
 8008884:	2b00      	cmp	r3, #0
 8008886:	f43f ae62 	beq.w	800854e <main+0x136>
 800888a:	78a3      	ldrb	r3, [r4, #2]
 800888c:	b143      	cbz	r3, 80088a0 <main+0x488>
 800888e:	2b04      	cmp	r3, #4
 8008890:	f47f ae5d 	bne.w	800854e <main+0x136>
 8008894:	4a64      	ldr	r2, [pc, #400]	; (8008a28 <main+0x610>)
 8008896:	792b      	ldrb	r3, [r5, #4]
 8008898:	f8b2 1050 	ldrh.w	r1, [r2, #80]	; 0x50
 800889c:	4a6d      	ldr	r2, [pc, #436]	; (8008a54 <main+0x63c>)
 800889e:	e003      	b.n	80088a8 <main+0x490>
 80088a0:	4a61      	ldr	r2, [pc, #388]	; (8008a28 <main+0x610>)
 80088a2:	782b      	ldrb	r3, [r5, #0]
 80088a4:	8811      	ldrh	r1, [r2, #0]
 80088a6:	4a6c      	ldr	r2, [pc, #432]	; (8008a58 <main+0x640>)
 80088a8:	eb02 0283 	add.w	r2, r2, r3, lsl #2
 80088ac:	5c53      	ldrb	r3, [r2, r1]
 80088ae:	2b00      	cmp	r3, #0
 80088b0:	f43f ae0e 	beq.w	80084d0 <main+0xb8>
 80088b4:	3b01      	subs	r3, #1
 80088b6:	5453      	strb	r3, [r2, r1]
 80088b8:	e649      	b.n	800854e <main+0x136>
 80088ba:	4868      	ldr	r0, [pc, #416]	; (8008a5c <main+0x644>)
 80088bc:	f109 020a 	add.w	r2, r9, #10
 80088c0:	f830 0012 	ldrh.w	r0, [r0, r2, lsl #1]
 80088c4:	f833 1012 	ldrh.w	r1, [r3, r2, lsl #1]
 80088c8:	4288      	cmp	r0, r1
 80088ca:	bf3f      	itttt	cc
 80088cc:	4859      	ldrcc	r0, [pc, #356]	; (8008a34 <main+0x61c>)
 80088ce:	f830 0012 	ldrhcc.w	r0, [r0, r2, lsl #1]
 80088d2:	1a09      	subcc	r1, r1, r0
 80088d4:	f823 1012 	strhcc.w	r1, [r3, r2, lsl #1]
 80088d8:	f1b9 0f00 	cmp.w	r9, #0
 80088dc:	d0ab      	beq.n	8008836 <main+0x41e>
 80088de:	f1b9 0f05 	cmp.w	r9, #5
 80088e2:	d0ae      	beq.n	8008842 <main+0x42a>
 80088e4:	f1b9 0f03 	cmp.w	r9, #3
 80088e8:	d18a      	bne.n	8008800 <main+0x3e8>
 80088ea:	4b5d      	ldr	r3, [pc, #372]	; (8008a60 <main+0x648>)
 80088ec:	e7b1      	b.n	8008852 <main+0x43a>
 80088ee:	4b57      	ldr	r3, [pc, #348]	; (8008a4c <main+0x634>)
 80088f0:	f893 9000 	ldrb.w	r9, [r3]
 80088f4:	f1b9 0f00 	cmp.w	r9, #0
 80088f8:	d009      	beq.n	800890e <main+0x4f6>
 80088fa:	78a2      	ldrb	r2, [r4, #2]
 80088fc:	494c      	ldr	r1, [pc, #304]	; (8008a30 <main+0x618>)
 80088fe:	5cab      	ldrb	r3, [r5, r2]
 8008900:	5c89      	ldrb	r1, [r1, r2]
 8008902:	4299      	cmp	r1, r3
 8008904:	f67f ade4 	bls.w	80084d0 <main+0xb8>
 8008908:	3301      	adds	r3, #1
 800890a:	54ab      	strb	r3, [r5, r2]
 800890c:	e5e0      	b.n	80084d0 <main+0xb8>
 800890e:	4b50      	ldr	r3, [pc, #320]	; (8008a50 <main+0x638>)
 8008910:	781b      	ldrb	r3, [r3, #0]
 8008912:	2b00      	cmp	r3, #0
 8008914:	f43f addc 	beq.w	80084d0 <main+0xb8>
 8008918:	78a3      	ldrb	r3, [r4, #2]
 800891a:	b163      	cbz	r3, 8008936 <main+0x51e>
 800891c:	2b04      	cmp	r3, #4
 800891e:	f47f ae16 	bne.w	800854e <main+0x136>
 8008922:	4b41      	ldr	r3, [pc, #260]	; (8008a28 <main+0x610>)
 8008924:	7929      	ldrb	r1, [r5, #4]
 8008926:	f8b3 0050 	ldrh.w	r0, [r3, #80]	; 0x50
 800892a:	4b4a      	ldr	r3, [pc, #296]	; (8008a54 <main+0x63c>)
 800892c:	0089      	lsls	r1, r1, #2
 800892e:	440b      	add	r3, r1
 8008930:	5c1a      	ldrb	r2, [r3, r0]
 8008932:	4e4c      	ldr	r6, [pc, #304]	; (8008a64 <main+0x64c>)
 8008934:	e007      	b.n	8008946 <main+0x52e>
 8008936:	4b3c      	ldr	r3, [pc, #240]	; (8008a28 <main+0x610>)
 8008938:	7829      	ldrb	r1, [r5, #0]
 800893a:	8818      	ldrh	r0, [r3, #0]
 800893c:	4b46      	ldr	r3, [pc, #280]	; (8008a58 <main+0x640>)
 800893e:	0089      	lsls	r1, r1, #2
 8008940:	440b      	add	r3, r1
 8008942:	5c1a      	ldrb	r2, [r3, r0]
 8008944:	4e48      	ldr	r6, [pc, #288]	; (8008a68 <main+0x650>)
 8008946:	4431      	add	r1, r6
 8008948:	5c09      	ldrb	r1, [r1, r0]
 800894a:	4291      	cmp	r1, r2
 800894c:	f67f adff 	bls.w	800854e <main+0x136>
 8008950:	3201      	adds	r2, #1
 8008952:	541a      	strb	r2, [r3, r0]
 8008954:	e5fb      	b.n	800854e <main+0x136>
 8008956:	7823      	ldrb	r3, [r4, #0]
 8008958:	b153      	cbz	r3, 8008970 <main+0x558>
 800895a:	f894 9002 	ldrb.w	r9, [r4, #2]
 800895e:	f1b9 0f00 	cmp.w	r9, #0
 8008962:	f43f adf4 	beq.w	800854e <main+0x136>
 8008966:	f109 39ff 	add.w	r9, r9, #4294967295	; 0xffffffff
 800896a:	f884 9002 	strb.w	r9, [r4, #2]
 800896e:	e5af      	b.n	80084d0 <main+0xb8>
 8008970:	4b36      	ldr	r3, [pc, #216]	; (8008a4c <main+0x634>)
 8008972:	f893 9000 	ldrb.w	r9, [r3]
 8008976:	f1b9 0f00 	cmp.w	r9, #0
 800897a:	f000 80cf 	beq.w	8008b1c <main+0x704>
 800897e:	78a3      	ldrb	r3, [r4, #2]
 8008980:	4e29      	ldr	r6, [pc, #164]	; (8008a28 <main+0x610>)
 8008982:	2b01      	cmp	r3, #1
 8008984:	f040 80a0 	bne.w	8008ac8 <main+0x6b0>
 8008988:	786a      	ldrb	r2, [r5, #1]
 800898a:	2a03      	cmp	r2, #3
 800898c:	d10b      	bne.n	80089a6 <main+0x58e>
 800898e:	4a34      	ldr	r2, [pc, #208]	; (8008a60 <main+0x648>)
 8008990:	7013      	strb	r3, [r2, #0]
 8008992:	f003 fdcf 	bl	800c534 <UpdateXposiTime>
 8008996:	7d33      	ldrb	r3, [r6, #20]
 8008998:	f44f 728c 	mov.w	r2, #280	; 0x118
 800899c:	2137      	movs	r1, #55	; 0x37
 800899e:	2029      	movs	r0, #41	; 0x29
 80089a0:	f001 fd08 	bl	800a3b4 <ShowScaleRuleTime>
 80089a4:	e019      	b.n	80089da <main+0x5c2>
 80089a6:	2a01      	cmp	r2, #1
 80089a8:	f8df 9088 	ldr.w	r9, [pc, #136]	; 8008a34 <main+0x61c>
 80089ac:	d160      	bne.n	8008a70 <main+0x658>
 80089ae:	482b      	ldr	r0, [pc, #172]	; (8008a5c <main+0x644>)
 80089b0:	f04f 0b0a 	mov.w	fp, #10
 80089b4:	4682      	mov	sl, r0
 80089b6:	8af2      	ldrh	r2, [r6, #22]
 80089b8:	f8b9 3016 	ldrh.w	r3, [r9, #22]
 80089bc:	8ac1      	ldrh	r1, [r0, #22]
 80089be:	1ad3      	subs	r3, r2, r3
 80089c0:	428b      	cmp	r3, r1
 80089c2:	bfac      	ite	ge
 80089c4:	82f3      	strhge	r3, [r6, #22]
 80089c6:	82f1      	strhlt	r1, [r6, #22]
 80089c8:	4928      	ldr	r1, [pc, #160]	; (8008a6c <main+0x654>)
 80089ca:	2006      	movs	r0, #6
 80089cc:	f7f7 fb98 	bl	8000100 <__Bios>
 80089d0:	43c0      	mvns	r0, r0
 80089d2:	f000 000f 	and.w	r0, r0, #15
 80089d6:	2808      	cmp	r0, #8
 80089d8:	d010      	beq.n	80089fc <main+0x5e4>
 80089da:	78a3      	ldrb	r3, [r4, #2]
 80089dc:	2b04      	cmp	r3, #4
 80089de:	d103      	bne.n	80089e8 <main+0x5d0>
 80089e0:	f896 0050 	ldrb.w	r0, [r6, #80]	; 0x50
 80089e4:	f004 fb58 	bl	800d098 <PIO_Init>
 80089e8:	f894 9002 	ldrb.w	r9, [r4, #2]
 80089ec:	f1b9 0f00 	cmp.w	r9, #0
 80089f0:	f47f ad6e 	bne.w	80084d0 <main+0xb8>
 80089f4:	7830      	ldrb	r0, [r6, #0]
 80089f6:	f003 fcd1 	bl	800c39c <DefaultTir>
 80089fa:	e5a8      	b.n	800854e <main+0x136>
 80089fc:	78a0      	ldrb	r0, [r4, #2]
 80089fe:	5c2b      	ldrb	r3, [r5, r0]
 8008a00:	fb0b 3300 	mla	r3, fp, r0, r3
 8008a04:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008a08:	f839 c013 	ldrh.w	ip, [r9, r3, lsl #1]
 8008a0c:	f83a 1013 	ldrh.w	r1, [sl, r3, lsl #1]
 8008a10:	eba2 020c 	sub.w	r2, r2, ip
 8008a14:	428a      	cmp	r2, r1
 8008a16:	bfac      	ite	ge
 8008a18:	f826 2013 	strhge.w	r2, [r6, r3, lsl #1]
 8008a1c:	f826 1013 	strhlt.w	r1, [r6, r3, lsl #1]
 8008a20:	f002 feb8 	bl	800b794 <ShowPopItem>
 8008a24:	e7d0      	b.n	80089c8 <main+0x5b0>
 8008a26:	bf00      	nop
 8008a28:	2000000c 	.word	0x2000000c
 8008a2c:	200000d8 	.word	0x200000d8
 8008a30:	080133a8 	.word	0x080133a8
 8008a34:	0801360a 	.word	0x0801360a
 8008a38:	20000af3 	.word	0x20000af3
 8008a3c:	200009da 	.word	0x200009da
 8008a40:	08012d18 	.word	0x08012d18
 8008a44:	200009ce 	.word	0x200009ce
 8008a48:	20004c20 	.word	0x20004c20
 8008a4c:	20004c0f 	.word	0x20004c0f
 8008a50:	20004c0c 	.word	0x20004c0c
 8008a54:	2000024b 	.word	0x2000024b
 8008a58:	20000240 	.word	0x20000240
 8008a5c:	08012c8c 	.word	0x08012c8c
 8008a60:	20004c38 	.word	0x20004c38
 8008a64:	08012e74 	.word	0x08012e74
 8008a68:	08012e70 	.word	0x08012e70
 8008a6c:	fffc0000 	.word	0xfffc0000
 8008a70:	2a02      	cmp	r2, #2
 8008a72:	d141      	bne.n	8008af8 <main+0x6e0>
 8008a74:	f04f 0b0a 	mov.w	fp, #10
 8008a78:	8b32      	ldrh	r2, [r6, #24]
 8008a7a:	f8b9 3018 	ldrh.w	r3, [r9, #24]
 8008a7e:	8af1      	ldrh	r1, [r6, #22]
 8008a80:	1ad3      	subs	r3, r2, r3
 8008a82:	428b      	cmp	r3, r1
 8008a84:	f8df a2d0 	ldr.w	sl, [pc, #720]	; 8008d58 <main+0x940>
 8008a88:	bfac      	ite	ge
 8008a8a:	8333      	strhge	r3, [r6, #24]
 8008a8c:	8331      	strhlt	r1, [r6, #24]
 8008a8e:	4651      	mov	r1, sl
 8008a90:	2006      	movs	r0, #6
 8008a92:	f7f7 fb35 	bl	8000100 <__Bios>
 8008a96:	43c0      	mvns	r0, r0
 8008a98:	f000 000f 	and.w	r0, r0, #15
 8008a9c:	2808      	cmp	r0, #8
 8008a9e:	d19c      	bne.n	80089da <main+0x5c2>
 8008aa0:	78a0      	ldrb	r0, [r4, #2]
 8008aa2:	8af1      	ldrh	r1, [r6, #22]
 8008aa4:	5c2b      	ldrb	r3, [r5, r0]
 8008aa6:	fb0b 3300 	mla	r3, fp, r0, r3
 8008aaa:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008aae:	f839 c013 	ldrh.w	ip, [r9, r3, lsl #1]
 8008ab2:	eba2 020c 	sub.w	r2, r2, ip
 8008ab6:	428a      	cmp	r2, r1
 8008ab8:	bfac      	ite	ge
 8008aba:	f826 2013 	strhge.w	r2, [r6, r3, lsl #1]
 8008abe:	f826 1013 	strhlt.w	r1, [r6, r3, lsl #1]
 8008ac2:	f002 fe67 	bl	800b794 <ShowPopItem>
 8008ac6:	e7e2      	b.n	8008a8e <main+0x676>
 8008ac8:	2b02      	cmp	r3, #2
 8008aca:	d115      	bne.n	8008af8 <main+0x6e0>
 8008acc:	78aa      	ldrb	r2, [r5, #2]
 8008ace:	1e51      	subs	r1, r2, #1
 8008ad0:	2901      	cmp	r1, #1
 8008ad2:	d811      	bhi.n	8008af8 <main+0x6e0>
 8008ad4:	f102 0314 	add.w	r3, r2, #20
 8008ad8:	4992      	ldr	r1, [pc, #584]	; (8008d24 <main+0x90c>)
 8008ada:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008ade:	f831 0013 	ldrh.w	r0, [r1, r3, lsl #1]
 8008ae2:	b2d1      	uxtb	r1, r2
 8008ae4:	4288      	cmp	r0, r1
 8008ae6:	f4bf af7f 	bcs.w	80089e8 <main+0x5d0>
 8008aea:	498f      	ldr	r1, [pc, #572]	; (8008d28 <main+0x910>)
 8008aec:	f831 1013 	ldrh.w	r1, [r1, r3, lsl #1]
 8008af0:	1a52      	subs	r2, r2, r1
 8008af2:	f826 2013 	strh.w	r2, [r6, r3, lsl #1]
 8008af6:	e770      	b.n	80089da <main+0x5c2>
 8008af8:	210a      	movs	r1, #10
 8008afa:	5cea      	ldrb	r2, [r5, r3]
 8008afc:	fb01 2303 	mla	r3, r1, r3, r2
 8008b00:	4989      	ldr	r1, [pc, #548]	; (8008d28 <main+0x910>)
 8008b02:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008b06:	f831 0013 	ldrh.w	r0, [r1, r3, lsl #1]
 8008b0a:	4986      	ldr	r1, [pc, #536]	; (8008d24 <main+0x90c>)
 8008b0c:	1a12      	subs	r2, r2, r0
 8008b0e:	f831 1013 	ldrh.w	r1, [r1, r3, lsl #1]
 8008b12:	428a      	cmp	r2, r1
 8008b14:	daed      	bge.n	8008af2 <main+0x6da>
 8008b16:	f826 1013 	strh.w	r1, [r6, r3, lsl #1]
 8008b1a:	e75e      	b.n	80089da <main+0x5c2>
 8008b1c:	4b83      	ldr	r3, [pc, #524]	; (8008d2c <main+0x914>)
 8008b1e:	781b      	ldrb	r3, [r3, #0]
 8008b20:	2b00      	cmp	r3, #0
 8008b22:	f43f ad14 	beq.w	800854e <main+0x136>
 8008b26:	78a3      	ldrb	r3, [r4, #2]
 8008b28:	b9b3      	cbnz	r3, 8008b58 <main+0x740>
 8008b2a:	f895 9000 	ldrb.w	r9, [r5]
 8008b2e:	f1b9 0f00 	cmp.w	r9, #0
 8008b32:	f47f accd 	bne.w	80084d0 <main+0xb8>
 8008b36:	4b7e      	ldr	r3, [pc, #504]	; (8008d30 <main+0x918>)
 8008b38:	881a      	ldrh	r2, [r3, #0]
 8008b3a:	4b7e      	ldr	r3, [pc, #504]	; (8008d34 <main+0x91c>)
 8008b3c:	5c98      	ldrb	r0, [r3, r2]
 8008b3e:	4b7e      	ldr	r3, [pc, #504]	; (8008d38 <main+0x920>)
 8008b40:	00d2      	lsls	r2, r2, #3
 8008b42:	1899      	adds	r1, r3, r2
 8008b44:	5c0e      	ldrb	r6, [r1, r0]
 8008b46:	497d      	ldr	r1, [pc, #500]	; (8008d3c <main+0x924>)
 8008b48:	4411      	add	r1, r2
 8008b4a:	5c09      	ldrb	r1, [r1, r0]
 8008b4c:	42b1      	cmp	r1, r6
 8008b4e:	f4bf acfe 	bcs.w	800854e <main+0x136>
 8008b52:	4413      	add	r3, r2
 8008b54:	1e72      	subs	r2, r6, #1
 8008b56:	e6fc      	b.n	8008952 <main+0x53a>
 8008b58:	2b04      	cmp	r3, #4
 8008b5a:	f47f acf8 	bne.w	800854e <main+0x136>
 8008b5e:	4b74      	ldr	r3, [pc, #464]	; (8008d30 <main+0x918>)
 8008b60:	4977      	ldr	r1, [pc, #476]	; (8008d40 <main+0x928>)
 8008b62:	f8b3 2050 	ldrh.w	r2, [r3, #80]	; 0x50
 8008b66:	792b      	ldrb	r3, [r5, #4]
 8008b68:	eb01 0183 	add.w	r1, r1, r3, lsl #2
 8008b6c:	4b75      	ldr	r3, [pc, #468]	; (8008d44 <main+0x92c>)
 8008b6e:	5c89      	ldrb	r1, [r1, r2]
 8008b70:	00d2      	lsls	r2, r2, #3
 8008b72:	1898      	adds	r0, r3, r2
 8008b74:	5c46      	ldrb	r6, [r0, r1]
 8008b76:	4874      	ldr	r0, [pc, #464]	; (8008d48 <main+0x930>)
 8008b78:	4410      	add	r0, r2
 8008b7a:	5c40      	ldrb	r0, [r0, r1]
 8008b7c:	42b0      	cmp	r0, r6
 8008b7e:	f4bf ace6 	bcs.w	800854e <main+0x136>
 8008b82:	4413      	add	r3, r2
 8008b84:	1e72      	subs	r2, r6, #1
 8008b86:	545a      	strb	r2, [r3, r1]
 8008b88:	e4e1      	b.n	800854e <main+0x136>
 8008b8a:	7823      	ldrb	r3, [r4, #0]
 8008b8c:	b13b      	cbz	r3, 8008b9e <main+0x786>
 8008b8e:	78a3      	ldrb	r3, [r4, #2]
 8008b90:	7862      	ldrb	r2, [r4, #1]
 8008b92:	429a      	cmp	r2, r3
 8008b94:	f67f ac9c 	bls.w	80084d0 <main+0xb8>
 8008b98:	3301      	adds	r3, #1
 8008b9a:	70a3      	strb	r3, [r4, #2]
 8008b9c:	e498      	b.n	80084d0 <main+0xb8>
 8008b9e:	4b6b      	ldr	r3, [pc, #428]	; (8008d4c <main+0x934>)
 8008ba0:	f893 9000 	ldrb.w	r9, [r3]
 8008ba4:	f1b9 0f00 	cmp.w	r9, #0
 8008ba8:	f000 808d 	beq.w	8008cc6 <main+0x8ae>
 8008bac:	78a3      	ldrb	r3, [r4, #2]
 8008bae:	4e60      	ldr	r6, [pc, #384]	; (8008d30 <main+0x918>)
 8008bb0:	2b01      	cmp	r3, #1
 8008bb2:	d161      	bne.n	8008c78 <main+0x860>
 8008bb4:	786a      	ldrb	r2, [r5, #1]
 8008bb6:	2a03      	cmp	r2, #3
 8008bb8:	d103      	bne.n	8008bc2 <main+0x7aa>
 8008bba:	2201      	movs	r2, #1
 8008bbc:	4b64      	ldr	r3, [pc, #400]	; (8008d50 <main+0x938>)
 8008bbe:	701a      	strb	r2, [r3, #0]
 8008bc0:	e6e7      	b.n	8008992 <main+0x57a>
 8008bc2:	2a01      	cmp	r2, #1
 8008bc4:	f8df 9160 	ldr.w	r9, [pc, #352]	; 8008d28 <main+0x910>
 8008bc8:	d129      	bne.n	8008c1e <main+0x806>
 8008bca:	f04f 0b0a 	mov.w	fp, #10
 8008bce:	8af2      	ldrh	r2, [r6, #22]
 8008bd0:	f8b9 3016 	ldrh.w	r3, [r9, #22]
 8008bd4:	8b31      	ldrh	r1, [r6, #24]
 8008bd6:	4413      	add	r3, r2
 8008bd8:	428b      	cmp	r3, r1
 8008bda:	f8df a17c 	ldr.w	sl, [pc, #380]	; 8008d58 <main+0x940>
 8008bde:	bfb4      	ite	lt
 8008be0:	82f3      	strhlt	r3, [r6, #22]
 8008be2:	82f1      	strhge	r1, [r6, #22]
 8008be4:	4651      	mov	r1, sl
 8008be6:	2006      	movs	r0, #6
 8008be8:	f7f7 fa8a 	bl	8000100 <__Bios>
 8008bec:	43c0      	mvns	r0, r0
 8008bee:	f000 000f 	and.w	r0, r0, #15
 8008bf2:	2808      	cmp	r0, #8
 8008bf4:	f47f aef1 	bne.w	80089da <main+0x5c2>
 8008bf8:	78a0      	ldrb	r0, [r4, #2]
 8008bfa:	8b31      	ldrh	r1, [r6, #24]
 8008bfc:	5c2b      	ldrb	r3, [r5, r0]
 8008bfe:	fb0b 3300 	mla	r3, fp, r0, r3
 8008c02:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008c06:	f839 c013 	ldrh.w	ip, [r9, r3, lsl #1]
 8008c0a:	4462      	add	r2, ip
 8008c0c:	428a      	cmp	r2, r1
 8008c0e:	bfb4      	ite	lt
 8008c10:	f826 2013 	strhlt.w	r2, [r6, r3, lsl #1]
 8008c14:	f826 1013 	strhge.w	r1, [r6, r3, lsl #1]
 8008c18:	f002 fdbc 	bl	800b794 <ShowPopItem>
 8008c1c:	e7e2      	b.n	8008be4 <main+0x7cc>
 8008c1e:	2a02      	cmp	r2, #2
 8008c20:	d140      	bne.n	8008ca4 <main+0x88c>
 8008c22:	484c      	ldr	r0, [pc, #304]	; (8008d54 <main+0x93c>)
 8008c24:	f04f 0b0a 	mov.w	fp, #10
 8008c28:	4682      	mov	sl, r0
 8008c2a:	8b32      	ldrh	r2, [r6, #24]
 8008c2c:	f8b9 3018 	ldrh.w	r3, [r9, #24]
 8008c30:	8b01      	ldrh	r1, [r0, #24]
 8008c32:	4413      	add	r3, r2
 8008c34:	428b      	cmp	r3, r1
 8008c36:	bfb4      	ite	lt
 8008c38:	8333      	strhlt	r3, [r6, #24]
 8008c3a:	8331      	strhge	r1, [r6, #24]
 8008c3c:	4946      	ldr	r1, [pc, #280]	; (8008d58 <main+0x940>)
 8008c3e:	2006      	movs	r0, #6
 8008c40:	f7f7 fa5e 	bl	8000100 <__Bios>
 8008c44:	43c0      	mvns	r0, r0
 8008c46:	f000 000f 	and.w	r0, r0, #15
 8008c4a:	2808      	cmp	r0, #8
 8008c4c:	f47f aec5 	bne.w	80089da <main+0x5c2>
 8008c50:	78a0      	ldrb	r0, [r4, #2]
 8008c52:	5c2b      	ldrb	r3, [r5, r0]
 8008c54:	fb0b 3300 	mla	r3, fp, r0, r3
 8008c58:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008c5c:	f839 c013 	ldrh.w	ip, [r9, r3, lsl #1]
 8008c60:	f83a 1013 	ldrh.w	r1, [sl, r3, lsl #1]
 8008c64:	4462      	add	r2, ip
 8008c66:	428a      	cmp	r2, r1
 8008c68:	bfb4      	ite	lt
 8008c6a:	f826 2013 	strhlt.w	r2, [r6, r3, lsl #1]
 8008c6e:	f826 1013 	strhge.w	r1, [r6, r3, lsl #1]
 8008c72:	f002 fd8f 	bl	800b794 <ShowPopItem>
 8008c76:	e7e1      	b.n	8008c3c <main+0x824>
 8008c78:	2b02      	cmp	r3, #2
 8008c7a:	d113      	bne.n	8008ca4 <main+0x88c>
 8008c7c:	78aa      	ldrb	r2, [r5, #2]
 8008c7e:	1e51      	subs	r1, r2, #1
 8008c80:	2901      	cmp	r1, #1
 8008c82:	d80f      	bhi.n	8008ca4 <main+0x88c>
 8008c84:	f102 0314 	add.w	r3, r2, #20
 8008c88:	4932      	ldr	r1, [pc, #200]	; (8008d54 <main+0x93c>)
 8008c8a:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008c8e:	f831 0013 	ldrh.w	r0, [r1, r3, lsl #1]
 8008c92:	b2d1      	uxtb	r1, r2
 8008c94:	4288      	cmp	r0, r1
 8008c96:	f67f aea7 	bls.w	80089e8 <main+0x5d0>
 8008c9a:	4923      	ldr	r1, [pc, #140]	; (8008d28 <main+0x910>)
 8008c9c:	f831 1013 	ldrh.w	r1, [r1, r3, lsl #1]
 8008ca0:	440a      	add	r2, r1
 8008ca2:	e726      	b.n	8008af2 <main+0x6da>
 8008ca4:	210a      	movs	r1, #10
 8008ca6:	5cea      	ldrb	r2, [r5, r3]
 8008ca8:	fb01 2303 	mla	r3, r1, r3, r2
 8008cac:	491e      	ldr	r1, [pc, #120]	; (8008d28 <main+0x910>)
 8008cae:	f836 2013 	ldrh.w	r2, [r6, r3, lsl #1]
 8008cb2:	f831 0013 	ldrh.w	r0, [r1, r3, lsl #1]
 8008cb6:	4927      	ldr	r1, [pc, #156]	; (8008d54 <main+0x93c>)
 8008cb8:	4402      	add	r2, r0
 8008cba:	f831 1013 	ldrh.w	r1, [r1, r3, lsl #1]
 8008cbe:	428a      	cmp	r2, r1
 8008cc0:	f6bf af29 	bge.w	8008b16 <main+0x6fe>
 8008cc4:	e715      	b.n	8008af2 <main+0x6da>
 8008cc6:	4b19      	ldr	r3, [pc, #100]	; (8008d2c <main+0x914>)
 8008cc8:	781b      	ldrb	r3, [r3, #0]
 8008cca:	2b00      	cmp	r3, #0
 8008ccc:	f43f ac3f 	beq.w	800854e <main+0x136>
 8008cd0:	78a3      	ldrb	r3, [r4, #2]
 8008cd2:	b9b3      	cbnz	r3, 8008d02 <main+0x8ea>
 8008cd4:	f895 9000 	ldrb.w	r9, [r5]
 8008cd8:	f1b9 0f00 	cmp.w	r9, #0
 8008cdc:	f47f abf8 	bne.w	80084d0 <main+0xb8>
 8008ce0:	4b13      	ldr	r3, [pc, #76]	; (8008d30 <main+0x918>)
 8008ce2:	4a14      	ldr	r2, [pc, #80]	; (8008d34 <main+0x91c>)
 8008ce4:	881b      	ldrh	r3, [r3, #0]
 8008ce6:	4e1d      	ldr	r6, [pc, #116]	; (8008d5c <main+0x944>)
 8008ce8:	5cd0      	ldrb	r0, [r2, r3]
 8008cea:	4a13      	ldr	r2, [pc, #76]	; (8008d38 <main+0x920>)
 8008cec:	00db      	lsls	r3, r3, #3
 8008cee:	441a      	add	r2, r3
 8008cf0:	5c11      	ldrb	r1, [r2, r0]
 8008cf2:	4433      	add	r3, r6
 8008cf4:	5c1b      	ldrb	r3, [r3, r0]
 8008cf6:	428b      	cmp	r3, r1
 8008cf8:	f67f ac29 	bls.w	800854e <main+0x136>
 8008cfc:	3101      	adds	r1, #1
 8008cfe:	5411      	strb	r1, [r2, r0]
 8008d00:	e425      	b.n	800854e <main+0x136>
 8008d02:	2b04      	cmp	r3, #4
 8008d04:	f47f ac23 	bne.w	800854e <main+0x136>
 8008d08:	4b09      	ldr	r3, [pc, #36]	; (8008d30 <main+0x918>)
 8008d0a:	7929      	ldrb	r1, [r5, #4]
 8008d0c:	4a0c      	ldr	r2, [pc, #48]	; (8008d40 <main+0x928>)
 8008d0e:	f8b3 3050 	ldrh.w	r3, [r3, #80]	; 0x50
 8008d12:	eb02 0281 	add.w	r2, r2, r1, lsl #2
 8008d16:	5cd0      	ldrb	r0, [r2, r3]
 8008d18:	4a0a      	ldr	r2, [pc, #40]	; (8008d44 <main+0x92c>)
 8008d1a:	00db      	lsls	r3, r3, #3
 8008d1c:	441a      	add	r2, r3
 8008d1e:	5c11      	ldrb	r1, [r2, r0]
 8008d20:	4e0f      	ldr	r6, [pc, #60]	; (8008d60 <main+0x948>)
 8008d22:	e7e6      	b.n	8008cf2 <main+0x8da>
 8008d24:	08012c8c 	.word	0x08012c8c
 8008d28:	0801360a 	.word	0x0801360a
 8008d2c:	20004c0c 	.word	0x20004c0c
 8008d30:	2000000c 	.word	0x2000000c
 8008d34:	20000240 	.word	0x20000240
 8008d38:	20000098 	.word	0x20000098
 8008d3c:	08012c0c 	.word	0x08012c0c
 8008d40:	2000024b 	.word	0x2000024b
 8008d44:	200000b8 	.word	0x200000b8
 8008d48:	08012c4c 	.word	0x08012c4c
 8008d4c:	20004c0f 	.word	0x20004c0f
 8008d50:	20004c20 	.word	0x20004c20
 8008d54:	08012d18 	.word	0x08012d18
 8008d58:	fffc0000 	.word	0xfffc0000
 8008d5c:	08012c2c 	.word	0x08012c2c
 8008d60:	08012c6c 	.word	0x08012c6c

08008d64 <DrawTriangle.part.1>:
 8008d64:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8008d68:	461f      	mov	r7, r3
 8008d6a:	4614      	mov	r4, r2
 8008d6c:	eb01 0802 	add.w	r8, r1, r2
 8008d70:	b90c      	cbnz	r4, 8008d76 <DrawTriangle.part.1+0x12>
 8008d72:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8008d76:	eba8 0104 	sub.w	r1, r8, r4
 8008d7a:	b289      	uxth	r1, r1
 8008d7c:	1c45      	adds	r5, r0, #1
 8008d7e:	2600      	movs	r6, #0
 8008d80:	f002 fed8 	bl	800bb34 <Set_Posi>
 8008d84:	b2ad      	uxth	r5, r5
 8008d86:	b2b3      	uxth	r3, r6
 8008d88:	429c      	cmp	r4, r3
 8008d8a:	d803      	bhi.n	8008d94 <DrawTriangle.part.1+0x30>
 8008d8c:	3c02      	subs	r4, #2
 8008d8e:	b2a4      	uxth	r4, r4
 8008d90:	4628      	mov	r0, r5
 8008d92:	e7ed      	b.n	8008d70 <DrawTriangle.part.1+0xc>
 8008d94:	4638      	mov	r0, r7
 8008d96:	f002 fed9 	bl	800bb4c <Set_Pixel>
 8008d9a:	3601      	adds	r6, #1
 8008d9c:	e7f3      	b.n	8008d86 <DrawTriangle.part.1+0x22>

08008d9e <DrawTriCol.part.3>:
 8008d9e:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8008da2:	461f      	mov	r7, r3
 8008da4:	4614      	mov	r4, r2
 8008da6:	f8bd 8020 	ldrh.w	r8, [sp, #32]
 8008daa:	eb01 0902 	add.w	r9, r1, r2
 8008dae:	f100 0a03 	add.w	sl, r0, #3
 8008db2:	b90c      	cbnz	r4, 8008db8 <DrawTriCol.part.3+0x1a>
 8008db4:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8008db8:	eba9 0104 	sub.w	r1, r9, r4
 8008dbc:	b289      	uxth	r1, r1
 8008dbe:	1c45      	adds	r5, r0, #1
 8008dc0:	2600      	movs	r6, #0
 8008dc2:	f002 feb7 	bl	800bb34 <Set_Posi>
 8008dc6:	b2ad      	uxth	r5, r5
 8008dc8:	b2b3      	uxth	r3, r6
 8008dca:	429c      	cmp	r4, r3
 8008dcc:	d803      	bhi.n	8008dd6 <DrawTriCol.part.3+0x38>
 8008dce:	3c02      	subs	r4, #2
 8008dd0:	b2a4      	uxth	r4, r4
 8008dd2:	4628      	mov	r0, r5
 8008dd4:	e7ed      	b.n	8008db2 <DrawTriCol.part.3+0x14>
 8008dd6:	45aa      	cmp	sl, r5
 8008dd8:	bfac      	ite	ge
 8008dda:	4638      	movge	r0, r7
 8008ddc:	4640      	movlt	r0, r8
 8008dde:	f002 feb5 	bl	800bb4c <Set_Pixel>
 8008de2:	3601      	adds	r6, #1
 8008de4:	e7f0      	b.n	8008dc8 <DrawTriCol.part.3+0x2a>

08008de6 <DrawTriggerIco>:
 8008de6:	e92d 41f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, lr}
 8008dea:	4e38      	ldr	r6, [pc, #224]	; (8008ecc <DrawTriggerIco+0xe6>)
 8008dec:	24d0      	movs	r4, #208	; 0xd0
 8008dee:	2500      	movs	r5, #0
 8008df0:	4637      	mov	r7, r6
 8008df2:	f8df 8100 	ldr.w	r8, [pc, #256]	; 8008ef4 <DrawTriggerIco+0x10e>
 8008df6:	f647 30ef 	movw	r0, #31727	; 0x7bef
 8008dfa:	2100      	movs	r1, #0
 8008dfc:	f003 f93b 	bl	800c076 <Set_Color>
 8008e00:	8db3      	ldrh	r3, [r6, #44]	; 0x2c
 8008e02:	0a1b      	lsrs	r3, r3, #8
 8008e04:	412b      	asrs	r3, r5
 8008e06:	07d8      	lsls	r0, r3, #31
 8008e08:	d50f      	bpl.n	8008e2a <DrawTriggerIco+0x44>
 8008e0a:	4b31      	ldr	r3, [pc, #196]	; (8008ed0 <DrawTriggerIco+0xea>)
 8008e0c:	2220      	movs	r2, #32
 8008e0e:	9300      	str	r3, [sp, #0]
 8008e10:	2301      	movs	r3, #1
 8008e12:	4621      	mov	r1, r4
 8008e14:	2012      	movs	r0, #18
 8008e16:	f003 f935 	bl	800c084 <Show_Str6x8>
 8008e1a:	3501      	adds	r5, #1
 8008e1c:	3c19      	subs	r4, #25
 8008e1e:	2d04      	cmp	r5, #4
 8008e20:	b2a4      	uxth	r4, r4
 8008e22:	d1e8      	bne.n	8008df6 <DrawTriggerIco+0x10>
 8008e24:	b002      	add	sp, #8
 8008e26:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8008e2a:	8d33      	ldrh	r3, [r6, #40]	; 0x28
 8008e2c:	1e5a      	subs	r2, r3, #1
 8008e2e:	2a06      	cmp	r2, #6
 8008e30:	d8f3      	bhi.n	8008e1a <DrawTriggerIco+0x34>
 8008e32:	e8df f002 	tbb	[pc, r2]
 8008e36:	0909      	.short	0x0909
 8008e38:	36222204 	.word	0x36222204
 8008e3c:	36          	.byte	0x36
 8008e3d:	00          	.byte	0x00
 8008e3e:	4b25      	ldr	r3, [pc, #148]	; (8008ed4 <DrawTriggerIco+0xee>)
 8008e40:	2200      	movs	r2, #0
 8008e42:	9300      	str	r3, [sp, #0]
 8008e44:	2301      	movs	r3, #1
 8008e46:	e7e4      	b.n	8008e12 <DrawTriggerIco+0x2c>
 8008e48:	4a23      	ldr	r2, [pc, #140]	; (8008ed8 <DrawTriggerIco+0xf2>)
 8008e4a:	4621      	mov	r1, r4
 8008e4c:	2b02      	cmp	r3, #2
 8008e4e:	bf0c      	ite	eq
 8008e50:	4643      	moveq	r3, r8
 8008e52:	4613      	movne	r3, r2
 8008e54:	2012      	movs	r0, #18
 8008e56:	9300      	str	r3, [sp, #0]
 8008e58:	2220      	movs	r2, #32
 8008e5a:	2301      	movs	r3, #1
 8008e5c:	f003 f912 	bl	800c084 <Show_Str6x8>
 8008e60:	8d7b      	ldrh	r3, [r7, #42]	; 0x2a
 8008e62:	0a1b      	lsrs	r3, r3, #8
 8008e64:	412b      	asrs	r3, r5
 8008e66:	07d9      	lsls	r1, r3, #31
 8008e68:	bf4c      	ite	mi
 8008e6a:	4b1c      	ldrmi	r3, [pc, #112]	; (8008edc <DrawTriggerIco+0xf6>)
 8008e6c:	4b1c      	ldrpl	r3, [pc, #112]	; (8008ee0 <DrawTriggerIco+0xfa>)
 8008e6e:	9300      	str	r3, [sp, #0]
 8008e70:	2220      	movs	r2, #32
 8008e72:	2301      	movs	r3, #1
 8008e74:	4621      	mov	r1, r4
 8008e76:	2018      	movs	r0, #24
 8008e78:	e7cd      	b.n	8008e16 <DrawTriggerIco+0x30>
 8008e7a:	4a17      	ldr	r2, [pc, #92]	; (8008ed8 <DrawTriggerIco+0xf2>)
 8008e7c:	4621      	mov	r1, r4
 8008e7e:	2b05      	cmp	r3, #5
 8008e80:	bf0c      	ite	eq
 8008e82:	4643      	moveq	r3, r8
 8008e84:	4613      	movne	r3, r2
 8008e86:	2012      	movs	r0, #18
 8008e88:	9300      	str	r3, [sp, #0]
 8008e8a:	2220      	movs	r2, #32
 8008e8c:	2301      	movs	r3, #1
 8008e8e:	f003 f8f9 	bl	800c084 <Show_Str6x8>
 8008e92:	8d7b      	ldrh	r3, [r7, #42]	; 0x2a
 8008e94:	0a1b      	lsrs	r3, r3, #8
 8008e96:	412b      	asrs	r3, r5
 8008e98:	07da      	lsls	r2, r3, #31
 8008e9a:	bf4c      	ite	mi
 8008e9c:	4b11      	ldrmi	r3, [pc, #68]	; (8008ee4 <DrawTriggerIco+0xfe>)
 8008e9e:	4b12      	ldrpl	r3, [pc, #72]	; (8008ee8 <DrawTriggerIco+0x102>)
 8008ea0:	e7e5      	b.n	8008e6e <DrawTriggerIco+0x88>
 8008ea2:	4a0d      	ldr	r2, [pc, #52]	; (8008ed8 <DrawTriggerIco+0xf2>)
 8008ea4:	4621      	mov	r1, r4
 8008ea6:	2b07      	cmp	r3, #7
 8008ea8:	bf0c      	ite	eq
 8008eaa:	4643      	moveq	r3, r8
 8008eac:	4613      	movne	r3, r2
 8008eae:	2012      	movs	r0, #18
 8008eb0:	9300      	str	r3, [sp, #0]
 8008eb2:	2220      	movs	r2, #32
 8008eb4:	2301      	movs	r3, #1
 8008eb6:	f003 f8e5 	bl	800c084 <Show_Str6x8>
 8008eba:	8d7b      	ldrh	r3, [r7, #42]	; 0x2a
 8008ebc:	0a1b      	lsrs	r3, r3, #8
 8008ebe:	412b      	asrs	r3, r5
 8008ec0:	07db      	lsls	r3, r3, #31
 8008ec2:	bf4c      	ite	mi
 8008ec4:	4b09      	ldrmi	r3, [pc, #36]	; (8008eec <DrawTriggerIco+0x106>)
 8008ec6:	4b0a      	ldrpl	r3, [pc, #40]	; (8008ef0 <DrawTriggerIco+0x10a>)
 8008ec8:	e7d1      	b.n	8008e6e <DrawTriggerIco+0x88>
 8008eca:	bf00      	nop
 8008ecc:	2000000c 	.word	0x2000000c
 8008ed0:	08013826 	.word	0x08013826
 8008ed4:	0801382a 	.word	0x0801382a
 8008ed8:	08013824 	.word	0x08013824
 8008edc:	0801382e 	.word	0x0801382e
 8008ee0:	08013831 	.word	0x08013831
 8008ee4:	08013834 	.word	0x08013834
 8008ee8:	08013837 	.word	0x08013837
 8008eec:	0801467e 	.word	0x0801467e
 8008ef0:	0801383a 	.word	0x0801383a
 8008ef4:	08013822 	.word	0x08013822

08008ef8 <ShowSubPopStr>:
 8008ef8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8008efc:	468b      	mov	fp, r1
 8008efe:	2700      	movs	r7, #0
 8008f00:	b089      	sub	sp, #36	; 0x24
 8008f02:	f89d 1058 	ldrb.w	r1, [sp, #88]	; 0x58
 8008f06:	9304      	str	r3, [sp, #16]
 8008f08:	f89d 3050 	ldrb.w	r3, [sp, #80]	; 0x50
 8008f0c:	f100 0806 	add.w	r8, r0, #6
 8008f10:	4692      	mov	sl, r2
 8008f12:	1e4a      	subs	r2, r1, #1
 8008f14:	9305      	str	r3, [sp, #20]
 8008f16:	fa1f f888 	uxth.w	r8, r8
 8008f1a:	f89d 3054 	ldrb.w	r3, [sp, #84]	; 0x54
 8008f1e:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 8008f22:	eb08 0242 	add.w	r2, r8, r2, lsl #1
 8008f26:	9306      	str	r3, [sp, #24]
 8008f28:	b293      	uxth	r3, r2
 8008f2a:	9302      	str	r3, [sp, #8]
 8008f2c:	f89d 305c 	ldrb.w	r3, [sp, #92]	; 0x5c
 8008f30:	eb01 0541 	add.w	r5, r1, r1, lsl #1
 8008f34:	3b01      	subs	r3, #1
 8008f36:	440b      	add	r3, r1
 8008f38:	eb00 0545 	add.w	r5, r0, r5, lsl #1
 8008f3c:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 8008f40:	300c      	adds	r0, #12
 8008f42:	eb00 0343 	add.w	r3, r0, r3, lsl #1
 8008f46:	b29b      	uxth	r3, r3
 8008f48:	9303      	str	r3, [sp, #12]
 8008f4a:	f10b 0313 	add.w	r3, fp, #19
 8008f4e:	3506      	adds	r5, #6
 8008f50:	b29b      	uxth	r3, r3
 8008f52:	4e69      	ldr	r6, [pc, #420]	; (80090f8 <ShowSubPopStr+0x200>)
 8008f54:	b2ad      	uxth	r5, r5
 8008f56:	9307      	str	r3, [sp, #28]
 8008f58:	b2fa      	uxtb	r2, r7
 8008f5a:	f1ba 0f00 	cmp.w	sl, #0
 8008f5e:	d172      	bne.n	8009046 <ShowSubPopStr+0x14e>
 8008f60:	9c07      	ldr	r4, [sp, #28]
 8008f62:	9b04      	ldr	r3, [sp, #16]
 8008f64:	4293      	cmp	r3, r2
 8008f66:	d178      	bne.n	800905a <ShowSubPopStr+0x162>
 8008f68:	2100      	movs	r1, #0
 8008f6a:	f240 70ff 	movw	r0, #2047	; 0x7ff
 8008f6e:	f003 f882 	bl	800c076 <Set_Color>
 8008f72:	9b12      	ldr	r3, [sp, #72]	; 0x48
 8008f74:	2200      	movs	r2, #0
 8008f76:	9300      	str	r3, [sp, #0]
 8008f78:	4621      	mov	r1, r4
 8008f7a:	2301      	movs	r3, #1
 8008f7c:	4640      	mov	r0, r8
 8008f7e:	f003 f881 	bl	800c084 <Show_Str6x8>
 8008f82:	495e      	ldr	r1, [pc, #376]	; (80090fc <ShowSubPopStr+0x204>)
 8008f84:	4a5e      	ldr	r2, [pc, #376]	; (8009100 <ShowSubPopStr+0x208>)
 8008f86:	7889      	ldrb	r1, [r1, #2]
 8008f88:	8833      	ldrh	r3, [r6, #0]
 8008f8a:	4411      	add	r1, r2
 8008f8c:	7909      	ldrb	r1, [r1, #4]
 8008f8e:	eb02 0281 	add.w	r2, r2, r1, lsl #2
 8008f92:	5cd0      	ldrb	r0, [r2, r3]
 8008f94:	00db      	lsls	r3, r3, #3
 8008f96:	4a5b      	ldr	r2, [pc, #364]	; (8009104 <ShowSubPopStr+0x20c>)
 8008f98:	18f1      	adds	r1, r6, r3
 8008f9a:	4401      	add	r1, r0
 8008f9c:	4413      	add	r3, r2
 8008f9e:	5c1b      	ldrb	r3, [r3, r0]
 8008fa0:	f891 208c 	ldrb.w	r2, [r1, #140]	; 0x8c
 8008fa4:	f240 70ff 	movw	r0, #2047	; 0x7ff
 8008fa8:	429a      	cmp	r2, r3
 8008faa:	bf0c      	ite	eq
 8008fac:	f643 11e7 	movweq	r1, #14823	; 0x39e7
 8008fb0:	2100      	movne	r1, #0
 8008fb2:	f003 f860 	bl	800c076 <Set_Color>
 8008fb6:	4b54      	ldr	r3, [pc, #336]	; (8009108 <ShowSubPopStr+0x210>)
 8008fb8:	2200      	movs	r2, #0
 8008fba:	9300      	str	r3, [sp, #0]
 8008fbc:	4621      	mov	r1, r4
 8008fbe:	2301      	movs	r3, #1
 8008fc0:	9802      	ldr	r0, [sp, #8]
 8008fc2:	f003 f85f 	bl	800c084 <Show_Str6x8>
 8008fc6:	2100      	movs	r1, #0
 8008fc8:	f240 70ff 	movw	r0, #2047	; 0x7ff
 8008fcc:	f003 f853 	bl	800c076 <Set_Color>
 8008fd0:	9b13      	ldr	r3, [sp, #76]	; 0x4c
 8008fd2:	2200      	movs	r2, #0
 8008fd4:	9300      	str	r3, [sp, #0]
 8008fd6:	4621      	mov	r1, r4
 8008fd8:	2301      	movs	r3, #1
 8008fda:	4628      	mov	r0, r5
 8008fdc:	f003 f852 	bl	800c084 <Show_Str6x8>
 8008fe0:	4946      	ldr	r1, [pc, #280]	; (80090fc <ShowSubPopStr+0x204>)
 8008fe2:	4a47      	ldr	r2, [pc, #284]	; (8009100 <ShowSubPopStr+0x208>)
 8008fe4:	7889      	ldrb	r1, [r1, #2]
 8008fe6:	8833      	ldrh	r3, [r6, #0]
 8008fe8:	4411      	add	r1, r2
 8008fea:	7909      	ldrb	r1, [r1, #4]
 8008fec:	eb02 0281 	add.w	r2, r2, r1, lsl #2
 8008ff0:	5cd0      	ldrb	r0, [r2, r3]
 8008ff2:	00da      	lsls	r2, r3, #3
 8008ff4:	4b43      	ldr	r3, [pc, #268]	; (8009104 <ShowSubPopStr+0x20c>)
 8008ff6:	18b1      	adds	r1, r6, r2
 8008ff8:	4413      	add	r3, r2
 8008ffa:	4401      	add	r1, r0
 8008ffc:	4403      	add	r3, r0
 8008ffe:	f891 208c 	ldrb.w	r2, [r1, #140]	; 0x8c
 8009002:	f893 3020 	ldrb.w	r3, [r3, #32]
 8009006:	f240 70ff 	movw	r0, #2047	; 0x7ff
 800900a:	429a      	cmp	r2, r3
 800900c:	bf0c      	ite	eq
 800900e:	f643 11e7 	movweq	r1, #14823	; 0x39e7
 8009012:	2100      	movne	r1, #0
 8009014:	f003 f82f 	bl	800c076 <Set_Color>
 8009018:	4b3c      	ldr	r3, [pc, #240]	; (800910c <ShowSubPopStr+0x214>)
 800901a:	2200      	movs	r2, #0
 800901c:	9300      	str	r3, [sp, #0]
 800901e:	4621      	mov	r1, r4
 8009020:	2301      	movs	r3, #1
 8009022:	9803      	ldr	r0, [sp, #12]
 8009024:	f003 f82e 	bl	800c084 <Show_Str6x8>
 8009028:	9b12      	ldr	r3, [sp, #72]	; 0x48
 800902a:	9a05      	ldr	r2, [sp, #20]
 800902c:	3701      	adds	r7, #1
 800902e:	4413      	add	r3, r2
 8009030:	9312      	str	r3, [sp, #72]	; 0x48
 8009032:	9a06      	ldr	r2, [sp, #24]
 8009034:	9b13      	ldr	r3, [sp, #76]	; 0x4c
 8009036:	4413      	add	r3, r2
 8009038:	9313      	str	r3, [sp, #76]	; 0x4c
 800903a:	b2fb      	uxtb	r3, r7
 800903c:	4553      	cmp	r3, sl
 800903e:	d98b      	bls.n	8008f58 <ShowSubPopStr+0x60>
 8009040:	b009      	add	sp, #36	; 0x24
 8009042:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8009046:	ebaa 0402 	sub.w	r4, sl, r2
 800904a:	ebc4 04c4 	rsb	r4, r4, r4, lsl #3
 800904e:	f10b 0305 	add.w	r3, fp, #5
 8009052:	eb03 0444 	add.w	r4, r3, r4, lsl #1
 8009056:	b2a4      	uxth	r4, r4
 8009058:	e783      	b.n	8008f62 <ShowSubPopStr+0x6a>
 800905a:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800905e:	f643 10e7 	movw	r0, #14823	; 0x39e7
 8009062:	f003 f808 	bl	800c076 <Set_Color>
 8009066:	9b12      	ldr	r3, [sp, #72]	; 0x48
 8009068:	2200      	movs	r2, #0
 800906a:	9300      	str	r3, [sp, #0]
 800906c:	4621      	mov	r1, r4
 800906e:	2301      	movs	r3, #1
 8009070:	4640      	mov	r0, r8
 8009072:	f003 f807 	bl	800c084 <Show_Str6x8>
 8009076:	8833      	ldrh	r3, [r6, #0]
 8009078:	4a22      	ldr	r2, [pc, #136]	; (8009104 <ShowSubPopStr+0x20c>)
 800907a:	00db      	lsls	r3, r3, #3
 800907c:	fa5f f987 	uxtb.w	r9, r7
 8009080:	18f1      	adds	r1, r6, r3
 8009082:	4449      	add	r1, r9
 8009084:	4413      	add	r3, r2
 8009086:	f813 3009 	ldrb.w	r3, [r3, r9]
 800908a:	f891 208c 	ldrb.w	r2, [r1, #140]	; 0x8c
 800908e:	f643 10e7 	movw	r0, #14823	; 0x39e7
 8009092:	429a      	cmp	r2, r3
 8009094:	bf0c      	ite	eq
 8009096:	f647 31ef 	movweq	r1, #31727	; 0x7bef
 800909a:	f64f 71ff 	movwne	r1, #65535	; 0xffff
 800909e:	f002 ffea 	bl	800c076 <Set_Color>
 80090a2:	4b19      	ldr	r3, [pc, #100]	; (8009108 <ShowSubPopStr+0x210>)
 80090a4:	2200      	movs	r2, #0
 80090a6:	9300      	str	r3, [sp, #0]
 80090a8:	4621      	mov	r1, r4
 80090aa:	2301      	movs	r3, #1
 80090ac:	9802      	ldr	r0, [sp, #8]
 80090ae:	f002 ffe9 	bl	800c084 <Show_Str6x8>
 80090b2:	f64f 71ff 	movw	r1, #65535	; 0xffff
 80090b6:	f643 10e7 	movw	r0, #14823	; 0x39e7
 80090ba:	f002 ffdc 	bl	800c076 <Set_Color>
 80090be:	9b13      	ldr	r3, [sp, #76]	; 0x4c
 80090c0:	2200      	movs	r2, #0
 80090c2:	9300      	str	r3, [sp, #0]
 80090c4:	4621      	mov	r1, r4
 80090c6:	2301      	movs	r3, #1
 80090c8:	4628      	mov	r0, r5
 80090ca:	f002 ffdb 	bl	800c084 <Show_Str6x8>
 80090ce:	8833      	ldrh	r3, [r6, #0]
 80090d0:	f643 10e7 	movw	r0, #14823	; 0x39e7
 80090d4:	00da      	lsls	r2, r3, #3
 80090d6:	4b0b      	ldr	r3, [pc, #44]	; (8009104 <ShowSubPopStr+0x20c>)
 80090d8:	18b1      	adds	r1, r6, r2
 80090da:	4413      	add	r3, r2
 80090dc:	4449      	add	r1, r9
 80090de:	4499      	add	r9, r3
 80090e0:	f891 208c 	ldrb.w	r2, [r1, #140]	; 0x8c
 80090e4:	f899 3020 	ldrb.w	r3, [r9, #32]
 80090e8:	429a      	cmp	r2, r3
 80090ea:	bf0c      	ite	eq
 80090ec:	f647 31ef 	movweq	r1, #31727	; 0x7bef
 80090f0:	f64f 71ff 	movwne	r1, #65535	; 0xffff
 80090f4:	e78e      	b.n	8009014 <ShowSubPopStr+0x11c>
 80090f6:	bf00      	nop
 80090f8:	2000000c 	.word	0x2000000c
 80090fc:	20004c12 	.word	0x20004c12
 8009100:	20000240 	.word	0x20000240
 8009104:	08012c0c 	.word	0x08012c0c
 8009108:	0801383d 	.word	0x0801383d
 800910c:	0801383f 	.word	0x0801383f

08009110 <ShowOutSubPopStr>:
 8009110:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8009114:	468b      	mov	fp, r1
 8009116:	2700      	movs	r7, #0
 8009118:	b089      	sub	sp, #36	; 0x24
 800911a:	f89d 1058 	ldrb.w	r1, [sp, #88]	; 0x58
 800911e:	9304      	str	r3, [sp, #16]
 8009120:	f89d 3050 	ldrb.w	r3, [sp, #80]	; 0x50
 8009124:	f100 0806 	add.w	r8, r0, #6
 8009128:	4692      	mov	sl, r2
 800912a:	1e4a      	subs	r2, r1, #1
 800912c:	9305      	str	r3, [sp, #20]
 800912e:	fa1f f888 	uxth.w	r8, r8
 8009132:	f89d 3054 	ldrb.w	r3, [sp, #84]	; 0x54
 8009136:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 800913a:	eb08 0242 	add.w	r2, r8, r2, lsl #1
 800913e:	9306      	str	r3, [sp, #24]
 8009140:	b293      	uxth	r3, r2
 8009142:	9302      	str	r3, [sp, #8]
 8009144:	f89d 305c 	ldrb.w	r3, [sp, #92]	; 0x5c
 8009148:	eb01 0541 	add.w	r5, r1, r1, lsl #1
 800914c:	3b01      	subs	r3, #1
 800914e:	440b      	add	r3, r1
 8009150:	eb00 0545 	add.w	r5, r0, r5, lsl #1
 8009154:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 8009158:	300c      	adds	r0, #12
 800915a:	eb00 0343 	add.w	r3, r0, r3, lsl #1
 800915e:	b29b      	uxth	r3, r3
 8009160:	9303      	str	r3, [sp, #12]
 8009162:	f10b 0313 	add.w	r3, fp, #19
 8009166:	3506      	adds	r5, #6
 8009168:	b29b      	uxth	r3, r3
 800916a:	4e6d      	ldr	r6, [pc, #436]	; (8009320 <ShowOutSubPopStr+0x210>)
 800916c:	b2ad      	uxth	r5, r5
 800916e:	9307      	str	r3, [sp, #28]
 8009170:	b2fa      	uxtb	r2, r7
 8009172:	f1ba 0f00 	cmp.w	sl, #0
 8009176:	d178      	bne.n	800926a <ShowOutSubPopStr+0x15a>
 8009178:	9c07      	ldr	r4, [sp, #28]
 800917a:	9b04      	ldr	r3, [sp, #16]
 800917c:	4293      	cmp	r3, r2
 800917e:	d17e      	bne.n	800927e <ShowOutSubPopStr+0x16e>
 8009180:	2100      	movs	r1, #0
 8009182:	f240 70ff 	movw	r0, #2047	; 0x7ff
 8009186:	f002 ff76 	bl	800c076 <Set_Color>
 800918a:	9b12      	ldr	r3, [sp, #72]	; 0x48
 800918c:	2200      	movs	r2, #0
 800918e:	9300      	str	r3, [sp, #0]
 8009190:	4621      	mov	r1, r4
 8009192:	2301      	movs	r3, #1
 8009194:	4640      	mov	r0, r8
 8009196:	f002 ff75 	bl	800c084 <Show_Str6x8>
 800919a:	4962      	ldr	r1, [pc, #392]	; (8009324 <ShowOutSubPopStr+0x214>)
 800919c:	4b62      	ldr	r3, [pc, #392]	; (8009328 <ShowOutSubPopStr+0x218>)
 800919e:	7889      	ldrb	r1, [r1, #2]
 80091a0:	f8b6 2050 	ldrh.w	r2, [r6, #80]	; 0x50
 80091a4:	4419      	add	r1, r3
 80091a6:	7909      	ldrb	r1, [r1, #4]
 80091a8:	eb03 0381 	add.w	r3, r3, r1, lsl #2
 80091ac:	4413      	add	r3, r2
 80091ae:	7ad8      	ldrb	r0, [r3, #11]
 80091b0:	4b5e      	ldr	r3, [pc, #376]	; (800932c <ShowOutSubPopStr+0x21c>)
 80091b2:	00d2      	lsls	r2, r2, #3
 80091b4:	18b1      	adds	r1, r6, r2
 80091b6:	4413      	add	r3, r2
 80091b8:	4401      	add	r1, r0
 80091ba:	4403      	add	r3, r0
 80091bc:	f891 20ac 	ldrb.w	r2, [r1, #172]	; 0xac
 80091c0:	f893 3040 	ldrb.w	r3, [r3, #64]	; 0x40
 80091c4:	f240 70ff 	movw	r0, #2047	; 0x7ff
 80091c8:	429a      	cmp	r2, r3
 80091ca:	bf0c      	ite	eq
 80091cc:	f643 11e7 	movweq	r1, #14823	; 0x39e7
 80091d0:	2100      	movne	r1, #0
 80091d2:	f002 ff50 	bl	800c076 <Set_Color>
 80091d6:	4b56      	ldr	r3, [pc, #344]	; (8009330 <ShowOutSubPopStr+0x220>)
 80091d8:	2200      	movs	r2, #0
 80091da:	9300      	str	r3, [sp, #0]
 80091dc:	4621      	mov	r1, r4
 80091de:	2301      	movs	r3, #1
 80091e0:	9802      	ldr	r0, [sp, #8]
 80091e2:	f002 ff4f 	bl	800c084 <Show_Str6x8>
 80091e6:	2100      	movs	r1, #0
 80091e8:	f240 70ff 	movw	r0, #2047	; 0x7ff
 80091ec:	f002 ff43 	bl	800c076 <Set_Color>
 80091f0:	9b13      	ldr	r3, [sp, #76]	; 0x4c
 80091f2:	2200      	movs	r2, #0
 80091f4:	9300      	str	r3, [sp, #0]
 80091f6:	4621      	mov	r1, r4
 80091f8:	2301      	movs	r3, #1
 80091fa:	4628      	mov	r0, r5
 80091fc:	f002 ff42 	bl	800c084 <Show_Str6x8>
 8009200:	4948      	ldr	r1, [pc, #288]	; (8009324 <ShowOutSubPopStr+0x214>)
 8009202:	4b49      	ldr	r3, [pc, #292]	; (8009328 <ShowOutSubPopStr+0x218>)
 8009204:	7889      	ldrb	r1, [r1, #2]
 8009206:	f8b6 2050 	ldrh.w	r2, [r6, #80]	; 0x50
 800920a:	4419      	add	r1, r3
 800920c:	7909      	ldrb	r1, [r1, #4]
 800920e:	eb03 0381 	add.w	r3, r3, r1, lsl #2
 8009212:	4413      	add	r3, r2
 8009214:	7ad8      	ldrb	r0, [r3, #11]
 8009216:	4b45      	ldr	r3, [pc, #276]	; (800932c <ShowOutSubPopStr+0x21c>)
 8009218:	00d2      	lsls	r2, r2, #3
 800921a:	18b1      	adds	r1, r6, r2
 800921c:	4413      	add	r3, r2
 800921e:	4401      	add	r1, r0
 8009220:	4403      	add	r3, r0
 8009222:	f891 20ac 	ldrb.w	r2, [r1, #172]	; 0xac
 8009226:	f893 3060 	ldrb.w	r3, [r3, #96]	; 0x60
 800922a:	f240 70ff 	movw	r0, #2047	; 0x7ff
 800922e:	429a      	cmp	r2, r3
 8009230:	bf0c      	ite	eq
 8009232:	f643 11e7 	movweq	r1, #14823	; 0x39e7
 8009236:	2100      	movne	r1, #0
 8009238:	f002 ff1d 	bl	800c076 <Set_Color>
 800923c:	4b3d      	ldr	r3, [pc, #244]	; (8009334 <ShowOutSubPopStr+0x224>)
 800923e:	2200      	movs	r2, #0
 8009240:	9300      	str	r3, [sp, #0]
 8009242:	4621      	mov	r1, r4
 8009244:	2301      	movs	r3, #1
 8009246:	9803      	ldr	r0, [sp, #12]
 8009248:	f002 ff1c 	bl	800c084 <Show_Str6x8>
 800924c:	9b12      	ldr	r3, [sp, #72]	; 0x48
 800924e:	9a05      	ldr	r2, [sp, #20]
 8009250:	3701      	adds	r7, #1
 8009252:	4413      	add	r3, r2
 8009254:	9312      	str	r3, [sp, #72]	; 0x48
 8009256:	9a06      	ldr	r2, [sp, #24]
 8009258:	9b13      	ldr	r3, [sp, #76]	; 0x4c
 800925a:	4413      	add	r3, r2
 800925c:	9313      	str	r3, [sp, #76]	; 0x4c
 800925e:	b2fb      	uxtb	r3, r7
 8009260:	4553      	cmp	r3, sl
 8009262:	d985      	bls.n	8009170 <ShowOutSubPopStr+0x60>
 8009264:	b009      	add	sp, #36	; 0x24
 8009266:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800926a:	ebaa 0402 	sub.w	r4, sl, r2
 800926e:	ebc4 04c4 	rsb	r4, r4, r4, lsl #3
 8009272:	f10b 0305 	add.w	r3, fp, #5
 8009276:	eb03 0444 	add.w	r4, r3, r4, lsl #1
 800927a:	b2a4      	uxth	r4, r4
 800927c:	e77d      	b.n	800917a <ShowOutSubPopStr+0x6a>
 800927e:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8009282:	f643 10e7 	movw	r0, #14823	; 0x39e7
 8009286:	f002 fef6 	bl	800c076 <Set_Color>
 800928a:	9b12      	ldr	r3, [sp, #72]	; 0x48
 800928c:	2200      	movs	r2, #0
 800928e:	9300      	str	r3, [sp, #0]
 8009290:	4621      	mov	r1, r4
 8009292:	2301      	movs	r3, #1
 8009294:	4640      	mov	r0, r8
 8009296:	f002 fef5 	bl	800c084 <Show_Str6x8>
 800929a:	f8b6 3050 	ldrh.w	r3, [r6, #80]	; 0x50
 800929e:	fa5f f987 	uxtb.w	r9, r7
 80092a2:	00da      	lsls	r2, r3, #3
 80092a4:	4b21      	ldr	r3, [pc, #132]	; (800932c <ShowOutSubPopStr+0x21c>)
 80092a6:	18b1      	adds	r1, r6, r2
 80092a8:	4413      	add	r3, r2
 80092aa:	4449      	add	r1, r9
 80092ac:	444b      	add	r3, r9
 80092ae:	f891 20ac 	ldrb.w	r2, [r1, #172]	; 0xac
 80092b2:	f893 3040 	ldrb.w	r3, [r3, #64]	; 0x40
 80092b6:	f643 10e7 	movw	r0, #14823	; 0x39e7
 80092ba:	429a      	cmp	r2, r3
 80092bc:	bf0c      	ite	eq
 80092be:	f647 31ef 	movweq	r1, #31727	; 0x7bef
 80092c2:	f64f 71ff 	movwne	r1, #65535	; 0xffff
 80092c6:	f002 fed6 	bl	800c076 <Set_Color>
 80092ca:	4b19      	ldr	r3, [pc, #100]	; (8009330 <ShowOutSubPopStr+0x220>)
 80092cc:	2200      	movs	r2, #0
 80092ce:	9300      	str	r3, [sp, #0]
 80092d0:	4621      	mov	r1, r4
 80092d2:	2301      	movs	r3, #1
 80092d4:	9802      	ldr	r0, [sp, #8]
 80092d6:	f002 fed5 	bl	800c084 <Show_Str6x8>
 80092da:	f64f 71ff 	movw	r1, #65535	; 0xffff
 80092de:	f643 10e7 	movw	r0, #14823	; 0x39e7
 80092e2:	f002 fec8 	bl	800c076 <Set_Color>
 80092e6:	9b13      	ldr	r3, [sp, #76]	; 0x4c
 80092e8:	2200      	movs	r2, #0
 80092ea:	9300      	str	r3, [sp, #0]
 80092ec:	4621      	mov	r1, r4
 80092ee:	2301      	movs	r3, #1
 80092f0:	4628      	mov	r0, r5
 80092f2:	f002 fec7 	bl	800c084 <Show_Str6x8>
 80092f6:	f8b6 3050 	ldrh.w	r3, [r6, #80]	; 0x50
 80092fa:	f643 10e7 	movw	r0, #14823	; 0x39e7
 80092fe:	00da      	lsls	r2, r3, #3
 8009300:	4b0a      	ldr	r3, [pc, #40]	; (800932c <ShowOutSubPopStr+0x21c>)
 8009302:	18b1      	adds	r1, r6, r2
 8009304:	4413      	add	r3, r2
 8009306:	4449      	add	r1, r9
 8009308:	4499      	add	r9, r3
 800930a:	f891 20ac 	ldrb.w	r2, [r1, #172]	; 0xac
 800930e:	f899 3060 	ldrb.w	r3, [r9, #96]	; 0x60
 8009312:	429a      	cmp	r2, r3
 8009314:	bf0c      	ite	eq
 8009316:	f647 31ef 	movweq	r1, #31727	; 0x7bef
 800931a:	f64f 71ff 	movwne	r1, #65535	; 0xffff
 800931e:	e78b      	b.n	8009238 <ShowOutSubPopStr+0x128>
 8009320:	2000000c 	.word	0x2000000c
 8009324:	20004c12 	.word	0x20004c12
 8009328:	20000240 	.word	0x20000240
 800932c:	08012c0c 	.word	0x08012c0c
 8009330:	0801383d 	.word	0x0801383d
 8009334:	0801383f 	.word	0x0801383f

08009338 <ShowPopStr>:
 8009338:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800933c:	2500      	movs	r5, #0
 800933e:	b087      	sub	sp, #28
 8009340:	f89d 4048 	ldrb.w	r4, [sp, #72]	; 0x48
 8009344:	9304      	str	r3, [sp, #16]
 8009346:	f89d 304c 	ldrb.w	r3, [sp, #76]	; 0x4c
 800934a:	468b      	mov	fp, r1
 800934c:	eb04 0144 	add.w	r1, r4, r4, lsl #1
 8009350:	eb00 0141 	add.w	r1, r0, r1, lsl #1
 8009354:	3b01      	subs	r3, #1
 8009356:	3106      	adds	r1, #6
 8009358:	4423      	add	r3, r4
 800935a:	4616      	mov	r6, r2
 800935c:	fa1f fa81 	uxth.w	sl, r1
 8009360:	1e62      	subs	r2, r4, #1
 8009362:	f100 010c 	add.w	r1, r0, #12
 8009366:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 800936a:	3006      	adds	r0, #6
 800936c:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 8009370:	eb01 0343 	add.w	r3, r1, r3, lsl #1
 8009374:	b287      	uxth	r7, r0
 8009376:	b29b      	uxth	r3, r3
 8009378:	eb07 0242 	add.w	r2, r7, r2, lsl #1
 800937c:	9302      	str	r3, [sp, #8]
 800937e:	b293      	uxth	r3, r2
 8009380:	9305      	str	r3, [sp, #20]
 8009382:	f04f 090d 	mov.w	r9, #13
 8009386:	9b10      	ldr	r3, [sp, #64]	; 0x40
 8009388:	9a11      	ldr	r2, [sp, #68]	; 0x44
 800938a:	fb09 3905 	mla	r9, r9, r5, r3
 800938e:	eb05 03c5 	add.w	r3, r5, r5, lsl #3
 8009392:	18d3      	adds	r3, r2, r3
 8009394:	9303      	str	r3, [sp, #12]
 8009396:	fa5f f885 	uxtb.w	r8, r5
 800939a:	2e00      	cmp	r6, #0
 800939c:	d169      	bne.n	8009472 <ShowPopStr+0x13a>
 800939e:	4b62      	ldr	r3, [pc, #392]	; (8009528 <ShowPopStr+0x1f0>)
 80093a0:	789b      	ldrb	r3, [r3, #2]
 80093a2:	2b04      	cmp	r3, #4
 80093a4:	bf0c      	ite	eq
 80093a6:	f10b 043d 	addeq.w	r4, fp, #61	; 0x3d
 80093aa:	f10b 0413 	addne.w	r4, fp, #19
 80093ae:	9b04      	ldr	r3, [sp, #16]
 80093b0:	b2a4      	uxth	r4, r4
 80093b2:	4543      	cmp	r3, r8
 80093b4:	d166      	bne.n	8009484 <ShowPopStr+0x14c>
 80093b6:	2100      	movs	r1, #0
 80093b8:	f64b 50e0 	movw	r0, #48608	; 0xbde0
 80093bc:	f002 fe5b 	bl	800c076 <Set_Color>
 80093c0:	2301      	movs	r3, #1
 80093c2:	2200      	movs	r2, #0
 80093c4:	4621      	mov	r1, r4
 80093c6:	4638      	mov	r0, r7
 80093c8:	f8cd 9000 	str.w	r9, [sp]
 80093cc:	f002 fe5a 	bl	800c084 <Show_Str6x8>
 80093d0:	220a      	movs	r2, #10
 80093d2:	4b55      	ldr	r3, [pc, #340]	; (8009528 <ShowPopStr+0x1f0>)
 80093d4:	9904      	ldr	r1, [sp, #16]
 80093d6:	789b      	ldrb	r3, [r3, #2]
 80093d8:	f64b 50e0 	movw	r0, #48608	; 0xbde0
 80093dc:	fb02 1303 	mla	r3, r2, r3, r1
 80093e0:	4a52      	ldr	r2, [pc, #328]	; (800952c <ShowPopStr+0x1f4>)
 80093e2:	4953      	ldr	r1, [pc, #332]	; (8009530 <ShowPopStr+0x1f8>)
 80093e4:	eb02 0243 	add.w	r2, r2, r3, lsl #1
 80093e8:	f831 1013 	ldrh.w	r1, [r1, r3, lsl #1]
 80093ec:	f8b2 3080 	ldrh.w	r3, [r2, #128]	; 0x80
 80093f0:	4299      	cmp	r1, r3
 80093f2:	bf0c      	ite	eq
 80093f4:	f643 11e7 	movweq	r1, #14823	; 0x39e7
 80093f8:	2100      	movne	r1, #0
 80093fa:	f002 fe3c 	bl	800c076 <Set_Color>
 80093fe:	4b4d      	ldr	r3, [pc, #308]	; (8009534 <ShowPopStr+0x1fc>)
 8009400:	2200      	movs	r2, #0
 8009402:	9300      	str	r3, [sp, #0]
 8009404:	4621      	mov	r1, r4
 8009406:	2301      	movs	r3, #1
 8009408:	9805      	ldr	r0, [sp, #20]
 800940a:	f002 fe3b 	bl	800c084 <Show_Str6x8>
 800940e:	2100      	movs	r1, #0
 8009410:	f64b 50e0 	movw	r0, #48608	; 0xbde0
 8009414:	f002 fe2f 	bl	800c076 <Set_Color>
 8009418:	9b03      	ldr	r3, [sp, #12]
 800941a:	4621      	mov	r1, r4
 800941c:	9300      	str	r3, [sp, #0]
 800941e:	2200      	movs	r2, #0
 8009420:	2301      	movs	r3, #1
 8009422:	4650      	mov	r0, sl
 8009424:	f002 fe2e 	bl	800c084 <Show_Str6x8>
 8009428:	220a      	movs	r2, #10
 800942a:	4b3f      	ldr	r3, [pc, #252]	; (8009528 <ShowPopStr+0x1f0>)
 800942c:	f64b 50e0 	movw	r0, #48608	; 0xbde0
 8009430:	789b      	ldrb	r3, [r3, #2]
 8009432:	fb02 8803 	mla	r8, r2, r3, r8
 8009436:	4b3d      	ldr	r3, [pc, #244]	; (800952c <ShowPopStr+0x1f4>)
 8009438:	4a3d      	ldr	r2, [pc, #244]	; (8009530 <ShowPopStr+0x1f8>)
 800943a:	eb03 0348 	add.w	r3, r3, r8, lsl #1
 800943e:	f832 2018 	ldrh.w	r2, [r2, r8, lsl #1]
 8009442:	f8b3 310c 	ldrh.w	r3, [r3, #268]	; 0x10c
 8009446:	429a      	cmp	r2, r3
 8009448:	bf0c      	ite	eq
 800944a:	f643 11e7 	movweq	r1, #14823	; 0x39e7
 800944e:	2100      	movne	r1, #0
 8009450:	f002 fe11 	bl	800c076 <Set_Color>
 8009454:	4b38      	ldr	r3, [pc, #224]	; (8009538 <ShowPopStr+0x200>)
 8009456:	2200      	movs	r2, #0
 8009458:	9300      	str	r3, [sp, #0]
 800945a:	4621      	mov	r1, r4
 800945c:	2301      	movs	r3, #1
 800945e:	9802      	ldr	r0, [sp, #8]
 8009460:	3501      	adds	r5, #1
 8009462:	f002 fe0f 	bl	800c084 <Show_Str6x8>
 8009466:	b2eb      	uxtb	r3, r5
 8009468:	42b3      	cmp	r3, r6
 800946a:	d98a      	bls.n	8009382 <ShowPopStr+0x4a>
 800946c:	b007      	add	sp, #28
 800946e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8009472:	eba6 0408 	sub.w	r4, r6, r8
 8009476:	ebc4 04c4 	rsb	r4, r4, r4, lsl #3
 800947a:	f10b 0305 	add.w	r3, fp, #5
 800947e:	eb03 0444 	add.w	r4, r3, r4, lsl #1
 8009482:	e794      	b.n	80093ae <ShowPopStr+0x76>
 8009484:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8009488:	f643 10e7 	movw	r0, #14823	; 0x39e7
 800948c:	f002 fdf3 	bl	800c076 <Set_Color>
 8009490:	2301      	movs	r3, #1
 8009492:	2200      	movs	r2, #0
 8009494:	4621      	mov	r1, r4
 8009496:	4638      	mov	r0, r7
 8009498:	f8cd 9000 	str.w	r9, [sp]
 800949c:	f002 fdf2 	bl	800c084 <Show_Str6x8>
 80094a0:	220a      	movs	r2, #10
 80094a2:	4b21      	ldr	r3, [pc, #132]	; (8009528 <ShowPopStr+0x1f0>)
 80094a4:	4922      	ldr	r1, [pc, #136]	; (8009530 <ShowPopStr+0x1f8>)
 80094a6:	789b      	ldrb	r3, [r3, #2]
 80094a8:	f643 10e7 	movw	r0, #14823	; 0x39e7
 80094ac:	fb02 8303 	mla	r3, r2, r3, r8
 80094b0:	4a1e      	ldr	r2, [pc, #120]	; (800952c <ShowPopStr+0x1f4>)
 80094b2:	f831 1013 	ldrh.w	r1, [r1, r3, lsl #1]
 80094b6:	eb02 0243 	add.w	r2, r2, r3, lsl #1
 80094ba:	f8b2 3080 	ldrh.w	r3, [r2, #128]	; 0x80
 80094be:	4299      	cmp	r1, r3
 80094c0:	bf0c      	ite	eq
 80094c2:	f647 31ef 	movweq	r1, #31727	; 0x7bef
 80094c6:	f64f 71ff 	movwne	r1, #65535	; 0xffff
 80094ca:	f002 fdd4 	bl	800c076 <Set_Color>
 80094ce:	4b19      	ldr	r3, [pc, #100]	; (8009534 <ShowPopStr+0x1fc>)
 80094d0:	2200      	movs	r2, #0
 80094d2:	9300      	str	r3, [sp, #0]
 80094d4:	4621      	mov	r1, r4
 80094d6:	2301      	movs	r3, #1
 80094d8:	9805      	ldr	r0, [sp, #20]
 80094da:	f002 fdd3 	bl	800c084 <Show_Str6x8>
 80094de:	f64f 71ff 	movw	r1, #65535	; 0xffff
 80094e2:	f643 10e7 	movw	r0, #14823	; 0x39e7
 80094e6:	f002 fdc6 	bl	800c076 <Set_Color>
 80094ea:	9b03      	ldr	r3, [sp, #12]
 80094ec:	4621      	mov	r1, r4
 80094ee:	9300      	str	r3, [sp, #0]
 80094f0:	2200      	movs	r2, #0
 80094f2:	2301      	movs	r3, #1
 80094f4:	4650      	mov	r0, sl
 80094f6:	f002 fdc5 	bl	800c084 <Show_Str6x8>
 80094fa:	220a      	movs	r2, #10
 80094fc:	4b0a      	ldr	r3, [pc, #40]	; (8009528 <ShowPopStr+0x1f0>)
 80094fe:	f643 10e7 	movw	r0, #14823	; 0x39e7
 8009502:	789b      	ldrb	r3, [r3, #2]
 8009504:	fb02 8803 	mla	r8, r2, r3, r8
 8009508:	4b08      	ldr	r3, [pc, #32]	; (800952c <ShowPopStr+0x1f4>)
 800950a:	4a09      	ldr	r2, [pc, #36]	; (8009530 <ShowPopStr+0x1f8>)
 800950c:	eb03 0348 	add.w	r3, r3, r8, lsl #1
 8009510:	f832 2018 	ldrh.w	r2, [r2, r8, lsl #1]
 8009514:	f8b3 310c 	ldrh.w	r3, [r3, #268]	; 0x10c
 8009518:	429a      	cmp	r2, r3
 800951a:	bf0c      	ite	eq
 800951c:	f647 31ef 	movweq	r1, #31727	; 0x7bef
 8009520:	f64f 71ff 	movwne	r1, #65535	; 0xffff
 8009524:	e794      	b.n	8009450 <ShowPopStr+0x118>
 8009526:	bf00      	nop
 8009528:	20004c12 	.word	0x20004c12
 800952c:	08012c0c 	.word	0x08012c0c
 8009530:	2000000c 	.word	0x2000000c
 8009534:	0801383d 	.word	0x0801383d
 8009538:	0801383f 	.word	0x0801383f

0800953c <DrawDottedLine>:
 800953c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8009540:	b085      	sub	sp, #20
 8009542:	9301      	str	r3, [sp, #4]
 8009544:	1883      	adds	r3, r0, r2
 8009546:	f5b3 7fa0 	cmp.w	r3, #320	; 0x140
 800954a:	4680      	mov	r8, r0
 800954c:	4689      	mov	r9, r1
 800954e:	4614      	mov	r4, r2
 8009550:	f8bd a038 	ldrh.w	sl, [sp, #56]	; 0x38
 8009554:	dc56      	bgt.n	8009604 <DrawDottedLine+0xc8>
 8009556:	9b01      	ldr	r3, [sp, #4]
 8009558:	18cb      	adds	r3, r1, r3
 800955a:	2bf0      	cmp	r3, #240	; 0xf0
 800955c:	dc52      	bgt.n	8009604 <DrawDottedLine+0xc8>
 800955e:	2600      	movs	r6, #0
 8009560:	fa1f fb86 	uxth.w	fp, r6
 8009564:	f10b 01d8 	add.w	r1, fp, #216	; 0xd8
 8009568:	b28b      	uxth	r3, r1
 800956a:	0072      	lsls	r2, r6, #1
 800956c:	2500      	movs	r5, #0
 800956e:	9302      	str	r3, [sp, #8]
 8009570:	e01c      	b.n	80095ac <DrawDottedLine+0x70>
 8009572:	eba8 0106 	sub.w	r1, r8, r6
 8009576:	4429      	add	r1, r5
 8009578:	2927      	cmp	r1, #39	; 0x27
 800957a:	dd15      	ble.n	80095a8 <DrawDottedLine+0x6c>
 800957c:	eba8 070b 	sub.w	r7, r8, fp
 8009580:	442f      	add	r7, r5
 8009582:	b2bf      	uxth	r7, r7
 8009584:	9902      	ldr	r1, [sp, #8]
 8009586:	4638      	mov	r0, r7
 8009588:	9203      	str	r2, [sp, #12]
 800958a:	f002 fad3 	bl	800bb34 <Set_Posi>
 800958e:	4650      	mov	r0, sl
 8009590:	f002 fadc 	bl	800bb4c <Set_Pixel>
 8009594:	f1cb 0145 	rsb	r1, fp, #69	; 0x45
 8009598:	b289      	uxth	r1, r1
 800959a:	4638      	mov	r0, r7
 800959c:	f002 faca 	bl	800bb34 <Set_Posi>
 80095a0:	4650      	mov	r0, sl
 80095a2:	f002 fad3 	bl	800bb4c <Set_Pixel>
 80095a6:	9a03      	ldr	r2, [sp, #12]
 80095a8:	3501      	adds	r5, #1
 80095aa:	b2ad      	uxth	r5, r5
 80095ac:	4295      	cmp	r5, r2
 80095ae:	dde0      	ble.n	8009572 <DrawDottedLine+0x36>
 80095b0:	3601      	adds	r6, #1
 80095b2:	2e04      	cmp	r6, #4
 80095b4:	d1d4      	bne.n	8009560 <DrawDottedLine+0x24>
 80095b6:	3c33      	subs	r4, #51	; 0x33
 80095b8:	f1a8 0533 	sub.w	r5, r8, #51	; 0x33
 80095bc:	44a0      	add	r8, r4
 80095be:	b2ad      	uxth	r5, r5
 80095c0:	fa1f f888 	uxth.w	r8, r8
 80095c4:	f105 0433 	add.w	r4, r5, #51	; 0x33
 80095c8:	4545      	cmp	r5, r8
 80095ca:	b2a4      	uxth	r4, r4
 80095cc:	d01e      	beq.n	800960c <DrawDottedLine+0xd0>
 80095ce:	2600      	movs	r6, #0
 80095d0:	e011      	b.n	80095f6 <DrawDottedLine+0xba>
 80095d2:	eb07 0109 	add.w	r1, r7, r9
 80095d6:	b289      	uxth	r1, r1
 80095d8:	4620      	mov	r0, r4
 80095da:	f002 faab 	bl	800bb34 <Set_Posi>
 80095de:	07bb      	lsls	r3, r7, #30
 80095e0:	d108      	bne.n	80095f4 <DrawDottedLine+0xb8>
 80095e2:	2d7c      	cmp	r5, #124	; 0x7c
 80095e4:	d803      	bhi.n	80095ee <DrawDottedLine+0xb2>
 80095e6:	eb09 0306 	add.w	r3, r9, r6
 80095ea:	2b94      	cmp	r3, #148	; 0x94
 80095ec:	dc02      	bgt.n	80095f4 <DrawDottedLine+0xb8>
 80095ee:	4650      	mov	r0, sl
 80095f0:	f002 faac 	bl	800bb4c <Set_Pixel>
 80095f4:	3601      	adds	r6, #1
 80095f6:	9b01      	ldr	r3, [sp, #4]
 80095f8:	b2b7      	uxth	r7, r6
 80095fa:	429e      	cmp	r6, r3
 80095fc:	d1e9      	bne.n	80095d2 <DrawDottedLine+0x96>
 80095fe:	3501      	adds	r5, #1
 8009600:	b2ad      	uxth	r5, r5
 8009602:	e7df      	b.n	80095c4 <DrawDottedLine+0x88>
 8009604:	2000      	movs	r0, #0
 8009606:	b005      	add	sp, #20
 8009608:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800960c:	2001      	movs	r0, #1
 800960e:	e7fa      	b.n	8009606 <DrawDottedLine+0xca>

08009610 <ClrCursor>:
 8009610:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8009614:	4682      	mov	sl, r0
 8009616:	4691      	mov	r9, r2
 8009618:	2500      	movs	r5, #0
 800961a:	fa1f f885 	uxth.w	r8, r5
 800961e:	f108 07d8 	add.w	r7, r8, #216	; 0xd8
 8009622:	ea4f 0b45 	mov.w	fp, r5, lsl #1
 8009626:	2400      	movs	r4, #0
 8009628:	b2bf      	uxth	r7, r7
 800962a:	e01a      	b.n	8009662 <ClrCursor+0x52>
 800962c:	ebaa 0305 	sub.w	r3, sl, r5
 8009630:	4423      	add	r3, r4
 8009632:	2b27      	cmp	r3, #39	; 0x27
 8009634:	dd13      	ble.n	800965e <ClrCursor+0x4e>
 8009636:	ebaa 0608 	sub.w	r6, sl, r8
 800963a:	4426      	add	r6, r4
 800963c:	b2b6      	uxth	r6, r6
 800963e:	4639      	mov	r1, r7
 8009640:	4630      	mov	r0, r6
 8009642:	f002 fa77 	bl	800bb34 <Set_Posi>
 8009646:	4648      	mov	r0, r9
 8009648:	f002 fa80 	bl	800bb4c <Set_Pixel>
 800964c:	f1c8 0145 	rsb	r1, r8, #69	; 0x45
 8009650:	4630      	mov	r0, r6
 8009652:	b289      	uxth	r1, r1
 8009654:	f002 fa6e 	bl	800bb34 <Set_Posi>
 8009658:	4648      	mov	r0, r9
 800965a:	f002 fa77 	bl	800bb4c <Set_Pixel>
 800965e:	3401      	adds	r4, #1
 8009660:	b2a4      	uxth	r4, r4
 8009662:	455c      	cmp	r4, fp
 8009664:	dde2      	ble.n	800962c <ClrCursor+0x1c>
 8009666:	3501      	adds	r5, #1
 8009668:	2d04      	cmp	r5, #4
 800966a:	d1d6      	bne.n	800961a <ClrCursor+0xa>
 800966c:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}

08009670 <Draw_ScaleRule>:
 8009670:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8009672:	2400      	movs	r4, #0
 8009674:	2532      	movs	r5, #50	; 0x32
 8009676:	2619      	movs	r6, #25
 8009678:	2705      	movs	r7, #5
 800967a:	b2a0      	uxth	r0, r4
 800967c:	fbb0 f3f5 	udiv	r3, r0, r5
 8009680:	fb05 0313 	mls	r3, r5, r3, r0
 8009684:	b29b      	uxth	r3, r3
 8009686:	b9eb      	cbnz	r3, 80096c4 <Draw_ScaleRule+0x54>
 8009688:	3028      	adds	r0, #40	; 0x28
 800968a:	2142      	movs	r1, #66	; 0x42
 800968c:	b280      	uxth	r0, r0
 800968e:	f002 fa51 	bl	800bb34 <Set_Posi>
 8009692:	f64f 70ff 	movw	r0, #65535	; 0xffff
 8009696:	f002 fa59 	bl	800bb4c <Set_Pixel>
 800969a:	f64f 70ff 	movw	r0, #65535	; 0xffff
 800969e:	f002 fa55 	bl	800bb4c <Set_Pixel>
 80096a2:	f64f 70ff 	movw	r0, #65535	; 0xffff
 80096a6:	f002 fa51 	bl	800bb4c <Set_Pixel>
 80096aa:	f64f 70ff 	movw	r0, #65535	; 0xffff
 80096ae:	f002 fa4d 	bl	800bb4c <Set_Pixel>
 80096b2:	f64f 70ff 	movw	r0, #65535	; 0xffff
 80096b6:	f002 fa49 	bl	800bb4c <Set_Pixel>
 80096ba:	3401      	adds	r4, #1
 80096bc:	f5b4 7f8c 	cmp.w	r4, #280	; 0x118
 80096c0:	d1db      	bne.n	800967a <Draw_ScaleRule+0xa>
 80096c2:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 80096c4:	fbb0 f3f6 	udiv	r3, r0, r6
 80096c8:	fb06 0313 	mls	r3, r6, r3, r0
 80096cc:	b29b      	uxth	r3, r3
 80096ce:	b92b      	cbnz	r3, 80096dc <Draw_ScaleRule+0x6c>
 80096d0:	3028      	adds	r0, #40	; 0x28
 80096d2:	2142      	movs	r1, #66	; 0x42
 80096d4:	b280      	uxth	r0, r0
 80096d6:	f002 fa2d 	bl	800bb34 <Set_Posi>
 80096da:	e7e2      	b.n	80096a2 <Draw_ScaleRule+0x32>
 80096dc:	fbb0 f3f7 	udiv	r3, r0, r7
 80096e0:	eb03 0383 	add.w	r3, r3, r3, lsl #2
 80096e4:	1ac3      	subs	r3, r0, r3
 80096e6:	b29b      	uxth	r3, r3
 80096e8:	2b00      	cmp	r3, #0
 80096ea:	d1e6      	bne.n	80096ba <Draw_ScaleRule+0x4a>
 80096ec:	3028      	adds	r0, #40	; 0x28
 80096ee:	2142      	movs	r1, #66	; 0x42
 80096f0:	b280      	uxth	r0, r0
 80096f2:	f002 fa1f 	bl	800bb34 <Set_Posi>
 80096f6:	e7d8      	b.n	80096aa <Draw_ScaleRule+0x3a>

080096f8 <DrawCursor>:
 80096f8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 80096fc:	4605      	mov	r5, r0
 80096fe:	460e      	mov	r6, r1
 8009700:	4690      	mov	r8, r2
 8009702:	2700      	movs	r7, #0
 8009704:	2400      	movs	r4, #0
 8009706:	e009      	b.n	800971c <DrawCursor+0x24>
 8009708:	1960      	adds	r0, r4, r5
 800970a:	4631      	mov	r1, r6
 800970c:	b280      	uxth	r0, r0
 800970e:	f002 fa11 	bl	800bb34 <Set_Posi>
 8009712:	4640      	mov	r0, r8
 8009714:	f002 fa1a 	bl	800bb4c <Set_Pixel>
 8009718:	3401      	adds	r4, #1
 800971a:	b2a4      	uxth	r4, r4
 800971c:	42a7      	cmp	r7, r4
 800971e:	daf3      	bge.n	8009708 <DrawCursor+0x10>
 8009720:	3702      	adds	r7, #2
 8009722:	3d01      	subs	r5, #1
 8009724:	3601      	adds	r6, #1
 8009726:	2f06      	cmp	r7, #6
 8009728:	b2ad      	uxth	r5, r5
 800972a:	b2b6      	uxth	r6, r6
 800972c:	d1ea      	bne.n	8009704 <DrawCursor+0xc>
 800972e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}

08009732 <DrawRectangle>:
 8009732:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 8009736:	4402      	add	r2, r0
 8009738:	f5b2 7fa0 	cmp.w	r2, #320	; 0x140
 800973c:	460e      	mov	r6, r1
 800973e:	461f      	mov	r7, r3
 8009740:	f8bd 8020 	ldrh.w	r8, [sp, #32]
 8009744:	dc19      	bgt.n	800977a <DrawRectangle+0x48>
 8009746:	18cb      	adds	r3, r1, r3
 8009748:	2bf0      	cmp	r3, #240	; 0xf0
 800974a:	dc16      	bgt.n	800977a <DrawRectangle+0x48>
 800974c:	b294      	uxth	r4, r2
 800974e:	4284      	cmp	r4, r0
 8009750:	d102      	bne.n	8009758 <DrawRectangle+0x26>
 8009752:	2001      	movs	r0, #1
 8009754:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8009758:	4631      	mov	r1, r6
 800975a:	1c45      	adds	r5, r0, #1
 800975c:	f04f 0900 	mov.w	r9, #0
 8009760:	f002 f9e8 	bl	800bb34 <Set_Posi>
 8009764:	b2ad      	uxth	r5, r5
 8009766:	454f      	cmp	r7, r9
 8009768:	d101      	bne.n	800976e <DrawRectangle+0x3c>
 800976a:	4628      	mov	r0, r5
 800976c:	e7ef      	b.n	800974e <DrawRectangle+0x1c>
 800976e:	4640      	mov	r0, r8
 8009770:	f002 f9ec 	bl	800bb4c <Set_Pixel>
 8009774:	f109 0901 	add.w	r9, r9, #1
 8009778:	e7f5      	b.n	8009766 <DrawRectangle+0x34>
 800977a:	2000      	movs	r0, #0
 800977c:	e7ea      	b.n	8009754 <DrawRectangle+0x22>

0800977e <ShowLeftTitle>:
 800977e:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8009782:	2600      	movs	r6, #0
 8009784:	24c3      	movs	r4, #195	; 0xc3
 8009786:	46b0      	mov	r8, r6
 8009788:	f8df a0d0 	ldr.w	sl, [pc, #208]	; 800985c <ShowLeftTitle+0xde>
 800978c:	f8df 90d0 	ldr.w	r9, [pc, #208]	; 8009860 <ShowLeftTitle+0xe2>
 8009790:	b088      	sub	sp, #32
 8009792:	f647 33ef 	movw	r3, #31727	; 0x7bef
 8009796:	4621      	mov	r1, r4
 8009798:	1c77      	adds	r7, r6, #1
 800979a:	9300      	str	r3, [sp, #0]
 800979c:	2226      	movs	r2, #38	; 0x26
 800979e:	2318      	movs	r3, #24
 80097a0:	ad05      	add	r5, sp, #20
 80097a2:	2000      	movs	r0, #0
 80097a4:	f7ff ffc5 	bl	8009732 <DrawRectangle>
 80097a8:	463a      	mov	r2, r7
 80097aa:	4651      	mov	r1, sl
 80097ac:	4628      	mov	r0, r5
 80097ae:	f006 f885 	bl	800f8bc <sprintf>
 80097b2:	2200      	movs	r2, #0
 80097b4:	f104 010d 	add.w	r1, r4, #13
 80097b8:	f839 3016 	ldrh.w	r3, [r9, r6, lsl #1]
 80097bc:	e9cd 8501 	strd	r8, r5, [sp, #4]
 80097c0:	f8cd 8000 	str.w	r8, [sp]
 80097c4:	b289      	uxth	r1, r1
 80097c6:	4610      	mov	r0, r2
 80097c8:	f002 fb62 	bl	800be90 <Disp_Str7x9>
 80097cc:	3c19      	subs	r4, #25
 80097ce:	2f06      	cmp	r7, #6
 80097d0:	b2a4      	uxth	r4, r4
 80097d2:	463e      	mov	r6, r7
 80097d4:	d1dd      	bne.n	8009792 <ShowLeftTitle+0x14>
 80097d6:	24c4      	movs	r4, #196	; 0xc4
 80097d8:	2700      	movs	r7, #0
 80097da:	f8df 8088 	ldr.w	r8, [pc, #136]	; 8009864 <ShowLeftTitle+0xe6>
 80097de:	f8df 9088 	ldr.w	r9, [pc, #136]	; 8009868 <ShowLeftTitle+0xea>
 80097e2:	f64f 71ff 	movw	r1, #65535	; 0xffff
 80097e6:	f647 30ef 	movw	r0, #31727	; 0x7bef
 80097ea:	f002 fc44 	bl	800c076 <Set_Color>
 80097ee:	2206      	movs	r2, #6
 80097f0:	2600      	movs	r6, #0
 80097f2:	f8b9 3000 	ldrh.w	r3, [r9]
 80097f6:	f8cd 601a 	str.w	r6, [sp, #26]
 80097fa:	009b      	lsls	r3, r3, #2
 80097fc:	fb02 7303 	mla	r3, r2, r3, r7
 8009800:	4443      	add	r3, r8
 8009802:	6818      	ldr	r0, [r3, #0]
 8009804:	889b      	ldrh	r3, [r3, #4]
 8009806:	4621      	mov	r1, r4
 8009808:	80ab      	strh	r3, [r5, #4]
 800980a:	2301      	movs	r3, #1
 800980c:	3c19      	subs	r4, #25
 800980e:	6028      	str	r0, [r5, #0]
 8009810:	4632      	mov	r2, r6
 8009812:	9500      	str	r5, [sp, #0]
 8009814:	4618      	mov	r0, r3
 8009816:	b2a4      	uxth	r4, r4
 8009818:	f002 fc34 	bl	800c084 <Show_Str6x8>
 800981c:	2c60      	cmp	r4, #96	; 0x60
 800981e:	f107 0706 	add.w	r7, r7, #6
 8009822:	d1de      	bne.n	80097e2 <ShowLeftTitle+0x64>
 8009824:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8009828:	f647 30ef 	movw	r0, #31727	; 0x7bef
 800982c:	f002 fc23 	bl	800c076 <Set_Color>
 8009830:	2301      	movs	r3, #1
 8009832:	4d09      	ldr	r5, [pc, #36]	; (8009858 <ShowLeftTitle+0xda>)
 8009834:	4632      	mov	r2, r6
 8009836:	4621      	mov	r1, r4
 8009838:	4618      	mov	r0, r3
 800983a:	9500      	str	r5, [sp, #0]
 800983c:	f002 fc22 	bl	800c084 <Show_Str6x8>
 8009840:	2301      	movs	r3, #1
 8009842:	4632      	mov	r2, r6
 8009844:	2147      	movs	r1, #71	; 0x47
 8009846:	4618      	mov	r0, r3
 8009848:	9500      	str	r5, [sp, #0]
 800984a:	f002 fc1b 	bl	800c084 <Show_Str6x8>
 800984e:	f7ff faca 	bl	8008de6 <DrawTriggerIco>
 8009852:	b008      	add	sp, #32
 8009854:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8009858:	08013845 	.word	0x08013845
 800985c:	08013841 	.word	0x08013841
 8009860:	08012da4 	.word	0x08012da4
 8009864:	08012db0 	.word	0x08012db0
 8009868:	2000000c 	.word	0x2000000c

0800986c <ShowSubPop>:
 800986c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8009870:	1ac9      	subs	r1, r1, r3
 8009872:	461c      	mov	r4, r3
 8009874:	4b41      	ldr	r3, [pc, #260]	; (800997c <ShowSubPop+0x110>)
 8009876:	b289      	uxth	r1, r1
 8009878:	781b      	ldrb	r3, [r3, #0]
 800987a:	b087      	sub	sp, #28
 800987c:	1f0d      	subs	r5, r1, #4
 800987e:	4606      	mov	r6, r0
 8009880:	e9dd 9a12 	ldrd	r9, sl, [sp, #72]	; 0x48
 8009884:	4617      	mov	r7, r2
 8009886:	f8bd 8040 	ldrh.w	r8, [sp, #64]	; 0x40
 800988a:	f89d b050 	ldrb.w	fp, [sp, #80]	; 0x50
 800988e:	b2ad      	uxth	r5, r5
 8009890:	b38b      	cbz	r3, 80098f6 <ShowSubPop+0x8a>
 8009892:	f8bd c044 	ldrh.w	ip, [sp, #68]	; 0x44
 8009896:	1ea3      	subs	r3, r4, #2
 8009898:	3a02      	subs	r2, #2
 800989a:	3903      	subs	r1, #3
 800989c:	3001      	adds	r0, #1
 800989e:	f8cd c000 	str.w	ip, [sp]
 80098a2:	b29b      	uxth	r3, r3
 80098a4:	b292      	uxth	r2, r2
 80098a6:	b289      	uxth	r1, r1
 80098a8:	b280      	uxth	r0, r0
 80098aa:	f7ff ff42 	bl	8009732 <DrawRectangle>
 80098ae:	4623      	mov	r3, r4
 80098b0:	2201      	movs	r2, #1
 80098b2:	4629      	mov	r1, r5
 80098b4:	f8cd 8000 	str.w	r8, [sp]
 80098b8:	4630      	mov	r0, r6
 80098ba:	f7ff ff3a 	bl	8009732 <DrawRectangle>
 80098be:	1e78      	subs	r0, r7, #1
 80098c0:	4430      	add	r0, r6
 80098c2:	4623      	mov	r3, r4
 80098c4:	2201      	movs	r2, #1
 80098c6:	4629      	mov	r1, r5
 80098c8:	f8cd 8000 	str.w	r8, [sp]
 80098cc:	b280      	uxth	r0, r0
 80098ce:	f7ff ff30 	bl	8009732 <DrawRectangle>
 80098d2:	2301      	movs	r3, #1
 80098d4:	463a      	mov	r2, r7
 80098d6:	4629      	mov	r1, r5
 80098d8:	f8cd 8000 	str.w	r8, [sp]
 80098dc:	4630      	mov	r0, r6
 80098de:	f7ff ff28 	bl	8009732 <DrawRectangle>
 80098e2:	1e61      	subs	r1, r4, #1
 80098e4:	4429      	add	r1, r5
 80098e6:	f8cd 8000 	str.w	r8, [sp]
 80098ea:	2305      	movs	r3, #5
 80098ec:	463a      	mov	r2, r7
 80098ee:	b289      	uxth	r1, r1
 80098f0:	4630      	mov	r0, r6
 80098f2:	f7ff ff1e 	bl	8009732 <DrawRectangle>
 80098f6:	4b22      	ldr	r3, [pc, #136]	; (8009980 <ShowSubPop+0x114>)
 80098f8:	789b      	ldrb	r3, [r3, #2]
 80098fa:	b123      	cbz	r3, 8009906 <ShowSubPop+0x9a>
 80098fc:	2b04      	cmp	r3, #4
 80098fe:	d01f      	beq.n	8009940 <ShowSubPop+0xd4>
 8009900:	b007      	add	sp, #28
 8009902:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8009906:	4b1f      	ldr	r3, [pc, #124]	; (8009984 <ShowSubPop+0x118>)
 8009908:	8818      	ldrh	r0, [r3, #0]
 800990a:	2800      	cmp	r0, #0
 800990c:	d0f8      	beq.n	8009900 <ShowSubPop+0x94>
 800990e:	4b1e      	ldr	r3, [pc, #120]	; (8009988 <ShowSubPop+0x11c>)
 8009910:	791a      	ldrb	r2, [r3, #4]
 8009912:	0091      	lsls	r1, r2, #2
 8009914:	4a1d      	ldr	r2, [pc, #116]	; (800998c <ShowSubPop+0x120>)
 8009916:	440b      	add	r3, r1
 8009918:	440a      	add	r2, r1
 800991a:	5c1b      	ldrb	r3, [r3, r0]
 800991c:	2110      	movs	r1, #16
 800991e:	4402      	add	r2, r0
 8009920:	2009      	movs	r0, #9
 8009922:	e9cd a101 	strd	sl, r1, [sp, #4]
 8009926:	e9cd 0103 	strd	r0, r1, [sp, #12]
 800992a:	f892 2264 	ldrb.w	r2, [r2, #612]	; 0x264
 800992e:	f8cd b014 	str.w	fp, [sp, #20]
 8009932:	f8cd 9000 	str.w	r9, [sp]
 8009936:	4629      	mov	r1, r5
 8009938:	4630      	mov	r0, r6
 800993a:	f7ff fadd 	bl	8008ef8 <ShowSubPopStr>
 800993e:	e7df      	b.n	8009900 <ShowSubPop+0x94>
 8009940:	4b10      	ldr	r3, [pc, #64]	; (8009984 <ShowSubPop+0x118>)
 8009942:	f8b3 3050 	ldrh.w	r3, [r3, #80]	; 0x50
 8009946:	2b00      	cmp	r3, #0
 8009948:	d1da      	bne.n	8009900 <ShowSubPop+0x94>
 800994a:	4b0f      	ldr	r3, [pc, #60]	; (8009988 <ShowSubPop+0x11c>)
 800994c:	490f      	ldr	r1, [pc, #60]	; (800998c <ShowSubPop+0x120>)
 800994e:	7a1a      	ldrb	r2, [r3, #8]
 8009950:	2009      	movs	r0, #9
 8009952:	0092      	lsls	r2, r2, #2
 8009954:	4413      	add	r3, r2
 8009956:	440a      	add	r2, r1
 8009958:	2110      	movs	r1, #16
 800995a:	7adb      	ldrb	r3, [r3, #11]
 800995c:	f892 2268 	ldrb.w	r2, [r2, #616]	; 0x268
 8009960:	e9cd 0103 	strd	r0, r1, [sp, #12]
 8009964:	e9cd a101 	strd	sl, r1, [sp, #4]
 8009968:	f8cd b014 	str.w	fp, [sp, #20]
 800996c:	f8cd 9000 	str.w	r9, [sp]
 8009970:	4629      	mov	r1, r5
 8009972:	4630      	mov	r0, r6
 8009974:	f7ff fbcc 	bl	8009110 <ShowOutSubPopStr>
 8009978:	e7c2      	b.n	8009900 <ShowSubPop+0x94>
 800997a:	bf00      	nop
 800997c:	20004c0f 	.word	0x20004c0f
 8009980:	20004c12 	.word	0x20004c12
 8009984:	2000000c 	.word	0x2000000c
 8009988:	20000240 	.word	0x20000240
 800998c:	08012c0c 	.word	0x08012c0c

08009990 <ShowSubItem>:
 8009990:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8009994:	b0a0      	sub	sp, #128	; 0x80
 8009996:	ae09      	add	r6, sp, #36	; 0x24
 8009998:	4605      	mov	r5, r0
 800999a:	460f      	mov	r7, r1
 800999c:	225a      	movs	r2, #90	; 0x5a
 800999e:	2100      	movs	r1, #0
 80099a0:	4630      	mov	r0, r6
 80099a2:	f009 f8ca 	bl	8012b3a <memset>
 80099a6:	2300      	movs	r3, #0
 80099a8:	ac06      	add	r4, sp, #24
 80099aa:	9306      	str	r3, [sp, #24]
 80099ac:	6063      	str	r3, [r4, #4]
 80099ae:	8123      	strh	r3, [r4, #8]
 80099b0:	b12d      	cbz	r5, 80099be <ShowSubItem+0x2e>
 80099b2:	2d04      	cmp	r5, #4
 80099b4:	f000 815c 	beq.w	8009c70 <ShowSubItem+0x2e0>
 80099b8:	b020      	add	sp, #128	; 0x80
 80099ba:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 80099be:	2f00      	cmp	r7, #0
 80099c0:	d1fa      	bne.n	80099b8 <ShowSubItem+0x28>
 80099c2:	4d9b      	ldr	r5, [pc, #620]	; (8009c30 <ShowSubItem+0x2a0>)
 80099c4:	882b      	ldrh	r3, [r5, #0]
 80099c6:	2b03      	cmp	r3, #3
 80099c8:	d8f6      	bhi.n	80099b8 <ShowSubItem+0x28>
 80099ca:	e8df f003 	tbb	[pc, r3]
 80099ce:	2402      	.short	0x2402
 80099d0:	b688      	.short	0xb688
 80099d2:	4b98      	ldr	r3, [pc, #608]	; (8009c34 <ShowSubItem+0x2a4>)
 80099d4:	4a98      	ldr	r2, [pc, #608]	; (8009c38 <ShowSubItem+0x2a8>)
 80099d6:	7899      	ldrb	r1, [r3, #2]
 80099d8:	4b98      	ldr	r3, [pc, #608]	; (8009c3c <ShowSubItem+0x2ac>)
 80099da:	440b      	add	r3, r1
 80099dc:	2105      	movs	r1, #5
 80099de:	791b      	ldrb	r3, [r3, #4]
 80099e0:	e9cd 6103 	strd	r6, r1, [sp, #12]
 80099e4:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 80099e8:	f893 3264 	ldrb.w	r3, [r3, #612]	; 0x264
 80099ec:	f502 721b 	add.w	r2, r2, #620	; 0x26c
 80099f0:	3301      	adds	r3, #1
 80099f2:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 80099f6:	005b      	lsls	r3, r3, #1
 80099f8:	3306      	adds	r3, #6
 80099fa:	b29b      	uxth	r3, r3
 80099fc:	9202      	str	r2, [sp, #8]
 80099fe:	f643 14e7 	movw	r4, #14823	; 0x39e7
 8009a02:	f240 72ff 	movw	r2, #2047	; 0x7ff
 8009a06:	e9cd 2400 	strd	r2, r4, [sp]
 8009a0a:	2290      	movs	r2, #144	; 0x90
 8009a0c:	21c3      	movs	r1, #195	; 0xc3
 8009a0e:	203c      	movs	r0, #60	; 0x3c
 8009a10:	f7ff ff2c 	bl	800986c <ShowSubPop>
 8009a14:	e7d0      	b.n	80099b8 <ShowSubItem+0x28>
 8009a16:	498a      	ldr	r1, [pc, #552]	; (8009c40 <ShowSubItem+0x2b0>)
 8009a18:	4b8a      	ldr	r3, [pc, #552]	; (8009c44 <ShowSubItem+0x2b4>)
 8009a1a:	f895 2094 	ldrb.w	r2, [r5, #148]	; 0x94
 8009a1e:	2a00      	cmp	r2, #0
 8009a20:	bf14      	ite	ne
 8009a22:	461a      	movne	r2, r3
 8009a24:	460a      	moveq	r2, r1
 8009a26:	6810      	ldr	r0, [r2, #0]
 8009a28:	7912      	ldrb	r2, [r2, #4]
 8009a2a:	9009      	str	r0, [sp, #36]	; 0x24
 8009a2c:	7132      	strb	r2, [r6, #4]
 8009a2e:	f895 2095 	ldrb.w	r2, [r5, #149]	; 0x95
 8009a32:	2a00      	cmp	r2, #0
 8009a34:	bf14      	ite	ne
 8009a36:	461a      	movne	r2, r3
 8009a38:	460a      	moveq	r2, r1
 8009a3a:	6810      	ldr	r0, [r2, #0]
 8009a3c:	7912      	ldrb	r2, [r2, #4]
 8009a3e:	f8cd 002d 	str.w	r0, [sp, #45]	; 0x2d
 8009a42:	f88d 2031 	strb.w	r2, [sp, #49]	; 0x31
 8009a46:	f895 2096 	ldrb.w	r2, [r5, #150]	; 0x96
 8009a4a:	4620      	mov	r0, r4
 8009a4c:	2a00      	cmp	r2, #0
 8009a4e:	bf08      	it	eq
 8009a50:	460b      	moveq	r3, r1
 8009a52:	681a      	ldr	r2, [r3, #0]
 8009a54:	791b      	ldrb	r3, [r3, #4]
 8009a56:	f895 1097 	ldrb.w	r1, [r5, #151]	; 0x97
 8009a5a:	f8cd 2036 	str.w	r2, [sp, #54]	; 0x36
 8009a5e:	f88d 303a 	strb.w	r3, [sp, #58]	; 0x3a
 8009a62:	4a79      	ldr	r2, [pc, #484]	; (8009c48 <ShowSubItem+0x2b8>)
 8009a64:	4b79      	ldr	r3, [pc, #484]	; (8009c4c <ShowSubItem+0x2bc>)
 8009a66:	2900      	cmp	r1, #0
 8009a68:	bf08      	it	eq
 8009a6a:	4613      	moveq	r3, r2
 8009a6c:	681a      	ldr	r2, [r3, #0]
 8009a6e:	791b      	ldrb	r3, [r3, #4]
 8009a70:	f8cd 203f 	str.w	r2, [sp, #63]	; 0x3f
 8009a74:	4976      	ldr	r1, [pc, #472]	; (8009c50 <ShowSubItem+0x2c0>)
 8009a76:	f895 2098 	ldrb.w	r2, [r5, #152]	; 0x98
 8009a7a:	f88d 3043 	strb.w	r3, [sp, #67]	; 0x43
 8009a7e:	f005 ff1d 	bl	800f8bc <sprintf>
 8009a82:	4974      	ldr	r1, [pc, #464]	; (8009c54 <ShowSubItem+0x2c4>)
 8009a84:	4620      	mov	r0, r4
 8009a86:	f009 f860 	bl	8012b4a <strcat>
 8009a8a:	e894 0003 	ldmia.w	r4, {r0, r1}
 8009a8e:	4a6a      	ldr	r2, [pc, #424]	; (8009c38 <ShowSubItem+0x2a8>)
 8009a90:	f88d 104c 	strb.w	r1, [sp, #76]	; 0x4c
 8009a94:	f895 1099 	ldrb.w	r1, [r5, #153]	; 0x99
 8009a98:	f502 7323 	add.w	r3, r2, #652	; 0x28c
 8009a9c:	ebc1 01c1 	rsb	r1, r1, r1, lsl #3
 8009aa0:	440b      	add	r3, r1
 8009aa2:	6819      	ldr	r1, [r3, #0]
 8009aa4:	791b      	ldrb	r3, [r3, #4]
 8009aa6:	f8cd 1051 	str.w	r1, [sp, #81]	; 0x51
 8009aaa:	f88d 3055 	strb.w	r3, [sp, #85]	; 0x55
 8009aae:	4b61      	ldr	r3, [pc, #388]	; (8009c34 <ShowSubItem+0x2a4>)
 8009ab0:	9012      	str	r0, [sp, #72]	; 0x48
 8009ab2:	7899      	ldrb	r1, [r3, #2]
 8009ab4:	4b61      	ldr	r3, [pc, #388]	; (8009c3c <ShowSubItem+0x2ac>)
 8009ab6:	440b      	add	r3, r1
 8009ab8:	791b      	ldrb	r3, [r3, #4]
 8009aba:	8829      	ldrh	r1, [r5, #0]
 8009abc:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 8009ac0:	440b      	add	r3, r1
 8009ac2:	2105      	movs	r1, #5
 8009ac4:	f893 3264 	ldrb.w	r3, [r3, #612]	; 0x264
 8009ac8:	e9cd 6103 	strd	r6, r1, [sp, #12]
 8009acc:	3301      	adds	r3, #1
 8009ace:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 8009ad2:	005b      	lsls	r3, r3, #1
 8009ad4:	3306      	adds	r3, #6
 8009ad6:	b29b      	uxth	r3, r3
 8009ad8:	f502 722a 	add.w	r2, r2, #680	; 0x2a8
 8009adc:	e78e      	b.n	80099fc <ShowSubItem+0x6c>
 8009ade:	2006      	movs	r0, #6
 8009ae0:	4a55      	ldr	r2, [pc, #340]	; (8009c38 <ShowSubItem+0x2a8>)
 8009ae2:	f895 309c 	ldrb.w	r3, [r5, #156]	; 0x9c
 8009ae6:	f502 7142 	add.w	r1, r2, #776	; 0x308
 8009aea:	fb00 1303 	mla	r3, r0, r3, r1
 8009aee:	f895 109d 	ldrb.w	r1, [r5, #157]	; 0x9d
 8009af2:	6818      	ldr	r0, [r3, #0]
 8009af4:	791b      	ldrb	r3, [r3, #4]
 8009af6:	ebc1 01c1 	rsb	r1, r1, r1, lsl #3
 8009afa:	9009      	str	r0, [sp, #36]	; 0x24
 8009afc:	7133      	strb	r3, [r6, #4]
 8009afe:	f502 7323 	add.w	r3, r2, #652	; 0x28c
 8009b02:	440b      	add	r3, r1
 8009b04:	6819      	ldr	r1, [r3, #0]
 8009b06:	791b      	ldrb	r3, [r3, #4]
 8009b08:	f8cd 102d 	str.w	r1, [sp, #45]	; 0x2d
 8009b0c:	f88d 3031 	strb.w	r3, [sp, #49]	; 0x31
 8009b10:	4b48      	ldr	r3, [pc, #288]	; (8009c34 <ShowSubItem+0x2a4>)
 8009b12:	7899      	ldrb	r1, [r3, #2]
 8009b14:	4b49      	ldr	r3, [pc, #292]	; (8009c3c <ShowSubItem+0x2ac>)
 8009b16:	440b      	add	r3, r1
 8009b18:	2105      	movs	r1, #5
 8009b1a:	791b      	ldrb	r3, [r3, #4]
 8009b1c:	e9cd 6103 	strd	r6, r1, [sp, #12]
 8009b20:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 8009b24:	f893 3266 	ldrb.w	r3, [r3, #614]	; 0x266
 8009b28:	f502 7245 	add.w	r2, r2, #788	; 0x314
 8009b2c:	3301      	adds	r3, #1
 8009b2e:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 8009b32:	005b      	lsls	r3, r3, #1
 8009b34:	3306      	adds	r3, #6
 8009b36:	b29b      	uxth	r3, r3
 8009b38:	e760      	b.n	80099fc <ShowSubItem+0x6c>
 8009b3a:	f895 30a4 	ldrb.w	r3, [r5, #164]	; 0xa4
 8009b3e:	4f46      	ldr	r7, [pc, #280]	; (8009c58 <ShowSubItem+0x2c8>)
 8009b40:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 8009b44:	443b      	add	r3, r7
 8009b46:	6818      	ldr	r0, [r3, #0]
 8009b48:	889b      	ldrh	r3, [r3, #4]
 8009b4a:	9009      	str	r0, [sp, #36]	; 0x24
 8009b4c:	f895 20a5 	ldrb.w	r2, [r5, #165]	; 0xa5
 8009b50:	4942      	ldr	r1, [pc, #264]	; (8009c5c <ShowSubItem+0x2cc>)
 8009b52:	4620      	mov	r0, r4
 8009b54:	80b3      	strh	r3, [r6, #4]
 8009b56:	f005 feb1 	bl	800f8bc <sprintf>
 8009b5a:	493e      	ldr	r1, [pc, #248]	; (8009c54 <ShowSubItem+0x2c4>)
 8009b5c:	4620      	mov	r0, r4
 8009b5e:	f008 fff4 	bl	8012b4a <strcat>
 8009b62:	88a3      	ldrh	r3, [r4, #4]
 8009b64:	f895 20a6 	ldrb.w	r2, [r5, #166]	; 0xa6
 8009b68:	f8ad 3031 	strh.w	r3, [sp, #49]	; 0x31
 8009b6c:	ebc2 02c2 	rsb	r2, r2, r2, lsl #3
 8009b70:	f107 0338 	add.w	r3, r7, #56	; 0x38
 8009b74:	4413      	add	r3, r2
 8009b76:	681a      	ldr	r2, [r3, #0]
 8009b78:	889b      	ldrh	r3, [r3, #4]
 8009b7a:	f8cd 2036 	str.w	r2, [sp, #54]	; 0x36
 8009b7e:	f895 20a7 	ldrb.w	r2, [r5, #167]	; 0xa7
 8009b82:	f8ad 303a 	strh.w	r3, [sp, #58]	; 0x3a
 8009b86:	ebc2 02c2 	rsb	r2, r2, r2, lsl #3
 8009b8a:	f107 0354 	add.w	r3, r7, #84	; 0x54
 8009b8e:	4413      	add	r3, r2
 8009b90:	681a      	ldr	r2, [r3, #0]
 8009b92:	889b      	ldrh	r3, [r3, #4]
 8009b94:	f895 10a8 	ldrb.w	r1, [r5, #168]	; 0xa8
 8009b98:	f8cd 203f 	str.w	r2, [sp, #63]	; 0x3f
 8009b9c:	f8ad 3043 	strh.w	r3, [sp, #67]	; 0x43
 8009ba0:	4a2f      	ldr	r2, [pc, #188]	; (8009c60 <ShowSubItem+0x2d0>)
 8009ba2:	4b30      	ldr	r3, [pc, #192]	; (8009c64 <ShowSubItem+0x2d4>)
 8009ba4:	9806      	ldr	r0, [sp, #24]
 8009ba6:	2900      	cmp	r1, #0
 8009ba8:	bf08      	it	eq
 8009baa:	4613      	moveq	r3, r2
 8009bac:	f8cd 002d 	str.w	r0, [sp, #45]	; 0x2d
 8009bb0:	6818      	ldr	r0, [r3, #0]
 8009bb2:	889b      	ldrh	r3, [r3, #4]
 8009bb4:	f895 10a9 	ldrb.w	r1, [r5, #169]	; 0xa9
 8009bb8:	f8ad 304c 	strh.w	r3, [sp, #76]	; 0x4c
 8009bbc:	4a2a      	ldr	r2, [pc, #168]	; (8009c68 <ShowSubItem+0x2d8>)
 8009bbe:	4b2b      	ldr	r3, [pc, #172]	; (8009c6c <ShowSubItem+0x2dc>)
 8009bc0:	9012      	str	r0, [sp, #72]	; 0x48
 8009bc2:	2900      	cmp	r1, #0
 8009bc4:	bf08      	it	eq
 8009bc6:	4613      	moveq	r3, r2
 8009bc8:	f895 10aa 	ldrb.w	r1, [r5, #170]	; 0xaa
 8009bcc:	681a      	ldr	r2, [r3, #0]
 8009bce:	889b      	ldrh	r3, [r3, #4]
 8009bd0:	ebc1 01c1 	rsb	r1, r1, r1, lsl #3
 8009bd4:	f8ad 3055 	strh.w	r3, [sp, #85]	; 0x55
 8009bd8:	f1a7 03a8 	sub.w	r3, r7, #168	; 0xa8
 8009bdc:	440b      	add	r3, r1
 8009bde:	6819      	ldr	r1, [r3, #0]
 8009be0:	889b      	ldrh	r3, [r3, #4]
 8009be2:	f8cd 105a 	str.w	r1, [sp, #90]	; 0x5a
 8009be6:	f8ad 305e 	strh.w	r3, [sp, #94]	; 0x5e
 8009bea:	4b12      	ldr	r3, [pc, #72]	; (8009c34 <ShowSubItem+0x2a4>)
 8009bec:	f8cd 2051 	str.w	r2, [sp, #81]	; 0x51
 8009bf0:	7899      	ldrb	r1, [r3, #2]
 8009bf2:	4b12      	ldr	r3, [pc, #72]	; (8009c3c <ShowSubItem+0x2ac>)
 8009bf4:	f5a7 724d 	sub.w	r2, r7, #820	; 0x334
 8009bf8:	440b      	add	r3, r1
 8009bfa:	791b      	ldrb	r3, [r3, #4]
 8009bfc:	8829      	ldrh	r1, [r5, #0]
 8009bfe:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 8009c02:	440b      	add	r3, r1
 8009c04:	f107 0269 	add.w	r2, r7, #105	; 0x69
 8009c08:	f893 3264 	ldrb.w	r3, [r3, #612]	; 0x264
 8009c0c:	2106      	movs	r1, #6
 8009c0e:	9202      	str	r2, [sp, #8]
 8009c10:	f643 10e7 	movw	r0, #14823	; 0x39e7
 8009c14:	f240 72ff 	movw	r2, #2047	; 0x7ff
 8009c18:	3301      	adds	r3, #1
 8009c1a:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 8009c1e:	005b      	lsls	r3, r3, #1
 8009c20:	3306      	adds	r3, #6
 8009c22:	e9cd 2000 	strd	r2, r0, [sp]
 8009c26:	b29b      	uxth	r3, r3
 8009c28:	e9cd 6103 	strd	r6, r1, [sp, #12]
 8009c2c:	2296      	movs	r2, #150	; 0x96
 8009c2e:	e6ed      	b.n	8009a0c <ShowSubItem+0x7c>
 8009c30:	2000000c 	.word	0x2000000c
 8009c34:	20004c12 	.word	0x20004c12
 8009c38:	08012c0c 	.word	0x08012c0c
 8009c3c:	20000240 	.word	0x20000240
 8009c40:	08013850 	.word	0x08013850
 8009c44:	0801384a 	.word	0x0801384a
 8009c48:	0801385e 	.word	0x0801385e
 8009c4c:	08013857 	.word	0x08013857
 8009c50:	08013880 	.word	0x08013880
 8009c54:	08012bc6 	.word	0x08012bc6
 8009c58:	08012f40 	.word	0x08012f40
 8009c5c:	08013884 	.word	0x08013884
 8009c60:	0801385d 	.word	0x0801385d
 8009c64:	08013856 	.word	0x08013856
 8009c68:	0801386b 	.word	0x0801386b
 8009c6c:	08013864 	.word	0x08013864
 8009c70:	4f52      	ldr	r7, [pc, #328]	; (8009dbc <ShowSubItem+0x42c>)
 8009c72:	f8b7 3050 	ldrh.w	r3, [r7, #80]	; 0x50
 8009c76:	2b00      	cmp	r3, #0
 8009c78:	f47f ae9e 	bne.w	80099b8 <ShowSubItem+0x28>
 8009c7c:	f897 30ac 	ldrb.w	r3, [r7, #172]	; 0xac
 8009c80:	4d4f      	ldr	r5, [pc, #316]	; (8009dc0 <ShowSubItem+0x430>)
 8009c82:	f8df 9150 	ldr.w	r9, [pc, #336]	; 8009dd4 <ShowSubItem+0x444>
 8009c86:	f04f 0a0a 	mov.w	sl, #10
 8009c8a:	2b00      	cmp	r3, #0
 8009c8c:	bf14      	ite	ne
 8009c8e:	462b      	movne	r3, r5
 8009c90:	464b      	moveq	r3, r9
 8009c92:	6818      	ldr	r0, [r3, #0]
 8009c94:	889b      	ldrh	r3, [r3, #4]
 8009c96:	9009      	str	r0, [sp, #36]	; 0x24
 8009c98:	80b3      	strh	r3, [r6, #4]
 8009c9a:	f897 30ad 	ldrb.w	r3, [r7, #173]	; 0xad
 8009c9e:	f8df 8138 	ldr.w	r8, [pc, #312]	; 8009dd8 <ShowSubItem+0x448>
 8009ca2:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 8009ca6:	4443      	add	r3, r8
 8009ca8:	681a      	ldr	r2, [r3, #0]
 8009caa:	889b      	ldrh	r3, [r3, #4]
 8009cac:	f8cd 202d 	str.w	r2, [sp, #45]	; 0x2d
 8009cb0:	f897 20ae 	ldrb.w	r2, [r7, #174]	; 0xae
 8009cb4:	4943      	ldr	r1, [pc, #268]	; (8009dc4 <ShowSubItem+0x434>)
 8009cb6:	fb0a f202 	mul.w	r2, sl, r2
 8009cba:	4620      	mov	r0, r4
 8009cbc:	f8ad 3031 	strh.w	r3, [sp, #49]	; 0x31
 8009cc0:	f005 fdfc 	bl	800f8bc <sprintf>
 8009cc4:	4940      	ldr	r1, [pc, #256]	; (8009dc8 <ShowSubItem+0x438>)
 8009cc6:	4620      	mov	r0, r4
 8009cc8:	f008 ff3f 	bl	8012b4a <strcat>
 8009ccc:	9806      	ldr	r0, [sp, #24]
 8009cce:	f897 20af 	ldrb.w	r2, [r7, #175]	; 0xaf
 8009cd2:	88a3      	ldrh	r3, [r4, #4]
 8009cd4:	fb0a f202 	mul.w	r2, sl, r2
 8009cd8:	f8cd 0036 	str.w	r0, [sp, #54]	; 0x36
 8009cdc:	4939      	ldr	r1, [pc, #228]	; (8009dc4 <ShowSubItem+0x434>)
 8009cde:	4620      	mov	r0, r4
 8009ce0:	f8ad 303a 	strh.w	r3, [sp, #58]	; 0x3a
 8009ce4:	f005 fdea 	bl	800f8bc <sprintf>
 8009ce8:	4937      	ldr	r1, [pc, #220]	; (8009dc8 <ShowSubItem+0x438>)
 8009cea:	4620      	mov	r0, r4
 8009cec:	f008 ff2d 	bl	8012b4a <strcat>
 8009cf0:	88a3      	ldrh	r3, [r4, #4]
 8009cf2:	9806      	ldr	r0, [sp, #24]
 8009cf4:	f8ad 3043 	strh.w	r3, [sp, #67]	; 0x43
 8009cf8:	f897 30b0 	ldrb.w	r3, [r7, #176]	; 0xb0
 8009cfc:	f8cd 003f 	str.w	r0, [sp, #63]	; 0x3f
 8009d00:	2b00      	cmp	r3, #0
 8009d02:	bf08      	it	eq
 8009d04:	464d      	moveq	r5, r9
 8009d06:	88ab      	ldrh	r3, [r5, #4]
 8009d08:	6828      	ldr	r0, [r5, #0]
 8009d0a:	f8ad 304c 	strh.w	r3, [sp, #76]	; 0x4c
 8009d0e:	f897 30b1 	ldrb.w	r3, [r7, #177]	; 0xb1
 8009d12:	f2a8 450d 	subw	r5, r8, #1037	; 0x40d
 8009d16:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 8009d1a:	4498      	add	r8, r3
 8009d1c:	f8d8 3000 	ldr.w	r3, [r8]
 8009d20:	f897 20b2 	ldrb.w	r2, [r7, #178]	; 0xb2
 8009d24:	f8cd 3051 	str.w	r3, [sp, #81]	; 0x51
 8009d28:	f8b8 3004 	ldrh.w	r3, [r8, #4]
 8009d2c:	fb0a f202 	mul.w	r2, sl, r2
 8009d30:	9012      	str	r0, [sp, #72]	; 0x48
 8009d32:	4924      	ldr	r1, [pc, #144]	; (8009dc4 <ShowSubItem+0x434>)
 8009d34:	4620      	mov	r0, r4
 8009d36:	f8ad 3055 	strh.w	r3, [sp, #85]	; 0x55
 8009d3a:	f005 fdbf 	bl	800f8bc <sprintf>
 8009d3e:	4922      	ldr	r1, [pc, #136]	; (8009dc8 <ShowSubItem+0x438>)
 8009d40:	4620      	mov	r0, r4
 8009d42:	f008 ff02 	bl	8012b4a <strcat>
 8009d46:	9806      	ldr	r0, [sp, #24]
 8009d48:	f897 20b3 	ldrb.w	r2, [r7, #179]	; 0xb3
 8009d4c:	88a3      	ldrh	r3, [r4, #4]
 8009d4e:	fb0a f202 	mul.w	r2, sl, r2
 8009d52:	f8cd 005a 	str.w	r0, [sp, #90]	; 0x5a
 8009d56:	491b      	ldr	r1, [pc, #108]	; (8009dc4 <ShowSubItem+0x434>)
 8009d58:	4620      	mov	r0, r4
 8009d5a:	f8ad 305e 	strh.w	r3, [sp, #94]	; 0x5e
 8009d5e:	f005 fdad 	bl	800f8bc <sprintf>
 8009d62:	4919      	ldr	r1, [pc, #100]	; (8009dc8 <ShowSubItem+0x438>)
 8009d64:	4620      	mov	r0, r4
 8009d66:	f008 fef0 	bl	8012b4a <strcat>
 8009d6a:	88a3      	ldrh	r3, [r4, #4]
 8009d6c:	f643 11e7 	movw	r1, #14823	; 0x39e7
 8009d70:	f8ad 3067 	strh.w	r3, [sp, #103]	; 0x67
 8009d74:	4b15      	ldr	r3, [pc, #84]	; (8009dcc <ShowSubItem+0x43c>)
 8009d76:	9806      	ldr	r0, [sp, #24]
 8009d78:	789a      	ldrb	r2, [r3, #2]
 8009d7a:	4b15      	ldr	r3, [pc, #84]	; (8009dd0 <ShowSubItem+0x440>)
 8009d7c:	f8cd 0063 	str.w	r0, [sp, #99]	; 0x63
 8009d80:	4413      	add	r3, r2
 8009d82:	791b      	ldrb	r3, [r3, #4]
 8009d84:	f8b7 2050 	ldrh.w	r2, [r7, #80]	; 0x50
 8009d88:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 8009d8c:	4413      	add	r3, r2
 8009d8e:	2206      	movs	r2, #6
 8009d90:	e9cd 6203 	strd	r6, r2, [sp, #12]
 8009d94:	f240 72ff 	movw	r2, #2047	; 0x7ff
 8009d98:	f893 3268 	ldrb.w	r3, [r3, #616]	; 0x268
 8009d9c:	f205 457d 	addw	r5, r5, #1149	; 0x47d
 8009da0:	3301      	adds	r3, #1
 8009da2:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 8009da6:	005b      	lsls	r3, r3, #1
 8009da8:	3306      	adds	r3, #6
 8009daa:	e9cd 2100 	strd	r2, r1, [sp]
 8009dae:	b29b      	uxth	r3, r3
 8009db0:	9502      	str	r5, [sp, #8]
 8009db2:	2296      	movs	r2, #150	; 0x96
 8009db4:	21c3      	movs	r1, #195	; 0xc3
 8009db6:	20a0      	movs	r0, #160	; 0xa0
 8009db8:	e62a      	b.n	8009a10 <ShowSubItem+0x80>
 8009dba:	bf00      	nop
 8009dbc:	2000000c 	.word	0x2000000c
 8009dc0:	08013872 	.word	0x08013872
 8009dc4:	08013884 	.word	0x08013884
 8009dc8:	08013888 	.word	0x08013888
 8009dcc:	20004c12 	.word	0x20004c12
 8009dd0:	20000240 	.word	0x20000240
 8009dd4:	08013879 	.word	0x08013879
 8009dd8:	08013019 	.word	0x08013019

08009ddc <ShowSmplStatus>:
 8009ddc:	b537      	push	{r0, r1, r2, r4, r5, lr}
 8009dde:	f647 34ef 	movw	r4, #31727	; 0x7bef
 8009de2:	4605      	mov	r5, r0
 8009de4:	2302      	movs	r3, #2
 8009de6:	2211      	movs	r2, #17
 8009de8:	21df      	movs	r1, #223	; 0xdf
 8009dea:	9400      	str	r4, [sp, #0]
 8009dec:	2000      	movs	r0, #0
 8009dee:	f7ff fca0 	bl	8009732 <DrawRectangle>
 8009df2:	2302      	movs	r3, #2
 8009df4:	2211      	movs	r2, #17
 8009df6:	21eb      	movs	r1, #235	; 0xeb
 8009df8:	9400      	str	r4, [sp, #0]
 8009dfa:	2000      	movs	r0, #0
 8009dfc:	f7ff fc99 	bl	8009732 <DrawRectangle>
 8009e00:	4623      	mov	r3, r4
 8009e02:	220e      	movs	r2, #14
 8009e04:	21df      	movs	r1, #223	; 0xdf
 8009e06:	2011      	movs	r0, #17
 8009e08:	f7fe ffac 	bl	8008d64 <DrawTriangle.part.1>
 8009e0c:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8009e10:	4620      	mov	r0, r4
 8009e12:	f002 f930 	bl	800c076 <Set_Color>
 8009e16:	2200      	movs	r2, #0
 8009e18:	4b13      	ldr	r3, [pc, #76]	; (8009e68 <ShowSmplStatus+0x8c>)
 8009e1a:	21e1      	movs	r1, #225	; 0xe1
 8009e1c:	9300      	str	r3, [sp, #0]
 8009e1e:	4610      	mov	r0, r2
 8009e20:	2301      	movs	r3, #1
 8009e22:	f002 f92f 	bl	800c084 <Show_Str6x8>
 8009e26:	b9bd      	cbnz	r5, 8009e58 <ShowSmplStatus+0x7c>
 8009e28:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8009e2c:	4620      	mov	r0, r4
 8009e2e:	f002 f922 	bl	800c076 <Set_Color>
 8009e32:	4b0e      	ldr	r3, [pc, #56]	; (8009e6c <ShowSmplStatus+0x90>)
 8009e34:	9300      	str	r3, [sp, #0]
 8009e36:	2220      	movs	r2, #32
 8009e38:	2301      	movs	r3, #1
 8009e3a:	21e1      	movs	r1, #225	; 0xe1
 8009e3c:	2006      	movs	r0, #6
 8009e3e:	f002 f921 	bl	800c084 <Show_Str6x8>
 8009e42:	f647 33ef 	movw	r3, #31727	; 0x7bef
 8009e46:	2205      	movs	r2, #5
 8009e48:	9300      	str	r3, [sp, #0]
 8009e4a:	21df      	movs	r1, #223	; 0xdf
 8009e4c:	230e      	movs	r3, #14
 8009e4e:	200c      	movs	r0, #12
 8009e50:	f7ff fc6f 	bl	8009732 <DrawRectangle>
 8009e54:	b003      	add	sp, #12
 8009e56:	bd30      	pop	{r4, r5, pc}
 8009e58:	f44f 61fc 	mov.w	r1, #2016	; 0x7e0
 8009e5c:	4620      	mov	r0, r4
 8009e5e:	f002 f90a 	bl	800c076 <Set_Color>
 8009e62:	4b03      	ldr	r3, [pc, #12]	; (8009e70 <ShowSmplStatus+0x94>)
 8009e64:	e7e6      	b.n	8009e34 <ShowSmplStatus+0x58>
 8009e66:	bf00      	nop
 8009e68:	08012bc7 	.word	0x08012bc7
 8009e6c:	0801388b 	.word	0x0801388b
 8009e70:	0801388d 	.word	0x0801388d

08009e74 <DrawRecLine>:
 8009e74:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8009e78:	eb00 0902 	add.w	r9, r0, r2
 8009e7c:	f5b9 7fa0 	cmp.w	r9, #320	; 0x140
 8009e80:	4604      	mov	r4, r0
 8009e82:	460d      	mov	r5, r1
 8009e84:	4616      	mov	r6, r2
 8009e86:	4698      	mov	r8, r3
 8009e88:	f8bd 7020 	ldrh.w	r7, [sp, #32]
 8009e8c:	da3e      	bge.n	8009f0c <DrawRecLine+0x98>
 8009e8e:	18cb      	adds	r3, r1, r3
 8009e90:	2bef      	cmp	r3, #239	; 0xef
 8009e92:	dc3b      	bgt.n	8009f0c <DrawRecLine+0x98>
 8009e94:	f001 fe4e 	bl	800bb34 <Set_Posi>
 8009e98:	f04f 0a00 	mov.w	sl, #0
 8009e9c:	45c2      	cmp	sl, r8
 8009e9e:	d116      	bne.n	8009ece <DrawRecLine+0x5a>
 8009ea0:	1e70      	subs	r0, r6, #1
 8009ea2:	4420      	add	r0, r4
 8009ea4:	4629      	mov	r1, r5
 8009ea6:	b280      	uxth	r0, r0
 8009ea8:	f001 fe44 	bl	800bb34 <Set_Posi>
 8009eac:	2600      	movs	r6, #0
 8009eae:	4556      	cmp	r6, sl
 8009eb0:	d113      	bne.n	8009eda <DrawRecLine+0x66>
 8009eb2:	4626      	mov	r6, r4
 8009eb4:	fa1f f989 	uxth.w	r9, r9
 8009eb8:	45b1      	cmp	r9, r6
 8009eba:	d113      	bne.n	8009ee4 <DrawRecLine+0x70>
 8009ebc:	f108 38ff 	add.w	r8, r8, #4294967295	; 0xffffffff
 8009ec0:	4445      	add	r5, r8
 8009ec2:	b2ad      	uxth	r5, r5
 8009ec4:	42b4      	cmp	r4, r6
 8009ec6:	d117      	bne.n	8009ef8 <DrawRecLine+0x84>
 8009ec8:	2001      	movs	r0, #1
 8009eca:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8009ece:	4638      	mov	r0, r7
 8009ed0:	f001 fe3c 	bl	800bb4c <Set_Pixel>
 8009ed4:	f10a 0a01 	add.w	sl, sl, #1
 8009ed8:	e7e0      	b.n	8009e9c <DrawRecLine+0x28>
 8009eda:	4638      	mov	r0, r7
 8009edc:	f001 fe36 	bl	800bb4c <Set_Pixel>
 8009ee0:	3601      	adds	r6, #1
 8009ee2:	e7e4      	b.n	8009eae <DrawRecLine+0x3a>
 8009ee4:	4630      	mov	r0, r6
 8009ee6:	4629      	mov	r1, r5
 8009ee8:	f001 fe24 	bl	800bb34 <Set_Posi>
 8009eec:	3601      	adds	r6, #1
 8009eee:	4638      	mov	r0, r7
 8009ef0:	f001 fe2c 	bl	800bb4c <Set_Pixel>
 8009ef4:	b2b6      	uxth	r6, r6
 8009ef6:	e7df      	b.n	8009eb8 <DrawRecLine+0x44>
 8009ef8:	4620      	mov	r0, r4
 8009efa:	4629      	mov	r1, r5
 8009efc:	f001 fe1a 	bl	800bb34 <Set_Posi>
 8009f00:	3401      	adds	r4, #1
 8009f02:	4638      	mov	r0, r7
 8009f04:	f001 fe22 	bl	800bb4c <Set_Pixel>
 8009f08:	b2a4      	uxth	r4, r4
 8009f0a:	e7db      	b.n	8009ec4 <DrawRecLine+0x50>
 8009f0c:	2000      	movs	r0, #0
 8009f0e:	e7dc      	b.n	8009eca <DrawRecLine+0x56>

08009f10 <DrawTriangle>:
 8009f10:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8009f14:	f89d 4020 	ldrb.w	r4, [sp, #32]
 8009f18:	4605      	mov	r5, r0
 8009f1a:	4688      	mov	r8, r1
 8009f1c:	4617      	mov	r7, r2
 8009f1e:	4699      	mov	r9, r3
 8009f20:	b1a4      	cbz	r4, 8009f4c <DrawTriangle+0x3c>
 8009f22:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8009f26:	f7fe bf1d 	b.w	8008d64 <DrawTriangle.part.1>
 8009f2a:	4628      	mov	r0, r5
 8009f2c:	4641      	mov	r1, r8
 8009f2e:	1c6e      	adds	r6, r5, #1
 8009f30:	f001 fe00 	bl	800bb34 <Set_Posi>
 8009f34:	2500      	movs	r5, #0
 8009f36:	b2b6      	uxth	r6, r6
 8009f38:	f104 0a01 	add.w	sl, r4, #1
 8009f3c:	b2ab      	uxth	r3, r5
 8009f3e:	459a      	cmp	sl, r3
 8009f40:	f105 0501 	add.w	r5, r5, #1
 8009f44:	da06      	bge.n	8009f54 <DrawTriangle+0x44>
 8009f46:	4635      	mov	r5, r6
 8009f48:	3402      	adds	r4, #2
 8009f4a:	b2a4      	uxth	r4, r4
 8009f4c:	42bc      	cmp	r4, r7
 8009f4e:	d3ec      	bcc.n	8009f2a <DrawTriangle+0x1a>
 8009f50:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 8009f54:	4648      	mov	r0, r9
 8009f56:	f001 fdf9 	bl	800bb4c <Set_Pixel>
 8009f5a:	e7ef      	b.n	8009f3c <DrawTriangle+0x2c>

08009f5c <ShowBattery>:
 8009f5c:	b57f      	push	{r0, r1, r2, r3, r4, r5, r6, lr}
 8009f5e:	4b37      	ldr	r3, [pc, #220]	; (800a03c <ShowBattery+0xe0>)
 8009f60:	f893 40cc 	ldrb.w	r4, [r3, #204]	; 0xcc
 8009f64:	2c01      	cmp	r4, #1
 8009f66:	d14a      	bne.n	8009ffe <ShowBattery+0xa2>
 8009f68:	2200      	movs	r2, #0
 8009f6a:	f883 20cc 	strb.w	r2, [r3, #204]	; 0xcc
 8009f6e:	4a34      	ldr	r2, [pc, #208]	; (800a040 <ShowBattery+0xe4>)
 8009f70:	4b34      	ldr	r3, [pc, #208]	; (800a044 <ShowBattery+0xe8>)
 8009f72:	8811      	ldrh	r1, [r2, #0]
 8009f74:	881b      	ldrh	r3, [r3, #0]
 8009f76:	f241 0268 	movw	r2, #4200	; 0x1068
 8009f7a:	2900      	cmp	r1, #0
 8009f7c:	d041      	beq.n	800a002 <ShowBattery+0xa6>
 8009f7e:	4293      	cmp	r3, r2
 8009f80:	bf94      	ite	ls
 8009f82:	2405      	movls	r4, #5
 8009f84:	2406      	movhi	r4, #6
 8009f86:	2300      	movs	r3, #0
 8009f88:	f647 36ef 	movw	r6, #31727	; 0x7bef
 8009f8c:	9300      	str	r3, [sp, #0]
 8009f8e:	220e      	movs	r2, #14
 8009f90:	f647 33ef 	movw	r3, #31727	; 0x7bef
 8009f94:	21df      	movs	r1, #223	; 0xdf
 8009f96:	f44f 7092 	mov.w	r0, #292	; 0x124
 8009f9a:	f7ff ffb9 	bl	8009f10 <DrawTriangle>
 8009f9e:	2302      	movs	r3, #2
 8009fa0:	2212      	movs	r2, #18
 8009fa2:	21df      	movs	r1, #223	; 0xdf
 8009fa4:	9600      	str	r6, [sp, #0]
 8009fa6:	f240 102b 	movw	r0, #299	; 0x12b
 8009faa:	f7ff fbc2 	bl	8009732 <DrawRectangle>
 8009fae:	4d26      	ldr	r5, [pc, #152]	; (800a048 <ShowBattery+0xec>)
 8009fb0:	2302      	movs	r3, #2
 8009fb2:	2212      	movs	r2, #18
 8009fb4:	21eb      	movs	r1, #235	; 0xeb
 8009fb6:	9600      	str	r6, [sp, #0]
 8009fb8:	f240 102b 	movw	r0, #299	; 0x12b
 8009fbc:	f7ff fbb9 	bl	8009732 <DrawRectangle>
 8009fc0:	2202      	movs	r2, #2
 8009fc2:	230e      	movs	r3, #14
 8009fc4:	21df      	movs	r1, #223	; 0xdf
 8009fc6:	9600      	str	r6, [sp, #0]
 8009fc8:	f240 103d 	movw	r0, #317	; 0x13d
 8009fcc:	f7ff fbb1 	bl	8009732 <DrawRectangle>
 8009fd0:	eb05 0344 	add.w	r3, r5, r4, lsl #1
 8009fd4:	eb04 0484 	add.w	r4, r4, r4, lsl #2
 8009fd8:	f8b3 14fe 	ldrh.w	r1, [r3, #1278]	; 0x4fe
 8009fdc:	4630      	mov	r0, r6
 8009fde:	442c      	add	r4, r5
 8009fe0:	f002 f849 	bl	800c076 <Set_Color>
 8009fe4:	f8d4 250c 	ldr.w	r2, [r4, #1292]	; 0x50c
 8009fe8:	ab04      	add	r3, sp, #16
 8009fea:	f843 2d08 	str.w	r2, [r3, #-8]!
 8009fee:	9300      	str	r3, [sp, #0]
 8009ff0:	2220      	movs	r2, #32
 8009ff2:	2301      	movs	r3, #1
 8009ff4:	21e1      	movs	r1, #225	; 0xe1
 8009ff6:	f240 102b 	movw	r0, #299	; 0x12b
 8009ffa:	f002 f843 	bl	800c084 <Show_Str6x8>
 8009ffe:	b004      	add	sp, #16
 800a000:	bd70      	pop	{r4, r5, r6, pc}
 800a002:	4293      	cmp	r3, r2
 800a004:	d811      	bhi.n	800a02a <ShowBattery+0xce>
 800a006:	f640 723c 	movw	r2, #3900	; 0xf3c
 800a00a:	4293      	cmp	r3, r2
 800a00c:	d80f      	bhi.n	800a02e <ShowBattery+0xd2>
 800a00e:	f640 6274 	movw	r2, #3700	; 0xe74
 800a012:	4293      	cmp	r3, r2
 800a014:	d80d      	bhi.n	800a032 <ShowBattery+0xd6>
 800a016:	f640 5248 	movw	r2, #3400	; 0xd48
 800a01a:	4293      	cmp	r3, r2
 800a01c:	d80b      	bhi.n	800a036 <ShowBattery+0xda>
 800a01e:	f640 42e4 	movw	r2, #3300	; 0xce4
 800a022:	4293      	cmp	r3, r2
 800a024:	bf98      	it	ls
 800a026:	2400      	movls	r4, #0
 800a028:	e7ad      	b.n	8009f86 <ShowBattery+0x2a>
 800a02a:	2405      	movs	r4, #5
 800a02c:	e7ab      	b.n	8009f86 <ShowBattery+0x2a>
 800a02e:	2404      	movs	r4, #4
 800a030:	e7a9      	b.n	8009f86 <ShowBattery+0x2a>
 800a032:	2403      	movs	r4, #3
 800a034:	e7a7      	b.n	8009f86 <ShowBattery+0x2a>
 800a036:	2402      	movs	r4, #2
 800a038:	e7a5      	b.n	8009f86 <ShowBattery+0x2a>
 800a03a:	bf00      	nop
 800a03c:	2000000c 	.word	0x2000000c
 800a040:	200009cc 	.word	0x200009cc
 800a044:	20000104 	.word	0x20000104
 800a048:	08012c0c 	.word	0x08012c0c

0800a04c <ShowTopTitle>:
 800a04c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800a050:	f8df 819c 	ldr.w	r8, [pc, #412]	; 800a1f0 <ShowTopTitle+0x1a4>
 800a054:	2600      	movs	r6, #0
 800a056:	251d      	movs	r5, #29
 800a058:	46c1      	mov	r9, r8
 800a05a:	b087      	sub	sp, #28
 800a05c:	ac03      	add	r4, sp, #12
 800a05e:	f8df a194 	ldr.w	sl, [pc, #404]	; 800a1f4 <ShowTopTitle+0x1a8>
 800a062:	9603      	str	r6, [sp, #12]
 800a064:	6066      	str	r6, [r4, #4]
 800a066:	8126      	strh	r6, [r4, #8]
 800a068:	f898 3002 	ldrb.w	r3, [r8, #2]
 800a06c:	b2f7      	uxtb	r7, r6
 800a06e:	42bb      	cmp	r3, r7
 800a070:	d141      	bne.n	800a0f6 <ShowTopTitle+0xaa>
 800a072:	2100      	movs	r1, #0
 800a074:	f64b 50e0 	movw	r0, #48608	; 0xbde0
 800a078:	f001 fffd 	bl	800c076 <Set_Color>
 800a07c:	f64b 5be0 	movw	fp, #48608	; 0xbde0
 800a080:	2300      	movs	r3, #0
 800a082:	1fe8      	subs	r0, r5, #7
 800a084:	9300      	str	r3, [sp, #0]
 800a086:	220e      	movs	r2, #14
 800a088:	465b      	mov	r3, fp
 800a08a:	21df      	movs	r1, #223	; 0xdf
 800a08c:	b280      	uxth	r0, r0
 800a08e:	f7ff ff3f 	bl	8009f10 <DrawTriangle>
 800a092:	2302      	movs	r3, #2
 800a094:	222a      	movs	r2, #42	; 0x2a
 800a096:	21df      	movs	r1, #223	; 0xdf
 800a098:	f8cd b000 	str.w	fp, [sp]
 800a09c:	4628      	mov	r0, r5
 800a09e:	f7ff fb48 	bl	8009732 <DrawRectangle>
 800a0a2:	2302      	movs	r3, #2
 800a0a4:	f8cd b000 	str.w	fp, [sp]
 800a0a8:	222a      	movs	r2, #42	; 0x2a
 800a0aa:	21eb      	movs	r1, #235	; 0xeb
 800a0ac:	4628      	mov	r0, r5
 800a0ae:	f7ff fb40 	bl	8009732 <DrawRectangle>
 800a0b2:	f899 3002 	ldrb.w	r3, [r9, #2]
 800a0b6:	f105 002a 	add.w	r0, r5, #42	; 0x2a
 800a0ba:	2b03      	cmp	r3, #3
 800a0bc:	b280      	uxth	r0, r0
 800a0be:	d927      	bls.n	800a110 <ShowTopTitle+0xc4>
 800a0c0:	2e04      	cmp	r6, #4
 800a0c2:	d125      	bne.n	800a110 <ShowTopTitle+0xc4>
 800a0c4:	2b06      	cmp	r3, #6
 800a0c6:	d023      	beq.n	800a110 <ShowTopTitle+0xc4>
 800a0c8:	f240 73ff 	movw	r3, #2047	; 0x7ff
 800a0cc:	220e      	movs	r2, #14
 800a0ce:	9300      	str	r3, [sp, #0]
 800a0d0:	21df      	movs	r1, #223	; 0xdf
 800a0d2:	465b      	mov	r3, fp
 800a0d4:	f7fe fe63 	bl	8008d9e <DrawTriCol.part.3>
 800a0d8:	f899 3002 	ldrb.w	r3, [r9, #2]
 800a0dc:	2b04      	cmp	r3, #4
 800a0de:	bf82      	ittt	hi
 800a0e0:	3b04      	subhi	r3, #4
 800a0e2:	18ff      	addhi	r7, r7, r3
 800a0e4:	b2ff      	uxtbhi	r7, r7
 800a0e6:	2f06      	cmp	r7, #6
 800a0e8:	d83a      	bhi.n	800a160 <ShowTopTitle+0x114>
 800a0ea:	e8df f007 	tbb	[pc, r7]
 800a0ee:	2517      	.short	0x2517
 800a0f0:	64585042 	.word	0x64585042
 800a0f4:	66          	.byte	0x66
 800a0f5:	00          	.byte	0x00
 800a0f6:	2b04      	cmp	r3, #4
 800a0f8:	d901      	bls.n	800a0fe <ShowTopTitle+0xb2>
 800a0fa:	2f04      	cmp	r7, #4
 800a0fc:	d0b9      	beq.n	800a072 <ShowTopTitle+0x26>
 800a0fe:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a102:	f647 30ef 	movw	r0, #31727	; 0x7bef
 800a106:	f001 ffb6 	bl	800c076 <Set_Color>
 800a10a:	f647 3bef 	movw	fp, #31727	; 0x7bef
 800a10e:	e7b7      	b.n	800a080 <ShowTopTitle+0x34>
 800a110:	465b      	mov	r3, fp
 800a112:	220e      	movs	r2, #14
 800a114:	21df      	movs	r1, #223	; 0xdf
 800a116:	f7fe fe25 	bl	8008d64 <DrawTriangle.part.1>
 800a11a:	e7dd      	b.n	800a0d8 <ShowTopTitle+0x8c>
 800a11c:	4b28      	ldr	r3, [pc, #160]	; (800a1c0 <ShowTopTitle+0x174>)
 800a11e:	881a      	ldrh	r2, [r3, #0]
 800a120:	789b      	ldrb	r3, [r3, #2]
 800a122:	8022      	strh	r2, [r4, #0]
 800a124:	70a3      	strb	r3, [r4, #2]
 800a126:	4b27      	ldr	r3, [pc, #156]	; (800a1c4 <ShowTopTitle+0x178>)
 800a128:	4a27      	ldr	r2, [pc, #156]	; (800a1c8 <ShowTopTitle+0x17c>)
 800a12a:	881b      	ldrh	r3, [r3, #0]
 800a12c:	eb03 0383 	add.w	r3, r3, r3, lsl #2
 800a130:	589b      	ldr	r3, [r3, r2]
 800a132:	f8cd 300f 	str.w	r3, [sp, #15]
 800a136:	e00c      	b.n	800a152 <ShowTopTitle+0x106>
 800a138:	4b24      	ldr	r3, [pc, #144]	; (800a1cc <ShowTopTitle+0x180>)
 800a13a:	4a25      	ldr	r2, [pc, #148]	; (800a1d0 <ShowTopTitle+0x184>)
 800a13c:	6818      	ldr	r0, [r3, #0]
 800a13e:	791b      	ldrb	r3, [r3, #4]
 800a140:	6020      	str	r0, [r4, #0]
 800a142:	7123      	strb	r3, [r4, #4]
 800a144:	4b23      	ldr	r3, [pc, #140]	; (800a1d4 <ShowTopTitle+0x188>)
 800a146:	795b      	ldrb	r3, [r3, #5]
 800a148:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 800a14c:	5a9b      	ldrh	r3, [r3, r2]
 800a14e:	f8ad 3011 	strh.w	r3, [sp, #17]
 800a152:	9400      	str	r4, [sp, #0]
 800a154:	2301      	movs	r3, #1
 800a156:	2200      	movs	r2, #0
 800a158:	21e1      	movs	r1, #225	; 0xe1
 800a15a:	4628      	mov	r0, r5
 800a15c:	f001 ff92 	bl	800c084 <Show_Str6x8>
 800a160:	3601      	adds	r6, #1
 800a162:	3536      	adds	r5, #54	; 0x36
 800a164:	2e05      	cmp	r6, #5
 800a166:	b2ad      	uxth	r5, r5
 800a168:	f47f af7e 	bne.w	800a068 <ShowTopTitle+0x1c>
 800a16c:	b007      	add	sp, #28
 800a16e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800a172:	4b14      	ldr	r3, [pc, #80]	; (800a1c4 <ShowTopTitle+0x178>)
 800a174:	f8c4 a000 	str.w	sl, [r4]
 800a178:	8d1a      	ldrh	r2, [r3, #40]	; 0x28
 800a17a:	4b17      	ldr	r3, [pc, #92]	; (800a1d8 <ShowTopTitle+0x18c>)
 800a17c:	eb03 0382 	add.w	r3, r3, r2, lsl #2
 800a180:	881a      	ldrh	r2, [r3, #0]
 800a182:	789b      	ldrb	r3, [r3, #2]
 800a184:	f8ad 2010 	strh.w	r2, [sp, #16]
 800a188:	f88d 3012 	strb.w	r3, [sp, #18]
 800a18c:	e7e1      	b.n	800a152 <ShowTopTitle+0x106>
 800a18e:	4b13      	ldr	r3, [pc, #76]	; (800a1dc <ShowTopTitle+0x190>)
 800a190:	6818      	ldr	r0, [r3, #0]
 800a192:	889a      	ldrh	r2, [r3, #4]
 800a194:	799b      	ldrb	r3, [r3, #6]
 800a196:	6020      	str	r0, [r4, #0]
 800a198:	80a2      	strh	r2, [r4, #4]
 800a19a:	71a3      	strb	r3, [r4, #6]
 800a19c:	e7d9      	b.n	800a152 <ShowTopTitle+0x106>
 800a19e:	4b10      	ldr	r3, [pc, #64]	; (800a1e0 <ShowTopTitle+0x194>)
 800a1a0:	881a      	ldrh	r2, [r3, #0]
 800a1a2:	789b      	ldrb	r3, [r3, #2]
 800a1a4:	8022      	strh	r2, [r4, #0]
 800a1a6:	70a3      	strb	r3, [r4, #2]
 800a1a8:	4b06      	ldr	r3, [pc, #24]	; (800a1c4 <ShowTopTitle+0x178>)
 800a1aa:	4a0e      	ldr	r2, [pc, #56]	; (800a1e4 <ShowTopTitle+0x198>)
 800a1ac:	f8b3 3050 	ldrh.w	r3, [r3, #80]	; 0x50
 800a1b0:	eb03 0383 	add.w	r3, r3, r3, lsl #2
 800a1b4:	e7bc      	b.n	800a130 <ShowTopTitle+0xe4>
 800a1b6:	4b0c      	ldr	r3, [pc, #48]	; (800a1e8 <ShowTopTitle+0x19c>)
 800a1b8:	e7ea      	b.n	800a190 <ShowTopTitle+0x144>
 800a1ba:	4b0c      	ldr	r3, [pc, #48]	; (800a1ec <ShowTopTitle+0x1a0>)
 800a1bc:	e7e8      	b.n	800a190 <ShowTopTitle+0x144>
 800a1be:	bf00      	nop
 800a1c0:	0801388f 	.word	0x0801388f
 800a1c4:	2000000c 	.word	0x2000000c
 800a1c8:	0801313b 	.word	0x0801313b
 800a1cc:	08013893 	.word	0x08013893
 800a1d0:	08013163 	.word	0x08013163
 800a1d4:	20000240 	.word	0x20000240
 800a1d8:	08013175 	.word	0x08013175
 800a1dc:	08013899 	.word	0x08013899
 800a1e0:	080138a1 	.word	0x080138a1
 800a1e4:	08013195 	.word	0x08013195
 800a1e8:	080138a5 	.word	0x080138a5
 800a1ec:	080138ad 	.word	0x080138ad
 800a1f0:	20004c12 	.word	0x20004c12
 800a1f4:	3a697254 	.word	0x3a697254

0800a1f8 <DrawTriCol>:
 800a1f8:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 800a1fc:	f89d 4024 	ldrb.w	r4, [sp, #36]	; 0x24
 800a200:	4605      	mov	r5, r0
 800a202:	4688      	mov	r8, r1
 800a204:	4617      	mov	r7, r2
 800a206:	4699      	mov	r9, r3
 800a208:	f8bd 6020 	ldrh.w	r6, [sp, #32]
 800a20c:	b1ac      	cbz	r4, 800a23a <DrawTriCol+0x42>
 800a20e:	9608      	str	r6, [sp, #32]
 800a210:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 800a214:	f7fe bdc3 	b.w	8008d9e <DrawTriCol.part.3>
 800a218:	4628      	mov	r0, r5
 800a21a:	4641      	mov	r1, r8
 800a21c:	1c6e      	adds	r6, r5, #1
 800a21e:	f001 fc89 	bl	800bb34 <Set_Posi>
 800a222:	2500      	movs	r5, #0
 800a224:	b2b6      	uxth	r6, r6
 800a226:	f104 0a01 	add.w	sl, r4, #1
 800a22a:	b2ab      	uxth	r3, r5
 800a22c:	459a      	cmp	sl, r3
 800a22e:	f105 0501 	add.w	r5, r5, #1
 800a232:	da06      	bge.n	800a242 <DrawTriCol+0x4a>
 800a234:	4635      	mov	r5, r6
 800a236:	3402      	adds	r4, #2
 800a238:	b2a4      	uxth	r4, r4
 800a23a:	42bc      	cmp	r4, r7
 800a23c:	d3ec      	bcc.n	800a218 <DrawTriCol+0x20>
 800a23e:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 800a242:	4648      	mov	r0, r9
 800a244:	f001 fc82 	bl	800bb4c <Set_Pixel>
 800a248:	e7ef      	b.n	800a22a <DrawTriCol+0x32>

0800a24a <DrawScaleRule>:
 800a24a:	2200      	movs	r2, #0
 800a24c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800a250:	b085      	sub	sp, #20
 800a252:	ad02      	add	r5, sp, #8
 800a254:	9202      	str	r2, [sp, #8]
 800a256:	f8c5 2003 	str.w	r2, [r5, #3]
 800a25a:	2203      	movs	r2, #3
 800a25c:	fbb3 f2f2 	udiv	r2, r3, r2
 800a260:	4680      	mov	r8, r0
 800a262:	b2d0      	uxtb	r0, r2
 800a264:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 800a268:	1a9b      	subs	r3, r3, r2
 800a26a:	b2db      	uxtb	r3, r3
 800a26c:	2b01      	cmp	r3, #1
 800a26e:	4689      	mov	r9, r1
 800a270:	d03f      	beq.n	800a2f2 <DrawScaleRule+0xa8>
 800a272:	2b02      	cmp	r3, #2
 800a274:	d041      	beq.n	800a2fa <DrawScaleRule+0xb0>
 800a276:	3801      	subs	r0, #1
 800a278:	b2c0      	uxtb	r0, r0
 800a27a:	f003 fd2b 	bl	800dcd4 <Exp>
 800a27e:	260a      	movs	r6, #10
 800a280:	4346      	muls	r6, r0
 800a282:	2700      	movs	r7, #0
 800a284:	4649      	mov	r1, r9
 800a286:	4640      	mov	r0, r8
 800a288:	f001 fc54 	bl	800bb34 <Set_Posi>
 800a28c:	463c      	mov	r4, r7
 800a28e:	f8df b090 	ldr.w	fp, [pc, #144]	; 800a320 <DrawScaleRule+0xd6>
 800a292:	f014 0a01 	ands.w	sl, r4, #1
 800a296:	d124      	bne.n	800a2e2 <DrawScaleRule+0x98>
 800a298:	4649      	mov	r1, r9
 800a29a:	4640      	mov	r0, r8
 800a29c:	f001 fc4a 	bl	800bb34 <Set_Posi>
 800a2a0:	f647 31ef 	movw	r1, #31727	; 0x7bef
 800a2a4:	4650      	mov	r0, sl
 800a2a6:	f001 fee6 	bl	800c076 <Set_Color>
 800a2aa:	2303      	movs	r3, #3
 800a2ac:	4a1b      	ldr	r2, [pc, #108]	; (800a31c <DrawScaleRule+0xd2>)
 800a2ae:	0879      	lsrs	r1, r7, #1
 800a2b0:	4628      	mov	r0, r5
 800a2b2:	f003 fd9f 	bl	800ddf4 <u32ToStr_Unit>
 800a2b6:	4628      	mov	r0, r5
 800a2b8:	f008 fc56 	bl	8012b68 <strlen>
 800a2bc:	b2c2      	uxtb	r2, r0
 800a2be:	4659      	mov	r1, fp
 800a2c0:	f1c2 0207 	rsb	r2, r2, #7
 800a2c4:	4628      	mov	r0, r5
 800a2c6:	f008 fc57 	bl	8012b78 <strncat>
 800a2ca:	f109 0101 	add.w	r1, r9, #1
 800a2ce:	b289      	uxth	r1, r1
 800a2d0:	b9c4      	cbnz	r4, 800a304 <DrawScaleRule+0xba>
 800a2d2:	4623      	mov	r3, r4
 800a2d4:	4622      	mov	r2, r4
 800a2d6:	f1a8 0002 	sub.w	r0, r8, #2
 800a2da:	9500      	str	r5, [sp, #0]
 800a2dc:	b280      	uxth	r0, r0
 800a2de:	f001 fed1 	bl	800c084 <Show_Str6x8>
 800a2e2:	3401      	adds	r4, #1
 800a2e4:	b2a4      	uxth	r4, r4
 800a2e6:	2c0b      	cmp	r4, #11
 800a2e8:	4437      	add	r7, r6
 800a2ea:	d1d2      	bne.n	800a292 <DrawScaleRule+0x48>
 800a2ec:	b005      	add	sp, #20
 800a2ee:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800a2f2:	f003 fcef 	bl	800dcd4 <Exp>
 800a2f6:	0046      	lsls	r6, r0, #1
 800a2f8:	e7c3      	b.n	800a282 <DrawScaleRule+0x38>
 800a2fa:	f003 fceb 	bl	800dcd4 <Exp>
 800a2fe:	eb00 0680 	add.w	r6, r0, r0, lsl #2
 800a302:	e7be      	b.n	800a282 <DrawScaleRule+0x38>
 800a304:	eb04 0084 	add.w	r0, r4, r4, lsl #2
 800a308:	f1a8 0309 	sub.w	r3, r8, #9
 800a30c:	eb00 0080 	add.w	r0, r0, r0, lsl #2
 800a310:	4418      	add	r0, r3
 800a312:	9500      	str	r5, [sp, #0]
 800a314:	4653      	mov	r3, sl
 800a316:	4652      	mov	r2, sl
 800a318:	e7e0      	b.n	800a2dc <DrawScaleRule+0x92>
 800a31a:	bf00      	nop
 800a31c:	080131a9 	.word	0x080131a9
 800a320:	08012bc1 	.word	0x08012bc1

0800a324 <ShowWindowPrecent>:
 800a324:	4b20      	ldr	r3, [pc, #128]	; (800a3a8 <ShowWindowPrecent+0x84>)
 800a326:	e92d 41f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, lr}
 800a32a:	e9d3 4500 	ldrd	r4, r5, [r3]
 800a32e:	ea54 0305 	orrs.w	r3, r4, r5
 800a332:	d036      	beq.n	800a3a2 <ShowWindowPrecent+0x7e>
 800a334:	f44f 739b 	mov.w	r3, #310	; 0x136
 800a338:	4a1c      	ldr	r2, [pc, #112]	; (800a3ac <ShowWindowPrecent+0x88>)
 800a33a:	f64b 58e0 	movw	r8, #48608	; 0xbde0
 800a33e:	6810      	ldr	r0, [r2, #0]
 800a340:	6856      	ldr	r6, [r2, #4]
 800a342:	fba0 0103 	umull	r0, r1, r0, r3
 800a346:	4622      	mov	r2, r4
 800a348:	fb03 1106 	mla	r1, r3, r6, r1
 800a34c:	462b      	mov	r3, r5
 800a34e:	f008 fa67 	bl	8012820 <__aeabi_uldivmod>
 800a352:	4604      	mov	r4, r0
 800a354:	460d      	mov	r5, r1
 800a356:	2700      	movs	r7, #0
 800a358:	4e15      	ldr	r6, [pc, #84]	; (800a3b0 <ShowWindowPrecent+0x8c>)
 800a35a:	4639      	mov	r1, r7
 800a35c:	8a30      	ldrh	r0, [r6, #16]
 800a35e:	2301      	movs	r3, #1
 800a360:	9700      	str	r7, [sp, #0]
 800a362:	220a      	movs	r2, #10
 800a364:	f7ff f9e5 	bl	8009732 <DrawRectangle>
 800a368:	8a30      	ldrh	r0, [r6, #16]
 800a36a:	2301      	movs	r3, #1
 800a36c:	9700      	str	r7, [sp, #0]
 800a36e:	220a      	movs	r2, #10
 800a370:	2104      	movs	r1, #4
 800a372:	f7ff f9de 	bl	8009732 <DrawRectangle>
 800a376:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800a37a:	8a30      	ldrh	r0, [r6, #16]
 800a37c:	220a      	movs	r2, #10
 800a37e:	9300      	str	r3, [sp, #0]
 800a380:	2101      	movs	r1, #1
 800a382:	2303      	movs	r3, #3
 800a384:	f7ff f9d5 	bl	8009732 <DrawRectangle>
 800a388:	f8cd 8000 	str.w	r8, [sp]
 800a38c:	2305      	movs	r3, #5
 800a38e:	220a      	movs	r2, #10
 800a390:	4639      	mov	r1, r7
 800a392:	b2a0      	uxth	r0, r4
 800a394:	f7ff f9cd 	bl	8009732 <DrawRectangle>
 800a398:	e9c6 4504 	strd	r4, r5, [r6, #16]
 800a39c:	b002      	add	sp, #8
 800a39e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800a3a2:	f647 38ef 	movw	r8, #31727	; 0x7bef
 800a3a6:	e7d6      	b.n	800a356 <ShowWindowPrecent+0x32>
 800a3a8:	20004c30 	.word	0x20004c30
 800a3ac:	20004c18 	.word	0x20004c18
 800a3b0:	20000240 	.word	0x20000240

0800a3b4 <ShowScaleRuleTime>:
 800a3b4:	2200      	movs	r2, #0
 800a3b6:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800a3ba:	b085      	sub	sp, #20
 800a3bc:	ad02      	add	r5, sp, #8
 800a3be:	9202      	str	r2, [sp, #8]
 800a3c0:	f8c5 2003 	str.w	r2, [r5, #3]
 800a3c4:	4a3b      	ldr	r2, [pc, #236]	; (800a4b4 <ShowScaleRuleTime+0x100>)
 800a3c6:	4680      	mov	r8, r0
 800a3c8:	e9d2 ab00 	ldrd	sl, fp, [r2]
 800a3cc:	ea5a 020b 	orrs.w	r2, sl, fp
 800a3d0:	4689      	mov	r9, r1
 800a3d2:	d063      	beq.n	800a49c <ShowScaleRuleTime+0xe8>
 800a3d4:	2203      	movs	r2, #3
 800a3d6:	3301      	adds	r3, #1
 800a3d8:	b2db      	uxtb	r3, r3
 800a3da:	fbb3 f2f2 	udiv	r2, r3, r2
 800a3de:	b2d0      	uxtb	r0, r2
 800a3e0:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 800a3e4:	1a9b      	subs	r3, r3, r2
 800a3e6:	b2db      	uxtb	r3, r3
 800a3e8:	2b01      	cmp	r3, #1
 800a3ea:	d043      	beq.n	800a474 <ShowScaleRuleTime+0xc0>
 800a3ec:	2b02      	cmp	r3, #2
 800a3ee:	d045      	beq.n	800a47c <ShowScaleRuleTime+0xc8>
 800a3f0:	3801      	subs	r0, #1
 800a3f2:	b2c0      	uxtb	r0, r0
 800a3f4:	f003 fc6e 	bl	800dcd4 <Exp>
 800a3f8:	260a      	movs	r6, #10
 800a3fa:	4346      	muls	r6, r0
 800a3fc:	2700      	movs	r7, #0
 800a3fe:	4649      	mov	r1, r9
 800a400:	4640      	mov	r0, r8
 800a402:	f001 fb97 	bl	800bb34 <Set_Posi>
 800a406:	230a      	movs	r3, #10
 800a408:	463c      	mov	r4, r7
 800a40a:	fb03 fb0a 	mul.w	fp, r3, sl
 800a40e:	f014 0a01 	ands.w	sl, r4, #1
 800a412:	d125      	bne.n	800a460 <ShowScaleRuleTime+0xac>
 800a414:	4649      	mov	r1, r9
 800a416:	4640      	mov	r0, r8
 800a418:	f001 fb8c 	bl	800bb34 <Set_Posi>
 800a41c:	f647 31ef 	movw	r1, #31727	; 0x7bef
 800a420:	4650      	mov	r0, sl
 800a422:	f001 fe28 	bl	800c076 <Set_Color>
 800a426:	2303      	movs	r3, #3
 800a428:	4a23      	ldr	r2, [pc, #140]	; (800a4b8 <ShowScaleRuleTime+0x104>)
 800a42a:	eb0b 0157 	add.w	r1, fp, r7, lsr #1
 800a42e:	4628      	mov	r0, r5
 800a430:	f003 fce0 	bl	800ddf4 <u32ToStr_Unit>
 800a434:	4628      	mov	r0, r5
 800a436:	f008 fb97 	bl	8012b68 <strlen>
 800a43a:	b2c2      	uxtb	r2, r0
 800a43c:	491f      	ldr	r1, [pc, #124]	; (800a4bc <ShowScaleRuleTime+0x108>)
 800a43e:	f1c2 0207 	rsb	r2, r2, #7
 800a442:	4628      	mov	r0, r5
 800a444:	f008 fb98 	bl	8012b78 <strncat>
 800a448:	f109 0101 	add.w	r1, r9, #1
 800a44c:	b289      	uxth	r1, r1
 800a44e:	b9d4      	cbnz	r4, 800a486 <ShowScaleRuleTime+0xd2>
 800a450:	4623      	mov	r3, r4
 800a452:	4622      	mov	r2, r4
 800a454:	f1a8 0002 	sub.w	r0, r8, #2
 800a458:	9500      	str	r5, [sp, #0]
 800a45a:	b280      	uxth	r0, r0
 800a45c:	f001 fe12 	bl	800c084 <Show_Str6x8>
 800a460:	3401      	adds	r4, #1
 800a462:	b2a4      	uxth	r4, r4
 800a464:	2c0b      	cmp	r4, #11
 800a466:	4437      	add	r7, r6
 800a468:	d1d1      	bne.n	800a40e <ShowScaleRuleTime+0x5a>
 800a46a:	f7ff ff5b 	bl	800a324 <ShowWindowPrecent>
 800a46e:	b005      	add	sp, #20
 800a470:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800a474:	f003 fc2e 	bl	800dcd4 <Exp>
 800a478:	0046      	lsls	r6, r0, #1
 800a47a:	e7bf      	b.n	800a3fc <ShowScaleRuleTime+0x48>
 800a47c:	f003 fc2a 	bl	800dcd4 <Exp>
 800a480:	eb00 0680 	add.w	r6, r0, r0, lsl #2
 800a484:	e7ba      	b.n	800a3fc <ShowScaleRuleTime+0x48>
 800a486:	eb04 0084 	add.w	r0, r4, r4, lsl #2
 800a48a:	f1a8 0309 	sub.w	r3, r8, #9
 800a48e:	eb00 0080 	add.w	r0, r0, r0, lsl #2
 800a492:	4418      	add	r0, r3
 800a494:	9500      	str	r5, [sp, #0]
 800a496:	4653      	mov	r3, sl
 800a498:	4652      	mov	r2, sl
 800a49a:	e7de      	b.n	800a45a <ShowScaleRuleTime+0xa6>
 800a49c:	4b08      	ldr	r3, [pc, #32]	; (800a4c0 <ShowScaleRuleTime+0x10c>)
 800a49e:	f44f 728c 	mov.w	r2, #280	; 0x118
 800a4a2:	7d1b      	ldrb	r3, [r3, #20]
 800a4a4:	2137      	movs	r1, #55	; 0x37
 800a4a6:	3301      	adds	r3, #1
 800a4a8:	b2db      	uxtb	r3, r3
 800a4aa:	2029      	movs	r0, #41	; 0x29
 800a4ac:	f7ff fecd 	bl	800a24a <DrawScaleRule>
 800a4b0:	e7db      	b.n	800a46a <ShowScaleRuleTime+0xb6>
 800a4b2:	bf00      	nop
 800a4b4:	20004c18 	.word	0x20004c18
 800a4b8:	080131a9 	.word	0x080131a9
 800a4bc:	08012bc1 	.word	0x08012bc1
 800a4c0:	2000000c 	.word	0x2000000c

0800a4c4 <ShowTimeBaseStr>:
 800a4c4:	b530      	push	{r4, r5, lr}
 800a4c6:	2400      	movs	r4, #0
 800a4c8:	4b0e      	ldr	r3, [pc, #56]	; (800a504 <ShowTimeBaseStr+0x40>)
 800a4ca:	b087      	sub	sp, #28
 800a4cc:	8a9a      	ldrh	r2, [r3, #20]
 800a4ce:	4b0e      	ldr	r3, [pc, #56]	; (800a508 <ShowTimeBaseStr+0x44>)
 800a4d0:	ad03      	add	r5, sp, #12
 800a4d2:	eb03 03c2 	add.w	r3, r3, r2, lsl #3
 800a4d6:	6818      	ldr	r0, [r3, #0]
 800a4d8:	889b      	ldrh	r3, [r3, #4]
 800a4da:	9003      	str	r0, [sp, #12]
 800a4dc:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a4e0:	f647 30ef 	movw	r0, #31727	; 0x7bef
 800a4e4:	f8ad 3010 	strh.w	r3, [sp, #16]
 800a4e8:	f8cd 4012 	str.w	r4, [sp, #18]
 800a4ec:	f001 fdc3 	bl	800c076 <Set_Color>
 800a4f0:	9500      	str	r5, [sp, #0]
 800a4f2:	2301      	movs	r3, #1
 800a4f4:	4622      	mov	r2, r4
 800a4f6:	2139      	movs	r1, #57	; 0x39
 800a4f8:	4620      	mov	r0, r4
 800a4fa:	f001 fdc3 	bl	800c084 <Show_Str6x8>
 800a4fe:	b007      	add	sp, #28
 800a500:	bd30      	pop	{r4, r5, pc}
 800a502:	bf00      	nop
 800a504:	2000000c 	.word	0x2000000c
 800a508:	080131bb 	.word	0x080131bb

0800a50c <BackupPreCursorPosition>:
 800a50c:	1e43      	subs	r3, r0, #1
 800a50e:	b2da      	uxtb	r2, r3
 800a510:	2a04      	cmp	r2, #4
 800a512:	bf86      	itte	hi
 800a514:	2300      	movhi	r3, #0
 800a516:	4619      	movhi	r1, r3
 800a518:	4b20      	ldrls	r3, [pc, #128]	; (800a59c <BackupPreCursorPosition+0x90>)
 800a51a:	b510      	push	{r4, lr}
 800a51c:	bf9c      	itt	ls
 800a51e:	189b      	addls	r3, r3, r2
 800a520:	f893 164f 	ldrbls.w	r1, [r3, #1615]	; 0x64f
 800a524:	4a1e      	ldr	r2, [pc, #120]	; (800a5a0 <BackupPreCursorPosition+0x94>)
 800a526:	bf98      	it	ls
 800a528:	f893 3654 	ldrbls.w	r3, [r3, #1620]	; 0x654
 800a52c:	f832 1011 	ldrh.w	r1, [r2, r1, lsl #1]
 800a530:	8b94      	ldrh	r4, [r2, #28]
 800a532:	428c      	cmp	r4, r1
 800a534:	d10c      	bne.n	800a550 <BackupPreCursorPosition+0x44>
 800a536:	2804      	cmp	r0, #4
 800a538:	d009      	beq.n	800a54e <BackupPreCursorPosition+0x42>
 800a53a:	f44f 729a 	mov.w	r2, #308	; 0x134
 800a53e:	4919      	ldr	r1, [pc, #100]	; (800a5a4 <BackupPreCursorPosition+0x98>)
 800a540:	fb02 1003 	mla	r0, r2, r3, r1
 800a544:	f501 7120 	add.w	r1, r1, #640	; 0x280
 800a548:	3018      	adds	r0, #24
 800a54a:	f008 faeb 	bl	8012b24 <memcpy>
 800a54e:	bd10      	pop	{r4, pc}
 800a550:	8ad4      	ldrh	r4, [r2, #22]
 800a552:	428c      	cmp	r4, r1
 800a554:	d108      	bne.n	800a568 <BackupPreCursorPosition+0x5c>
 800a556:	2801      	cmp	r0, #1
 800a558:	d0f9      	beq.n	800a54e <BackupPreCursorPosition+0x42>
 800a55a:	f44f 729a 	mov.w	r2, #308	; 0x134
 800a55e:	4911      	ldr	r1, [pc, #68]	; (800a5a4 <BackupPreCursorPosition+0x98>)
 800a560:	fb02 1003 	mla	r0, r2, r3, r1
 800a564:	3118      	adds	r1, #24
 800a566:	e7ef      	b.n	800a548 <BackupPreCursorPosition+0x3c>
 800a568:	8b14      	ldrh	r4, [r2, #24]
 800a56a:	428c      	cmp	r4, r1
 800a56c:	d109      	bne.n	800a582 <BackupPreCursorPosition+0x76>
 800a56e:	2802      	cmp	r0, #2
 800a570:	d0ed      	beq.n	800a54e <BackupPreCursorPosition+0x42>
 800a572:	f44f 729a 	mov.w	r2, #308	; 0x134
 800a576:	490b      	ldr	r1, [pc, #44]	; (800a5a4 <BackupPreCursorPosition+0x98>)
 800a578:	fb02 1003 	mla	r0, r2, r3, r1
 800a57c:	f501 71a6 	add.w	r1, r1, #332	; 0x14c
 800a580:	e7e2      	b.n	800a548 <BackupPreCursorPosition+0x3c>
 800a582:	8bd2      	ldrh	r2, [r2, #30]
 800a584:	428a      	cmp	r2, r1
 800a586:	d1e2      	bne.n	800a54e <BackupPreCursorPosition+0x42>
 800a588:	2805      	cmp	r0, #5
 800a58a:	d0e0      	beq.n	800a54e <BackupPreCursorPosition+0x42>
 800a58c:	f44f 729a 	mov.w	r2, #308	; 0x134
 800a590:	4904      	ldr	r1, [pc, #16]	; (800a5a4 <BackupPreCursorPosition+0x98>)
 800a592:	fb02 1003 	mla	r0, r2, r3, r1
 800a596:	f501 716d 	add.w	r1, r1, #948	; 0x3b4
 800a59a:	e7d5      	b.n	800a548 <BackupPreCursorPosition+0x3c>
 800a59c:	08012c0c 	.word	0x08012c0c
 800a5a0:	2000000c 	.word	0x2000000c
 800a5a4:	20000240 	.word	0x20000240

0800a5a8 <ShowTimeMarkValue>:
 800a5a8:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 800a5ac:	f04f 0a05 	mov.w	sl, #5
 800a5b0:	4e48      	ldr	r6, [pc, #288]	; (800a6d4 <ShowTimeMarkValue+0x12c>)
 800a5b2:	4d49      	ldr	r5, [pc, #292]	; (800a6d8 <ShowTimeMarkValue+0x130>)
 800a5b4:	8ab3      	ldrh	r3, [r6, #20]
 800a5b6:	8af2      	ldrh	r2, [r6, #22]
 800a5b8:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 800a5bc:	f8d3 165c 	ldr.w	r1, [r3, #1628]	; 0x65c
 800a5c0:	f04f 080a 	mov.w	r8, #10
 800a5c4:	4351      	muls	r1, r2
 800a5c6:	fbb1 f1fa 	udiv	r1, r1, sl
 800a5ca:	2700      	movs	r7, #0
 800a5cc:	f8df 9114 	ldr.w	r9, [pc, #276]	; 800a6e4 <ShowTimeMarkValue+0x13c>
 800a5d0:	b086      	sub	sp, #24
 800a5d2:	f8d9 3000 	ldr.w	r3, [r9]
 800a5d6:	ac03      	add	r4, sp, #12
 800a5d8:	4419      	add	r1, r3
 800a5da:	f205 529d 	addw	r2, r5, #1437	; 0x59d
 800a5de:	2303      	movs	r3, #3
 800a5e0:	fb08 f101 	mul.w	r1, r8, r1
 800a5e4:	4620      	mov	r0, r4
 800a5e6:	9703      	str	r7, [sp, #12]
 800a5e8:	6067      	str	r7, [r4, #4]
 800a5ea:	8127      	strh	r7, [r4, #8]
 800a5ec:	f003 fc02 	bl	800ddf4 <u32ToStr_Unit>
 800a5f0:	4620      	mov	r0, r4
 800a5f2:	f008 fab9 	bl	8012b68 <strlen>
 800a5f6:	b2c2      	uxtb	r2, r0
 800a5f8:	f1c2 0206 	rsb	r2, r2, #6
 800a5fc:	4937      	ldr	r1, [pc, #220]	; (800a6dc <ShowTimeMarkValue+0x134>)
 800a5fe:	4620      	mov	r0, r4
 800a600:	f008 faba 	bl	8012b78 <strncat>
 800a604:	4638      	mov	r0, r7
 800a606:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a60a:	f001 fd34 	bl	800c076 <Set_Color>
 800a60e:	463b      	mov	r3, r7
 800a610:	463a      	mov	r2, r7
 800a612:	9400      	str	r4, [sp, #0]
 800a614:	212c      	movs	r1, #44	; 0x2c
 800a616:	f44f 7089 	mov.w	r0, #274	; 0x112
 800a61a:	f001 fd33 	bl	800c084 <Show_Str6x8>
 800a61e:	8ab3      	ldrh	r3, [r6, #20]
 800a620:	8b32      	ldrh	r2, [r6, #24]
 800a622:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 800a626:	f8d3 165c 	ldr.w	r1, [r3, #1628]	; 0x65c
 800a62a:	f8d9 3000 	ldr.w	r3, [r9]
 800a62e:	4351      	muls	r1, r2
 800a630:	fbb1 f1fa 	udiv	r1, r1, sl
 800a634:	4419      	add	r1, r3
 800a636:	f205 529d 	addw	r2, r5, #1437	; 0x59d
 800a63a:	2303      	movs	r3, #3
 800a63c:	fb08 f101 	mul.w	r1, r8, r1
 800a640:	4620      	mov	r0, r4
 800a642:	f003 fbd7 	bl	800ddf4 <u32ToStr_Unit>
 800a646:	4620      	mov	r0, r4
 800a648:	f008 fa8e 	bl	8012b68 <strlen>
 800a64c:	b2c2      	uxtb	r2, r0
 800a64e:	f1c2 0206 	rsb	r2, r2, #6
 800a652:	4922      	ldr	r1, [pc, #136]	; (800a6dc <ShowTimeMarkValue+0x134>)
 800a654:	4620      	mov	r0, r4
 800a656:	f008 fa8f 	bl	8012b78 <strncat>
 800a65a:	4638      	mov	r0, r7
 800a65c:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a660:	f001 fd09 	bl	800c076 <Set_Color>
 800a664:	463b      	mov	r3, r7
 800a666:	463a      	mov	r2, r7
 800a668:	9400      	str	r4, [sp, #0]
 800a66a:	2120      	movs	r1, #32
 800a66c:	f44f 7089 	mov.w	r0, #274	; 0x112
 800a670:	f001 fd08 	bl	800c084 <Show_Str6x8>
 800a674:	8b33      	ldrh	r3, [r6, #24]
 800a676:	8af1      	ldrh	r1, [r6, #22]
 800a678:	200a      	movs	r0, #10
 800a67a:	428b      	cmp	r3, r1
 800a67c:	bf2c      	ite	cs
 800a67e:	1a59      	subcs	r1, r3, r1
 800a680:	1ac9      	subcc	r1, r1, r3
 800a682:	8ab3      	ldrh	r3, [r6, #20]
 800a684:	4a16      	ldr	r2, [pc, #88]	; (800a6e0 <ShowTimeMarkValue+0x138>)
 800a686:	eb05 0583 	add.w	r5, r5, r3, lsl #2
 800a68a:	f8d5 365c 	ldr.w	r3, [r5, #1628]	; 0x65c
 800a68e:	4359      	muls	r1, r3
 800a690:	2305      	movs	r3, #5
 800a692:	fbb1 f1f3 	udiv	r1, r1, r3
 800a696:	2303      	movs	r3, #3
 800a698:	4341      	muls	r1, r0
 800a69a:	4620      	mov	r0, r4
 800a69c:	f003 fbaa 	bl	800ddf4 <u32ToStr_Unit>
 800a6a0:	4620      	mov	r0, r4
 800a6a2:	f008 fa61 	bl	8012b68 <strlen>
 800a6a6:	b2c2      	uxtb	r2, r0
 800a6a8:	f1c2 0206 	rsb	r2, r2, #6
 800a6ac:	490b      	ldr	r1, [pc, #44]	; (800a6dc <ShowTimeMarkValue+0x134>)
 800a6ae:	4620      	mov	r0, r4
 800a6b0:	f008 fa62 	bl	8012b78 <strncat>
 800a6b4:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a6b8:	2000      	movs	r0, #0
 800a6ba:	f001 fcdc 	bl	800c076 <Set_Color>
 800a6be:	2300      	movs	r3, #0
 800a6c0:	9400      	str	r4, [sp, #0]
 800a6c2:	461a      	mov	r2, r3
 800a6c4:	2114      	movs	r1, #20
 800a6c6:	f44f 7089 	mov.w	r0, #274	; 0x112
 800a6ca:	f001 fcdb 	bl	800c084 <Show_Str6x8>
 800a6ce:	b006      	add	sp, #24
 800a6d0:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 800a6d4:	2000000c 	.word	0x2000000c
 800a6d8:	08012c0c 	.word	0x08012c0c
 800a6dc:	08012bc2 	.word	0x08012bc2
 800a6e0:	080131a9 	.word	0x080131a9
 800a6e4:	20004c18 	.word	0x20004c18

0800a6e8 <ShowMeasureSourceStr>:
 800a6e8:	b570      	push	{r4, r5, r6, lr}
 800a6ea:	4e13      	ldr	r6, [pc, #76]	; (800a738 <ShowMeasureSourceStr+0x50>)
 800a6ec:	b086      	sub	sp, #24
 800a6ee:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a6f2:	f647 30ef 	movw	r0, #31727	; 0x7bef
 800a6f6:	f001 fcbe 	bl	800c076 <Set_Color>
 800a6fa:	4d10      	ldr	r5, [pc, #64]	; (800a73c <ShowMeasureSourceStr+0x54>)
 800a6fc:	8fb3      	ldrh	r3, [r6, #60]	; 0x3c
 800a6fe:	ac06      	add	r4, sp, #24
 800a700:	eb05 0383 	add.w	r3, r5, r3, lsl #2
 800a704:	f8d3 36ac 	ldr.w	r3, [r3, #1708]	; 0x6ac
 800a708:	2120      	movs	r1, #32
 800a70a:	f844 3d0c 	str.w	r3, [r4, #-12]!
 800a70e:	2300      	movs	r3, #0
 800a710:	9400      	str	r4, [sp, #0]
 800a712:	461a      	mov	r2, r3
 800a714:	2005      	movs	r0, #5
 800a716:	f001 fcb5 	bl	800c084 <Show_Str6x8>
 800a71a:	8ff3      	ldrh	r3, [r6, #62]	; 0x3e
 800a71c:	2114      	movs	r1, #20
 800a71e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
 800a722:	f8d5 36ac 	ldr.w	r3, [r5, #1708]	; 0x6ac
 800a726:	2005      	movs	r0, #5
 800a728:	9303      	str	r3, [sp, #12]
 800a72a:	2300      	movs	r3, #0
 800a72c:	9400      	str	r4, [sp, #0]
 800a72e:	461a      	mov	r2, r3
 800a730:	f001 fca8 	bl	800c084 <Show_Str6x8>
 800a734:	b006      	add	sp, #24
 800a736:	bd70      	pop	{r4, r5, r6, pc}
 800a738:	2000000c 	.word	0x2000000c
 800a73c:	08012c0c 	.word	0x08012c0c

0800a740 <ShowMeasureStr>:
 800a740:	b513      	push	{r0, r1, r4, lr}
 800a742:	f647 34ef 	movw	r4, #31727	; 0x7bef
 800a746:	230b      	movs	r3, #11
 800a748:	9400      	str	r4, [sp, #0]
 800a74a:	2226      	movs	r2, #38	; 0x26
 800a74c:	2114      	movs	r1, #20
 800a74e:	2000      	movs	r0, #0
 800a750:	f7fe ffef 	bl	8009732 <DrawRectangle>
 800a754:	9400      	str	r4, [sp, #0]
 800a756:	230b      	movs	r3, #11
 800a758:	2226      	movs	r2, #38	; 0x26
 800a75a:	2120      	movs	r1, #32
 800a75c:	2000      	movs	r0, #0
 800a75e:	f7fe ffe8 	bl	8009732 <DrawRectangle>
 800a762:	230b      	movs	r3, #11
 800a764:	2226      	movs	r2, #38	; 0x26
 800a766:	9400      	str	r4, [sp, #0]
 800a768:	212c      	movs	r1, #44	; 0x2c
 800a76a:	2000      	movs	r0, #0
 800a76c:	f7fe ffe1 	bl	8009732 <DrawRectangle>
 800a770:	4620      	mov	r0, r4
 800a772:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a776:	f001 fc7e 	bl	800c076 <Set_Color>
 800a77a:	4b52      	ldr	r3, [pc, #328]	; (800a8c4 <ShowMeasureStr+0x184>)
 800a77c:	2200      	movs	r2, #0
 800a77e:	9300      	str	r3, [sp, #0]
 800a780:	2301      	movs	r3, #1
 800a782:	212c      	movs	r1, #44	; 0x2c
 800a784:	4618      	mov	r0, r3
 800a786:	f001 fc7d 	bl	800c084 <Show_Str6x8>
 800a78a:	4620      	mov	r0, r4
 800a78c:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a790:	f001 fc71 	bl	800c076 <Set_Color>
 800a794:	f7ff ffa8 	bl	800a6e8 <ShowMeasureSourceStr>
 800a798:	9400      	str	r4, [sp, #0]
 800a79a:	2301      	movs	r3, #1
 800a79c:	f44f 728c 	mov.w	r2, #280	; 0x118
 800a7a0:	2137      	movs	r1, #55	; 0x37
 800a7a2:	2028      	movs	r0, #40	; 0x28
 800a7a4:	f7fe ffc5 	bl	8009732 <DrawRectangle>
 800a7a8:	9400      	str	r4, [sp, #0]
 800a7aa:	2301      	movs	r3, #1
 800a7ac:	f44f 728c 	mov.w	r2, #280	; 0x118
 800a7b0:	212b      	movs	r1, #43	; 0x2b
 800a7b2:	2028      	movs	r0, #40	; 0x28
 800a7b4:	f7fe ffbd 	bl	8009732 <DrawRectangle>
 800a7b8:	9400      	str	r4, [sp, #0]
 800a7ba:	2301      	movs	r3, #1
 800a7bc:	f44f 728c 	mov.w	r2, #280	; 0x118
 800a7c0:	211f      	movs	r1, #31
 800a7c2:	2028      	movs	r0, #40	; 0x28
 800a7c4:	f7fe ffb5 	bl	8009732 <DrawRectangle>
 800a7c8:	9400      	str	r4, [sp, #0]
 800a7ca:	2301      	movs	r3, #1
 800a7cc:	f44f 728c 	mov.w	r2, #280	; 0x118
 800a7d0:	2113      	movs	r1, #19
 800a7d2:	2028      	movs	r0, #40	; 0x28
 800a7d4:	f7fe ffad 	bl	8009732 <DrawRectangle>
 800a7d8:	9400      	str	r4, [sp, #0]
 800a7da:	2325      	movs	r3, #37	; 0x25
 800a7dc:	2201      	movs	r2, #1
 800a7de:	2113      	movs	r1, #19
 800a7e0:	2027      	movs	r0, #39	; 0x27
 800a7e2:	f7fe ffa6 	bl	8009732 <DrawRectangle>
 800a7e6:	9400      	str	r4, [sp, #0]
 800a7e8:	2324      	movs	r3, #36	; 0x24
 800a7ea:	2201      	movs	r2, #1
 800a7ec:	2113      	movs	r1, #19
 800a7ee:	205b      	movs	r0, #91	; 0x5b
 800a7f0:	f7fe ff9f 	bl	8009732 <DrawRectangle>
 800a7f4:	9400      	str	r4, [sp, #0]
 800a7f6:	2324      	movs	r3, #36	; 0x24
 800a7f8:	2201      	movs	r2, #1
 800a7fa:	2113      	movs	r1, #19
 800a7fc:	2091      	movs	r0, #145	; 0x91
 800a7fe:	f7fe ff98 	bl	8009732 <DrawRectangle>
 800a802:	9400      	str	r4, [sp, #0]
 800a804:	2324      	movs	r3, #36	; 0x24
 800a806:	2201      	movs	r2, #1
 800a808:	2113      	movs	r1, #19
 800a80a:	20c7      	movs	r0, #199	; 0xc7
 800a80c:	f7fe ff91 	bl	8009732 <DrawRectangle>
 800a810:	9400      	str	r4, [sp, #0]
 800a812:	2324      	movs	r3, #36	; 0x24
 800a814:	2201      	movs	r2, #1
 800a816:	2113      	movs	r1, #19
 800a818:	20fd      	movs	r0, #253	; 0xfd
 800a81a:	f7fe ff8a 	bl	8009732 <DrawRectangle>
 800a81e:	2325      	movs	r3, #37	; 0x25
 800a820:	2201      	movs	r2, #1
 800a822:	9400      	str	r4, [sp, #0]
 800a824:	2113      	movs	r1, #19
 800a826:	f240 103f 	movw	r0, #319	; 0x13f
 800a82a:	f7fe ff82 	bl	8009732 <DrawRectangle>
 800a82e:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800a832:	2000      	movs	r0, #0
 800a834:	f001 fc1f 	bl	800c076 <Set_Color>
 800a838:	4b23      	ldr	r3, [pc, #140]	; (800a8c8 <ShowMeasureStr+0x188>)
 800a83a:	212c      	movs	r1, #44	; 0x2c
 800a83c:	9300      	str	r3, [sp, #0]
 800a83e:	2300      	movs	r3, #0
 800a840:	2028      	movs	r0, #40	; 0x28
 800a842:	461a      	mov	r2, r3
 800a844:	f001 fc1e 	bl	800c084 <Show_Str6x8>
 800a848:	4b20      	ldr	r3, [pc, #128]	; (800a8cc <ShowMeasureStr+0x18c>)
 800a84a:	212c      	movs	r1, #44	; 0x2c
 800a84c:	9300      	str	r3, [sp, #0]
 800a84e:	2300      	movs	r3, #0
 800a850:	205e      	movs	r0, #94	; 0x5e
 800a852:	461a      	mov	r2, r3
 800a854:	f001 fc16 	bl	800c084 <Show_Str6x8>
 800a858:	4b1d      	ldr	r3, [pc, #116]	; (800a8d0 <ShowMeasureStr+0x190>)
 800a85a:	212c      	movs	r1, #44	; 0x2c
 800a85c:	9300      	str	r3, [sp, #0]
 800a85e:	2300      	movs	r3, #0
 800a860:	2094      	movs	r0, #148	; 0x94
 800a862:	461a      	mov	r2, r3
 800a864:	f001 fc0e 	bl	800c084 <Show_Str6x8>
 800a868:	4b1a      	ldr	r3, [pc, #104]	; (800a8d4 <ShowMeasureStr+0x194>)
 800a86a:	212c      	movs	r1, #44	; 0x2c
 800a86c:	9300      	str	r3, [sp, #0]
 800a86e:	2300      	movs	r3, #0
 800a870:	20ca      	movs	r0, #202	; 0xca
 800a872:	461a      	mov	r2, r3
 800a874:	f001 fc06 	bl	800c084 <Show_Str6x8>
 800a878:	4b17      	ldr	r3, [pc, #92]	; (800a8d8 <ShowMeasureStr+0x198>)
 800a87a:	2200      	movs	r2, #0
 800a87c:	9300      	str	r3, [sp, #0]
 800a87e:	212c      	movs	r1, #44	; 0x2c
 800a880:	2301      	movs	r3, #1
 800a882:	f44f 7080 	mov.w	r0, #256	; 0x100
 800a886:	f001 fbfd 	bl	800c084 <Show_Str6x8>
 800a88a:	4b14      	ldr	r3, [pc, #80]	; (800a8dc <ShowMeasureStr+0x19c>)
 800a88c:	2200      	movs	r2, #0
 800a88e:	9300      	str	r3, [sp, #0]
 800a890:	2120      	movs	r1, #32
 800a892:	2301      	movs	r3, #1
 800a894:	f44f 7080 	mov.w	r0, #256	; 0x100
 800a898:	f001 fbf4 	bl	800c084 <Show_Str6x8>
 800a89c:	4b10      	ldr	r3, [pc, #64]	; (800a8e0 <ShowMeasureStr+0x1a0>)
 800a89e:	2220      	movs	r2, #32
 800a8a0:	9300      	str	r3, [sp, #0]
 800a8a2:	2114      	movs	r1, #20
 800a8a4:	2301      	movs	r3, #1
 800a8a6:	f44f 7080 	mov.w	r0, #256	; 0x100
 800a8aa:	f001 fbeb 	bl	800c084 <Show_Str6x8>
 800a8ae:	4b0d      	ldr	r3, [pc, #52]	; (800a8e4 <ShowMeasureStr+0x1a4>)
 800a8b0:	2200      	movs	r2, #0
 800a8b2:	9300      	str	r3, [sp, #0]
 800a8b4:	2114      	movs	r1, #20
 800a8b6:	2301      	movs	r3, #1
 800a8b8:	f44f 7083 	mov.w	r0, #262	; 0x106
 800a8bc:	f001 fbe2 	bl	800c084 <Show_Str6x8>
 800a8c0:	b002      	add	sp, #8
 800a8c2:	bd10      	pop	{r4, pc}
 800a8c4:	080138b5 	.word	0x080138b5
 800a8c8:	080138bc 	.word	0x080138bc
 800a8cc:	080138c4 	.word	0x080138c4
 800a8d0:	080138cc 	.word	0x080138cc
 800a8d4:	080138d4 	.word	0x080138d4
 800a8d8:	080138dc 	.word	0x080138dc
 800a8dc:	080138e6 	.word	0x080138e6
 800a8e0:	080138f0 	.word	0x080138f0
 800a8e4:	080138f2 	.word	0x080138f2

0800a8e8 <ShowSmallWins>:
 800a8e8:	b573      	push	{r0, r1, r4, r5, r6, lr}
 800a8ea:	2308      	movs	r3, #8
 800a8ec:	2400      	movs	r4, #0
 800a8ee:	f44f 72a0 	mov.w	r2, #320	; 0x140
 800a8f2:	9400      	str	r4, [sp, #0]
 800a8f4:	4619      	mov	r1, r3
 800a8f6:	4620      	mov	r0, r4
 800a8f8:	f7fe ff1b 	bl	8009732 <DrawRectangle>
 800a8fc:	4e0f      	ldr	r6, [pc, #60]	; (800a93c <ShowSmallWins+0x54>)
 800a8fe:	5da3      	ldrb	r3, [r4, r6]
 800a900:	b2a0      	uxth	r0, r4
 800a902:	b963      	cbnz	r3, 800a91e <ShowSmallWins+0x36>
 800a904:	210c      	movs	r1, #12
 800a906:	f001 f915 	bl	800bb34 <Set_Posi>
 800a90a:	f64b 50e0 	movw	r0, #48608	; 0xbde0
 800a90e:	f001 f91d 	bl	800bb4c <Set_Pixel>
 800a912:	3401      	adds	r4, #1
 800a914:	f5b4 7fa0 	cmp.w	r4, #320	; 0x140
 800a918:	d1f1      	bne.n	800a8fe <ShowSmallWins+0x16>
 800a91a:	b002      	add	sp, #8
 800a91c:	bd70      	pop	{r4, r5, r6, pc}
 800a91e:	2b01      	cmp	r3, #1
 800a920:	d1f7      	bne.n	800a912 <ShowSmallWins+0x2a>
 800a922:	2108      	movs	r1, #8
 800a924:	f001 f906 	bl	800bb34 <Set_Posi>
 800a928:	2508      	movs	r5, #8
 800a92a:	3d01      	subs	r5, #1
 800a92c:	f64b 50e0 	movw	r0, #48608	; 0xbde0
 800a930:	b2ad      	uxth	r5, r5
 800a932:	f001 f90b 	bl	800bb4c <Set_Pixel>
 800a936:	2d00      	cmp	r5, #0
 800a938:	d1f7      	bne.n	800a92a <ShowSmallWins+0x42>
 800a93a:	e7ea      	b.n	800a912 <ShowSmallWins+0x2a>
 800a93c:	20000728 	.word	0x20000728

0800a940 <SmallWinsInit>:
 800a940:	b507      	push	{r0, r1, r2, lr}
 800a942:	2308      	movs	r3, #8
 800a944:	2000      	movs	r0, #0
 800a946:	f44f 72a0 	mov.w	r2, #320	; 0x140
 800a94a:	9000      	str	r0, [sp, #0]
 800a94c:	4619      	mov	r1, r3
 800a94e:	f7fe fef0 	bl	8009732 <DrawRectangle>
 800a952:	b003      	add	sp, #12
 800a954:	f85d fb04 	ldr.w	pc, [sp], #4

0800a958 <ShowAllMenu>:
 800a958:	b513      	push	{r0, r1, r4, lr}
 800a95a:	f643 14e7 	movw	r4, #14823	; 0x39e7
 800a95e:	2314      	movs	r3, #20
 800a960:	9400      	str	r4, [sp, #0]
 800a962:	f44f 72a0 	mov.w	r2, #320	; 0x140
 800a966:	21dc      	movs	r1, #220	; 0xdc
 800a968:	2000      	movs	r0, #0
 800a96a:	f7fe fee2 	bl	8009732 <DrawRectangle>
 800a96e:	9400      	str	r4, [sp, #0]
 800a970:	2303      	movs	r3, #3
 800a972:	f44f 72a0 	mov.w	r2, #320	; 0x140
 800a976:	2101      	movs	r1, #1
 800a978:	2000      	movs	r0, #0
 800a97a:	f7fe feda 	bl	8009732 <DrawRectangle>
 800a97e:	f647 33ef 	movw	r3, #31727	; 0x7bef
 800a982:	2226      	movs	r2, #38	; 0x26
 800a984:	2138      	movs	r1, #56	; 0x38
 800a986:	9300      	str	r3, [sp, #0]
 800a988:	2000      	movs	r0, #0
 800a98a:	230d      	movs	r3, #13
 800a98c:	f7fe fed1 	bl	8009732 <DrawRectangle>
 800a990:	2000      	movs	r0, #0
 800a992:	f7ff fa23 	bl	8009ddc <ShowSmplStatus>
 800a996:	f7ff fae1 	bl	8009f5c <ShowBattery>
 800a99a:	f7ff fb57 	bl	800a04c <ShowTopTitle>
 800a99e:	f7fe feee 	bl	800977e <ShowLeftTitle>
 800a9a2:	f7fe fa20 	bl	8008de6 <DrawTriggerIco>
 800a9a6:	f7ff fd8d 	bl	800a4c4 <ShowTimeBaseStr>
 800a9aa:	4b09      	ldr	r3, [pc, #36]	; (800a9d0 <ShowAllMenu+0x78>)
 800a9ac:	f44f 728c 	mov.w	r2, #280	; 0x118
 800a9b0:	7d1b      	ldrb	r3, [r3, #20]
 800a9b2:	2137      	movs	r1, #55	; 0x37
 800a9b4:	2029      	movs	r0, #41	; 0x29
 800a9b6:	f7ff fcfd 	bl	800a3b4 <ShowScaleRuleTime>
 800a9ba:	f7ff fec1 	bl	800a740 <ShowMeasureStr>
 800a9be:	f7ff fdf3 	bl	800a5a8 <ShowTimeMarkValue>
 800a9c2:	f001 ff71 	bl	800c8a8 <ShowMeasureValue>
 800a9c6:	b002      	add	sp, #8
 800a9c8:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800a9cc:	f7ff bfb8 	b.w	800a940 <SmallWinsInit>
 800a9d0:	2000000c 	.word	0x2000000c

0800a9d4 <ProcessSmallWins>:
 800a9d4:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800a9d8:	f8df b0c8 	ldr.w	fp, [pc, #200]	; 800aaa4 <ProcessSmallWins+0xd0>
 800a9dc:	f44f 72a0 	mov.w	r2, #320	; 0x140
 800a9e0:	e9db 4500 	ldrd	r4, r5, [fp]
 800a9e4:	2300      	movs	r3, #0
 800a9e6:	4620      	mov	r0, r4
 800a9e8:	4629      	mov	r1, r5
 800a9ea:	f007 ff19 	bl	8012820 <__aeabi_uldivmod>
 800a9ee:	4620      	mov	r0, r4
 800a9f0:	4313      	orrs	r3, r2
 800a9f2:	4629      	mov	r1, r5
 800a9f4:	f04f 0300 	mov.w	r3, #0
 800a9f8:	f44f 72a0 	mov.w	r2, #320	; 0x140
 800a9fc:	bf14      	ite	ne
 800a9fe:	2601      	movne	r6, #1
 800aa00:	2600      	moveq	r6, #0
 800aa02:	f007 ff0d 	bl	8012820 <__aeabi_uldivmod>
 800aa06:	2700      	movs	r7, #0
 800aa08:	f04f 0a0b 	mov.w	sl, #11
 800aa0c:	1833      	adds	r3, r6, r0
 800aa0e:	eb47 0401 	adc.w	r4, r7, r1
 800aa12:	e9cd 3400 	strd	r3, r4, [sp]
 800aa16:	465e      	mov	r6, fp
 800aa18:	f04f 0800 	mov.w	r8, #0
 800aa1c:	f04f 0900 	mov.w	r9, #0
 800aa20:	4655      	mov	r5, sl
 800aa22:	2401      	movs	r4, #1
 800aa24:	f04f 0b00 	mov.w	fp, #0
 800aa28:	9b00      	ldr	r3, [sp, #0]
 800aa2a:	b2a0      	uxth	r0, r4
 800aa2c:	fba0 2303 	umull	r2, r3, r0, r3
 800aa30:	9f01      	ldr	r7, [sp, #4]
 800aa32:	fb00 3307 	mla	r3, r0, r7, r3
 800aa36:	4599      	cmp	r9, r3
 800aa38:	bf08      	it	eq
 800aa3a:	4590      	cmpeq	r8, r2
 800aa3c:	d321      	bcc.n	800aa82 <ProcessSmallWins+0xae>
 800aa3e:	4a18      	ldr	r2, [pc, #96]	; (800aaa0 <ProcessSmallWins+0xcc>)
 800aa40:	1e63      	subs	r3, r4, #1
 800aa42:	4555      	cmp	r5, sl
 800aa44:	eb02 0a03 	add.w	sl, r2, r3
 800aa48:	d102      	bne.n	800aa50 <ProcessSmallWins+0x7c>
 800aa4a:	f88a b4e8 	strb.w	fp, [sl, #1256]	; 0x4e8
 800aa4e:	e008      	b.n	800aa62 <ProcessSmallWins+0x8e>
 800aa50:	f04f 0301 	mov.w	r3, #1
 800aa54:	4628      	mov	r0, r5
 800aa56:	f88a 34e8 	strb.w	r3, [sl, #1256]	; 0x4e8
 800aa5a:	f001 fd29 	bl	800c4b0 <TransStatusInfo>
 800aa5e:	0743      	lsls	r3, r0, #29
 800aa60:	d4f3      	bmi.n	800aa4a <ProcessSmallWins+0x76>
 800aa62:	46aa      	mov	sl, r5
 800aa64:	3401      	adds	r4, #1
 800aa66:	b2a4      	uxth	r4, r4
 800aa68:	f240 1341 	movw	r3, #321	; 0x141
 800aa6c:	429c      	cmp	r4, r3
 800aa6e:	d1db      	bne.n	800aa28 <ProcessSmallWins+0x54>
 800aa70:	e9d6 2300 	ldrd	r2, r3, [r6]
 800aa74:	4313      	orrs	r3, r2
 800aa76:	d00e      	beq.n	800aa96 <ProcessSmallWins+0xc2>
 800aa78:	b003      	add	sp, #12
 800aa7a:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800aa7e:	f7ff bf33 	b.w	800a8e8 <ShowSmallWins>
 800aa82:	4628      	mov	r0, r5
 800aa84:	f001 fd1a 	bl	800c4bc <TransformTime>
 800aa88:	3501      	adds	r5, #1
 800aa8a:	eb18 0800 	adds.w	r8, r8, r0
 800aa8e:	f149 0900 	adc.w	r9, r9, #0
 800aa92:	b2ad      	uxth	r5, r5
 800aa94:	e7e8      	b.n	800aa68 <ProcessSmallWins+0x94>
 800aa96:	b003      	add	sp, #12
 800aa98:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800aa9c:	f7ff bf50 	b.w	800a940 <SmallWinsInit>
 800aaa0:	20000240 	.word	0x20000240
 800aaa4:	20004c30 	.word	0x20004c30

0800aaa8 <Show_PIO_Name>:
 800aaa8:	e92d 41ff 	stmdb	sp!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, lr}
 800aaac:	2a03      	cmp	r2, #3
 800aaae:	d820      	bhi.n	800aaf2 <Show_PIO_Name+0x4a>
 800aab0:	e8df f002 	tbb	[pc, r2]
 800aab4:	5c3f2202 	.word	0x5c3f2202
 800aab8:	2600      	movs	r6, #0
 800aaba:	f04f 080e 	mov.w	r8, #14
 800aabe:	312e      	adds	r1, #46	; 0x2e
 800aac0:	1d85      	adds	r5, r0, #6
 800aac2:	4f39      	ldr	r7, [pc, #228]	; (800aba8 <Show_PIO_Name+0x100>)
 800aac4:	b28c      	uxth	r4, r1
 800aac6:	b2ad      	uxth	r5, r5
 800aac8:	fb08 7306 	mla	r3, r8, r6, r7
 800aacc:	f04f 0c01 	mov.w	ip, #1
 800aad0:	9302      	str	r3, [sp, #8]
 800aad2:	2300      	movs	r3, #0
 800aad4:	4621      	mov	r1, r4
 800aad6:	e9cd 3c00 	strd	r3, ip, [sp]
 800aada:	f647 32ef 	movw	r2, #31727	; 0x7bef
 800aade:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800aae2:	4628      	mov	r0, r5
 800aae4:	3601      	adds	r6, #1
 800aae6:	f001 f94f 	bl	800bd88 <Disp_Str6x8>
 800aaea:	3c0e      	subs	r4, #14
 800aaec:	2e04      	cmp	r6, #4
 800aaee:	b2a4      	uxth	r4, r4
 800aaf0:	d1ea      	bne.n	800aac8 <Show_PIO_Name+0x20>
 800aaf2:	b004      	add	sp, #16
 800aaf4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800aaf8:	2600      	movs	r6, #0
 800aafa:	f04f 080e 	mov.w	r8, #14
 800aafe:	312e      	adds	r1, #46	; 0x2e
 800ab00:	1d85      	adds	r5, r0, #6
 800ab02:	4f2a      	ldr	r7, [pc, #168]	; (800abac <Show_PIO_Name+0x104>)
 800ab04:	b28c      	uxth	r4, r1
 800ab06:	b2ad      	uxth	r5, r5
 800ab08:	fb08 7306 	mla	r3, r8, r6, r7
 800ab0c:	2001      	movs	r0, #1
 800ab0e:	9302      	str	r3, [sp, #8]
 800ab10:	2300      	movs	r3, #0
 800ab12:	4621      	mov	r1, r4
 800ab14:	e9cd 3000 	strd	r3, r0, [sp]
 800ab18:	f647 32ef 	movw	r2, #31727	; 0x7bef
 800ab1c:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800ab20:	4628      	mov	r0, r5
 800ab22:	3601      	adds	r6, #1
 800ab24:	f001 f930 	bl	800bd88 <Disp_Str6x8>
 800ab28:	3c0e      	subs	r4, #14
 800ab2a:	2e04      	cmp	r6, #4
 800ab2c:	b2a4      	uxth	r4, r4
 800ab2e:	d1eb      	bne.n	800ab08 <Show_PIO_Name+0x60>
 800ab30:	e7df      	b.n	800aaf2 <Show_PIO_Name+0x4a>
 800ab32:	2600      	movs	r6, #0
 800ab34:	f04f 080e 	mov.w	r8, #14
 800ab38:	312e      	adds	r1, #46	; 0x2e
 800ab3a:	1d85      	adds	r5, r0, #6
 800ab3c:	4f1c      	ldr	r7, [pc, #112]	; (800abb0 <Show_PIO_Name+0x108>)
 800ab3e:	b28c      	uxth	r4, r1
 800ab40:	b2ad      	uxth	r5, r5
 800ab42:	fb08 7306 	mla	r3, r8, r6, r7
 800ab46:	2101      	movs	r1, #1
 800ab48:	9302      	str	r3, [sp, #8]
 800ab4a:	2300      	movs	r3, #0
 800ab4c:	f647 32ef 	movw	r2, #31727	; 0x7bef
 800ab50:	e9cd 3100 	strd	r3, r1, [sp]
 800ab54:	4628      	mov	r0, r5
 800ab56:	4621      	mov	r1, r4
 800ab58:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800ab5c:	3601      	adds	r6, #1
 800ab5e:	f001 f913 	bl	800bd88 <Disp_Str6x8>
 800ab62:	3c0e      	subs	r4, #14
 800ab64:	2e04      	cmp	r6, #4
 800ab66:	b2a4      	uxth	r4, r4
 800ab68:	d1eb      	bne.n	800ab42 <Show_PIO_Name+0x9a>
 800ab6a:	e7c2      	b.n	800aaf2 <Show_PIO_Name+0x4a>
 800ab6c:	2600      	movs	r6, #0
 800ab6e:	f04f 080e 	mov.w	r8, #14
 800ab72:	312e      	adds	r1, #46	; 0x2e
 800ab74:	1d85      	adds	r5, r0, #6
 800ab76:	4f0f      	ldr	r7, [pc, #60]	; (800abb4 <Show_PIO_Name+0x10c>)
 800ab78:	b28c      	uxth	r4, r1
 800ab7a:	b2ad      	uxth	r5, r5
 800ab7c:	fb08 7306 	mla	r3, r8, r6, r7
 800ab80:	2201      	movs	r2, #1
 800ab82:	9302      	str	r3, [sp, #8]
 800ab84:	2300      	movs	r3, #0
 800ab86:	4621      	mov	r1, r4
 800ab88:	e9cd 3200 	strd	r3, r2, [sp]
 800ab8c:	4628      	mov	r0, r5
 800ab8e:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800ab92:	f647 32ef 	movw	r2, #31727	; 0x7bef
 800ab96:	3601      	adds	r6, #1
 800ab98:	f001 f8f6 	bl	800bd88 <Disp_Str6x8>
 800ab9c:	3c0e      	subs	r4, #14
 800ab9e:	2e04      	cmp	r6, #4
 800aba0:	b2a4      	uxth	r4, r4
 800aba2:	d1eb      	bne.n	800ab7c <Show_PIO_Name+0xd4>
 800aba4:	e7a5      	b.n	800aaf2 <Show_PIO_Name+0x4a>
 800aba6:	bf00      	nop
 800aba8:	080132c8 	.word	0x080132c8
 800abac:	08013300 	.word	0x08013300
 800abb0:	08013338 	.word	0x08013338
 800abb4:	08013370 	.word	0x08013370

0800abb8 <ShowPopWindow>:
 800abb8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800abbc:	460d      	mov	r5, r1
 800abbe:	461e      	mov	r6, r3
 800abc0:	f04f 0b00 	mov.w	fp, #0
 800abc4:	b0a1      	sub	sp, #132	; 0x84
 800abc6:	f8bd 30a8 	ldrh.w	r3, [sp, #168]	; 0xa8
 800abca:	f10d 0824 	add.w	r8, sp, #36	; 0x24
 800abce:	9204      	str	r2, [sp, #16]
 800abd0:	4607      	mov	r7, r0
 800abd2:	225a      	movs	r2, #90	; 0x5a
 800abd4:	2100      	movs	r1, #0
 800abd6:	4640      	mov	r0, r8
 800abd8:	9305      	str	r3, [sp, #20]
 800abda:	f89d a0b0 	ldrb.w	sl, [sp, #176]	; 0xb0
 800abde:	f007 ffac 	bl	8012b3a <memset>
 800abe2:	4bba      	ldr	r3, [pc, #744]	; (800aecc <ShowPopWindow+0x314>)
 800abe4:	1bad      	subs	r5, r5, r6
 800abe6:	b2ad      	uxth	r5, r5
 800abe8:	781b      	ldrb	r3, [r3, #0]
 800abea:	ac06      	add	r4, sp, #24
 800abec:	f1a5 0904 	sub.w	r9, r5, #4
 800abf0:	f8cd b018 	str.w	fp, [sp, #24]
 800abf4:	fa1f f989 	uxth.w	r9, r9
 800abf8:	f8c4 b004 	str.w	fp, [r4, #4]
 800abfc:	f8a4 b008 	strh.w	fp, [r4, #8]
 800ac00:	2b00      	cmp	r3, #0
 800ac02:	f000 80ab 	beq.w	800ad5c <ShowPopWindow+0x1a4>
 800ac06:	9a04      	ldr	r2, [sp, #16]
 800ac08:	f8bd c0ac 	ldrh.w	ip, [sp, #172]	; 0xac
 800ac0c:	1eb3      	subs	r3, r6, #2
 800ac0e:	3a02      	subs	r2, #2
 800ac10:	1ee9      	subs	r1, r5, #3
 800ac12:	1c78      	adds	r0, r7, #1
 800ac14:	f8cd c000 	str.w	ip, [sp]
 800ac18:	b29b      	uxth	r3, r3
 800ac1a:	b292      	uxth	r2, r2
 800ac1c:	b289      	uxth	r1, r1
 800ac1e:	b280      	uxth	r0, r0
 800ac20:	f7fe fd87 	bl	8009732 <DrawRectangle>
 800ac24:	9b05      	ldr	r3, [sp, #20]
 800ac26:	2201      	movs	r2, #1
 800ac28:	9300      	str	r3, [sp, #0]
 800ac2a:	4649      	mov	r1, r9
 800ac2c:	4633      	mov	r3, r6
 800ac2e:	4638      	mov	r0, r7
 800ac30:	f7fe fd7f 	bl	8009732 <DrawRectangle>
 800ac34:	9b04      	ldr	r3, [sp, #16]
 800ac36:	2201      	movs	r2, #1
 800ac38:	1e58      	subs	r0, r3, #1
 800ac3a:	9b05      	ldr	r3, [sp, #20]
 800ac3c:	4438      	add	r0, r7
 800ac3e:	9300      	str	r3, [sp, #0]
 800ac40:	4649      	mov	r1, r9
 800ac42:	4633      	mov	r3, r6
 800ac44:	b280      	uxth	r0, r0
 800ac46:	f7fe fd74 	bl	8009732 <DrawRectangle>
 800ac4a:	9b05      	ldr	r3, [sp, #20]
 800ac4c:	9a04      	ldr	r2, [sp, #16]
 800ac4e:	9300      	str	r3, [sp, #0]
 800ac50:	4649      	mov	r1, r9
 800ac52:	2301      	movs	r3, #1
 800ac54:	4638      	mov	r0, r7
 800ac56:	f7fe fd6c 	bl	8009732 <DrawRectangle>
 800ac5a:	9b05      	ldr	r3, [sp, #20]
 800ac5c:	1e71      	subs	r1, r6, #1
 800ac5e:	4449      	add	r1, r9
 800ac60:	9300      	str	r3, [sp, #0]
 800ac62:	9a04      	ldr	r2, [sp, #16]
 800ac64:	2305      	movs	r3, #5
 800ac66:	b289      	uxth	r1, r1
 800ac68:	4638      	mov	r0, r7
 800ac6a:	f7fe fd62 	bl	8009732 <DrawRectangle>
 800ac6e:	f1ba 0f01 	cmp.w	sl, #1
 800ac72:	d173      	bne.n	800ad5c <ShowPopWindow+0x1a4>
 800ac74:	4d96      	ldr	r5, [pc, #600]	; (800aed0 <ShowPopWindow+0x318>)
 800ac76:	4b97      	ldr	r3, [pc, #604]	; (800aed4 <ShowPopWindow+0x31c>)
 800ac78:	8ba8      	ldrh	r0, [r5, #28]
 800ac7a:	229a      	movs	r2, #154	; 0x9a
 800ac7c:	3028      	adds	r0, #40	; 0x28
 800ac7e:	2142      	movs	r1, #66	; 0x42
 800ac80:	b280      	uxth	r0, r0
 800ac82:	f8cd b000 	str.w	fp, [sp]
 800ac86:	f001 f9d5 	bl	800c034 <RW_Row_Color>
 800ac8a:	8ae8      	ldrh	r0, [r5, #22]
 800ac8c:	4b92      	ldr	r3, [pc, #584]	; (800aed8 <ShowPopWindow+0x320>)
 800ac8e:	3028      	adds	r0, #40	; 0x28
 800ac90:	229a      	movs	r2, #154	; 0x9a
 800ac92:	2142      	movs	r1, #66	; 0x42
 800ac94:	b280      	uxth	r0, r0
 800ac96:	f8cd b000 	str.w	fp, [sp]
 800ac9a:	f001 f9cb 	bl	800c034 <RW_Row_Color>
 800ac9e:	8b28      	ldrh	r0, [r5, #24]
 800aca0:	4b8e      	ldr	r3, [pc, #568]	; (800aedc <ShowPopWindow+0x324>)
 800aca2:	3028      	adds	r0, #40	; 0x28
 800aca4:	229a      	movs	r2, #154	; 0x9a
 800aca6:	2142      	movs	r1, #66	; 0x42
 800aca8:	b280      	uxth	r0, r0
 800acaa:	f8cd b000 	str.w	fp, [sp]
 800acae:	f001 f9c1 	bl	800c034 <RW_Row_Color>
 800acb2:	8be8      	ldrh	r0, [r5, #30]
 800acb4:	4b8a      	ldr	r3, [pc, #552]	; (800aee0 <ShowPopWindow+0x328>)
 800acb6:	3028      	adds	r0, #40	; 0x28
 800acb8:	229a      	movs	r2, #154	; 0x9a
 800acba:	2142      	movs	r1, #66	; 0x42
 800acbc:	b280      	uxth	r0, r0
 800acbe:	f8cd b000 	str.w	fp, [sp]
 800acc2:	f001 f9b7 	bl	800c034 <RW_Row_Color>
 800acc6:	f44f 63fc 	mov.w	r3, #2016	; 0x7e0
 800acca:	f64f 76ff 	movw	r6, #65535	; 0xffff
 800acce:	8ba8      	ldrh	r0, [r5, #28]
 800acd0:	4652      	mov	r2, sl
 800acd2:	3028      	adds	r0, #40	; 0x28
 800acd4:	9300      	str	r3, [sp, #0]
 800acd6:	2142      	movs	r1, #66	; 0x42
 800acd8:	239a      	movs	r3, #154	; 0x9a
 800acda:	b280      	uxth	r0, r0
 800acdc:	f7fe fc2e 	bl	800953c <DrawDottedLine>
 800ace0:	8ae8      	ldrh	r0, [r5, #22]
 800ace2:	239a      	movs	r3, #154	; 0x9a
 800ace4:	3028      	adds	r0, #40	; 0x28
 800ace6:	4652      	mov	r2, sl
 800ace8:	2142      	movs	r1, #66	; 0x42
 800acea:	9600      	str	r6, [sp, #0]
 800acec:	b280      	uxth	r0, r0
 800acee:	f7fe fc25 	bl	800953c <DrawDottedLine>
 800acf2:	8b28      	ldrh	r0, [r5, #24]
 800acf4:	239a      	movs	r3, #154	; 0x9a
 800acf6:	3028      	adds	r0, #40	; 0x28
 800acf8:	4652      	mov	r2, sl
 800acfa:	2142      	movs	r1, #66	; 0x42
 800acfc:	9600      	str	r6, [sp, #0]
 800acfe:	b280      	uxth	r0, r0
 800ad00:	f7fe fc1c 	bl	800953c <DrawDottedLine>
 800ad04:	f240 531f 	movw	r3, #1311	; 0x51f
 800ad08:	8be8      	ldrh	r0, [r5, #30]
 800ad0a:	4652      	mov	r2, sl
 800ad0c:	3028      	adds	r0, #40	; 0x28
 800ad0e:	9300      	str	r3, [sp, #0]
 800ad10:	2142      	movs	r1, #66	; 0x42
 800ad12:	239a      	movs	r3, #154	; 0x9a
 800ad14:	b280      	uxth	r0, r0
 800ad16:	f7fe fc11 	bl	800953c <DrawDottedLine>
 800ad1a:	4a6c      	ldr	r2, [pc, #432]	; (800aecc <ShowPopWindow+0x314>)
 800ad1c:	4d71      	ldr	r5, [pc, #452]	; (800aee4 <ShowPopWindow+0x32c>)
 800ad1e:	7893      	ldrb	r3, [r2, #2]
 800ad20:	9204      	str	r2, [sp, #16]
 800ad22:	442b      	add	r3, r5
 800ad24:	f893 a004 	ldrb.w	sl, [r3, #4]
 800ad28:	4e69      	ldr	r6, [pc, #420]	; (800aed0 <ShowPopWindow+0x318>)
 800ad2a:	f1ba 0f01 	cmp.w	sl, #1
 800ad2e:	f885 a628 	strb.w	sl, [r5, #1576]	; 0x628
 800ad32:	d168      	bne.n	800ae06 <ShowPopWindow+0x24e>
 800ad34:	f44f 729a 	mov.w	r2, #308	; 0x134
 800ad38:	f105 0118 	add.w	r1, r5, #24
 800ad3c:	f205 602a 	addw	r0, r5, #1578	; 0x62a
 800ad40:	f007 fef0 	bl	8012b24 <memcpy>
 800ad44:	f8b5 375e 	ldrh.w	r3, [r5, #1886]	; 0x75e
 800ad48:	8af2      	ldrh	r2, [r6, #22]
 800ad4a:	f103 0028 	add.w	r0, r3, #40	; 0x28
 800ad4e:	429a      	cmp	r2, r3
 800ad50:	b280      	uxth	r0, r0
 800ad52:	f040 81ae 	bne.w	800b0b2 <ShowPopWindow+0x4fa>
 800ad56:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800ad5a:	e1a5      	b.n	800b0a8 <ShowPopWindow+0x4f0>
 800ad5c:	f1ba 0f06 	cmp.w	sl, #6
 800ad60:	f200 833e 	bhi.w	800b3e0 <ShowPopWindow+0x828>
 800ad64:	a301      	add	r3, pc, #4	; (adr r3, 800ad6c <ShowPopWindow+0x1b4>)
 800ad66:	f853 f02a 	ldr.w	pc, [r3, sl, lsl #2]
 800ad6a:	bf00      	nop
 800ad6c:	0800ad89 	.word	0x0800ad89
 800ad70:	0800ad1b 	.word	0x0800ad1b
 800ad74:	0800b22d 	.word	0x0800b22d
 800ad78:	0800b449 	.word	0x0800b449
 800ad7c:	0800b4a7 	.word	0x0800b4a7
 800ad80:	0800b56d 	.word	0x0800b56d
 800ad84:	0800b63b 	.word	0x0800b63b
 800ad88:	4b57      	ldr	r3, [pc, #348]	; (800aee8 <ShowPopWindow+0x330>)
 800ad8a:	2607      	movs	r6, #7
 800ad8c:	6818      	ldr	r0, [r3, #0]
 800ad8e:	4c50      	ldr	r4, [pc, #320]	; (800aed0 <ShowPopWindow+0x318>)
 800ad90:	9009      	str	r0, [sp, #36]	; 0x24
 800ad92:	2008      	movs	r0, #8
 800ad94:	799b      	ldrb	r3, [r3, #6]
 800ad96:	4955      	ldr	r1, [pc, #340]	; (800aeec <ShowPopWindow+0x334>)
 800ad98:	f888 3006 	strb.w	r3, [r8, #6]
 800ad9c:	8823      	ldrh	r3, [r4, #0]
 800ad9e:	eb03 0383 	add.w	r3, r3, r3, lsl #2
 800ada2:	440b      	add	r3, r1
 800ada4:	f8d3 352f 	ldr.w	r3, [r3, #1327]	; 0x52f
 800ada8:	f8cd 3026 	str.w	r3, [sp, #38]	; 0x26
 800adac:	4b47      	ldr	r3, [pc, #284]	; (800aecc <ShowPopWindow+0x314>)
 800adae:	789a      	ldrb	r2, [r3, #2]
 800adb0:	4b4c      	ldr	r3, [pc, #304]	; (800aee4 <ShowPopWindow+0x32c>)
 800adb2:	4413      	add	r3, r2
 800adb4:	440a      	add	r2, r1
 800adb6:	f201 71a3 	addw	r1, r1, #1955	; 0x7a3
 800adba:	791b      	ldrb	r3, [r3, #4]
 800adbc:	f892 279c 	ldrb.w	r2, [r2, #1948]	; 0x79c
 800adc0:	e9cd 0602 	strd	r0, r6, [sp, #8]
 800adc4:	9100      	str	r1, [sp, #0]
 800adc6:	4638      	mov	r0, r7
 800adc8:	4649      	mov	r1, r9
 800adca:	f8cd 8004 	str.w	r8, [sp, #4]
 800adce:	f7fe fab3 	bl	8009338 <ShowPopStr>
 800add2:	2301      	movs	r3, #1
 800add4:	4a46      	ldr	r2, [pc, #280]	; (800aef0 <ShowPopWindow+0x338>)
 800add6:	8824      	ldrh	r4, [r4, #0]
 800add8:	e9cd 3201 	strd	r3, r2, [sp, #4]
 800addc:	2320      	movs	r3, #32
 800adde:	f107 0068 	add.w	r0, r7, #104	; 0x68
 800ade2:	f105 010e 	add.w	r1, r5, #14
 800ade6:	9300      	str	r3, [sp, #0]
 800ade8:	b280      	uxth	r0, r0
 800adea:	b289      	uxth	r1, r1
 800adec:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800adf0:	b934      	cbnz	r4, 800ae00 <ShowPopWindow+0x248>
 800adf2:	f647 32ef 	movw	r2, #31727	; 0x7bef
 800adf6:	f000 ffc7 	bl	800bd88 <Disp_Str6x8>
 800adfa:	f7fe fcc0 	bl	800977e <ShowLeftTitle>
 800adfe:	e2ef      	b.n	800b3e0 <ShowPopWindow+0x828>
 800ae00:	f240 72ff 	movw	r2, #2047	; 0x7ff
 800ae04:	e7f7      	b.n	800adf6 <ShowPopWindow+0x23e>
 800ae06:	f1ba 0f02 	cmp.w	sl, #2
 800ae0a:	d173      	bne.n	800aef4 <ShowPopWindow+0x33c>
 800ae0c:	f44f 729a 	mov.w	r2, #308	; 0x134
 800ae10:	f505 71a6 	add.w	r1, r5, #332	; 0x14c
 800ae14:	f205 602a 	addw	r0, r5, #1578	; 0x62a
 800ae18:	f007 fe84 	bl	8012b24 <memcpy>
 800ae1c:	f8b5 3760 	ldrh.w	r3, [r5, #1888]	; 0x760
 800ae20:	8b32      	ldrh	r2, [r6, #24]
 800ae22:	f103 0028 	add.w	r0, r3, #40	; 0x28
 800ae26:	429a      	cmp	r2, r3
 800ae28:	b280      	uxth	r0, r0
 800ae2a:	d094      	beq.n	800ad56 <ShowPopWindow+0x19e>
 800ae2c:	f04f 0a01 	mov.w	sl, #1
 800ae30:	f04f 0b00 	mov.w	fp, #0
 800ae34:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800ae38:	229a      	movs	r2, #154	; 0x9a
 800ae3a:	2142      	movs	r1, #66	; 0x42
 800ae3c:	f8cd a000 	str.w	sl, [sp]
 800ae40:	f001 f8f8 	bl	800c034 <RW_Row_Color>
 800ae44:	8b30      	ldrh	r0, [r6, #24]
 800ae46:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800ae4a:	3028      	adds	r0, #40	; 0x28
 800ae4c:	229a      	movs	r2, #154	; 0x9a
 800ae4e:	2142      	movs	r1, #66	; 0x42
 800ae50:	b280      	uxth	r0, r0
 800ae52:	f8cd b000 	str.w	fp, [sp]
 800ae56:	f001 f8ed 	bl	800c034 <RW_Row_Color>
 800ae5a:	f205 612a 	addw	r1, r5, #1578	; 0x62a
 800ae5e:	f44f 729a 	mov.w	r2, #308	; 0x134
 800ae62:	f505 70a6 	add.w	r0, r5, #332	; 0x14c
 800ae66:	f007 fe5d 	bl	8012b24 <memcpy>
 800ae6a:	9b04      	ldr	r3, [sp, #16]
 800ae6c:	789b      	ldrb	r3, [r3, #2]
 800ae6e:	442b      	add	r3, r5
 800ae70:	7918      	ldrb	r0, [r3, #4]
 800ae72:	f7ff fb4b 	bl	800a50c <BackupPreCursorPosition>
 800ae76:	f8b5 0760 	ldrh.w	r0, [r5, #1888]	; 0x760
 800ae7a:	f64f 75ff 	movw	r5, #65535	; 0xffff
 800ae7e:	3028      	adds	r0, #40	; 0x28
 800ae80:	465a      	mov	r2, fp
 800ae82:	2142      	movs	r1, #66	; 0x42
 800ae84:	b280      	uxth	r0, r0
 800ae86:	f7fe fbc3 	bl	8009610 <ClrCursor>
 800ae8a:	8af0      	ldrh	r0, [r6, #22]
 800ae8c:	239a      	movs	r3, #154	; 0x9a
 800ae8e:	3028      	adds	r0, #40	; 0x28
 800ae90:	4652      	mov	r2, sl
 800ae92:	2142      	movs	r1, #66	; 0x42
 800ae94:	9500      	str	r5, [sp, #0]
 800ae96:	b280      	uxth	r0, r0
 800ae98:	f7fe fb50 	bl	800953c <DrawDottedLine>
 800ae9c:	f44f 63fc 	mov.w	r3, #2016	; 0x7e0
 800aea0:	8bb0      	ldrh	r0, [r6, #28]
 800aea2:	4652      	mov	r2, sl
 800aea4:	3028      	adds	r0, #40	; 0x28
 800aea6:	9300      	str	r3, [sp, #0]
 800aea8:	2142      	movs	r1, #66	; 0x42
 800aeaa:	239a      	movs	r3, #154	; 0x9a
 800aeac:	b280      	uxth	r0, r0
 800aeae:	f7fe fb45 	bl	800953c <DrawDottedLine>
 800aeb2:	f240 531f 	movw	r3, #1311	; 0x51f
 800aeb6:	8bf0      	ldrh	r0, [r6, #30]
 800aeb8:	4652      	mov	r2, sl
 800aeba:	3028      	adds	r0, #40	; 0x28
 800aebc:	9300      	str	r3, [sp, #0]
 800aebe:	2142      	movs	r1, #66	; 0x42
 800aec0:	239a      	movs	r3, #154	; 0x9a
 800aec2:	b280      	uxth	r0, r0
 800aec4:	f7fe fb3a 	bl	800953c <DrawDottedLine>
 800aec8:	8b30      	ldrh	r0, [r6, #24]
 800aeca:	e13f      	b.n	800b14c <ShowPopWindow+0x594>
 800aecc:	20004c12 	.word	0x20004c12
 800aed0:	2000000c 	.word	0x2000000c
 800aed4:	200004c0 	.word	0x200004c0
 800aed8:	20000258 	.word	0x20000258
 800aedc:	2000038c 	.word	0x2000038c
 800aee0:	200005f4 	.word	0x200005f4
 800aee4:	20000240 	.word	0x20000240
 800aee8:	08012bc1 	.word	0x08012bc1
 800aeec:	08012c0c 	.word	0x08012c0c
 800aef0:	080138da 	.word	0x080138da
 800aef4:	f1ba 0f04 	cmp.w	sl, #4
 800aef8:	d07a      	beq.n	800aff0 <ShowPopWindow+0x438>
 800aefa:	f1ba 0f05 	cmp.w	sl, #5
 800aefe:	f000 812e 	beq.w	800b15e <ShowPopWindow+0x5a6>
 800af02:	f1ba 0f00 	cmp.w	sl, #0
 800af06:	d106      	bne.n	800af16 <ShowPopWindow+0x35e>
 800af08:	7d33      	ldrb	r3, [r6, #20]
 800af0a:	f44f 728c 	mov.w	r2, #280	; 0x118
 800af0e:	2137      	movs	r1, #55	; 0x37
 800af10:	2029      	movs	r0, #41	; 0x29
 800af12:	f7ff fa4f 	bl	800a3b4 <ShowScaleRuleTime>
 800af16:	f7fe fbab 	bl	8009670 <Draw_ScaleRule>
 800af1a:	4dbf      	ldr	r5, [pc, #764]	; (800b218 <ShowPopWindow+0x660>)
 800af1c:	8bb3      	ldrh	r3, [r6, #28]
 800af1e:	f8df a308 	ldr.w	sl, [pc, #776]	; 800b228 <ShowPopWindow+0x670>
 800af22:	f8a5 3762 	strh.w	r3, [r5, #1890]	; 0x762
 800af26:	8b33      	ldrh	r3, [r6, #24]
 800af28:	8ab1      	ldrh	r1, [r6, #20]
 800af2a:	f8a5 3760 	strh.w	r3, [r5, #1888]	; 0x760
 800af2e:	8bf3      	ldrh	r3, [r6, #30]
 800af30:	8af2      	ldrh	r2, [r6, #22]
 800af32:	f8a5 3764 	strh.w	r3, [r5, #1892]	; 0x764
 800af36:	f50a 63f6 	add.w	r3, sl, #1968	; 0x7b0
 800af3a:	eb03 03c1 	add.w	r3, r3, r1, lsl #3
 800af3e:	6818      	ldr	r0, [r3, #0]
 800af40:	8899      	ldrh	r1, [r3, #4]
 800af42:	799b      	ldrb	r3, [r3, #6]
 800af44:	9009      	str	r0, [sp, #36]	; 0x24
 800af46:	f888 3006 	strb.w	r3, [r8, #6]
 800af4a:	f8a8 1004 	strh.w	r1, [r8, #4]
 800af4e:	4620      	mov	r0, r4
 800af50:	49b2      	ldr	r1, [pc, #712]	; (800b21c <ShowPopWindow+0x664>)
 800af52:	f8a5 275e 	strh.w	r2, [r5, #1886]	; 0x75e
 800af56:	f004 fcb1 	bl	800f8bc <sprintf>
 800af5a:	49b1      	ldr	r1, [pc, #708]	; (800b220 <ShowPopWindow+0x668>)
 800af5c:	4620      	mov	r0, r4
 800af5e:	f007 fdf4 	bl	8012b4a <strcat>
 800af62:	9806      	ldr	r0, [sp, #24]
 800af64:	88a2      	ldrh	r2, [r4, #4]
 800af66:	79a3      	ldrb	r3, [r4, #6]
 800af68:	f8cd 002d 	str.w	r0, [sp, #45]	; 0x2d
 800af6c:	f8ad 2031 	strh.w	r2, [sp, #49]	; 0x31
 800af70:	49aa      	ldr	r1, [pc, #680]	; (800b21c <ShowPopWindow+0x664>)
 800af72:	8b32      	ldrh	r2, [r6, #24]
 800af74:	4620      	mov	r0, r4
 800af76:	f88d 3033 	strb.w	r3, [sp, #51]	; 0x33
 800af7a:	f004 fc9f 	bl	800f8bc <sprintf>
 800af7e:	49a8      	ldr	r1, [pc, #672]	; (800b220 <ShowPopWindow+0x668>)
 800af80:	4620      	mov	r0, r4
 800af82:	f007 fde2 	bl	8012b4a <strcat>
 800af86:	9806      	ldr	r0, [sp, #24]
 800af88:	79a3      	ldrb	r3, [r4, #6]
 800af8a:	f8cd 0036 	str.w	r0, [sp, #54]	; 0x36
 800af8e:	200a      	movs	r0, #10
 800af90:	f88d 303c 	strb.w	r3, [sp, #60]	; 0x3c
 800af94:	4ba3      	ldr	r3, [pc, #652]	; (800b224 <ShowPopWindow+0x66c>)
 800af96:	88a2      	ldrh	r2, [r4, #4]
 800af98:	6819      	ldr	r1, [r3, #0]
 800af9a:	f8ad 203a 	strh.w	r2, [sp, #58]	; 0x3a
 800af9e:	2303      	movs	r3, #3
 800afa0:	f20a 529d 	addw	r2, sl, #1437	; 0x59d
 800afa4:	4341      	muls	r1, r0
 800afa6:	4620      	mov	r0, r4
 800afa8:	f002 ff24 	bl	800ddf4 <u32ToStr_Unit>
 800afac:	499c      	ldr	r1, [pc, #624]	; (800b220 <ShowPopWindow+0x668>)
 800afae:	4620      	mov	r0, r4
 800afb0:	f007 fdcb 	bl	8012b4a <strcat>
 800afb4:	9806      	ldr	r0, [sp, #24]
 800afb6:	210b      	movs	r1, #11
 800afb8:	f8cd 003f 	str.w	r0, [sp, #63]	; 0x3f
 800afbc:	2007      	movs	r0, #7
 800afbe:	79a3      	ldrb	r3, [r4, #6]
 800afc0:	88a2      	ldrh	r2, [r4, #4]
 800afc2:	f88d 3045 	strb.w	r3, [sp, #69]	; 0x45
 800afc6:	9b04      	ldr	r3, [sp, #16]
 800afc8:	f8ad 2043 	strh.w	r2, [sp, #67]	; 0x43
 800afcc:	789a      	ldrb	r2, [r3, #2]
 800afce:	4415      	add	r5, r2
 800afd0:	4452      	add	r2, sl
 800afd2:	f892 279c 	ldrb.w	r2, [r2, #1948]	; 0x79c
 800afd6:	792b      	ldrb	r3, [r5, #4]
 800afd8:	e9cd 1002 	strd	r1, r0, [sp, #8]
 800afdc:	f8cd 8004 	str.w	r8, [sp, #4]
 800afe0:	f50a 6105 	add.w	r1, sl, #2128	; 0x850
 800afe4:	9100      	str	r1, [sp, #0]
 800afe6:	4638      	mov	r0, r7
 800afe8:	4649      	mov	r1, r9
 800afea:	f7fe f9a5 	bl	8009338 <ShowPopStr>
 800afee:	e1f7      	b.n	800b3e0 <ShowPopWindow+0x828>
 800aff0:	f8b5 3762 	ldrh.w	r3, [r5, #1890]	; 0x762
 800aff4:	8bb2      	ldrh	r2, [r6, #28]
 800aff6:	f103 0028 	add.w	r0, r3, #40	; 0x28
 800affa:	429a      	cmp	r2, r3
 800affc:	b280      	uxth	r0, r0
 800affe:	d051      	beq.n	800b0a4 <ShowPopWindow+0x4ec>
 800b000:	f04f 0a01 	mov.w	sl, #1
 800b004:	f04f 0b00 	mov.w	fp, #0
 800b008:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800b00c:	229a      	movs	r2, #154	; 0x9a
 800b00e:	2142      	movs	r1, #66	; 0x42
 800b010:	f8cd a000 	str.w	sl, [sp]
 800b014:	f001 f80e 	bl	800c034 <RW_Row_Color>
 800b018:	8bb0      	ldrh	r0, [r6, #28]
 800b01a:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800b01e:	3028      	adds	r0, #40	; 0x28
 800b020:	229a      	movs	r2, #154	; 0x9a
 800b022:	2142      	movs	r1, #66	; 0x42
 800b024:	b280      	uxth	r0, r0
 800b026:	f8cd b000 	str.w	fp, [sp]
 800b02a:	f001 f803 	bl	800c034 <RW_Row_Color>
 800b02e:	f205 612a 	addw	r1, r5, #1578	; 0x62a
 800b032:	f44f 729a 	mov.w	r2, #308	; 0x134
 800b036:	f505 7020 	add.w	r0, r5, #640	; 0x280
 800b03a:	f007 fd73 	bl	8012b24 <memcpy>
 800b03e:	9b04      	ldr	r3, [sp, #16]
 800b040:	789b      	ldrb	r3, [r3, #2]
 800b042:	442b      	add	r3, r5
 800b044:	7918      	ldrb	r0, [r3, #4]
 800b046:	f7ff fa61 	bl	800a50c <BackupPreCursorPosition>
 800b04a:	f8b5 0762 	ldrh.w	r0, [r5, #1890]	; 0x762
 800b04e:	f64f 75ff 	movw	r5, #65535	; 0xffff
 800b052:	3028      	adds	r0, #40	; 0x28
 800b054:	465a      	mov	r2, fp
 800b056:	2142      	movs	r1, #66	; 0x42
 800b058:	b280      	uxth	r0, r0
 800b05a:	f7fe fad9 	bl	8009610 <ClrCursor>
 800b05e:	8af0      	ldrh	r0, [r6, #22]
 800b060:	239a      	movs	r3, #154	; 0x9a
 800b062:	3028      	adds	r0, #40	; 0x28
 800b064:	4652      	mov	r2, sl
 800b066:	2142      	movs	r1, #66	; 0x42
 800b068:	9500      	str	r5, [sp, #0]
 800b06a:	b280      	uxth	r0, r0
 800b06c:	f7fe fa66 	bl	800953c <DrawDottedLine>
 800b070:	8b30      	ldrh	r0, [r6, #24]
 800b072:	239a      	movs	r3, #154	; 0x9a
 800b074:	3028      	adds	r0, #40	; 0x28
 800b076:	4652      	mov	r2, sl
 800b078:	2142      	movs	r1, #66	; 0x42
 800b07a:	9500      	str	r5, [sp, #0]
 800b07c:	b280      	uxth	r0, r0
 800b07e:	f7fe fa5d 	bl	800953c <DrawDottedLine>
 800b082:	f240 531f 	movw	r3, #1311	; 0x51f
 800b086:	8bf0      	ldrh	r0, [r6, #30]
 800b088:	4652      	mov	r2, sl
 800b08a:	3028      	adds	r0, #40	; 0x28
 800b08c:	9300      	str	r3, [sp, #0]
 800b08e:	2142      	movs	r1, #66	; 0x42
 800b090:	239a      	movs	r3, #154	; 0x9a
 800b092:	b280      	uxth	r0, r0
 800b094:	f7fe fa52 	bl	800953c <DrawDottedLine>
 800b098:	f44f 63fc 	mov.w	r3, #2016	; 0x7e0
 800b09c:	8bb0      	ldrh	r0, [r6, #28]
 800b09e:	3028      	adds	r0, #40	; 0x28
 800b0a0:	9300      	str	r3, [sp, #0]
 800b0a2:	e055      	b.n	800b150 <ShowPopWindow+0x598>
 800b0a4:	f44f 63fc 	mov.w	r3, #2016	; 0x7e0
 800b0a8:	9300      	str	r3, [sp, #0]
 800b0aa:	2201      	movs	r2, #1
 800b0ac:	239a      	movs	r3, #154	; 0x9a
 800b0ae:	2142      	movs	r1, #66	; 0x42
 800b0b0:	e052      	b.n	800b158 <ShowPopWindow+0x5a0>
 800b0b2:	f04f 0b00 	mov.w	fp, #0
 800b0b6:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800b0ba:	229a      	movs	r2, #154	; 0x9a
 800b0bc:	2142      	movs	r1, #66	; 0x42
 800b0be:	f8cd a000 	str.w	sl, [sp]
 800b0c2:	f000 ffb7 	bl	800c034 <RW_Row_Color>
 800b0c6:	8af0      	ldrh	r0, [r6, #22]
 800b0c8:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800b0cc:	3028      	adds	r0, #40	; 0x28
 800b0ce:	229a      	movs	r2, #154	; 0x9a
 800b0d0:	2142      	movs	r1, #66	; 0x42
 800b0d2:	b280      	uxth	r0, r0
 800b0d4:	f8cd b000 	str.w	fp, [sp]
 800b0d8:	f000 ffac 	bl	800c034 <RW_Row_Color>
 800b0dc:	f205 612a 	addw	r1, r5, #1578	; 0x62a
 800b0e0:	f44f 729a 	mov.w	r2, #308	; 0x134
 800b0e4:	f105 0018 	add.w	r0, r5, #24
 800b0e8:	f007 fd1c 	bl	8012b24 <memcpy>
 800b0ec:	9b04      	ldr	r3, [sp, #16]
 800b0ee:	789b      	ldrb	r3, [r3, #2]
 800b0f0:	442b      	add	r3, r5
 800b0f2:	7918      	ldrb	r0, [r3, #4]
 800b0f4:	f7ff fa0a 	bl	800a50c <BackupPreCursorPosition>
 800b0f8:	f8b5 075e 	ldrh.w	r0, [r5, #1886]	; 0x75e
 800b0fc:	465a      	mov	r2, fp
 800b0fe:	3028      	adds	r0, #40	; 0x28
 800b100:	2142      	movs	r1, #66	; 0x42
 800b102:	b280      	uxth	r0, r0
 800b104:	f7fe fa84 	bl	8009610 <ClrCursor>
 800b108:	f44f 63fc 	mov.w	r3, #2016	; 0x7e0
 800b10c:	f64f 75ff 	movw	r5, #65535	; 0xffff
 800b110:	8bb0      	ldrh	r0, [r6, #28]
 800b112:	4652      	mov	r2, sl
 800b114:	3028      	adds	r0, #40	; 0x28
 800b116:	9300      	str	r3, [sp, #0]
 800b118:	2142      	movs	r1, #66	; 0x42
 800b11a:	239a      	movs	r3, #154	; 0x9a
 800b11c:	b280      	uxth	r0, r0
 800b11e:	f7fe fa0d 	bl	800953c <DrawDottedLine>
 800b122:	8b30      	ldrh	r0, [r6, #24]
 800b124:	239a      	movs	r3, #154	; 0x9a
 800b126:	3028      	adds	r0, #40	; 0x28
 800b128:	4652      	mov	r2, sl
 800b12a:	2142      	movs	r1, #66	; 0x42
 800b12c:	9500      	str	r5, [sp, #0]
 800b12e:	b280      	uxth	r0, r0
 800b130:	f7fe fa04 	bl	800953c <DrawDottedLine>
 800b134:	f240 531f 	movw	r3, #1311	; 0x51f
 800b138:	8bf0      	ldrh	r0, [r6, #30]
 800b13a:	4652      	mov	r2, sl
 800b13c:	3028      	adds	r0, #40	; 0x28
 800b13e:	9300      	str	r3, [sp, #0]
 800b140:	2142      	movs	r1, #66	; 0x42
 800b142:	239a      	movs	r3, #154	; 0x9a
 800b144:	b280      	uxth	r0, r0
 800b146:	f7fe f9f9 	bl	800953c <DrawDottedLine>
 800b14a:	8af0      	ldrh	r0, [r6, #22]
 800b14c:	3028      	adds	r0, #40	; 0x28
 800b14e:	9500      	str	r5, [sp, #0]
 800b150:	239a      	movs	r3, #154	; 0x9a
 800b152:	4652      	mov	r2, sl
 800b154:	2142      	movs	r1, #66	; 0x42
 800b156:	b280      	uxth	r0, r0
 800b158:	f7fe f9f0 	bl	800953c <DrawDottedLine>
 800b15c:	e6db      	b.n	800af16 <ShowPopWindow+0x35e>
 800b15e:	f8b5 3764 	ldrh.w	r3, [r5, #1892]	; 0x764
 800b162:	8bf2      	ldrh	r2, [r6, #30]
 800b164:	f103 0028 	add.w	r0, r3, #40	; 0x28
 800b168:	429a      	cmp	r2, r3
 800b16a:	b280      	uxth	r0, r0
 800b16c:	d050      	beq.n	800b210 <ShowPopWindow+0x658>
 800b16e:	f04f 0a01 	mov.w	sl, #1
 800b172:	f04f 0b00 	mov.w	fp, #0
 800b176:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800b17a:	229a      	movs	r2, #154	; 0x9a
 800b17c:	2142      	movs	r1, #66	; 0x42
 800b17e:	f8cd a000 	str.w	sl, [sp]
 800b182:	f000 ff57 	bl	800c034 <RW_Row_Color>
 800b186:	8bf0      	ldrh	r0, [r6, #30]
 800b188:	f205 632a 	addw	r3, r5, #1578	; 0x62a
 800b18c:	3028      	adds	r0, #40	; 0x28
 800b18e:	229a      	movs	r2, #154	; 0x9a
 800b190:	2142      	movs	r1, #66	; 0x42
 800b192:	b280      	uxth	r0, r0
 800b194:	f8cd b000 	str.w	fp, [sp]
 800b198:	f000 ff4c 	bl	800c034 <RW_Row_Color>
 800b19c:	f205 612a 	addw	r1, r5, #1578	; 0x62a
 800b1a0:	f44f 729a 	mov.w	r2, #308	; 0x134
 800b1a4:	f505 706d 	add.w	r0, r5, #948	; 0x3b4
 800b1a8:	f007 fcbc 	bl	8012b24 <memcpy>
 800b1ac:	9b04      	ldr	r3, [sp, #16]
 800b1ae:	789b      	ldrb	r3, [r3, #2]
 800b1b0:	442b      	add	r3, r5
 800b1b2:	7918      	ldrb	r0, [r3, #4]
 800b1b4:	f7ff f9aa 	bl	800a50c <BackupPreCursorPosition>
 800b1b8:	f8b5 0764 	ldrh.w	r0, [r5, #1892]	; 0x764
 800b1bc:	465a      	mov	r2, fp
 800b1be:	3028      	adds	r0, #40	; 0x28
 800b1c0:	2142      	movs	r1, #66	; 0x42
 800b1c2:	b280      	uxth	r0, r0
 800b1c4:	f7fe fa24 	bl	8009610 <ClrCursor>
 800b1c8:	f44f 63fc 	mov.w	r3, #2016	; 0x7e0
 800b1cc:	f64f 75ff 	movw	r5, #65535	; 0xffff
 800b1d0:	8bb0      	ldrh	r0, [r6, #28]
 800b1d2:	4652      	mov	r2, sl
 800b1d4:	3028      	adds	r0, #40	; 0x28
 800b1d6:	9300      	str	r3, [sp, #0]
 800b1d8:	2142      	movs	r1, #66	; 0x42
 800b1da:	239a      	movs	r3, #154	; 0x9a
 800b1dc:	b280      	uxth	r0, r0
 800b1de:	f7fe f9ad 	bl	800953c <DrawDottedLine>
 800b1e2:	8af0      	ldrh	r0, [r6, #22]
 800b1e4:	239a      	movs	r3, #154	; 0x9a
 800b1e6:	3028      	adds	r0, #40	; 0x28
 800b1e8:	4652      	mov	r2, sl
 800b1ea:	2142      	movs	r1, #66	; 0x42
 800b1ec:	9500      	str	r5, [sp, #0]
 800b1ee:	b280      	uxth	r0, r0
 800b1f0:	f7fe f9a4 	bl	800953c <DrawDottedLine>
 800b1f4:	8b30      	ldrh	r0, [r6, #24]
 800b1f6:	239a      	movs	r3, #154	; 0x9a
 800b1f8:	3028      	adds	r0, #40	; 0x28
 800b1fa:	9500      	str	r5, [sp, #0]
 800b1fc:	4652      	mov	r2, sl
 800b1fe:	2142      	movs	r1, #66	; 0x42
 800b200:	b280      	uxth	r0, r0
 800b202:	f7fe f99b 	bl	800953c <DrawDottedLine>
 800b206:	8bf0      	ldrh	r0, [r6, #30]
 800b208:	f240 531f 	movw	r3, #1311	; 0x51f
 800b20c:	3028      	adds	r0, #40	; 0x28
 800b20e:	e747      	b.n	800b0a0 <ShowPopWindow+0x4e8>
 800b210:	f240 531f 	movw	r3, #1311	; 0x51f
 800b214:	e748      	b.n	800b0a8 <ShowPopWindow+0x4f0>
 800b216:	bf00      	nop
 800b218:	20000240 	.word	0x20000240
 800b21c:	080138fb 	.word	0x080138fb
 800b220:	08012bc6 	.word	0x08012bc6
 800b224:	20004c18 	.word	0x20004c18
 800b228:	08012c0c 	.word	0x08012c0c
 800b22c:	4ac4      	ldr	r2, [pc, #784]	; (800b540 <ShowPopWindow+0x988>)
 800b22e:	4bc5      	ldr	r3, [pc, #788]	; (800b544 <ShowPopWindow+0x98c>)
 800b230:	8d11      	ldrh	r1, [r2, #40]	; 0x28
 800b232:	4692      	mov	sl, r2
 800b234:	eb03 03c1 	add.w	r3, r3, r1, lsl #3
 800b238:	6818      	ldr	r0, [r3, #0]
 800b23a:	8899      	ldrh	r1, [r3, #4]
 800b23c:	799b      	ldrb	r3, [r3, #6]
 800b23e:	9009      	str	r0, [sp, #36]	; 0x24
 800b240:	f888 3006 	strb.w	r3, [r8, #6]
 800b244:	4bc0      	ldr	r3, [pc, #768]	; (800b548 <ShowPopWindow+0x990>)
 800b246:	f8a8 1004 	strh.w	r1, [r8, #4]
 800b24a:	881b      	ldrh	r3, [r3, #0]
 800b24c:	207d      	movs	r0, #125	; 0x7d
 800b24e:	f8ad 302d 	strh.w	r3, [sp, #45]	; 0x2d
 800b252:	267b      	movs	r6, #123	; 0x7b
 800b254:	2308      	movs	r3, #8
 800b256:	f10d 0127 	add.w	r1, sp, #39	; 0x27
 800b25a:	f8ba 202a 	ldrh.w	r2, [sl, #42]	; 0x2a
 800b25e:	411a      	asrs	r2, r3
 800b260:	07d2      	lsls	r2, r2, #31
 800b262:	bf4c      	ite	mi
 800b264:	54ce      	strbmi	r6, [r1, r3]
 800b266:	54c8      	strbpl	r0, [r1, r3]
 800b268:	3301      	adds	r3, #1
 800b26a:	2b0c      	cmp	r3, #12
 800b26c:	d1f5      	bne.n	800b25a <ShowPopWindow+0x6a2>
 800b26e:	2320      	movs	r3, #32
 800b270:	f88d 3033 	strb.w	r3, [sp, #51]	; 0x33
 800b274:	f242 0320 	movw	r3, #8224	; 0x2020
 800b278:	2024      	movs	r0, #36	; 0x24
 800b27a:	f8ad 3036 	strh.w	r3, [sp, #54]	; 0x36
 800b27e:	2678      	movs	r6, #120	; 0x78
 800b280:	2308      	movs	r3, #8
 800b282:	f108 010c 	add.w	r1, r8, #12
 800b286:	f8ba 202c 	ldrh.w	r2, [sl, #44]	; 0x2c
 800b28a:	411a      	asrs	r2, r3
 800b28c:	07d2      	lsls	r2, r2, #31
 800b28e:	bf4c      	ite	mi
 800b290:	54ce      	strbmi	r6, [r1, r3]
 800b292:	54c8      	strbpl	r0, [r1, r3]
 800b294:	3301      	adds	r3, #1
 800b296:	2b0c      	cmp	r3, #12
 800b298:	d1f5      	bne.n	800b286 <ShowPopWindow+0x6ce>
 800b29a:	2320      	movs	r3, #32
 800b29c:	210a      	movs	r1, #10
 800b29e:	f04f 0b00 	mov.w	fp, #0
 800b2a2:	f8ba 202e 	ldrh.w	r2, [sl, #46]	; 0x2e
 800b2a6:	4ea9      	ldr	r6, [pc, #676]	; (800b54c <ShowPopWindow+0x994>)
 800b2a8:	f88d 303c 	strb.w	r3, [sp, #60]	; 0x3c
 800b2ac:	f8ba 3014 	ldrh.w	r3, [sl, #20]
 800b2b0:	4351      	muls	r1, r2
 800b2b2:	eb06 0383 	add.w	r3, r6, r3, lsl #2
 800b2b6:	f8d3 065c 	ldr.w	r0, [r3, #1628]	; 0x65c
 800b2ba:	f8cd b018 	str.w	fp, [sp, #24]
 800b2be:	2303      	movs	r3, #3
 800b2c0:	f8c4 b004 	str.w	fp, [r4, #4]
 800b2c4:	f8a4 b008 	strh.w	fp, [r4, #8]
 800b2c8:	f206 529d 	addw	r2, r6, #1437	; 0x59d
 800b2cc:	4341      	muls	r1, r0
 800b2ce:	4620      	mov	r0, r4
 800b2d0:	f002 fd90 	bl	800ddf4 <u32ToStr_Unit>
 800b2d4:	499c      	ldr	r1, [pc, #624]	; (800b548 <ShowPopWindow+0x990>)
 800b2d6:	4620      	mov	r0, r4
 800b2d8:	f007 fc37 	bl	8012b4a <strcat>
 800b2dc:	9806      	ldr	r0, [sp, #24]
 800b2de:	88a2      	ldrh	r2, [r4, #4]
 800b2e0:	79a3      	ldrb	r3, [r4, #6]
 800b2e2:	f8cd 003f 	str.w	r0, [sp, #63]	; 0x3f
 800b2e6:	f8cd b018 	str.w	fp, [sp, #24]
 800b2ea:	4620      	mov	r0, r4
 800b2ec:	f8c4 b004 	str.w	fp, [r4, #4]
 800b2f0:	f8a4 b008 	strh.w	fp, [r4, #8]
 800b2f4:	f8ba 1030 	ldrh.w	r1, [sl, #48]	; 0x30
 800b2f8:	f8ad 2043 	strh.w	r2, [sp, #67]	; 0x43
 800b2fc:	f88d 3045 	strb.w	r3, [sp, #69]	; 0x45
 800b300:	f002 fe6f 	bl	800dfe2 <ValuedivTen2Str>
 800b304:	f242 0320 	movw	r3, #8224	; 0x2020
 800b308:	210b      	movs	r1, #11
 800b30a:	f04f 0b07 	mov.w	fp, #7
 800b30e:	f8ad 3048 	strh.w	r3, [sp, #72]	; 0x48
 800b312:	78a3      	ldrb	r3, [r4, #2]
 800b314:	8822      	ldrh	r2, [r4, #0]
 800b316:	f88d 304c 	strb.w	r3, [sp, #76]	; 0x4c
 800b31a:	4b8d      	ldr	r3, [pc, #564]	; (800b550 <ShowPopWindow+0x998>)
 800b31c:	f8ad 204a 	strh.w	r2, [sp, #74]	; 0x4a
 800b320:	881b      	ldrh	r3, [r3, #0]
 800b322:	4c8c      	ldr	r4, [pc, #560]	; (800b554 <ShowPopWindow+0x99c>)
 800b324:	f8ad 304d 	strh.w	r3, [sp, #77]	; 0x4d
 800b328:	4b8b      	ldr	r3, [pc, #556]	; (800b558 <ShowPopWindow+0x9a0>)
 800b32a:	4638      	mov	r0, r7
 800b32c:	789a      	ldrb	r2, [r3, #2]
 800b32e:	18a3      	adds	r3, r4, r2
 800b330:	791b      	ldrb	r3, [r3, #4]
 800b332:	4432      	add	r2, r6
 800b334:	e9cd 1b02 	strd	r1, fp, [sp, #8]
 800b338:	f606 01de 	addw	r1, r6, #2270	; 0x8de
 800b33c:	9100      	str	r1, [sp, #0]
 800b33e:	f892 279c 	ldrb.w	r2, [r2, #1948]	; 0x79c
 800b342:	f8cd 8004 	str.w	r8, [sp, #4]
 800b346:	4649      	mov	r1, r9
 800b348:	f7fd fff6 	bl	8009338 <ShowPopStr>
 800b34c:	79a3      	ldrb	r3, [r4, #6]
 800b34e:	2b01      	cmp	r3, #1
 800b350:	d149      	bne.n	800b3e6 <ShowPopWindow+0x82e>
 800b352:	f894 0766 	ldrb.w	r0, [r4, #1894]	; 0x766
 800b356:	f643 12e7 	movw	r2, #14823	; 0x39e7
 800b35a:	eb00 0040 	add.w	r0, r0, r0, lsl #1
 800b35e:	0040      	lsls	r0, r0, #1
 800b360:	21b4      	movs	r1, #180	; 0xb4
 800b362:	30ab      	adds	r0, #171	; 0xab
 800b364:	f7fe f9c8 	bl	80096f8 <DrawCursor>
 800b368:	f894 0767 	ldrb.w	r0, [r4, #1895]	; 0x767
 800b36c:	f643 12e7 	movw	r2, #14823	; 0x39e7
 800b370:	eb00 0040 	add.w	r0, r0, r0, lsl #1
 800b374:	0040      	lsls	r0, r0, #1
 800b376:	21c2      	movs	r1, #194	; 0xc2
 800b378:	30ab      	adds	r0, #171	; 0xab
 800b37a:	f7fe f9bd 	bl	80096f8 <DrawCursor>
 800b37e:	21c2      	movs	r1, #194	; 0xc2
 800b380:	f89a 002a 	ldrb.w	r0, [sl, #42]	; 0x2a
 800b384:	0043      	lsls	r3, r0, #1
 800b386:	441e      	add	r6, r3
 800b388:	f884 0767 	strb.w	r0, [r4, #1895]	; 0x767
 800b38c:	f8b6 2920 	ldrh.w	r2, [r6, #2336]	; 0x920
 800b390:	4418      	add	r0, r3
 800b392:	0040      	lsls	r0, r0, #1
 800b394:	2420      	movs	r4, #32
 800b396:	2601      	movs	r6, #1
 800b398:	30ab      	adds	r0, #171	; 0xab
 800b39a:	f8df 81cc 	ldr.w	r8, [pc, #460]	; 800b568 <ShowPopWindow+0x9b0>
 800b39e:	377a      	adds	r7, #122	; 0x7a
 800b3a0:	f7fe f9aa 	bl	80096f8 <DrawCursor>
 800b3a4:	b2bf      	uxth	r7, r7
 800b3a6:	f7fd fd1e 	bl	8008de6 <DrawTriggerIco>
 800b3aa:	f105 011c 	add.w	r1, r5, #28
 800b3ae:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b3b2:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800b3b6:	b289      	uxth	r1, r1
 800b3b8:	4638      	mov	r0, r7
 800b3ba:	f8cd 8008 	str.w	r8, [sp, #8]
 800b3be:	e9cd 4600 	strd	r4, r6, [sp]
 800b3c2:	f000 fce1 	bl	800bd88 <Disp_Str6x8>
 800b3c6:	f105 012a 	add.w	r1, r5, #42	; 0x2a
 800b3ca:	e9cd 6801 	strd	r6, r8, [sp, #4]
 800b3ce:	9400      	str	r4, [sp, #0]
 800b3d0:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b3d4:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800b3d8:	b289      	uxth	r1, r1
 800b3da:	4638      	mov	r0, r7
 800b3dc:	f000 fcd4 	bl	800bd88 <Disp_Str6x8>
 800b3e0:	b021      	add	sp, #132	; 0x84
 800b3e2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800b3e6:	f894 0767 	ldrb.w	r0, [r4, #1895]	; 0x767
 800b3ea:	2b02      	cmp	r3, #2
 800b3ec:	eb00 0040 	add.w	r0, r0, r0, lsl #1
 800b3f0:	ea4f 0040 	mov.w	r0, r0, lsl #1
 800b3f4:	f100 00ab 	add.w	r0, r0, #171	; 0xab
 800b3f8:	f643 12e7 	movw	r2, #14823	; 0x39e7
 800b3fc:	f04f 01c2 	mov.w	r1, #194	; 0xc2
 800b400:	d118      	bne.n	800b434 <ShowPopWindow+0x87c>
 800b402:	f7fe f979 	bl	80096f8 <DrawCursor>
 800b406:	f894 0766 	ldrb.w	r0, [r4, #1894]	; 0x766
 800b40a:	f643 12e7 	movw	r2, #14823	; 0x39e7
 800b40e:	eb00 0040 	add.w	r0, r0, r0, lsl #1
 800b412:	0040      	lsls	r0, r0, #1
 800b414:	30ab      	adds	r0, #171	; 0xab
 800b416:	21b4      	movs	r1, #180	; 0xb4
 800b418:	f7fe f96e 	bl	80096f8 <DrawCursor>
 800b41c:	f89a 002c 	ldrb.w	r0, [sl, #44]	; 0x2c
 800b420:	0043      	lsls	r3, r0, #1
 800b422:	441e      	add	r6, r3
 800b424:	f884 0766 	strb.w	r0, [r4, #1894]	; 0x766
 800b428:	f8b6 2920 	ldrh.w	r2, [r6, #2336]	; 0x920
 800b42c:	4418      	add	r0, r3
 800b42e:	0040      	lsls	r0, r0, #1
 800b430:	21b4      	movs	r1, #180	; 0xb4
 800b432:	e7af      	b.n	800b394 <ShowPopWindow+0x7dc>
 800b434:	f7fe f960 	bl	80096f8 <DrawCursor>
 800b438:	f894 0766 	ldrb.w	r0, [r4, #1894]	; 0x766
 800b43c:	f643 12e7 	movw	r2, #14823	; 0x39e7
 800b440:	eb00 0040 	add.w	r0, r0, r0, lsl #1
 800b444:	0040      	lsls	r0, r0, #1
 800b446:	e7f3      	b.n	800b430 <ShowPopWindow+0x878>
 800b448:	4c3d      	ldr	r4, [pc, #244]	; (800b540 <ShowPopWindow+0x988>)
 800b44a:	4940      	ldr	r1, [pc, #256]	; (800b54c <ShowPopWindow+0x994>)
 800b44c:	8fa3      	ldrh	r3, [r4, #60]	; 0x3c
 800b44e:	f601 122c 	addw	r2, r1, #2348	; 0x92c
 800b452:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 800b456:	4413      	add	r3, r2
 800b458:	6818      	ldr	r0, [r3, #0]
 800b45a:	f04f 0e06 	mov.w	lr, #6
 800b45e:	9009      	str	r0, [sp, #36]	; 0x24
 800b460:	8898      	ldrh	r0, [r3, #4]
 800b462:	799b      	ldrb	r3, [r3, #6]
 800b464:	f8a8 0004 	strh.w	r0, [r8, #4]
 800b468:	200b      	movs	r0, #11
 800b46a:	f888 3006 	strb.w	r3, [r8, #6]
 800b46e:	8fe3      	ldrh	r3, [r4, #62]	; 0x3e
 800b470:	ebc3 03c3 	rsb	r3, r3, r3, lsl #3
 800b474:	4413      	add	r3, r2
 800b476:	681a      	ldr	r2, [r3, #0]
 800b478:	f8cd 202d 	str.w	r2, [sp, #45]	; 0x2d
 800b47c:	889a      	ldrh	r2, [r3, #4]
 800b47e:	799b      	ldrb	r3, [r3, #6]
 800b480:	f8ad 2031 	strh.w	r2, [sp, #49]	; 0x31
 800b484:	f88d 3033 	strb.w	r3, [sp, #51]	; 0x33
 800b488:	4b33      	ldr	r3, [pc, #204]	; (800b558 <ShowPopWindow+0x9a0>)
 800b48a:	789a      	ldrb	r2, [r3, #2]
 800b48c:	4b31      	ldr	r3, [pc, #196]	; (800b554 <ShowPopWindow+0x99c>)
 800b48e:	4413      	add	r3, r2
 800b490:	440a      	add	r2, r1
 800b492:	791b      	ldrb	r3, [r3, #4]
 800b494:	f892 279c 	ldrb.w	r2, [r2, #1948]	; 0x79c
 800b498:	e9cd 0e02 	strd	r0, lr, [sp, #8]
 800b49c:	f8cd 8004 	str.w	r8, [sp, #4]
 800b4a0:	f601 1148 	addw	r1, r1, #2376	; 0x948
 800b4a4:	e59e      	b.n	800afe4 <ShowPopWindow+0x42c>
 800b4a6:	4b2d      	ldr	r3, [pc, #180]	; (800b55c <ShowPopWindow+0x9a4>)
 800b4a8:	4e25      	ldr	r6, [pc, #148]	; (800b540 <ShowPopWindow+0x988>)
 800b4aa:	6818      	ldr	r0, [r3, #0]
 800b4ac:	889a      	ldrh	r2, [r3, #4]
 800b4ae:	799b      	ldrb	r3, [r3, #6]
 800b4b0:	9009      	str	r0, [sp, #36]	; 0x24
 800b4b2:	f888 3006 	strb.w	r3, [r8, #6]
 800b4b6:	f8b6 3050 	ldrh.w	r3, [r6, #80]	; 0x50
 800b4ba:	4c24      	ldr	r4, [pc, #144]	; (800b54c <ShowPopWindow+0x994>)
 800b4bc:	f8a8 2004 	strh.w	r2, [r8, #4]
 800b4c0:	eb03 0283 	add.w	r2, r3, r3, lsl #2
 800b4c4:	4422      	add	r2, r4
 800b4c6:	f8d2 2589 	ldr.w	r2, [r2, #1417]	; 0x589
 800b4ca:	f107 0068 	add.w	r0, r7, #104	; 0x68
 800b4ce:	f105 0138 	add.w	r1, r5, #56	; 0x38
 800b4d2:	f8cd 2025 	str.w	r2, [sp, #37]	; 0x25
 800b4d6:	b280      	uxth	r0, r0
 800b4d8:	b289      	uxth	r1, r1
 800b4da:	2201      	movs	r2, #1
 800b4dc:	bb33      	cbnz	r3, 800b52c <ShowPopWindow+0x974>
 800b4de:	4b20      	ldr	r3, [pc, #128]	; (800b560 <ShowPopWindow+0x9a8>)
 800b4e0:	e9cd 2301 	strd	r2, r3, [sp, #4]
 800b4e4:	2320      	movs	r3, #32
 800b4e6:	f240 72ff 	movw	r2, #2047	; 0x7ff
 800b4ea:	9300      	str	r3, [sp, #0]
 800b4ec:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b4f0:	f000 fc4a 	bl	800bd88 <Disp_Str6x8>
 800b4f4:	f04f 0c07 	mov.w	ip, #7
 800b4f8:	2108      	movs	r1, #8
 800b4fa:	4b17      	ldr	r3, [pc, #92]	; (800b558 <ShowPopWindow+0x9a0>)
 800b4fc:	4638      	mov	r0, r7
 800b4fe:	789a      	ldrb	r2, [r3, #2]
 800b500:	4b14      	ldr	r3, [pc, #80]	; (800b554 <ShowPopWindow+0x99c>)
 800b502:	4414      	add	r4, r2
 800b504:	4413      	add	r3, r2
 800b506:	791b      	ldrb	r3, [r3, #4]
 800b508:	e9cd 1c02 	strd	r1, ip, [sp, #8]
 800b50c:	4915      	ldr	r1, [pc, #84]	; (800b564 <ShowPopWindow+0x9ac>)
 800b50e:	f894 279c 	ldrb.w	r2, [r4, #1948]	; 0x79c
 800b512:	9100      	str	r1, [sp, #0]
 800b514:	f8cd 8004 	str.w	r8, [sp, #4]
 800b518:	4649      	mov	r1, r9
 800b51a:	f7fd ff0d 	bl	8009338 <ShowPopStr>
 800b51e:	f896 2050 	ldrb.w	r2, [r6, #80]	; 0x50
 800b522:	4649      	mov	r1, r9
 800b524:	4638      	mov	r0, r7
 800b526:	f7ff fabf 	bl	800aaa8 <Show_PIO_Name>
 800b52a:	e466      	b.n	800adfa <ShowPopWindow+0x242>
 800b52c:	4b0e      	ldr	r3, [pc, #56]	; (800b568 <ShowPopWindow+0x9b0>)
 800b52e:	e9cd 2301 	strd	r2, r3, [sp, #4]
 800b532:	2320      	movs	r3, #32
 800b534:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800b538:	9300      	str	r3, [sp, #0]
 800b53a:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b53e:	e7d7      	b.n	800b4f0 <ShowPopWindow+0x938>
 800b540:	2000000c 	.word	0x2000000c
 800b544:	080134aa 	.word	0x080134aa
 800b548:	08012bc6 	.word	0x08012bc6
 800b54c:	08012c0c 	.word	0x08012c0c
 800b550:	080138ff 	.word	0x080138ff
 800b554:	20000240 	.word	0x20000240
 800b558:	20004c12 	.word	0x20004c12
 800b55c:	08012bc1 	.word	0x08012bc1
 800b560:	08013905 	.word	0x08013905
 800b564:	0801357b 	.word	0x0801357b
 800b568:	08013902 	.word	0x08013902
 800b56c:	4e7e      	ldr	r6, [pc, #504]	; (800b768 <ShowPopWindow+0xbb0>)
 800b56e:	497f      	ldr	r1, [pc, #508]	; (800b76c <ShowPopWindow+0xbb4>)
 800b570:	f8b6 2064 	ldrh.w	r2, [r6, #100]	; 0x64
 800b574:	4620      	mov	r0, r4
 800b576:	f004 f9a1 	bl	800f8bc <sprintf>
 800b57a:	497d      	ldr	r1, [pc, #500]	; (800b770 <ShowPopWindow+0xbb8>)
 800b57c:	4620      	mov	r0, r4
 800b57e:	f007 fae4 	bl	8012b4a <strcat>
 800b582:	9b06      	ldr	r3, [sp, #24]
 800b584:	f8b6 2066 	ldrh.w	r2, [r6, #102]	; 0x66
 800b588:	4978      	ldr	r1, [pc, #480]	; (800b76c <ShowPopWindow+0xbb4>)
 800b58a:	4620      	mov	r0, r4
 800b58c:	9309      	str	r3, [sp, #36]	; 0x24
 800b58e:	f004 f995 	bl	800f8bc <sprintf>
 800b592:	4977      	ldr	r1, [pc, #476]	; (800b770 <ShowPopWindow+0xbb8>)
 800b594:	4620      	mov	r0, r4
 800b596:	f007 fad8 	bl	8012b4a <strcat>
 800b59a:	9b06      	ldr	r3, [sp, #24]
 800b59c:	f8b6 2068 	ldrh.w	r2, [r6, #104]	; 0x68
 800b5a0:	4972      	ldr	r1, [pc, #456]	; (800b76c <ShowPopWindow+0xbb4>)
 800b5a2:	4620      	mov	r0, r4
 800b5a4:	f8cd 302d 	str.w	r3, [sp, #45]	; 0x2d
 800b5a8:	f004 f988 	bl	800f8bc <sprintf>
 800b5ac:	4970      	ldr	r1, [pc, #448]	; (800b770 <ShowPopWindow+0xbb8>)
 800b5ae:	4620      	mov	r0, r4
 800b5b0:	f007 facb 	bl	8012b4a <strcat>
 800b5b4:	f04f 0a01 	mov.w	sl, #1
 800b5b8:	f8b6 206a 	ldrh.w	r2, [r6, #106]	; 0x6a
 800b5bc:	2620      	movs	r6, #32
 800b5be:	9b06      	ldr	r3, [sp, #24]
 800b5c0:	496a      	ldr	r1, [pc, #424]	; (800b76c <ShowPopWindow+0xbb4>)
 800b5c2:	4620      	mov	r0, r4
 800b5c4:	f8cd 3036 	str.w	r3, [sp, #54]	; 0x36
 800b5c8:	f004 f978 	bl	800f8bc <sprintf>
 800b5cc:	4968      	ldr	r1, [pc, #416]	; (800b770 <ShowPopWindow+0xbb8>)
 800b5ce:	4620      	mov	r0, r4
 800b5d0:	f007 fabb 	bl	8012b4a <strcat>
 800b5d4:	f8df b1b8 	ldr.w	fp, [pc, #440]	; 800b790 <ShowPopWindow+0xbd8>
 800b5d8:	9b06      	ldr	r3, [sp, #24]
 800b5da:	f107 0468 	add.w	r4, r7, #104	; 0x68
 800b5de:	b2a4      	uxth	r4, r4
 800b5e0:	4629      	mov	r1, r5
 800b5e2:	f8cd 303f 	str.w	r3, [sp, #63]	; 0x3f
 800b5e6:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800b5ea:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b5ee:	4620      	mov	r0, r4
 800b5f0:	f8cd b008 	str.w	fp, [sp, #8]
 800b5f4:	e9cd 6a00 	strd	r6, sl, [sp]
 800b5f8:	f000 fbc6 	bl	800bd88 <Disp_Str6x8>
 800b5fc:	f105 010e 	add.w	r1, r5, #14
 800b600:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b604:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800b608:	b289      	uxth	r1, r1
 800b60a:	4620      	mov	r0, r4
 800b60c:	e9cd ab01 	strd	sl, fp, [sp, #4]
 800b610:	9600      	str	r6, [sp, #0]
 800b612:	f000 fbb9 	bl	800bd88 <Disp_Str6x8>
 800b616:	2504      	movs	r5, #4
 800b618:	200b      	movs	r0, #11
 800b61a:	4b56      	ldr	r3, [pc, #344]	; (800b774 <ShowPopWindow+0xbbc>)
 800b61c:	4956      	ldr	r1, [pc, #344]	; (800b778 <ShowPopWindow+0xbc0>)
 800b61e:	789a      	ldrb	r2, [r3, #2]
 800b620:	4b56      	ldr	r3, [pc, #344]	; (800b77c <ShowPopWindow+0xbc4>)
 800b622:	4413      	add	r3, r2
 800b624:	440a      	add	r2, r1
 800b626:	791b      	ldrb	r3, [r3, #4]
 800b628:	f892 279c 	ldrb.w	r2, [r2, #1948]	; 0x79c
 800b62c:	e9cd 0502 	strd	r0, r5, [sp, #8]
 800b630:	f8cd 8004 	str.w	r8, [sp, #4]
 800b634:	f601 117c 	addw	r1, r1, #2428	; 0x97c
 800b638:	e4d4      	b.n	800afe4 <ShowPopWindow+0x42c>
 800b63a:	f04f 0a0a 	mov.w	sl, #10
 800b63e:	4b50      	ldr	r3, [pc, #320]	; (800b780 <ShowPopWindow+0xbc8>)
 800b640:	4e49      	ldr	r6, [pc, #292]	; (800b768 <ShowPopWindow+0xbb0>)
 800b642:	6818      	ldr	r0, [r3, #0]
 800b644:	889a      	ldrh	r2, [r3, #4]
 800b646:	9009      	str	r0, [sp, #36]	; 0x24
 800b648:	799b      	ldrb	r3, [r3, #6]
 800b64a:	f8a8 2004 	strh.w	r2, [r8, #4]
 800b64e:	f8ad 2031 	strh.w	r2, [sp, #49]	; 0x31
 800b652:	f8b6 207c 	ldrh.w	r2, [r6, #124]	; 0x7c
 800b656:	f888 3006 	strb.w	r3, [r8, #6]
 800b65a:	fb0a f202 	mul.w	r2, sl, r2
 800b65e:	f8cd 002d 	str.w	r0, [sp, #45]	; 0x2d
 800b662:	4948      	ldr	r1, [pc, #288]	; (800b784 <ShowPopWindow+0xbcc>)
 800b664:	4620      	mov	r0, r4
 800b666:	f88d 3033 	strb.w	r3, [sp, #51]	; 0x33
 800b66a:	f004 f927 	bl	800f8bc <sprintf>
 800b66e:	4946      	ldr	r1, [pc, #280]	; (800b788 <ShowPopWindow+0xbd0>)
 800b670:	4620      	mov	r0, r4
 800b672:	f007 fa6a 	bl	8012b4a <strcat>
 800b676:	88a2      	ldrh	r2, [r4, #4]
 800b678:	9806      	ldr	r0, [sp, #24]
 800b67a:	f8ad 203a 	strh.w	r2, [sp, #58]	; 0x3a
 800b67e:	f8b6 207e 	ldrh.w	r2, [r6, #126]	; 0x7e
 800b682:	79a3      	ldrb	r3, [r4, #6]
 800b684:	fb0a f202 	mul.w	r2, sl, r2
 800b688:	f8cd 0036 	str.w	r0, [sp, #54]	; 0x36
 800b68c:	493d      	ldr	r1, [pc, #244]	; (800b784 <ShowPopWindow+0xbcc>)
 800b68e:	4620      	mov	r0, r4
 800b690:	f88d 303c 	strb.w	r3, [sp, #60]	; 0x3c
 800b694:	f004 f912 	bl	800f8bc <sprintf>
 800b698:	493b      	ldr	r1, [pc, #236]	; (800b788 <ShowPopWindow+0xbd0>)
 800b69a:	4620      	mov	r0, r4
 800b69c:	f007 fa55 	bl	8012b4a <strcat>
 800b6a0:	88a2      	ldrh	r2, [r4, #4]
 800b6a2:	9806      	ldr	r0, [sp, #24]
 800b6a4:	79a3      	ldrb	r3, [r4, #6]
 800b6a6:	f8cd 003f 	str.w	r0, [sp, #63]	; 0x3f
 800b6aa:	f8ad 2043 	strh.w	r2, [sp, #67]	; 0x43
 800b6ae:	492f      	ldr	r1, [pc, #188]	; (800b76c <ShowPopWindow+0xbb4>)
 800b6b0:	f8b6 2080 	ldrh.w	r2, [r6, #128]	; 0x80
 800b6b4:	4620      	mov	r0, r4
 800b6b6:	f88d 3045 	strb.w	r3, [sp, #69]	; 0x45
 800b6ba:	f004 f8ff 	bl	800f8bc <sprintf>
 800b6be:	4933      	ldr	r1, [pc, #204]	; (800b78c <ShowPopWindow+0xbd4>)
 800b6c0:	4620      	mov	r0, r4
 800b6c2:	f007 fa42 	bl	8012b4a <strcat>
 800b6c6:	e894 0003 	ldmia.w	r4, {r0, r1}
 800b6ca:	f8b6 2082 	ldrh.w	r2, [r6, #130]	; 0x82
 800b6ce:	f04f 0a01 	mov.w	sl, #1
 800b6d2:	2620      	movs	r6, #32
 800b6d4:	f8ad 104c 	strh.w	r1, [sp, #76]	; 0x4c
 800b6d8:	0c09      	lsrs	r1, r1, #16
 800b6da:	9012      	str	r0, [sp, #72]	; 0x48
 800b6dc:	f88d 104e 	strb.w	r1, [sp, #78]	; 0x4e
 800b6e0:	4620      	mov	r0, r4
 800b6e2:	4922      	ldr	r1, [pc, #136]	; (800b76c <ShowPopWindow+0xbb4>)
 800b6e4:	f004 f8ea 	bl	800f8bc <sprintf>
 800b6e8:	4928      	ldr	r1, [pc, #160]	; (800b78c <ShowPopWindow+0xbd4>)
 800b6ea:	4620      	mov	r0, r4
 800b6ec:	f007 fa2d 	bl	8012b4a <strcat>
 800b6f0:	f8df b09c 	ldr.w	fp, [pc, #156]	; 800b790 <ShowPopWindow+0xbd8>
 800b6f4:	88a2      	ldrh	r2, [r4, #4]
 800b6f6:	79a3      	ldrb	r3, [r4, #6]
 800b6f8:	9806      	ldr	r0, [sp, #24]
 800b6fa:	f107 047a 	add.w	r4, r7, #122	; 0x7a
 800b6fe:	b2a4      	uxth	r4, r4
 800b700:	f105 0138 	add.w	r1, r5, #56	; 0x38
 800b704:	f8cd 0051 	str.w	r0, [sp, #81]	; 0x51
 800b708:	f8ad 2055 	strh.w	r2, [sp, #85]	; 0x55
 800b70c:	4620      	mov	r0, r4
 800b70e:	f88d 3057 	strb.w	r3, [sp, #87]	; 0x57
 800b712:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800b716:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b71a:	b289      	uxth	r1, r1
 800b71c:	f8cd b008 	str.w	fp, [sp, #8]
 800b720:	e9cd 6a00 	strd	r6, sl, [sp]
 800b724:	f000 fb30 	bl	800bd88 <Disp_Str6x8>
 800b728:	f105 0146 	add.w	r1, r5, #70	; 0x46
 800b72c:	4620      	mov	r0, r4
 800b72e:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b732:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800b736:	b289      	uxth	r1, r1
 800b738:	e9cd ab01 	strd	sl, fp, [sp, #4]
 800b73c:	9600      	str	r6, [sp, #0]
 800b73e:	f000 fb23 	bl	800bd88 <Disp_Str6x8>
 800b742:	2407      	movs	r4, #7
 800b744:	200b      	movs	r0, #11
 800b746:	4b0b      	ldr	r3, [pc, #44]	; (800b774 <ShowPopWindow+0xbbc>)
 800b748:	490b      	ldr	r1, [pc, #44]	; (800b778 <ShowPopWindow+0xbc0>)
 800b74a:	789a      	ldrb	r2, [r3, #2]
 800b74c:	4b0b      	ldr	r3, [pc, #44]	; (800b77c <ShowPopWindow+0xbc4>)
 800b74e:	4413      	add	r3, r2
 800b750:	440a      	add	r2, r1
 800b752:	791b      	ldrb	r3, [r3, #4]
 800b754:	f892 279c 	ldrb.w	r2, [r2, #1948]	; 0x79c
 800b758:	e9cd 0402 	strd	r0, r4, [sp, #8]
 800b75c:	f8cd 8004 	str.w	r8, [sp, #4]
 800b760:	f501 611b 	add.w	r1, r1, #2480	; 0x9b0
 800b764:	e43e      	b.n	800afe4 <ShowPopWindow+0x42c>
 800b766:	bf00      	nop
 800b768:	2000000c 	.word	0x2000000c
 800b76c:	08013880 	.word	0x08013880
 800b770:	08012bc7 	.word	0x08012bc7
 800b774:	20004c12 	.word	0x20004c12
 800b778:	08012c0c 	.word	0x08012c0c
 800b77c:	20000240 	.word	0x20000240
 800b780:	08013908 	.word	0x08013908
 800b784:	08013884 	.word	0x08013884
 800b788:	08013911 	.word	0x08013911
 800b78c:	08013915 	.word	0x08013915
 800b790:	08013902 	.word	0x08013902

0800b794 <ShowPopItem>:
 800b794:	b51f      	push	{r0, r1, r2, r3, r4, lr}
 800b796:	2806      	cmp	r0, #6
 800b798:	d813      	bhi.n	800b7c2 <ShowPopItem+0x2e>
 800b79a:	e8df f000 	tbb	[pc, r0]
 800b79e:	1504      	.short	0x1504
 800b7a0:	8e817365 	.word	0x8e817365
 800b7a4:	98          	.byte	0x98
 800b7a5:	00          	.byte	0x00
 800b7a6:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b7aa:	2100      	movs	r1, #0
 800b7ac:	e9cd 3101 	strd	r3, r1, [sp, #4]
 800b7b0:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800b7b4:	2272      	movs	r2, #114	; 0x72
 800b7b6:	9300      	str	r3, [sp, #0]
 800b7b8:	21d8      	movs	r1, #216	; 0xd8
 800b7ba:	2322      	movs	r3, #34	; 0x22
 800b7bc:	2028      	movs	r0, #40	; 0x28
 800b7be:	f7ff f9fb 	bl	800abb8 <ShowPopWindow>
 800b7c2:	b005      	add	sp, #20
 800b7c4:	f85d fb04 	ldr.w	pc, [sp], #4
 800b7c8:	4b47      	ldr	r3, [pc, #284]	; (800b8e8 <ShowPopItem+0x154>)
 800b7ca:	781b      	ldrb	r3, [r3, #0]
 800b7cc:	2b01      	cmp	r3, #1
 800b7ce:	d13e      	bne.n	800b84e <ShowPopItem+0xba>
 800b7d0:	f240 1219 	movw	r2, #281	; 0x119
 800b7d4:	4945      	ldr	r1, [pc, #276]	; (800b8ec <ShowPopItem+0x158>)
 800b7d6:	4846      	ldr	r0, [pc, #280]	; (800b8f0 <ShowPopItem+0x15c>)
 800b7d8:	f007 f9a4 	bl	8012b24 <memcpy>
 800b7dc:	4a44      	ldr	r2, [pc, #272]	; (800b8f0 <ShowPopItem+0x15c>)
 800b7de:	f44f 718c 	mov.w	r1, #280	; 0x118
 800b7e2:	2003      	movs	r0, #3
 800b7e4:	f000 fd1a 	bl	800c21c <FPGA_LCD_RW>
 800b7e8:	f44f 7296 	mov.w	r2, #300	; 0x12c
 800b7ec:	2102      	movs	r1, #2
 800b7ee:	2019      	movs	r0, #25
 800b7f0:	f000 fcfc 	bl	800c1ec <FPGA_ReadWrite>
 800b7f4:	f44f 7296 	mov.w	r2, #300	; 0x12c
 800b7f8:	2102      	movs	r1, #2
 800b7fa:	201a      	movs	r0, #26
 800b7fc:	f000 fcf6 	bl	800c1ec <FPGA_ReadWrite>
 800b800:	f44f 7296 	mov.w	r2, #300	; 0x12c
 800b804:	2102      	movs	r1, #2
 800b806:	201b      	movs	r0, #27
 800b808:	f000 fcf0 	bl	800c1ec <FPGA_ReadWrite>
 800b80c:	f44f 7296 	mov.w	r2, #300	; 0x12c
 800b810:	2102      	movs	r1, #2
 800b812:	2018      	movs	r0, #24
 800b814:	f000 fcea 	bl	800c1ec <FPGA_ReadWrite>
 800b818:	23db      	movs	r3, #219	; 0xdb
 800b81a:	f240 123f 	movw	r2, #319	; 0x13f
 800b81e:	2142      	movs	r1, #66	; 0x42
 800b820:	2028      	movs	r0, #40	; 0x28
 800b822:	f000 f9bf 	bl	800bba4 <Set_Block>
 800b826:	4933      	ldr	r1, [pc, #204]	; (800b8f4 <ShowPopItem+0x160>)
 800b828:	2008      	movs	r0, #8
 800b82a:	f7f4 fc69 	bl	8000100 <__Bios>
 800b82e:	2200      	movs	r2, #0
 800b830:	2006      	movs	r0, #6
 800b832:	4611      	mov	r1, r2
 800b834:	f000 fcda 	bl	800c1ec <FPGA_ReadWrite>
 800b838:	200a      	movs	r0, #10
 800b83a:	f000 fd37 	bl	800c2ac <Delay_mS>
 800b83e:	492e      	ldr	r1, [pc, #184]	; (800b8f8 <ShowPopItem+0x164>)
 800b840:	2008      	movs	r0, #8
 800b842:	f7f4 fc5d 	bl	8000100 <__Bios>
 800b846:	4b2d      	ldr	r3, [pc, #180]	; (800b8fc <ShowPopItem+0x168>)
 800b848:	7818      	ldrb	r0, [r3, #0]
 800b84a:	f002 fa2a 	bl	800dca2 <Show_AnalyzeData>
 800b84e:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b852:	2201      	movs	r2, #1
 800b854:	e9cd 3201 	strd	r3, r2, [sp, #4]
 800b858:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800b85c:	227e      	movs	r2, #126	; 0x7e
 800b85e:	9300      	str	r3, [sp, #0]
 800b860:	21d8      	movs	r1, #216	; 0xd8
 800b862:	233e      	movs	r3, #62	; 0x3e
 800b864:	2032      	movs	r0, #50	; 0x32
 800b866:	e7aa      	b.n	800b7be <ShowPopItem+0x2a>
 800b868:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b86c:	f04f 0e02 	mov.w	lr, #2
 800b870:	e9cd 3e01 	strd	r3, lr, [sp, #4]
 800b874:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800b878:	228a      	movs	r2, #138	; 0x8a
 800b87a:	9300      	str	r3, [sp, #0]
 800b87c:	21d8      	movs	r1, #216	; 0xd8
 800b87e:	234c      	movs	r3, #76	; 0x4c
 800b880:	205a      	movs	r0, #90	; 0x5a
 800b882:	e79c      	b.n	800b7be <ShowPopItem+0x2a>
 800b884:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b888:	f04f 0c03 	mov.w	ip, #3
 800b88c:	e9cd 3c01 	strd	r3, ip, [sp, #4]
 800b890:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800b894:	2278      	movs	r2, #120	; 0x78
 800b896:	9300      	str	r3, [sp, #0]
 800b898:	21d8      	movs	r1, #216	; 0xd8
 800b89a:	2322      	movs	r3, #34	; 0x22
 800b89c:	2096      	movs	r0, #150	; 0x96
 800b89e:	e78e      	b.n	800b7be <ShowPopItem+0x2a>
 800b8a0:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b8a4:	2004      	movs	r0, #4
 800b8a6:	e9cd 3001 	strd	r3, r0, [sp, #4]
 800b8aa:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800b8ae:	9300      	str	r3, [sp, #0]
 800b8b0:	234c      	movs	r3, #76	; 0x4c
 800b8b2:	2278      	movs	r2, #120	; 0x78
 800b8b4:	21d8      	movs	r1, #216	; 0xd8
 800b8b6:	20c8      	movs	r0, #200	; 0xc8
 800b8b8:	e781      	b.n	800b7be <ShowPopItem+0x2a>
 800b8ba:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b8be:	2105      	movs	r1, #5
 800b8c0:	e9cd 3101 	strd	r3, r1, [sp, #4]
 800b8c4:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800b8c8:	9300      	str	r3, [sp, #0]
 800b8ca:	2322      	movs	r3, #34	; 0x22
 800b8cc:	e7f1      	b.n	800b8b2 <ShowPopItem+0x11e>
 800b8ce:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800b8d2:	2206      	movs	r2, #6
 800b8d4:	e9cd 3201 	strd	r3, r2, [sp, #4]
 800b8d8:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800b8dc:	228a      	movs	r2, #138	; 0x8a
 800b8de:	9300      	str	r3, [sp, #0]
 800b8e0:	21d8      	movs	r1, #216	; 0xd8
 800b8e2:	235a      	movs	r3, #90	; 0x5a
 800b8e4:	20b4      	movs	r0, #180	; 0xb4
 800b8e6:	e76a      	b.n	800b7be <ShowPopItem+0x2a>
 800b8e8:	20004c12 	.word	0x20004c12
 800b8ec:	20000af3 	.word	0x20000af3
 800b8f0:	200009da 	.word	0x200009da
 800b8f4:	fff90000 	.word	0xfff90000
 800b8f8:	fffb0000 	.word	0xfffb0000
 800b8fc:	2000000c 	.word	0x2000000c

0800b900 <NMIException>:
 800b900:	4770      	bx	lr

0800b902 <HardFaultException>:
 800b902:	e7fe      	b.n	800b902 <HardFaultException>

0800b904 <MemManageException>:
 800b904:	e7fe      	b.n	800b904 <MemManageException>

0800b906 <BusFaultException>:
 800b906:	e7fe      	b.n	800b906 <BusFaultException>

0800b908 <UsageFaultException>:
 800b908:	e7fe      	b.n	800b908 <UsageFaultException>

0800b90a <DebugMonitor>:
 800b90a:	4770      	bx	lr

0800b90c <SVCHandler>:
 800b90c:	4770      	bx	lr

0800b90e <PendSVC>:
 800b90e:	4770      	bx	lr

0800b910 <SysTick_Handler>:
 800b910:	4a52      	ldr	r2, [pc, #328]	; (800ba5c <SysTick_Handler+0x14c>)
 800b912:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800b914:	8813      	ldrh	r3, [r2, #0]
 800b916:	b10b      	cbz	r3, 800b91c <SysTick_Handler+0xc>
 800b918:	3b01      	subs	r3, #1
 800b91a:	8013      	strh	r3, [r2, #0]
 800b91c:	4c50      	ldr	r4, [pc, #320]	; (800ba60 <SysTick_Handler+0x150>)
 800b91e:	8823      	ldrh	r3, [r4, #0]
 800b920:	b29b      	uxth	r3, r3
 800b922:	b11b      	cbz	r3, 800b92c <SysTick_Handler+0x1c>
 800b924:	8823      	ldrh	r3, [r4, #0]
 800b926:	3b01      	subs	r3, #1
 800b928:	b29b      	uxth	r3, r3
 800b92a:	8023      	strh	r3, [r4, #0]
 800b92c:	8863      	ldrh	r3, [r4, #2]
 800b92e:	3301      	adds	r3, #1
 800b930:	8063      	strh	r3, [r4, #2]
 800b932:	88a3      	ldrh	r3, [r4, #4]
 800b934:	3301      	adds	r3, #1
 800b936:	b29b      	uxth	r3, r3
 800b938:	80a3      	strh	r3, [r4, #4]
 800b93a:	07db      	lsls	r3, r3, #31
 800b93c:	d462      	bmi.n	800ba04 <SysTick_Handler+0xf4>
 800b93e:	4949      	ldr	r1, [pc, #292]	; (800ba64 <SysTick_Handler+0x154>)
 800b940:	2006      	movs	r0, #6
 800b942:	f7f4 fbdd 	bl	8000100 <__Bios>
 800b946:	b280      	uxth	r0, r0
 800b948:	f9b4 3006 	ldrsh.w	r3, [r4, #6]
 800b94c:	f400 4170 	and.w	r1, r0, #61440	; 0xf000
 800b950:	b20a      	sxth	r2, r1
 800b952:	ea22 0503 	bic.w	r5, r2, r3
 800b956:	ea23 0302 	bic.w	r3, r3, r2
 800b95a:	b29b      	uxth	r3, r3
 800b95c:	04df      	lsls	r7, r3, #19
 800b95e:	b2ae      	uxth	r6, r5
 800b960:	80e1      	strh	r1, [r4, #6]
 800b962:	d509      	bpl.n	800b978 <SysTick_Handler+0x68>
 800b964:	f410 5f00 	tst.w	r0, #8192	; 0x2000
 800b968:	bf14      	ite	ne
 800b96a:	f44f 5180 	movne.w	r1, #4096	; 0x1000
 800b96e:	f44f 5100 	moveq.w	r1, #8192	; 0x2000
 800b972:	8927      	ldrh	r7, [r4, #8]
 800b974:	4339      	orrs	r1, r7
 800b976:	8121      	strh	r1, [r4, #8]
 800b978:	04b6      	lsls	r6, r6, #18
 800b97a:	d509      	bpl.n	800b990 <SysTick_Handler+0x80>
 800b97c:	f410 5f80 	tst.w	r0, #4096	; 0x1000
 800b980:	bf14      	ite	ne
 800b982:	f44f 5180 	movne.w	r1, #4096	; 0x1000
 800b986:	f44f 5100 	moveq.w	r1, #8192	; 0x2000
 800b98a:	8926      	ldrh	r6, [r4, #8]
 800b98c:	4331      	orrs	r1, r6
 800b98e:	8121      	strh	r1, [r4, #8]
 800b990:	0459      	lsls	r1, r3, #17
 800b992:	d507      	bpl.n	800b9a4 <SysTick_Handler+0x94>
 800b994:	2a00      	cmp	r2, #0
 800b996:	4b34      	ldr	r3, [pc, #208]	; (800ba68 <SysTick_Handler+0x158>)
 800b998:	bfa8      	it	ge
 800b99a:	f44f 4380 	movge.w	r3, #16384	; 0x4000
 800b99e:	8922      	ldrh	r2, [r4, #8]
 800b9a0:	4313      	orrs	r3, r2
 800b9a2:	8123      	strh	r3, [r4, #8]
 800b9a4:	2d00      	cmp	r5, #0
 800b9a6:	da08      	bge.n	800b9ba <SysTick_Handler+0xaa>
 800b9a8:	f410 4f80 	tst.w	r0, #16384	; 0x4000
 800b9ac:	4b2e      	ldr	r3, [pc, #184]	; (800ba68 <SysTick_Handler+0x158>)
 800b9ae:	bf08      	it	eq
 800b9b0:	f44f 4380 	moveq.w	r3, #16384	; 0x4000
 800b9b4:	8922      	ldrh	r2, [r4, #8]
 800b9b6:	4313      	orrs	r3, r2
 800b9b8:	8123      	strh	r3, [r4, #8]
 800b9ba:	88a3      	ldrh	r3, [r4, #4]
 800b9bc:	06db      	lsls	r3, r3, #27
 800b9be:	d11c      	bne.n	800b9fa <SysTick_Handler+0xea>
 800b9c0:	89a2      	ldrh	r2, [r4, #12]
 800b9c2:	f000 000f 	and.w	r0, r0, #15
 800b9c6:	8923      	ldrh	r3, [r4, #8]
 800b9c8:	ea20 0102 	bic.w	r1, r0, r2
 800b9cc:	ea43 2301 	orr.w	r3, r3, r1, lsl #8
 800b9d0:	ea22 0200 	bic.w	r2, r2, r0
 800b9d4:	ea43 1302 	orr.w	r3, r3, r2, lsl #4
 800b9d8:	8160      	strh	r0, [r4, #10]
 800b9da:	81a0      	strh	r0, [r4, #12]
 800b9dc:	4923      	ldr	r1, [pc, #140]	; (800ba6c <SysTick_Handler+0x15c>)
 800b9de:	2004      	movs	r0, #4
 800b9e0:	8123      	strh	r3, [r4, #8]
 800b9e2:	f7f4 fb8d 	bl	8000100 <__Bios>
 800b9e6:	4a22      	ldr	r2, [pc, #136]	; (800ba70 <SysTick_Handler+0x160>)
 800b9e8:	6813      	ldr	r3, [r2, #0]
 800b9ea:	eba3 1393 	sub.w	r3, r3, r3, lsr #6
 800b9ee:	4403      	add	r3, r0
 800b9f0:	6013      	str	r3, [r2, #0]
 800b9f2:	f000 fefd 	bl	800c7f0 <GetBatteryVol>
 800b9f6:	4b1f      	ldr	r3, [pc, #124]	; (800ba74 <SysTick_Handler+0x164>)
 800b9f8:	8018      	strh	r0, [r3, #0]
 800b9fa:	89e3      	ldrh	r3, [r4, #14]
 800b9fc:	2b01      	cmp	r3, #1
 800b9fe:	d927      	bls.n	800ba50 <SysTick_Handler+0x140>
 800ba00:	3b02      	subs	r3, #2
 800ba02:	81e3      	strh	r3, [r4, #14]
 800ba04:	f241 3388 	movw	r3, #5000	; 0x1388
 800ba08:	88a2      	ldrh	r2, [r4, #4]
 800ba0a:	429a      	cmp	r2, r3
 800ba0c:	bf84      	itt	hi
 800ba0e:	2300      	movhi	r3, #0
 800ba10:	80a3      	strhhi	r3, [r4, #4]
 800ba12:	8863      	ldrh	r3, [r4, #2]
 800ba14:	f5b3 7f7a 	cmp.w	r3, #1000	; 0x3e8
 800ba18:	d319      	bcc.n	800ba4e <SysTick_Handler+0x13e>
 800ba1a:	2300      	movs	r3, #0
 800ba1c:	8063      	strh	r3, [r4, #2]
 800ba1e:	4b16      	ldr	r3, [pc, #88]	; (800ba78 <SysTick_Handler+0x168>)
 800ba20:	f8b3 2080 	ldrh.w	r2, [r3, #128]	; 0x80
 800ba24:	b122      	cbz	r2, 800ba30 <SysTick_Handler+0x120>
 800ba26:	4915      	ldr	r1, [pc, #84]	; (800ba7c <SysTick_Handler+0x16c>)
 800ba28:	880a      	ldrh	r2, [r1, #0]
 800ba2a:	b10a      	cbz	r2, 800ba30 <SysTick_Handler+0x120>
 800ba2c:	3a01      	subs	r2, #1
 800ba2e:	800a      	strh	r2, [r1, #0]
 800ba30:	f8b3 3082 	ldrh.w	r3, [r3, #130]	; 0x82
 800ba34:	b123      	cbz	r3, 800ba40 <SysTick_Handler+0x130>
 800ba36:	4a12      	ldr	r2, [pc, #72]	; (800ba80 <SysTick_Handler+0x170>)
 800ba38:	8813      	ldrh	r3, [r2, #0]
 800ba3a:	b10b      	cbz	r3, 800ba40 <SysTick_Handler+0x130>
 800ba3c:	3b01      	subs	r3, #1
 800ba3e:	8013      	strh	r3, [r2, #0]
 800ba40:	f000 fede 	bl	800c800 <Get_USB_Vol>
 800ba44:	2201      	movs	r2, #1
 800ba46:	4b0f      	ldr	r3, [pc, #60]	; (800ba84 <SysTick_Handler+0x174>)
 800ba48:	8018      	strh	r0, [r3, #0]
 800ba4a:	4b0f      	ldr	r3, [pc, #60]	; (800ba88 <SysTick_Handler+0x178>)
 800ba4c:	701a      	strb	r2, [r3, #0]
 800ba4e:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 800ba50:	2100      	movs	r1, #0
 800ba52:	2005      	movs	r0, #5
 800ba54:	f7f4 fb54 	bl	8000100 <__Bios>
 800ba58:	e7d4      	b.n	800ba04 <SysTick_Handler+0xf4>
 800ba5a:	bf00      	nop
 800ba5c:	200009d0 	.word	0x200009d0
 800ba60:	200009ae 	.word	0x200009ae
 800ba64:	fffc0000 	.word	0xfffc0000
 800ba68:	ffff8000 	.word	0xffff8000
 800ba6c:	fffb0000 	.word	0xfffb0000
 800ba70:	200000dc 	.word	0x200000dc
 800ba74:	20000104 	.word	0x20000104
 800ba78:	2000000c 	.word	0x2000000c
 800ba7c:	200009d4 	.word	0x200009d4
 800ba80:	200009d2 	.word	0x200009d2
 800ba84:	200009cc 	.word	0x200009cc
 800ba88:	200000d8 	.word	0x200000d8

0800ba8c <Beep_mS>:
 800ba8c:	b508      	push	{r3, lr}
 800ba8e:	4b08      	ldr	r3, [pc, #32]	; (800bab0 <Beep_mS+0x24>)
 800ba90:	2101      	movs	r1, #1
 800ba92:	81d8      	strh	r0, [r3, #14]
 800ba94:	2005      	movs	r0, #5
 800ba96:	f7f4 fb33 	bl	8000100 <__Bios>
 800ba9a:	210a      	movs	r1, #10
 800ba9c:	4b05      	ldr	r3, [pc, #20]	; (800bab4 <Beep_mS+0x28>)
 800ba9e:	2005      	movs	r0, #5
 800baa0:	f8b3 307c 	ldrh.w	r3, [r3, #124]	; 0x7c
 800baa4:	4359      	muls	r1, r3
 800baa6:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 800baaa:	f7f4 bb29 	b.w	8000100 <__Bios>
 800baae:	bf00      	nop
 800bab0:	200009ae 	.word	0x200009ae
 800bab4:	2000000c 	.word	0x2000000c

0800bab8 <WWDG_IRQHandler>:
 800bab8:	4770      	bx	lr

0800baba <PVD_IRQHandler>:
 800baba:	4770      	bx	lr

0800babc <TAMPER_IRQHandler>:
 800babc:	4770      	bx	lr

0800babe <RTC_IRQHandler>:
 800babe:	4770      	bx	lr

0800bac0 <FLASH_IRQHandler>:
 800bac0:	4770      	bx	lr

0800bac2 <RCC_IRQHandler>:
 800bac2:	4770      	bx	lr

0800bac4 <EXTI0_IRQHandler>:
 800bac4:	4770      	bx	lr

0800bac6 <EXTI1_IRQHandler>:
 800bac6:	4770      	bx	lr

0800bac8 <EXTI2_IRQHandler>:
 800bac8:	4770      	bx	lr

0800baca <EXTI3_IRQHandler>:
 800baca:	4770      	bx	lr

0800bacc <EXTI4_IRQHandler>:
 800bacc:	4770      	bx	lr

0800bace <DMA1_Channel1_IRQHandler>:
 800bace:	4770      	bx	lr

0800bad0 <DMA1_Channel2_IRQHandler>:
 800bad0:	4770      	bx	lr

0800bad2 <DMA1_Channel3_IRQHandler>:
 800bad2:	4770      	bx	lr

0800bad4 <DMA1_Channel4_IRQHandler>:
 800bad4:	4770      	bx	lr

0800bad6 <DMA1_Channel5_IRQHandler>:
 800bad6:	4770      	bx	lr

0800bad8 <DMA1_Channel6_IRQHandler>:
 800bad8:	4770      	bx	lr

0800bada <DMA1_Channel7_IRQHandler>:
 800bada:	4770      	bx	lr

0800badc <ADC1_2_IRQHandler>:
 800badc:	4770      	bx	lr

0800bade <USB_HP_CAN1_TX_IRQHandler>:
 800bade:	4770      	bx	lr

0800bae0 <USB_LP_CAN1_RX0_IRQHandler>:
 800bae0:	f006 bb30 	b.w	8012144 <USB_Istr>

0800bae4 <CAN_RX1_IRQHandler>:
 800bae4:	4770      	bx	lr

0800bae6 <CAN_SCE_IRQHandler>:
 800bae6:	4770      	bx	lr

0800bae8 <EXTI9_5_IRQHandler>:
 800bae8:	4770      	bx	lr

0800baea <TIM1_BRK_IRQHandler>:
 800baea:	4770      	bx	lr

0800baec <TIM1_UP_IRQHandler>:
 800baec:	4770      	bx	lr

0800baee <TIM1_TRG_COM_IRQHandler>:
 800baee:	4770      	bx	lr

0800baf0 <TIM1_CC_IRQHandler>:
 800baf0:	4770      	bx	lr

0800baf2 <TIM2_IRQHandler>:
 800baf2:	4770      	bx	lr

0800baf4 <TIM3_IRQHandler>:
 800baf4:	4770      	bx	lr

0800baf6 <TIM4_IRQHandler>:
 800baf6:	4770      	bx	lr

0800baf8 <I2C1_EV_IRQHandler>:
 800baf8:	4770      	bx	lr

0800bafa <I2C1_ER_IRQHandler>:
 800bafa:	4770      	bx	lr

0800bafc <I2C2_EV_IRQHandler>:
 800bafc:	4770      	bx	lr

0800bafe <I2C2_ER_IRQHandler>:
 800bafe:	4770      	bx	lr

0800bb00 <SPI1_IRQHandler>:
 800bb00:	4770      	bx	lr

0800bb02 <SPI2_IRQHandler>:
 800bb02:	4770      	bx	lr

0800bb04 <USART1_IRQHandler>:
 800bb04:	4770      	bx	lr

0800bb06 <USART2_IRQHandler>:
 800bb06:	4770      	bx	lr

0800bb08 <USART3_IRQHandler>:
 800bb08:	4770      	bx	lr

0800bb0a <EXTI15_10_IRQHandler>:
 800bb0a:	4770      	bx	lr

0800bb0c <RTCAlarm_IRQHandler>:
 800bb0c:	4770      	bx	lr

0800bb0e <USBWakeUp_IRQHandler>:
 800bb0e:	4770      	bx	lr

0800bb10 <TIM8_BRK_IRQHandler>:
 800bb10:	4770      	bx	lr

0800bb12 <TIM8_UP_IRQHandler>:
 800bb12:	4770      	bx	lr

0800bb14 <TIM8_TRG_COM_IRQHandler>:
 800bb14:	4770      	bx	lr

0800bb16 <TIM8_CC_IRQHandler>:
 800bb16:	4770      	bx	lr

0800bb18 <ADC3_IRQHandler>:
 800bb18:	4770      	bx	lr

0800bb1a <FSMC_IRQHandler>:
 800bb1a:	4770      	bx	lr

0800bb1c <SDIO_IRQHandler>:
 800bb1c:	4770      	bx	lr

0800bb1e <TIM5_IRQHandler>:
 800bb1e:	4770      	bx	lr

0800bb20 <SPI3_IRQHandler>:
 800bb20:	4770      	bx	lr

0800bb22 <UART4_IRQHandler>:
 800bb22:	4770      	bx	lr

0800bb24 <UART5_IRQHandler>:
 800bb24:	4770      	bx	lr

0800bb26 <TIM6_IRQHandler>:
 800bb26:	4770      	bx	lr

0800bb28 <TIM7_IRQHandler>:
 800bb28:	4770      	bx	lr

0800bb2a <DMA2_Channel1_IRQHandler>:
 800bb2a:	4770      	bx	lr

0800bb2c <DMA2_Channel2_IRQHandler>:
 800bb2c:	4770      	bx	lr

0800bb2e <DMA2_Channel3_IRQHandler>:
 800bb2e:	4770      	bx	lr

0800bb30 <DMA2_Channel4_5_IRQHandler>:
 800bb30:	4770      	bx	lr
 800bb32:	bf00      	nop

0800bb34 <Set_Posi>:
 800bb34:	b510      	push	{r4, lr}
 800bb36:	460c      	mov	r4, r1
 800bb38:	4601      	mov	r1, r0
 800bb3a:	200b      	movs	r0, #11
 800bb3c:	f7f4 fae0 	bl	8000100 <__Bios>
 800bb40:	4621      	mov	r1, r4
 800bb42:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800bb46:	200c      	movs	r0, #12
 800bb48:	f7f4 bada 	b.w	8000100 <__Bios>

0800bb4c <Set_Pixel>:
 800bb4c:	4601      	mov	r1, r0
 800bb4e:	200d      	movs	r0, #13
 800bb50:	f7f4 bad6 	b.w	8000100 <__Bios>

0800bb54 <Read_Pixel>:
 800bb54:	b508      	push	{r3, lr}
 800bb56:	2100      	movs	r1, #0
 800bb58:	200e      	movs	r0, #14
 800bb5a:	f7f4 fad1 	bl	8000100 <__Bios>
 800bb5e:	b280      	uxth	r0, r0
 800bb60:	bd08      	pop	{r3, pc}

0800bb62 <Add_Pixel_Color>:
 800bb62:	b538      	push	{r3, r4, r5, lr}
 800bb64:	460d      	mov	r5, r1
 800bb66:	4614      	mov	r4, r2
 800bb68:	4601      	mov	r1, r0
 800bb6a:	200b      	movs	r0, #11
 800bb6c:	f7f4 fac8 	bl	8000100 <__Bios>
 800bb70:	4629      	mov	r1, r5
 800bb72:	200c      	movs	r0, #12
 800bb74:	f7f4 fac4 	bl	8000100 <__Bios>
 800bb78:	4621      	mov	r1, r4
 800bb7a:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 800bb7e:	200d      	movs	r0, #13
 800bb80:	f7f4 babe 	b.w	8000100 <__Bios>

0800bb84 <Read_Pixel_Color>:
 800bb84:	b510      	push	{r4, lr}
 800bb86:	460c      	mov	r4, r1
 800bb88:	4601      	mov	r1, r0
 800bb8a:	200b      	movs	r0, #11
 800bb8c:	f7f4 fab8 	bl	8000100 <__Bios>
 800bb90:	4621      	mov	r1, r4
 800bb92:	200c      	movs	r0, #12
 800bb94:	f7f4 fab4 	bl	8000100 <__Bios>
 800bb98:	2100      	movs	r1, #0
 800bb9a:	200e      	movs	r0, #14
 800bb9c:	f7f4 fab0 	bl	8000100 <__Bios>
 800bba0:	b280      	uxth	r0, r0
 800bba2:	bd10      	pop	{r4, pc}

0800bba4 <Set_Block>:
 800bba4:	b538      	push	{r3, r4, r5, lr}
 800bba6:	460d      	mov	r5, r1
 800bba8:	461c      	mov	r4, r3
 800bbaa:	eb02 4100 	add.w	r1, r2, r0, lsl #16
 800bbae:	2009      	movs	r0, #9
 800bbb0:	f7f4 faa6 	bl	8000100 <__Bios>
 800bbb4:	eb04 4105 	add.w	r1, r4, r5, lsl #16
 800bbb8:	200a      	movs	r0, #10
 800bbba:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 800bbbe:	f7f4 ba9f 	b.w	8000100 <__Bios>

0800bbc2 <Clr_Scrn>:
 800bbc2:	2100      	movs	r1, #0
 800bbc4:	b538      	push	{r3, r4, r5, lr}
 800bbc6:	4605      	mov	r5, r0
 800bbc8:	4608      	mov	r0, r1
 800bbca:	f7ff ffb3 	bl	800bb34 <Set_Posi>
 800bbce:	f44f 3496 	mov.w	r4, #76800	; 0x12c00
 800bbd2:	4628      	mov	r0, r5
 800bbd4:	f7ff ffba 	bl	800bb4c <Set_Pixel>
 800bbd8:	3c01      	subs	r4, #1
 800bbda:	d1fa      	bne.n	800bbd2 <Clr_Scrn+0x10>
 800bbdc:	bd38      	pop	{r3, r4, r5, pc}

0800bbde <Get_TAB_8x14>:
 800bbde:	3821      	subs	r0, #33	; 0x21
 800bbe0:	b2c3      	uxtb	r3, r0
 800bbe2:	2b5d      	cmp	r3, #93	; 0x5d
 800bbe4:	d807      	bhi.n	800bbf6 <Get_TAB_8x14+0x18>
 800bbe6:	2907      	cmp	r1, #7
 800bbe8:	d805      	bhi.n	800bbf6 <Get_TAB_8x14+0x18>
 800bbea:	4b04      	ldr	r3, [pc, #16]	; (800bbfc <Get_TAB_8x14+0x1e>)
 800bbec:	eb01 01c0 	add.w	r1, r1, r0, lsl #3
 800bbf0:	f833 0011 	ldrh.w	r0, [r3, r1, lsl #1]
 800bbf4:	4770      	bx	lr
 800bbf6:	2000      	movs	r0, #0
 800bbf8:	4770      	bx	lr
 800bbfa:	bf00      	nop
 800bbfc:	0801391a 	.word	0x0801391a

0800bc00 <Disp_Str8x14>:
 800bc00:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800bc04:	b087      	sub	sp, #28
 800bc06:	f89d 8040 	ldrb.w	r8, [sp, #64]	; 0x40
 800bc0a:	9102      	str	r1, [sp, #8]
 800bc0c:	f008 0110 	and.w	r1, r8, #16
 800bc10:	2900      	cmp	r1, #0
 800bc12:	bf0c      	ite	eq
 800bc14:	4619      	moveq	r1, r3
 800bc16:	4611      	movne	r1, r2
 800bc18:	4605      	mov	r5, r0
 800bc1a:	bf08      	it	eq
 800bc1c:	4693      	moveq	fp, r2
 800bc1e:	9e11      	ldr	r6, [sp, #68]	; 0x44
 800bc20:	bf18      	it	ne
 800bc22:	469b      	movne	fp, r3
 800bc24:	f008 0320 	and.w	r3, r8, #32
 800bc28:	9101      	str	r1, [sp, #4]
 800bc2a:	3e01      	subs	r6, #1
 800bc2c:	9305      	str	r3, [sp, #20]
 800bc2e:	f816 9f01 	ldrb.w	r9, [r6, #1]!
 800bc32:	f1b9 0f00 	cmp.w	r9, #0
 800bc36:	d102      	bne.n	800bc3e <Disp_Str8x14+0x3e>
 800bc38:	b007      	add	sp, #28
 800bc3a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800bc3e:	2700      	movs	r7, #0
 800bc40:	f89d 3014 	ldrb.w	r3, [sp, #20]
 800bc44:	9303      	str	r3, [sp, #12]
 800bc46:	f008 0340 	and.w	r3, r8, #64	; 0x40
 800bc4a:	9304      	str	r3, [sp, #16]
 800bc4c:	fa1f fa87 	uxth.w	sl, r7
 800bc50:	9b03      	ldr	r3, [sp, #12]
 800bc52:	eb05 020a 	add.w	r2, r5, sl
 800bc56:	b292      	uxth	r2, r2
 800bc58:	b923      	cbnz	r3, 800bc64 <Disp_Str8x14+0x64>
 800bc5a:	4651      	mov	r1, sl
 800bc5c:	4648      	mov	r0, r9
 800bc5e:	f7ff ffbe 	bl	800bbde <Get_TAB_8x14>
 800bc62:	4604      	mov	r4, r0
 800bc64:	9b04      	ldr	r3, [sp, #16]
 800bc66:	b143      	cbz	r3, 800bc7a <Disp_Str8x14+0x7a>
 800bc68:	4911      	ldr	r1, [pc, #68]	; (800bcb0 <Disp_Str8x14+0xb0>)
 800bc6a:	f01a 0f01 	tst.w	sl, #1
 800bc6e:	f245 5355 	movw	r3, #21845	; 0x5555
 800bc72:	bf18      	it	ne
 800bc74:	460b      	movne	r3, r1
 800bc76:	401c      	ands	r4, r3
 800bc78:	b2a4      	uxth	r4, r4
 800bc7a:	9902      	ldr	r1, [sp, #8]
 800bc7c:	4610      	mov	r0, r2
 800bc7e:	f7ff ff59 	bl	800bb34 <Set_Posi>
 800bc82:	f04f 0a0e 	mov.w	sl, #14
 800bc86:	0763      	lsls	r3, r4, #29
 800bc88:	bf4c      	ite	mi
 800bc8a:	4658      	movmi	r0, fp
 800bc8c:	9801      	ldrpl	r0, [sp, #4]
 800bc8e:	f7ff ff5d 	bl	800bb4c <Set_Pixel>
 800bc92:	f10a 33ff 	add.w	r3, sl, #4294967295	; 0xffffffff
 800bc96:	fa1f fa83 	uxth.w	sl, r3
 800bc9a:	f3c4 044f 	ubfx	r4, r4, #1, #16
 800bc9e:	f1ba 0f00 	cmp.w	sl, #0
 800bca2:	d1f0      	bne.n	800bc86 <Disp_Str8x14+0x86>
 800bca4:	3701      	adds	r7, #1
 800bca6:	2f08      	cmp	r7, #8
 800bca8:	d1d0      	bne.n	800bc4c <Disp_Str8x14+0x4c>
 800bcaa:	3508      	adds	r5, #8
 800bcac:	b2ad      	uxth	r5, r5
 800bcae:	e7be      	b.n	800bc2e <Disp_Str8x14+0x2e>
 800bcb0:	ffffaaaa 	.word	0xffffaaaa

0800bcb4 <Disp_Char8x14>:
 800bcb4:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800bcb8:	f89d 5030 	ldrb.w	r5, [sp, #48]	; 0x30
 800bcbc:	4688      	mov	r8, r1
 800bcbe:	f005 0110 	and.w	r1, r5, #16
 800bcc2:	2900      	cmp	r1, #0
 800bcc4:	bf14      	ite	ne
 800bcc6:	4693      	movne	fp, r2
 800bcc8:	4692      	moveq	sl, r2
 800bcca:	f04f 0600 	mov.w	r6, #0
 800bcce:	bf0c      	ite	eq
 800bcd0:	469b      	moveq	fp, r3
 800bcd2:	469a      	movne	sl, r3
 800bcd4:	f89d 9034 	ldrb.w	r9, [sp, #52]	; 0x34
 800bcd8:	f005 0320 	and.w	r3, r5, #32
 800bcdc:	9000      	str	r0, [sp, #0]
 800bcde:	9301      	str	r3, [sp, #4]
 800bce0:	f005 0540 	and.w	r5, r5, #64	; 0x40
 800bce4:	9b00      	ldr	r3, [sp, #0]
 800bce6:	b2b7      	uxth	r7, r6
 800bce8:	19da      	adds	r2, r3, r7
 800bcea:	9b01      	ldr	r3, [sp, #4]
 800bcec:	b292      	uxth	r2, r2
 800bcee:	b923      	cbnz	r3, 800bcfa <Disp_Char8x14+0x46>
 800bcf0:	4639      	mov	r1, r7
 800bcf2:	4648      	mov	r0, r9
 800bcf4:	f7ff ff73 	bl	800bbde <Get_TAB_8x14>
 800bcf8:	4604      	mov	r4, r0
 800bcfa:	b145      	cbz	r5, 800bd0e <Disp_Char8x14+0x5a>
 800bcfc:	490f      	ldr	r1, [pc, #60]	; (800bd3c <Disp_Char8x14+0x88>)
 800bcfe:	f017 0f01 	tst.w	r7, #1
 800bd02:	f245 5355 	movw	r3, #21845	; 0x5555
 800bd06:	bf18      	it	ne
 800bd08:	460b      	movne	r3, r1
 800bd0a:	401c      	ands	r4, r3
 800bd0c:	b2a4      	uxth	r4, r4
 800bd0e:	4641      	mov	r1, r8
 800bd10:	4610      	mov	r0, r2
 800bd12:	f7ff ff0f 	bl	800bb34 <Set_Posi>
 800bd16:	270e      	movs	r7, #14
 800bd18:	0763      	lsls	r3, r4, #29
 800bd1a:	bf4c      	ite	mi
 800bd1c:	4650      	movmi	r0, sl
 800bd1e:	4658      	movpl	r0, fp
 800bd20:	f7ff ff14 	bl	800bb4c <Set_Pixel>
 800bd24:	1e7b      	subs	r3, r7, #1
 800bd26:	b29f      	uxth	r7, r3
 800bd28:	f3c4 044f 	ubfx	r4, r4, #1, #16
 800bd2c:	2f00      	cmp	r7, #0
 800bd2e:	d1f3      	bne.n	800bd18 <Disp_Char8x14+0x64>
 800bd30:	3601      	adds	r6, #1
 800bd32:	2e08      	cmp	r6, #8
 800bd34:	d1d6      	bne.n	800bce4 <Disp_Char8x14+0x30>
 800bd36:	b003      	add	sp, #12
 800bd38:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800bd3c:	ffffaaaa 	.word	0xffffaaaa

0800bd40 <Get_TAB_6x8>:
 800bd40:	3821      	subs	r0, #33	; 0x21
 800bd42:	b2c3      	uxtb	r3, r0
 800bd44:	2b5d      	cmp	r3, #93	; 0x5d
 800bd46:	d809      	bhi.n	800bd5c <Get_TAB_6x8+0x1c>
 800bd48:	2905      	cmp	r1, #5
 800bd4a:	d807      	bhi.n	800bd5c <Get_TAB_6x8+0x1c>
 800bd4c:	2206      	movs	r2, #6
 800bd4e:	4b04      	ldr	r3, [pc, #16]	; (800bd60 <Get_TAB_6x8+0x20>)
 800bd50:	fb02 3000 	mla	r0, r2, r0, r3
 800bd54:	4401      	add	r1, r0
 800bd56:	f891 05f0 	ldrb.w	r0, [r1, #1520]	; 0x5f0
 800bd5a:	4770      	bx	lr
 800bd5c:	2000      	movs	r0, #0
 800bd5e:	4770      	bx	lr
 800bd60:	0801391a 	.word	0x0801391a

0800bd64 <Get_Mini6x8>:
 800bd64:	3821      	subs	r0, #33	; 0x21
 800bd66:	b2c3      	uxtb	r3, r0
 800bd68:	2b5d      	cmp	r3, #93	; 0x5d
 800bd6a:	d809      	bhi.n	800bd80 <Get_Mini6x8+0x1c>
 800bd6c:	2905      	cmp	r1, #5
 800bd6e:	d807      	bhi.n	800bd80 <Get_Mini6x8+0x1c>
 800bd70:	2206      	movs	r2, #6
 800bd72:	4b04      	ldr	r3, [pc, #16]	; (800bd84 <Get_Mini6x8+0x20>)
 800bd74:	fb02 3000 	mla	r0, r2, r0, r3
 800bd78:	4401      	add	r1, r0
 800bd7a:	f891 0824 	ldrb.w	r0, [r1, #2084]	; 0x824
 800bd7e:	4770      	bx	lr
 800bd80:	2000      	movs	r0, #0
 800bd82:	4770      	bx	lr
 800bd84:	0801391a 	.word	0x0801391a

0800bd88 <Disp_Str6x8>:
 800bd88:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800bd8c:	b089      	sub	sp, #36	; 0x24
 800bd8e:	461d      	mov	r5, r3
 800bd90:	f89d 704c 	ldrb.w	r7, [sp, #76]	; 0x4c
 800bd94:	f89d 3048 	ldrb.w	r3, [sp, #72]	; 0x48
 800bd98:	4614      	mov	r4, r2
 800bd9a:	9304      	str	r3, [sp, #16]
 800bd9c:	e9cd 0102 	strd	r0, r1, [sp, #8]
 800bda0:	f7ff fec8 	bl	800bb34 <Set_Posi>
 800bda4:	2f01      	cmp	r7, #1
 800bda6:	bf0c      	ite	eq
 800bda8:	2306      	moveq	r3, #6
 800bdaa:	2307      	movne	r3, #7
 800bdac:	9306      	str	r3, [sp, #24]
 800bdae:	9b04      	ldr	r3, [sp, #16]
 800bdb0:	f003 0310 	and.w	r3, r3, #16
 800bdb4:	2b00      	cmp	r3, #0
 800bdb6:	bf13      	iteet	ne
 800bdb8:	462b      	movne	r3, r5
 800bdba:	4623      	moveq	r3, r4
 800bdbc:	46a9      	moveq	r9, r5
 800bdbe:	46a1      	movne	r9, r4
 800bdc0:	9301      	str	r3, [sp, #4]
 800bdc2:	9b14      	ldr	r3, [sp, #80]	; 0x50
 800bdc4:	f103 3bff 	add.w	fp, r3, #4294967295	; 0xffffffff
 800bdc8:	f81b 6f01 	ldrb.w	r6, [fp, #1]!
 800bdcc:	b916      	cbnz	r6, 800bdd4 <Disp_Str6x8+0x4c>
 800bdce:	b009      	add	sp, #36	; 0x24
 800bdd0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800bdd4:	fab7 f387 	clz	r3, r7
 800bdd8:	095b      	lsrs	r3, r3, #5
 800bdda:	b29a      	uxth	r2, r3
 800bddc:	9205      	str	r2, [sp, #20]
 800bdde:	2206      	movs	r2, #6
 800bde0:	2500      	movs	r5, #0
 800bde2:	f1a6 0821 	sub.w	r8, r6, #33	; 0x21
 800bde6:	fb02 f808 	mul.w	r8, r2, r8
 800bdea:	4a28      	ldr	r2, [pc, #160]	; (800be8c <Disp_Str6x8+0x104>)
 800bdec:	4443      	add	r3, r8
 800bdee:	18d3      	adds	r3, r2, r3
 800bdf0:	9307      	str	r3, [sp, #28]
 800bdf2:	9b05      	ldr	r3, [sp, #20]
 800bdf4:	b2ac      	uxth	r4, r5
 800bdf6:	1919      	adds	r1, r3, r4
 800bdf8:	9b02      	ldr	r3, [sp, #8]
 800bdfa:	b289      	uxth	r1, r1
 800bdfc:	441c      	add	r4, r3
 800bdfe:	9b06      	ldr	r3, [sp, #24]
 800be00:	b2a4      	uxth	r4, r4
 800be02:	428b      	cmp	r3, r1
 800be04:	d801      	bhi.n	800be0a <Disp_Str6x8+0x82>
 800be06:	9402      	str	r4, [sp, #8]
 800be08:	e7de      	b.n	800bdc8 <Disp_Str6x8+0x40>
 800be0a:	9b04      	ldr	r3, [sp, #16]
 800be0c:	2f01      	cmp	r7, #1
 800be0e:	f003 0320 	and.w	r3, r3, #32
 800be12:	d128      	bne.n	800be66 <Disp_Str6x8+0xde>
 800be14:	bb13      	cbnz	r3, 800be5c <Disp_Str6x8+0xd4>
 800be16:	4630      	mov	r0, r6
 800be18:	f7ff ff92 	bl	800bd40 <Get_TAB_6x8>
 800be1c:	4682      	mov	sl, r0
 800be1e:	9903      	ldr	r1, [sp, #12]
 800be20:	4620      	mov	r0, r4
 800be22:	f7ff fe87 	bl	800bb34 <Set_Posi>
 800be26:	4648      	mov	r0, r9
 800be28:	f7ff fe90 	bl	800bb4c <Set_Pixel>
 800be2c:	9b03      	ldr	r3, [sp, #12]
 800be2e:	4620      	mov	r0, r4
 800be30:	1c59      	adds	r1, r3, #1
 800be32:	b289      	uxth	r1, r1
 800be34:	f7ff fe7e 	bl	800bb34 <Set_Posi>
 800be38:	2409      	movs	r4, #9
 800be3a:	2f01      	cmp	r7, #1
 800be3c:	d11a      	bne.n	800be74 <Disp_Str6x8+0xec>
 800be3e:	f01a 0f01 	tst.w	sl, #1
 800be42:	bf14      	ite	ne
 800be44:	9801      	ldrne	r0, [sp, #4]
 800be46:	4648      	moveq	r0, r9
 800be48:	f7ff fe80 	bl	800bb4c <Set_Pixel>
 800be4c:	f3ca 0a4f 	ubfx	sl, sl, #1, #16
 800be50:	3c01      	subs	r4, #1
 800be52:	b2a4      	uxth	r4, r4
 800be54:	2c00      	cmp	r4, #0
 800be56:	d1f0      	bne.n	800be3a <Disp_Str6x8+0xb2>
 800be58:	3501      	adds	r5, #1
 800be5a:	e7ca      	b.n	800bdf2 <Disp_Str6x8+0x6a>
 800be5c:	4b0b      	ldr	r3, [pc, #44]	; (800be8c <Disp_Str6x8+0x104>)
 800be5e:	4443      	add	r3, r8
 800be60:	f813 a005 	ldrb.w	sl, [r3, r5]
 800be64:	e7db      	b.n	800be1e <Disp_Str6x8+0x96>
 800be66:	b91b      	cbnz	r3, 800be70 <Disp_Str6x8+0xe8>
 800be68:	4630      	mov	r0, r6
 800be6a:	f7ff ff7b 	bl	800bd64 <Get_Mini6x8>
 800be6e:	e7d5      	b.n	800be1c <Disp_Str6x8+0x94>
 800be70:	9b07      	ldr	r3, [sp, #28]
 800be72:	e7f5      	b.n	800be60 <Disp_Str6x8+0xd8>
 800be74:	f01a 0f80 	tst.w	sl, #128	; 0x80
 800be78:	bf14      	ite	ne
 800be7a:	9801      	ldrne	r0, [sp, #4]
 800be7c:	4648      	moveq	r0, r9
 800be7e:	f7ff fe65 	bl	800bb4c <Set_Pixel>
 800be82:	ea4f 034a 	mov.w	r3, sl, lsl #1
 800be86:	fa1f fa83 	uxth.w	sl, r3
 800be8a:	e7e1      	b.n	800be50 <Disp_Str6x8+0xc8>
 800be8c:	08014360 	.word	0x08014360

0800be90 <Disp_Str7x9>:
 800be90:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800be94:	b087      	sub	sp, #28
 800be96:	461d      	mov	r5, r3
 800be98:	f89d b044 	ldrb.w	fp, [sp, #68]	; 0x44
 800be9c:	f89d 3040 	ldrb.w	r3, [sp, #64]	; 0x40
 800bea0:	4614      	mov	r4, r2
 800bea2:	9304      	str	r3, [sp, #16]
 800bea4:	4682      	mov	sl, r0
 800bea6:	9103      	str	r1, [sp, #12]
 800bea8:	f7ff fe44 	bl	800bb34 <Set_Posi>
 800beac:	f1bb 0f01 	cmp.w	fp, #1
 800beb0:	bf0c      	ite	eq
 800beb2:	2306      	moveq	r3, #6
 800beb4:	2307      	movne	r3, #7
 800beb6:	9305      	str	r3, [sp, #20]
 800beb8:	9b04      	ldr	r3, [sp, #16]
 800beba:	f003 0310 	and.w	r3, r3, #16
 800bebe:	2b00      	cmp	r3, #0
 800bec0:	bf13      	iteet	ne
 800bec2:	462b      	movne	r3, r5
 800bec4:	4623      	moveq	r3, r4
 800bec6:	46a9      	moveq	r9, r5
 800bec8:	46a1      	movne	r9, r4
 800beca:	9301      	str	r3, [sp, #4]
 800becc:	9b12      	ldr	r3, [sp, #72]	; 0x48
 800bece:	3b01      	subs	r3, #1
 800bed0:	9302      	str	r3, [sp, #8]
 800bed2:	9b02      	ldr	r3, [sp, #8]
 800bed4:	f813 7f01 	ldrb.w	r7, [r3, #1]!
 800bed8:	9302      	str	r3, [sp, #8]
 800beda:	b917      	cbnz	r7, 800bee2 <Disp_Str7x9+0x52>
 800bedc:	b007      	add	sp, #28
 800bede:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800bee2:	2206      	movs	r2, #6
 800bee4:	2500      	movs	r5, #0
 800bee6:	f8bd 3014 	ldrh.w	r3, [sp, #20]
 800beea:	f1a7 0421 	sub.w	r4, r7, #33	; 0x21
 800beee:	9300      	str	r3, [sp, #0]
 800bef0:	4b26      	ldr	r3, [pc, #152]	; (800bf8c <Disp_Str7x9+0xfc>)
 800bef2:	fb02 3404 	mla	r4, r2, r4, r3
 800bef6:	9b00      	ldr	r3, [sp, #0]
 800bef8:	b2a9      	uxth	r1, r5
 800befa:	eb01 060a 	add.w	r6, r1, sl
 800befe:	428b      	cmp	r3, r1
 800bf00:	b2b6      	uxth	r6, r6
 800bf02:	d803      	bhi.n	800bf0c <Disp_Str7x9+0x7c>
 800bf04:	4453      	add	r3, sl
 800bf06:	fa1f fa83 	uxth.w	sl, r3
 800bf0a:	e7e2      	b.n	800bed2 <Disp_Str7x9+0x42>
 800bf0c:	9b04      	ldr	r3, [sp, #16]
 800bf0e:	f1bb 0f01 	cmp.w	fp, #1
 800bf12:	f003 0320 	and.w	r3, r3, #32
 800bf16:	d124      	bne.n	800bf62 <Disp_Str7x9+0xd2>
 800bf18:	bb43      	cbnz	r3, 800bf6c <Disp_Str7x9+0xdc>
 800bf1a:	4638      	mov	r0, r7
 800bf1c:	f7ff ff10 	bl	800bd40 <Get_TAB_6x8>
 800bf20:	4680      	mov	r8, r0
 800bf22:	9903      	ldr	r1, [sp, #12]
 800bf24:	4630      	mov	r0, r6
 800bf26:	f7ff fe05 	bl	800bb34 <Set_Posi>
 800bf2a:	4648      	mov	r0, r9
 800bf2c:	f7ff fe0e 	bl	800bb4c <Set_Pixel>
 800bf30:	9b03      	ldr	r3, [sp, #12]
 800bf32:	4630      	mov	r0, r6
 800bf34:	1c59      	adds	r1, r3, #1
 800bf36:	b289      	uxth	r1, r1
 800bf38:	f7ff fdfc 	bl	800bb34 <Set_Posi>
 800bf3c:	2609      	movs	r6, #9
 800bf3e:	f1bb 0f01 	cmp.w	fp, #1
 800bf42:	d116      	bne.n	800bf72 <Disp_Str7x9+0xe2>
 800bf44:	f018 0f01 	tst.w	r8, #1
 800bf48:	bf14      	ite	ne
 800bf4a:	9801      	ldrne	r0, [sp, #4]
 800bf4c:	4648      	moveq	r0, r9
 800bf4e:	f7ff fdfd 	bl	800bb4c <Set_Pixel>
 800bf52:	f3c8 084f 	ubfx	r8, r8, #1, #16
 800bf56:	3e01      	subs	r6, #1
 800bf58:	b2b6      	uxth	r6, r6
 800bf5a:	2e00      	cmp	r6, #0
 800bf5c:	d1ef      	bne.n	800bf3e <Disp_Str7x9+0xae>
 800bf5e:	3501      	adds	r5, #1
 800bf60:	e7c9      	b.n	800bef6 <Disp_Str7x9+0x66>
 800bf62:	b91b      	cbnz	r3, 800bf6c <Disp_Str7x9+0xdc>
 800bf64:	4638      	mov	r0, r7
 800bf66:	f7ff fefd 	bl	800bd64 <Get_Mini6x8>
 800bf6a:	e7d9      	b.n	800bf20 <Disp_Str7x9+0x90>
 800bf6c:	f814 8005 	ldrb.w	r8, [r4, r5]
 800bf70:	e7d7      	b.n	800bf22 <Disp_Str7x9+0x92>
 800bf72:	f018 0f80 	tst.w	r8, #128	; 0x80
 800bf76:	bf14      	ite	ne
 800bf78:	9801      	ldrne	r0, [sp, #4]
 800bf7a:	4648      	moveq	r0, r9
 800bf7c:	f7ff fde6 	bl	800bb4c <Set_Pixel>
 800bf80:	ea4f 0348 	mov.w	r3, r8, lsl #1
 800bf84:	fa1f f883 	uxth.w	r8, r3
 800bf88:	e7e5      	b.n	800bf56 <Disp_Str7x9+0xc6>
 800bf8a:	bf00      	nop
 800bf8c:	08014360 	.word	0x08014360

0800bf90 <Show_Run_ICO>:
 800bf90:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800bf94:	461e      	mov	r6, r3
 800bf96:	4615      	mov	r5, r2
 800bf98:	468a      	mov	sl, r1
 800bf9a:	f89d 4034 	ldrb.w	r4, [sp, #52]	; 0x34
 800bf9e:	9001      	str	r0, [sp, #4]
 800bfa0:	f7ff fdc8 	bl	800bb34 <Set_Posi>
 800bfa4:	f89d 1030 	ldrb.w	r1, [sp, #48]	; 0x30
 800bfa8:	f10a 0701 	add.w	r7, sl, #1
 800bfac:	f001 0110 	and.w	r1, r1, #16
 800bfb0:	2900      	cmp	r1, #0
 800bfb2:	bf12      	itee	ne
 800bfb4:	4633      	movne	r3, r6
 800bfb6:	462b      	moveq	r3, r5
 800bfb8:	46b1      	moveq	r9, r6
 800bfba:	f04f 060c 	mov.w	r6, #12
 800bfbe:	bf18      	it	ne
 800bfc0:	46a9      	movne	r9, r5
 800bfc2:	2500      	movs	r5, #0
 800bfc4:	9300      	str	r3, [sp, #0]
 800bfc6:	4b1a      	ldr	r3, [pc, #104]	; (800c030 <Show_Run_ICO+0xa0>)
 800bfc8:	b2bf      	uxth	r7, r7
 800bfca:	fb06 3604 	mla	r6, r6, r4, r3
 800bfce:	9b01      	ldr	r3, [sp, #4]
 800bfd0:	4651      	mov	r1, sl
 800bfd2:	442b      	add	r3, r5
 800bfd4:	fa1f fb83 	uxth.w	fp, r3
 800bfd8:	4658      	mov	r0, fp
 800bfda:	f816 8005 	ldrb.w	r8, [r6, r5]
 800bfde:	f7ff fda9 	bl	800bb34 <Set_Posi>
 800bfe2:	4648      	mov	r0, r9
 800bfe4:	f7ff fdb2 	bl	800bb4c <Set_Pixel>
 800bfe8:	4658      	mov	r0, fp
 800bfea:	4639      	mov	r1, r7
 800bfec:	f7ff fda2 	bl	800bb34 <Set_Posi>
 800bff0:	f04f 0b08 	mov.w	fp, #8
 800bff4:	f018 0f01 	tst.w	r8, #1
 800bff8:	bf14      	ite	ne
 800bffa:	9800      	ldrne	r0, [sp, #0]
 800bffc:	4648      	moveq	r0, r9
 800bffe:	f7ff fda5 	bl	800bb4c <Set_Pixel>
 800c002:	f10b 33ff 	add.w	r3, fp, #4294967295	; 0xffffffff
 800c006:	f013 0bff 	ands.w	fp, r3, #255	; 0xff
 800c00a:	ea4f 0858 	mov.w	r8, r8, lsr #1
 800c00e:	d1f1      	bne.n	800bff4 <Show_Run_ICO+0x64>
 800c010:	3501      	adds	r5, #1
 800c012:	2d0c      	cmp	r5, #12
 800c014:	d1db      	bne.n	800bfce <Show_Run_ICO+0x3e>
 800c016:	2c06      	cmp	r4, #6
 800c018:	bf8e      	itee	hi
 800c01a:	465c      	movhi	r4, fp
 800c01c:	3401      	addls	r4, #1
 800c01e:	b2e4      	uxtbls	r4, r4
 800c020:	2032      	movs	r0, #50	; 0x32
 800c022:	f000 f943 	bl	800c2ac <Delay_mS>
 800c026:	4620      	mov	r0, r4
 800c028:	b003      	add	sp, #12
 800c02a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800c02e:	bf00      	nop
 800c030:	08014594 	.word	0x08014594

0800c034 <RW_Row_Color>:
 800c034:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 800c038:	4607      	mov	r7, r0
 800c03a:	4688      	mov	r8, r1
 800c03c:	4616      	mov	r6, r2
 800c03e:	461d      	mov	r5, r3
 800c040:	2400      	movs	r4, #0
 800c042:	f89d 9020 	ldrb.w	r9, [sp, #32]
 800c046:	42a6      	cmp	r6, r4
 800c048:	d101      	bne.n	800c04e <RW_Row_Color+0x1a>
 800c04a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 800c04e:	eb08 0104 	add.w	r1, r8, r4
 800c052:	b289      	uxth	r1, r1
 800c054:	4638      	mov	r0, r7
 800c056:	f7ff fd6d 	bl	800bb34 <Set_Posi>
 800c05a:	f1b9 0f01 	cmp.w	r9, #1
 800c05e:	d105      	bne.n	800c06c <RW_Row_Color+0x38>
 800c060:	f835 0014 	ldrh.w	r0, [r5, r4, lsl #1]
 800c064:	f7ff fd72 	bl	800bb4c <Set_Pixel>
 800c068:	3401      	adds	r4, #1
 800c06a:	e7ec      	b.n	800c046 <RW_Row_Color+0x12>
 800c06c:	f7ff fd72 	bl	800bb54 <Read_Pixel>
 800c070:	f825 0014 	strh.w	r0, [r5, r4, lsl #1]
 800c074:	e7f8      	b.n	800c068 <RW_Row_Color+0x34>

0800c076 <Set_Color>:
 800c076:	4b02      	ldr	r3, [pc, #8]	; (800c080 <Set_Color+0xa>)
 800c078:	8018      	strh	r0, [r3, #0]
 800c07a:	8059      	strh	r1, [r3, #2]
 800c07c:	4770      	bx	lr
 800c07e:	bf00      	nop
 800c080:	200009be 	.word	0x200009be

0800c084 <Show_Str6x8>:
 800c084:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800c088:	469a      	mov	sl, r3
 800c08a:	b089      	sub	sp, #36	; 0x24
 800c08c:	e9cd 0103 	strd	r0, r1, [sp, #12]
 800c090:	9205      	str	r2, [sp, #20]
 800c092:	f7ff fd4f 	bl	800bb34 <Set_Posi>
 800c096:	f1ba 0f00 	cmp.w	sl, #0
 800c09a:	bf14      	ite	ne
 800c09c:	2306      	movne	r3, #6
 800c09e:	2307      	moveq	r3, #7
 800c0a0:	9306      	str	r3, [sp, #24]
 800c0a2:	9b12      	ldr	r3, [sp, #72]	; 0x48
 800c0a4:	4d3e      	ldr	r5, [pc, #248]	; (800c1a0 <Show_Str6x8+0x11c>)
 800c0a6:	3b01      	subs	r3, #1
 800c0a8:	9302      	str	r3, [sp, #8]
 800c0aa:	9b02      	ldr	r3, [sp, #8]
 800c0ac:	f813 9f01 	ldrb.w	r9, [r3, #1]!
 800c0b0:	9302      	str	r3, [sp, #8]
 800c0b2:	f1b9 0f00 	cmp.w	r9, #0
 800c0b6:	d102      	bne.n	800c0be <Show_Str6x8+0x3a>
 800c0b8:	b009      	add	sp, #36	; 0x24
 800c0ba:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800c0be:	2106      	movs	r1, #6
 800c0c0:	faba f38a 	clz	r3, sl
 800c0c4:	f04f 0800 	mov.w	r8, #0
 800c0c8:	f1a9 0221 	sub.w	r2, r9, #33	; 0x21
 800c0cc:	434a      	muls	r2, r1
 800c0ce:	095b      	lsrs	r3, r3, #5
 800c0d0:	b29e      	uxth	r6, r3
 800c0d2:	9201      	str	r2, [sp, #4]
 800c0d4:	4413      	add	r3, r2
 800c0d6:	4a33      	ldr	r2, [pc, #204]	; (800c1a4 <Show_Str6x8+0x120>)
 800c0d8:	18d3      	adds	r3, r2, r3
 800c0da:	9307      	str	r3, [sp, #28]
 800c0dc:	9b03      	ldr	r3, [sp, #12]
 800c0de:	eb03 0708 	add.w	r7, r3, r8
 800c0e2:	9b06      	ldr	r3, [sp, #24]
 800c0e4:	b2bf      	uxth	r7, r7
 800c0e6:	42b3      	cmp	r3, r6
 800c0e8:	d801      	bhi.n	800c0ee <Show_Str6x8+0x6a>
 800c0ea:	9703      	str	r7, [sp, #12]
 800c0ec:	e7dd      	b.n	800c0aa <Show_Str6x8+0x26>
 800c0ee:	9b05      	ldr	r3, [sp, #20]
 800c0f0:	f1ba 0f01 	cmp.w	sl, #1
 800c0f4:	f003 0320 	and.w	r3, r3, #32
 800c0f8:	d128      	bne.n	800c14c <Show_Str6x8+0xc8>
 800c0fa:	bb0b      	cbnz	r3, 800c140 <Show_Str6x8+0xbc>
 800c0fc:	4631      	mov	r1, r6
 800c0fe:	4648      	mov	r0, r9
 800c100:	f7ff fe1e 	bl	800bd40 <Get_TAB_6x8>
 800c104:	4604      	mov	r4, r0
 800c106:	4638      	mov	r0, r7
 800c108:	9904      	ldr	r1, [sp, #16]
 800c10a:	f7ff fd13 	bl	800bb34 <Set_Posi>
 800c10e:	9b05      	ldr	r3, [sp, #20]
 800c110:	f013 0b10 	ands.w	fp, r3, #16
 800c114:	bf14      	ite	ne
 800c116:	8868      	ldrhne	r0, [r5, #2]
 800c118:	8828      	ldrheq	r0, [r5, #0]
 800c11a:	f7ff fd17 	bl	800bb4c <Set_Pixel>
 800c11e:	9b04      	ldr	r3, [sp, #16]
 800c120:	4638      	mov	r0, r7
 800c122:	1c59      	adds	r1, r3, #1
 800c124:	b289      	uxth	r1, r1
 800c126:	f7ff fd05 	bl	800bb34 <Set_Posi>
 800c12a:	2709      	movs	r7, #9
 800c12c:	f1ba 0f01 	cmp.w	sl, #1
 800c130:	d125      	bne.n	800c17e <Show_Str6x8+0xfa>
 800c132:	07e2      	lsls	r2, r4, #31
 800c134:	d512      	bpl.n	800c15c <Show_Str6x8+0xd8>
 800c136:	f1bb 0f00 	cmp.w	fp, #0
 800c13a:	d012      	beq.n	800c162 <Show_Str6x8+0xde>
 800c13c:	8828      	ldrh	r0, [r5, #0]
 800c13e:	e011      	b.n	800c164 <Show_Str6x8+0xe0>
 800c140:	4b18      	ldr	r3, [pc, #96]	; (800c1a4 <Show_Str6x8+0x120>)
 800c142:	9a01      	ldr	r2, [sp, #4]
 800c144:	4413      	add	r3, r2
 800c146:	f813 4008 	ldrb.w	r4, [r3, r8]
 800c14a:	e7dc      	b.n	800c106 <Show_Str6x8+0x82>
 800c14c:	b923      	cbnz	r3, 800c158 <Show_Str6x8+0xd4>
 800c14e:	4631      	mov	r1, r6
 800c150:	4648      	mov	r0, r9
 800c152:	f7ff fe07 	bl	800bd64 <Get_Mini6x8>
 800c156:	e7d5      	b.n	800c104 <Show_Str6x8+0x80>
 800c158:	9b07      	ldr	r3, [sp, #28]
 800c15a:	e7f4      	b.n	800c146 <Show_Str6x8+0xc2>
 800c15c:	f1bb 0f00 	cmp.w	fp, #0
 800c160:	d0ec      	beq.n	800c13c <Show_Str6x8+0xb8>
 800c162:	8868      	ldrh	r0, [r5, #2]
 800c164:	f7ff fcf2 	bl	800bb4c <Set_Pixel>
 800c168:	f3c4 044f 	ubfx	r4, r4, #1, #16
 800c16c:	3f01      	subs	r7, #1
 800c16e:	b2bf      	uxth	r7, r7
 800c170:	2f00      	cmp	r7, #0
 800c172:	d1db      	bne.n	800c12c <Show_Str6x8+0xa8>
 800c174:	3601      	adds	r6, #1
 800c176:	b2b6      	uxth	r6, r6
 800c178:	f108 0801 	add.w	r8, r8, #1
 800c17c:	e7ae      	b.n	800c0dc <Show_Str6x8+0x58>
 800c17e:	0623      	lsls	r3, r4, #24
 800c180:	d504      	bpl.n	800c18c <Show_Str6x8+0x108>
 800c182:	f1bb 0f00 	cmp.w	fp, #0
 800c186:	d004      	beq.n	800c192 <Show_Str6x8+0x10e>
 800c188:	8828      	ldrh	r0, [r5, #0]
 800c18a:	e003      	b.n	800c194 <Show_Str6x8+0x110>
 800c18c:	f1bb 0f00 	cmp.w	fp, #0
 800c190:	d0fa      	beq.n	800c188 <Show_Str6x8+0x104>
 800c192:	8868      	ldrh	r0, [r5, #2]
 800c194:	0064      	lsls	r4, r4, #1
 800c196:	f7ff fcd9 	bl	800bb4c <Set_Pixel>
 800c19a:	b2a4      	uxth	r4, r4
 800c19c:	e7e6      	b.n	800c16c <Show_Str6x8+0xe8>
 800c19e:	bf00      	nop
 800c1a0:	200009be 	.word	0x200009be
 800c1a4:	08014360 	.word	0x08014360

0800c1a8 <FpgaRW>:
 800c1a8:	b507      	push	{r0, r1, r2, lr}
 800c1aa:	7010      	strb	r0, [r2, #0]
 800c1ac:	f8ad 1000 	strh.w	r1, [sp]
 800c1b0:	2010      	movs	r0, #16
 800c1b2:	4669      	mov	r1, sp
 800c1b4:	9201      	str	r2, [sp, #4]
 800c1b6:	f7f3 ffa3 	bl	8000100 <__Bios>
 800c1ba:	b003      	add	sp, #12
 800c1bc:	f85d fb04 	ldr.w	pc, [sp], #4

0800c1c0 <ProgmIO_SPIWr>:
 800c1c0:	b507      	push	{r0, r1, r2, lr}
 800c1c2:	f8ad 0000 	strh.w	r0, [sp]
 800c1c6:	9101      	str	r1, [sp, #4]
 800c1c8:	201a      	movs	r0, #26
 800c1ca:	4669      	mov	r1, sp
 800c1cc:	f7f3 ff98 	bl	8000100 <__Bios>
 800c1d0:	b003      	add	sp, #12
 800c1d2:	f85d fb04 	ldr.w	pc, [sp], #4

0800c1d6 <ProgmIO_UARTTx>:
 800c1d6:	4601      	mov	r1, r0
 800c1d8:	201b      	movs	r0, #27
 800c1da:	f7f3 bf91 	b.w	8000100 <__Bios>

0800c1de <ProgmIO_I2CWr>:
 800c1de:	ea42 2101 	orr.w	r1, r2, r1, lsl #8
 800c1e2:	ea41 4100 	orr.w	r1, r1, r0, lsl #16
 800c1e6:	201c      	movs	r0, #28
 800c1e8:	f7f3 bf8a 	b.w	8000100 <__Bios>

0800c1ec <FPGA_ReadWrite>:
 800c1ec:	b537      	push	{r0, r1, r2, r4, r5, lr}
 800c1ee:	4c0a      	ldr	r4, [pc, #40]	; (800c218 <FPGA_ReadWrite+0x2c>)
 800c1f0:	ab02      	add	r3, sp, #8
 800c1f2:	6825      	ldr	r5, [r4, #0]
 800c1f4:	f823 1d08 	strh.w	r1, [r3, #-8]!
 800c1f8:	9501      	str	r5, [sp, #4]
 800c1fa:	7028      	strb	r0, [r5, #0]
 800c1fc:	6861      	ldr	r1, [r4, #4]
 800c1fe:	2010      	movs	r0, #16
 800c200:	800a      	strh	r2, [r1, #0]
 800c202:	4619      	mov	r1, r3
 800c204:	f7f3 ff7c 	bl	8000100 <__Bios>
 800c208:	68a3      	ldr	r3, [r4, #8]
 800c20a:	7818      	ldrb	r0, [r3, #0]
 800c20c:	68e3      	ldr	r3, [r4, #12]
 800c20e:	781b      	ldrb	r3, [r3, #0]
 800c210:	ea43 2000 	orr.w	r0, r3, r0, lsl #8
 800c214:	b003      	add	sp, #12
 800c216:	bd30      	pop	{r4, r5, pc}
 800c218:	200000e0 	.word	0x200000e0

0800c21c <FPGA_LCD_RW>:
 800c21c:	b513      	push	{r0, r1, r4, lr}
 800c21e:	ab02      	add	r3, sp, #8
 800c220:	4614      	mov	r4, r2
 800c222:	f823 1d08 	strh.w	r1, [r3, #-8]!
 800c226:	7010      	strb	r0, [r2, #0]
 800c228:	4619      	mov	r1, r3
 800c22a:	2010      	movs	r0, #16
 800c22c:	9201      	str	r2, [sp, #4]
 800c22e:	f7f3 ff67 	bl	8000100 <__Bios>
 800c232:	7820      	ldrb	r0, [r4, #0]
 800c234:	b002      	add	sp, #8
 800c236:	bd10      	pop	{r4, pc}

0800c238 <FPGA_RW_4Byte>:
 800c238:	b537      	push	{r0, r1, r2, r4, r5, lr}
 800c23a:	4c0a      	ldr	r4, [pc, #40]	; (800c264 <FPGA_RW_4Byte+0x2c>)
 800c23c:	ab02      	add	r3, sp, #8
 800c23e:	6825      	ldr	r5, [r4, #0]
 800c240:	f823 1d08 	strh.w	r1, [r3, #-8]!
 800c244:	9501      	str	r5, [sp, #4]
 800c246:	7028      	strb	r0, [r5, #0]
 800c248:	6921      	ldr	r1, [r4, #16]
 800c24a:	2010      	movs	r0, #16
 800c24c:	600a      	str	r2, [r1, #0]
 800c24e:	4619      	mov	r1, r3
 800c250:	f7f3 ff56 	bl	8000100 <__Bios>
 800c254:	68a3      	ldr	r3, [r4, #8]
 800c256:	6922      	ldr	r2, [r4, #16]
 800c258:	781b      	ldrb	r3, [r3, #0]
 800c25a:	6810      	ldr	r0, [r2, #0]
 800c25c:	ea43 2000 	orr.w	r0, r3, r0, lsl #8
 800c260:	b003      	add	sp, #12
 800c262:	bd30      	pop	{r4, r5, pc}
 800c264:	200000e0 	.word	0x200000e0

0800c268 <FPGA_Read>:
 800c268:	b537      	push	{r0, r1, r2, r4, r5, lr}
 800c26a:	4c08      	ldr	r4, [pc, #32]	; (800c28c <FPGA_Read+0x24>)
 800c26c:	ab02      	add	r3, sp, #8
 800c26e:	6825      	ldr	r5, [r4, #0]
 800c270:	f823 1d08 	strh.w	r1, [r3, #-8]!
 800c274:	9501      	str	r5, [sp, #4]
 800c276:	7028      	strb	r0, [r5, #0]
 800c278:	6861      	ldr	r1, [r4, #4]
 800c27a:	2010      	movs	r0, #16
 800c27c:	800a      	strh	r2, [r1, #0]
 800c27e:	4619      	mov	r1, r3
 800c280:	f7f3 ff3e 	bl	8000100 <__Bios>
 800c284:	6863      	ldr	r3, [r4, #4]
 800c286:	8818      	ldrh	r0, [r3, #0]
 800c288:	b003      	add	sp, #12
 800c28a:	bd30      	pop	{r4, r5, pc}
 800c28c:	200000e0 	.word	0x200000e0

0800c290 <Delay_uS>:
 800c290:	b082      	sub	sp, #8
 800c292:	9001      	str	r0, [sp, #4]
 800c294:	9b01      	ldr	r3, [sp, #4]
 800c296:	ebc3 1383 	rsb	r3, r3, r3, lsl #6
 800c29a:	08db      	lsrs	r3, r3, #3
 800c29c:	9301      	str	r3, [sp, #4]
 800c29e:	9b01      	ldr	r3, [sp, #4]
 800c2a0:	1e5a      	subs	r2, r3, #1
 800c2a2:	9201      	str	r2, [sp, #4]
 800c2a4:	2b00      	cmp	r3, #0
 800c2a6:	d1fa      	bne.n	800c29e <Delay_uS+0xe>
 800c2a8:	b002      	add	sp, #8
 800c2aa:	4770      	bx	lr

0800c2ac <Delay_mS>:
 800c2ac:	b082      	sub	sp, #8
 800c2ae:	9001      	str	r0, [sp, #4]
 800c2b0:	9a01      	ldr	r2, [sp, #4]
 800c2b2:	4b04      	ldr	r3, [pc, #16]	; (800c2c4 <Delay_mS+0x18>)
 800c2b4:	b292      	uxth	r2, r2
 800c2b6:	801a      	strh	r2, [r3, #0]
 800c2b8:	881a      	ldrh	r2, [r3, #0]
 800c2ba:	b292      	uxth	r2, r2
 800c2bc:	2a00      	cmp	r2, #0
 800c2be:	d1fb      	bne.n	800c2b8 <Delay_mS+0xc>
 800c2c0:	b002      	add	sp, #8
 800c2c2:	4770      	bx	lr
 800c2c4:	200009ae 	.word	0x200009ae

0800c2c8 <NVIC_Conifg>:
 800c2c8:	b507      	push	{r0, r1, r2, lr}
 800c2ca:	f44f 60a0 	mov.w	r0, #1280	; 0x500
 800c2ce:	f004 f94d 	bl	801056c <NVIC_PriorityGroupConfig>
 800c2d2:	f44f 7305 	mov.w	r3, #532	; 0x214
 800c2d6:	f8ad 3004 	strh.w	r3, [sp, #4]
 800c2da:	2300      	movs	r3, #0
 800c2dc:	f88d 3006 	strb.w	r3, [sp, #6]
 800c2e0:	2301      	movs	r3, #1
 800c2e2:	a801      	add	r0, sp, #4
 800c2e4:	f88d 3007 	strb.w	r3, [sp, #7]
 800c2e8:	f004 f94a 	bl	8010580 <NVIC_Init>
 800c2ec:	b003      	add	sp, #12
 800c2ee:	f85d fb04 	ldr.w	pc, [sp], #4

0800c2f2 <DiskConfig>:
 800c2f2:	b508      	push	{r3, lr}
 800c2f4:	f004 fa1c 	bl	8010730 <USB_Init>
 800c2f8:	f002 feae 	bl	800f058 <Disk_Init>
 800c2fc:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 800c300:	f002 bf92 	b.w	800f228 <Init_Fat_Value>

0800c304 <ExtFlash_CS_LOW>:
 800c304:	2100      	movs	r1, #0
 800c306:	2015      	movs	r0, #21
 800c308:	f7f3 befa 	b.w	8000100 <__Bios>

0800c30c <ExtFlash_CS_HIGH>:
 800c30c:	2101      	movs	r1, #1
 800c30e:	2015      	movs	r0, #21
 800c310:	f7f3 bef6 	b.w	8000100 <__Bios>

0800c314 <PIO_PWM_SetFreq.part.1>:
 800c314:	b510      	push	{r4, lr}
 800c316:	460c      	mov	r4, r1
 800c318:	f5a0 2120 	sub.w	r1, r0, #655360	; 0xa0000
 800c31c:	2018      	movs	r0, #24
 800c31e:	f7f3 feef 	bl	8000100 <__Bios>
 800c322:	f5a4 2130 	sub.w	r1, r4, #720896	; 0xb0000
 800c326:	2018      	movs	r0, #24
 800c328:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800c32c:	f7f3 bee8 	b.w	8000100 <__Bios>

0800c330 <PIO_PWM_SetDuty.part.2>:
 800c330:	f5a0 2150 	sub.w	r1, r0, #851968	; 0xd0000
 800c334:	2018      	movs	r0, #24
 800c336:	f7f3 bee3 	b.w	8000100 <__Bios>

0800c33a <TriConfig>:
 800c33a:	b510      	push	{r4, lr}
 800c33c:	4c15      	ldr	r4, [pc, #84]	; (800c394 <TriConfig+0x5a>)
 800c33e:	2101      	movs	r1, #1
 800c340:	8d62      	ldrh	r2, [r4, #42]	; 0x2a
 800c342:	2021      	movs	r0, #33	; 0x21
 800c344:	0a12      	lsrs	r2, r2, #8
 800c346:	f7ff ff51 	bl	800c1ec <FPGA_ReadWrite>
 800c34a:	2001      	movs	r0, #1
 800c34c:	f7ff ffae 	bl	800c2ac <Delay_mS>
 800c350:	8da2      	ldrh	r2, [r4, #44]	; 0x2c
 800c352:	2101      	movs	r1, #1
 800c354:	0a12      	lsrs	r2, r2, #8
 800c356:	2022      	movs	r0, #34	; 0x22
 800c358:	f7ff ff48 	bl	800c1ec <FPGA_ReadWrite>
 800c35c:	2001      	movs	r0, #1
 800c35e:	f7ff ffa5 	bl	800c2ac <Delay_mS>
 800c362:	8aa2      	ldrh	r2, [r4, #20]
 800c364:	4b0c      	ldr	r3, [pc, #48]	; (800c398 <TriConfig+0x5e>)
 800c366:	2102      	movs	r1, #2
 800c368:	f853 2022 	ldr.w	r2, [r3, r2, lsl #2]
 800c36c:	8de3      	ldrh	r3, [r4, #46]	; 0x2e
 800c36e:	2023      	movs	r0, #35	; 0x23
 800c370:	435a      	muls	r2, r3
 800c372:	b292      	uxth	r2, r2
 800c374:	f7ff ff3a 	bl	800c1ec <FPGA_ReadWrite>
 800c378:	2001      	movs	r0, #1
 800c37a:	f7ff ff97 	bl	800c2ac <Delay_mS>
 800c37e:	8d22      	ldrh	r2, [r4, #40]	; 0x28
 800c380:	2101      	movs	r1, #1
 800c382:	2020      	movs	r0, #32
 800c384:	f7ff ff32 	bl	800c1ec <FPGA_ReadWrite>
 800c388:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800c38c:	2001      	movs	r0, #1
 800c38e:	f7ff bf8d 	b.w	800c2ac <Delay_mS>
 800c392:	bf00      	nop
 800c394:	2000000c 	.word	0x2000000c
 800c398:	08013268 	.word	0x08013268

0800c39c <DefaultTir>:
 800c39c:	2803      	cmp	r0, #3
 800c39e:	d80c      	bhi.n	800c3ba <DefaultTir+0x1e>
 800c3a0:	e8df f000 	tbb	[pc, r0]
 800c3a4:	02020c02 	.word	0x02020c02
 800c3a8:	4b09      	ldr	r3, [pc, #36]	; (800c3d0 <DefaultTir+0x34>)
 800c3aa:	f893 202a 	ldrb.w	r2, [r3, #42]	; 0x2a
 800c3ae:	855a      	strh	r2, [r3, #42]	; 0x2a
 800c3b0:	f893 202c 	ldrb.w	r2, [r3, #44]	; 0x2c
 800c3b4:	f442 6260 	orr.w	r2, r2, #3584	; 0xe00
 800c3b8:	859a      	strh	r2, [r3, #44]	; 0x2c
 800c3ba:	4770      	bx	lr
 800c3bc:	4b04      	ldr	r3, [pc, #16]	; (800c3d0 <DefaultTir+0x34>)
 800c3be:	f893 202a 	ldrb.w	r2, [r3, #42]	; 0x2a
 800c3c2:	855a      	strh	r2, [r3, #42]	; 0x2a
 800c3c4:	f893 202c 	ldrb.w	r2, [r3, #44]	; 0x2c
 800c3c8:	f442 62e0 	orr.w	r2, r2, #1792	; 0x700
 800c3cc:	e7f4      	b.n	800c3b8 <DefaultTir+0x1c>
 800c3ce:	bf00      	nop
 800c3d0:	2000000c 	.word	0x2000000c

0800c3d4 <ShowWaveToLCD>:
 800c3d4:	b510      	push	{r4, lr}
 800c3d6:	4a29      	ldr	r2, [pc, #164]	; (800c47c <ShowWaveToLCD+0xa8>)
 800c3d8:	f44f 718c 	mov.w	r1, #280	; 0x118
 800c3dc:	2003      	movs	r0, #3
 800c3de:	f7ff ff1d 	bl	800c21c <FPGA_LCD_RW>
 800c3e2:	2005      	movs	r0, #5
 800c3e4:	f7ff ff62 	bl	800c2ac <Delay_mS>
 800c3e8:	4b25      	ldr	r3, [pc, #148]	; (800c480 <ShowWaveToLCD+0xac>)
 800c3ea:	4c26      	ldr	r4, [pc, #152]	; (800c484 <ShowWaveToLCD+0xb0>)
 800c3ec:	e9d3 2300 	ldrd	r2, r3, [r3]
 800c3f0:	4313      	orrs	r3, r2
 800c3f2:	bf0c      	ite	eq
 800c3f4:	2302      	moveq	r3, #2
 800c3f6:	f44f 7396 	movne.w	r3, #300	; 0x12c
 800c3fa:	83a3      	strh	r3, [r4, #28]
 800c3fc:	8ba2      	ldrh	r2, [r4, #28]
 800c3fe:	2102      	movs	r1, #2
 800c400:	2019      	movs	r0, #25
 800c402:	f7ff fef3 	bl	800c1ec <FPGA_ReadWrite>
 800c406:	8ae2      	ldrh	r2, [r4, #22]
 800c408:	2102      	movs	r1, #2
 800c40a:	201a      	movs	r0, #26
 800c40c:	f7ff feee 	bl	800c1ec <FPGA_ReadWrite>
 800c410:	8b22      	ldrh	r2, [r4, #24]
 800c412:	2102      	movs	r1, #2
 800c414:	201b      	movs	r0, #27
 800c416:	f7ff fee9 	bl	800c1ec <FPGA_ReadWrite>
 800c41a:	8be2      	ldrh	r2, [r4, #30]
 800c41c:	2102      	movs	r1, #2
 800c41e:	2018      	movs	r0, #24
 800c420:	f7ff fee4 	bl	800c1ec <FPGA_ReadWrite>
 800c424:	8ba2      	ldrh	r2, [r4, #28]
 800c426:	4b18      	ldr	r3, [pc, #96]	; (800c488 <ShowWaveToLCD+0xb4>)
 800c428:	2142      	movs	r1, #66	; 0x42
 800c42a:	801a      	strh	r2, [r3, #0]
 800c42c:	8ae2      	ldrh	r2, [r4, #22]
 800c42e:	4b17      	ldr	r3, [pc, #92]	; (800c48c <ShowWaveToLCD+0xb8>)
 800c430:	2028      	movs	r0, #40	; 0x28
 800c432:	801a      	strh	r2, [r3, #0]
 800c434:	8b22      	ldrh	r2, [r4, #24]
 800c436:	4b16      	ldr	r3, [pc, #88]	; (800c490 <ShowWaveToLCD+0xbc>)
 800c438:	801a      	strh	r2, [r3, #0]
 800c43a:	8be2      	ldrh	r2, [r4, #30]
 800c43c:	4b15      	ldr	r3, [pc, #84]	; (800c494 <ShowWaveToLCD+0xc0>)
 800c43e:	801a      	strh	r2, [r3, #0]
 800c440:	f240 123f 	movw	r2, #319	; 0x13f
 800c444:	23db      	movs	r3, #219	; 0xdb
 800c446:	f7ff fbad 	bl	800bba4 <Set_Block>
 800c44a:	4913      	ldr	r1, [pc, #76]	; (800c498 <ShowWaveToLCD+0xc4>)
 800c44c:	2008      	movs	r0, #8
 800c44e:	f7f3 fe57 	bl	8000100 <__Bios>
 800c452:	2200      	movs	r2, #0
 800c454:	2006      	movs	r0, #6
 800c456:	4611      	mov	r1, r2
 800c458:	f7ff fec8 	bl	800c1ec <FPGA_ReadWrite>
 800c45c:	2200      	movs	r2, #0
 800c45e:	4611      	mov	r1, r2
 800c460:	4610      	mov	r0, r2
 800c462:	f7ff fec3 	bl	800c1ec <FPGA_ReadWrite>
 800c466:	05c3      	lsls	r3, r0, #23
 800c468:	d4f8      	bmi.n	800c45c <ShowWaveToLCD+0x88>
 800c46a:	490c      	ldr	r1, [pc, #48]	; (800c49c <ShowWaveToLCD+0xc8>)
 800c46c:	2008      	movs	r0, #8
 800c46e:	f7f3 fe47 	bl	8000100 <__Bios>
 800c472:	7820      	ldrb	r0, [r4, #0]
 800c474:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800c478:	f001 bc13 	b.w	800dca2 <Show_AnalyzeData>
 800c47c:	200009da 	.word	0x200009da
 800c480:	20004c18 	.word	0x20004c18
 800c484:	2000000c 	.word	0x2000000c
 800c488:	200009a2 	.word	0x200009a2
 800c48c:	2000099e 	.word	0x2000099e
 800c490:	200009a0 	.word	0x200009a0
 800c494:	200009a4 	.word	0x200009a4
 800c498:	fff90000 	.word	0xfff90000
 800c49c:	fffb0000 	.word	0xfffb0000

0800c4a0 <TransStatusData>:
 800c4a0:	4b02      	ldr	r3, [pc, #8]	; (800c4ac <TransStatusData+0xc>)
 800c4a2:	eb03 0080 	add.w	r0, r3, r0, lsl #2
 800c4a6:	78c0      	ldrb	r0, [r0, #3]
 800c4a8:	0900      	lsrs	r0, r0, #4
 800c4aa:	4770      	bx	lr
 800c4ac:	20000c0c 	.word	0x20000c0c

0800c4b0 <TransStatusInfo>:
 800c4b0:	4b01      	ldr	r3, [pc, #4]	; (800c4b8 <TransStatusInfo+0x8>)
 800c4b2:	f813 0020 	ldrb.w	r0, [r3, r0, lsl #2]
 800c4b6:	4770      	bx	lr
 800c4b8:	20000c0c 	.word	0x20000c0c

0800c4bc <TransformTime>:
 800c4bc:	4b06      	ldr	r3, [pc, #24]	; (800c4d8 <TransformTime+0x1c>)
 800c4be:	eb03 0380 	add.w	r3, r3, r0, lsl #2
 800c4c2:	7858      	ldrb	r0, [r3, #1]
 800c4c4:	789a      	ldrb	r2, [r3, #2]
 800c4c6:	78db      	ldrb	r3, [r3, #3]
 800c4c8:	ea40 2002 	orr.w	r0, r0, r2, lsl #8
 800c4cc:	041b      	lsls	r3, r3, #16
 800c4ce:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
 800c4d2:	4318      	orrs	r0, r3
 800c4d4:	4770      	bx	lr
 800c4d6:	bf00      	nop
 800c4d8:	20000c0c 	.word	0x20000c0c

0800c4dc <CalculateTimeSum>:
 800c4dc:	f5b0 6f60 	cmp.w	r0, #3584	; 0xe00
 800c4e0:	bf28      	it	cs
 800c4e2:	f44f 6060 	movcs.w	r0, #3584	; 0xe00
 800c4e6:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800c4e8:	210a      	movs	r1, #10
 800c4ea:	4607      	mov	r7, r0
 800c4ec:	2400      	movs	r4, #0
 800c4ee:	2500      	movs	r5, #0
 800c4f0:	4e0a      	ldr	r6, [pc, #40]	; (800c51c <CalculateTimeSum+0x40>)
 800c4f2:	428f      	cmp	r7, r1
 800c4f4:	d802      	bhi.n	800c4fc <CalculateTimeSum+0x20>
 800c4f6:	4620      	mov	r0, r4
 800c4f8:	4629      	mov	r1, r5
 800c4fa:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 800c4fc:	f896 3028 	ldrb.w	r3, [r6, #40]	; 0x28
 800c500:	3604      	adds	r6, #4
 800c502:	075b      	lsls	r3, r3, #29
 800c504:	d4f7      	bmi.n	800c4f6 <CalculateTimeSum+0x1a>
 800c506:	4608      	mov	r0, r1
 800c508:	f7ff ffd8 	bl	800c4bc <TransformTime>
 800c50c:	1824      	adds	r4, r4, r0
 800c50e:	f101 0101 	add.w	r1, r1, #1
 800c512:	f145 0500 	adc.w	r5, r5, #0
 800c516:	b289      	uxth	r1, r1
 800c518:	e7eb      	b.n	800c4f2 <CalculateTimeSum+0x16>
 800c51a:	bf00      	nop
 800c51c:	20000c0c 	.word	0x20000c0c

0800c520 <AddAnalyzeStatus>:
 800c520:	4b03      	ldr	r3, [pc, #12]	; (800c530 <AddAnalyzeStatus+0x10>)
 800c522:	f813 3021 	ldrb.w	r3, [r3, r1, lsl #2]
 800c526:	109b      	asrs	r3, r3, #2
 800c528:	f003 0330 	and.w	r3, r3, #48	; 0x30
 800c52c:	4318      	orrs	r0, r3
 800c52e:	4770      	bx	lr
 800c530:	20000c0c 	.word	0x20000c0c

0800c534 <UpdateXposiTime>:
 800c534:	e92d 4830 	stmdb	sp!, {r4, r5, fp, lr}
 800c538:	4b1f      	ldr	r3, [pc, #124]	; (800c5b8 <UpdateXposiTime+0x84>)
 800c53a:	7a1a      	ldrb	r2, [r3, #8]
 800c53c:	e9d3 4500 	ldrd	r4, r5, [r3]
 800c540:	2a01      	cmp	r2, #1
 800c542:	d11a      	bne.n	800c57a <UpdateXposiTime+0x46>
 800c544:	2200      	movs	r2, #0
 800c546:	721a      	strb	r2, [r3, #8]
 800c548:	4a1c      	ldr	r2, [pc, #112]	; (800c5bc <UpdateXposiTime+0x88>)
 800c54a:	f04f 0c00 	mov.w	ip, #0
 800c54e:	8a91      	ldrh	r1, [r2, #20]
 800c550:	4a1b      	ldr	r2, [pc, #108]	; (800c5c0 <UpdateXposiTime+0x8c>)
 800c552:	f852 0021 	ldr.w	r0, [r2, r1, lsl #2]
 800c556:	4683      	mov	fp, r0
 800c558:	1900      	adds	r0, r0, r4
 800c55a:	eb4c 0105 	adc.w	r1, ip, r5
 800c55e:	e9d3 4506 	ldrd	r4, r5, [r3, #24]
 800c562:	428d      	cmp	r5, r1
 800c564:	bf08      	it	eq
 800c566:	4284      	cmpeq	r4, r0
 800c568:	e9c3 bc04 	strd	fp, ip, [r3, #16]
 800c56c:	d21c      	bcs.n	800c5a8 <UpdateXposiTime+0x74>
 800c56e:	e9c3 4500 	strd	r4, r5, [r3]
 800c572:	e9d3 0100 	ldrd	r0, r1, [r3]
 800c576:	e8bd 8830 	ldmia.w	sp!, {r4, r5, fp, pc}
 800c57a:	f893 2020 	ldrb.w	r2, [r3, #32]
 800c57e:	2a01      	cmp	r2, #1
 800c580:	d1f7      	bne.n	800c572 <UpdateXposiTime+0x3e>
 800c582:	2200      	movs	r2, #0
 800c584:	f883 2020 	strb.w	r2, [r3, #32]
 800c588:	4a0c      	ldr	r2, [pc, #48]	; (800c5bc <UpdateXposiTime+0x88>)
 800c58a:	f04f 0c00 	mov.w	ip, #0
 800c58e:	8a91      	ldrh	r1, [r2, #20]
 800c590:	4a0b      	ldr	r2, [pc, #44]	; (800c5c0 <UpdateXposiTime+0x8c>)
 800c592:	45ac      	cmp	ip, r5
 800c594:	f852 0021 	ldr.w	r0, [r2, r1, lsl #2]
 800c598:	4683      	mov	fp, r0
 800c59a:	bf08      	it	eq
 800c59c:	42a0      	cmpeq	r0, r4
 800c59e:	e9c3 bc04 	strd	fp, ip, [r3, #16]
 800c5a2:	d304      	bcc.n	800c5ae <UpdateXposiTime+0x7a>
 800c5a4:	2000      	movs	r0, #0
 800c5a6:	2100      	movs	r1, #0
 800c5a8:	e9c3 0100 	strd	r0, r1, [r3]
 800c5ac:	e7e1      	b.n	800c572 <UpdateXposiTime+0x3e>
 800c5ae:	ebb4 000b 	subs.w	r0, r4, fp
 800c5b2:	eb65 010c 	sbc.w	r1, r5, ip
 800c5b6:	e7f7      	b.n	800c5a8 <UpdateXposiTime+0x74>
 800c5b8:	20004c18 	.word	0x20004c18
 800c5bc:	2000000c 	.word	0x2000000c
 800c5c0:	08013268 	.word	0x08013268

0800c5c4 <ZC_Scale>:
 800c5c4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800c5c8:	4d70      	ldr	r5, [pc, #448]	; (800c78c <ZC_Scale+0x1c8>)
 800c5ca:	b087      	sub	sp, #28
 800c5cc:	f240 1219 	movw	r2, #281	; 0x119
 800c5d0:	2100      	movs	r1, #0
 800c5d2:	486f      	ldr	r0, [pc, #444]	; (800c790 <ZC_Scale+0x1cc>)
 800c5d4:	f006 fab1 	bl	8012b3a <memset>
 800c5d8:	f895 3021 	ldrb.w	r3, [r5, #33]	; 0x21
 800c5dc:	b923      	cbnz	r3, 800c5e8 <ZC_Scale+0x24>
 800c5de:	7a2a      	ldrb	r2, [r5, #8]
 800c5e0:	f895 3020 	ldrb.w	r3, [r5, #32]
 800c5e4:	4313      	orrs	r3, r2
 800c5e6:	d021      	beq.n	800c62c <ZC_Scale+0x68>
 800c5e8:	2300      	movs	r3, #0
 800c5ea:	f885 3021 	strb.w	r3, [r5, #33]	; 0x21
 800c5ee:	f7ff ffa1 	bl	800c534 <UpdateXposiTime>
 800c5f2:	f04f 0800 	mov.w	r8, #0
 800c5f6:	460f      	mov	r7, r1
 800c5f8:	4606      	mov	r6, r0
 800c5fa:	f04f 0900 	mov.w	r9, #0
 800c5fe:	210b      	movs	r1, #11
 800c600:	4608      	mov	r0, r1
 800c602:	f7ff ff5b 	bl	800c4bc <TransformTime>
 800c606:	eb18 0800 	adds.w	r8, r8, r0
 800c60a:	f149 0900 	adc.w	r9, r9, #0
 800c60e:	45b9      	cmp	r9, r7
 800c610:	f101 33ff 	add.w	r3, r1, #4294967295	; 0xffffffff
 800c614:	bf08      	it	eq
 800c616:	45b0      	cmpeq	r8, r6
 800c618:	b29b      	uxth	r3, r3
 800c61a:	f0c0 8087 	bcc.w	800c72c <ZC_Scale+0x168>
 800c61e:	eba8 0606 	sub.w	r6, r8, r6
 800c622:	626e      	str	r6, [r5, #36]	; 0x24
 800c624:	852b      	strh	r3, [r5, #40]	; 0x28
 800c626:	856b      	strh	r3, [r5, #42]	; 0x2a
 800c628:	2300      	movs	r3, #0
 800c62a:	85ab      	strh	r3, [r5, #44]	; 0x2c
 800c62c:	e9d5 3400 	ldrd	r3, r4, [r5]
 800c630:	e9cd 3400 	strd	r3, r4, [sp]
 800c634:	4323      	orrs	r3, r4
 800c636:	f8b5 902a 	ldrh.w	r9, [r5, #42]	; 0x2a
 800c63a:	d17f      	bne.n	800c73c <ZC_Scale+0x178>
 800c63c:	2006      	movs	r0, #6
 800c63e:	f7ff ff2f 	bl	800c4a0 <TransStatusData>
 800c642:	4a53      	ldr	r2, [pc, #332]	; (800c790 <ZC_Scale+0x1cc>)
 800c644:	2602      	movs	r6, #2
 800c646:	7010      	strb	r0, [r2, #0]
 800c648:	2007      	movs	r0, #7
 800c64a:	f7ff ff29 	bl	800c4a0 <TransStatusData>
 800c64e:	4683      	mov	fp, r0
 800c650:	7050      	strb	r0, [r2, #1]
 800c652:	464f      	mov	r7, r9
 800c654:	f04f 0800 	mov.w	r8, #0
 800c658:	4b4e      	ldr	r3, [pc, #312]	; (800c794 <ZC_Scale+0x1d0>)
 800c65a:	8dea      	ldrh	r2, [r5, #46]	; 0x2e
 800c65c:	f8df a140 	ldr.w	sl, [pc, #320]	; 800c7a0 <ZC_Scale+0x1dc>
 800c660:	839a      	strh	r2, [r3, #28]
 800c662:	b2b2      	uxth	r2, r6
 800c664:	9203      	str	r2, [sp, #12]
 800c666:	9304      	str	r3, [sp, #16]
 800c668:	f5b7 6f60 	cmp.w	r7, #3584	; 0xe00
 800c66c:	f280 8084 	bge.w	800c778 <ZC_Scale+0x1b4>
 800c670:	b2bb      	uxth	r3, r7
 800c672:	f81a 3023 	ldrb.w	r3, [sl, r3, lsl #2]
 800c676:	0759      	lsls	r1, r3, #29
 800c678:	d47e      	bmi.n	800c778 <ZC_Scale+0x1b4>
 800c67a:	b2ba      	uxth	r2, r7
 800c67c:	4610      	mov	r0, r2
 800c67e:	f7ff ff0f 	bl	800c4a0 <TransStatusData>
 800c682:	454f      	cmp	r7, r9
 800c684:	4683      	mov	fp, r0
 800c686:	d15c      	bne.n	800c742 <ZC_Scale+0x17e>
 800c688:	e9dd 3400 	ldrd	r3, r4, [sp]
 800c68c:	4323      	orrs	r3, r4
 800c68e:	6a69      	ldr	r1, [r5, #36]	; 0x24
 800c690:	d102      	bne.n	800c698 <ZC_Scale+0xd4>
 800c692:	2902      	cmp	r1, #2
 800c694:	d900      	bls.n	800c698 <ZC_Scale+0xd4>
 800c696:	3902      	subs	r1, #2
 800c698:	4b3e      	ldr	r3, [pc, #248]	; (800c794 <ZC_Scale+0x1d0>)
 800c69a:	eb01 0481 	add.w	r4, r1, r1, lsl #2
 800c69e:	8a9a      	ldrh	r2, [r3, #20]
 800c6a0:	4b3d      	ldr	r3, [pc, #244]	; (800c798 <ZC_Scale+0x1d4>)
 800c6a2:	454f      	cmp	r7, r9
 800c6a4:	f853 3022 	ldr.w	r3, [r3, r2, lsl #2]
 800c6a8:	fbb4 f4f3 	udiv	r4, r4, r3
 800c6ac:	b2a4      	uxth	r4, r4
 800c6ae:	d160      	bne.n	800c772 <ZC_Scale+0x1ae>
 800c6b0:	2c00      	cmp	r4, #0
 800c6b2:	bf08      	it	eq
 800c6b4:	2401      	moveq	r4, #1
 800c6b6:	4b36      	ldr	r3, [pc, #216]	; (800c790 <ZC_Scale+0x1cc>)
 800c6b8:	b2b9      	uxth	r1, r7
 800c6ba:	18f3      	adds	r3, r6, r3
 800c6bc:	4658      	mov	r0, fp
 800c6be:	19a2      	adds	r2, r4, r6
 800c6c0:	9305      	str	r3, [sp, #20]
 800c6c2:	f7ff ff2d 	bl	800c520 <AddAnalyzeStatus>
 800c6c6:	f5b2 7f8c 	cmp.w	r2, #280	; 0x118
 800c6ca:	bfa8      	it	ge
 800c6cc:	f5c6 748c 	rsbge	r4, r6, #280	; 0x118
 800c6d0:	9b05      	ldr	r3, [sp, #20]
 800c6d2:	bfa8      	it	ge
 800c6d4:	b2a4      	uxthge	r4, r4
 800c6d6:	4601      	mov	r1, r0
 800c6d8:	4683      	mov	fp, r0
 800c6da:	4622      	mov	r2, r4
 800c6dc:	4618      	mov	r0, r3
 800c6de:	4426      	add	r6, r4
 800c6e0:	f006 fa2b 	bl	8012b3a <memset>
 800c6e4:	f5b6 7f8c 	cmp.w	r6, #280	; 0x118
 800c6e8:	dc0a      	bgt.n	800c700 <ZC_Scale+0x13c>
 800c6ea:	2300      	movs	r3, #0
 800c6ec:	3701      	adds	r7, #1
 800c6ee:	862b      	strh	r3, [r5, #48]	; 0x30
 800c6f0:	9b03      	ldr	r3, [sp, #12]
 800c6f2:	f108 0801 	add.w	r8, r8, #1
 800c6f6:	4443      	add	r3, r8
 800c6f8:	b29b      	uxth	r3, r3
 800c6fa:	f5b3 7f8c 	cmp.w	r3, #280	; 0x118
 800c6fe:	d9b3      	bls.n	800c668 <ZC_Scale+0xa4>
 800c700:	f240 1219 	movw	r2, #281	; 0x119
 800c704:	4922      	ldr	r1, [pc, #136]	; (800c790 <ZC_Scale+0x1cc>)
 800c706:	4825      	ldr	r0, [pc, #148]	; (800c79c <ZC_Scale+0x1d8>)
 800c708:	f006 fa0c 	bl	8012b24 <memcpy>
 800c70c:	f7ff fe62 	bl	800c3d4 <ShowWaveToLCD>
 800c710:	9b04      	ldr	r3, [sp, #16]
 800c712:	f44f 728c 	mov.w	r2, #280	; 0x118
 800c716:	7d1b      	ldrb	r3, [r3, #20]
 800c718:	2137      	movs	r1, #55	; 0x37
 800c71a:	2029      	movs	r0, #41	; 0x29
 800c71c:	f7fd fe4a 	bl	800a3b4 <ShowScaleRuleTime>
 800c720:	f7fd fed0 	bl	800a4c4 <ShowTimeBaseStr>
 800c724:	2001      	movs	r0, #1
 800c726:	b007      	add	sp, #28
 800c728:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800c72c:	3101      	adds	r1, #1
 800c72e:	b289      	uxth	r1, r1
 800c730:	f5b1 6f60 	cmp.w	r1, #3584	; 0xe00
 800c734:	852b      	strh	r3, [r5, #40]	; 0x28
 800c736:	f47f af63 	bne.w	800c600 <ZC_Scale+0x3c>
 800c73a:	e774      	b.n	800c626 <ZC_Scale+0x62>
 800c73c:	2600      	movs	r6, #0
 800c73e:	46b3      	mov	fp, r6
 800c740:	e787      	b.n	800c652 <ZC_Scale+0x8e>
 800c742:	1c50      	adds	r0, r2, #1
 800c744:	b280      	uxth	r0, r0
 800c746:	f7ff feb9 	bl	800c4bc <TransformTime>
 800c74a:	4601      	mov	r1, r0
 800c74c:	e7a4      	b.n	800c698 <ZC_Scale+0xd4>
 800c74e:	3701      	adds	r7, #1
 800c750:	1c78      	adds	r0, r7, #1
 800c752:	b280      	uxth	r0, r0
 800c754:	9305      	str	r3, [sp, #20]
 800c756:	f7ff feb1 	bl	800c4bc <TransformTime>
 800c75a:	9b05      	ldr	r3, [sp, #20]
 800c75c:	4401      	add	r1, r0
 800c75e:	eb01 0481 	add.w	r4, r1, r1, lsl #2
 800c762:	fbb4 f4f3 	udiv	r4, r4, r3
 800c766:	b2ba      	uxth	r2, r7
 800c768:	f81a 2022 	ldrb.w	r2, [sl, r2, lsl #2]
 800c76c:	b2a4      	uxth	r4, r4
 800c76e:	0752      	lsls	r2, r2, #29
 800c770:	d4a1      	bmi.n	800c6b6 <ZC_Scale+0xf2>
 800c772:	2c00      	cmp	r4, #0
 800c774:	d0eb      	beq.n	800c74e <ZC_Scale+0x18a>
 800c776:	e79e      	b.n	800c6b6 <ZC_Scale+0xf2>
 800c778:	f5b6 7f8c 	cmp.w	r6, #280	; 0x118
 800c77c:	dcb8      	bgt.n	800c6f0 <ZC_Scale+0x12c>
 800c77e:	4b04      	ldr	r3, [pc, #16]	; (800c790 <ZC_Scale+0x1cc>)
 800c780:	f806 b003 	strb.w	fp, [r6, r3]
 800c784:	8e2b      	ldrh	r3, [r5, #48]	; 0x30
 800c786:	3601      	adds	r6, #1
 800c788:	3301      	adds	r3, #1
 800c78a:	e7b0      	b.n	800c6ee <ZC_Scale+0x12a>
 800c78c:	20004c18 	.word	0x20004c18
 800c790:	200009da 	.word	0x200009da
 800c794:	2000000c 	.word	0x2000000c
 800c798:	08013268 	.word	0x08013268
 800c79c:	20000af3 	.word	0x20000af3
 800c7a0:	20000c0c 	.word	0x20000c0c

0800c7a4 <ResetPowerOffTime>:
 800c7a4:	b510      	push	{r4, lr}
 800c7a6:	4b0e      	ldr	r3, [pc, #56]	; (800c7e0 <ResetPowerOffTime+0x3c>)
 800c7a8:	4c0e      	ldr	r4, [pc, #56]	; (800c7e4 <ResetPowerOffTime+0x40>)
 800c7aa:	781a      	ldrb	r2, [r3, #0]
 800c7ac:	b142      	cbz	r2, 800c7c0 <ResetPowerOffTime+0x1c>
 800c7ae:	2200      	movs	r2, #0
 800c7b0:	210a      	movs	r1, #10
 800c7b2:	701a      	strb	r2, [r3, #0]
 800c7b4:	f8b4 307e 	ldrh.w	r3, [r4, #126]	; 0x7e
 800c7b8:	2008      	movs	r0, #8
 800c7ba:	4359      	muls	r1, r3
 800c7bc:	f7f3 fca0 	bl	8000100 <__Bios>
 800c7c0:	f8b4 3080 	ldrh.w	r3, [r4, #128]	; 0x80
 800c7c4:	4a08      	ldr	r2, [pc, #32]	; (800c7e8 <ResetPowerOffTime+0x44>)
 800c7c6:	ebc3 1303 	rsb	r3, r3, r3, lsl #4
 800c7ca:	009b      	lsls	r3, r3, #2
 800c7cc:	8013      	strh	r3, [r2, #0]
 800c7ce:	f8b4 3082 	ldrh.w	r3, [r4, #130]	; 0x82
 800c7d2:	4a06      	ldr	r2, [pc, #24]	; (800c7ec <ResetPowerOffTime+0x48>)
 800c7d4:	ebc3 1303 	rsb	r3, r3, r3, lsl #4
 800c7d8:	009b      	lsls	r3, r3, #2
 800c7da:	8013      	strh	r3, [r2, #0]
 800c7dc:	bd10      	pop	{r4, pc}
 800c7de:	bf00      	nop
 800c7e0:	200009d8 	.word	0x200009d8
 800c7e4:	2000000c 	.word	0x2000000c
 800c7e8:	200009d4 	.word	0x200009d4
 800c7ec:	200009d2 	.word	0x200009d2

0800c7f0 <GetBatteryVol>:
 800c7f0:	4b02      	ldr	r3, [pc, #8]	; (800c7fc <GetBatteryVol+0xc>)
 800c7f2:	6818      	ldr	r0, [r3, #0]
 800c7f4:	f3c0 108f 	ubfx	r0, r0, #6, #16
 800c7f8:	4770      	bx	lr
 800c7fa:	bf00      	nop
 800c7fc:	200000dc 	.word	0x200000dc

0800c800 <Get_USB_Vol>:
 800c800:	b508      	push	{r3, lr}
 800c802:	4903      	ldr	r1, [pc, #12]	; (800c810 <Get_USB_Vol+0x10>)
 800c804:	2004      	movs	r0, #4
 800c806:	f7f3 fc7b 	bl	8000100 <__Bios>
 800c80a:	b280      	uxth	r0, r0
 800c80c:	bd08      	pop	{r3, pc}
 800c80e:	bf00      	nop
 800c810:	fffc0000 	.word	0xfffc0000

0800c814 <StandbyAndPowerOff>:
 800c814:	b570      	push	{r4, r5, r6, lr}
 800c816:	4c1f      	ldr	r4, [pc, #124]	; (800c894 <StandbyAndPowerOff+0x80>)
 800c818:	f8b4 3080 	ldrh.w	r3, [r4, #128]	; 0x80
 800c81c:	b14b      	cbz	r3, 800c832 <StandbyAndPowerOff+0x1e>
 800c81e:	4b1e      	ldr	r3, [pc, #120]	; (800c898 <StandbyAndPowerOff+0x84>)
 800c820:	881b      	ldrh	r3, [r3, #0]
 800c822:	b933      	cbnz	r3, 800c832 <StandbyAndPowerOff+0x1e>
 800c824:	2101      	movs	r1, #1
 800c826:	2008      	movs	r0, #8
 800c828:	f7f3 fc6a 	bl	8000100 <__Bios>
 800c82c:	2201      	movs	r2, #1
 800c82e:	4b1b      	ldr	r3, [pc, #108]	; (800c89c <StandbyAndPowerOff+0x88>)
 800c830:	701a      	strb	r2, [r3, #0]
 800c832:	f8b4 3082 	ldrh.w	r3, [r4, #130]	; 0x82
 800c836:	b1c3      	cbz	r3, 800c86a <StandbyAndPowerOff+0x56>
 800c838:	4d19      	ldr	r5, [pc, #100]	; (800c8a0 <StandbyAndPowerOff+0x8c>)
 800c83a:	882b      	ldrh	r3, [r5, #0]
 800c83c:	b9ab      	cbnz	r3, 800c86a <StandbyAndPowerOff+0x56>
 800c83e:	f7ff ffdf 	bl	800c800 <Get_USB_Vol>
 800c842:	4606      	mov	r6, r0
 800c844:	b9f0      	cbnz	r0, 800c884 <StandbyAndPowerOff+0x70>
 800c846:	20c8      	movs	r0, #200	; 0xc8
 800c848:	f7ff f920 	bl	800ba8c <Beep_mS>
 800c84c:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
 800c850:	f7ff fd2c 	bl	800c2ac <Delay_mS>
 800c854:	20c8      	movs	r0, #200	; 0xc8
 800c856:	f7ff f919 	bl	800ba8c <Beep_mS>
 800c85a:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
 800c85e:	f7ff fd25 	bl	800c2ac <Delay_mS>
 800c862:	4631      	mov	r1, r6
 800c864:	2004      	movs	r0, #4
 800c866:	f7f3 fc4b 	bl	8000100 <__Bios>
 800c86a:	4b0e      	ldr	r3, [pc, #56]	; (800c8a4 <StandbyAndPowerOff+0x90>)
 800c86c:	681b      	ldr	r3, [r3, #0]
 800c86e:	f3c3 138f 	ubfx	r3, r3, #6, #16
 800c872:	f5b3 6f48 	cmp.w	r3, #3200	; 0xc80
 800c876:	d20c      	bcs.n	800c892 <StandbyAndPowerOff+0x7e>
 800c878:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800c87c:	2100      	movs	r1, #0
 800c87e:	2004      	movs	r0, #4
 800c880:	f7f3 bc3e 	b.w	8000100 <__Bios>
 800c884:	f8b4 3082 	ldrh.w	r3, [r4, #130]	; 0x82
 800c888:	ebc3 1303 	rsb	r3, r3, r3, lsl #4
 800c88c:	009b      	lsls	r3, r3, #2
 800c88e:	802b      	strh	r3, [r5, #0]
 800c890:	e7eb      	b.n	800c86a <StandbyAndPowerOff+0x56>
 800c892:	bd70      	pop	{r4, r5, r6, pc}
 800c894:	2000000c 	.word	0x2000000c
 800c898:	200009d4 	.word	0x200009d4
 800c89c:	200009d8 	.word	0x200009d8
 800c8a0:	200009d2 	.word	0x200009d2
 800c8a4:	200000dc 	.word	0x200000dc

0800c8a8 <ShowMeasureValue>:
 800c8a8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800c8ac:	2500      	movs	r5, #0
 800c8ae:	b097      	sub	sp, #92	; 0x5c
 800c8b0:	4629      	mov	r1, r5
 800c8b2:	ac0b      	add	r4, sp, #44	; 0x2c
 800c8b4:	2210      	movs	r2, #16
 800c8b6:	a80e      	add	r0, sp, #56	; 0x38
 800c8b8:	950b      	str	r5, [sp, #44]	; 0x2c
 800c8ba:	6065      	str	r5, [r4, #4]
 800c8bc:	8125      	strh	r5, [r4, #8]
 800c8be:	e9cd 5507 	strd	r5, r5, [sp, #28]
 800c8c2:	e9cd 5509 	strd	r5, r5, [sp, #36]	; 0x24
 800c8c6:	f006 f938 	bl	8012b3a <memset>
 800c8ca:	4629      	mov	r1, r5
 800c8cc:	2505      	movs	r5, #5
 800c8ce:	2210      	movs	r2, #16
 800c8d0:	a812      	add	r0, sp, #72	; 0x48
 800c8d2:	f006 f932 	bl	8012b3a <memset>
 800c8d6:	4aa2      	ldr	r2, [pc, #648]	; (800cb60 <ShowMeasureValue+0x2b8>)
 800c8d8:	4ba2      	ldr	r3, [pc, #648]	; (800cb64 <ShowMeasureValue+0x2bc>)
 800c8da:	8a90      	ldrh	r0, [r2, #20]
 800c8dc:	8ad1      	ldrh	r1, [r2, #22]
 800c8de:	f853 3020 	ldr.w	r3, [r3, r0, lsl #2]
 800c8e2:	48a1      	ldr	r0, [pc, #644]	; (800cb68 <ShowMeasureValue+0x2c0>)
 800c8e4:	f04f 0800 	mov.w	r8, #0
 800c8e8:	e9d0 6700 	ldrd	r6, r7, [r0]
 800c8ec:	fb03 f001 	mul.w	r0, r3, r1
 800c8f0:	fbb0 f0f5 	udiv	r0, r0, r5
 800c8f4:	f04f 0900 	mov.w	r9, #0
 800c8f8:	250b      	movs	r5, #11
 800c8fa:	eb16 0a00 	adds.w	sl, r6, r0
 800c8fe:	f147 0b00 	adc.w	fp, r7, #0
 800c902:	4628      	mov	r0, r5
 800c904:	9204      	str	r2, [sp, #16]
 800c906:	9302      	str	r3, [sp, #8]
 800c908:	f7ff fdd8 	bl	800c4bc <TransformTime>
 800c90c:	eb18 0800 	adds.w	r8, r8, r0
 800c910:	f149 0900 	adc.w	r9, r9, #0
 800c914:	45d9      	cmp	r9, fp
 800c916:	bf08      	it	eq
 800c918:	45d0      	cmpeq	r8, sl
 800c91a:	9b02      	ldr	r3, [sp, #8]
 800c91c:	9a04      	ldr	r2, [sp, #16]
 800c91e:	f0c0 80e5 	bcc.w	800caec <ShowMeasureValue+0x244>
 800c922:	eba8 0a0a 	sub.w	sl, r8, sl
 800c926:	fa1f fa8a 	uxth.w	sl, sl
 800c92a:	f8b2 8018 	ldrh.w	r8, [r2, #24]
 800c92e:	2205      	movs	r2, #5
 800c930:	4541      	cmp	r1, r8
 800c932:	bf94      	ite	ls
 800c934:	eba8 0801 	subls.w	r8, r8, r1
 800c938:	eba1 0808 	subhi.w	r8, r1, r8
 800c93c:	fba3 0108 	umull	r0, r1, r3, r8
 800c940:	ea4f 79e8 	mov.w	r9, r8, asr #31
 800c944:	fb03 1109 	mla	r1, r3, r9, r1
 800c948:	2300      	movs	r3, #0
 800c94a:	f005 ff69 	bl	8012820 <__aeabi_uldivmod>
 800c94e:	1832      	adds	r2, r6, r0
 800c950:	eb47 0301 	adc.w	r3, r7, r1
 800c954:	e9cd 2304 	strd	r2, r3, [sp, #16]
 800c958:	2300      	movs	r3, #0
 800c95a:	2200      	movs	r2, #0
 800c95c:	e9cd 2302 	strd	r2, r3, [sp, #8]
 800c960:	2300      	movs	r3, #0
 800c962:	3d01      	subs	r5, #1
 800c964:	b2ad      	uxth	r5, r5
 800c966:	4629      	mov	r1, r5
 800c968:	469b      	mov	fp, r3
 800c96a:	f10d 0924 	add.w	r9, sp, #36	; 0x24
 800c96e:	ae12      	add	r6, sp, #72	; 0x48
 800c970:	f10d 0838 	add.w	r8, sp, #56	; 0x38
 800c974:	4608      	mov	r0, r1
 800c976:	f7ff fd93 	bl	800c4a0 <TransStatusData>
 800c97a:	42a9      	cmp	r1, r5
 800c97c:	4607      	mov	r7, r0
 800c97e:	f040 80c0 	bne.w	800cb02 <ShowMeasureValue+0x25a>
 800c982:	4650      	mov	r0, sl
 800c984:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
 800c988:	1812      	adds	r2, r2, r0
 800c98a:	f143 0300 	adc.w	r3, r3, #0
 800c98e:	42a9      	cmp	r1, r5
 800c990:	e9cd 2302 	strd	r2, r3, [sp, #8]
 800c994:	d917      	bls.n	800c9c6 <ShowMeasureValue+0x11e>
 800c996:	2300      	movs	r3, #0
 800c998:	fa4b fe03 	asr.w	lr, fp, r3
 800c99c:	fa47 fc03 	asr.w	ip, r7, r3
 800c9a0:	f01e 0f01 	tst.w	lr, #1
 800c9a4:	f00c 0c01 	and.w	ip, ip, #1
 800c9a8:	f040 80b0 	bne.w	800cb0c <ShowMeasureValue+0x264>
 800c9ac:	f1bc 0f00 	cmp.w	ip, #0
 800c9b0:	d006      	beq.n	800c9c0 <ShowMeasureValue+0x118>
 800c9b2:	aa07      	add	r2, sp, #28
 800c9b4:	f832 c013 	ldrh.w	ip, [r2, r3, lsl #1]
 800c9b8:	f10c 0c01 	add.w	ip, ip, #1
 800c9bc:	f822 c013 	strh.w	ip, [r2, r3, lsl #1]
 800c9c0:	3301      	adds	r3, #1
 800c9c2:	2b04      	cmp	r3, #4
 800c9c4:	d1e8      	bne.n	800c998 <ShowMeasureValue+0xf0>
 800c9c6:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
 800c9ca:	e9dd bc04 	ldrd	fp, ip, [sp, #16]
 800c9ce:	4563      	cmp	r3, ip
 800c9d0:	bf08      	it	eq
 800c9d2:	455a      	cmpeq	r2, fp
 800c9d4:	f0c0 80a5 	bcc.w	800cb22 <ShowMeasureValue+0x27a>
 800c9d8:	9b04      	ldr	r3, [sp, #16]
 800c9da:	a912      	add	r1, sp, #72	; 0x48
 800c9dc:	4418      	add	r0, r3
 800c9de:	9b02      	ldr	r3, [sp, #8]
 800c9e0:	ad0e      	add	r5, sp, #56	; 0x38
 800c9e2:	1ac0      	subs	r0, r0, r3
 800c9e4:	2300      	movs	r3, #0
 800c9e6:	b280      	uxth	r0, r0
 800c9e8:	fa47 f203 	asr.w	r2, r7, r3
 800c9ec:	07d2      	lsls	r2, r2, #31
 800c9ee:	bf4b      	itete	mi
 800c9f0:	f855 2023 	ldrmi.w	r2, [r5, r3, lsl #2]
 800c9f4:	f851 2023 	ldrpl.w	r2, [r1, r3, lsl #2]
 800c9f8:	1812      	addmi	r2, r2, r0
 800c9fa:	1812      	addpl	r2, r2, r0
 800c9fc:	bf4c      	ite	mi
 800c9fe:	f845 2023 	strmi.w	r2, [r5, r3, lsl #2]
 800ca02:	f841 2023 	strpl.w	r2, [r1, r3, lsl #2]
 800ca06:	3301      	adds	r3, #1
 800ca08:	2b04      	cmp	r3, #4
 800ca0a:	d1ed      	bne.n	800c9e8 <ShowMeasureValue+0x140>
 800ca0c:	2520      	movs	r5, #32
 800ca0e:	4e57      	ldr	r6, [pc, #348]	; (800cb6c <ShowMeasureValue+0x2c4>)
 800ca10:	f8df 8160 	ldr.w	r8, [pc, #352]	; 800cb74 <ShowMeasureValue+0x2cc>
 800ca14:	4f56      	ldr	r7, [pc, #344]	; (800cb70 <ShowMeasureValue+0x2c8>)
 800ca16:	f04f 090a 	mov.w	r9, #10
 800ca1a:	f64f 71ff 	movw	r1, #65535	; 0xffff
 800ca1e:	2000      	movs	r0, #0
 800ca20:	f7ff fb29 	bl	800c076 <Set_Color>
 800ca24:	f836 3f02 	ldrh.w	r3, [r6, #2]!
 800ca28:	aa16      	add	r2, sp, #88	; 0x58
 800ca2a:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 800ca2e:	f853 1c20 	ldr.w	r1, [r3, #-32]
 800ca32:	4642      	mov	r2, r8
 800ca34:	2303      	movs	r3, #3
 800ca36:	fb09 f101 	mul.w	r1, r9, r1
 800ca3a:	4620      	mov	r0, r4
 800ca3c:	f001 f9da 	bl	800ddf4 <u32ToStr_Unit>
 800ca40:	4620      	mov	r0, r4
 800ca42:	f006 f891 	bl	8012b68 <strlen>
 800ca46:	b2c2      	uxtb	r2, r0
 800ca48:	f1c2 0206 	rsb	r2, r2, #6
 800ca4c:	4639      	mov	r1, r7
 800ca4e:	4620      	mov	r0, r4
 800ca50:	f006 f892 	bl	8012b78 <strncat>
 800ca54:	2300      	movs	r3, #0
 800ca56:	4629      	mov	r1, r5
 800ca58:	461a      	mov	r2, r3
 800ca5a:	2028      	movs	r0, #40	; 0x28
 800ca5c:	9400      	str	r4, [sp, #0]
 800ca5e:	f7ff fb11 	bl	800c084 <Show_Str6x8>
 800ca62:	8833      	ldrh	r3, [r6, #0]
 800ca64:	aa16      	add	r2, sp, #88	; 0x58
 800ca66:	eb02 0383 	add.w	r3, r2, r3, lsl #2
 800ca6a:	f853 1c10 	ldr.w	r1, [r3, #-16]
 800ca6e:	4642      	mov	r2, r8
 800ca70:	2303      	movs	r3, #3
 800ca72:	fb09 f101 	mul.w	r1, r9, r1
 800ca76:	4620      	mov	r0, r4
 800ca78:	f001 f9bc 	bl	800ddf4 <u32ToStr_Unit>
 800ca7c:	4620      	mov	r0, r4
 800ca7e:	f006 f873 	bl	8012b68 <strlen>
 800ca82:	b2c2      	uxtb	r2, r0
 800ca84:	f1c2 0206 	rsb	r2, r2, #6
 800ca88:	4639      	mov	r1, r7
 800ca8a:	4620      	mov	r0, r4
 800ca8c:	f006 f874 	bl	8012b78 <strncat>
 800ca90:	2300      	movs	r3, #0
 800ca92:	4629      	mov	r1, r5
 800ca94:	461a      	mov	r2, r3
 800ca96:	205e      	movs	r0, #94	; 0x5e
 800ca98:	9400      	str	r4, [sp, #0]
 800ca9a:	f7ff faf3 	bl	800c084 <Show_Str6x8>
 800ca9e:	8833      	ldrh	r3, [r6, #0]
 800caa0:	aa16      	add	r2, sp, #88	; 0x58
 800caa2:	eb02 0343 	add.w	r3, r2, r3, lsl #1
 800caa6:	f813 1c3c 	ldrb.w	r1, [r3, #-60]
 800caaa:	4620      	mov	r0, r4
 800caac:	f001 fbdd 	bl	800e26a <u8ToDec3Str>
 800cab0:	2300      	movs	r3, #0
 800cab2:	4629      	mov	r1, r5
 800cab4:	461a      	mov	r2, r3
 800cab6:	2094      	movs	r0, #148	; 0x94
 800cab8:	9400      	str	r4, [sp, #0]
 800caba:	f7ff fae3 	bl	800c084 <Show_Str6x8>
 800cabe:	8833      	ldrh	r3, [r6, #0]
 800cac0:	aa16      	add	r2, sp, #88	; 0x58
 800cac2:	eb02 0343 	add.w	r3, r2, r3, lsl #1
 800cac6:	f813 1c34 	ldrb.w	r1, [r3, #-52]
 800caca:	4620      	mov	r0, r4
 800cacc:	f001 fbcd 	bl	800e26a <u8ToDec3Str>
 800cad0:	2300      	movs	r3, #0
 800cad2:	4629      	mov	r1, r5
 800cad4:	3d0c      	subs	r5, #12
 800cad6:	9400      	str	r4, [sp, #0]
 800cad8:	461a      	mov	r2, r3
 800cada:	20ca      	movs	r0, #202	; 0xca
 800cadc:	b2ad      	uxth	r5, r5
 800cade:	f7ff fad1 	bl	800c084 <Show_Str6x8>
 800cae2:	2d08      	cmp	r5, #8
 800cae4:	d197      	bne.n	800ca16 <ShowMeasureValue+0x16e>
 800cae6:	b017      	add	sp, #92	; 0x5c
 800cae8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800caec:	3501      	adds	r5, #1
 800caee:	b2ad      	uxth	r5, r5
 800caf0:	f5b5 6f60 	cmp.w	r5, #3584	; 0xe00
 800caf4:	f47f af05 	bne.w	800c902 <ShowMeasureValue+0x5a>
 800caf8:	f640 55ff 	movw	r5, #3583	; 0xdff
 800cafc:	f04f 0a00 	mov.w	sl, #0
 800cb00:	e713      	b.n	800c92a <ShowMeasureValue+0x82>
 800cb02:	1c48      	adds	r0, r1, #1
 800cb04:	b280      	uxth	r0, r0
 800cb06:	f7ff fcd9 	bl	800c4bc <TransformTime>
 800cb0a:	e73b      	b.n	800c984 <ShowMeasureValue+0xdc>
 800cb0c:	f1bc 0f00 	cmp.w	ip, #0
 800cb10:	f47f af56 	bne.w	800c9c0 <ShowMeasureValue+0x118>
 800cb14:	f839 c013 	ldrh.w	ip, [r9, r3, lsl #1]
 800cb18:	f10c 0c01 	add.w	ip, ip, #1
 800cb1c:	f829 c013 	strh.w	ip, [r9, r3, lsl #1]
 800cb20:	e74e      	b.n	800c9c0 <ShowMeasureValue+0x118>
 800cb22:	f04f 0c00 	mov.w	ip, #0
 800cb26:	fa47 f30c 	asr.w	r3, r7, ip
 800cb2a:	07db      	lsls	r3, r3, #31
 800cb2c:	bf4b      	itete	mi
 800cb2e:	f858 302c 	ldrmi.w	r3, [r8, ip, lsl #2]
 800cb32:	f856 302c 	ldrpl.w	r3, [r6, ip, lsl #2]
 800cb36:	181b      	addmi	r3, r3, r0
 800cb38:	181b      	addpl	r3, r3, r0
 800cb3a:	bf4c      	ite	mi
 800cb3c:	f848 302c 	strmi.w	r3, [r8, ip, lsl #2]
 800cb40:	f846 302c 	strpl.w	r3, [r6, ip, lsl #2]
 800cb44:	f10c 0c01 	add.w	ip, ip, #1
 800cb48:	f1bc 0f04 	cmp.w	ip, #4
 800cb4c:	d1eb      	bne.n	800cb26 <ShowMeasureValue+0x27e>
 800cb4e:	3101      	adds	r1, #1
 800cb50:	b289      	uxth	r1, r1
 800cb52:	f5b1 6f60 	cmp.w	r1, #3584	; 0xe00
 800cb56:	46bb      	mov	fp, r7
 800cb58:	f47f af0c 	bne.w	800c974 <ShowMeasureValue+0xcc>
 800cb5c:	e756      	b.n	800ca0c <ShowMeasureValue+0x164>
 800cb5e:	bf00      	nop
 800cb60:	2000000c 	.word	0x2000000c
 800cb64:	08013268 	.word	0x08013268
 800cb68:	20004c18 	.word	0x20004c18
 800cb6c:	20000046 	.word	0x20000046
 800cb70:	08012bc2 	.word	0x08012bc2
 800cb74:	080131a9 	.word	0x080131a9

0800cb78 <ShortcutBMP>:
 800cb78:	b508      	push	{r3, lr}
 800cb7a:	4911      	ldr	r1, [pc, #68]	; (800cbc0 <ShortcutBMP+0x48>)
 800cb7c:	2006      	movs	r0, #6
 800cb7e:	f7f3 fabf 	bl	8000100 <__Bios>
 800cb82:	43c0      	mvns	r0, r0
 800cb84:	f000 000f 	and.w	r0, r0, #15
 800cb88:	2809      	cmp	r0, #9
 800cb8a:	d118      	bne.n	800cbbe <ShortcutBMP+0x46>
 800cb8c:	2032      	movs	r0, #50	; 0x32
 800cb8e:	f7fe ff7d 	bl	800ba8c <Beep_mS>
 800cb92:	21e1      	movs	r1, #225	; 0xe1
 800cb94:	f240 102b 	movw	r0, #299	; 0x12b
 800cb98:	f002 f88a 	bl	800ecb0 <Clear_File_ICO>
 800cb9c:	4b09      	ldr	r3, [pc, #36]	; (800cbc4 <ShortcutBMP+0x4c>)
 800cb9e:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
 800cba2:	f001 fdaf 	bl	800e704 <Save_Bmp>
 800cba6:	f002 f893 	bl	800ecd0 <DispFileInfo>
 800cbaa:	2201      	movs	r2, #1
 800cbac:	4b06      	ldr	r3, [pc, #24]	; (800cbc8 <ShortcutBMP+0x50>)
 800cbae:	701a      	strb	r2, [r3, #0]
 800cbb0:	f7fd f9d4 	bl	8009f5c <ShowBattery>
 800cbb4:	f7ff fdf6 	bl	800c7a4 <ResetPowerOffTime>
 800cbb8:	2200      	movs	r2, #0
 800cbba:	4b04      	ldr	r3, [pc, #16]	; (800cbcc <ShortcutBMP+0x54>)
 800cbbc:	801a      	strh	r2, [r3, #0]
 800cbbe:	bd08      	pop	{r3, pc}
 800cbc0:	fffc0000 	.word	0xfffc0000
 800cbc4:	2000000c 	.word	0x2000000c
 800cbc8:	200000d8 	.word	0x200000d8
 800cbcc:	200009b6 	.word	0x200009b6

0800cbd0 <ShutdownTest>:
 800cbd0:	b510      	push	{r4, lr}
 800cbd2:	2108      	movs	r1, #8
 800cbd4:	2000      	movs	r0, #0
 800cbd6:	f7f3 fa93 	bl	8000100 <__Bios>
 800cbda:	4604      	mov	r4, r0
 800cbdc:	b970      	cbnz	r0, 800cbfc <ShutdownTest+0x2c>
 800cbde:	4908      	ldr	r1, [pc, #32]	; (800cc00 <ShutdownTest+0x30>)
 800cbe0:	2006      	movs	r0, #6
 800cbe2:	f7f3 fa8d 	bl	8000100 <__Bios>
 800cbe6:	43c0      	mvns	r0, r0
 800cbe8:	f000 000f 	and.w	r0, r0, #15
 800cbec:	2806      	cmp	r0, #6
 800cbee:	d105      	bne.n	800cbfc <ShutdownTest+0x2c>
 800cbf0:	4621      	mov	r1, r4
 800cbf2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800cbf6:	2004      	movs	r0, #4
 800cbf8:	f7f3 ba82 	b.w	8000100 <__Bios>
 800cbfc:	bd10      	pop	{r4, pc}
 800cbfe:	bf00      	nop
 800cc00:	fffc0000 	.word	0xfffc0000

0800cc04 <PIO_SendData>:
 800cc04:	b530      	push	{r4, r5, lr}
 800cc06:	4605      	mov	r5, r0
 800cc08:	b087      	sub	sp, #28
 800cc0a:	ac01      	add	r4, sp, #4
 800cc0c:	2214      	movs	r2, #20
 800cc0e:	2100      	movs	r1, #0
 800cc10:	4620      	mov	r0, r4
 800cc12:	f005 ff92 	bl	8012b3a <memset>
 800cc16:	2d02      	cmp	r5, #2
 800cc18:	d00e      	beq.n	800cc38 <PIO_SendData+0x34>
 800cc1a:	2d03      	cmp	r5, #3
 800cc1c:	d01b      	beq.n	800cc56 <PIO_SendData+0x52>
 800cc1e:	2d01      	cmp	r5, #1
 800cc20:	d108      	bne.n	800cc34 <PIO_SendData+0x30>
 800cc22:	4623      	mov	r3, r4
 800cc24:	4a15      	ldr	r2, [pc, #84]	; (800cc7c <PIO_SendData+0x78>)
 800cc26:	6810      	ldr	r0, [r2, #0]
 800cc28:	6851      	ldr	r1, [r2, #4]
 800cc2a:	c303      	stmia	r3!, {r0, r1}
 800cc2c:	4621      	mov	r1, r4
 800cc2e:	2008      	movs	r0, #8
 800cc30:	f7ff fac6 	bl	800c1c0 <ProgmIO_SPIWr>
 800cc34:	b007      	add	sp, #28
 800cc36:	bd30      	pop	{r4, r5, pc}
 800cc38:	2204      	movs	r2, #4
 800cc3a:	2100      	movs	r1, #0
 800cc3c:	2001      	movs	r0, #1
 800cc3e:	f7ff face 	bl	800c1de <ProgmIO_I2CWr>
 800cc42:	2132      	movs	r1, #50	; 0x32
 800cc44:	2007      	movs	r0, #7
 800cc46:	f7f3 fa5b 	bl	8000100 <__Bios>
 800cc4a:	2233      	movs	r2, #51	; 0x33
 800cc4c:	2122      	movs	r1, #34	; 0x22
 800cc4e:	2010      	movs	r0, #16
 800cc50:	f7ff fac5 	bl	800c1de <ProgmIO_I2CWr>
 800cc54:	e7ee      	b.n	800cc34 <PIO_SendData+0x30>
 800cc56:	4623      	mov	r3, r4
 800cc58:	2500      	movs	r5, #0
 800cc5a:	4a09      	ldr	r2, [pc, #36]	; (800cc80 <PIO_SendData+0x7c>)
 800cc5c:	6810      	ldr	r0, [r2, #0]
 800cc5e:	6851      	ldr	r1, [r2, #4]
 800cc60:	7a12      	ldrb	r2, [r2, #8]
 800cc62:	c303      	stmia	r3!, {r0, r1}
 800cc64:	701a      	strb	r2, [r3, #0]
 800cc66:	5d60      	ldrb	r0, [r4, r5]
 800cc68:	f7ff fab5 	bl	800c1d6 <ProgmIO_UARTTx>
 800cc6c:	3501      	adds	r5, #1
 800cc6e:	2164      	movs	r1, #100	; 0x64
 800cc70:	2007      	movs	r0, #7
 800cc72:	f7f3 fa45 	bl	8000100 <__Bios>
 800cc76:	2d09      	cmp	r5, #9
 800cc78:	d1f5      	bne.n	800cc66 <PIO_SendData+0x62>
 800cc7a:	e7db      	b.n	800cc34 <PIO_SendData+0x30>
 800cc7c:	08014634 	.word	0x08014634
 800cc80:	0801463d 	.word	0x0801463d

0800cc84 <Process>:
 800cc84:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800cc88:	2400      	movs	r4, #0
 800cc8a:	4e5a      	ldr	r6, [pc, #360]	; (800cdf4 <Process+0x170>)
 800cc8c:	4b5a      	ldr	r3, [pc, #360]	; (800cdf8 <Process+0x174>)
 800cc8e:	8ab2      	ldrh	r2, [r6, #20]
 800cc90:	b085      	sub	sp, #20
 800cc92:	801a      	strh	r2, [r3, #0]
 800cc94:	4621      	mov	r1, r4
 800cc96:	f44f 5260 	mov.w	r2, #14336	; 0x3800
 800cc9a:	4858      	ldr	r0, [pc, #352]	; (800cdfc <Process+0x178>)
 800cc9c:	f005 ff4d 	bl	8012b3a <memset>
 800cca0:	f7ff fb4b 	bl	800c33a <TriConfig>
 800cca4:	4622      	mov	r2, r4
 800cca6:	2101      	movs	r1, #1
 800cca8:	2002      	movs	r0, #2
 800ccaa:	f7ff fa9f 	bl	800c1ec <FPGA_ReadWrite>
 800ccae:	4f54      	ldr	r7, [pc, #336]	; (800ce00 <Process+0x17c>)
 800ccb0:	2002      	movs	r0, #2
 800ccb2:	f7ff fafb 	bl	800c2ac <Delay_mS>
 800ccb6:	f44f 737a 	mov.w	r3, #1000	; 0x3e8
 800ccba:	46a2      	mov	sl, r4
 800ccbc:	46b8      	mov	r8, r7
 800ccbe:	4d51      	ldr	r5, [pc, #324]	; (800ce04 <Process+0x180>)
 800ccc0:	802b      	strh	r3, [r5, #0]
 800ccc2:	2200      	movs	r2, #0
 800ccc4:	4611      	mov	r1, r2
 800ccc6:	4610      	mov	r0, r2
 800ccc8:	f7ff fa90 	bl	800c1ec <FPGA_ReadWrite>
 800cccc:	e9cd a400 	strd	sl, r4, [sp]
 800ccd0:	f647 33ef 	movw	r3, #31727	; 0x7bef
 800ccd4:	f3c0 2907 	ubfx	r9, r0, #8, #8
 800ccd8:	f64b 52e0 	movw	r2, #48608	; 0xbde0
 800ccdc:	21e1      	movs	r1, #225	; 0xe1
 800ccde:	2004      	movs	r0, #4
 800cce0:	f7ff f956 	bl	800bf90 <Show_Run_ICO>
 800cce4:	883b      	ldrh	r3, [r7, #0]
 800cce6:	4604      	mov	r4, r0
 800cce8:	f413 7b80 	ands.w	fp, r3, #256	; 0x100
 800ccec:	d042      	beq.n	800cd74 <Process+0xf0>
 800ccee:	2201      	movs	r2, #1
 800ccf0:	2400      	movs	r4, #0
 800ccf2:	4b45      	ldr	r3, [pc, #276]	; (800ce08 <Process+0x184>)
 800ccf4:	701a      	strb	r2, [r3, #0]
 800ccf6:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
 800ccfa:	f7ff fad7 	bl	800c2ac <Delay_mS>
 800ccfe:	4f43      	ldr	r7, [pc, #268]	; (800ce0c <Process+0x188>)
 800cd00:	2c00      	cmp	r4, #0
 800cd02:	d067      	beq.n	800cdd4 <Process+0x150>
 800cd04:	f887 4021 	strb.w	r4, [r7, #33]	; 0x21
 800cd08:	2400      	movs	r4, #0
 800cd0a:	f8df 90f0 	ldr.w	r9, [pc, #240]	; 800cdfc <Process+0x178>
 800cd0e:	2200      	movs	r2, #0
 800cd10:	2103      	movs	r1, #3
 800cd12:	2032      	movs	r0, #50	; 0x32
 800cd14:	f7ff fa90 	bl	800c238 <FPGA_RW_4Byte>
 800cd18:	0a02      	lsrs	r2, r0, #8
 800cd1a:	b2c3      	uxtb	r3, r0
 800cd1c:	f88d 200d 	strb.w	r2, [sp, #13]
 800cd20:	0c02      	lsrs	r2, r0, #16
 800cd22:	f88d 200e 	strb.w	r2, [sp, #14]
 800cd26:	f003 0206 	and.w	r2, r3, #6
 800cd2a:	0e00      	lsrs	r0, r0, #24
 800cd2c:	2a06      	cmp	r2, #6
 800cd2e:	f88d 300c 	strb.w	r3, [sp, #12]
 800cd32:	f88d 000f 	strb.w	r0, [sp, #15]
 800cd36:	d12d      	bne.n	800cd94 <Process+0x110>
 800cd38:	4a30      	ldr	r2, [pc, #192]	; (800cdfc <Process+0x178>)
 800cd3a:	f802 3024 	strb.w	r3, [r2, r4, lsl #2]
 800cd3e:	f04f 0c01 	mov.w	ip, #1
 800cd42:	f04f 0e00 	mov.w	lr, #0
 800cd46:	00a5      	lsls	r5, r4, #2
 800cd48:	4a2c      	ldr	r2, [pc, #176]	; (800cdfc <Process+0x178>)
 800cd4a:	1c6b      	adds	r3, r5, #1
 800cd4c:	1e68      	subs	r0, r5, #1
 800cd4e:	4413      	add	r3, r2
 800cd50:	f5b4 6f60 	cmp.w	r4, #3584	; 0xe00
 800cd54:	d23b      	bcs.n	800cdce <Process+0x14a>
 800cd56:	5d51      	ldrb	r1, [r2, r5]
 800cd58:	f883 c000 	strb.w	ip, [r3]
 800cd5c:	f803 1c01 	strb.w	r1, [r3, #-1]
 800cd60:	f883 e001 	strb.w	lr, [r3, #1]
 800cd64:	5c11      	ldrb	r1, [r2, r0]
 800cd66:	3401      	adds	r4, #1
 800cd68:	f001 01f0 	and.w	r1, r1, #240	; 0xf0
 800cd6c:	7099      	strb	r1, [r3, #2]
 800cd6e:	b2a4      	uxth	r4, r4
 800cd70:	3304      	adds	r3, #4
 800cd72:	e7ed      	b.n	800cd50 <Process+0xcc>
 800cd74:	051b      	lsls	r3, r3, #20
 800cd76:	d508      	bpl.n	800cd8a <Process+0x106>
 800cd78:	2032      	movs	r0, #50	; 0x32
 800cd7a:	f7fe fe87 	bl	800ba8c <Beep_mS>
 800cd7e:	f896 0050 	ldrb.w	r0, [r6, #80]	; 0x50
 800cd82:	f8a7 b000 	strh.w	fp, [r7]
 800cd86:	f7ff ff3d 	bl	800cc04 <PIO_SendData>
 800cd8a:	f019 0f10 	tst.w	r9, #16
 800cd8e:	d098      	beq.n	800ccc2 <Process+0x3e>
 800cd90:	2401      	movs	r4, #1
 800cd92:	e7b0      	b.n	800ccf6 <Process+0x72>
 800cd94:	0798      	lsls	r0, r3, #30
 800cd96:	d508      	bpl.n	800cdaa <Process+0x126>
 800cd98:	9b03      	ldr	r3, [sp, #12]
 800cd9a:	f849 3024 	str.w	r3, [r9, r4, lsl #2]
 800cd9e:	f44f 73fa 	mov.w	r3, #500	; 0x1f4
 800cda2:	3401      	adds	r4, #1
 800cda4:	b2a4      	uxth	r4, r4
 800cda6:	802b      	strh	r3, [r5, #0]
 800cda8:	e00e      	b.n	800cdc8 <Process+0x144>
 800cdaa:	0759      	lsls	r1, r3, #29
 800cdac:	d51f      	bpl.n	800cdee <Process+0x16a>
 800cdae:	f809 3024 	strb.w	r3, [r9, r4, lsl #2]
 800cdb2:	f8b8 3000 	ldrh.w	r3, [r8]
 800cdb6:	05da      	lsls	r2, r3, #23
 800cdb8:	d503      	bpl.n	800cdc2 <Process+0x13e>
 800cdba:	2201      	movs	r2, #1
 800cdbc:	4b12      	ldr	r3, [pc, #72]	; (800ce08 <Process+0x184>)
 800cdbe:	701a      	strb	r2, [r3, #0]
 800cdc0:	e7bd      	b.n	800cd3e <Process+0xba>
 800cdc2:	882b      	ldrh	r3, [r5, #0]
 800cdc4:	2b00      	cmp	r3, #0
 800cdc6:	d0f8      	beq.n	800cdba <Process+0x136>
 800cdc8:	f5b4 6f60 	cmp.w	r4, #3584	; 0xe00
 800cdcc:	d19f      	bne.n	800cd0e <Process+0x8a>
 800cdce:	7830      	ldrb	r0, [r6, #0]
 800cdd0:	f000 ff5a 	bl	800dc88 <AnalyzeFrame>
 800cdd4:	f44f 6060 	mov.w	r0, #3584	; 0xe00
 800cdd8:	f7ff fb80 	bl	800c4dc <CalculateTimeSum>
 800cddc:	e9c7 0106 	strd	r0, r1, [r7, #24]
 800cde0:	f7fd fdf8 	bl	800a9d4 <ProcessSmallWins>
 800cde4:	b005      	add	sp, #20
 800cde6:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800cdea:	f7ff bbeb 	b.w	800c5c4 <ZC_Scale>
 800cdee:	07db      	lsls	r3, r3, #31
 800cdf0:	d4ea      	bmi.n	800cdc8 <Process+0x144>
 800cdf2:	e7d1      	b.n	800cd98 <Process+0x114>
 800cdf4:	2000000c 	.word	0x2000000c
 800cdf8:	200009ce 	.word	0x200009ce
 800cdfc:	20000c0c 	.word	0x20000c0c
 800ce00:	200009b6 	.word	0x200009b6
 800ce04:	200009d0 	.word	0x200009d0
 800ce08:	200009d9 	.word	0x200009d9
 800ce0c:	20004c18 	.word	0x20004c18

0800ce10 <PIO_PWM_SetFreq>:
 800ce10:	b510      	push	{r4, lr}
 800ce12:	460b      	mov	r3, r1
 800ce14:	4614      	mov	r4, r2
 800ce16:	b958      	cbnz	r0, 800ce30 <PIO_PWM_SetFreq+0x20>
 800ce18:	f5a1 2180 	sub.w	r1, r1, #262144	; 0x40000
 800ce1c:	2018      	movs	r0, #24
 800ce1e:	f7f3 f96f 	bl	8000100 <__Bios>
 800ce22:	f5a4 21a0 	sub.w	r1, r4, #327680	; 0x50000
 800ce26:	2018      	movs	r0, #24
 800ce28:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800ce2c:	f7f3 b968 	b.w	8000100 <__Bios>
 800ce30:	2801      	cmp	r0, #1
 800ce32:	d105      	bne.n	800ce40 <PIO_PWM_SetFreq+0x30>
 800ce34:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800ce38:	4611      	mov	r1, r2
 800ce3a:	4618      	mov	r0, r3
 800ce3c:	f7ff ba6a 	b.w	800c314 <PIO_PWM_SetFreq.part.1>
 800ce40:	bd10      	pop	{r4, pc}

0800ce42 <PIO_PWM_SetDuty>:
 800ce42:	b950      	cbnz	r0, 800ce5a <PIO_PWM_SetDuty+0x18>
 800ce44:	b921      	cbnz	r1, 800ce50 <PIO_PWM_SetDuty+0xe>
 800ce46:	f5a2 21c0 	sub.w	r1, r2, #393216	; 0x60000
 800ce4a:	2018      	movs	r0, #24
 800ce4c:	f7f3 b958 	b.w	8000100 <__Bios>
 800ce50:	2901      	cmp	r1, #1
 800ce52:	d10d      	bne.n	800ce70 <PIO_PWM_SetDuty+0x2e>
 800ce54:	f5a2 21e0 	sub.w	r1, r2, #458752	; 0x70000
 800ce58:	e7f7      	b.n	800ce4a <PIO_PWM_SetDuty+0x8>
 800ce5a:	2801      	cmp	r0, #1
 800ce5c:	d108      	bne.n	800ce70 <PIO_PWM_SetDuty+0x2e>
 800ce5e:	b911      	cbnz	r1, 800ce66 <PIO_PWM_SetDuty+0x24>
 800ce60:	f5a2 2140 	sub.w	r1, r2, #786432	; 0xc0000
 800ce64:	e7f1      	b.n	800ce4a <PIO_PWM_SetDuty+0x8>
 800ce66:	2901      	cmp	r1, #1
 800ce68:	d102      	bne.n	800ce70 <PIO_PWM_SetDuty+0x2e>
 800ce6a:	4610      	mov	r0, r2
 800ce6c:	f7ff ba60 	b.w	800c330 <PIO_PWM_SetDuty.part.2>
 800ce70:	4770      	bx	lr

0800ce72 <PIO_PWM_Ctrl>:
 800ce72:	b938      	cbnz	r0, 800ce84 <PIO_PWM_Ctrl+0x12>
 800ce74:	2901      	cmp	r1, #1
 800ce76:	bf14      	ite	ne
 800ce78:	4907      	ldrne	r1, [pc, #28]	; (800ce98 <PIO_PWM_Ctrl+0x26>)
 800ce7a:	f5a1 2100 	subeq.w	r1, r1, #524288	; 0x80000
 800ce7e:	2018      	movs	r0, #24
 800ce80:	f7f3 b93e 	b.w	8000100 <__Bios>
 800ce84:	2801      	cmp	r0, #1
 800ce86:	d105      	bne.n	800ce94 <PIO_PWM_Ctrl+0x22>
 800ce88:	2901      	cmp	r1, #1
 800ce8a:	bf0c      	ite	eq
 800ce8c:	f5a1 2160 	subeq.w	r1, r1, #917504	; 0xe0000
 800ce90:	4902      	ldrne	r1, [pc, #8]	; (800ce9c <PIO_PWM_Ctrl+0x2a>)
 800ce92:	e7f4      	b.n	800ce7e <PIO_PWM_Ctrl+0xc>
 800ce94:	4770      	bx	lr
 800ce96:	bf00      	nop
 800ce98:	fff80000 	.word	0xfff80000
 800ce9c:	fff20000 	.word	0xfff20000

0800cea0 <HW_Ctrl>:
 800cea0:	3802      	subs	r0, #2
 800cea2:	b570      	push	{r4, r5, r6, lr}
 800cea4:	2804      	cmp	r0, #4
 800cea6:	f200 80d6 	bhi.w	800d056 <HW_Ctrl+0x1b6>
 800ceaa:	e8df f010 	tbh	[pc, r0, lsl #1]
 800ceae:	0005      	.short	0x0005
 800ceb0:	00390035 	.word	0x00390035
 800ceb4:	00cf00d4 	.word	0x00cf00d4
 800ceb8:	2904      	cmp	r1, #4
 800ceba:	f200 80cc 	bhi.w	800d056 <HW_Ctrl+0x1b6>
 800cebe:	e8df f001 	tbb	[pc, r1]
 800cec2:	0b03      	.short	0x0b03
 800cec4:	1711      	.short	0x1711
 800cec6:	22          	.byte	0x22
 800cec7:	00          	.byte	0x00
 800cec8:	2101      	movs	r1, #1
 800ceca:	2020      	movs	r0, #32
 800cecc:	4b6a      	ldr	r3, [pc, #424]	; (800d078 <HW_Ctrl+0x1d8>)
 800cece:	8d1a      	ldrh	r2, [r3, #40]	; 0x28
 800ced0:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800ced4:	f7ff b98a 	b.w	800c1ec <FPGA_ReadWrite>
 800ced8:	4b67      	ldr	r3, [pc, #412]	; (800d078 <HW_Ctrl+0x1d8>)
 800ceda:	2101      	movs	r1, #1
 800cedc:	8d5a      	ldrh	r2, [r3, #42]	; 0x2a
 800cede:	2021      	movs	r0, #33	; 0x21
 800cee0:	0a12      	lsrs	r2, r2, #8
 800cee2:	e7f5      	b.n	800ced0 <HW_Ctrl+0x30>
 800cee4:	4b64      	ldr	r3, [pc, #400]	; (800d078 <HW_Ctrl+0x1d8>)
 800cee6:	2101      	movs	r1, #1
 800cee8:	8d9a      	ldrh	r2, [r3, #44]	; 0x2c
 800ceea:	2022      	movs	r0, #34	; 0x22
 800ceec:	0a12      	lsrs	r2, r2, #8
 800ceee:	e7ef      	b.n	800ced0 <HW_Ctrl+0x30>
 800cef0:	4b61      	ldr	r3, [pc, #388]	; (800d078 <HW_Ctrl+0x1d8>)
 800cef2:	4a62      	ldr	r2, [pc, #392]	; (800d07c <HW_Ctrl+0x1dc>)
 800cef4:	8a99      	ldrh	r1, [r3, #20]
 800cef6:	8ddb      	ldrh	r3, [r3, #46]	; 0x2e
 800cef8:	f852 2021 	ldr.w	r2, [r2, r1, lsl #2]
 800cefc:	2023      	movs	r0, #35	; 0x23
 800cefe:	435a      	muls	r2, r3
 800cf00:	2102      	movs	r1, #2
 800cf02:	b292      	uxth	r2, r2
 800cf04:	e7e4      	b.n	800ced0 <HW_Ctrl+0x30>
 800cf06:	2164      	movs	r1, #100	; 0x64
 800cf08:	2011      	movs	r0, #17
 800cf0a:	4b5b      	ldr	r3, [pc, #364]	; (800d078 <HW_Ctrl+0x1d8>)
 800cf0c:	8e1b      	ldrh	r3, [r3, #48]	; 0x30
 800cf0e:	4359      	muls	r1, r3
 800cf10:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800cf14:	f7f3 b8f4 	b.w	8000100 <__Bios>
 800cf18:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800cf1c:	f7fd bbe4 	b.w	800a6e8 <ShowMeasureSourceStr>
 800cf20:	2900      	cmp	r1, #0
 800cf22:	f040 8098 	bne.w	800d056 <HW_Ctrl+0x1b6>
 800cf26:	4b54      	ldr	r3, [pc, #336]	; (800d078 <HW_Ctrl+0x1d8>)
 800cf28:	f8b3 3050 	ldrh.w	r3, [r3, #80]	; 0x50
 800cf2c:	2b00      	cmp	r3, #0
 800cf2e:	f040 8092 	bne.w	800d056 <HW_Ctrl+0x1b6>
 800cf32:	4b53      	ldr	r3, [pc, #332]	; (800d080 <HW_Ctrl+0x1e0>)
 800cf34:	781b      	ldrb	r3, [r3, #0]
 800cf36:	2b07      	cmp	r3, #7
 800cf38:	f200 808d 	bhi.w	800d056 <HW_Ctrl+0x1b6>
 800cf3c:	e8df f003 	tbb	[pc, r3]
 800cf40:	3b2b0b04 	.word	0x3b2b0b04
 800cf44:	7a6b4b47 	.word	0x7a6b4b47
 800cf48:	2000      	movs	r0, #0
 800cf4a:	4b4e      	ldr	r3, [pc, #312]	; (800d084 <HW_Ctrl+0x1e4>)
 800cf4c:	7819      	ldrb	r1, [r3, #0]
 800cf4e:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800cf52:	f7ff bf8e 	b.w	800ce72 <PIO_PWM_Ctrl>
 800cf56:	260a      	movs	r6, #10
 800cf58:	4d4a      	ldr	r5, [pc, #296]	; (800d084 <HW_Ctrl+0x1e4>)
 800cf5a:	4b4b      	ldr	r3, [pc, #300]	; (800d088 <HW_Ctrl+0x1e8>)
 800cf5c:	786a      	ldrb	r2, [r5, #1]
 800cf5e:	2000      	movs	r0, #0
 800cf60:	f833 4012 	ldrh.w	r4, [r3, r2, lsl #1]
 800cf64:	eb03 0342 	add.w	r3, r3, r2, lsl #1
 800cf68:	8c19      	ldrh	r1, [r3, #32]
 800cf6a:	4622      	mov	r2, r4
 800cf6c:	f7ff ff50 	bl	800ce10 <PIO_PWM_SetFreq>
 800cf70:	78a9      	ldrb	r1, [r5, #2]
 800cf72:	3401      	adds	r4, #1
 800cf74:	4361      	muls	r1, r4
 800cf76:	fb91 f1f6 	sdiv	r1, r1, r6
 800cf7a:	b289      	uxth	r1, r1
 800cf7c:	f5a1 21c0 	sub.w	r1, r1, #393216	; 0x60000
 800cf80:	2018      	movs	r0, #24
 800cf82:	f7f3 f8bd 	bl	8000100 <__Bios>
 800cf86:	78e9      	ldrb	r1, [r5, #3]
 800cf88:	4361      	muls	r1, r4
 800cf8a:	fb91 f1f6 	sdiv	r1, r1, r6
 800cf8e:	b289      	uxth	r1, r1
 800cf90:	f5a1 21e0 	sub.w	r1, r1, #458752	; 0x70000
 800cf94:	e00d      	b.n	800cfb2 <HW_Ctrl+0x112>
 800cf96:	4b3b      	ldr	r3, [pc, #236]	; (800d084 <HW_Ctrl+0x1e4>)
 800cf98:	4a3b      	ldr	r2, [pc, #236]	; (800d088 <HW_Ctrl+0x1e8>)
 800cf9a:	7859      	ldrb	r1, [r3, #1]
 800cf9c:	f832 2011 	ldrh.w	r2, [r2, r1, lsl #1]
 800cfa0:	7899      	ldrb	r1, [r3, #2]
 800cfa2:	230a      	movs	r3, #10
 800cfa4:	fb02 1101 	mla	r1, r2, r1, r1
 800cfa8:	fb91 f1f3 	sdiv	r1, r1, r3
 800cfac:	b289      	uxth	r1, r1
 800cfae:	f5a1 21c0 	sub.w	r1, r1, #393216	; 0x60000
 800cfb2:	2018      	movs	r0, #24
 800cfb4:	e7ac      	b.n	800cf10 <HW_Ctrl+0x70>
 800cfb6:	4b33      	ldr	r3, [pc, #204]	; (800d084 <HW_Ctrl+0x1e4>)
 800cfb8:	4a33      	ldr	r2, [pc, #204]	; (800d088 <HW_Ctrl+0x1e8>)
 800cfba:	7859      	ldrb	r1, [r3, #1]
 800cfbc:	f832 2011 	ldrh.w	r2, [r2, r1, lsl #1]
 800cfc0:	78d9      	ldrb	r1, [r3, #3]
 800cfc2:	230a      	movs	r3, #10
 800cfc4:	fb02 1101 	mla	r1, r2, r1, r1
 800cfc8:	fb91 f1f3 	sdiv	r1, r1, r3
 800cfcc:	e7df      	b.n	800cf8e <HW_Ctrl+0xee>
 800cfce:	4b2d      	ldr	r3, [pc, #180]	; (800d084 <HW_Ctrl+0x1e4>)
 800cfd0:	2001      	movs	r0, #1
 800cfd2:	7919      	ldrb	r1, [r3, #4]
 800cfd4:	e7bb      	b.n	800cf4e <HW_Ctrl+0xae>
 800cfd6:	260a      	movs	r6, #10
 800cfd8:	4d2a      	ldr	r5, [pc, #168]	; (800d084 <HW_Ctrl+0x1e4>)
 800cfda:	4b2b      	ldr	r3, [pc, #172]	; (800d088 <HW_Ctrl+0x1e8>)
 800cfdc:	796a      	ldrb	r2, [r5, #5]
 800cfde:	f833 4012 	ldrh.w	r4, [r3, r2, lsl #1]
 800cfe2:	eb03 0342 	add.w	r3, r3, r2, lsl #1
 800cfe6:	4621      	mov	r1, r4
 800cfe8:	8c18      	ldrh	r0, [r3, #32]
 800cfea:	f7ff f993 	bl	800c314 <PIO_PWM_SetFreq.part.1>
 800cfee:	79a9      	ldrb	r1, [r5, #6]
 800cff0:	3401      	adds	r4, #1
 800cff2:	4361      	muls	r1, r4
 800cff4:	fb91 f1f6 	sdiv	r1, r1, r6
 800cff8:	b289      	uxth	r1, r1
 800cffa:	f5a1 2140 	sub.w	r1, r1, #786432	; 0xc0000
 800cffe:	2018      	movs	r0, #24
 800d000:	f7f3 f87e 	bl	8000100 <__Bios>
 800d004:	79e8      	ldrb	r0, [r5, #7]
 800d006:	4360      	muls	r0, r4
 800d008:	fb90 f0f6 	sdiv	r0, r0, r6
 800d00c:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800d010:	b280      	uxth	r0, r0
 800d012:	f7ff b98d 	b.w	800c330 <PIO_PWM_SetDuty.part.2>
 800d016:	4b1b      	ldr	r3, [pc, #108]	; (800d084 <HW_Ctrl+0x1e4>)
 800d018:	4a1b      	ldr	r2, [pc, #108]	; (800d088 <HW_Ctrl+0x1e8>)
 800d01a:	7959      	ldrb	r1, [r3, #5]
 800d01c:	f832 2011 	ldrh.w	r2, [r2, r1, lsl #1]
 800d020:	7999      	ldrb	r1, [r3, #6]
 800d022:	230a      	movs	r3, #10
 800d024:	fb02 1101 	mla	r1, r2, r1, r1
 800d028:	fb91 f1f3 	sdiv	r1, r1, r3
 800d02c:	b289      	uxth	r1, r1
 800d02e:	f5a1 2140 	sub.w	r1, r1, #786432	; 0xc0000
 800d032:	e7be      	b.n	800cfb2 <HW_Ctrl+0x112>
 800d034:	4b13      	ldr	r3, [pc, #76]	; (800d084 <HW_Ctrl+0x1e4>)
 800d036:	4a14      	ldr	r2, [pc, #80]	; (800d088 <HW_Ctrl+0x1e8>)
 800d038:	7959      	ldrb	r1, [r3, #5]
 800d03a:	79d8      	ldrb	r0, [r3, #7]
 800d03c:	230a      	movs	r3, #10
 800d03e:	f832 2011 	ldrh.w	r2, [r2, r1, lsl #1]
 800d042:	fb02 0000 	mla	r0, r2, r0, r0
 800d046:	fb90 f0f3 	sdiv	r0, r0, r3
 800d04a:	e7df      	b.n	800d00c <HW_Ctrl+0x16c>
 800d04c:	313c      	adds	r1, #60	; 0x3c
 800d04e:	293e      	cmp	r1, #62	; 0x3e
 800d050:	d002      	beq.n	800d058 <HW_Ctrl+0x1b8>
 800d052:	293f      	cmp	r1, #63	; 0x3f
 800d054:	d007      	beq.n	800d066 <HW_Ctrl+0x1c6>
 800d056:	bd70      	pop	{r4, r5, r6, pc}
 800d058:	210a      	movs	r1, #10
 800d05a:	4b07      	ldr	r3, [pc, #28]	; (800d078 <HW_Ctrl+0x1d8>)
 800d05c:	2005      	movs	r0, #5
 800d05e:	f8b3 307c 	ldrh.w	r3, [r3, #124]	; 0x7c
 800d062:	4359      	muls	r1, r3
 800d064:	e754      	b.n	800cf10 <HW_Ctrl+0x70>
 800d066:	210a      	movs	r1, #10
 800d068:	4b03      	ldr	r3, [pc, #12]	; (800d078 <HW_Ctrl+0x1d8>)
 800d06a:	2008      	movs	r0, #8
 800d06c:	f8b3 307e 	ldrh.w	r3, [r3, #126]	; 0x7e
 800d070:	4359      	muls	r1, r3
 800d072:	3901      	subs	r1, #1
 800d074:	e74c      	b.n	800cf10 <HW_Ctrl+0x70>
 800d076:	bf00      	nop
 800d078:	2000000c 	.word	0x2000000c
 800d07c:	08013268 	.word	0x08013268
 800d080:	2000024b 	.word	0x2000024b
 800d084:	200000b8 	.word	0x200000b8
 800d088:	080145f4 	.word	0x080145f4

0800d08c <PIO_AllDisable>:
 800d08c:	4901      	ldr	r1, [pc, #4]	; (800d094 <PIO_AllDisable+0x8>)
 800d08e:	2016      	movs	r0, #22
 800d090:	f7f3 b836 	b.w	8000100 <__Bios>
 800d094:	ffff0000 	.word	0xffff0000

0800d098 <PIO_Init>:
 800d098:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800d09c:	2803      	cmp	r0, #3
 800d09e:	d85b      	bhi.n	800d158 <PIO_Init+0xc0>
 800d0a0:	e8df f000 	tbb	[pc, r0]
 800d0a4:	57544e02 	.word	0x57544e02
 800d0a8:	260a      	movs	r6, #10
 800d0aa:	4f2c      	ldr	r7, [pc, #176]	; (800d15c <PIO_Init+0xc4>)
 800d0ac:	492c      	ldr	r1, [pc, #176]	; (800d160 <PIO_Init+0xc8>)
 800d0ae:	2018      	movs	r0, #24
 800d0b0:	f7f3 f826 	bl	8000100 <__Bios>
 800d0b4:	f8df 80ac 	ldr.w	r8, [pc, #172]	; 800d164 <PIO_Init+0xcc>
 800d0b8:	787c      	ldrb	r4, [r7, #1]
 800d0ba:	7839      	ldrb	r1, [r7, #0]
 800d0bc:	2000      	movs	r0, #0
 800d0be:	f7ff fed8 	bl	800ce72 <PIO_PWM_Ctrl>
 800d0c2:	f838 5014 	ldrh.w	r5, [r8, r4, lsl #1]
 800d0c6:	eb08 0444 	add.w	r4, r8, r4, lsl #1
 800d0ca:	462a      	mov	r2, r5
 800d0cc:	8c21      	ldrh	r1, [r4, #32]
 800d0ce:	2000      	movs	r0, #0
 800d0d0:	f7ff fe9e 	bl	800ce10 <PIO_PWM_SetFreq>
 800d0d4:	78b9      	ldrb	r1, [r7, #2]
 800d0d6:	2018      	movs	r0, #24
 800d0d8:	4369      	muls	r1, r5
 800d0da:	fb91 f1f6 	sdiv	r1, r1, r6
 800d0de:	b289      	uxth	r1, r1
 800d0e0:	f5a1 21c0 	sub.w	r1, r1, #393216	; 0x60000
 800d0e4:	f7f3 f80c 	bl	8000100 <__Bios>
 800d0e8:	78f9      	ldrb	r1, [r7, #3]
 800d0ea:	2018      	movs	r0, #24
 800d0ec:	4369      	muls	r1, r5
 800d0ee:	fb91 f1f6 	sdiv	r1, r1, r6
 800d0f2:	b289      	uxth	r1, r1
 800d0f4:	f5a1 21e0 	sub.w	r1, r1, #458752	; 0x70000
 800d0f8:	f7f3 f802 	bl	8000100 <__Bios>
 800d0fc:	797d      	ldrb	r5, [r7, #5]
 800d0fe:	7939      	ldrb	r1, [r7, #4]
 800d100:	2001      	movs	r0, #1
 800d102:	f7ff feb6 	bl	800ce72 <PIO_PWM_Ctrl>
 800d106:	f838 4015 	ldrh.w	r4, [r8, r5, lsl #1]
 800d10a:	eb08 0845 	add.w	r8, r8, r5, lsl #1
 800d10e:	4621      	mov	r1, r4
 800d110:	f8b8 0020 	ldrh.w	r0, [r8, #32]
 800d114:	f7ff f8fe 	bl	800c314 <PIO_PWM_SetFreq.part.1>
 800d118:	79b9      	ldrb	r1, [r7, #6]
 800d11a:	3401      	adds	r4, #1
 800d11c:	4361      	muls	r1, r4
 800d11e:	fb91 f1f6 	sdiv	r1, r1, r6
 800d122:	b289      	uxth	r1, r1
 800d124:	f5a1 2140 	sub.w	r1, r1, #786432	; 0xc0000
 800d128:	2018      	movs	r0, #24
 800d12a:	f7f2 ffe9 	bl	8000100 <__Bios>
 800d12e:	79f8      	ldrb	r0, [r7, #7]
 800d130:	4360      	muls	r0, r4
 800d132:	fb90 f0f6 	sdiv	r0, r0, r6
 800d136:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
 800d13a:	b280      	uxth	r0, r0
 800d13c:	f7ff b8f8 	b.w	800c330 <PIO_PWM_SetDuty.part.2>
 800d140:	201a      	movs	r0, #26
 800d142:	4907      	ldr	r1, [pc, #28]	; (800d160 <PIO_Init+0xc8>)
 800d144:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
 800d148:	f7f2 bfda 	b.w	8000100 <__Bios>
 800d14c:	4904      	ldr	r1, [pc, #16]	; (800d160 <PIO_Init+0xc8>)
 800d14e:	201c      	movs	r0, #28
 800d150:	e7f8      	b.n	800d144 <PIO_Init+0xac>
 800d152:	4903      	ldr	r1, [pc, #12]	; (800d160 <PIO_Init+0xc8>)
 800d154:	201b      	movs	r0, #27
 800d156:	e7f5      	b.n	800d144 <PIO_Init+0xac>
 800d158:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800d15c:	200000b8 	.word	0x200000b8
 800d160:	ffff0000 	.word	0xffff0000
 800d164:	080145f4 	.word	0x080145f4

0800d168 <KeyQuickAct>:
 800d168:	b570      	push	{r4, r5, r6, lr}
 800d16a:	4b4e      	ldr	r3, [pc, #312]	; (800d2a4 <KeyQuickAct+0x13c>)
 800d16c:	781b      	ldrb	r3, [r3, #0]
 800d16e:	b323      	cbz	r3, 800d1ba <KeyQuickAct+0x52>
 800d170:	494d      	ldr	r1, [pc, #308]	; (800d2a8 <KeyQuickAct+0x140>)
 800d172:	2006      	movs	r0, #6
 800d174:	f7f2 ffc4 	bl	8000100 <__Bios>
 800d178:	43c0      	mvns	r0, r0
 800d17a:	f000 000f 	and.w	r0, r0, #15
 800d17e:	2808      	cmp	r0, #8
 800d180:	d148      	bne.n	800d214 <KeyQuickAct+0xac>
 800d182:	4b4a      	ldr	r3, [pc, #296]	; (800d2ac <KeyQuickAct+0x144>)
 800d184:	881b      	ldrh	r3, [r3, #0]
 800d186:	045b      	lsls	r3, r3, #17
 800d188:	d544      	bpl.n	800d214 <KeyQuickAct+0xac>
 800d18a:	4c47      	ldr	r4, [pc, #284]	; (800d2a8 <KeyQuickAct+0x140>)
 800d18c:	4d48      	ldr	r5, [pc, #288]	; (800d2b0 <KeyQuickAct+0x148>)
 800d18e:	4e49      	ldr	r6, [pc, #292]	; (800d2b4 <KeyQuickAct+0x14c>)
 800d190:	4621      	mov	r1, r4
 800d192:	2006      	movs	r0, #6
 800d194:	f7f2 ffb4 	bl	8000100 <__Bios>
 800d198:	43c0      	mvns	r0, r0
 800d19a:	f000 000f 	and.w	r0, r0, #15
 800d19e:	2808      	cmp	r0, #8
 800d1a0:	d00c      	beq.n	800d1bc <KeyQuickAct+0x54>
 800d1a2:	2064      	movs	r0, #100	; 0x64
 800d1a4:	f7ff f882 	bl	800c2ac <Delay_mS>
 800d1a8:	f7fd f9fe 	bl	800a5a8 <ShowTimeMarkValue>
 800d1ac:	f7ff fb7c 	bl	800c8a8 <ShowMeasureValue>
 800d1b0:	f7ff faf8 	bl	800c7a4 <ResetPowerOffTime>
 800d1b4:	2200      	movs	r2, #0
 800d1b6:	4b3d      	ldr	r3, [pc, #244]	; (800d2ac <KeyQuickAct+0x144>)
 800d1b8:	801a      	strh	r2, [r3, #0]
 800d1ba:	bd70      	pop	{r4, r5, r6, pc}
 800d1bc:	786b      	ldrb	r3, [r5, #1]
 800d1be:	2b01      	cmp	r3, #1
 800d1c0:	d112      	bne.n	800d1e8 <KeyQuickAct+0x80>
 800d1c2:	4b3d      	ldr	r3, [pc, #244]	; (800d2b8 <KeyQuickAct+0x150>)
 800d1c4:	483d      	ldr	r0, [pc, #244]	; (800d2bc <KeyQuickAct+0x154>)
 800d1c6:	8ada      	ldrh	r2, [r3, #22]
 800d1c8:	8ac0      	ldrh	r0, [r0, #22]
 800d1ca:	8b19      	ldrh	r1, [r3, #24]
 800d1cc:	4402      	add	r2, r0
 800d1ce:	428a      	cmp	r2, r1
 800d1d0:	bfb4      	ite	lt
 800d1d2:	82da      	strhlt	r2, [r3, #22]
 800d1d4:	82d9      	strhge	r1, [r3, #22]
 800d1d6:	f240 1219 	movw	r2, #281	; 0x119
 800d1da:	4939      	ldr	r1, [pc, #228]	; (800d2c0 <KeyQuickAct+0x158>)
 800d1dc:	4839      	ldr	r0, [pc, #228]	; (800d2c4 <KeyQuickAct+0x15c>)
 800d1de:	f005 fca1 	bl	8012b24 <memcpy>
 800d1e2:	f7ff f8f7 	bl	800c3d4 <ShowWaveToLCD>
 800d1e6:	e7d3      	b.n	800d190 <KeyQuickAct+0x28>
 800d1e8:	2b02      	cmp	r3, #2
 800d1ea:	d10a      	bne.n	800d202 <KeyQuickAct+0x9a>
 800d1ec:	4a32      	ldr	r2, [pc, #200]	; (800d2b8 <KeyQuickAct+0x150>)
 800d1ee:	4936      	ldr	r1, [pc, #216]	; (800d2c8 <KeyQuickAct+0x160>)
 800d1f0:	8b13      	ldrh	r3, [r2, #24]
 800d1f2:	8b09      	ldrh	r1, [r1, #24]
 800d1f4:	4299      	cmp	r1, r3
 800d1f6:	d9ee      	bls.n	800d1d6 <KeyQuickAct+0x6e>
 800d1f8:	4930      	ldr	r1, [pc, #192]	; (800d2bc <KeyQuickAct+0x154>)
 800d1fa:	8b09      	ldrh	r1, [r1, #24]
 800d1fc:	440b      	add	r3, r1
 800d1fe:	8313      	strh	r3, [r2, #24]
 800d200:	e7e9      	b.n	800d1d6 <KeyQuickAct+0x6e>
 800d202:	2b03      	cmp	r3, #3
 800d204:	d1e7      	bne.n	800d1d6 <KeyQuickAct+0x6e>
 800d206:	2301      	movs	r3, #1
 800d208:	7233      	strb	r3, [r6, #8]
 800d20a:	f7ff f9db 	bl	800c5c4 <ZC_Scale>
 800d20e:	f7fd f889 	bl	800a324 <ShowWindowPrecent>
 800d212:	e7e0      	b.n	800d1d6 <KeyQuickAct+0x6e>
 800d214:	4924      	ldr	r1, [pc, #144]	; (800d2a8 <KeyQuickAct+0x140>)
 800d216:	2006      	movs	r0, #6
 800d218:	f7f2 ff72 	bl	8000100 <__Bios>
 800d21c:	43c0      	mvns	r0, r0
 800d21e:	f000 000f 	and.w	r0, r0, #15
 800d222:	2808      	cmp	r0, #8
 800d224:	d1c9      	bne.n	800d1ba <KeyQuickAct+0x52>
 800d226:	4b21      	ldr	r3, [pc, #132]	; (800d2ac <KeyQuickAct+0x144>)
 800d228:	f9b3 3000 	ldrsh.w	r3, [r3]
 800d22c:	2b00      	cmp	r3, #0
 800d22e:	dac4      	bge.n	800d1ba <KeyQuickAct+0x52>
 800d230:	4c1d      	ldr	r4, [pc, #116]	; (800d2a8 <KeyQuickAct+0x140>)
 800d232:	4d1f      	ldr	r5, [pc, #124]	; (800d2b0 <KeyQuickAct+0x148>)
 800d234:	4e1f      	ldr	r6, [pc, #124]	; (800d2b4 <KeyQuickAct+0x14c>)
 800d236:	4621      	mov	r1, r4
 800d238:	2006      	movs	r0, #6
 800d23a:	f7f2 ff61 	bl	8000100 <__Bios>
 800d23e:	43c0      	mvns	r0, r0
 800d240:	f000 000f 	and.w	r0, r0, #15
 800d244:	2808      	cmp	r0, #8
 800d246:	d1ac      	bne.n	800d1a2 <KeyQuickAct+0x3a>
 800d248:	786b      	ldrb	r3, [r5, #1]
 800d24a:	2b02      	cmp	r3, #2
 800d24c:	d112      	bne.n	800d274 <KeyQuickAct+0x10c>
 800d24e:	4b1a      	ldr	r3, [pc, #104]	; (800d2b8 <KeyQuickAct+0x150>)
 800d250:	481a      	ldr	r0, [pc, #104]	; (800d2bc <KeyQuickAct+0x154>)
 800d252:	8b1a      	ldrh	r2, [r3, #24]
 800d254:	8b00      	ldrh	r0, [r0, #24]
 800d256:	8ad9      	ldrh	r1, [r3, #22]
 800d258:	1a12      	subs	r2, r2, r0
 800d25a:	428a      	cmp	r2, r1
 800d25c:	bfac      	ite	ge
 800d25e:	831a      	strhge	r2, [r3, #24]
 800d260:	8319      	strhlt	r1, [r3, #24]
 800d262:	f240 1219 	movw	r2, #281	; 0x119
 800d266:	4916      	ldr	r1, [pc, #88]	; (800d2c0 <KeyQuickAct+0x158>)
 800d268:	4816      	ldr	r0, [pc, #88]	; (800d2c4 <KeyQuickAct+0x15c>)
 800d26a:	f005 fc5b 	bl	8012b24 <memcpy>
 800d26e:	f7ff f8b1 	bl	800c3d4 <ShowWaveToLCD>
 800d272:	e7e0      	b.n	800d236 <KeyQuickAct+0xce>
 800d274:	2b01      	cmp	r3, #1
 800d276:	d10a      	bne.n	800d28e <KeyQuickAct+0x126>
 800d278:	4a0f      	ldr	r2, [pc, #60]	; (800d2b8 <KeyQuickAct+0x150>)
 800d27a:	4914      	ldr	r1, [pc, #80]	; (800d2cc <KeyQuickAct+0x164>)
 800d27c:	8ad3      	ldrh	r3, [r2, #22]
 800d27e:	8ac9      	ldrh	r1, [r1, #22]
 800d280:	4299      	cmp	r1, r3
 800d282:	d2ee      	bcs.n	800d262 <KeyQuickAct+0xfa>
 800d284:	490d      	ldr	r1, [pc, #52]	; (800d2bc <KeyQuickAct+0x154>)
 800d286:	8ac9      	ldrh	r1, [r1, #22]
 800d288:	1a5b      	subs	r3, r3, r1
 800d28a:	82d3      	strh	r3, [r2, #22]
 800d28c:	e7e9      	b.n	800d262 <KeyQuickAct+0xfa>
 800d28e:	2b03      	cmp	r3, #3
 800d290:	d1e7      	bne.n	800d262 <KeyQuickAct+0xfa>
 800d292:	2301      	movs	r3, #1
 800d294:	f886 3020 	strb.w	r3, [r6, #32]
 800d298:	f7ff f994 	bl	800c5c4 <ZC_Scale>
 800d29c:	f7fd f842 	bl	800a324 <ShowWindowPrecent>
 800d2a0:	e7df      	b.n	800d262 <KeyQuickAct+0xfa>
 800d2a2:	bf00      	nop
 800d2a4:	20004c12 	.word	0x20004c12
 800d2a8:	fffc0000 	.word	0xfffc0000
 800d2ac:	200009b6 	.word	0x200009b6
 800d2b0:	20000244 	.word	0x20000244
 800d2b4:	20004c18 	.word	0x20004c18
 800d2b8:	2000000c 	.word	0x2000000c
 800d2bc:	0801360a 	.word	0x0801360a
 800d2c0:	20000af3 	.word	0x20000af3
 800d2c4:	200009da 	.word	0x200009da
 800d2c8:	08012d18 	.word	0x08012d18
 800d2cc:	08012c8c 	.word	0x08012c8c

0800d2d0 <SPI_AnalyzeFrame>:
 800d2d0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800d2d4:	2500      	movs	r5, #0
 800d2d6:	4b61      	ldr	r3, [pc, #388]	; (800d45c <SPI_AnalyzeFrame+0x18c>)
 800d2d8:	b087      	sub	sp, #28
 800d2da:	7a1a      	ldrb	r2, [r3, #8]
 800d2dc:	7b1c      	ldrb	r4, [r3, #12]
 800d2de:	9204      	str	r2, [sp, #16]
 800d2e0:	7a9a      	ldrb	r2, [r3, #10]
 800d2e2:	f893 9009 	ldrb.w	r9, [r3, #9]
 800d2e6:	9202      	str	r2, [sp, #8]
 800d2e8:	7ada      	ldrb	r2, [r3, #11]
 800d2ea:	2100      	movs	r1, #0
 800d2ec:	9200      	str	r2, [sp, #0]
 800d2ee:	485c      	ldr	r0, [pc, #368]	; (800d460 <SPI_AnalyzeFrame+0x190>)
 800d2f0:	f44f 52e0 	mov.w	r2, #7168	; 0x1c00
 800d2f4:	f005 fc21 	bl	8012b3a <memset>
 800d2f8:	0063      	lsls	r3, r4, #1
 800d2fa:	9303      	str	r3, [sp, #12]
 800d2fc:	f04f 080a 	mov.w	r8, #10
 800d300:	464b      	mov	r3, r9
 800d302:	462c      	mov	r4, r5
 800d304:	46ab      	mov	fp, r5
 800d306:	462e      	mov	r6, r5
 800d308:	46aa      	mov	sl, r5
 800d30a:	4f55      	ldr	r7, [pc, #340]	; (800d460 <SPI_AnalyzeFrame+0x190>)
 800d30c:	fa1f f088 	uxth.w	r0, r8
 800d310:	9305      	str	r3, [sp, #20]
 800d312:	f7ff f8c5 	bl	800c4a0 <TransStatusData>
 800d316:	f3c0 0380 	ubfx	r3, r0, #2, #1
 800d31a:	9301      	str	r3, [sp, #4]
 800d31c:	9b04      	ldr	r3, [sp, #16]
 800d31e:	f3c0 02c0 	ubfx	r2, r0, #3, #1
 800d322:	4293      	cmp	r3, r2
 800d324:	9b05      	ldr	r3, [sp, #20]
 800d326:	f040 808d 	bne.w	800d444 <SPI_AnalyzeFrame+0x174>
 800d32a:	f1ba 0f00 	cmp.w	sl, #0
 800d32e:	d142      	bne.n	800d3b6 <SPI_AnalyzeFrame+0xe6>
 800d330:	4599      	cmp	r9, r3
 800d332:	d12d      	bne.n	800d390 <SPI_AnalyzeFrame+0xc0>
 800d334:	9b01      	ldr	r3, [sp, #4]
 800d336:	454b      	cmp	r3, r9
 800d338:	d02a      	beq.n	800d390 <SPI_AnalyzeFrame+0xc0>
 800d33a:	4a4a      	ldr	r2, [pc, #296]	; (800d464 <SPI_AnalyzeFrame+0x194>)
 800d33c:	f812 3028 	ldrb.w	r3, [r2, r8, lsl #2]
 800d340:	f063 033f 	orn	r3, r3, #63	; 0x3f
 800d344:	f802 3028 	strb.w	r3, [r2, r8, lsl #2]
 800d348:	9b02      	ldr	r3, [sp, #8]
 800d34a:	2b01      	cmp	r3, #1
 800d34c:	d01c      	beq.n	800d388 <SPI_AnalyzeFrame+0xb8>
 800d34e:	9900      	ldr	r1, [sp, #0]
 800d350:	eb05 0245 	add.w	r2, r5, r5, lsl #1
 800d354:	f917 c002 	ldrsb.w	ip, [r7, r2]
 800d358:	f000 0301 	and.w	r3, r0, #1
 800d35c:	b309      	cbz	r1, 800d3a2 <SPI_AnalyzeFrame+0xd2>
 800d35e:	40a3      	lsls	r3, r4
 800d360:	b25b      	sxtb	r3, r3
 800d362:	ea4c 0303 	orr.w	r3, ip, r3
 800d366:	9900      	ldr	r1, [sp, #0]
 800d368:	54bb      	strb	r3, [r7, r2]
 800d36a:	3201      	adds	r2, #1
 800d36c:	56bb      	ldrsb	r3, [r7, r2]
 800d36e:	f3c0 0040 	ubfx	r0, r0, #1, #1
 800d372:	b1d9      	cbz	r1, 800d3ac <SPI_AnalyzeFrame+0xdc>
 800d374:	40a0      	lsls	r0, r4
 800d376:	2c06      	cmp	r4, #6
 800d378:	b240      	sxtb	r0, r0
 800d37a:	ea40 0003 	orr.w	r0, r0, r3
 800d37e:	bf98      	it	ls
 800d380:	3401      	addls	r4, #1
 800d382:	54b8      	strb	r0, [r7, r2]
 800d384:	bf98      	it	ls
 800d386:	b2e4      	uxtbls	r4, r4
 800d388:	f04f 0a01 	mov.w	sl, #1
 800d38c:	3601      	adds	r6, #1
 800d38e:	b2f6      	uxtb	r6, r6
 800d390:	f108 0801 	add.w	r8, r8, #1
 800d394:	f5b8 6f60 	cmp.w	r8, #3584	; 0xe00
 800d398:	9b01      	ldr	r3, [sp, #4]
 800d39a:	d1b7      	bne.n	800d30c <SPI_AnalyzeFrame+0x3c>
 800d39c:	b007      	add	sp, #28
 800d39e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800d3a2:	f1c4 0e07 	rsb	lr, r4, #7
 800d3a6:	fa03 f30e 	lsl.w	r3, r3, lr
 800d3aa:	e7d9      	b.n	800d360 <SPI_AnalyzeFrame+0x90>
 800d3ac:	f1c4 0c07 	rsb	ip, r4, #7
 800d3b0:	fa00 f00c 	lsl.w	r0, r0, ip
 800d3b4:	e7df      	b.n	800d376 <SPI_AnalyzeFrame+0xa6>
 800d3b6:	9a01      	ldr	r2, [sp, #4]
 800d3b8:	429a      	cmp	r2, r3
 800d3ba:	d027      	beq.n	800d40c <SPI_AnalyzeFrame+0x13c>
 800d3bc:	9b02      	ldr	r3, [sp, #8]
 800d3be:	3601      	adds	r6, #1
 800d3c0:	2b01      	cmp	r3, #1
 800d3c2:	b2f6      	uxtb	r6, r6
 800d3c4:	d11f      	bne.n	800d406 <SPI_AnalyzeFrame+0x136>
 800d3c6:	4591      	cmp	r9, r2
 800d3c8:	d120      	bne.n	800d40c <SPI_AnalyzeFrame+0x13c>
 800d3ca:	9900      	ldr	r1, [sp, #0]
 800d3cc:	eb05 0245 	add.w	r2, r5, r5, lsl #1
 800d3d0:	f917 c002 	ldrsb.w	ip, [r7, r2]
 800d3d4:	f000 0301 	and.w	r3, r0, #1
 800d3d8:	b319      	cbz	r1, 800d422 <SPI_AnalyzeFrame+0x152>
 800d3da:	40a3      	lsls	r3, r4
 800d3dc:	b25b      	sxtb	r3, r3
 800d3de:	ea4c 0303 	orr.w	r3, ip, r3
 800d3e2:	9900      	ldr	r1, [sp, #0]
 800d3e4:	54bb      	strb	r3, [r7, r2]
 800d3e6:	3201      	adds	r2, #1
 800d3e8:	56bb      	ldrsb	r3, [r7, r2]
 800d3ea:	f3c0 0040 	ubfx	r0, r0, #1, #1
 800d3ee:	b1e9      	cbz	r1, 800d42c <SPI_AnalyzeFrame+0x15c>
 800d3f0:	40a0      	lsls	r0, r4
 800d3f2:	2c06      	cmp	r4, #6
 800d3f4:	b240      	sxtb	r0, r0
 800d3f6:	ea40 0003 	orr.w	r0, r0, r3
 800d3fa:	bf98      	it	ls
 800d3fc:	3401      	addls	r4, #1
 800d3fe:	54b8      	strb	r0, [r7, r2]
 800d400:	bf98      	it	ls
 800d402:	b2e4      	uxtbls	r4, r4
 800d404:	e002      	b.n	800d40c <SPI_AnalyzeFrame+0x13c>
 800d406:	9b01      	ldr	r3, [sp, #4]
 800d408:	4599      	cmp	r9, r3
 800d40a:	d1de      	bne.n	800d3ca <SPI_AnalyzeFrame+0xfa>
 800d40c:	9b03      	ldr	r3, [sp, #12]
 800d40e:	429e      	cmp	r6, r3
 800d410:	da11      	bge.n	800d436 <SPI_AnalyzeFrame+0x166>
 800d412:	4a14      	ldr	r2, [pc, #80]	; (800d464 <SPI_AnalyzeFrame+0x194>)
 800d414:	f812 3028 	ldrb.w	r3, [r2, r8, lsl #2]
 800d418:	f063 033f 	orn	r3, r3, #63	; 0x3f
 800d41c:	f802 3028 	strb.w	r3, [r2, r8, lsl #2]
 800d420:	e7b6      	b.n	800d390 <SPI_AnalyzeFrame+0xc0>
 800d422:	f1c4 0e07 	rsb	lr, r4, #7
 800d426:	fa03 f30e 	lsl.w	r3, r3, lr
 800d42a:	e7d7      	b.n	800d3dc <SPI_AnalyzeFrame+0x10c>
 800d42c:	f1c4 0c07 	rsb	ip, r4, #7
 800d430:	fa00 f00c 	lsl.w	r0, r0, ip
 800d434:	e7dd      	b.n	800d3f2 <SPI_AnalyzeFrame+0x122>
 800d436:	46d3      	mov	fp, sl
 800d438:	2400      	movs	r4, #0
 800d43a:	3501      	adds	r5, #1
 800d43c:	b2ad      	uxth	r5, r5
 800d43e:	4626      	mov	r6, r4
 800d440:	46a2      	mov	sl, r4
 800d442:	e7a5      	b.n	800d390 <SPI_AnalyzeFrame+0xc0>
 800d444:	f1bb 0f00 	cmp.w	fp, #0
 800d448:	d105      	bne.n	800d456 <SPI_AnalyzeFrame+0x186>
 800d44a:	3501      	adds	r5, #1
 800d44c:	b2ad      	uxth	r5, r5
 800d44e:	465c      	mov	r4, fp
 800d450:	465e      	mov	r6, fp
 800d452:	46da      	mov	sl, fp
 800d454:	e79c      	b.n	800d390 <SPI_AnalyzeFrame+0xc0>
 800d456:	2400      	movs	r4, #0
 800d458:	46a3      	mov	fp, r4
 800d45a:	e7f0      	b.n	800d43e <SPI_AnalyzeFrame+0x16e>
 800d45c:	20000098 	.word	0x20000098
 800d460:	20006854 	.word	0x20006854
 800d464:	20000c0c 	.word	0x20000c0c

0800d468 <IIC_AnalyzeFrame>:
 800d468:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800d46c:	f44f 52e0 	mov.w	r2, #7168	; 0x1c00
 800d470:	b087      	sub	sp, #28
 800d472:	2100      	movs	r1, #0
 800d474:	4857      	ldr	r0, [pc, #348]	; (800d5d4 <IIC_AnalyzeFrame+0x16c>)
 800d476:	2500      	movs	r5, #0
 800d478:	f005 fb5f 	bl	8012b3a <memset>
 800d47c:	2301      	movs	r3, #1
 800d47e:	f04f 080a 	mov.w	r8, #10
 800d482:	462c      	mov	r4, r5
 800d484:	462e      	mov	r6, r5
 800d486:	462f      	mov	r7, r5
 800d488:	469b      	mov	fp, r3
 800d48a:	f8df 9148 	ldr.w	r9, [pc, #328]	; 800d5d4 <IIC_AnalyzeFrame+0x16c>
 800d48e:	9501      	str	r5, [sp, #4]
 800d490:	9502      	str	r5, [sp, #8]
 800d492:	fa1f f188 	uxth.w	r1, r8
 800d496:	4608      	mov	r0, r1
 800d498:	9305      	str	r3, [sp, #20]
 800d49a:	9104      	str	r1, [sp, #16]
 800d49c:	f7ff f800 	bl	800c4a0 <TransStatusData>
 800d4a0:	9904      	ldr	r1, [sp, #16]
 800d4a2:	9003      	str	r0, [sp, #12]
 800d4a4:	f000 0a01 	and.w	sl, r0, #1
 800d4a8:	4608      	mov	r0, r1
 800d4aa:	f7fe fff9 	bl	800c4a0 <TransStatusData>
 800d4ae:	9a03      	ldr	r2, [sp, #12]
 800d4b0:	f3c0 0040 	ubfx	r0, r0, #1, #1
 800d4b4:	9b05      	ldr	r3, [sp, #20]
 800d4b6:	b99c      	cbnz	r4, 800d4e0 <IIC_AnalyzeFrame+0x78>
 800d4b8:	2800      	cmp	r0, #0
 800d4ba:	d037      	beq.n	800d52c <IIC_AnalyzeFrame+0xc4>
 800d4bc:	2b00      	cmp	r3, #0
 800d4be:	d035      	beq.n	800d52c <IIC_AnalyzeFrame+0xc4>
 800d4c0:	f1bb 0f00 	cmp.w	fp, #0
 800d4c4:	d006      	beq.n	800d4d4 <IIC_AnalyzeFrame+0x6c>
 800d4c6:	f1ba 0f00 	cmp.w	sl, #0
 800d4ca:	9902      	ldr	r1, [sp, #8]
 800d4cc:	bf04      	itt	eq
 800d4ce:	4659      	moveq	r1, fp
 800d4d0:	465f      	moveq	r7, fp
 800d4d2:	9102      	str	r1, [sp, #8]
 800d4d4:	9901      	ldr	r1, [sp, #4]
 800d4d6:	b139      	cbz	r1, 800d4e8 <IIC_AnalyzeFrame+0x80>
 800d4d8:	2f00      	cmp	r7, #0
 800d4da:	d145      	bne.n	800d568 <IIC_AnalyzeFrame+0x100>
 800d4dc:	463c      	mov	r4, r7
 800d4de:	e01b      	b.n	800d518 <IIC_AnalyzeFrame+0xb0>
 800d4e0:	9901      	ldr	r1, [sp, #4]
 800d4e2:	bb19      	cbnz	r1, 800d52c <IIC_AnalyzeFrame+0xc4>
 800d4e4:	b310      	cbz	r0, 800d52c <IIC_AnalyzeFrame+0xc4>
 800d4e6:	b30b      	cbz	r3, 800d52c <IIC_AnalyzeFrame+0xc4>
 800d4e8:	f1bb 0f00 	cmp.w	fp, #0
 800d4ec:	d11e      	bne.n	800d52c <IIC_AnalyzeFrame+0xc4>
 800d4ee:	f1ba 0f00 	cmp.w	sl, #0
 800d4f2:	d01b      	beq.n	800d52c <IIC_AnalyzeFrame+0xc4>
 800d4f4:	4653      	mov	r3, sl
 800d4f6:	e00a      	b.n	800d50e <IIC_AnalyzeFrame+0xa6>
 800d4f8:	4e37      	ldr	r6, [pc, #220]	; (800d5d8 <IIC_AnalyzeFrame+0x170>)
 800d4fa:	eba8 0103 	sub.w	r1, r8, r3
 800d4fe:	f816 2021 	ldrb.w	r2, [r6, r1, lsl #2]
 800d502:	3301      	adds	r3, #1
 800d504:	f022 0250 	bic.w	r2, r2, #80	; 0x50
 800d508:	f806 2021 	strb.w	r2, [r6, r1, lsl #2]
 800d50c:	b2db      	uxtb	r3, r3
 800d50e:	42a3      	cmp	r3, r4
 800d510:	d9f2      	bls.n	800d4f8 <IIC_AnalyzeFrame+0x90>
 800d512:	2400      	movs	r4, #0
 800d514:	4627      	mov	r7, r4
 800d516:	4626      	mov	r6, r4
 800d518:	f108 0801 	add.w	r8, r8, #1
 800d51c:	f5b8 6f60 	cmp.w	r8, #3584	; 0xe00
 800d520:	46d3      	mov	fp, sl
 800d522:	4603      	mov	r3, r0
 800d524:	d1b5      	bne.n	800d492 <IIC_AnalyzeFrame+0x2a>
 800d526:	b007      	add	sp, #28
 800d528:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800d52c:	2f00      	cmp	r7, #0
 800d52e:	d0f3      	beq.n	800d518 <IIC_AnalyzeFrame+0xb0>
 800d530:	4298      	cmp	r0, r3
 800d532:	d006      	beq.n	800d542 <IIC_AnalyzeFrame+0xda>
 800d534:	b924      	cbnz	r4, 800d540 <IIC_AnalyzeFrame+0xd8>
 800d536:	b938      	cbnz	r0, 800d548 <IIC_AnalyzeFrame+0xe0>
 800d538:	2e00      	cmp	r6, #0
 800d53a:	d148      	bne.n	800d5ce <IIC_AnalyzeFrame+0x166>
 800d53c:	4634      	mov	r4, r6
 800d53e:	e7eb      	b.n	800d518 <IIC_AnalyzeFrame+0xb0>
 800d540:	b918      	cbnz	r0, 800d54a <IIC_AnalyzeFrame+0xe2>
 800d542:	2c09      	cmp	r4, #9
 800d544:	d8e8      	bhi.n	800d518 <IIC_AnalyzeFrame+0xb0>
 800d546:	e00f      	b.n	800d568 <IIC_AnalyzeFrame+0x100>
 800d548:	4606      	mov	r6, r0
 800d54a:	3401      	adds	r4, #1
 800d54c:	b2e4      	uxtb	r4, r4
 800d54e:	2c08      	cmp	r4, #8
 800d550:	d817      	bhi.n	800d582 <IIC_AnalyzeFrame+0x11a>
 800d552:	eb05 0245 	add.w	r2, r5, r5, lsl #1
 800d556:	f819 1002 	ldrb.w	r1, [r9, r2]
 800d55a:	f1c4 0308 	rsb	r3, r4, #8
 800d55e:	fa0a f303 	lsl.w	r3, sl, r3
 800d562:	430b      	orrs	r3, r1
 800d564:	f809 3002 	strb.w	r3, [r9, r2]
 800d568:	b13e      	cbz	r6, 800d57a <IIC_AnalyzeFrame+0x112>
 800d56a:	2601      	movs	r6, #1
 800d56c:	4a1a      	ldr	r2, [pc, #104]	; (800d5d8 <IIC_AnalyzeFrame+0x170>)
 800d56e:	f812 3028 	ldrb.w	r3, [r2, r8, lsl #2]
 800d572:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 800d576:	f802 3028 	strb.w	r3, [r2, r8, lsl #2]
 800d57a:	2c09      	cmp	r4, #9
 800d57c:	d009      	beq.n	800d592 <IIC_AnalyzeFrame+0x12a>
 800d57e:	2701      	movs	r7, #1
 800d580:	e7ca      	b.n	800d518 <IIC_AnalyzeFrame+0xb0>
 800d582:	2c09      	cmp	r4, #9
 800d584:	d1dd      	bne.n	800d542 <IIC_AnalyzeFrame+0xda>
 800d586:	43d2      	mvns	r2, r2
 800d588:	f002 0301 	and.w	r3, r2, #1
 800d58c:	9301      	str	r3, [sp, #4]
 800d58e:	2e00      	cmp	r6, #0
 800d590:	d1eb      	bne.n	800d56a <IIC_AnalyzeFrame+0x102>
 800d592:	9a01      	ldr	r2, [sp, #4]
 800d594:	eb05 0445 	add.w	r4, r5, r5, lsl #1
 800d598:	9e02      	ldr	r6, [sp, #8]
 800d59a:	1c61      	adds	r1, r4, #1
 800d59c:	0092      	lsls	r2, r2, #2
 800d59e:	f819 3001 	ldrb.w	r3, [r9, r1]
 800d5a2:	b252      	sxtb	r2, r2
 800d5a4:	b14e      	cbz	r6, 800d5ba <IIC_AnalyzeFrame+0x152>
 800d5a6:	f043 0301 	orr.w	r3, r3, #1
 800d5aa:	f809 3001 	strb.w	r3, [r9, r1]
 800d5ae:	f819 4004 	ldrb.w	r4, [r9, r4]
 800d5b2:	0064      	lsls	r4, r4, #1
 800d5b4:	f004 0402 	and.w	r4, r4, #2
 800d5b8:	4323      	orrs	r3, r4
 800d5ba:	2400      	movs	r4, #0
 800d5bc:	4313      	orrs	r3, r2
 800d5be:	3501      	adds	r5, #1
 800d5c0:	f809 3001 	strb.w	r3, [r9, r1]
 800d5c4:	b2ad      	uxth	r5, r5
 800d5c6:	2701      	movs	r7, #1
 800d5c8:	4626      	mov	r6, r4
 800d5ca:	9402      	str	r4, [sp, #8]
 800d5cc:	e7a4      	b.n	800d518 <IIC_AnalyzeFrame+0xb0>
 800d5ce:	4604      	mov	r4, r0
 800d5d0:	e7cb      	b.n	800d56a <IIC_AnalyzeFrame+0x102>
 800d5d2:	bf00      	nop
 800d5d4:	20006854 	.word	0x20006854
 800d5d8:	20000c0c 	.word	0x20000c0c

0800d5dc <I2C_AnalyzeStartCnt>:
 800d5dc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800d5e0:	2600      	movs	r6, #0
 800d5e2:	f04f 0801 	mov.w	r8, #1
 800d5e6:	b087      	sub	sp, #28
 800d5e8:	f8df b0e8 	ldr.w	fp, [pc, #232]	; 800d6d4 <I2C_AnalyzeStartCnt+0xf8>
 800d5ec:	e9cd 0100 	strd	r0, r1, [sp]
 800d5f0:	f44f 62e0 	mov.w	r2, #1792	; 0x700
 800d5f4:	2100      	movs	r1, #0
 800d5f6:	4837      	ldr	r0, [pc, #220]	; (800d6d4 <I2C_AnalyzeStartCnt+0xf8>)
 800d5f8:	f005 fa9f 	bl	8012b3a <memset>
 800d5fc:	f04f 090a 	mov.w	r9, #10
 800d600:	4642      	mov	r2, r8
 800d602:	4634      	mov	r4, r6
 800d604:	4635      	mov	r5, r6
 800d606:	4637      	mov	r7, r6
 800d608:	4659      	mov	r1, fp
 800d60a:	4648      	mov	r0, r9
 800d60c:	e9cd 2104 	strd	r2, r1, [sp, #16]
 800d610:	f7fe ff46 	bl	800c4a0 <TransStatusData>
 800d614:	f109 0901 	add.w	r9, r9, #1
 800d618:	fa1f f989 	uxth.w	r9, r9
 800d61c:	f3c0 0340 	ubfx	r3, r0, #1, #1
 800d620:	f000 0a01 	and.w	sl, r0, #1
 800d624:	4648      	mov	r0, r9
 800d626:	9303      	str	r3, [sp, #12]
 800d628:	f7fe ff48 	bl	800c4bc <TransformTime>
 800d62c:	f85b 3026 	ldr.w	r3, [fp, r6, lsl #2]
 800d630:	9905      	ldr	r1, [sp, #20]
 800d632:	4418      	add	r0, r3
 800d634:	9b03      	ldr	r3, [sp, #12]
 800d636:	f84b 0026 	str.w	r0, [fp, r6, lsl #2]
 800d63a:	b15b      	cbz	r3, 800d654 <I2C_AnalyzeStartCnt+0x78>
 800d63c:	f1b8 0f01 	cmp.w	r8, #1
 800d640:	9a04      	ldr	r2, [sp, #16]
 800d642:	d113      	bne.n	800d66c <I2C_AnalyzeStartCnt+0x90>
 800d644:	b1ea      	cbz	r2, 800d682 <I2C_AnalyzeStartCnt+0xa6>
 800d646:	f1ba 0f00 	cmp.w	sl, #0
 800d64a:	d040      	beq.n	800d6ce <I2C_AnalyzeStartCnt+0xf2>
 800d64c:	2c01      	cmp	r4, #1
 800d64e:	d03e      	beq.n	800d6ce <I2C_AnalyzeStartCnt+0xf2>
 800d650:	4625      	mov	r5, r4
 800d652:	e009      	b.n	800d668 <I2C_AnalyzeStartCnt+0x8c>
 800d654:	2c01      	cmp	r4, #1
 800d656:	d1fb      	bne.n	800d650 <I2C_AnalyzeStartCnt+0x74>
 800d658:	f1b8 0f00 	cmp.w	r8, #0
 800d65c:	d134      	bne.n	800d6c8 <I2C_AnalyzeStartCnt+0xec>
 800d65e:	2f01      	cmp	r7, #1
 800d660:	d008      	beq.n	800d674 <I2C_AnalyzeStartCnt+0x98>
 800d662:	46a0      	mov	r8, r4
 800d664:	e012      	b.n	800d68c <I2C_AnalyzeStartCnt+0xb0>
 800d666:	2401      	movs	r4, #1
 800d668:	4627      	mov	r7, r4
 800d66a:	e011      	b.n	800d690 <I2C_AnalyzeStartCnt+0xb4>
 800d66c:	2c01      	cmp	r4, #1
 800d66e:	d1ef      	bne.n	800d650 <I2C_AnalyzeStartCnt+0x74>
 800d670:	3501      	adds	r5, #1
 800d672:	b2ed      	uxtb	r5, r5
 800d674:	2d11      	cmp	r5, #17
 800d676:	d1f6      	bne.n	800d666 <I2C_AnalyzeStartCnt+0x8a>
 800d678:	3601      	adds	r6, #1
 800d67a:	b2b6      	uxth	r6, r6
 800d67c:	2401      	movs	r4, #1
 800d67e:	2500      	movs	r5, #0
 800d680:	e005      	b.n	800d68e <I2C_AnalyzeStartCnt+0xb2>
 800d682:	f1ba 0f00 	cmp.w	sl, #0
 800d686:	d0e1      	beq.n	800d64c <I2C_AnalyzeStartCnt+0x70>
 800d688:	4690      	mov	r8, r2
 800d68a:	4615      	mov	r5, r2
 800d68c:	4644      	mov	r4, r8
 800d68e:	2700      	movs	r7, #0
 800d690:	f5b9 6f60 	cmp.w	r9, #3584	; 0xe00
 800d694:	4652      	mov	r2, sl
 800d696:	4698      	mov	r8, r3
 800d698:	d1b7      	bne.n	800d60a <I2C_AnalyzeStartCnt+0x2e>
 800d69a:	2300      	movs	r3, #0
 800d69c:	2400      	movs	r4, #0
 800d69e:	2500      	movs	r5, #0
 800d6a0:	e9dd 6700 	ldrd	r6, r7, [sp]
 800d6a4:	f851 2023 	ldr.w	r2, [r1, r3, lsl #2]
 800d6a8:	b298      	uxth	r0, r3
 800d6aa:	18a4      	adds	r4, r4, r2
 800d6ac:	f145 0500 	adc.w	r5, r5, #0
 800d6b0:	42bd      	cmp	r5, r7
 800d6b2:	bf08      	it	eq
 800d6b4:	42b4      	cmpeq	r4, r6
 800d6b6:	d204      	bcs.n	800d6c2 <I2C_AnalyzeStartCnt+0xe6>
 800d6b8:	3301      	adds	r3, #1
 800d6ba:	f5b3 6fe0 	cmp.w	r3, #1792	; 0x700
 800d6be:	d1ef      	bne.n	800d6a0 <I2C_AnalyzeStartCnt+0xc4>
 800d6c0:	2000      	movs	r0, #0
 800d6c2:	b007      	add	sp, #28
 800d6c4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800d6c8:	2f01      	cmp	r7, #1
 800d6ca:	d1df      	bne.n	800d68c <I2C_AnalyzeStartCnt+0xb0>
 800d6cc:	e7d0      	b.n	800d670 <I2C_AnalyzeStartCnt+0x94>
 800d6ce:	2f01      	cmp	r7, #1
 800d6d0:	d0d0      	beq.n	800d674 <I2C_AnalyzeStartCnt+0x98>
 800d6d2:	e7db      	b.n	800d68c <I2C_AnalyzeStartCnt+0xb0>
 800d6d4:	20004c54 	.word	0x20004c54

0800d6d8 <Show_I2C_AnalyzeData>:
 800d6d8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800d6dc:	4b4e      	ldr	r3, [pc, #312]	; (800d818 <Show_I2C_AnalyzeData+0x140>)
 800d6de:	4c4f      	ldr	r4, [pc, #316]	; (800d81c <Show_I2C_AnalyzeData+0x144>)
 800d6e0:	b08d      	sub	sp, #52	; 0x34
 800d6e2:	e9d3 0100 	ldrd	r0, r1, [r3]
 800d6e6:	f894 a011 	ldrb.w	sl, [r4, #17]
 800d6ea:	f7ff ff77 	bl	800d5dc <I2C_AnalyzeStartCnt>
 800d6ee:	f1ba 0f02 	cmp.w	sl, #2
 800d6f2:	4606      	mov	r6, r0
 800d6f4:	9407      	str	r4, [sp, #28]
 800d6f6:	f200 808d 	bhi.w	800d814 <Show_I2C_AnalyzeData+0x13c>
 800d6fa:	4b49      	ldr	r3, [pc, #292]	; (800d820 <Show_I2C_AnalyzeData+0x148>)
 800d6fc:	f813 300a 	ldrb.w	r3, [r3, sl]
 800d700:	2500      	movs	r5, #0
 800d702:	46a9      	mov	r9, r5
 800d704:	9305      	str	r3, [sp, #20]
 800d706:	9b05      	ldr	r3, [sp, #20]
 800d708:	4c46      	ldr	r4, [pc, #280]	; (800d824 <Show_I2C_AnalyzeData+0x14c>)
 800d70a:	085b      	lsrs	r3, r3, #1
 800d70c:	9306      	str	r3, [sp, #24]
 800d70e:	4a46      	ldr	r2, [pc, #280]	; (800d828 <Show_I2C_AnalyzeData+0x150>)
 800d710:	b2af      	uxth	r7, r5
 800d712:	18ab      	adds	r3, r5, r2
 800d714:	5caa      	ldrb	r2, [r5, r2]
 800d716:	785b      	ldrb	r3, [r3, #1]
 800d718:	06d2      	lsls	r2, r2, #27
 800d71a:	d40c      	bmi.n	800d736 <Show_I2C_AnalyzeData+0x5e>
 800d71c:	06d8      	lsls	r0, r3, #27
 800d71e:	d502      	bpl.n	800d726 <Show_I2C_AnalyzeData+0x4e>
 800d720:	3703      	adds	r7, #3
 800d722:	fa1f f987 	uxth.w	r9, r7
 800d726:	f240 1317 	movw	r3, #279	; 0x117
 800d72a:	3501      	adds	r5, #1
 800d72c:	429d      	cmp	r5, r3
 800d72e:	d1ee      	bne.n	800d70e <Show_I2C_AnalyzeData+0x36>
 800d730:	b00d      	add	sp, #52	; 0x34
 800d732:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800d736:	06d9      	lsls	r1, r3, #27
 800d738:	d4f5      	bmi.n	800d726 <Show_I2C_AnalyzeData+0x4e>
 800d73a:	eb06 0346 	add.w	r3, r6, r6, lsl #1
 800d73e:	f103 0b01 	add.w	fp, r3, #1
 800d742:	f814 200b 	ldrb.w	r2, [r4, fp]
 800d746:	eba7 0709 	sub.w	r7, r7, r9
 800d74a:	07d2      	lsls	r2, r2, #31
 800d74c:	b2bf      	uxth	r7, r7
 800d74e:	d506      	bpl.n	800d75e <Show_I2C_AnalyzeData+0x86>
 800d750:	9a07      	ldr	r2, [sp, #28]
 800d752:	7c12      	ldrb	r2, [r2, #16]
 800d754:	b11a      	cbz	r2, 800d75e <Show_I2C_AnalyzeData+0x86>
 800d756:	5ce2      	ldrb	r2, [r4, r3]
 800d758:	f022 0201 	bic.w	r2, r2, #1
 800d75c:	54e2      	strb	r2, [r4, r3]
 800d75e:	f1ba 0f01 	cmp.w	sl, #1
 800d762:	d04d      	beq.n	800d800 <Show_I2C_AnalyzeData+0x128>
 800d764:	d347      	bcc.n	800d7f6 <Show_I2C_AnalyzeData+0x11e>
 800d766:	f1ba 0f02 	cmp.w	sl, #2
 800d76a:	d04e      	beq.n	800d80a <Show_I2C_AnalyzeData+0x132>
 800d76c:	9b06      	ldr	r3, [sp, #24]
 800d76e:	42bb      	cmp	r3, r7
 800d770:	d23e      	bcs.n	800d7f0 <Show_I2C_AnalyzeData+0x118>
 800d772:	f814 300b 	ldrb.w	r3, [r4, fp]
 800d776:	f109 0728 	add.w	r7, r9, #40	; 0x28
 800d77a:	f013 0801 	ands.w	r8, r3, #1
 800d77e:	b2bf      	uxth	r7, r7
 800d780:	d011      	beq.n	800d7a6 <Show_I2C_AnalyzeData+0xce>
 800d782:	4a2a      	ldr	r2, [pc, #168]	; (800d82c <Show_I2C_AnalyzeData+0x154>)
 800d784:	f013 0f02 	tst.w	r3, #2
 800d788:	4b29      	ldr	r3, [pc, #164]	; (800d830 <Show_I2C_AnalyzeData+0x158>)
 800d78a:	bf18      	it	ne
 800d78c:	4613      	movne	r3, r2
 800d78e:	2200      	movs	r2, #0
 800d790:	9302      	str	r3, [sp, #8]
 800d792:	e9cd 2200 	strd	r2, r2, [sp]
 800d796:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800d79a:	2166      	movs	r1, #102	; 0x66
 800d79c:	4638      	mov	r0, r7
 800d79e:	f7fe fb77 	bl	800be90 <Disp_Str7x9>
 800d7a2:	f04f 080e 	mov.w	r8, #14
 800d7a6:	eb07 0008 	add.w	r0, r7, r8
 800d7aa:	2700      	movs	r7, #0
 800d7ac:	ab09      	add	r3, sp, #36	; 0x24
 800d7ae:	9302      	str	r3, [sp, #8]
 800d7b0:	463a      	mov	r2, r7
 800d7b2:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800d7b6:	2166      	movs	r1, #102	; 0x66
 800d7b8:	b280      	uxth	r0, r0
 800d7ba:	e9cd 7700 	strd	r7, r7, [sp]
 800d7be:	f7fe fb67 	bl	800be90 <Disp_Str7x9>
 800d7c2:	9b05      	ldr	r3, [sp, #20]
 800d7c4:	4a1b      	ldr	r2, [pc, #108]	; (800d834 <Show_I2C_AnalyzeData+0x15c>)
 800d7c6:	f103 0028 	add.w	r0, r3, #40	; 0x28
 800d7ca:	f814 300b 	ldrb.w	r3, [r4, fp]
 800d7ce:	4448      	add	r0, r9
 800d7d0:	f013 0f04 	tst.w	r3, #4
 800d7d4:	4b18      	ldr	r3, [pc, #96]	; (800d838 <Show_I2C_AnalyzeData+0x160>)
 800d7d6:	bf18      	it	ne
 800d7d8:	4613      	movne	r3, r2
 800d7da:	4440      	add	r0, r8
 800d7dc:	e9cd 7301 	strd	r7, r3, [sp, #4]
 800d7e0:	9700      	str	r7, [sp, #0]
 800d7e2:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800d7e6:	463a      	mov	r2, r7
 800d7e8:	2166      	movs	r1, #102	; 0x66
 800d7ea:	b280      	uxth	r0, r0
 800d7ec:	f7fe fb50 	bl	800be90 <Disp_Str7x9>
 800d7f0:	3601      	adds	r6, #1
 800d7f2:	b2b6      	uxth	r6, r6
 800d7f4:	e797      	b.n	800d726 <Show_I2C_AnalyzeData+0x4e>
 800d7f6:	5ce1      	ldrb	r1, [r4, r3]
 800d7f8:	a809      	add	r0, sp, #36	; 0x24
 800d7fa:	f000 fc61 	bl	800e0c0 <Char2Str>
 800d7fe:	e7b5      	b.n	800d76c <Show_I2C_AnalyzeData+0x94>
 800d800:	5ce1      	ldrb	r1, [r4, r3]
 800d802:	a809      	add	r0, sp, #36	; 0x24
 800d804:	f000 fd31 	bl	800e26a <u8ToDec3Str>
 800d808:	e7b0      	b.n	800d76c <Show_I2C_AnalyzeData+0x94>
 800d80a:	5ce1      	ldrb	r1, [r4, r3]
 800d80c:	a809      	add	r0, sp, #36	; 0x24
 800d80e:	f000 fd98 	bl	800e342 <Char2HexFullStr>
 800d812:	e7ab      	b.n	800d76c <Show_I2C_AnalyzeData+0x94>
 800d814:	2300      	movs	r3, #0
 800d816:	e773      	b.n	800d700 <Show_I2C_AnalyzeData+0x28>
 800d818:	20004c18 	.word	0x20004c18
 800d81c:	20000098 	.word	0x20000098
 800d820:	08014648 	.word	0x08014648
 800d824:	20006854 	.word	0x20006854
 800d828:	20000af3 	.word	0x20000af3
 800d82c:	08014660 	.word	0x08014660
 800d830:	08014663 	.word	0x08014663
 800d834:	08014666 	.word	0x08014666
 800d838:	0801466b 	.word	0x0801466b

0800d83c <UART_AnalyzeFrame>:
 800d83c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800d840:	2400      	movs	r4, #0
 800d842:	4b3c      	ldr	r3, [pc, #240]	; (800d934 <UART_AnalyzeFrame+0xf8>)
 800d844:	260a      	movs	r6, #10
 800d846:	7e1a      	ldrb	r2, [r3, #24]
 800d848:	f893 a019 	ldrb.w	sl, [r3, #25]
 800d84c:	f893 801b 	ldrb.w	r8, [r3, #27]
 800d850:	4b39      	ldr	r3, [pc, #228]	; (800d938 <UART_AnalyzeFrame+0xfc>)
 800d852:	4625      	mov	r5, r4
 800d854:	eb03 0342 	add.w	r3, r3, r2, lsl #1
 800d858:	8899      	ldrh	r1, [r3, #4]
 800d85a:	4a38      	ldr	r2, [pc, #224]	; (800d93c <UART_AnalyzeFrame+0x100>)
 800d85c:	4b38      	ldr	r3, [pc, #224]	; (800d940 <UART_AnalyzeFrame+0x104>)
 800d85e:	fb92 f9f1 	sdiv	r9, r2, r1
 800d862:	fb93 f3f1 	sdiv	r3, r3, r1
 800d866:	2101      	movs	r1, #1
 800d868:	4499      	add	r9, r3
 800d86a:	f8df b0d8 	ldr.w	fp, [pc, #216]	; 800d944 <UART_AnalyzeFrame+0x108>
 800d86e:	b085      	sub	sp, #20
 800d870:	ea4f 0259 	mov.w	r2, r9, lsr #1
 800d874:	b2b3      	uxth	r3, r6
 800d876:	4618      	mov	r0, r3
 800d878:	e9cd 2102 	strd	r2, r1, [sp, #8]
 800d87c:	9301      	str	r3, [sp, #4]
 800d87e:	f7fe fe0f 	bl	800c4a0 <TransStatusData>
 800d882:	9b01      	ldr	r3, [sp, #4]
 800d884:	f000 0701 	and.w	r7, r0, #1
 800d888:	1c58      	adds	r0, r3, #1
 800d88a:	b280      	uxth	r0, r0
 800d88c:	f7fe fe16 	bl	800c4bc <TransformTime>
 800d890:	9903      	ldr	r1, [sp, #12]
 800d892:	9a02      	ldr	r2, [sp, #8]
 800d894:	428f      	cmp	r7, r1
 800d896:	d041      	beq.n	800d91c <UART_AnalyzeFrame+0xe0>
 800d898:	fbb0 fcf9 	udiv	ip, r0, r9
 800d89c:	fa5f f38c 	uxtb.w	r3, ip
 800d8a0:	fb09 0c1c 	mls	ip, r9, ip, r0
 800d8a4:	4594      	cmp	ip, r2
 800d8a6:	bf84      	itt	hi
 800d8a8:	3301      	addhi	r3, #1
 800d8aa:	b2db      	uxtbhi	r3, r3
 800d8ac:	b9ed      	cbnz	r5, 800d8ea <UART_AnalyzeFrame+0xae>
 800d8ae:	2901      	cmp	r1, #1
 800d8b0:	d119      	bne.n	800d8e6 <UART_AnalyzeFrame+0xaa>
 800d8b2:	bb3f      	cbnz	r7, 800d904 <UART_AnalyzeFrame+0xc8>
 800d8b4:	f81b 0026 	ldrb.w	r0, [fp, r6, lsl #2]
 800d8b8:	f040 0040 	orr.w	r0, r0, #64	; 0x40
 800d8bc:	f80b 0026 	strb.w	r0, [fp, r6, lsl #2]
 800d8c0:	b18b      	cbz	r3, 800d8e6 <UART_AnalyzeFrame+0xaa>
 800d8c2:	3b01      	subs	r3, #1
 800d8c4:	f013 03ff 	ands.w	r3, r3, #255	; 0xff
 800d8c8:	d00d      	beq.n	800d8e6 <UART_AnalyzeFrame+0xaa>
 800d8ca:	4423      	add	r3, r4
 800d8cc:	b2dc      	uxtb	r4, r3
 800d8ce:	f1b8 0f00 	cmp.w	r8, #0
 800d8d2:	d106      	bne.n	800d8e2 <UART_AnalyzeFrame+0xa6>
 800d8d4:	45a2      	cmp	sl, r4
 800d8d6:	d923      	bls.n	800d920 <UART_AnalyzeFrame+0xe4>
 800d8d8:	460d      	mov	r5, r1
 800d8da:	4647      	mov	r7, r8
 800d8dc:	e012      	b.n	800d904 <UART_AnalyzeFrame+0xc8>
 800d8de:	4639      	mov	r1, r7
 800d8e0:	e7c8      	b.n	800d874 <UART_AnalyzeFrame+0x38>
 800d8e2:	45a2      	cmp	sl, r4
 800d8e4:	d31f      	bcc.n	800d926 <UART_AnalyzeFrame+0xea>
 800d8e6:	460d      	mov	r5, r1
 800d8e8:	e00c      	b.n	800d904 <UART_AnalyzeFrame+0xc8>
 800d8ea:	f81b 1026 	ldrb.w	r1, [fp, r6, lsl #2]
 800d8ee:	4423      	add	r3, r4
 800d8f0:	f041 0140 	orr.w	r1, r1, #64	; 0x40
 800d8f4:	f80b 1026 	strb.w	r1, [fp, r6, lsl #2]
 800d8f8:	b2dc      	uxtb	r4, r3
 800d8fa:	f1b8 0f00 	cmp.w	r8, #0
 800d8fe:	d108      	bne.n	800d912 <UART_AnalyzeFrame+0xd6>
 800d900:	45a2      	cmp	sl, r4
 800d902:	d913      	bls.n	800d92c <UART_AnalyzeFrame+0xf0>
 800d904:	3601      	adds	r6, #1
 800d906:	f5b6 6f60 	cmp.w	r6, #3584	; 0xe00
 800d90a:	d1e8      	bne.n	800d8de <UART_AnalyzeFrame+0xa2>
 800d90c:	b005      	add	sp, #20
 800d90e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800d912:	45a2      	cmp	sl, r4
 800d914:	d2f6      	bcs.n	800d904 <UART_AnalyzeFrame+0xc8>
 800d916:	2400      	movs	r4, #0
 800d918:	4625      	mov	r5, r4
 800d91a:	e7f3      	b.n	800d904 <UART_AnalyzeFrame+0xc8>
 800d91c:	460f      	mov	r7, r1
 800d91e:	e7f1      	b.n	800d904 <UART_AnalyzeFrame+0xc8>
 800d920:	4644      	mov	r4, r8
 800d922:	4645      	mov	r5, r8
 800d924:	e7d9      	b.n	800d8da <UART_AnalyzeFrame+0x9e>
 800d926:	463c      	mov	r4, r7
 800d928:	463d      	mov	r5, r7
 800d92a:	e7eb      	b.n	800d904 <UART_AnalyzeFrame+0xc8>
 800d92c:	4644      	mov	r4, r8
 800d92e:	4645      	mov	r5, r8
 800d930:	e7e8      	b.n	800d904 <UART_AnalyzeFrame+0xc8>
 800d932:	bf00      	nop
 800d934:	20000098 	.word	0x20000098
 800d938:	08014648 	.word	0x08014648
 800d93c:	000f4240 	.word	0x000f4240
 800d940:	ffff3cb0 	.word	0xffff3cb0
 800d944:	20000c0c 	.word	0x20000c0c

0800d948 <Show_UART_AnalyzeData>:
 800d948:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800d94c:	4b4c      	ldr	r3, [pc, #304]	; (800da80 <Show_UART_AnalyzeData+0x138>)
 800d94e:	2400      	movs	r4, #0
 800d950:	7f9f      	ldrb	r7, [r3, #30]
 800d952:	7e1a      	ldrb	r2, [r3, #24]
 800d954:	f893 a019 	ldrb.w	sl, [r3, #25]
 800d958:	f893 901c 	ldrb.w	r9, [r3, #28]
 800d95c:	4b49      	ldr	r3, [pc, #292]	; (800da84 <Show_UART_AnalyzeData+0x13c>)
 800d95e:	2f02      	cmp	r7, #2
 800d960:	eb03 0342 	add.w	r3, r3, r2, lsl #1
 800d964:	889a      	ldrh	r2, [r3, #4]
 800d966:	4b48      	ldr	r3, [pc, #288]	; (800da88 <Show_UART_AnalyzeData+0x140>)
 800d968:	bf88      	it	hi
 800d96a:	2600      	movhi	r6, #0
 800d96c:	fb93 f2f2 	sdiv	r2, r3, r2
 800d970:	4b46      	ldr	r3, [pc, #280]	; (800da8c <Show_UART_AnalyzeData+0x144>)
 800d972:	eb02 0582 	add.w	r5, r2, r2, lsl #2
 800d976:	8a99      	ldrh	r1, [r3, #20]
 800d978:	4b45      	ldr	r3, [pc, #276]	; (800da90 <Show_UART_AnalyzeData+0x148>)
 800d97a:	b08d      	sub	sp, #52	; 0x34
 800d97c:	f853 1021 	ldr.w	r1, [r3, r1, lsl #2]
 800d980:	f04f 0328 	mov.w	r3, #40	; 0x28
 800d984:	fb03 f302 	mul.w	r3, r3, r2
 800d988:	fbb5 f5f1 	udiv	r5, r5, r1
 800d98c:	fbb3 f3f1 	udiv	r3, r3, r1
 800d990:	bf98      	it	ls
 800d992:	4a40      	ldrls	r2, [pc, #256]	; (800da94 <Show_UART_AnalyzeData+0x14c>)
 800d994:	fb05 fb0a 	mul.w	fp, r5, sl
 800d998:	bf98      	it	ls
 800d99a:	5dd6      	ldrbls	r6, [r2, r7]
 800d99c:	f8df 80f8 	ldr.w	r8, [pc, #248]	; 800da98 <Show_UART_AnalyzeData+0x150>
 800d9a0:	1b9b      	subs	r3, r3, r6
 800d9a2:	085b      	lsrs	r3, r3, #1
 800d9a4:	b29a      	uxth	r2, r3
 800d9a6:	9204      	str	r2, [sp, #16]
 800d9a8:	b29b      	uxth	r3, r3
 800d9aa:	086a      	lsrs	r2, r5, #1
 800d9ac:	eb0b 0102 	add.w	r1, fp, r2
 800d9b0:	9306      	str	r3, [sp, #24]
 800d9b2:	eba8 0302 	sub.w	r3, r8, r2
 800d9b6:	9105      	str	r1, [sp, #20]
 800d9b8:	9307      	str	r3, [sp, #28]
 800d9ba:	f814 2008 	ldrb.w	r2, [r4, r8]
 800d9be:	eb04 0308 	add.w	r3, r4, r8
 800d9c2:	06d2      	lsls	r2, r2, #27
 800d9c4:	d53c      	bpl.n	800da40 <Show_UART_AnalyzeData+0xf8>
 800d9c6:	7859      	ldrb	r1, [r3, #1]
 800d9c8:	f011 0110 	ands.w	r1, r1, #16
 800d9cc:	d138      	bne.n	800da40 <Show_UART_AnalyzeData+0xf8>
 800d9ce:	45a3      	cmp	fp, r4
 800d9d0:	d236      	bcs.n	800da40 <Show_UART_AnalyzeData+0xf8>
 800d9d2:	f1b9 0f00 	cmp.w	r9, #0
 800d9d6:	d011      	beq.n	800d9fc <Show_UART_AnalyzeData+0xb4>
 800d9d8:	4650      	mov	r0, sl
 800d9da:	9b05      	ldr	r3, [sp, #20]
 800d9dc:	f1c5 0c00 	rsb	ip, r5, #0
 800d9e0:	eba8 0203 	sub.w	r2, r8, r3
 800d9e4:	4422      	add	r2, r4
 800d9e6:	442a      	add	r2, r5
 800d9e8:	b188      	cbz	r0, 800da0e <Show_UART_AnalyzeData+0xc6>
 800d9ea:	f812 300c 	ldrb.w	r3, [r2, ip]
 800d9ee:	f003 0301 	and.w	r3, r3, #1
 800d9f2:	4083      	lsls	r3, r0
 800d9f4:	4319      	orrs	r1, r3
 800d9f6:	b2c9      	uxtb	r1, r1
 800d9f8:	3801      	subs	r0, #1
 800d9fa:	e7f4      	b.n	800d9e6 <Show_UART_AnalyzeData+0x9e>
 800d9fc:	4649      	mov	r1, r9
 800d9fe:	464a      	mov	r2, r9
 800da00:	9b07      	ldr	r3, [sp, #28]
 800da02:	1918      	adds	r0, r3, r4
 800da04:	b2d3      	uxtb	r3, r2
 800da06:	459a      	cmp	sl, r3
 800da08:	eba0 0005 	sub.w	r0, r0, r5
 800da0c:	d820      	bhi.n	800da50 <Show_UART_AnalyzeData+0x108>
 800da0e:	2f01      	cmp	r7, #1
 800da10:	d02d      	beq.n	800da6e <Show_UART_AnalyzeData+0x126>
 800da12:	d328      	bcc.n	800da66 <Show_UART_AnalyzeData+0x11e>
 800da14:	2f02      	cmp	r7, #2
 800da16:	d02e      	beq.n	800da76 <Show_UART_AnalyzeData+0x12e>
 800da18:	9b06      	ldr	r3, [sp, #24]
 800da1a:	4433      	add	r3, r6
 800da1c:	42a3      	cmp	r3, r4
 800da1e:	da0f      	bge.n	800da40 <Show_UART_AnalyzeData+0xf8>
 800da20:	2200      	movs	r2, #0
 800da22:	9b04      	ldr	r3, [sp, #16]
 800da24:	f1c6 0028 	rsb	r0, r6, #40	; 0x28
 800da28:	1ac0      	subs	r0, r0, r3
 800da2a:	4420      	add	r0, r4
 800da2c:	ab09      	add	r3, sp, #36	; 0x24
 800da2e:	9302      	str	r3, [sp, #8]
 800da30:	e9cd 2200 	strd	r2, r2, [sp]
 800da34:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800da38:	2166      	movs	r1, #102	; 0x66
 800da3a:	b280      	uxth	r0, r0
 800da3c:	f7fe fa28 	bl	800be90 <Disp_Str7x9>
 800da40:	f240 1317 	movw	r3, #279	; 0x117
 800da44:	3401      	adds	r4, #1
 800da46:	429c      	cmp	r4, r3
 800da48:	d1b7      	bne.n	800d9ba <Show_UART_AnalyzeData+0x72>
 800da4a:	b00d      	add	sp, #52	; 0x34
 800da4c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800da50:	5d43      	ldrb	r3, [r0, r5]
 800da52:	f1c2 0c07 	rsb	ip, r2, #7
 800da56:	f003 0301 	and.w	r3, r3, #1
 800da5a:	fa03 f30c 	lsl.w	r3, r3, ip
 800da5e:	4319      	orrs	r1, r3
 800da60:	b2c9      	uxtb	r1, r1
 800da62:	3201      	adds	r2, #1
 800da64:	e7ce      	b.n	800da04 <Show_UART_AnalyzeData+0xbc>
 800da66:	a809      	add	r0, sp, #36	; 0x24
 800da68:	f000 fb2a 	bl	800e0c0 <Char2Str>
 800da6c:	e7d4      	b.n	800da18 <Show_UART_AnalyzeData+0xd0>
 800da6e:	a809      	add	r0, sp, #36	; 0x24
 800da70:	f000 fbfb 	bl	800e26a <u8ToDec3Str>
 800da74:	e7d0      	b.n	800da18 <Show_UART_AnalyzeData+0xd0>
 800da76:	a809      	add	r0, sp, #36	; 0x24
 800da78:	f000 fc63 	bl	800e342 <Char2HexFullStr>
 800da7c:	e7cc      	b.n	800da18 <Show_UART_AnalyzeData+0xd0>
 800da7e:	bf00      	nop
 800da80:	20000098 	.word	0x20000098
 800da84:	08014648 	.word	0x08014648
 800da88:	000f4240 	.word	0x000f4240
 800da8c:	2000000c 	.word	0x2000000c
 800da90:	08013268 	.word	0x08013268
 800da94:	08014648 	.word	0x08014648
 800da98:	20000af3 	.word	0x20000af3

0800da9c <SPI_AnalyzeDataStartCnt>:
 800da9c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800daa0:	2400      	movs	r4, #0
 800daa2:	4b2f      	ldr	r3, [pc, #188]	; (800db60 <SPI_AnalyzeDataStartCnt+0xc4>)
 800daa4:	b087      	sub	sp, #28
 800daa6:	f893 9008 	ldrb.w	r9, [r3, #8]
 800daaa:	f8df a0b8 	ldr.w	sl, [pc, #184]	; 800db64 <SPI_AnalyzeDataStartCnt+0xc8>
 800daae:	e9cd 0100 	strd	r0, r1, [sp]
 800dab2:	f44f 62e0 	mov.w	r2, #1792	; 0x700
 800dab6:	2100      	movs	r1, #0
 800dab8:	482a      	ldr	r0, [pc, #168]	; (800db64 <SPI_AnalyzeDataStartCnt+0xc8>)
 800daba:	f005 f83e 	bl	8012b3a <memset>
 800dabe:	46cb      	mov	fp, r9
 800dac0:	260a      	movs	r6, #10
 800dac2:	4625      	mov	r5, r4
 800dac4:	4627      	mov	r7, r4
 800dac6:	46a0      	mov	r8, r4
 800dac8:	4651      	mov	r1, sl
 800daca:	4630      	mov	r0, r6
 800dacc:	9105      	str	r1, [sp, #20]
 800dace:	f7fe fce7 	bl	800c4a0 <TransStatusData>
 800dad2:	3601      	adds	r6, #1
 800dad4:	b2b6      	uxth	r6, r6
 800dad6:	f3c0 02c0 	ubfx	r2, r0, #3, #1
 800dada:	f3c0 0380 	ubfx	r3, r0, #2, #1
 800dade:	4630      	mov	r0, r6
 800dae0:	9204      	str	r2, [sp, #16]
 800dae2:	9303      	str	r3, [sp, #12]
 800dae4:	f7fe fcea 	bl	800c4bc <TransformTime>
 800dae8:	f85a 3024 	ldr.w	r3, [sl, r4, lsl #2]
 800daec:	9a04      	ldr	r2, [sp, #16]
 800daee:	4418      	add	r0, r3
 800daf0:	4591      	cmp	r9, r2
 800daf2:	f84a 0024 	str.w	r0, [sl, r4, lsl #2]
 800daf6:	9b03      	ldr	r3, [sp, #12]
 800daf8:	9905      	ldr	r1, [sp, #20]
 800dafa:	d002      	beq.n	800db02 <SPI_AnalyzeDataStartCnt+0x66>
 800dafc:	f1b8 0f00 	cmp.w	r8, #0
 800db00:	d02b      	beq.n	800db5a <SPI_AnalyzeDataStartCnt+0xbe>
 800db02:	455b      	cmp	r3, fp
 800db04:	bf1c      	itt	ne
 800db06:	3501      	addne	r5, #1
 800db08:	b2ed      	uxtbne	r5, r5
 800db0a:	bb0f      	cbnz	r7, 800db50 <SPI_AnalyzeDataStartCnt+0xb4>
 800db0c:	4591      	cmp	r9, r2
 800db0e:	d01f      	beq.n	800db50 <SPI_AnalyzeDataStartCnt+0xb4>
 800db10:	2500      	movs	r5, #0
 800db12:	2701      	movs	r7, #1
 800db14:	46a8      	mov	r8, r5
 800db16:	3401      	adds	r4, #1
 800db18:	b2a4      	uxth	r4, r4
 800db1a:	f5b6 6f60 	cmp.w	r6, #3584	; 0xe00
 800db1e:	469b      	mov	fp, r3
 800db20:	d1d3      	bne.n	800daca <SPI_AnalyzeDataStartCnt+0x2e>
 800db22:	2300      	movs	r3, #0
 800db24:	2400      	movs	r4, #0
 800db26:	2500      	movs	r5, #0
 800db28:	e9dd 6700 	ldrd	r6, r7, [sp]
 800db2c:	f851 2023 	ldr.w	r2, [r1, r3, lsl #2]
 800db30:	b298      	uxth	r0, r3
 800db32:	18a4      	adds	r4, r4, r2
 800db34:	f145 0500 	adc.w	r5, r5, #0
 800db38:	42bd      	cmp	r5, r7
 800db3a:	bf08      	it	eq
 800db3c:	42b4      	cmpeq	r4, r6
 800db3e:	d204      	bcs.n	800db4a <SPI_AnalyzeDataStartCnt+0xae>
 800db40:	3301      	adds	r3, #1
 800db42:	f5b3 6fe0 	cmp.w	r3, #1792	; 0x700
 800db46:	d1ef      	bne.n	800db28 <SPI_AnalyzeDataStartCnt+0x8c>
 800db48:	2000      	movs	r0, #0
 800db4a:	b007      	add	sp, #28
 800db4c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800db50:	2d0f      	cmp	r5, #15
 800db52:	d8dd      	bhi.n	800db10 <SPI_AnalyzeDataStartCnt+0x74>
 800db54:	f04f 0801 	mov.w	r8, #1
 800db58:	e7df      	b.n	800db1a <SPI_AnalyzeDataStartCnt+0x7e>
 800db5a:	2f00      	cmp	r7, #0
 800db5c:	d0d8      	beq.n	800db10 <SPI_AnalyzeDataStartCnt+0x74>
 800db5e:	e7dc      	b.n	800db1a <SPI_AnalyzeDataStartCnt+0x7e>
 800db60:	20000098 	.word	0x20000098
 800db64:	20004c54 	.word	0x20004c54

0800db68 <Show_SPI_AnalyzeData>:
 800db68:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800db6c:	4b41      	ldr	r3, [pc, #260]	; (800dc74 <Show_SPI_AnalyzeData+0x10c>)
 800db6e:	b08d      	sub	sp, #52	; 0x34
 800db70:	f893 900d 	ldrb.w	r9, [r3, #13]
 800db74:	4b40      	ldr	r3, [pc, #256]	; (800dc78 <Show_SPI_AnalyzeData+0x110>)
 800db76:	e9d3 0100 	ldrd	r0, r1, [r3]
 800db7a:	f7ff ff8f 	bl	800da9c <SPI_AnalyzeDataStartCnt>
 800db7e:	f1b9 0f02 	cmp.w	r9, #2
 800db82:	4604      	mov	r4, r0
 800db84:	d874      	bhi.n	800dc70 <Show_SPI_AnalyzeData+0x108>
 800db86:	4b3d      	ldr	r3, [pc, #244]	; (800dc7c <Show_SPI_AnalyzeData+0x114>)
 800db88:	f813 3009 	ldrb.w	r3, [r3, r9]
 800db8c:	2500      	movs	r5, #0
 800db8e:	46ab      	mov	fp, r5
 800db90:	085b      	lsrs	r3, r3, #1
 800db92:	4e3b      	ldr	r6, [pc, #236]	; (800dc80 <Show_SPI_AnalyzeData+0x118>)
 800db94:	9305      	str	r3, [sp, #20]
 800db96:	f10d 0818 	add.w	r8, sp, #24
 800db9a:	4a3a      	ldr	r2, [pc, #232]	; (800dc84 <Show_SPI_AnalyzeData+0x11c>)
 800db9c:	b2af      	uxth	r7, r5
 800db9e:	18ab      	adds	r3, r5, r2
 800dba0:	5caa      	ldrb	r2, [r5, r2]
 800dba2:	785b      	ldrb	r3, [r3, #1]
 800dba4:	06d1      	lsls	r1, r2, #27
 800dba6:	d40c      	bmi.n	800dbc2 <Show_SPI_AnalyzeData+0x5a>
 800dba8:	06da      	lsls	r2, r3, #27
 800dbaa:	d502      	bpl.n	800dbb2 <Show_SPI_AnalyzeData+0x4a>
 800dbac:	3703      	adds	r7, #3
 800dbae:	fa1f fb87 	uxth.w	fp, r7
 800dbb2:	f240 1317 	movw	r3, #279	; 0x117
 800dbb6:	3501      	adds	r5, #1
 800dbb8:	429d      	cmp	r5, r3
 800dbba:	d1ee      	bne.n	800db9a <Show_SPI_AnalyzeData+0x32>
 800dbbc:	b00d      	add	sp, #52	; 0x34
 800dbbe:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800dbc2:	06db      	lsls	r3, r3, #27
 800dbc4:	d4f5      	bmi.n	800dbb2 <Show_SPI_AnalyzeData+0x4a>
 800dbc6:	eba7 070b 	sub.w	r7, r7, fp
 800dbca:	f1b9 0f01 	cmp.w	r9, #1
 800dbce:	b2bf      	uxth	r7, r7
 800dbd0:	d032      	beq.n	800dc38 <Show_SPI_AnalyzeData+0xd0>
 800dbd2:	d323      	bcc.n	800dc1c <Show_SPI_AnalyzeData+0xb4>
 800dbd4:	f1b9 0f02 	cmp.w	r9, #2
 800dbd8:	d03c      	beq.n	800dc54 <Show_SPI_AnalyzeData+0xec>
 800dbda:	9b05      	ldr	r3, [sp, #20]
 800dbdc:	42bb      	cmp	r3, r7
 800dbde:	d21a      	bcs.n	800dc16 <Show_SPI_AnalyzeData+0xae>
 800dbe0:	2700      	movs	r7, #0
 800dbe2:	f10b 0a28 	add.w	sl, fp, #40	; 0x28
 800dbe6:	fa1f fa8a 	uxth.w	sl, sl
 800dbea:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800dbee:	463a      	mov	r2, r7
 800dbf0:	2166      	movs	r1, #102	; 0x66
 800dbf2:	4650      	mov	r0, sl
 800dbf4:	f8cd 8008 	str.w	r8, [sp, #8]
 800dbf8:	e9cd 7700 	strd	r7, r7, [sp]
 800dbfc:	f7fe f948 	bl	800be90 <Disp_Str7x9>
 800dc00:	ab09      	add	r3, sp, #36	; 0x24
 800dc02:	e9cd 7301 	strd	r7, r3, [sp, #4]
 800dc06:	9700      	str	r7, [sp, #0]
 800dc08:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800dc0c:	463a      	mov	r2, r7
 800dc0e:	214d      	movs	r1, #77	; 0x4d
 800dc10:	4650      	mov	r0, sl
 800dc12:	f7fe f93d 	bl	800be90 <Disp_Str7x9>
 800dc16:	3401      	adds	r4, #1
 800dc18:	b2a4      	uxth	r4, r4
 800dc1a:	e7ca      	b.n	800dbb2 <Show_SPI_AnalyzeData+0x4a>
 800dc1c:	eb04 0a44 	add.w	sl, r4, r4, lsl #1
 800dc20:	f816 100a 	ldrb.w	r1, [r6, sl]
 800dc24:	4640      	mov	r0, r8
 800dc26:	44b2      	add	sl, r6
 800dc28:	f000 fa4a 	bl	800e0c0 <Char2Str>
 800dc2c:	f89a 1001 	ldrb.w	r1, [sl, #1]
 800dc30:	a809      	add	r0, sp, #36	; 0x24
 800dc32:	f000 fa45 	bl	800e0c0 <Char2Str>
 800dc36:	e7d0      	b.n	800dbda <Show_SPI_AnalyzeData+0x72>
 800dc38:	eb04 0a44 	add.w	sl, r4, r4, lsl #1
 800dc3c:	f816 100a 	ldrb.w	r1, [r6, sl]
 800dc40:	4640      	mov	r0, r8
 800dc42:	44b2      	add	sl, r6
 800dc44:	f000 fb11 	bl	800e26a <u8ToDec3Str>
 800dc48:	f89a 1001 	ldrb.w	r1, [sl, #1]
 800dc4c:	a809      	add	r0, sp, #36	; 0x24
 800dc4e:	f000 fb0c 	bl	800e26a <u8ToDec3Str>
 800dc52:	e7c2      	b.n	800dbda <Show_SPI_AnalyzeData+0x72>
 800dc54:	eb04 0a44 	add.w	sl, r4, r4, lsl #1
 800dc58:	f816 100a 	ldrb.w	r1, [r6, sl]
 800dc5c:	4640      	mov	r0, r8
 800dc5e:	44b2      	add	sl, r6
 800dc60:	f000 fb6f 	bl	800e342 <Char2HexFullStr>
 800dc64:	f89a 1001 	ldrb.w	r1, [sl, #1]
 800dc68:	a809      	add	r0, sp, #36	; 0x24
 800dc6a:	f000 fb6a 	bl	800e342 <Char2HexFullStr>
 800dc6e:	e7b4      	b.n	800dbda <Show_SPI_AnalyzeData+0x72>
 800dc70:	2300      	movs	r3, #0
 800dc72:	e78b      	b.n	800db8c <Show_SPI_AnalyzeData+0x24>
 800dc74:	20000098 	.word	0x20000098
 800dc78:	20004c18 	.word	0x20004c18
 800dc7c:	08014648 	.word	0x08014648
 800dc80:	20006854 	.word	0x20006854
 800dc84:	20000af3 	.word	0x20000af3

0800dc88 <AnalyzeFrame>:
 800dc88:	2802      	cmp	r0, #2
 800dc8a:	d005      	beq.n	800dc98 <AnalyzeFrame+0x10>
 800dc8c:	2803      	cmp	r0, #3
 800dc8e:	d005      	beq.n	800dc9c <AnalyzeFrame+0x14>
 800dc90:	2801      	cmp	r0, #1
 800dc92:	d105      	bne.n	800dca0 <AnalyzeFrame+0x18>
 800dc94:	f7ff bb1c 	b.w	800d2d0 <SPI_AnalyzeFrame>
 800dc98:	f7ff bbe6 	b.w	800d468 <IIC_AnalyzeFrame>
 800dc9c:	f7ff bdce 	b.w	800d83c <UART_AnalyzeFrame>
 800dca0:	4770      	bx	lr

0800dca2 <Show_AnalyzeData>:
 800dca2:	2802      	cmp	r0, #2
 800dca4:	d005      	beq.n	800dcb2 <Show_AnalyzeData+0x10>
 800dca6:	2803      	cmp	r0, #3
 800dca8:	d005      	beq.n	800dcb6 <Show_AnalyzeData+0x14>
 800dcaa:	2801      	cmp	r0, #1
 800dcac:	d105      	bne.n	800dcba <Show_AnalyzeData+0x18>
 800dcae:	f7ff bf5b 	b.w	800db68 <Show_SPI_AnalyzeData>
 800dcb2:	f7ff bd11 	b.w	800d6d8 <Show_I2C_AnalyzeData>
 800dcb6:	f7ff be47 	b.w	800d948 <Show_UART_AnalyzeData>
 800dcba:	4770      	bx	lr

0800dcbc <Power>:
 800dcbc:	b141      	cbz	r1, 800dcd0 <Power+0x14>
 800dcbe:	4603      	mov	r3, r0
 800dcc0:	3901      	subs	r1, #1
 800dcc2:	f011 01ff 	ands.w	r1, r1, #255	; 0xff
 800dcc6:	d101      	bne.n	800dccc <Power+0x10>
 800dcc8:	4618      	mov	r0, r3
 800dcca:	4770      	bx	lr
 800dccc:	4343      	muls	r3, r0
 800dcce:	e7f7      	b.n	800dcc0 <Power+0x4>
 800dcd0:	2301      	movs	r3, #1
 800dcd2:	e7f9      	b.n	800dcc8 <Power+0xc>

0800dcd4 <Exp>:
 800dcd4:	4603      	mov	r3, r0
 800dcd6:	220a      	movs	r2, #10
 800dcd8:	2001      	movs	r0, #1
 800dcda:	3b01      	subs	r3, #1
 800dcdc:	b2db      	uxtb	r3, r3
 800dcde:	2bff      	cmp	r3, #255	; 0xff
 800dce0:	d100      	bne.n	800dce4 <Exp+0x10>
 800dce2:	4770      	bx	lr
 800dce4:	4350      	muls	r0, r2
 800dce6:	e7f8      	b.n	800dcda <Exp+0x6>

0800dce8 <SeekStr>:
 800dce8:	3901      	subs	r1, #1
 800dcea:	b2c9      	uxtb	r1, r1
 800dcec:	29ff      	cmp	r1, #255	; 0xff
 800dcee:	d100      	bne.n	800dcf2 <SeekStr+0xa>
 800dcf0:	4770      	bx	lr
 800dcf2:	f810 3b01 	ldrb.w	r3, [r0], #1
 800dcf6:	2b00      	cmp	r3, #0
 800dcf8:	d1fb      	bne.n	800dcf2 <SeekStr+0xa>
 800dcfa:	e7f5      	b.n	800dce8 <SeekStr>

0800dcfc <Value2Str>:
 800dcfc:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800dd00:	461d      	mov	r5, r3
 800dd02:	f89d 3028 	ldrb.w	r3, [sp, #40]	; 0x28
 800dd06:	4604      	mov	r4, r0
 800dd08:	4692      	mov	sl, r2
 800dd0a:	2b00      	cmp	r3, #0
 800dd0c:	d147      	bne.n	800dd9e <Value2Str+0xa2>
 800dd0e:	2900      	cmp	r1, #0
 800dd10:	da45      	bge.n	800dd9e <Value2Str+0xa2>
 800dd12:	232d      	movs	r3, #45	; 0x2d
 800dd14:	424e      	negs	r6, r1
 800dd16:	7003      	strb	r3, [r0, #0]
 800dd18:	3401      	adds	r4, #1
 800dd1a:	2200      	movs	r2, #0
 800dd1c:	200a      	movs	r0, #10
 800dd1e:	e001      	b.n	800dd24 <Value2Str+0x28>
 800dd20:	fb91 f1f0 	sdiv	r1, r1, r0
 800dd24:	2909      	cmp	r1, #9
 800dd26:	b213      	sxth	r3, r2
 800dd28:	f102 0201 	add.w	r2, r2, #1
 800dd2c:	dcf8      	bgt.n	800dd20 <Value2Str+0x24>
 800dd2e:	2203      	movs	r2, #3
 800dd30:	fb93 f2f2 	sdiv	r2, r3, r2
 800dd34:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 800dd38:	1a9b      	subs	r3, r3, r2
 800dd3a:	b21b      	sxth	r3, r3
 800dd3c:	2b02      	cmp	r3, #2
 800dd3e:	d102      	bne.n	800dd46 <Value2Str+0x4a>
 800dd40:	2d02      	cmp	r5, #2
 800dd42:	bf08      	it	eq
 800dd44:	2503      	moveq	r5, #3
 800dd46:	4631      	mov	r1, r6
 800dd48:	2205      	movs	r2, #5
 800dd4a:	2300      	movs	r3, #0
 800dd4c:	270a      	movs	r7, #10
 800dd4e:	b228      	sxth	r0, r5
 800dd50:	2909      	cmp	r1, #9
 800dd52:	dc26      	bgt.n	800dda2 <Value2Str+0xa6>
 800dd54:	4283      	cmp	r3, r0
 800dd56:	bfa8      	it	ge
 800dd58:	18b6      	addge	r6, r6, r2
 800dd5a:	4633      	mov	r3, r6
 800dd5c:	2200      	movs	r2, #0
 800dd5e:	200a      	movs	r0, #10
 800dd60:	2b09      	cmp	r3, #9
 800dd62:	b211      	sxth	r1, r2
 800dd64:	f102 0201 	add.w	r2, r2, #1
 800dd68:	dc23      	bgt.n	800ddb2 <Value2Str+0xb6>
 800dd6a:	f04f 0803 	mov.w	r8, #3
 800dd6e:	f04f 0b2e 	mov.w	fp, #46	; 0x2e
 800dd72:	fb91 f7f8 	sdiv	r7, r1, r8
 800dd76:	3d01      	subs	r5, #1
 800dd78:	b26d      	sxtb	r5, r5
 800dd7a:	1c6b      	adds	r3, r5, #1
 800dd7c:	f104 0901 	add.w	r9, r4, #1
 800dd80:	d11a      	bne.n	800ddb8 <Value2Str+0xbc>
 800dd82:	2206      	movs	r2, #6
 800dd84:	fb02 a707 	mla	r7, r2, r7, sl
 800dd88:	3c01      	subs	r4, #1
 800dd8a:	3f01      	subs	r7, #1
 800dd8c:	f817 3f01 	ldrb.w	r3, [r7, #1]!
 800dd90:	f804 3f01 	strb.w	r3, [r4, #1]!
 800dd94:	783b      	ldrb	r3, [r7, #0]
 800dd96:	2b00      	cmp	r3, #0
 800dd98:	d1f8      	bne.n	800dd8c <Value2Str+0x90>
 800dd9a:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800dd9e:	460e      	mov	r6, r1
 800dda0:	e7bb      	b.n	800dd1a <Value2Str+0x1e>
 800dda2:	3301      	adds	r3, #1
 800dda4:	b21b      	sxth	r3, r3
 800dda6:	4298      	cmp	r0, r3
 800dda8:	fb91 f1f7 	sdiv	r1, r1, r7
 800ddac:	bfb8      	it	lt
 800ddae:	437a      	mullt	r2, r7
 800ddb0:	e7ce      	b.n	800dd50 <Value2Str+0x54>
 800ddb2:	fb93 f3f0 	sdiv	r3, r3, r0
 800ddb6:	e7d3      	b.n	800dd60 <Value2Str+0x64>
 800ddb8:	b2c8      	uxtb	r0, r1
 800ddba:	f7ff ff8b 	bl	800dcd4 <Exp>
 800ddbe:	fbb6 f2f0 	udiv	r2, r6, r0
 800ddc2:	f102 0330 	add.w	r3, r2, #48	; 0x30
 800ddc6:	7023      	strb	r3, [r4, #0]
 800ddc8:	b155      	cbz	r5, 800dde0 <Value2Str+0xe4>
 800ddca:	fb91 f3f8 	sdiv	r3, r1, r8
 800ddce:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 800ddd2:	1acb      	subs	r3, r1, r3
 800ddd4:	b21b      	sxth	r3, r3
 800ddd6:	b91b      	cbnz	r3, 800dde0 <Value2Str+0xe4>
 800ddd8:	f104 0902 	add.w	r9, r4, #2
 800dddc:	f884 b001 	strb.w	fp, [r4, #1]
 800dde0:	2900      	cmp	r1, #0
 800dde2:	f101 31ff 	add.w	r1, r1, #4294967295	; 0xffffffff
 800dde6:	bfac      	ite	ge
 800dde8:	fb00 6612 	mlsge	r6, r0, r2, r6
 800ddec:	2600      	movlt	r6, #0
 800ddee:	b209      	sxth	r1, r1
 800ddf0:	464c      	mov	r4, r9
 800ddf2:	e7c0      	b.n	800dd76 <Value2Str+0x7a>

0800ddf4 <u32ToStr_Unit>:
 800ddf4:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800ddf8:	4605      	mov	r5, r0
 800ddfa:	4692      	mov	sl, r2
 800ddfc:	461c      	mov	r4, r3
 800ddfe:	460a      	mov	r2, r1
 800de00:	2000      	movs	r0, #0
 800de02:	260a      	movs	r6, #10
 800de04:	2a09      	cmp	r2, #9
 800de06:	b203      	sxth	r3, r0
 800de08:	f100 0001 	add.w	r0, r0, #1
 800de0c:	d837      	bhi.n	800de7e <u32ToStr_Unit+0x8a>
 800de0e:	2203      	movs	r2, #3
 800de10:	fb93 f2f2 	sdiv	r2, r3, r2
 800de14:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 800de18:	1a9b      	subs	r3, r3, r2
 800de1a:	b21b      	sxth	r3, r3
 800de1c:	2b02      	cmp	r3, #2
 800de1e:	d102      	bne.n	800de26 <u32ToStr_Unit+0x32>
 800de20:	2c02      	cmp	r4, #2
 800de22:	bf08      	it	eq
 800de24:	2403      	moveq	r4, #3
 800de26:	4608      	mov	r0, r1
 800de28:	2205      	movs	r2, #5
 800de2a:	2300      	movs	r3, #0
 800de2c:	270a      	movs	r7, #10
 800de2e:	b226      	sxth	r6, r4
 800de30:	2809      	cmp	r0, #9
 800de32:	d827      	bhi.n	800de84 <u32ToStr_Unit+0x90>
 800de34:	42b3      	cmp	r3, r6
 800de36:	bfa8      	it	ge
 800de38:	1889      	addge	r1, r1, r2
 800de3a:	460b      	mov	r3, r1
 800de3c:	2200      	movs	r2, #0
 800de3e:	200a      	movs	r0, #10
 800de40:	2b09      	cmp	r3, #9
 800de42:	b216      	sxth	r6, r2
 800de44:	f102 0201 	add.w	r2, r2, #1
 800de48:	d824      	bhi.n	800de94 <u32ToStr_Unit+0xa0>
 800de4a:	f04f 0803 	mov.w	r8, #3
 800de4e:	f04f 0b2e 	mov.w	fp, #46	; 0x2e
 800de52:	fb96 f7f8 	sdiv	r7, r6, r8
 800de56:	3c01      	subs	r4, #1
 800de58:	b264      	sxtb	r4, r4
 800de5a:	1c63      	adds	r3, r4, #1
 800de5c:	f105 0901 	add.w	r9, r5, #1
 800de60:	d11b      	bne.n	800de9a <u32ToStr_Unit+0xa6>
 800de62:	2206      	movs	r2, #6
 800de64:	fb02 a707 	mla	r7, r2, r7, sl
 800de68:	3d01      	subs	r5, #1
 800de6a:	3f01      	subs	r7, #1
 800de6c:	f817 3f01 	ldrb.w	r3, [r7, #1]!
 800de70:	f805 3f01 	strb.w	r3, [r5, #1]!
 800de74:	783b      	ldrb	r3, [r7, #0]
 800de76:	2b00      	cmp	r3, #0
 800de78:	d1f8      	bne.n	800de6c <u32ToStr_Unit+0x78>
 800de7a:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800de7e:	fbb2 f2f6 	udiv	r2, r2, r6
 800de82:	e7bf      	b.n	800de04 <u32ToStr_Unit+0x10>
 800de84:	3301      	adds	r3, #1
 800de86:	b21b      	sxth	r3, r3
 800de88:	429e      	cmp	r6, r3
 800de8a:	fbb0 f0f7 	udiv	r0, r0, r7
 800de8e:	bfb8      	it	lt
 800de90:	437a      	mullt	r2, r7
 800de92:	e7cd      	b.n	800de30 <u32ToStr_Unit+0x3c>
 800de94:	fbb3 f3f0 	udiv	r3, r3, r0
 800de98:	e7d2      	b.n	800de40 <u32ToStr_Unit+0x4c>
 800de9a:	b2f0      	uxtb	r0, r6
 800de9c:	f7ff ff1a 	bl	800dcd4 <Exp>
 800dea0:	fbb1 f2f0 	udiv	r2, r1, r0
 800dea4:	f102 0330 	add.w	r3, r2, #48	; 0x30
 800dea8:	702b      	strb	r3, [r5, #0]
 800deaa:	b154      	cbz	r4, 800dec2 <u32ToStr_Unit+0xce>
 800deac:	fb96 f3f8 	sdiv	r3, r6, r8
 800deb0:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 800deb4:	1af3      	subs	r3, r6, r3
 800deb6:	b21b      	sxth	r3, r3
 800deb8:	b91b      	cbnz	r3, 800dec2 <u32ToStr_Unit+0xce>
 800deba:	f105 0902 	add.w	r9, r5, #2
 800debe:	f885 b001 	strb.w	fp, [r5, #1]
 800dec2:	2e00      	cmp	r6, #0
 800dec4:	f106 36ff 	add.w	r6, r6, #4294967295	; 0xffffffff
 800dec8:	bfac      	ite	ge
 800deca:	fb00 1112 	mlsge	r1, r0, r2, r1
 800dece:	2100      	movlt	r1, #0
 800ded0:	b236      	sxth	r6, r6
 800ded2:	464d      	mov	r5, r9
 800ded4:	e7bf      	b.n	800de56 <u32ToStr_Unit+0x62>

0800ded6 <u64ToStr_Unit>:
 800ded6:	e92d 4ff7 	stmdb	sp!, {r0, r1, r2, r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800deda:	4699      	mov	r9, r3
 800dedc:	f89d 3038 	ldrb.w	r3, [sp, #56]	; 0x38
 800dee0:	4604      	mov	r4, r0
 800dee2:	2b02      	cmp	r3, #2
 800dee4:	4690      	mov	r8, r2
 800dee6:	f99d 5034 	ldrsb.w	r5, [sp, #52]	; 0x34
 800deea:	4611      	mov	r1, r2
 800deec:	d102      	bne.n	800def4 <u64ToStr_Unit+0x1e>
 800deee:	232d      	movs	r3, #45	; 0x2d
 800def0:	3401      	adds	r4, #1
 800def2:	7003      	strb	r3, [r0, #0]
 800def4:	460a      	mov	r2, r1
 800def6:	2000      	movs	r0, #0
 800def8:	260a      	movs	r6, #10
 800defa:	e001      	b.n	800df00 <u64ToStr_Unit+0x2a>
 800defc:	fbb2 f2f6 	udiv	r2, r2, r6
 800df00:	2a09      	cmp	r2, #9
 800df02:	b203      	sxth	r3, r0
 800df04:	f100 0001 	add.w	r0, r0, #1
 800df08:	d8f8      	bhi.n	800defc <u64ToStr_Unit+0x26>
 800df0a:	2203      	movs	r2, #3
 800df0c:	fb93 f2f2 	sdiv	r2, r3, r2
 800df10:	eb02 0242 	add.w	r2, r2, r2, lsl #1
 800df14:	1a9b      	subs	r3, r3, r2
 800df16:	b21b      	sxth	r3, r3
 800df18:	2b02      	cmp	r3, #2
 800df1a:	d102      	bne.n	800df22 <u64ToStr_Unit+0x4c>
 800df1c:	2d02      	cmp	r5, #2
 800df1e:	bf08      	it	eq
 800df20:	2503      	moveq	r5, #3
 800df22:	2205      	movs	r2, #5
 800df24:	2300      	movs	r3, #0
 800df26:	260a      	movs	r6, #10
 800df28:	b228      	sxth	r0, r5
 800df2a:	2909      	cmp	r1, #9
 800df2c:	d82a      	bhi.n	800df84 <u64ToStr_Unit+0xae>
 800df2e:	4283      	cmp	r3, r0
 800df30:	db03      	blt.n	800df3a <u64ToStr_Unit+0x64>
 800df32:	eb18 0802 	adds.w	r8, r8, r2
 800df36:	f149 0900 	adc.w	r9, r9, #0
 800df3a:	4643      	mov	r3, r8
 800df3c:	2200      	movs	r2, #0
 800df3e:	210a      	movs	r1, #10
 800df40:	2b09      	cmp	r3, #9
 800df42:	b216      	sxth	r6, r2
 800df44:	f102 0201 	add.w	r2, r2, #1
 800df48:	d824      	bhi.n	800df94 <u64ToStr_Unit+0xbe>
 800df4a:	2703      	movs	r7, #3
 800df4c:	fb96 f3f7 	sdiv	r3, r6, r7
 800df50:	f04f 0b2e 	mov.w	fp, #46	; 0x2e
 800df54:	9301      	str	r3, [sp, #4]
 800df56:	3d01      	subs	r5, #1
 800df58:	b26d      	sxtb	r5, r5
 800df5a:	1c6b      	adds	r3, r5, #1
 800df5c:	f104 0a01 	add.w	sl, r4, #1
 800df60:	d11b      	bne.n	800df9a <u64ToStr_Unit+0xc4>
 800df62:	2206      	movs	r2, #6
 800df64:	9b0c      	ldr	r3, [sp, #48]	; 0x30
 800df66:	9901      	ldr	r1, [sp, #4]
 800df68:	3c01      	subs	r4, #1
 800df6a:	fb02 3301 	mla	r3, r2, r1, r3
 800df6e:	3b01      	subs	r3, #1
 800df70:	f813 2f01 	ldrb.w	r2, [r3, #1]!
 800df74:	f804 2f01 	strb.w	r2, [r4, #1]!
 800df78:	781a      	ldrb	r2, [r3, #0]
 800df7a:	2a00      	cmp	r2, #0
 800df7c:	d1f8      	bne.n	800df70 <u64ToStr_Unit+0x9a>
 800df7e:	b003      	add	sp, #12
 800df80:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800df84:	3301      	adds	r3, #1
 800df86:	b21b      	sxth	r3, r3
 800df88:	4298      	cmp	r0, r3
 800df8a:	fbb1 f1f6 	udiv	r1, r1, r6
 800df8e:	bfb8      	it	lt
 800df90:	4372      	mullt	r2, r6
 800df92:	e7ca      	b.n	800df2a <u64ToStr_Unit+0x54>
 800df94:	fbb3 f3f1 	udiv	r3, r3, r1
 800df98:	e7d2      	b.n	800df40 <u64ToStr_Unit+0x6a>
 800df9a:	b2f0      	uxtb	r0, r6
 800df9c:	f7ff fe9a 	bl	800dcd4 <Exp>
 800dfa0:	2300      	movs	r3, #0
 800dfa2:	4602      	mov	r2, r0
 800dfa4:	4649      	mov	r1, r9
 800dfa6:	4640      	mov	r0, r8
 800dfa8:	f004 fc3a 	bl	8012820 <__aeabi_uldivmod>
 800dfac:	3030      	adds	r0, #48	; 0x30
 800dfae:	7020      	strb	r0, [r4, #0]
 800dfb0:	b155      	cbz	r5, 800dfc8 <u64ToStr_Unit+0xf2>
 800dfb2:	fb96 f1f7 	sdiv	r1, r6, r7
 800dfb6:	eb01 0141 	add.w	r1, r1, r1, lsl #1
 800dfba:	1a71      	subs	r1, r6, r1
 800dfbc:	b209      	sxth	r1, r1
 800dfbe:	b919      	cbnz	r1, 800dfc8 <u64ToStr_Unit+0xf2>
 800dfc0:	f104 0a02 	add.w	sl, r4, #2
 800dfc4:	f884 b001 	strb.w	fp, [r4, #1]
 800dfc8:	2e00      	cmp	r6, #0
 800dfca:	f106 36ff 	add.w	r6, r6, #4294967295	; 0xffffffff
 800dfce:	bfa7      	ittee	ge
 800dfd0:	4690      	movge	r8, r2
 800dfd2:	4699      	movge	r9, r3
 800dfd4:	f04f 0800 	movlt.w	r8, #0
 800dfd8:	f04f 0900 	movlt.w	r9, #0
 800dfdc:	b236      	sxth	r6, r6
 800dfde:	4654      	mov	r4, sl
 800dfe0:	e7b9      	b.n	800df56 <u64ToStr_Unit+0x80>

0800dfe2 <ValuedivTen2Str>:
 800dfe2:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800dfe4:	2200      	movs	r2, #0
 800dfe6:	4605      	mov	r5, r0
 800dfe8:	460b      	mov	r3, r1
 800dfea:	200a      	movs	r0, #10
 800dfec:	2b09      	cmp	r3, #9
 800dfee:	b294      	uxth	r4, r2
 800dff0:	f102 0201 	add.w	r2, r2, #1
 800dff4:	d819      	bhi.n	800e02a <ValuedivTen2Str+0x48>
 800dff6:	b1dc      	cbz	r4, 800e030 <ValuedivTen2Str+0x4e>
 800dff8:	262e      	movs	r6, #46	; 0x2e
 800dffa:	b2e0      	uxtb	r0, r4
 800dffc:	f7ff fe6a 	bl	800dcd4 <Exp>
 800e000:	fbb1 f3f0 	udiv	r3, r1, r0
 800e004:	fb00 1013 	mls	r0, r0, r3, r1
 800e008:	2c01      	cmp	r4, #1
 800e00a:	f104 34ff 	add.w	r4, r4, #4294967295	; 0xffffffff
 800e00e:	f103 0730 	add.w	r7, r3, #48	; 0x30
 800e012:	b2a4      	uxth	r4, r4
 800e014:	f105 0201 	add.w	r2, r5, #1
 800e018:	702f      	strb	r7, [r5, #0]
 800e01a:	bf04      	itt	eq
 800e01c:	1caa      	addeq	r2, r5, #2
 800e01e:	706e      	strbeq	r6, [r5, #1]
 800e020:	b281      	uxth	r1, r0
 800e022:	b964      	cbnz	r4, 800e03e <ValuedivTen2Str+0x5c>
 800e024:	3030      	adds	r0, #48	; 0x30
 800e026:	7010      	strb	r0, [r2, #0]
 800e028:	e008      	b.n	800e03c <ValuedivTen2Str+0x5a>
 800e02a:	fbb3 f3f0 	udiv	r3, r3, r0
 800e02e:	e7dd      	b.n	800dfec <ValuedivTen2Str+0xa>
 800e030:	2330      	movs	r3, #48	; 0x30
 800e032:	702b      	strb	r3, [r5, #0]
 800e034:	232e      	movs	r3, #46	; 0x2e
 800e036:	3130      	adds	r1, #48	; 0x30
 800e038:	706b      	strb	r3, [r5, #1]
 800e03a:	70a9      	strb	r1, [r5, #2]
 800e03c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 800e03e:	4615      	mov	r5, r2
 800e040:	e7db      	b.n	800dffa <ValuedivTen2Str+0x18>

0800e042 <Str2Byte>:
 800e042:	b5f0      	push	{r4, r5, r6, r7, lr}
 800e044:	b087      	sub	sp, #28
 800e046:	ad01      	add	r5, sp, #4
 800e048:	4604      	mov	r4, r0
 800e04a:	460a      	mov	r2, r1
 800e04c:	462f      	mov	r7, r5
 800e04e:	4b1b      	ldr	r3, [pc, #108]	; (800e0bc <Str2Byte+0x7a>)
 800e050:	f103 0c10 	add.w	ip, r3, #16
 800e054:	462e      	mov	r6, r5
 800e056:	6818      	ldr	r0, [r3, #0]
 800e058:	6859      	ldr	r1, [r3, #4]
 800e05a:	3308      	adds	r3, #8
 800e05c:	c603      	stmia	r6!, {r0, r1}
 800e05e:	4563      	cmp	r3, ip
 800e060:	4635      	mov	r5, r6
 800e062:	d1f7      	bne.n	800e054 <Str2Byte+0x12>
 800e064:	781b      	ldrb	r3, [r3, #0]
 800e066:	7033      	strb	r3, [r6, #0]
 800e068:	f1a4 0361 	sub.w	r3, r4, #97	; 0x61
 800e06c:	2b19      	cmp	r3, #25
 800e06e:	bf98      	it	ls
 800e070:	3c20      	subls	r4, #32
 800e072:	f1a2 0361 	sub.w	r3, r2, #97	; 0x61
 800e076:	bf98      	it	ls
 800e078:	b2e4      	uxtbls	r4, r4
 800e07a:	2b19      	cmp	r3, #25
 800e07c:	f04f 0300 	mov.w	r3, #0
 800e080:	4618      	mov	r0, r3
 800e082:	bf9c      	itt	ls
 800e084:	3a20      	subls	r2, #32
 800e086:	b2d2      	uxtbls	r2, r2
 800e088:	5cf9      	ldrb	r1, [r7, r3]
 800e08a:	42a1      	cmp	r1, r4
 800e08c:	bf08      	it	eq
 800e08e:	eb00 1003 	addeq.w	r0, r0, r3, lsl #4
 800e092:	f103 0301 	add.w	r3, r3, #1
 800e096:	bf08      	it	eq
 800e098:	b2c0      	uxtbeq	r0, r0
 800e09a:	2b10      	cmp	r3, #16
 800e09c:	d1f4      	bne.n	800e088 <Str2Byte+0x46>
 800e09e:	2300      	movs	r3, #0
 800e0a0:	5cfc      	ldrb	r4, [r7, r3]
 800e0a2:	b2d9      	uxtb	r1, r3
 800e0a4:	4294      	cmp	r4, r2
 800e0a6:	bf08      	it	eq
 800e0a8:	1840      	addeq	r0, r0, r1
 800e0aa:	f103 0301 	add.w	r3, r3, #1
 800e0ae:	bf08      	it	eq
 800e0b0:	b2c0      	uxtbeq	r0, r0
 800e0b2:	2b10      	cmp	r3, #16
 800e0b4:	d1f4      	bne.n	800e0a0 <Str2Byte+0x5e>
 800e0b6:	b007      	add	sp, #28
 800e0b8:	bdf0      	pop	{r4, r5, r6, r7, pc}
 800e0ba:	bf00      	nop
 800e0bc:	08014670 	.word	0x08014670

0800e0c0 <Char2Str>:
 800e0c0:	2300      	movs	r3, #0
 800e0c2:	7001      	strb	r1, [r0, #0]
 800e0c4:	7043      	strb	r3, [r0, #1]
 800e0c6:	4770      	bx	lr

0800e0c8 <u16ToDec4Str>:
 800e0c8:	f242 730f 	movw	r3, #9999	; 0x270f
 800e0cc:	4299      	cmp	r1, r3
 800e0ce:	b570      	push	{r4, r5, r6, lr}
 800e0d0:	d90a      	bls.n	800e0e8 <u16ToDec4Str+0x20>
 800e0d2:	234f      	movs	r3, #79	; 0x4f
 800e0d4:	7003      	strb	r3, [r0, #0]
 800e0d6:	2376      	movs	r3, #118	; 0x76
 800e0d8:	7043      	strb	r3, [r0, #1]
 800e0da:	2365      	movs	r3, #101	; 0x65
 800e0dc:	7083      	strb	r3, [r0, #2]
 800e0de:	2372      	movs	r3, #114	; 0x72
 800e0e0:	70c3      	strb	r3, [r0, #3]
 800e0e2:	2300      	movs	r3, #0
 800e0e4:	7103      	strb	r3, [r0, #4]
 800e0e6:	bd70      	pop	{r4, r5, r6, pc}
 800e0e8:	f44f 727a 	mov.w	r2, #1000	; 0x3e8
 800e0ec:	fbb1 f3f2 	udiv	r3, r1, r2
 800e0f0:	f103 0530 	add.w	r5, r3, #48	; 0x30
 800e0f4:	fb02 1313 	mls	r3, r2, r3, r1
 800e0f8:	2264      	movs	r2, #100	; 0x64
 800e0fa:	b299      	uxth	r1, r3
 800e0fc:	fbb1 f3f2 	udiv	r3, r1, r2
 800e100:	260a      	movs	r6, #10
 800e102:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e106:	fb02 1313 	mls	r3, r2, r3, r1
 800e10a:	b29b      	uxth	r3, r3
 800e10c:	fbb3 f2f6 	udiv	r2, r3, r6
 800e110:	fb06 3312 	mls	r3, r6, r2, r3
 800e114:	3330      	adds	r3, #48	; 0x30
 800e116:	70c3      	strb	r3, [r0, #3]
 800e118:	2300      	movs	r3, #0
 800e11a:	b2ed      	uxtb	r5, r5
 800e11c:	f102 0130 	add.w	r1, r2, #48	; 0x30
 800e120:	b2e4      	uxtb	r4, r4
 800e122:	b2c9      	uxtb	r1, r1
 800e124:	2d30      	cmp	r5, #48	; 0x30
 800e126:	7005      	strb	r5, [r0, #0]
 800e128:	7044      	strb	r4, [r0, #1]
 800e12a:	7081      	strb	r1, [r0, #2]
 800e12c:	7103      	strb	r3, [r0, #4]
 800e12e:	d1da      	bne.n	800e0e6 <u16ToDec4Str+0x1e>
 800e130:	2320      	movs	r3, #32
 800e132:	2c30      	cmp	r4, #48	; 0x30
 800e134:	7003      	strb	r3, [r0, #0]
 800e136:	d1d6      	bne.n	800e0e6 <u16ToDec4Str+0x1e>
 800e138:	2930      	cmp	r1, #48	; 0x30
 800e13a:	7043      	strb	r3, [r0, #1]
 800e13c:	bf08      	it	eq
 800e13e:	7083      	strbeq	r3, [r0, #2]
 800e140:	e7d1      	b.n	800e0e6 <u16ToDec4Str+0x1e>

0800e142 <u16ToDec5Str>:
 800e142:	f242 7210 	movw	r2, #10000	; 0x2710
 800e146:	fbb1 f3f2 	udiv	r3, r1, r2
 800e14a:	b510      	push	{r4, lr}
 800e14c:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e150:	fb02 1313 	mls	r3, r2, r3, r1
 800e154:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
 800e158:	b29a      	uxth	r2, r3
 800e15a:	fbb2 f3f1 	udiv	r3, r2, r1
 800e15e:	7004      	strb	r4, [r0, #0]
 800e160:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e164:	fb01 2313 	mls	r3, r1, r3, r2
 800e168:	2164      	movs	r1, #100	; 0x64
 800e16a:	b29a      	uxth	r2, r3
 800e16c:	fbb2 f3f1 	udiv	r3, r2, r1
 800e170:	7044      	strb	r4, [r0, #1]
 800e172:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e176:	fb01 2313 	mls	r3, r1, r3, r2
 800e17a:	210a      	movs	r1, #10
 800e17c:	b29b      	uxth	r3, r3
 800e17e:	fbb3 f2f1 	udiv	r2, r3, r1
 800e182:	fb01 3312 	mls	r3, r1, r2, r3
 800e186:	3330      	adds	r3, #48	; 0x30
 800e188:	7103      	strb	r3, [r0, #4]
 800e18a:	2300      	movs	r3, #0
 800e18c:	7084      	strb	r4, [r0, #2]
 800e18e:	f102 0430 	add.w	r4, r2, #48	; 0x30
 800e192:	70c4      	strb	r4, [r0, #3]
 800e194:	7143      	strb	r3, [r0, #5]
 800e196:	bd10      	pop	{r4, pc}

0800e198 <u32ToDecStr>:
 800e198:	4a27      	ldr	r2, [pc, #156]	; (800e238 <u32ToDecStr+0xa0>)
 800e19a:	b510      	push	{r4, lr}
 800e19c:	fbb1 f3f2 	udiv	r3, r1, r2
 800e1a0:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e1a4:	fb02 1313 	mls	r3, r2, r3, r1
 800e1a8:	4924      	ldr	r1, [pc, #144]	; (800e23c <u32ToDecStr+0xa4>)
 800e1aa:	7004      	strb	r4, [r0, #0]
 800e1ac:	fbb3 f2f1 	udiv	r2, r3, r1
 800e1b0:	fb01 3312 	mls	r3, r1, r2, r3
 800e1b4:	4922      	ldr	r1, [pc, #136]	; (800e240 <u32ToDecStr+0xa8>)
 800e1b6:	f102 0430 	add.w	r4, r2, #48	; 0x30
 800e1ba:	fbb3 f2f1 	udiv	r2, r3, r1
 800e1be:	7044      	strb	r4, [r0, #1]
 800e1c0:	f102 0430 	add.w	r4, r2, #48	; 0x30
 800e1c4:	fb01 3212 	mls	r2, r1, r2, r3
 800e1c8:	491e      	ldr	r1, [pc, #120]	; (800e244 <u32ToDecStr+0xac>)
 800e1ca:	7084      	strb	r4, [r0, #2]
 800e1cc:	fbb2 f3f1 	udiv	r3, r2, r1
 800e1d0:	fb01 2213 	mls	r2, r1, r3, r2
 800e1d4:	491c      	ldr	r1, [pc, #112]	; (800e248 <u32ToDecStr+0xb0>)
 800e1d6:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e1da:	fbb2 f3f1 	udiv	r3, r2, r1
 800e1de:	70c4      	strb	r4, [r0, #3]
 800e1e0:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e1e4:	fb01 2313 	mls	r3, r1, r3, r2
 800e1e8:	f242 7110 	movw	r1, #10000	; 0x2710
 800e1ec:	fbb3 f2f1 	udiv	r2, r3, r1
 800e1f0:	fb01 3312 	mls	r3, r1, r2, r3
 800e1f4:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
 800e1f8:	7104      	strb	r4, [r0, #4]
 800e1fa:	f102 0430 	add.w	r4, r2, #48	; 0x30
 800e1fe:	fbb3 f2f1 	udiv	r2, r3, r1
 800e202:	7144      	strb	r4, [r0, #5]
 800e204:	f102 0430 	add.w	r4, r2, #48	; 0x30
 800e208:	fb01 3212 	mls	r2, r1, r2, r3
 800e20c:	2164      	movs	r1, #100	; 0x64
 800e20e:	fbb2 f3f1 	udiv	r3, r2, r1
 800e212:	fb01 2213 	mls	r2, r1, r3, r2
 800e216:	210a      	movs	r1, #10
 800e218:	7184      	strb	r4, [r0, #6]
 800e21a:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e21e:	fbb2 f3f1 	udiv	r3, r2, r1
 800e222:	71c4      	strb	r4, [r0, #7]
 800e224:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e228:	fb01 2313 	mls	r3, r1, r3, r2
 800e22c:	3330      	adds	r3, #48	; 0x30
 800e22e:	7243      	strb	r3, [r0, #9]
 800e230:	2300      	movs	r3, #0
 800e232:	7204      	strb	r4, [r0, #8]
 800e234:	7283      	strb	r3, [r0, #10]
 800e236:	bd10      	pop	{r4, pc}
 800e238:	3b9aca00 	.word	0x3b9aca00
 800e23c:	05f5e100 	.word	0x05f5e100
 800e240:	00989680 	.word	0x00989680
 800e244:	000f4240 	.word	0x000f4240
 800e248:	000186a0 	.word	0x000186a0

0800e24c <s16ToDec5Str>:
 800e24c:	2900      	cmp	r1, #0
 800e24e:	4603      	mov	r3, r0
 800e250:	bfaf      	iteee	ge
 800e252:	222b      	movge	r2, #43	; 0x2b
 800e254:	222d      	movlt	r2, #45	; 0x2d
 800e256:	4249      	neglt	r1, r1
 800e258:	b209      	sxthlt	r1, r1
 800e25a:	f100 0001 	add.w	r0, r0, #1
 800e25e:	bfac      	ite	ge
 800e260:	701a      	strbge	r2, [r3, #0]
 800e262:	701a      	strblt	r2, [r3, #0]
 800e264:	b289      	uxth	r1, r1
 800e266:	f7ff bf6c 	b.w	800e142 <u16ToDec5Str>

0800e26a <u8ToDec3Str>:
 800e26a:	2264      	movs	r2, #100	; 0x64
 800e26c:	fbb1 f3f2 	udiv	r3, r1, r2
 800e270:	fb02 1113 	mls	r1, r2, r3, r1
 800e274:	220a      	movs	r2, #10
 800e276:	b2c9      	uxtb	r1, r1
 800e278:	b510      	push	{r4, lr}
 800e27a:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e27e:	fbb1 f3f2 	udiv	r3, r1, r2
 800e282:	7004      	strb	r4, [r0, #0]
 800e284:	fb02 1113 	mls	r1, r2, r3, r1
 800e288:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e28c:	2300      	movs	r3, #0
 800e28e:	3130      	adds	r1, #48	; 0x30
 800e290:	7044      	strb	r4, [r0, #1]
 800e292:	7081      	strb	r1, [r0, #2]
 800e294:	70c3      	strb	r3, [r0, #3]
 800e296:	bd10      	pop	{r4, pc}

0800e298 <s8ToPercen>:
 800e298:	2900      	cmp	r1, #0
 800e29a:	f04f 0264 	mov.w	r2, #100	; 0x64
 800e29e:	bfaf      	iteee	ge
 800e2a0:	232b      	movge	r3, #43	; 0x2b
 800e2a2:	232d      	movlt	r3, #45	; 0x2d
 800e2a4:	4249      	neglt	r1, r1
 800e2a6:	b249      	sxtblt	r1, r1
 800e2a8:	bfac      	ite	ge
 800e2aa:	7003      	strbge	r3, [r0, #0]
 800e2ac:	7003      	strblt	r3, [r0, #0]
 800e2ae:	fb91 f3f2 	sdiv	r3, r1, r2
 800e2b2:	b510      	push	{r4, lr}
 800e2b4:	fb02 1113 	mls	r1, r2, r3, r1
 800e2b8:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e2bc:	220a      	movs	r2, #10
 800e2be:	232e      	movs	r3, #46	; 0x2e
 800e2c0:	b249      	sxtb	r1, r1
 800e2c2:	7083      	strb	r3, [r0, #2]
 800e2c4:	fb91 f3f2 	sdiv	r3, r1, r2
 800e2c8:	7044      	strb	r4, [r0, #1]
 800e2ca:	fb02 1113 	mls	r1, r2, r3, r1
 800e2ce:	f103 0430 	add.w	r4, r3, #48	; 0x30
 800e2d2:	2300      	movs	r3, #0
 800e2d4:	3130      	adds	r1, #48	; 0x30
 800e2d6:	70c4      	strb	r4, [r0, #3]
 800e2d8:	7101      	strb	r1, [r0, #4]
 800e2da:	7143      	strb	r3, [r0, #5]
 800e2dc:	bd10      	pop	{r4, pc}

0800e2de <u8ToDec2Str>:
 800e2de:	2909      	cmp	r1, #9
 800e2e0:	d90b      	bls.n	800e2fa <u8ToDec2Str+0x1c>
 800e2e2:	2364      	movs	r3, #100	; 0x64
 800e2e4:	fbb1 f2f3 	udiv	r2, r1, r3
 800e2e8:	fb03 1112 	mls	r1, r3, r2, r1
 800e2ec:	230a      	movs	r3, #10
 800e2ee:	b2c9      	uxtb	r1, r1
 800e2f0:	fbb1 f3f3 	udiv	r3, r1, r3
 800e2f4:	3330      	adds	r3, #48	; 0x30
 800e2f6:	7003      	strb	r3, [r0, #0]
 800e2f8:	3001      	adds	r0, #1
 800e2fa:	220a      	movs	r2, #10
 800e2fc:	fbb1 f3f2 	udiv	r3, r1, r2
 800e300:	fb02 1113 	mls	r1, r2, r3, r1
 800e304:	2300      	movs	r3, #0
 800e306:	3130      	adds	r1, #48	; 0x30
 800e308:	7001      	strb	r1, [r0, #0]
 800e30a:	7043      	strb	r3, [r0, #1]
 800e30c:	4770      	bx	lr

0800e30e <u8ToDecStr>:
 800e30e:	230a      	movs	r3, #10
 800e310:	fbb1 f2f3 	udiv	r2, r1, r3
 800e314:	fb02 1113 	mls	r1, r2, r3, r1
 800e318:	2300      	movs	r3, #0
 800e31a:	3130      	adds	r1, #48	; 0x30
 800e31c:	7001      	strb	r1, [r0, #0]
 800e31e:	7043      	strb	r3, [r0, #1]
 800e320:	4770      	bx	lr

0800e322 <Char2Hex>:
 800e322:	090b      	lsrs	r3, r1, #4
 800e324:	299f      	cmp	r1, #159	; 0x9f
 800e326:	bf8c      	ite	hi
 800e328:	3337      	addhi	r3, #55	; 0x37
 800e32a:	3330      	addls	r3, #48	; 0x30
 800e32c:	7003      	strb	r3, [r0, #0]
 800e32e:	2300      	movs	r3, #0
 800e330:	f001 010f 	and.w	r1, r1, #15
 800e334:	2909      	cmp	r1, #9
 800e336:	bf8c      	ite	hi
 800e338:	3137      	addhi	r1, #55	; 0x37
 800e33a:	3130      	addls	r1, #48	; 0x30
 800e33c:	7041      	strb	r1, [r0, #1]
 800e33e:	7083      	strb	r3, [r0, #2]
 800e340:	4770      	bx	lr

0800e342 <Char2HexFullStr>:
 800e342:	2330      	movs	r3, #48	; 0x30
 800e344:	7003      	strb	r3, [r0, #0]
 800e346:	2378      	movs	r3, #120	; 0x78
 800e348:	299f      	cmp	r1, #159	; 0x9f
 800e34a:	7043      	strb	r3, [r0, #1]
 800e34c:	ea4f 1311 	mov.w	r3, r1, lsr #4
 800e350:	bf8c      	ite	hi
 800e352:	3337      	addhi	r3, #55	; 0x37
 800e354:	3330      	addls	r3, #48	; 0x30
 800e356:	7083      	strb	r3, [r0, #2]
 800e358:	2300      	movs	r3, #0
 800e35a:	f001 010f 	and.w	r1, r1, #15
 800e35e:	2909      	cmp	r1, #9
 800e360:	bf8c      	ite	hi
 800e362:	3137      	addhi	r1, #55	; 0x37
 800e364:	3130      	addls	r1, #48	; 0x30
 800e366:	70c1      	strb	r1, [r0, #3]
 800e368:	7103      	strb	r3, [r0, #4]
 800e36a:	4770      	bx	lr

0800e36c <Shor2Hex>:
 800e36c:	0b0b      	lsrs	r3, r1, #12
 800e36e:	f5b1 4f20 	cmp.w	r1, #40960	; 0xa000
 800e372:	f3c1 020b 	ubfx	r2, r1, #0, #12
 800e376:	bf2c      	ite	cs
 800e378:	3337      	addcs	r3, #55	; 0x37
 800e37a:	3330      	addcc	r3, #48	; 0x30
 800e37c:	7003      	strb	r3, [r0, #0]
 800e37e:	f5b2 6f20 	cmp.w	r2, #2560	; 0xa00
 800e382:	ea4f 2312 	mov.w	r3, r2, lsr #8
 800e386:	bf2c      	ite	cs
 800e388:	3337      	addcs	r3, #55	; 0x37
 800e38a:	3330      	addcc	r3, #48	; 0x30
 800e38c:	b2ca      	uxtb	r2, r1
 800e38e:	2a9f      	cmp	r2, #159	; 0x9f
 800e390:	7043      	strb	r3, [r0, #1]
 800e392:	ea4f 1312 	mov.w	r3, r2, lsr #4
 800e396:	bf8c      	ite	hi
 800e398:	3337      	addhi	r3, #55	; 0x37
 800e39a:	3330      	addls	r3, #48	; 0x30
 800e39c:	f001 010f 	and.w	r1, r1, #15
 800e3a0:	2909      	cmp	r1, #9
 800e3a2:	7083      	strb	r3, [r0, #2]
 800e3a4:	b2cb      	uxtb	r3, r1
 800e3a6:	bf8c      	ite	hi
 800e3a8:	3337      	addhi	r3, #55	; 0x37
 800e3aa:	3330      	addls	r3, #48	; 0x30
 800e3ac:	70c3      	strb	r3, [r0, #3]
 800e3ae:	2300      	movs	r3, #0
 800e3b0:	7103      	strb	r3, [r0, #4]
 800e3b2:	4770      	bx	lr

0800e3b4 <Sqrt32>:
 800e3b4:	b510      	push	{r4, lr}
 800e3b6:	4604      	mov	r4, r0
 800e3b8:	b150      	cbz	r0, 800e3d0 <Sqrt32+0x1c>
 800e3ba:	0880      	lsrs	r0, r0, #2
 800e3bc:	f7ff fffa 	bl	800e3b4 <Sqrt32>
 800e3c0:	0043      	lsls	r3, r0, #1
 800e3c2:	1c58      	adds	r0, r3, #1
 800e3c4:	fb00 f200 	mul.w	r2, r0, r0
 800e3c8:	42a2      	cmp	r2, r4
 800e3ca:	bf8c      	ite	hi
 800e3cc:	b298      	uxthhi	r0, r3
 800e3ce:	b280      	uxthls	r0, r0
 800e3d0:	bd10      	pop	{r4, pc}
 800e3d2:	bf00      	nop

0800e3d4 <Color_Num>:
 800e3d4:	f64f 73ff 	movw	r3, #65535	; 0xffff
 800e3d8:	4298      	cmp	r0, r3
 800e3da:	d04e      	beq.n	800e47a <Color_Num+0xa6>
 800e3dc:	f64f 72e0 	movw	r2, #65504	; 0xffe0
 800e3e0:	f020 031f 	bic.w	r3, r0, #31
 800e3e4:	4293      	cmp	r3, r2
 800e3e6:	d04a      	beq.n	800e47e <Color_Num+0xaa>
 800e3e8:	f64b 53e0 	movw	r3, #48608	; 0xbde0
 800e3ec:	f420 4284 	bic.w	r2, r0, #16896	; 0x4200
 800e3f0:	f022 021f 	bic.w	r2, r2, #31
 800e3f4:	429a      	cmp	r2, r3
 800e3f6:	d044      	beq.n	800e482 <Color_Num+0xae>
 800e3f8:	f240 73ff 	movw	r3, #2047	; 0x7ff
 800e3fc:	f3c0 020a 	ubfx	r2, r0, #0, #11
 800e400:	429a      	cmp	r2, r3
 800e402:	d040      	beq.n	800e486 <Color_Num+0xb2>
 800e404:	f240 53f7 	movw	r3, #1527	; 0x5f7
 800e408:	4383      	bics	r3, r0
 800e40a:	d03e      	beq.n	800e48a <Color_Num+0xb6>
 800e40c:	f64f 021f 	movw	r2, #63519	; 0xf81f
 800e410:	f420 63fc 	bic.w	r3, r0, #2016	; 0x7e0
 800e414:	4293      	cmp	r3, r2
 800e416:	d03a      	beq.n	800e48e <Color_Num+0xba>
 800e418:	f64b 0317 	movw	r3, #47127	; 0xb817
 800e41c:	f420 428f 	bic.w	r2, r0, #18304	; 0x4780
 800e420:	f022 0268 	bic.w	r2, r2, #104	; 0x68
 800e424:	429a      	cmp	r2, r3
 800e426:	d034      	beq.n	800e492 <Color_Num+0xbe>
 800e428:	f400 63fc 	and.w	r3, r0, #2016	; 0x7e0
 800e42c:	f5b3 6ffc 	cmp.w	r3, #2016	; 0x7e0
 800e430:	d031      	beq.n	800e496 <Color_Num+0xc2>
 800e432:	f400 63bc 	and.w	r3, r0, #1504	; 0x5e0
 800e436:	f5b3 6fbc 	cmp.w	r3, #1504	; 0x5e0
 800e43a:	d02e      	beq.n	800e49a <Color_Num+0xc6>
 800e43c:	f647 33ef 	movw	r3, #31727	; 0x7bef
 800e440:	4383      	bics	r3, r0
 800e442:	d02c      	beq.n	800e49e <Color_Num+0xca>
 800e444:	f240 531f 	movw	r3, #1311	; 0x51f
 800e448:	4383      	bics	r3, r0
 800e44a:	d02a      	beq.n	800e4a2 <Color_Num+0xce>
 800e44c:	f64f 4310 	movw	r3, #64528	; 0xfc10
 800e450:	f420 727b 	bic.w	r2, r0, #1004	; 0x3ec
 800e454:	f022 0203 	bic.w	r2, r2, #3
 800e458:	429a      	cmp	r2, r3
 800e45a:	d024      	beq.n	800e4a6 <Color_Num+0xd2>
 800e45c:	f000 031f 	and.w	r3, r0, #31
 800e460:	2b1f      	cmp	r3, #31
 800e462:	d022      	beq.n	800e4aa <Color_Num+0xd6>
 800e464:	f242 73e4 	movw	r3, #10212	; 0x27e4
 800e468:	4383      	bics	r3, r0
 800e46a:	d020      	beq.n	800e4ae <Color_Num+0xda>
 800e46c:	f643 13e7 	movw	r3, #14823	; 0x39e7
 800e470:	4383      	bics	r3, r0
 800e472:	bf0c      	ite	eq
 800e474:	200e      	moveq	r0, #14
 800e476:	200f      	movne	r0, #15
 800e478:	4770      	bx	lr
 800e47a:	2000      	movs	r0, #0
 800e47c:	4770      	bx	lr
 800e47e:	2001      	movs	r0, #1
 800e480:	4770      	bx	lr
 800e482:	2002      	movs	r0, #2
 800e484:	4770      	bx	lr
 800e486:	2003      	movs	r0, #3
 800e488:	4770      	bx	lr
 800e48a:	2004      	movs	r0, #4
 800e48c:	4770      	bx	lr
 800e48e:	2005      	movs	r0, #5
 800e490:	4770      	bx	lr
 800e492:	2006      	movs	r0, #6
 800e494:	4770      	bx	lr
 800e496:	2007      	movs	r0, #7
 800e498:	4770      	bx	lr
 800e49a:	2008      	movs	r0, #8
 800e49c:	4770      	bx	lr
 800e49e:	2009      	movs	r0, #9
 800e4a0:	4770      	bx	lr
 800e4a2:	200a      	movs	r0, #10
 800e4a4:	4770      	bx	lr
 800e4a6:	200b      	movs	r0, #11
 800e4a8:	4770      	bx	lr
 800e4aa:	200c      	movs	r0, #12
 800e4ac:	4770      	bx	lr
 800e4ae:	200d      	movs	r0, #13
 800e4b0:	4770      	bx	lr

0800e4b2 <Make_Filename>:
 800e4b2:	b513      	push	{r0, r1, r4, lr}
 800e4b4:	460c      	mov	r4, r1
 800e4b6:	b2c1      	uxtb	r1, r0
 800e4b8:	a801      	add	r0, sp, #4
 800e4ba:	f7ff fed6 	bl	800e26a <u8ToDec3Str>
 800e4be:	f89d 3004 	ldrb.w	r3, [sp, #4]
 800e4c2:	7123      	strb	r3, [r4, #4]
 800e4c4:	f89d 3005 	ldrb.w	r3, [sp, #5]
 800e4c8:	7163      	strb	r3, [r4, #5]
 800e4ca:	f89d 3006 	ldrb.w	r3, [sp, #6]
 800e4ce:	71a3      	strb	r3, [r4, #6]
 800e4d0:	b002      	add	sp, #8
 800e4d2:	bd10      	pop	{r4, pc}

0800e4d4 <Load_File_Num>:
 800e4d4:	4603      	mov	r3, r0
 800e4d6:	b570      	push	{r4, r5, r6, lr}
 800e4d8:	495f      	ldr	r1, [pc, #380]	; (800e658 <Load_File_Num+0x184>)
 800e4da:	b086      	sub	sp, #24
 800e4dc:	6808      	ldr	r0, [r1, #0]
 800e4de:	6849      	ldr	r1, [r1, #4]
 800e4e0:	aa03      	add	r2, sp, #12
 800e4e2:	c203      	stmia	r2!, {r0, r1}
 800e4e4:	f1a3 0032 	sub.w	r0, r3, #50	; 0x32
 800e4e8:	2804      	cmp	r0, #4
 800e4ea:	f200 80b2 	bhi.w	800e652 <Load_File_Num+0x17e>
 800e4ee:	e8df f000 	tbb	[pc, r0]
 800e4f2:	6b03      	.short	0x6b03
 800e4f4:	498d      	.short	0x498d
 800e4f6:	27          	.byte	0x27
 800e4f7:	00          	.byte	0x00
 800e4f8:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 800e4fc:	2100      	movs	r1, #0
 800e4fe:	4857      	ldr	r0, [pc, #348]	; (800e65c <Load_File_Num+0x188>)
 800e500:	f004 fb1b 	bl	8012b3a <memset>
 800e504:	4a56      	ldr	r2, [pc, #344]	; (800e660 <Load_File_Num+0x18c>)
 800e506:	ab03      	add	r3, sp, #12
 800e508:	6810      	ldr	r0, [r2, #0]
 800e50a:	6851      	ldr	r1, [r2, #4]
 800e50c:	2500      	movs	r5, #0
 800e50e:	c303      	stmia	r3!, {r0, r1}
 800e510:	6890      	ldr	r0, [r2, #8]
 800e512:	a903      	add	r1, sp, #12
 800e514:	6018      	str	r0, [r3, #0]
 800e516:	2000      	movs	r0, #0
 800e518:	f7ff ffcb 	bl	800e4b2 <Make_Filename>
 800e51c:	4e4f      	ldr	r6, [pc, #316]	; (800e65c <Load_File_Num+0x188>)
 800e51e:	466b      	mov	r3, sp
 800e520:	aa01      	add	r2, sp, #4
 800e522:	a903      	add	r1, sp, #12
 800e524:	4630      	mov	r0, r6
 800e526:	f000 ffa5 	bl	800f474 <OpenFileRd>
 800e52a:	b2ac      	uxth	r4, r5
 800e52c:	3501      	adds	r5, #1
 800e52e:	b110      	cbz	r0, 800e536 <Load_File_Num+0x62>
 800e530:	4620      	mov	r0, r4
 800e532:	b006      	add	sp, #24
 800e534:	bd70      	pop	{r4, r5, r6, pc}
 800e536:	a903      	add	r1, sp, #12
 800e538:	b2a8      	uxth	r0, r5
 800e53a:	f7ff ffba 	bl	800e4b2 <Make_Filename>
 800e53e:	e7ee      	b.n	800e51e <Load_File_Num+0x4a>
 800e540:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 800e544:	2100      	movs	r1, #0
 800e546:	4845      	ldr	r0, [pc, #276]	; (800e65c <Load_File_Num+0x188>)
 800e548:	f004 faf7 	bl	8012b3a <memset>
 800e54c:	4a45      	ldr	r2, [pc, #276]	; (800e664 <Load_File_Num+0x190>)
 800e54e:	ab03      	add	r3, sp, #12
 800e550:	6810      	ldr	r0, [r2, #0]
 800e552:	6851      	ldr	r1, [r2, #4]
 800e554:	2500      	movs	r5, #0
 800e556:	c303      	stmia	r3!, {r0, r1}
 800e558:	6890      	ldr	r0, [r2, #8]
 800e55a:	a903      	add	r1, sp, #12
 800e55c:	6018      	str	r0, [r3, #0]
 800e55e:	2000      	movs	r0, #0
 800e560:	f7ff ffa7 	bl	800e4b2 <Make_Filename>
 800e564:	4e3d      	ldr	r6, [pc, #244]	; (800e65c <Load_File_Num+0x188>)
 800e566:	466b      	mov	r3, sp
 800e568:	aa01      	add	r2, sp, #4
 800e56a:	a903      	add	r1, sp, #12
 800e56c:	4630      	mov	r0, r6
 800e56e:	f000 ff81 	bl	800f474 <OpenFileRd>
 800e572:	b2ac      	uxth	r4, r5
 800e574:	3501      	adds	r5, #1
 800e576:	2800      	cmp	r0, #0
 800e578:	d1da      	bne.n	800e530 <Load_File_Num+0x5c>
 800e57a:	a903      	add	r1, sp, #12
 800e57c:	b2a8      	uxth	r0, r5
 800e57e:	f7ff ff98 	bl	800e4b2 <Make_Filename>
 800e582:	e7f0      	b.n	800e566 <Load_File_Num+0x92>
 800e584:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 800e588:	2100      	movs	r1, #0
 800e58a:	4834      	ldr	r0, [pc, #208]	; (800e65c <Load_File_Num+0x188>)
 800e58c:	f004 fad5 	bl	8012b3a <memset>
 800e590:	4a35      	ldr	r2, [pc, #212]	; (800e668 <Load_File_Num+0x194>)
 800e592:	ab03      	add	r3, sp, #12
 800e594:	6810      	ldr	r0, [r2, #0]
 800e596:	6851      	ldr	r1, [r2, #4]
 800e598:	2500      	movs	r5, #0
 800e59a:	c303      	stmia	r3!, {r0, r1}
 800e59c:	6890      	ldr	r0, [r2, #8]
 800e59e:	a903      	add	r1, sp, #12
 800e5a0:	6018      	str	r0, [r3, #0]
 800e5a2:	2000      	movs	r0, #0
 800e5a4:	f7ff ff85 	bl	800e4b2 <Make_Filename>
 800e5a8:	4e2c      	ldr	r6, [pc, #176]	; (800e65c <Load_File_Num+0x188>)
 800e5aa:	466b      	mov	r3, sp
 800e5ac:	aa01      	add	r2, sp, #4
 800e5ae:	a903      	add	r1, sp, #12
 800e5b0:	4630      	mov	r0, r6
 800e5b2:	f000 ff5f 	bl	800f474 <OpenFileRd>
 800e5b6:	b2ac      	uxth	r4, r5
 800e5b8:	3501      	adds	r5, #1
 800e5ba:	2800      	cmp	r0, #0
 800e5bc:	d1b8      	bne.n	800e530 <Load_File_Num+0x5c>
 800e5be:	a903      	add	r1, sp, #12
 800e5c0:	b2a8      	uxth	r0, r5
 800e5c2:	f7ff ff76 	bl	800e4b2 <Make_Filename>
 800e5c6:	e7f0      	b.n	800e5aa <Load_File_Num+0xd6>
 800e5c8:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 800e5cc:	2100      	movs	r1, #0
 800e5ce:	4823      	ldr	r0, [pc, #140]	; (800e65c <Load_File_Num+0x188>)
 800e5d0:	f004 fab3 	bl	8012b3a <memset>
 800e5d4:	4a25      	ldr	r2, [pc, #148]	; (800e66c <Load_File_Num+0x198>)
 800e5d6:	ab03      	add	r3, sp, #12
 800e5d8:	6810      	ldr	r0, [r2, #0]
 800e5da:	6851      	ldr	r1, [r2, #4]
 800e5dc:	2500      	movs	r5, #0
 800e5de:	c303      	stmia	r3!, {r0, r1}
 800e5e0:	6890      	ldr	r0, [r2, #8]
 800e5e2:	a903      	add	r1, sp, #12
 800e5e4:	6018      	str	r0, [r3, #0]
 800e5e6:	2000      	movs	r0, #0
 800e5e8:	f7ff ff63 	bl	800e4b2 <Make_Filename>
 800e5ec:	4e1b      	ldr	r6, [pc, #108]	; (800e65c <Load_File_Num+0x188>)
 800e5ee:	466b      	mov	r3, sp
 800e5f0:	aa01      	add	r2, sp, #4
 800e5f2:	a903      	add	r1, sp, #12
 800e5f4:	4630      	mov	r0, r6
 800e5f6:	f000 ff3d 	bl	800f474 <OpenFileRd>
 800e5fa:	b2ac      	uxth	r4, r5
 800e5fc:	3501      	adds	r5, #1
 800e5fe:	2800      	cmp	r0, #0
 800e600:	d196      	bne.n	800e530 <Load_File_Num+0x5c>
 800e602:	a903      	add	r1, sp, #12
 800e604:	b2a8      	uxth	r0, r5
 800e606:	f7ff ff54 	bl	800e4b2 <Make_Filename>
 800e60a:	e7f0      	b.n	800e5ee <Load_File_Num+0x11a>
 800e60c:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 800e610:	2100      	movs	r1, #0
 800e612:	4812      	ldr	r0, [pc, #72]	; (800e65c <Load_File_Num+0x188>)
 800e614:	f004 fa91 	bl	8012b3a <memset>
 800e618:	4a15      	ldr	r2, [pc, #84]	; (800e670 <Load_File_Num+0x19c>)
 800e61a:	ab03      	add	r3, sp, #12
 800e61c:	6810      	ldr	r0, [r2, #0]
 800e61e:	6851      	ldr	r1, [r2, #4]
 800e620:	2500      	movs	r5, #0
 800e622:	c303      	stmia	r3!, {r0, r1}
 800e624:	6890      	ldr	r0, [r2, #8]
 800e626:	a903      	add	r1, sp, #12
 800e628:	6018      	str	r0, [r3, #0]
 800e62a:	2000      	movs	r0, #0
 800e62c:	f7ff ff41 	bl	800e4b2 <Make_Filename>
 800e630:	4e0a      	ldr	r6, [pc, #40]	; (800e65c <Load_File_Num+0x188>)
 800e632:	466b      	mov	r3, sp
 800e634:	aa01      	add	r2, sp, #4
 800e636:	a903      	add	r1, sp, #12
 800e638:	4630      	mov	r0, r6
 800e63a:	f000 ff1b 	bl	800f474 <OpenFileRd>
 800e63e:	b2ac      	uxth	r4, r5
 800e640:	3501      	adds	r5, #1
 800e642:	2800      	cmp	r0, #0
 800e644:	f47f af74 	bne.w	800e530 <Load_File_Num+0x5c>
 800e648:	a903      	add	r1, sp, #12
 800e64a:	b2a8      	uxth	r0, r5
 800e64c:	f7ff ff31 	bl	800e4b2 <Make_Filename>
 800e650:	e7ef      	b.n	800e632 <Load_File_Num+0x15e>
 800e652:	2400      	movs	r4, #0
 800e654:	e76c      	b.n	800e530 <Load_File_Num+0x5c>
 800e656:	bf00      	nop
 800e658:	0801474a 	.word	0x0801474a
 800e65c:	200084a4 	.word	0x200084a4
 800e660:	08014756 	.word	0x08014756
 800e664:	08014762 	.word	0x08014762
 800e668:	0801476e 	.word	0x0801476e
 800e66c:	0801477a 	.word	0x0801477a
 800e670:	08014786 	.word	0x08014786

0800e674 <File_Num>:
 800e674:	b510      	push	{r4, lr}
 800e676:	2032      	movs	r0, #50	; 0x32
 800e678:	f7ff ff2c 	bl	800e4d4 <Load_File_Num>
 800e67c:	4c04      	ldr	r4, [pc, #16]	; (800e690 <File_Num+0x1c>)
 800e67e:	f8a4 0064 	strh.w	r0, [r4, #100]	; 0x64
 800e682:	2033      	movs	r0, #51	; 0x33
 800e684:	f7ff ff26 	bl	800e4d4 <Load_File_Num>
 800e688:	f8a4 0066 	strh.w	r0, [r4, #102]	; 0x66
 800e68c:	bd10      	pop	{r4, pc}
 800e68e:	bf00      	nop
 800e690:	2000000c 	.word	0x2000000c

0800e694 <PrintClk>:
 800e694:	3003      	adds	r0, #3
 800e696:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 800e69a:	b286      	uxth	r6, r0
 800e69c:	4630      	mov	r0, r6
 800e69e:	4617      	mov	r7, r2
 800e6a0:	468a      	mov	sl, r1
 800e6a2:	f7fd fa47 	bl	800bb34 <Set_Posi>
 800e6a6:	220c      	movs	r2, #12
 800e6a8:	2400      	movs	r4, #0
 800e6aa:	4b15      	ldr	r3, [pc, #84]	; (800e700 <PrintClk+0x6c>)
 800e6ac:	f10a 0801 	add.w	r8, sl, #1
 800e6b0:	fb02 3707 	mla	r7, r2, r7, r3
 800e6b4:	fa1f f888 	uxth.w	r8, r8
 800e6b8:	1935      	adds	r5, r6, r4
 800e6ba:	b2ad      	uxth	r5, r5
 800e6bc:	4651      	mov	r1, sl
 800e6be:	4628      	mov	r0, r5
 800e6c0:	f817 9004 	ldrb.w	r9, [r7, r4]
 800e6c4:	f7fd fa36 	bl	800bb34 <Set_Posi>
 800e6c8:	f647 30ef 	movw	r0, #31727	; 0x7bef
 800e6cc:	f7fd fa3e 	bl	800bb4c <Set_Pixel>
 800e6d0:	4628      	mov	r0, r5
 800e6d2:	4641      	mov	r1, r8
 800e6d4:	f7fd fa2e 	bl	800bb34 <Set_Posi>
 800e6d8:	2508      	movs	r5, #8
 800e6da:	f019 0f01 	tst.w	r9, #1
 800e6de:	bf14      	ite	ne
 800e6e0:	201f      	movne	r0, #31
 800e6e2:	f647 30ef 	movweq	r0, #31727	; 0x7bef
 800e6e6:	3d01      	subs	r5, #1
 800e6e8:	f7fd fa30 	bl	800bb4c <Set_Pixel>
 800e6ec:	f015 05ff 	ands.w	r5, r5, #255	; 0xff
 800e6f0:	ea4f 0959 	mov.w	r9, r9, lsr #1
 800e6f4:	d1f1      	bne.n	800e6da <PrintClk+0x46>
 800e6f6:	3401      	adds	r4, #1
 800e6f8:	2c0c      	cmp	r4, #12
 800e6fa:	d1dd      	bne.n	800e6b8 <PrintClk+0x24>
 800e6fc:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 800e700:	08014594 	.word	0x08014594

0800e704 <Save_Bmp>:
 800e704:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800e708:	4604      	mov	r4, r0
 800e70a:	4a55      	ldr	r2, [pc, #340]	; (800e860 <Save_Bmp+0x15c>)
 800e70c:	b089      	sub	sp, #36	; 0x24
 800e70e:	6810      	ldr	r0, [r2, #0]
 800e710:	6851      	ldr	r1, [r2, #4]
 800e712:	ab05      	add	r3, sp, #20
 800e714:	c303      	stmia	r3!, {r0, r1}
 800e716:	6890      	ldr	r0, [r2, #8]
 800e718:	f8df 8148 	ldr.w	r8, [pc, #328]	; 800e864 <Save_Bmp+0x160>
 800e71c:	6018      	str	r0, [r3, #0]
 800e71e:	2000      	movs	r0, #0
 800e720:	f000 fd60 	bl	800f1e4 <USB_Connect>
 800e724:	a905      	add	r1, sp, #20
 800e726:	4620      	mov	r0, r4
 800e728:	f7ff fec3 	bl	800e4b2 <Make_Filename>
 800e72c:	ab02      	add	r3, sp, #8
 800e72e:	aa03      	add	r2, sp, #12
 800e730:	a905      	add	r1, sp, #20
 800e732:	484c      	ldr	r0, [pc, #304]	; (800e864 <Save_Bmp+0x160>)
 800e734:	f000 feda 	bl	800f4ec <OpenFileWr>
 800e738:	2800      	cmp	r0, #0
 800e73a:	f040 808e 	bne.w	800e85a <Save_Bmp+0x156>
 800e73e:	4642      	mov	r2, r8
 800e740:	4b49      	ldr	r3, [pc, #292]	; (800e868 <Save_Bmp+0x164>)
 800e742:	f103 0134 	add.w	r1, r3, #52	; 0x34
 800e746:	f853 0b04 	ldr.w	r0, [r3], #4
 800e74a:	428b      	cmp	r3, r1
 800e74c:	f842 0b04 	str.w	r0, [r2], #4
 800e750:	d1f9      	bne.n	800e746 <Save_Bmp+0x42>
 800e752:	2500      	movs	r5, #0
 800e754:	881b      	ldrh	r3, [r3, #0]
 800e756:	46aa      	mov	sl, r5
 800e758:	8013      	strh	r3, [r2, #0]
 800e75a:	462e      	mov	r6, r5
 800e75c:	2336      	movs	r3, #54	; 0x36
 800e75e:	f64f 7be0 	movw	fp, #65504	; 0xffe0
 800e762:	f8df 9100 	ldr.w	r9, [pc, #256]	; 800e864 <Save_Bmp+0x160>
 800e766:	2700      	movs	r7, #0
 800e768:	b2b2      	uxth	r2, r6
 800e76a:	9200      	str	r2, [sp, #0]
 800e76c:	9900      	ldr	r1, [sp, #0]
 800e76e:	b2b8      	uxth	r0, r7
 800e770:	9301      	str	r3, [sp, #4]
 800e772:	f7fd f9df 	bl	800bb34 <Set_Posi>
 800e776:	f7fd f9ed 	bl	800bb54 <Read_Pixel>
 800e77a:	ea0b 0460 	and.w	r4, fp, r0, asr #1
 800e77e:	f000 001f 	and.w	r0, r0, #31
 800e782:	4320      	orrs	r0, r4
 800e784:	f400 7478 	and.w	r4, r0, #992	; 0x3e0
 800e788:	ea44 2190 	orr.w	r1, r4, r0, lsr #10
 800e78c:	0284      	lsls	r4, r0, #10
 800e78e:	9b01      	ldr	r3, [sp, #4]
 800e790:	f404 44f8 	and.w	r4, r4, #31744	; 0x7c00
 800e794:	430c      	orrs	r4, r1
 800e796:	f808 4003 	strb.w	r4, [r8, r3]
 800e79a:	3301      	adds	r3, #1
 800e79c:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800e7a0:	d31a      	bcc.n	800e7d8 <Save_Bmp+0xd4>
 800e7a2:	a903      	add	r1, sp, #12
 800e7a4:	4648      	mov	r0, r9
 800e7a6:	f000 fe3b 	bl	800f420 <ProgFileSec>
 800e7aa:	b120      	cbz	r0, 800e7b6 <Save_Bmp+0xb2>
 800e7ac:	2403      	movs	r4, #3
 800e7ae:	4620      	mov	r0, r4
 800e7b0:	b009      	add	sp, #36	; 0x24
 800e7b2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800e7b6:	b965      	cbnz	r5, 800e7d2 <Save_Bmp+0xce>
 800e7b8:	f10a 0301 	add.w	r3, sl, #1
 800e7bc:	b29b      	uxth	r3, r3
 800e7be:	f3ca 0241 	ubfx	r2, sl, #1, #2
 800e7c2:	21e1      	movs	r1, #225	; 0xe1
 800e7c4:	f240 102b 	movw	r0, #299	; 0x12b
 800e7c8:	9301      	str	r3, [sp, #4]
 800e7ca:	f7ff ff63 	bl	800e694 <PrintClk>
 800e7ce:	9b01      	ldr	r3, [sp, #4]
 800e7d0:	469a      	mov	sl, r3
 800e7d2:	2300      	movs	r3, #0
 800e7d4:	f085 0501 	eor.w	r5, r5, #1
 800e7d8:	1224      	asrs	r4, r4, #8
 800e7da:	f808 4003 	strb.w	r4, [r8, r3]
 800e7de:	3301      	adds	r3, #1
 800e7e0:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800e7e4:	d114      	bne.n	800e810 <Save_Bmp+0x10c>
 800e7e6:	a903      	add	r1, sp, #12
 800e7e8:	4648      	mov	r0, r9
 800e7ea:	f000 fe19 	bl	800f420 <ProgFileSec>
 800e7ee:	2800      	cmp	r0, #0
 800e7f0:	d1dc      	bne.n	800e7ac <Save_Bmp+0xa8>
 800e7f2:	b955      	cbnz	r5, 800e80a <Save_Bmp+0x106>
 800e7f4:	f10a 0401 	add.w	r4, sl, #1
 800e7f8:	f3ca 0241 	ubfx	r2, sl, #1, #2
 800e7fc:	b2a4      	uxth	r4, r4
 800e7fe:	21e1      	movs	r1, #225	; 0xe1
 800e800:	f240 102b 	movw	r0, #299	; 0x12b
 800e804:	f7ff ff46 	bl	800e694 <PrintClk>
 800e808:	46a2      	mov	sl, r4
 800e80a:	2300      	movs	r3, #0
 800e80c:	f085 0501 	eor.w	r5, r5, #1
 800e810:	3701      	adds	r7, #1
 800e812:	f5b7 7fa0 	cmp.w	r7, #320	; 0x140
 800e816:	d1a9      	bne.n	800e76c <Save_Bmp+0x68>
 800e818:	3601      	adds	r6, #1
 800e81a:	2ef0      	cmp	r6, #240	; 0xf0
 800e81c:	d1a3      	bne.n	800e766 <Save_Bmp+0x62>
 800e81e:	b9ab      	cbnz	r3, 800e84c <Save_Bmp+0x148>
 800e820:	ab02      	add	r3, sp, #8
 800e822:	aa03      	add	r2, sp, #12
 800e824:	f44f 3118 	mov.w	r1, #155648	; 0x26000
 800e828:	480e      	ldr	r0, [pc, #56]	; (800e864 <Save_Bmp+0x160>)
 800e82a:	f000 fe95 	bl	800f558 <CloseFile>
 800e82e:	4604      	mov	r4, r0
 800e830:	2800      	cmp	r0, #0
 800e832:	d1bb      	bne.n	800e7ac <Save_Bmp+0xa8>
 800e834:	4a0d      	ldr	r2, [pc, #52]	; (800e86c <Save_Bmp+0x168>)
 800e836:	2001      	movs	r0, #1
 800e838:	f8b2 3064 	ldrh.w	r3, [r2, #100]	; 0x64
 800e83c:	2b62      	cmp	r3, #98	; 0x62
 800e83e:	bf9c      	itt	ls
 800e840:	3301      	addls	r3, #1
 800e842:	f8a2 3064 	strhls.w	r3, [r2, #100]	; 0x64
 800e846:	f000 fccd 	bl	800f1e4 <USB_Connect>
 800e84a:	e7b0      	b.n	800e7ae <Save_Bmp+0xaa>
 800e84c:	a903      	add	r1, sp, #12
 800e84e:	4805      	ldr	r0, [pc, #20]	; (800e864 <Save_Bmp+0x160>)
 800e850:	f000 fde6 	bl	800f420 <ProgFileSec>
 800e854:	2800      	cmp	r0, #0
 800e856:	d0e3      	beq.n	800e820 <Save_Bmp+0x11c>
 800e858:	e7a8      	b.n	800e7ac <Save_Bmp+0xa8>
 800e85a:	2404      	movs	r4, #4
 800e85c:	e7a7      	b.n	800e7ae <Save_Bmp+0xaa>
 800e85e:	bf00      	nop
 800e860:	08014756 	.word	0x08014756
 800e864:	200084a4 	.word	0x200084a4
 800e868:	08014682 	.word	0x08014682
 800e86c:	2000000c 	.word	0x2000000c

0800e870 <Save_Csv>:
 800e870:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800e874:	4604      	mov	r4, r0
 800e876:	4a2d      	ldr	r2, [pc, #180]	; (800e92c <Save_Csv+0xbc>)
 800e878:	b08d      	sub	sp, #52	; 0x34
 800e87a:	6810      	ldr	r0, [r2, #0]
 800e87c:	6851      	ldr	r1, [r2, #4]
 800e87e:	ab06      	add	r3, sp, #24
 800e880:	c303      	stmia	r3!, {r0, r1}
 800e882:	6890      	ldr	r0, [r2, #8]
 800e884:	4f2a      	ldr	r7, [pc, #168]	; (800e930 <Save_Csv+0xc0>)
 800e886:	6018      	str	r0, [r3, #0]
 800e888:	2000      	movs	r0, #0
 800e88a:	f000 fcab 	bl	800f1e4 <USB_Connect>
 800e88e:	a906      	add	r1, sp, #24
 800e890:	b2a0      	uxth	r0, r4
 800e892:	f7ff fe0e 	bl	800e4b2 <Make_Filename>
 800e896:	ab03      	add	r3, sp, #12
 800e898:	aa04      	add	r2, sp, #16
 800e89a:	a906      	add	r1, sp, #24
 800e89c:	4824      	ldr	r0, [pc, #144]	; (800e930 <Save_Csv+0xc0>)
 800e89e:	f000 fe25 	bl	800f4ec <OpenFileWr>
 800e8a2:	2800      	cmp	r0, #0
 800e8a4:	f040 81fe 	bne.w	800eca4 <Save_Csv+0x434>
 800e8a8:	463a      	mov	r2, r7
 800e8aa:	4b22      	ldr	r3, [pc, #136]	; (800e934 <Save_Csv+0xc4>)
 800e8ac:	f103 011c 	add.w	r1, r3, #28
 800e8b0:	f853 0b04 	ldr.w	r0, [r3], #4
 800e8b4:	428b      	cmp	r3, r1
 800e8b6:	f842 0b04 	str.w	r0, [r2], #4
 800e8ba:	d1f9      	bne.n	800e8b0 <Save_Csv+0x40>
 800e8bc:	2400      	movs	r4, #0
 800e8be:	781b      	ldrb	r3, [r3, #0]
 800e8c0:	f8df 9078 	ldr.w	r9, [pc, #120]	; 800e93c <Save_Csv+0xcc>
 800e8c4:	7013      	strb	r3, [r2, #0]
 800e8c6:	4b1c      	ldr	r3, [pc, #112]	; (800e938 <Save_Csv+0xc8>)
 800e8c8:	261f      	movs	r6, #31
 800e8ca:	881b      	ldrh	r3, [r3, #0]
 800e8cc:	46a0      	mov	r8, r4
 800e8ce:	f829 391d 	strh.w	r3, [r9], #-29
 800e8d2:	230a      	movs	r3, #10
 800e8d4:	ad09      	add	r5, sp, #36	; 0x24
 800e8d6:	9300      	str	r3, [sp, #0]
 800e8d8:	f8bd b000 	ldrh.w	fp, [sp]
 800e8dc:	4658      	mov	r0, fp
 800e8de:	f7fd fddf 	bl	800c4a0 <TransStatusData>
 800e8e2:	4682      	mov	sl, r0
 800e8e4:	f10b 0001 	add.w	r0, fp, #1
 800e8e8:	b280      	uxth	r0, r0
 800e8ea:	f7fd fde7 	bl	800c4bc <TransformTime>
 800e8ee:	210a      	movs	r1, #10
 800e8f0:	e9c5 8800 	strd	r8, r8, [r5]
 800e8f4:	4341      	muls	r1, r0
 800e8f6:	f8c5 8008 	str.w	r8, [r5, #8]
 800e8fa:	4628      	mov	r0, r5
 800e8fc:	f7ff fc4c 	bl	800e198 <u32ToDecStr>
 800e900:	f04f 0b00 	mov.w	fp, #0
 800e904:	f815 200b 	ldrb.w	r2, [r5, fp]
 800e908:	1c73      	adds	r3, r6, #1
 800e90a:	b34a      	cbz	r2, 800e960 <Save_Csv+0xf0>
 800e90c:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800e910:	55ba      	strb	r2, [r7, r6]
 800e912:	f0c0 81bd 	bcc.w	800ec90 <Save_Csv+0x420>
 800e916:	a904      	add	r1, sp, #16
 800e918:	4648      	mov	r0, r9
 800e91a:	f000 fd81 	bl	800f420 <ProgFileSec>
 800e91e:	4606      	mov	r6, r0
 800e920:	b170      	cbz	r0, 800e940 <Save_Csv+0xd0>
 800e922:	2403      	movs	r4, #3
 800e924:	4620      	mov	r0, r4
 800e926:	b00d      	add	sp, #52	; 0x34
 800e928:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800e92c:	0801477a 	.word	0x0801477a
 800e930:	200084a4 	.word	0x200084a4
 800e934:	08014792 	.word	0x08014792
 800e938:	080147b0 	.word	0x080147b0
 800e93c:	200084c1 	.word	0x200084c1
 800e940:	1c63      	adds	r3, r4, #1
 800e942:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800e946:	21e1      	movs	r1, #225	; 0xe1
 800e948:	f240 102b 	movw	r0, #299	; 0x12b
 800e94c:	9301      	str	r3, [sp, #4]
 800e94e:	f7ff fea1 	bl	800e694 <PrintClk>
 800e952:	9b01      	ldr	r3, [sp, #4]
 800e954:	461c      	mov	r4, r3
 800e956:	f10b 0b01 	add.w	fp, fp, #1
 800e95a:	f1bb 0f0c 	cmp.w	fp, #12
 800e95e:	d1d1      	bne.n	800e904 <Save_Csv+0x94>
 800e960:	222c      	movs	r2, #44	; 0x2c
 800e962:	1c73      	adds	r3, r6, #1
 800e964:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800e968:	55ba      	strb	r2, [r7, r6]
 800e96a:	d311      	bcc.n	800e990 <Save_Csv+0x120>
 800e96c:	a904      	add	r1, sp, #16
 800e96e:	4648      	mov	r0, r9
 800e970:	f000 fd56 	bl	800f420 <ProgFileSec>
 800e974:	4606      	mov	r6, r0
 800e976:	2800      	cmp	r0, #0
 800e978:	d1d3      	bne.n	800e922 <Save_Csv+0xb2>
 800e97a:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800e97e:	f104 0b01 	add.w	fp, r4, #1
 800e982:	21e1      	movs	r1, #225	; 0xe1
 800e984:	f240 102b 	movw	r0, #299	; 0x12b
 800e988:	f7ff fe84 	bl	800e694 <PrintClk>
 800e98c:	465c      	mov	r4, fp
 800e98e:	4633      	mov	r3, r6
 800e990:	f04f 0b00 	mov.w	fp, #0
 800e994:	e9c5 8800 	strd	r8, r8, [r5]
 800e998:	f8c5 8008 	str.w	r8, [r5, #8]
 800e99c:	f00a 0101 	and.w	r1, sl, #1
 800e9a0:	4628      	mov	r0, r5
 800e9a2:	9301      	str	r3, [sp, #4]
 800e9a4:	f7ff fcbd 	bl	800e322 <Char2Hex>
 800e9a8:	9b01      	ldr	r3, [sp, #4]
 800e9aa:	f815 100b 	ldrb.w	r1, [r5, fp]
 800e9ae:	1c5a      	adds	r2, r3, #1
 800e9b0:	b1e1      	cbz	r1, 800e9ec <Save_Csv+0x17c>
 800e9b2:	f5b2 5f80 	cmp.w	r2, #4096	; 0x1000
 800e9b6:	54f9      	strb	r1, [r7, r3]
 800e9b8:	f0c0 816c 	bcc.w	800ec94 <Save_Csv+0x424>
 800e9bc:	a904      	add	r1, sp, #16
 800e9be:	4648      	mov	r0, r9
 800e9c0:	f000 fd2e 	bl	800f420 <ProgFileSec>
 800e9c4:	4606      	mov	r6, r0
 800e9c6:	2800      	cmp	r0, #0
 800e9c8:	d1ab      	bne.n	800e922 <Save_Csv+0xb2>
 800e9ca:	1c63      	adds	r3, r4, #1
 800e9cc:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800e9d0:	21e1      	movs	r1, #225	; 0xe1
 800e9d2:	f240 102b 	movw	r0, #299	; 0x12b
 800e9d6:	9301      	str	r3, [sp, #4]
 800e9d8:	f7ff fe5c 	bl	800e694 <PrintClk>
 800e9dc:	9b01      	ldr	r3, [sp, #4]
 800e9de:	461c      	mov	r4, r3
 800e9e0:	4633      	mov	r3, r6
 800e9e2:	f10b 0b01 	add.w	fp, fp, #1
 800e9e6:	f1bb 0f03 	cmp.w	fp, #3
 800e9ea:	d1de      	bne.n	800e9aa <Save_Csv+0x13a>
 800e9ec:	222c      	movs	r2, #44	; 0x2c
 800e9ee:	1c5e      	adds	r6, r3, #1
 800e9f0:	f5b6 5f80 	cmp.w	r6, #4096	; 0x1000
 800e9f4:	54fa      	strb	r2, [r7, r3]
 800e9f6:	d310      	bcc.n	800ea1a <Save_Csv+0x1aa>
 800e9f8:	a904      	add	r1, sp, #16
 800e9fa:	4648      	mov	r0, r9
 800e9fc:	f000 fd10 	bl	800f420 <ProgFileSec>
 800ea00:	4606      	mov	r6, r0
 800ea02:	2800      	cmp	r0, #0
 800ea04:	d18d      	bne.n	800e922 <Save_Csv+0xb2>
 800ea06:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800ea0a:	f104 0b01 	add.w	fp, r4, #1
 800ea0e:	21e1      	movs	r1, #225	; 0xe1
 800ea10:	f240 102b 	movw	r0, #299	; 0x12b
 800ea14:	f7ff fe3e 	bl	800e694 <PrintClk>
 800ea18:	465c      	mov	r4, fp
 800ea1a:	e9c5 8800 	strd	r8, r8, [r5]
 800ea1e:	f8c5 8008 	str.w	r8, [r5, #8]
 800ea22:	f3ca 0140 	ubfx	r1, sl, #1, #1
 800ea26:	4628      	mov	r0, r5
 800ea28:	f7ff fc7b 	bl	800e322 <Char2Hex>
 800ea2c:	f04f 0b00 	mov.w	fp, #0
 800ea30:	f815 200b 	ldrb.w	r2, [r5, fp]
 800ea34:	1c73      	adds	r3, r6, #1
 800ea36:	b1e2      	cbz	r2, 800ea72 <Save_Csv+0x202>
 800ea38:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800ea3c:	55ba      	strb	r2, [r7, r6]
 800ea3e:	f0c0 812b 	bcc.w	800ec98 <Save_Csv+0x428>
 800ea42:	a904      	add	r1, sp, #16
 800ea44:	4648      	mov	r0, r9
 800ea46:	f000 fceb 	bl	800f420 <ProgFileSec>
 800ea4a:	4606      	mov	r6, r0
 800ea4c:	2800      	cmp	r0, #0
 800ea4e:	f47f af68 	bne.w	800e922 <Save_Csv+0xb2>
 800ea52:	1c63      	adds	r3, r4, #1
 800ea54:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800ea58:	21e1      	movs	r1, #225	; 0xe1
 800ea5a:	f240 102b 	movw	r0, #299	; 0x12b
 800ea5e:	9301      	str	r3, [sp, #4]
 800ea60:	f7ff fe18 	bl	800e694 <PrintClk>
 800ea64:	9b01      	ldr	r3, [sp, #4]
 800ea66:	461c      	mov	r4, r3
 800ea68:	f10b 0b01 	add.w	fp, fp, #1
 800ea6c:	f1bb 0f03 	cmp.w	fp, #3
 800ea70:	d1de      	bne.n	800ea30 <Save_Csv+0x1c0>
 800ea72:	222c      	movs	r2, #44	; 0x2c
 800ea74:	1c73      	adds	r3, r6, #1
 800ea76:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800ea7a:	55ba      	strb	r2, [r7, r6]
 800ea7c:	d312      	bcc.n	800eaa4 <Save_Csv+0x234>
 800ea7e:	a904      	add	r1, sp, #16
 800ea80:	4648      	mov	r0, r9
 800ea82:	f000 fccd 	bl	800f420 <ProgFileSec>
 800ea86:	4606      	mov	r6, r0
 800ea88:	2800      	cmp	r0, #0
 800ea8a:	f47f af4a 	bne.w	800e922 <Save_Csv+0xb2>
 800ea8e:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800ea92:	f104 0b01 	add.w	fp, r4, #1
 800ea96:	21e1      	movs	r1, #225	; 0xe1
 800ea98:	f240 102b 	movw	r0, #299	; 0x12b
 800ea9c:	f7ff fdfa 	bl	800e694 <PrintClk>
 800eaa0:	465c      	mov	r4, fp
 800eaa2:	4633      	mov	r3, r6
 800eaa4:	f04f 0b00 	mov.w	fp, #0
 800eaa8:	e9c5 8800 	strd	r8, r8, [r5]
 800eaac:	f8c5 8008 	str.w	r8, [r5, #8]
 800eab0:	f3ca 0180 	ubfx	r1, sl, #2, #1
 800eab4:	4628      	mov	r0, r5
 800eab6:	9301      	str	r3, [sp, #4]
 800eab8:	f7ff fc33 	bl	800e322 <Char2Hex>
 800eabc:	9b01      	ldr	r3, [sp, #4]
 800eabe:	f815 100b 	ldrb.w	r1, [r5, fp]
 800eac2:	1c5a      	adds	r2, r3, #1
 800eac4:	b1e9      	cbz	r1, 800eb02 <Save_Csv+0x292>
 800eac6:	f5b2 5f80 	cmp.w	r2, #4096	; 0x1000
 800eaca:	54f9      	strb	r1, [r7, r3]
 800eacc:	f0c0 80e6 	bcc.w	800ec9c <Save_Csv+0x42c>
 800ead0:	a904      	add	r1, sp, #16
 800ead2:	4648      	mov	r0, r9
 800ead4:	f000 fca4 	bl	800f420 <ProgFileSec>
 800ead8:	4606      	mov	r6, r0
 800eada:	2800      	cmp	r0, #0
 800eadc:	f47f af21 	bne.w	800e922 <Save_Csv+0xb2>
 800eae0:	1c63      	adds	r3, r4, #1
 800eae2:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800eae6:	21e1      	movs	r1, #225	; 0xe1
 800eae8:	f240 102b 	movw	r0, #299	; 0x12b
 800eaec:	9301      	str	r3, [sp, #4]
 800eaee:	f7ff fdd1 	bl	800e694 <PrintClk>
 800eaf2:	9b01      	ldr	r3, [sp, #4]
 800eaf4:	461c      	mov	r4, r3
 800eaf6:	4633      	mov	r3, r6
 800eaf8:	f10b 0b01 	add.w	fp, fp, #1
 800eafc:	f1bb 0f03 	cmp.w	fp, #3
 800eb00:	d1dd      	bne.n	800eabe <Save_Csv+0x24e>
 800eb02:	222c      	movs	r2, #44	; 0x2c
 800eb04:	1c5e      	adds	r6, r3, #1
 800eb06:	f5b6 5f80 	cmp.w	r6, #4096	; 0x1000
 800eb0a:	54fa      	strb	r2, [r7, r3]
 800eb0c:	d311      	bcc.n	800eb32 <Save_Csv+0x2c2>
 800eb0e:	a904      	add	r1, sp, #16
 800eb10:	4648      	mov	r0, r9
 800eb12:	f000 fc85 	bl	800f420 <ProgFileSec>
 800eb16:	4606      	mov	r6, r0
 800eb18:	2800      	cmp	r0, #0
 800eb1a:	f47f af02 	bne.w	800e922 <Save_Csv+0xb2>
 800eb1e:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800eb22:	f104 0b01 	add.w	fp, r4, #1
 800eb26:	21e1      	movs	r1, #225	; 0xe1
 800eb28:	f240 102b 	movw	r0, #299	; 0x12b
 800eb2c:	f7ff fdb2 	bl	800e694 <PrintClk>
 800eb30:	465c      	mov	r4, fp
 800eb32:	f3ca 01c0 	ubfx	r1, sl, #3, #1
 800eb36:	e9c5 8800 	strd	r8, r8, [r5]
 800eb3a:	f8c5 8008 	str.w	r8, [r5, #8]
 800eb3e:	4628      	mov	r0, r5
 800eb40:	f7ff fbef 	bl	800e322 <Char2Hex>
 800eb44:	f04f 0a00 	mov.w	sl, #0
 800eb48:	f815 200a 	ldrb.w	r2, [r5, sl]
 800eb4c:	1c73      	adds	r3, r6, #1
 800eb4e:	b1da      	cbz	r2, 800eb88 <Save_Csv+0x318>
 800eb50:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800eb54:	55ba      	strb	r2, [r7, r6]
 800eb56:	f0c0 80a3 	bcc.w	800eca0 <Save_Csv+0x430>
 800eb5a:	a904      	add	r1, sp, #16
 800eb5c:	4648      	mov	r0, r9
 800eb5e:	f000 fc5f 	bl	800f420 <ProgFileSec>
 800eb62:	4606      	mov	r6, r0
 800eb64:	2800      	cmp	r0, #0
 800eb66:	f47f aedc 	bne.w	800e922 <Save_Csv+0xb2>
 800eb6a:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800eb6e:	f104 0b01 	add.w	fp, r4, #1
 800eb72:	21e1      	movs	r1, #225	; 0xe1
 800eb74:	f240 102b 	movw	r0, #299	; 0x12b
 800eb78:	f7ff fd8c 	bl	800e694 <PrintClk>
 800eb7c:	465c      	mov	r4, fp
 800eb7e:	f10a 0a01 	add.w	sl, sl, #1
 800eb82:	f1ba 0f03 	cmp.w	sl, #3
 800eb86:	d1df      	bne.n	800eb48 <Save_Csv+0x2d8>
 800eb88:	232c      	movs	r3, #44	; 0x2c
 800eb8a:	1c70      	adds	r0, r6, #1
 800eb8c:	f5b0 5f80 	cmp.w	r0, #4096	; 0x1000
 800eb90:	55bb      	strb	r3, [r7, r6]
 800eb92:	d312      	bcc.n	800ebba <Save_Csv+0x34a>
 800eb94:	a904      	add	r1, sp, #16
 800eb96:	4648      	mov	r0, r9
 800eb98:	f000 fc42 	bl	800f420 <ProgFileSec>
 800eb9c:	4606      	mov	r6, r0
 800eb9e:	2800      	cmp	r0, #0
 800eba0:	f47f aebf 	bne.w	800e922 <Save_Csv+0xb2>
 800eba4:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800eba8:	f240 102b 	movw	r0, #299	; 0x12b
 800ebac:	f104 0a01 	add.w	sl, r4, #1
 800ebb0:	21e1      	movs	r1, #225	; 0xe1
 800ebb2:	f7ff fd6f 	bl	800e694 <PrintClk>
 800ebb6:	4654      	mov	r4, sl
 800ebb8:	4630      	mov	r0, r6
 800ebba:	220d      	movs	r2, #13
 800ebbc:	1c43      	adds	r3, r0, #1
 800ebbe:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 800ebc2:	543a      	strb	r2, [r7, r0]
 800ebc4:	d112      	bne.n	800ebec <Save_Csv+0x37c>
 800ebc6:	a904      	add	r1, sp, #16
 800ebc8:	4648      	mov	r0, r9
 800ebca:	f000 fc29 	bl	800f420 <ProgFileSec>
 800ebce:	4606      	mov	r6, r0
 800ebd0:	2800      	cmp	r0, #0
 800ebd2:	f47f aea6 	bne.w	800e922 <Save_Csv+0xb2>
 800ebd6:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800ebda:	f104 0a01 	add.w	sl, r4, #1
 800ebde:	21e1      	movs	r1, #225	; 0xe1
 800ebe0:	f240 102b 	movw	r0, #299	; 0x12b
 800ebe4:	f7ff fd56 	bl	800e694 <PrintClk>
 800ebe8:	4654      	mov	r4, sl
 800ebea:	4633      	mov	r3, r6
 800ebec:	220a      	movs	r2, #10
 800ebee:	1c5e      	adds	r6, r3, #1
 800ebf0:	f5b6 5f80 	cmp.w	r6, #4096	; 0x1000
 800ebf4:	54fa      	strb	r2, [r7, r3]
 800ebf6:	d111      	bne.n	800ec1c <Save_Csv+0x3ac>
 800ebf8:	a904      	add	r1, sp, #16
 800ebfa:	4648      	mov	r0, r9
 800ebfc:	f000 fc10 	bl	800f420 <ProgFileSec>
 800ec00:	4606      	mov	r6, r0
 800ec02:	2800      	cmp	r0, #0
 800ec04:	f47f ae8d 	bne.w	800e922 <Save_Csv+0xb2>
 800ec08:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800ec0c:	f104 0a01 	add.w	sl, r4, #1
 800ec10:	21e1      	movs	r1, #225	; 0xe1
 800ec12:	f240 102b 	movw	r0, #299	; 0x12b
 800ec16:	f7ff fd3d 	bl	800e694 <PrintClk>
 800ec1a:	4654      	mov	r4, sl
 800ec1c:	9b00      	ldr	r3, [sp, #0]
 800ec1e:	3301      	adds	r3, #1
 800ec20:	9300      	str	r3, [sp, #0]
 800ec22:	f640 53fd 	movw	r3, #3581	; 0xdfd
 800ec26:	9a00      	ldr	r2, [sp, #0]
 800ec28:	429a      	cmp	r2, r3
 800ec2a:	f47f ae55 	bne.w	800e8d8 <Save_Csv+0x68>
 800ec2e:	b1de      	cbz	r6, 800ec68 <Save_Csv+0x3f8>
 800ec30:	230d      	movs	r3, #13
 800ec32:	55bb      	strb	r3, [r7, r6]
 800ec34:	230a      	movs	r3, #10
 800ec36:	1cb0      	adds	r0, r6, #2
 800ec38:	f5c0 5280 	rsb	r2, r0, #4096	; 0x1000
 800ec3c:	2120      	movs	r1, #32
 800ec3e:	443e      	add	r6, r7
 800ec40:	4438      	add	r0, r7
 800ec42:	7073      	strb	r3, [r6, #1]
 800ec44:	f003 ff79 	bl	8012b3a <memset>
 800ec48:	a904      	add	r1, sp, #16
 800ec4a:	4817      	ldr	r0, [pc, #92]	; (800eca8 <Save_Csv+0x438>)
 800ec4c:	f000 fbe8 	bl	800f420 <ProgFileSec>
 800ec50:	2800      	cmp	r0, #0
 800ec52:	f47f ae66 	bne.w	800e922 <Save_Csv+0xb2>
 800ec56:	f3c4 0241 	ubfx	r2, r4, #1, #2
 800ec5a:	1c65      	adds	r5, r4, #1
 800ec5c:	21e1      	movs	r1, #225	; 0xe1
 800ec5e:	f240 102b 	movw	r0, #299	; 0x12b
 800ec62:	f7ff fd17 	bl	800e694 <PrintClk>
 800ec66:	462c      	mov	r4, r5
 800ec68:	0321      	lsls	r1, r4, #12
 800ec6a:	ab03      	add	r3, sp, #12
 800ec6c:	aa04      	add	r2, sp, #16
 800ec6e:	480e      	ldr	r0, [pc, #56]	; (800eca8 <Save_Csv+0x438>)
 800ec70:	f000 fc72 	bl	800f558 <CloseFile>
 800ec74:	4604      	mov	r4, r0
 800ec76:	b9a8      	cbnz	r0, 800eca4 <Save_Csv+0x434>
 800ec78:	4a0c      	ldr	r2, [pc, #48]	; (800ecac <Save_Csv+0x43c>)
 800ec7a:	2001      	movs	r0, #1
 800ec7c:	f8b2 3066 	ldrh.w	r3, [r2, #102]	; 0x66
 800ec80:	2b62      	cmp	r3, #98	; 0x62
 800ec82:	bf9c      	itt	ls
 800ec84:	3301      	addls	r3, #1
 800ec86:	f8a2 3066 	strhls.w	r3, [r2, #102]	; 0x66
 800ec8a:	f000 faab 	bl	800f1e4 <USB_Connect>
 800ec8e:	e649      	b.n	800e924 <Save_Csv+0xb4>
 800ec90:	461e      	mov	r6, r3
 800ec92:	e660      	b.n	800e956 <Save_Csv+0xe6>
 800ec94:	4613      	mov	r3, r2
 800ec96:	e6a4      	b.n	800e9e2 <Save_Csv+0x172>
 800ec98:	461e      	mov	r6, r3
 800ec9a:	e6e5      	b.n	800ea68 <Save_Csv+0x1f8>
 800ec9c:	4613      	mov	r3, r2
 800ec9e:	e72b      	b.n	800eaf8 <Save_Csv+0x288>
 800eca0:	461e      	mov	r6, r3
 800eca2:	e76c      	b.n	800eb7e <Save_Csv+0x30e>
 800eca4:	2404      	movs	r4, #4
 800eca6:	e63d      	b.n	800e924 <Save_Csv+0xb4>
 800eca8:	200084a4 	.word	0x200084a4
 800ecac:	2000000c 	.word	0x2000000c

0800ecb0 <Clear_File_ICO>:
 800ecb0:	b51f      	push	{r0, r1, r2, r3, r4, lr}
 800ecb2:	4b06      	ldr	r3, [pc, #24]	; (800eccc <Clear_File_ICO+0x1c>)
 800ecb4:	9302      	str	r3, [sp, #8]
 800ecb6:	2300      	movs	r3, #0
 800ecb8:	e9cd 3300 	strd	r3, r3, [sp]
 800ecbc:	f647 33ef 	movw	r3, #31727	; 0x7bef
 800ecc0:	461a      	mov	r2, r3
 800ecc2:	f7fd f861 	bl	800bd88 <Disp_Str6x8>
 800ecc6:	b005      	add	sp, #20
 800ecc8:	f85d fb04 	ldr.w	pc, [sp], #4
 800eccc:	08012bc5 	.word	0x08012bc5

0800ecd0 <DispFileInfo>:
 800ecd0:	b5f0      	push	{r4, r5, r6, r7, lr}
 800ecd2:	b08b      	sub	sp, #44	; 0x2c
 800ecd4:	aa03      	add	r2, sp, #12
 800ecd6:	4604      	mov	r4, r0
 800ecd8:	4616      	mov	r6, r2
 800ecda:	4b11      	ldr	r3, [pc, #68]	; (800ed20 <DispFileInfo+0x50>)
 800ecdc:	f103 0718 	add.w	r7, r3, #24
 800ece0:	4615      	mov	r5, r2
 800ece2:	6818      	ldr	r0, [r3, #0]
 800ece4:	6859      	ldr	r1, [r3, #4]
 800ece6:	3308      	adds	r3, #8
 800ece8:	c503      	stmia	r5!, {r0, r1}
 800ecea:	42bb      	cmp	r3, r7
 800ecec:	462a      	mov	r2, r5
 800ecee:	d1f7      	bne.n	800ece0 <DispFileInfo+0x10>
 800ecf0:	781b      	ldrb	r3, [r3, #0]
 800ecf2:	211f      	movs	r1, #31
 800ecf4:	702b      	strb	r3, [r5, #0]
 800ecf6:	f647 30ef 	movw	r0, #31727	; 0x7bef
 800ecfa:	f7fd f9bc 	bl	800c076 <Set_Color>
 800ecfe:	2300      	movs	r3, #0
 800ed00:	eb04 0484 	add.w	r4, r4, r4, lsl #2
 800ed04:	4434      	add	r4, r6
 800ed06:	461a      	mov	r2, r3
 800ed08:	21e1      	movs	r1, #225	; 0xe1
 800ed0a:	9400      	str	r4, [sp, #0]
 800ed0c:	f240 102b 	movw	r0, #299	; 0x12b
 800ed10:	f7fd f9b8 	bl	800c084 <Show_Str6x8>
 800ed14:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
 800ed18:	f7fd fac8 	bl	800c2ac <Delay_mS>
 800ed1c:	b00b      	add	sp, #44	; 0x2c
 800ed1e:	bdf0      	pop	{r4, r5, r6, r7, pc}
 800ed20:	080146b8 	.word	0x080146b8

0800ed24 <SaveParameter>:
 800ed24:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 800ed26:	f001 f9d7 	bl	80100d8 <FLASH_Unlock>
 800ed2a:	483b      	ldr	r0, [pc, #236]	; (800ee18 <SaveParameter+0xf4>)
 800ed2c:	f001 fa63 	bl	80101f6 <FLASH_ErasePage>
 800ed30:	b286      	uxth	r6, r0
 800ed32:	2e04      	cmp	r6, #4
 800ed34:	d169      	bne.n	800ee0a <SaveParameter+0xe6>
 800ed36:	f04f 024c 	mov.w	r2, #76	; 0x4c
 800ed3a:	4c38      	ldr	r4, [pc, #224]	; (800ee1c <SaveParameter+0xf8>)
 800ed3c:	4838      	ldr	r0, [pc, #224]	; (800ee20 <SaveParameter+0xfc>)
 800ed3e:	8022      	strh	r2, [r4, #0]
 800ed40:	f04f 0241 	mov.w	r2, #65	; 0x41
 800ed44:	8062      	strh	r2, [r4, #2]
 800ed46:	f04f 0231 	mov.w	r2, #49	; 0x31
 800ed4a:	80a2      	strh	r2, [r4, #4]
 800ed4c:	f04f 0230 	mov.w	r2, #48	; 0x30
 800ed50:	80e2      	strh	r2, [r4, #6]
 800ed52:	f04f 0234 	mov.w	r2, #52	; 0x34
 800ed56:	8122      	strh	r2, [r4, #8]
 800ed58:	f44f 7280 	mov.w	r2, #256	; 0x100
 800ed5c:	8162      	strh	r2, [r4, #10]
 800ed5e:	4a31      	ldr	r2, [pc, #196]	; (800ee24 <SaveParameter+0x100>)
 800ed60:	7892      	ldrb	r2, [r2, #2]
 800ed62:	81a2      	strh	r2, [r4, #12]
 800ed64:	2200      	movs	r2, #0
 800ed66:	eb04 0342 	add.w	r3, r4, r2, lsl #1
 800ed6a:	5c11      	ldrb	r1, [r2, r0]
 800ed6c:	3201      	adds	r2, #1
 800ed6e:	2a07      	cmp	r2, #7
 800ed70:	81d9      	strh	r1, [r3, #14]
 800ed72:	d1f8      	bne.n	800ed66 <SaveParameter+0x42>
 800ed74:	2300      	movs	r3, #0
 800ed76:	482c      	ldr	r0, [pc, #176]	; (800ee28 <SaveParameter+0x104>)
 800ed78:	eb04 0243 	add.w	r2, r4, r3, lsl #1
 800ed7c:	5c19      	ldrb	r1, [r3, r0]
 800ed7e:	3301      	adds	r3, #1
 800ed80:	2b04      	cmp	r3, #4
 800ed82:	8391      	strh	r1, [r2, #28]
 800ed84:	d1f8      	bne.n	800ed78 <SaveParameter+0x54>
 800ed86:	2300      	movs	r3, #0
 800ed88:	4a28      	ldr	r2, [pc, #160]	; (800ee2c <SaveParameter+0x108>)
 800ed8a:	18e1      	adds	r1, r4, r3
 800ed8c:	f812 0f01 	ldrb.w	r0, [r2, #1]!
 800ed90:	3302      	adds	r3, #2
 800ed92:	2b08      	cmp	r3, #8
 800ed94:	8488      	strh	r0, [r1, #36]	; 0x24
 800ed96:	d1f8      	bne.n	800ed8a <SaveParameter+0x66>
 800ed98:	2300      	movs	r3, #0
 800ed9a:	4825      	ldr	r0, [pc, #148]	; (800ee30 <SaveParameter+0x10c>)
 800ed9c:	18e2      	adds	r2, r4, r3
 800ed9e:	5a19      	ldrh	r1, [r3, r0]
 800eda0:	3302      	adds	r3, #2
 800eda2:	2b8c      	cmp	r3, #140	; 0x8c
 800eda4:	8591      	strh	r1, [r2, #44]	; 0x2c
 800eda6:	d1f9      	bne.n	800ed9c <SaveParameter+0x78>
 800eda8:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
 800edac:	4921      	ldr	r1, [pc, #132]	; (800ee34 <SaveParameter+0x110>)
 800edae:	4d22      	ldr	r5, [pc, #136]	; (800ee38 <SaveParameter+0x114>)
 800edb0:	4e22      	ldr	r6, [pc, #136]	; (800ee3c <SaveParameter+0x118>)
 800edb2:	f1a1 0310 	sub.w	r3, r1, #16
 800edb6:	1950      	adds	r0, r2, r5
 800edb8:	f810 7f01 	ldrb.w	r7, [r0, #1]!
 800edbc:	f823 7b02 	strh.w	r7, [r3], #2
 800edc0:	4299      	cmp	r1, r3
 800edc2:	d1f9      	bne.n	800edb8 <SaveParameter+0x94>
 800edc4:	1990      	adds	r0, r2, r6
 800edc6:	f101 0710 	add.w	r7, r1, #16
 800edca:	f810 cf01 	ldrb.w	ip, [r0, #1]!
 800edce:	f823 cb02 	strh.w	ip, [r3], #2
 800edd2:	42bb      	cmp	r3, r7
 800edd4:	d1f9      	bne.n	800edca <SaveParameter+0xa6>
 800edd6:	3208      	adds	r2, #8
 800edd8:	2a1f      	cmp	r2, #31
 800edda:	f101 0120 	add.w	r1, r1, #32
 800edde:	d1e8      	bne.n	800edb2 <SaveParameter+0x8e>
 800ede0:	f64a 2355 	movw	r3, #43605	; 0xaa55
 800ede4:	4d0c      	ldr	r5, [pc, #48]	; (800ee18 <SaveParameter+0xf4>)
 800ede6:	4f16      	ldr	r7, [pc, #88]	; (800ee40 <SaveParameter+0x11c>)
 800ede8:	f8a4 3138 	strh.w	r3, [r4, #312]	; 0x138
 800edec:	1963      	adds	r3, r4, r5
 800edee:	f103 4378 	add.w	r3, r3, #4160749568	; 0xf8000000
 800edf2:	f5a3 43f0 	sub.w	r3, r3, #30720	; 0x7800
 800edf6:	8819      	ldrh	r1, [r3, #0]
 800edf8:	4628      	mov	r0, r5
 800edfa:	f001 faa7 	bl	801034c <FLASH_ProgramHalfWord>
 800edfe:	b286      	uxth	r6, r0
 800ee00:	2e04      	cmp	r6, #4
 800ee02:	d102      	bne.n	800ee0a <SaveParameter+0xe6>
 800ee04:	3502      	adds	r5, #2
 800ee06:	42bd      	cmp	r5, r7
 800ee08:	d1f0      	bne.n	800edec <SaveParameter+0xc8>
 800ee0a:	f001 f973 	bl	80100f4 <FLASH_Lock>
 800ee0e:	2e04      	cmp	r6, #4
 800ee10:	bf14      	ite	ne
 800ee12:	2003      	movne	r0, #3
 800ee14:	2000      	moveq	r0, #0
 800ee16:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 800ee18:	08007800 	.word	0x08007800
 800ee1c:	200084a4 	.word	0x200084a4
 800ee20:	20000244 	.word	0x20000244
 800ee24:	20004c12 	.word	0x20004c12
 800ee28:	20000240 	.word	0x20000240
 800ee2c:	2000025a 	.word	0x2000025a
 800ee30:	2000000c 	.word	0x2000000c
 800ee34:	2000856c 	.word	0x2000856c
 800ee38:	20000098 	.word	0x20000098
 800ee3c:	200000b8 	.word	0x200000b8
 800ee40:	08007a00 	.word	0x08007a00

0800ee44 <ReadParameter>:
 800ee44:	4b36      	ldr	r3, [pc, #216]	; (800ef20 <ReadParameter+0xdc>)
 800ee46:	b5f0      	push	{r4, r5, r6, r7, lr}
 800ee48:	881b      	ldrh	r3, [r3, #0]
 800ee4a:	2b4c      	cmp	r3, #76	; 0x4c
 800ee4c:	d165      	bne.n	800ef1a <ReadParameter+0xd6>
 800ee4e:	4b35      	ldr	r3, [pc, #212]	; (800ef24 <ReadParameter+0xe0>)
 800ee50:	881b      	ldrh	r3, [r3, #0]
 800ee52:	2b41      	cmp	r3, #65	; 0x41
 800ee54:	d161      	bne.n	800ef1a <ReadParameter+0xd6>
 800ee56:	4b34      	ldr	r3, [pc, #208]	; (800ef28 <ReadParameter+0xe4>)
 800ee58:	881b      	ldrh	r3, [r3, #0]
 800ee5a:	2b31      	cmp	r3, #49	; 0x31
 800ee5c:	d15d      	bne.n	800ef1a <ReadParameter+0xd6>
 800ee5e:	4b33      	ldr	r3, [pc, #204]	; (800ef2c <ReadParameter+0xe8>)
 800ee60:	881b      	ldrh	r3, [r3, #0]
 800ee62:	2b30      	cmp	r3, #48	; 0x30
 800ee64:	d159      	bne.n	800ef1a <ReadParameter+0xd6>
 800ee66:	4b32      	ldr	r3, [pc, #200]	; (800ef30 <ReadParameter+0xec>)
 800ee68:	881b      	ldrh	r3, [r3, #0]
 800ee6a:	2b34      	cmp	r3, #52	; 0x34
 800ee6c:	d155      	bne.n	800ef1a <ReadParameter+0xd6>
 800ee6e:	4b31      	ldr	r3, [pc, #196]	; (800ef34 <ReadParameter+0xf0>)
 800ee70:	881b      	ldrh	r3, [r3, #0]
 800ee72:	f5b3 7f80 	cmp.w	r3, #256	; 0x100
 800ee76:	d150      	bne.n	800ef1a <ReadParameter+0xd6>
 800ee78:	4b2f      	ldr	r3, [pc, #188]	; (800ef38 <ReadParameter+0xf4>)
 800ee7a:	4930      	ldr	r1, [pc, #192]	; (800ef3c <ReadParameter+0xf8>)
 800ee7c:	881a      	ldrh	r2, [r3, #0]
 800ee7e:	4b30      	ldr	r3, [pc, #192]	; (800ef40 <ReadParameter+0xfc>)
 800ee80:	709a      	strb	r2, [r3, #2]
 800ee82:	4a30      	ldr	r2, [pc, #192]	; (800ef44 <ReadParameter+0x100>)
 800ee84:	4b30      	ldr	r3, [pc, #192]	; (800ef48 <ReadParameter+0x104>)
 800ee86:	8818      	ldrh	r0, [r3, #0]
 800ee88:	3302      	adds	r3, #2
 800ee8a:	428b      	cmp	r3, r1
 800ee8c:	f802 0f01 	strb.w	r0, [r2, #1]!
 800ee90:	d1f9      	bne.n	800ee86 <ReadParameter+0x42>
 800ee92:	4a2e      	ldr	r2, [pc, #184]	; (800ef4c <ReadParameter+0x108>)
 800ee94:	881b      	ldrh	r3, [r3, #0]
 800ee96:	4d2e      	ldr	r5, [pc, #184]	; (800ef50 <ReadParameter+0x10c>)
 800ee98:	7013      	strb	r3, [r2, #0]
 800ee9a:	4b2e      	ldr	r3, [pc, #184]	; (800ef54 <ReadParameter+0x110>)
 800ee9c:	482e      	ldr	r0, [pc, #184]	; (800ef58 <ReadParameter+0x114>)
 800ee9e:	881b      	ldrh	r3, [r3, #0]
 800eea0:	492e      	ldr	r1, [pc, #184]	; (800ef5c <ReadParameter+0x118>)
 800eea2:	7053      	strb	r3, [r2, #1]
 800eea4:	4b2e      	ldr	r3, [pc, #184]	; (800ef60 <ReadParameter+0x11c>)
 800eea6:	881b      	ldrh	r3, [r3, #0]
 800eea8:	7093      	strb	r3, [r2, #2]
 800eeaa:	4b2e      	ldr	r3, [pc, #184]	; (800ef64 <ReadParameter+0x120>)
 800eeac:	881b      	ldrh	r3, [r3, #0]
 800eeae:	70d3      	strb	r3, [r2, #3]
 800eeb0:	4a2d      	ldr	r2, [pc, #180]	; (800ef68 <ReadParameter+0x124>)
 800eeb2:	4b2e      	ldr	r3, [pc, #184]	; (800ef6c <ReadParameter+0x128>)
 800eeb4:	8812      	ldrh	r2, [r2, #0]
 800eeb6:	741a      	strb	r2, [r3, #16]
 800eeb8:	4a2d      	ldr	r2, [pc, #180]	; (800ef70 <ReadParameter+0x12c>)
 800eeba:	8812      	ldrh	r2, [r2, #0]
 800eebc:	745a      	strb	r2, [r3, #17]
 800eebe:	4a2d      	ldr	r2, [pc, #180]	; (800ef74 <ReadParameter+0x130>)
 800eec0:	8812      	ldrh	r2, [r2, #0]
 800eec2:	749a      	strb	r2, [r3, #18]
 800eec4:	4a2c      	ldr	r2, [pc, #176]	; (800ef78 <ReadParameter+0x134>)
 800eec6:	8812      	ldrh	r2, [r2, #0]
 800eec8:	74da      	strb	r2, [r3, #19]
 800eeca:	4b2c      	ldr	r3, [pc, #176]	; (800ef7c <ReadParameter+0x138>)
 800eecc:	1c9a      	adds	r2, r3, #2
 800eece:	881c      	ldrh	r4, [r3, #0]
 800eed0:	428a      	cmp	r2, r1
 800eed2:	eb02 0305 	add.w	r3, r2, r5
 800eed6:	521c      	strh	r4, [r3, r0]
 800eed8:	4613      	mov	r3, r2
 800eeda:	d1f7      	bne.n	800eecc <ReadParameter+0x88>
 800eedc:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
 800eee0:	4a27      	ldr	r2, [pc, #156]	; (800ef80 <ReadParameter+0x13c>)
 800eee2:	4c28      	ldr	r4, [pc, #160]	; (800ef84 <ReadParameter+0x140>)
 800eee4:	4d28      	ldr	r5, [pc, #160]	; (800ef88 <ReadParameter+0x144>)
 800eee6:	4e29      	ldr	r6, [pc, #164]	; (800ef8c <ReadParameter+0x148>)
 800eee8:	f1a2 0310 	sub.w	r3, r2, #16
 800eeec:	1908      	adds	r0, r1, r4
 800eeee:	f833 7b02 	ldrh.w	r7, [r3], #2
 800eef2:	4293      	cmp	r3, r2
 800eef4:	f800 7f01 	strb.w	r7, [r0, #1]!
 800eef8:	d1f9      	bne.n	800eeee <ReadParameter+0xaa>
 800eefa:	1948      	adds	r0, r1, r5
 800eefc:	f103 0710 	add.w	r7, r3, #16
 800ef00:	f833 cb02 	ldrh.w	ip, [r3], #2
 800ef04:	42bb      	cmp	r3, r7
 800ef06:	f800 cf01 	strb.w	ip, [r0, #1]!
 800ef0a:	d1f9      	bne.n	800ef00 <ReadParameter+0xbc>
 800ef0c:	3220      	adds	r2, #32
 800ef0e:	42b2      	cmp	r2, r6
 800ef10:	f101 0108 	add.w	r1, r1, #8
 800ef14:	d1e8      	bne.n	800eee8 <ReadParameter+0xa4>
 800ef16:	2001      	movs	r0, #1
 800ef18:	e000      	b.n	800ef1c <ReadParameter+0xd8>
 800ef1a:	2000      	movs	r0, #0
 800ef1c:	bdf0      	pop	{r4, r5, r6, r7, pc}
 800ef1e:	bf00      	nop
 800ef20:	08007800 	.word	0x08007800
 800ef24:	08007802 	.word	0x08007802
 800ef28:	08007804 	.word	0x08007804
 800ef2c:	08007806 	.word	0x08007806
 800ef30:	08007808 	.word	0x08007808
 800ef34:	0800780a 	.word	0x0800780a
 800ef38:	0800780c 	.word	0x0800780c
 800ef3c:	0800781c 	.word	0x0800781c
 800ef40:	20004c12 	.word	0x20004c12
 800ef44:	20000243 	.word	0x20000243
 800ef48:	0800780e 	.word	0x0800780e
 800ef4c:	20000240 	.word	0x20000240
 800ef50:	f7ff87d2 	.word	0xf7ff87d2
 800ef54:	0800781e 	.word	0x0800781e
 800ef58:	2000000c 	.word	0x2000000c
 800ef5c:	080078b8 	.word	0x080078b8
 800ef60:	08007820 	.word	0x08007820
 800ef64:	08007822 	.word	0x08007822
 800ef68:	08007824 	.word	0x08007824
 800ef6c:	2000024b 	.word	0x2000024b
 800ef70:	08007826 	.word	0x08007826
 800ef74:	08007828 	.word	0x08007828
 800ef78:	0800782a 	.word	0x0800782a
 800ef7c:	0800782c 	.word	0x0800782c
 800ef80:	080078c8 	.word	0x080078c8
 800ef84:	20000098 	.word	0x20000098
 800ef88:	200000b8 	.word	0x200000b8
 800ef8c:	08007948 	.word	0x08007948

0800ef90 <RestoreParameter>:
 800ef90:	2301      	movs	r3, #1
 800ef92:	4923      	ldr	r1, [pc, #140]	; (800f020 <RestoreParameter+0x90>)
 800ef94:	b5f0      	push	{r4, r5, r6, r7, lr}
 800ef96:	700b      	strb	r3, [r1, #0]
 800ef98:	2300      	movs	r3, #0
 800ef9a:	4a22      	ldr	r2, [pc, #136]	; (800f024 <RestoreParameter+0x94>)
 800ef9c:	4822      	ldr	r0, [pc, #136]	; (800f028 <RestoreParameter+0x98>)
 800ef9e:	7013      	strb	r3, [r2, #0]
 800efa0:	7093      	strb	r3, [r2, #2]
 800efa2:	4a22      	ldr	r2, [pc, #136]	; (800f02c <RestoreParameter+0x9c>)
 800efa4:	708b      	strb	r3, [r1, #2]
 800efa6:	5c19      	ldrb	r1, [r3, r0]
 800efa8:	5499      	strb	r1, [r3, r2]
 800efaa:	3301      	adds	r3, #1
 800efac:	2b07      	cmp	r3, #7
 800efae:	d1fa      	bne.n	800efa6 <RestoreParameter+0x16>
 800efb0:	4a1f      	ldr	r2, [pc, #124]	; (800f030 <RestoreParameter+0xa0>)
 800efb2:	4b20      	ldr	r3, [pc, #128]	; (800f034 <RestoreParameter+0xa4>)
 800efb4:	7811      	ldrb	r1, [r2, #0]
 800efb6:	4820      	ldr	r0, [pc, #128]	; (800f038 <RestoreParameter+0xa8>)
 800efb8:	7019      	strb	r1, [r3, #0]
 800efba:	7851      	ldrb	r1, [r2, #1]
 800efbc:	7059      	strb	r1, [r3, #1]
 800efbe:	7891      	ldrb	r1, [r2, #2]
 800efc0:	78d2      	ldrb	r2, [r2, #3]
 800efc2:	7099      	strb	r1, [r3, #2]
 800efc4:	70da      	strb	r2, [r3, #3]
 800efc6:	4a1d      	ldr	r2, [pc, #116]	; (800f03c <RestoreParameter+0xac>)
 800efc8:	4b1d      	ldr	r3, [pc, #116]	; (800f040 <RestoreParameter+0xb0>)
 800efca:	7c11      	ldrb	r1, [r2, #16]
 800efcc:	7419      	strb	r1, [r3, #16]
 800efce:	7c51      	ldrb	r1, [r2, #17]
 800efd0:	7459      	strb	r1, [r3, #17]
 800efd2:	7c91      	ldrb	r1, [r2, #18]
 800efd4:	7cd2      	ldrb	r2, [r2, #19]
 800efd6:	7499      	strb	r1, [r3, #18]
 800efd8:	74da      	strb	r2, [r3, #19]
 800efda:	2300      	movs	r3, #0
 800efdc:	4a19      	ldr	r2, [pc, #100]	; (800f044 <RestoreParameter+0xb4>)
 800efde:	f830 1013 	ldrh.w	r1, [r0, r3, lsl #1]
 800efe2:	f822 1013 	strh.w	r1, [r2, r3, lsl #1]
 800efe6:	3301      	adds	r3, #1
 800efe8:	2b46      	cmp	r3, #70	; 0x46
 800efea:	d1f8      	bne.n	800efde <RestoreParameter+0x4e>
 800efec:	2300      	movs	r3, #0
 800efee:	4916      	ldr	r1, [pc, #88]	; (800f048 <RestoreParameter+0xb8>)
 800eff0:	4816      	ldr	r0, [pc, #88]	; (800f04c <RestoreParameter+0xbc>)
 800eff2:	4c17      	ldr	r4, [pc, #92]	; (800f050 <RestoreParameter+0xc0>)
 800eff4:	4d17      	ldr	r5, [pc, #92]	; (800f054 <RestoreParameter+0xc4>)
 800eff6:	2200      	movs	r2, #0
 800eff8:	1817      	adds	r7, r2, r0
 800effa:	1856      	adds	r6, r2, r1
 800effc:	5cff      	ldrb	r7, [r7, r3]
 800effe:	3201      	adds	r2, #1
 800f000:	2a08      	cmp	r2, #8
 800f002:	54f7      	strb	r7, [r6, r3]
 800f004:	d1f8      	bne.n	800eff8 <RestoreParameter+0x68>
 800f006:	2200      	movs	r2, #0
 800f008:	1957      	adds	r7, r2, r5
 800f00a:	1916      	adds	r6, r2, r4
 800f00c:	5cff      	ldrb	r7, [r7, r3]
 800f00e:	3201      	adds	r2, #1
 800f010:	2a08      	cmp	r2, #8
 800f012:	54f7      	strb	r7, [r6, r3]
 800f014:	d1f8      	bne.n	800f008 <RestoreParameter+0x78>
 800f016:	3308      	adds	r3, #8
 800f018:	2b20      	cmp	r3, #32
 800f01a:	d1ec      	bne.n	800eff6 <RestoreParameter+0x66>
 800f01c:	2000      	movs	r0, #0
 800f01e:	bdf0      	pop	{r4, r5, r6, r7, pc}
 800f020:	20004c12 	.word	0x20004c12
 800f024:	20004c0f 	.word	0x20004c0f
 800f028:	0801376a 	.word	0x0801376a
 800f02c:	20000244 	.word	0x20000244
 800f030:	08013766 	.word	0x08013766
 800f034:	20000240 	.word	0x20000240
 800f038:	08013696 	.word	0x08013696
 800f03c:	08013762 	.word	0x08013762
 800f040:	2000024b 	.word	0x2000024b
 800f044:	2000000c 	.word	0x2000000c
 800f048:	20000098 	.word	0x20000098
 800f04c:	08013742 	.word	0x08013742
 800f050:	200000b8 	.word	0x200000b8
 800f054:	08013722 	.word	0x08013722

0800f058 <Disk_Init>:
 800f058:	4b09      	ldr	r3, [pc, #36]	; (800f080 <Disk_Init+0x28>)
 800f05a:	4a0a      	ldr	r2, [pc, #40]	; (800f084 <Disk_Init+0x2c>)
 800f05c:	f503 5180 	add.w	r1, r3, #4096	; 0x1000
 800f060:	6051      	str	r1, [r2, #4]
 800f062:	f503 5181 	add.w	r1, r3, #4128	; 0x1020
 800f066:	6091      	str	r1, [r2, #8]
 800f068:	3109      	adds	r1, #9
 800f06a:	60d1      	str	r1, [r2, #12]
 800f06c:	6013      	str	r3, [r2, #0]
 800f06e:	f241 011c 	movw	r1, #4124	; 0x101c
 800f072:	2200      	movs	r2, #0
 800f074:	505a      	str	r2, [r3, r1]
 800f076:	f241 0118 	movw	r1, #4120	; 0x1018
 800f07a:	505a      	str	r2, [r3, r1]
 800f07c:	4770      	bx	lr
 800f07e:	bf00      	nop
 800f080:	200084a4 	.word	0x200084a4
 800f084:	20008480 	.word	0x20008480

0800f088 <Disk_SecWr>:
 800f088:	f000 bce4 	b.w	800fa54 <ExtFlashSecWr>

0800f08c <Disk_SecRd>:
 800f08c:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 800f090:	f000 bcf5 	b.w	800fa7e <ExtFlashDataRd>

0800f094 <Read_Memory>:
 800f094:	b510      	push	{r4, lr}
 800f096:	4c25      	ldr	r4, [pc, #148]	; (800f12c <Read_Memory+0x98>)
 800f098:	68a3      	ldr	r3, [r4, #8]
 800f09a:	781a      	ldrb	r2, [r3, #0]
 800f09c:	b92a      	cbnz	r2, 800f0aa <Read_Memory+0x16>
 800f09e:	2201      	movs	r2, #1
 800f0a0:	0300      	lsls	r0, r0, #12
 800f0a2:	0309      	lsls	r1, r1, #12
 800f0a4:	6120      	str	r0, [r4, #16]
 800f0a6:	6161      	str	r1, [r4, #20]
 800f0a8:	701a      	strb	r2, [r3, #0]
 800f0aa:	68a3      	ldr	r3, [r4, #8]
 800f0ac:	781b      	ldrb	r3, [r3, #0]
 800f0ae:	2b01      	cmp	r3, #1
 800f0b0:	d123      	bne.n	800f0fa <Read_Memory+0x66>
 800f0b2:	e9d4 0300 	ldrd	r0, r3, [r4]
 800f0b6:	699b      	ldr	r3, [r3, #24]
 800f0b8:	bb5b      	cbnz	r3, 800f112 <Read_Memory+0x7e>
 800f0ba:	6921      	ldr	r1, [r4, #16]
 800f0bc:	f7ff ffe6 	bl	800f08c <Disk_SecRd>
 800f0c0:	2240      	movs	r2, #64	; 0x40
 800f0c2:	2198      	movs	r1, #152	; 0x98
 800f0c4:	6820      	ldr	r0, [r4, #0]
 800f0c6:	f002 f94f 	bl	8011368 <UserToPMABufferCopy>
 800f0ca:	f44f 627c 	mov.w	r2, #4032	; 0xfc0
 800f0ce:	6863      	ldr	r3, [r4, #4]
 800f0d0:	619a      	str	r2, [r3, #24]
 800f0d2:	2340      	movs	r3, #64	; 0x40
 800f0d4:	2140      	movs	r1, #64	; 0x40
 800f0d6:	2001      	movs	r0, #1
 800f0d8:	61a3      	str	r3, [r4, #24]
 800f0da:	f001 ffbd 	bl	8011058 <SetEPTxCount>
 800f0de:	2130      	movs	r1, #48	; 0x30
 800f0e0:	2001      	movs	r0, #1
 800f0e2:	f001 fe45 	bl	8010d70 <SetEPTxStatus>
 800f0e6:	6923      	ldr	r3, [r4, #16]
 800f0e8:	4a11      	ldr	r2, [pc, #68]	; (800f130 <Read_Memory+0x9c>)
 800f0ea:	3340      	adds	r3, #64	; 0x40
 800f0ec:	6123      	str	r3, [r4, #16]
 800f0ee:	6963      	ldr	r3, [r4, #20]
 800f0f0:	3b40      	subs	r3, #64	; 0x40
 800f0f2:	6163      	str	r3, [r4, #20]
 800f0f4:	6893      	ldr	r3, [r2, #8]
 800f0f6:	3b40      	subs	r3, #64	; 0x40
 800f0f8:	6093      	str	r3, [r2, #8]
 800f0fa:	6963      	ldr	r3, [r4, #20]
 800f0fc:	b943      	cbnz	r3, 800f110 <Read_Memory+0x7c>
 800f0fe:	2103      	movs	r1, #3
 800f100:	6862      	ldr	r2, [r4, #4]
 800f102:	61a3      	str	r3, [r4, #24]
 800f104:	6193      	str	r3, [r2, #24]
 800f106:	4a0b      	ldr	r2, [pc, #44]	; (800f134 <Read_Memory+0xa0>)
 800f108:	6123      	str	r3, [r4, #16]
 800f10a:	7011      	strb	r1, [r2, #0]
 800f10c:	68a2      	ldr	r2, [r4, #8]
 800f10e:	7013      	strb	r3, [r2, #0]
 800f110:	bd10      	pop	{r4, pc}
 800f112:	69a3      	ldr	r3, [r4, #24]
 800f114:	2240      	movs	r2, #64	; 0x40
 800f116:	4418      	add	r0, r3
 800f118:	2198      	movs	r1, #152	; 0x98
 800f11a:	f002 f925 	bl	8011368 <UserToPMABufferCopy>
 800f11e:	6862      	ldr	r2, [r4, #4]
 800f120:	6993      	ldr	r3, [r2, #24]
 800f122:	3b40      	subs	r3, #64	; 0x40
 800f124:	6193      	str	r3, [r2, #24]
 800f126:	69a3      	ldr	r3, [r4, #24]
 800f128:	3340      	adds	r3, #64	; 0x40
 800f12a:	e7d3      	b.n	800f0d4 <Read_Memory+0x40>
 800f12c:	20008480 	.word	0x20008480
 800f130:	20009538 	.word	0x20009538
 800f134:	20009534 	.word	0x20009534

0800f138 <Write_Memory>:
 800f138:	b538      	push	{r3, r4, r5, lr}
 800f13a:	4c25      	ldr	r4, [pc, #148]	; (800f1d0 <Write_Memory+0x98>)
 800f13c:	68a2      	ldr	r2, [r4, #8]
 800f13e:	6863      	ldr	r3, [r4, #4]
 800f140:	7815      	ldrb	r5, [r2, #0]
 800f142:	69db      	ldr	r3, [r3, #28]
 800f144:	b92d      	cbnz	r5, 800f152 <Write_Memory+0x1a>
 800f146:	0309      	lsls	r1, r1, #12
 800f148:	6221      	str	r1, [r4, #32]
 800f14a:	2101      	movs	r1, #1
 800f14c:	0300      	lsls	r0, r0, #12
 800f14e:	61e0      	str	r0, [r4, #28]
 800f150:	7011      	strb	r1, [r2, #0]
 800f152:	68a2      	ldr	r2, [r4, #8]
 800f154:	7812      	ldrb	r2, [r2, #0]
 800f156:	2a01      	cmp	r2, #1
 800f158:	d121      	bne.n	800f19e <Write_Memory+0x66>
 800f15a:	491e      	ldr	r1, [pc, #120]	; (800f1d4 <Write_Memory+0x9c>)
 800f15c:	3340      	adds	r3, #64	; 0x40
 800f15e:	6860      	ldr	r0, [r4, #4]
 800f160:	69c2      	ldr	r2, [r0, #28]
 800f162:	429a      	cmp	r2, r3
 800f164:	d32b      	bcc.n	800f1be <Write_Memory+0x86>
 800f166:	4a1c      	ldr	r2, [pc, #112]	; (800f1d8 <Write_Memory+0xa0>)
 800f168:	6a23      	ldr	r3, [r4, #32]
 800f16a:	8815      	ldrh	r5, [r2, #0]
 800f16c:	69e1      	ldr	r1, [r4, #28]
 800f16e:	1b5b      	subs	r3, r3, r5
 800f170:	4429      	add	r1, r5
 800f172:	6223      	str	r3, [r4, #32]
 800f174:	f3c3 030b 	ubfx	r3, r3, #0, #12
 800f178:	61e1      	str	r1, [r4, #28]
 800f17a:	4615      	mov	r5, r2
 800f17c:	b92b      	cbnz	r3, 800f18a <Write_Memory+0x52>
 800f17e:	61c3      	str	r3, [r0, #28]
 800f180:	f5a1 5180 	sub.w	r1, r1, #4096	; 0x1000
 800f184:	6820      	ldr	r0, [r4, #0]
 800f186:	f000 fc65 	bl	800fa54 <ExtFlashSecWr>
 800f18a:	4a14      	ldr	r2, [pc, #80]	; (800f1dc <Write_Memory+0xa4>)
 800f18c:	8829      	ldrh	r1, [r5, #0]
 800f18e:	6893      	ldr	r3, [r2, #8]
 800f190:	2002      	movs	r0, #2
 800f192:	1a5b      	subs	r3, r3, r1
 800f194:	f44f 5140 	mov.w	r1, #12288	; 0x3000
 800f198:	6093      	str	r3, [r2, #8]
 800f19a:	f001 fe02 	bl	8010da2 <SetEPRxStatus>
 800f19e:	6a23      	ldr	r3, [r4, #32]
 800f1a0:	b11b      	cbz	r3, 800f1aa <Write_Memory+0x72>
 800f1a2:	4b0f      	ldr	r3, [pc, #60]	; (800f1e0 <Write_Memory+0xa8>)
 800f1a4:	781b      	ldrb	r3, [r3, #0]
 800f1a6:	2b04      	cmp	r3, #4
 800f1a8:	d108      	bne.n	800f1bc <Write_Memory+0x84>
 800f1aa:	2500      	movs	r5, #0
 800f1ac:	6863      	ldr	r3, [r4, #4]
 800f1ae:	2101      	movs	r1, #1
 800f1b0:	61dd      	str	r5, [r3, #28]
 800f1b2:	4628      	mov	r0, r5
 800f1b4:	f001 faf4 	bl	80107a0 <Set_CSW>
 800f1b8:	68a3      	ldr	r3, [r4, #8]
 800f1ba:	701d      	strb	r5, [r3, #0]
 800f1bc:	bd38      	pop	{r3, r4, r5, pc}
 800f1be:	f811 5f01 	ldrb.w	r5, [r1, #1]!
 800f1c2:	6820      	ldr	r0, [r4, #0]
 800f1c4:	5485      	strb	r5, [r0, r2]
 800f1c6:	6860      	ldr	r0, [r4, #4]
 800f1c8:	69c2      	ldr	r2, [r0, #28]
 800f1ca:	3201      	adds	r2, #1
 800f1cc:	61c2      	str	r2, [r0, #28]
 800f1ce:	e7c6      	b.n	800f15e <Write_Memory+0x26>
 800f1d0:	20008480 	.word	0x20008480
 800f1d4:	20009571 	.word	0x20009571
 800f1d8:	20009570 	.word	0x20009570
 800f1dc:	20009538 	.word	0x20009538
 800f1e0:	20009534 	.word	0x20009534

0800f1e4 <USB_Connect>:
 800f1e4:	b508      	push	{r3, lr}
 800f1e6:	b980      	cbnz	r0, 800f20a <USB_Connect+0x26>
 800f1e8:	f002 f8f4 	bl	80113d4 <PowerOff>
 800f1ec:	2800      	cmp	r0, #0
 800f1ee:	d1fb      	bne.n	800f1e8 <USB_Connect+0x4>
 800f1f0:	20c8      	movs	r0, #200	; 0xc8
 800f1f2:	f7fd f85b 	bl	800c2ac <Delay_mS>
 800f1f6:	4b0b      	ldr	r3, [pc, #44]	; (800f224 <USB_Connect+0x40>)
 800f1f8:	685a      	ldr	r2, [r3, #4]
 800f1fa:	f442 324c 	orr.w	r2, r2, #208896	; 0x33000
 800f1fe:	605a      	str	r2, [r3, #4]
 800f200:	685a      	ldr	r2, [r3, #4]
 800f202:	f422 224c 	bic.w	r2, r2, #835584	; 0xcc000
 800f206:	605a      	str	r2, [r3, #4]
 800f208:	bd08      	pop	{r3, pc}
 800f20a:	4b06      	ldr	r3, [pc, #24]	; (800f224 <USB_Connect+0x40>)
 800f20c:	685a      	ldr	r2, [r3, #4]
 800f20e:	f442 223b 	orr.w	r2, r2, #765952	; 0xbb000
 800f212:	605a      	str	r2, [r3, #4]
 800f214:	685a      	ldr	r2, [r3, #4]
 800f216:	f422 2288 	bic.w	r2, r2, #278528	; 0x44000
 800f21a:	605a      	str	r2, [r3, #4]
 800f21c:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 800f220:	f001 ba86 	b.w	8010730 <USB_Init>
 800f224:	40010800 	.word	0x40010800

0800f228 <Init_Fat_Value>:
 800f228:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 800f22c:	f44f 5100 	mov.w	r1, #8192	; 0x2000
 800f230:	4b09      	ldr	r3, [pc, #36]	; (800f258 <Init_Fat_Value+0x30>)
 800f232:	f44f 5040 	mov.w	r0, #12288	; 0x3000
 800f236:	e9c3 2100 	strd	r2, r1, [r3]
 800f23a:	e9c3 2204 	strd	r2, r2, [r3, #16]
 800f23e:	f240 72ff 	movw	r2, #2047	; 0x7ff
 800f242:	f44f 41e0 	mov.w	r1, #28672	; 0x7000
 800f246:	619a      	str	r2, [r3, #24]
 800f248:	f240 1201 	movw	r2, #257	; 0x101
 800f24c:	e9c3 0102 	strd	r0, r1, [r3, #8]
 800f250:	839a      	strh	r2, [r3, #28]
 800f252:	2000      	movs	r0, #0
 800f254:	4770      	bx	lr
 800f256:	bf00      	nop
 800f258:	200094e0 	.word	0x200094e0

0800f25c <ReadDiskData>:
 800f25c:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 800f260:	4606      	mov	r6, r0
 800f262:	460f      	mov	r7, r1
 800f264:	4690      	mov	r8, r2
 800f266:	2408      	movs	r4, #8
 800f268:	f04f 0900 	mov.w	r9, #0
 800f26c:	4d09      	ldr	r5, [pc, #36]	; (800f294 <ReadDiskData+0x38>)
 800f26e:	4642      	mov	r2, r8
 800f270:	4639      	mov	r1, r7
 800f272:	4630      	mov	r0, r6
 800f274:	3c01      	subs	r4, #1
 800f276:	f885 9020 	strb.w	r9, [r5, #32]
 800f27a:	f000 fc00 	bl	800fa7e <ExtFlashDataRd>
 800f27e:	f014 04ff 	ands.w	r4, r4, #255	; 0xff
 800f282:	d005      	beq.n	800f290 <ReadDiskData+0x34>
 800f284:	f895 0020 	ldrb.w	r0, [r5, #32]
 800f288:	2800      	cmp	r0, #0
 800f28a:	d1f0      	bne.n	800f26e <ReadDiskData+0x12>
 800f28c:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 800f290:	2001      	movs	r0, #1
 800f292:	e7fb      	b.n	800f28c <ReadDiskData+0x30>
 800f294:	200094e0 	.word	0x200094e0

0800f298 <ProgDiskPage>:
 800f298:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800f29c:	4606      	mov	r6, r0
 800f29e:	460f      	mov	r7, r1
 800f2a0:	2408      	movs	r4, #8
 800f2a2:	f04f 0800 	mov.w	r8, #0
 800f2a6:	4d09      	ldr	r5, [pc, #36]	; (800f2cc <ProgDiskPage+0x34>)
 800f2a8:	4639      	mov	r1, r7
 800f2aa:	4630      	mov	r0, r6
 800f2ac:	3c01      	subs	r4, #1
 800f2ae:	f885 8020 	strb.w	r8, [r5, #32]
 800f2b2:	f000 fbcf 	bl	800fa54 <ExtFlashSecWr>
 800f2b6:	f014 04ff 	ands.w	r4, r4, #255	; 0xff
 800f2ba:	d005      	beq.n	800f2c8 <ProgDiskPage+0x30>
 800f2bc:	f895 0020 	ldrb.w	r0, [r5, #32]
 800f2c0:	2800      	cmp	r0, #0
 800f2c2:	d1f1      	bne.n	800f2a8 <ProgDiskPage+0x10>
 800f2c4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800f2c8:	2001      	movs	r0, #1
 800f2ca:	e7fb      	b.n	800f2c4 <ProgDiskPage+0x2c>
 800f2cc:	200094e0 	.word	0x200094e0

0800f2d0 <NextCluster>:
 800f2d0:	8803      	ldrh	r3, [r0, #0]
 800f2d2:	4a11      	ldr	r2, [pc, #68]	; (800f318 <NextCluster+0x48>)
 800f2d4:	b513      	push	{r0, r1, r4, lr}
 800f2d6:	6811      	ldr	r1, [r2, #0]
 800f2d8:	4604      	mov	r4, r0
 800f2da:	8043      	strh	r3, [r0, #2]
 800f2dc:	2000      	movs	r0, #0
 800f2de:	6992      	ldr	r2, [r2, #24]
 800f2e0:	8020      	strh	r0, [r4, #0]
 800f2e2:	4293      	cmp	r3, r2
 800f2e4:	d302      	bcc.n	800f2ec <NextCluster+0x1c>
 800f2e6:	2001      	movs	r0, #1
 800f2e8:	b002      	add	sp, #8
 800f2ea:	bd10      	pop	{r4, pc}
 800f2ec:	2b01      	cmp	r3, #1
 800f2ee:	d9fa      	bls.n	800f2e6 <NextCluster+0x16>
 800f2f0:	eb03 0353 	add.w	r3, r3, r3, lsr #1
 800f2f4:	2202      	movs	r2, #2
 800f2f6:	4419      	add	r1, r3
 800f2f8:	f10d 0006 	add.w	r0, sp, #6
 800f2fc:	f7ff ffae 	bl	800f25c <ReadDiskData>
 800f300:	2800      	cmp	r0, #0
 800f302:	d1f0      	bne.n	800f2e6 <NextCluster+0x16>
 800f304:	8862      	ldrh	r2, [r4, #2]
 800f306:	f8bd 3006 	ldrh.w	r3, [sp, #6]
 800f30a:	07d2      	lsls	r2, r2, #31
 800f30c:	bf4c      	ite	mi
 800f30e:	091b      	lsrmi	r3, r3, #4
 800f310:	f3c3 030b 	ubfxpl	r3, r3, #0, #12
 800f314:	8023      	strh	r3, [r4, #0]
 800f316:	e7e7      	b.n	800f2e8 <NextCluster+0x18>
 800f318:	200094e0 	.word	0x200094e0

0800f31c <ReadFileSec>:
 800f31c:	b538      	push	{r3, r4, r5, lr}
 800f31e:	460c      	mov	r4, r1
 800f320:	4a0a      	ldr	r2, [pc, #40]	; (800f34c <ReadFileSec+0x30>)
 800f322:	8823      	ldrh	r3, [r4, #0]
 800f324:	6951      	ldr	r1, [r2, #20]
 800f326:	68d5      	ldr	r5, [r2, #12]
 800f328:	3b02      	subs	r3, #2
 800f32a:	b28a      	uxth	r2, r1
 800f32c:	fb01 5103 	mla	r1, r1, r3, r5
 800f330:	f7ff ff94 	bl	800f25c <ReadDiskData>
 800f334:	b938      	cbnz	r0, 800f346 <ReadFileSec+0x2a>
 800f336:	4620      	mov	r0, r4
 800f338:	f7ff ffca 	bl	800f2d0 <NextCluster>
 800f33c:	2800      	cmp	r0, #0
 800f33e:	bf14      	ite	ne
 800f340:	2002      	movne	r0, #2
 800f342:	2000      	moveq	r0, #0
 800f344:	bd38      	pop	{r3, r4, r5, pc}
 800f346:	2001      	movs	r0, #1
 800f348:	e7fc      	b.n	800f344 <ReadFileSec+0x28>
 800f34a:	bf00      	nop
 800f34c:	200094e0 	.word	0x200094e0

0800f350 <SeekBlank>:
 800f350:	880b      	ldrh	r3, [r1, #0]
 800f352:	b5f7      	push	{r0, r1, r2, r4, r5, r6, r7, lr}
 800f354:	804b      	strh	r3, [r1, #2]
 800f356:	2300      	movs	r3, #0
 800f358:	460c      	mov	r4, r1
 800f35a:	2501      	movs	r5, #1
 800f35c:	f640 76fe 	movw	r6, #4094	; 0xffe
 800f360:	4f12      	ldr	r7, [pc, #72]	; (800f3ac <SeekBlank+0x5c>)
 800f362:	800b      	strh	r3, [r1, #0]
 800f364:	8823      	ldrh	r3, [r4, #0]
 800f366:	42b3      	cmp	r3, r6
 800f368:	d815      	bhi.n	800f396 <SeekBlank+0x46>
 800f36a:	6839      	ldr	r1, [r7, #0]
 800f36c:	2202      	movs	r2, #2
 800f36e:	4419      	add	r1, r3
 800f370:	eb01 0153 	add.w	r1, r1, r3, lsr #1
 800f374:	a801      	add	r0, sp, #4
 800f376:	f7ff ff71 	bl	800f25c <ReadDiskData>
 800f37a:	b9a0      	cbnz	r0, 800f3a6 <SeekBlank+0x56>
 800f37c:	8823      	ldrh	r3, [r4, #0]
 800f37e:	f8bd 2004 	ldrh.w	r2, [sp, #4]
 800f382:	07d9      	lsls	r1, r3, #31
 800f384:	bf4c      	ite	mi
 800f386:	0912      	lsrmi	r2, r2, #4
 800f388:	f3c2 020b 	ubfxpl	r2, r2, #0, #12
 800f38c:	b942      	cbnz	r2, 800f3a0 <SeekBlank+0x50>
 800f38e:	b92d      	cbnz	r5, 800f39c <SeekBlank+0x4c>
 800f390:	8862      	ldrh	r2, [r4, #2]
 800f392:	429a      	cmp	r2, r3
 800f394:	d004      	beq.n	800f3a0 <SeekBlank+0x50>
 800f396:	2000      	movs	r0, #0
 800f398:	b003      	add	sp, #12
 800f39a:	bdf0      	pop	{r4, r5, r6, r7, pc}
 800f39c:	4615      	mov	r5, r2
 800f39e:	80a3      	strh	r3, [r4, #4]
 800f3a0:	3301      	adds	r3, #1
 800f3a2:	8023      	strh	r3, [r4, #0]
 800f3a4:	e7de      	b.n	800f364 <SeekBlank+0x14>
 800f3a6:	2001      	movs	r0, #1
 800f3a8:	e7f6      	b.n	800f398 <SeekBlank+0x48>
 800f3aa:	bf00      	nop
 800f3ac:	200094e0 	.word	0x200094e0

0800f3b0 <SetCluster>:
 800f3b0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800f3b4:	f8b1 8002 	ldrh.w	r8, [r1, #2]
 800f3b8:	880d      	ldrh	r5, [r1, #0]
 800f3ba:	eb08 0358 	add.w	r3, r8, r8, lsr #1
 800f3be:	b29c      	uxth	r4, r3
 800f3c0:	4b16      	ldr	r3, [pc, #88]	; (800f41c <SetCluster+0x6c>)
 800f3c2:	f404 4770 	and.w	r7, r4, #61440	; 0xf000
 800f3c6:	681a      	ldr	r2, [r3, #0]
 800f3c8:	4606      	mov	r6, r0
 800f3ca:	4417      	add	r7, r2
 800f3cc:	4639      	mov	r1, r7
 800f3ce:	8a9a      	ldrh	r2, [r3, #20]
 800f3d0:	f7ff ff44 	bl	800f25c <ReadDiskData>
 800f3d4:	b9f8      	cbnz	r0, 800f416 <SetCluster+0x66>
 800f3d6:	f3c4 030b 	ubfx	r3, r4, #0, #12
 800f3da:	f018 0f01 	tst.w	r8, #1
 800f3de:	f103 0101 	add.w	r1, r3, #1
 800f3e2:	d010      	beq.n	800f406 <SetCluster+0x56>
 800f3e4:	5cf2      	ldrb	r2, [r6, r3]
 800f3e6:	f002 020f 	and.w	r2, r2, #15
 800f3ea:	eb02 1205 	add.w	r2, r2, r5, lsl #4
 800f3ee:	54f2      	strb	r2, [r6, r3]
 800f3f0:	092d      	lsrs	r5, r5, #4
 800f3f2:	5475      	strb	r5, [r6, r1]
 800f3f4:	4630      	mov	r0, r6
 800f3f6:	4639      	mov	r1, r7
 800f3f8:	f7ff ff4e 	bl	800f298 <ProgDiskPage>
 800f3fc:	3000      	adds	r0, #0
 800f3fe:	bf18      	it	ne
 800f400:	2001      	movne	r0, #1
 800f402:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800f406:	54f5      	strb	r5, [r6, r3]
 800f408:	5c73      	ldrb	r3, [r6, r1]
 800f40a:	f3c5 2503 	ubfx	r5, r5, #8, #4
 800f40e:	f023 030f 	bic.w	r3, r3, #15
 800f412:	431d      	orrs	r5, r3
 800f414:	e7ed      	b.n	800f3f2 <SetCluster+0x42>
 800f416:	2001      	movs	r0, #1
 800f418:	e7f3      	b.n	800f402 <SetCluster+0x52>
 800f41a:	bf00      	nop
 800f41c:	200094e0 	.word	0x200094e0

0800f420 <ProgFileSec>:
 800f420:	4a13      	ldr	r2, [pc, #76]	; (800f470 <ProgFileSec+0x50>)
 800f422:	b538      	push	{r3, r4, r5, lr}
 800f424:	880b      	ldrh	r3, [r1, #0]
 800f426:	460c      	mov	r4, r1
 800f428:	6951      	ldr	r1, [r2, #20]
 800f42a:	68d2      	ldr	r2, [r2, #12]
 800f42c:	3b02      	subs	r3, #2
 800f42e:	fb01 2103 	mla	r1, r1, r3, r2
 800f432:	4605      	mov	r5, r0
 800f434:	f7ff ff30 	bl	800f298 <ProgDiskPage>
 800f438:	b9a0      	cbnz	r0, 800f464 <ProgFileSec+0x44>
 800f43a:	4620      	mov	r0, r4
 800f43c:	f7ff ff48 	bl	800f2d0 <NextCluster>
 800f440:	b990      	cbnz	r0, 800f468 <ProgFileSec+0x48>
 800f442:	8823      	ldrh	r3, [r4, #0]
 800f444:	b96b      	cbnz	r3, 800f462 <ProgFileSec+0x42>
 800f446:	8863      	ldrh	r3, [r4, #2]
 800f448:	4621      	mov	r1, r4
 800f44a:	8023      	strh	r3, [r4, #0]
 800f44c:	4628      	mov	r0, r5
 800f44e:	f7ff ff7f 	bl	800f350 <SeekBlank>
 800f452:	b958      	cbnz	r0, 800f46c <ProgFileSec+0x4c>
 800f454:	4621      	mov	r1, r4
 800f456:	4628      	mov	r0, r5
 800f458:	f7ff ffaa 	bl	800f3b0 <SetCluster>
 800f45c:	3000      	adds	r0, #0
 800f45e:	bf18      	it	ne
 800f460:	2001      	movne	r0, #1
 800f462:	bd38      	pop	{r3, r4, r5, pc}
 800f464:	2001      	movs	r0, #1
 800f466:	e7fc      	b.n	800f462 <ProgFileSec+0x42>
 800f468:	2002      	movs	r0, #2
 800f46a:	e7fa      	b.n	800f462 <ProgFileSec+0x42>
 800f46c:	2003      	movs	r0, #3
 800f46e:	e7f8      	b.n	800f462 <ProgFileSec+0x42>
 800f470:	200094e0 	.word	0x200094e0

0800f474 <OpenFileRd>:
 800f474:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800f478:	4698      	mov	r8, r3
 800f47a:	2300      	movs	r3, #0
 800f47c:	4605      	mov	r5, r0
 800f47e:	460f      	mov	r7, r1
 800f480:	4616      	mov	r6, r2
 800f482:	4c19      	ldr	r4, [pc, #100]	; (800f4e8 <OpenFileRd+0x74>)
 800f484:	8013      	strh	r3, [r2, #0]
 800f486:	68a3      	ldr	r3, [r4, #8]
 800f488:	f8c8 3000 	str.w	r3, [r8]
 800f48c:	f8d8 1000 	ldr.w	r1, [r8]
 800f490:	68e3      	ldr	r3, [r4, #12]
 800f492:	4299      	cmp	r1, r3
 800f494:	d301      	bcc.n	800f49a <OpenFileRd+0x26>
 800f496:	2003      	movs	r0, #3
 800f498:	e01a      	b.n	800f4d0 <OpenFileRd+0x5c>
 800f49a:	8aa2      	ldrh	r2, [r4, #20]
 800f49c:	4628      	mov	r0, r5
 800f49e:	f7ff fedd 	bl	800f25c <ReadDiskData>
 800f4a2:	b9f8      	cbnz	r0, 800f4e4 <OpenFileRd+0x70>
 800f4a4:	4602      	mov	r2, r0
 800f4a6:	6963      	ldr	r3, [r4, #20]
 800f4a8:	429a      	cmp	r2, r3
 800f4aa:	d2ef      	bcs.n	800f48c <OpenFileRd+0x18>
 800f4ac:	2100      	movs	r1, #0
 800f4ae:	eb05 0c02 	add.w	ip, r5, r2
 800f4b2:	f81c 3001 	ldrb.w	r3, [ip, r1]
 800f4b6:	b90b      	cbnz	r3, 800f4bc <OpenFileRd+0x48>
 800f4b8:	2004      	movs	r0, #4
 800f4ba:	e009      	b.n	800f4d0 <OpenFileRd+0x5c>
 800f4bc:	f817 e001 	ldrb.w	lr, [r7, r1]
 800f4c0:	459e      	cmp	lr, r3
 800f4c2:	d107      	bne.n	800f4d4 <OpenFileRd+0x60>
 800f4c4:	3101      	adds	r1, #1
 800f4c6:	290b      	cmp	r1, #11
 800f4c8:	d1f3      	bne.n	800f4b2 <OpenFileRd+0x3e>
 800f4ca:	f8bc 301a 	ldrh.w	r3, [ip, #26]
 800f4ce:	8033      	strh	r3, [r6, #0]
 800f4d0:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800f4d4:	f8d8 3000 	ldr.w	r3, [r8]
 800f4d8:	3220      	adds	r2, #32
 800f4da:	3320      	adds	r3, #32
 800f4dc:	f8c8 3000 	str.w	r3, [r8]
 800f4e0:	b292      	uxth	r2, r2
 800f4e2:	e7e0      	b.n	800f4a6 <OpenFileRd+0x32>
 800f4e4:	2001      	movs	r0, #1
 800f4e6:	e7f3      	b.n	800f4d0 <OpenFileRd+0x5c>
 800f4e8:	200094e0 	.word	0x200094e0

0800f4ec <OpenFileWr>:
 800f4ec:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800f4f0:	4606      	mov	r6, r0
 800f4f2:	460f      	mov	r7, r1
 800f4f4:	4690      	mov	r8, r2
 800f4f6:	461c      	mov	r4, r3
 800f4f8:	f7ff ffbc 	bl	800f474 <OpenFileRd>
 800f4fc:	2804      	cmp	r0, #4
 800f4fe:	d123      	bne.n	800f548 <OpenFileWr+0x5c>
 800f500:	4641      	mov	r1, r8
 800f502:	4630      	mov	r0, r6
 800f504:	f7ff ff24 	bl	800f350 <SeekBlank>
 800f508:	bb00      	cbnz	r0, 800f54c <OpenFileWr+0x60>
 800f50a:	6825      	ldr	r5, [r4, #0]
 800f50c:	4b11      	ldr	r3, [pc, #68]	; (800f554 <OpenFileWr+0x68>)
 800f50e:	f3c5 040b 	ubfx	r4, r5, #0, #12
 800f512:	1b2d      	subs	r5, r5, r4
 800f514:	8a9a      	ldrh	r2, [r3, #20]
 800f516:	4629      	mov	r1, r5
 800f518:	4630      	mov	r0, r6
 800f51a:	f7ff fe9f 	bl	800f25c <ReadDiskData>
 800f51e:	b9b8      	cbnz	r0, 800f550 <OpenFileWr+0x64>
 800f520:	1933      	adds	r3, r6, r4
 800f522:	4619      	mov	r1, r3
 800f524:	1e7a      	subs	r2, r7, #1
 800f526:	370a      	adds	r7, #10
 800f528:	f812 0f01 	ldrb.w	r0, [r2, #1]!
 800f52c:	42ba      	cmp	r2, r7
 800f52e:	f803 0b01 	strb.w	r0, [r3], #1
 800f532:	d1f9      	bne.n	800f528 <OpenFileWr+0x3c>
 800f534:	f8b8 3000 	ldrh.w	r3, [r8]
 800f538:	4630      	mov	r0, r6
 800f53a:	834b      	strh	r3, [r1, #26]
 800f53c:	4629      	mov	r1, r5
 800f53e:	f7ff feab 	bl	800f298 <ProgDiskPage>
 800f542:	3000      	adds	r0, #0
 800f544:	bf18      	it	ne
 800f546:	2001      	movne	r0, #1
 800f548:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800f54c:	2003      	movs	r0, #3
 800f54e:	e7fb      	b.n	800f548 <OpenFileWr+0x5c>
 800f550:	2001      	movs	r0, #1
 800f552:	e7f9      	b.n	800f548 <OpenFileWr+0x5c>
 800f554:	200094e0 	.word	0x200094e0

0800f558 <CloseFile>:
 800f558:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800f55c:	461e      	mov	r6, r3
 800f55e:	f640 73ff 	movw	r3, #4095	; 0xfff
 800f562:	4604      	mov	r4, r0
 800f564:	8013      	strh	r3, [r2, #0]
 800f566:	460f      	mov	r7, r1
 800f568:	4611      	mov	r1, r2
 800f56a:	f7ff ff21 	bl	800f3b0 <SetCluster>
 800f56e:	4d18      	ldr	r5, [pc, #96]	; (800f5d0 <CloseFile+0x78>)
 800f570:	6831      	ldr	r1, [r6, #0]
 800f572:	f8df 8060 	ldr.w	r8, [pc, #96]	; 800f5d4 <CloseFile+0x7c>
 800f576:	8aaa      	ldrh	r2, [r5, #20]
 800f578:	ea01 0108 	and.w	r1, r1, r8
 800f57c:	4620      	mov	r0, r4
 800f57e:	f7ff fe6d 	bl	800f25c <ReadDiskData>
 800f582:	b110      	cbz	r0, 800f58a <CloseFile+0x32>
 800f584:	2001      	movs	r0, #1
 800f586:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800f58a:	2320      	movs	r3, #32
 800f58c:	6832      	ldr	r2, [r6, #0]
 800f58e:	4620      	mov	r0, r4
 800f590:	f3c2 020b 	ubfx	r2, r2, #0, #12
 800f594:	4422      	add	r2, r4
 800f596:	72d3      	strb	r3, [r2, #11]
 800f598:	6832      	ldr	r2, [r6, #0]
 800f59a:	f3c2 020b 	ubfx	r2, r2, #0, #12
 800f59e:	4422      	add	r2, r4
 800f5a0:	61d7      	str	r7, [r2, #28]
 800f5a2:	6831      	ldr	r1, [r6, #0]
 800f5a4:	ea01 0108 	and.w	r1, r1, r8
 800f5a8:	f7ff fe76 	bl	800f298 <ProgDiskPage>
 800f5ac:	2800      	cmp	r0, #0
 800f5ae:	d1e9      	bne.n	800f584 <CloseFile+0x2c>
 800f5b0:	8aaa      	ldrh	r2, [r5, #20]
 800f5b2:	6829      	ldr	r1, [r5, #0]
 800f5b4:	4620      	mov	r0, r4
 800f5b6:	f7ff fe51 	bl	800f25c <ReadDiskData>
 800f5ba:	2800      	cmp	r0, #0
 800f5bc:	d1e2      	bne.n	800f584 <CloseFile+0x2c>
 800f5be:	6869      	ldr	r1, [r5, #4]
 800f5c0:	4620      	mov	r0, r4
 800f5c2:	f7ff fe69 	bl	800f298 <ProgDiskPage>
 800f5c6:	3000      	adds	r0, #0
 800f5c8:	bf18      	it	ne
 800f5ca:	2001      	movne	r0, #1
 800f5cc:	e7db      	b.n	800f586 <CloseFile+0x2e>
 800f5ce:	bf00      	nop
 800f5d0:	200094e0 	.word	0x200094e0
 800f5d4:	fffff000 	.word	0xfffff000

0800f5d8 <sfp_printchar>:
 800f5d8:	6803      	ldr	r3, [r0, #0]
 800f5da:	7019      	strb	r1, [r3, #0]
 800f5dc:	6803      	ldr	r3, [r0, #0]
 800f5de:	3301      	adds	r3, #1
 800f5e0:	6003      	str	r3, [r0, #0]
 800f5e2:	4770      	bx	lr

0800f5e4 <sfp_prints>:
 800f5e4:	2a00      	cmp	r2, #0
 800f5e6:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 800f5ea:	4607      	mov	r7, r0
 800f5ec:	460c      	mov	r4, r1
 800f5ee:	dd12      	ble.n	800f616 <sfp_prints+0x32>
 800f5f0:	1b0d      	subs	r5, r1, r4
 800f5f2:	f811 0b01 	ldrb.w	r0, [r1], #1
 800f5f6:	2800      	cmp	r0, #0
 800f5f8:	d1fa      	bne.n	800f5f0 <sfp_prints+0xc>
 800f5fa:	42aa      	cmp	r2, r5
 800f5fc:	bfcc      	ite	gt
 800f5fe:	1b52      	subgt	r2, r2, r5
 800f600:	4602      	movle	r2, r0
 800f602:	f013 0f02 	tst.w	r3, #2
 800f606:	bf0c      	ite	eq
 800f608:	2620      	moveq	r6, #32
 800f60a:	2630      	movne	r6, #48	; 0x30
 800f60c:	07db      	lsls	r3, r3, #31
 800f60e:	d50f      	bpl.n	800f630 <sfp_prints+0x4c>
 800f610:	2300      	movs	r3, #0
 800f612:	1b1d      	subs	r5, r3, r4
 800f614:	e011      	b.n	800f63a <sfp_prints+0x56>
 800f616:	2620      	movs	r6, #32
 800f618:	e7f8      	b.n	800f60c <sfp_prints+0x28>
 800f61a:	4631      	mov	r1, r6
 800f61c:	4638      	mov	r0, r7
 800f61e:	f7ff ffdb 	bl	800f5d8 <sfp_printchar>
 800f622:	3d01      	subs	r5, #1
 800f624:	2d00      	cmp	r5, #0
 800f626:	dcf8      	bgt.n	800f61a <sfp_prints+0x36>
 800f628:	ea22 73e2 	bic.w	r3, r2, r2, asr #31
 800f62c:	1ad2      	subs	r2, r2, r3
 800f62e:	e7f0      	b.n	800f612 <sfp_prints+0x2e>
 800f630:	4615      	mov	r5, r2
 800f632:	e7f7      	b.n	800f624 <sfp_prints+0x40>
 800f634:	4638      	mov	r0, r7
 800f636:	f7ff ffcf 	bl	800f5d8 <sfp_printchar>
 800f63a:	eb05 0804 	add.w	r8, r5, r4
 800f63e:	f814 1b01 	ldrb.w	r1, [r4], #1
 800f642:	2900      	cmp	r1, #0
 800f644:	d1f6      	bne.n	800f634 <sfp_prints+0x50>
 800f646:	4614      	mov	r4, r2
 800f648:	2c00      	cmp	r4, #0
 800f64a:	dc07      	bgt.n	800f65c <sfp_prints+0x78>
 800f64c:	2a00      	cmp	r2, #0
 800f64e:	bfac      	ite	ge
 800f650:	eb08 0002 	addge.w	r0, r8, r2
 800f654:	f108 0000 	addlt.w	r0, r8, #0
 800f658:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 800f65c:	4631      	mov	r1, r6
 800f65e:	4638      	mov	r0, r7
 800f660:	f7ff ffba 	bl	800f5d8 <sfp_printchar>
 800f664:	3c01      	subs	r4, #1
 800f666:	e7ef      	b.n	800f648 <sfp_prints+0x64>

0800f668 <sfp_printi>:
 800f668:	b5f0      	push	{r4, r5, r6, r7, lr}
 800f66a:	b085      	sub	sp, #20
 800f66c:	4694      	mov	ip, r2
 800f66e:	e9dd 270a 	ldrd	r2, r7, [sp, #40]	; 0x28
 800f672:	4606      	mov	r6, r0
 800f674:	461c      	mov	r4, r3
 800f676:	b941      	cbnz	r1, 800f68a <sfp_printi+0x22>
 800f678:	2330      	movs	r3, #48	; 0x30
 800f67a:	a904      	add	r1, sp, #16
 800f67c:	f821 3d0c 	strh.w	r3, [r1, #-12]!
 800f680:	463b      	mov	r3, r7
 800f682:	f7ff ffaf 	bl	800f5e4 <sfp_prints>
 800f686:	b005      	add	sp, #20
 800f688:	bdf0      	pop	{r4, r5, r6, r7, pc}
 800f68a:	b12b      	cbz	r3, 800f698 <sfp_printi+0x30>
 800f68c:	f1bc 0f0a 	cmp.w	ip, #10
 800f690:	d119      	bne.n	800f6c6 <sfp_printi+0x5e>
 800f692:	2900      	cmp	r1, #0
 800f694:	da17      	bge.n	800f6c6 <sfp_printi+0x5e>
 800f696:	4249      	negs	r1, r1
 800f698:	2300      	movs	r3, #0
 800f69a:	ad04      	add	r5, sp, #16
 800f69c:	f805 3d01 	strb.w	r3, [r5, #-1]!
 800f6a0:	9b0c      	ldr	r3, [sp, #48]	; 0x30
 800f6a2:	3b3a      	subs	r3, #58	; 0x3a
 800f6a4:	b989      	cbnz	r1, 800f6ca <sfp_printi+0x62>
 800f6a6:	b13c      	cbz	r4, 800f6b8 <sfp_printi+0x50>
 800f6a8:	b1da      	cbz	r2, 800f6e2 <sfp_printi+0x7a>
 800f6aa:	07bb      	lsls	r3, r7, #30
 800f6ac:	d519      	bpl.n	800f6e2 <sfp_printi+0x7a>
 800f6ae:	212d      	movs	r1, #45	; 0x2d
 800f6b0:	4630      	mov	r0, r6
 800f6b2:	f7ff ff91 	bl	800f5d8 <sfp_printchar>
 800f6b6:	3a01      	subs	r2, #1
 800f6b8:	463b      	mov	r3, r7
 800f6ba:	4629      	mov	r1, r5
 800f6bc:	4630      	mov	r0, r6
 800f6be:	f7ff ff91 	bl	800f5e4 <sfp_prints>
 800f6c2:	4420      	add	r0, r4
 800f6c4:	e7df      	b.n	800f686 <sfp_printi+0x1e>
 800f6c6:	2400      	movs	r4, #0
 800f6c8:	e7e6      	b.n	800f698 <sfp_printi+0x30>
 800f6ca:	fbb1 f0fc 	udiv	r0, r1, ip
 800f6ce:	fb0c 1110 	mls	r1, ip, r0, r1
 800f6d2:	2909      	cmp	r1, #9
 800f6d4:	bf88      	it	hi
 800f6d6:	18c9      	addhi	r1, r1, r3
 800f6d8:	3130      	adds	r1, #48	; 0x30
 800f6da:	f805 1d01 	strb.w	r1, [r5, #-1]!
 800f6de:	4601      	mov	r1, r0
 800f6e0:	e7e0      	b.n	800f6a4 <sfp_printi+0x3c>
 800f6e2:	232d      	movs	r3, #45	; 0x2d
 800f6e4:	3d01      	subs	r5, #1
 800f6e6:	702b      	strb	r3, [r5, #0]
 800f6e8:	2400      	movs	r4, #0
 800f6ea:	e7e5      	b.n	800f6b8 <sfp_printi+0x50>

0800f6ec <sfp_print>:
 800f6ec:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 800f6f0:	4680      	mov	r8, r0
 800f6f2:	460e      	mov	r6, r1
 800f6f4:	4614      	mov	r4, r2
 800f6f6:	2500      	movs	r5, #0
 800f6f8:	f04f 0b00 	mov.w	fp, #0
 800f6fc:	f8df a1b8 	ldr.w	sl, [pc, #440]	; 800f8b8 <sfp_print+0x1cc>
 800f700:	b08b      	sub	sp, #44	; 0x2c
 800f702:	7833      	ldrb	r3, [r6, #0]
 800f704:	b93b      	cbnz	r3, 800f716 <sfp_print+0x2a>
 800f706:	2200      	movs	r2, #0
 800f708:	4628      	mov	r0, r5
 800f70a:	f8d8 3000 	ldr.w	r3, [r8]
 800f70e:	701a      	strb	r2, [r3, #0]
 800f710:	b00b      	add	sp, #44	; 0x2c
 800f712:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 800f716:	2b25      	cmp	r3, #37	; 0x25
 800f718:	f040 80c6 	bne.w	800f8a8 <sfp_print+0x1bc>
 800f71c:	7873      	ldrb	r3, [r6, #1]
 800f71e:	2b00      	cmp	r3, #0
 800f720:	d0f1      	beq.n	800f706 <sfp_print+0x1a>
 800f722:	2b25      	cmp	r3, #37	; 0x25
 800f724:	f106 0201 	add.w	r2, r6, #1
 800f728:	f000 80bd 	beq.w	800f8a6 <sfp_print+0x1ba>
 800f72c:	2b2d      	cmp	r3, #45	; 0x2d
 800f72e:	bf06      	itte	eq
 800f730:	1cb2      	addeq	r2, r6, #2
 800f732:	2301      	moveq	r3, #1
 800f734:	2300      	movne	r3, #0
 800f736:	4616      	mov	r6, r2
 800f738:	7832      	ldrb	r2, [r6, #0]
 800f73a:	1c71      	adds	r1, r6, #1
 800f73c:	2a30      	cmp	r2, #48	; 0x30
 800f73e:	d019      	beq.n	800f774 <sfp_print+0x88>
 800f740:	2700      	movs	r7, #0
 800f742:	f04f 0e0a 	mov.w	lr, #10
 800f746:	4631      	mov	r1, r6
 800f748:	f811 2b01 	ldrb.w	r2, [r1], #1
 800f74c:	f1a2 0030 	sub.w	r0, r2, #48	; 0x30
 800f750:	fa5f fc80 	uxtb.w	ip, r0
 800f754:	f1bc 0f09 	cmp.w	ip, #9
 800f758:	d910      	bls.n	800f77c <sfp_print+0x90>
 800f75a:	2a73      	cmp	r2, #115	; 0x73
 800f75c:	d112      	bne.n	800f784 <sfp_print+0x98>
 800f75e:	6821      	ldr	r1, [r4, #0]
 800f760:	4854      	ldr	r0, [pc, #336]	; (800f8b4 <sfp_print+0x1c8>)
 800f762:	463a      	mov	r2, r7
 800f764:	2900      	cmp	r1, #0
 800f766:	bf08      	it	eq
 800f768:	4601      	moveq	r1, r0
 800f76a:	3404      	adds	r4, #4
 800f76c:	4640      	mov	r0, r8
 800f76e:	f7ff ff39 	bl	800f5e4 <sfp_prints>
 800f772:	e015      	b.n	800f7a0 <sfp_print+0xb4>
 800f774:	f043 0302 	orr.w	r3, r3, #2
 800f778:	460e      	mov	r6, r1
 800f77a:	e7dd      	b.n	800f738 <sfp_print+0x4c>
 800f77c:	fb0e 0707 	mla	r7, lr, r7, r0
 800f780:	460e      	mov	r6, r1
 800f782:	e7e0      	b.n	800f746 <sfp_print+0x5a>
 800f784:	2a64      	cmp	r2, #100	; 0x64
 800f786:	d10e      	bne.n	800f7a6 <sfp_print+0xba>
 800f788:	2261      	movs	r2, #97	; 0x61
 800f78a:	4621      	mov	r1, r4
 800f78c:	e9cd 3201 	strd	r3, r2, [sp, #4]
 800f790:	2301      	movs	r3, #1
 800f792:	9700      	str	r7, [sp, #0]
 800f794:	220a      	movs	r2, #10
 800f796:	6809      	ldr	r1, [r1, #0]
 800f798:	4640      	mov	r0, r8
 800f79a:	3404      	adds	r4, #4
 800f79c:	f7ff ff64 	bl	800f668 <sfp_printi>
 800f7a0:	4405      	add	r5, r0
 800f7a2:	3601      	adds	r6, #1
 800f7a4:	e7ad      	b.n	800f702 <sfp_print+0x16>
 800f7a6:	2a66      	cmp	r2, #102	; 0x66
 800f7a8:	d159      	bne.n	800f85e <sfp_print+0x172>
 800f7aa:	3407      	adds	r4, #7
 800f7ac:	f024 0307 	bic.w	r3, r4, #7
 800f7b0:	e9d3 0100 	ldrd	r0, r1, [r3]
 800f7b4:	f103 0408 	add.w	r4, r3, #8
 800f7b8:	f002 fce8 	bl	801218c <__aeabi_d2f>
 800f7bc:	4659      	mov	r1, fp
 800f7be:	4681      	mov	r9, r0
 800f7c0:	f002 ffe0 	bl	8012784 <__aeabi_fcmplt>
 800f7c4:	2800      	cmp	r0, #0
 800f7c6:	d03a      	beq.n	800f83e <sfp_print+0x152>
 800f7c8:	232d      	movs	r3, #45	; 0x2d
 800f7ca:	f109 4900 	add.w	r9, r9, #2147483648	; 0x80000000
 800f7ce:	f88d 301c 	strb.w	r3, [sp, #28]
 800f7d2:	f10d 011d 	add.w	r1, sp, #29
 800f7d6:	4648      	mov	r0, r9
 800f7d8:	9105      	str	r1, [sp, #20]
 800f7da:	f002 fffb 	bl	80127d4 <__aeabi_f2iz>
 800f7de:	2201      	movs	r2, #1
 800f7e0:	f04f 0c0a 	mov.w	ip, #10
 800f7e4:	9905      	ldr	r1, [sp, #20]
 800f7e6:	fb0c f302 	mul.w	r3, ip, r2
 800f7ea:	4298      	cmp	r0, r3
 800f7ec:	da29      	bge.n	800f842 <sfp_print+0x156>
 800f7ee:	f04f 0c0a 	mov.w	ip, #10
 800f7f2:	2a00      	cmp	r2, #0
 800f7f4:	f101 0301 	add.w	r3, r1, #1
 800f7f8:	dc25      	bgt.n	800f846 <sfp_print+0x15a>
 800f7fa:	2f00      	cmp	r7, #0
 800f7fc:	f04f 022e 	mov.w	r2, #46	; 0x2e
 800f800:	bf08      	it	eq
 800f802:	2703      	moveq	r7, #3
 800f804:	700a      	strb	r2, [r1, #0]
 800f806:	441f      	add	r7, r3
 800f808:	4648      	mov	r0, r9
 800f80a:	9305      	str	r3, [sp, #20]
 800f80c:	f002 ffe2 	bl	80127d4 <__aeabi_f2iz>
 800f810:	f002 fdc6 	bl	80123a0 <__aeabi_i2f>
 800f814:	4601      	mov	r1, r0
 800f816:	4648      	mov	r0, r9
 800f818:	f002 fd0c 	bl	8012234 <__aeabi_fsub>
 800f81c:	4651      	mov	r1, sl
 800f81e:	f002 fe13 	bl	8012448 <__aeabi_fmul>
 800f822:	4681      	mov	r9, r0
 800f824:	f002 ffd6 	bl	80127d4 <__aeabi_f2iz>
 800f828:	9b05      	ldr	r3, [sp, #20]
 800f82a:	3030      	adds	r0, #48	; 0x30
 800f82c:	f803 0b01 	strb.w	r0, [r3], #1
 800f830:	42bb      	cmp	r3, r7
 800f832:	d1e9      	bne.n	800f808 <sfp_print+0x11c>
 800f834:	2200      	movs	r2, #0
 800f836:	a907      	add	r1, sp, #28
 800f838:	701a      	strb	r2, [r3, #0]
 800f83a:	2320      	movs	r3, #32
 800f83c:	e796      	b.n	800f76c <sfp_print+0x80>
 800f83e:	a907      	add	r1, sp, #28
 800f840:	e7c9      	b.n	800f7d6 <sfp_print+0xea>
 800f842:	461a      	mov	r2, r3
 800f844:	e7cf      	b.n	800f7e6 <sfp_print+0xfa>
 800f846:	fb90 f1f2 	sdiv	r1, r0, r2
 800f84a:	f101 0e30 	add.w	lr, r1, #48	; 0x30
 800f84e:	fb02 0011 	mls	r0, r2, r1, r0
 800f852:	f803 ec01 	strb.w	lr, [r3, #-1]
 800f856:	fb92 f2fc 	sdiv	r2, r2, ip
 800f85a:	4619      	mov	r1, r3
 800f85c:	e7c9      	b.n	800f7f2 <sfp_print+0x106>
 800f85e:	2a78      	cmp	r2, #120	; 0x78
 800f860:	d107      	bne.n	800f872 <sfp_print+0x186>
 800f862:	4621      	mov	r1, r4
 800f864:	2261      	movs	r2, #97	; 0x61
 800f866:	e9cd 3201 	strd	r3, r2, [sp, #4]
 800f86a:	9700      	str	r7, [sp, #0]
 800f86c:	2300      	movs	r3, #0
 800f86e:	2210      	movs	r2, #16
 800f870:	e791      	b.n	800f796 <sfp_print+0xaa>
 800f872:	2a58      	cmp	r2, #88	; 0x58
 800f874:	d102      	bne.n	800f87c <sfp_print+0x190>
 800f876:	4621      	mov	r1, r4
 800f878:	2241      	movs	r2, #65	; 0x41
 800f87a:	e7f4      	b.n	800f866 <sfp_print+0x17a>
 800f87c:	2a75      	cmp	r2, #117	; 0x75
 800f87e:	d106      	bne.n	800f88e <sfp_print+0x1a2>
 800f880:	2261      	movs	r2, #97	; 0x61
 800f882:	4621      	mov	r1, r4
 800f884:	e9cd 3201 	strd	r3, r2, [sp, #4]
 800f888:	9700      	str	r7, [sp, #0]
 800f88a:	2300      	movs	r3, #0
 800f88c:	e782      	b.n	800f794 <sfp_print+0xa8>
 800f88e:	2a63      	cmp	r2, #99	; 0x63
 800f890:	d187      	bne.n	800f7a2 <sfp_print+0xb6>
 800f892:	6822      	ldr	r2, [r4, #0]
 800f894:	a906      	add	r1, sp, #24
 800f896:	f88d 2018 	strb.w	r2, [sp, #24]
 800f89a:	2200      	movs	r2, #0
 800f89c:	3404      	adds	r4, #4
 800f89e:	f88d 2019 	strb.w	r2, [sp, #25]
 800f8a2:	463a      	mov	r2, r7
 800f8a4:	e762      	b.n	800f76c <sfp_print+0x80>
 800f8a6:	4616      	mov	r6, r2
 800f8a8:	7831      	ldrb	r1, [r6, #0]
 800f8aa:	4640      	mov	r0, r8
 800f8ac:	f7ff fe94 	bl	800f5d8 <sfp_printchar>
 800f8b0:	3501      	adds	r5, #1
 800f8b2:	e776      	b.n	800f7a2 <sfp_print+0xb6>
 800f8b4:	080147b3 	.word	0x080147b3
 800f8b8:	41200000 	.word	0x41200000

0800f8bc <sprintf>:
 800f8bc:	b40e      	push	{r1, r2, r3}
 800f8be:	b50f      	push	{r0, r1, r2, r3, lr}
 800f8c0:	aa05      	add	r2, sp, #20
 800f8c2:	f852 1b04 	ldr.w	r1, [r2], #4
 800f8c6:	9001      	str	r0, [sp, #4]
 800f8c8:	a801      	add	r0, sp, #4
 800f8ca:	9203      	str	r2, [sp, #12]
 800f8cc:	f7ff ff0e 	bl	800f6ec <sfp_print>
 800f8d0:	b004      	add	sp, #16
 800f8d2:	f85d eb04 	ldr.w	lr, [sp], #4
 800f8d6:	b003      	add	sp, #12
 800f8d8:	4770      	bx	lr
 800f8da:	bf00      	nop

0800f8dc <FLASH_Prog>:
 800f8dc:	b538      	push	{r3, r4, r5, lr}
 800f8de:	4604      	mov	r4, r0
 800f8e0:	4807      	ldr	r0, [pc, #28]	; (800f900 <FLASH_Prog+0x24>)
 800f8e2:	460d      	mov	r5, r1
 800f8e4:	f000 fc76 	bl	80101d4 <FLASH_WaitForLastOperation>
 800f8e8:	2805      	cmp	r0, #5
 800f8ea:	d002      	beq.n	800f8f2 <FLASH_Prog+0x16>
 800f8ec:	2034      	movs	r0, #52	; 0x34
 800f8ee:	f000 fc43 	bl	8010178 <FLASH_ClearFlag>
 800f8f2:	4629      	mov	r1, r5
 800f8f4:	4620      	mov	r0, r4
 800f8f6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 800f8fa:	f000 bd27 	b.w	801034c <FLASH_ProgramHalfWord>
 800f8fe:	bf00      	nop
 800f900:	000186a0 	.word	0x000186a0

0800f904 <FLASH_Erase>:
 800f904:	f3c0 030a 	ubfx	r3, r0, #0, #11
 800f908:	b510      	push	{r4, lr}
 800f90a:	4604      	mov	r4, r0
 800f90c:	b963      	cbnz	r3, 800f928 <FLASH_Erase+0x24>
 800f90e:	4807      	ldr	r0, [pc, #28]	; (800f92c <FLASH_Erase+0x28>)
 800f910:	f000 fc60 	bl	80101d4 <FLASH_WaitForLastOperation>
 800f914:	2805      	cmp	r0, #5
 800f916:	d002      	beq.n	800f91e <FLASH_Erase+0x1a>
 800f918:	2034      	movs	r0, #52	; 0x34
 800f91a:	f000 fc2d 	bl	8010178 <FLASH_ClearFlag>
 800f91e:	4620      	mov	r0, r4
 800f920:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800f924:	f000 bc67 	b.w	80101f6 <FLASH_ErasePage>
 800f928:	bd10      	pop	{r4, pc}
 800f92a:	bf00      	nop
 800f92c:	000186a0 	.word	0x000186a0

0800f930 <ExtFlashSendByte>:
 800f930:	b570      	push	{r4, r5, r6, lr}
 800f932:	4605      	mov	r5, r0
 800f934:	4c0e      	ldr	r4, [pc, #56]	; (800f970 <ExtFlashSendByte+0x40>)
 800f936:	4e0f      	ldr	r6, [pc, #60]	; (800f974 <ExtFlashSendByte+0x44>)
 800f938:	2102      	movs	r1, #2
 800f93a:	4630      	mov	r0, r6
 800f93c:	f000 f9e8 	bl	800fd10 <SPI_I2S_GetFlagStatus>
 800f940:	b178      	cbz	r0, 800f962 <ExtFlashSendByte+0x32>
 800f942:	4629      	mov	r1, r5
 800f944:	480b      	ldr	r0, [pc, #44]	; (800f974 <ExtFlashSendByte+0x44>)
 800f946:	f000 f999 	bl	800fc7c <SPI_I2S_SendData>
 800f94a:	4c09      	ldr	r4, [pc, #36]	; (800f970 <ExtFlashSendByte+0x40>)
 800f94c:	4d09      	ldr	r5, [pc, #36]	; (800f974 <ExtFlashSendByte+0x44>)
 800f94e:	2101      	movs	r1, #1
 800f950:	4628      	mov	r0, r5
 800f952:	f000 f9dd 	bl	800fd10 <SPI_I2S_GetFlagStatus>
 800f956:	b140      	cbz	r0, 800f96a <ExtFlashSendByte+0x3a>
 800f958:	4806      	ldr	r0, [pc, #24]	; (800f974 <ExtFlashSendByte+0x44>)
 800f95a:	f000 f991 	bl	800fc80 <SPI_I2S_ReceiveData>
 800f95e:	b2c0      	uxtb	r0, r0
 800f960:	e002      	b.n	800f968 <ExtFlashSendByte+0x38>
 800f962:	3c01      	subs	r4, #1
 800f964:	d1e8      	bne.n	800f938 <ExtFlashSendByte+0x8>
 800f966:	20ff      	movs	r0, #255	; 0xff
 800f968:	bd70      	pop	{r4, r5, r6, pc}
 800f96a:	3c01      	subs	r4, #1
 800f96c:	d1ef      	bne.n	800f94e <ExtFlashSendByte+0x1e>
 800f96e:	e7fa      	b.n	800f966 <ExtFlashSendByte+0x36>
 800f970:	000186a2 	.word	0x000186a2
 800f974:	40013000 	.word	0x40013000

0800f978 <ExtFlashReadByte>:
 800f978:	20a5      	movs	r0, #165	; 0xa5
 800f97a:	f7ff bfd9 	b.w	800f930 <ExtFlashSendByte>

0800f97e <ExtFlashWrCtrl>:
 800f97e:	b510      	push	{r4, lr}
 800f980:	4604      	mov	r4, r0
 800f982:	f7fc fcbf 	bl	800c304 <ExtFlash_CS_LOW>
 800f986:	2c01      	cmp	r4, #1
 800f988:	bf0c      	ite	eq
 800f98a:	2006      	moveq	r0, #6
 800f98c:	2004      	movne	r0, #4
 800f98e:	f7ff ffcf 	bl	800f930 <ExtFlashSendByte>
 800f992:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800f996:	f7fc bcb9 	b.w	800c30c <ExtFlash_CS_HIGH>

0800f99a <ExtFlashWaitForWrEnd>:
 800f99a:	b510      	push	{r4, lr}
 800f99c:	f7fc fcb2 	bl	800c304 <ExtFlash_CS_LOW>
 800f9a0:	2005      	movs	r0, #5
 800f9a2:	f7ff ffc5 	bl	800f930 <ExtFlashSendByte>
 800f9a6:	4c06      	ldr	r4, [pc, #24]	; (800f9c0 <ExtFlashWaitForWrEnd+0x26>)
 800f9a8:	f7ff ffe6 	bl	800f978 <ExtFlashReadByte>
 800f9ac:	07c3      	lsls	r3, r0, #31
 800f9ae:	d403      	bmi.n	800f9b8 <ExtFlashWaitForWrEnd+0x1e>
 800f9b0:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800f9b4:	f7fc bcaa 	b.w	800c30c <ExtFlash_CS_HIGH>
 800f9b8:	3c01      	subs	r4, #1
 800f9ba:	d1f5      	bne.n	800f9a8 <ExtFlashWaitForWrEnd+0xe>
 800f9bc:	bd10      	pop	{r4, pc}
 800f9be:	bf00      	nop
 800f9c0:	000186a2 	.word	0x000186a2

0800f9c4 <ExtFlashSectorErase>:
 800f9c4:	b510      	push	{r4, lr}
 800f9c6:	4604      	mov	r4, r0
 800f9c8:	f7ff ffe7 	bl	800f99a <ExtFlashWaitForWrEnd>
 800f9cc:	2001      	movs	r0, #1
 800f9ce:	f7ff ffd6 	bl	800f97e <ExtFlashWrCtrl>
 800f9d2:	f7fc fc97 	bl	800c304 <ExtFlash_CS_LOW>
 800f9d6:	2020      	movs	r0, #32
 800f9d8:	f7ff ffaa 	bl	800f930 <ExtFlashSendByte>
 800f9dc:	f3c4 4007 	ubfx	r0, r4, #16, #8
 800f9e0:	f7ff ffa6 	bl	800f930 <ExtFlashSendByte>
 800f9e4:	f3c4 2007 	ubfx	r0, r4, #8, #8
 800f9e8:	f7ff ffa2 	bl	800f930 <ExtFlashSendByte>
 800f9ec:	b2e0      	uxtb	r0, r4
 800f9ee:	f7ff ff9f 	bl	800f930 <ExtFlashSendByte>
 800f9f2:	f7fc fc8b 	bl	800c30c <ExtFlash_CS_HIGH>
 800f9f6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 800f9fa:	2000      	movs	r0, #0
 800f9fc:	f7ff bfbf 	b.w	800f97e <ExtFlashWrCtrl>

0800fa00 <ExtFlashPageProg>:
 800fa00:	b570      	push	{r4, r5, r6, lr}
 800fa02:	4616      	mov	r6, r2
 800fa04:	460d      	mov	r5, r1
 800fa06:	4604      	mov	r4, r0
 800fa08:	f7ff ffc7 	bl	800f99a <ExtFlashWaitForWrEnd>
 800fa0c:	2001      	movs	r0, #1
 800fa0e:	f7ff ffb6 	bl	800f97e <ExtFlashWrCtrl>
 800fa12:	f7fc fc77 	bl	800c304 <ExtFlash_CS_LOW>
 800fa16:	4630      	mov	r0, r6
 800fa18:	f7ff ff8a 	bl	800f930 <ExtFlashSendByte>
 800fa1c:	f3c5 4007 	ubfx	r0, r5, #16, #8
 800fa20:	f7ff ff86 	bl	800f930 <ExtFlashSendByte>
 800fa24:	f3c5 2007 	ubfx	r0, r5, #8, #8
 800fa28:	f7ff ff82 	bl	800f930 <ExtFlashSendByte>
 800fa2c:	b2e8      	uxtb	r0, r5
 800fa2e:	f7ff ff7f 	bl	800f930 <ExtFlashSendByte>
 800fa32:	1e65      	subs	r5, r4, #1
 800fa34:	34ff      	adds	r4, #255	; 0xff
 800fa36:	f815 0f01 	ldrb.w	r0, [r5, #1]!
 800fa3a:	43c0      	mvns	r0, r0
 800fa3c:	b2c0      	uxtb	r0, r0
 800fa3e:	f7ff ff77 	bl	800f930 <ExtFlashSendByte>
 800fa42:	42a5      	cmp	r5, r4
 800fa44:	d1f7      	bne.n	800fa36 <ExtFlashPageProg+0x36>
 800fa46:	f7fc fc61 	bl	800c30c <ExtFlash_CS_HIGH>
 800fa4a:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800fa4e:	2000      	movs	r0, #0
 800fa50:	f7ff bf95 	b.w	800f97e <ExtFlashWrCtrl>

0800fa54 <ExtFlashSecWr>:
 800fa54:	b570      	push	{r4, r5, r6, lr}
 800fa56:	f421 657f 	bic.w	r5, r1, #4080	; 0xff0
 800fa5a:	f025 050f 	bic.w	r5, r5, #15
 800fa5e:	4606      	mov	r6, r0
 800fa60:	4628      	mov	r0, r5
 800fa62:	f7ff ffaf 	bl	800f9c4 <ExtFlashSectorErase>
 800fa66:	2400      	movs	r4, #0
 800fa68:	1929      	adds	r1, r5, r4
 800fa6a:	1930      	adds	r0, r6, r4
 800fa6c:	2202      	movs	r2, #2
 800fa6e:	f504 7480 	add.w	r4, r4, #256	; 0x100
 800fa72:	f7ff ffc5 	bl	800fa00 <ExtFlashPageProg>
 800fa76:	f5b4 5f80 	cmp.w	r4, #4096	; 0x1000
 800fa7a:	d1f5      	bne.n	800fa68 <ExtFlashSecWr+0x14>
 800fa7c:	bd70      	pop	{r4, r5, r6, pc}

0800fa7e <ExtFlashDataRd>:
 800fa7e:	b570      	push	{r4, r5, r6, lr}
 800fa80:	460e      	mov	r6, r1
 800fa82:	4615      	mov	r5, r2
 800fa84:	4604      	mov	r4, r0
 800fa86:	f7ff ff88 	bl	800f99a <ExtFlashWaitForWrEnd>
 800fa8a:	f7fc fc3b 	bl	800c304 <ExtFlash_CS_LOW>
 800fa8e:	2003      	movs	r0, #3
 800fa90:	f7ff ff4e 	bl	800f930 <ExtFlashSendByte>
 800fa94:	f3c6 4007 	ubfx	r0, r6, #16, #8
 800fa98:	f7ff ff4a 	bl	800f930 <ExtFlashSendByte>
 800fa9c:	f3c6 2007 	ubfx	r0, r6, #8, #8
 800faa0:	f7ff ff46 	bl	800f930 <ExtFlashSendByte>
 800faa4:	b2f0      	uxtb	r0, r6
 800faa6:	f7ff ff43 	bl	800f930 <ExtFlashSendByte>
 800faaa:	3c01      	subs	r4, #1
 800faac:	4425      	add	r5, r4
 800faae:	42ac      	cmp	r4, r5
 800fab0:	d103      	bne.n	800faba <ExtFlashDataRd+0x3c>
 800fab2:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 800fab6:	f7fc bc29 	b.w	800c30c <ExtFlash_CS_HIGH>
 800faba:	f7ff ff5d 	bl	800f978 <ExtFlashReadByte>
 800fabe:	43c0      	mvns	r0, r0
 800fac0:	f804 0f01 	strb.w	r0, [r4, #1]!
 800fac4:	e7f3      	b.n	800faae <ExtFlashDataRd+0x30>
 800fac6:	bf00      	nop

0800fac8 <SPI_I2S_DeInit>:
 800fac8:	b508      	push	{r3, lr}
 800faca:	4b15      	ldr	r3, [pc, #84]	; (800fb20 <SPI_I2S_DeInit+0x58>)
 800facc:	4298      	cmp	r0, r3
 800face:	d10b      	bne.n	800fae8 <SPI_I2S_DeInit+0x20>
 800fad0:	2101      	movs	r1, #1
 800fad2:	f44f 5080 	mov.w	r0, #4096	; 0x1000
 800fad6:	f000 fa6f 	bl	800ffb8 <RCC_APB2PeriphResetCmd>
 800fada:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 800fade:	2100      	movs	r1, #0
 800fae0:	f44f 5080 	mov.w	r0, #4096	; 0x1000
 800fae4:	f000 ba68 	b.w	800ffb8 <RCC_APB2PeriphResetCmd>
 800fae8:	4b0e      	ldr	r3, [pc, #56]	; (800fb24 <SPI_I2S_DeInit+0x5c>)
 800faea:	4298      	cmp	r0, r3
 800faec:	d10b      	bne.n	800fb06 <SPI_I2S_DeInit+0x3e>
 800faee:	2101      	movs	r1, #1
 800faf0:	f44f 4080 	mov.w	r0, #16384	; 0x4000
 800faf4:	f000 fa6c 	bl	800ffd0 <RCC_APB1PeriphResetCmd>
 800faf8:	2100      	movs	r1, #0
 800fafa:	f44f 4080 	mov.w	r0, #16384	; 0x4000
 800fafe:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 800fb02:	f000 ba65 	b.w	800ffd0 <RCC_APB1PeriphResetCmd>
 800fb06:	4b08      	ldr	r3, [pc, #32]	; (800fb28 <SPI_I2S_DeInit+0x60>)
 800fb08:	4298      	cmp	r0, r3
 800fb0a:	d108      	bne.n	800fb1e <SPI_I2S_DeInit+0x56>
 800fb0c:	2101      	movs	r1, #1
 800fb0e:	f44f 4000 	mov.w	r0, #32768	; 0x8000
 800fb12:	f000 fa5d 	bl	800ffd0 <RCC_APB1PeriphResetCmd>
 800fb16:	2100      	movs	r1, #0
 800fb18:	f44f 4000 	mov.w	r0, #32768	; 0x8000
 800fb1c:	e7ef      	b.n	800fafe <SPI_I2S_DeInit+0x36>
 800fb1e:	bd08      	pop	{r3, pc}
 800fb20:	40013000 	.word	0x40013000
 800fb24:	40003800 	.word	0x40003800
 800fb28:	40003c00 	.word	0x40003c00

0800fb2c <SPI_Init>:
 800fb2c:	8803      	ldrh	r3, [r0, #0]
 800fb2e:	b510      	push	{r4, lr}
 800fb30:	f403 5241 	and.w	r2, r3, #12352	; 0x3040
 800fb34:	884c      	ldrh	r4, [r1, #2]
 800fb36:	880b      	ldrh	r3, [r1, #0]
 800fb38:	4323      	orrs	r3, r4
 800fb3a:	888c      	ldrh	r4, [r1, #4]
 800fb3c:	4323      	orrs	r3, r4
 800fb3e:	88cc      	ldrh	r4, [r1, #6]
 800fb40:	4323      	orrs	r3, r4
 800fb42:	890c      	ldrh	r4, [r1, #8]
 800fb44:	4323      	orrs	r3, r4
 800fb46:	894c      	ldrh	r4, [r1, #10]
 800fb48:	4323      	orrs	r3, r4
 800fb4a:	898c      	ldrh	r4, [r1, #12]
 800fb4c:	4323      	orrs	r3, r4
 800fb4e:	89cc      	ldrh	r4, [r1, #14]
 800fb50:	4323      	orrs	r3, r4
 800fb52:	4313      	orrs	r3, r2
 800fb54:	8003      	strh	r3, [r0, #0]
 800fb56:	8b83      	ldrh	r3, [r0, #28]
 800fb58:	f423 6300 	bic.w	r3, r3, #2048	; 0x800
 800fb5c:	041b      	lsls	r3, r3, #16
 800fb5e:	0c1b      	lsrs	r3, r3, #16
 800fb60:	8383      	strh	r3, [r0, #28]
 800fb62:	8a0b      	ldrh	r3, [r1, #16]
 800fb64:	8203      	strh	r3, [r0, #16]
 800fb66:	bd10      	pop	{r4, pc}

0800fb68 <I2S_Init>:
 800fb68:	8b83      	ldrh	r3, [r0, #28]
 800fb6a:	b5f0      	push	{r4, r5, r6, r7, lr}
 800fb6c:	f423 637b 	bic.w	r3, r3, #4016	; 0xfb0
 800fb70:	f023 030f 	bic.w	r3, r3, #15
 800fb74:	8383      	strh	r3, [r0, #28]
 800fb76:	2302      	movs	r3, #2
 800fb78:	8403      	strh	r3, [r0, #32]
 800fb7a:	688b      	ldr	r3, [r1, #8]
 800fb7c:	8b86      	ldrh	r6, [r0, #28]
 800fb7e:	2b02      	cmp	r3, #2
 800fb80:	b087      	sub	sp, #28
 800fb82:	4605      	mov	r5, r0
 800fb84:	460c      	mov	r4, r1
 800fb86:	b2b6      	uxth	r6, r6
 800fb88:	d039      	beq.n	800fbfe <I2S_Init+0x96>
 800fb8a:	888b      	ldrh	r3, [r1, #4]
 800fb8c:	a801      	add	r0, sp, #4
 800fb8e:	2b00      	cmp	r3, #0
 800fb90:	bf14      	ite	ne
 800fb92:	2702      	movne	r7, #2
 800fb94:	2701      	moveq	r7, #1
 800fb96:	f000 f9ab 	bl	800fef0 <RCC_GetClocksFreq>
 800fb9a:	88e3      	ldrh	r3, [r4, #6]
 800fb9c:	9901      	ldr	r1, [sp, #4]
 800fb9e:	f5b3 7f00 	cmp.w	r3, #512	; 0x200
 800fba2:	bf18      	it	ne
 800fba4:	017b      	lslne	r3, r7, #5
 800fba6:	f04f 020a 	mov.w	r2, #10
 800fbaa:	bf14      	ite	ne
 800fbac:	fbb1 f3f3 	udivne	r3, r1, r3
 800fbb0:	0a0b      	lsreq	r3, r1, #8
 800fbb2:	4353      	muls	r3, r2
 800fbb4:	68a0      	ldr	r0, [r4, #8]
 800fbb6:	220a      	movs	r2, #10
 800fbb8:	fbb3 f3f0 	udiv	r3, r3, r0
 800fbbc:	3305      	adds	r3, #5
 800fbbe:	b29b      	uxth	r3, r3
 800fbc0:	fbb3 f3f2 	udiv	r3, r3, r2
 800fbc4:	f003 0201 	and.w	r2, r3, #1
 800fbc8:	0212      	lsls	r2, r2, #8
 800fbca:	f3c3 034f 	ubfx	r3, r3, #1, #16
 800fbce:	1e99      	subs	r1, r3, #2
 800fbd0:	b289      	uxth	r1, r1
 800fbd2:	29fe      	cmp	r1, #254	; 0xfe
 800fbd4:	bf24      	itt	cs
 800fbd6:	2200      	movcs	r2, #0
 800fbd8:	2302      	movcs	r3, #2
 800fbda:	4313      	orrs	r3, r2
 800fbdc:	88e2      	ldrh	r2, [r4, #6]
 800fbde:	4313      	orrs	r3, r2
 800fbe0:	b29b      	uxth	r3, r3
 800fbe2:	8862      	ldrh	r2, [r4, #2]
 800fbe4:	842b      	strh	r3, [r5, #32]
 800fbe6:	8823      	ldrh	r3, [r4, #0]
 800fbe8:	4313      	orrs	r3, r2
 800fbea:	88a2      	ldrh	r2, [r4, #4]
 800fbec:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
 800fbf0:	4313      	orrs	r3, r2
 800fbf2:	89a2      	ldrh	r2, [r4, #12]
 800fbf4:	4313      	orrs	r3, r2
 800fbf6:	431e      	orrs	r6, r3
 800fbf8:	83ae      	strh	r6, [r5, #28]
 800fbfa:	b007      	add	sp, #28
 800fbfc:	bdf0      	pop	{r4, r5, r6, r7, pc}
 800fbfe:	2200      	movs	r2, #0
 800fc00:	e7e5      	b.n	800fbce <I2S_Init+0x66>

0800fc02 <SPI_StructInit>:
 800fc02:	2300      	movs	r3, #0
 800fc04:	8003      	strh	r3, [r0, #0]
 800fc06:	8043      	strh	r3, [r0, #2]
 800fc08:	8083      	strh	r3, [r0, #4]
 800fc0a:	80c3      	strh	r3, [r0, #6]
 800fc0c:	8103      	strh	r3, [r0, #8]
 800fc0e:	8143      	strh	r3, [r0, #10]
 800fc10:	8183      	strh	r3, [r0, #12]
 800fc12:	81c3      	strh	r3, [r0, #14]
 800fc14:	2307      	movs	r3, #7
 800fc16:	8203      	strh	r3, [r0, #16]
 800fc18:	4770      	bx	lr

0800fc1a <I2S_StructInit>:
 800fc1a:	2300      	movs	r3, #0
 800fc1c:	2202      	movs	r2, #2
 800fc1e:	e9c0 3300 	strd	r3, r3, [r0]
 800fc22:	6082      	str	r2, [r0, #8]
 800fc24:	8183      	strh	r3, [r0, #12]
 800fc26:	4770      	bx	lr

0800fc28 <SPI_Cmd>:
 800fc28:	8803      	ldrh	r3, [r0, #0]
 800fc2a:	b119      	cbz	r1, 800fc34 <SPI_Cmd+0xc>
 800fc2c:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 800fc30:	8003      	strh	r3, [r0, #0]
 800fc32:	4770      	bx	lr
 800fc34:	f023 0340 	bic.w	r3, r3, #64	; 0x40
 800fc38:	041b      	lsls	r3, r3, #16
 800fc3a:	0c1b      	lsrs	r3, r3, #16
 800fc3c:	e7f8      	b.n	800fc30 <SPI_Cmd+0x8>

0800fc3e <I2S_Cmd>:
 800fc3e:	8b83      	ldrh	r3, [r0, #28]
 800fc40:	b119      	cbz	r1, 800fc4a <I2S_Cmd+0xc>
 800fc42:	f443 6380 	orr.w	r3, r3, #1024	; 0x400
 800fc46:	8383      	strh	r3, [r0, #28]
 800fc48:	4770      	bx	lr
 800fc4a:	f423 6380 	bic.w	r3, r3, #1024	; 0x400
 800fc4e:	041b      	lsls	r3, r3, #16
 800fc50:	0c1b      	lsrs	r3, r3, #16
 800fc52:	e7f8      	b.n	800fc46 <I2S_Cmd+0x8>

0800fc54 <SPI_I2S_ITConfig>:
 800fc54:	090b      	lsrs	r3, r1, #4
 800fc56:	2101      	movs	r1, #1
 800fc58:	4099      	lsls	r1, r3
 800fc5a:	b289      	uxth	r1, r1
 800fc5c:	8883      	ldrh	r3, [r0, #4]
 800fc5e:	b112      	cbz	r2, 800fc66 <SPI_I2S_ITConfig+0x12>
 800fc60:	4319      	orrs	r1, r3
 800fc62:	8081      	strh	r1, [r0, #4]
 800fc64:	4770      	bx	lr
 800fc66:	ea23 0101 	bic.w	r1, r3, r1
 800fc6a:	e7fa      	b.n	800fc62 <SPI_I2S_ITConfig+0xe>

0800fc6c <SPI_I2S_DMACmd>:
 800fc6c:	8883      	ldrh	r3, [r0, #4]
 800fc6e:	b112      	cbz	r2, 800fc76 <SPI_I2S_DMACmd+0xa>
 800fc70:	4319      	orrs	r1, r3
 800fc72:	8081      	strh	r1, [r0, #4]
 800fc74:	4770      	bx	lr
 800fc76:	ea23 0101 	bic.w	r1, r3, r1
 800fc7a:	e7fa      	b.n	800fc72 <SPI_I2S_DMACmd+0x6>

0800fc7c <SPI_I2S_SendData>:
 800fc7c:	8181      	strh	r1, [r0, #12]
 800fc7e:	4770      	bx	lr

0800fc80 <SPI_I2S_ReceiveData>:
 800fc80:	8980      	ldrh	r0, [r0, #12]
 800fc82:	4770      	bx	lr

0800fc84 <SPI_NSSInternalSoftwareConfig>:
 800fc84:	f64f 63ff 	movw	r3, #65279	; 0xfeff
 800fc88:	4299      	cmp	r1, r3
 800fc8a:	8803      	ldrh	r3, [r0, #0]
 800fc8c:	bf05      	ittet	eq
 800fc8e:	f423 7380 	biceq.w	r3, r3, #256	; 0x100
 800fc92:	041b      	lsleq	r3, r3, #16
 800fc94:	f443 7380 	orrne.w	r3, r3, #256	; 0x100
 800fc98:	0c1b      	lsreq	r3, r3, #16
 800fc9a:	8003      	strh	r3, [r0, #0]
 800fc9c:	4770      	bx	lr

0800fc9e <SPI_SSOutputCmd>:
 800fc9e:	8883      	ldrh	r3, [r0, #4]
 800fca0:	b119      	cbz	r1, 800fcaa <SPI_SSOutputCmd+0xc>
 800fca2:	f043 0304 	orr.w	r3, r3, #4
 800fca6:	8083      	strh	r3, [r0, #4]
 800fca8:	4770      	bx	lr
 800fcaa:	f023 0304 	bic.w	r3, r3, #4
 800fcae:	041b      	lsls	r3, r3, #16
 800fcb0:	0c1b      	lsrs	r3, r3, #16
 800fcb2:	e7f8      	b.n	800fca6 <SPI_SSOutputCmd+0x8>

0800fcb4 <SPI_DataSizeConfig>:
 800fcb4:	8803      	ldrh	r3, [r0, #0]
 800fcb6:	f423 6300 	bic.w	r3, r3, #2048	; 0x800
 800fcba:	041b      	lsls	r3, r3, #16
 800fcbc:	0c1b      	lsrs	r3, r3, #16
 800fcbe:	8003      	strh	r3, [r0, #0]
 800fcc0:	8803      	ldrh	r3, [r0, #0]
 800fcc2:	4319      	orrs	r1, r3
 800fcc4:	8001      	strh	r1, [r0, #0]
 800fcc6:	4770      	bx	lr

0800fcc8 <SPI_TransmitCRC>:
 800fcc8:	8803      	ldrh	r3, [r0, #0]
 800fcca:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
 800fcce:	8003      	strh	r3, [r0, #0]
 800fcd0:	4770      	bx	lr

0800fcd2 <SPI_CalculateCRC>:
 800fcd2:	8803      	ldrh	r3, [r0, #0]
 800fcd4:	b119      	cbz	r1, 800fcde <SPI_CalculateCRC+0xc>
 800fcd6:	f443 5300 	orr.w	r3, r3, #8192	; 0x2000
 800fcda:	8003      	strh	r3, [r0, #0]
 800fcdc:	4770      	bx	lr
 800fcde:	f423 5300 	bic.w	r3, r3, #8192	; 0x2000
 800fce2:	041b      	lsls	r3, r3, #16
 800fce4:	0c1b      	lsrs	r3, r3, #16
 800fce6:	e7f8      	b.n	800fcda <SPI_CalculateCRC+0x8>

0800fce8 <SPI_GetCRC>:
 800fce8:	2901      	cmp	r1, #1
 800fcea:	bf14      	ite	ne
 800fcec:	8b00      	ldrhne	r0, [r0, #24]
 800fcee:	8a80      	ldrheq	r0, [r0, #20]
 800fcf0:	b280      	uxth	r0, r0
 800fcf2:	4770      	bx	lr

0800fcf4 <SPI_GetCRCPolynomial>:
 800fcf4:	8a00      	ldrh	r0, [r0, #16]
 800fcf6:	4770      	bx	lr

0800fcf8 <SPI_BiDirectionalLineConfig>:
 800fcf8:	8803      	ldrh	r3, [r0, #0]
 800fcfa:	f5b1 4f80 	cmp.w	r1, #16384	; 0x4000
 800fcfe:	bf1b      	ittet	ne
 800fd00:	f423 4380 	bicne.w	r3, r3, #16384	; 0x4000
 800fd04:	041b      	lslne	r3, r3, #16
 800fd06:	f443 4380 	orreq.w	r3, r3, #16384	; 0x4000
 800fd0a:	0c1b      	lsrne	r3, r3, #16
 800fd0c:	8003      	strh	r3, [r0, #0]
 800fd0e:	4770      	bx	lr

0800fd10 <SPI_I2S_GetFlagStatus>:
 800fd10:	8903      	ldrh	r3, [r0, #8]
 800fd12:	4219      	tst	r1, r3
 800fd14:	bf14      	ite	ne
 800fd16:	2001      	movne	r0, #1
 800fd18:	2000      	moveq	r0, #0
 800fd1a:	4770      	bx	lr

0800fd1c <SPI_I2S_ClearFlag>:
 800fd1c:	43c9      	mvns	r1, r1
 800fd1e:	b289      	uxth	r1, r1
 800fd20:	8101      	strh	r1, [r0, #8]
 800fd22:	4770      	bx	lr

0800fd24 <SPI_I2S_GetITStatus>:
 800fd24:	2301      	movs	r3, #1
 800fd26:	8882      	ldrh	r2, [r0, #4]
 800fd28:	b510      	push	{r4, lr}
 800fd2a:	8904      	ldrh	r4, [r0, #8]
 800fd2c:	f001 000f 	and.w	r0, r1, #15
 800fd30:	fa03 f000 	lsl.w	r0, r3, r0
 800fd34:	4020      	ands	r0, r4
 800fd36:	b292      	uxth	r2, r2
 800fd38:	d005      	beq.n	800fd46 <SPI_I2S_GetITStatus+0x22>
 800fd3a:	0909      	lsrs	r1, r1, #4
 800fd3c:	408b      	lsls	r3, r1
 800fd3e:	421a      	tst	r2, r3
 800fd40:	bf14      	ite	ne
 800fd42:	2001      	movne	r0, #1
 800fd44:	2000      	moveq	r0, #0
 800fd46:	bd10      	pop	{r4, pc}

0800fd48 <SPI_I2S_ClearITPendingBit>:
 800fd48:	f001 030f 	and.w	r3, r1, #15
 800fd4c:	2101      	movs	r1, #1
 800fd4e:	4099      	lsls	r1, r3
 800fd50:	43c9      	mvns	r1, r1
 800fd52:	b289      	uxth	r1, r1
 800fd54:	8101      	strh	r1, [r0, #8]
 800fd56:	4770      	bx	lr

0800fd58 <RCC_DeInit>:
 800fd58:	4b0d      	ldr	r3, [pc, #52]	; (800fd90 <RCC_DeInit+0x38>)
 800fd5a:	681a      	ldr	r2, [r3, #0]
 800fd5c:	f042 0201 	orr.w	r2, r2, #1
 800fd60:	601a      	str	r2, [r3, #0]
 800fd62:	6859      	ldr	r1, [r3, #4]
 800fd64:	4a0b      	ldr	r2, [pc, #44]	; (800fd94 <RCC_DeInit+0x3c>)
 800fd66:	400a      	ands	r2, r1
 800fd68:	605a      	str	r2, [r3, #4]
 800fd6a:	681a      	ldr	r2, [r3, #0]
 800fd6c:	f022 7284 	bic.w	r2, r2, #17301504	; 0x1080000
 800fd70:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
 800fd74:	601a      	str	r2, [r3, #0]
 800fd76:	681a      	ldr	r2, [r3, #0]
 800fd78:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
 800fd7c:	601a      	str	r2, [r3, #0]
 800fd7e:	685a      	ldr	r2, [r3, #4]
 800fd80:	f422 02fe 	bic.w	r2, r2, #8323072	; 0x7f0000
 800fd84:	605a      	str	r2, [r3, #4]
 800fd86:	f44f 021f 	mov.w	r2, #10420224	; 0x9f0000
 800fd8a:	609a      	str	r2, [r3, #8]
 800fd8c:	4770      	bx	lr
 800fd8e:	bf00      	nop
 800fd90:	40021000 	.word	0x40021000
 800fd94:	f8ff0000 	.word	0xf8ff0000

0800fd98 <RCC_HSEConfig>:
 800fd98:	4b0c      	ldr	r3, [pc, #48]	; (800fdcc <RCC_HSEConfig+0x34>)
 800fd9a:	f5b0 3f80 	cmp.w	r0, #65536	; 0x10000
 800fd9e:	681a      	ldr	r2, [r3, #0]
 800fda0:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
 800fda4:	601a      	str	r2, [r3, #0]
 800fda6:	681a      	ldr	r2, [r3, #0]
 800fda8:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
 800fdac:	601a      	str	r2, [r3, #0]
 800fdae:	d003      	beq.n	800fdb8 <RCC_HSEConfig+0x20>
 800fdb0:	f5b0 2f80 	cmp.w	r0, #262144	; 0x40000
 800fdb4:	d005      	beq.n	800fdc2 <RCC_HSEConfig+0x2a>
 800fdb6:	4770      	bx	lr
 800fdb8:	681a      	ldr	r2, [r3, #0]
 800fdba:	f442 3280 	orr.w	r2, r2, #65536	; 0x10000
 800fdbe:	601a      	str	r2, [r3, #0]
 800fdc0:	4770      	bx	lr
 800fdc2:	681a      	ldr	r2, [r3, #0]
 800fdc4:	f442 22a0 	orr.w	r2, r2, #327680	; 0x50000
 800fdc8:	e7f9      	b.n	800fdbe <RCC_HSEConfig+0x26>
 800fdca:	bf00      	nop
 800fdcc:	40021000 	.word	0x40021000

0800fdd0 <RCC_AdjustHSICalibrationValue>:
 800fdd0:	4a03      	ldr	r2, [pc, #12]	; (800fde0 <RCC_AdjustHSICalibrationValue+0x10>)
 800fdd2:	6813      	ldr	r3, [r2, #0]
 800fdd4:	f023 03f8 	bic.w	r3, r3, #248	; 0xf8
 800fdd8:	ea43 00c0 	orr.w	r0, r3, r0, lsl #3
 800fddc:	6010      	str	r0, [r2, #0]
 800fdde:	4770      	bx	lr
 800fde0:	40021000 	.word	0x40021000

0800fde4 <RCC_HSICmd>:
 800fde4:	4b01      	ldr	r3, [pc, #4]	; (800fdec <RCC_HSICmd+0x8>)
 800fde6:	6018      	str	r0, [r3, #0]
 800fde8:	4770      	bx	lr
 800fdea:	bf00      	nop
 800fdec:	42420000 	.word	0x42420000

0800fdf0 <RCC_PLLConfig>:
 800fdf0:	4a03      	ldr	r2, [pc, #12]	; (800fe00 <RCC_PLLConfig+0x10>)
 800fdf2:	6853      	ldr	r3, [r2, #4]
 800fdf4:	f423 137c 	bic.w	r3, r3, #4128768	; 0x3f0000
 800fdf8:	4319      	orrs	r1, r3
 800fdfa:	4301      	orrs	r1, r0
 800fdfc:	6051      	str	r1, [r2, #4]
 800fdfe:	4770      	bx	lr
 800fe00:	40021000 	.word	0x40021000

0800fe04 <RCC_PLLCmd>:
 800fe04:	4b01      	ldr	r3, [pc, #4]	; (800fe0c <RCC_PLLCmd+0x8>)
 800fe06:	6018      	str	r0, [r3, #0]
 800fe08:	4770      	bx	lr
 800fe0a:	bf00      	nop
 800fe0c:	42420060 	.word	0x42420060

0800fe10 <RCC_SYSCLKConfig>:
 800fe10:	4a03      	ldr	r2, [pc, #12]	; (800fe20 <RCC_SYSCLKConfig+0x10>)
 800fe12:	6853      	ldr	r3, [r2, #4]
 800fe14:	f023 0303 	bic.w	r3, r3, #3
 800fe18:	4318      	orrs	r0, r3
 800fe1a:	6050      	str	r0, [r2, #4]
 800fe1c:	4770      	bx	lr
 800fe1e:	bf00      	nop
 800fe20:	40021000 	.word	0x40021000

0800fe24 <RCC_GetSYSCLKSource>:
 800fe24:	4b02      	ldr	r3, [pc, #8]	; (800fe30 <RCC_GetSYSCLKSource+0xc>)
 800fe26:	6858      	ldr	r0, [r3, #4]
 800fe28:	f000 000c 	and.w	r0, r0, #12
 800fe2c:	4770      	bx	lr
 800fe2e:	bf00      	nop
 800fe30:	40021000 	.word	0x40021000

0800fe34 <RCC_HCLKConfig>:
 800fe34:	4a03      	ldr	r2, [pc, #12]	; (800fe44 <RCC_HCLKConfig+0x10>)
 800fe36:	6853      	ldr	r3, [r2, #4]
 800fe38:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 800fe3c:	4318      	orrs	r0, r3
 800fe3e:	6050      	str	r0, [r2, #4]
 800fe40:	4770      	bx	lr
 800fe42:	bf00      	nop
 800fe44:	40021000 	.word	0x40021000

0800fe48 <RCC_PCLK1Config>:
 800fe48:	4a03      	ldr	r2, [pc, #12]	; (800fe58 <RCC_PCLK1Config+0x10>)
 800fe4a:	6853      	ldr	r3, [r2, #4]
 800fe4c:	f423 63e0 	bic.w	r3, r3, #1792	; 0x700
 800fe50:	4318      	orrs	r0, r3
 800fe52:	6050      	str	r0, [r2, #4]
 800fe54:	4770      	bx	lr
 800fe56:	bf00      	nop
 800fe58:	40021000 	.word	0x40021000

0800fe5c <RCC_PCLK2Config>:
 800fe5c:	4a03      	ldr	r2, [pc, #12]	; (800fe6c <RCC_PCLK2Config+0x10>)
 800fe5e:	6853      	ldr	r3, [r2, #4]
 800fe60:	f423 5360 	bic.w	r3, r3, #14336	; 0x3800
 800fe64:	ea43 00c0 	orr.w	r0, r3, r0, lsl #3
 800fe68:	6050      	str	r0, [r2, #4]
 800fe6a:	4770      	bx	lr
 800fe6c:	40021000 	.word	0x40021000

0800fe70 <RCC_ITConfig>:
 800fe70:	4b04      	ldr	r3, [pc, #16]	; (800fe84 <RCC_ITConfig+0x14>)
 800fe72:	781a      	ldrb	r2, [r3, #0]
 800fe74:	b111      	cbz	r1, 800fe7c <RCC_ITConfig+0xc>
 800fe76:	4310      	orrs	r0, r2
 800fe78:	7018      	strb	r0, [r3, #0]
 800fe7a:	4770      	bx	lr
 800fe7c:	ea22 0000 	bic.w	r0, r2, r0
 800fe80:	e7fa      	b.n	800fe78 <RCC_ITConfig+0x8>
 800fe82:	bf00      	nop
 800fe84:	40021009 	.word	0x40021009

0800fe88 <RCC_USBCLKConfig>:
 800fe88:	4b01      	ldr	r3, [pc, #4]	; (800fe90 <RCC_USBCLKConfig+0x8>)
 800fe8a:	6018      	str	r0, [r3, #0]
 800fe8c:	4770      	bx	lr
 800fe8e:	bf00      	nop
 800fe90:	424200d8 	.word	0x424200d8

0800fe94 <RCC_ADCCLKConfig>:
 800fe94:	4a03      	ldr	r2, [pc, #12]	; (800fea4 <RCC_ADCCLKConfig+0x10>)
 800fe96:	6853      	ldr	r3, [r2, #4]
 800fe98:	f423 4340 	bic.w	r3, r3, #49152	; 0xc000
 800fe9c:	4318      	orrs	r0, r3
 800fe9e:	6050      	str	r0, [r2, #4]
 800fea0:	4770      	bx	lr
 800fea2:	bf00      	nop
 800fea4:	40021000 	.word	0x40021000

0800fea8 <RCC_LSEConfig>:
 800fea8:	2200      	movs	r2, #0
 800feaa:	4b06      	ldr	r3, [pc, #24]	; (800fec4 <RCC_LSEConfig+0x1c>)
 800feac:	2801      	cmp	r0, #1
 800feae:	701a      	strb	r2, [r3, #0]
 800feb0:	701a      	strb	r2, [r3, #0]
 800feb2:	d002      	beq.n	800feba <RCC_LSEConfig+0x12>
 800feb4:	2804      	cmp	r0, #4
 800feb6:	d002      	beq.n	800febe <RCC_LSEConfig+0x16>
 800feb8:	4770      	bx	lr
 800feba:	7018      	strb	r0, [r3, #0]
 800febc:	4770      	bx	lr
 800febe:	2205      	movs	r2, #5
 800fec0:	701a      	strb	r2, [r3, #0]
 800fec2:	4770      	bx	lr
 800fec4:	40021020 	.word	0x40021020

0800fec8 <RCC_LSICmd>:
 800fec8:	4b01      	ldr	r3, [pc, #4]	; (800fed0 <RCC_LSICmd+0x8>)
 800feca:	6018      	str	r0, [r3, #0]
 800fecc:	4770      	bx	lr
 800fece:	bf00      	nop
 800fed0:	42420480 	.word	0x42420480

0800fed4 <RCC_RTCCLKConfig>:
 800fed4:	4a02      	ldr	r2, [pc, #8]	; (800fee0 <RCC_RTCCLKConfig+0xc>)
 800fed6:	6a13      	ldr	r3, [r2, #32]
 800fed8:	4318      	orrs	r0, r3
 800feda:	6210      	str	r0, [r2, #32]
 800fedc:	4770      	bx	lr
 800fede:	bf00      	nop
 800fee0:	40021000 	.word	0x40021000

0800fee4 <RCC_RTCCLKCmd>:
 800fee4:	4b01      	ldr	r3, [pc, #4]	; (800feec <RCC_RTCCLKCmd+0x8>)
 800fee6:	6018      	str	r0, [r3, #0]
 800fee8:	4770      	bx	lr
 800feea:	bf00      	nop
 800feec:	4242043c 	.word	0x4242043c

0800fef0 <RCC_GetClocksFreq>:
 800fef0:	4a1b      	ldr	r2, [pc, #108]	; (800ff60 <RCC_GetClocksFreq+0x70>)
 800fef2:	b510      	push	{r4, lr}
 800fef4:	6853      	ldr	r3, [r2, #4]
 800fef6:	f003 030c 	and.w	r3, r3, #12
 800fefa:	2b08      	cmp	r3, #8
 800fefc:	d01f      	beq.n	800ff3e <RCC_GetClocksFreq+0x4e>
 800fefe:	4b19      	ldr	r3, [pc, #100]	; (800ff64 <RCC_GetClocksFreq+0x74>)
 800ff00:	6003      	str	r3, [r0, #0]
 800ff02:	6853      	ldr	r3, [r2, #4]
 800ff04:	4918      	ldr	r1, [pc, #96]	; (800ff68 <RCC_GetClocksFreq+0x78>)
 800ff06:	f3c3 1303 	ubfx	r3, r3, #4, #4
 800ff0a:	5ccc      	ldrb	r4, [r1, r3]
 800ff0c:	6803      	ldr	r3, [r0, #0]
 800ff0e:	40e3      	lsrs	r3, r4
 800ff10:	6043      	str	r3, [r0, #4]
 800ff12:	6854      	ldr	r4, [r2, #4]
 800ff14:	f3c4 2402 	ubfx	r4, r4, #8, #3
 800ff18:	5d0c      	ldrb	r4, [r1, r4]
 800ff1a:	fa23 f404 	lsr.w	r4, r3, r4
 800ff1e:	6084      	str	r4, [r0, #8]
 800ff20:	6854      	ldr	r4, [r2, #4]
 800ff22:	f3c4 24c2 	ubfx	r4, r4, #11, #3
 800ff26:	5d0c      	ldrb	r4, [r1, r4]
 800ff28:	40e3      	lsrs	r3, r4
 800ff2a:	60c3      	str	r3, [r0, #12]
 800ff2c:	6852      	ldr	r2, [r2, #4]
 800ff2e:	f3c2 3281 	ubfx	r2, r2, #14, #2
 800ff32:	4411      	add	r1, r2
 800ff34:	7c0a      	ldrb	r2, [r1, #16]
 800ff36:	fbb3 f3f2 	udiv	r3, r3, r2
 800ff3a:	6103      	str	r3, [r0, #16]
 800ff3c:	bd10      	pop	{r4, pc}
 800ff3e:	6853      	ldr	r3, [r2, #4]
 800ff40:	6851      	ldr	r1, [r2, #4]
 800ff42:	f3c3 4383 	ubfx	r3, r3, #18, #4
 800ff46:	03cc      	lsls	r4, r1, #15
 800ff48:	f103 0302 	add.w	r3, r3, #2
 800ff4c:	d402      	bmi.n	800ff54 <RCC_GetClocksFreq+0x64>
 800ff4e:	4907      	ldr	r1, [pc, #28]	; (800ff6c <RCC_GetClocksFreq+0x7c>)
 800ff50:	434b      	muls	r3, r1
 800ff52:	e7d5      	b.n	800ff00 <RCC_GetClocksFreq+0x10>
 800ff54:	6851      	ldr	r1, [r2, #4]
 800ff56:	0389      	lsls	r1, r1, #14
 800ff58:	d4f9      	bmi.n	800ff4e <RCC_GetClocksFreq+0x5e>
 800ff5a:	4902      	ldr	r1, [pc, #8]	; (800ff64 <RCC_GetClocksFreq+0x74>)
 800ff5c:	e7f8      	b.n	800ff50 <RCC_GetClocksFreq+0x60>
 800ff5e:	bf00      	nop
 800ff60:	40021000 	.word	0x40021000
 800ff64:	007a1200 	.word	0x007a1200
 800ff68:	20000108 	.word	0x20000108
 800ff6c:	003d0900 	.word	0x003d0900

0800ff70 <RCC_AHBPeriphClockCmd>:
 800ff70:	4b04      	ldr	r3, [pc, #16]	; (800ff84 <RCC_AHBPeriphClockCmd+0x14>)
 800ff72:	695a      	ldr	r2, [r3, #20]
 800ff74:	b111      	cbz	r1, 800ff7c <RCC_AHBPeriphClockCmd+0xc>
 800ff76:	4310      	orrs	r0, r2
 800ff78:	6158      	str	r0, [r3, #20]
 800ff7a:	4770      	bx	lr
 800ff7c:	ea22 0000 	bic.w	r0, r2, r0
 800ff80:	e7fa      	b.n	800ff78 <RCC_AHBPeriphClockCmd+0x8>
 800ff82:	bf00      	nop
 800ff84:	40021000 	.word	0x40021000

0800ff88 <RCC_APB2PeriphClockCmd>:
 800ff88:	4b04      	ldr	r3, [pc, #16]	; (800ff9c <RCC_APB2PeriphClockCmd+0x14>)
 800ff8a:	699a      	ldr	r2, [r3, #24]
 800ff8c:	b111      	cbz	r1, 800ff94 <RCC_APB2PeriphClockCmd+0xc>
 800ff8e:	4310      	orrs	r0, r2
 800ff90:	6198      	str	r0, [r3, #24]
 800ff92:	4770      	bx	lr
 800ff94:	ea22 0000 	bic.w	r0, r2, r0
 800ff98:	e7fa      	b.n	800ff90 <RCC_APB2PeriphClockCmd+0x8>
 800ff9a:	bf00      	nop
 800ff9c:	40021000 	.word	0x40021000

0800ffa0 <RCC_APB1PeriphClockCmd>:
 800ffa0:	4b04      	ldr	r3, [pc, #16]	; (800ffb4 <RCC_APB1PeriphClockCmd+0x14>)
 800ffa2:	69da      	ldr	r2, [r3, #28]
 800ffa4:	b111      	cbz	r1, 800ffac <RCC_APB1PeriphClockCmd+0xc>
 800ffa6:	4310      	orrs	r0, r2
 800ffa8:	61d8      	str	r0, [r3, #28]
 800ffaa:	4770      	bx	lr
 800ffac:	ea22 0000 	bic.w	r0, r2, r0
 800ffb0:	e7fa      	b.n	800ffa8 <RCC_APB1PeriphClockCmd+0x8>
 800ffb2:	bf00      	nop
 800ffb4:	40021000 	.word	0x40021000

0800ffb8 <RCC_APB2PeriphResetCmd>:
 800ffb8:	4b04      	ldr	r3, [pc, #16]	; (800ffcc <RCC_APB2PeriphResetCmd+0x14>)
 800ffba:	68da      	ldr	r2, [r3, #12]
 800ffbc:	b111      	cbz	r1, 800ffc4 <RCC_APB2PeriphResetCmd+0xc>
 800ffbe:	4310      	orrs	r0, r2
 800ffc0:	60d8      	str	r0, [r3, #12]
 800ffc2:	4770      	bx	lr
 800ffc4:	ea22 0000 	bic.w	r0, r2, r0
 800ffc8:	e7fa      	b.n	800ffc0 <RCC_APB2PeriphResetCmd+0x8>
 800ffca:	bf00      	nop
 800ffcc:	40021000 	.word	0x40021000

0800ffd0 <RCC_APB1PeriphResetCmd>:
 800ffd0:	4b04      	ldr	r3, [pc, #16]	; (800ffe4 <RCC_APB1PeriphResetCmd+0x14>)
 800ffd2:	691a      	ldr	r2, [r3, #16]
 800ffd4:	b111      	cbz	r1, 800ffdc <RCC_APB1PeriphResetCmd+0xc>
 800ffd6:	4310      	orrs	r0, r2
 800ffd8:	6118      	str	r0, [r3, #16]
 800ffda:	4770      	bx	lr
 800ffdc:	ea22 0000 	bic.w	r0, r2, r0
 800ffe0:	e7fa      	b.n	800ffd8 <RCC_APB1PeriphResetCmd+0x8>
 800ffe2:	bf00      	nop
 800ffe4:	40021000 	.word	0x40021000

0800ffe8 <RCC_BackupResetCmd>:
 800ffe8:	4b01      	ldr	r3, [pc, #4]	; (800fff0 <RCC_BackupResetCmd+0x8>)
 800ffea:	6018      	str	r0, [r3, #0]
 800ffec:	4770      	bx	lr
 800ffee:	bf00      	nop
 800fff0:	42420440 	.word	0x42420440

0800fff4 <RCC_ClockSecuritySystemCmd>:
 800fff4:	4b01      	ldr	r3, [pc, #4]	; (800fffc <RCC_ClockSecuritySystemCmd+0x8>)
 800fff6:	6018      	str	r0, [r3, #0]
 800fff8:	4770      	bx	lr
 800fffa:	bf00      	nop
 800fffc:	4242004c 	.word	0x4242004c

08010000 <RCC_MCOConfig>:
 8010000:	4b01      	ldr	r3, [pc, #4]	; (8010008 <RCC_MCOConfig+0x8>)
 8010002:	7018      	strb	r0, [r3, #0]
 8010004:	4770      	bx	lr
 8010006:	bf00      	nop
 8010008:	40021007 	.word	0x40021007

0801000c <RCC_GetFlagStatus>:
 801000c:	0942      	lsrs	r2, r0, #5
 801000e:	2a01      	cmp	r2, #1
 8010010:	4b07      	ldr	r3, [pc, #28]	; (8010030 <RCC_GetFlagStatus+0x24>)
 8010012:	d107      	bne.n	8010024 <RCC_GetFlagStatus+0x18>
 8010014:	681b      	ldr	r3, [r3, #0]
 8010016:	f000 001f 	and.w	r0, r0, #31
 801001a:	fa23 f000 	lsr.w	r0, r3, r0
 801001e:	f000 0001 	and.w	r0, r0, #1
 8010022:	4770      	bx	lr
 8010024:	2a02      	cmp	r2, #2
 8010026:	bf0c      	ite	eq
 8010028:	6a1b      	ldreq	r3, [r3, #32]
 801002a:	6a5b      	ldrne	r3, [r3, #36]	; 0x24
 801002c:	e7f3      	b.n	8010016 <RCC_GetFlagStatus+0xa>
 801002e:	bf00      	nop
 8010030:	40021000 	.word	0x40021000

08010034 <RCC_WaitForHSEStartUp>:
 8010034:	2300      	movs	r3, #0
 8010036:	b507      	push	{r0, r1, r2, lr}
 8010038:	9301      	str	r3, [sp, #4]
 801003a:	2031      	movs	r0, #49	; 0x31
 801003c:	f7ff ffe6 	bl	801000c <RCC_GetFlagStatus>
 8010040:	9b01      	ldr	r3, [sp, #4]
 8010042:	3301      	adds	r3, #1
 8010044:	9301      	str	r3, [sp, #4]
 8010046:	9b01      	ldr	r3, [sp, #4]
 8010048:	f5b3 6fa0 	cmp.w	r3, #1280	; 0x500
 801004c:	d001      	beq.n	8010052 <RCC_WaitForHSEStartUp+0x1e>
 801004e:	2800      	cmp	r0, #0
 8010050:	d0f3      	beq.n	801003a <RCC_WaitForHSEStartUp+0x6>
 8010052:	2031      	movs	r0, #49	; 0x31
 8010054:	f7ff ffda 	bl	801000c <RCC_GetFlagStatus>
 8010058:	3000      	adds	r0, #0
 801005a:	bf18      	it	ne
 801005c:	2001      	movne	r0, #1
 801005e:	b003      	add	sp, #12
 8010060:	f85d fb04 	ldr.w	pc, [sp], #4

08010064 <RCC_ClearFlag>:
 8010064:	4a02      	ldr	r2, [pc, #8]	; (8010070 <RCC_ClearFlag+0xc>)
 8010066:	6a53      	ldr	r3, [r2, #36]	; 0x24
 8010068:	f043 7380 	orr.w	r3, r3, #16777216	; 0x1000000
 801006c:	6253      	str	r3, [r2, #36]	; 0x24
 801006e:	4770      	bx	lr
 8010070:	40021000 	.word	0x40021000

08010074 <RCC_GetITStatus>:
 8010074:	4b03      	ldr	r3, [pc, #12]	; (8010084 <RCC_GetITStatus+0x10>)
 8010076:	689b      	ldr	r3, [r3, #8]
 8010078:	4218      	tst	r0, r3
 801007a:	bf14      	ite	ne
 801007c:	2001      	movne	r0, #1
 801007e:	2000      	moveq	r0, #0
 8010080:	4770      	bx	lr
 8010082:	bf00      	nop
 8010084:	40021000 	.word	0x40021000

08010088 <RCC_ClearITPendingBit>:
 8010088:	4b01      	ldr	r3, [pc, #4]	; (8010090 <RCC_ClearITPendingBit+0x8>)
 801008a:	7018      	strb	r0, [r3, #0]
 801008c:	4770      	bx	lr
 801008e:	bf00      	nop
 8010090:	4002100a 	.word	0x4002100a

08010094 <FLASH_SetLatency>:
 8010094:	4a03      	ldr	r2, [pc, #12]	; (80100a4 <FLASH_SetLatency+0x10>)
 8010096:	6813      	ldr	r3, [r2, #0]
 8010098:	f003 0338 	and.w	r3, r3, #56	; 0x38
 801009c:	4318      	orrs	r0, r3
 801009e:	6010      	str	r0, [r2, #0]
 80100a0:	4770      	bx	lr
 80100a2:	bf00      	nop
 80100a4:	40022000 	.word	0x40022000

080100a8 <FLASH_HalfCycleAccessCmd>:
 80100a8:	4b04      	ldr	r3, [pc, #16]	; (80100bc <FLASH_HalfCycleAccessCmd+0x14>)
 80100aa:	681a      	ldr	r2, [r3, #0]
 80100ac:	f022 0208 	bic.w	r2, r2, #8
 80100b0:	601a      	str	r2, [r3, #0]
 80100b2:	681a      	ldr	r2, [r3, #0]
 80100b4:	4310      	orrs	r0, r2
 80100b6:	6018      	str	r0, [r3, #0]
 80100b8:	4770      	bx	lr
 80100ba:	bf00      	nop
 80100bc:	40022000 	.word	0x40022000

080100c0 <FLASH_PrefetchBufferCmd>:
 80100c0:	4b04      	ldr	r3, [pc, #16]	; (80100d4 <FLASH_PrefetchBufferCmd+0x14>)
 80100c2:	681a      	ldr	r2, [r3, #0]
 80100c4:	f022 0210 	bic.w	r2, r2, #16
 80100c8:	601a      	str	r2, [r3, #0]
 80100ca:	681a      	ldr	r2, [r3, #0]
 80100cc:	4310      	orrs	r0, r2
 80100ce:	6018      	str	r0, [r3, #0]
 80100d0:	4770      	bx	lr
 80100d2:	bf00      	nop
 80100d4:	40022000 	.word	0x40022000

080100d8 <FLASH_Unlock>:
 80100d8:	4b03      	ldr	r3, [pc, #12]	; (80100e8 <FLASH_Unlock+0x10>)
 80100da:	4a04      	ldr	r2, [pc, #16]	; (80100ec <FLASH_Unlock+0x14>)
 80100dc:	605a      	str	r2, [r3, #4]
 80100de:	f102 3288 	add.w	r2, r2, #2290649224	; 0x88888888
 80100e2:	605a      	str	r2, [r3, #4]
 80100e4:	4770      	bx	lr
 80100e6:	bf00      	nop
 80100e8:	40022000 	.word	0x40022000
 80100ec:	45670123 	.word	0x45670123

080100f0 <FLASH_UnlockBank1>:
 80100f0:	f7ff bff2 	b.w	80100d8 <FLASH_Unlock>

080100f4 <FLASH_Lock>:
 80100f4:	4a02      	ldr	r2, [pc, #8]	; (8010100 <FLASH_Lock+0xc>)
 80100f6:	6913      	ldr	r3, [r2, #16]
 80100f8:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 80100fc:	6113      	str	r3, [r2, #16]
 80100fe:	4770      	bx	lr
 8010100:	40022000 	.word	0x40022000

08010104 <FLASH_LockBank1>:
 8010104:	f7ff bff6 	b.w	80100f4 <FLASH_Lock>

08010108 <FLASH_GetUserOptionByte>:
 8010108:	4b01      	ldr	r3, [pc, #4]	; (8010110 <FLASH_GetUserOptionByte+0x8>)
 801010a:	69d8      	ldr	r0, [r3, #28]
 801010c:	0880      	lsrs	r0, r0, #2
 801010e:	4770      	bx	lr
 8010110:	40022000 	.word	0x40022000

08010114 <FLASH_GetWriteProtectionOptionByte>:
 8010114:	4b01      	ldr	r3, [pc, #4]	; (801011c <FLASH_GetWriteProtectionOptionByte+0x8>)
 8010116:	6a18      	ldr	r0, [r3, #32]
 8010118:	4770      	bx	lr
 801011a:	bf00      	nop
 801011c:	40022000 	.word	0x40022000

08010120 <FLASH_GetReadOutProtectionStatus>:
 8010120:	4b02      	ldr	r3, [pc, #8]	; (801012c <FLASH_GetReadOutProtectionStatus+0xc>)
 8010122:	69d8      	ldr	r0, [r3, #28]
 8010124:	f3c0 0040 	ubfx	r0, r0, #1, #1
 8010128:	4770      	bx	lr
 801012a:	bf00      	nop
 801012c:	40022000 	.word	0x40022000

08010130 <FLASH_GetPrefetchBufferStatus>:
 8010130:	4b02      	ldr	r3, [pc, #8]	; (801013c <FLASH_GetPrefetchBufferStatus+0xc>)
 8010132:	6818      	ldr	r0, [r3, #0]
 8010134:	f3c0 1040 	ubfx	r0, r0, #5, #1
 8010138:	4770      	bx	lr
 801013a:	bf00      	nop
 801013c:	40022000 	.word	0x40022000

08010140 <FLASH_ITConfig>:
 8010140:	4b04      	ldr	r3, [pc, #16]	; (8010154 <FLASH_ITConfig+0x14>)
 8010142:	691a      	ldr	r2, [r3, #16]
 8010144:	b111      	cbz	r1, 801014c <FLASH_ITConfig+0xc>
 8010146:	4310      	orrs	r0, r2
 8010148:	6118      	str	r0, [r3, #16]
 801014a:	4770      	bx	lr
 801014c:	ea22 0000 	bic.w	r0, r2, r0
 8010150:	e7fa      	b.n	8010148 <FLASH_ITConfig+0x8>
 8010152:	bf00      	nop
 8010154:	40022000 	.word	0x40022000

08010158 <FLASH_GetFlagStatus>:
 8010158:	2801      	cmp	r0, #1
 801015a:	4b06      	ldr	r3, [pc, #24]	; (8010174 <FLASH_GetFlagStatus+0x1c>)
 801015c:	d103      	bne.n	8010166 <FLASH_GetFlagStatus+0xe>
 801015e:	69d8      	ldr	r0, [r3, #28]
 8010160:	f000 0001 	and.w	r0, r0, #1
 8010164:	4770      	bx	lr
 8010166:	68db      	ldr	r3, [r3, #12]
 8010168:	4203      	tst	r3, r0
 801016a:	bf14      	ite	ne
 801016c:	2001      	movne	r0, #1
 801016e:	2000      	moveq	r0, #0
 8010170:	4770      	bx	lr
 8010172:	bf00      	nop
 8010174:	40022000 	.word	0x40022000

08010178 <FLASH_ClearFlag>:
 8010178:	4b01      	ldr	r3, [pc, #4]	; (8010180 <FLASH_ClearFlag+0x8>)
 801017a:	60d8      	str	r0, [r3, #12]
 801017c:	4770      	bx	lr
 801017e:	bf00      	nop
 8010180:	40022000 	.word	0x40022000

08010184 <FLASH_GetStatus>:
 8010184:	4b08      	ldr	r3, [pc, #32]	; (80101a8 <FLASH_GetStatus+0x24>)
 8010186:	68da      	ldr	r2, [r3, #12]
 8010188:	07d1      	lsls	r1, r2, #31
 801018a:	d409      	bmi.n	80101a0 <FLASH_GetStatus+0x1c>
 801018c:	68da      	ldr	r2, [r3, #12]
 801018e:	0752      	lsls	r2, r2, #29
 8010190:	d408      	bmi.n	80101a4 <FLASH_GetStatus+0x20>
 8010192:	68db      	ldr	r3, [r3, #12]
 8010194:	f013 0f10 	tst.w	r3, #16
 8010198:	bf14      	ite	ne
 801019a:	2003      	movne	r0, #3
 801019c:	2004      	moveq	r0, #4
 801019e:	4770      	bx	lr
 80101a0:	2001      	movs	r0, #1
 80101a2:	4770      	bx	lr
 80101a4:	2002      	movs	r0, #2
 80101a6:	4770      	bx	lr
 80101a8:	40022000 	.word	0x40022000

080101ac <FLASH_GetBank1Status>:
 80101ac:	4b08      	ldr	r3, [pc, #32]	; (80101d0 <FLASH_GetBank1Status+0x24>)
 80101ae:	68da      	ldr	r2, [r3, #12]
 80101b0:	07d1      	lsls	r1, r2, #31
 80101b2:	d409      	bmi.n	80101c8 <FLASH_GetBank1Status+0x1c>
 80101b4:	68da      	ldr	r2, [r3, #12]
 80101b6:	0752      	lsls	r2, r2, #29
 80101b8:	d408      	bmi.n	80101cc <FLASH_GetBank1Status+0x20>
 80101ba:	68db      	ldr	r3, [r3, #12]
 80101bc:	f013 0f10 	tst.w	r3, #16
 80101c0:	bf14      	ite	ne
 80101c2:	2003      	movne	r0, #3
 80101c4:	2004      	moveq	r0, #4
 80101c6:	4770      	bx	lr
 80101c8:	2001      	movs	r0, #1
 80101ca:	4770      	bx	lr
 80101cc:	2002      	movs	r0, #2
 80101ce:	4770      	bx	lr
 80101d0:	40022000 	.word	0x40022000

080101d4 <FLASH_WaitForLastOperation>:
 80101d4:	b510      	push	{r4, lr}
 80101d6:	4604      	mov	r4, r0
 80101d8:	f7ff ffe8 	bl	80101ac <FLASH_GetBank1Status>
 80101dc:	2801      	cmp	r0, #1
 80101de:	d106      	bne.n	80101ee <FLASH_WaitForLastOperation+0x1a>
 80101e0:	b90c      	cbnz	r4, 80101e6 <FLASH_WaitForLastOperation+0x12>
 80101e2:	2005      	movs	r0, #5
 80101e4:	bd10      	pop	{r4, pc}
 80101e6:	f7ff ffe1 	bl	80101ac <FLASH_GetBank1Status>
 80101ea:	3c01      	subs	r4, #1
 80101ec:	e7f6      	b.n	80101dc <FLASH_WaitForLastOperation+0x8>
 80101ee:	2c00      	cmp	r4, #0
 80101f0:	bf08      	it	eq
 80101f2:	2005      	moveq	r0, #5
 80101f4:	e7f6      	b.n	80101e4 <FLASH_WaitForLastOperation+0x10>

080101f6 <FLASH_ErasePage>:
 80101f6:	b538      	push	{r3, r4, r5, lr}
 80101f8:	4605      	mov	r5, r0
 80101fa:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 80101fe:	f7ff ffe9 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010202:	2804      	cmp	r0, #4
 8010204:	d113      	bne.n	801022e <FLASH_ErasePage+0x38>
 8010206:	4c0a      	ldr	r4, [pc, #40]	; (8010230 <FLASH_ErasePage+0x3a>)
 8010208:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 801020c:	6923      	ldr	r3, [r4, #16]
 801020e:	f043 0302 	orr.w	r3, r3, #2
 8010212:	6123      	str	r3, [r4, #16]
 8010214:	6165      	str	r5, [r4, #20]
 8010216:	6923      	ldr	r3, [r4, #16]
 8010218:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 801021c:	6123      	str	r3, [r4, #16]
 801021e:	f7ff ffd9 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010222:	6923      	ldr	r3, [r4, #16]
 8010224:	f023 0302 	bic.w	r3, r3, #2
 8010228:	04db      	lsls	r3, r3, #19
 801022a:	0cdb      	lsrs	r3, r3, #19
 801022c:	6123      	str	r3, [r4, #16]
 801022e:	bd38      	pop	{r3, r4, r5, pc}
 8010230:	40022000 	.word	0x40022000

08010234 <FLASH_EraseAllPages>:
 8010234:	b510      	push	{r4, lr}
 8010236:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 801023a:	f7ff ffcb 	bl	80101d4 <FLASH_WaitForLastOperation>
 801023e:	2804      	cmp	r0, #4
 8010240:	d112      	bne.n	8010268 <FLASH_EraseAllPages+0x34>
 8010242:	4c0a      	ldr	r4, [pc, #40]	; (801026c <FLASH_EraseAllPages+0x38>)
 8010244:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 8010248:	6923      	ldr	r3, [r4, #16]
 801024a:	f043 0304 	orr.w	r3, r3, #4
 801024e:	6123      	str	r3, [r4, #16]
 8010250:	6923      	ldr	r3, [r4, #16]
 8010252:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 8010256:	6123      	str	r3, [r4, #16]
 8010258:	f7ff ffbc 	bl	80101d4 <FLASH_WaitForLastOperation>
 801025c:	6923      	ldr	r3, [r4, #16]
 801025e:	f023 0304 	bic.w	r3, r3, #4
 8010262:	04db      	lsls	r3, r3, #19
 8010264:	0cdb      	lsrs	r3, r3, #19
 8010266:	6123      	str	r3, [r4, #16]
 8010268:	bd10      	pop	{r4, pc}
 801026a:	bf00      	nop
 801026c:	40022000 	.word	0x40022000

08010270 <FLASH_EraseAllBank1Pages>:
 8010270:	f7ff bfe0 	b.w	8010234 <FLASH_EraseAllPages>

08010274 <FLASH_EraseOptionBytes>:
 8010274:	b538      	push	{r3, r4, r5, lr}
 8010276:	f7ff ff53 	bl	8010120 <FLASH_GetReadOutProtectionStatus>
 801027a:	2800      	cmp	r0, #0
 801027c:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 8010280:	bf0c      	ite	eq
 8010282:	25a5      	moveq	r5, #165	; 0xa5
 8010284:	2500      	movne	r5, #0
 8010286:	f7ff ffa5 	bl	80101d4 <FLASH_WaitForLastOperation>
 801028a:	2804      	cmp	r0, #4
 801028c:	d12b      	bne.n	80102e6 <FLASH_EraseOptionBytes+0x72>
 801028e:	4c16      	ldr	r4, [pc, #88]	; (80102e8 <FLASH_EraseOptionBytes+0x74>)
 8010290:	4b16      	ldr	r3, [pc, #88]	; (80102ec <FLASH_EraseOptionBytes+0x78>)
 8010292:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 8010296:	60a3      	str	r3, [r4, #8]
 8010298:	f103 3388 	add.w	r3, r3, #2290649224	; 0x88888888
 801029c:	60a3      	str	r3, [r4, #8]
 801029e:	6923      	ldr	r3, [r4, #16]
 80102a0:	f043 0320 	orr.w	r3, r3, #32
 80102a4:	6123      	str	r3, [r4, #16]
 80102a6:	6923      	ldr	r3, [r4, #16]
 80102a8:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 80102ac:	6123      	str	r3, [r4, #16]
 80102ae:	f7ff ff91 	bl	80101d4 <FLASH_WaitForLastOperation>
 80102b2:	2804      	cmp	r0, #4
 80102b4:	d10f      	bne.n	80102d6 <FLASH_EraseOptionBytes+0x62>
 80102b6:	6923      	ldr	r3, [r4, #16]
 80102b8:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 80102bc:	f023 0320 	bic.w	r3, r3, #32
 80102c0:	04db      	lsls	r3, r3, #19
 80102c2:	0cdb      	lsrs	r3, r3, #19
 80102c4:	6123      	str	r3, [r4, #16]
 80102c6:	6923      	ldr	r3, [r4, #16]
 80102c8:	f043 0310 	orr.w	r3, r3, #16
 80102cc:	6123      	str	r3, [r4, #16]
 80102ce:	4b08      	ldr	r3, [pc, #32]	; (80102f0 <FLASH_EraseOptionBytes+0x7c>)
 80102d0:	801d      	strh	r5, [r3, #0]
 80102d2:	f7ff ff7f 	bl	80101d4 <FLASH_WaitForLastOperation>
 80102d6:	2805      	cmp	r0, #5
 80102d8:	d005      	beq.n	80102e6 <FLASH_EraseOptionBytes+0x72>
 80102da:	6923      	ldr	r3, [r4, #16]
 80102dc:	f023 0310 	bic.w	r3, r3, #16
 80102e0:	04db      	lsls	r3, r3, #19
 80102e2:	0cdb      	lsrs	r3, r3, #19
 80102e4:	6123      	str	r3, [r4, #16]
 80102e6:	bd38      	pop	{r3, r4, r5, pc}
 80102e8:	40022000 	.word	0x40022000
 80102ec:	45670123 	.word	0x45670123
 80102f0:	1ffff800 	.word	0x1ffff800

080102f4 <FLASH_ProgramWord>:
 80102f4:	2300      	movs	r3, #0
 80102f6:	b573      	push	{r0, r1, r4, r5, r6, lr}
 80102f8:	4606      	mov	r6, r0
 80102fa:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 80102fe:	460d      	mov	r5, r1
 8010300:	9301      	str	r3, [sp, #4]
 8010302:	f7ff ff67 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010306:	2804      	cmp	r0, #4
 8010308:	d11b      	bne.n	8010342 <FLASH_ProgramWord+0x4e>
 801030a:	4c0f      	ldr	r4, [pc, #60]	; (8010348 <FLASH_ProgramWord+0x54>)
 801030c:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010310:	6923      	ldr	r3, [r4, #16]
 8010312:	f043 0301 	orr.w	r3, r3, #1
 8010316:	6123      	str	r3, [r4, #16]
 8010318:	b2ab      	uxth	r3, r5
 801031a:	8033      	strh	r3, [r6, #0]
 801031c:	f7ff ff5a 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010320:	2804      	cmp	r0, #4
 8010322:	d108      	bne.n	8010336 <FLASH_ProgramWord+0x42>
 8010324:	3602      	adds	r6, #2
 8010326:	9601      	str	r6, [sp, #4]
 8010328:	9b01      	ldr	r3, [sp, #4]
 801032a:	0c2d      	lsrs	r5, r5, #16
 801032c:	801d      	strh	r5, [r3, #0]
 801032e:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010332:	f7ff ff4f 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010336:	6923      	ldr	r3, [r4, #16]
 8010338:	f023 0301 	bic.w	r3, r3, #1
 801033c:	04db      	lsls	r3, r3, #19
 801033e:	0cdb      	lsrs	r3, r3, #19
 8010340:	6123      	str	r3, [r4, #16]
 8010342:	b002      	add	sp, #8
 8010344:	bd70      	pop	{r4, r5, r6, pc}
 8010346:	bf00      	nop
 8010348:	40022000 	.word	0x40022000

0801034c <FLASH_ProgramHalfWord>:
 801034c:	b570      	push	{r4, r5, r6, lr}
 801034e:	4605      	mov	r5, r0
 8010350:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010354:	460e      	mov	r6, r1
 8010356:	f7ff ff3d 	bl	80101d4 <FLASH_WaitForLastOperation>
 801035a:	2804      	cmp	r0, #4
 801035c:	d10f      	bne.n	801037e <FLASH_ProgramHalfWord+0x32>
 801035e:	4c08      	ldr	r4, [pc, #32]	; (8010380 <FLASH_ProgramHalfWord+0x34>)
 8010360:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010364:	6923      	ldr	r3, [r4, #16]
 8010366:	f043 0301 	orr.w	r3, r3, #1
 801036a:	6123      	str	r3, [r4, #16]
 801036c:	802e      	strh	r6, [r5, #0]
 801036e:	f7ff ff31 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010372:	6923      	ldr	r3, [r4, #16]
 8010374:	f023 0301 	bic.w	r3, r3, #1
 8010378:	04db      	lsls	r3, r3, #19
 801037a:	0cdb      	lsrs	r3, r3, #19
 801037c:	6123      	str	r3, [r4, #16]
 801037e:	bd70      	pop	{r4, r5, r6, pc}
 8010380:	40022000 	.word	0x40022000

08010384 <FLASH_ProgramOptionByteData>:
 8010384:	b570      	push	{r4, r5, r6, lr}
 8010386:	4606      	mov	r6, r0
 8010388:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 801038c:	460d      	mov	r5, r1
 801038e:	f7ff ff21 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010392:	2804      	cmp	r0, #4
 8010394:	d117      	bne.n	80103c6 <FLASH_ProgramOptionByteData+0x42>
 8010396:	4c0c      	ldr	r4, [pc, #48]	; (80103c8 <FLASH_ProgramOptionByteData+0x44>)
 8010398:	4b0c      	ldr	r3, [pc, #48]	; (80103cc <FLASH_ProgramOptionByteData+0x48>)
 801039a:	b2ad      	uxth	r5, r5
 801039c:	60a3      	str	r3, [r4, #8]
 801039e:	f103 3388 	add.w	r3, r3, #2290649224	; 0x88888888
 80103a2:	60a3      	str	r3, [r4, #8]
 80103a4:	6923      	ldr	r3, [r4, #16]
 80103a6:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 80103aa:	f043 0310 	orr.w	r3, r3, #16
 80103ae:	6123      	str	r3, [r4, #16]
 80103b0:	8035      	strh	r5, [r6, #0]
 80103b2:	f7ff ff0f 	bl	80101d4 <FLASH_WaitForLastOperation>
 80103b6:	2805      	cmp	r0, #5
 80103b8:	d005      	beq.n	80103c6 <FLASH_ProgramOptionByteData+0x42>
 80103ba:	6923      	ldr	r3, [r4, #16]
 80103bc:	f023 0310 	bic.w	r3, r3, #16
 80103c0:	04db      	lsls	r3, r3, #19
 80103c2:	0cdb      	lsrs	r3, r3, #19
 80103c4:	6123      	str	r3, [r4, #16]
 80103c6:	bd70      	pop	{r4, r5, r6, pc}
 80103c8:	40022000 	.word	0x40022000
 80103cc:	45670123 	.word	0x45670123

080103d0 <FLASH_EnableWriteProtection>:
 80103d0:	b510      	push	{r4, lr}
 80103d2:	4604      	mov	r4, r0
 80103d4:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 80103d8:	f7ff fefc 	bl	80101d4 <FLASH_WaitForLastOperation>
 80103dc:	2804      	cmp	r0, #4
 80103de:	d13b      	bne.n	8010458 <FLASH_EnableWriteProtection+0x88>
 80103e0:	43e4      	mvns	r4, r4
 80103e2:	4b22      	ldr	r3, [pc, #136]	; (801046c <FLASH_EnableWriteProtection+0x9c>)
 80103e4:	4a22      	ldr	r2, [pc, #136]	; (8010470 <FLASH_EnableWriteProtection+0xa0>)
 80103e6:	b2e1      	uxtb	r1, r4
 80103e8:	609a      	str	r2, [r3, #8]
 80103ea:	f102 3288 	add.w	r2, r2, #2290649224	; 0x88888888
 80103ee:	609a      	str	r2, [r3, #8]
 80103f0:	691a      	ldr	r2, [r3, #16]
 80103f2:	29ff      	cmp	r1, #255	; 0xff
 80103f4:	f042 0210 	orr.w	r2, r2, #16
 80103f8:	611a      	str	r2, [r3, #16]
 80103fa:	d123      	bne.n	8010444 <FLASH_EnableWriteProtection+0x74>
 80103fc:	f3c4 2307 	ubfx	r3, r4, #8, #8
 8010400:	2bff      	cmp	r3, #255	; 0xff
 8010402:	d007      	beq.n	8010414 <FLASH_EnableWriteProtection+0x44>
 8010404:	4a1b      	ldr	r2, [pc, #108]	; (8010474 <FLASH_EnableWriteProtection+0xa4>)
 8010406:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 801040a:	8153      	strh	r3, [r2, #10]
 801040c:	f7ff fee2 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010410:	2804      	cmp	r0, #4
 8010412:	d11f      	bne.n	8010454 <FLASH_EnableWriteProtection+0x84>
 8010414:	f3c4 4307 	ubfx	r3, r4, #16, #8
 8010418:	2bff      	cmp	r3, #255	; 0xff
 801041a:	d007      	beq.n	801042c <FLASH_EnableWriteProtection+0x5c>
 801041c:	4a15      	ldr	r2, [pc, #84]	; (8010474 <FLASH_EnableWriteProtection+0xa4>)
 801041e:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010422:	8193      	strh	r3, [r2, #12]
 8010424:	f7ff fed6 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010428:	2804      	cmp	r0, #4
 801042a:	d113      	bne.n	8010454 <FLASH_EnableWriteProtection+0x84>
 801042c:	0e24      	lsrs	r4, r4, #24
 801042e:	2cff      	cmp	r4, #255	; 0xff
 8010430:	d113      	bne.n	801045a <FLASH_EnableWriteProtection+0x8a>
 8010432:	2004      	movs	r0, #4
 8010434:	4a0d      	ldr	r2, [pc, #52]	; (801046c <FLASH_EnableWriteProtection+0x9c>)
 8010436:	6913      	ldr	r3, [r2, #16]
 8010438:	f023 0310 	bic.w	r3, r3, #16
 801043c:	04db      	lsls	r3, r3, #19
 801043e:	0cdb      	lsrs	r3, r3, #19
 8010440:	6113      	str	r3, [r2, #16]
 8010442:	e009      	b.n	8010458 <FLASH_EnableWriteProtection+0x88>
 8010444:	4b0b      	ldr	r3, [pc, #44]	; (8010474 <FLASH_EnableWriteProtection+0xa4>)
 8010446:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 801044a:	8119      	strh	r1, [r3, #8]
 801044c:	f7ff fec2 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010450:	2804      	cmp	r0, #4
 8010452:	d0d3      	beq.n	80103fc <FLASH_EnableWriteProtection+0x2c>
 8010454:	2805      	cmp	r0, #5
 8010456:	d1ed      	bne.n	8010434 <FLASH_EnableWriteProtection+0x64>
 8010458:	bd10      	pop	{r4, pc}
 801045a:	4b06      	ldr	r3, [pc, #24]	; (8010474 <FLASH_EnableWriteProtection+0xa4>)
 801045c:	b2a4      	uxth	r4, r4
 801045e:	81dc      	strh	r4, [r3, #14]
 8010460:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010464:	f7ff feb6 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010468:	e7f4      	b.n	8010454 <FLASH_EnableWriteProtection+0x84>
 801046a:	bf00      	nop
 801046c:	40022000 	.word	0x40022000
 8010470:	45670123 	.word	0x45670123
 8010474:	1ffff800 	.word	0x1ffff800

08010478 <FLASH_ReadOutProtection>:
 8010478:	b538      	push	{r3, r4, r5, lr}
 801047a:	4605      	mov	r5, r0
 801047c:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 8010480:	f7ff fea8 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010484:	2804      	cmp	r0, #4
 8010486:	d12e      	bne.n	80104e6 <FLASH_ReadOutProtection+0x6e>
 8010488:	4c1d      	ldr	r4, [pc, #116]	; (8010500 <FLASH_ReadOutProtection+0x88>)
 801048a:	4b1e      	ldr	r3, [pc, #120]	; (8010504 <FLASH_ReadOutProtection+0x8c>)
 801048c:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 8010490:	60a3      	str	r3, [r4, #8]
 8010492:	f103 3388 	add.w	r3, r3, #2290649224	; 0x88888888
 8010496:	60a3      	str	r3, [r4, #8]
 8010498:	6923      	ldr	r3, [r4, #16]
 801049a:	f043 0320 	orr.w	r3, r3, #32
 801049e:	6123      	str	r3, [r4, #16]
 80104a0:	6923      	ldr	r3, [r4, #16]
 80104a2:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 80104a6:	6123      	str	r3, [r4, #16]
 80104a8:	f7ff fe94 	bl	80101d4 <FLASH_WaitForLastOperation>
 80104ac:	2804      	cmp	r0, #4
 80104ae:	d11d      	bne.n	80104ec <FLASH_ReadOutProtection+0x74>
 80104b0:	6923      	ldr	r3, [r4, #16]
 80104b2:	f023 0320 	bic.w	r3, r3, #32
 80104b6:	04db      	lsls	r3, r3, #19
 80104b8:	0cdb      	lsrs	r3, r3, #19
 80104ba:	6123      	str	r3, [r4, #16]
 80104bc:	6923      	ldr	r3, [r4, #16]
 80104be:	f043 0310 	orr.w	r3, r3, #16
 80104c2:	6123      	str	r3, [r4, #16]
 80104c4:	4b10      	ldr	r3, [pc, #64]	; (8010508 <FLASH_ReadOutProtection+0x90>)
 80104c6:	b17d      	cbz	r5, 80104e8 <FLASH_ReadOutProtection+0x70>
 80104c8:	2200      	movs	r2, #0
 80104ca:	801a      	strh	r2, [r3, #0]
 80104cc:	f44f 2030 	mov.w	r0, #720896	; 0xb0000
 80104d0:	f7ff fe80 	bl	80101d4 <FLASH_WaitForLastOperation>
 80104d4:	2805      	cmp	r0, #5
 80104d6:	d006      	beq.n	80104e6 <FLASH_ReadOutProtection+0x6e>
 80104d8:	4a09      	ldr	r2, [pc, #36]	; (8010500 <FLASH_ReadOutProtection+0x88>)
 80104da:	6913      	ldr	r3, [r2, #16]
 80104dc:	f023 0310 	bic.w	r3, r3, #16
 80104e0:	04db      	lsls	r3, r3, #19
 80104e2:	0cdb      	lsrs	r3, r3, #19
 80104e4:	6113      	str	r3, [r2, #16]
 80104e6:	bd38      	pop	{r3, r4, r5, pc}
 80104e8:	22a5      	movs	r2, #165	; 0xa5
 80104ea:	e7ee      	b.n	80104ca <FLASH_ReadOutProtection+0x52>
 80104ec:	2805      	cmp	r0, #5
 80104ee:	d0fa      	beq.n	80104e6 <FLASH_ReadOutProtection+0x6e>
 80104f0:	6923      	ldr	r3, [r4, #16]
 80104f2:	f023 0320 	bic.w	r3, r3, #32
 80104f6:	04db      	lsls	r3, r3, #19
 80104f8:	0cdb      	lsrs	r3, r3, #19
 80104fa:	6123      	str	r3, [r4, #16]
 80104fc:	e7f3      	b.n	80104e6 <FLASH_ReadOutProtection+0x6e>
 80104fe:	bf00      	nop
 8010500:	40022000 	.word	0x40022000
 8010504:	45670123 	.word	0x45670123
 8010508:	1ffff800 	.word	0x1ffff800

0801050c <FLASH_UserOptionByteConfig>:
 801050c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 801050e:	4c13      	ldr	r4, [pc, #76]	; (801055c <FLASH_UserOptionByteConfig+0x50>)
 8010510:	4b13      	ldr	r3, [pc, #76]	; (8010560 <FLASH_UserOptionByteConfig+0x54>)
 8010512:	4607      	mov	r7, r0
 8010514:	60a3      	str	r3, [r4, #8]
 8010516:	f103 3388 	add.w	r3, r3, #2290649224	; 0x88888888
 801051a:	60a3      	str	r3, [r4, #8]
 801051c:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010520:	460e      	mov	r6, r1
 8010522:	4615      	mov	r5, r2
 8010524:	f7ff fe56 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010528:	2804      	cmp	r0, #4
 801052a:	d115      	bne.n	8010558 <FLASH_UserOptionByteConfig+0x4c>
 801052c:	6923      	ldr	r3, [r4, #16]
 801052e:	f047 07f8 	orr.w	r7, r7, #248	; 0xf8
 8010532:	f043 0310 	orr.w	r3, r3, #16
 8010536:	6123      	str	r3, [r4, #16]
 8010538:	433e      	orrs	r6, r7
 801053a:	4b0a      	ldr	r3, [pc, #40]	; (8010564 <FLASH_UserOptionByteConfig+0x58>)
 801053c:	4335      	orrs	r5, r6
 801053e:	805d      	strh	r5, [r3, #2]
 8010540:	f44f 5000 	mov.w	r0, #8192	; 0x2000
 8010544:	f7ff fe46 	bl	80101d4 <FLASH_WaitForLastOperation>
 8010548:	2805      	cmp	r0, #5
 801054a:	d005      	beq.n	8010558 <FLASH_UserOptionByteConfig+0x4c>
 801054c:	6923      	ldr	r3, [r4, #16]
 801054e:	f023 0310 	bic.w	r3, r3, #16
 8010552:	04db      	lsls	r3, r3, #19
 8010554:	0cdb      	lsrs	r3, r3, #19
 8010556:	6123      	str	r3, [r4, #16]
 8010558:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 801055a:	bf00      	nop
 801055c:	40022000 	.word	0x40022000
 8010560:	45670123 	.word	0x45670123
 8010564:	1ffff800 	.word	0x1ffff800

08010568 <FLASH_WaitForLastBank1Operation>:
 8010568:	f7ff be34 	b.w	80101d4 <FLASH_WaitForLastOperation>

0801056c <NVIC_PriorityGroupConfig>:
 801056c:	f040 60bf 	orr.w	r0, r0, #100139008	; 0x5f80000
 8010570:	4b02      	ldr	r3, [pc, #8]	; (801057c <NVIC_PriorityGroupConfig+0x10>)
 8010572:	f440 3000 	orr.w	r0, r0, #131072	; 0x20000
 8010576:	60d8      	str	r0, [r3, #12]
 8010578:	4770      	bx	lr
 801057a:	bf00      	nop
 801057c:	e000ed00 	.word	0xe000ed00

08010580 <NVIC_Init>:
 8010580:	b570      	push	{r4, r5, r6, lr}
 8010582:	2501      	movs	r5, #1
 8010584:	7801      	ldrb	r1, [r0, #0]
 8010586:	f001 031f 	and.w	r3, r1, #31
 801058a:	409d      	lsls	r5, r3
 801058c:	78c3      	ldrb	r3, [r0, #3]
 801058e:	094c      	lsrs	r4, r1, #5
 8010590:	b1c3      	cbz	r3, 80105c4 <NVIC_Init+0x44>
 8010592:	4b0f      	ldr	r3, [pc, #60]	; (80105d0 <NVIC_Init+0x50>)
 8010594:	68da      	ldr	r2, [r3, #12]
 8010596:	7843      	ldrb	r3, [r0, #1]
 8010598:	43d2      	mvns	r2, r2
 801059a:	f3c2 2202 	ubfx	r2, r2, #8, #3
 801059e:	f1c2 0604 	rsb	r6, r2, #4
 80105a2:	fa03 f606 	lsl.w	r6, r3, r6
 80105a6:	7883      	ldrb	r3, [r0, #2]
 80105a8:	200f      	movs	r0, #15
 80105aa:	fa20 f202 	lsr.w	r2, r0, r2
 80105ae:	4013      	ands	r3, r2
 80105b0:	4333      	orrs	r3, r6
 80105b2:	4a08      	ldr	r2, [pc, #32]	; (80105d4 <NVIC_Init+0x54>)
 80105b4:	011b      	lsls	r3, r3, #4
 80105b6:	b2db      	uxtb	r3, r3
 80105b8:	4411      	add	r1, r2
 80105ba:	f881 3300 	strb.w	r3, [r1, #768]	; 0x300
 80105be:	f842 5024 	str.w	r5, [r2, r4, lsl #2]
 80105c2:	bd70      	pop	{r4, r5, r6, pc}
 80105c4:	4b03      	ldr	r3, [pc, #12]	; (80105d4 <NVIC_Init+0x54>)
 80105c6:	3420      	adds	r4, #32
 80105c8:	f843 5024 	str.w	r5, [r3, r4, lsl #2]
 80105cc:	e7f9      	b.n	80105c2 <NVIC_Init+0x42>
 80105ce:	bf00      	nop
 80105d0:	e000ed00 	.word	0xe000ed00
 80105d4:	e000e100 	.word	0xe000e100

080105d8 <NVIC_SetVectorTable>:
 80105d8:	f021 4160 	bic.w	r1, r1, #3758096384	; 0xe0000000
 80105dc:	f021 017f 	bic.w	r1, r1, #127	; 0x7f
 80105e0:	4b01      	ldr	r3, [pc, #4]	; (80105e8 <NVIC_SetVectorTable+0x10>)
 80105e2:	4301      	orrs	r1, r0
 80105e4:	6099      	str	r1, [r3, #8]
 80105e6:	4770      	bx	lr
 80105e8:	e000ed00 	.word	0xe000ed00

080105ec <NVIC_SystemLPConfig>:
 80105ec:	4b04      	ldr	r3, [pc, #16]	; (8010600 <NVIC_SystemLPConfig+0x14>)
 80105ee:	691a      	ldr	r2, [r3, #16]
 80105f0:	b111      	cbz	r1, 80105f8 <NVIC_SystemLPConfig+0xc>
 80105f2:	4310      	orrs	r0, r2
 80105f4:	6118      	str	r0, [r3, #16]
 80105f6:	4770      	bx	lr
 80105f8:	ea22 0000 	bic.w	r0, r2, r0
 80105fc:	e7fa      	b.n	80105f4 <NVIC_SystemLPConfig+0x8>
 80105fe:	bf00      	nop
 8010600:	e000ed00 	.word	0xe000ed00

08010604 <SysTick_CLKSourceConfig>:
 8010604:	4b04      	ldr	r3, [pc, #16]	; (8010618 <SysTick_CLKSourceConfig+0x14>)
 8010606:	2804      	cmp	r0, #4
 8010608:	681a      	ldr	r2, [r3, #0]
 801060a:	bf0c      	ite	eq
 801060c:	f042 0204 	orreq.w	r2, r2, #4
 8010610:	f022 0204 	bicne.w	r2, r2, #4
 8010614:	601a      	str	r2, [r3, #0]
 8010616:	4770      	bx	lr
 8010618:	e000e010 	.word	0xe000e010

0801061c <SystemInit>:
 801061c:	4b2e      	ldr	r3, [pc, #184]	; (80106d8 <SystemInit+0xbc>)
 801061e:	681a      	ldr	r2, [r3, #0]
 8010620:	f042 0201 	orr.w	r2, r2, #1
 8010624:	601a      	str	r2, [r3, #0]
 8010626:	6859      	ldr	r1, [r3, #4]
 8010628:	4a2c      	ldr	r2, [pc, #176]	; (80106dc <SystemInit+0xc0>)
 801062a:	400a      	ands	r2, r1
 801062c:	605a      	str	r2, [r3, #4]
 801062e:	681a      	ldr	r2, [r3, #0]
 8010630:	f022 7284 	bic.w	r2, r2, #17301504	; 0x1080000
 8010634:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
 8010638:	601a      	str	r2, [r3, #0]
 801063a:	681a      	ldr	r2, [r3, #0]
 801063c:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
 8010640:	601a      	str	r2, [r3, #0]
 8010642:	685a      	ldr	r2, [r3, #4]
 8010644:	f422 02fe 	bic.w	r2, r2, #8323072	; 0x7f0000
 8010648:	605a      	str	r2, [r3, #4]
 801064a:	f44f 021f 	mov.w	r2, #10420224	; 0x9f0000
 801064e:	609a      	str	r2, [r3, #8]
 8010650:	681a      	ldr	r2, [r3, #0]
 8010652:	f442 3280 	orr.w	r2, r2, #65536	; 0x10000
 8010656:	601a      	str	r2, [r3, #0]
 8010658:	f44f 62a0 	mov.w	r2, #1280	; 0x500
 801065c:	6819      	ldr	r1, [r3, #0]
 801065e:	0388      	lsls	r0, r1, #14
 8010660:	d401      	bmi.n	8010666 <SystemInit+0x4a>
 8010662:	3a01      	subs	r2, #1
 8010664:	d1fa      	bne.n	801065c <SystemInit+0x40>
 8010666:	681a      	ldr	r2, [r3, #0]
 8010668:	0391      	lsls	r1, r2, #14
 801066a:	d530      	bpl.n	80106ce <SystemInit+0xb2>
 801066c:	4a1c      	ldr	r2, [pc, #112]	; (80106e0 <SystemInit+0xc4>)
 801066e:	6811      	ldr	r1, [r2, #0]
 8010670:	f041 0110 	orr.w	r1, r1, #16
 8010674:	6011      	str	r1, [r2, #0]
 8010676:	6811      	ldr	r1, [r2, #0]
 8010678:	f021 0103 	bic.w	r1, r1, #3
 801067c:	6011      	str	r1, [r2, #0]
 801067e:	6811      	ldr	r1, [r2, #0]
 8010680:	f041 0102 	orr.w	r1, r1, #2
 8010684:	6011      	str	r1, [r2, #0]
 8010686:	685a      	ldr	r2, [r3, #4]
 8010688:	605a      	str	r2, [r3, #4]
 801068a:	685a      	ldr	r2, [r3, #4]
 801068c:	605a      	str	r2, [r3, #4]
 801068e:	685a      	ldr	r2, [r3, #4]
 8010690:	f442 6280 	orr.w	r2, r2, #1024	; 0x400
 8010694:	605a      	str	r2, [r3, #4]
 8010696:	685a      	ldr	r2, [r3, #4]
 8010698:	f422 127c 	bic.w	r2, r2, #4128768	; 0x3f0000
 801069c:	605a      	str	r2, [r3, #4]
 801069e:	685a      	ldr	r2, [r3, #4]
 80106a0:	f442 12e8 	orr.w	r2, r2, #1900544	; 0x1d0000
 80106a4:	605a      	str	r2, [r3, #4]
 80106a6:	681a      	ldr	r2, [r3, #0]
 80106a8:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
 80106ac:	601a      	str	r2, [r3, #0]
 80106ae:	681a      	ldr	r2, [r3, #0]
 80106b0:	0192      	lsls	r2, r2, #6
 80106b2:	d5fc      	bpl.n	80106ae <SystemInit+0x92>
 80106b4:	685a      	ldr	r2, [r3, #4]
 80106b6:	f022 0203 	bic.w	r2, r2, #3
 80106ba:	605a      	str	r2, [r3, #4]
 80106bc:	685a      	ldr	r2, [r3, #4]
 80106be:	f042 0202 	orr.w	r2, r2, #2
 80106c2:	605a      	str	r2, [r3, #4]
 80106c4:	685a      	ldr	r2, [r3, #4]
 80106c6:	f002 020c 	and.w	r2, r2, #12
 80106ca:	2a08      	cmp	r2, #8
 80106cc:	d1fa      	bne.n	80106c4 <SystemInit+0xa8>
 80106ce:	f04f 6200 	mov.w	r2, #134217728	; 0x8000000
 80106d2:	4b04      	ldr	r3, [pc, #16]	; (80106e4 <SystemInit+0xc8>)
 80106d4:	609a      	str	r2, [r3, #8]
 80106d6:	4770      	bx	lr
 80106d8:	40021000 	.word	0x40021000
 80106dc:	f8ff0000 	.word	0xf8ff0000
 80106e0:	40022000 	.word	0x40022000
 80106e4:	e000ed00 	.word	0xe000ed00

080106e8 <SystemCoreClockUpdate>:
 80106e8:	4b0d      	ldr	r3, [pc, #52]	; (8010720 <SystemCoreClockUpdate+0x38>)
 80106ea:	685a      	ldr	r2, [r3, #4]
 80106ec:	f002 020c 	and.w	r2, r2, #12
 80106f0:	2a08      	cmp	r2, #8
 80106f2:	4a0c      	ldr	r2, [pc, #48]	; (8010724 <SystemCoreClockUpdate+0x3c>)
 80106f4:	d00a      	beq.n	801070c <SystemCoreClockUpdate+0x24>
 80106f6:	490c      	ldr	r1, [pc, #48]	; (8010728 <SystemCoreClockUpdate+0x40>)
 80106f8:	6011      	str	r1, [r2, #0]
 80106fa:	685b      	ldr	r3, [r3, #4]
 80106fc:	f3c3 1303 	ubfx	r3, r3, #4, #4
 8010700:	4413      	add	r3, r2
 8010702:	7919      	ldrb	r1, [r3, #4]
 8010704:	6813      	ldr	r3, [r2, #0]
 8010706:	40cb      	lsrs	r3, r1
 8010708:	6013      	str	r3, [r2, #0]
 801070a:	4770      	bx	lr
 801070c:	6859      	ldr	r1, [r3, #4]
 801070e:	6858      	ldr	r0, [r3, #4]
 8010710:	03c0      	lsls	r0, r0, #15
 8010712:	d4f2      	bmi.n	80106fa <SystemCoreClockUpdate+0x12>
 8010714:	f3c1 4183 	ubfx	r1, r1, #18, #4
 8010718:	4804      	ldr	r0, [pc, #16]	; (801072c <SystemCoreClockUpdate+0x44>)
 801071a:	3102      	adds	r1, #2
 801071c:	4341      	muls	r1, r0
 801071e:	e7eb      	b.n	80106f8 <SystemCoreClockUpdate+0x10>
 8010720:	40021000 	.word	0x40021000
 8010724:	2000011c 	.word	0x2000011c
 8010728:	007a1200 	.word	0x007a1200
 801072c:	003d0900 	.word	0x003d0900

08010730 <USB_Init>:
 8010730:	4b05      	ldr	r3, [pc, #20]	; (8010748 <USB_Init+0x18>)
 8010732:	4906      	ldr	r1, [pc, #24]	; (801074c <USB_Init+0x1c>)
 8010734:	1d1a      	adds	r2, r3, #4
 8010736:	601a      	str	r2, [r3, #0]
 8010738:	2202      	movs	r2, #2
 801073a:	731a      	strb	r2, [r3, #12]
 801073c:	4a04      	ldr	r2, [pc, #16]	; (8010750 <USB_Init+0x20>)
 801073e:	6259      	str	r1, [r3, #36]	; 0x24
 8010740:	621a      	str	r2, [r3, #32]
 8010742:	6813      	ldr	r3, [r2, #0]
 8010744:	4718      	bx	r3
 8010746:	bf00      	nop
 8010748:	20009504 	.word	0x20009504
 801074c:	200001c4 	.word	0x200001c4
 8010750:	2000015c 	.word	0x2000015c

08010754 <Bot_Abort.part.1>:
 8010754:	b508      	push	{r3, lr}
 8010756:	2110      	movs	r1, #16
 8010758:	2001      	movs	r0, #1
 801075a:	f000 fb09 	bl	8010d70 <SetEPTxStatus>
 801075e:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8010762:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 8010766:	2002      	movs	r0, #2
 8010768:	f000 bb1b 	b.w	8010da2 <SetEPRxStatus>

0801076c <Transfer_Data_Request>:
 801076c:	b510      	push	{r4, lr}
 801076e:	460c      	mov	r4, r1
 8010770:	460a      	mov	r2, r1
 8010772:	2198      	movs	r1, #152	; 0x98
 8010774:	f000 fdf8 	bl	8011368 <UserToPMABufferCopy>
 8010778:	4621      	mov	r1, r4
 801077a:	2001      	movs	r0, #1
 801077c:	f000 fc6c 	bl	8011058 <SetEPTxCount>
 8010780:	2130      	movs	r1, #48	; 0x30
 8010782:	2001      	movs	r0, #1
 8010784:	f000 faf4 	bl	8010d70 <SetEPTxStatus>
 8010788:	2203      	movs	r2, #3
 801078a:	4b04      	ldr	r3, [pc, #16]	; (801079c <Transfer_Data_Request+0x30>)
 801078c:	701a      	strb	r2, [r3, #0]
 801078e:	2200      	movs	r2, #0
 8010790:	68d9      	ldr	r1, [r3, #12]
 8010792:	741a      	strb	r2, [r3, #16]
 8010794:	1b09      	subs	r1, r1, r4
 8010796:	60d9      	str	r1, [r3, #12]
 8010798:	bd10      	pop	{r4, pc}
 801079a:	bf00      	nop
 801079c:	20009534 	.word	0x20009534

080107a0 <Set_CSW>:
 80107a0:	b538      	push	{r3, r4, r5, lr}
 80107a2:	460d      	mov	r5, r1
 80107a4:	4c0c      	ldr	r4, [pc, #48]	; (80107d8 <Set_CSW+0x38>)
 80107a6:	4b0d      	ldr	r3, [pc, #52]	; (80107dc <Set_CSW+0x3c>)
 80107a8:	7420      	strb	r0, [r4, #16]
 80107aa:	2198      	movs	r1, #152	; 0x98
 80107ac:	1d20      	adds	r0, r4, #4
 80107ae:	220d      	movs	r2, #13
 80107b0:	6063      	str	r3, [r4, #4]
 80107b2:	f000 fdd9 	bl	8011368 <UserToPMABufferCopy>
 80107b6:	210d      	movs	r1, #13
 80107b8:	2001      	movs	r0, #1
 80107ba:	f000 fc4d 	bl	8011058 <SetEPTxCount>
 80107be:	b915      	cbnz	r5, 80107c6 <Set_CSW+0x26>
 80107c0:	2305      	movs	r3, #5
 80107c2:	7023      	strb	r3, [r4, #0]
 80107c4:	bd38      	pop	{r3, r4, r5, pc}
 80107c6:	2304      	movs	r3, #4
 80107c8:	2130      	movs	r1, #48	; 0x30
 80107ca:	7023      	strb	r3, [r4, #0]
 80107cc:	2001      	movs	r0, #1
 80107ce:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80107d2:	f000 bacd 	b.w	8010d70 <SetEPTxStatus>
 80107d6:	bf00      	nop
 80107d8:	20009534 	.word	0x20009534
 80107dc:	53425355 	.word	0x53425355

080107e0 <Mass_Storage_In>:
 80107e0:	b508      	push	{r3, lr}
 80107e2:	4b11      	ldr	r3, [pc, #68]	; (8010828 <Mass_Storage_In+0x48>)
 80107e4:	781a      	ldrb	r2, [r3, #0]
 80107e6:	3a02      	subs	r2, #2
 80107e8:	2a03      	cmp	r2, #3
 80107ea:	d81b      	bhi.n	8010824 <Mass_Storage_In+0x44>
 80107ec:	e8df f002 	tbb	[pc, r2]
 80107f0:	0202150b 	.word	0x0202150b
 80107f4:	2200      	movs	r2, #0
 80107f6:	701a      	strb	r2, [r3, #0]
 80107f8:	f44f 5140 	mov.w	r1, #12288	; 0x3000
 80107fc:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8010800:	2002      	movs	r0, #2
 8010802:	f000 bace 	b.w	8010da2 <SetEPRxStatus>
 8010806:	f893 2023 	ldrb.w	r2, [r3, #35]	; 0x23
 801080a:	2a28      	cmp	r2, #40	; 0x28
 801080c:	d10a      	bne.n	8010824 <Mass_Storage_In+0x44>
 801080e:	e9d3 100d 	ldrd	r1, r0, [r3, #52]	; 0x34
 8010812:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8010816:	f000 b9e7 	b.w	8010be8 <SCSI_Read10_Cmd>
 801081a:	2101      	movs	r1, #1
 801081c:	2000      	movs	r0, #0
 801081e:	f7ff ffbf 	bl	80107a0 <Set_CSW>
 8010822:	e7e9      	b.n	80107f8 <Mass_Storage_In+0x18>
 8010824:	bd08      	pop	{r3, pc}
 8010826:	bf00      	nop
 8010828:	20009534 	.word	0x20009534

0801082c <CBW_Decode>:
 801082c:	b538      	push	{r3, r4, r5, lr}
 801082e:	2300      	movs	r3, #0
 8010830:	4c57      	ldr	r4, [pc, #348]	; (8010990 <CBW_Decode+0x164>)
 8010832:	8fa2      	ldrh	r2, [r4, #60]	; 0x3c
 8010834:	f104 0114 	add.w	r1, r4, #20
 8010838:	f104 003e 	add.w	r0, r4, #62	; 0x3e
 801083c:	429a      	cmp	r2, r3
 801083e:	d813      	bhi.n	8010868 <CBW_Decode+0x3c>
 8010840:	69a3      	ldr	r3, [r4, #24]
 8010842:	2a1f      	cmp	r2, #31
 8010844:	60a3      	str	r3, [r4, #8]
 8010846:	69e3      	ldr	r3, [r4, #28]
 8010848:	60e3      	str	r3, [r4, #12]
 801084a:	d011      	beq.n	8010870 <CBW_Decode+0x44>
 801084c:	2500      	movs	r5, #0
 801084e:	f7ff ff81 	bl	8010754 <Bot_Abort.part.1>
 8010852:	211a      	movs	r1, #26
 8010854:	6165      	str	r5, [r4, #20]
 8010856:	2005      	movs	r0, #5
 8010858:	f000 f932 	bl	8010ac0 <Set_Scsi_Sense_Data>
 801085c:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010860:	2100      	movs	r1, #0
 8010862:	2001      	movs	r0, #1
 8010864:	f7ff bf9c 	b.w	80107a0 <Set_CSW>
 8010868:	5c1d      	ldrb	r5, [r3, r0]
 801086a:	545d      	strb	r5, [r3, r1]
 801086c:	3301      	adds	r3, #1
 801086e:	e7e5      	b.n	801083c <CBW_Decode+0x10>
 8010870:	f894 3023 	ldrb.w	r3, [r4, #35]	; 0x23
 8010874:	f003 02fd 	and.w	r2, r3, #253	; 0xfd
 8010878:	2a28      	cmp	r2, #40	; 0x28
 801087a:	d107      	bne.n	801088c <CBW_Decode+0x60>
 801087c:	f8d4 2025 	ldr.w	r2, [r4, #37]	; 0x25
 8010880:	ba12      	rev	r2, r2
 8010882:	63a2      	str	r2, [r4, #56]	; 0x38
 8010884:	8d62      	ldrh	r2, [r4, #42]	; 0x2a
 8010886:	ba52      	rev16	r2, r2
 8010888:	b292      	uxth	r2, r2
 801088a:	6362      	str	r2, [r4, #52]	; 0x34
 801088c:	6961      	ldr	r1, [r4, #20]
 801088e:	4a41      	ldr	r2, [pc, #260]	; (8010994 <CBW_Decode+0x168>)
 8010890:	4291      	cmp	r1, r2
 8010892:	d11b      	bne.n	80108cc <CBW_Decode+0xa0>
 8010894:	4a40      	ldr	r2, [pc, #256]	; (8010998 <CBW_Decode+0x16c>)
 8010896:	f894 1021 	ldrb.w	r1, [r4, #33]	; 0x21
 801089a:	6812      	ldr	r2, [r2, #0]
 801089c:	4291      	cmp	r1, r2
 801089e:	d804      	bhi.n	80108aa <CBW_Decode+0x7e>
 80108a0:	f894 2022 	ldrb.w	r2, [r4, #34]	; 0x22
 80108a4:	3a01      	subs	r2, #1
 80108a6:	2a0f      	cmp	r2, #15
 80108a8:	d903      	bls.n	80108b2 <CBW_Decode+0x86>
 80108aa:	f7ff ff53 	bl	8010754 <Bot_Abort.part.1>
 80108ae:	2124      	movs	r1, #36	; 0x24
 80108b0:	e7d1      	b.n	8010856 <CBW_Decode+0x2a>
 80108b2:	2b25      	cmp	r3, #37	; 0x25
 80108b4:	d05a      	beq.n	801096c <CBW_Decode+0x140>
 80108b6:	d828      	bhi.n	801090a <CBW_Decode+0xde>
 80108b8:	2b15      	cmp	r3, #21
 80108ba:	d045      	beq.n	8010948 <CBW_Decode+0x11c>
 80108bc:	d812      	bhi.n	80108e4 <CBW_Decode+0xb8>
 80108be:	2b08      	cmp	r3, #8
 80108c0:	d042      	beq.n	8010948 <CBW_Decode+0x11c>
 80108c2:	d807      	bhi.n	80108d4 <CBW_Decode+0xa8>
 80108c4:	2b00      	cmp	r3, #0
 80108c6:	d055      	beq.n	8010974 <CBW_Decode+0x148>
 80108c8:	2b03      	cmp	r3, #3
 80108ca:	d047      	beq.n	801095c <CBW_Decode+0x130>
 80108cc:	f7ff ff42 	bl	8010754 <Bot_Abort.part.1>
 80108d0:	2120      	movs	r1, #32
 80108d2:	e7c0      	b.n	8010856 <CBW_Decode+0x2a>
 80108d4:	2b0a      	cmp	r3, #10
 80108d6:	d037      	beq.n	8010948 <CBW_Decode+0x11c>
 80108d8:	2b12      	cmp	r3, #18
 80108da:	d1f7      	bne.n	80108cc <CBW_Decode+0xa0>
 80108dc:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80108e0:	f000 b89c 	b.w	8010a1c <SCSI_Inquiry_Cmd>
 80108e4:	2b1d      	cmp	r3, #29
 80108e6:	d02f      	beq.n	8010948 <CBW_Decode+0x11c>
 80108e8:	d807      	bhi.n	80108fa <CBW_Decode+0xce>
 80108ea:	2b1a      	cmp	r3, #26
 80108ec:	d03a      	beq.n	8010964 <CBW_Decode+0x138>
 80108ee:	2b1b      	cmp	r3, #27
 80108f0:	d1ec      	bne.n	80108cc <CBW_Decode+0xa0>
 80108f2:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80108f6:	f000 b8eb 	b.w	8010ad0 <SCSI_Start_Stop_Unit_Cmd>
 80108fa:	2b1e      	cmp	r3, #30
 80108fc:	d0f9      	beq.n	80108f2 <CBW_Decode+0xc6>
 80108fe:	2b23      	cmp	r3, #35	; 0x23
 8010900:	d1e4      	bne.n	80108cc <CBW_Decode+0xa0>
 8010902:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010906:	f000 b89f 	b.w	8010a48 <SCSI_ReadFormatCapacity_Cmd>
 801090a:	2b88      	cmp	r3, #136	; 0x88
 801090c:	d01c      	beq.n	8010948 <CBW_Decode+0x11c>
 801090e:	d814      	bhi.n	801093a <CBW_Decode+0x10e>
 8010910:	2b2f      	cmp	r3, #47	; 0x2f
 8010912:	d039      	beq.n	8010988 <CBW_Decode+0x15c>
 8010914:	d809      	bhi.n	801092a <CBW_Decode+0xfe>
 8010916:	2b28      	cmp	r3, #40	; 0x28
 8010918:	d030      	beq.n	801097c <CBW_Decode+0x150>
 801091a:	2b2a      	cmp	r3, #42	; 0x2a
 801091c:	d1d6      	bne.n	80108cc <CBW_Decode+0xa0>
 801091e:	e9d4 100d 	ldrd	r1, r0, [r4, #52]	; 0x34
 8010922:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010926:	f000 b991 	b.w	8010c4c <SCSI_Write10_Cmd>
 801092a:	2b55      	cmp	r3, #85	; 0x55
 801092c:	d00c      	beq.n	8010948 <CBW_Decode+0x11c>
 801092e:	2b5a      	cmp	r3, #90	; 0x5a
 8010930:	d1cc      	bne.n	80108cc <CBW_Decode+0xa0>
 8010932:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010936:	f000 b8b1 	b.w	8010a9c <SCSI_ModeSense10_Cmd>
 801093a:	2b9e      	cmp	r3, #158	; 0x9e
 801093c:	d004      	beq.n	8010948 <CBW_Decode+0x11c>
 801093e:	d807      	bhi.n	8010950 <CBW_Decode+0x124>
 8010940:	2b8a      	cmp	r3, #138	; 0x8a
 8010942:	d001      	beq.n	8010948 <CBW_Decode+0x11c>
 8010944:	2b8f      	cmp	r3, #143	; 0x8f
 8010946:	d1c1      	bne.n	80108cc <CBW_Decode+0xa0>
 8010948:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 801094c:	f000 b8fe 	b.w	8010b4c <SCSI_Invalid_Cmd>
 8010950:	2baa      	cmp	r3, #170	; 0xaa
 8010952:	d0f9      	beq.n	8010948 <CBW_Decode+0x11c>
 8010954:	2baf      	cmp	r3, #175	; 0xaf
 8010956:	d0f7      	beq.n	8010948 <CBW_Decode+0x11c>
 8010958:	2ba8      	cmp	r3, #168	; 0xa8
 801095a:	e7f4      	b.n	8010946 <CBW_Decode+0x11a>
 801095c:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010960:	f000 b8a2 	b.w	8010aa8 <SCSI_RequestSense_Cmd>
 8010964:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010968:	f000 b892 	b.w	8010a90 <SCSI_ModeSense6_Cmd>
 801096c:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010970:	f000 b87a 	b.w	8010a68 <SCSI_ReadCapacity10_Cmd>
 8010974:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010978:	f000 b8e6 	b.w	8010b48 <SCSI_TestUnitReady_Cmd>
 801097c:	e9d4 100d 	ldrd	r1, r0, [r4, #52]	; 0x34
 8010980:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010984:	f000 b930 	b.w	8010be8 <SCSI_Read10_Cmd>
 8010988:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 801098c:	f000 b8a4 	b.w	8010ad8 <SCSI_Verify10_Cmd>
 8010990:	20009534 	.word	0x20009534
 8010994:	43425355 	.word	0x43425355
 8010998:	200095c8 	.word	0x200095c8

0801099c <Bot_Abort>:
 801099c:	2801      	cmp	r0, #1
 801099e:	d007      	beq.n	80109b0 <Bot_Abort+0x14>
 80109a0:	d302      	bcc.n	80109a8 <Bot_Abort+0xc>
 80109a2:	2802      	cmp	r0, #2
 80109a4:	d009      	beq.n	80109ba <Bot_Abort+0x1e>
 80109a6:	4770      	bx	lr
 80109a8:	2110      	movs	r1, #16
 80109aa:	2001      	movs	r0, #1
 80109ac:	f000 b9e0 	b.w	8010d70 <SetEPTxStatus>
 80109b0:	f44f 5180 	mov.w	r1, #4096	; 0x1000
 80109b4:	2002      	movs	r0, #2
 80109b6:	f000 b9f4 	b.w	8010da2 <SetEPRxStatus>
 80109ba:	e6cb      	b.n	8010754 <Bot_Abort.part.1>

080109bc <Mass_Storage_Out>:
 80109bc:	b538      	push	{r3, r4, r5, lr}
 80109be:	4c16      	ldr	r4, [pc, #88]	; (8010a18 <Mass_Storage_Out+0x5c>)
 80109c0:	2002      	movs	r0, #2
 80109c2:	f894 5023 	ldrb.w	r5, [r4, #35]	; 0x23
 80109c6:	f000 fb99 	bl	80110fc <GetEPRxCount>
 80109ca:	21d8      	movs	r1, #216	; 0xd8
 80109cc:	4602      	mov	r2, r0
 80109ce:	87a0      	strh	r0, [r4, #60]	; 0x3c
 80109d0:	f104 003e 	add.w	r0, r4, #62	; 0x3e
 80109d4:	f000 fcd7 	bl	8011386 <PMAToUserBufferCopy>
 80109d8:	7820      	ldrb	r0, [r4, #0]
 80109da:	b120      	cbz	r0, 80109e6 <Mass_Storage_Out+0x2a>
 80109dc:	2801      	cmp	r0, #1
 80109de:	d006      	beq.n	80109ee <Mass_Storage_Out+0x32>
 80109e0:	f7ff feb8 	bl	8010754 <Bot_Abort.part.1>
 80109e4:	e00d      	b.n	8010a02 <Mass_Storage_Out+0x46>
 80109e6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80109ea:	f7ff bf1f 	b.w	801082c <CBW_Decode>
 80109ee:	2d2a      	cmp	r5, #42	; 0x2a
 80109f0:	d105      	bne.n	80109fe <Mass_Storage_Out+0x42>
 80109f2:	e9d4 100d 	ldrd	r1, r0, [r4, #52]	; 0x34
 80109f6:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80109fa:	f000 b927 	b.w	8010c4c <SCSI_Write10_Cmd>
 80109fe:	f7ff ffcd 	bl	801099c <Bot_Abort>
 8010a02:	2124      	movs	r1, #36	; 0x24
 8010a04:	2005      	movs	r0, #5
 8010a06:	f000 f85b 	bl	8010ac0 <Set_Scsi_Sense_Data>
 8010a0a:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010a0e:	2100      	movs	r1, #0
 8010a10:	2002      	movs	r0, #2
 8010a12:	f7ff bec5 	b.w	80107a0 <Set_CSW>
 8010a16:	bf00      	nop
 8010a18:	20009534 	.word	0x20009534

08010a1c <SCSI_Inquiry_Cmd>:
 8010a1c:	4b07      	ldr	r3, [pc, #28]	; (8010a3c <SCSI_Inquiry_Cmd+0x20>)
 8010a1e:	7c1a      	ldrb	r2, [r3, #16]
 8010a20:	07d2      	lsls	r2, r2, #31
 8010a22:	d405      	bmi.n	8010a30 <SCSI_Inquiry_Cmd+0x14>
 8010a24:	7cd9      	ldrb	r1, [r3, #19]
 8010a26:	2924      	cmp	r1, #36	; 0x24
 8010a28:	d806      	bhi.n	8010a38 <SCSI_Inquiry_Cmd+0x1c>
 8010a2a:	b289      	uxth	r1, r1
 8010a2c:	4804      	ldr	r0, [pc, #16]	; (8010a40 <SCSI_Inquiry_Cmd+0x24>)
 8010a2e:	e001      	b.n	8010a34 <SCSI_Inquiry_Cmd+0x18>
 8010a30:	2105      	movs	r1, #5
 8010a32:	4804      	ldr	r0, [pc, #16]	; (8010a44 <SCSI_Inquiry_Cmd+0x28>)
 8010a34:	f7ff be9a 	b.w	801076c <Transfer_Data_Request>
 8010a38:	2124      	movs	r1, #36	; 0x24
 8010a3a:	e7f7      	b.n	8010a2c <SCSI_Inquiry_Cmd+0x10>
 8010a3c:	20009548 	.word	0x20009548
 8010a40:	080147ba 	.word	0x080147ba
 8010a44:	200095b2 	.word	0x200095b2

08010a48 <SCSI_ReadFormatCapacity_Cmd>:
 8010a48:	2300      	movs	r3, #0
 8010a4a:	2210      	movs	r2, #16
 8010a4c:	4805      	ldr	r0, [pc, #20]	; (8010a64 <SCSI_ReadFormatCapacity_Cmd+0x1c>)
 8010a4e:	210c      	movs	r1, #12
 8010a50:	7103      	strb	r3, [r0, #4]
 8010a52:	7143      	strb	r3, [r0, #5]
 8010a54:	7182      	strb	r2, [r0, #6]
 8010a56:	71c3      	strb	r3, [r0, #7]
 8010a58:	7243      	strb	r3, [r0, #9]
 8010a5a:	7282      	strb	r2, [r0, #10]
 8010a5c:	72c3      	strb	r3, [r0, #11]
 8010a5e:	f7ff be85 	b.w	801076c <Transfer_Data_Request>
 8010a62:	bf00      	nop
 8010a64:	20000130 	.word	0x20000130

08010a68 <SCSI_ReadCapacity10_Cmd>:
 8010a68:	2207      	movs	r2, #7
 8010a6a:	4808      	ldr	r0, [pc, #32]	; (8010a8c <SCSI_ReadCapacity10_Cmd+0x24>)
 8010a6c:	2300      	movs	r3, #0
 8010a6e:	71c2      	strb	r2, [r0, #7]
 8010a70:	22ff      	movs	r2, #255	; 0xff
 8010a72:	7202      	strb	r2, [r0, #8]
 8010a74:	2210      	movs	r2, #16
 8010a76:	7143      	strb	r3, [r0, #5]
 8010a78:	7183      	strb	r3, [r0, #6]
 8010a7a:	7243      	strb	r3, [r0, #9]
 8010a7c:	7283      	strb	r3, [r0, #10]
 8010a7e:	72c2      	strb	r2, [r0, #11]
 8010a80:	7303      	strb	r3, [r0, #12]
 8010a82:	2108      	movs	r1, #8
 8010a84:	3005      	adds	r0, #5
 8010a86:	f7ff be71 	b.w	801076c <Transfer_Data_Request>
 8010a8a:	bf00      	nop
 8010a8c:	200095b2 	.word	0x200095b2

08010a90 <SCSI_ModeSense6_Cmd>:
 8010a90:	2104      	movs	r1, #4
 8010a92:	4801      	ldr	r0, [pc, #4]	; (8010a98 <SCSI_ModeSense6_Cmd+0x8>)
 8010a94:	f7ff be6a 	b.w	801076c <Transfer_Data_Request>
 8010a98:	2000013c 	.word	0x2000013c

08010a9c <SCSI_ModeSense10_Cmd>:
 8010a9c:	2108      	movs	r1, #8
 8010a9e:	4801      	ldr	r0, [pc, #4]	; (8010aa4 <SCSI_ModeSense10_Cmd+0x8>)
 8010aa0:	f7ff be64 	b.w	801076c <Transfer_Data_Request>
 8010aa4:	20000140 	.word	0x20000140

08010aa8 <SCSI_RequestSense_Cmd>:
 8010aa8:	4b03      	ldr	r3, [pc, #12]	; (8010ab8 <SCSI_RequestSense_Cmd+0x10>)
 8010aaa:	4804      	ldr	r0, [pc, #16]	; (8010abc <SCSI_RequestSense_Cmd+0x14>)
 8010aac:	7cd9      	ldrb	r1, [r3, #19]
 8010aae:	2912      	cmp	r1, #18
 8010ab0:	bf28      	it	cs
 8010ab2:	2112      	movcs	r1, #18
 8010ab4:	f7ff be5a 	b.w	801076c <Transfer_Data_Request>
 8010ab8:	20009548 	.word	0x20009548
 8010abc:	20000148 	.word	0x20000148

08010ac0 <Set_Scsi_Sense_Data>:
 8010ac0:	4b02      	ldr	r3, [pc, #8]	; (8010acc <Set_Scsi_Sense_Data+0xc>)
 8010ac2:	7698      	strb	r0, [r3, #26]
 8010ac4:	f883 1024 	strb.w	r1, [r3, #36]	; 0x24
 8010ac8:	4770      	bx	lr
 8010aca:	bf00      	nop
 8010acc:	20000130 	.word	0x20000130

08010ad0 <SCSI_Start_Stop_Unit_Cmd>:
 8010ad0:	2101      	movs	r1, #1
 8010ad2:	2000      	movs	r0, #0
 8010ad4:	f7ff be64 	b.w	80107a0 <Set_CSW>

08010ad8 <SCSI_Verify10_Cmd>:
 8010ad8:	b508      	push	{r3, lr}
 8010ada:	4b0c      	ldr	r3, [pc, #48]	; (8010b0c <SCSI_Verify10_Cmd+0x34>)
 8010adc:	689a      	ldr	r2, [r3, #8]
 8010ade:	b942      	cbnz	r2, 8010af2 <SCSI_Verify10_Cmd+0x1a>
 8010ae0:	7c18      	ldrb	r0, [r3, #16]
 8010ae2:	f010 0004 	ands.w	r0, r0, #4
 8010ae6:	d104      	bne.n	8010af2 <SCSI_Verify10_Cmd+0x1a>
 8010ae8:	2101      	movs	r1, #1
 8010aea:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8010aee:	f7ff be57 	b.w	80107a0 <Set_CSW>
 8010af2:	2002      	movs	r0, #2
 8010af4:	f7ff ff52 	bl	801099c <Bot_Abort>
 8010af8:	2205      	movs	r2, #5
 8010afa:	4b05      	ldr	r3, [pc, #20]	; (8010b10 <SCSI_Verify10_Cmd+0x38>)
 8010afc:	2100      	movs	r1, #0
 8010afe:	769a      	strb	r2, [r3, #26]
 8010b00:	2224      	movs	r2, #36	; 0x24
 8010b02:	2001      	movs	r0, #1
 8010b04:	f883 2024 	strb.w	r2, [r3, #36]	; 0x24
 8010b08:	e7ef      	b.n	8010aea <SCSI_Verify10_Cmd+0x12>
 8010b0a:	bf00      	nop
 8010b0c:	20009548 	.word	0x20009548
 8010b10:	20000130 	.word	0x20000130

08010b14 <SCSI_Valid_Cmd>:
 8010b14:	b508      	push	{r3, lr}
 8010b16:	4b0a      	ldr	r3, [pc, #40]	; (8010b40 <SCSI_Valid_Cmd+0x2c>)
 8010b18:	6898      	ldr	r0, [r3, #8]
 8010b1a:	b170      	cbz	r0, 8010b3a <SCSI_Valid_Cmd+0x26>
 8010b1c:	2002      	movs	r0, #2
 8010b1e:	f7ff ff3d 	bl	801099c <Bot_Abort>
 8010b22:	2205      	movs	r2, #5
 8010b24:	4b07      	ldr	r3, [pc, #28]	; (8010b44 <SCSI_Valid_Cmd+0x30>)
 8010b26:	2100      	movs	r1, #0
 8010b28:	769a      	strb	r2, [r3, #26]
 8010b2a:	2220      	movs	r2, #32
 8010b2c:	2001      	movs	r0, #1
 8010b2e:	f883 2024 	strb.w	r2, [r3, #36]	; 0x24
 8010b32:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8010b36:	f7ff be33 	b.w	80107a0 <Set_CSW>
 8010b3a:	2101      	movs	r1, #1
 8010b3c:	e7f9      	b.n	8010b32 <SCSI_Valid_Cmd+0x1e>
 8010b3e:	bf00      	nop
 8010b40:	20009548 	.word	0x20009548
 8010b44:	20000130 	.word	0x20000130

08010b48 <SCSI_TestUnitReady_Cmd>:
 8010b48:	f7ff bfc2 	b.w	8010ad0 <SCSI_Start_Stop_Unit_Cmd>

08010b4c <SCSI_Invalid_Cmd>:
 8010b4c:	b508      	push	{r3, lr}
 8010b4e:	4b0c      	ldr	r3, [pc, #48]	; (8010b80 <SCSI_Invalid_Cmd+0x34>)
 8010b50:	689a      	ldr	r2, [r3, #8]
 8010b52:	b972      	cbnz	r2, 8010b72 <SCSI_Invalid_Cmd+0x26>
 8010b54:	2000      	movs	r0, #0
 8010b56:	f7ff ff21 	bl	801099c <Bot_Abort>
 8010b5a:	2205      	movs	r2, #5
 8010b5c:	4b09      	ldr	r3, [pc, #36]	; (8010b84 <SCSI_Invalid_Cmd+0x38>)
 8010b5e:	2100      	movs	r1, #0
 8010b60:	769a      	strb	r2, [r3, #26]
 8010b62:	2220      	movs	r2, #32
 8010b64:	f883 2024 	strb.w	r2, [r3, #36]	; 0x24
 8010b68:	2001      	movs	r0, #1
 8010b6a:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8010b6e:	f7ff be17 	b.w	80107a0 <Set_CSW>
 8010b72:	f993 300c 	ldrsb.w	r3, [r3, #12]
 8010b76:	2b00      	cmp	r3, #0
 8010b78:	dbec      	blt.n	8010b54 <SCSI_Invalid_Cmd+0x8>
 8010b7a:	2002      	movs	r0, #2
 8010b7c:	e7eb      	b.n	8010b56 <SCSI_Invalid_Cmd+0xa>
 8010b7e:	bf00      	nop
 8010b80:	20009548 	.word	0x20009548
 8010b84:	20000130 	.word	0x20000130

08010b88 <SCSI_Address_Management>:
 8010b88:	4411      	add	r1, r2
 8010b8a:	f5b1 6f00 	cmp.w	r1, #2048	; 0x800
 8010b8e:	b508      	push	{r3, lr}
 8010b90:	d913      	bls.n	8010bba <SCSI_Address_Management+0x32>
 8010b92:	282a      	cmp	r0, #42	; 0x2a
 8010b94:	d102      	bne.n	8010b9c <SCSI_Address_Management+0x14>
 8010b96:	2002      	movs	r0, #2
 8010b98:	f7ff ff00 	bl	801099c <Bot_Abort>
 8010b9c:	2000      	movs	r0, #0
 8010b9e:	f7ff fefd 	bl	801099c <Bot_Abort>
 8010ba2:	2205      	movs	r2, #5
 8010ba4:	4b0e      	ldr	r3, [pc, #56]	; (8010be0 <SCSI_Address_Management+0x58>)
 8010ba6:	769a      	strb	r2, [r3, #26]
 8010ba8:	2221      	movs	r2, #33	; 0x21
 8010baa:	2001      	movs	r0, #1
 8010bac:	2100      	movs	r1, #0
 8010bae:	f883 2024 	strb.w	r2, [r3, #36]	; 0x24
 8010bb2:	f7ff fdf5 	bl	80107a0 <Set_CSW>
 8010bb6:	2000      	movs	r0, #0
 8010bb8:	bd08      	pop	{r3, pc}
 8010bba:	4b0a      	ldr	r3, [pc, #40]	; (8010be4 <SCSI_Address_Management+0x5c>)
 8010bbc:	689b      	ldr	r3, [r3, #8]
 8010bbe:	ebb3 3f02 	cmp.w	r3, r2, lsl #12
 8010bc2:	d00a      	beq.n	8010bda <SCSI_Address_Management+0x52>
 8010bc4:	282a      	cmp	r0, #42	; 0x2a
 8010bc6:	bf0c      	ite	eq
 8010bc8:	2002      	moveq	r0, #2
 8010bca:	2000      	movne	r0, #0
 8010bcc:	f7ff fee6 	bl	801099c <Bot_Abort>
 8010bd0:	2205      	movs	r2, #5
 8010bd2:	4b03      	ldr	r3, [pc, #12]	; (8010be0 <SCSI_Address_Management+0x58>)
 8010bd4:	769a      	strb	r2, [r3, #26]
 8010bd6:	2224      	movs	r2, #36	; 0x24
 8010bd8:	e7e7      	b.n	8010baa <SCSI_Address_Management+0x22>
 8010bda:	2001      	movs	r0, #1
 8010bdc:	e7ec      	b.n	8010bb8 <SCSI_Address_Management+0x30>
 8010bde:	bf00      	nop
 8010be0:	20000130 	.word	0x20000130
 8010be4:	20009548 	.word	0x20009548

08010be8 <SCSI_Read10_Cmd>:
 8010be8:	b570      	push	{r4, r5, r6, lr}
 8010bea:	4e15      	ldr	r6, [pc, #84]	; (8010c40 <SCSI_Read10_Cmd+0x58>)
 8010bec:	4604      	mov	r4, r0
 8010bee:	7833      	ldrb	r3, [r6, #0]
 8010bf0:	460d      	mov	r5, r1
 8010bf2:	bb0b      	cbnz	r3, 8010c38 <SCSI_Read10_Cmd+0x50>
 8010bf4:	460a      	mov	r2, r1
 8010bf6:	4601      	mov	r1, r0
 8010bf8:	2028      	movs	r0, #40	; 0x28
 8010bfa:	f7ff ffc5 	bl	8010b88 <SCSI_Address_Management>
 8010bfe:	b1e8      	cbz	r0, 8010c3c <SCSI_Read10_Cmd+0x54>
 8010c00:	4b10      	ldr	r3, [pc, #64]	; (8010c44 <SCSI_Read10_Cmd+0x5c>)
 8010c02:	f993 300c 	ldrsb.w	r3, [r3, #12]
 8010c06:	2b00      	cmp	r3, #0
 8010c08:	da07      	bge.n	8010c1a <SCSI_Read10_Cmd+0x32>
 8010c0a:	2302      	movs	r3, #2
 8010c0c:	4629      	mov	r1, r5
 8010c0e:	4620      	mov	r0, r4
 8010c10:	7033      	strb	r3, [r6, #0]
 8010c12:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 8010c16:	f7fe ba3d 	b.w	800f094 <Read_Memory>
 8010c1a:	2002      	movs	r0, #2
 8010c1c:	f7ff febe 	bl	801099c <Bot_Abort>
 8010c20:	2205      	movs	r2, #5
 8010c22:	4b09      	ldr	r3, [pc, #36]	; (8010c48 <SCSI_Read10_Cmd+0x60>)
 8010c24:	2101      	movs	r1, #1
 8010c26:	769a      	strb	r2, [r3, #26]
 8010c28:	2224      	movs	r2, #36	; 0x24
 8010c2a:	f883 2024 	strb.w	r2, [r3, #36]	; 0x24
 8010c2e:	4608      	mov	r0, r1
 8010c30:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 8010c34:	f7ff bdb4 	b.w	80107a0 <Set_CSW>
 8010c38:	2b02      	cmp	r3, #2
 8010c3a:	d0ea      	beq.n	8010c12 <SCSI_Read10_Cmd+0x2a>
 8010c3c:	bd70      	pop	{r4, r5, r6, pc}
 8010c3e:	bf00      	nop
 8010c40:	20009534 	.word	0x20009534
 8010c44:	20009548 	.word	0x20009548
 8010c48:	20000130 	.word	0x20000130

08010c4c <SCSI_Write10_Cmd>:
 8010c4c:	b538      	push	{r3, r4, r5, lr}
 8010c4e:	4d16      	ldr	r5, [pc, #88]	; (8010ca8 <SCSI_Write10_Cmd+0x5c>)
 8010c50:	460a      	mov	r2, r1
 8010c52:	782c      	ldrb	r4, [r5, #0]
 8010c54:	bb0c      	cbnz	r4, 8010c9a <SCSI_Write10_Cmd+0x4e>
 8010c56:	4601      	mov	r1, r0
 8010c58:	202a      	movs	r0, #42	; 0x2a
 8010c5a:	f7ff ff95 	bl	8010b88 <SCSI_Address_Management>
 8010c5e:	b310      	cbz	r0, 8010ca6 <SCSI_Write10_Cmd+0x5a>
 8010c60:	4b12      	ldr	r3, [pc, #72]	; (8010cac <SCSI_Write10_Cmd+0x60>)
 8010c62:	f993 300c 	ldrsb.w	r3, [r3, #12]
 8010c66:	2b00      	cmp	r3, #0
 8010c68:	db08      	blt.n	8010c7c <SCSI_Write10_Cmd+0x30>
 8010c6a:	2301      	movs	r3, #1
 8010c6c:	f44f 5140 	mov.w	r1, #12288	; 0x3000
 8010c70:	702b      	strb	r3, [r5, #0]
 8010c72:	2002      	movs	r0, #2
 8010c74:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010c78:	f000 b893 	b.w	8010da2 <SetEPRxStatus>
 8010c7c:	4620      	mov	r0, r4
 8010c7e:	f7ff fe8d 	bl	801099c <Bot_Abort>
 8010c82:	2205      	movs	r2, #5
 8010c84:	4b0a      	ldr	r3, [pc, #40]	; (8010cb0 <SCSI_Write10_Cmd+0x64>)
 8010c86:	4621      	mov	r1, r4
 8010c88:	769a      	strb	r2, [r3, #26]
 8010c8a:	2224      	movs	r2, #36	; 0x24
 8010c8c:	f883 2024 	strb.w	r2, [r3, #36]	; 0x24
 8010c90:	2001      	movs	r0, #1
 8010c92:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010c96:	f7ff bd83 	b.w	80107a0 <Set_CSW>
 8010c9a:	2c01      	cmp	r4, #1
 8010c9c:	d103      	bne.n	8010ca6 <SCSI_Write10_Cmd+0x5a>
 8010c9e:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8010ca2:	f7fe ba49 	b.w	800f138 <Write_Memory>
 8010ca6:	bd38      	pop	{r3, r4, r5, pc}
 8010ca8:	20009534 	.word	0x20009534
 8010cac:	20009548 	.word	0x20009548
 8010cb0:	20000130 	.word	0x20000130

08010cb4 <SetCNTR>:
 8010cb4:	4b01      	ldr	r3, [pc, #4]	; (8010cbc <SetCNTR+0x8>)
 8010cb6:	6018      	str	r0, [r3, #0]
 8010cb8:	4770      	bx	lr
 8010cba:	bf00      	nop
 8010cbc:	40005c40 	.word	0x40005c40

08010cc0 <GetCNTR>:
 8010cc0:	4b01      	ldr	r3, [pc, #4]	; (8010cc8 <GetCNTR+0x8>)
 8010cc2:	6818      	ldr	r0, [r3, #0]
 8010cc4:	b280      	uxth	r0, r0
 8010cc6:	4770      	bx	lr
 8010cc8:	40005c40 	.word	0x40005c40

08010ccc <SetISTR>:
 8010ccc:	4b01      	ldr	r3, [pc, #4]	; (8010cd4 <SetISTR+0x8>)
 8010cce:	6018      	str	r0, [r3, #0]
 8010cd0:	4770      	bx	lr
 8010cd2:	bf00      	nop
 8010cd4:	40005c44 	.word	0x40005c44

08010cd8 <GetISTR>:
 8010cd8:	4b01      	ldr	r3, [pc, #4]	; (8010ce0 <GetISTR+0x8>)
 8010cda:	6818      	ldr	r0, [r3, #0]
 8010cdc:	b280      	uxth	r0, r0
 8010cde:	4770      	bx	lr
 8010ce0:	40005c44 	.word	0x40005c44

08010ce4 <GetFNR>:
 8010ce4:	4b01      	ldr	r3, [pc, #4]	; (8010cec <GetFNR+0x8>)
 8010ce6:	6818      	ldr	r0, [r3, #0]
 8010ce8:	b280      	uxth	r0, r0
 8010cea:	4770      	bx	lr
 8010cec:	40005c48 	.word	0x40005c48

08010cf0 <SetDADDR>:
 8010cf0:	4b01      	ldr	r3, [pc, #4]	; (8010cf8 <SetDADDR+0x8>)
 8010cf2:	6018      	str	r0, [r3, #0]
 8010cf4:	4770      	bx	lr
 8010cf6:	bf00      	nop
 8010cf8:	40005c4c 	.word	0x40005c4c

08010cfc <GetDADDR>:
 8010cfc:	4b01      	ldr	r3, [pc, #4]	; (8010d04 <GetDADDR+0x8>)
 8010cfe:	6818      	ldr	r0, [r3, #0]
 8010d00:	b280      	uxth	r0, r0
 8010d02:	4770      	bx	lr
 8010d04:	40005c4c 	.word	0x40005c4c

08010d08 <SetBTABLE>:
 8010d08:	f64f 73f8 	movw	r3, #65528	; 0xfff8
 8010d0c:	4a01      	ldr	r2, [pc, #4]	; (8010d14 <SetBTABLE+0xc>)
 8010d0e:	4003      	ands	r3, r0
 8010d10:	6013      	str	r3, [r2, #0]
 8010d12:	4770      	bx	lr
 8010d14:	40005c50 	.word	0x40005c50

08010d18 <GetBTABLE>:
 8010d18:	4b01      	ldr	r3, [pc, #4]	; (8010d20 <GetBTABLE+0x8>)
 8010d1a:	6818      	ldr	r0, [r3, #0]
 8010d1c:	b280      	uxth	r0, r0
 8010d1e:	4770      	bx	lr
 8010d20:	40005c50 	.word	0x40005c50

08010d24 <SetENDPOINT>:
 8010d24:	0080      	lsls	r0, r0, #2
 8010d26:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010d2a:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010d2e:	6001      	str	r1, [r0, #0]
 8010d30:	4770      	bx	lr

08010d32 <GetENDPOINT>:
 8010d32:	0080      	lsls	r0, r0, #2
 8010d34:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010d38:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010d3c:	6800      	ldr	r0, [r0, #0]
 8010d3e:	b280      	uxth	r0, r0
 8010d40:	4770      	bx	lr

08010d42 <SetEPType>:
 8010d42:	0080      	lsls	r0, r0, #2
 8010d44:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010d48:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010d4c:	6803      	ldr	r3, [r0, #0]
 8010d4e:	f423 43ec 	bic.w	r3, r3, #30208	; 0x7600
 8010d52:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8010d56:	4319      	orrs	r1, r3
 8010d58:	b289      	uxth	r1, r1
 8010d5a:	6001      	str	r1, [r0, #0]
 8010d5c:	4770      	bx	lr

08010d5e <GetEPType>:
 8010d5e:	0080      	lsls	r0, r0, #2
 8010d60:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010d64:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010d68:	6800      	ldr	r0, [r0, #0]
 8010d6a:	f400 60c0 	and.w	r0, r0, #1536	; 0x600
 8010d6e:	4770      	bx	lr

08010d70 <SetEPTxStatus>:
 8010d70:	0080      	lsls	r0, r0, #2
 8010d72:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010d76:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010d7a:	6803      	ldr	r3, [r0, #0]
 8010d7c:	06ca      	lsls	r2, r1, #27
 8010d7e:	f423 43e0 	bic.w	r3, r3, #28672	; 0x7000
 8010d82:	f023 0340 	bic.w	r3, r3, #64	; 0x40
 8010d86:	b29b      	uxth	r3, r3
 8010d88:	bf48      	it	mi
 8010d8a:	f083 0310 	eormi.w	r3, r3, #16
 8010d8e:	068a      	lsls	r2, r1, #26
 8010d90:	bf48      	it	mi
 8010d92:	f083 0320 	eormi.w	r3, r3, #32
 8010d96:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8010d9a:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010d9e:	6003      	str	r3, [r0, #0]
 8010da0:	4770      	bx	lr

08010da2 <SetEPRxStatus>:
 8010da2:	0080      	lsls	r0, r0, #2
 8010da4:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010da8:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010dac:	6803      	ldr	r3, [r0, #0]
 8010dae:	04ca      	lsls	r2, r1, #19
 8010db0:	f423 4380 	bic.w	r3, r3, #16384	; 0x4000
 8010db4:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8010db8:	b29b      	uxth	r3, r3
 8010dba:	bf48      	it	mi
 8010dbc:	f483 5380 	eormi.w	r3, r3, #4096	; 0x1000
 8010dc0:	048a      	lsls	r2, r1, #18
 8010dc2:	bf48      	it	mi
 8010dc4:	f483 5300 	eormi.w	r3, r3, #8192	; 0x2000
 8010dc8:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8010dcc:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010dd0:	6003      	str	r3, [r0, #0]
 8010dd2:	4770      	bx	lr

08010dd4 <SetDouBleBuffEPStall>:
 8010dd4:	0080      	lsls	r0, r0, #2
 8010dd6:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010dda:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010dde:	2901      	cmp	r1, #1
 8010de0:	6803      	ldr	r3, [r0, #0]
 8010de2:	d105      	bne.n	8010df0 <SetDouBleBuffEPStall+0x1c>
 8010de4:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
 8010de8:	041b      	lsls	r3, r3, #16
 8010dea:	0c1b      	lsrs	r3, r3, #16
 8010dec:	6003      	str	r3, [r0, #0]
 8010dee:	4770      	bx	lr
 8010df0:	2902      	cmp	r1, #2
 8010df2:	d1fc      	bne.n	8010dee <SetDouBleBuffEPStall+0x1a>
 8010df4:	f023 0310 	bic.w	r3, r3, #16
 8010df8:	e7f6      	b.n	8010de8 <SetDouBleBuffEPStall+0x14>

08010dfa <GetEPTxStatus>:
 8010dfa:	0080      	lsls	r0, r0, #2
 8010dfc:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010e00:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010e04:	6800      	ldr	r0, [r0, #0]
 8010e06:	f000 0030 	and.w	r0, r0, #48	; 0x30
 8010e0a:	4770      	bx	lr

08010e0c <GetEPRxStatus>:
 8010e0c:	0080      	lsls	r0, r0, #2
 8010e0e:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010e12:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010e16:	6800      	ldr	r0, [r0, #0]
 8010e18:	f400 5040 	and.w	r0, r0, #12288	; 0x3000
 8010e1c:	4770      	bx	lr

08010e1e <SetEPTxValid>:
 8010e1e:	0080      	lsls	r0, r0, #2
 8010e20:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010e24:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010e28:	6803      	ldr	r3, [r0, #0]
 8010e2a:	f423 43e0 	bic.w	r3, r3, #28672	; 0x7000
 8010e2e:	f023 0340 	bic.w	r3, r3, #64	; 0x40
 8010e32:	b29b      	uxth	r3, r3
 8010e34:	f083 0330 	eor.w	r3, r3, #48	; 0x30
 8010e38:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8010e3c:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010e40:	6003      	str	r3, [r0, #0]
 8010e42:	4770      	bx	lr

08010e44 <SetEPRxValid>:
 8010e44:	0080      	lsls	r0, r0, #2
 8010e46:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010e4a:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010e4e:	6803      	ldr	r3, [r0, #0]
 8010e50:	f423 4380 	bic.w	r3, r3, #16384	; 0x4000
 8010e54:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8010e58:	b29b      	uxth	r3, r3
 8010e5a:	f483 5340 	eor.w	r3, r3, #12288	; 0x3000
 8010e5e:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8010e62:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010e66:	6003      	str	r3, [r0, #0]
 8010e68:	4770      	bx	lr

08010e6a <SetEP_KIND>:
 8010e6a:	f640 630f 	movw	r3, #3599	; 0xe0f
 8010e6e:	0080      	lsls	r0, r0, #2
 8010e70:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010e74:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010e78:	6802      	ldr	r2, [r0, #0]
 8010e7a:	4013      	ands	r3, r2
 8010e7c:	f443 4301 	orr.w	r3, r3, #33024	; 0x8100
 8010e80:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010e84:	6003      	str	r3, [r0, #0]
 8010e86:	4770      	bx	lr

08010e88 <ClearEP_KIND>:
 8010e88:	0080      	lsls	r0, r0, #2
 8010e8a:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010e8e:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010e92:	6803      	ldr	r3, [r0, #0]
 8010e94:	f423 73f8 	bic.w	r3, r3, #496	; 0x1f0
 8010e98:	051b      	lsls	r3, r3, #20
 8010e9a:	0d1b      	lsrs	r3, r3, #20
 8010e9c:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8010ea0:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010ea4:	6003      	str	r3, [r0, #0]
 8010ea6:	4770      	bx	lr

08010ea8 <Clear_Status_Out>:
 8010ea8:	f7ff bfee 	b.w	8010e88 <ClearEP_KIND>

08010eac <Set_Status_Out>:
 8010eac:	f7ff bfdd 	b.w	8010e6a <SetEP_KIND>

08010eb0 <SetEPDoubleBuff>:
 8010eb0:	f7ff bfdb 	b.w	8010e6a <SetEP_KIND>

08010eb4 <ClearEPDoubleBuff>:
 8010eb4:	f7ff bfe8 	b.w	8010e88 <ClearEP_KIND>

08010eb8 <GetTxStallStatus>:
 8010eb8:	0080      	lsls	r0, r0, #2
 8010eba:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010ebe:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010ec2:	6800      	ldr	r0, [r0, #0]
 8010ec4:	f000 0030 	and.w	r0, r0, #48	; 0x30
 8010ec8:	f1a0 0310 	sub.w	r3, r0, #16
 8010ecc:	4258      	negs	r0, r3
 8010ece:	4158      	adcs	r0, r3
 8010ed0:	4770      	bx	lr

08010ed2 <GetRxStallStatus>:
 8010ed2:	0080      	lsls	r0, r0, #2
 8010ed4:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010ed8:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010edc:	6800      	ldr	r0, [r0, #0]
 8010ede:	f400 5040 	and.w	r0, r0, #12288	; 0x3000
 8010ee2:	f5a0 5380 	sub.w	r3, r0, #4096	; 0x1000
 8010ee6:	4258      	negs	r0, r3
 8010ee8:	4158      	adcs	r0, r3
 8010eea:	4770      	bx	lr

08010eec <ClearEP_CTR_RX>:
 8010eec:	0080      	lsls	r0, r0, #2
 8010eee:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010ef2:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010ef6:	6803      	ldr	r3, [r0, #0]
 8010ef8:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8010efc:	051b      	lsls	r3, r3, #20
 8010efe:	0d1b      	lsrs	r3, r3, #20
 8010f00:	6003      	str	r3, [r0, #0]
 8010f02:	4770      	bx	lr

08010f04 <ClearEP_CTR_TX>:
 8010f04:	0080      	lsls	r0, r0, #2
 8010f06:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010f0a:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010f0e:	6803      	ldr	r3, [r0, #0]
 8010f10:	f423 43e1 	bic.w	r3, r3, #28800	; 0x7080
 8010f14:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8010f18:	041b      	lsls	r3, r3, #16
 8010f1a:	0c1b      	lsrs	r3, r3, #16
 8010f1c:	6003      	str	r3, [r0, #0]
 8010f1e:	4770      	bx	lr

08010f20 <ToggleDTOG_RX>:
 8010f20:	0080      	lsls	r0, r0, #2
 8010f22:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010f26:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010f2a:	6803      	ldr	r3, [r0, #0]
 8010f2c:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8010f30:	051b      	lsls	r3, r3, #20
 8010f32:	0d1b      	lsrs	r3, r3, #20
 8010f34:	f443 4340 	orr.w	r3, r3, #49152	; 0xc000
 8010f38:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010f3c:	6003      	str	r3, [r0, #0]
 8010f3e:	4770      	bx	lr

08010f40 <ToggleDTOG_TX>:
 8010f40:	0080      	lsls	r0, r0, #2
 8010f42:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010f46:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010f4a:	6803      	ldr	r3, [r0, #0]
 8010f4c:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8010f50:	051b      	lsls	r3, r3, #20
 8010f52:	0d1b      	lsrs	r3, r3, #20
 8010f54:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8010f58:	f043 03c0 	orr.w	r3, r3, #192	; 0xc0
 8010f5c:	6003      	str	r3, [r0, #0]
 8010f5e:	4770      	bx	lr

08010f60 <ClearDTOG_RX>:
 8010f60:	0080      	lsls	r0, r0, #2
 8010f62:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010f66:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010f6a:	6803      	ldr	r3, [r0, #0]
 8010f6c:	045b      	lsls	r3, r3, #17
 8010f6e:	d509      	bpl.n	8010f84 <ClearDTOG_RX+0x24>
 8010f70:	6803      	ldr	r3, [r0, #0]
 8010f72:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8010f76:	051b      	lsls	r3, r3, #20
 8010f78:	0d1b      	lsrs	r3, r3, #20
 8010f7a:	f443 4340 	orr.w	r3, r3, #49152	; 0xc000
 8010f7e:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8010f82:	6003      	str	r3, [r0, #0]
 8010f84:	4770      	bx	lr

08010f86 <ClearDTOG_TX>:
 8010f86:	0080      	lsls	r0, r0, #2
 8010f88:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010f8c:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010f90:	6803      	ldr	r3, [r0, #0]
 8010f92:	065b      	lsls	r3, r3, #25
 8010f94:	d509      	bpl.n	8010faa <ClearDTOG_TX+0x24>
 8010f96:	6803      	ldr	r3, [r0, #0]
 8010f98:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8010f9c:	051b      	lsls	r3, r3, #20
 8010f9e:	0d1b      	lsrs	r3, r3, #20
 8010fa0:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8010fa4:	f043 03c0 	orr.w	r3, r3, #192	; 0xc0
 8010fa8:	6003      	str	r3, [r0, #0]
 8010faa:	4770      	bx	lr

08010fac <SetEPAddress>:
 8010fac:	0080      	lsls	r0, r0, #2
 8010fae:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010fb2:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010fb6:	6803      	ldr	r3, [r0, #0]
 8010fb8:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
 8010fbc:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8010fc0:	051b      	lsls	r3, r3, #20
 8010fc2:	0d1b      	lsrs	r3, r3, #20
 8010fc4:	f041 0180 	orr.w	r1, r1, #128	; 0x80
 8010fc8:	430b      	orrs	r3, r1
 8010fca:	6003      	str	r3, [r0, #0]
 8010fcc:	4770      	bx	lr

08010fce <GetEPAddress>:
 8010fce:	0080      	lsls	r0, r0, #2
 8010fd0:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 8010fd4:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8010fd8:	6800      	ldr	r0, [r0, #0]
 8010fda:	f000 000f 	and.w	r0, r0, #15
 8010fde:	4770      	bx	lr

08010fe0 <SetEPTxAddr>:
 8010fe0:	4b06      	ldr	r3, [pc, #24]	; (8010ffc <SetEPTxAddr+0x1c>)
 8010fe2:	0849      	lsrs	r1, r1, #1
 8010fe4:	681b      	ldr	r3, [r3, #0]
 8010fe6:	0049      	lsls	r1, r1, #1
 8010fe8:	b29b      	uxth	r3, r3
 8010fea:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 8010fee:	f100 5000 	add.w	r0, r0, #536870912	; 0x20000000
 8010ff2:	f500 5040 	add.w	r0, r0, #12288	; 0x3000
 8010ff6:	0040      	lsls	r0, r0, #1
 8010ff8:	6001      	str	r1, [r0, #0]
 8010ffa:	4770      	bx	lr
 8010ffc:	40005c50 	.word	0x40005c50

08011000 <SetEPRxAddr>:
 8011000:	4b05      	ldr	r3, [pc, #20]	; (8011018 <SetEPRxAddr+0x18>)
 8011002:	0849      	lsrs	r1, r1, #1
 8011004:	681b      	ldr	r3, [r3, #0]
 8011006:	0049      	lsls	r1, r1, #1
 8011008:	b29b      	uxth	r3, r3
 801100a:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 801100e:	4b03      	ldr	r3, [pc, #12]	; (801101c <SetEPRxAddr+0x1c>)
 8011010:	4403      	add	r3, r0
 8011012:	005b      	lsls	r3, r3, #1
 8011014:	6019      	str	r1, [r3, #0]
 8011016:	4770      	bx	lr
 8011018:	40005c50 	.word	0x40005c50
 801101c:	20003004 	.word	0x20003004

08011020 <GetEPTxAddr>:
 8011020:	4b05      	ldr	r3, [pc, #20]	; (8011038 <GetEPTxAddr+0x18>)
 8011022:	681b      	ldr	r3, [r3, #0]
 8011024:	b29b      	uxth	r3, r3
 8011026:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 801102a:	f100 5000 	add.w	r0, r0, #536870912	; 0x20000000
 801102e:	f500 5040 	add.w	r0, r0, #12288	; 0x3000
 8011032:	0040      	lsls	r0, r0, #1
 8011034:	8800      	ldrh	r0, [r0, #0]
 8011036:	4770      	bx	lr
 8011038:	40005c50 	.word	0x40005c50

0801103c <GetEPRxAddr>:
 801103c:	4b04      	ldr	r3, [pc, #16]	; (8011050 <GetEPRxAddr+0x14>)
 801103e:	681b      	ldr	r3, [r3, #0]
 8011040:	b29b      	uxth	r3, r3
 8011042:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 8011046:	4b03      	ldr	r3, [pc, #12]	; (8011054 <GetEPRxAddr+0x18>)
 8011048:	4403      	add	r3, r0
 801104a:	005b      	lsls	r3, r3, #1
 801104c:	8818      	ldrh	r0, [r3, #0]
 801104e:	4770      	bx	lr
 8011050:	40005c50 	.word	0x40005c50
 8011054:	20003004 	.word	0x20003004

08011058 <SetEPTxCount>:
 8011058:	4b04      	ldr	r3, [pc, #16]	; (801106c <SetEPTxCount+0x14>)
 801105a:	681b      	ldr	r3, [r3, #0]
 801105c:	b29b      	uxth	r3, r3
 801105e:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 8011062:	4b03      	ldr	r3, [pc, #12]	; (8011070 <SetEPTxCount+0x18>)
 8011064:	4403      	add	r3, r0
 8011066:	005b      	lsls	r3, r3, #1
 8011068:	6019      	str	r1, [r3, #0]
 801106a:	4770      	bx	lr
 801106c:	40005c50 	.word	0x40005c50
 8011070:	20003002 	.word	0x20003002

08011074 <SetEPCountRxReg>:
 8011074:	293e      	cmp	r1, #62	; 0x3e
 8011076:	d90a      	bls.n	801108e <SetEPCountRxReg+0x1a>
 8011078:	094b      	lsrs	r3, r1, #5
 801107a:	06c9      	lsls	r1, r1, #27
 801107c:	bf04      	itt	eq
 801107e:	f103 33ff 	addeq.w	r3, r3, #4294967295	; 0xffffffff
 8011082:	b29b      	uxtheq	r3, r3
 8011084:	029b      	lsls	r3, r3, #10
 8011086:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 801108a:	6003      	str	r3, [r0, #0]
 801108c:	4770      	bx	lr
 801108e:	084b      	lsrs	r3, r1, #1
 8011090:	07ca      	lsls	r2, r1, #31
 8011092:	bf48      	it	mi
 8011094:	3301      	addmi	r3, #1
 8011096:	029b      	lsls	r3, r3, #10
 8011098:	e7f7      	b.n	801108a <SetEPCountRxReg+0x16>

0801109a <SetEPRxCount>:
 801109a:	4b0e      	ldr	r3, [pc, #56]	; (80110d4 <SetEPRxCount+0x3a>)
 801109c:	4a0e      	ldr	r2, [pc, #56]	; (80110d8 <SetEPRxCount+0x3e>)
 801109e:	681b      	ldr	r3, [r3, #0]
 80110a0:	293e      	cmp	r1, #62	; 0x3e
 80110a2:	b29b      	uxth	r3, r3
 80110a4:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 80110a8:	4402      	add	r2, r0
 80110aa:	ea4f 0242 	mov.w	r2, r2, lsl #1
 80110ae:	d90a      	bls.n	80110c6 <SetEPRxCount+0x2c>
 80110b0:	094b      	lsrs	r3, r1, #5
 80110b2:	06c8      	lsls	r0, r1, #27
 80110b4:	bf04      	itt	eq
 80110b6:	f103 33ff 	addeq.w	r3, r3, #4294967295	; 0xffffffff
 80110ba:	b29b      	uxtheq	r3, r3
 80110bc:	029b      	lsls	r3, r3, #10
 80110be:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 80110c2:	6013      	str	r3, [r2, #0]
 80110c4:	4770      	bx	lr
 80110c6:	084b      	lsrs	r3, r1, #1
 80110c8:	07c9      	lsls	r1, r1, #31
 80110ca:	bf48      	it	mi
 80110cc:	3301      	addmi	r3, #1
 80110ce:	029b      	lsls	r3, r3, #10
 80110d0:	e7f7      	b.n	80110c2 <SetEPRxCount+0x28>
 80110d2:	bf00      	nop
 80110d4:	40005c50 	.word	0x40005c50
 80110d8:	20003006 	.word	0x20003006

080110dc <GetEPTxCount>:
 80110dc:	4b05      	ldr	r3, [pc, #20]	; (80110f4 <GetEPTxCount+0x18>)
 80110de:	681b      	ldr	r3, [r3, #0]
 80110e0:	b29b      	uxth	r3, r3
 80110e2:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 80110e6:	4b04      	ldr	r3, [pc, #16]	; (80110f8 <GetEPTxCount+0x1c>)
 80110e8:	4403      	add	r3, r0
 80110ea:	005b      	lsls	r3, r3, #1
 80110ec:	6818      	ldr	r0, [r3, #0]
 80110ee:	f3c0 0009 	ubfx	r0, r0, #0, #10
 80110f2:	4770      	bx	lr
 80110f4:	40005c50 	.word	0x40005c50
 80110f8:	20003002 	.word	0x20003002

080110fc <GetEPRxCount>:
 80110fc:	4b05      	ldr	r3, [pc, #20]	; (8011114 <GetEPRxCount+0x18>)
 80110fe:	681b      	ldr	r3, [r3, #0]
 8011100:	b29b      	uxth	r3, r3
 8011102:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 8011106:	4b04      	ldr	r3, [pc, #16]	; (8011118 <GetEPRxCount+0x1c>)
 8011108:	4403      	add	r3, r0
 801110a:	005b      	lsls	r3, r3, #1
 801110c:	6818      	ldr	r0, [r3, #0]
 801110e:	f3c0 0009 	ubfx	r0, r0, #0, #10
 8011112:	4770      	bx	lr
 8011114:	40005c50 	.word	0x40005c50
 8011118:	20003006 	.word	0x20003006

0801111c <SetEPDblBuffAddr>:
 801111c:	b510      	push	{r4, lr}
 801111e:	4c0b      	ldr	r4, [pc, #44]	; (801114c <SetEPDblBuffAddr+0x30>)
 8011120:	00c0      	lsls	r0, r0, #3
 8011122:	6823      	ldr	r3, [r4, #0]
 8011124:	0849      	lsrs	r1, r1, #1
 8011126:	b29b      	uxth	r3, r3
 8011128:	4403      	add	r3, r0
 801112a:	f103 5300 	add.w	r3, r3, #536870912	; 0x20000000
 801112e:	f503 5340 	add.w	r3, r3, #12288	; 0x3000
 8011132:	005b      	lsls	r3, r3, #1
 8011134:	0049      	lsls	r1, r1, #1
 8011136:	6019      	str	r1, [r3, #0]
 8011138:	6823      	ldr	r3, [r4, #0]
 801113a:	0852      	lsrs	r2, r2, #1
 801113c:	b29b      	uxth	r3, r3
 801113e:	4418      	add	r0, r3
 8011140:	4b03      	ldr	r3, [pc, #12]	; (8011150 <SetEPDblBuffAddr+0x34>)
 8011142:	0052      	lsls	r2, r2, #1
 8011144:	4403      	add	r3, r0
 8011146:	005b      	lsls	r3, r3, #1
 8011148:	601a      	str	r2, [r3, #0]
 801114a:	bd10      	pop	{r4, pc}
 801114c:	40005c50 	.word	0x40005c50
 8011150:	20003004 	.word	0x20003004

08011154 <SetEPDblBuf0Addr>:
 8011154:	f7ff bf44 	b.w	8010fe0 <SetEPTxAddr>

08011158 <SetEPDblBuf1Addr>:
 8011158:	f7ff bf52 	b.w	8011000 <SetEPRxAddr>

0801115c <GetEPDblBuf0Addr>:
 801115c:	f7ff bf60 	b.w	8011020 <GetEPTxAddr>

08011160 <GetEPDblBuf1Addr>:
 8011160:	f7ff bf6c 	b.w	801103c <GetEPRxAddr>

08011164 <SetEPDblBuffCount>:
 8011164:	2901      	cmp	r1, #1
 8011166:	b510      	push	{r4, lr}
 8011168:	d12b      	bne.n	80111c2 <SetEPDblBuffCount+0x5e>
 801116a:	4c25      	ldr	r4, [pc, #148]	; (8011200 <SetEPDblBuffCount+0x9c>)
 801116c:	00c0      	lsls	r0, r0, #3
 801116e:	6821      	ldr	r1, [r4, #0]
 8011170:	4b24      	ldr	r3, [pc, #144]	; (8011204 <SetEPDblBuffCount+0xa0>)
 8011172:	b289      	uxth	r1, r1
 8011174:	4401      	add	r1, r0
 8011176:	440b      	add	r3, r1
 8011178:	2a3e      	cmp	r2, #62	; 0x3e
 801117a:	ea4f 0143 	mov.w	r1, r3, lsl #1
 801117e:	d919      	bls.n	80111b4 <SetEPDblBuffCount+0x50>
 8011180:	0953      	lsrs	r3, r2, #5
 8011182:	f012 0f1f 	tst.w	r2, #31
 8011186:	bf04      	itt	eq
 8011188:	f103 33ff 	addeq.w	r3, r3, #4294967295	; 0xffffffff
 801118c:	b29b      	uxtheq	r3, r3
 801118e:	029b      	lsls	r3, r3, #10
 8011190:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8011194:	600b      	str	r3, [r1, #0]
 8011196:	6823      	ldr	r3, [r4, #0]
 8011198:	491b      	ldr	r1, [pc, #108]	; (8011208 <SetEPDblBuffCount+0xa4>)
 801119a:	b29b      	uxth	r3, r3
 801119c:	4418      	add	r0, r3
 801119e:	4401      	add	r1, r0
 80111a0:	2a3e      	cmp	r2, #62	; 0x3e
 80111a2:	ea4f 0141 	mov.w	r1, r1, lsl #1
 80111a6:	d81f      	bhi.n	80111e8 <SetEPDblBuffCount+0x84>
 80111a8:	0853      	lsrs	r3, r2, #1
 80111aa:	07d2      	lsls	r2, r2, #31
 80111ac:	bf48      	it	mi
 80111ae:	3301      	addmi	r3, #1
 80111b0:	029b      	lsls	r3, r3, #10
 80111b2:	e022      	b.n	80111fa <SetEPDblBuffCount+0x96>
 80111b4:	0853      	lsrs	r3, r2, #1
 80111b6:	f012 0f01 	tst.w	r2, #1
 80111ba:	bf18      	it	ne
 80111bc:	3301      	addne	r3, #1
 80111be:	029b      	lsls	r3, r3, #10
 80111c0:	e7e8      	b.n	8011194 <SetEPDblBuffCount+0x30>
 80111c2:	2902      	cmp	r1, #2
 80111c4:	d10f      	bne.n	80111e6 <SetEPDblBuffCount+0x82>
 80111c6:	4c0e      	ldr	r4, [pc, #56]	; (8011200 <SetEPDblBuffCount+0x9c>)
 80111c8:	00c0      	lsls	r0, r0, #3
 80111ca:	6821      	ldr	r1, [r4, #0]
 80111cc:	4b0d      	ldr	r3, [pc, #52]	; (8011204 <SetEPDblBuffCount+0xa0>)
 80111ce:	b289      	uxth	r1, r1
 80111d0:	4401      	add	r1, r0
 80111d2:	440b      	add	r3, r1
 80111d4:	005b      	lsls	r3, r3, #1
 80111d6:	601a      	str	r2, [r3, #0]
 80111d8:	6823      	ldr	r3, [r4, #0]
 80111da:	b29b      	uxth	r3, r3
 80111dc:	4418      	add	r0, r3
 80111de:	4b0a      	ldr	r3, [pc, #40]	; (8011208 <SetEPDblBuffCount+0xa4>)
 80111e0:	4403      	add	r3, r0
 80111e2:	005b      	lsls	r3, r3, #1
 80111e4:	601a      	str	r2, [r3, #0]
 80111e6:	bd10      	pop	{r4, pc}
 80111e8:	0953      	lsrs	r3, r2, #5
 80111ea:	06d0      	lsls	r0, r2, #27
 80111ec:	bf04      	itt	eq
 80111ee:	f103 33ff 	addeq.w	r3, r3, #4294967295	; 0xffffffff
 80111f2:	b29b      	uxtheq	r3, r3
 80111f4:	029b      	lsls	r3, r3, #10
 80111f6:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 80111fa:	600b      	str	r3, [r1, #0]
 80111fc:	e7f3      	b.n	80111e6 <SetEPDblBuffCount+0x82>
 80111fe:	bf00      	nop
 8011200:	40005c50 	.word	0x40005c50
 8011204:	20003002 	.word	0x20003002
 8011208:	20003006 	.word	0x20003006

0801120c <SetEPDblBuf0Count>:
 801120c:	2901      	cmp	r1, #1
 801120e:	d11b      	bne.n	8011248 <SetEPDblBuf0Count+0x3c>
 8011210:	4b13      	ldr	r3, [pc, #76]	; (8011260 <SetEPDblBuf0Count+0x54>)
 8011212:	4914      	ldr	r1, [pc, #80]	; (8011264 <SetEPDblBuf0Count+0x58>)
 8011214:	681b      	ldr	r3, [r3, #0]
 8011216:	2a3e      	cmp	r2, #62	; 0x3e
 8011218:	b29b      	uxth	r3, r3
 801121a:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 801121e:	4401      	add	r1, r0
 8011220:	ea4f 0141 	mov.w	r1, r1, lsl #1
 8011224:	d90a      	bls.n	801123c <SetEPDblBuf0Count+0x30>
 8011226:	0953      	lsrs	r3, r2, #5
 8011228:	06d0      	lsls	r0, r2, #27
 801122a:	bf04      	itt	eq
 801122c:	f103 33ff 	addeq.w	r3, r3, #4294967295	; 0xffffffff
 8011230:	b29b      	uxtheq	r3, r3
 8011232:	029b      	lsls	r3, r3, #10
 8011234:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8011238:	600b      	str	r3, [r1, #0]
 801123a:	4770      	bx	lr
 801123c:	0853      	lsrs	r3, r2, #1
 801123e:	07d2      	lsls	r2, r2, #31
 8011240:	bf48      	it	mi
 8011242:	3301      	addmi	r3, #1
 8011244:	029b      	lsls	r3, r3, #10
 8011246:	e7f7      	b.n	8011238 <SetEPDblBuf0Count+0x2c>
 8011248:	2902      	cmp	r1, #2
 801124a:	d108      	bne.n	801125e <SetEPDblBuf0Count+0x52>
 801124c:	4b04      	ldr	r3, [pc, #16]	; (8011260 <SetEPDblBuf0Count+0x54>)
 801124e:	681b      	ldr	r3, [r3, #0]
 8011250:	b29b      	uxth	r3, r3
 8011252:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 8011256:	4b03      	ldr	r3, [pc, #12]	; (8011264 <SetEPDblBuf0Count+0x58>)
 8011258:	4403      	add	r3, r0
 801125a:	005b      	lsls	r3, r3, #1
 801125c:	601a      	str	r2, [r3, #0]
 801125e:	4770      	bx	lr
 8011260:	40005c50 	.word	0x40005c50
 8011264:	20003002 	.word	0x20003002

08011268 <SetEPDblBuf1Count>:
 8011268:	2901      	cmp	r1, #1
 801126a:	d11b      	bne.n	80112a4 <SetEPDblBuf1Count+0x3c>
 801126c:	4b13      	ldr	r3, [pc, #76]	; (80112bc <SetEPDblBuf1Count+0x54>)
 801126e:	4914      	ldr	r1, [pc, #80]	; (80112c0 <SetEPDblBuf1Count+0x58>)
 8011270:	681b      	ldr	r3, [r3, #0]
 8011272:	2a3e      	cmp	r2, #62	; 0x3e
 8011274:	b29b      	uxth	r3, r3
 8011276:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 801127a:	4401      	add	r1, r0
 801127c:	ea4f 0141 	mov.w	r1, r1, lsl #1
 8011280:	d90a      	bls.n	8011298 <SetEPDblBuf1Count+0x30>
 8011282:	0953      	lsrs	r3, r2, #5
 8011284:	06d0      	lsls	r0, r2, #27
 8011286:	bf04      	itt	eq
 8011288:	f103 33ff 	addeq.w	r3, r3, #4294967295	; 0xffffffff
 801128c:	b29b      	uxtheq	r3, r3
 801128e:	029b      	lsls	r3, r3, #10
 8011290:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8011294:	600b      	str	r3, [r1, #0]
 8011296:	4770      	bx	lr
 8011298:	0853      	lsrs	r3, r2, #1
 801129a:	07d2      	lsls	r2, r2, #31
 801129c:	bf48      	it	mi
 801129e:	3301      	addmi	r3, #1
 80112a0:	029b      	lsls	r3, r3, #10
 80112a2:	e7f7      	b.n	8011294 <SetEPDblBuf1Count+0x2c>
 80112a4:	2902      	cmp	r1, #2
 80112a6:	d108      	bne.n	80112ba <SetEPDblBuf1Count+0x52>
 80112a8:	4b04      	ldr	r3, [pc, #16]	; (80112bc <SetEPDblBuf1Count+0x54>)
 80112aa:	681b      	ldr	r3, [r3, #0]
 80112ac:	b29b      	uxth	r3, r3
 80112ae:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
 80112b2:	4b03      	ldr	r3, [pc, #12]	; (80112c0 <SetEPDblBuf1Count+0x58>)
 80112b4:	4403      	add	r3, r0
 80112b6:	005b      	lsls	r3, r3, #1
 80112b8:	601a      	str	r2, [r3, #0]
 80112ba:	4770      	bx	lr
 80112bc:	40005c50 	.word	0x40005c50
 80112c0:	20003006 	.word	0x20003006

080112c4 <GetEPDblBuf0Count>:
 80112c4:	f7ff bf0a 	b.w	80110dc <GetEPTxCount>

080112c8 <GetEPDblBuf1Count>:
 80112c8:	f7ff bf18 	b.w	80110fc <GetEPRxCount>

080112cc <GetEPDblBufDir>:
 80112cc:	490d      	ldr	r1, [pc, #52]	; (8011304 <GetEPDblBufDir+0x38>)
 80112ce:	00c0      	lsls	r0, r0, #3
 80112d0:	680b      	ldr	r3, [r1, #0]
 80112d2:	b29b      	uxth	r3, r3
 80112d4:	18c2      	adds	r2, r0, r3
 80112d6:	4b0c      	ldr	r3, [pc, #48]	; (8011308 <GetEPDblBufDir+0x3c>)
 80112d8:	4413      	add	r3, r2
 80112da:	005b      	lsls	r3, r3, #1
 80112dc:	681b      	ldr	r3, [r3, #0]
 80112de:	f413 4f7c 	tst.w	r3, #64512	; 0xfc00
 80112e2:	d10d      	bne.n	8011300 <GetEPDblBufDir+0x34>
 80112e4:	680b      	ldr	r3, [r1, #0]
 80112e6:	b29b      	uxth	r3, r3
 80112e8:	4418      	add	r0, r3
 80112ea:	4b08      	ldr	r3, [pc, #32]	; (801130c <GetEPDblBufDir+0x40>)
 80112ec:	4403      	add	r3, r0
 80112ee:	005b      	lsls	r3, r3, #1
 80112f0:	681b      	ldr	r3, [r3, #0]
 80112f2:	f3c3 0309 	ubfx	r3, r3, #0, #10
 80112f6:	2b00      	cmp	r3, #0
 80112f8:	bf14      	ite	ne
 80112fa:	2002      	movne	r0, #2
 80112fc:	2000      	moveq	r0, #0
 80112fe:	4770      	bx	lr
 8011300:	2001      	movs	r0, #1
 8011302:	4770      	bx	lr
 8011304:	40005c50 	.word	0x40005c50
 8011308:	20003006 	.word	0x20003006
 801130c:	20003002 	.word	0x20003002

08011310 <FreeUserBuffer>:
 8011310:	2901      	cmp	r1, #1
 8011312:	d10f      	bne.n	8011334 <FreeUserBuffer+0x24>
 8011314:	0080      	lsls	r0, r0, #2
 8011316:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 801131a:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 801131e:	6803      	ldr	r3, [r0, #0]
 8011320:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8011324:	051b      	lsls	r3, r3, #20
 8011326:	0d1b      	lsrs	r3, r3, #20
 8011328:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 801132c:	f043 03c0 	orr.w	r3, r3, #192	; 0xc0
 8011330:	6003      	str	r3, [r0, #0]
 8011332:	4770      	bx	lr
 8011334:	2902      	cmp	r1, #2
 8011336:	d1fc      	bne.n	8011332 <FreeUserBuffer+0x22>
 8011338:	0080      	lsls	r0, r0, #2
 801133a:	f100 4080 	add.w	r0, r0, #1073741824	; 0x40000000
 801133e:	f500 40b8 	add.w	r0, r0, #23552	; 0x5c00
 8011342:	6803      	ldr	r3, [r0, #0]
 8011344:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8011348:	051b      	lsls	r3, r3, #20
 801134a:	0d1b      	lsrs	r3, r3, #20
 801134c:	f443 4340 	orr.w	r3, r3, #49152	; 0xc000
 8011350:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8011354:	e7ec      	b.n	8011330 <FreeUserBuffer+0x20>

08011356 <ToWord>:
 8011356:	ea41 2000 	orr.w	r0, r1, r0, lsl #8
 801135a:	4770      	bx	lr

0801135c <ByteSwap>:
 801135c:	b2c3      	uxtb	r3, r0
 801135e:	0a00      	lsrs	r0, r0, #8
 8011360:	ea40 2003 	orr.w	r0, r0, r3, lsl #8
 8011364:	4770      	bx	lr
 8011366:	bf00      	nop

08011368 <UserToPMABufferCopy>:
 8011368:	f101 5100 	add.w	r1, r1, #536870912	; 0x20000000
 801136c:	3201      	adds	r2, #1
 801136e:	f501 5140 	add.w	r1, r1, #12288	; 0x3000
 8011372:	1052      	asrs	r2, r2, #1
 8011374:	0049      	lsls	r1, r1, #1
 8011376:	b902      	cbnz	r2, 801137a <UserToPMABufferCopy+0x12>
 8011378:	4770      	bx	lr
 801137a:	f830 3b02 	ldrh.w	r3, [r0], #2
 801137e:	3a01      	subs	r2, #1
 8011380:	f821 3b04 	strh.w	r3, [r1], #4
 8011384:	e7f7      	b.n	8011376 <UserToPMABufferCopy+0xe>

08011386 <PMAToUserBufferCopy>:
 8011386:	2300      	movs	r3, #0
 8011388:	b510      	push	{r4, lr}
 801138a:	f101 5100 	add.w	r1, r1, #536870912	; 0x20000000
 801138e:	3201      	adds	r2, #1
 8011390:	f501 5140 	add.w	r1, r1, #12288	; 0x3000
 8011394:	1052      	asrs	r2, r2, #1
 8011396:	0049      	lsls	r1, r1, #1
 8011398:	429a      	cmp	r2, r3
 801139a:	d100      	bne.n	801139e <PMAToUserBufferCopy+0x18>
 801139c:	bd10      	pop	{r4, pc}
 801139e:	f851 4023 	ldr.w	r4, [r1, r3, lsl #2]
 80113a2:	f820 4013 	strh.w	r4, [r0, r3, lsl #1]
 80113a6:	3301      	adds	r3, #1
 80113a8:	e7f6      	b.n	8011398 <PMAToUserBufferCopy+0x12>
	...

080113ac <PowerOn>:
 80113ac:	2201      	movs	r2, #1
 80113ae:	2000      	movs	r0, #0
 80113b0:	4b05      	ldr	r3, [pc, #20]	; (80113c8 <PowerOn+0x1c>)
 80113b2:	4906      	ldr	r1, [pc, #24]	; (80113cc <PowerOn+0x20>)
 80113b4:	601a      	str	r2, [r3, #0]
 80113b6:	4a06      	ldr	r2, [pc, #24]	; (80113d0 <PowerOn+0x24>)
 80113b8:	6018      	str	r0, [r3, #0]
 80113ba:	6010      	str	r0, [r2, #0]
 80113bc:	f44f 52e0 	mov.w	r2, #7168	; 0x1c00
 80113c0:	800a      	strh	r2, [r1, #0]
 80113c2:	601a      	str	r2, [r3, #0]
 80113c4:	4770      	bx	lr
 80113c6:	bf00      	nop
 80113c8:	40005c40 	.word	0x40005c40
 80113cc:	2000952c 	.word	0x2000952c
 80113d0:	40005c44 	.word	0x40005c44

080113d4 <PowerOff>:
 80113d4:	2201      	movs	r2, #1
 80113d6:	2000      	movs	r0, #0
 80113d8:	4b03      	ldr	r3, [pc, #12]	; (80113e8 <PowerOff+0x14>)
 80113da:	601a      	str	r2, [r3, #0]
 80113dc:	4a03      	ldr	r2, [pc, #12]	; (80113ec <PowerOff+0x18>)
 80113de:	6010      	str	r0, [r2, #0]
 80113e0:	2203      	movs	r2, #3
 80113e2:	601a      	str	r2, [r3, #0]
 80113e4:	4770      	bx	lr
 80113e6:	bf00      	nop
 80113e8:	40005c40 	.word	0x40005c40
 80113ec:	40005c44 	.word	0x40005c44

080113f0 <Suspend>:
 80113f0:	4a05      	ldr	r2, [pc, #20]	; (8011408 <Suspend+0x18>)
 80113f2:	6813      	ldr	r3, [r2, #0]
 80113f4:	b29b      	uxth	r3, r3
 80113f6:	f043 0308 	orr.w	r3, r3, #8
 80113fa:	6013      	str	r3, [r2, #0]
 80113fc:	6813      	ldr	r3, [r2, #0]
 80113fe:	b29b      	uxth	r3, r3
 8011400:	f043 0304 	orr.w	r3, r3, #4
 8011404:	6013      	str	r3, [r2, #0]
 8011406:	4770      	bx	lr
 8011408:	40005c40 	.word	0x40005c40

0801140c <Resume_Init>:
 801140c:	f64f 72fb 	movw	r2, #65531	; 0xfffb
 8011410:	4b03      	ldr	r3, [pc, #12]	; (8011420 <Resume_Init+0x14>)
 8011412:	6819      	ldr	r1, [r3, #0]
 8011414:	400a      	ands	r2, r1
 8011416:	601a      	str	r2, [r3, #0]
 8011418:	f44f 4204 	mov.w	r2, #33792	; 0x8400
 801141c:	601a      	str	r2, [r3, #0]
 801141e:	4770      	bx	lr
 8011420:	40005c40 	.word	0x40005c40

08011424 <Resume>:
 8011424:	b510      	push	{r4, lr}
 8011426:	4c1d      	ldr	r4, [pc, #116]	; (801149c <Resume+0x78>)
 8011428:	2807      	cmp	r0, #7
 801142a:	bf18      	it	ne
 801142c:	7020      	strbne	r0, [r4, #0]
 801142e:	7823      	ldrb	r3, [r4, #0]
 8011430:	2b05      	cmp	r3, #5
 8011432:	d806      	bhi.n	8011442 <Resume+0x1e>
 8011434:	e8df f003 	tbb	[pc, r3]
 8011438:	100b0703 	.word	0x100b0703
 801143c:	2318      	.short	0x2318
 801143e:	f7ff ffe5 	bl	801140c <Resume_Init>
 8011442:	2306      	movs	r3, #6
 8011444:	e006      	b.n	8011454 <Resume+0x30>
 8011446:	f7ff ffe1 	bl	801140c <Resume_Init>
 801144a:	2304      	movs	r3, #4
 801144c:	e002      	b.n	8011454 <Resume+0x30>
 801144e:	2302      	movs	r3, #2
 8011450:	7063      	strb	r3, [r4, #1]
 8011452:	2303      	movs	r3, #3
 8011454:	7023      	strb	r3, [r4, #0]
 8011456:	e006      	b.n	8011466 <Resume+0x42>
 8011458:	7863      	ldrb	r3, [r4, #1]
 801145a:	3b01      	subs	r3, #1
 801145c:	b2db      	uxtb	r3, r3
 801145e:	7063      	strb	r3, [r4, #1]
 8011460:	7863      	ldrb	r3, [r4, #1]
 8011462:	2b00      	cmp	r3, #0
 8011464:	d0f1      	beq.n	801144a <Resume+0x26>
 8011466:	bd10      	pop	{r4, pc}
 8011468:	4a0d      	ldr	r2, [pc, #52]	; (80114a0 <Resume+0x7c>)
 801146a:	6813      	ldr	r3, [r2, #0]
 801146c:	b29b      	uxth	r3, r3
 801146e:	f043 0310 	orr.w	r3, r3, #16
 8011472:	6013      	str	r3, [r2, #0]
 8011474:	2305      	movs	r3, #5
 8011476:	7023      	strb	r3, [r4, #0]
 8011478:	230a      	movs	r3, #10
 801147a:	7063      	strb	r3, [r4, #1]
 801147c:	e7f3      	b.n	8011466 <Resume+0x42>
 801147e:	7863      	ldrb	r3, [r4, #1]
 8011480:	3b01      	subs	r3, #1
 8011482:	b2db      	uxtb	r3, r3
 8011484:	7063      	strb	r3, [r4, #1]
 8011486:	7863      	ldrb	r3, [r4, #1]
 8011488:	2b00      	cmp	r3, #0
 801148a:	d1ec      	bne.n	8011466 <Resume+0x42>
 801148c:	f64f 73ef 	movw	r3, #65519	; 0xffef
 8011490:	4a03      	ldr	r2, [pc, #12]	; (80114a0 <Resume+0x7c>)
 8011492:	6811      	ldr	r1, [r2, #0]
 8011494:	400b      	ands	r3, r1
 8011496:	6013      	str	r3, [r2, #0]
 8011498:	e7d3      	b.n	8011442 <Resume+0x1e>
 801149a:	bf00      	nop
 801149c:	200095c0 	.word	0x200095c0
 80114a0:	40005c40 	.word	0x40005c40

080114a4 <Mass_Storage_SetDeviceAddress>:
 80114a4:	2204      	movs	r2, #4
 80114a6:	4b01      	ldr	r3, [pc, #4]	; (80114ac <Mass_Storage_SetDeviceAddress+0x8>)
 80114a8:	601a      	str	r2, [r3, #0]
 80114aa:	4770      	bx	lr
 80114ac:	200095c4 	.word	0x200095c4

080114b0 <MASS_Status_In>:
 80114b0:	4770      	bx	lr

080114b2 <MASS_Get_Interface_Setting>:
 80114b2:	b921      	cbnz	r1, 80114be <MASS_Get_Interface_Setting+0xc>
 80114b4:	2800      	cmp	r0, #0
 80114b6:	bf14      	ite	ne
 80114b8:	2002      	movne	r0, #2
 80114ba:	2000      	moveq	r0, #0
 80114bc:	4770      	bx	lr
 80114be:	2002      	movs	r0, #2
 80114c0:	4770      	bx	lr

080114c2 <Get_Max_Lun>:
 80114c2:	b920      	cbnz	r0, 80114ce <Get_Max_Lun+0xc>
 80114c4:	2201      	movs	r2, #1
 80114c6:	4b03      	ldr	r3, [pc, #12]	; (80114d4 <Get_Max_Lun+0x12>)
 80114c8:	681b      	ldr	r3, [r3, #0]
 80114ca:	821a      	strh	r2, [r3, #16]
 80114cc:	4770      	bx	lr
 80114ce:	4802      	ldr	r0, [pc, #8]	; (80114d8 <Get_Max_Lun+0x16>)
 80114d0:	4770      	bx	lr
 80114d2:	bf00      	nop
 80114d4:	20009504 	.word	0x20009504
 80114d8:	200095c8 	.word	0x200095c8

080114dc <MASS_Reset>:
 80114dc:	b538      	push	{r3, r4, r5, lr}
 80114de:	2400      	movs	r4, #0
 80114e0:	4b30      	ldr	r3, [pc, #192]	; (80115a4 <MASS_Reset+0xc8>)
 80114e2:	4a31      	ldr	r2, [pc, #196]	; (80115a8 <MASS_Reset+0xcc>)
 80114e4:	729c      	strb	r4, [r3, #10]
 80114e6:	4b31      	ldr	r3, [pc, #196]	; (80115ac <MASS_Reset+0xd0>)
 80114e8:	79d2      	ldrb	r2, [r2, #7]
 80114ea:	681b      	ldr	r3, [r3, #0]
 80114ec:	4620      	mov	r0, r4
 80114ee:	725a      	strb	r2, [r3, #9]
 80114f0:	f7ff fc0a 	bl	8010d08 <SetBTABLE>
 80114f4:	f44f 7100 	mov.w	r1, #512	; 0x200
 80114f8:	4620      	mov	r0, r4
 80114fa:	f7ff fc22 	bl	8010d42 <SetEPType>
 80114fe:	4d2c      	ldr	r5, [pc, #176]	; (80115b0 <MASS_Reset+0xd4>)
 8011500:	2120      	movs	r1, #32
 8011502:	4620      	mov	r0, r4
 8011504:	f7ff fc34 	bl	8010d70 <SetEPTxStatus>
 8011508:	2118      	movs	r1, #24
 801150a:	4620      	mov	r0, r4
 801150c:	f7ff fd78 	bl	8011000 <SetEPRxAddr>
 8011510:	f895 102c 	ldrb.w	r1, [r5, #44]	; 0x2c
 8011514:	4620      	mov	r0, r4
 8011516:	f7ff fdc0 	bl	801109a <SetEPRxCount>
 801151a:	2158      	movs	r1, #88	; 0x58
 801151c:	4620      	mov	r0, r4
 801151e:	f7ff fd5f 	bl	8010fe0 <SetEPTxAddr>
 8011522:	4620      	mov	r0, r4
 8011524:	f7ff fcc0 	bl	8010ea8 <Clear_Status_Out>
 8011528:	4620      	mov	r0, r4
 801152a:	f7ff fc8b 	bl	8010e44 <SetEPRxValid>
 801152e:	4621      	mov	r1, r4
 8011530:	2001      	movs	r0, #1
 8011532:	f7ff fc06 	bl	8010d42 <SetEPType>
 8011536:	2198      	movs	r1, #152	; 0x98
 8011538:	2001      	movs	r0, #1
 801153a:	f7ff fd51 	bl	8010fe0 <SetEPTxAddr>
 801153e:	2120      	movs	r1, #32
 8011540:	2001      	movs	r0, #1
 8011542:	f7ff fc15 	bl	8010d70 <SetEPTxStatus>
 8011546:	4621      	mov	r1, r4
 8011548:	2001      	movs	r0, #1
 801154a:	f7ff fc2a 	bl	8010da2 <SetEPRxStatus>
 801154e:	4621      	mov	r1, r4
 8011550:	2002      	movs	r0, #2
 8011552:	f7ff fbf6 	bl	8010d42 <SetEPType>
 8011556:	21d8      	movs	r1, #216	; 0xd8
 8011558:	2002      	movs	r0, #2
 801155a:	f7ff fd51 	bl	8011000 <SetEPRxAddr>
 801155e:	f895 102c 	ldrb.w	r1, [r5, #44]	; 0x2c
 8011562:	2002      	movs	r0, #2
 8011564:	f7ff fd99 	bl	801109a <SetEPRxCount>
 8011568:	f44f 5140 	mov.w	r1, #12288	; 0x3000
 801156c:	2002      	movs	r0, #2
 801156e:	f7ff fc18 	bl	8010da2 <SetEPRxStatus>
 8011572:	4621      	mov	r1, r4
 8011574:	2002      	movs	r0, #2
 8011576:	f7ff fbfb 	bl	8010d70 <SetEPTxStatus>
 801157a:	f895 102c 	ldrb.w	r1, [r5, #44]	; 0x2c
 801157e:	4620      	mov	r0, r4
 8011580:	f7ff fd8b 	bl	801109a <SetEPRxCount>
 8011584:	4620      	mov	r0, r4
 8011586:	f7ff fc5d 	bl	8010e44 <SetEPRxValid>
 801158a:	4620      	mov	r0, r4
 801158c:	f000 fc88 	bl	8011ea0 <SetDeviceAddress>
 8011590:	2201      	movs	r2, #1
 8011592:	4b08      	ldr	r3, [pc, #32]	; (80115b4 <MASS_Reset+0xd8>)
 8011594:	601a      	str	r2, [r3, #0]
 8011596:	4b08      	ldr	r3, [pc, #32]	; (80115b8 <MASS_Reset+0xdc>)
 8011598:	4a08      	ldr	r2, [pc, #32]	; (80115bc <MASS_Reset+0xe0>)
 801159a:	601a      	str	r2, [r3, #0]
 801159c:	4b08      	ldr	r3, [pc, #32]	; (80115c0 <MASS_Reset+0xe4>)
 801159e:	701c      	strb	r4, [r3, #0]
 80115a0:	bd38      	pop	{r3, r4, r5, pc}
 80115a2:	bf00      	nop
 80115a4:	20009508 	.word	0x20009508
 80115a8:	0801483e 	.word	0x0801483e
 80115ac:	20009504 	.word	0x20009504
 80115b0:	2000015c 	.word	0x2000015c
 80115b4:	200095c4 	.word	0x200095c4
 80115b8:	20009548 	.word	0x20009548
 80115bc:	43425355 	.word	0x43425355
 80115c0:	20009534 	.word	0x20009534

080115c4 <Mass_Storage_ClearFeature>:
 80115c4:	4b04      	ldr	r3, [pc, #16]	; (80115d8 <Mass_Storage_ClearFeature+0x14>)
 80115c6:	681a      	ldr	r2, [r3, #0]
 80115c8:	4b04      	ldr	r3, [pc, #16]	; (80115dc <Mass_Storage_ClearFeature+0x18>)
 80115ca:	429a      	cmp	r2, r3
 80115cc:	d002      	beq.n	80115d4 <Mass_Storage_ClearFeature+0x10>
 80115ce:	2002      	movs	r0, #2
 80115d0:	f7ff b9e4 	b.w	801099c <Bot_Abort>
 80115d4:	4770      	bx	lr
 80115d6:	bf00      	nop
 80115d8:	20009548 	.word	0x20009548
 80115dc:	43425355 	.word	0x43425355

080115e0 <MASS_GetDeviceDescriptor>:
 80115e0:	4901      	ldr	r1, [pc, #4]	; (80115e8 <MASS_GetDeviceDescriptor+0x8>)
 80115e2:	f000 ba6d 	b.w	8011ac0 <Standard_GetDescriptorData>
 80115e6:	bf00      	nop
 80115e8:	2000018c 	.word	0x2000018c

080115ec <MASS_GetConfigDescriptor>:
 80115ec:	4901      	ldr	r1, [pc, #4]	; (80115f4 <MASS_GetConfigDescriptor+0x8>)
 80115ee:	f000 ba67 	b.w	8011ac0 <Standard_GetDescriptorData>
 80115f2:	bf00      	nop
 80115f4:	20000194 	.word	0x20000194

080115f8 <MASS_GetStringDescriptor>:
 80115f8:	4b05      	ldr	r3, [pc, #20]	; (8011610 <MASS_GetStringDescriptor+0x18>)
 80115fa:	681b      	ldr	r3, [r3, #0]
 80115fc:	78db      	ldrb	r3, [r3, #3]
 80115fe:	2b05      	cmp	r3, #5
 8011600:	d804      	bhi.n	801160c <MASS_GetStringDescriptor+0x14>
 8011602:	4904      	ldr	r1, [pc, #16]	; (8011614 <MASS_GetStringDescriptor+0x1c>)
 8011604:	eb01 01c3 	add.w	r1, r1, r3, lsl #3
 8011608:	f000 ba5a 	b.w	8011ac0 <Standard_GetDescriptorData>
 801160c:	2000      	movs	r0, #0
 801160e:	4770      	bx	lr
 8011610:	20009504 	.word	0x20009504
 8011614:	2000019c 	.word	0x2000019c

08011618 <Mass_Storage_SetConfiguration>:
 8011618:	b508      	push	{r3, lr}
 801161a:	4b08      	ldr	r3, [pc, #32]	; (801163c <Mass_Storage_SetConfiguration+0x24>)
 801161c:	681b      	ldr	r3, [r3, #0]
 801161e:	7a9b      	ldrb	r3, [r3, #10]
 8011620:	b15b      	cbz	r3, 801163a <Mass_Storage_SetConfiguration+0x22>
 8011622:	2205      	movs	r2, #5
 8011624:	4b06      	ldr	r3, [pc, #24]	; (8011640 <Mass_Storage_SetConfiguration+0x28>)
 8011626:	2001      	movs	r0, #1
 8011628:	601a      	str	r2, [r3, #0]
 801162a:	f7ff fcac 	bl	8010f86 <ClearDTOG_TX>
 801162e:	2002      	movs	r0, #2
 8011630:	f7ff fc96 	bl	8010f60 <ClearDTOG_RX>
 8011634:	2200      	movs	r2, #0
 8011636:	4b03      	ldr	r3, [pc, #12]	; (8011644 <Mass_Storage_SetConfiguration+0x2c>)
 8011638:	701a      	strb	r2, [r3, #0]
 801163a:	bd08      	pop	{r3, pc}
 801163c:	20009504 	.word	0x20009504
 8011640:	200095c4 	.word	0x200095c4
 8011644:	20009534 	.word	0x20009534

08011648 <MASS_NoData_Setup>:
 8011648:	b510      	push	{r4, lr}
 801164a:	4b0e      	ldr	r3, [pc, #56]	; (8011684 <MASS_NoData_Setup+0x3c>)
 801164c:	681b      	ldr	r3, [r3, #0]
 801164e:	781a      	ldrb	r2, [r3, #0]
 8011650:	f002 027f 	and.w	r2, r2, #127	; 0x7f
 8011654:	2a21      	cmp	r2, #33	; 0x21
 8011656:	d112      	bne.n	801167e <MASS_NoData_Setup+0x36>
 8011658:	28ff      	cmp	r0, #255	; 0xff
 801165a:	d110      	bne.n	801167e <MASS_NoData_Setup+0x36>
 801165c:	885a      	ldrh	r2, [r3, #2]
 801165e:	b972      	cbnz	r2, 801167e <MASS_NoData_Setup+0x36>
 8011660:	685c      	ldr	r4, [r3, #4]
 8011662:	b964      	cbnz	r4, 801167e <MASS_NoData_Setup+0x36>
 8011664:	2001      	movs	r0, #1
 8011666:	f7ff fc8e 	bl	8010f86 <ClearDTOG_TX>
 801166a:	2002      	movs	r0, #2
 801166c:	f7ff fc78 	bl	8010f60 <ClearDTOG_RX>
 8011670:	4620      	mov	r0, r4
 8011672:	4b05      	ldr	r3, [pc, #20]	; (8011688 <MASS_NoData_Setup+0x40>)
 8011674:	4a05      	ldr	r2, [pc, #20]	; (801168c <MASS_NoData_Setup+0x44>)
 8011676:	601a      	str	r2, [r3, #0]
 8011678:	4b05      	ldr	r3, [pc, #20]	; (8011690 <MASS_NoData_Setup+0x48>)
 801167a:	701c      	strb	r4, [r3, #0]
 801167c:	bd10      	pop	{r4, pc}
 801167e:	2002      	movs	r0, #2
 8011680:	e7fc      	b.n	801167c <MASS_NoData_Setup+0x34>
 8011682:	bf00      	nop
 8011684:	20009504 	.word	0x20009504
 8011688:	20009548 	.word	0x20009548
 801168c:	43425355 	.word	0x43425355
 8011690:	20009534 	.word	0x20009534

08011694 <MASS_Status_Out>:
 8011694:	4770      	bx	lr

08011696 <MASS_Data_Setup>:
 8011696:	4b0b      	ldr	r3, [pc, #44]	; (80116c4 <MASS_Data_Setup+0x2e>)
 8011698:	681b      	ldr	r3, [r3, #0]
 801169a:	781a      	ldrb	r2, [r3, #0]
 801169c:	f002 027f 	and.w	r2, r2, #127	; 0x7f
 80116a0:	2a21      	cmp	r2, #33	; 0x21
 80116a2:	d10c      	bne.n	80116be <MASS_Data_Setup+0x28>
 80116a4:	28fe      	cmp	r0, #254	; 0xfe
 80116a6:	d10a      	bne.n	80116be <MASS_Data_Setup+0x28>
 80116a8:	8858      	ldrh	r0, [r3, #2]
 80116aa:	b940      	cbnz	r0, 80116be <MASS_Data_Setup+0x28>
 80116ac:	685a      	ldr	r2, [r3, #4]
 80116ae:	f5b2 3f80 	cmp.w	r2, #65536	; 0x10000
 80116b2:	d104      	bne.n	80116be <MASS_Data_Setup+0x28>
 80116b4:	4a04      	ldr	r2, [pc, #16]	; (80116c8 <MASS_Data_Setup+0x32>)
 80116b6:	619a      	str	r2, [r3, #24]
 80116b8:	2201      	movs	r2, #1
 80116ba:	611a      	str	r2, [r3, #16]
 80116bc:	4770      	bx	lr
 80116be:	2002      	movs	r0, #2
 80116c0:	4770      	bx	lr
 80116c2:	bf00      	nop
 80116c4:	20009504 	.word	0x20009504
 80116c8:	080114c3 	.word	0x080114c3

080116cc <Get_SerialNum>:
 80116cc:	4b0e      	ldr	r3, [pc, #56]	; (8011708 <Get_SerialNum+0x3c>)
 80116ce:	b510      	push	{r4, lr}
 80116d0:	681a      	ldr	r2, [r3, #0]
 80116d2:	6858      	ldr	r0, [r3, #4]
 80116d4:	6899      	ldr	r1, [r3, #8]
 80116d6:	b1aa      	cbz	r2, 8011704 <Get_SerialNum+0x38>
 80116d8:	4b0c      	ldr	r3, [pc, #48]	; (801170c <Get_SerialNum+0x40>)
 80116da:	0a14      	lsrs	r4, r2, #8
 80116dc:	709a      	strb	r2, [r3, #2]
 80116de:	711c      	strb	r4, [r3, #4]
 80116e0:	0c14      	lsrs	r4, r2, #16
 80116e2:	0e12      	lsrs	r2, r2, #24
 80116e4:	721a      	strb	r2, [r3, #8]
 80116e6:	0a02      	lsrs	r2, r0, #8
 80116e8:	731a      	strb	r2, [r3, #12]
 80116ea:	0c02      	lsrs	r2, r0, #16
 80116ec:	739a      	strb	r2, [r3, #14]
 80116ee:	0a0a      	lsrs	r2, r1, #8
 80116f0:	7298      	strb	r0, [r3, #10]
 80116f2:	7499      	strb	r1, [r3, #18]
 80116f4:	0e00      	lsrs	r0, r0, #24
 80116f6:	751a      	strb	r2, [r3, #20]
 80116f8:	0c0a      	lsrs	r2, r1, #16
 80116fa:	0e09      	lsrs	r1, r1, #24
 80116fc:	719c      	strb	r4, [r3, #6]
 80116fe:	7418      	strb	r0, [r3, #16]
 8011700:	759a      	strb	r2, [r3, #22]
 8011702:	7619      	strb	r1, [r3, #24]
 8011704:	bd10      	pop	{r4, pc}
 8011706:	bf00      	nop
 8011708:	1ffff7e8 	.word	0x1ffff7e8
 801170c:	200001ea 	.word	0x200001ea

08011710 <MASS_init>:
 8011710:	b510      	push	{r4, lr}
 8011712:	2400      	movs	r4, #0
 8011714:	f7ff ffda 	bl	80116cc <Get_SerialNum>
 8011718:	4b07      	ldr	r3, [pc, #28]	; (8011738 <MASS_init+0x28>)
 801171a:	681b      	ldr	r3, [r3, #0]
 801171c:	729c      	strb	r4, [r3, #10]
 801171e:	f7ff fe45 	bl	80113ac <PowerOn>
 8011722:	4b06      	ldr	r3, [pc, #24]	; (801173c <MASS_init+0x2c>)
 8011724:	4a06      	ldr	r2, [pc, #24]	; (8011740 <MASS_init+0x30>)
 8011726:	601c      	str	r4, [r3, #0]
 8011728:	f44f 4304 	mov.w	r3, #33792	; 0x8400
 801172c:	8013      	strh	r3, [r2, #0]
 801172e:	4a05      	ldr	r2, [pc, #20]	; (8011744 <MASS_init+0x34>)
 8011730:	6013      	str	r3, [r2, #0]
 8011732:	4b05      	ldr	r3, [pc, #20]	; (8011748 <MASS_init+0x38>)
 8011734:	601c      	str	r4, [r3, #0]
 8011736:	bd10      	pop	{r4, pc}
 8011738:	20009504 	.word	0x20009504
 801173c:	40005c44 	.word	0x40005c44
 8011740:	2000952c 	.word	0x2000952c
 8011744:	40005c40 	.word	0x40005c40
 8011748:	200095c4 	.word	0x200095c4

0801174c <Standard_GetConfiguration>:
 801174c:	b510      	push	{r4, lr}
 801174e:	4c06      	ldr	r4, [pc, #24]	; (8011768 <Standard_GetConfiguration+0x1c>)
 8011750:	b918      	cbnz	r0, 801175a <Standard_GetConfiguration+0xe>
 8011752:	2201      	movs	r2, #1
 8011754:	6823      	ldr	r3, [r4, #0]
 8011756:	821a      	strh	r2, [r3, #16]
 8011758:	bd10      	pop	{r4, pc}
 801175a:	4b04      	ldr	r3, [pc, #16]	; (801176c <Standard_GetConfiguration+0x20>)
 801175c:	681b      	ldr	r3, [r3, #0]
 801175e:	681b      	ldr	r3, [r3, #0]
 8011760:	4798      	blx	r3
 8011762:	6820      	ldr	r0, [r4, #0]
 8011764:	300a      	adds	r0, #10
 8011766:	e7f7      	b.n	8011758 <Standard_GetConfiguration+0xc>
 8011768:	20009504 	.word	0x20009504
 801176c:	20009528 	.word	0x20009528

08011770 <Standard_GetInterface>:
 8011770:	b510      	push	{r4, lr}
 8011772:	4c06      	ldr	r4, [pc, #24]	; (801178c <Standard_GetInterface+0x1c>)
 8011774:	b918      	cbnz	r0, 801177e <Standard_GetInterface+0xe>
 8011776:	2201      	movs	r2, #1
 8011778:	6823      	ldr	r3, [r4, #0]
 801177a:	821a      	strh	r2, [r3, #16]
 801177c:	bd10      	pop	{r4, pc}
 801177e:	4b04      	ldr	r3, [pc, #16]	; (8011790 <Standard_GetInterface+0x20>)
 8011780:	681b      	ldr	r3, [r3, #0]
 8011782:	689b      	ldr	r3, [r3, #8]
 8011784:	4798      	blx	r3
 8011786:	6820      	ldr	r0, [r4, #0]
 8011788:	300c      	adds	r0, #12
 801178a:	e7f7      	b.n	801177c <Standard_GetInterface+0xc>
 801178c:	20009504 	.word	0x20009504
 8011790:	20009528 	.word	0x20009528

08011794 <Standard_GetStatus>:
 8011794:	4b1e      	ldr	r3, [pc, #120]	; (8011810 <Standard_GetStatus+0x7c>)
 8011796:	b510      	push	{r4, lr}
 8011798:	681c      	ldr	r4, [r3, #0]
 801179a:	2300      	movs	r3, #0
 801179c:	b910      	cbnz	r0, 80117a4 <Standard_GetStatus+0x10>
 801179e:	2302      	movs	r3, #2
 80117a0:	8223      	strh	r3, [r4, #16]
 80117a2:	bd10      	pop	{r4, pc}
 80117a4:	481b      	ldr	r0, [pc, #108]	; (8011814 <Standard_GetStatus+0x80>)
 80117a6:	8003      	strh	r3, [r0, #0]
 80117a8:	7821      	ldrb	r1, [r4, #0]
 80117aa:	4602      	mov	r2, r0
 80117ac:	f011 017f 	ands.w	r1, r1, #127	; 0x7f
 80117b0:	d112      	bne.n	80117d8 <Standard_GetStatus+0x44>
 80117b2:	7a61      	ldrb	r1, [r4, #9]
 80117b4:	068c      	lsls	r4, r1, #26
 80117b6:	bf44      	itt	mi
 80117b8:	2302      	movmi	r3, #2
 80117ba:	7003      	strbmi	r3, [r0, #0]
 80117bc:	7803      	ldrb	r3, [r0, #0]
 80117be:	0648      	lsls	r0, r1, #25
 80117c0:	bf4c      	ite	mi
 80117c2:	f043 0301 	orrmi.w	r3, r3, #1
 80117c6:	f023 0301 	bicpl.w	r3, r3, #1
 80117ca:	7013      	strb	r3, [r2, #0]
 80117cc:	4b12      	ldr	r3, [pc, #72]	; (8011818 <Standard_GetStatus+0x84>)
 80117ce:	681b      	ldr	r3, [r3, #0]
 80117d0:	691b      	ldr	r3, [r3, #16]
 80117d2:	4798      	blx	r3
 80117d4:	480f      	ldr	r0, [pc, #60]	; (8011814 <Standard_GetStatus+0x80>)
 80117d6:	e7e4      	b.n	80117a2 <Standard_GetStatus+0xe>
 80117d8:	2901      	cmp	r1, #1
 80117da:	d0e2      	beq.n	80117a2 <Standard_GetStatus+0xe>
 80117dc:	2902      	cmp	r1, #2
 80117de:	d115      	bne.n	801180c <Standard_GetStatus+0x78>
 80117e0:	7961      	ldrb	r1, [r4, #5]
 80117e2:	f001 030f 	and.w	r3, r1, #15
 80117e6:	009b      	lsls	r3, r3, #2
 80117e8:	f103 4380 	add.w	r3, r3, #1073741824	; 0x40000000
 80117ec:	f503 43b8 	add.w	r3, r3, #23552	; 0x5c00
 80117f0:	0609      	lsls	r1, r1, #24
 80117f2:	681b      	ldr	r3, [r3, #0]
 80117f4:	d505      	bpl.n	8011802 <Standard_GetStatus+0x6e>
 80117f6:	f003 0330 	and.w	r3, r3, #48	; 0x30
 80117fa:	2b10      	cmp	r3, #16
 80117fc:	d1e6      	bne.n	80117cc <Standard_GetStatus+0x38>
 80117fe:	2301      	movs	r3, #1
 8011800:	e7e3      	b.n	80117ca <Standard_GetStatus+0x36>
 8011802:	f403 5340 	and.w	r3, r3, #12288	; 0x3000
 8011806:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 801180a:	e7f7      	b.n	80117fc <Standard_GetStatus+0x68>
 801180c:	4618      	mov	r0, r3
 801180e:	e7c8      	b.n	80117a2 <Standard_GetStatus+0xe>
 8011810:	20009504 	.word	0x20009504
 8011814:	200095cc 	.word	0x200095cc
 8011818:	20009528 	.word	0x20009528

0801181c <DataStageIn>:
 801181c:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
 8011820:	4f23      	ldr	r7, [pc, #140]	; (80118b0 <DataStageIn+0x94>)
 8011822:	f8df 80a0 	ldr.w	r8, [pc, #160]	; 80118c4 <DataStageIn+0xa8>
 8011826:	683d      	ldr	r5, [r7, #0]
 8011828:	8a2c      	ldrh	r4, [r5, #16]
 801182a:	b9d4      	cbnz	r4, 8011862 <DataStageIn+0x46>
 801182c:	7a2e      	ldrb	r6, [r5, #8]
 801182e:	2e04      	cmp	r6, #4
 8011830:	d117      	bne.n	8011862 <DataStageIn+0x46>
 8011832:	4920      	ldr	r1, [pc, #128]	; (80118b4 <DataStageIn+0x98>)
 8011834:	788b      	ldrb	r3, [r1, #2]
 8011836:	2b01      	cmp	r3, #1
 8011838:	d10e      	bne.n	8011858 <DataStageIn+0x3c>
 801183a:	4b1f      	ldr	r3, [pc, #124]	; (80118b8 <DataStageIn+0x9c>)
 801183c:	681a      	ldr	r2, [r3, #0]
 801183e:	4b1f      	ldr	r3, [pc, #124]	; (80118bc <DataStageIn+0xa0>)
 8011840:	b292      	uxth	r2, r2
 8011842:	4413      	add	r3, r2
 8011844:	005b      	lsls	r3, r3, #1
 8011846:	601c      	str	r4, [r3, #0]
 8011848:	2330      	movs	r3, #48	; 0x30
 801184a:	708c      	strb	r4, [r1, #2]
 801184c:	f8a8 3000 	strh.w	r3, [r8]
 8011850:	683b      	ldr	r3, [r7, #0]
 8011852:	721e      	strb	r6, [r3, #8]
 8011854:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
 8011858:	2310      	movs	r3, #16
 801185a:	2607      	movs	r6, #7
 801185c:	f8a8 3000 	strh.w	r3, [r8]
 8011860:	e7f6      	b.n	8011850 <DataStageIn+0x34>
 8011862:	8aab      	ldrh	r3, [r5, #20]
 8011864:	429c      	cmp	r4, r3
 8011866:	bf94      	ite	ls
 8011868:	2604      	movls	r6, #4
 801186a:	2602      	movhi	r6, #2
 801186c:	429c      	cmp	r4, r3
 801186e:	bf28      	it	cs
 8011870:	461c      	movcs	r4, r3
 8011872:	69ab      	ldr	r3, [r5, #24]
 8011874:	4620      	mov	r0, r4
 8011876:	4798      	blx	r3
 8011878:	4681      	mov	r9, r0
 801187a:	2000      	movs	r0, #0
 801187c:	f7ff fbd0 	bl	8011020 <GetEPTxAddr>
 8011880:	4622      	mov	r2, r4
 8011882:	4601      	mov	r1, r0
 8011884:	4648      	mov	r0, r9
 8011886:	f7ff fd6f 	bl	8011368 <UserToPMABufferCopy>
 801188a:	4621      	mov	r1, r4
 801188c:	2000      	movs	r0, #0
 801188e:	f7ff fbe3 	bl	8011058 <SetEPTxCount>
 8011892:	8a2b      	ldrh	r3, [r5, #16]
 8011894:	f44f 5240 	mov.w	r2, #12288	; 0x3000
 8011898:	1b1b      	subs	r3, r3, r4
 801189a:	822b      	strh	r3, [r5, #16]
 801189c:	8a6b      	ldrh	r3, [r5, #18]
 801189e:	441c      	add	r4, r3
 80118a0:	2330      	movs	r3, #48	; 0x30
 80118a2:	f8a8 3000 	strh.w	r3, [r8]
 80118a6:	4b06      	ldr	r3, [pc, #24]	; (80118c0 <DataStageIn+0xa4>)
 80118a8:	826c      	strh	r4, [r5, #18]
 80118aa:	801a      	strh	r2, [r3, #0]
 80118ac:	e7d0      	b.n	8011850 <DataStageIn+0x34>
 80118ae:	bf00      	nop
 80118b0:	20009504 	.word	0x20009504
 80118b4:	200095cc 	.word	0x200095cc
 80118b8:	40005c50 	.word	0x40005c50
 80118bc:	20003002 	.word	0x20003002
 80118c0:	200095d0 	.word	0x200095d0
 80118c4:	200095d2 	.word	0x200095d2

080118c8 <Standard_SetConfiguration>:
 80118c8:	b510      	push	{r4, lr}
 80118ca:	4b0a      	ldr	r3, [pc, #40]	; (80118f4 <Standard_SetConfiguration+0x2c>)
 80118cc:	490a      	ldr	r1, [pc, #40]	; (80118f8 <Standard_SetConfiguration+0x30>)
 80118ce:	681b      	ldr	r3, [r3, #0]
 80118d0:	7849      	ldrb	r1, [r1, #1]
 80118d2:	78da      	ldrb	r2, [r3, #3]
 80118d4:	4291      	cmp	r1, r2
 80118d6:	d30a      	bcc.n	80118ee <Standard_SetConfiguration+0x26>
 80118d8:	7899      	ldrb	r1, [r3, #2]
 80118da:	b941      	cbnz	r1, 80118ee <Standard_SetConfiguration+0x26>
 80118dc:	889c      	ldrh	r4, [r3, #4]
 80118de:	b934      	cbnz	r4, 80118ee <Standard_SetConfiguration+0x26>
 80118e0:	729a      	strb	r2, [r3, #10]
 80118e2:	4b06      	ldr	r3, [pc, #24]	; (80118fc <Standard_SetConfiguration+0x34>)
 80118e4:	681b      	ldr	r3, [r3, #0]
 80118e6:	685b      	ldr	r3, [r3, #4]
 80118e8:	4798      	blx	r3
 80118ea:	4620      	mov	r0, r4
 80118ec:	bd10      	pop	{r4, pc}
 80118ee:	2002      	movs	r0, #2
 80118f0:	e7fc      	b.n	80118ec <Standard_SetConfiguration+0x24>
 80118f2:	bf00      	nop
 80118f4:	20009504 	.word	0x20009504
 80118f8:	200001e8 	.word	0x200001e8
 80118fc:	20009528 	.word	0x20009528

08011900 <Standard_SetInterface>:
 8011900:	b538      	push	{r3, r4, r5, lr}
 8011902:	4a0f      	ldr	r2, [pc, #60]	; (8011940 <Standard_SetInterface+0x40>)
 8011904:	4d0f      	ldr	r5, [pc, #60]	; (8011944 <Standard_SetInterface+0x44>)
 8011906:	6812      	ldr	r2, [r2, #0]
 8011908:	682b      	ldr	r3, [r5, #0]
 801190a:	6992      	ldr	r2, [r2, #24]
 801190c:	78d9      	ldrb	r1, [r3, #3]
 801190e:	7958      	ldrb	r0, [r3, #5]
 8011910:	4790      	blx	r2
 8011912:	682b      	ldr	r3, [r5, #0]
 8011914:	4604      	mov	r4, r0
 8011916:	7a9a      	ldrb	r2, [r3, #10]
 8011918:	b17a      	cbz	r2, 801193a <Standard_SetInterface+0x3a>
 801191a:	b970      	cbnz	r0, 801193a <Standard_SetInterface+0x3a>
 801191c:	791a      	ldrb	r2, [r3, #4]
 801191e:	b962      	cbnz	r2, 801193a <Standard_SetInterface+0x3a>
 8011920:	789b      	ldrb	r3, [r3, #2]
 8011922:	b953      	cbnz	r3, 801193a <Standard_SetInterface+0x3a>
 8011924:	4b08      	ldr	r3, [pc, #32]	; (8011948 <Standard_SetInterface+0x48>)
 8011926:	681b      	ldr	r3, [r3, #0]
 8011928:	68db      	ldr	r3, [r3, #12]
 801192a:	4798      	blx	r3
 801192c:	682b      	ldr	r3, [r5, #0]
 801192e:	795a      	ldrb	r2, [r3, #5]
 8011930:	72da      	strb	r2, [r3, #11]
 8011932:	78da      	ldrb	r2, [r3, #3]
 8011934:	731a      	strb	r2, [r3, #12]
 8011936:	4620      	mov	r0, r4
 8011938:	bd38      	pop	{r3, r4, r5, pc}
 801193a:	2402      	movs	r4, #2
 801193c:	e7fb      	b.n	8011936 <Standard_SetInterface+0x36>
 801193e:	bf00      	nop
 8011940:	20009524 	.word	0x20009524
 8011944:	20009504 	.word	0x20009504
 8011948:	20009528 	.word	0x20009528

0801194c <Standard_ClearFeature>:
 801194c:	b538      	push	{r3, r4, r5, lr}
 801194e:	4b2d      	ldr	r3, [pc, #180]	; (8011a04 <Standard_ClearFeature+0xb8>)
 8011950:	681b      	ldr	r3, [r3, #0]
 8011952:	7818      	ldrb	r0, [r3, #0]
 8011954:	f010 007f 	ands.w	r0, r0, #127	; 0x7f
 8011958:	d104      	bne.n	8011964 <Standard_ClearFeature+0x18>
 801195a:	7a5a      	ldrb	r2, [r3, #9]
 801195c:	f022 0220 	bic.w	r2, r2, #32
 8011960:	725a      	strb	r2, [r3, #9]
 8011962:	bd38      	pop	{r3, r4, r5, pc}
 8011964:	2802      	cmp	r0, #2
 8011966:	d14b      	bne.n	8011a00 <Standard_ClearFeature+0xb4>
 8011968:	885a      	ldrh	r2, [r3, #2]
 801196a:	2a00      	cmp	r2, #0
 801196c:	d1f9      	bne.n	8011962 <Standard_ClearFeature+0x16>
 801196e:	791a      	ldrb	r2, [r3, #4]
 8011970:	2a00      	cmp	r2, #0
 8011972:	d1f6      	bne.n	8011962 <Standard_ClearFeature+0x16>
 8011974:	7959      	ldrb	r1, [r3, #5]
 8011976:	4d24      	ldr	r5, [pc, #144]	; (8011a08 <Standard_ClearFeature+0xbc>)
 8011978:	f021 0080 	bic.w	r0, r1, #128	; 0x80
 801197c:	0084      	lsls	r4, r0, #2
 801197e:	f104 4480 	add.w	r4, r4, #1073741824	; 0x40000000
 8011982:	f504 44b8 	add.w	r4, r4, #23552	; 0x5c00
 8011986:	6822      	ldr	r2, [r4, #0]
 8011988:	b249      	sxtb	r1, r1
 801198a:	782d      	ldrb	r5, [r5, #0]
 801198c:	2900      	cmp	r1, #0
 801198e:	bfb4      	ite	lt
 8011990:	f002 0230 	andlt.w	r2, r2, #48	; 0x30
 8011994:	f402 5240 	andge.w	r2, r2, #12288	; 0x3000
 8011998:	4285      	cmp	r5, r0
 801199a:	d931      	bls.n	8011a00 <Standard_ClearFeature+0xb4>
 801199c:	b382      	cbz	r2, 8011a00 <Standard_ClearFeature+0xb4>
 801199e:	7a9b      	ldrb	r3, [r3, #10]
 80119a0:	b373      	cbz	r3, 8011a00 <Standard_ClearFeature+0xb4>
 80119a2:	2900      	cmp	r1, #0
 80119a4:	6823      	ldr	r3, [r4, #0]
 80119a6:	da11      	bge.n	80119cc <Standard_ClearFeature+0x80>
 80119a8:	f003 0330 	and.w	r3, r3, #48	; 0x30
 80119ac:	2b10      	cmp	r3, #16
 80119ae:	d107      	bne.n	80119c0 <Standard_ClearFeature+0x74>
 80119b0:	b2c4      	uxtb	r4, r0
 80119b2:	4620      	mov	r0, r4
 80119b4:	f7ff fae7 	bl	8010f86 <ClearDTOG_TX>
 80119b8:	2130      	movs	r1, #48	; 0x30
 80119ba:	4620      	mov	r0, r4
 80119bc:	f7ff f9d8 	bl	8010d70 <SetEPTxStatus>
 80119c0:	4b12      	ldr	r3, [pc, #72]	; (8011a0c <Standard_ClearFeature+0xc0>)
 80119c2:	681b      	ldr	r3, [r3, #0]
 80119c4:	695b      	ldr	r3, [r3, #20]
 80119c6:	4798      	blx	r3
 80119c8:	2000      	movs	r0, #0
 80119ca:	e7ca      	b.n	8011962 <Standard_ClearFeature+0x16>
 80119cc:	f403 5340 	and.w	r3, r3, #12288	; 0x3000
 80119d0:	f5b3 5f80 	cmp.w	r3, #4096	; 0x1000
 80119d4:	d1f4      	bne.n	80119c0 <Standard_ClearFeature+0x74>
 80119d6:	4d0e      	ldr	r5, [pc, #56]	; (8011a10 <Standard_ClearFeature+0xc4>)
 80119d8:	b978      	cbnz	r0, 80119fa <Standard_ClearFeature+0xae>
 80119da:	4b0e      	ldr	r3, [pc, #56]	; (8011a14 <Standard_ClearFeature+0xc8>)
 80119dc:	f893 102c 	ldrb.w	r1, [r3, #44]	; 0x2c
 80119e0:	f7ff fb5b 	bl	801109a <SetEPRxCount>
 80119e4:	6823      	ldr	r3, [r4, #0]
 80119e6:	402b      	ands	r3, r5
 80119e8:	b29b      	uxth	r3, r3
 80119ea:	f483 5340 	eor.w	r3, r3, #12288	; 0x3000
 80119ee:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 80119f2:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 80119f6:	6023      	str	r3, [r4, #0]
 80119f8:	e7e2      	b.n	80119c0 <Standard_ClearFeature+0x74>
 80119fa:	f7ff fab1 	bl	8010f60 <ClearDTOG_RX>
 80119fe:	e7f1      	b.n	80119e4 <Standard_ClearFeature+0x98>
 8011a00:	2002      	movs	r0, #2
 8011a02:	e7ae      	b.n	8011962 <Standard_ClearFeature+0x16>
 8011a04:	20009504 	.word	0x20009504
 8011a08:	200001e8 	.word	0x200001e8
 8011a0c:	20009528 	.word	0x20009528
 8011a10:	ffffbf8f 	.word	0xffffbf8f
 8011a14:	2000015c 	.word	0x2000015c

08011a18 <Standard_SetEndPointFeature>:
 8011a18:	b538      	push	{r3, r4, r5, lr}
 8011a1a:	4b1d      	ldr	r3, [pc, #116]	; (8011a90 <Standard_SetEndPointFeature+0x78>)
 8011a1c:	4d1d      	ldr	r5, [pc, #116]	; (8011a94 <Standard_SetEndPointFeature+0x7c>)
 8011a1e:	6818      	ldr	r0, [r3, #0]
 8011a20:	782d      	ldrb	r5, [r5, #0]
 8011a22:	7941      	ldrb	r1, [r0, #5]
 8011a24:	f021 0480 	bic.w	r4, r1, #128	; 0x80
 8011a28:	00a2      	lsls	r2, r4, #2
 8011a2a:	f102 4280 	add.w	r2, r2, #1073741824	; 0x40000000
 8011a2e:	f502 42b8 	add.w	r2, r2, #23552	; 0x5c00
 8011a32:	6813      	ldr	r3, [r2, #0]
 8011a34:	b249      	sxtb	r1, r1
 8011a36:	2900      	cmp	r1, #0
 8011a38:	bfb4      	ite	lt
 8011a3a:	f003 0330 	andlt.w	r3, r3, #48	; 0x30
 8011a3e:	f403 5340 	andge.w	r3, r3, #12288	; 0x3000
 8011a42:	42a5      	cmp	r5, r4
 8011a44:	d921      	bls.n	8011a8a <Standard_SetEndPointFeature+0x72>
 8011a46:	8844      	ldrh	r4, [r0, #2]
 8011a48:	b9fc      	cbnz	r4, 8011a8a <Standard_SetEndPointFeature+0x72>
 8011a4a:	b1f3      	cbz	r3, 8011a8a <Standard_SetEndPointFeature+0x72>
 8011a4c:	7a83      	ldrb	r3, [r0, #10]
 8011a4e:	b1e3      	cbz	r3, 8011a8a <Standard_SetEndPointFeature+0x72>
 8011a50:	6813      	ldr	r3, [r2, #0]
 8011a52:	2900      	cmp	r1, #0
 8011a54:	bfb5      	itete	lt
 8011a56:	f423 43e0 	biclt.w	r3, r3, #28672	; 0x7000
 8011a5a:	f423 4380 	bicge.w	r3, r3, #16384	; 0x4000
 8011a5e:	f023 0340 	biclt.w	r3, r3, #64	; 0x40
 8011a62:	f023 0370 	bicge.w	r3, r3, #112	; 0x70
 8011a66:	bfb5      	itete	lt
 8011a68:	b29b      	uxthlt	r3, r3
 8011a6a:	b29b      	uxthge	r3, r3
 8011a6c:	f083 0310 	eorlt.w	r3, r3, #16
 8011a70:	f483 5380 	eorge.w	r3, r3, #4096	; 0x1000
 8011a74:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8011a78:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8011a7c:	6013      	str	r3, [r2, #0]
 8011a7e:	4b06      	ldr	r3, [pc, #24]	; (8011a98 <Standard_SetEndPointFeature+0x80>)
 8011a80:	681b      	ldr	r3, [r3, #0]
 8011a82:	699b      	ldr	r3, [r3, #24]
 8011a84:	4798      	blx	r3
 8011a86:	2000      	movs	r0, #0
 8011a88:	bd38      	pop	{r3, r4, r5, pc}
 8011a8a:	2002      	movs	r0, #2
 8011a8c:	e7fc      	b.n	8011a88 <Standard_SetEndPointFeature+0x70>
 8011a8e:	bf00      	nop
 8011a90:	20009504 	.word	0x20009504
 8011a94:	200001e8 	.word	0x200001e8
 8011a98:	20009528 	.word	0x20009528

08011a9c <Standard_SetDeviceFeature>:
 8011a9c:	b508      	push	{r3, lr}
 8011a9e:	4b06      	ldr	r3, [pc, #24]	; (8011ab8 <Standard_SetDeviceFeature+0x1c>)
 8011aa0:	681a      	ldr	r2, [r3, #0]
 8011aa2:	7a53      	ldrb	r3, [r2, #9]
 8011aa4:	f043 0320 	orr.w	r3, r3, #32
 8011aa8:	7253      	strb	r3, [r2, #9]
 8011aaa:	4b04      	ldr	r3, [pc, #16]	; (8011abc <Standard_SetDeviceFeature+0x20>)
 8011aac:	681b      	ldr	r3, [r3, #0]
 8011aae:	69db      	ldr	r3, [r3, #28]
 8011ab0:	4798      	blx	r3
 8011ab2:	2000      	movs	r0, #0
 8011ab4:	bd08      	pop	{r3, pc}
 8011ab6:	bf00      	nop
 8011ab8:	20009504 	.word	0x20009504
 8011abc:	20009528 	.word	0x20009528

08011ac0 <Standard_GetDescriptorData>:
 8011ac0:	4b05      	ldr	r3, [pc, #20]	; (8011ad8 <Standard_GetDescriptorData+0x18>)
 8011ac2:	b510      	push	{r4, lr}
 8011ac4:	681c      	ldr	r4, [r3, #0]
 8011ac6:	8a62      	ldrh	r2, [r4, #18]
 8011ac8:	b918      	cbnz	r0, 8011ad2 <Standard_GetDescriptorData+0x12>
 8011aca:	888b      	ldrh	r3, [r1, #4]
 8011acc:	1a9b      	subs	r3, r3, r2
 8011ace:	8223      	strh	r3, [r4, #16]
 8011ad0:	bd10      	pop	{r4, pc}
 8011ad2:	6808      	ldr	r0, [r1, #0]
 8011ad4:	4410      	add	r0, r2
 8011ad6:	e7fb      	b.n	8011ad0 <Standard_GetDescriptorData+0x10>
 8011ad8:	20009504 	.word	0x20009504

08011adc <Post0_Process>:
 8011adc:	b508      	push	{r3, lr}
 8011ade:	4b0b      	ldr	r3, [pc, #44]	; (8011b0c <Post0_Process+0x30>)
 8011ae0:	2000      	movs	r0, #0
 8011ae2:	f893 102c 	ldrb.w	r1, [r3, #44]	; 0x2c
 8011ae6:	f7ff fad8 	bl	801109a <SetEPRxCount>
 8011aea:	4b09      	ldr	r3, [pc, #36]	; (8011b10 <Post0_Process+0x34>)
 8011aec:	681b      	ldr	r3, [r3, #0]
 8011aee:	7a18      	ldrb	r0, [r3, #8]
 8011af0:	2808      	cmp	r0, #8
 8011af2:	d106      	bne.n	8011b02 <Post0_Process+0x26>
 8011af4:	f44f 5280 	mov.w	r2, #4096	; 0x1000
 8011af8:	4b06      	ldr	r3, [pc, #24]	; (8011b14 <Post0_Process+0x38>)
 8011afa:	801a      	strh	r2, [r3, #0]
 8011afc:	2210      	movs	r2, #16
 8011afe:	4b06      	ldr	r3, [pc, #24]	; (8011b18 <Post0_Process+0x3c>)
 8011b00:	801a      	strh	r2, [r3, #0]
 8011b02:	f1a0 0309 	sub.w	r3, r0, #9
 8011b06:	4258      	negs	r0, r3
 8011b08:	4158      	adcs	r0, r3
 8011b0a:	bd08      	pop	{r3, pc}
 8011b0c:	2000015c 	.word	0x2000015c
 8011b10:	20009504 	.word	0x20009504
 8011b14:	200095d0 	.word	0x200095d0
 8011b18:	200095d2 	.word	0x200095d2

08011b1c <Setup0_Process>:
 8011b1c:	b5f7      	push	{r0, r1, r2, r4, r5, r6, r7, lr}
 8011b1e:	4c96      	ldr	r4, [pc, #600]	; (8011d78 <Setup0_Process+0x25c>)
 8011b20:	4e96      	ldr	r6, [pc, #600]	; (8011d7c <Setup0_Process+0x260>)
 8011b22:	6827      	ldr	r7, [r4, #0]
 8011b24:	6832      	ldr	r2, [r6, #0]
 8011b26:	7a3b      	ldrb	r3, [r7, #8]
 8011b28:	2b09      	cmp	r3, #9
 8011b2a:	d019      	beq.n	8011b60 <Setup0_Process+0x44>
 8011b2c:	4b94      	ldr	r3, [pc, #592]	; (8011d80 <Setup0_Process+0x264>)
 8011b2e:	b292      	uxth	r2, r2
 8011b30:	4413      	add	r3, r2
 8011b32:	005b      	lsls	r3, r3, #1
 8011b34:	881d      	ldrh	r5, [r3, #0]
 8011b36:	006d      	lsls	r5, r5, #1
 8011b38:	f105 4580 	add.w	r5, r5, #1073741824	; 0x40000000
 8011b3c:	f505 45c0 	add.w	r5, r5, #24576	; 0x6000
 8011b40:	782b      	ldrb	r3, [r5, #0]
 8011b42:	703b      	strb	r3, [r7, #0]
 8011b44:	786b      	ldrb	r3, [r5, #1]
 8011b46:	707b      	strb	r3, [r7, #1]
 8011b48:	88a8      	ldrh	r0, [r5, #4]
 8011b4a:	f7ff fc07 	bl	801135c <ByteSwap>
 8011b4e:	8078      	strh	r0, [r7, #2]
 8011b50:	8928      	ldrh	r0, [r5, #8]
 8011b52:	6827      	ldr	r7, [r4, #0]
 8011b54:	f7ff fc02 	bl	801135c <ByteSwap>
 8011b58:	80b8      	strh	r0, [r7, #4]
 8011b5a:	89aa      	ldrh	r2, [r5, #12]
 8011b5c:	6823      	ldr	r3, [r4, #0]
 8011b5e:	80da      	strh	r2, [r3, #6]
 8011b60:	2201      	movs	r2, #1
 8011b62:	6823      	ldr	r3, [r4, #0]
 8011b64:	721a      	strb	r2, [r3, #8]
 8011b66:	88da      	ldrh	r2, [r3, #6]
 8011b68:	785d      	ldrb	r5, [r3, #1]
 8011b6a:	2a00      	cmp	r2, #0
 8011b6c:	d161      	bne.n	8011c32 <Setup0_Process+0x116>
 8011b6e:	781a      	ldrb	r2, [r3, #0]
 8011b70:	f012 027f 	ands.w	r2, r2, #127	; 0x7f
 8011b74:	d14a      	bne.n	8011c0c <Setup0_Process+0xf0>
 8011b76:	2d09      	cmp	r5, #9
 8011b78:	d103      	bne.n	8011b82 <Setup0_Process+0x66>
 8011b7a:	f7ff fea5 	bl	80118c8 <Standard_SetConfiguration>
 8011b7e:	bb40      	cbnz	r0, 8011bd2 <Setup0_Process+0xb6>
 8011b80:	e016      	b.n	8011bb0 <Setup0_Process+0x94>
 8011b82:	2d05      	cmp	r5, #5
 8011b84:	d120      	bne.n	8011bc8 <Setup0_Process+0xac>
 8011b86:	f993 2003 	ldrsb.w	r2, [r3, #3]
 8011b8a:	2a00      	cmp	r2, #0
 8011b8c:	da07      	bge.n	8011b9e <Setup0_Process+0x82>
 8011b8e:	2308      	movs	r3, #8
 8011b90:	6822      	ldr	r2, [r4, #0]
 8011b92:	7213      	strb	r3, [r2, #8]
 8011b94:	b003      	add	sp, #12
 8011b96:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
 8011b9a:	f7ff bf9f 	b.w	8011adc <Post0_Process>
 8011b9e:	789a      	ldrb	r2, [r3, #2]
 8011ba0:	2a00      	cmp	r2, #0
 8011ba2:	d1f4      	bne.n	8011b8e <Setup0_Process+0x72>
 8011ba4:	889a      	ldrh	r2, [r3, #4]
 8011ba6:	2a00      	cmp	r2, #0
 8011ba8:	d1f1      	bne.n	8011b8e <Setup0_Process+0x72>
 8011baa:	7a9b      	ldrb	r3, [r3, #10]
 8011bac:	2b00      	cmp	r3, #0
 8011bae:	d1ee      	bne.n	8011b8e <Setup0_Process+0x72>
 8011bb0:	6832      	ldr	r2, [r6, #0]
 8011bb2:	4b74      	ldr	r3, [pc, #464]	; (8011d84 <Setup0_Process+0x268>)
 8011bb4:	b292      	uxth	r2, r2
 8011bb6:	4413      	add	r3, r2
 8011bb8:	2200      	movs	r2, #0
 8011bba:	005b      	lsls	r3, r3, #1
 8011bbc:	601a      	str	r2, [r3, #0]
 8011bbe:	2230      	movs	r2, #48	; 0x30
 8011bc0:	4b71      	ldr	r3, [pc, #452]	; (8011d88 <Setup0_Process+0x26c>)
 8011bc2:	801a      	strh	r2, [r3, #0]
 8011bc4:	2306      	movs	r3, #6
 8011bc6:	e7e3      	b.n	8011b90 <Setup0_Process+0x74>
 8011bc8:	2d03      	cmp	r5, #3
 8011bca:	d111      	bne.n	8011bf0 <Setup0_Process+0xd4>
 8011bcc:	78da      	ldrb	r2, [r3, #3]
 8011bce:	2a01      	cmp	r2, #1
 8011bd0:	d008      	beq.n	8011be4 <Setup0_Process+0xc8>
 8011bd2:	4b6e      	ldr	r3, [pc, #440]	; (8011d8c <Setup0_Process+0x270>)
 8011bd4:	4628      	mov	r0, r5
 8011bd6:	681b      	ldr	r3, [r3, #0]
 8011bd8:	695b      	ldr	r3, [r3, #20]
 8011bda:	4798      	blx	r3
 8011bdc:	2803      	cmp	r0, #3
 8011bde:	d125      	bne.n	8011c2c <Setup0_Process+0x110>
 8011be0:	2309      	movs	r3, #9
 8011be2:	e7d5      	b.n	8011b90 <Setup0_Process+0x74>
 8011be4:	889b      	ldrh	r3, [r3, #4]
 8011be6:	2b00      	cmp	r3, #0
 8011be8:	d1f3      	bne.n	8011bd2 <Setup0_Process+0xb6>
 8011bea:	f7ff ff57 	bl	8011a9c <Standard_SetDeviceFeature>
 8011bee:	e7c6      	b.n	8011b7e <Setup0_Process+0x62>
 8011bf0:	2d01      	cmp	r5, #1
 8011bf2:	d1ee      	bne.n	8011bd2 <Setup0_Process+0xb6>
 8011bf4:	78da      	ldrb	r2, [r3, #3]
 8011bf6:	2a01      	cmp	r2, #1
 8011bf8:	d1eb      	bne.n	8011bd2 <Setup0_Process+0xb6>
 8011bfa:	889a      	ldrh	r2, [r3, #4]
 8011bfc:	2a00      	cmp	r2, #0
 8011bfe:	d1e8      	bne.n	8011bd2 <Setup0_Process+0xb6>
 8011c00:	7a5b      	ldrb	r3, [r3, #9]
 8011c02:	069f      	lsls	r7, r3, #26
 8011c04:	d5e5      	bpl.n	8011bd2 <Setup0_Process+0xb6>
 8011c06:	f7ff fea1 	bl	801194c <Standard_ClearFeature>
 8011c0a:	e7b8      	b.n	8011b7e <Setup0_Process+0x62>
 8011c0c:	2a01      	cmp	r2, #1
 8011c0e:	d104      	bne.n	8011c1a <Setup0_Process+0xfe>
 8011c10:	2d0b      	cmp	r5, #11
 8011c12:	d1de      	bne.n	8011bd2 <Setup0_Process+0xb6>
 8011c14:	f7ff fe74 	bl	8011900 <Standard_SetInterface>
 8011c18:	e7b1      	b.n	8011b7e <Setup0_Process+0x62>
 8011c1a:	2a02      	cmp	r2, #2
 8011c1c:	d1d9      	bne.n	8011bd2 <Setup0_Process+0xb6>
 8011c1e:	2d01      	cmp	r5, #1
 8011c20:	d0f1      	beq.n	8011c06 <Setup0_Process+0xea>
 8011c22:	2d03      	cmp	r5, #3
 8011c24:	d1d5      	bne.n	8011bd2 <Setup0_Process+0xb6>
 8011c26:	f7ff fef7 	bl	8011a18 <Standard_SetEndPointFeature>
 8011c2a:	e7a8      	b.n	8011b7e <Setup0_Process+0x62>
 8011c2c:	2800      	cmp	r0, #0
 8011c2e:	d1ae      	bne.n	8011b8e <Setup0_Process+0x72>
 8011c30:	e7be      	b.n	8011bb0 <Setup0_Process+0x94>
 8011c32:	2d06      	cmp	r5, #6
 8011c34:	d112      	bne.n	8011c5c <Setup0_Process+0x140>
 8011c36:	781a      	ldrb	r2, [r3, #0]
 8011c38:	0655      	lsls	r5, r2, #25
 8011c3a:	d15a      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011c3c:	789b      	ldrb	r3, [r3, #2]
 8011c3e:	4a53      	ldr	r2, [pc, #332]	; (8011d8c <Setup0_Process+0x270>)
 8011c40:	2b01      	cmp	r3, #1
 8011c42:	6812      	ldr	r2, [r2, #0]
 8011c44:	d102      	bne.n	8011c4c <Setup0_Process+0x130>
 8011c46:	69d3      	ldr	r3, [r2, #28]
 8011c48:	b9db      	cbnz	r3, 8011c82 <Setup0_Process+0x166>
 8011c4a:	e052      	b.n	8011cf2 <Setup0_Process+0x1d6>
 8011c4c:	2b02      	cmp	r3, #2
 8011c4e:	d101      	bne.n	8011c54 <Setup0_Process+0x138>
 8011c50:	6a13      	ldr	r3, [r2, #32]
 8011c52:	e7f9      	b.n	8011c48 <Setup0_Process+0x12c>
 8011c54:	2b03      	cmp	r3, #3
 8011c56:	d14c      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011c58:	6a53      	ldr	r3, [r2, #36]	; 0x24
 8011c5a:	e7f5      	b.n	8011c48 <Setup0_Process+0x12c>
 8011c5c:	2d00      	cmp	r5, #0
 8011c5e:	d154      	bne.n	8011d0a <Setup0_Process+0x1ee>
 8011c60:	8859      	ldrh	r1, [r3, #2]
 8011c62:	2900      	cmp	r1, #0
 8011c64:	d145      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011c66:	685a      	ldr	r2, [r3, #4]
 8011c68:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
 8011c6c:	f5b2 3f00 	cmp.w	r2, #131072	; 0x20000
 8011c70:	d13f      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011c72:	781a      	ldrb	r2, [r3, #0]
 8011c74:	f012 027f 	ands.w	r2, r2, #127	; 0x7f
 8011c78:	d116      	bne.n	8011ca8 <Setup0_Process+0x18c>
 8011c7a:	889b      	ldrh	r3, [r3, #4]
 8011c7c:	2b00      	cmp	r3, #0
 8011c7e:	d138      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011c80:	4b43      	ldr	r3, [pc, #268]	; (8011d90 <Setup0_Process+0x274>)
 8011c82:	2500      	movs	r5, #0
 8011c84:	6822      	ldr	r2, [r4, #0]
 8011c86:	4628      	mov	r0, r5
 8011c88:	8255      	strh	r5, [r2, #18]
 8011c8a:	6193      	str	r3, [r2, #24]
 8011c8c:	4798      	blx	r3
 8011c8e:	4628      	mov	r0, r5
 8011c90:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8011c94:	6823      	ldr	r3, [r4, #0]
 8011c96:	8a1a      	ldrh	r2, [r3, #16]
 8011c98:	428a      	cmp	r2, r1
 8011c9a:	d033      	beq.n	8011d04 <Setup0_Process+0x1e8>
 8011c9c:	2802      	cmp	r0, #2
 8011c9e:	d001      	beq.n	8011ca4 <Setup0_Process+0x188>
 8011ca0:	2a00      	cmp	r2, #0
 8011ca2:	d155      	bne.n	8011d50 <Setup0_Process+0x234>
 8011ca4:	2208      	movs	r2, #8
 8011ca6:	e02e      	b.n	8011d06 <Setup0_Process+0x1ea>
 8011ca8:	2a01      	cmp	r2, #1
 8011caa:	d10a      	bne.n	8011cc2 <Setup0_Process+0x1a6>
 8011cac:	4a37      	ldr	r2, [pc, #220]	; (8011d8c <Setup0_Process+0x270>)
 8011cae:	7958      	ldrb	r0, [r3, #5]
 8011cb0:	6812      	ldr	r2, [r2, #0]
 8011cb2:	6992      	ldr	r2, [r2, #24]
 8011cb4:	4790      	blx	r2
 8011cb6:	b9e0      	cbnz	r0, 8011cf2 <Setup0_Process+0x1d6>
 8011cb8:	6823      	ldr	r3, [r4, #0]
 8011cba:	7a9b      	ldrb	r3, [r3, #10]
 8011cbc:	2b00      	cmp	r3, #0
 8011cbe:	d1df      	bne.n	8011c80 <Setup0_Process+0x164>
 8011cc0:	e017      	b.n	8011cf2 <Setup0_Process+0x1d6>
 8011cc2:	2a02      	cmp	r2, #2
 8011cc4:	d115      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011cc6:	795a      	ldrb	r2, [r3, #5]
 8011cc8:	f002 010f 	and.w	r1, r2, #15
 8011ccc:	008b      	lsls	r3, r1, #2
 8011cce:	0610      	lsls	r0, r2, #24
 8011cd0:	f103 4380 	add.w	r3, r3, #1073741824	; 0x40000000
 8011cd4:	482f      	ldr	r0, [pc, #188]	; (8011d94 <Setup0_Process+0x278>)
 8011cd6:	f503 43b8 	add.w	r3, r3, #23552	; 0x5c00
 8011cda:	681b      	ldr	r3, [r3, #0]
 8011cdc:	7800      	ldrb	r0, [r0, #0]
 8011cde:	bf4c      	ite	mi
 8011ce0:	f003 0330 	andmi.w	r3, r3, #48	; 0x30
 8011ce4:	f403 5340 	andpl.w	r3, r3, #12288	; 0x3000
 8011ce8:	4288      	cmp	r0, r1
 8011cea:	d902      	bls.n	8011cf2 <Setup0_Process+0x1d6>
 8011cec:	f012 0f70 	tst.w	r2, #112	; 0x70
 8011cf0:	d0e4      	beq.n	8011cbc <Setup0_Process+0x1a0>
 8011cf2:	4b26      	ldr	r3, [pc, #152]	; (8011d8c <Setup0_Process+0x270>)
 8011cf4:	6822      	ldr	r2, [r4, #0]
 8011cf6:	681b      	ldr	r3, [r3, #0]
 8011cf8:	7850      	ldrb	r0, [r2, #1]
 8011cfa:	691b      	ldr	r3, [r3, #16]
 8011cfc:	4798      	blx	r3
 8011cfe:	2803      	cmp	r0, #3
 8011d00:	d1c6      	bne.n	8011c90 <Setup0_Process+0x174>
 8011d02:	6823      	ldr	r3, [r4, #0]
 8011d04:	2209      	movs	r2, #9
 8011d06:	721a      	strb	r2, [r3, #8]
 8011d08:	e744      	b.n	8011b94 <Setup0_Process+0x78>
 8011d0a:	2d08      	cmp	r5, #8
 8011d0c:	d104      	bne.n	8011d18 <Setup0_Process+0x1fc>
 8011d0e:	781b      	ldrb	r3, [r3, #0]
 8011d10:	065b      	lsls	r3, r3, #25
 8011d12:	d1ee      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011d14:	4b20      	ldr	r3, [pc, #128]	; (8011d98 <Setup0_Process+0x27c>)
 8011d16:	e7b4      	b.n	8011c82 <Setup0_Process+0x166>
 8011d18:	2d0a      	cmp	r5, #10
 8011d1a:	d1ea      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011d1c:	781a      	ldrb	r2, [r3, #0]
 8011d1e:	f002 027f 	and.w	r2, r2, #127	; 0x7f
 8011d22:	2a01      	cmp	r2, #1
 8011d24:	d1e5      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011d26:	7a9a      	ldrb	r2, [r3, #10]
 8011d28:	2a00      	cmp	r2, #0
 8011d2a:	d0e2      	beq.n	8011cf2 <Setup0_Process+0x1d6>
 8011d2c:	8859      	ldrh	r1, [r3, #2]
 8011d2e:	2900      	cmp	r1, #0
 8011d30:	d1df      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011d32:	685a      	ldr	r2, [r3, #4]
 8011d34:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
 8011d38:	f5b2 3f80 	cmp.w	r2, #65536	; 0x10000
 8011d3c:	d1d9      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011d3e:	4a13      	ldr	r2, [pc, #76]	; (8011d8c <Setup0_Process+0x270>)
 8011d40:	7958      	ldrb	r0, [r3, #5]
 8011d42:	6812      	ldr	r2, [r2, #0]
 8011d44:	6992      	ldr	r2, [r2, #24]
 8011d46:	4790      	blx	r2
 8011d48:	2800      	cmp	r0, #0
 8011d4a:	d1d2      	bne.n	8011cf2 <Setup0_Process+0x1d6>
 8011d4c:	4b13      	ldr	r3, [pc, #76]	; (8011d9c <Setup0_Process+0x280>)
 8011d4e:	e798      	b.n	8011c82 <Setup0_Process+0x166>
 8011d50:	f993 1000 	ldrsb.w	r1, [r3]
 8011d54:	2900      	cmp	r1, #0
 8011d56:	da34      	bge.n	8011dc2 <Setup0_Process+0x2a6>
 8011d58:	88d9      	ldrh	r1, [r3, #6]
 8011d5a:	480c      	ldr	r0, [pc, #48]	; (8011d8c <Setup0_Process+0x270>)
 8011d5c:	9101      	str	r1, [sp, #4]
 8011d5e:	6800      	ldr	r0, [r0, #0]
 8011d60:	9d01      	ldr	r5, [sp, #4]
 8011d62:	f890 002c 	ldrb.w	r0, [r0, #44]	; 0x2c
 8011d66:	42aa      	cmp	r2, r5
 8011d68:	b284      	uxth	r4, r0
 8011d6a:	d919      	bls.n	8011da0 <Setup0_Process+0x284>
 8011d6c:	9a01      	ldr	r2, [sp, #4]
 8011d6e:	821a      	strh	r2, [r3, #16]
 8011d70:	829c      	strh	r4, [r3, #20]
 8011d72:	f7ff fd53 	bl	801181c <DataStageIn>
 8011d76:	e70d      	b.n	8011b94 <Setup0_Process+0x78>
 8011d78:	20009504 	.word	0x20009504
 8011d7c:	40005c50 	.word	0x40005c50
 8011d80:	20003004 	.word	0x20003004
 8011d84:	20003002 	.word	0x20003002
 8011d88:	200095d2 	.word	0x200095d2
 8011d8c:	20009524 	.word	0x20009524
 8011d90:	08011795 	.word	0x08011795
 8011d94:	200001e8 	.word	0x200001e8
 8011d98:	0801174d 	.word	0x0801174d
 8011d9c:	08011771 	.word	0x08011771
 8011da0:	428a      	cmp	r2, r1
 8011da2:	d2e5      	bcs.n	8011d70 <Setup0_Process+0x254>
 8011da4:	4294      	cmp	r4, r2
 8011da6:	d903      	bls.n	8011db0 <Setup0_Process+0x294>
 8011da8:	2100      	movs	r1, #0
 8011daa:	4a09      	ldr	r2, [pc, #36]	; (8011dd0 <Setup0_Process+0x2b4>)
 8011dac:	7091      	strb	r1, [r2, #2]
 8011dae:	e7df      	b.n	8011d70 <Setup0_Process+0x254>
 8011db0:	fb92 f1f0 	sdiv	r1, r2, r0
 8011db4:	fb00 2211 	mls	r2, r0, r1, r2
 8011db8:	2a00      	cmp	r2, #0
 8011dba:	d1d9      	bne.n	8011d70 <Setup0_Process+0x254>
 8011dbc:	4a04      	ldr	r2, [pc, #16]	; (8011dd0 <Setup0_Process+0x2b4>)
 8011dbe:	2101      	movs	r1, #1
 8011dc0:	e7f4      	b.n	8011dac <Setup0_Process+0x290>
 8011dc2:	2203      	movs	r2, #3
 8011dc4:	721a      	strb	r2, [r3, #8]
 8011dc6:	f44f 5240 	mov.w	r2, #12288	; 0x3000
 8011dca:	4b02      	ldr	r3, [pc, #8]	; (8011dd4 <Setup0_Process+0x2b8>)
 8011dcc:	801a      	strh	r2, [r3, #0]
 8011dce:	e6e1      	b.n	8011b94 <Setup0_Process+0x78>
 8011dd0:	200095cc 	.word	0x200095cc
 8011dd4:	200095d0 	.word	0x200095d0

08011dd8 <Out0_Process>:
 8011dd8:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8011dda:	4e2b      	ldr	r6, [pc, #172]	; (8011e88 <Out0_Process+0xb0>)
 8011ddc:	6834      	ldr	r4, [r6, #0]
 8011dde:	7a23      	ldrb	r3, [r4, #8]
 8011de0:	2b02      	cmp	r3, #2
 8011de2:	d04f      	beq.n	8011e84 <Out0_Process+0xac>
 8011de4:	2b04      	cmp	r3, #4
 8011de6:	d04d      	beq.n	8011e84 <Out0_Process+0xac>
 8011de8:	2b03      	cmp	r3, #3
 8011dea:	d001      	beq.n	8011df0 <Out0_Process+0x18>
 8011dec:	2b05      	cmp	r3, #5
 8011dee:	d143      	bne.n	8011e78 <Out0_Process+0xa0>
 8011df0:	69a3      	ldr	r3, [r4, #24]
 8011df2:	b1b3      	cbz	r3, 8011e22 <Out0_Process+0x4a>
 8011df4:	8a25      	ldrh	r5, [r4, #16]
 8011df6:	b1a5      	cbz	r5, 8011e22 <Out0_Process+0x4a>
 8011df8:	8aa2      	ldrh	r2, [r4, #20]
 8011dfa:	4295      	cmp	r5, r2
 8011dfc:	bf28      	it	cs
 8011dfe:	4615      	movcs	r5, r2
 8011e00:	4628      	mov	r0, r5
 8011e02:	4798      	blx	r3
 8011e04:	8a23      	ldrh	r3, [r4, #16]
 8011e06:	4607      	mov	r7, r0
 8011e08:	1b5b      	subs	r3, r3, r5
 8011e0a:	8223      	strh	r3, [r4, #16]
 8011e0c:	8a63      	ldrh	r3, [r4, #18]
 8011e0e:	2000      	movs	r0, #0
 8011e10:	442b      	add	r3, r5
 8011e12:	8263      	strh	r3, [r4, #18]
 8011e14:	f7ff f912 	bl	801103c <GetEPRxAddr>
 8011e18:	462a      	mov	r2, r5
 8011e1a:	4601      	mov	r1, r0
 8011e1c:	4638      	mov	r0, r7
 8011e1e:	f7ff fab2 	bl	8011386 <PMAToUserBufferCopy>
 8011e22:	8a23      	ldrh	r3, [r4, #16]
 8011e24:	b153      	cbz	r3, 8011e3c <Out0_Process+0x64>
 8011e26:	2100      	movs	r1, #0
 8011e28:	f44f 5240 	mov.w	r2, #12288	; 0x3000
 8011e2c:	4b17      	ldr	r3, [pc, #92]	; (8011e8c <Out0_Process+0xb4>)
 8011e2e:	4608      	mov	r0, r1
 8011e30:	801a      	strh	r2, [r3, #0]
 8011e32:	f7ff f911 	bl	8011058 <SetEPTxCount>
 8011e36:	2230      	movs	r2, #48	; 0x30
 8011e38:	4b15      	ldr	r3, [pc, #84]	; (8011e90 <Out0_Process+0xb8>)
 8011e3a:	801a      	strh	r2, [r3, #0]
 8011e3c:	8a21      	ldrh	r1, [r4, #16]
 8011e3e:	8aa3      	ldrh	r3, [r4, #20]
 8011e40:	6832      	ldr	r2, [r6, #0]
 8011e42:	428b      	cmp	r3, r1
 8011e44:	d808      	bhi.n	8011e58 <Out0_Process+0x80>
 8011e46:	2303      	movs	r3, #3
 8011e48:	7213      	strb	r3, [r2, #8]
 8011e4a:	7a13      	ldrb	r3, [r2, #8]
 8011e4c:	6832      	ldr	r2, [r6, #0]
 8011e4e:	7213      	strb	r3, [r2, #8]
 8011e50:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
 8011e54:	f7ff be42 	b.w	8011adc <Post0_Process>
 8011e58:	b109      	cbz	r1, 8011e5e <Out0_Process+0x86>
 8011e5a:	2305      	movs	r3, #5
 8011e5c:	e7f4      	b.n	8011e48 <Out0_Process+0x70>
 8011e5e:	2306      	movs	r3, #6
 8011e60:	7213      	strb	r3, [r2, #8]
 8011e62:	4b0c      	ldr	r3, [pc, #48]	; (8011e94 <Out0_Process+0xbc>)
 8011e64:	6818      	ldr	r0, [r3, #0]
 8011e66:	4b0c      	ldr	r3, [pc, #48]	; (8011e98 <Out0_Process+0xc0>)
 8011e68:	b280      	uxth	r0, r0
 8011e6a:	4403      	add	r3, r0
 8011e6c:	005b      	lsls	r3, r3, #1
 8011e6e:	6019      	str	r1, [r3, #0]
 8011e70:	2130      	movs	r1, #48	; 0x30
 8011e72:	4b07      	ldr	r3, [pc, #28]	; (8011e90 <Out0_Process+0xb8>)
 8011e74:	8019      	strh	r1, [r3, #0]
 8011e76:	e7e8      	b.n	8011e4a <Out0_Process+0x72>
 8011e78:	2b07      	cmp	r3, #7
 8011e7a:	d103      	bne.n	8011e84 <Out0_Process+0xac>
 8011e7c:	4b07      	ldr	r3, [pc, #28]	; (8011e9c <Out0_Process+0xc4>)
 8011e7e:	681b      	ldr	r3, [r3, #0]
 8011e80:	68db      	ldr	r3, [r3, #12]
 8011e82:	4798      	blx	r3
 8011e84:	2308      	movs	r3, #8
 8011e86:	e7e1      	b.n	8011e4c <Out0_Process+0x74>
 8011e88:	20009504 	.word	0x20009504
 8011e8c:	200095d0 	.word	0x200095d0
 8011e90:	200095d2 	.word	0x200095d2
 8011e94:	40005c50 	.word	0x40005c50
 8011e98:	20003002 	.word	0x20003002
 8011e9c:	20009524 	.word	0x20009524

08011ea0 <SetDeviceAddress>:
 8011ea0:	b5f0      	push	{r4, r5, r6, r7, lr}
 8011ea2:	2200      	movs	r2, #0
 8011ea4:	f640 750f 	movw	r5, #3855	; 0xf0f
 8011ea8:	f248 0680 	movw	r6, #32896	; 0x8080
 8011eac:	4b0a      	ldr	r3, [pc, #40]	; (8011ed8 <SetDeviceAddress+0x38>)
 8011eae:	781c      	ldrb	r4, [r3, #0]
 8011eb0:	42a2      	cmp	r2, r4
 8011eb2:	d304      	bcc.n	8011ebe <SetDeviceAddress+0x1e>
 8011eb4:	4b09      	ldr	r3, [pc, #36]	; (8011edc <SetDeviceAddress+0x3c>)
 8011eb6:	f040 0080 	orr.w	r0, r0, #128	; 0x80
 8011eba:	6018      	str	r0, [r3, #0]
 8011ebc:	bdf0      	pop	{r4, r5, r6, r7, pc}
 8011ebe:	f102 5380 	add.w	r3, r2, #268435456	; 0x10000000
 8011ec2:	f503 53b8 	add.w	r3, r3, #5888	; 0x1700
 8011ec6:	009b      	lsls	r3, r3, #2
 8011ec8:	6819      	ldr	r1, [r3, #0]
 8011eca:	ea42 0706 	orr.w	r7, r2, r6
 8011ece:	4029      	ands	r1, r5
 8011ed0:	4339      	orrs	r1, r7
 8011ed2:	6019      	str	r1, [r3, #0]
 8011ed4:	3201      	adds	r2, #1
 8011ed6:	e7eb      	b.n	8011eb0 <SetDeviceAddress+0x10>
 8011ed8:	200001e8 	.word	0x200001e8
 8011edc:	40005c4c 	.word	0x40005c4c

08011ee0 <In0_Process>:
 8011ee0:	b510      	push	{r4, lr}
 8011ee2:	4c13      	ldr	r4, [pc, #76]	; (8011f30 <In0_Process+0x50>)
 8011ee4:	6823      	ldr	r3, [r4, #0]
 8011ee6:	7a1a      	ldrb	r2, [r3, #8]
 8011ee8:	2a02      	cmp	r2, #2
 8011eea:	d001      	beq.n	8011ef0 <In0_Process+0x10>
 8011eec:	2a04      	cmp	r2, #4
 8011eee:	d109      	bne.n	8011f04 <In0_Process+0x24>
 8011ef0:	f7ff fc94 	bl	801181c <DataStageIn>
 8011ef4:	6823      	ldr	r3, [r4, #0]
 8011ef6:	7a1b      	ldrb	r3, [r3, #8]
 8011ef8:	6822      	ldr	r2, [r4, #0]
 8011efa:	7213      	strb	r3, [r2, #8]
 8011efc:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8011f00:	f7ff bdec 	b.w	8011adc <Post0_Process>
 8011f04:	2a06      	cmp	r2, #6
 8011f06:	d110      	bne.n	8011f2a <In0_Process+0x4a>
 8011f08:	785a      	ldrb	r2, [r3, #1]
 8011f0a:	2a05      	cmp	r2, #5
 8011f0c:	d109      	bne.n	8011f22 <In0_Process+0x42>
 8011f0e:	781a      	ldrb	r2, [r3, #0]
 8011f10:	0652      	lsls	r2, r2, #25
 8011f12:	d106      	bne.n	8011f22 <In0_Process+0x42>
 8011f14:	78d8      	ldrb	r0, [r3, #3]
 8011f16:	f7ff ffc3 	bl	8011ea0 <SetDeviceAddress>
 8011f1a:	4b06      	ldr	r3, [pc, #24]	; (8011f34 <In0_Process+0x54>)
 8011f1c:	681b      	ldr	r3, [r3, #0]
 8011f1e:	6a1b      	ldr	r3, [r3, #32]
 8011f20:	4798      	blx	r3
 8011f22:	4b05      	ldr	r3, [pc, #20]	; (8011f38 <In0_Process+0x58>)
 8011f24:	681b      	ldr	r3, [r3, #0]
 8011f26:	689b      	ldr	r3, [r3, #8]
 8011f28:	4798      	blx	r3
 8011f2a:	2308      	movs	r3, #8
 8011f2c:	e7e4      	b.n	8011ef8 <In0_Process+0x18>
 8011f2e:	bf00      	nop
 8011f30:	20009504 	.word	0x20009504
 8011f34:	20009528 	.word	0x20009528
 8011f38:	20009524 	.word	0x20009524

08011f3c <NOP_Process>:
 8011f3c:	4770      	bx	lr
 8011f3e:	bf00      	nop

08011f40 <CTR_LP>:
 8011f40:	2300      	movs	r3, #0
 8011f42:	e92d 41f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, lr}
 8011f46:	4f58      	ldr	r7, [pc, #352]	; (80120a8 <CTR_LP+0x168>)
 8011f48:	4e58      	ldr	r6, [pc, #352]	; (80120ac <CTR_LP+0x16c>)
 8011f4a:	f8ad 3006 	strh.w	r3, [sp, #6]
 8011f4e:	683b      	ldr	r3, [r7, #0]
 8011f50:	4a57      	ldr	r2, [pc, #348]	; (80120b0 <CTR_LP+0x170>)
 8011f52:	b299      	uxth	r1, r3
 8011f54:	041b      	lsls	r3, r3, #16
 8011f56:	8011      	strh	r1, [r2, #0]
 8011f58:	d551      	bpl.n	8011ffe <CTR_LP+0xbe>
 8011f5a:	8813      	ldrh	r3, [r2, #0]
 8011f5c:	f003 030f 	and.w	r3, r3, #15
 8011f60:	7033      	strb	r3, [r6, #0]
 8011f62:	2b00      	cmp	r3, #0
 8011f64:	d16f      	bne.n	8012046 <CTR_LP+0x106>
 8011f66:	f64b 75bf 	movw	r5, #49087	; 0xbfbf
 8011f6a:	f8df 814c 	ldr.w	r8, [pc, #332]	; 80120b8 <CTR_LP+0x178>
 8011f6e:	4c51      	ldr	r4, [pc, #324]	; (80120b4 <CTR_LP+0x174>)
 8011f70:	f8d8 3000 	ldr.w	r3, [r8]
 8011f74:	b29b      	uxth	r3, r3
 8011f76:	8023      	strh	r3, [r4, #0]
 8011f78:	8823      	ldrh	r3, [r4, #0]
 8011f7a:	f003 0330 	and.w	r3, r3, #48	; 0x30
 8011f7e:	8063      	strh	r3, [r4, #2]
 8011f80:	8823      	ldrh	r3, [r4, #0]
 8011f82:	f403 5340 	and.w	r3, r3, #12288	; 0x3000
 8011f86:	8023      	strh	r3, [r4, #0]
 8011f88:	f8d8 3000 	ldr.w	r3, [r8]
 8011f8c:	402b      	ands	r3, r5
 8011f8e:	f483 5300 	eor.w	r3, r3, #8192	; 0x2000
 8011f92:	f083 0320 	eor.w	r3, r3, #32
 8011f96:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8011f9a:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8011f9e:	f8c8 3000 	str.w	r3, [r8]
 8011fa2:	8813      	ldrh	r3, [r2, #0]
 8011fa4:	f013 0f10 	tst.w	r3, #16
 8011fa8:	f8d8 3000 	ldr.w	r3, [r8]
 8011fac:	d12a      	bne.n	8012004 <CTR_LP+0xc4>
 8011fae:	f423 43e1 	bic.w	r3, r3, #28800	; 0x7080
 8011fb2:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8011fb6:	041b      	lsls	r3, r3, #16
 8011fb8:	0c1b      	lsrs	r3, r3, #16
 8011fba:	f8c8 3000 	str.w	r3, [r8]
 8011fbe:	f7ff ff8f 	bl	8011ee0 <In0_Process>
 8011fc2:	f8d8 3000 	ldr.w	r3, [r8]
 8011fc6:	8822      	ldrh	r2, [r4, #0]
 8011fc8:	402b      	ands	r3, r5
 8011fca:	04d7      	lsls	r7, r2, #19
 8011fcc:	8822      	ldrh	r2, [r4, #0]
 8011fce:	bf48      	it	mi
 8011fd0:	f483 5380 	eormi.w	r3, r3, #4096	; 0x1000
 8011fd4:	0496      	lsls	r6, r2, #18
 8011fd6:	8862      	ldrh	r2, [r4, #2]
 8011fd8:	bf48      	it	mi
 8011fda:	f483 5300 	eormi.w	r3, r3, #8192	; 0x2000
 8011fde:	06d5      	lsls	r5, r2, #27
 8011fe0:	8862      	ldrh	r2, [r4, #2]
 8011fe2:	bf48      	it	mi
 8011fe4:	f083 0310 	eormi.w	r3, r3, #16
 8011fe8:	0690      	lsls	r0, r2, #26
 8011fea:	bf48      	it	mi
 8011fec:	f083 0320 	eormi.w	r3, r3, #32
 8011ff0:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 8011ff4:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 8011ff8:	4a2f      	ldr	r2, [pc, #188]	; (80120b8 <CTR_LP+0x178>)
 8011ffa:	b29b      	uxth	r3, r3
 8011ffc:	6013      	str	r3, [r2, #0]
 8011ffe:	b002      	add	sp, #8
 8012000:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8012004:	b29b      	uxth	r3, r3
 8012006:	f8ad 3006 	strh.w	r3, [sp, #6]
 801200a:	f8bd 3006 	ldrh.w	r3, [sp, #6]
 801200e:	051a      	lsls	r2, r3, #20
 8012010:	d50a      	bpl.n	8012028 <CTR_LP+0xe8>
 8012012:	f8d8 3000 	ldr.w	r3, [r8]
 8012016:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 801201a:	051b      	lsls	r3, r3, #20
 801201c:	0d1b      	lsrs	r3, r3, #20
 801201e:	f8c8 3000 	str.w	r3, [r8]
 8012022:	f7ff fd7b 	bl	8011b1c <Setup0_Process>
 8012026:	e7cc      	b.n	8011fc2 <CTR_LP+0x82>
 8012028:	f8bd 3006 	ldrh.w	r3, [sp, #6]
 801202c:	041b      	lsls	r3, r3, #16
 801202e:	d58e      	bpl.n	8011f4e <CTR_LP+0xe>
 8012030:	f8d8 3000 	ldr.w	r3, [r8]
 8012034:	f023 0370 	bic.w	r3, r3, #112	; 0x70
 8012038:	051b      	lsls	r3, r3, #20
 801203a:	0d1b      	lsrs	r3, r3, #20
 801203c:	f8c8 3000 	str.w	r3, [r8]
 8012040:	f7ff feca 	bl	8011dd8 <Out0_Process>
 8012044:	e7bd      	b.n	8011fc2 <CTR_LP+0x82>
 8012046:	009a      	lsls	r2, r3, #2
 8012048:	f102 4280 	add.w	r2, r2, #1073741824	; 0x40000000
 801204c:	f502 42b8 	add.w	r2, r2, #23552	; 0x5c00
 8012050:	6811      	ldr	r1, [r2, #0]
 8012052:	b289      	uxth	r1, r1
 8012054:	f8ad 1006 	strh.w	r1, [sp, #6]
 8012058:	f8bd 1006 	ldrh.w	r1, [sp, #6]
 801205c:	0409      	lsls	r1, r1, #16
 801205e:	d50a      	bpl.n	8012076 <CTR_LP+0x136>
 8012060:	6811      	ldr	r1, [r2, #0]
 8012062:	3b01      	subs	r3, #1
 8012064:	f021 0170 	bic.w	r1, r1, #112	; 0x70
 8012068:	0509      	lsls	r1, r1, #20
 801206a:	0d09      	lsrs	r1, r1, #20
 801206c:	6011      	str	r1, [r2, #0]
 801206e:	4a13      	ldr	r2, [pc, #76]	; (80120bc <CTR_LP+0x17c>)
 8012070:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
 8012074:	4798      	blx	r3
 8012076:	f8bd 3006 	ldrh.w	r3, [sp, #6]
 801207a:	061a      	lsls	r2, r3, #24
 801207c:	f57f af67 	bpl.w	8011f4e <CTR_LP+0xe>
 8012080:	7832      	ldrb	r2, [r6, #0]
 8012082:	0093      	lsls	r3, r2, #2
 8012084:	f103 4380 	add.w	r3, r3, #1073741824	; 0x40000000
 8012088:	f503 43b8 	add.w	r3, r3, #23552	; 0x5c00
 801208c:	6819      	ldr	r1, [r3, #0]
 801208e:	3a01      	subs	r2, #1
 8012090:	f421 41e1 	bic.w	r1, r1, #28800	; 0x7080
 8012094:	f021 0170 	bic.w	r1, r1, #112	; 0x70
 8012098:	0409      	lsls	r1, r1, #16
 801209a:	0c09      	lsrs	r1, r1, #16
 801209c:	6019      	str	r1, [r3, #0]
 801209e:	4b08      	ldr	r3, [pc, #32]	; (80120c0 <CTR_LP+0x180>)
 80120a0:	f853 3022 	ldr.w	r3, [r3, r2, lsl #2]
 80120a4:	4798      	blx	r3
 80120a6:	e752      	b.n	8011f4e <CTR_LP+0xe>
 80120a8:	40005c44 	.word	0x40005c44
 80120ac:	20009530 	.word	0x20009530
 80120b0:	200095d4 	.word	0x200095d4
 80120b4:	200095d0 	.word	0x200095d0
 80120b8:	40005c00 	.word	0x40005c00
 80120bc:	20000204 	.word	0x20000204
 80120c0:	20000220 	.word	0x20000220

080120c4 <CTR_HP>:
 80120c4:	b570      	push	{r4, r5, r6, lr}
 80120c6:	4d18      	ldr	r5, [pc, #96]	; (8012128 <CTR_HP+0x64>)
 80120c8:	4c18      	ldr	r4, [pc, #96]	; (801212c <CTR_HP+0x68>)
 80120ca:	4e19      	ldr	r6, [pc, #100]	; (8012130 <CTR_HP+0x6c>)
 80120cc:	682b      	ldr	r3, [r5, #0]
 80120ce:	b29a      	uxth	r2, r3
 80120d0:	041b      	lsls	r3, r3, #16
 80120d2:	8022      	strh	r2, [r4, #0]
 80120d4:	d400      	bmi.n	80120d8 <CTR_HP+0x14>
 80120d6:	bd70      	pop	{r4, r5, r6, pc}
 80120d8:	f647 73ff 	movw	r3, #32767	; 0x7fff
 80120dc:	602b      	str	r3, [r5, #0]
 80120de:	8823      	ldrh	r3, [r4, #0]
 80120e0:	f003 030f 	and.w	r3, r3, #15
 80120e4:	009a      	lsls	r2, r3, #2
 80120e6:	f102 4280 	add.w	r2, r2, #1073741824	; 0x40000000
 80120ea:	7033      	strb	r3, [r6, #0]
 80120ec:	f502 42b8 	add.w	r2, r2, #23552	; 0x5c00
 80120f0:	6811      	ldr	r1, [r2, #0]
 80120f2:	0408      	lsls	r0, r1, #16
 80120f4:	d50b      	bpl.n	801210e <CTR_HP+0x4a>
 80120f6:	6811      	ldr	r1, [r2, #0]
 80120f8:	3b01      	subs	r3, #1
 80120fa:	f021 0170 	bic.w	r1, r1, #112	; 0x70
 80120fe:	0509      	lsls	r1, r1, #20
 8012100:	0d09      	lsrs	r1, r1, #20
 8012102:	6011      	str	r1, [r2, #0]
 8012104:	4a0b      	ldr	r2, [pc, #44]	; (8012134 <CTR_HP+0x70>)
 8012106:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
 801210a:	4798      	blx	r3
 801210c:	e7de      	b.n	80120cc <CTR_HP+0x8>
 801210e:	0609      	lsls	r1, r1, #24
 8012110:	d5dc      	bpl.n	80120cc <CTR_HP+0x8>
 8012112:	6811      	ldr	r1, [r2, #0]
 8012114:	3b01      	subs	r3, #1
 8012116:	f421 41e1 	bic.w	r1, r1, #28800	; 0x7080
 801211a:	f021 0170 	bic.w	r1, r1, #112	; 0x70
 801211e:	0409      	lsls	r1, r1, #16
 8012120:	0c09      	lsrs	r1, r1, #16
 8012122:	6011      	str	r1, [r2, #0]
 8012124:	4a04      	ldr	r2, [pc, #16]	; (8012138 <CTR_HP+0x74>)
 8012126:	e7ee      	b.n	8012106 <CTR_HP+0x42>
 8012128:	40005c44 	.word	0x40005c44
 801212c:	200095d4 	.word	0x200095d4
 8012130:	20009530 	.word	0x20009530
 8012134:	20000204 	.word	0x20000204
 8012138:	20000220 	.word	0x20000220

0801213c <EP1_IN_Callback>:
 801213c:	f7fe bb50 	b.w	80107e0 <Mass_Storage_In>

08012140 <EP2_OUT_Callback>:
 8012140:	f7fe bc3c 	b.w	80109bc <Mass_Storage_Out>

08012144 <USB_Istr>:
 8012144:	b570      	push	{r4, r5, r6, lr}
 8012146:	4e0d      	ldr	r6, [pc, #52]	; (801217c <USB_Istr+0x38>)
 8012148:	4c0d      	ldr	r4, [pc, #52]	; (8012180 <USB_Istr+0x3c>)
 801214a:	6832      	ldr	r2, [r6, #0]
 801214c:	4d0d      	ldr	r5, [pc, #52]	; (8012184 <USB_Istr+0x40>)
 801214e:	b292      	uxth	r2, r2
 8012150:	8022      	strh	r2, [r4, #0]
 8012152:	8821      	ldrh	r1, [r4, #0]
 8012154:	8828      	ldrh	r0, [r5, #0]
 8012156:	4001      	ands	r1, r0
 8012158:	054a      	lsls	r2, r1, #21
 801215a:	d505      	bpl.n	8012168 <USB_Istr+0x24>
 801215c:	4b0a      	ldr	r3, [pc, #40]	; (8012188 <USB_Istr+0x44>)
 801215e:	685b      	ldr	r3, [r3, #4]
 8012160:	4798      	blx	r3
 8012162:	f64f 33ff 	movw	r3, #64511	; 0xfbff
 8012166:	6033      	str	r3, [r6, #0]
 8012168:	8823      	ldrh	r3, [r4, #0]
 801216a:	882a      	ldrh	r2, [r5, #0]
 801216c:	4013      	ands	r3, r2
 801216e:	041b      	lsls	r3, r3, #16
 8012170:	d503      	bpl.n	801217a <USB_Istr+0x36>
 8012172:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 8012176:	f7ff bee3 	b.w	8011f40 <CTR_LP>
 801217a:	bd70      	pop	{r4, r5, r6, pc}
 801217c:	40005c44 	.word	0x40005c44
 8012180:	200095d4 	.word	0x200095d4
 8012184:	2000952c 	.word	0x2000952c
 8012188:	2000015c 	.word	0x2000015c

0801218c <__aeabi_d2f>:
 801218c:	ea4f 0241 	mov.w	r2, r1, lsl #1
 8012190:	f1b2 43e0 	subs.w	r3, r2, #1879048192	; 0x70000000
 8012194:	bf24      	itt	cs
 8012196:	f5b3 1c00 	subscs.w	ip, r3, #2097152	; 0x200000
 801219a:	f1dc 5cfe 	rsbscs	ip, ip, #532676608	; 0x1fc00000
 801219e:	d90d      	bls.n	80121bc <__aeabi_d2f+0x30>
 80121a0:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 80121a4:	ea4f 02c0 	mov.w	r2, r0, lsl #3
 80121a8:	ea4c 7050 	orr.w	r0, ip, r0, lsr #29
 80121ac:	f1b2 4f00 	cmp.w	r2, #2147483648	; 0x80000000
 80121b0:	eb40 0083 	adc.w	r0, r0, r3, lsl #2
 80121b4:	bf08      	it	eq
 80121b6:	f020 0001 	biceq.w	r0, r0, #1
 80121ba:	4770      	bx	lr
 80121bc:	f011 4f80 	tst.w	r1, #1073741824	; 0x40000000
 80121c0:	d121      	bne.n	8012206 <__aeabi_d2f+0x7a>
 80121c2:	f113 7238 	adds.w	r2, r3, #48234496	; 0x2e00000
 80121c6:	bfbc      	itt	lt
 80121c8:	f001 4000 	andlt.w	r0, r1, #2147483648	; 0x80000000
 80121cc:	4770      	bxlt	lr
 80121ce:	f441 1180 	orr.w	r1, r1, #1048576	; 0x100000
 80121d2:	ea4f 5252 	mov.w	r2, r2, lsr #21
 80121d6:	f1c2 0218 	rsb	r2, r2, #24
 80121da:	f1c2 0c20 	rsb	ip, r2, #32
 80121de:	fa10 f30c 	lsls.w	r3, r0, ip
 80121e2:	fa20 f002 	lsr.w	r0, r0, r2
 80121e6:	bf18      	it	ne
 80121e8:	f040 0001 	orrne.w	r0, r0, #1
 80121ec:	ea4f 23c1 	mov.w	r3, r1, lsl #11
 80121f0:	ea4f 23d3 	mov.w	r3, r3, lsr #11
 80121f4:	fa03 fc0c 	lsl.w	ip, r3, ip
 80121f8:	ea40 000c 	orr.w	r0, r0, ip
 80121fc:	fa23 f302 	lsr.w	r3, r3, r2
 8012200:	ea4f 0343 	mov.w	r3, r3, lsl #1
 8012204:	e7cc      	b.n	80121a0 <__aeabi_d2f+0x14>
 8012206:	ea7f 5362 	mvns.w	r3, r2, asr #21
 801220a:	d107      	bne.n	801221c <__aeabi_d2f+0x90>
 801220c:	ea50 3301 	orrs.w	r3, r0, r1, lsl #12
 8012210:	bf1e      	ittt	ne
 8012212:	f04f 40fe 	movne.w	r0, #2130706432	; 0x7f000000
 8012216:	f440 0040 	orrne.w	r0, r0, #12582912	; 0xc00000
 801221a:	4770      	bxne	lr
 801221c:	f001 4000 	and.w	r0, r1, #2147483648	; 0x80000000
 8012220:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 8012224:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8012228:	4770      	bx	lr
 801222a:	bf00      	nop

0801222c <__aeabi_frsub>:
 801222c:	f080 4000 	eor.w	r0, r0, #2147483648	; 0x80000000
 8012230:	e002      	b.n	8012238 <__addsf3>
 8012232:	bf00      	nop

08012234 <__aeabi_fsub>:
 8012234:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000

08012238 <__addsf3>:
 8012238:	0042      	lsls	r2, r0, #1
 801223a:	bf1f      	itttt	ne
 801223c:	ea5f 0341 	movsne.w	r3, r1, lsl #1
 8012240:	ea92 0f03 	teqne	r2, r3
 8012244:	ea7f 6c22 	mvnsne.w	ip, r2, asr #24
 8012248:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 801224c:	d06a      	beq.n	8012324 <__addsf3+0xec>
 801224e:	ea4f 6212 	mov.w	r2, r2, lsr #24
 8012252:	ebd2 6313 	rsbs	r3, r2, r3, lsr #24
 8012256:	bfc1      	itttt	gt
 8012258:	18d2      	addgt	r2, r2, r3
 801225a:	4041      	eorgt	r1, r0
 801225c:	4048      	eorgt	r0, r1
 801225e:	4041      	eorgt	r1, r0
 8012260:	bfb8      	it	lt
 8012262:	425b      	neglt	r3, r3
 8012264:	2b19      	cmp	r3, #25
 8012266:	bf88      	it	hi
 8012268:	4770      	bxhi	lr
 801226a:	f010 4f00 	tst.w	r0, #2147483648	; 0x80000000
 801226e:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8012272:	f020 407f 	bic.w	r0, r0, #4278190080	; 0xff000000
 8012276:	bf18      	it	ne
 8012278:	4240      	negne	r0, r0
 801227a:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 801227e:	f441 0100 	orr.w	r1, r1, #8388608	; 0x800000
 8012282:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 8012286:	bf18      	it	ne
 8012288:	4249      	negne	r1, r1
 801228a:	ea92 0f03 	teq	r2, r3
 801228e:	d03f      	beq.n	8012310 <__addsf3+0xd8>
 8012290:	f1a2 0201 	sub.w	r2, r2, #1
 8012294:	fa41 fc03 	asr.w	ip, r1, r3
 8012298:	eb10 000c 	adds.w	r0, r0, ip
 801229c:	f1c3 0320 	rsb	r3, r3, #32
 80122a0:	fa01 f103 	lsl.w	r1, r1, r3
 80122a4:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 80122a8:	d502      	bpl.n	80122b0 <__addsf3+0x78>
 80122aa:	4249      	negs	r1, r1
 80122ac:	eb60 0040 	sbc.w	r0, r0, r0, lsl #1
 80122b0:	f5b0 0f00 	cmp.w	r0, #8388608	; 0x800000
 80122b4:	d313      	bcc.n	80122de <__addsf3+0xa6>
 80122b6:	f1b0 7f80 	cmp.w	r0, #16777216	; 0x1000000
 80122ba:	d306      	bcc.n	80122ca <__addsf3+0x92>
 80122bc:	0840      	lsrs	r0, r0, #1
 80122be:	ea4f 0131 	mov.w	r1, r1, rrx
 80122c2:	f102 0201 	add.w	r2, r2, #1
 80122c6:	2afe      	cmp	r2, #254	; 0xfe
 80122c8:	d251      	bcs.n	801236e <__addsf3+0x136>
 80122ca:	f1b1 4f00 	cmp.w	r1, #2147483648	; 0x80000000
 80122ce:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 80122d2:	bf08      	it	eq
 80122d4:	f020 0001 	biceq.w	r0, r0, #1
 80122d8:	ea40 0003 	orr.w	r0, r0, r3
 80122dc:	4770      	bx	lr
 80122de:	0049      	lsls	r1, r1, #1
 80122e0:	eb40 0000 	adc.w	r0, r0, r0
 80122e4:	f410 0f00 	tst.w	r0, #8388608	; 0x800000
 80122e8:	f1a2 0201 	sub.w	r2, r2, #1
 80122ec:	d1ed      	bne.n	80122ca <__addsf3+0x92>
 80122ee:	fab0 fc80 	clz	ip, r0
 80122f2:	f1ac 0c08 	sub.w	ip, ip, #8
 80122f6:	ebb2 020c 	subs.w	r2, r2, ip
 80122fa:	fa00 f00c 	lsl.w	r0, r0, ip
 80122fe:	bfaa      	itet	ge
 8012300:	eb00 50c2 	addge.w	r0, r0, r2, lsl #23
 8012304:	4252      	neglt	r2, r2
 8012306:	4318      	orrge	r0, r3
 8012308:	bfbc      	itt	lt
 801230a:	40d0      	lsrlt	r0, r2
 801230c:	4318      	orrlt	r0, r3
 801230e:	4770      	bx	lr
 8012310:	f092 0f00 	teq	r2, #0
 8012314:	f481 0100 	eor.w	r1, r1, #8388608	; 0x800000
 8012318:	bf06      	itte	eq
 801231a:	f480 0000 	eoreq.w	r0, r0, #8388608	; 0x800000
 801231e:	3201      	addeq	r2, #1
 8012320:	3b01      	subne	r3, #1
 8012322:	e7b5      	b.n	8012290 <__addsf3+0x58>
 8012324:	ea4f 0341 	mov.w	r3, r1, lsl #1
 8012328:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 801232c:	bf18      	it	ne
 801232e:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 8012332:	d021      	beq.n	8012378 <__addsf3+0x140>
 8012334:	ea92 0f03 	teq	r2, r3
 8012338:	d004      	beq.n	8012344 <__addsf3+0x10c>
 801233a:	f092 0f00 	teq	r2, #0
 801233e:	bf08      	it	eq
 8012340:	4608      	moveq	r0, r1
 8012342:	4770      	bx	lr
 8012344:	ea90 0f01 	teq	r0, r1
 8012348:	bf1c      	itt	ne
 801234a:	2000      	movne	r0, #0
 801234c:	4770      	bxne	lr
 801234e:	f012 4f7f 	tst.w	r2, #4278190080	; 0xff000000
 8012352:	d104      	bne.n	801235e <__addsf3+0x126>
 8012354:	0040      	lsls	r0, r0, #1
 8012356:	bf28      	it	cs
 8012358:	f040 4000 	orrcs.w	r0, r0, #2147483648	; 0x80000000
 801235c:	4770      	bx	lr
 801235e:	f112 7200 	adds.w	r2, r2, #33554432	; 0x2000000
 8012362:	bf3c      	itt	cc
 8012364:	f500 0000 	addcc.w	r0, r0, #8388608	; 0x800000
 8012368:	4770      	bxcc	lr
 801236a:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 801236e:	f043 40fe 	orr.w	r0, r3, #2130706432	; 0x7f000000
 8012372:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8012376:	4770      	bx	lr
 8012378:	ea7f 6222 	mvns.w	r2, r2, asr #24
 801237c:	bf16      	itet	ne
 801237e:	4608      	movne	r0, r1
 8012380:	ea7f 6323 	mvnseq.w	r3, r3, asr #24
 8012384:	4601      	movne	r1, r0
 8012386:	0242      	lsls	r2, r0, #9
 8012388:	bf06      	itte	eq
 801238a:	ea5f 2341 	movseq.w	r3, r1, lsl #9
 801238e:	ea90 0f01 	teqeq	r0, r1
 8012392:	f440 0080 	orrne.w	r0, r0, #4194304	; 0x400000
 8012396:	4770      	bx	lr

08012398 <__aeabi_ui2f>:
 8012398:	f04f 0300 	mov.w	r3, #0
 801239c:	e004      	b.n	80123a8 <__aeabi_i2f+0x8>
 801239e:	bf00      	nop

080123a0 <__aeabi_i2f>:
 80123a0:	f010 4300 	ands.w	r3, r0, #2147483648	; 0x80000000
 80123a4:	bf48      	it	mi
 80123a6:	4240      	negmi	r0, r0
 80123a8:	ea5f 0c00 	movs.w	ip, r0
 80123ac:	bf08      	it	eq
 80123ae:	4770      	bxeq	lr
 80123b0:	f043 4396 	orr.w	r3, r3, #1258291200	; 0x4b000000
 80123b4:	4601      	mov	r1, r0
 80123b6:	f04f 0000 	mov.w	r0, #0
 80123ba:	e01c      	b.n	80123f6 <__aeabi_l2f+0x2a>

080123bc <__aeabi_ul2f>:
 80123bc:	ea50 0201 	orrs.w	r2, r0, r1
 80123c0:	bf08      	it	eq
 80123c2:	4770      	bxeq	lr
 80123c4:	f04f 0300 	mov.w	r3, #0
 80123c8:	e00a      	b.n	80123e0 <__aeabi_l2f+0x14>
 80123ca:	bf00      	nop

080123cc <__aeabi_l2f>:
 80123cc:	ea50 0201 	orrs.w	r2, r0, r1
 80123d0:	bf08      	it	eq
 80123d2:	4770      	bxeq	lr
 80123d4:	f011 4300 	ands.w	r3, r1, #2147483648	; 0x80000000
 80123d8:	d502      	bpl.n	80123e0 <__aeabi_l2f+0x14>
 80123da:	4240      	negs	r0, r0
 80123dc:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 80123e0:	ea5f 0c01 	movs.w	ip, r1
 80123e4:	bf02      	ittt	eq
 80123e6:	4684      	moveq	ip, r0
 80123e8:	4601      	moveq	r1, r0
 80123ea:	2000      	moveq	r0, #0
 80123ec:	f043 43b6 	orr.w	r3, r3, #1526726656	; 0x5b000000
 80123f0:	bf08      	it	eq
 80123f2:	f1a3 5380 	subeq.w	r3, r3, #268435456	; 0x10000000
 80123f6:	f5a3 0300 	sub.w	r3, r3, #8388608	; 0x800000
 80123fa:	fabc f28c 	clz	r2, ip
 80123fe:	3a08      	subs	r2, #8
 8012400:	eba3 53c2 	sub.w	r3, r3, r2, lsl #23
 8012404:	db10      	blt.n	8012428 <__aeabi_l2f+0x5c>
 8012406:	fa01 fc02 	lsl.w	ip, r1, r2
 801240a:	4463      	add	r3, ip
 801240c:	fa00 fc02 	lsl.w	ip, r0, r2
 8012410:	f1c2 0220 	rsb	r2, r2, #32
 8012414:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 8012418:	fa20 f202 	lsr.w	r2, r0, r2
 801241c:	eb43 0002 	adc.w	r0, r3, r2
 8012420:	bf08      	it	eq
 8012422:	f020 0001 	biceq.w	r0, r0, #1
 8012426:	4770      	bx	lr
 8012428:	f102 0220 	add.w	r2, r2, #32
 801242c:	fa01 fc02 	lsl.w	ip, r1, r2
 8012430:	f1c2 0220 	rsb	r2, r2, #32
 8012434:	ea50 004c 	orrs.w	r0, r0, ip, lsl #1
 8012438:	fa21 f202 	lsr.w	r2, r1, r2
 801243c:	eb43 0002 	adc.w	r0, r3, r2
 8012440:	bf08      	it	eq
 8012442:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 8012446:	4770      	bx	lr

08012448 <__aeabi_fmul>:
 8012448:	f04f 0cff 	mov.w	ip, #255	; 0xff
 801244c:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 8012450:	bf1e      	ittt	ne
 8012452:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 8012456:	ea92 0f0c 	teqne	r2, ip
 801245a:	ea93 0f0c 	teqne	r3, ip
 801245e:	d06f      	beq.n	8012540 <__aeabi_fmul+0xf8>
 8012460:	441a      	add	r2, r3
 8012462:	ea80 0c01 	eor.w	ip, r0, r1
 8012466:	0240      	lsls	r0, r0, #9
 8012468:	bf18      	it	ne
 801246a:	ea5f 2141 	movsne.w	r1, r1, lsl #9
 801246e:	d01e      	beq.n	80124ae <__aeabi_fmul+0x66>
 8012470:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
 8012474:	ea43 1050 	orr.w	r0, r3, r0, lsr #5
 8012478:	ea43 1151 	orr.w	r1, r3, r1, lsr #5
 801247c:	fba0 3101 	umull	r3, r1, r0, r1
 8012480:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 8012484:	f5b1 0f00 	cmp.w	r1, #8388608	; 0x800000
 8012488:	bf3e      	ittt	cc
 801248a:	0049      	lslcc	r1, r1, #1
 801248c:	ea41 71d3 	orrcc.w	r1, r1, r3, lsr #31
 8012490:	005b      	lslcc	r3, r3, #1
 8012492:	ea40 0001 	orr.w	r0, r0, r1
 8012496:	f162 027f 	sbc.w	r2, r2, #127	; 0x7f
 801249a:	2afd      	cmp	r2, #253	; 0xfd
 801249c:	d81d      	bhi.n	80124da <__aeabi_fmul+0x92>
 801249e:	f1b3 4f00 	cmp.w	r3, #2147483648	; 0x80000000
 80124a2:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 80124a6:	bf08      	it	eq
 80124a8:	f020 0001 	biceq.w	r0, r0, #1
 80124ac:	4770      	bx	lr
 80124ae:	f090 0f00 	teq	r0, #0
 80124b2:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 80124b6:	bf08      	it	eq
 80124b8:	0249      	lsleq	r1, r1, #9
 80124ba:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 80124be:	ea40 2051 	orr.w	r0, r0, r1, lsr #9
 80124c2:	3a7f      	subs	r2, #127	; 0x7f
 80124c4:	bfc2      	ittt	gt
 80124c6:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 80124ca:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 80124ce:	4770      	bxgt	lr
 80124d0:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 80124d4:	f04f 0300 	mov.w	r3, #0
 80124d8:	3a01      	subs	r2, #1
 80124da:	dc5d      	bgt.n	8012598 <__aeabi_fmul+0x150>
 80124dc:	f112 0f19 	cmn.w	r2, #25
 80124e0:	bfdc      	itt	le
 80124e2:	f000 4000 	andle.w	r0, r0, #2147483648	; 0x80000000
 80124e6:	4770      	bxle	lr
 80124e8:	f1c2 0200 	rsb	r2, r2, #0
 80124ec:	0041      	lsls	r1, r0, #1
 80124ee:	fa21 f102 	lsr.w	r1, r1, r2
 80124f2:	f1c2 0220 	rsb	r2, r2, #32
 80124f6:	fa00 fc02 	lsl.w	ip, r0, r2
 80124fa:	ea5f 0031 	movs.w	r0, r1, rrx
 80124fe:	f140 0000 	adc.w	r0, r0, #0
 8012502:	ea53 034c 	orrs.w	r3, r3, ip, lsl #1
 8012506:	bf08      	it	eq
 8012508:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 801250c:	4770      	bx	lr
 801250e:	f092 0f00 	teq	r2, #0
 8012512:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 8012516:	bf02      	ittt	eq
 8012518:	0040      	lsleq	r0, r0, #1
 801251a:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 801251e:	3a01      	subeq	r2, #1
 8012520:	d0f9      	beq.n	8012516 <__aeabi_fmul+0xce>
 8012522:	ea40 000c 	orr.w	r0, r0, ip
 8012526:	f093 0f00 	teq	r3, #0
 801252a:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 801252e:	bf02      	ittt	eq
 8012530:	0049      	lsleq	r1, r1, #1
 8012532:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 8012536:	3b01      	subeq	r3, #1
 8012538:	d0f9      	beq.n	801252e <__aeabi_fmul+0xe6>
 801253a:	ea41 010c 	orr.w	r1, r1, ip
 801253e:	e78f      	b.n	8012460 <__aeabi_fmul+0x18>
 8012540:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 8012544:	ea92 0f0c 	teq	r2, ip
 8012548:	bf18      	it	ne
 801254a:	ea93 0f0c 	teqne	r3, ip
 801254e:	d00a      	beq.n	8012566 <__aeabi_fmul+0x11e>
 8012550:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 8012554:	bf18      	it	ne
 8012556:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 801255a:	d1d8      	bne.n	801250e <__aeabi_fmul+0xc6>
 801255c:	ea80 0001 	eor.w	r0, r0, r1
 8012560:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 8012564:	4770      	bx	lr
 8012566:	f090 0f00 	teq	r0, #0
 801256a:	bf17      	itett	ne
 801256c:	f090 4f00 	teqne	r0, #2147483648	; 0x80000000
 8012570:	4608      	moveq	r0, r1
 8012572:	f091 0f00 	teqne	r1, #0
 8012576:	f091 4f00 	teqne	r1, #2147483648	; 0x80000000
 801257a:	d014      	beq.n	80125a6 <__aeabi_fmul+0x15e>
 801257c:	ea92 0f0c 	teq	r2, ip
 8012580:	d101      	bne.n	8012586 <__aeabi_fmul+0x13e>
 8012582:	0242      	lsls	r2, r0, #9
 8012584:	d10f      	bne.n	80125a6 <__aeabi_fmul+0x15e>
 8012586:	ea93 0f0c 	teq	r3, ip
 801258a:	d103      	bne.n	8012594 <__aeabi_fmul+0x14c>
 801258c:	024b      	lsls	r3, r1, #9
 801258e:	bf18      	it	ne
 8012590:	4608      	movne	r0, r1
 8012592:	d108      	bne.n	80125a6 <__aeabi_fmul+0x15e>
 8012594:	ea80 0001 	eor.w	r0, r0, r1
 8012598:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 801259c:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 80125a0:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 80125a4:	4770      	bx	lr
 80125a6:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 80125aa:	f440 0040 	orr.w	r0, r0, #12582912	; 0xc00000
 80125ae:	4770      	bx	lr

080125b0 <__aeabi_fdiv>:
 80125b0:	f04f 0cff 	mov.w	ip, #255	; 0xff
 80125b4:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 80125b8:	bf1e      	ittt	ne
 80125ba:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 80125be:	ea92 0f0c 	teqne	r2, ip
 80125c2:	ea93 0f0c 	teqne	r3, ip
 80125c6:	d069      	beq.n	801269c <__aeabi_fdiv+0xec>
 80125c8:	eba2 0203 	sub.w	r2, r2, r3
 80125cc:	ea80 0c01 	eor.w	ip, r0, r1
 80125d0:	0249      	lsls	r1, r1, #9
 80125d2:	ea4f 2040 	mov.w	r0, r0, lsl #9
 80125d6:	d037      	beq.n	8012648 <__aeabi_fdiv+0x98>
 80125d8:	f04f 5380 	mov.w	r3, #268435456	; 0x10000000
 80125dc:	ea43 1111 	orr.w	r1, r3, r1, lsr #4
 80125e0:	ea43 1310 	orr.w	r3, r3, r0, lsr #4
 80125e4:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 80125e8:	428b      	cmp	r3, r1
 80125ea:	bf38      	it	cc
 80125ec:	005b      	lslcc	r3, r3, #1
 80125ee:	f142 027d 	adc.w	r2, r2, #125	; 0x7d
 80125f2:	f44f 0c00 	mov.w	ip, #8388608	; 0x800000
 80125f6:	428b      	cmp	r3, r1
 80125f8:	bf24      	itt	cs
 80125fa:	1a5b      	subcs	r3, r3, r1
 80125fc:	ea40 000c 	orrcs.w	r0, r0, ip
 8012600:	ebb3 0f51 	cmp.w	r3, r1, lsr #1
 8012604:	bf24      	itt	cs
 8012606:	eba3 0351 	subcs.w	r3, r3, r1, lsr #1
 801260a:	ea40 005c 	orrcs.w	r0, r0, ip, lsr #1
 801260e:	ebb3 0f91 	cmp.w	r3, r1, lsr #2
 8012612:	bf24      	itt	cs
 8012614:	eba3 0391 	subcs.w	r3, r3, r1, lsr #2
 8012618:	ea40 009c 	orrcs.w	r0, r0, ip, lsr #2
 801261c:	ebb3 0fd1 	cmp.w	r3, r1, lsr #3
 8012620:	bf24      	itt	cs
 8012622:	eba3 03d1 	subcs.w	r3, r3, r1, lsr #3
 8012626:	ea40 00dc 	orrcs.w	r0, r0, ip, lsr #3
 801262a:	011b      	lsls	r3, r3, #4
 801262c:	bf18      	it	ne
 801262e:	ea5f 1c1c 	movsne.w	ip, ip, lsr #4
 8012632:	d1e0      	bne.n	80125f6 <__aeabi_fdiv+0x46>
 8012634:	2afd      	cmp	r2, #253	; 0xfd
 8012636:	f63f af50 	bhi.w	80124da <__aeabi_fmul+0x92>
 801263a:	428b      	cmp	r3, r1
 801263c:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 8012640:	bf08      	it	eq
 8012642:	f020 0001 	biceq.w	r0, r0, #1
 8012646:	4770      	bx	lr
 8012648:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 801264c:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 8012650:	327f      	adds	r2, #127	; 0x7f
 8012652:	bfc2      	ittt	gt
 8012654:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 8012658:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 801265c:	4770      	bxgt	lr
 801265e:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8012662:	f04f 0300 	mov.w	r3, #0
 8012666:	3a01      	subs	r2, #1
 8012668:	e737      	b.n	80124da <__aeabi_fmul+0x92>
 801266a:	f092 0f00 	teq	r2, #0
 801266e:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 8012672:	bf02      	ittt	eq
 8012674:	0040      	lsleq	r0, r0, #1
 8012676:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 801267a:	3a01      	subeq	r2, #1
 801267c:	d0f9      	beq.n	8012672 <__aeabi_fdiv+0xc2>
 801267e:	ea40 000c 	orr.w	r0, r0, ip
 8012682:	f093 0f00 	teq	r3, #0
 8012686:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 801268a:	bf02      	ittt	eq
 801268c:	0049      	lsleq	r1, r1, #1
 801268e:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 8012692:	3b01      	subeq	r3, #1
 8012694:	d0f9      	beq.n	801268a <__aeabi_fdiv+0xda>
 8012696:	ea41 010c 	orr.w	r1, r1, ip
 801269a:	e795      	b.n	80125c8 <__aeabi_fdiv+0x18>
 801269c:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 80126a0:	ea92 0f0c 	teq	r2, ip
 80126a4:	d108      	bne.n	80126b8 <__aeabi_fdiv+0x108>
 80126a6:	0242      	lsls	r2, r0, #9
 80126a8:	f47f af7d 	bne.w	80125a6 <__aeabi_fmul+0x15e>
 80126ac:	ea93 0f0c 	teq	r3, ip
 80126b0:	f47f af70 	bne.w	8012594 <__aeabi_fmul+0x14c>
 80126b4:	4608      	mov	r0, r1
 80126b6:	e776      	b.n	80125a6 <__aeabi_fmul+0x15e>
 80126b8:	ea93 0f0c 	teq	r3, ip
 80126bc:	d104      	bne.n	80126c8 <__aeabi_fdiv+0x118>
 80126be:	024b      	lsls	r3, r1, #9
 80126c0:	f43f af4c 	beq.w	801255c <__aeabi_fmul+0x114>
 80126c4:	4608      	mov	r0, r1
 80126c6:	e76e      	b.n	80125a6 <__aeabi_fmul+0x15e>
 80126c8:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 80126cc:	bf18      	it	ne
 80126ce:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 80126d2:	d1ca      	bne.n	801266a <__aeabi_fdiv+0xba>
 80126d4:	f030 4200 	bics.w	r2, r0, #2147483648	; 0x80000000
 80126d8:	f47f af5c 	bne.w	8012594 <__aeabi_fmul+0x14c>
 80126dc:	f031 4300 	bics.w	r3, r1, #2147483648	; 0x80000000
 80126e0:	f47f af3c 	bne.w	801255c <__aeabi_fmul+0x114>
 80126e4:	e75f      	b.n	80125a6 <__aeabi_fmul+0x15e>
 80126e6:	bf00      	nop

080126e8 <__gesf2>:
 80126e8:	f04f 3cff 	mov.w	ip, #4294967295	; 0xffffffff
 80126ec:	e006      	b.n	80126fc <__cmpsf2+0x4>
 80126ee:	bf00      	nop

080126f0 <__lesf2>:
 80126f0:	f04f 0c01 	mov.w	ip, #1
 80126f4:	e002      	b.n	80126fc <__cmpsf2+0x4>
 80126f6:	bf00      	nop

080126f8 <__cmpsf2>:
 80126f8:	f04f 0c01 	mov.w	ip, #1
 80126fc:	f84d cd04 	str.w	ip, [sp, #-4]!
 8012700:	ea4f 0240 	mov.w	r2, r0, lsl #1
 8012704:	ea4f 0341 	mov.w	r3, r1, lsl #1
 8012708:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 801270c:	bf18      	it	ne
 801270e:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 8012712:	d011      	beq.n	8012738 <__cmpsf2+0x40>
 8012714:	b001      	add	sp, #4
 8012716:	ea52 0c53 	orrs.w	ip, r2, r3, lsr #1
 801271a:	bf18      	it	ne
 801271c:	ea90 0f01 	teqne	r0, r1
 8012720:	bf58      	it	pl
 8012722:	ebb2 0003 	subspl.w	r0, r2, r3
 8012726:	bf88      	it	hi
 8012728:	17c8      	asrhi	r0, r1, #31
 801272a:	bf38      	it	cc
 801272c:	ea6f 70e1 	mvncc.w	r0, r1, asr #31
 8012730:	bf18      	it	ne
 8012732:	f040 0001 	orrne.w	r0, r0, #1
 8012736:	4770      	bx	lr
 8012738:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 801273c:	d102      	bne.n	8012744 <__cmpsf2+0x4c>
 801273e:	ea5f 2c40 	movs.w	ip, r0, lsl #9
 8012742:	d105      	bne.n	8012750 <__cmpsf2+0x58>
 8012744:	ea7f 6c23 	mvns.w	ip, r3, asr #24
 8012748:	d1e4      	bne.n	8012714 <__cmpsf2+0x1c>
 801274a:	ea5f 2c41 	movs.w	ip, r1, lsl #9
 801274e:	d0e1      	beq.n	8012714 <__cmpsf2+0x1c>
 8012750:	f85d 0b04 	ldr.w	r0, [sp], #4
 8012754:	4770      	bx	lr
 8012756:	bf00      	nop

08012758 <__aeabi_cfrcmple>:
 8012758:	4684      	mov	ip, r0
 801275a:	4608      	mov	r0, r1
 801275c:	4661      	mov	r1, ip
 801275e:	e7ff      	b.n	8012760 <__aeabi_cfcmpeq>

08012760 <__aeabi_cfcmpeq>:
 8012760:	b50f      	push	{r0, r1, r2, r3, lr}
 8012762:	f7ff ffc9 	bl	80126f8 <__cmpsf2>
 8012766:	2800      	cmp	r0, #0
 8012768:	bf48      	it	mi
 801276a:	f110 0f00 	cmnmi.w	r0, #0
 801276e:	bd0f      	pop	{r0, r1, r2, r3, pc}

08012770 <__aeabi_fcmpeq>:
 8012770:	f84d ed08 	str.w	lr, [sp, #-8]!
 8012774:	f7ff fff4 	bl	8012760 <__aeabi_cfcmpeq>
 8012778:	bf0c      	ite	eq
 801277a:	2001      	moveq	r0, #1
 801277c:	2000      	movne	r0, #0
 801277e:	f85d fb08 	ldr.w	pc, [sp], #8
 8012782:	bf00      	nop

08012784 <__aeabi_fcmplt>:
 8012784:	f84d ed08 	str.w	lr, [sp, #-8]!
 8012788:	f7ff ffea 	bl	8012760 <__aeabi_cfcmpeq>
 801278c:	bf34      	ite	cc
 801278e:	2001      	movcc	r0, #1
 8012790:	2000      	movcs	r0, #0
 8012792:	f85d fb08 	ldr.w	pc, [sp], #8
 8012796:	bf00      	nop

08012798 <__aeabi_fcmple>:
 8012798:	f84d ed08 	str.w	lr, [sp, #-8]!
 801279c:	f7ff ffe0 	bl	8012760 <__aeabi_cfcmpeq>
 80127a0:	bf94      	ite	ls
 80127a2:	2001      	movls	r0, #1
 80127a4:	2000      	movhi	r0, #0
 80127a6:	f85d fb08 	ldr.w	pc, [sp], #8
 80127aa:	bf00      	nop

080127ac <__aeabi_fcmpge>:
 80127ac:	f84d ed08 	str.w	lr, [sp, #-8]!
 80127b0:	f7ff ffd2 	bl	8012758 <__aeabi_cfrcmple>
 80127b4:	bf94      	ite	ls
 80127b6:	2001      	movls	r0, #1
 80127b8:	2000      	movhi	r0, #0
 80127ba:	f85d fb08 	ldr.w	pc, [sp], #8
 80127be:	bf00      	nop

080127c0 <__aeabi_fcmpgt>:
 80127c0:	f84d ed08 	str.w	lr, [sp, #-8]!
 80127c4:	f7ff ffc8 	bl	8012758 <__aeabi_cfrcmple>
 80127c8:	bf34      	ite	cc
 80127ca:	2001      	movcc	r0, #1
 80127cc:	2000      	movcs	r0, #0
 80127ce:	f85d fb08 	ldr.w	pc, [sp], #8
 80127d2:	bf00      	nop

080127d4 <__aeabi_f2iz>:
 80127d4:	ea4f 0240 	mov.w	r2, r0, lsl #1
 80127d8:	f1b2 4ffe 	cmp.w	r2, #2130706432	; 0x7f000000
 80127dc:	d30f      	bcc.n	80127fe <__aeabi_f2iz+0x2a>
 80127de:	f04f 039e 	mov.w	r3, #158	; 0x9e
 80127e2:	ebb3 6212 	subs.w	r2, r3, r2, lsr #24
 80127e6:	d90d      	bls.n	8012804 <__aeabi_f2iz+0x30>
 80127e8:	ea4f 2300 	mov.w	r3, r0, lsl #8
 80127ec:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
 80127f0:	f010 4f00 	tst.w	r0, #2147483648	; 0x80000000
 80127f4:	fa23 f002 	lsr.w	r0, r3, r2
 80127f8:	bf18      	it	ne
 80127fa:	4240      	negne	r0, r0
 80127fc:	4770      	bx	lr
 80127fe:	f04f 0000 	mov.w	r0, #0
 8012802:	4770      	bx	lr
 8012804:	f112 0f61 	cmn.w	r2, #97	; 0x61
 8012808:	d101      	bne.n	801280e <__aeabi_f2iz+0x3a>
 801280a:	0242      	lsls	r2, r0, #9
 801280c:	d105      	bne.n	801281a <__aeabi_f2iz+0x46>
 801280e:	f010 4000 	ands.w	r0, r0, #2147483648	; 0x80000000
 8012812:	bf08      	it	eq
 8012814:	f06f 4000 	mvneq.w	r0, #2147483648	; 0x80000000
 8012818:	4770      	bx	lr
 801281a:	f04f 0000 	mov.w	r0, #0
 801281e:	4770      	bx	lr

08012820 <__aeabi_uldivmod>:
 8012820:	b953      	cbnz	r3, 8012838 <__aeabi_uldivmod+0x18>
 8012822:	b94a      	cbnz	r2, 8012838 <__aeabi_uldivmod+0x18>
 8012824:	2900      	cmp	r1, #0
 8012826:	bf08      	it	eq
 8012828:	2800      	cmpeq	r0, #0
 801282a:	bf1c      	itt	ne
 801282c:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
 8012830:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
 8012834:	f000 b974 	b.w	8012b20 <__aeabi_idiv0>
 8012838:	f1ad 0c08 	sub.w	ip, sp, #8
 801283c:	e96d ce04 	strd	ip, lr, [sp, #-16]!
 8012840:	f000 f806 	bl	8012850 <__udivmoddi4>
 8012844:	f8dd e004 	ldr.w	lr, [sp, #4]
 8012848:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
 801284c:	b004      	add	sp, #16
 801284e:	4770      	bx	lr

08012850 <__udivmoddi4>:
 8012850:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8012854:	468c      	mov	ip, r1
 8012856:	4604      	mov	r4, r0
 8012858:	9e08      	ldr	r6, [sp, #32]
 801285a:	2b00      	cmp	r3, #0
 801285c:	d14b      	bne.n	80128f6 <__udivmoddi4+0xa6>
 801285e:	428a      	cmp	r2, r1
 8012860:	4615      	mov	r5, r2
 8012862:	d967      	bls.n	8012934 <__udivmoddi4+0xe4>
 8012864:	fab2 f282 	clz	r2, r2
 8012868:	b14a      	cbz	r2, 801287e <__udivmoddi4+0x2e>
 801286a:	f1c2 0720 	rsb	r7, r2, #32
 801286e:	fa01 f302 	lsl.w	r3, r1, r2
 8012872:	fa20 f707 	lsr.w	r7, r0, r7
 8012876:	4095      	lsls	r5, r2
 8012878:	ea47 0c03 	orr.w	ip, r7, r3
 801287c:	4094      	lsls	r4, r2
 801287e:	ea4f 4e15 	mov.w	lr, r5, lsr #16
 8012882:	fbbc f7fe 	udiv	r7, ip, lr
 8012886:	fa1f f885 	uxth.w	r8, r5
 801288a:	fb0e c317 	mls	r3, lr, r7, ip
 801288e:	fb07 f908 	mul.w	r9, r7, r8
 8012892:	0c21      	lsrs	r1, r4, #16
 8012894:	ea41 4303 	orr.w	r3, r1, r3, lsl #16
 8012898:	4599      	cmp	r9, r3
 801289a:	d909      	bls.n	80128b0 <__udivmoddi4+0x60>
 801289c:	18eb      	adds	r3, r5, r3
 801289e:	f107 31ff 	add.w	r1, r7, #4294967295	; 0xffffffff
 80128a2:	f080 811c 	bcs.w	8012ade <__udivmoddi4+0x28e>
 80128a6:	4599      	cmp	r9, r3
 80128a8:	f240 8119 	bls.w	8012ade <__udivmoddi4+0x28e>
 80128ac:	3f02      	subs	r7, #2
 80128ae:	442b      	add	r3, r5
 80128b0:	eba3 0309 	sub.w	r3, r3, r9
 80128b4:	fbb3 f0fe 	udiv	r0, r3, lr
 80128b8:	fb0e 3310 	mls	r3, lr, r0, r3
 80128bc:	fb00 f108 	mul.w	r1, r0, r8
 80128c0:	b2a4      	uxth	r4, r4
 80128c2:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
 80128c6:	42a1      	cmp	r1, r4
 80128c8:	d909      	bls.n	80128de <__udivmoddi4+0x8e>
 80128ca:	192c      	adds	r4, r5, r4
 80128cc:	f100 33ff 	add.w	r3, r0, #4294967295	; 0xffffffff
 80128d0:	f080 8107 	bcs.w	8012ae2 <__udivmoddi4+0x292>
 80128d4:	42a1      	cmp	r1, r4
 80128d6:	f240 8104 	bls.w	8012ae2 <__udivmoddi4+0x292>
 80128da:	3802      	subs	r0, #2
 80128dc:	442c      	add	r4, r5
 80128de:	ea40 4007 	orr.w	r0, r0, r7, lsl #16
 80128e2:	2700      	movs	r7, #0
 80128e4:	1a64      	subs	r4, r4, r1
 80128e6:	b11e      	cbz	r6, 80128f0 <__udivmoddi4+0xa0>
 80128e8:	2300      	movs	r3, #0
 80128ea:	40d4      	lsrs	r4, r2
 80128ec:	e9c6 4300 	strd	r4, r3, [r6]
 80128f0:	4639      	mov	r1, r7
 80128f2:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 80128f6:	428b      	cmp	r3, r1
 80128f8:	d909      	bls.n	801290e <__udivmoddi4+0xbe>
 80128fa:	2e00      	cmp	r6, #0
 80128fc:	f000 80ec 	beq.w	8012ad8 <__udivmoddi4+0x288>
 8012900:	2700      	movs	r7, #0
 8012902:	e9c6 0100 	strd	r0, r1, [r6]
 8012906:	4638      	mov	r0, r7
 8012908:	4639      	mov	r1, r7
 801290a:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 801290e:	fab3 f783 	clz	r7, r3
 8012912:	2f00      	cmp	r7, #0
 8012914:	d148      	bne.n	80129a8 <__udivmoddi4+0x158>
 8012916:	428b      	cmp	r3, r1
 8012918:	d302      	bcc.n	8012920 <__udivmoddi4+0xd0>
 801291a:	4282      	cmp	r2, r0
 801291c:	f200 80fb 	bhi.w	8012b16 <__udivmoddi4+0x2c6>
 8012920:	1a84      	subs	r4, r0, r2
 8012922:	eb61 0303 	sbc.w	r3, r1, r3
 8012926:	2001      	movs	r0, #1
 8012928:	469c      	mov	ip, r3
 801292a:	2e00      	cmp	r6, #0
 801292c:	d0e0      	beq.n	80128f0 <__udivmoddi4+0xa0>
 801292e:	e9c6 4c00 	strd	r4, ip, [r6]
 8012932:	e7dd      	b.n	80128f0 <__udivmoddi4+0xa0>
 8012934:	b902      	cbnz	r2, 8012938 <__udivmoddi4+0xe8>
 8012936:	deff      	udf	#255	; 0xff
 8012938:	fab2 f282 	clz	r2, r2
 801293c:	2a00      	cmp	r2, #0
 801293e:	f040 808f 	bne.w	8012a60 <__udivmoddi4+0x210>
 8012942:	2701      	movs	r7, #1
 8012944:	1b49      	subs	r1, r1, r5
 8012946:	ea4f 4815 	mov.w	r8, r5, lsr #16
 801294a:	fa1f f985 	uxth.w	r9, r5
 801294e:	fbb1 fef8 	udiv	lr, r1, r8
 8012952:	fb08 111e 	mls	r1, r8, lr, r1
 8012956:	fb09 f00e 	mul.w	r0, r9, lr
 801295a:	ea4f 4c14 	mov.w	ip, r4, lsr #16
 801295e:	ea4c 4301 	orr.w	r3, ip, r1, lsl #16
 8012962:	4298      	cmp	r0, r3
 8012964:	d907      	bls.n	8012976 <__udivmoddi4+0x126>
 8012966:	18eb      	adds	r3, r5, r3
 8012968:	f10e 31ff 	add.w	r1, lr, #4294967295	; 0xffffffff
 801296c:	d202      	bcs.n	8012974 <__udivmoddi4+0x124>
 801296e:	4298      	cmp	r0, r3
 8012970:	f200 80cd 	bhi.w	8012b0e <__udivmoddi4+0x2be>
 8012974:	468e      	mov	lr, r1
 8012976:	1a1b      	subs	r3, r3, r0
 8012978:	fbb3 f0f8 	udiv	r0, r3, r8
 801297c:	fb08 3310 	mls	r3, r8, r0, r3
 8012980:	fb09 f900 	mul.w	r9, r9, r0
 8012984:	b2a4      	uxth	r4, r4
 8012986:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
 801298a:	45a1      	cmp	r9, r4
 801298c:	d907      	bls.n	801299e <__udivmoddi4+0x14e>
 801298e:	192c      	adds	r4, r5, r4
 8012990:	f100 33ff 	add.w	r3, r0, #4294967295	; 0xffffffff
 8012994:	d202      	bcs.n	801299c <__udivmoddi4+0x14c>
 8012996:	45a1      	cmp	r9, r4
 8012998:	f200 80b6 	bhi.w	8012b08 <__udivmoddi4+0x2b8>
 801299c:	4618      	mov	r0, r3
 801299e:	eba4 0409 	sub.w	r4, r4, r9
 80129a2:	ea40 400e 	orr.w	r0, r0, lr, lsl #16
 80129a6:	e79e      	b.n	80128e6 <__udivmoddi4+0x96>
 80129a8:	f1c7 0520 	rsb	r5, r7, #32
 80129ac:	40bb      	lsls	r3, r7
 80129ae:	fa22 fc05 	lsr.w	ip, r2, r5
 80129b2:	ea4c 0c03 	orr.w	ip, ip, r3
 80129b6:	fa21 f405 	lsr.w	r4, r1, r5
 80129ba:	ea4f 4e1c 	mov.w	lr, ip, lsr #16
 80129be:	fbb4 f9fe 	udiv	r9, r4, lr
 80129c2:	fa1f f88c 	uxth.w	r8, ip
 80129c6:	fb0e 4419 	mls	r4, lr, r9, r4
 80129ca:	fa20 f305 	lsr.w	r3, r0, r5
 80129ce:	40b9      	lsls	r1, r7
 80129d0:	fb09 fa08 	mul.w	sl, r9, r8
 80129d4:	4319      	orrs	r1, r3
 80129d6:	0c0b      	lsrs	r3, r1, #16
 80129d8:	ea43 4404 	orr.w	r4, r3, r4, lsl #16
 80129dc:	45a2      	cmp	sl, r4
 80129de:	fa02 f207 	lsl.w	r2, r2, r7
 80129e2:	fa00 f307 	lsl.w	r3, r0, r7
 80129e6:	d90b      	bls.n	8012a00 <__udivmoddi4+0x1b0>
 80129e8:	eb1c 0404 	adds.w	r4, ip, r4
 80129ec:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
 80129f0:	f080 8088 	bcs.w	8012b04 <__udivmoddi4+0x2b4>
 80129f4:	45a2      	cmp	sl, r4
 80129f6:	f240 8085 	bls.w	8012b04 <__udivmoddi4+0x2b4>
 80129fa:	f1a9 0902 	sub.w	r9, r9, #2
 80129fe:	4464      	add	r4, ip
 8012a00:	eba4 040a 	sub.w	r4, r4, sl
 8012a04:	fbb4 f0fe 	udiv	r0, r4, lr
 8012a08:	fb0e 4410 	mls	r4, lr, r0, r4
 8012a0c:	fb00 fa08 	mul.w	sl, r0, r8
 8012a10:	b289      	uxth	r1, r1
 8012a12:	ea41 4404 	orr.w	r4, r1, r4, lsl #16
 8012a16:	45a2      	cmp	sl, r4
 8012a18:	d908      	bls.n	8012a2c <__udivmoddi4+0x1dc>
 8012a1a:	eb1c 0404 	adds.w	r4, ip, r4
 8012a1e:	f100 31ff 	add.w	r1, r0, #4294967295	; 0xffffffff
 8012a22:	d26b      	bcs.n	8012afc <__udivmoddi4+0x2ac>
 8012a24:	45a2      	cmp	sl, r4
 8012a26:	d969      	bls.n	8012afc <__udivmoddi4+0x2ac>
 8012a28:	3802      	subs	r0, #2
 8012a2a:	4464      	add	r4, ip
 8012a2c:	ea40 4009 	orr.w	r0, r0, r9, lsl #16
 8012a30:	fba0 8902 	umull	r8, r9, r0, r2
 8012a34:	eba4 040a 	sub.w	r4, r4, sl
 8012a38:	454c      	cmp	r4, r9
 8012a3a:	4641      	mov	r1, r8
 8012a3c:	46ce      	mov	lr, r9
 8012a3e:	d354      	bcc.n	8012aea <__udivmoddi4+0x29a>
 8012a40:	d051      	beq.n	8012ae6 <__udivmoddi4+0x296>
 8012a42:	2e00      	cmp	r6, #0
 8012a44:	d069      	beq.n	8012b1a <__udivmoddi4+0x2ca>
 8012a46:	1a5a      	subs	r2, r3, r1
 8012a48:	eb64 040e 	sbc.w	r4, r4, lr
 8012a4c:	fa04 f505 	lsl.w	r5, r4, r5
 8012a50:	fa22 f307 	lsr.w	r3, r2, r7
 8012a54:	40fc      	lsrs	r4, r7
 8012a56:	431d      	orrs	r5, r3
 8012a58:	e9c6 5400 	strd	r5, r4, [r6]
 8012a5c:	2700      	movs	r7, #0
 8012a5e:	e747      	b.n	80128f0 <__udivmoddi4+0xa0>
 8012a60:	4095      	lsls	r5, r2
 8012a62:	f1c2 0320 	rsb	r3, r2, #32
 8012a66:	fa21 f003 	lsr.w	r0, r1, r3
 8012a6a:	ea4f 4815 	mov.w	r8, r5, lsr #16
 8012a6e:	fbb0 f7f8 	udiv	r7, r0, r8
 8012a72:	fa1f f985 	uxth.w	r9, r5
 8012a76:	fb08 0017 	mls	r0, r8, r7, r0
 8012a7a:	fa24 f303 	lsr.w	r3, r4, r3
 8012a7e:	4091      	lsls	r1, r2
 8012a80:	fb07 fc09 	mul.w	ip, r7, r9
 8012a84:	430b      	orrs	r3, r1
 8012a86:	0c19      	lsrs	r1, r3, #16
 8012a88:	ea41 4100 	orr.w	r1, r1, r0, lsl #16
 8012a8c:	458c      	cmp	ip, r1
 8012a8e:	fa04 f402 	lsl.w	r4, r4, r2
 8012a92:	d907      	bls.n	8012aa4 <__udivmoddi4+0x254>
 8012a94:	1869      	adds	r1, r5, r1
 8012a96:	f107 30ff 	add.w	r0, r7, #4294967295	; 0xffffffff
 8012a9a:	d231      	bcs.n	8012b00 <__udivmoddi4+0x2b0>
 8012a9c:	458c      	cmp	ip, r1
 8012a9e:	d92f      	bls.n	8012b00 <__udivmoddi4+0x2b0>
 8012aa0:	3f02      	subs	r7, #2
 8012aa2:	4429      	add	r1, r5
 8012aa4:	eba1 010c 	sub.w	r1, r1, ip
 8012aa8:	fbb1 f0f8 	udiv	r0, r1, r8
 8012aac:	fb08 1c10 	mls	ip, r8, r0, r1
 8012ab0:	fb00 fe09 	mul.w	lr, r0, r9
 8012ab4:	b299      	uxth	r1, r3
 8012ab6:	ea41 410c 	orr.w	r1, r1, ip, lsl #16
 8012aba:	458e      	cmp	lr, r1
 8012abc:	d907      	bls.n	8012ace <__udivmoddi4+0x27e>
 8012abe:	1869      	adds	r1, r5, r1
 8012ac0:	f100 33ff 	add.w	r3, r0, #4294967295	; 0xffffffff
 8012ac4:	d218      	bcs.n	8012af8 <__udivmoddi4+0x2a8>
 8012ac6:	458e      	cmp	lr, r1
 8012ac8:	d916      	bls.n	8012af8 <__udivmoddi4+0x2a8>
 8012aca:	3802      	subs	r0, #2
 8012acc:	4429      	add	r1, r5
 8012ace:	eba1 010e 	sub.w	r1, r1, lr
 8012ad2:	ea40 4707 	orr.w	r7, r0, r7, lsl #16
 8012ad6:	e73a      	b.n	801294e <__udivmoddi4+0xfe>
 8012ad8:	4637      	mov	r7, r6
 8012ada:	4630      	mov	r0, r6
 8012adc:	e708      	b.n	80128f0 <__udivmoddi4+0xa0>
 8012ade:	460f      	mov	r7, r1
 8012ae0:	e6e6      	b.n	80128b0 <__udivmoddi4+0x60>
 8012ae2:	4618      	mov	r0, r3
 8012ae4:	e6fb      	b.n	80128de <__udivmoddi4+0x8e>
 8012ae6:	4543      	cmp	r3, r8
 8012ae8:	d2ab      	bcs.n	8012a42 <__udivmoddi4+0x1f2>
 8012aea:	ebb8 0102 	subs.w	r1, r8, r2
 8012aee:	eb69 020c 	sbc.w	r2, r9, ip
 8012af2:	3801      	subs	r0, #1
 8012af4:	4696      	mov	lr, r2
 8012af6:	e7a4      	b.n	8012a42 <__udivmoddi4+0x1f2>
 8012af8:	4618      	mov	r0, r3
 8012afa:	e7e8      	b.n	8012ace <__udivmoddi4+0x27e>
 8012afc:	4608      	mov	r0, r1
 8012afe:	e795      	b.n	8012a2c <__udivmoddi4+0x1dc>
 8012b00:	4607      	mov	r7, r0
 8012b02:	e7cf      	b.n	8012aa4 <__udivmoddi4+0x254>
 8012b04:	4681      	mov	r9, r0
 8012b06:	e77b      	b.n	8012a00 <__udivmoddi4+0x1b0>
 8012b08:	3802      	subs	r0, #2
 8012b0a:	442c      	add	r4, r5
 8012b0c:	e747      	b.n	801299e <__udivmoddi4+0x14e>
 8012b0e:	f1ae 0e02 	sub.w	lr, lr, #2
 8012b12:	442b      	add	r3, r5
 8012b14:	e72f      	b.n	8012976 <__udivmoddi4+0x126>
 8012b16:	4638      	mov	r0, r7
 8012b18:	e707      	b.n	801292a <__udivmoddi4+0xda>
 8012b1a:	4637      	mov	r7, r6
 8012b1c:	e6e8      	b.n	80128f0 <__udivmoddi4+0xa0>
 8012b1e:	bf00      	nop

08012b20 <__aeabi_idiv0>:
 8012b20:	4770      	bx	lr
 8012b22:	bf00      	nop

08012b24 <memcpy>:
 8012b24:	b510      	push	{r4, lr}
 8012b26:	1e43      	subs	r3, r0, #1
 8012b28:	440a      	add	r2, r1
 8012b2a:	4291      	cmp	r1, r2
 8012b2c:	d100      	bne.n	8012b30 <memcpy+0xc>
 8012b2e:	bd10      	pop	{r4, pc}
 8012b30:	f811 4b01 	ldrb.w	r4, [r1], #1
 8012b34:	f803 4f01 	strb.w	r4, [r3, #1]!
 8012b38:	e7f7      	b.n	8012b2a <memcpy+0x6>

08012b3a <memset>:
 8012b3a:	4603      	mov	r3, r0
 8012b3c:	4402      	add	r2, r0
 8012b3e:	4293      	cmp	r3, r2
 8012b40:	d100      	bne.n	8012b44 <memset+0xa>
 8012b42:	4770      	bx	lr
 8012b44:	f803 1b01 	strb.w	r1, [r3], #1
 8012b48:	e7f9      	b.n	8012b3e <memset+0x4>

08012b4a <strcat>:
 8012b4a:	4603      	mov	r3, r0
 8012b4c:	b510      	push	{r4, lr}
 8012b4e:	781a      	ldrb	r2, [r3, #0]
 8012b50:	1c5c      	adds	r4, r3, #1
 8012b52:	b93a      	cbnz	r2, 8012b64 <strcat+0x1a>
 8012b54:	3b01      	subs	r3, #1
 8012b56:	f811 2b01 	ldrb.w	r2, [r1], #1
 8012b5a:	f803 2f01 	strb.w	r2, [r3, #1]!
 8012b5e:	2a00      	cmp	r2, #0
 8012b60:	d1f9      	bne.n	8012b56 <strcat+0xc>
 8012b62:	bd10      	pop	{r4, pc}
 8012b64:	4623      	mov	r3, r4
 8012b66:	e7f2      	b.n	8012b4e <strcat+0x4>

08012b68 <strlen>:
 8012b68:	4603      	mov	r3, r0
 8012b6a:	f813 2b01 	ldrb.w	r2, [r3], #1
 8012b6e:	2a00      	cmp	r2, #0
 8012b70:	d1fb      	bne.n	8012b6a <strlen+0x2>
 8012b72:	1a18      	subs	r0, r3, r0
 8012b74:	3801      	subs	r0, #1
 8012b76:	4770      	bx	lr

08012b78 <strncat>:
 8012b78:	4603      	mov	r3, r0
 8012b7a:	b530      	push	{r4, r5, lr}
 8012b7c:	781c      	ldrb	r4, [r3, #0]
 8012b7e:	1c5d      	adds	r5, r3, #1
 8012b80:	b944      	cbnz	r4, 8012b94 <strncat+0x1c>
 8012b82:	f112 32ff 	adds.w	r2, r2, #4294967295	; 0xffffffff
 8012b86:	d304      	bcc.n	8012b92 <strncat+0x1a>
 8012b88:	f811 4b01 	ldrb.w	r4, [r1], #1
 8012b8c:	f803 4b01 	strb.w	r4, [r3], #1
 8012b90:	b914      	cbnz	r4, 8012b98 <strncat+0x20>
 8012b92:	bd30      	pop	{r4, r5, pc}
 8012b94:	462b      	mov	r3, r5
 8012b96:	e7f1      	b.n	8012b7c <strncat+0x4>
 8012b98:	2a00      	cmp	r2, #0
 8012b9a:	d1f2      	bne.n	8012b82 <strncat+0xa>
 8012b9c:	701a      	strb	r2, [r3, #0]
 8012b9e:	e7f0      	b.n	8012b82 <strncat+0xa>
 8012ba0:	20202020 	.word	0x20202020
 8012ba4:	20202020 	.word	0x20202020
 8012ba8:	20202020 	.word	0x20202020
 8012bac:	20202020 	.word	0x20202020
 8012bb0:	20202020 	.word	0x20202020
 8012bb4:	20202020 	.word	0x20202020
 8012bb8:	20202020 	.word	0x20202020
 8012bbc:	20202020 	.word	0x20202020
 8012bc0:	20202020 	.word	0x20202020
 8012bc4:	20202020 	.word	0x20202020
 8012bc8:	20202000 	.word	0x20202000
 8012bcc:	20202020 	.word	0x20202020
 8012bd0:	69676f4c 	.word	0x69676f4c
 8012bd4:	6e412063 	.word	0x6e412063
 8012bd8:	7a796c61 	.word	0x7a796c61
 8012bdc:	41207265 	.word	0x41207265
 8012be0:	20005050 	.word	0x20005050
 8012be4:	20202020 	.word	0x20202020
 8012be8:	53202020 	.word	0x53202020
 8012bec:	65747379 	.word	0x65747379
 8012bf0:	6e49206d 	.word	0x6e49206d
 8012bf4:	61697469 	.word	0x61697469
 8012bf8:	697a696c 	.word	0x697a696c
 8012bfc:	2e2e676e 	.word	0x2e2e676e
 8012c00:	2020202e 	.word	0x2020202e
 8012c04:	20202020 	.word	0x20202020
 8012c08:	00000000 	.word	0x00000000

08012c0c <gInputSubParamMin>:
	...
 8012c18:	00000008 00000000 00000000 00010800     ................
 8012c28:	00000000                                ....

08012c2c <gInputSubParamMax>:
	...
 8012c34:	01010101 00000208 01010201 01010101     ................
 8012c44:	00010807 01020001                       ........

08012c4c <gOutSubParamMin>:
 8012c4c:	01010600 01010600 00000000 00000000     ................
	...

08012c6c <gOutSubParamMax>:
 8012c6c:	09090f01 09090f01 00000000 00000000     ................
	...

08012c8c <gItemParamMin>:
	...
 8012ca0:	00000005 00000000 00000000 00000000     ................
 8012cb0:	00000000 00010001 00010001 0000000c     ................
	...
 8012d08:	00010000 00000000 00000000 00000000     ................

08012d18 <gItemParamMax>:
 8012d18:	00000003 00000000 00000000 00000000     ................
 8012d28:	00000000 01130013 00000113 00000000     ................
	...
 8012d40:	00040007 00370004 0000001e 00000000     ......7.........
 8012d50:	00000000 00030003 00000000 00000000     ................
	...
 8012d68:	00000003 00000000 00000000 00000000     ................
 8012d78:	00000000 00630063 00630063 00630063     ....c.c.c.c.c.c.
 8012d88:	00630063 00630063 00000000 000a000a     c.c.c.c.........
 8012d98:	003c001e 00000000 00000000              ..<.........

08012da4 <LeftTitleCol>:
 8012da4:	07ffffe0 07e0f81f fc10051f              ............

08012db0 <ProtocolStr>:
 8012db0:	20314843 48432020 20202032 20334843     CH1   CH2   CH3 
 8012dc0:	48432020 20202034 49534f4d 494d2020       CH4   MOSI  MI
 8012dd0:	20204f53 204b4353 53432020 20202020     SO  SCK   CS    
 8012de0:	20414453 43532020 2020204c 20202020     SDA   SCL       
 8012df0:	20202020 20202020 54524155 20202020             UART    
 8012e00:	20202020 20202020 20202020 20202020                     
 8012e10:	20202b44 2d442020 20202020 20202020     D+    D-        
 8012e20:	20202020 20202020 204e4143 20202020             CAN     
 8012e30:	20202020 20202020 20202020 20202020                     
 8012e40:	35584d44 20203231 20202020 20202020     DMX512          
 8012e50:	20202020 20202020 4c435753 5753204b             SWCLK SW
 8012e60:	204f4944 20202020 20202020 20202020     DIO             

08012e70 <gInSubMaxNum>:
 8012e70:	06010500                                ....

08012e74 <gOutSubMaxNum>:
 8012e74:	00000003                                ....

08012e78 <User_SetStr>:
 8012e78:	20202020 20202020 20202020 00202020                    .
 8012e88:	20202020 20202020 20202020 00202020                    .

08012e98 <DateFormatStr>:
 8012e98:	69637341 20002069 20434544 48200020     Ascii . DEC  . H
 8012ea8:	20205845 49422000 0020204e              EX  . BIN  .

08012eb4 <SPI_SetStr>:
 8012eb4:	45205343 6c62616e 20203a65 00202020     CS Enable:     .
 8012ec4:	636f6c43 6f50206b 6972616c 003a7974     Clock Polarity:.
 8012ed4:	636f6c43 6850206b 3a657361 00202020     Clock Phase:   .
 8012ee4:	20746942 6564724f 20203a72 00202020     Bit Order:     .
 8012ef4:	64726f57 7a695320 20203a65 00202020     Word Size:     .
 8012f04:	61746144 726f4620 3a74616d 00202020     Data Format:   .

08012f14 <ITEM_STStr>:
 8012f14:	6f4e2020 59200020 00207365                No . Yes .

08012f20 <I2C_SetStr>:
 8012f20:	20572f52 20746962 20746553 00203a30     R/W bit Set 0: .
 8012f30:	61746144 726f4620 3a74616d 00202020     Data Format:   .

08012f40 <BaudRateStr>:
 8012f40:	30383420 20002030 30303639 31200020      4800 . 9600 . 1
 8012f50:	30303434 39312000 00303032 34383320     4400. 19200. 384
 8012f60:	20003030 30303635 35200030 30303637     00. 56000. 57600
 8012f70:	35313100 00303032                       .115200.

08012f78 <StopBitsStr>:
 8012f78:	2e302020 20002030 302e3120 20200020       0.0 .  1.0 .  
 8012f88:	20352e31 32202000 0020302e              1.5 .  2.0 .

08012f94 <ParityTypeStr>:
 8012f94:	6e6f4e20 20002065 64644f20 45200020      None .  Odd . E
 8012fa4:	206e6576                                 ven .

08012fa9 <UART_SetStr>:
 8012fa9:	64756142 74615220 20203a65 00202020     Baud Rate:     .
 8012fb9:	61746144 74694220 20203a73 00202020     Data Bits:     .
 8012fc9:	706f7453 74694220 20203a73 00202020     Stop Bits:     .
 8012fd9:	69726150 54207974 3a657079 00202020     Parity Type:   .
 8012fe9:	20746942 6564724f 20203a72 00202020     Bit Order:     .
 8012ff9:	65766e49 53207472 616e6769 00203a6c     Invert Signal: .
 8013009:	61746144 726f4620 3a74616d 00202020     Data Format:   .

08013019 <Freq_Str>:
 8013019:	48303120 2000207a 7a483032 35200020      10Hz . 20Hz . 5
 8013029:	207a4830 30312000 007a4830 30303220     0Hz . 100Hz. 200
 8013039:	20007a48 48303035 3120007a 207a484b     Hz. 500Hz. 1KHz 
 8013049:	4b322000 00207a48 484b3520 2000207a     . 2KHz . 5KHz . 
 8013059:	484b3031 3220007a 7a484b30 30352000     10KHz. 20KHz. 50
 8013069:	007a484b 4b303031 32007a48 484b3030     KHz.100KHz.200KH
 8013079:	3035007a 7a484b30 4d312000 00207a48     z.500KHz. 1MHz .

08013089 <PWM_SetStr>:
 8013089:	314d5750 616e4520 3a656c62 00202020     PWM1 Enable:   .
 8013099:	314d5750 65724620 20203a71 00202020     PWM1 Freq:     .
 80130a9:	314d5750 31484320 74754420 00203a79     PWM1 CH1 Duty: .
 80130b9:	314d5750 32484320 74754420 00203a79     PWM1 CH2 Duty: .
 80130c9:	324d5750 616e4520 3a656c62 00202020     PWM2 Enable:   .
 80130d9:	324d5750 65724620 20203a71 00202020     PWM2 Freq:     .
 80130e9:	324d5750 31484320 74754420 00203a79     PWM2 CH1 Duty: .
 80130f9:	324d5750 32484320 74754420 00203a79     PWM2 CH2 Duty: .
	...

0801310a <BT_C>:
 801310a:	07ff001f ffffffff bde0ffff               ..............

08013118 <BT_S>:
 8013118:	003d3c3b 3d3c3b00 263b0000 3b000040     ;<=..;<=..;&@..;
 8013128:	0000402a 0040603e 24232200 23220000     *@..>`@.."#$.."#
 8013138:	                                         $..

0801313b <InputTypeStr>:
 801313b:	72657355 49505300 32490020 55002043     User.SPI .I2C .U
 801314b:	00545241 20425355 4e414300 4d440020     ART.USB .CAN .DM
 801315b:	53002058 00204457                       X .SWD .

08013163 <TimeStr>:
 8013163:	54004254 32540031 00582000 5a003054     TB.T1.T2. X.T0.Z
 8013173:	                                         C.

08013175 <TriCondStr>:
 8013175:	006e6f4e 00605e20 003d3d20 00796e41     Non. ^`. ==.Any.
 8013185:	00605e20 003d3d20 00605e20 003d3d20      ^`. ==. ^`. ==.

08013195 <OutputTypeStr>:
 8013195:	4d575020 50532000 49200049 55004332      PWM. SPI. I2C.U
 80131a5:	00545241                                ART.

080131a9 <TimeUnitStr>:
 80131a9:	0000536e 53750000 00000000 0000536d     nS....uS....mS..
	...

080131bb <TB_Str>:
 80131bb:	6e302e31 00002053 6e302e32 00002053     1.0nS ..2.0nS ..
 80131cb:	6e302e35 00002053 536e3031 00002020     5.0nS ..10nS  ..
 80131db:	536e3032 00002020 536e3031 00002020     20nS  ..10nS  ..
 80131eb:	536e3032 00002020 536e3035 00002020     20nS  ..50nS  ..
 80131fb:	75312e30 00002053 75322e30 00002053     0.1uS ..0.2uS ..
 801320b:	75352e30 00002053 75302e31 00002053     0.5uS ..1.0uS ..
 801321b:	75302e32 00002053 75302e35 00002053     2.0uS ..5.0uS ..
 801322b:	53753031 00002020 53753032 00002020     10uS  ..20uS  ..
 801323b:	53753035 00002020 6d312e30 00002053     50uS  ..0.1mS ..
 801324b:	6d322e30 00002053 6d352e30 00002053     0.2mS ..0.5mS ..

0801325b <CSWTCH.119>:
 801325b:	0e000c0b                                 .....

08013260 <CSWTCH.120>:
 8013260:	02000100 00000003                       ........

08013268 <gTB_Scale>:
 8013268:	00000001 00000002 00000005 00000001     ................
 8013278:	00000002 00000001 00000002 00000005     ................
 8013288:	0000000a 00000014 00000032 00000064     ........2...d...
 8013298:	000000c8 000001f4 000003e8 000007d0     ................
 80132a8:	00001388 00002710 00004e20 0000c350     .....'.. N..P...

080132b8 <ChannelStr>:
 80132b8:	00314843 00324843 00334843 00344843     CH1.CH2.CH3.CH4.

080132c8 <OUT_PWM_Str>:
 80132c8:	203a3150 314d5750 3148435f 32500020     P1: PWM1_CH1 .P2
 80132d8:	5750203a 435f314d 00203248 203a3350     : PWM1_CH2 .P3: 
 80132e8:	20202020 20202020 34500020 2020203a              .P4:   
 80132f8:	20202020 00202020                              .

08013300 <OUT_SPI_Str>:
 8013300:	203a3150 5f495053 49534f4d 32500020     P1: SPI_MOSI .P2
 8013310:	5053203a 494d5f49 00204f53 203a3350     : SPI_MISO .P3: 
 8013320:	5f495053 204b4353 34500020 5053203a     SPI_SCK  .P4: SP
 8013330:	53435f49 00202020                       I_CS   .

08013338 <OUT_I2C_Str>:
 8013338:	203a3150 5f433249 20414453 32500020     P1: I2C_SDA  .P2
 8013348:	3249203a 43535f43 0020204c 203a3350     : I2C_SCL  .P3: 
 8013358:	20202020 20202020 34500020 2020203a              .P4:   
 8013368:	20202020 00202020                              .

08013370 <OUT_UART_Str>:
 8013370:	203a3150 54524155 2058545f 32500020     P1: UART_TX  .P2
 8013380:	2020203a 20202020 00202020 203a3350     :          .P3: 
 8013390:	20202020 20202020 34500020 2020203a              .P4:   
 80133a0:	20202020 00202020                              .

080133a8 <gItemMaxNum>:
 80133a8:	01040300                                 .......

080133af <InputStr>:
 80133af:	75706e49 20203a74 00002020               Input:    ...

080133bc <TimeBaseStr>:
 80133bc:	302e3120 0020536e 302e3220 0020536e      1.0nS . 2.0nS .
 80133cc:	302e3520 0020536e 30312020 0020536e      5.0nS .  10nS .
 80133dc:	30322020 0020536e 30312020 0020536e       20nS .  10nS .
 80133ec:	30322020 0020536e 30352020 0020536e       20nS .  50nS .
 80133fc:	312e3020 00205375 322e3020 00205375      0.1uS . 0.2uS .
 801340c:	352e3020 00205375 302e3120 00205375      0.5uS . 1.0uS .
 801341c:	302e3220 00205375 302e3520 00205375      2.0uS . 5.0uS .
 801342c:	30312020 00205375 30322020 00205375       10uS .  20uS .
 801343c:	30352020 00205375 312e3020 0020536d       50uS . 0.1mS .
 801344c:	322e3020 0020536d 352e3020 0020536d      0.2mS . 0.5mS .

0801345c <TimeSetStr>:
 801345c:	656d6954 65736142 0000203a 2e315400     TimeBase: ...T1.
 801346c:	69736f50 0020203a 32540000 736f502e     Posi:  ...T2.Pos
 801347c:	20203a69 58000000 6f502e20 203a6973     i:  ...X .Posi: 
 801348c:	00000020 502e3054 3a69736f 00002020      ...T0.Posi:  ..
 801349c:	2e435a00 69736f50 0020203a               .ZC.Posi:  ...

080134aa <TriTypeStr>:
 80134aa:	6f4e2020 0020656e 5f454e20 00204d43       None . NE_CM .
 80134ba:	5f514520 00204d43 41202020 0020796e      EQ_CM .   Any .
 80134ca:	454e4c20 0020575f 51454c20 0020575f      LNE_W . LEQ_W .
 80134da:	454e4720 0020575f 51454720 0020575f      GNE_W . GEQ_W .

080134ea <TriggerStr>:
 80134ea:	54697254 3a657079 00002020 69725400     TriType:  ...Tri
 80134fa:	646e6f43 0020203a 72540000 73614d69     Cond:  ...TriMas
 801350a:	20203a6b 54000000 69576972 3a687464     k:  ...TriWidth:
 801351a:	00000020 4c676f4c 6c657665 0000203a      ...LogLevel: ..
	...

0801352c <CondMaskCol>:
 801352c:	ffe0bde0 f81f07ff bde007e0              ............

08013538 <SourceStr>:
 8013538:	5f484320 20002031 325f4843 43200020      CH_1 . CH_2 . C
 8013548:	20335f48 48432000 0020345f              H_3 . CH_4 .

08013554 <MeasureStr>:
 8013554:	72756f53 3a316563 00002020 756f5300     Source1:  ...Sou
 8013564:	32656372 0020203a 6f530000 65637275     rce2:  ...Source
 8013574:	20203a33                                 3:  ...

0801357b <OutputStr>:
 801357b:	7074754f 203a7475 00002020               Output:   ...

08013588 <FileSysStr>:
 8013588:	65766153 3a706d42 00002020 76615300     SaveBmp:  ...Sav
 8013598:	76734365 0020203a 61530000 76536576     eCsv:  ...SaveSv
 80135a8:	20203a67 53000000 42657661 203a6675     g:  ...SaveBuf: 
 80135b8:	00000020                                 ...

080135bc <SettingStr>:
 80135bc:	65766153 2020203a 00002020 73655200     Save:     ...Res
 80135cc:	65726f74 0020203a 6f560000 656d756c     tore:  ...Volume
 80135dc:	2020203a 42000000 67696c4b 203a7468     :   ...BKlight: 
 80135ec:	00000020 6e617453 3a796264 00002020      ...Standby:  ..
 80135fc:	776f5000 664f7265 00203a66               .PowerOff: ...

0801360a <gItemParamStep>:
 801360a:	00010001 00010001 00010001 00010001     ................
 801361a:	00010001 00010001 00010001 00050001     ................
 801362a:	00010001 00010001 00010001 00010001     ................
 801363a:	00010001 00010001 00010001 00010001     ................
 801364a:	00010001 00010001 00010001 00010001     ................
 801365a:	00010001 00010001 00010001 00010001     ................
 801366a:	00010001 00010001 00010001 00010001     ................
 801367a:	00010001 00010001 00010001 00010001     ................
 801368a:	00010001 00010001 00010001              ............

08013696 <gItemParamBackup>:
 8013696:	00000001 00000000 00000000 00000000     ................
 80136a6:	00000000 00320008 000000e6 012c0002     ......2.......,.
	...
 80136be:	00010002 00010701 0000001e 00000000     ................
 80136ce:	00000000 00010000 00000000 00000000     ................
	...
 80136e6:	00000001 00000000 00000000 00000000     ................
	...
 8013712:	00050005 001e0005 00000000 00000000     ................

08013722 <gOutSubParamBackup>:
 8013722:	05050a00 05050a00 00000000 00000000     ................
	...

08013742 <gInputSubParamBackup>:
	...
 801374a:	00010100 00000008 00000200 00000000     ................
 801375a:	00010807 00000000                       ........

08013762 <gOutSubIndexNumBackup>:
 8013762:	00000000                                ....

08013766 <gInSubIndexNumBackup>:
 8013766:	00000000                                ....

0801376a <gItemIndexNumBackup>:
 801376a:	00000000                                 .......

08013771 <ItemNum>:
 8013771:	02050402                                 .......

08013778 <PercentStr>:
 8013778:	25302020 20202020 31200020 20202530       0%     . 10%  
 8013788:	00202020 25303220 20202020 33200020        . 20%     . 3
 8013798:	20202530 00202020 25303420 20202020     0%     . 40%    
 80137a8:	35200020 20202530 00202020 25303620      . 50%     . 60%
 80137b8:	20202020 37200020 20202530 00202020          . 70%     .
 80137c8:	25303820 20202020 39200020 20202530      80%     . 90%  
 80137d8:	00202020 30303120 20202025                  . 100%    .

080137e6 <V_BT>:
 80137e6:	0ce40cb2 0e740d48 10680f3c               ....H.t.<.h.h.

080137f4 <Switch_Str>:
 80137f4:	464f2020 20002046 204e4f20                 OFF .  ON  .

08013802 <VoltageStr>:
 8013802:	0020566d 20560000 00000020 0020566b     mV ...V  ...kV .
	...

08013814 <CursorCol>:
 8013814:	ffffffff 051f07e0                       ........

0801381c <TitlePosi>:
 801381c:	00160000 007d0124 4d5c007e 6e41004e     ....$.}.~.\MN.An
 801382c:	4a490079 004c4b00 43004241 48470044     y.IJ.KL.AB.CD.GH
 801383c:	3e003c00 64312500 54414400 20200041     .<.>.%1d.DATA.  
 801384c:	00202031 20302020 20200020 2042534d     1  .  0  .  MSB 
 801385c:	4c202000 00204253 65592020 20002073     .  LSB .  Yes . 
 801386c:	206f4e20 20200020 20204e4f 4f202000      No  .  ON  .  O
 801387c:	00204646 00643325 00643425 7c002025     FF .%3d.%4d.% .|
 801388c:	49007b00 54003a6e 3a656d69 61654d00     .{.In:.Time:.Mea
 801389c:	65727573 74754f00 6c694600 73795365     sure.Out.FileSys
 80138ac:	74655300 676e6974 61654d00 00727573     .Setting.Measur.
 80138bc:	57505f50 006d7553 57502d4e 006d7553     P_PWSum.N-PWSum.
 80138cc:	754e7055 0020206d 6e776f44 006d754e     UpNum  .DownNum.
 80138dc:	2d3a3154 2d2d2d2d 3254002d 2d2d2d3a     T1:------.T2:---
 80138ec:	002d2d2d 3a54005d 2d2d2d2d 25002d2d     ---.].T:------.%
 80138fc:	56006435 6b6f0020 006e6d00 72615020     5d.V .ok.mn. Par
 801390c:	20206d61 20202500 6e696d00               am  .%  .min .

0801391a <Font8x14>:
 801391a:	00000000 3f601c00 1c003f60 00000000     ......`?`?......
 801392a:	70000000 00007800 00000000 70007800     ...p.x.......x.p
 801393a:	08800000 3fe03fe0 3fe00880 08803fe0     .....?.?...?.?..
 801394a:	1cc00000 22203e60 e238e238 19c033e0     ....`> "8.8..3..
 801395a:	0c200000 00c00c60 03000180 0c600660     .. .`.......`.`.
 801396a:	01c00000 3e201be0 3dc02720 02201be0     ...... > '.=.. .
 801397a:	08000000 70007800 00000000 00000000     .....x.p........
 801398a:	00000000 1fc00f80 20203060 00000000     ........`0  ....
 801399a:	00000000 20200000 1fc03060 00000f80     ......  `0......
 80139aa:	0a800200 07000f80 0f800700 02000a80     ................
 80139ba:	02000200 1fc00200 02001fc0 02000200     ................
 80139ca:	00000000 00f00010 000000e0 00000000     ................
 80139da:	02000200 02000200 02000200 02000200     ................
 80139ea:	00000000 00600000 00000060 00000000     ......`.`.......
 80139fa:	00c00000 03000180 0c000600 30001800     ...............0
 8013a0a:	1fc00000 23203fe0 2c202620 1fc03fe0     .....? # & ,.?..
 8013a1a:	00000000 18200820 3fe03fe0 00200020     .... . ..?.? . .
 8013a2a:	10600000 21a030e0 26202320 18603c60     ..`..0.! # &`<`.
 8013a3a:	10400000 22203060 22202220 1dc03fe0     ..@.`0 " " ".?..
 8013a4a:	03000000 0d000700 3fe01920 01203fe0     ........ ..?.? .
 8013a5a:	3e400000 22203e60 22202220 21c023e0     ..@>`> " " ".#.!
 8013a6a:	0fc00000 32201fe0 22202220 01c003e0     ...... 2 " "....
 8013a7a:	30000000 21e03000 260023e0 38003c00     ...0.0.!.#.&.<.8
 8013a8a:	1dc00000 22203fe0 22202220 1dc03fe0     .....? " " ".?..
 8013a9a:	1c000000 22203e20 22602220 1f803fc0     .... > " "`".?..
 8013aaa:	00000000 18c00000 000018c0 00000000     ................
 8013aba:	00000000 18e00020 000018c0 00000000     .... ...........
 8013aca:	02000000 0d800700 306018c0 00002020     ..........`0  ..
 8013ada:	04800000 04800480 04800480 00000480     ................
 8013aea:	20200000 18c03060 07000d80 00000200     ..  `0..........
 8013afa:	18000000 20003800 27602360 18003c00     .....8. `#`'.<..
 8013b0a:	1fc00000 20203fe0 27a027a0 1f003fa0     .....?  .'.'.?..
 8013b1a:	07e00000 19000fe0 19003100 07e00fe0     .........1......
 8013b2a:	20200000 3fe03fe0 22202220 1dc03fe0     ..  .?.? " ".?..
 8013b3a:	0f800000 30601fc0 20202020 18c03060     ......`0    `0..
 8013b4a:	20200000 3fe03fe0 30602020 0f801fc0     ..  .?.?  `0....
 8013b5a:	20200000 3fe03fe0 27202220 38e03060     ..  .?.? " '`0.8
 8013b6a:	20200000 3fe03fe0 27002220 38003000     ..  .?.? ".'.0.8
 8013b7a:	0f800000 30601fc0 21202120 19e031c0     ......`0 ! !.1..
 8013b8a:	3fe00000 02003fe0 02000200 3fe03fe0     ...?.?.......?.?
 8013b9a:	00000000 3fe02020 20203fe0 00000000     ....  .?.?  ....
 8013baa:	00c00000 002000e0 3fe02020 20003fc0     ...... .  .?.?. 
 8013bba:	20200000 3fe03fe0 0f800200 30603de0     ..  .?.?.....=`0
 8013bca:	20200000 3fe03fe0 00202020 00e00060     ..  .?.?   .`...
 8013bda:	3fe03fe0 0c001800 18000c00 3fe03fe0     .?.?.........?.?
 8013bea:	3fe00000 1c003fe0 07000e00 3fe03fe0     ...?.?.......?.?
 8013bfa:	0f800000 30601fc0 30602020 0f801fc0     ......`0  `0....
 8013c0a:	20200000 3fe03fe0 22002220 1c003e00     ..  .?.? ".".>..
 8013c1a:	1f800000 20403fc0 20f021c0 1f903ff0     .....?@ .!. .?..
 8013c2a:	20200000 3fe03fe0 23002200 1ce03fe0     ..  .?.?.".#.?..
 8013c3a:	18c00000 26203ce0 23202220 18c039e0     .....< & " #.9..
 8013c4a:	30003800 3fe02020 20203fe0 38003000     .8.0  .?.?  .0.8
 8013c5a:	3fc00000 00203fe0 00200020 3fc03fe0     ...?.? . . ..?.?
 8013c6a:	3f803f00 006000c0 00c00060 3f003f80     .?.?..`.`....?.?
 8013c7a:	3fe03f80 038000e0 00e00380 3f803fe0     .?.?.........?.?
 8013c8a:	38e03060 07000d80 0d800700 306038e0     `0.8.........8`0
 8013c9a:	3c003800 03e00620 062003e0 38003c00     .8.< ..... ..<.8
 8013caa:	30e03860 232021a0 2c202620 30e03860     `8.0.! # & ,`8.0
 8013cba:	00000000 3fe03fe0 20202020 00000000     .....?.?    ....
 8013cca:	38000000 0e001c00 03800700 00e001c0     ...8............
 8013cda:	00000000 20202020 3fe03fe0 00000000     ....    .?.?....
 8013cea:	10000000 60003000 6000c000 10003000     .....0.`...`.0..
 8013cfa:	00080008 00080008 00080008 00080008     ................
 8013d0a:	00000000 c0000000 2000e000 00000000     ........... ....
 8013d1a:	00c00000 052005e0 07c00520 002003e0     ...... . ..... .
 8013d2a:	20200000 3fc03fe0 06200420 01c003e0     ..  .?.? . .....
 8013d3a:	03c00000 042007e0 04200420 02400660     ...... . . .`.@.
 8013d4a:	01c00000 062003e0 3fc02420 00203fe0     ...... . $.?.? .
 8013d5a:	03c00000 052007e0 05200520 03400760     ...... . . .`.@.
 8013d6a:	02200000 3fe01fe0 32002220 00001800     .. ....? ".2....
 8013d7a:	03900000 044807d8 03f80448 040007f0     ......H.H.......
 8013d8a:	20200000 3fe03fe0 04000200 03e007e0     ..  .?.?........
 8013d9a:	00000000 04200000 37e037e0 00000020     ...... ..7.7 ...
 8013daa:	00300000 00080038 37f80408 000037f0     ..0.8......7.7..
 8013dba:	20200000 3fe03fe0 03800100 046006e0     ..  .?.?......`.
 8013dca:	00000000 20200000 3fe03fe0 00000020     ......  .?.? ...
 8013dda:	07e007e0 03e00600 060003e0 03e007e0     ................
 8013dea:	04000000 03e007e0 04000400 03e007e0     ................
 8013dfa:	03c00000 042007e0 04200420 03c007e0     ...... . . .....
 8013e0a:	04080000 03f807f8 04400448 038007c0     ........H.@.....
 8013e1a:	03800000 044007c0 03f80448 040807f8     ......@.H.......
 8013e2a:	04200000 03e007e0 04000620 03000700     .. ..... .......
 8013e3a:	02400000 05200760 04a005a0 024006e0     ..@.`. .......@.
 8013e4a:	04000000 1fc00400 04203fe0 00400460     .........? .`.@.
 8013e5a:	07c00000 002007e0 07c00020 002007e0     ...... . ..... .
 8013e6a:	07800700 006000c0 00c00060 07000780     ......`.`.......
 8013e7a:	07e007c0 01c00060 006001c0 07c007e0     ....`.....`.....
 8013e8a:	04200000 03c00660 03c00180 04200660     .. .`.......`. .
 8013e9a:	07800000 004807c8 00580048 07e007f0     ......H.H.X.....
 8013eaa:	06200000 04e00660 072005a0 04600660     .. .`..... .`.`.
 8013eba:	02000000 1fc00200 20203de0 00002020     .........=    ..
 8013eca:	00000000 3de00000 00003de0 00000000     .......=.=......
 8013eda:	20200000 3de02020 02001fc0 00000200     ..    .=........
 8013eea:	10000000 20003000 10003000 20003000     .....0. .0...0. 
 8013efa:	01c00000 064003c0 06400c40 01c003c0     ......@.@.@.....

08013f0a <CHAR6x8>:
 8013f0a:	fafa0000 00000000 00e000e0 28fe2800     .............(.(
 8013f1a:	0c0028fe 30180e06 186c6600 4c006636     .(.....0.fl.6f.L
 8013f2a:	6a9e92fe e0200000 000000c0 82c67c38     ...j.. .....8|..
 8013f3a:	c6820000 5400387c 547c387c 7c101000     ....|8.T|8|T...|
 8013f4a:	00001010 00060701 10101000 00001010     ................
 8013f5a:	00060600 180c0600 7c006030 7cfe82fe     ........0`.|...|
 8013f6a:	fe420000 460002fe 66f29ace 92c64400     ..B....F...f.D..
 8013f7a:	18006cfe fefe4828 a2a2e200 7c009cbe     .l..(H.........|
 8013f8a:	1cbea2fe be8e8000 6c00c0f0 6cfe92fe     ...........l...l
 8013f9a:	8afa7000 00007cfe 00242400 27010000     .p...|...$$....'
 8013faa:	18000026 00ff7e3c 28282800 ff002828     &...<~...(((((..
 8013fba:	00183c7e 9aca4000 7c0060f0 74b282fe     ~<...@...`.|...t
 8013fca:	c87e3e00 fe003e7e 6cfe92fe 82fe7c00     .>~.~>.....l.|..
 8013fda:	fe0044c6 7cfe82fe 92fefe00 fe009292     .D.....|........
 8013fea:	909090fe 82fe7c00 fe005c92 fefe10fe     .....|...\......
 8013ffa:	fefe8200 04000082 80fcfe82 38fefe00     ...............8
 801400a:	fe00c6ee 020202fe 3c70fe00 fe00fe70     ..........p<p...
 801401a:	fe3870fe 82fe7c00 fe007cfe 708888fe     .p8..|...|.....p
 801402a:	86fc7800 fe007afe 66fc98fe 92f66400     .x...z.....f.d..
 801403a:	80804cde 8080fefe 02fefc00 f800fcfe     .L..............
 801404a:	f8fc06fc 781cfe00 c600fe1c c6ee38ee     .......x.....8..
 801405a:	1ef2e000 c600e0f2 e6f29a8e 7d0183ff     ...............}
 801406a:	60008301 060c1830 01bdffff 0000fd01     ...`0...........
 801407a:	38282c2a 02020200 68280202 000028a8     *,(8......(h.(..
 801408a:	2a2e0400 7e001e2a 0c1e127e 223e1c00     ...**..~~.....>"
 801409a:	0c002222 7e7e121e 2a3e1c00 1000183a     ""....~~..>*:...
 80140aa:	50507e3e 253d1a00 7e001e3f 0e1e107e     >~PP..=%?..~~...
 80140ba:	5e120000 0000025e 5e5f0101 087e7e00     ...^^....._^.~~.
 80140ca:	fc00361c 000202fe 1e303e00 3e003e30     .6.......>0.0>.>
 80140da:	1e3e203e 223e1c00 3f001c3e 183c243f     > >...>">..??$<.
 80140ea:	243c1800 3e003f3f 2020103e 2a3a1400     ..<$??.>>.  ..:*
 80140fa:	2000042e 02227e7c 023e3c00 38003e3e     ... |~"..<>.>>.8
 801410a:	383c063c 180c3e00 22003e0c 22361c36     <.<8.>...>."6.6"
 801411a:	053d3900 32003e3f 363a2e26 98ff1909     .9=.?>.2&.:6....
 801412a:	00000088 0000fe00 19ff9890 30000011     ...............0
 801413a:	30183060                                `0.0

0801413e <MiniChar6x8>:
 801413e:	2f000000 00000000 00070007 147f1400     .../............
 801414e:	2400147f 122a7f2a 08646200 36002313     ...$*.*..bd..#.6
 801415e:	50225549 03050000 00000000 0041221c     IU"P........."A.
 801416e:	22410000 1400001c 14083e08 3e080800     ..A".....>.....>
 801417e:	00000808 0060a000 08080800 00000808     ......`.........
 801418e:	00006060 08102000 3e000204 3e454951     ``... .....>QIE>
 801419e:	7f420000 42000040 46495161 45412100     ..B.@..BaQIF.!AE
 80141ae:	1800314b 107f1214 45452700 3c003945     K1.......'EEE9.<
 80141be:	3049494a 09710100 36000305 36494949     JII0..q....6III6
 80141ce:	49490600 00001e29 00003636 36560000     ..II)...66....V6
 80141de:	08000000 00412214 14141400 00001414     ....."A.........
 80141ee:	08142241 51010200 32000609 3e515949     A".....Q...2IYQ>
 80141fe:	11127c00 7f007c12 36494949 41413e00     .|...|..III6.>AA
 801420e:	7f002241 1c224141 49497f00 7f004149     A"..AA"...IIIA..
 801421e:	01090909 49413e00 7f007a49 7f080808     .....>AIIz......
 801422e:	7f410000 20000041 013f4140 14087f00     ..A.A.. @A?.....
 801423e:	7f004122 40404040 0c027f00 7f007f02     "A..@@@@........
 801424e:	7f100804 41413e00 7f003e41 06090909     .....>AAA>......
 801425e:	51413e00 7f005e21 46291909 49494600     .>AQ!^....)F.FII
 801426e:	01003149 01017f01 40403f00 1f003f40     I1.......?@@@?..
 801427e:	1f204020 38403f00 63003f40 63140814      @ ..?@8@?.c...c
 801428e:	70080700 61000708 43454951 417f0000     ...p...aQIEC...A
 801429e:	55000041 552a552a 41410000 0400007f     A..U*U*U..AA....
 80142ae:	04020102 40404000 00004040 00040201     .....@@@@@......
 80142be:	54542000 7f007854 38444448 44443800     . TTTx..HDD8.8DD
 80142ce:	38002044 7f484444 54543800 08001854     D .8DDH..8TTT...
 80142de:	0201097e a4a41800 7f007ca4 78040408     ~........|.....x
 80142ee:	7d440000 40000040 007d8480 28107f00     ..D}@..@..}....(
 80142fe:	00000044 00407f41 18047c00 7c007804     D...A.@..|...x.|
 801430e:	78040408 44443800 fc003844 18242424     ...x.8DDD8..$$$.
 801431e:	24241800 7c00fc18 08040408 54544800     ..$$...|.....HTT
 801432e:	04002054 2040443f 40403c00 1c007c20     T ..?D@ .<@@ |..
 801433e:	1c204020 30403c00 44003c40 44281028      @ ..<@0@<.D(.(D
 801434e:	a0a01c00 44007ca0 444c5464 14141414     .....|.DdTLD....
 801435e:	                                         ..

08014360 <SYMB6x8>:
 8014360:	fafa0000 38180000 30101038 d4989050     .......88..0P...
 8014370:	131712d2 10303810 186c6600 82826636     .....80..fl.6f..
 8014380:	baba8282 76331100 7e0098dc 99b1ff81     ......3v...~....
 8014390:	7e81ffb1 ba820000 babababa 7c101000     ...~...........|
 80143a0:	00001010 00060701 10101000 00001010     ................
 80143b0:	00060600 180c0600 7c006030 7cfe82fe     ........0`.|...|
 80143c0:	fe420000 460002fe 66f29ace 92c64400     ..B....F...f.D..
 80143d0:	18006cfe fefe4828 a2a2e200 7c009cbe     .l..(H.........|
 80143e0:	1cbea2fe be8e8000 6c00c0f0 6cfe92fe     ...........l...l
 80143f0:	8afa7000 00007cfe 00242400 82ee3800     .p...|...$$..8..
 8014400:	82828282 82828282 82828282 380000fe     ...............8
 8014410:	baba82ee 9aca4000 baba60f0 00fe82ba     .....@...`......
 8014420:	397d1110 ff8080ff 10117d39 9cbe8808     ..}9....9}......
 8014430:	ff0101ff 0888be9c 9cff0101 be8888be     ................
 8014440:	0101ff9c 39ff8080 7d11117d 8080ff39     .......9}..}9...
 8014450:	01000000 98ff1909 00008088 80000000     ................
 8014460:	19ff9890 00000111 c6000000 c6ee38ee     .............8..
 8014470:	00000000 82fe7c00 fe007cfe 708888fe     .....|...|.....p
 8014480:	86fc7800 fe007afe 66fc98fe 92f66400     .x...z.....f.d..
 8014490:	80804cde 8080fefe 02fefc00 f800fcfe     .L..............
 80144a0:	f8fc06fc 781cfe00 c600fe1c c6ee38ee     .......x.....8..
 80144b0:	1ef2e000 c600e0f2 e6f29a8e a5817e00     .............~..
 80144c0:	0000d3f3 00000000 3c1c0c00 50000c1c     ...........<...P
 80144d0:	120a7e48 7e504800 baba0a12 babababa     H~...HP~........
 80144e0:	2a2e0400 7e001e2a 0c1e127e 223e1c00     ...**..~~.....>"
 80144f0:	0c002222 7e7e121e 2a3e1c00 1000183a     ""....~~..>*:...
 8014500:	50507e3e 253d1a00 7e001e3f 0e1e107e     >~PP..=%?..~~...
 8014510:	5e120000 0000025e 5e5f0101 ad99bd81     ...^^....._^....
 8014520:	fc00ff81 000202fe aaaa00aa 0000aaaa     ................
 8014530:	00000000 bd81ff00 3f00bda5 183c243f     ...........??$<.
 8014540:	243c1800 3e003f3f 2020103e 2a3a1400     ..<$??.>>.  ..:*
 8014550:	2000042e 02227e7c 023e3c00 38003e3e     ... |~"..<>.>>.8
 8014560:	383c063c 180c3e00 22003e0c 22361c36     <.<8.>...>."6.6"
 8014570:	053d3900 32003e3f 363a2e26 7cfe0000     .9=.?>.2&.:6...|
 8014580:	fefe1038 fefe0000 14141414 16150014     8...............
 8014590:	0054341c                                .4T.

08014594 <Run_ICO>:
 8014594:	663c0000 c30101c3 00003c66 663c0000     ..<f....f<....<f
 80145a4:	038181c3 00001c06 663c0000 c38181c3     ..........<f....
 80145b4:	00002062 663c0000 c08181c3 00003c64     b ....<f....d<..
 80145c4:	663c0000 c38080c2 00003c66 60380000     ..<f....f<....8`
 80145d4:	c38181c1 00003c66 46040000 c38181c3     ....f<.....F....
 80145e4:	00003c66 263c0000 c3818103 00003c66     f<....<&....f<..

080145f4 <OUT_ARR>:
 80145f4:	6ddcdbb9 2bf157e3 232757e3 2327464f     ...m.W.+.W'#OF'#
 8014604:	1c1f383f 059f0e0f 016702cf 0047008f     ?8........g...G.

08014614 <OUT_PSC>:
 8014614:	007f007f 003f003f 000f000f 00030003     ....?.?.........
	...
 8014634:	3031414c 49505334 31414c00 41553430     LA104SPI.LA104UA
 8014644:	00005452                                RT..

08014648 <CSWTCH.49>:
 8014648:	001c1507                                ....

0801464c <BaudRate>:
 801464c:	00600030 00c00090 02300180 04800240     0.`.......0.@...
 801465c:	00000000 57003a52 4120003a 20004b43     ....R:.W:. ACK. 
 801466c:	004b414e 33323130 37363534 42413938     NAK.0123456789AB
 801467c:	46454443                                 CDEF..

08014682 <BmpHead>:
 8014682:	58384d42 00000002 00360000 00280000     BM8X......6...(.
 8014692:	01400000 00f00000 00010000 00000010     ..@.............
 80146a2:	58020000 0b120002 0b120000 00000000     ...X............
 80146b2:	00000000 204f0000 4500004b 00007272     ......O K..Err..
 80146c2:	00727245 72724500 72450000 00000072     Err..Err..Err...

080146d2 <BMP_Color>:
 80146d2:	ffe0ffff 07ffbde0 f81f05f7 07e0b817     ................
 80146e2:	7bef05e0 fc10051f 27e4001f 000039e7     ...{.......'.9..

080146f2 <CLK_TAB>:
	...
 80146fa:	03ff0070 00000070 00000000 00000000     p...p...........
 801470a:	00040002 00700008 00700070 01000080     ......p.p.p.....
 801471a:	00000200 00200020 00200020 00700070     .... . . . .p.p.
 801472a:	00200070 00200020 00000020 01000200     p. . . . .......
 801473a:	00700080 00700070 00040008 00000002     ..p.p.p.........
 801474a:	454c4946 20202020 00202020 5f474d49     FILE       .IMG_
 801475a:	20202020 00504d42 454c4946 20202020         BMP.FILE    
 801476a:	00544144 41544144 20202020 00465542     DAT.DATA    BUF.
 801477a:	41544144 20202020 00565343 41544144     DATA    CSV.DATA
 801478a:	20202020 00475653 656d6954 29536e28         SVG.Time(nS)
 801479a:	4843202c 43202c31 202c3248 2c334843     , CH1, CH2, CH3,
 80147aa:	34484320 0a0d002c 756e2800 00296c6c      CH4,....(null).

080147ba <Disk_Inquiry_Str>:
 80147ba:	02028000 00000020 696e694d 4f534420     .... ...Mini DSO
 80147ca:	6b736944 20202020 00000000 00000000     Disk    ........
 80147da:	00000000                                ....

080147de <MASS_StringInterface>:
 80147de:	00530310 00200054 0061004d 00730073     ..S.T. .M.a.s.s.

080147ee <MASS_StringProduct>:
 80147ee:	00530326 004d0054 00320033 004d0020     &.S.T.M.3.2. .M.
 80147fe:	00730061 00200073 00740053 0072006f     a.s.s. .S.t.o.r.
 801480e:	00670061                                 a.g.e.

08014814 <MASS_StringVendor>:
 8014814:	00530326 004d0054 00630069 006f0072     &.S.T.M.i.c.r.o.
 8014824:	006c0065 00630065 00720074 006e006f     e.l.e.c.t.r.o.n.
 8014834:	00630069                                 i.c.s.

0801483a <MASS_StringLangID>:
 801483a:	04090304                                ....

0801483e <MASS_ConfigDescriptor>:
 801483e:	00200209 c0000101 00040932 06080200     .. .....2.......
 801484e:	05070450 00400281 02050700 00004002     P.....@......@..

0801485e <MASS_DeviceDescriptor>:
 801485e:	02000112 40000000 57200483 02010200     .......@.. W....
 801486e:	                                         ..
