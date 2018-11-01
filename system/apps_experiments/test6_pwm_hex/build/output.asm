
output.elf:     file format elf32-littlearm


Disassembly of section .text:

20007000 <main>:
20007000:	e92d 47f3 	stmdb	sp!, {r0, r1, r4, r5, r6, r7, r8, r9, sl, lr}
20007004:	f000 f85f 	bl	200070c6 <RCCInit>
20007008:	f000 f892 	bl	20007130 <GPIOInit>
2000700c:	f240 30e7 	movw	r0, #999	; 0x3e7
20007010:	f000 f86c 	bl	200070ec <TimerInit>
20007014:	f04f 080a 	mov.w	r8, #10
20007018:	f000 f8a8 	bl	2000716c <PWMInit>
2000701c:	f04f 0a00 	mov.w	sl, #0
20007020:	f8df 909c 	ldr.w	r9, [pc, #156]	; 200070c0 <main+0xc0>
20007024:	2400      	movs	r4, #0
20007026:	4625      	mov	r5, r4
20007028:	4626      	mov	r6, r4
2000702a:	4627      	mov	r7, r4
2000702c:	4621      	mov	r1, r4
2000702e:	e00a      	b.n	20007046 <main+0x46>
20007030:	f5b4 7f7a 	cmp.w	r4, #1000	; 0x3e8
20007034:	db3a      	blt.n	200070ac <main+0xac>
20007036:	f5b4 6ffa 	cmp.w	r4, #2000	; 0x7d0
2000703a:	da28      	bge.n	2000708e <main+0x8e>
2000703c:	2500      	movs	r5, #0
2000703e:	462e      	mov	r6, r5
20007040:	f5a4 777a 	sub.w	r7, r4, #1000	; 0x3e8
20007044:	4629      	mov	r1, r5
20007046:	b289      	uxth	r1, r1
20007048:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
2000704c:	f000 ff5b 	bl	20007f06 <TIM_SetCompare3>
20007050:	b2b9      	uxth	r1, r7
20007052:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
20007056:	f000 ff58 	bl	20007f0a <TIM_SetCompare4>
2000705a:	b2b1      	uxth	r1, r6
2000705c:	4648      	mov	r0, r9
2000705e:	f000 ff52 	bl	20007f06 <TIM_SetCompare3>
20007062:	b2a9      	uxth	r1, r5
20007064:	4648      	mov	r0, r9
20007066:	f000 ff50 	bl	20007f0a <TIM_SetCompare4>
2000706a:	f242 720f 	movw	r2, #9999	; 0x270f
2000706e:	f8cd a004 	str.w	sl, [sp, #4]
20007072:	9b01      	ldr	r3, [sp, #4]
20007074:	4293      	cmp	r3, r2
20007076:	d91e      	bls.n	200070b6 <main+0xb6>
20007078:	3401      	adds	r4, #1
2000707a:	f5b4 6f7a 	cmp.w	r4, #4000	; 0xfa0
2000707e:	d1d7      	bne.n	20007030 <main+0x30>
20007080:	f1b8 0801 	subs.w	r8, r8, #1
20007084:	d1ce      	bne.n	20007024 <main+0x24>
20007086:	4640      	mov	r0, r8
20007088:	b002      	add	sp, #8
2000708a:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
2000708e:	f640 33b7 	movw	r3, #2999	; 0xbb7
20007092:	429c      	cmp	r4, r3
20007094:	dc04      	bgt.n	200070a0 <main+0xa0>
20007096:	2500      	movs	r5, #0
20007098:	f5a4 66fa 	sub.w	r6, r4, #2000	; 0x7d0
2000709c:	462f      	mov	r7, r5
2000709e:	e7d1      	b.n	20007044 <main+0x44>
200070a0:	2600      	movs	r6, #0
200070a2:	f6a4 35b8 	subw	r5, r4, #3000	; 0xbb8
200070a6:	4637      	mov	r7, r6
200070a8:	4631      	mov	r1, r6
200070aa:	e7cc      	b.n	20007046 <main+0x46>
200070ac:	2500      	movs	r5, #0
200070ae:	4621      	mov	r1, r4
200070b0:	462e      	mov	r6, r5
200070b2:	462f      	mov	r7, r5
200070b4:	e7c7      	b.n	20007046 <main+0x46>
200070b6:	9b01      	ldr	r3, [sp, #4]
200070b8:	3301      	adds	r3, #1
200070ba:	9301      	str	r3, [sp, #4]
200070bc:	e7d9      	b.n	20007072 <main+0x72>
200070be:	bf00      	nop
200070c0:	40000800 	.word	0x40000800

200070c4 <assert_failed>:
200070c4:	e7fe      	b.n	200070c4 <assert_failed>

200070c6 <RCCInit>:
200070c6:	b508      	push	{r3, lr}
200070c8:	2101      	movs	r1, #1
200070ca:	2008      	movs	r0, #8
200070cc:	f000 f9a6 	bl	2000741c <RCC_APB2PeriphClockCmd>
200070d0:	2101      	movs	r1, #1
200070d2:	4608      	mov	r0, r1
200070d4:	f000 f9ae 	bl	20007434 <RCC_APB1PeriphClockCmd>
200070d8:	2101      	movs	r1, #1
200070da:	4608      	mov	r0, r1
200070dc:	f000 f99e 	bl	2000741c <RCC_APB2PeriphClockCmd>
200070e0:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
200070e4:	2101      	movs	r1, #1
200070e6:	2004      	movs	r0, #4
200070e8:	f000 b9a4 	b.w	20007434 <RCC_APB1PeriphClockCmd>

200070ec <TimerInit>:
200070ec:	b51f      	push	{r0, r1, r2, r3, r4, lr}
200070ee:	2348      	movs	r3, #72	; 0x48
200070f0:	9301      	str	r3, [sp, #4]
200070f2:	2300      	movs	r3, #0
200070f4:	f8ad 0008 	strh.w	r0, [sp, #8]
200070f8:	a901      	add	r1, sp, #4
200070fa:	4c0c      	ldr	r4, [pc, #48]	; (2000712c <TimerInit+0x40>)
200070fc:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
20007100:	f8ad 300a 	strh.w	r3, [sp, #10]
20007104:	f88d 300c 	strb.w	r3, [sp, #12]
20007108:	f000 fb64 	bl	200077d4 <TIM_TimeBaseInit>
2000710c:	2101      	movs	r1, #1
2000710e:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
20007112:	f000 fcd1 	bl	20007ab8 <TIM_Cmd>
20007116:	a901      	add	r1, sp, #4
20007118:	4620      	mov	r0, r4
2000711a:	f000 fb5b 	bl	200077d4 <TIM_TimeBaseInit>
2000711e:	2101      	movs	r1, #1
20007120:	4620      	mov	r0, r4
20007122:	f000 fcc9 	bl	20007ab8 <TIM_Cmd>
20007126:	b004      	add	sp, #16
20007128:	bd10      	pop	{r4, pc}
2000712a:	bf00      	nop
2000712c:	40000800 	.word	0x40000800

20007130 <GPIOInit>:
20007130:	f44f 6340 	mov.w	r3, #3072	; 0xc00
20007134:	b537      	push	{r0, r1, r2, r4, r5, lr}
20007136:	2503      	movs	r5, #3
20007138:	f8ad 3004 	strh.w	r3, [sp, #4]
2000713c:	2318      	movs	r3, #24
2000713e:	4c0a      	ldr	r4, [pc, #40]	; (20007168 <GPIOInit+0x38>)
20007140:	a901      	add	r1, sp, #4
20007142:	4620      	mov	r0, r4
20007144:	f88d 3007 	strb.w	r3, [sp, #7]
20007148:	f88d 5006 	strb.w	r5, [sp, #6]
2000714c:	f001 f869 	bl	20008222 <GPIO_Init>
20007150:	f44f 7340 	mov.w	r3, #768	; 0x300
20007154:	a901      	add	r1, sp, #4
20007156:	4620      	mov	r0, r4
20007158:	f8ad 3004 	strh.w	r3, [sp, #4]
2000715c:	f88d 5006 	strb.w	r5, [sp, #6]
20007160:	f001 f85f 	bl	20008222 <GPIO_Init>
20007164:	b003      	add	sp, #12
20007166:	bd30      	pop	{r4, r5, pc}
20007168:	40010c00 	.word	0x40010c00

2000716c <PWMInit>:
2000716c:	b51f      	push	{r0, r1, r2, r3, r4, lr}
2000716e:	4b1b      	ldr	r3, [pc, #108]	; (200071dc <PWMInit+0x70>)
20007170:	2401      	movs	r4, #1
20007172:	9300      	str	r3, [sp, #0]
20007174:	2300      	movs	r3, #0
20007176:	4669      	mov	r1, sp
20007178:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
2000717c:	f8ad 3008 	strh.w	r3, [sp, #8]
20007180:	f8ad 4006 	strh.w	r4, [sp, #6]
20007184:	f000 fbf2 	bl	2000796c <TIM_OC3Init>
20007188:	2108      	movs	r1, #8
2000718a:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
2000718e:	f000 fd95 	bl	20007cbc <TIM_OC3PreloadConfig>
20007192:	4669      	mov	r1, sp
20007194:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
20007198:	f000 fc26 	bl	200079e8 <TIM_OC4Init>
2000719c:	2108      	movs	r1, #8
2000719e:	f04f 4080 	mov.w	r0, #1073741824	; 0x40000000
200071a2:	f000 fd93 	bl	20007ccc <TIM_OC4PreloadConfig>
200071a6:	4621      	mov	r1, r4
200071a8:	480d      	ldr	r0, [pc, #52]	; (200071e0 <PWMInit+0x74>)
200071aa:	f001 f8cd 	bl	20008348 <GPIO_PinRemapConfig>
200071ae:	4621      	mov	r1, r4
200071b0:	4c0c      	ldr	r4, [pc, #48]	; (200071e4 <PWMInit+0x78>)
200071b2:	480d      	ldr	r0, [pc, #52]	; (200071e8 <PWMInit+0x7c>)
200071b4:	f001 f8c8 	bl	20008348 <GPIO_PinRemapConfig>
200071b8:	4669      	mov	r1, sp
200071ba:	4620      	mov	r0, r4
200071bc:	f000 fbd6 	bl	2000796c <TIM_OC3Init>
200071c0:	4620      	mov	r0, r4
200071c2:	2108      	movs	r1, #8
200071c4:	f000 fd7a 	bl	20007cbc <TIM_OC3PreloadConfig>
200071c8:	4669      	mov	r1, sp
200071ca:	4620      	mov	r0, r4
200071cc:	f000 fc0c 	bl	200079e8 <TIM_OC4Init>
200071d0:	2108      	movs	r1, #8
200071d2:	4620      	mov	r0, r4
200071d4:	f000 fd7a 	bl	20007ccc <TIM_OC4PreloadConfig>
200071d8:	b004      	add	sp, #16
200071da:	bd10      	pop	{r4, pc}
200071dc:	00010060 	.word	0x00010060
200071e0:	00300200 	.word	0x00300200
200071e4:	40000800 	.word	0x40000800
200071e8:	00180300 	.word	0x00180300

200071ec <RCC_DeInit>:
200071ec:	4b0d      	ldr	r3, [pc, #52]	; (20007224 <RCC_DeInit+0x38>)
200071ee:	681a      	ldr	r2, [r3, #0]
200071f0:	f042 0201 	orr.w	r2, r2, #1
200071f4:	601a      	str	r2, [r3, #0]
200071f6:	6859      	ldr	r1, [r3, #4]
200071f8:	4a0b      	ldr	r2, [pc, #44]	; (20007228 <RCC_DeInit+0x3c>)
200071fa:	400a      	ands	r2, r1
200071fc:	605a      	str	r2, [r3, #4]
200071fe:	681a      	ldr	r2, [r3, #0]
20007200:	f022 7284 	bic.w	r2, r2, #17301504	; 0x1080000
20007204:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
20007208:	601a      	str	r2, [r3, #0]
2000720a:	681a      	ldr	r2, [r3, #0]
2000720c:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
20007210:	601a      	str	r2, [r3, #0]
20007212:	685a      	ldr	r2, [r3, #4]
20007214:	f422 02fe 	bic.w	r2, r2, #8323072	; 0x7f0000
20007218:	605a      	str	r2, [r3, #4]
2000721a:	f44f 021f 	mov.w	r2, #10420224	; 0x9f0000
2000721e:	609a      	str	r2, [r3, #8]
20007220:	4770      	bx	lr
20007222:	bf00      	nop
20007224:	40021000 	.word	0x40021000
20007228:	f8ff0000 	.word	0xf8ff0000

2000722c <RCC_HSEConfig>:
2000722c:	4b0c      	ldr	r3, [pc, #48]	; (20007260 <RCC_HSEConfig+0x34>)
2000722e:	f5b0 3f80 	cmp.w	r0, #65536	; 0x10000
20007232:	681a      	ldr	r2, [r3, #0]
20007234:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
20007238:	601a      	str	r2, [r3, #0]
2000723a:	681a      	ldr	r2, [r3, #0]
2000723c:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
20007240:	601a      	str	r2, [r3, #0]
20007242:	d003      	beq.n	2000724c <RCC_HSEConfig+0x20>
20007244:	f5b0 2f80 	cmp.w	r0, #262144	; 0x40000
20007248:	d005      	beq.n	20007256 <RCC_HSEConfig+0x2a>
2000724a:	4770      	bx	lr
2000724c:	681a      	ldr	r2, [r3, #0]
2000724e:	f442 3280 	orr.w	r2, r2, #65536	; 0x10000
20007252:	601a      	str	r2, [r3, #0]
20007254:	4770      	bx	lr
20007256:	681a      	ldr	r2, [r3, #0]
20007258:	f442 22a0 	orr.w	r2, r2, #327680	; 0x50000
2000725c:	e7f9      	b.n	20007252 <RCC_HSEConfig+0x26>
2000725e:	bf00      	nop
20007260:	40021000 	.word	0x40021000

20007264 <RCC_AdjustHSICalibrationValue>:
20007264:	4a03      	ldr	r2, [pc, #12]	; (20007274 <RCC_AdjustHSICalibrationValue+0x10>)
20007266:	6813      	ldr	r3, [r2, #0]
20007268:	f023 03f8 	bic.w	r3, r3, #248	; 0xf8
2000726c:	ea43 00c0 	orr.w	r0, r3, r0, lsl #3
20007270:	6010      	str	r0, [r2, #0]
20007272:	4770      	bx	lr
20007274:	40021000 	.word	0x40021000

20007278 <RCC_HSICmd>:
20007278:	4b01      	ldr	r3, [pc, #4]	; (20007280 <RCC_HSICmd+0x8>)
2000727a:	6018      	str	r0, [r3, #0]
2000727c:	4770      	bx	lr
2000727e:	bf00      	nop
20007280:	42420000 	.word	0x42420000

20007284 <RCC_PLLConfig>:
20007284:	4a03      	ldr	r2, [pc, #12]	; (20007294 <RCC_PLLConfig+0x10>)
20007286:	6853      	ldr	r3, [r2, #4]
20007288:	f423 137c 	bic.w	r3, r3, #4128768	; 0x3f0000
2000728c:	4319      	orrs	r1, r3
2000728e:	4301      	orrs	r1, r0
20007290:	6051      	str	r1, [r2, #4]
20007292:	4770      	bx	lr
20007294:	40021000 	.word	0x40021000

20007298 <RCC_PLLCmd>:
20007298:	4b01      	ldr	r3, [pc, #4]	; (200072a0 <RCC_PLLCmd+0x8>)
2000729a:	6018      	str	r0, [r3, #0]
2000729c:	4770      	bx	lr
2000729e:	bf00      	nop
200072a0:	42420060 	.word	0x42420060

200072a4 <RCC_SYSCLKConfig>:
200072a4:	4a03      	ldr	r2, [pc, #12]	; (200072b4 <RCC_SYSCLKConfig+0x10>)
200072a6:	6853      	ldr	r3, [r2, #4]
200072a8:	f023 0303 	bic.w	r3, r3, #3
200072ac:	4318      	orrs	r0, r3
200072ae:	6050      	str	r0, [r2, #4]
200072b0:	4770      	bx	lr
200072b2:	bf00      	nop
200072b4:	40021000 	.word	0x40021000

200072b8 <RCC_GetSYSCLKSource>:
200072b8:	4b02      	ldr	r3, [pc, #8]	; (200072c4 <RCC_GetSYSCLKSource+0xc>)
200072ba:	6858      	ldr	r0, [r3, #4]
200072bc:	f000 000c 	and.w	r0, r0, #12
200072c0:	4770      	bx	lr
200072c2:	bf00      	nop
200072c4:	40021000 	.word	0x40021000

200072c8 <RCC_HCLKConfig>:
200072c8:	4a03      	ldr	r2, [pc, #12]	; (200072d8 <RCC_HCLKConfig+0x10>)
200072ca:	6853      	ldr	r3, [r2, #4]
200072cc:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
200072d0:	4318      	orrs	r0, r3
200072d2:	6050      	str	r0, [r2, #4]
200072d4:	4770      	bx	lr
200072d6:	bf00      	nop
200072d8:	40021000 	.word	0x40021000

200072dc <RCC_PCLK1Config>:
200072dc:	4a03      	ldr	r2, [pc, #12]	; (200072ec <RCC_PCLK1Config+0x10>)
200072de:	6853      	ldr	r3, [r2, #4]
200072e0:	f423 63e0 	bic.w	r3, r3, #1792	; 0x700
200072e4:	4318      	orrs	r0, r3
200072e6:	6050      	str	r0, [r2, #4]
200072e8:	4770      	bx	lr
200072ea:	bf00      	nop
200072ec:	40021000 	.word	0x40021000

200072f0 <RCC_PCLK2Config>:
200072f0:	4a03      	ldr	r2, [pc, #12]	; (20007300 <RCC_PCLK2Config+0x10>)
200072f2:	6853      	ldr	r3, [r2, #4]
200072f4:	f423 5360 	bic.w	r3, r3, #14336	; 0x3800
200072f8:	ea43 00c0 	orr.w	r0, r3, r0, lsl #3
200072fc:	6050      	str	r0, [r2, #4]
200072fe:	4770      	bx	lr
20007300:	40021000 	.word	0x40021000

20007304 <RCC_ITConfig>:
20007304:	4b04      	ldr	r3, [pc, #16]	; (20007318 <RCC_ITConfig+0x14>)
20007306:	781a      	ldrb	r2, [r3, #0]
20007308:	b111      	cbz	r1, 20007310 <RCC_ITConfig+0xc>
2000730a:	4310      	orrs	r0, r2
2000730c:	7018      	strb	r0, [r3, #0]
2000730e:	4770      	bx	lr
20007310:	ea22 0000 	bic.w	r0, r2, r0
20007314:	e7fa      	b.n	2000730c <RCC_ITConfig+0x8>
20007316:	bf00      	nop
20007318:	40021009 	.word	0x40021009

2000731c <RCC_USBCLKConfig>:
2000731c:	4b01      	ldr	r3, [pc, #4]	; (20007324 <RCC_USBCLKConfig+0x8>)
2000731e:	6018      	str	r0, [r3, #0]
20007320:	4770      	bx	lr
20007322:	bf00      	nop
20007324:	424200d8 	.word	0x424200d8

20007328 <RCC_ADCCLKConfig>:
20007328:	4a03      	ldr	r2, [pc, #12]	; (20007338 <RCC_ADCCLKConfig+0x10>)
2000732a:	6853      	ldr	r3, [r2, #4]
2000732c:	f423 4340 	bic.w	r3, r3, #49152	; 0xc000
20007330:	4318      	orrs	r0, r3
20007332:	6050      	str	r0, [r2, #4]
20007334:	4770      	bx	lr
20007336:	bf00      	nop
20007338:	40021000 	.word	0x40021000

2000733c <RCC_LSEConfig>:
2000733c:	2200      	movs	r2, #0
2000733e:	4b06      	ldr	r3, [pc, #24]	; (20007358 <RCC_LSEConfig+0x1c>)
20007340:	2801      	cmp	r0, #1
20007342:	701a      	strb	r2, [r3, #0]
20007344:	701a      	strb	r2, [r3, #0]
20007346:	d002      	beq.n	2000734e <RCC_LSEConfig+0x12>
20007348:	2804      	cmp	r0, #4
2000734a:	d002      	beq.n	20007352 <RCC_LSEConfig+0x16>
2000734c:	4770      	bx	lr
2000734e:	7018      	strb	r0, [r3, #0]
20007350:	4770      	bx	lr
20007352:	2205      	movs	r2, #5
20007354:	701a      	strb	r2, [r3, #0]
20007356:	4770      	bx	lr
20007358:	40021020 	.word	0x40021020

2000735c <RCC_LSICmd>:
2000735c:	4b01      	ldr	r3, [pc, #4]	; (20007364 <RCC_LSICmd+0x8>)
2000735e:	6018      	str	r0, [r3, #0]
20007360:	4770      	bx	lr
20007362:	bf00      	nop
20007364:	42420480 	.word	0x42420480

20007368 <RCC_RTCCLKConfig>:
20007368:	4a02      	ldr	r2, [pc, #8]	; (20007374 <RCC_RTCCLKConfig+0xc>)
2000736a:	6a13      	ldr	r3, [r2, #32]
2000736c:	4318      	orrs	r0, r3
2000736e:	6210      	str	r0, [r2, #32]
20007370:	4770      	bx	lr
20007372:	bf00      	nop
20007374:	40021000 	.word	0x40021000

20007378 <RCC_RTCCLKCmd>:
20007378:	4b01      	ldr	r3, [pc, #4]	; (20007380 <RCC_RTCCLKCmd+0x8>)
2000737a:	6018      	str	r0, [r3, #0]
2000737c:	4770      	bx	lr
2000737e:	bf00      	nop
20007380:	4242043c 	.word	0x4242043c

20007384 <RCC_GetClocksFreq>:
20007384:	4a1b      	ldr	r2, [pc, #108]	; (200073f4 <RCC_GetClocksFreq+0x70>)
20007386:	b510      	push	{r4, lr}
20007388:	6853      	ldr	r3, [r2, #4]
2000738a:	f003 030c 	and.w	r3, r3, #12
2000738e:	2b08      	cmp	r3, #8
20007390:	d01f      	beq.n	200073d2 <RCC_GetClocksFreq+0x4e>
20007392:	4b19      	ldr	r3, [pc, #100]	; (200073f8 <RCC_GetClocksFreq+0x74>)
20007394:	6003      	str	r3, [r0, #0]
20007396:	6853      	ldr	r3, [r2, #4]
20007398:	4918      	ldr	r1, [pc, #96]	; (200073fc <RCC_GetClocksFreq+0x78>)
2000739a:	f3c3 1303 	ubfx	r3, r3, #4, #4
2000739e:	5ccc      	ldrb	r4, [r1, r3]
200073a0:	6803      	ldr	r3, [r0, #0]
200073a2:	40e3      	lsrs	r3, r4
200073a4:	6043      	str	r3, [r0, #4]
200073a6:	6854      	ldr	r4, [r2, #4]
200073a8:	f3c4 2402 	ubfx	r4, r4, #8, #3
200073ac:	5d0c      	ldrb	r4, [r1, r4]
200073ae:	fa23 f404 	lsr.w	r4, r3, r4
200073b2:	6084      	str	r4, [r0, #8]
200073b4:	6854      	ldr	r4, [r2, #4]
200073b6:	f3c4 24c2 	ubfx	r4, r4, #11, #3
200073ba:	5d0c      	ldrb	r4, [r1, r4]
200073bc:	40e3      	lsrs	r3, r4
200073be:	60c3      	str	r3, [r0, #12]
200073c0:	6852      	ldr	r2, [r2, #4]
200073c2:	f3c2 3281 	ubfx	r2, r2, #14, #2
200073c6:	4411      	add	r1, r2
200073c8:	7c0a      	ldrb	r2, [r1, #16]
200073ca:	fbb3 f3f2 	udiv	r3, r3, r2
200073ce:	6103      	str	r3, [r0, #16]
200073d0:	bd10      	pop	{r4, pc}
200073d2:	6853      	ldr	r3, [r2, #4]
200073d4:	6851      	ldr	r1, [r2, #4]
200073d6:	f3c3 4383 	ubfx	r3, r3, #18, #4
200073da:	03cc      	lsls	r4, r1, #15
200073dc:	f103 0302 	add.w	r3, r3, #2
200073e0:	d402      	bmi.n	200073e8 <RCC_GetClocksFreq+0x64>
200073e2:	4907      	ldr	r1, [pc, #28]	; (20007400 <RCC_GetClocksFreq+0x7c>)
200073e4:	434b      	muls	r3, r1
200073e6:	e7d5      	b.n	20007394 <RCC_GetClocksFreq+0x10>
200073e8:	6851      	ldr	r1, [r2, #4]
200073ea:	0389      	lsls	r1, r1, #14
200073ec:	d4f9      	bmi.n	200073e2 <RCC_GetClocksFreq+0x5e>
200073ee:	4902      	ldr	r1, [pc, #8]	; (200073f8 <RCC_GetClocksFreq+0x74>)
200073f0:	e7f8      	b.n	200073e4 <RCC_GetClocksFreq+0x60>
200073f2:	bf00      	nop
200073f4:	40021000 	.word	0x40021000
200073f8:	007a1200 	.word	0x007a1200
200073fc:	200083dc 	.word	0x200083dc
20007400:	003d0900 	.word	0x003d0900

20007404 <RCC_AHBPeriphClockCmd>:
20007404:	4b04      	ldr	r3, [pc, #16]	; (20007418 <RCC_AHBPeriphClockCmd+0x14>)
20007406:	695a      	ldr	r2, [r3, #20]
20007408:	b111      	cbz	r1, 20007410 <RCC_AHBPeriphClockCmd+0xc>
2000740a:	4310      	orrs	r0, r2
2000740c:	6158      	str	r0, [r3, #20]
2000740e:	4770      	bx	lr
20007410:	ea22 0000 	bic.w	r0, r2, r0
20007414:	e7fa      	b.n	2000740c <RCC_AHBPeriphClockCmd+0x8>
20007416:	bf00      	nop
20007418:	40021000 	.word	0x40021000

2000741c <RCC_APB2PeriphClockCmd>:
2000741c:	4b04      	ldr	r3, [pc, #16]	; (20007430 <RCC_APB2PeriphClockCmd+0x14>)
2000741e:	699a      	ldr	r2, [r3, #24]
20007420:	b111      	cbz	r1, 20007428 <RCC_APB2PeriphClockCmd+0xc>
20007422:	4310      	orrs	r0, r2
20007424:	6198      	str	r0, [r3, #24]
20007426:	4770      	bx	lr
20007428:	ea22 0000 	bic.w	r0, r2, r0
2000742c:	e7fa      	b.n	20007424 <RCC_APB2PeriphClockCmd+0x8>
2000742e:	bf00      	nop
20007430:	40021000 	.word	0x40021000

20007434 <RCC_APB1PeriphClockCmd>:
20007434:	4b04      	ldr	r3, [pc, #16]	; (20007448 <RCC_APB1PeriphClockCmd+0x14>)
20007436:	69da      	ldr	r2, [r3, #28]
20007438:	b111      	cbz	r1, 20007440 <RCC_APB1PeriphClockCmd+0xc>
2000743a:	4310      	orrs	r0, r2
2000743c:	61d8      	str	r0, [r3, #28]
2000743e:	4770      	bx	lr
20007440:	ea22 0000 	bic.w	r0, r2, r0
20007444:	e7fa      	b.n	2000743c <RCC_APB1PeriphClockCmd+0x8>
20007446:	bf00      	nop
20007448:	40021000 	.word	0x40021000

2000744c <RCC_APB2PeriphResetCmd>:
2000744c:	4b04      	ldr	r3, [pc, #16]	; (20007460 <RCC_APB2PeriphResetCmd+0x14>)
2000744e:	68da      	ldr	r2, [r3, #12]
20007450:	b111      	cbz	r1, 20007458 <RCC_APB2PeriphResetCmd+0xc>
20007452:	4310      	orrs	r0, r2
20007454:	60d8      	str	r0, [r3, #12]
20007456:	4770      	bx	lr
20007458:	ea22 0000 	bic.w	r0, r2, r0
2000745c:	e7fa      	b.n	20007454 <RCC_APB2PeriphResetCmd+0x8>
2000745e:	bf00      	nop
20007460:	40021000 	.word	0x40021000

20007464 <RCC_APB1PeriphResetCmd>:
20007464:	4b04      	ldr	r3, [pc, #16]	; (20007478 <RCC_APB1PeriphResetCmd+0x14>)
20007466:	691a      	ldr	r2, [r3, #16]
20007468:	b111      	cbz	r1, 20007470 <RCC_APB1PeriphResetCmd+0xc>
2000746a:	4310      	orrs	r0, r2
2000746c:	6118      	str	r0, [r3, #16]
2000746e:	4770      	bx	lr
20007470:	ea22 0000 	bic.w	r0, r2, r0
20007474:	e7fa      	b.n	2000746c <RCC_APB1PeriphResetCmd+0x8>
20007476:	bf00      	nop
20007478:	40021000 	.word	0x40021000

2000747c <RCC_BackupResetCmd>:
2000747c:	4b01      	ldr	r3, [pc, #4]	; (20007484 <RCC_BackupResetCmd+0x8>)
2000747e:	6018      	str	r0, [r3, #0]
20007480:	4770      	bx	lr
20007482:	bf00      	nop
20007484:	42420440 	.word	0x42420440

20007488 <RCC_ClockSecuritySystemCmd>:
20007488:	4b01      	ldr	r3, [pc, #4]	; (20007490 <RCC_ClockSecuritySystemCmd+0x8>)
2000748a:	6018      	str	r0, [r3, #0]
2000748c:	4770      	bx	lr
2000748e:	bf00      	nop
20007490:	4242004c 	.word	0x4242004c

20007494 <RCC_MCOConfig>:
20007494:	4b01      	ldr	r3, [pc, #4]	; (2000749c <RCC_MCOConfig+0x8>)
20007496:	7018      	strb	r0, [r3, #0]
20007498:	4770      	bx	lr
2000749a:	bf00      	nop
2000749c:	40021007 	.word	0x40021007

200074a0 <RCC_GetFlagStatus>:
200074a0:	0942      	lsrs	r2, r0, #5
200074a2:	2a01      	cmp	r2, #1
200074a4:	4b07      	ldr	r3, [pc, #28]	; (200074c4 <RCC_GetFlagStatus+0x24>)
200074a6:	d107      	bne.n	200074b8 <RCC_GetFlagStatus+0x18>
200074a8:	681b      	ldr	r3, [r3, #0]
200074aa:	f000 001f 	and.w	r0, r0, #31
200074ae:	fa23 f000 	lsr.w	r0, r3, r0
200074b2:	f000 0001 	and.w	r0, r0, #1
200074b6:	4770      	bx	lr
200074b8:	2a02      	cmp	r2, #2
200074ba:	bf0c      	ite	eq
200074bc:	6a1b      	ldreq	r3, [r3, #32]
200074be:	6a5b      	ldrne	r3, [r3, #36]	; 0x24
200074c0:	e7f3      	b.n	200074aa <RCC_GetFlagStatus+0xa>
200074c2:	bf00      	nop
200074c4:	40021000 	.word	0x40021000

200074c8 <RCC_WaitForHSEStartUp>:
200074c8:	2300      	movs	r3, #0
200074ca:	b507      	push	{r0, r1, r2, lr}
200074cc:	9301      	str	r3, [sp, #4]
200074ce:	2031      	movs	r0, #49	; 0x31
200074d0:	f7ff ffe6 	bl	200074a0 <RCC_GetFlagStatus>
200074d4:	9b01      	ldr	r3, [sp, #4]
200074d6:	3301      	adds	r3, #1
200074d8:	9301      	str	r3, [sp, #4]
200074da:	9b01      	ldr	r3, [sp, #4]
200074dc:	f5b3 6fa0 	cmp.w	r3, #1280	; 0x500
200074e0:	d001      	beq.n	200074e6 <RCC_WaitForHSEStartUp+0x1e>
200074e2:	2800      	cmp	r0, #0
200074e4:	d0f3      	beq.n	200074ce <RCC_WaitForHSEStartUp+0x6>
200074e6:	2031      	movs	r0, #49	; 0x31
200074e8:	f7ff ffda 	bl	200074a0 <RCC_GetFlagStatus>
200074ec:	3000      	adds	r0, #0
200074ee:	bf18      	it	ne
200074f0:	2001      	movne	r0, #1
200074f2:	b003      	add	sp, #12
200074f4:	f85d fb04 	ldr.w	pc, [sp], #4

200074f8 <RCC_ClearFlag>:
200074f8:	4a02      	ldr	r2, [pc, #8]	; (20007504 <RCC_ClearFlag+0xc>)
200074fa:	6a53      	ldr	r3, [r2, #36]	; 0x24
200074fc:	f043 7380 	orr.w	r3, r3, #16777216	; 0x1000000
20007500:	6253      	str	r3, [r2, #36]	; 0x24
20007502:	4770      	bx	lr
20007504:	40021000 	.word	0x40021000

20007508 <RCC_GetITStatus>:
20007508:	4b03      	ldr	r3, [pc, #12]	; (20007518 <RCC_GetITStatus+0x10>)
2000750a:	689b      	ldr	r3, [r3, #8]
2000750c:	4218      	tst	r0, r3
2000750e:	bf14      	ite	ne
20007510:	2001      	movne	r0, #1
20007512:	2000      	moveq	r0, #0
20007514:	4770      	bx	lr
20007516:	bf00      	nop
20007518:	40021000 	.word	0x40021000

2000751c <RCC_ClearITPendingBit>:
2000751c:	4b01      	ldr	r3, [pc, #4]	; (20007524 <RCC_ClearITPendingBit+0x8>)
2000751e:	7018      	strb	r0, [r3, #0]
20007520:	4770      	bx	lr
20007522:	bf00      	nop
20007524:	4002100a 	.word	0x4002100a

20007528 <TI1_Config>:
20007528:	b530      	push	{r4, r5, lr}
2000752a:	8c04      	ldrh	r4, [r0, #32]
2000752c:	ea42 1303 	orr.w	r3, r2, r3, lsl #4
20007530:	f024 0401 	bic.w	r4, r4, #1
20007534:	0424      	lsls	r4, r4, #16
20007536:	0c24      	lsrs	r4, r4, #16
20007538:	8404      	strh	r4, [r0, #32]
2000753a:	8b04      	ldrh	r4, [r0, #24]
2000753c:	8c05      	ldrh	r5, [r0, #32]
2000753e:	f024 04f3 	bic.w	r4, r4, #243	; 0xf3
20007542:	0424      	lsls	r4, r4, #16
20007544:	0c24      	lsrs	r4, r4, #16
20007546:	431c      	orrs	r4, r3
20007548:	4b11      	ldr	r3, [pc, #68]	; (20007590 <TI1_Config+0x68>)
2000754a:	b2ad      	uxth	r5, r5
2000754c:	4298      	cmp	r0, r3
2000754e:	b2a4      	uxth	r4, r4
20007550:	f041 0101 	orr.w	r1, r1, #1
20007554:	d012      	beq.n	2000757c <TI1_Config+0x54>
20007556:	f503 6300 	add.w	r3, r3, #2048	; 0x800
2000755a:	4298      	cmp	r0, r3
2000755c:	d00e      	beq.n	2000757c <TI1_Config+0x54>
2000755e:	f1b0 4f80 	cmp.w	r0, #1073741824	; 0x40000000
20007562:	d00b      	beq.n	2000757c <TI1_Config+0x54>
20007564:	f5a3 3398 	sub.w	r3, r3, #77824	; 0x13000
20007568:	4298      	cmp	r0, r3
2000756a:	d007      	beq.n	2000757c <TI1_Config+0x54>
2000756c:	f503 6380 	add.w	r3, r3, #1024	; 0x400
20007570:	4298      	cmp	r0, r3
20007572:	d003      	beq.n	2000757c <TI1_Config+0x54>
20007574:	f503 6380 	add.w	r3, r3, #1024	; 0x400
20007578:	4298      	cmp	r0, r3
2000757a:	d105      	bne.n	20007588 <TI1_Config+0x60>
2000757c:	f025 0502 	bic.w	r5, r5, #2
20007580:	4329      	orrs	r1, r5
20007582:	8304      	strh	r4, [r0, #24]
20007584:	8401      	strh	r1, [r0, #32]
20007586:	bd30      	pop	{r4, r5, pc}
20007588:	f025 050a 	bic.w	r5, r5, #10
2000758c:	e7f8      	b.n	20007580 <TI1_Config+0x58>
2000758e:	bf00      	nop
20007590:	40012c00 	.word	0x40012c00

20007594 <TI2_Config>:
20007594:	b570      	push	{r4, r5, r6, lr}
20007596:	8c05      	ldrh	r5, [r0, #32]
20007598:	0212      	lsls	r2, r2, #8
2000759a:	f025 0510 	bic.w	r5, r5, #16
2000759e:	042d      	lsls	r5, r5, #16
200075a0:	0c2d      	lsrs	r5, r5, #16
200075a2:	8405      	strh	r5, [r0, #32]
200075a4:	8b05      	ldrh	r5, [r0, #24]
200075a6:	ea42 3303 	orr.w	r3, r2, r3, lsl #12
200075aa:	f425 7540 	bic.w	r5, r5, #768	; 0x300
200075ae:	052d      	lsls	r5, r5, #20
200075b0:	0d2d      	lsrs	r5, r5, #20
200075b2:	431d      	orrs	r5, r3
200075b4:	4b14      	ldr	r3, [pc, #80]	; (20007608 <TI2_Config+0x74>)
200075b6:	8c04      	ldrh	r4, [r0, #32]
200075b8:	010e      	lsls	r6, r1, #4
200075ba:	4298      	cmp	r0, r3
200075bc:	b2a4      	uxth	r4, r4
200075be:	b2b6      	uxth	r6, r6
200075c0:	b2ad      	uxth	r5, r5
200075c2:	d012      	beq.n	200075ea <TI2_Config+0x56>
200075c4:	f503 6300 	add.w	r3, r3, #2048	; 0x800
200075c8:	4298      	cmp	r0, r3
200075ca:	d00e      	beq.n	200075ea <TI2_Config+0x56>
200075cc:	f1b0 4f80 	cmp.w	r0, #1073741824	; 0x40000000
200075d0:	d00b      	beq.n	200075ea <TI2_Config+0x56>
200075d2:	f5a3 3398 	sub.w	r3, r3, #77824	; 0x13000
200075d6:	4298      	cmp	r0, r3
200075d8:	d007      	beq.n	200075ea <TI2_Config+0x56>
200075da:	f503 6380 	add.w	r3, r3, #1024	; 0x400
200075de:	4298      	cmp	r0, r3
200075e0:	d003      	beq.n	200075ea <TI2_Config+0x56>
200075e2:	f503 6380 	add.w	r3, r3, #1024	; 0x400
200075e6:	4298      	cmp	r0, r3
200075e8:	d107      	bne.n	200075fa <TI2_Config+0x66>
200075ea:	f024 0420 	bic.w	r4, r4, #32
200075ee:	f046 0610 	orr.w	r6, r6, #16
200075f2:	4334      	orrs	r4, r6
200075f4:	8305      	strh	r5, [r0, #24]
200075f6:	8404      	strh	r4, [r0, #32]
200075f8:	bd70      	pop	{r4, r5, r6, pc}
200075fa:	f024 04a0 	bic.w	r4, r4, #160	; 0xa0
200075fe:	f041 0110 	orr.w	r1, r1, #16
20007602:	430c      	orrs	r4, r1
20007604:	e7f6      	b.n	200075f4 <TI2_Config+0x60>
20007606:	bf00      	nop
20007608:	40012c00 	.word	0x40012c00

2000760c <TIM_DeInit>:
2000760c:	b508      	push	{r3, lr}
2000760e:	4b61      	ldr	r3, [pc, #388]	; (20007794 <TIM_DeInit+0x188>)
20007610:	4298      	cmp	r0, r3
20007612:	d10b      	bne.n	2000762c <TIM_DeInit+0x20>
20007614:	2101      	movs	r1, #1
20007616:	f44f 6000 	mov.w	r0, #2048	; 0x800
2000761a:	f7ff ff17 	bl	2000744c <RCC_APB2PeriphResetCmd>
2000761e:	2100      	movs	r1, #0
20007620:	f44f 6000 	mov.w	r0, #2048	; 0x800
20007624:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
20007628:	f7ff bf10 	b.w	2000744c <RCC_APB2PeriphResetCmd>
2000762c:	f1b0 4f80 	cmp.w	r0, #1073741824	; 0x40000000
20007630:	d109      	bne.n	20007646 <TIM_DeInit+0x3a>
20007632:	2101      	movs	r1, #1
20007634:	4608      	mov	r0, r1
20007636:	f7ff ff15 	bl	20007464 <RCC_APB1PeriphResetCmd>
2000763a:	2100      	movs	r1, #0
2000763c:	2001      	movs	r0, #1
2000763e:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
20007642:	f7ff bf0f 	b.w	20007464 <RCC_APB1PeriphResetCmd>
20007646:	4b54      	ldr	r3, [pc, #336]	; (20007798 <TIM_DeInit+0x18c>)
20007648:	4298      	cmp	r0, r3
2000764a:	d106      	bne.n	2000765a <TIM_DeInit+0x4e>
2000764c:	2101      	movs	r1, #1
2000764e:	2002      	movs	r0, #2
20007650:	f7ff ff08 	bl	20007464 <RCC_APB1PeriphResetCmd>
20007654:	2100      	movs	r1, #0
20007656:	2002      	movs	r0, #2
20007658:	e7f1      	b.n	2000763e <TIM_DeInit+0x32>
2000765a:	4b50      	ldr	r3, [pc, #320]	; (2000779c <TIM_DeInit+0x190>)
2000765c:	4298      	cmp	r0, r3
2000765e:	d106      	bne.n	2000766e <TIM_DeInit+0x62>
20007660:	2101      	movs	r1, #1
20007662:	2004      	movs	r0, #4
20007664:	f7ff fefe 	bl	20007464 <RCC_APB1PeriphResetCmd>
20007668:	2100      	movs	r1, #0
2000766a:	2004      	movs	r0, #4
2000766c:	e7e7      	b.n	2000763e <TIM_DeInit+0x32>
2000766e:	4b4c      	ldr	r3, [pc, #304]	; (200077a0 <TIM_DeInit+0x194>)
20007670:	4298      	cmp	r0, r3
20007672:	d106      	bne.n	20007682 <TIM_DeInit+0x76>
20007674:	2101      	movs	r1, #1
20007676:	2008      	movs	r0, #8
20007678:	f7ff fef4 	bl	20007464 <RCC_APB1PeriphResetCmd>
2000767c:	2100      	movs	r1, #0
2000767e:	2008      	movs	r0, #8
20007680:	e7dd      	b.n	2000763e <TIM_DeInit+0x32>
20007682:	4b48      	ldr	r3, [pc, #288]	; (200077a4 <TIM_DeInit+0x198>)
20007684:	4298      	cmp	r0, r3
20007686:	d106      	bne.n	20007696 <TIM_DeInit+0x8a>
20007688:	2101      	movs	r1, #1
2000768a:	2010      	movs	r0, #16
2000768c:	f7ff feea 	bl	20007464 <RCC_APB1PeriphResetCmd>
20007690:	2100      	movs	r1, #0
20007692:	2010      	movs	r0, #16
20007694:	e7d3      	b.n	2000763e <TIM_DeInit+0x32>
20007696:	4b44      	ldr	r3, [pc, #272]	; (200077a8 <TIM_DeInit+0x19c>)
20007698:	4298      	cmp	r0, r3
2000769a:	d106      	bne.n	200076aa <TIM_DeInit+0x9e>
2000769c:	2101      	movs	r1, #1
2000769e:	2020      	movs	r0, #32
200076a0:	f7ff fee0 	bl	20007464 <RCC_APB1PeriphResetCmd>
200076a4:	2100      	movs	r1, #0
200076a6:	2020      	movs	r0, #32
200076a8:	e7c9      	b.n	2000763e <TIM_DeInit+0x32>
200076aa:	4b40      	ldr	r3, [pc, #256]	; (200077ac <TIM_DeInit+0x1a0>)
200076ac:	4298      	cmp	r0, r3
200076ae:	d108      	bne.n	200076c2 <TIM_DeInit+0xb6>
200076b0:	2101      	movs	r1, #1
200076b2:	f44f 5000 	mov.w	r0, #8192	; 0x2000
200076b6:	f7ff fec9 	bl	2000744c <RCC_APB2PeriphResetCmd>
200076ba:	2100      	movs	r1, #0
200076bc:	f44f 5000 	mov.w	r0, #8192	; 0x2000
200076c0:	e7b0      	b.n	20007624 <TIM_DeInit+0x18>
200076c2:	4b3b      	ldr	r3, [pc, #236]	; (200077b0 <TIM_DeInit+0x1a4>)
200076c4:	4298      	cmp	r0, r3
200076c6:	d108      	bne.n	200076da <TIM_DeInit+0xce>
200076c8:	2101      	movs	r1, #1
200076ca:	f44f 2000 	mov.w	r0, #524288	; 0x80000
200076ce:	f7ff febd 	bl	2000744c <RCC_APB2PeriphResetCmd>
200076d2:	2100      	movs	r1, #0
200076d4:	f44f 2000 	mov.w	r0, #524288	; 0x80000
200076d8:	e7a4      	b.n	20007624 <TIM_DeInit+0x18>
200076da:	4b36      	ldr	r3, [pc, #216]	; (200077b4 <TIM_DeInit+0x1a8>)
200076dc:	4298      	cmp	r0, r3
200076de:	d108      	bne.n	200076f2 <TIM_DeInit+0xe6>
200076e0:	2101      	movs	r1, #1
200076e2:	f44f 1080 	mov.w	r0, #1048576	; 0x100000
200076e6:	f7ff feb1 	bl	2000744c <RCC_APB2PeriphResetCmd>
200076ea:	2100      	movs	r1, #0
200076ec:	f44f 1080 	mov.w	r0, #1048576	; 0x100000
200076f0:	e798      	b.n	20007624 <TIM_DeInit+0x18>
200076f2:	4b31      	ldr	r3, [pc, #196]	; (200077b8 <TIM_DeInit+0x1ac>)
200076f4:	4298      	cmp	r0, r3
200076f6:	d108      	bne.n	2000770a <TIM_DeInit+0xfe>
200076f8:	2101      	movs	r1, #1
200076fa:	f44f 1000 	mov.w	r0, #2097152	; 0x200000
200076fe:	f7ff fea5 	bl	2000744c <RCC_APB2PeriphResetCmd>
20007702:	2100      	movs	r1, #0
20007704:	f44f 1000 	mov.w	r0, #2097152	; 0x200000
20007708:	e78c      	b.n	20007624 <TIM_DeInit+0x18>
2000770a:	4b2c      	ldr	r3, [pc, #176]	; (200077bc <TIM_DeInit+0x1b0>)
2000770c:	4298      	cmp	r0, r3
2000770e:	d106      	bne.n	2000771e <TIM_DeInit+0x112>
20007710:	2101      	movs	r1, #1
20007712:	2040      	movs	r0, #64	; 0x40
20007714:	f7ff fea6 	bl	20007464 <RCC_APB1PeriphResetCmd>
20007718:	2100      	movs	r1, #0
2000771a:	2040      	movs	r0, #64	; 0x40
2000771c:	e78f      	b.n	2000763e <TIM_DeInit+0x32>
2000771e:	4b28      	ldr	r3, [pc, #160]	; (200077c0 <TIM_DeInit+0x1b4>)
20007720:	4298      	cmp	r0, r3
20007722:	d106      	bne.n	20007732 <TIM_DeInit+0x126>
20007724:	2101      	movs	r1, #1
20007726:	2080      	movs	r0, #128	; 0x80
20007728:	f7ff fe9c 	bl	20007464 <RCC_APB1PeriphResetCmd>
2000772c:	2100      	movs	r1, #0
2000772e:	2080      	movs	r0, #128	; 0x80
20007730:	e785      	b.n	2000763e <TIM_DeInit+0x32>
20007732:	4b24      	ldr	r3, [pc, #144]	; (200077c4 <TIM_DeInit+0x1b8>)
20007734:	4298      	cmp	r0, r3
20007736:	d108      	bne.n	2000774a <TIM_DeInit+0x13e>
20007738:	2101      	movs	r1, #1
2000773a:	f44f 7080 	mov.w	r0, #256	; 0x100
2000773e:	f7ff fe91 	bl	20007464 <RCC_APB1PeriphResetCmd>
20007742:	2100      	movs	r1, #0
20007744:	f44f 7080 	mov.w	r0, #256	; 0x100
20007748:	e779      	b.n	2000763e <TIM_DeInit+0x32>
2000774a:	4b1f      	ldr	r3, [pc, #124]	; (200077c8 <TIM_DeInit+0x1bc>)
2000774c:	4298      	cmp	r0, r3
2000774e:	d108      	bne.n	20007762 <TIM_DeInit+0x156>
20007750:	2101      	movs	r1, #1
20007752:	f44f 3080 	mov.w	r0, #65536	; 0x10000
20007756:	f7ff fe79 	bl	2000744c <RCC_APB2PeriphResetCmd>
2000775a:	2100      	movs	r1, #0
2000775c:	f44f 3080 	mov.w	r0, #65536	; 0x10000
20007760:	e760      	b.n	20007624 <TIM_DeInit+0x18>
20007762:	4b1a      	ldr	r3, [pc, #104]	; (200077cc <TIM_DeInit+0x1c0>)
20007764:	4298      	cmp	r0, r3
20007766:	d108      	bne.n	2000777a <TIM_DeInit+0x16e>
20007768:	2101      	movs	r1, #1
2000776a:	f44f 3000 	mov.w	r0, #131072	; 0x20000
2000776e:	f7ff fe6d 	bl	2000744c <RCC_APB2PeriphResetCmd>
20007772:	2100      	movs	r1, #0
20007774:	f44f 3000 	mov.w	r0, #131072	; 0x20000
20007778:	e754      	b.n	20007624 <TIM_DeInit+0x18>
2000777a:	4b15      	ldr	r3, [pc, #84]	; (200077d0 <TIM_DeInit+0x1c4>)
2000777c:	4298      	cmp	r0, r3
2000777e:	d108      	bne.n	20007792 <TIM_DeInit+0x186>
20007780:	2101      	movs	r1, #1
20007782:	f44f 2080 	mov.w	r0, #262144	; 0x40000
20007786:	f7ff fe61 	bl	2000744c <RCC_APB2PeriphResetCmd>
2000778a:	2100      	movs	r1, #0
2000778c:	f44f 2080 	mov.w	r0, #262144	; 0x40000
20007790:	e748      	b.n	20007624 <TIM_DeInit+0x18>
20007792:	bd08      	pop	{r3, pc}
20007794:	40012c00 	.word	0x40012c00
20007798:	40000400 	.word	0x40000400
2000779c:	40000800 	.word	0x40000800
200077a0:	40000c00 	.word	0x40000c00
200077a4:	40001000 	.word	0x40001000
200077a8:	40001400 	.word	0x40001400
200077ac:	40013400 	.word	0x40013400
200077b0:	40014c00 	.word	0x40014c00
200077b4:	40015000 	.word	0x40015000
200077b8:	40015400 	.word	0x40015400
200077bc:	40001800 	.word	0x40001800
200077c0:	40001c00 	.word	0x40001c00
200077c4:	40002000 	.word	0x40002000
200077c8:	40014000 	.word	0x40014000
200077cc:	40014400 	.word	0x40014400
200077d0:	40014800 	.word	0x40014800

200077d4 <TIM_TimeBaseInit>:
200077d4:	4a22      	ldr	r2, [pc, #136]	; (20007860 <TIM_TimeBaseInit+0x8c>)
200077d6:	8803      	ldrh	r3, [r0, #0]
200077d8:	4290      	cmp	r0, r2
200077da:	b29b      	uxth	r3, r3
200077dc:	d012      	beq.n	20007804 <TIM_TimeBaseInit+0x30>
200077de:	f502 6200 	add.w	r2, r2, #2048	; 0x800
200077e2:	4290      	cmp	r0, r2
200077e4:	d00e      	beq.n	20007804 <TIM_TimeBaseInit+0x30>
200077e6:	f1b0 4f80 	cmp.w	r0, #1073741824	; 0x40000000
200077ea:	d00b      	beq.n	20007804 <TIM_TimeBaseInit+0x30>
200077ec:	f5a2 3298 	sub.w	r2, r2, #77824	; 0x13000
200077f0:	4290      	cmp	r0, r2
200077f2:	d007      	beq.n	20007804 <TIM_TimeBaseInit+0x30>
200077f4:	f502 6280 	add.w	r2, r2, #1024	; 0x400
200077f8:	4290      	cmp	r0, r2
200077fa:	d003      	beq.n	20007804 <TIM_TimeBaseInit+0x30>
200077fc:	f502 6280 	add.w	r2, r2, #1024	; 0x400
20007800:	4290      	cmp	r0, r2
20007802:	d103      	bne.n	2000780c <TIM_TimeBaseInit+0x38>
20007804:	884a      	ldrh	r2, [r1, #2]
20007806:	f023 0370 	bic.w	r3, r3, #112	; 0x70
2000780a:	4313      	orrs	r3, r2
2000780c:	4a15      	ldr	r2, [pc, #84]	; (20007864 <TIM_TimeBaseInit+0x90>)
2000780e:	4290      	cmp	r0, r2
20007810:	d008      	beq.n	20007824 <TIM_TimeBaseInit+0x50>
20007812:	f502 6280 	add.w	r2, r2, #1024	; 0x400
20007816:	4290      	cmp	r0, r2
20007818:	bf1f      	itttt	ne
2000781a:	f423 7340 	bicne.w	r3, r3, #768	; 0x300
2000781e:	88ca      	ldrhne	r2, [r1, #6]
20007820:	b29b      	uxthne	r3, r3
20007822:	4313      	orrne	r3, r2
20007824:	8003      	strh	r3, [r0, #0]
20007826:	888b      	ldrh	r3, [r1, #4]
20007828:	8583      	strh	r3, [r0, #44]	; 0x2c
2000782a:	880b      	ldrh	r3, [r1, #0]
2000782c:	8503      	strh	r3, [r0, #40]	; 0x28
2000782e:	4b0c      	ldr	r3, [pc, #48]	; (20007860 <TIM_TimeBaseInit+0x8c>)
20007830:	4298      	cmp	r0, r3
20007832:	d00f      	beq.n	20007854 <TIM_TimeBaseInit+0x80>
20007834:	f503 6300 	add.w	r3, r3, #2048	; 0x800
20007838:	4298      	cmp	r0, r3
2000783a:	d00b      	beq.n	20007854 <TIM_TimeBaseInit+0x80>
2000783c:	f503 6340 	add.w	r3, r3, #3072	; 0xc00
20007840:	4298      	cmp	r0, r3
20007842:	d007      	beq.n	20007854 <TIM_TimeBaseInit+0x80>
20007844:	f503 6380 	add.w	r3, r3, #1024	; 0x400
20007848:	4298      	cmp	r0, r3
2000784a:	d003      	beq.n	20007854 <TIM_TimeBaseInit+0x80>
2000784c:	f503 6380 	add.w	r3, r3, #1024	; 0x400
20007850:	4298      	cmp	r0, r3
20007852:	d101      	bne.n	20007858 <TIM_TimeBaseInit+0x84>
20007854:	7a0b      	ldrb	r3, [r1, #8]
20007856:	8603      	strh	r3, [r0, #48]	; 0x30
20007858:	2301      	movs	r3, #1
2000785a:	8283      	strh	r3, [r0, #20]
2000785c:	4770      	bx	lr
2000785e:	bf00      	nop
20007860:	40012c00 	.word	0x40012c00
20007864:	40001000 	.word	0x40001000

20007868 <TIM_OC1Init>:
20007868:	8c03      	ldrh	r3, [r0, #32]
2000786a:	b570      	push	{r4, r5, r6, lr}
2000786c:	f023 0301 	bic.w	r3, r3, #1
20007870:	041b      	lsls	r3, r3, #16
20007872:	0c1b      	lsrs	r3, r3, #16
20007874:	8403      	strh	r3, [r0, #32]
20007876:	8c03      	ldrh	r3, [r0, #32]
20007878:	8884      	ldrh	r4, [r0, #4]
2000787a:	8b02      	ldrh	r2, [r0, #24]
2000787c:	880d      	ldrh	r5, [r1, #0]
2000787e:	f022 0273 	bic.w	r2, r2, #115	; 0x73
20007882:	0412      	lsls	r2, r2, #16
20007884:	0c12      	lsrs	r2, r2, #16
20007886:	432a      	orrs	r2, r5
20007888:	f023 0302 	bic.w	r3, r3, #2
2000788c:	890d      	ldrh	r5, [r1, #8]
2000788e:	884e      	ldrh	r6, [r1, #2]
20007890:	041b      	lsls	r3, r3, #16
20007892:	4335      	orrs	r5, r6
20007894:	0c1b      	lsrs	r3, r3, #16
20007896:	432b      	orrs	r3, r5
20007898:	4d13      	ldr	r5, [pc, #76]	; (200078e8 <TIM_OC1Init+0x80>)
2000789a:	b2a4      	uxth	r4, r4
2000789c:	42a8      	cmp	r0, r5
2000789e:	d00f      	beq.n	200078c0 <TIM_OC1Init+0x58>
200078a0:	f505 6500 	add.w	r5, r5, #2048	; 0x800
200078a4:	42a8      	cmp	r0, r5
200078a6:	d00b      	beq.n	200078c0 <TIM_OC1Init+0x58>
200078a8:	f505 6540 	add.w	r5, r5, #3072	; 0xc00
200078ac:	42a8      	cmp	r0, r5
200078ae:	d007      	beq.n	200078c0 <TIM_OC1Init+0x58>
200078b0:	f505 6580 	add.w	r5, r5, #1024	; 0x400
200078b4:	42a8      	cmp	r0, r5
200078b6:	d003      	beq.n	200078c0 <TIM_OC1Init+0x58>
200078b8:	f505 6580 	add.w	r5, r5, #1024	; 0x400
200078bc:	42a8      	cmp	r0, r5
200078be:	d10d      	bne.n	200078dc <TIM_OC1Init+0x74>
200078c0:	894d      	ldrh	r5, [r1, #10]
200078c2:	f023 0308 	bic.w	r3, r3, #8
200078c6:	432b      	orrs	r3, r5
200078c8:	888d      	ldrh	r5, [r1, #4]
200078ca:	f023 0304 	bic.w	r3, r3, #4
200078ce:	432b      	orrs	r3, r5
200078d0:	89ce      	ldrh	r6, [r1, #14]
200078d2:	898d      	ldrh	r5, [r1, #12]
200078d4:	f424 7440 	bic.w	r4, r4, #768	; 0x300
200078d8:	4335      	orrs	r5, r6
200078da:	432c      	orrs	r4, r5
200078dc:	8084      	strh	r4, [r0, #4]
200078de:	8302      	strh	r2, [r0, #24]
200078e0:	88ca      	ldrh	r2, [r1, #6]
200078e2:	8682      	strh	r2, [r0, #52]	; 0x34
200078e4:	8403      	strh	r3, [r0, #32]
200078e6:	bd70      	pop	{r4, r5, r6, pc}
200078e8:	40012c00 	.word	0x40012c00

200078ec <TIM_OC2Init>:
200078ec:	8c03      	ldrh	r3, [r0, #32]
200078ee:	b570      	push	{r4, r5, r6, lr}
200078f0:	f023 0310 	bic.w	r3, r3, #16
200078f4:	041b      	lsls	r3, r3, #16
200078f6:	0c1b      	lsrs	r3, r3, #16
200078f8:	8403      	strh	r3, [r0, #32]
200078fa:	8c03      	ldrh	r3, [r0, #32]
200078fc:	8884      	ldrh	r4, [r0, #4]
200078fe:	8b02      	ldrh	r2, [r0, #24]
20007900:	880d      	ldrh	r5, [r1, #0]
20007902:	f422 42e6 	bic.w	r2, r2, #29440	; 0x7300
20007906:	0412      	lsls	r2, r2, #16
20007908:	0c12      	lsrs	r2, r2, #16
2000790a:	ea42 2205 	orr.w	r2, r2, r5, lsl #8
2000790e:	f023 0320 	bic.w	r3, r3, #32
20007912:	890d      	ldrh	r5, [r1, #8]
20007914:	884e      	ldrh	r6, [r1, #2]
20007916:	041b      	lsls	r3, r3, #16
20007918:	4335      	orrs	r5, r6
2000791a:	0c1b      	lsrs	r3, r3, #16
2000791c:	ea43 1305 	orr.w	r3, r3, r5, lsl #4
20007920:	4d11      	ldr	r5, [pc, #68]	; (20007968 <TIM_OC2Init+0x7c>)
20007922:	b2a4      	uxth	r4, r4
20007924:	42a8      	cmp	r0, r5
20007926:	b292      	uxth	r2, r2
20007928:	b29b      	uxth	r3, r3
2000792a:	d003      	beq.n	20007934 <TIM_OC2Init+0x48>
2000792c:	f505 6500 	add.w	r5, r5, #2048	; 0x800
20007930:	42a8      	cmp	r0, r5
20007932:	d113      	bne.n	2000795c <TIM_OC2Init+0x70>
20007934:	894d      	ldrh	r5, [r1, #10]
20007936:	f023 0380 	bic.w	r3, r3, #128	; 0x80
2000793a:	ea43 1305 	orr.w	r3, r3, r5, lsl #4
2000793e:	f64f 75bf 	movw	r5, #65471	; 0xffbf
20007942:	401d      	ands	r5, r3
20007944:	888b      	ldrh	r3, [r1, #4]
20007946:	89ce      	ldrh	r6, [r1, #14]
20007948:	ea45 1303 	orr.w	r3, r5, r3, lsl #4
2000794c:	898d      	ldrh	r5, [r1, #12]
2000794e:	f424 6440 	bic.w	r4, r4, #3072	; 0xc00
20007952:	4335      	orrs	r5, r6
20007954:	ea44 0485 	orr.w	r4, r4, r5, lsl #2
20007958:	b29b      	uxth	r3, r3
2000795a:	b2a4      	uxth	r4, r4
2000795c:	8084      	strh	r4, [r0, #4]
2000795e:	8302      	strh	r2, [r0, #24]
20007960:	88ca      	ldrh	r2, [r1, #6]
20007962:	8702      	strh	r2, [r0, #56]	; 0x38
20007964:	8403      	strh	r3, [r0, #32]
20007966:	bd70      	pop	{r4, r5, r6, pc}
20007968:	40012c00 	.word	0x40012c00

2000796c <TIM_OC3Init>:
2000796c:	8c03      	ldrh	r3, [r0, #32]
2000796e:	b570      	push	{r4, r5, r6, lr}
20007970:	f423 7380 	bic.w	r3, r3, #256	; 0x100
20007974:	041b      	lsls	r3, r3, #16
20007976:	0c1b      	lsrs	r3, r3, #16
20007978:	8403      	strh	r3, [r0, #32]
2000797a:	8c03      	ldrh	r3, [r0, #32]
2000797c:	8882      	ldrh	r2, [r0, #4]
2000797e:	8b84      	ldrh	r4, [r0, #28]
20007980:	880d      	ldrh	r5, [r1, #0]
20007982:	f024 0473 	bic.w	r4, r4, #115	; 0x73
20007986:	0424      	lsls	r4, r4, #16
20007988:	0c24      	lsrs	r4, r4, #16
2000798a:	432c      	orrs	r4, r5
2000798c:	f423 7300 	bic.w	r3, r3, #512	; 0x200
20007990:	890d      	ldrh	r5, [r1, #8]
20007992:	884e      	ldrh	r6, [r1, #2]
20007994:	041b      	lsls	r3, r3, #16
20007996:	4335      	orrs	r5, r6
20007998:	0c1b      	lsrs	r3, r3, #16
2000799a:	ea43 2305 	orr.w	r3, r3, r5, lsl #8
2000799e:	4d11      	ldr	r5, [pc, #68]	; (200079e4 <TIM_OC3Init+0x78>)
200079a0:	b292      	uxth	r2, r2
200079a2:	42a8      	cmp	r0, r5
200079a4:	b29b      	uxth	r3, r3
200079a6:	d003      	beq.n	200079b0 <TIM_OC3Init+0x44>
200079a8:	f505 6500 	add.w	r5, r5, #2048	; 0x800
200079ac:	42a8      	cmp	r0, r5
200079ae:	d113      	bne.n	200079d8 <TIM_OC3Init+0x6c>
200079b0:	894d      	ldrh	r5, [r1, #10]
200079b2:	f423 6300 	bic.w	r3, r3, #2048	; 0x800
200079b6:	ea43 2305 	orr.w	r3, r3, r5, lsl #8
200079ba:	f64f 35ff 	movw	r5, #64511	; 0xfbff
200079be:	401d      	ands	r5, r3
200079c0:	888b      	ldrh	r3, [r1, #4]
200079c2:	89ce      	ldrh	r6, [r1, #14]
200079c4:	ea45 2303 	orr.w	r3, r5, r3, lsl #8
200079c8:	898d      	ldrh	r5, [r1, #12]
200079ca:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
200079ce:	4335      	orrs	r5, r6
200079d0:	ea42 1205 	orr.w	r2, r2, r5, lsl #4
200079d4:	b29b      	uxth	r3, r3
200079d6:	b292      	uxth	r2, r2
200079d8:	8082      	strh	r2, [r0, #4]
200079da:	88ca      	ldrh	r2, [r1, #6]
200079dc:	8384      	strh	r4, [r0, #28]
200079de:	8782      	strh	r2, [r0, #60]	; 0x3c
200079e0:	8403      	strh	r3, [r0, #32]
200079e2:	bd70      	pop	{r4, r5, r6, pc}
200079e4:	40012c00 	.word	0x40012c00

200079e8 <TIM_OC4Init>:
200079e8:	8c03      	ldrh	r3, [r0, #32]
200079ea:	b570      	push	{r4, r5, r6, lr}
200079ec:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
200079f0:	041b      	lsls	r3, r3, #16
200079f2:	0c1b      	lsrs	r3, r3, #16
200079f4:	8403      	strh	r3, [r0, #32]
200079f6:	8c03      	ldrh	r3, [r0, #32]
200079f8:	8884      	ldrh	r4, [r0, #4]
200079fa:	8b82      	ldrh	r2, [r0, #28]
200079fc:	880d      	ldrh	r5, [r1, #0]
200079fe:	f422 42e6 	bic.w	r2, r2, #29440	; 0x7300
20007a02:	0412      	lsls	r2, r2, #16
20007a04:	0c12      	lsrs	r2, r2, #16
20007a06:	ea42 2205 	orr.w	r2, r2, r5, lsl #8
20007a0a:	f423 5300 	bic.w	r3, r3, #8192	; 0x2000
20007a0e:	890d      	ldrh	r5, [r1, #8]
20007a10:	884e      	ldrh	r6, [r1, #2]
20007a12:	041b      	lsls	r3, r3, #16
20007a14:	4335      	orrs	r5, r6
20007a16:	0c1b      	lsrs	r3, r3, #16
20007a18:	ea43 3305 	orr.w	r3, r3, r5, lsl #12
20007a1c:	4d0b      	ldr	r5, [pc, #44]	; (20007a4c <TIM_OC4Init+0x64>)
20007a1e:	b2a4      	uxth	r4, r4
20007a20:	42a8      	cmp	r0, r5
20007a22:	b292      	uxth	r2, r2
20007a24:	b29b      	uxth	r3, r3
20007a26:	d003      	beq.n	20007a30 <TIM_OC4Init+0x48>
20007a28:	f505 6500 	add.w	r5, r5, #2048	; 0x800
20007a2c:	42a8      	cmp	r0, r5
20007a2e:	d105      	bne.n	20007a3c <TIM_OC4Init+0x54>
20007a30:	898d      	ldrh	r5, [r1, #12]
20007a32:	f424 4480 	bic.w	r4, r4, #16384	; 0x4000
20007a36:	ea44 1485 	orr.w	r4, r4, r5, lsl #6
20007a3a:	b2a4      	uxth	r4, r4
20007a3c:	8084      	strh	r4, [r0, #4]
20007a3e:	8382      	strh	r2, [r0, #28]
20007a40:	88ca      	ldrh	r2, [r1, #6]
20007a42:	f8a0 2040 	strh.w	r2, [r0, #64]	; 0x40
20007a46:	8403      	strh	r3, [r0, #32]
20007a48:	bd70      	pop	{r4, r5, r6, pc}
20007a4a:	bf00      	nop
20007a4c:	40012c00 	.word	0x40012c00

20007a50 <TIM_BDTRConfig>:
20007a50:	884a      	ldrh	r2, [r1, #2]
20007a52:	880b      	ldrh	r3, [r1, #0]
20007a54:	4313      	orrs	r3, r2
20007a56:	888a      	ldrh	r2, [r1, #4]
20007a58:	4313      	orrs	r3, r2
20007a5a:	88ca      	ldrh	r2, [r1, #6]
20007a5c:	4313      	orrs	r3, r2
20007a5e:	890a      	ldrh	r2, [r1, #8]
20007a60:	4313      	orrs	r3, r2
20007a62:	894a      	ldrh	r2, [r1, #10]
20007a64:	4313      	orrs	r3, r2
20007a66:	898a      	ldrh	r2, [r1, #12]
20007a68:	4313      	orrs	r3, r2
20007a6a:	f8a0 3044 	strh.w	r3, [r0, #68]	; 0x44
20007a6e:	4770      	bx	lr

20007a70 <TIM_TimeBaseStructInit>:
20007a70:	f64f 73ff 	movw	r3, #65535	; 0xffff
20007a74:	8083      	strh	r3, [r0, #4]
20007a76:	2300      	movs	r3, #0
20007a78:	8003      	strh	r3, [r0, #0]
20007a7a:	80c3      	strh	r3, [r0, #6]
20007a7c:	8043      	strh	r3, [r0, #2]
20007a7e:	7203      	strb	r3, [r0, #8]
20007a80:	4770      	bx	lr

20007a82 <TIM_OCStructInit>:
20007a82:	2300      	movs	r3, #0
20007a84:	8003      	strh	r3, [r0, #0]
20007a86:	8043      	strh	r3, [r0, #2]
20007a88:	8083      	strh	r3, [r0, #4]
20007a8a:	80c3      	strh	r3, [r0, #6]
20007a8c:	8103      	strh	r3, [r0, #8]
20007a8e:	8143      	strh	r3, [r0, #10]
20007a90:	8183      	strh	r3, [r0, #12]
20007a92:	81c3      	strh	r3, [r0, #14]
20007a94:	4770      	bx	lr

20007a96 <TIM_ICStructInit>:
20007a96:	2300      	movs	r3, #0
20007a98:	2201      	movs	r2, #1
20007a9a:	8003      	strh	r3, [r0, #0]
20007a9c:	8043      	strh	r3, [r0, #2]
20007a9e:	8082      	strh	r2, [r0, #4]
20007aa0:	80c3      	strh	r3, [r0, #6]
20007aa2:	8103      	strh	r3, [r0, #8]
20007aa4:	4770      	bx	lr

20007aa6 <TIM_BDTRStructInit>:
20007aa6:	2300      	movs	r3, #0
20007aa8:	8003      	strh	r3, [r0, #0]
20007aaa:	8043      	strh	r3, [r0, #2]
20007aac:	8083      	strh	r3, [r0, #4]
20007aae:	80c3      	strh	r3, [r0, #6]
20007ab0:	8103      	strh	r3, [r0, #8]
20007ab2:	8143      	strh	r3, [r0, #10]
20007ab4:	8183      	strh	r3, [r0, #12]
20007ab6:	4770      	bx	lr

20007ab8 <TIM_Cmd>:
20007ab8:	8803      	ldrh	r3, [r0, #0]
20007aba:	b119      	cbz	r1, 20007ac4 <TIM_Cmd+0xc>
20007abc:	f043 0301 	orr.w	r3, r3, #1
20007ac0:	8003      	strh	r3, [r0, #0]
20007ac2:	4770      	bx	lr
20007ac4:	f023 0301 	bic.w	r3, r3, #1
20007ac8:	041b      	lsls	r3, r3, #16
20007aca:	0c1b      	lsrs	r3, r3, #16
20007acc:	e7f8      	b.n	20007ac0 <TIM_Cmd+0x8>

20007ace <TIM_CtrlPWMOutputs>:
20007ace:	f8b0 3044 	ldrh.w	r3, [r0, #68]	; 0x44
20007ad2:	b121      	cbz	r1, 20007ade <TIM_CtrlPWMOutputs+0x10>
20007ad4:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
20007ad8:	f8a0 3044 	strh.w	r3, [r0, #68]	; 0x44
20007adc:	4770      	bx	lr
20007ade:	f3c3 030e 	ubfx	r3, r3, #0, #15
20007ae2:	e7f9      	b.n	20007ad8 <TIM_CtrlPWMOutputs+0xa>

20007ae4 <TIM_ITConfig>:
20007ae4:	8983      	ldrh	r3, [r0, #12]
20007ae6:	b112      	cbz	r2, 20007aee <TIM_ITConfig+0xa>
20007ae8:	4319      	orrs	r1, r3
20007aea:	8181      	strh	r1, [r0, #12]
20007aec:	4770      	bx	lr
20007aee:	ea23 0101 	bic.w	r1, r3, r1
20007af2:	e7fa      	b.n	20007aea <TIM_ITConfig+0x6>

20007af4 <TIM_GenerateEvent>:
20007af4:	8281      	strh	r1, [r0, #20]
20007af6:	4770      	bx	lr

20007af8 <TIM_DMAConfig>:
20007af8:	430a      	orrs	r2, r1
20007afa:	f8a0 2048 	strh.w	r2, [r0, #72]	; 0x48
20007afe:	4770      	bx	lr

20007b00 <TIM_DMACmd>:
20007b00:	f7ff bff0 	b.w	20007ae4 <TIM_ITConfig>

20007b04 <TIM_InternalClockConfig>:
20007b04:	8903      	ldrh	r3, [r0, #8]
20007b06:	f023 0307 	bic.w	r3, r3, #7
20007b0a:	041b      	lsls	r3, r3, #16
20007b0c:	0c1b      	lsrs	r3, r3, #16
20007b0e:	8103      	strh	r3, [r0, #8]
20007b10:	4770      	bx	lr

20007b12 <TIM_ITRxExternalClockConfig>:
20007b12:	8903      	ldrh	r3, [r0, #8]
20007b14:	f023 0370 	bic.w	r3, r3, #112	; 0x70
20007b18:	041b      	lsls	r3, r3, #16
20007b1a:	0c1b      	lsrs	r3, r3, #16
20007b1c:	4319      	orrs	r1, r3
20007b1e:	8101      	strh	r1, [r0, #8]
20007b20:	8903      	ldrh	r3, [r0, #8]
20007b22:	f043 0307 	orr.w	r3, r3, #7
20007b26:	8103      	strh	r3, [r0, #8]
20007b28:	4770      	bx	lr

20007b2a <TIM_TIxExternalClockConfig>:
20007b2a:	b538      	push	{r3, r4, r5, lr}
20007b2c:	460d      	mov	r5, r1
20007b2e:	2d60      	cmp	r5, #96	; 0x60
20007b30:	4611      	mov	r1, r2
20007b32:	4604      	mov	r4, r0
20007b34:	f04f 0201 	mov.w	r2, #1
20007b38:	d10d      	bne.n	20007b56 <TIM_TIxExternalClockConfig+0x2c>
20007b3a:	f7ff fd2b 	bl	20007594 <TI2_Config>
20007b3e:	8921      	ldrh	r1, [r4, #8]
20007b40:	f021 0170 	bic.w	r1, r1, #112	; 0x70
20007b44:	0409      	lsls	r1, r1, #16
20007b46:	0c09      	lsrs	r1, r1, #16
20007b48:	4329      	orrs	r1, r5
20007b4a:	8121      	strh	r1, [r4, #8]
20007b4c:	8923      	ldrh	r3, [r4, #8]
20007b4e:	f043 0307 	orr.w	r3, r3, #7
20007b52:	8123      	strh	r3, [r4, #8]
20007b54:	bd38      	pop	{r3, r4, r5, pc}
20007b56:	f7ff fce7 	bl	20007528 <TI1_Config>
20007b5a:	e7f0      	b.n	20007b3e <TIM_TIxExternalClockConfig+0x14>

20007b5c <TIM_ETRConfig>:
20007b5c:	b510      	push	{r4, lr}
20007b5e:	8904      	ldrh	r4, [r0, #8]
20007b60:	430a      	orrs	r2, r1
20007b62:	b2e4      	uxtb	r4, r4
20007b64:	ea42 2303 	orr.w	r3, r2, r3, lsl #8
20007b68:	431c      	orrs	r4, r3
20007b6a:	b2a4      	uxth	r4, r4
20007b6c:	8104      	strh	r4, [r0, #8]
20007b6e:	bd10      	pop	{r4, pc}

20007b70 <TIM_ETRClockMode1Config>:
20007b70:	b510      	push	{r4, lr}
20007b72:	f7ff fff3 	bl	20007b5c <TIM_ETRConfig>
20007b76:	8903      	ldrh	r3, [r0, #8]
20007b78:	f023 0377 	bic.w	r3, r3, #119	; 0x77
20007b7c:	041b      	lsls	r3, r3, #16
20007b7e:	0c1b      	lsrs	r3, r3, #16
20007b80:	f043 0377 	orr.w	r3, r3, #119	; 0x77
20007b84:	8103      	strh	r3, [r0, #8]
20007b86:	bd10      	pop	{r4, pc}

20007b88 <TIM_ETRClockMode2Config>:
20007b88:	b510      	push	{r4, lr}
20007b8a:	f7ff ffe7 	bl	20007b5c <TIM_ETRConfig>
20007b8e:	8903      	ldrh	r3, [r0, #8]
20007b90:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
20007b94:	8103      	strh	r3, [r0, #8]
20007b96:	bd10      	pop	{r4, pc}

20007b98 <TIM_PrescalerConfig>:
20007b98:	8501      	strh	r1, [r0, #40]	; 0x28
20007b9a:	8282      	strh	r2, [r0, #20]
20007b9c:	4770      	bx	lr

20007b9e <TIM_CounterModeConfig>:
20007b9e:	8803      	ldrh	r3, [r0, #0]
20007ba0:	f023 0370 	bic.w	r3, r3, #112	; 0x70
20007ba4:	041b      	lsls	r3, r3, #16
20007ba6:	0c1b      	lsrs	r3, r3, #16
20007ba8:	4319      	orrs	r1, r3
20007baa:	8001      	strh	r1, [r0, #0]
20007bac:	4770      	bx	lr

20007bae <TIM_SelectInputTrigger>:
20007bae:	8903      	ldrh	r3, [r0, #8]
20007bb0:	f023 0370 	bic.w	r3, r3, #112	; 0x70
20007bb4:	041b      	lsls	r3, r3, #16
20007bb6:	0c1b      	lsrs	r3, r3, #16
20007bb8:	4319      	orrs	r1, r3
20007bba:	8101      	strh	r1, [r0, #8]
20007bbc:	4770      	bx	lr

20007bbe <TIM_EncoderInterfaceConfig>:
20007bbe:	b570      	push	{r4, r5, r6, lr}
20007bc0:	8905      	ldrh	r5, [r0, #8]
20007bc2:	8b06      	ldrh	r6, [r0, #24]
20007bc4:	f025 0507 	bic.w	r5, r5, #7
20007bc8:	8c04      	ldrh	r4, [r0, #32]
20007bca:	042d      	lsls	r5, r5, #16
20007bcc:	0c2d      	lsrs	r5, r5, #16
20007bce:	f024 0422 	bic.w	r4, r4, #34	; 0x22
20007bd2:	4329      	orrs	r1, r5
20007bd4:	0424      	lsls	r4, r4, #16
20007bd6:	f426 7540 	bic.w	r5, r6, #768	; 0x300
20007bda:	f025 0503 	bic.w	r5, r5, #3
20007bde:	0c24      	lsrs	r4, r4, #16
20007be0:	ea42 1303 	orr.w	r3, r2, r3, lsl #4
20007be4:	f445 7580 	orr.w	r5, r5, #256	; 0x100
20007be8:	431c      	orrs	r4, r3
20007bea:	f045 0501 	orr.w	r5, r5, #1
20007bee:	b2a4      	uxth	r4, r4
20007bf0:	8101      	strh	r1, [r0, #8]
20007bf2:	8305      	strh	r5, [r0, #24]
20007bf4:	8404      	strh	r4, [r0, #32]
20007bf6:	bd70      	pop	{r4, r5, r6, pc}

20007bf8 <TIM_ForcedOC1Config>:
20007bf8:	8b03      	ldrh	r3, [r0, #24]
20007bfa:	f023 0370 	bic.w	r3, r3, #112	; 0x70
20007bfe:	041b      	lsls	r3, r3, #16
20007c00:	0c1b      	lsrs	r3, r3, #16
20007c02:	4319      	orrs	r1, r3
20007c04:	8301      	strh	r1, [r0, #24]
20007c06:	4770      	bx	lr

20007c08 <TIM_ForcedOC2Config>:
20007c08:	8b03      	ldrh	r3, [r0, #24]
20007c0a:	f423 43e0 	bic.w	r3, r3, #28672	; 0x7000
20007c0e:	041b      	lsls	r3, r3, #16
20007c10:	0c1b      	lsrs	r3, r3, #16
20007c12:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007c16:	b289      	uxth	r1, r1
20007c18:	8301      	strh	r1, [r0, #24]
20007c1a:	4770      	bx	lr

20007c1c <TIM_ForcedOC3Config>:
20007c1c:	8b83      	ldrh	r3, [r0, #28]
20007c1e:	f023 0370 	bic.w	r3, r3, #112	; 0x70
20007c22:	041b      	lsls	r3, r3, #16
20007c24:	0c1b      	lsrs	r3, r3, #16
20007c26:	4319      	orrs	r1, r3
20007c28:	8381      	strh	r1, [r0, #28]
20007c2a:	4770      	bx	lr

20007c2c <TIM_ForcedOC4Config>:
20007c2c:	8b83      	ldrh	r3, [r0, #28]
20007c2e:	f423 43e0 	bic.w	r3, r3, #28672	; 0x7000
20007c32:	041b      	lsls	r3, r3, #16
20007c34:	0c1b      	lsrs	r3, r3, #16
20007c36:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007c3a:	b289      	uxth	r1, r1
20007c3c:	8381      	strh	r1, [r0, #28]
20007c3e:	4770      	bx	lr

20007c40 <TIM_ARRPreloadConfig>:
20007c40:	8803      	ldrh	r3, [r0, #0]
20007c42:	b119      	cbz	r1, 20007c4c <TIM_ARRPreloadConfig+0xc>
20007c44:	f043 0380 	orr.w	r3, r3, #128	; 0x80
20007c48:	8003      	strh	r3, [r0, #0]
20007c4a:	4770      	bx	lr
20007c4c:	f023 0380 	bic.w	r3, r3, #128	; 0x80
20007c50:	041b      	lsls	r3, r3, #16
20007c52:	0c1b      	lsrs	r3, r3, #16
20007c54:	e7f8      	b.n	20007c48 <TIM_ARRPreloadConfig+0x8>

20007c56 <TIM_SelectCOM>:
20007c56:	8883      	ldrh	r3, [r0, #4]
20007c58:	b119      	cbz	r1, 20007c62 <TIM_SelectCOM+0xc>
20007c5a:	f043 0304 	orr.w	r3, r3, #4
20007c5e:	8083      	strh	r3, [r0, #4]
20007c60:	4770      	bx	lr
20007c62:	f023 0304 	bic.w	r3, r3, #4
20007c66:	041b      	lsls	r3, r3, #16
20007c68:	0c1b      	lsrs	r3, r3, #16
20007c6a:	e7f8      	b.n	20007c5e <TIM_SelectCOM+0x8>

20007c6c <TIM_SelectCCDMA>:
20007c6c:	8883      	ldrh	r3, [r0, #4]
20007c6e:	b119      	cbz	r1, 20007c78 <TIM_SelectCCDMA+0xc>
20007c70:	f043 0308 	orr.w	r3, r3, #8
20007c74:	8083      	strh	r3, [r0, #4]
20007c76:	4770      	bx	lr
20007c78:	f023 0308 	bic.w	r3, r3, #8
20007c7c:	041b      	lsls	r3, r3, #16
20007c7e:	0c1b      	lsrs	r3, r3, #16
20007c80:	e7f8      	b.n	20007c74 <TIM_SelectCCDMA+0x8>

20007c82 <TIM_CCPreloadControl>:
20007c82:	8883      	ldrh	r3, [r0, #4]
20007c84:	b119      	cbz	r1, 20007c8e <TIM_CCPreloadControl+0xc>
20007c86:	f043 0301 	orr.w	r3, r3, #1
20007c8a:	8083      	strh	r3, [r0, #4]
20007c8c:	4770      	bx	lr
20007c8e:	f023 0301 	bic.w	r3, r3, #1
20007c92:	041b      	lsls	r3, r3, #16
20007c94:	0c1b      	lsrs	r3, r3, #16
20007c96:	e7f8      	b.n	20007c8a <TIM_CCPreloadControl+0x8>

20007c98 <TIM_OC1PreloadConfig>:
20007c98:	8b03      	ldrh	r3, [r0, #24]
20007c9a:	f023 0308 	bic.w	r3, r3, #8
20007c9e:	041b      	lsls	r3, r3, #16
20007ca0:	0c1b      	lsrs	r3, r3, #16
20007ca2:	4319      	orrs	r1, r3
20007ca4:	8301      	strh	r1, [r0, #24]
20007ca6:	4770      	bx	lr

20007ca8 <TIM_OC2PreloadConfig>:
20007ca8:	8b03      	ldrh	r3, [r0, #24]
20007caa:	f423 6300 	bic.w	r3, r3, #2048	; 0x800
20007cae:	041b      	lsls	r3, r3, #16
20007cb0:	0c1b      	lsrs	r3, r3, #16
20007cb2:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007cb6:	b289      	uxth	r1, r1
20007cb8:	8301      	strh	r1, [r0, #24]
20007cba:	4770      	bx	lr

20007cbc <TIM_OC3PreloadConfig>:
20007cbc:	8b83      	ldrh	r3, [r0, #28]
20007cbe:	f023 0308 	bic.w	r3, r3, #8
20007cc2:	041b      	lsls	r3, r3, #16
20007cc4:	0c1b      	lsrs	r3, r3, #16
20007cc6:	4319      	orrs	r1, r3
20007cc8:	8381      	strh	r1, [r0, #28]
20007cca:	4770      	bx	lr

20007ccc <TIM_OC4PreloadConfig>:
20007ccc:	8b83      	ldrh	r3, [r0, #28]
20007cce:	f423 6300 	bic.w	r3, r3, #2048	; 0x800
20007cd2:	041b      	lsls	r3, r3, #16
20007cd4:	0c1b      	lsrs	r3, r3, #16
20007cd6:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007cda:	b289      	uxth	r1, r1
20007cdc:	8381      	strh	r1, [r0, #28]
20007cde:	4770      	bx	lr

20007ce0 <TIM_OC1FastConfig>:
20007ce0:	8b03      	ldrh	r3, [r0, #24]
20007ce2:	f023 0304 	bic.w	r3, r3, #4
20007ce6:	041b      	lsls	r3, r3, #16
20007ce8:	0c1b      	lsrs	r3, r3, #16
20007cea:	4319      	orrs	r1, r3
20007cec:	8301      	strh	r1, [r0, #24]
20007cee:	4770      	bx	lr

20007cf0 <TIM_OC2FastConfig>:
20007cf0:	8b03      	ldrh	r3, [r0, #24]
20007cf2:	f423 6380 	bic.w	r3, r3, #1024	; 0x400
20007cf6:	041b      	lsls	r3, r3, #16
20007cf8:	0c1b      	lsrs	r3, r3, #16
20007cfa:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007cfe:	b289      	uxth	r1, r1
20007d00:	8301      	strh	r1, [r0, #24]
20007d02:	4770      	bx	lr

20007d04 <TIM_OC3FastConfig>:
20007d04:	8b83      	ldrh	r3, [r0, #28]
20007d06:	f023 0304 	bic.w	r3, r3, #4
20007d0a:	041b      	lsls	r3, r3, #16
20007d0c:	0c1b      	lsrs	r3, r3, #16
20007d0e:	4319      	orrs	r1, r3
20007d10:	8381      	strh	r1, [r0, #28]
20007d12:	4770      	bx	lr

20007d14 <TIM_OC4FastConfig>:
20007d14:	8b83      	ldrh	r3, [r0, #28]
20007d16:	f423 6380 	bic.w	r3, r3, #1024	; 0x400
20007d1a:	041b      	lsls	r3, r3, #16
20007d1c:	0c1b      	lsrs	r3, r3, #16
20007d1e:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007d22:	b289      	uxth	r1, r1
20007d24:	8381      	strh	r1, [r0, #28]
20007d26:	4770      	bx	lr

20007d28 <TIM_ClearOC1Ref>:
20007d28:	8b03      	ldrh	r3, [r0, #24]
20007d2a:	f023 0380 	bic.w	r3, r3, #128	; 0x80
20007d2e:	041b      	lsls	r3, r3, #16
20007d30:	0c1b      	lsrs	r3, r3, #16
20007d32:	4319      	orrs	r1, r3
20007d34:	8301      	strh	r1, [r0, #24]
20007d36:	4770      	bx	lr

20007d38 <TIM_ClearOC2Ref>:
20007d38:	8b03      	ldrh	r3, [r0, #24]
20007d3a:	f3c3 030e 	ubfx	r3, r3, #0, #15
20007d3e:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007d42:	b289      	uxth	r1, r1
20007d44:	8301      	strh	r1, [r0, #24]
20007d46:	4770      	bx	lr

20007d48 <TIM_ClearOC3Ref>:
20007d48:	8b83      	ldrh	r3, [r0, #28]
20007d4a:	f023 0380 	bic.w	r3, r3, #128	; 0x80
20007d4e:	041b      	lsls	r3, r3, #16
20007d50:	0c1b      	lsrs	r3, r3, #16
20007d52:	4319      	orrs	r1, r3
20007d54:	8381      	strh	r1, [r0, #28]
20007d56:	4770      	bx	lr

20007d58 <TIM_ClearOC4Ref>:
20007d58:	8b83      	ldrh	r3, [r0, #28]
20007d5a:	f3c3 030e 	ubfx	r3, r3, #0, #15
20007d5e:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007d62:	b289      	uxth	r1, r1
20007d64:	8381      	strh	r1, [r0, #28]
20007d66:	4770      	bx	lr

20007d68 <TIM_OC1PolarityConfig>:
20007d68:	8c03      	ldrh	r3, [r0, #32]
20007d6a:	f023 0302 	bic.w	r3, r3, #2
20007d6e:	041b      	lsls	r3, r3, #16
20007d70:	0c1b      	lsrs	r3, r3, #16
20007d72:	4319      	orrs	r1, r3
20007d74:	8401      	strh	r1, [r0, #32]
20007d76:	4770      	bx	lr

20007d78 <TIM_OC1NPolarityConfig>:
20007d78:	8c03      	ldrh	r3, [r0, #32]
20007d7a:	f023 0308 	bic.w	r3, r3, #8
20007d7e:	041b      	lsls	r3, r3, #16
20007d80:	0c1b      	lsrs	r3, r3, #16
20007d82:	4319      	orrs	r1, r3
20007d84:	8401      	strh	r1, [r0, #32]
20007d86:	4770      	bx	lr

20007d88 <TIM_OC2PolarityConfig>:
20007d88:	8c03      	ldrh	r3, [r0, #32]
20007d8a:	f023 0320 	bic.w	r3, r3, #32
20007d8e:	041b      	lsls	r3, r3, #16
20007d90:	0c1b      	lsrs	r3, r3, #16
20007d92:	ea43 1101 	orr.w	r1, r3, r1, lsl #4
20007d96:	b289      	uxth	r1, r1
20007d98:	8401      	strh	r1, [r0, #32]
20007d9a:	4770      	bx	lr

20007d9c <TIM_OC2NPolarityConfig>:
20007d9c:	8c03      	ldrh	r3, [r0, #32]
20007d9e:	f023 0380 	bic.w	r3, r3, #128	; 0x80
20007da2:	041b      	lsls	r3, r3, #16
20007da4:	0c1b      	lsrs	r3, r3, #16
20007da6:	ea43 1101 	orr.w	r1, r3, r1, lsl #4
20007daa:	b289      	uxth	r1, r1
20007dac:	8401      	strh	r1, [r0, #32]
20007dae:	4770      	bx	lr

20007db0 <TIM_OC3PolarityConfig>:
20007db0:	8c03      	ldrh	r3, [r0, #32]
20007db2:	f423 7300 	bic.w	r3, r3, #512	; 0x200
20007db6:	041b      	lsls	r3, r3, #16
20007db8:	0c1b      	lsrs	r3, r3, #16
20007dba:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007dbe:	b289      	uxth	r1, r1
20007dc0:	8401      	strh	r1, [r0, #32]
20007dc2:	4770      	bx	lr

20007dc4 <TIM_OC3NPolarityConfig>:
20007dc4:	8c03      	ldrh	r3, [r0, #32]
20007dc6:	f423 6300 	bic.w	r3, r3, #2048	; 0x800
20007dca:	041b      	lsls	r3, r3, #16
20007dcc:	0c1b      	lsrs	r3, r3, #16
20007dce:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007dd2:	b289      	uxth	r1, r1
20007dd4:	8401      	strh	r1, [r0, #32]
20007dd6:	4770      	bx	lr

20007dd8 <TIM_OC4PolarityConfig>:
20007dd8:	8c03      	ldrh	r3, [r0, #32]
20007dda:	f423 5300 	bic.w	r3, r3, #8192	; 0x2000
20007dde:	041b      	lsls	r3, r3, #16
20007de0:	0c1b      	lsrs	r3, r3, #16
20007de2:	ea43 3101 	orr.w	r1, r3, r1, lsl #12
20007de6:	b289      	uxth	r1, r1
20007de8:	8401      	strh	r1, [r0, #32]
20007dea:	4770      	bx	lr

20007dec <TIM_CCxCmd>:
20007dec:	b510      	push	{r4, lr}
20007dee:	2401      	movs	r4, #1
20007df0:	8c03      	ldrh	r3, [r0, #32]
20007df2:	408c      	lsls	r4, r1
20007df4:	ea23 0304 	bic.w	r3, r3, r4
20007df8:	8403      	strh	r3, [r0, #32]
20007dfa:	8c03      	ldrh	r3, [r0, #32]
20007dfc:	fa02 f101 	lsl.w	r1, r2, r1
20007e00:	4319      	orrs	r1, r3
20007e02:	b289      	uxth	r1, r1
20007e04:	8401      	strh	r1, [r0, #32]
20007e06:	bd10      	pop	{r4, pc}

20007e08 <TIM_CCxNCmd>:
20007e08:	b510      	push	{r4, lr}
20007e0a:	2404      	movs	r4, #4
20007e0c:	8c03      	ldrh	r3, [r0, #32]
20007e0e:	408c      	lsls	r4, r1
20007e10:	ea23 0304 	bic.w	r3, r3, r4
20007e14:	8403      	strh	r3, [r0, #32]
20007e16:	8c03      	ldrh	r3, [r0, #32]
20007e18:	fa02 f101 	lsl.w	r1, r2, r1
20007e1c:	4319      	orrs	r1, r3
20007e1e:	b289      	uxth	r1, r1
20007e20:	8401      	strh	r1, [r0, #32]
20007e22:	bd10      	pop	{r4, pc}

20007e24 <TIM_SelectOCxM>:
20007e24:	b530      	push	{r4, r5, lr}
20007e26:	2501      	movs	r5, #1
20007e28:	8c04      	ldrh	r4, [r0, #32]
20007e2a:	408d      	lsls	r5, r1
20007e2c:	ea24 0405 	bic.w	r4, r4, r5
20007e30:	f100 0318 	add.w	r3, r0, #24
20007e34:	8404      	strh	r4, [r0, #32]
20007e36:	f021 0008 	bic.w	r0, r1, #8
20007e3a:	b940      	cbnz	r0, 20007e4e <TIM_SelectOCxM+0x2a>
20007e3c:	0849      	lsrs	r1, r1, #1
20007e3e:	58c8      	ldr	r0, [r1, r3]
20007e40:	f020 0070 	bic.w	r0, r0, #112	; 0x70
20007e44:	50c8      	str	r0, [r1, r3]
20007e46:	58c8      	ldr	r0, [r1, r3]
20007e48:	4302      	orrs	r2, r0
20007e4a:	50ca      	str	r2, [r1, r3]
20007e4c:	bd30      	pop	{r4, r5, pc}
20007e4e:	3904      	subs	r1, #4
20007e50:	f3c1 014e 	ubfx	r1, r1, #1, #15
20007e54:	58c8      	ldr	r0, [r1, r3]
20007e56:	0212      	lsls	r2, r2, #8
20007e58:	f420 40e0 	bic.w	r0, r0, #28672	; 0x7000
20007e5c:	50c8      	str	r0, [r1, r3]
20007e5e:	58c8      	ldr	r0, [r1, r3]
20007e60:	b292      	uxth	r2, r2
20007e62:	e7f1      	b.n	20007e48 <TIM_SelectOCxM+0x24>

20007e64 <TIM_UpdateDisableConfig>:
20007e64:	8803      	ldrh	r3, [r0, #0]
20007e66:	b119      	cbz	r1, 20007e70 <TIM_UpdateDisableConfig+0xc>
20007e68:	f043 0302 	orr.w	r3, r3, #2
20007e6c:	8003      	strh	r3, [r0, #0]
20007e6e:	4770      	bx	lr
20007e70:	f023 0302 	bic.w	r3, r3, #2
20007e74:	041b      	lsls	r3, r3, #16
20007e76:	0c1b      	lsrs	r3, r3, #16
20007e78:	e7f8      	b.n	20007e6c <TIM_UpdateDisableConfig+0x8>

20007e7a <TIM_UpdateRequestConfig>:
20007e7a:	8803      	ldrh	r3, [r0, #0]
20007e7c:	b119      	cbz	r1, 20007e86 <TIM_UpdateRequestConfig+0xc>
20007e7e:	f043 0304 	orr.w	r3, r3, #4
20007e82:	8003      	strh	r3, [r0, #0]
20007e84:	4770      	bx	lr
20007e86:	f023 0304 	bic.w	r3, r3, #4
20007e8a:	041b      	lsls	r3, r3, #16
20007e8c:	0c1b      	lsrs	r3, r3, #16
20007e8e:	e7f8      	b.n	20007e82 <TIM_UpdateRequestConfig+0x8>

20007e90 <TIM_SelectHallSensor>:
20007e90:	8883      	ldrh	r3, [r0, #4]
20007e92:	b119      	cbz	r1, 20007e9c <TIM_SelectHallSensor+0xc>
20007e94:	f043 0380 	orr.w	r3, r3, #128	; 0x80
20007e98:	8083      	strh	r3, [r0, #4]
20007e9a:	4770      	bx	lr
20007e9c:	f023 0380 	bic.w	r3, r3, #128	; 0x80
20007ea0:	041b      	lsls	r3, r3, #16
20007ea2:	0c1b      	lsrs	r3, r3, #16
20007ea4:	e7f8      	b.n	20007e98 <TIM_SelectHallSensor+0x8>

20007ea6 <TIM_SelectOnePulseMode>:
20007ea6:	8803      	ldrh	r3, [r0, #0]
20007ea8:	f023 0308 	bic.w	r3, r3, #8
20007eac:	041b      	lsls	r3, r3, #16
20007eae:	0c1b      	lsrs	r3, r3, #16
20007eb0:	8003      	strh	r3, [r0, #0]
20007eb2:	8803      	ldrh	r3, [r0, #0]
20007eb4:	4319      	orrs	r1, r3
20007eb6:	8001      	strh	r1, [r0, #0]
20007eb8:	4770      	bx	lr

20007eba <TIM_SelectOutputTrigger>:
20007eba:	8883      	ldrh	r3, [r0, #4]
20007ebc:	f023 0370 	bic.w	r3, r3, #112	; 0x70
20007ec0:	041b      	lsls	r3, r3, #16
20007ec2:	0c1b      	lsrs	r3, r3, #16
20007ec4:	8083      	strh	r3, [r0, #4]
20007ec6:	8883      	ldrh	r3, [r0, #4]
20007ec8:	4319      	orrs	r1, r3
20007eca:	8081      	strh	r1, [r0, #4]
20007ecc:	4770      	bx	lr

20007ece <TIM_SelectSlaveMode>:
20007ece:	8903      	ldrh	r3, [r0, #8]
20007ed0:	f023 0307 	bic.w	r3, r3, #7
20007ed4:	041b      	lsls	r3, r3, #16
20007ed6:	0c1b      	lsrs	r3, r3, #16
20007ed8:	8103      	strh	r3, [r0, #8]
20007eda:	8903      	ldrh	r3, [r0, #8]
20007edc:	4319      	orrs	r1, r3
20007ede:	8101      	strh	r1, [r0, #8]
20007ee0:	4770      	bx	lr

20007ee2 <TIM_SelectMasterSlaveMode>:
20007ee2:	8903      	ldrh	r3, [r0, #8]
20007ee4:	f023 0380 	bic.w	r3, r3, #128	; 0x80
20007ee8:	041b      	lsls	r3, r3, #16
20007eea:	0c1b      	lsrs	r3, r3, #16
20007eec:	8103      	strh	r3, [r0, #8]
20007eee:	8903      	ldrh	r3, [r0, #8]
20007ef0:	4319      	orrs	r1, r3
20007ef2:	8101      	strh	r1, [r0, #8]
20007ef4:	4770      	bx	lr

20007ef6 <TIM_SetCounter>:
20007ef6:	8481      	strh	r1, [r0, #36]	; 0x24
20007ef8:	4770      	bx	lr

20007efa <TIM_SetAutoreload>:
20007efa:	8581      	strh	r1, [r0, #44]	; 0x2c
20007efc:	4770      	bx	lr

20007efe <TIM_SetCompare1>:
20007efe:	8681      	strh	r1, [r0, #52]	; 0x34
20007f00:	4770      	bx	lr

20007f02 <TIM_SetCompare2>:
20007f02:	8701      	strh	r1, [r0, #56]	; 0x38
20007f04:	4770      	bx	lr

20007f06 <TIM_SetCompare3>:
20007f06:	8781      	strh	r1, [r0, #60]	; 0x3c
20007f08:	4770      	bx	lr

20007f0a <TIM_SetCompare4>:
20007f0a:	f8a0 1040 	strh.w	r1, [r0, #64]	; 0x40
20007f0e:	4770      	bx	lr

20007f10 <TIM_SetIC1Prescaler>:
20007f10:	8b03      	ldrh	r3, [r0, #24]
20007f12:	f023 030c 	bic.w	r3, r3, #12
20007f16:	041b      	lsls	r3, r3, #16
20007f18:	0c1b      	lsrs	r3, r3, #16
20007f1a:	8303      	strh	r3, [r0, #24]
20007f1c:	8b03      	ldrh	r3, [r0, #24]
20007f1e:	4319      	orrs	r1, r3
20007f20:	8301      	strh	r1, [r0, #24]
20007f22:	4770      	bx	lr

20007f24 <TIM_SetIC2Prescaler>:
20007f24:	8b03      	ldrh	r3, [r0, #24]
20007f26:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
20007f2a:	041b      	lsls	r3, r3, #16
20007f2c:	0c1b      	lsrs	r3, r3, #16
20007f2e:	8303      	strh	r3, [r0, #24]
20007f30:	8b03      	ldrh	r3, [r0, #24]
20007f32:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007f36:	b289      	uxth	r1, r1
20007f38:	8301      	strh	r1, [r0, #24]
20007f3a:	4770      	bx	lr

20007f3c <TIM_PWMIConfig>:
20007f3c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
20007f3e:	460c      	mov	r4, r1
20007f40:	8849      	ldrh	r1, [r1, #2]
20007f42:	88a2      	ldrh	r2, [r4, #4]
20007f44:	2900      	cmp	r1, #0
20007f46:	f8b4 c000 	ldrh.w	ip, [r4]
20007f4a:	bf14      	ite	ne
20007f4c:	2600      	movne	r6, #0
20007f4e:	2602      	moveq	r6, #2
20007f50:	2a01      	cmp	r2, #1
20007f52:	bf14      	ite	ne
20007f54:	2701      	movne	r7, #1
20007f56:	2702      	moveq	r7, #2
20007f58:	8923      	ldrh	r3, [r4, #8]
20007f5a:	f1bc 0f00 	cmp.w	ip, #0
20007f5e:	d10e      	bne.n	20007f7e <TIM_PWMIConfig+0x42>
20007f60:	f7ff fae2 	bl	20007528 <TI1_Config>
20007f64:	88e1      	ldrh	r1, [r4, #6]
20007f66:	f7ff ffd3 	bl	20007f10 <TIM_SetIC1Prescaler>
20007f6a:	4631      	mov	r1, r6
20007f6c:	8923      	ldrh	r3, [r4, #8]
20007f6e:	463a      	mov	r2, r7
20007f70:	f7ff fb10 	bl	20007594 <TI2_Config>
20007f74:	88e1      	ldrh	r1, [r4, #6]
20007f76:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
20007f7a:	f7ff bfd3 	b.w	20007f24 <TIM_SetIC2Prescaler>
20007f7e:	f7ff fb09 	bl	20007594 <TI2_Config>
20007f82:	88e1      	ldrh	r1, [r4, #6]
20007f84:	f7ff ffce 	bl	20007f24 <TIM_SetIC2Prescaler>
20007f88:	4631      	mov	r1, r6
20007f8a:	8923      	ldrh	r3, [r4, #8]
20007f8c:	463a      	mov	r2, r7
20007f8e:	f7ff facb 	bl	20007528 <TI1_Config>
20007f92:	88e1      	ldrh	r1, [r4, #6]
20007f94:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
20007f98:	f7ff bfba 	b.w	20007f10 <TIM_SetIC1Prescaler>

20007f9c <TIM_SetIC3Prescaler>:
20007f9c:	8b83      	ldrh	r3, [r0, #28]
20007f9e:	f023 030c 	bic.w	r3, r3, #12
20007fa2:	041b      	lsls	r3, r3, #16
20007fa4:	0c1b      	lsrs	r3, r3, #16
20007fa6:	8383      	strh	r3, [r0, #28]
20007fa8:	8b83      	ldrh	r3, [r0, #28]
20007faa:	4319      	orrs	r1, r3
20007fac:	8381      	strh	r1, [r0, #28]
20007fae:	4770      	bx	lr

20007fb0 <TIM_SetIC4Prescaler>:
20007fb0:	8b83      	ldrh	r3, [r0, #28]
20007fb2:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
20007fb6:	041b      	lsls	r3, r3, #16
20007fb8:	0c1b      	lsrs	r3, r3, #16
20007fba:	8383      	strh	r3, [r0, #28]
20007fbc:	8b83      	ldrh	r3, [r0, #28]
20007fbe:	ea43 2101 	orr.w	r1, r3, r1, lsl #8
20007fc2:	b289      	uxth	r1, r1
20007fc4:	8381      	strh	r1, [r0, #28]
20007fc6:	4770      	bx	lr

20007fc8 <TIM_ICInit>:
20007fc8:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
20007fca:	460e      	mov	r6, r1
20007fcc:	8809      	ldrh	r1, [r1, #0]
20007fce:	4604      	mov	r4, r0
20007fd0:	8875      	ldrh	r5, [r6, #2]
20007fd2:	88b2      	ldrh	r2, [r6, #4]
20007fd4:	8933      	ldrh	r3, [r6, #8]
20007fd6:	b939      	cbnz	r1, 20007fe8 <TIM_ICInit+0x20>
20007fd8:	4629      	mov	r1, r5
20007fda:	f7ff faa5 	bl	20007528 <TI1_Config>
20007fde:	88f1      	ldrh	r1, [r6, #6]
20007fe0:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
20007fe4:	f7ff bf94 	b.w	20007f10 <TIM_SetIC1Prescaler>
20007fe8:	2904      	cmp	r1, #4
20007fea:	d107      	bne.n	20007ffc <TIM_ICInit+0x34>
20007fec:	4629      	mov	r1, r5
20007fee:	f7ff fad1 	bl	20007594 <TI2_Config>
20007ff2:	88f1      	ldrh	r1, [r6, #6]
20007ff4:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
20007ff8:	f7ff bf94 	b.w	20007f24 <TIM_SetIC2Prescaler>
20007ffc:	2908      	cmp	r1, #8
20007ffe:	8c00      	ldrh	r0, [r0, #32]
20008000:	88f1      	ldrh	r1, [r6, #6]
20008002:	d138      	bne.n	20008076 <TIM_ICInit+0xae>
20008004:	f420 7080 	bic.w	r0, r0, #256	; 0x100
20008008:	0400      	lsls	r0, r0, #16
2000800a:	0c00      	lsrs	r0, r0, #16
2000800c:	8420      	strh	r0, [r4, #32]
2000800e:	8ba6      	ldrh	r6, [r4, #28]
20008010:	8c20      	ldrh	r0, [r4, #32]
20008012:	b2b6      	uxth	r6, r6
20008014:	f026 06f3 	bic.w	r6, r6, #243	; 0xf3
20008018:	4332      	orrs	r2, r6
2000801a:	ea42 1303 	orr.w	r3, r2, r3, lsl #4
2000801e:	4a34      	ldr	r2, [pc, #208]	; (200080f0 <TIM_ICInit+0x128>)
20008020:	022f      	lsls	r7, r5, #8
20008022:	4294      	cmp	r4, r2
20008024:	b280      	uxth	r0, r0
20008026:	b2bf      	uxth	r7, r7
20008028:	b29b      	uxth	r3, r3
2000802a:	d012      	beq.n	20008052 <TIM_ICInit+0x8a>
2000802c:	f502 6200 	add.w	r2, r2, #2048	; 0x800
20008030:	4294      	cmp	r4, r2
20008032:	d00e      	beq.n	20008052 <TIM_ICInit+0x8a>
20008034:	f1b4 4f80 	cmp.w	r4, #1073741824	; 0x40000000
20008038:	d00b      	beq.n	20008052 <TIM_ICInit+0x8a>
2000803a:	f5a2 3298 	sub.w	r2, r2, #77824	; 0x13000
2000803e:	4294      	cmp	r4, r2
20008040:	d007      	beq.n	20008052 <TIM_ICInit+0x8a>
20008042:	f502 6280 	add.w	r2, r2, #1024	; 0x400
20008046:	4294      	cmp	r4, r2
20008048:	d003      	beq.n	20008052 <TIM_ICInit+0x8a>
2000804a:	f502 6280 	add.w	r2, r2, #1024	; 0x400
2000804e:	4294      	cmp	r4, r2
20008050:	d10b      	bne.n	2000806a <TIM_ICInit+0xa2>
20008052:	f420 7500 	bic.w	r5, r0, #512	; 0x200
20008056:	f445 7580 	orr.w	r5, r5, #256	; 0x100
2000805a:	433d      	orrs	r5, r7
2000805c:	83a3      	strh	r3, [r4, #28]
2000805e:	4620      	mov	r0, r4
20008060:	8425      	strh	r5, [r4, #32]
20008062:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
20008066:	f7ff bf99 	b.w	20007f9c <TIM_SetIC3Prescaler>
2000806a:	f420 6020 	bic.w	r0, r0, #2560	; 0xa00
2000806e:	4305      	orrs	r5, r0
20008070:	f445 7580 	orr.w	r5, r5, #256	; 0x100
20008074:	e7f2      	b.n	2000805c <TIM_ICInit+0x94>
20008076:	f420 5080 	bic.w	r0, r0, #4096	; 0x1000
2000807a:	0400      	lsls	r0, r0, #16
2000807c:	0c00      	lsrs	r0, r0, #16
2000807e:	8420      	strh	r0, [r4, #32]
20008080:	8ba0      	ldrh	r0, [r4, #28]
20008082:	031b      	lsls	r3, r3, #12
20008084:	f420 7040 	bic.w	r0, r0, #768	; 0x300
20008088:	ea43 2302 	orr.w	r3, r3, r2, lsl #8
2000808c:	0500      	lsls	r0, r0, #20
2000808e:	4a18      	ldr	r2, [pc, #96]	; (200080f0 <TIM_ICInit+0x128>)
20008090:	8c27      	ldrh	r7, [r4, #32]
20008092:	0d00      	lsrs	r0, r0, #20
20008094:	032e      	lsls	r6, r5, #12
20008096:	4303      	orrs	r3, r0
20008098:	4294      	cmp	r4, r2
2000809a:	b2bf      	uxth	r7, r7
2000809c:	b2b6      	uxth	r6, r6
2000809e:	b29b      	uxth	r3, r3
200080a0:	d012      	beq.n	200080c8 <TIM_ICInit+0x100>
200080a2:	f502 6200 	add.w	r2, r2, #2048	; 0x800
200080a6:	4294      	cmp	r4, r2
200080a8:	d00e      	beq.n	200080c8 <TIM_ICInit+0x100>
200080aa:	f1b4 4f80 	cmp.w	r4, #1073741824	; 0x40000000
200080ae:	d00b      	beq.n	200080c8 <TIM_ICInit+0x100>
200080b0:	f5a2 3298 	sub.w	r2, r2, #77824	; 0x13000
200080b4:	4294      	cmp	r4, r2
200080b6:	d007      	beq.n	200080c8 <TIM_ICInit+0x100>
200080b8:	f502 6280 	add.w	r2, r2, #1024	; 0x400
200080bc:	4294      	cmp	r4, r2
200080be:	d003      	beq.n	200080c8 <TIM_ICInit+0x100>
200080c0:	f502 6280 	add.w	r2, r2, #1024	; 0x400
200080c4:	4294      	cmp	r4, r2
200080c6:	d10b      	bne.n	200080e0 <TIM_ICInit+0x118>
200080c8:	f427 5500 	bic.w	r5, r7, #8192	; 0x2000
200080cc:	f445 5580 	orr.w	r5, r5, #4096	; 0x1000
200080d0:	4335      	orrs	r5, r6
200080d2:	83a3      	strh	r3, [r4, #28]
200080d4:	4620      	mov	r0, r4
200080d6:	8425      	strh	r5, [r4, #32]
200080d8:	e8bd 40f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, lr}
200080dc:	f7ff bf68 	b.w	20007fb0 <TIM_SetIC4Prescaler>
200080e0:	f427 7700 	bic.w	r7, r7, #512	; 0x200
200080e4:	047f      	lsls	r7, r7, #17
200080e6:	0c7f      	lsrs	r7, r7, #17
200080e8:	433d      	orrs	r5, r7
200080ea:	f445 5580 	orr.w	r5, r5, #4096	; 0x1000
200080ee:	e7f0      	b.n	200080d2 <TIM_ICInit+0x10a>
200080f0:	40012c00 	.word	0x40012c00

200080f4 <TIM_SetClockDivision>:
200080f4:	8803      	ldrh	r3, [r0, #0]
200080f6:	f423 7340 	bic.w	r3, r3, #768	; 0x300
200080fa:	041b      	lsls	r3, r3, #16
200080fc:	0c1b      	lsrs	r3, r3, #16
200080fe:	8003      	strh	r3, [r0, #0]
20008100:	8803      	ldrh	r3, [r0, #0]
20008102:	4319      	orrs	r1, r3
20008104:	8001      	strh	r1, [r0, #0]
20008106:	4770      	bx	lr

20008108 <TIM_GetCapture1>:
20008108:	8e80      	ldrh	r0, [r0, #52]	; 0x34
2000810a:	4770      	bx	lr

2000810c <TIM_GetCapture2>:
2000810c:	8f00      	ldrh	r0, [r0, #56]	; 0x38
2000810e:	4770      	bx	lr

20008110 <TIM_GetCapture3>:
20008110:	8f80      	ldrh	r0, [r0, #60]	; 0x3c
20008112:	4770      	bx	lr

20008114 <TIM_GetCapture4>:
20008114:	f8b0 0040 	ldrh.w	r0, [r0, #64]	; 0x40
20008118:	4770      	bx	lr

2000811a <TIM_GetCounter>:
2000811a:	8c80      	ldrh	r0, [r0, #36]	; 0x24
2000811c:	4770      	bx	lr

2000811e <TIM_GetPrescaler>:
2000811e:	8d00      	ldrh	r0, [r0, #40]	; 0x28
20008120:	4770      	bx	lr

20008122 <TIM_GetFlagStatus>:
20008122:	8a03      	ldrh	r3, [r0, #16]
20008124:	4219      	tst	r1, r3
20008126:	bf14      	ite	ne
20008128:	2001      	movne	r0, #1
2000812a:	2000      	moveq	r0, #0
2000812c:	4770      	bx	lr

2000812e <TIM_ClearFlag>:
2000812e:	43c9      	mvns	r1, r1
20008130:	b289      	uxth	r1, r1
20008132:	8201      	strh	r1, [r0, #16]
20008134:	4770      	bx	lr

20008136 <TIM_GetITStatus>:
20008136:	8a03      	ldrh	r3, [r0, #16]
20008138:	8982      	ldrh	r2, [r0, #12]
2000813a:	ea11 0003 	ands.w	r0, r1, r3
2000813e:	b292      	uxth	r2, r2
20008140:	d003      	beq.n	2000814a <TIM_GetITStatus+0x14>
20008142:	420a      	tst	r2, r1
20008144:	bf14      	ite	ne
20008146:	2001      	movne	r0, #1
20008148:	2000      	moveq	r0, #0
2000814a:	4770      	bx	lr

2000814c <TIM_ClearITPendingBit>:
2000814c:	43c9      	mvns	r1, r1
2000814e:	b289      	uxth	r1, r1
20008150:	8201      	strh	r1, [r0, #16]
20008152:	4770      	bx	lr

20008154 <GPIO_DeInit>:
20008154:	b508      	push	{r3, lr}
20008156:	4b26      	ldr	r3, [pc, #152]	; (200081f0 <GPIO_DeInit+0x9c>)
20008158:	4298      	cmp	r0, r3
2000815a:	d109      	bne.n	20008170 <GPIO_DeInit+0x1c>
2000815c:	2101      	movs	r1, #1
2000815e:	2004      	movs	r0, #4
20008160:	f7ff f974 	bl	2000744c <RCC_APB2PeriphResetCmd>
20008164:	2100      	movs	r1, #0
20008166:	2004      	movs	r0, #4
20008168:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
2000816c:	f7ff b96e 	b.w	2000744c <RCC_APB2PeriphResetCmd>
20008170:	4b20      	ldr	r3, [pc, #128]	; (200081f4 <GPIO_DeInit+0xa0>)
20008172:	4298      	cmp	r0, r3
20008174:	d106      	bne.n	20008184 <GPIO_DeInit+0x30>
20008176:	2101      	movs	r1, #1
20008178:	2008      	movs	r0, #8
2000817a:	f7ff f967 	bl	2000744c <RCC_APB2PeriphResetCmd>
2000817e:	2100      	movs	r1, #0
20008180:	2008      	movs	r0, #8
20008182:	e7f1      	b.n	20008168 <GPIO_DeInit+0x14>
20008184:	4b1c      	ldr	r3, [pc, #112]	; (200081f8 <GPIO_DeInit+0xa4>)
20008186:	4298      	cmp	r0, r3
20008188:	d106      	bne.n	20008198 <GPIO_DeInit+0x44>
2000818a:	2101      	movs	r1, #1
2000818c:	2010      	movs	r0, #16
2000818e:	f7ff f95d 	bl	2000744c <RCC_APB2PeriphResetCmd>
20008192:	2100      	movs	r1, #0
20008194:	2010      	movs	r0, #16
20008196:	e7e7      	b.n	20008168 <GPIO_DeInit+0x14>
20008198:	4b18      	ldr	r3, [pc, #96]	; (200081fc <GPIO_DeInit+0xa8>)
2000819a:	4298      	cmp	r0, r3
2000819c:	d106      	bne.n	200081ac <GPIO_DeInit+0x58>
2000819e:	2101      	movs	r1, #1
200081a0:	2020      	movs	r0, #32
200081a2:	f7ff f953 	bl	2000744c <RCC_APB2PeriphResetCmd>
200081a6:	2100      	movs	r1, #0
200081a8:	2020      	movs	r0, #32
200081aa:	e7dd      	b.n	20008168 <GPIO_DeInit+0x14>
200081ac:	4b14      	ldr	r3, [pc, #80]	; (20008200 <GPIO_DeInit+0xac>)
200081ae:	4298      	cmp	r0, r3
200081b0:	d106      	bne.n	200081c0 <GPIO_DeInit+0x6c>
200081b2:	2101      	movs	r1, #1
200081b4:	2040      	movs	r0, #64	; 0x40
200081b6:	f7ff f949 	bl	2000744c <RCC_APB2PeriphResetCmd>
200081ba:	2100      	movs	r1, #0
200081bc:	2040      	movs	r0, #64	; 0x40
200081be:	e7d3      	b.n	20008168 <GPIO_DeInit+0x14>
200081c0:	4b10      	ldr	r3, [pc, #64]	; (20008204 <GPIO_DeInit+0xb0>)
200081c2:	4298      	cmp	r0, r3
200081c4:	d106      	bne.n	200081d4 <GPIO_DeInit+0x80>
200081c6:	2101      	movs	r1, #1
200081c8:	2080      	movs	r0, #128	; 0x80
200081ca:	f7ff f93f 	bl	2000744c <RCC_APB2PeriphResetCmd>
200081ce:	2100      	movs	r1, #0
200081d0:	2080      	movs	r0, #128	; 0x80
200081d2:	e7c9      	b.n	20008168 <GPIO_DeInit+0x14>
200081d4:	4b0c      	ldr	r3, [pc, #48]	; (20008208 <GPIO_DeInit+0xb4>)
200081d6:	4298      	cmp	r0, r3
200081d8:	d108      	bne.n	200081ec <GPIO_DeInit+0x98>
200081da:	2101      	movs	r1, #1
200081dc:	f44f 7080 	mov.w	r0, #256	; 0x100
200081e0:	f7ff f934 	bl	2000744c <RCC_APB2PeriphResetCmd>
200081e4:	2100      	movs	r1, #0
200081e6:	f44f 7080 	mov.w	r0, #256	; 0x100
200081ea:	e7bd      	b.n	20008168 <GPIO_DeInit+0x14>
200081ec:	bd08      	pop	{r3, pc}
200081ee:	bf00      	nop
200081f0:	40010800 	.word	0x40010800
200081f4:	40010c00 	.word	0x40010c00
200081f8:	40011000 	.word	0x40011000
200081fc:	40011400 	.word	0x40011400
20008200:	40011800 	.word	0x40011800
20008204:	40011c00 	.word	0x40011c00
20008208:	40012000 	.word	0x40012000

2000820c <GPIO_AFIODeInit>:
2000820c:	2101      	movs	r1, #1
2000820e:	b508      	push	{r3, lr}
20008210:	4608      	mov	r0, r1
20008212:	f7ff f91b 	bl	2000744c <RCC_APB2PeriphResetCmd>
20008216:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
2000821a:	2100      	movs	r1, #0
2000821c:	2001      	movs	r0, #1
2000821e:	f7ff b915 	b.w	2000744c <RCC_APB2PeriphResetCmd>

20008222 <GPIO_Init>:
20008222:	78ca      	ldrb	r2, [r1, #3]
20008224:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
20008228:	06d3      	lsls	r3, r2, #27
2000822a:	bf48      	it	mi
2000822c:	788b      	ldrbmi	r3, [r1, #2]
2000822e:	880e      	ldrh	r6, [r1, #0]
20008230:	f002 050f 	and.w	r5, r2, #15
20008234:	bf48      	it	mi
20008236:	431d      	orrmi	r5, r3
20008238:	f016 0fff 	tst.w	r6, #255	; 0xff
2000823c:	d01a      	beq.n	20008274 <GPIO_Init+0x52>
2000823e:	2100      	movs	r1, #0
20008240:	f04f 0c01 	mov.w	ip, #1
20008244:	f04f 0e0f 	mov.w	lr, #15
20008248:	6803      	ldr	r3, [r0, #0]
2000824a:	fa0c f701 	lsl.w	r7, ip, r1
2000824e:	ea37 0406 	bics.w	r4, r7, r6
20008252:	d10b      	bne.n	2000826c <GPIO_Init+0x4a>
20008254:	008c      	lsls	r4, r1, #2
20008256:	fa0e f804 	lsl.w	r8, lr, r4
2000825a:	ea23 0308 	bic.w	r3, r3, r8
2000825e:	fa05 f404 	lsl.w	r4, r5, r4
20008262:	2a28      	cmp	r2, #40	; 0x28
20008264:	ea43 0304 	orr.w	r3, r3, r4
20008268:	d126      	bne.n	200082b8 <GPIO_Init+0x96>
2000826a:	6147      	str	r7, [r0, #20]
2000826c:	3101      	adds	r1, #1
2000826e:	2908      	cmp	r1, #8
20008270:	d1eb      	bne.n	2000824a <GPIO_Init+0x28>
20008272:	6003      	str	r3, [r0, #0]
20008274:	2eff      	cmp	r6, #255	; 0xff
20008276:	d91d      	bls.n	200082b4 <GPIO_Init+0x92>
20008278:	2408      	movs	r4, #8
2000827a:	f04f 0c01 	mov.w	ip, #1
2000827e:	f04f 0e0f 	mov.w	lr, #15
20008282:	6841      	ldr	r1, [r0, #4]
20008284:	fa0c f704 	lsl.w	r7, ip, r4
20008288:	ea37 0306 	bics.w	r3, r7, r6
2000828c:	d10e      	bne.n	200082ac <GPIO_Init+0x8a>
2000828e:	f104 4380 	add.w	r3, r4, #1073741824	; 0x40000000
20008292:	3b08      	subs	r3, #8
20008294:	009b      	lsls	r3, r3, #2
20008296:	fa0e f803 	lsl.w	r8, lr, r3
2000829a:	ea21 0108 	bic.w	r1, r1, r8
2000829e:	fa05 f303 	lsl.w	r3, r5, r3
200082a2:	2a28      	cmp	r2, #40	; 0x28
200082a4:	ea41 0103 	orr.w	r1, r1, r3
200082a8:	d10a      	bne.n	200082c0 <GPIO_Init+0x9e>
200082aa:	6147      	str	r7, [r0, #20]
200082ac:	3401      	adds	r4, #1
200082ae:	2c10      	cmp	r4, #16
200082b0:	d1e8      	bne.n	20008284 <GPIO_Init+0x62>
200082b2:	6041      	str	r1, [r0, #4]
200082b4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
200082b8:	2a48      	cmp	r2, #72	; 0x48
200082ba:	bf08      	it	eq
200082bc:	6107      	streq	r7, [r0, #16]
200082be:	e7d5      	b.n	2000826c <GPIO_Init+0x4a>
200082c0:	2a48      	cmp	r2, #72	; 0x48
200082c2:	bf08      	it	eq
200082c4:	6107      	streq	r7, [r0, #16]
200082c6:	e7f1      	b.n	200082ac <GPIO_Init+0x8a>

200082c8 <GPIO_StructInit>:
200082c8:	f64f 73ff 	movw	r3, #65535	; 0xffff
200082cc:	8003      	strh	r3, [r0, #0]
200082ce:	2302      	movs	r3, #2
200082d0:	7083      	strb	r3, [r0, #2]
200082d2:	2304      	movs	r3, #4
200082d4:	70c3      	strb	r3, [r0, #3]
200082d6:	4770      	bx	lr

200082d8 <GPIO_ReadInputDataBit>:
200082d8:	6883      	ldr	r3, [r0, #8]
200082da:	4219      	tst	r1, r3
200082dc:	bf14      	ite	ne
200082de:	2001      	movne	r0, #1
200082e0:	2000      	moveq	r0, #0
200082e2:	4770      	bx	lr

200082e4 <GPIO_ReadInputData>:
200082e4:	6880      	ldr	r0, [r0, #8]
200082e6:	b280      	uxth	r0, r0
200082e8:	4770      	bx	lr

200082ea <GPIO_ReadOutputDataBit>:
200082ea:	68c3      	ldr	r3, [r0, #12]
200082ec:	4219      	tst	r1, r3
200082ee:	bf14      	ite	ne
200082f0:	2001      	movne	r0, #1
200082f2:	2000      	moveq	r0, #0
200082f4:	4770      	bx	lr

200082f6 <GPIO_ReadOutputData>:
200082f6:	68c0      	ldr	r0, [r0, #12]
200082f8:	b280      	uxth	r0, r0
200082fa:	4770      	bx	lr

200082fc <GPIO_SetBits>:
200082fc:	6101      	str	r1, [r0, #16]
200082fe:	4770      	bx	lr

20008300 <GPIO_ResetBits>:
20008300:	6141      	str	r1, [r0, #20]
20008302:	4770      	bx	lr

20008304 <GPIO_WriteBit>:
20008304:	b10a      	cbz	r2, 2000830a <GPIO_WriteBit+0x6>
20008306:	6101      	str	r1, [r0, #16]
20008308:	4770      	bx	lr
2000830a:	6141      	str	r1, [r0, #20]
2000830c:	4770      	bx	lr

2000830e <GPIO_Write>:
2000830e:	60c1      	str	r1, [r0, #12]
20008310:	4770      	bx	lr

20008312 <GPIO_PinLockConfig>:
20008312:	f441 3380 	orr.w	r3, r1, #65536	; 0x10000
20008316:	6183      	str	r3, [r0, #24]
20008318:	6181      	str	r1, [r0, #24]
2000831a:	6183      	str	r3, [r0, #24]
2000831c:	6983      	ldr	r3, [r0, #24]
2000831e:	6983      	ldr	r3, [r0, #24]
20008320:	4770      	bx	lr

20008322 <GPIO_EventOutputConfig>:
20008322:	4a05      	ldr	r2, [pc, #20]	; (20008338 <GPIO_EventOutputConfig+0x16>)
20008324:	6813      	ldr	r3, [r2, #0]
20008326:	f023 037f 	bic.w	r3, r3, #127	; 0x7f
2000832a:	041b      	lsls	r3, r3, #16
2000832c:	0c1b      	lsrs	r3, r3, #16
2000832e:	4319      	orrs	r1, r3
20008330:	ea41 1100 	orr.w	r1, r1, r0, lsl #4
20008334:	6011      	str	r1, [r2, #0]
20008336:	4770      	bx	lr
20008338:	40010000 	.word	0x40010000

2000833c <GPIO_EventOutputCmd>:
2000833c:	4b01      	ldr	r3, [pc, #4]	; (20008344 <GPIO_EventOutputCmd+0x8>)
2000833e:	6018      	str	r0, [r3, #0]
20008340:	4770      	bx	lr
20008342:	bf00      	nop
20008344:	4220001c 	.word	0x4220001c

20008348 <GPIO_PinRemapConfig>:
20008348:	2800      	cmp	r0, #0
2000834a:	4a16      	ldr	r2, [pc, #88]	; (200083a4 <GPIO_PinRemapConfig+0x5c>)
2000834c:	b530      	push	{r4, r5, lr}
2000834e:	f400 1540 	and.w	r5, r0, #3145728	; 0x300000
20008352:	bfb4      	ite	lt
20008354:	69d3      	ldrlt	r3, [r2, #28]
20008356:	6853      	ldrge	r3, [r2, #4]
20008358:	f5b5 1f40 	cmp.w	r5, #3145728	; 0x300000
2000835c:	b284      	uxth	r4, r0
2000835e:	d110      	bne.n	20008382 <GPIO_PinRemapConfig+0x3a>
20008360:	6855      	ldr	r5, [r2, #4]
20008362:	f023 6370 	bic.w	r3, r3, #251658240	; 0xf000000
20008366:	f025 6570 	bic.w	r5, r5, #251658240	; 0xf000000
2000836a:	6055      	str	r5, [r2, #4]
2000836c:	b119      	cbz	r1, 20008376 <GPIO_PinRemapConfig+0x2e>
2000836e:	0d42      	lsrs	r2, r0, #21
20008370:	0112      	lsls	r2, r2, #4
20008372:	4094      	lsls	r4, r2
20008374:	4323      	orrs	r3, r4
20008376:	4a0b      	ldr	r2, [pc, #44]	; (200083a4 <GPIO_PinRemapConfig+0x5c>)
20008378:	2800      	cmp	r0, #0
2000837a:	bfb4      	ite	lt
2000837c:	61d3      	strlt	r3, [r2, #28]
2000837e:	6053      	strge	r3, [r2, #4]
20008380:	bd30      	pop	{r4, r5, pc}
20008382:	02c2      	lsls	r2, r0, #11
20008384:	bf4d      	iteet	mi
20008386:	2203      	movmi	r2, #3
20008388:	0d42      	lsrpl	r2, r0, #21
2000838a:	0112      	lslpl	r2, r2, #4
2000838c:	f3c0 4503 	ubfxmi	r5, r0, #16, #4
20008390:	bf4c      	ite	mi
20008392:	40aa      	lslmi	r2, r5
20008394:	fa04 f202 	lslpl.w	r2, r4, r2
20008398:	ea23 0302 	bic.w	r3, r3, r2
2000839c:	f043 6370 	orr.w	r3, r3, #251658240	; 0xf000000
200083a0:	e7e4      	b.n	2000836c <GPIO_PinRemapConfig+0x24>
200083a2:	bf00      	nop
200083a4:	40010000 	.word	0x40010000

200083a8 <GPIO_EXTILineConfig>:
200083a8:	b510      	push	{r4, lr}
200083aa:	240f      	movs	r4, #15
200083ac:	f001 0303 	and.w	r3, r1, #3
200083b0:	f001 01fc 	and.w	r1, r1, #252	; 0xfc
200083b4:	f101 4180 	add.w	r1, r1, #1073741824	; 0x40000000
200083b8:	f501 3180 	add.w	r1, r1, #65536	; 0x10000
200083bc:	688a      	ldr	r2, [r1, #8]
200083be:	009b      	lsls	r3, r3, #2
200083c0:	409c      	lsls	r4, r3
200083c2:	ea22 0204 	bic.w	r2, r2, r4
200083c6:	608a      	str	r2, [r1, #8]
200083c8:	688a      	ldr	r2, [r1, #8]
200083ca:	4098      	lsls	r0, r3
200083cc:	4310      	orrs	r0, r2
200083ce:	6088      	str	r0, [r1, #8]
200083d0:	bd10      	pop	{r4, pc}

200083d2 <GPIO_ETH_MediaInterfaceConfig>:
200083d2:	4b01      	ldr	r3, [pc, #4]	; (200083d8 <GPIO_ETH_MediaInterfaceConfig+0x6>)
200083d4:	6018      	str	r0, [r3, #0]
200083d6:	4770      	bx	lr
200083d8:	422000dc 	.word	0x422000dc
