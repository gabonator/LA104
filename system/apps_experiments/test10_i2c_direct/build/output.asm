
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08018000 <main>:
 8018000:	b5f0      	push	{r4, r5, r6, r7, lr}
 8018002:	f242 1004 	movw	r0, #8452	; 0x2104
 8018006:	b089      	sub	sp, #36	; 0x24
 8018008:	f003 fa68 	bl	801b4dc <_ZN4BIOS3LCD5ClearEt@plt>
 801800c:	4816      	ldr	r0, [pc, #88]	; (8018068 <main+0x68>)
 801800e:	f003 fa55 	bl	801b4bc <_ZN4BIOS3DBG5PrintEPKcz@plt>
 8018012:	f000 f843 	bl	801809c <_Z8i2c_initv>
 8018016:	4c15      	ldr	r4, [pc, #84]	; (801806c <main+0x6c>)
 8018018:	4f15      	ldr	r7, [pc, #84]	; (8018070 <main+0x70>)
 801801a:	f003 fa57 	bl	801b4cc <_ZN4BIOS3KEY6GetKeyEv@plt>
 801801e:	2806      	cmp	r0, #6
 8018020:	d01e      	beq.n	8018060 <main+0x60>
 8018022:	2103      	movs	r1, #3
 8018024:	a802      	add	r0, sp, #8
 8018026:	f000 f983 	bl	8018330 <_ZN15Adafruit_BMP0855beginEh>
 801802a:	b918      	cbnz	r0, 8018034 <main+0x34>
 801802c:	4638      	mov	r0, r7
 801802e:	f003 fa45 	bl	801b4bc <_ZN4BIOS3DBG5PrintEPKcz@plt>
 8018032:	e7f2      	b.n	801801a <main+0x1a>
 8018034:	a802      	add	r0, sp, #8
 8018036:	f000 f94a 	bl	80182ce <_ZN15Adafruit_BMP08515readTemperatureEv>
 801803a:	4606      	mov	r6, r0
 801803c:	a802      	add	r0, sp, #8
 801803e:	f000 f9e4 	bl	801840a <_ZN15Adafruit_BMP08512readPressureEv>
 8018042:	4605      	mov	r5, r0
 8018044:	4630      	mov	r0, r6
 8018046:	f002 f9ad 	bl	801a3a4 <__aeabi_f2d>
 801804a:	9500      	str	r5, [sp, #0]
 801804c:	4602      	mov	r2, r0
 801804e:	460b      	mov	r3, r1
 8018050:	4620      	mov	r0, r4
 8018052:	f003 fa33 	bl	801b4bc <_ZN4BIOS3DBG5PrintEPKcz@plt>
 8018056:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
 801805a:	f003 fa49 	bl	801b4f0 <_ZN4BIOS3SYS7DelayMsEi@plt+0x4>
 801805e:	e7dc      	b.n	801801a <main+0x1a>
 8018060:	2000      	movs	r0, #0
 8018062:	b009      	add	sp, #36	; 0x24
 8018064:	bdf0      	pop	{r4, r5, r6, r7, pc}
 8018066:	bf00      	nop
 8018068:	0801af6c 	.word	0x0801af6c
 801806c:	0801af81 	.word	0x0801af81
 8018070:	0801af76 	.word	0x0801af76
 8018074:	00000000 	.word	0x00000000

08018078 <_Z5delayi>:
 8018078:	f003 ba3a 	b.w	801b4f0 <_ZN4BIOS3SYS7DelayMsEi@plt+0x4>

0801807c <_ZN15Adafruit_BMP0859computeB5El>:
 801807c:	8ac3      	ldrh	r3, [r0, #22]
 801807e:	1acb      	subs	r3, r1, r3
 8018080:	8a81      	ldrh	r1, [r0, #20]
 8018082:	4359      	muls	r1, r3
 8018084:	f9b0 300e 	ldrsh.w	r3, [r0, #14]
 8018088:	f9b0 0010 	ldrsh.w	r0, [r0, #16]
 801808c:	13c9      	asrs	r1, r1, #15
 801808e:	02db      	lsls	r3, r3, #11
 8018090:	4408      	add	r0, r1
 8018092:	fb93 f0f0 	sdiv	r0, r3, r0
 8018096:	4408      	add	r0, r1
 8018098:	4770      	bx	lr

0801809a <_Z13assert_failedPhm>:
 801809a:	e7fe      	b.n	801809a <_Z13assert_failedPhm>

0801809c <_Z8i2c_initv>:
 801809c:	b510      	push	{r4, lr}
 801809e:	2101      	movs	r1, #1
 80180a0:	b086      	sub	sp, #24
 80180a2:	f44f 0080 	mov.w	r0, #4194304	; 0x400000
 80180a6:	f000 feb5 	bl	8018e14 <RCC_APB1PeriphClockCmd>
 80180aa:	4a13      	ldr	r2, [pc, #76]	; (80180f8 <_Z8i2c_initv+0x5c>)
 80180ac:	4b13      	ldr	r3, [pc, #76]	; (80180fc <_Z8i2c_initv+0x60>)
 80180ae:	4c14      	ldr	r4, [pc, #80]	; (8018100 <_Z8i2c_initv+0x64>)
 80180b0:	e9cd 2302 	strd	r2, r3, [sp, #8]
 80180b4:	2300      	movs	r3, #0
 80180b6:	9304      	str	r3, [sp, #16]
 80180b8:	f44f 4380 	mov.w	r3, #16384	; 0x4000
 80180bc:	a902      	add	r1, sp, #8
 80180be:	4620      	mov	r0, r4
 80180c0:	f8ad 3014 	strh.w	r3, [sp, #20]
 80180c4:	f000 faaa 	bl	801861c <I2C_Init>
 80180c8:	4620      	mov	r0, r4
 80180ca:	2101      	movs	r1, #1
 80180cc:	f000 fb18 	bl	8018700 <I2C_Cmd>
 80180d0:	2101      	movs	r1, #1
 80180d2:	2008      	movs	r0, #8
 80180d4:	f000 fe92 	bl	8018dfc <RCC_APB2PeriphClockCmd>
 80180d8:	f44f 6340 	mov.w	r3, #3072	; 0xc00
 80180dc:	f8ad 3004 	strh.w	r3, [sp, #4]
 80180e0:	231c      	movs	r3, #28
 80180e2:	f88d 3007 	strb.w	r3, [sp, #7]
 80180e6:	2302      	movs	r3, #2
 80180e8:	a901      	add	r1, sp, #4
 80180ea:	4806      	ldr	r0, [pc, #24]	; (8018104 <_Z8i2c_initv+0x68>)
 80180ec:	f88d 3006 	strb.w	r3, [sp, #6]
 80180f0:	f000 fc8f 	bl	8018a12 <GPIO_Init>
 80180f4:	b006      	add	sp, #24
 80180f6:	bd10      	pop	{r4, pc}
 80180f8:	000186a0 	.word	0x000186a0
 80180fc:	bfff0000 	.word	0xbfff0000
 8018100:	40005800 	.word	0x40005800
 8018104:	40010c00 	.word	0x40010c00

08018108 <_Z9i2c_startv>:
 8018108:	b538      	push	{r3, r4, r5, lr}
 801810a:	f44f 3100 	mov.w	r1, #131072	; 0x20000
 801810e:	4809      	ldr	r0, [pc, #36]	; (8018134 <_Z9i2c_startv+0x2c>)
 8018110:	f000 fbe8 	bl	80188e4 <I2C_GetFlagStatus>
 8018114:	2800      	cmp	r0, #0
 8018116:	d1f8      	bne.n	801810a <_Z9i2c_startv+0x2>
 8018118:	2101      	movs	r1, #1
 801811a:	4806      	ldr	r0, [pc, #24]	; (8018134 <_Z9i2c_startv+0x2c>)
 801811c:	f000 fb11 	bl	8018742 <I2C_GenerateSTART>
 8018120:	4d05      	ldr	r5, [pc, #20]	; (8018138 <_Z9i2c_startv+0x30>)
 8018122:	4c04      	ldr	r4, [pc, #16]	; (8018134 <_Z9i2c_startv+0x2c>)
 8018124:	4629      	mov	r1, r5
 8018126:	4620      	mov	r0, r4
 8018128:	f000 fbca 	bl	80188c0 <I2C_CheckEvent>
 801812c:	2800      	cmp	r0, #0
 801812e:	d0f9      	beq.n	8018124 <_Z9i2c_startv+0x1c>
 8018130:	bd38      	pop	{r3, r4, r5, pc}
 8018132:	bf00      	nop
 8018134:	40005800 	.word	0x40005800
 8018138:	00030001 	.word	0x00030001

0801813c <_Z8i2c_stopv>:
 801813c:	b538      	push	{r3, r4, r5, lr}
 801813e:	2101      	movs	r1, #1
 8018140:	4805      	ldr	r0, [pc, #20]	; (8018158 <_Z8i2c_stopv+0x1c>)
 8018142:	f000 fb09 	bl	8018758 <I2C_GenerateSTOP>
 8018146:	4d05      	ldr	r5, [pc, #20]	; (801815c <_Z8i2c_stopv+0x20>)
 8018148:	4c03      	ldr	r4, [pc, #12]	; (8018158 <_Z8i2c_stopv+0x1c>)
 801814a:	4629      	mov	r1, r5
 801814c:	4620      	mov	r0, r4
 801814e:	f000 fbc9 	bl	80188e4 <I2C_GetFlagStatus>
 8018152:	2800      	cmp	r0, #0
 8018154:	d1f9      	bne.n	801814a <_Z8i2c_stopv+0xe>
 8018156:	bd38      	pop	{r3, r4, r5, pc}
 8018158:	40005800 	.word	0x40005800
 801815c:	10000010 	.word	0x10000010

08018160 <_Z21i2c_address_directionhh>:
 8018160:	b538      	push	{r3, r4, r5, lr}
 8018162:	460c      	mov	r4, r1
 8018164:	460a      	mov	r2, r1
 8018166:	4601      	mov	r1, r0
 8018168:	480b      	ldr	r0, [pc, #44]	; (8018198 <_Z21i2c_address_directionhh+0x38>)
 801816a:	f000 fb37 	bl	80187dc <I2C_Send7bitAddress>
 801816e:	b944      	cbnz	r4, 8018182 <_Z21i2c_address_directionhh+0x22>
 8018170:	4d0a      	ldr	r5, [pc, #40]	; (801819c <_Z21i2c_address_directionhh+0x3c>)
 8018172:	4c09      	ldr	r4, [pc, #36]	; (8018198 <_Z21i2c_address_directionhh+0x38>)
 8018174:	4629      	mov	r1, r5
 8018176:	4620      	mov	r0, r4
 8018178:	f000 fba2 	bl	80188c0 <I2C_CheckEvent>
 801817c:	2800      	cmp	r0, #0
 801817e:	d0f9      	beq.n	8018174 <_Z21i2c_address_directionhh+0x14>
 8018180:	bd38      	pop	{r3, r4, r5, pc}
 8018182:	2c01      	cmp	r4, #1
 8018184:	d1fc      	bne.n	8018180 <_Z21i2c_address_directionhh+0x20>
 8018186:	4d06      	ldr	r5, [pc, #24]	; (80181a0 <_Z21i2c_address_directionhh+0x40>)
 8018188:	4c03      	ldr	r4, [pc, #12]	; (8018198 <_Z21i2c_address_directionhh+0x38>)
 801818a:	4629      	mov	r1, r5
 801818c:	4620      	mov	r0, r4
 801818e:	f000 fb97 	bl	80188c0 <I2C_CheckEvent>
 8018192:	2800      	cmp	r0, #0
 8018194:	d0f9      	beq.n	801818a <_Z21i2c_address_directionhh+0x2a>
 8018196:	e7f3      	b.n	8018180 <_Z21i2c_address_directionhh+0x20>
 8018198:	40005800 	.word	0x40005800
 801819c:	00070082 	.word	0x00070082
 80181a0:	00030002 	.word	0x00030002

080181a4 <_Z12i2c_transmith>:
 80181a4:	b538      	push	{r3, r4, r5, lr}
 80181a6:	4601      	mov	r1, r0
 80181a8:	4805      	ldr	r0, [pc, #20]	; (80181c0 <_Z12i2c_transmith+0x1c>)
 80181aa:	f000 fb12 	bl	80187d2 <I2C_SendData>
 80181ae:	4d05      	ldr	r5, [pc, #20]	; (80181c4 <_Z12i2c_transmith+0x20>)
 80181b0:	4c03      	ldr	r4, [pc, #12]	; (80181c0 <_Z12i2c_transmith+0x1c>)
 80181b2:	4629      	mov	r1, r5
 80181b4:	4620      	mov	r0, r4
 80181b6:	f000 fb83 	bl	80188c0 <I2C_CheckEvent>
 80181ba:	2800      	cmp	r0, #0
 80181bc:	d0f9      	beq.n	80181b2 <_Z12i2c_transmith+0xe>
 80181be:	bd38      	pop	{r3, r4, r5, pc}
 80181c0:	40005800 	.word	0x40005800
 80181c4:	00070084 	.word	0x00070084

080181c8 <_ZN15Adafruit_BMP0856write8Ehh>:
 80181c8:	b538      	push	{r3, r4, r5, lr}
 80181ca:	2377      	movs	r3, #119	; 0x77
 80181cc:	4c07      	ldr	r4, [pc, #28]	; (80181ec <_ZN15Adafruit_BMP0856write8Ehh+0x24>)
 80181ce:	4615      	mov	r5, r2
 80181d0:	7123      	strb	r3, [r4, #4]
 80181d2:	2300      	movs	r3, #0
 80181d4:	4620      	mov	r0, r4
 80181d6:	7163      	strb	r3, [r4, #5]
 80181d8:	f000 f9e8 	bl	80185ac <_ZN5CWire5writeEh>
 80181dc:	4629      	mov	r1, r5
 80181de:	4620      	mov	r0, r4
 80181e0:	f000 f9e4 	bl	80185ac <_ZN5CWire5writeEh>
 80181e4:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80181e8:	f7ff bfa8 	b.w	801813c <_Z8i2c_stopv>
 80181ec:	200054e0 	.word	0x200054e0

080181f0 <_Z15i2c_receive_ackv>:
 80181f0:	b510      	push	{r4, lr}
 80181f2:	2101      	movs	r1, #1
 80181f4:	4807      	ldr	r0, [pc, #28]	; (8018214 <_Z15i2c_receive_ackv+0x24>)
 80181f6:	f000 faba 	bl	801876e <I2C_AcknowledgeConfig>
 80181fa:	4c07      	ldr	r4, [pc, #28]	; (8018218 <_Z15i2c_receive_ackv+0x28>)
 80181fc:	4621      	mov	r1, r4
 80181fe:	4805      	ldr	r0, [pc, #20]	; (8018214 <_Z15i2c_receive_ackv+0x24>)
 8018200:	f000 fb5e 	bl	80188c0 <I2C_CheckEvent>
 8018204:	2800      	cmp	r0, #0
 8018206:	d0f9      	beq.n	80181fc <_Z15i2c_receive_ackv+0xc>
 8018208:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 801820c:	4801      	ldr	r0, [pc, #4]	; (8018214 <_Z15i2c_receive_ackv+0x24>)
 801820e:	f000 bae2 	b.w	80187d6 <I2C_ReceiveData>
 8018212:	bf00      	nop
 8018214:	40005800 	.word	0x40005800
 8018218:	00030040 	.word	0x00030040

0801821c <_Z16i2c_receive_nackv>:
 801821c:	b510      	push	{r4, lr}
 801821e:	2100      	movs	r1, #0
 8018220:	4807      	ldr	r0, [pc, #28]	; (8018240 <_Z16i2c_receive_nackv+0x24>)
 8018222:	f000 faa4 	bl	801876e <I2C_AcknowledgeConfig>
 8018226:	4c07      	ldr	r4, [pc, #28]	; (8018244 <_Z16i2c_receive_nackv+0x28>)
 8018228:	4621      	mov	r1, r4
 801822a:	4805      	ldr	r0, [pc, #20]	; (8018240 <_Z16i2c_receive_nackv+0x24>)
 801822c:	f000 fb48 	bl	80188c0 <I2C_CheckEvent>
 8018230:	2800      	cmp	r0, #0
 8018232:	d0f9      	beq.n	8018228 <_Z16i2c_receive_nackv+0xc>
 8018234:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8018238:	4801      	ldr	r0, [pc, #4]	; (8018240 <_Z16i2c_receive_nackv+0x24>)
 801823a:	f000 bacc 	b.w	80187d6 <I2C_ReceiveData>
 801823e:	bf00      	nop
 8018240:	40005800 	.word	0x40005800
 8018244:	00030040 	.word	0x00030040

08018248 <_ZN5CWire4readEv.isra.4.constprop.7>:
 8018248:	4b06      	ldr	r3, [pc, #24]	; (8018264 <_ZN5CWire4readEv.isra.4.constprop.7+0x1c>)
 801824a:	681a      	ldr	r2, [r3, #0]
 801824c:	2a00      	cmp	r2, #0
 801824e:	bfc4      	itt	gt
 8018250:	f102 32ff 	addgt.w	r2, r2, #4294967295	; 0xffffffff
 8018254:	601a      	strgt	r2, [r3, #0]
 8018256:	681b      	ldr	r3, [r3, #0]
 8018258:	2b00      	cmp	r3, #0
 801825a:	dd01      	ble.n	8018260 <_ZN5CWire4readEv.isra.4.constprop.7+0x18>
 801825c:	f7ff bfc8 	b.w	80181f0 <_Z15i2c_receive_ackv>
 8018260:	f7ff bfdc 	b.w	801821c <_Z16i2c_receive_nackv>
 8018264:	200054e0 	.word	0x200054e0

08018268 <_ZN15Adafruit_BMP0856read16Eh>:
 8018268:	b570      	push	{r4, r5, r6, lr}
 801826a:	2677      	movs	r6, #119	; 0x77
 801826c:	2500      	movs	r5, #0
 801826e:	4c0f      	ldr	r4, [pc, #60]	; (80182ac <_ZN15Adafruit_BMP0856read16Eh+0x44>)
 8018270:	4620      	mov	r0, r4
 8018272:	7126      	strb	r6, [r4, #4]
 8018274:	7165      	strb	r5, [r4, #5]
 8018276:	f000 f999 	bl	80185ac <_ZN5CWire5writeEh>
 801827a:	f7ff ff5f 	bl	801813c <_Z8i2c_stopv>
 801827e:	7126      	strb	r6, [r4, #4]
 8018280:	7165      	strb	r5, [r4, #5]
 8018282:	f7ff ff41 	bl	8018108 <_Z9i2c_startv>
 8018286:	2101      	movs	r1, #1
 8018288:	20ee      	movs	r0, #238	; 0xee
 801828a:	f7ff ff69 	bl	8018160 <_Z21i2c_address_directionhh>
 801828e:	2302      	movs	r3, #2
 8018290:	6023      	str	r3, [r4, #0]
 8018292:	f7ff ffd9 	bl	8018248 <_ZN5CWire4readEv.isra.4.constprop.7>
 8018296:	0200      	lsls	r0, r0, #8
 8018298:	f400 447f 	and.w	r4, r0, #65280	; 0xff00
 801829c:	f7ff ffd4 	bl	8018248 <_ZN5CWire4readEv.isra.4.constprop.7>
 80182a0:	4304      	orrs	r4, r0
 80182a2:	b2a4      	uxth	r4, r4
 80182a4:	f7ff ff4a 	bl	801813c <_Z8i2c_stopv>
 80182a8:	4620      	mov	r0, r4
 80182aa:	bd70      	pop	{r4, r5, r6, pc}
 80182ac:	200054e0 	.word	0x200054e0

080182b0 <_ZN15Adafruit_BMP08518readRawTemperatureEv>:
 80182b0:	b510      	push	{r4, lr}
 80182b2:	4604      	mov	r4, r0
 80182b4:	21f4      	movs	r1, #244	; 0xf4
 80182b6:	222e      	movs	r2, #46	; 0x2e
 80182b8:	f7ff ff86 	bl	80181c8 <_ZN15Adafruit_BMP0856write8Ehh>
 80182bc:	2005      	movs	r0, #5
 80182be:	f003 f917 	bl	801b4f0 <_ZN4BIOS3SYS7DelayMsEi@plt+0x4>
 80182c2:	4620      	mov	r0, r4
 80182c4:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80182c8:	21f6      	movs	r1, #246	; 0xf6
 80182ca:	f7ff bfcd 	b.w	8018268 <_ZN15Adafruit_BMP0856read16Eh>

080182ce <_ZN15Adafruit_BMP08515readTemperatureEv>:
 80182ce:	b510      	push	{r4, lr}
 80182d0:	4604      	mov	r4, r0
 80182d2:	f7ff ffed 	bl	80182b0 <_ZN15Adafruit_BMP08518readRawTemperatureEv>
 80182d6:	4601      	mov	r1, r0
 80182d8:	4620      	mov	r0, r4
 80182da:	f7ff fecf 	bl	801807c <_ZN15Adafruit_BMP0859computeB5El>
 80182de:	3008      	adds	r0, #8
 80182e0:	1100      	asrs	r0, r0, #4
 80182e2:	f002 fc99 	bl	801ac18 <__aeabi_i2f>
 80182e6:	4902      	ldr	r1, [pc, #8]	; (80182f0 <_ZN15Adafruit_BMP08515readTemperatureEv+0x22>)
 80182e8:	f002 fd9e 	bl	801ae28 <__aeabi_fdiv>
 80182ec:	bd10      	pop	{r4, pc}
 80182ee:	bf00      	nop
 80182f0:	41200000 	.word	0x41200000

080182f4 <_ZN15Adafruit_BMP0855read8Eh>:
 80182f4:	b570      	push	{r4, r5, r6, lr}
 80182f6:	2677      	movs	r6, #119	; 0x77
 80182f8:	2500      	movs	r5, #0
 80182fa:	4c0c      	ldr	r4, [pc, #48]	; (801832c <_ZN15Adafruit_BMP0855read8Eh+0x38>)
 80182fc:	4620      	mov	r0, r4
 80182fe:	7126      	strb	r6, [r4, #4]
 8018300:	7165      	strb	r5, [r4, #5]
 8018302:	f000 f953 	bl	80185ac <_ZN5CWire5writeEh>
 8018306:	f7ff ff19 	bl	801813c <_Z8i2c_stopv>
 801830a:	7126      	strb	r6, [r4, #4]
 801830c:	7165      	strb	r5, [r4, #5]
 801830e:	f7ff fefb 	bl	8018108 <_Z9i2c_startv>
 8018312:	2101      	movs	r1, #1
 8018314:	20ee      	movs	r0, #238	; 0xee
 8018316:	f7ff ff23 	bl	8018160 <_Z21i2c_address_directionhh>
 801831a:	2301      	movs	r3, #1
 801831c:	6023      	str	r3, [r4, #0]
 801831e:	f7ff ff93 	bl	8018248 <_ZN5CWire4readEv.isra.4.constprop.7>
 8018322:	4604      	mov	r4, r0
 8018324:	f7ff ff0a 	bl	801813c <_Z8i2c_stopv>
 8018328:	4620      	mov	r0, r4
 801832a:	bd70      	pop	{r4, r5, r6, pc}
 801832c:	200054e0 	.word	0x200054e0

08018330 <_ZN15Adafruit_BMP0855beginEh>:
 8018330:	2903      	cmp	r1, #3
 8018332:	bf28      	it	cs
 8018334:	2103      	movcs	r1, #3
 8018336:	b510      	push	{r4, lr}
 8018338:	7001      	strb	r1, [r0, #0]
 801833a:	21d0      	movs	r1, #208	; 0xd0
 801833c:	4604      	mov	r4, r0
 801833e:	f7ff ffd9 	bl	80182f4 <_ZN15Adafruit_BMP0855read8Eh>
 8018342:	2855      	cmp	r0, #85	; 0x55
 8018344:	d138      	bne.n	80183b8 <_ZN15Adafruit_BMP0855beginEh+0x88>
 8018346:	21aa      	movs	r1, #170	; 0xaa
 8018348:	4620      	mov	r0, r4
 801834a:	f7ff ff8d 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 801834e:	21ac      	movs	r1, #172	; 0xac
 8018350:	8060      	strh	r0, [r4, #2]
 8018352:	4620      	mov	r0, r4
 8018354:	f7ff ff88 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 8018358:	21ae      	movs	r1, #174	; 0xae
 801835a:	80a0      	strh	r0, [r4, #4]
 801835c:	4620      	mov	r0, r4
 801835e:	f7ff ff83 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 8018362:	21b0      	movs	r1, #176	; 0xb0
 8018364:	80e0      	strh	r0, [r4, #6]
 8018366:	4620      	mov	r0, r4
 8018368:	f7ff ff7e 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 801836c:	21b2      	movs	r1, #178	; 0xb2
 801836e:	8260      	strh	r0, [r4, #18]
 8018370:	4620      	mov	r0, r4
 8018372:	f7ff ff79 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 8018376:	21b4      	movs	r1, #180	; 0xb4
 8018378:	82a0      	strh	r0, [r4, #20]
 801837a:	4620      	mov	r0, r4
 801837c:	f7ff ff74 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 8018380:	21b6      	movs	r1, #182	; 0xb6
 8018382:	82e0      	strh	r0, [r4, #22]
 8018384:	4620      	mov	r0, r4
 8018386:	f7ff ff6f 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 801838a:	21b8      	movs	r1, #184	; 0xb8
 801838c:	8120      	strh	r0, [r4, #8]
 801838e:	4620      	mov	r0, r4
 8018390:	f7ff ff6a 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 8018394:	21ba      	movs	r1, #186	; 0xba
 8018396:	8160      	strh	r0, [r4, #10]
 8018398:	4620      	mov	r0, r4
 801839a:	f7ff ff65 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 801839e:	21bc      	movs	r1, #188	; 0xbc
 80183a0:	81a0      	strh	r0, [r4, #12]
 80183a2:	4620      	mov	r0, r4
 80183a4:	f7ff ff60 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 80183a8:	21be      	movs	r1, #190	; 0xbe
 80183aa:	81e0      	strh	r0, [r4, #14]
 80183ac:	4620      	mov	r0, r4
 80183ae:	f7ff ff5b 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 80183b2:	8220      	strh	r0, [r4, #16]
 80183b4:	2001      	movs	r0, #1
 80183b6:	bd10      	pop	{r4, pc}
 80183b8:	2000      	movs	r0, #0
 80183ba:	e7fc      	b.n	80183b6 <_ZN15Adafruit_BMP0855beginEh+0x86>

080183bc <_ZN15Adafruit_BMP08515readRawPressureEv>:
 80183bc:	b538      	push	{r3, r4, r5, lr}
 80183be:	4604      	mov	r4, r0
 80183c0:	7802      	ldrb	r2, [r0, #0]
 80183c2:	21f4      	movs	r1, #244	; 0xf4
 80183c4:	0192      	lsls	r2, r2, #6
 80183c6:	3234      	adds	r2, #52	; 0x34
 80183c8:	f002 02fc 	and.w	r2, r2, #252	; 0xfc
 80183cc:	f7ff fefc 	bl	80181c8 <_ZN15Adafruit_BMP0856write8Ehh>
 80183d0:	7823      	ldrb	r3, [r4, #0]
 80183d2:	b98b      	cbnz	r3, 80183f8 <_ZN15Adafruit_BMP08515readRawPressureEv+0x3c>
 80183d4:	2005      	movs	r0, #5
 80183d6:	f003 f88b 	bl	801b4f0 <_ZN4BIOS3SYS7DelayMsEi@plt+0x4>
 80183da:	21f6      	movs	r1, #246	; 0xf6
 80183dc:	4620      	mov	r0, r4
 80183de:	f7ff ff43 	bl	8018268 <_ZN15Adafruit_BMP0856read16Eh>
 80183e2:	21f8      	movs	r1, #248	; 0xf8
 80183e4:	0205      	lsls	r5, r0, #8
 80183e6:	4620      	mov	r0, r4
 80183e8:	f7ff ff84 	bl	80182f4 <_ZN15Adafruit_BMP0855read8Eh>
 80183ec:	7823      	ldrb	r3, [r4, #0]
 80183ee:	4328      	orrs	r0, r5
 80183f0:	f1c3 0308 	rsb	r3, r3, #8
 80183f4:	40d8      	lsrs	r0, r3
 80183f6:	bd38      	pop	{r3, r4, r5, pc}
 80183f8:	2b01      	cmp	r3, #1
 80183fa:	d101      	bne.n	8018400 <_ZN15Adafruit_BMP08515readRawPressureEv+0x44>
 80183fc:	2008      	movs	r0, #8
 80183fe:	e7ea      	b.n	80183d6 <_ZN15Adafruit_BMP08515readRawPressureEv+0x1a>
 8018400:	2b02      	cmp	r3, #2
 8018402:	bf0c      	ite	eq
 8018404:	200e      	moveq	r0, #14
 8018406:	201a      	movne	r0, #26
 8018408:	e7e5      	b.n	80183d6 <_ZN15Adafruit_BMP08515readRawPressureEv+0x1a>

0801840a <_ZN15Adafruit_BMP08512readPressureEv>:
 801840a:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 801840c:	4606      	mov	r6, r0
 801840e:	f7ff ff4f 	bl	80182b0 <_ZN15Adafruit_BMP08518readRawTemperatureEv>
 8018412:	4605      	mov	r5, r0
 8018414:	4630      	mov	r0, r6
 8018416:	f7ff ffd1 	bl	80183bc <_ZN15Adafruit_BMP08515readRawPressureEv>
 801841a:	4629      	mov	r1, r5
 801841c:	4604      	mov	r4, r0
 801841e:	4630      	mov	r0, r6
 8018420:	f7ff fe2c 	bl	801807c <_ZN15Adafruit_BMP0859computeB5El>
 8018424:	f5a0 607a 	sub.w	r0, r0, #4000	; 0xfa0
 8018428:	fb00 f700 	mul.w	r7, r0, r0
 801842c:	f9b6 2006 	ldrsh.w	r2, [r6, #6]
 8018430:	133f      	asrs	r7, r7, #12
 8018432:	fb00 f302 	mul.w	r3, r0, r2
 8018436:	f9b6 2008 	ldrsh.w	r2, [r6, #8]
 801843a:	7835      	ldrb	r5, [r6, #0]
 801843c:	437a      	muls	r2, r7
 801843e:	1412      	asrs	r2, r2, #16
 8018440:	eb02 3263 	add.w	r2, r2, r3, asr #13
 8018444:	3202      	adds	r2, #2
 8018446:	8a73      	ldrh	r3, [r6, #18]
 8018448:	1092      	asrs	r2, r2, #2
 801844a:	f502 4200 	add.w	r2, r2, #32768	; 0x8000
 801844e:	4353      	muls	r3, r2
 8018450:	2204      	movs	r2, #4
 8018452:	0bd9      	lsrs	r1, r3, #15
 8018454:	f9b6 300a 	ldrsh.w	r3, [r6, #10]
 8018458:	435f      	muls	r7, r3
 801845a:	f9b6 3004 	ldrsh.w	r3, [r6, #4]
 801845e:	4343      	muls	r3, r0
 8018460:	f9b6 0002 	ldrsh.w	r0, [r6, #2]
 8018464:	12db      	asrs	r3, r3, #11
 8018466:	eb03 23e7 	add.w	r3, r3, r7, asr #11
 801846a:	eb03 0380 	add.w	r3, r3, r0, lsl #2
 801846e:	40ab      	lsls	r3, r5
 8018470:	3302      	adds	r3, #2
 8018472:	fb93 f3f2 	sdiv	r3, r3, r2
 8018476:	f24c 3250 	movw	r2, #50000	; 0xc350
 801847a:	1ae4      	subs	r4, r4, r3
 801847c:	fa22 f305 	lsr.w	r3, r2, r5
 8018480:	4363      	muls	r3, r4
 8018482:	bf55      	itete	pl
 8018484:	005b      	lslpl	r3, r3, #1
 8018486:	fbb3 f3f1 	udivmi	r3, r3, r1
 801848a:	fbb3 f3f1 	udivpl	r3, r3, r1
 801848e:	005b      	lslmi	r3, r3, #1
 8018490:	1218      	asrs	r0, r3, #8
 8018492:	fb00 f200 	mul.w	r2, r0, r0
 8018496:	f640 30de 	movw	r0, #3038	; 0xbde
 801849a:	4350      	muls	r0, r2
 801849c:	4a04      	ldr	r2, [pc, #16]	; (80184b0 <_ZN15Adafruit_BMP08512readPressureEv+0xa6>)
 801849e:	1400      	asrs	r0, r0, #16
 80184a0:	435a      	muls	r2, r3
 80184a2:	eb00 4022 	add.w	r0, r0, r2, asr #16
 80184a6:	f600 60cf 	addw	r0, r0, #3791	; 0xecf
 80184aa:	eb03 1020 	add.w	r0, r3, r0, asr #4
 80184ae:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 80184b0:	ffffe343 	.word	0xffffe343

080184b4 <_ZN15Adafruit_BMP08520readSealevelPressureEf>:
 80184b4:	b570      	push	{r4, r5, r6, lr}
 80184b6:	460c      	mov	r4, r1
 80184b8:	f7ff ffa7 	bl	801840a <_ZN15Adafruit_BMP08512readPressureEv>
 80184bc:	4912      	ldr	r1, [pc, #72]	; (8018508 <_ZN15Adafruit_BMP08520readSealevelPressureEf+0x54>)
 80184be:	4606      	mov	r6, r0
 80184c0:	4620      	mov	r0, r4
 80184c2:	f002 fcb1 	bl	801ae28 <__aeabi_fdiv>
 80184c6:	f001 ff6d 	bl	801a3a4 <__aeabi_f2d>
 80184ca:	4602      	mov	r2, r0
 80184cc:	460b      	mov	r3, r1
 80184ce:	2000      	movs	r0, #0
 80184d0:	490e      	ldr	r1, [pc, #56]	; (801850c <_ZN15Adafruit_BMP08520readSealevelPressureEf+0x58>)
 80184d2:	f001 fe07 	bl	801a0e4 <__aeabi_dsub>
 80184d6:	a30a      	add	r3, pc, #40	; (adr r3, 8018500 <_ZN15Adafruit_BMP08520readSealevelPressureEf+0x4c>)
 80184d8:	e9d3 2300 	ldrd	r2, r3, [r3]
 80184dc:	f000 fd14 	bl	8018f08 <pow>
 80184e0:	460d      	mov	r5, r1
 80184e2:	4604      	mov	r4, r0
 80184e4:	4630      	mov	r0, r6
 80184e6:	f002 fb97 	bl	801ac18 <__aeabi_i2f>
 80184ea:	f001 ff5b 	bl	801a3a4 <__aeabi_f2d>
 80184ee:	4622      	mov	r2, r4
 80184f0:	462b      	mov	r3, r5
 80184f2:	f002 f8d9 	bl	801a6a8 <__aeabi_ddiv>
 80184f6:	f002 fa5d 	bl	801a9b4 <__aeabi_d2iz>
 80184fa:	bd70      	pop	{r4, r5, r6, pc}
 80184fc:	f3af 8000 	nop.w
 8018500:	b851eb85 	.word	0xb851eb85
 8018504:	4015051e 	.word	0x4015051e
 8018508:	472d2a00 	.word	0x472d2a00
 801850c:	3ff00000 	.word	0x3ff00000

08018510 <_ZN15Adafruit_BMP08512readAltitudeEf>:
 8018510:	b510      	push	{r4, lr}
 8018512:	460c      	mov	r4, r1
 8018514:	f7ff ff79 	bl	801840a <_ZN15Adafruit_BMP08512readPressureEv>
 8018518:	f002 fb7e 	bl	801ac18 <__aeabi_i2f>
 801851c:	4621      	mov	r1, r4
 801851e:	f002 fc83 	bl	801ae28 <__aeabi_fdiv>
 8018522:	f001 ff3f 	bl	801a3a4 <__aeabi_f2d>
 8018526:	a30a      	add	r3, pc, #40	; (adr r3, 8018550 <_ZN15Adafruit_BMP08512readAltitudeEf+0x40>)
 8018528:	e9d3 2300 	ldrd	r2, r3, [r3]
 801852c:	f000 fcec 	bl	8018f08 <pow>
 8018530:	4602      	mov	r2, r0
 8018532:	460b      	mov	r3, r1
 8018534:	2000      	movs	r0, #0
 8018536:	490a      	ldr	r1, [pc, #40]	; (8018560 <_ZN15Adafruit_BMP08512readAltitudeEf+0x50>)
 8018538:	f001 fdd4 	bl	801a0e4 <__aeabi_dsub>
 801853c:	a306      	add	r3, pc, #24	; (adr r3, 8018558 <_ZN15Adafruit_BMP08512readAltitudeEf+0x48>)
 801853e:	e9d3 2300 	ldrd	r2, r3, [r3]
 8018542:	f001 ff87 	bl	801a454 <__aeabi_dmul>
 8018546:	f002 fa5d 	bl	801aa04 <__aeabi_d2f>
 801854a:	bd10      	pop	{r4, pc}
 801854c:	f3af 8000 	nop.w
 8018550:	1a36e2eb 	.word	0x1a36e2eb
 8018554:	3fc85bc0 	.word	0x3fc85bc0
 8018558:	00000000 	.word	0x00000000
 801855c:	40e5a540 	.word	0x40e5a540
 8018560:	3ff00000 	.word	0x3ff00000

08018564 <_Z9i2c_writehh>:
 8018564:	b538      	push	{r3, r4, r5, lr}
 8018566:	4604      	mov	r4, r0
 8018568:	460d      	mov	r5, r1
 801856a:	f7ff fdcd 	bl	8018108 <_Z9i2c_startv>
 801856e:	0060      	lsls	r0, r4, #1
 8018570:	2100      	movs	r1, #0
 8018572:	f000 00fe 	and.w	r0, r0, #254	; 0xfe
 8018576:	f7ff fdf3 	bl	8018160 <_Z21i2c_address_directionhh>
 801857a:	4628      	mov	r0, r5
 801857c:	f7ff fe12 	bl	80181a4 <_Z12i2c_transmith>
 8018580:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8018584:	f7ff bdda 	b.w	801813c <_Z8i2c_stopv>

08018588 <_Z8i2c_readhPh>:
 8018588:	b538      	push	{r3, r4, r5, lr}
 801858a:	4604      	mov	r4, r0
 801858c:	460d      	mov	r5, r1
 801858e:	f7ff fdbb 	bl	8018108 <_Z9i2c_startv>
 8018592:	0060      	lsls	r0, r4, #1
 8018594:	2101      	movs	r1, #1
 8018596:	f000 00fe 	and.w	r0, r0, #254	; 0xfe
 801859a:	f7ff fde1 	bl	8018160 <_Z21i2c_address_directionhh>
 801859e:	f7ff fe3d 	bl	801821c <_Z16i2c_receive_nackv>
 80185a2:	7028      	strb	r0, [r5, #0]
 80185a4:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80185a8:	f7ff bdc8 	b.w	801813c <_Z8i2c_stopv>

080185ac <_ZN5CWire5writeEh>:
 80185ac:	b570      	push	{r4, r5, r6, lr}
 80185ae:	7945      	ldrb	r5, [r0, #5]
 80185b0:	4604      	mov	r4, r0
 80185b2:	460e      	mov	r6, r1
 80185b4:	b955      	cbnz	r5, 80185cc <_ZN5CWire5writeEh+0x20>
 80185b6:	2301      	movs	r3, #1
 80185b8:	7143      	strb	r3, [r0, #5]
 80185ba:	f7ff fda5 	bl	8018108 <_Z9i2c_startv>
 80185be:	7920      	ldrb	r0, [r4, #4]
 80185c0:	4629      	mov	r1, r5
 80185c2:	0040      	lsls	r0, r0, #1
 80185c4:	f000 00fe 	and.w	r0, r0, #254	; 0xfe
 80185c8:	f7ff fdca 	bl	8018160 <_Z21i2c_address_directionhh>
 80185cc:	4630      	mov	r0, r6
 80185ce:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
 80185d2:	f7ff bde7 	b.w	80181a4 <_Z12i2c_transmith>
	...

080185d8 <_GLOBAL__sub_I__Z5delayi>:
 80185d8:	2200      	movs	r2, #0
 80185da:	4b02      	ldr	r3, [pc, #8]	; (80185e4 <_GLOBAL__sub_I__Z5delayi+0xc>)
 80185dc:	601a      	str	r2, [r3, #0]
 80185de:	715a      	strb	r2, [r3, #5]
 80185e0:	719a      	strb	r2, [r3, #6]
 80185e2:	4770      	bx	lr
 80185e4:	200054e0 	.word	0x200054e0

080185e8 <I2C_DeInit>:
 80185e8:	b508      	push	{r3, lr}
 80185ea:	4b0b      	ldr	r3, [pc, #44]	; (8018618 <I2C_DeInit+0x30>)
 80185ec:	2101      	movs	r1, #1
 80185ee:	4298      	cmp	r0, r3
 80185f0:	d10a      	bne.n	8018608 <I2C_DeInit+0x20>
 80185f2:	f44f 1000 	mov.w	r0, #2097152	; 0x200000
 80185f6:	f000 fc25 	bl	8018e44 <RCC_APB1PeriphResetCmd>
 80185fa:	2100      	movs	r1, #0
 80185fc:	f44f 1000 	mov.w	r0, #2097152	; 0x200000
 8018600:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8018604:	f000 bc1e 	b.w	8018e44 <RCC_APB1PeriphResetCmd>
 8018608:	f44f 0080 	mov.w	r0, #4194304	; 0x400000
 801860c:	f000 fc1a 	bl	8018e44 <RCC_APB1PeriphResetCmd>
 8018610:	2100      	movs	r1, #0
 8018612:	f44f 0080 	mov.w	r0, #4194304	; 0x400000
 8018616:	e7f3      	b.n	8018600 <I2C_DeInit+0x18>
 8018618:	40005400 	.word	0x40005400

0801861c <I2C_Init>:
 801861c:	b570      	push	{r4, r5, r6, lr}
 801861e:	b086      	sub	sp, #24
 8018620:	4604      	mov	r4, r0
 8018622:	8886      	ldrh	r6, [r0, #4]
 8018624:	a801      	add	r0, sp, #4
 8018626:	460d      	mov	r5, r1
 8018628:	f000 fb9c 	bl	8018d64 <RCC_GetClocksFreq>
 801862c:	9a03      	ldr	r2, [sp, #12]
 801862e:	492b      	ldr	r1, [pc, #172]	; (80186dc <I2C_Init+0xc0>)
 8018630:	f026 063f 	bic.w	r6, r6, #63	; 0x3f
 8018634:	fbb2 f1f1 	udiv	r1, r2, r1
 8018638:	0436      	lsls	r6, r6, #16
 801863a:	0c36      	lsrs	r6, r6, #16
 801863c:	b288      	uxth	r0, r1
 801863e:	4306      	orrs	r6, r0
 8018640:	80a6      	strh	r6, [r4, #4]
 8018642:	8823      	ldrh	r3, [r4, #0]
 8018644:	4e26      	ldr	r6, [pc, #152]	; (80186e0 <I2C_Init+0xc4>)
 8018646:	f023 0301 	bic.w	r3, r3, #1
 801864a:	041b      	lsls	r3, r3, #16
 801864c:	0c1b      	lsrs	r3, r3, #16
 801864e:	8023      	strh	r3, [r4, #0]
 8018650:	682b      	ldr	r3, [r5, #0]
 8018652:	42b3      	cmp	r3, r6
 8018654:	d81e      	bhi.n	8018694 <I2C_Init+0x78>
 8018656:	005b      	lsls	r3, r3, #1
 8018658:	fbb2 f3f3 	udiv	r3, r2, r3
 801865c:	b29b      	uxth	r3, r3
 801865e:	2b04      	cmp	r3, #4
 8018660:	bf38      	it	cc
 8018662:	2304      	movcc	r3, #4
 8018664:	3001      	adds	r0, #1
 8018666:	b280      	uxth	r0, r0
 8018668:	8420      	strh	r0, [r4, #32]
 801866a:	83a3      	strh	r3, [r4, #28]
 801866c:	8823      	ldrh	r3, [r4, #0]
 801866e:	88aa      	ldrh	r2, [r5, #4]
 8018670:	f043 0301 	orr.w	r3, r3, #1
 8018674:	8023      	strh	r3, [r4, #0]
 8018676:	8823      	ldrh	r3, [r4, #0]
 8018678:	8969      	ldrh	r1, [r5, #10]
 801867a:	f423 6381 	bic.w	r3, r3, #1032	; 0x408
 801867e:	430a      	orrs	r2, r1
 8018680:	f023 0302 	bic.w	r3, r3, #2
 8018684:	4313      	orrs	r3, r2
 8018686:	8023      	strh	r3, [r4, #0]
 8018688:	892a      	ldrh	r2, [r5, #8]
 801868a:	89ab      	ldrh	r3, [r5, #12]
 801868c:	4313      	orrs	r3, r2
 801868e:	8123      	strh	r3, [r4, #8]
 8018690:	b006      	add	sp, #24
 8018692:	bd70      	pop	{r4, r5, r6, pc}
 8018694:	f64b 70ff 	movw	r0, #49151	; 0xbfff
 8018698:	88ee      	ldrh	r6, [r5, #6]
 801869a:	4286      	cmp	r6, r0
 801869c:	bf1b      	ittet	ne
 801869e:	2019      	movne	r0, #25
 80186a0:	4343      	mulne	r3, r0
 80186a2:	eb03 0343 	addeq.w	r3, r3, r3, lsl #1
 80186a6:	fbb2 f3f3 	udivne	r3, r2, r3
 80186aa:	bf0b      	itete	eq
 80186ac:	fbb2 f3f3 	udiveq	r3, r2, r3
 80186b0:	b29b      	uxthne	r3, r3
 80186b2:	b29b      	uxtheq	r3, r3
 80186b4:	f443 4380 	orrne.w	r3, r3, #16384	; 0x4000
 80186b8:	f3c3 020b 	ubfx	r2, r3, #0, #12
 80186bc:	b90a      	cbnz	r2, 80186c2 <I2C_Init+0xa6>
 80186be:	f043 0301 	orr.w	r3, r3, #1
 80186c2:	f44f 7296 	mov.w	r2, #300	; 0x12c
 80186c6:	434a      	muls	r2, r1
 80186c8:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
 80186cc:	fb92 f2f1 	sdiv	r2, r2, r1
 80186d0:	3201      	adds	r2, #1
 80186d2:	b292      	uxth	r2, r2
 80186d4:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 80186d8:	8422      	strh	r2, [r4, #32]
 80186da:	e7c6      	b.n	801866a <I2C_Init+0x4e>
 80186dc:	000f4240 	.word	0x000f4240
 80186e0:	000186a0 	.word	0x000186a0

080186e4 <I2C_StructInit>:
 80186e4:	f241 3288 	movw	r2, #5000	; 0x1388
 80186e8:	4b04      	ldr	r3, [pc, #16]	; (80186fc <I2C_StructInit+0x18>)
 80186ea:	e9c0 2300 	strd	r2, r3, [r0]
 80186ee:	2300      	movs	r3, #0
 80186f0:	6083      	str	r3, [r0, #8]
 80186f2:	f44f 4380 	mov.w	r3, #16384	; 0x4000
 80186f6:	8183      	strh	r3, [r0, #12]
 80186f8:	4770      	bx	lr
 80186fa:	bf00      	nop
 80186fc:	bfff0000 	.word	0xbfff0000

08018700 <I2C_Cmd>:
 8018700:	8803      	ldrh	r3, [r0, #0]
 8018702:	b119      	cbz	r1, 801870c <I2C_Cmd+0xc>
 8018704:	f043 0301 	orr.w	r3, r3, #1
 8018708:	8003      	strh	r3, [r0, #0]
 801870a:	4770      	bx	lr
 801870c:	f023 0301 	bic.w	r3, r3, #1
 8018710:	041b      	lsls	r3, r3, #16
 8018712:	0c1b      	lsrs	r3, r3, #16
 8018714:	e7f8      	b.n	8018708 <I2C_Cmd+0x8>

08018716 <I2C_DMACmd>:
 8018716:	8883      	ldrh	r3, [r0, #4]
 8018718:	b119      	cbz	r1, 8018722 <I2C_DMACmd+0xc>
 801871a:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
 801871e:	8083      	strh	r3, [r0, #4]
 8018720:	4770      	bx	lr
 8018722:	f423 6300 	bic.w	r3, r3, #2048	; 0x800
 8018726:	041b      	lsls	r3, r3, #16
 8018728:	0c1b      	lsrs	r3, r3, #16
 801872a:	e7f8      	b.n	801871e <I2C_DMACmd+0x8>

0801872c <I2C_DMALastTransferCmd>:
 801872c:	8883      	ldrh	r3, [r0, #4]
 801872e:	b119      	cbz	r1, 8018738 <I2C_DMALastTransferCmd+0xc>
 8018730:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
 8018734:	8083      	strh	r3, [r0, #4]
 8018736:	4770      	bx	lr
 8018738:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
 801873c:	041b      	lsls	r3, r3, #16
 801873e:	0c1b      	lsrs	r3, r3, #16
 8018740:	e7f8      	b.n	8018734 <I2C_DMALastTransferCmd+0x8>

08018742 <I2C_GenerateSTART>:
 8018742:	8803      	ldrh	r3, [r0, #0]
 8018744:	b119      	cbz	r1, 801874e <I2C_GenerateSTART+0xc>
 8018746:	f443 7380 	orr.w	r3, r3, #256	; 0x100
 801874a:	8003      	strh	r3, [r0, #0]
 801874c:	4770      	bx	lr
 801874e:	f423 7380 	bic.w	r3, r3, #256	; 0x100
 8018752:	041b      	lsls	r3, r3, #16
 8018754:	0c1b      	lsrs	r3, r3, #16
 8018756:	e7f8      	b.n	801874a <I2C_GenerateSTART+0x8>

08018758 <I2C_GenerateSTOP>:
 8018758:	8803      	ldrh	r3, [r0, #0]
 801875a:	b119      	cbz	r1, 8018764 <I2C_GenerateSTOP+0xc>
 801875c:	f443 7300 	orr.w	r3, r3, #512	; 0x200
 8018760:	8003      	strh	r3, [r0, #0]
 8018762:	4770      	bx	lr
 8018764:	f423 7300 	bic.w	r3, r3, #512	; 0x200
 8018768:	041b      	lsls	r3, r3, #16
 801876a:	0c1b      	lsrs	r3, r3, #16
 801876c:	e7f8      	b.n	8018760 <I2C_GenerateSTOP+0x8>

0801876e <I2C_AcknowledgeConfig>:
 801876e:	8803      	ldrh	r3, [r0, #0]
 8018770:	b119      	cbz	r1, 801877a <I2C_AcknowledgeConfig+0xc>
 8018772:	f443 6380 	orr.w	r3, r3, #1024	; 0x400
 8018776:	8003      	strh	r3, [r0, #0]
 8018778:	4770      	bx	lr
 801877a:	f423 6380 	bic.w	r3, r3, #1024	; 0x400
 801877e:	041b      	lsls	r3, r3, #16
 8018780:	0c1b      	lsrs	r3, r3, #16
 8018782:	e7f8      	b.n	8018776 <I2C_AcknowledgeConfig+0x8>

08018784 <I2C_OwnAddress2Config>:
 8018784:	8983      	ldrh	r3, [r0, #12]
 8018786:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
 801878a:	b29b      	uxth	r3, r3
 801878c:	f023 03fe 	bic.w	r3, r3, #254	; 0xfe
 8018790:	430b      	orrs	r3, r1
 8018792:	8183      	strh	r3, [r0, #12]
 8018794:	4770      	bx	lr

08018796 <I2C_DualAddressCmd>:
 8018796:	8983      	ldrh	r3, [r0, #12]
 8018798:	b119      	cbz	r1, 80187a2 <I2C_DualAddressCmd+0xc>
 801879a:	f043 0301 	orr.w	r3, r3, #1
 801879e:	8183      	strh	r3, [r0, #12]
 80187a0:	4770      	bx	lr
 80187a2:	f023 0301 	bic.w	r3, r3, #1
 80187a6:	041b      	lsls	r3, r3, #16
 80187a8:	0c1b      	lsrs	r3, r3, #16
 80187aa:	e7f8      	b.n	801879e <I2C_DualAddressCmd+0x8>

080187ac <I2C_GeneralCallCmd>:
 80187ac:	8803      	ldrh	r3, [r0, #0]
 80187ae:	b119      	cbz	r1, 80187b8 <I2C_GeneralCallCmd+0xc>
 80187b0:	f043 0340 	orr.w	r3, r3, #64	; 0x40
 80187b4:	8003      	strh	r3, [r0, #0]
 80187b6:	4770      	bx	lr
 80187b8:	f023 0340 	bic.w	r3, r3, #64	; 0x40
 80187bc:	041b      	lsls	r3, r3, #16
 80187be:	0c1b      	lsrs	r3, r3, #16
 80187c0:	e7f8      	b.n	80187b4 <I2C_GeneralCallCmd+0x8>

080187c2 <I2C_ITConfig>:
 80187c2:	8883      	ldrh	r3, [r0, #4]
 80187c4:	b112      	cbz	r2, 80187cc <I2C_ITConfig+0xa>
 80187c6:	4319      	orrs	r1, r3
 80187c8:	8081      	strh	r1, [r0, #4]
 80187ca:	4770      	bx	lr
 80187cc:	ea23 0101 	bic.w	r1, r3, r1
 80187d0:	e7fa      	b.n	80187c8 <I2C_ITConfig+0x6>

080187d2 <I2C_SendData>:
 80187d2:	8201      	strh	r1, [r0, #16]
 80187d4:	4770      	bx	lr

080187d6 <I2C_ReceiveData>:
 80187d6:	8a00      	ldrh	r0, [r0, #16]
 80187d8:	b2c0      	uxtb	r0, r0
 80187da:	4770      	bx	lr

080187dc <I2C_Send7bitAddress>:
 80187dc:	b122      	cbz	r2, 80187e8 <I2C_Send7bitAddress+0xc>
 80187de:	f041 0101 	orr.w	r1, r1, #1
 80187e2:	b289      	uxth	r1, r1
 80187e4:	8201      	strh	r1, [r0, #16]
 80187e6:	4770      	bx	lr
 80187e8:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
 80187ec:	e7f9      	b.n	80187e2 <I2C_Send7bitAddress+0x6>

080187ee <I2C_ReadRegister>:
 80187ee:	2300      	movs	r3, #0
 80187f0:	b082      	sub	sp, #8
 80187f2:	9301      	str	r3, [sp, #4]
 80187f4:	9001      	str	r0, [sp, #4]
 80187f6:	9b01      	ldr	r3, [sp, #4]
 80187f8:	4419      	add	r1, r3
 80187fa:	9101      	str	r1, [sp, #4]
 80187fc:	9b01      	ldr	r3, [sp, #4]
 80187fe:	8818      	ldrh	r0, [r3, #0]
 8018800:	b002      	add	sp, #8
 8018802:	4770      	bx	lr

08018804 <I2C_SoftwareResetCmd>:
 8018804:	8803      	ldrh	r3, [r0, #0]
 8018806:	b119      	cbz	r1, 8018810 <I2C_SoftwareResetCmd+0xc>
 8018808:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
 801880c:	8003      	strh	r3, [r0, #0]
 801880e:	4770      	bx	lr
 8018810:	f3c3 030e 	ubfx	r3, r3, #0, #15
 8018814:	e7fa      	b.n	801880c <I2C_SoftwareResetCmd+0x8>

08018816 <I2C_NACKPositionConfig>:
 8018816:	8803      	ldrh	r3, [r0, #0]
 8018818:	f5b1 6f00 	cmp.w	r1, #2048	; 0x800
 801881c:	bf1b      	ittet	ne
 801881e:	f423 6300 	bicne.w	r3, r3, #2048	; 0x800
 8018822:	041b      	lslne	r3, r3, #16
 8018824:	f443 6300 	orreq.w	r3, r3, #2048	; 0x800
 8018828:	0c1b      	lsrne	r3, r3, #16
 801882a:	8003      	strh	r3, [r0, #0]
 801882c:	4770      	bx	lr

0801882e <I2C_SMBusAlertConfig>:
 801882e:	8803      	ldrh	r3, [r0, #0]
 8018830:	f5b1 5f00 	cmp.w	r1, #8192	; 0x2000
 8018834:	bf1b      	ittet	ne
 8018836:	f423 5300 	bicne.w	r3, r3, #8192	; 0x2000
 801883a:	041b      	lslne	r3, r3, #16
 801883c:	f443 5300 	orreq.w	r3, r3, #8192	; 0x2000
 8018840:	0c1b      	lsrne	r3, r3, #16
 8018842:	8003      	strh	r3, [r0, #0]
 8018844:	4770      	bx	lr

08018846 <I2C_TransmitPEC>:
 8018846:	8803      	ldrh	r3, [r0, #0]
 8018848:	b119      	cbz	r1, 8018852 <I2C_TransmitPEC+0xc>
 801884a:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
 801884e:	8003      	strh	r3, [r0, #0]
 8018850:	4770      	bx	lr
 8018852:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
 8018856:	041b      	lsls	r3, r3, #16
 8018858:	0c1b      	lsrs	r3, r3, #16
 801885a:	e7f8      	b.n	801884e <I2C_TransmitPEC+0x8>

0801885c <I2C_PECPositionConfig>:
 801885c:	f7ff bfdb 	b.w	8018816 <I2C_NACKPositionConfig>

08018860 <I2C_CalculatePEC>:
 8018860:	8803      	ldrh	r3, [r0, #0]
 8018862:	b119      	cbz	r1, 801886c <I2C_CalculatePEC+0xc>
 8018864:	f043 0320 	orr.w	r3, r3, #32
 8018868:	8003      	strh	r3, [r0, #0]
 801886a:	4770      	bx	lr
 801886c:	f023 0320 	bic.w	r3, r3, #32
 8018870:	041b      	lsls	r3, r3, #16
 8018872:	0c1b      	lsrs	r3, r3, #16
 8018874:	e7f8      	b.n	8018868 <I2C_CalculatePEC+0x8>

08018876 <I2C_GetPEC>:
 8018876:	8b00      	ldrh	r0, [r0, #24]
 8018878:	0a00      	lsrs	r0, r0, #8
 801887a:	4770      	bx	lr

0801887c <I2C_ARPCmd>:
 801887c:	8803      	ldrh	r3, [r0, #0]
 801887e:	b119      	cbz	r1, 8018888 <I2C_ARPCmd+0xc>
 8018880:	f043 0310 	orr.w	r3, r3, #16
 8018884:	8003      	strh	r3, [r0, #0]
 8018886:	4770      	bx	lr
 8018888:	f023 0310 	bic.w	r3, r3, #16
 801888c:	041b      	lsls	r3, r3, #16
 801888e:	0c1b      	lsrs	r3, r3, #16
 8018890:	e7f8      	b.n	8018884 <I2C_ARPCmd+0x8>

08018892 <I2C_StretchClockCmd>:
 8018892:	8803      	ldrh	r3, [r0, #0]
 8018894:	b919      	cbnz	r1, 801889e <I2C_StretchClockCmd+0xc>
 8018896:	f043 0380 	orr.w	r3, r3, #128	; 0x80
 801889a:	8003      	strh	r3, [r0, #0]
 801889c:	4770      	bx	lr
 801889e:	f023 0380 	bic.w	r3, r3, #128	; 0x80
 80188a2:	041b      	lsls	r3, r3, #16
 80188a4:	0c1b      	lsrs	r3, r3, #16
 80188a6:	e7f8      	b.n	801889a <I2C_StretchClockCmd+0x8>

080188a8 <I2C_FastModeDutyCycleConfig>:
 80188a8:	8b83      	ldrh	r3, [r0, #28]
 80188aa:	f5b1 4f80 	cmp.w	r1, #16384	; 0x4000
 80188ae:	bf1d      	ittte	ne
 80188b0:	f423 4380 	bicne.w	r3, r3, #16384	; 0x4000
 80188b4:	041b      	lslne	r3, r3, #16
 80188b6:	0c1b      	lsrne	r3, r3, #16
 80188b8:	f443 4380 	orreq.w	r3, r3, #16384	; 0x4000
 80188bc:	8383      	strh	r3, [r0, #28]
 80188be:	4770      	bx	lr

080188c0 <I2C_CheckEvent>:
 80188c0:	8a83      	ldrh	r3, [r0, #20]
 80188c2:	8b00      	ldrh	r0, [r0, #24]
 80188c4:	ea43 4300 	orr.w	r3, r3, r0, lsl #16
 80188c8:	f021 407f 	bic.w	r0, r1, #4278190080	; 0xff000000
 80188cc:	4018      	ands	r0, r3
 80188ce:	1a43      	subs	r3, r0, r1
 80188d0:	4258      	negs	r0, r3
 80188d2:	4158      	adcs	r0, r3
 80188d4:	4770      	bx	lr

080188d6 <I2C_GetLastEvent>:
 80188d6:	8a83      	ldrh	r3, [r0, #20]
 80188d8:	8b00      	ldrh	r0, [r0, #24]
 80188da:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
 80188de:	f020 407f 	bic.w	r0, r0, #4278190080	; 0xff000000
 80188e2:	4770      	bx	lr

080188e4 <I2C_GetFlagStatus>:
 80188e4:	2300      	movs	r3, #0
 80188e6:	b082      	sub	sp, #8
 80188e8:	9300      	str	r3, [sp, #0]
 80188ea:	9301      	str	r3, [sp, #4]
 80188ec:	0f0b      	lsrs	r3, r1, #28
 80188ee:	9001      	str	r0, [sp, #4]
 80188f0:	9300      	str	r3, [sp, #0]
 80188f2:	9b00      	ldr	r3, [sp, #0]
 80188f4:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 80188f8:	b153      	cbz	r3, 8018910 <I2C_GetFlagStatus+0x2c>
 80188fa:	9b01      	ldr	r3, [sp, #4]
 80188fc:	3314      	adds	r3, #20
 80188fe:	9301      	str	r3, [sp, #4]
 8018900:	9b01      	ldr	r3, [sp, #4]
 8018902:	681b      	ldr	r3, [r3, #0]
 8018904:	420b      	tst	r3, r1
 8018906:	bf14      	ite	ne
 8018908:	2001      	movne	r0, #1
 801890a:	2000      	moveq	r0, #0
 801890c:	b002      	add	sp, #8
 801890e:	4770      	bx	lr
 8018910:	9b01      	ldr	r3, [sp, #4]
 8018912:	0c09      	lsrs	r1, r1, #16
 8018914:	3318      	adds	r3, #24
 8018916:	e7f2      	b.n	80188fe <I2C_GetFlagStatus+0x1a>

08018918 <I2C_ClearFlag>:
 8018918:	43c9      	mvns	r1, r1
 801891a:	b289      	uxth	r1, r1
 801891c:	8281      	strh	r1, [r0, #20]
 801891e:	4770      	bx	lr

08018920 <I2C_GetITStatus>:
 8018920:	8883      	ldrh	r3, [r0, #4]
 8018922:	8a80      	ldrh	r0, [r0, #20]
 8018924:	b29b      	uxth	r3, r3
 8018926:	4008      	ands	r0, r1
 8018928:	d006      	beq.n	8018938 <I2C_GetITStatus+0x18>
 801892a:	f403 63e0 	and.w	r3, r3, #1792	; 0x700
 801892e:	ea13 4311 	ands.w	r3, r3, r1, lsr #16
 8018932:	bf14      	ite	ne
 8018934:	2001      	movne	r0, #1
 8018936:	2000      	moveq	r0, #0
 8018938:	4770      	bx	lr

0801893a <I2C_ClearITPendingBit>:
 801893a:	43c9      	mvns	r1, r1
 801893c:	b289      	uxth	r1, r1
 801893e:	8281      	strh	r1, [r0, #20]
 8018940:	4770      	bx	lr
 8018942:	bf00      	nop

08018944 <GPIO_DeInit>:
 8018944:	b508      	push	{r3, lr}
 8018946:	4b26      	ldr	r3, [pc, #152]	; (80189e0 <GPIO_DeInit+0x9c>)
 8018948:	4298      	cmp	r0, r3
 801894a:	d109      	bne.n	8018960 <GPIO_DeInit+0x1c>
 801894c:	2101      	movs	r1, #1
 801894e:	2004      	movs	r0, #4
 8018950:	f000 fa6c 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 8018954:	2100      	movs	r1, #0
 8018956:	2004      	movs	r0, #4
 8018958:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 801895c:	f000 ba66 	b.w	8018e2c <RCC_APB2PeriphResetCmd>
 8018960:	4b20      	ldr	r3, [pc, #128]	; (80189e4 <GPIO_DeInit+0xa0>)
 8018962:	4298      	cmp	r0, r3
 8018964:	d106      	bne.n	8018974 <GPIO_DeInit+0x30>
 8018966:	2101      	movs	r1, #1
 8018968:	2008      	movs	r0, #8
 801896a:	f000 fa5f 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 801896e:	2100      	movs	r1, #0
 8018970:	2008      	movs	r0, #8
 8018972:	e7f1      	b.n	8018958 <GPIO_DeInit+0x14>
 8018974:	4b1c      	ldr	r3, [pc, #112]	; (80189e8 <GPIO_DeInit+0xa4>)
 8018976:	4298      	cmp	r0, r3
 8018978:	d106      	bne.n	8018988 <GPIO_DeInit+0x44>
 801897a:	2101      	movs	r1, #1
 801897c:	2010      	movs	r0, #16
 801897e:	f000 fa55 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 8018982:	2100      	movs	r1, #0
 8018984:	2010      	movs	r0, #16
 8018986:	e7e7      	b.n	8018958 <GPIO_DeInit+0x14>
 8018988:	4b18      	ldr	r3, [pc, #96]	; (80189ec <GPIO_DeInit+0xa8>)
 801898a:	4298      	cmp	r0, r3
 801898c:	d106      	bne.n	801899c <GPIO_DeInit+0x58>
 801898e:	2101      	movs	r1, #1
 8018990:	2020      	movs	r0, #32
 8018992:	f000 fa4b 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 8018996:	2100      	movs	r1, #0
 8018998:	2020      	movs	r0, #32
 801899a:	e7dd      	b.n	8018958 <GPIO_DeInit+0x14>
 801899c:	4b14      	ldr	r3, [pc, #80]	; (80189f0 <GPIO_DeInit+0xac>)
 801899e:	4298      	cmp	r0, r3
 80189a0:	d106      	bne.n	80189b0 <GPIO_DeInit+0x6c>
 80189a2:	2101      	movs	r1, #1
 80189a4:	2040      	movs	r0, #64	; 0x40
 80189a6:	f000 fa41 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 80189aa:	2100      	movs	r1, #0
 80189ac:	2040      	movs	r0, #64	; 0x40
 80189ae:	e7d3      	b.n	8018958 <GPIO_DeInit+0x14>
 80189b0:	4b10      	ldr	r3, [pc, #64]	; (80189f4 <GPIO_DeInit+0xb0>)
 80189b2:	4298      	cmp	r0, r3
 80189b4:	d106      	bne.n	80189c4 <GPIO_DeInit+0x80>
 80189b6:	2101      	movs	r1, #1
 80189b8:	2080      	movs	r0, #128	; 0x80
 80189ba:	f000 fa37 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 80189be:	2100      	movs	r1, #0
 80189c0:	2080      	movs	r0, #128	; 0x80
 80189c2:	e7c9      	b.n	8018958 <GPIO_DeInit+0x14>
 80189c4:	4b0c      	ldr	r3, [pc, #48]	; (80189f8 <GPIO_DeInit+0xb4>)
 80189c6:	4298      	cmp	r0, r3
 80189c8:	d108      	bne.n	80189dc <GPIO_DeInit+0x98>
 80189ca:	2101      	movs	r1, #1
 80189cc:	f44f 7080 	mov.w	r0, #256	; 0x100
 80189d0:	f000 fa2c 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 80189d4:	2100      	movs	r1, #0
 80189d6:	f44f 7080 	mov.w	r0, #256	; 0x100
 80189da:	e7bd      	b.n	8018958 <GPIO_DeInit+0x14>
 80189dc:	bd08      	pop	{r3, pc}
 80189de:	bf00      	nop
 80189e0:	40010800 	.word	0x40010800
 80189e4:	40010c00 	.word	0x40010c00
 80189e8:	40011000 	.word	0x40011000
 80189ec:	40011400 	.word	0x40011400
 80189f0:	40011800 	.word	0x40011800
 80189f4:	40011c00 	.word	0x40011c00
 80189f8:	40012000 	.word	0x40012000

080189fc <GPIO_AFIODeInit>:
 80189fc:	2101      	movs	r1, #1
 80189fe:	b508      	push	{r3, lr}
 8018a00:	4608      	mov	r0, r1
 8018a02:	f000 fa13 	bl	8018e2c <RCC_APB2PeriphResetCmd>
 8018a06:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8018a0a:	2100      	movs	r1, #0
 8018a0c:	2001      	movs	r0, #1
 8018a0e:	f000 ba0d 	b.w	8018e2c <RCC_APB2PeriphResetCmd>

08018a12 <GPIO_Init>:
 8018a12:	78ca      	ldrb	r2, [r1, #3]
 8018a14:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8018a18:	06d3      	lsls	r3, r2, #27
 8018a1a:	bf48      	it	mi
 8018a1c:	788b      	ldrbmi	r3, [r1, #2]
 8018a1e:	880e      	ldrh	r6, [r1, #0]
 8018a20:	f002 050f 	and.w	r5, r2, #15
 8018a24:	bf48      	it	mi
 8018a26:	431d      	orrmi	r5, r3
 8018a28:	f016 0fff 	tst.w	r6, #255	; 0xff
 8018a2c:	d01a      	beq.n	8018a64 <GPIO_Init+0x52>
 8018a2e:	2100      	movs	r1, #0
 8018a30:	f04f 0c01 	mov.w	ip, #1
 8018a34:	f04f 0e0f 	mov.w	lr, #15
 8018a38:	6803      	ldr	r3, [r0, #0]
 8018a3a:	fa0c f701 	lsl.w	r7, ip, r1
 8018a3e:	ea37 0406 	bics.w	r4, r7, r6
 8018a42:	d10b      	bne.n	8018a5c <GPIO_Init+0x4a>
 8018a44:	008c      	lsls	r4, r1, #2
 8018a46:	fa0e f804 	lsl.w	r8, lr, r4
 8018a4a:	ea23 0308 	bic.w	r3, r3, r8
 8018a4e:	fa05 f404 	lsl.w	r4, r5, r4
 8018a52:	2a28      	cmp	r2, #40	; 0x28
 8018a54:	ea43 0304 	orr.w	r3, r3, r4
 8018a58:	d126      	bne.n	8018aa8 <GPIO_Init+0x96>
 8018a5a:	6147      	str	r7, [r0, #20]
 8018a5c:	3101      	adds	r1, #1
 8018a5e:	2908      	cmp	r1, #8
 8018a60:	d1eb      	bne.n	8018a3a <GPIO_Init+0x28>
 8018a62:	6003      	str	r3, [r0, #0]
 8018a64:	2eff      	cmp	r6, #255	; 0xff
 8018a66:	d91d      	bls.n	8018aa4 <GPIO_Init+0x92>
 8018a68:	2408      	movs	r4, #8
 8018a6a:	f04f 0c01 	mov.w	ip, #1
 8018a6e:	f04f 0e0f 	mov.w	lr, #15
 8018a72:	6841      	ldr	r1, [r0, #4]
 8018a74:	fa0c f704 	lsl.w	r7, ip, r4
 8018a78:	ea37 0306 	bics.w	r3, r7, r6
 8018a7c:	d10e      	bne.n	8018a9c <GPIO_Init+0x8a>
 8018a7e:	f104 4380 	add.w	r3, r4, #1073741824	; 0x40000000
 8018a82:	3b08      	subs	r3, #8
 8018a84:	009b      	lsls	r3, r3, #2
 8018a86:	fa0e f803 	lsl.w	r8, lr, r3
 8018a8a:	ea21 0108 	bic.w	r1, r1, r8
 8018a8e:	fa05 f303 	lsl.w	r3, r5, r3
 8018a92:	2a28      	cmp	r2, #40	; 0x28
 8018a94:	ea41 0103 	orr.w	r1, r1, r3
 8018a98:	d10a      	bne.n	8018ab0 <GPIO_Init+0x9e>
 8018a9a:	6147      	str	r7, [r0, #20]
 8018a9c:	3401      	adds	r4, #1
 8018a9e:	2c10      	cmp	r4, #16
 8018aa0:	d1e8      	bne.n	8018a74 <GPIO_Init+0x62>
 8018aa2:	6041      	str	r1, [r0, #4]
 8018aa4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8018aa8:	2a48      	cmp	r2, #72	; 0x48
 8018aaa:	bf08      	it	eq
 8018aac:	6107      	streq	r7, [r0, #16]
 8018aae:	e7d5      	b.n	8018a5c <GPIO_Init+0x4a>
 8018ab0:	2a48      	cmp	r2, #72	; 0x48
 8018ab2:	bf08      	it	eq
 8018ab4:	6107      	streq	r7, [r0, #16]
 8018ab6:	e7f1      	b.n	8018a9c <GPIO_Init+0x8a>

08018ab8 <GPIO_StructInit>:
 8018ab8:	f64f 73ff 	movw	r3, #65535	; 0xffff
 8018abc:	8003      	strh	r3, [r0, #0]
 8018abe:	2302      	movs	r3, #2
 8018ac0:	7083      	strb	r3, [r0, #2]
 8018ac2:	2304      	movs	r3, #4
 8018ac4:	70c3      	strb	r3, [r0, #3]
 8018ac6:	4770      	bx	lr

08018ac8 <GPIO_ReadInputDataBit>:
 8018ac8:	6883      	ldr	r3, [r0, #8]
 8018aca:	4219      	tst	r1, r3
 8018acc:	bf14      	ite	ne
 8018ace:	2001      	movne	r0, #1
 8018ad0:	2000      	moveq	r0, #0
 8018ad2:	4770      	bx	lr

08018ad4 <GPIO_ReadInputData>:
 8018ad4:	6880      	ldr	r0, [r0, #8]
 8018ad6:	b280      	uxth	r0, r0
 8018ad8:	4770      	bx	lr

08018ada <GPIO_ReadOutputDataBit>:
 8018ada:	68c3      	ldr	r3, [r0, #12]
 8018adc:	4219      	tst	r1, r3
 8018ade:	bf14      	ite	ne
 8018ae0:	2001      	movne	r0, #1
 8018ae2:	2000      	moveq	r0, #0
 8018ae4:	4770      	bx	lr

08018ae6 <GPIO_ReadOutputData>:
 8018ae6:	68c0      	ldr	r0, [r0, #12]
 8018ae8:	b280      	uxth	r0, r0
 8018aea:	4770      	bx	lr

08018aec <GPIO_SetBits>:
 8018aec:	6101      	str	r1, [r0, #16]
 8018aee:	4770      	bx	lr

08018af0 <GPIO_ResetBits>:
 8018af0:	6141      	str	r1, [r0, #20]
 8018af2:	4770      	bx	lr

08018af4 <GPIO_WriteBit>:
 8018af4:	b10a      	cbz	r2, 8018afa <GPIO_WriteBit+0x6>
 8018af6:	6101      	str	r1, [r0, #16]
 8018af8:	4770      	bx	lr
 8018afa:	6141      	str	r1, [r0, #20]
 8018afc:	4770      	bx	lr

08018afe <GPIO_Write>:
 8018afe:	60c1      	str	r1, [r0, #12]
 8018b00:	4770      	bx	lr

08018b02 <GPIO_PinLockConfig>:
 8018b02:	f441 3380 	orr.w	r3, r1, #65536	; 0x10000
 8018b06:	6183      	str	r3, [r0, #24]
 8018b08:	6181      	str	r1, [r0, #24]
 8018b0a:	6183      	str	r3, [r0, #24]
 8018b0c:	6983      	ldr	r3, [r0, #24]
 8018b0e:	6983      	ldr	r3, [r0, #24]
 8018b10:	4770      	bx	lr

08018b12 <GPIO_EventOutputConfig>:
 8018b12:	4a05      	ldr	r2, [pc, #20]	; (8018b28 <GPIO_EventOutputConfig+0x16>)
 8018b14:	6813      	ldr	r3, [r2, #0]
 8018b16:	f023 037f 	bic.w	r3, r3, #127	; 0x7f
 8018b1a:	041b      	lsls	r3, r3, #16
 8018b1c:	0c1b      	lsrs	r3, r3, #16
 8018b1e:	4319      	orrs	r1, r3
 8018b20:	ea41 1100 	orr.w	r1, r1, r0, lsl #4
 8018b24:	6011      	str	r1, [r2, #0]
 8018b26:	4770      	bx	lr
 8018b28:	40010000 	.word	0x40010000

08018b2c <GPIO_EventOutputCmd>:
 8018b2c:	4b01      	ldr	r3, [pc, #4]	; (8018b34 <GPIO_EventOutputCmd+0x8>)
 8018b2e:	6018      	str	r0, [r3, #0]
 8018b30:	4770      	bx	lr
 8018b32:	bf00      	nop
 8018b34:	4220001c 	.word	0x4220001c

08018b38 <GPIO_PinRemapConfig>:
 8018b38:	2800      	cmp	r0, #0
 8018b3a:	4a16      	ldr	r2, [pc, #88]	; (8018b94 <GPIO_PinRemapConfig+0x5c>)
 8018b3c:	b530      	push	{r4, r5, lr}
 8018b3e:	f400 1540 	and.w	r5, r0, #3145728	; 0x300000
 8018b42:	bfb4      	ite	lt
 8018b44:	69d3      	ldrlt	r3, [r2, #28]
 8018b46:	6853      	ldrge	r3, [r2, #4]
 8018b48:	f5b5 1f40 	cmp.w	r5, #3145728	; 0x300000
 8018b4c:	b284      	uxth	r4, r0
 8018b4e:	d110      	bne.n	8018b72 <GPIO_PinRemapConfig+0x3a>
 8018b50:	6855      	ldr	r5, [r2, #4]
 8018b52:	f023 6370 	bic.w	r3, r3, #251658240	; 0xf000000
 8018b56:	f025 6570 	bic.w	r5, r5, #251658240	; 0xf000000
 8018b5a:	6055      	str	r5, [r2, #4]
 8018b5c:	b119      	cbz	r1, 8018b66 <GPIO_PinRemapConfig+0x2e>
 8018b5e:	0d42      	lsrs	r2, r0, #21
 8018b60:	0112      	lsls	r2, r2, #4
 8018b62:	4094      	lsls	r4, r2
 8018b64:	4323      	orrs	r3, r4
 8018b66:	4a0b      	ldr	r2, [pc, #44]	; (8018b94 <GPIO_PinRemapConfig+0x5c>)
 8018b68:	2800      	cmp	r0, #0
 8018b6a:	bfb4      	ite	lt
 8018b6c:	61d3      	strlt	r3, [r2, #28]
 8018b6e:	6053      	strge	r3, [r2, #4]
 8018b70:	bd30      	pop	{r4, r5, pc}
 8018b72:	02c2      	lsls	r2, r0, #11
 8018b74:	bf4d      	iteet	mi
 8018b76:	2203      	movmi	r2, #3
 8018b78:	0d42      	lsrpl	r2, r0, #21
 8018b7a:	0112      	lslpl	r2, r2, #4
 8018b7c:	f3c0 4503 	ubfxmi	r5, r0, #16, #4
 8018b80:	bf4c      	ite	mi
 8018b82:	40aa      	lslmi	r2, r5
 8018b84:	fa04 f202 	lslpl.w	r2, r4, r2
 8018b88:	ea23 0302 	bic.w	r3, r3, r2
 8018b8c:	f043 6370 	orr.w	r3, r3, #251658240	; 0xf000000
 8018b90:	e7e4      	b.n	8018b5c <GPIO_PinRemapConfig+0x24>
 8018b92:	bf00      	nop
 8018b94:	40010000 	.word	0x40010000

08018b98 <GPIO_EXTILineConfig>:
 8018b98:	b510      	push	{r4, lr}
 8018b9a:	240f      	movs	r4, #15
 8018b9c:	f001 0303 	and.w	r3, r1, #3
 8018ba0:	f001 01fc 	and.w	r1, r1, #252	; 0xfc
 8018ba4:	f101 4180 	add.w	r1, r1, #1073741824	; 0x40000000
 8018ba8:	f501 3180 	add.w	r1, r1, #65536	; 0x10000
 8018bac:	688a      	ldr	r2, [r1, #8]
 8018bae:	009b      	lsls	r3, r3, #2
 8018bb0:	409c      	lsls	r4, r3
 8018bb2:	ea22 0204 	bic.w	r2, r2, r4
 8018bb6:	608a      	str	r2, [r1, #8]
 8018bb8:	688a      	ldr	r2, [r1, #8]
 8018bba:	4098      	lsls	r0, r3
 8018bbc:	4310      	orrs	r0, r2
 8018bbe:	6088      	str	r0, [r1, #8]
 8018bc0:	bd10      	pop	{r4, pc}

08018bc2 <GPIO_ETH_MediaInterfaceConfig>:
 8018bc2:	4b01      	ldr	r3, [pc, #4]	; (8018bc8 <GPIO_ETH_MediaInterfaceConfig+0x6>)
 8018bc4:	6018      	str	r0, [r3, #0]
 8018bc6:	4770      	bx	lr
 8018bc8:	422000dc 	.word	0x422000dc

08018bcc <RCC_DeInit>:
 8018bcc:	4b0d      	ldr	r3, [pc, #52]	; (8018c04 <RCC_DeInit+0x38>)
 8018bce:	681a      	ldr	r2, [r3, #0]
 8018bd0:	f042 0201 	orr.w	r2, r2, #1
 8018bd4:	601a      	str	r2, [r3, #0]
 8018bd6:	6859      	ldr	r1, [r3, #4]
 8018bd8:	4a0b      	ldr	r2, [pc, #44]	; (8018c08 <RCC_DeInit+0x3c>)
 8018bda:	400a      	ands	r2, r1
 8018bdc:	605a      	str	r2, [r3, #4]
 8018bde:	681a      	ldr	r2, [r3, #0]
 8018be0:	f022 7284 	bic.w	r2, r2, #17301504	; 0x1080000
 8018be4:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
 8018be8:	601a      	str	r2, [r3, #0]
 8018bea:	681a      	ldr	r2, [r3, #0]
 8018bec:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
 8018bf0:	601a      	str	r2, [r3, #0]
 8018bf2:	685a      	ldr	r2, [r3, #4]
 8018bf4:	f422 02fe 	bic.w	r2, r2, #8323072	; 0x7f0000
 8018bf8:	605a      	str	r2, [r3, #4]
 8018bfa:	f44f 021f 	mov.w	r2, #10420224	; 0x9f0000
 8018bfe:	609a      	str	r2, [r3, #8]
 8018c00:	4770      	bx	lr
 8018c02:	bf00      	nop
 8018c04:	40021000 	.word	0x40021000
 8018c08:	f8ff0000 	.word	0xf8ff0000

08018c0c <RCC_HSEConfig>:
 8018c0c:	4b0c      	ldr	r3, [pc, #48]	; (8018c40 <RCC_HSEConfig+0x34>)
 8018c0e:	f5b0 3f80 	cmp.w	r0, #65536	; 0x10000
 8018c12:	681a      	ldr	r2, [r3, #0]
 8018c14:	f422 3280 	bic.w	r2, r2, #65536	; 0x10000
 8018c18:	601a      	str	r2, [r3, #0]
 8018c1a:	681a      	ldr	r2, [r3, #0]
 8018c1c:	f422 2280 	bic.w	r2, r2, #262144	; 0x40000
 8018c20:	601a      	str	r2, [r3, #0]
 8018c22:	d003      	beq.n	8018c2c <RCC_HSEConfig+0x20>
 8018c24:	f5b0 2f80 	cmp.w	r0, #262144	; 0x40000
 8018c28:	d005      	beq.n	8018c36 <RCC_HSEConfig+0x2a>
 8018c2a:	4770      	bx	lr
 8018c2c:	681a      	ldr	r2, [r3, #0]
 8018c2e:	f442 3280 	orr.w	r2, r2, #65536	; 0x10000
 8018c32:	601a      	str	r2, [r3, #0]
 8018c34:	4770      	bx	lr
 8018c36:	681a      	ldr	r2, [r3, #0]
 8018c38:	f442 22a0 	orr.w	r2, r2, #327680	; 0x50000
 8018c3c:	e7f9      	b.n	8018c32 <RCC_HSEConfig+0x26>
 8018c3e:	bf00      	nop
 8018c40:	40021000 	.word	0x40021000

08018c44 <RCC_AdjustHSICalibrationValue>:
 8018c44:	4a03      	ldr	r2, [pc, #12]	; (8018c54 <RCC_AdjustHSICalibrationValue+0x10>)
 8018c46:	6813      	ldr	r3, [r2, #0]
 8018c48:	f023 03f8 	bic.w	r3, r3, #248	; 0xf8
 8018c4c:	ea43 00c0 	orr.w	r0, r3, r0, lsl #3
 8018c50:	6010      	str	r0, [r2, #0]
 8018c52:	4770      	bx	lr
 8018c54:	40021000 	.word	0x40021000

08018c58 <RCC_HSICmd>:
 8018c58:	4b01      	ldr	r3, [pc, #4]	; (8018c60 <RCC_HSICmd+0x8>)
 8018c5a:	6018      	str	r0, [r3, #0]
 8018c5c:	4770      	bx	lr
 8018c5e:	bf00      	nop
 8018c60:	42420000 	.word	0x42420000

08018c64 <RCC_PLLConfig>:
 8018c64:	4a03      	ldr	r2, [pc, #12]	; (8018c74 <RCC_PLLConfig+0x10>)
 8018c66:	6853      	ldr	r3, [r2, #4]
 8018c68:	f423 137c 	bic.w	r3, r3, #4128768	; 0x3f0000
 8018c6c:	4319      	orrs	r1, r3
 8018c6e:	4301      	orrs	r1, r0
 8018c70:	6051      	str	r1, [r2, #4]
 8018c72:	4770      	bx	lr
 8018c74:	40021000 	.word	0x40021000

08018c78 <RCC_PLLCmd>:
 8018c78:	4b01      	ldr	r3, [pc, #4]	; (8018c80 <RCC_PLLCmd+0x8>)
 8018c7a:	6018      	str	r0, [r3, #0]
 8018c7c:	4770      	bx	lr
 8018c7e:	bf00      	nop
 8018c80:	42420060 	.word	0x42420060

08018c84 <RCC_SYSCLKConfig>:
 8018c84:	4a03      	ldr	r2, [pc, #12]	; (8018c94 <RCC_SYSCLKConfig+0x10>)
 8018c86:	6853      	ldr	r3, [r2, #4]
 8018c88:	f023 0303 	bic.w	r3, r3, #3
 8018c8c:	4318      	orrs	r0, r3
 8018c8e:	6050      	str	r0, [r2, #4]
 8018c90:	4770      	bx	lr
 8018c92:	bf00      	nop
 8018c94:	40021000 	.word	0x40021000

08018c98 <RCC_GetSYSCLKSource>:
 8018c98:	4b02      	ldr	r3, [pc, #8]	; (8018ca4 <RCC_GetSYSCLKSource+0xc>)
 8018c9a:	6858      	ldr	r0, [r3, #4]
 8018c9c:	f000 000c 	and.w	r0, r0, #12
 8018ca0:	4770      	bx	lr
 8018ca2:	bf00      	nop
 8018ca4:	40021000 	.word	0x40021000

08018ca8 <RCC_HCLKConfig>:
 8018ca8:	4a03      	ldr	r2, [pc, #12]	; (8018cb8 <RCC_HCLKConfig+0x10>)
 8018caa:	6853      	ldr	r3, [r2, #4]
 8018cac:	f023 03f0 	bic.w	r3, r3, #240	; 0xf0
 8018cb0:	4318      	orrs	r0, r3
 8018cb2:	6050      	str	r0, [r2, #4]
 8018cb4:	4770      	bx	lr
 8018cb6:	bf00      	nop
 8018cb8:	40021000 	.word	0x40021000

08018cbc <RCC_PCLK1Config>:
 8018cbc:	4a03      	ldr	r2, [pc, #12]	; (8018ccc <RCC_PCLK1Config+0x10>)
 8018cbe:	6853      	ldr	r3, [r2, #4]
 8018cc0:	f423 63e0 	bic.w	r3, r3, #1792	; 0x700
 8018cc4:	4318      	orrs	r0, r3
 8018cc6:	6050      	str	r0, [r2, #4]
 8018cc8:	4770      	bx	lr
 8018cca:	bf00      	nop
 8018ccc:	40021000 	.word	0x40021000

08018cd0 <RCC_PCLK2Config>:
 8018cd0:	4a03      	ldr	r2, [pc, #12]	; (8018ce0 <RCC_PCLK2Config+0x10>)
 8018cd2:	6853      	ldr	r3, [r2, #4]
 8018cd4:	f423 5360 	bic.w	r3, r3, #14336	; 0x3800
 8018cd8:	ea43 00c0 	orr.w	r0, r3, r0, lsl #3
 8018cdc:	6050      	str	r0, [r2, #4]
 8018cde:	4770      	bx	lr
 8018ce0:	40021000 	.word	0x40021000

08018ce4 <RCC_ITConfig>:
 8018ce4:	4b04      	ldr	r3, [pc, #16]	; (8018cf8 <RCC_ITConfig+0x14>)
 8018ce6:	781a      	ldrb	r2, [r3, #0]
 8018ce8:	b111      	cbz	r1, 8018cf0 <RCC_ITConfig+0xc>
 8018cea:	4310      	orrs	r0, r2
 8018cec:	7018      	strb	r0, [r3, #0]
 8018cee:	4770      	bx	lr
 8018cf0:	ea22 0000 	bic.w	r0, r2, r0
 8018cf4:	e7fa      	b.n	8018cec <RCC_ITConfig+0x8>
 8018cf6:	bf00      	nop
 8018cf8:	40021009 	.word	0x40021009

08018cfc <RCC_USBCLKConfig>:
 8018cfc:	4b01      	ldr	r3, [pc, #4]	; (8018d04 <RCC_USBCLKConfig+0x8>)
 8018cfe:	6018      	str	r0, [r3, #0]
 8018d00:	4770      	bx	lr
 8018d02:	bf00      	nop
 8018d04:	424200d8 	.word	0x424200d8

08018d08 <RCC_ADCCLKConfig>:
 8018d08:	4a03      	ldr	r2, [pc, #12]	; (8018d18 <RCC_ADCCLKConfig+0x10>)
 8018d0a:	6853      	ldr	r3, [r2, #4]
 8018d0c:	f423 4340 	bic.w	r3, r3, #49152	; 0xc000
 8018d10:	4318      	orrs	r0, r3
 8018d12:	6050      	str	r0, [r2, #4]
 8018d14:	4770      	bx	lr
 8018d16:	bf00      	nop
 8018d18:	40021000 	.word	0x40021000

08018d1c <RCC_LSEConfig>:
 8018d1c:	2200      	movs	r2, #0
 8018d1e:	4b06      	ldr	r3, [pc, #24]	; (8018d38 <RCC_LSEConfig+0x1c>)
 8018d20:	2801      	cmp	r0, #1
 8018d22:	701a      	strb	r2, [r3, #0]
 8018d24:	701a      	strb	r2, [r3, #0]
 8018d26:	d002      	beq.n	8018d2e <RCC_LSEConfig+0x12>
 8018d28:	2804      	cmp	r0, #4
 8018d2a:	d002      	beq.n	8018d32 <RCC_LSEConfig+0x16>
 8018d2c:	4770      	bx	lr
 8018d2e:	7018      	strb	r0, [r3, #0]
 8018d30:	4770      	bx	lr
 8018d32:	2205      	movs	r2, #5
 8018d34:	701a      	strb	r2, [r3, #0]
 8018d36:	4770      	bx	lr
 8018d38:	40021020 	.word	0x40021020

08018d3c <RCC_LSICmd>:
 8018d3c:	4b01      	ldr	r3, [pc, #4]	; (8018d44 <RCC_LSICmd+0x8>)
 8018d3e:	6018      	str	r0, [r3, #0]
 8018d40:	4770      	bx	lr
 8018d42:	bf00      	nop
 8018d44:	42420480 	.word	0x42420480

08018d48 <RCC_RTCCLKConfig>:
 8018d48:	4a02      	ldr	r2, [pc, #8]	; (8018d54 <RCC_RTCCLKConfig+0xc>)
 8018d4a:	6a13      	ldr	r3, [r2, #32]
 8018d4c:	4318      	orrs	r0, r3
 8018d4e:	6210      	str	r0, [r2, #32]
 8018d50:	4770      	bx	lr
 8018d52:	bf00      	nop
 8018d54:	40021000 	.word	0x40021000

08018d58 <RCC_RTCCLKCmd>:
 8018d58:	4b01      	ldr	r3, [pc, #4]	; (8018d60 <RCC_RTCCLKCmd+0x8>)
 8018d5a:	6018      	str	r0, [r3, #0]
 8018d5c:	4770      	bx	lr
 8018d5e:	bf00      	nop
 8018d60:	4242043c 	.word	0x4242043c

08018d64 <RCC_GetClocksFreq>:
 8018d64:	4a1b      	ldr	r2, [pc, #108]	; (8018dd4 <RCC_GetClocksFreq+0x70>)
 8018d66:	b510      	push	{r4, lr}
 8018d68:	6853      	ldr	r3, [r2, #4]
 8018d6a:	f003 030c 	and.w	r3, r3, #12
 8018d6e:	2b08      	cmp	r3, #8
 8018d70:	d01f      	beq.n	8018db2 <RCC_GetClocksFreq+0x4e>
 8018d72:	4b19      	ldr	r3, [pc, #100]	; (8018dd8 <RCC_GetClocksFreq+0x74>)
 8018d74:	6003      	str	r3, [r0, #0]
 8018d76:	6853      	ldr	r3, [r2, #4]
 8018d78:	4918      	ldr	r1, [pc, #96]	; (8018ddc <RCC_GetClocksFreq+0x78>)
 8018d7a:	f3c3 1303 	ubfx	r3, r3, #4, #4
 8018d7e:	5ccc      	ldrb	r4, [r1, r3]
 8018d80:	6803      	ldr	r3, [r0, #0]
 8018d82:	40e3      	lsrs	r3, r4
 8018d84:	6043      	str	r3, [r0, #4]
 8018d86:	6854      	ldr	r4, [r2, #4]
 8018d88:	f3c4 2402 	ubfx	r4, r4, #8, #3
 8018d8c:	5d0c      	ldrb	r4, [r1, r4]
 8018d8e:	fa23 f404 	lsr.w	r4, r3, r4
 8018d92:	6084      	str	r4, [r0, #8]
 8018d94:	6854      	ldr	r4, [r2, #4]
 8018d96:	f3c4 24c2 	ubfx	r4, r4, #11, #3
 8018d9a:	5d0c      	ldrb	r4, [r1, r4]
 8018d9c:	40e3      	lsrs	r3, r4
 8018d9e:	60c3      	str	r3, [r0, #12]
 8018da0:	6852      	ldr	r2, [r2, #4]
 8018da2:	f3c2 3281 	ubfx	r2, r2, #14, #2
 8018da6:	4411      	add	r1, r2
 8018da8:	7c0a      	ldrb	r2, [r1, #16]
 8018daa:	fbb3 f3f2 	udiv	r3, r3, r2
 8018dae:	6103      	str	r3, [r0, #16]
 8018db0:	bd10      	pop	{r4, pc}
 8018db2:	6853      	ldr	r3, [r2, #4]
 8018db4:	6851      	ldr	r1, [r2, #4]
 8018db6:	f3c3 4383 	ubfx	r3, r3, #18, #4
 8018dba:	03cc      	lsls	r4, r1, #15
 8018dbc:	f103 0302 	add.w	r3, r3, #2
 8018dc0:	d402      	bmi.n	8018dc8 <RCC_GetClocksFreq+0x64>
 8018dc2:	4907      	ldr	r1, [pc, #28]	; (8018de0 <RCC_GetClocksFreq+0x7c>)
 8018dc4:	434b      	muls	r3, r1
 8018dc6:	e7d5      	b.n	8018d74 <RCC_GetClocksFreq+0x10>
 8018dc8:	6851      	ldr	r1, [r2, #4]
 8018dca:	0389      	lsls	r1, r1, #14
 8018dcc:	d4f9      	bmi.n	8018dc2 <RCC_GetClocksFreq+0x5e>
 8018dce:	4902      	ldr	r1, [pc, #8]	; (8018dd8 <RCC_GetClocksFreq+0x74>)
 8018dd0:	e7f8      	b.n	8018dc4 <RCC_GetClocksFreq+0x60>
 8018dd2:	bf00      	nop
 8018dd4:	40021000 	.word	0x40021000
 8018dd8:	007a1200 	.word	0x007a1200
 8018ddc:	20005000 	.word	0x20005000
 8018de0:	003d0900 	.word	0x003d0900

08018de4 <RCC_AHBPeriphClockCmd>:
 8018de4:	4b04      	ldr	r3, [pc, #16]	; (8018df8 <RCC_AHBPeriphClockCmd+0x14>)
 8018de6:	695a      	ldr	r2, [r3, #20]
 8018de8:	b111      	cbz	r1, 8018df0 <RCC_AHBPeriphClockCmd+0xc>
 8018dea:	4310      	orrs	r0, r2
 8018dec:	6158      	str	r0, [r3, #20]
 8018dee:	4770      	bx	lr
 8018df0:	ea22 0000 	bic.w	r0, r2, r0
 8018df4:	e7fa      	b.n	8018dec <RCC_AHBPeriphClockCmd+0x8>
 8018df6:	bf00      	nop
 8018df8:	40021000 	.word	0x40021000

08018dfc <RCC_APB2PeriphClockCmd>:
 8018dfc:	4b04      	ldr	r3, [pc, #16]	; (8018e10 <RCC_APB2PeriphClockCmd+0x14>)
 8018dfe:	699a      	ldr	r2, [r3, #24]
 8018e00:	b111      	cbz	r1, 8018e08 <RCC_APB2PeriphClockCmd+0xc>
 8018e02:	4310      	orrs	r0, r2
 8018e04:	6198      	str	r0, [r3, #24]
 8018e06:	4770      	bx	lr
 8018e08:	ea22 0000 	bic.w	r0, r2, r0
 8018e0c:	e7fa      	b.n	8018e04 <RCC_APB2PeriphClockCmd+0x8>
 8018e0e:	bf00      	nop
 8018e10:	40021000 	.word	0x40021000

08018e14 <RCC_APB1PeriphClockCmd>:
 8018e14:	4b04      	ldr	r3, [pc, #16]	; (8018e28 <RCC_APB1PeriphClockCmd+0x14>)
 8018e16:	69da      	ldr	r2, [r3, #28]
 8018e18:	b111      	cbz	r1, 8018e20 <RCC_APB1PeriphClockCmd+0xc>
 8018e1a:	4310      	orrs	r0, r2
 8018e1c:	61d8      	str	r0, [r3, #28]
 8018e1e:	4770      	bx	lr
 8018e20:	ea22 0000 	bic.w	r0, r2, r0
 8018e24:	e7fa      	b.n	8018e1c <RCC_APB1PeriphClockCmd+0x8>
 8018e26:	bf00      	nop
 8018e28:	40021000 	.word	0x40021000

08018e2c <RCC_APB2PeriphResetCmd>:
 8018e2c:	4b04      	ldr	r3, [pc, #16]	; (8018e40 <RCC_APB2PeriphResetCmd+0x14>)
 8018e2e:	68da      	ldr	r2, [r3, #12]
 8018e30:	b111      	cbz	r1, 8018e38 <RCC_APB2PeriphResetCmd+0xc>
 8018e32:	4310      	orrs	r0, r2
 8018e34:	60d8      	str	r0, [r3, #12]
 8018e36:	4770      	bx	lr
 8018e38:	ea22 0000 	bic.w	r0, r2, r0
 8018e3c:	e7fa      	b.n	8018e34 <RCC_APB2PeriphResetCmd+0x8>
 8018e3e:	bf00      	nop
 8018e40:	40021000 	.word	0x40021000

08018e44 <RCC_APB1PeriphResetCmd>:
 8018e44:	4b04      	ldr	r3, [pc, #16]	; (8018e58 <RCC_APB1PeriphResetCmd+0x14>)
 8018e46:	691a      	ldr	r2, [r3, #16]
 8018e48:	b111      	cbz	r1, 8018e50 <RCC_APB1PeriphResetCmd+0xc>
 8018e4a:	4310      	orrs	r0, r2
 8018e4c:	6118      	str	r0, [r3, #16]
 8018e4e:	4770      	bx	lr
 8018e50:	ea22 0000 	bic.w	r0, r2, r0
 8018e54:	e7fa      	b.n	8018e4c <RCC_APB1PeriphResetCmd+0x8>
 8018e56:	bf00      	nop
 8018e58:	40021000 	.word	0x40021000

08018e5c <RCC_BackupResetCmd>:
 8018e5c:	4b01      	ldr	r3, [pc, #4]	; (8018e64 <RCC_BackupResetCmd+0x8>)
 8018e5e:	6018      	str	r0, [r3, #0]
 8018e60:	4770      	bx	lr
 8018e62:	bf00      	nop
 8018e64:	42420440 	.word	0x42420440

08018e68 <RCC_ClockSecuritySystemCmd>:
 8018e68:	4b01      	ldr	r3, [pc, #4]	; (8018e70 <RCC_ClockSecuritySystemCmd+0x8>)
 8018e6a:	6018      	str	r0, [r3, #0]
 8018e6c:	4770      	bx	lr
 8018e6e:	bf00      	nop
 8018e70:	4242004c 	.word	0x4242004c

08018e74 <RCC_MCOConfig>:
 8018e74:	4b01      	ldr	r3, [pc, #4]	; (8018e7c <RCC_MCOConfig+0x8>)
 8018e76:	7018      	strb	r0, [r3, #0]
 8018e78:	4770      	bx	lr
 8018e7a:	bf00      	nop
 8018e7c:	40021007 	.word	0x40021007

08018e80 <RCC_GetFlagStatus>:
 8018e80:	0942      	lsrs	r2, r0, #5
 8018e82:	2a01      	cmp	r2, #1
 8018e84:	4b07      	ldr	r3, [pc, #28]	; (8018ea4 <RCC_GetFlagStatus+0x24>)
 8018e86:	d107      	bne.n	8018e98 <RCC_GetFlagStatus+0x18>
 8018e88:	681b      	ldr	r3, [r3, #0]
 8018e8a:	f000 001f 	and.w	r0, r0, #31
 8018e8e:	fa23 f000 	lsr.w	r0, r3, r0
 8018e92:	f000 0001 	and.w	r0, r0, #1
 8018e96:	4770      	bx	lr
 8018e98:	2a02      	cmp	r2, #2
 8018e9a:	bf0c      	ite	eq
 8018e9c:	6a1b      	ldreq	r3, [r3, #32]
 8018e9e:	6a5b      	ldrne	r3, [r3, #36]	; 0x24
 8018ea0:	e7f3      	b.n	8018e8a <RCC_GetFlagStatus+0xa>
 8018ea2:	bf00      	nop
 8018ea4:	40021000 	.word	0x40021000

08018ea8 <RCC_WaitForHSEStartUp>:
 8018ea8:	2300      	movs	r3, #0
 8018eaa:	b507      	push	{r0, r1, r2, lr}
 8018eac:	9301      	str	r3, [sp, #4]
 8018eae:	2031      	movs	r0, #49	; 0x31
 8018eb0:	f7ff ffe6 	bl	8018e80 <RCC_GetFlagStatus>
 8018eb4:	9b01      	ldr	r3, [sp, #4]
 8018eb6:	3301      	adds	r3, #1
 8018eb8:	9301      	str	r3, [sp, #4]
 8018eba:	9b01      	ldr	r3, [sp, #4]
 8018ebc:	f5b3 6fa0 	cmp.w	r3, #1280	; 0x500
 8018ec0:	d001      	beq.n	8018ec6 <RCC_WaitForHSEStartUp+0x1e>
 8018ec2:	2800      	cmp	r0, #0
 8018ec4:	d0f3      	beq.n	8018eae <RCC_WaitForHSEStartUp+0x6>
 8018ec6:	2031      	movs	r0, #49	; 0x31
 8018ec8:	f7ff ffda 	bl	8018e80 <RCC_GetFlagStatus>
 8018ecc:	3000      	adds	r0, #0
 8018ece:	bf18      	it	ne
 8018ed0:	2001      	movne	r0, #1
 8018ed2:	b003      	add	sp, #12
 8018ed4:	f85d fb04 	ldr.w	pc, [sp], #4

08018ed8 <RCC_ClearFlag>:
 8018ed8:	4a02      	ldr	r2, [pc, #8]	; (8018ee4 <RCC_ClearFlag+0xc>)
 8018eda:	6a53      	ldr	r3, [r2, #36]	; 0x24
 8018edc:	f043 7380 	orr.w	r3, r3, #16777216	; 0x1000000
 8018ee0:	6253      	str	r3, [r2, #36]	; 0x24
 8018ee2:	4770      	bx	lr
 8018ee4:	40021000 	.word	0x40021000

08018ee8 <RCC_GetITStatus>:
 8018ee8:	4b03      	ldr	r3, [pc, #12]	; (8018ef8 <RCC_GetITStatus+0x10>)
 8018eea:	689b      	ldr	r3, [r3, #8]
 8018eec:	4218      	tst	r0, r3
 8018eee:	bf14      	ite	ne
 8018ef0:	2001      	movne	r0, #1
 8018ef2:	2000      	moveq	r0, #0
 8018ef4:	4770      	bx	lr
 8018ef6:	bf00      	nop
 8018ef8:	40021000 	.word	0x40021000

08018efc <RCC_ClearITPendingBit>:
 8018efc:	4b01      	ldr	r3, [pc, #4]	; (8018f04 <RCC_ClearITPendingBit+0x8>)
 8018efe:	7018      	strb	r0, [r3, #0]
 8018f00:	4770      	bx	lr
 8018f02:	bf00      	nop
 8018f04:	4002100a 	.word	0x4002100a

08018f08 <pow>:
 8018f08:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8018f0c:	b08d      	sub	sp, #52	; 0x34
 8018f0e:	4606      	mov	r6, r0
 8018f10:	460f      	mov	r7, r1
 8018f12:	4614      	mov	r4, r2
 8018f14:	461d      	mov	r5, r3
 8018f16:	f000 f993 	bl	8019240 <__ieee754_pow>
 8018f1a:	f8df 9314 	ldr.w	r9, [pc, #788]	; 8019230 <pow+0x328>
 8018f1e:	e9cd 0100 	strd	r0, r1, [sp]
 8018f22:	f999 8000 	ldrsb.w	r8, [r9]
 8018f26:	f1b8 3fff 	cmp.w	r8, #4294967295	; 0xffffffff
 8018f2a:	d036      	beq.n	8018f9a <pow+0x92>
 8018f2c:	4622      	mov	r2, r4
 8018f2e:	462b      	mov	r3, r5
 8018f30:	4620      	mov	r0, r4
 8018f32:	4629      	mov	r1, r5
 8018f34:	f001 fd28 	bl	801a988 <__aeabi_dcmpun>
 8018f38:	4683      	mov	fp, r0
 8018f3a:	bb70      	cbnz	r0, 8018f9a <pow+0x92>
 8018f3c:	4632      	mov	r2, r6
 8018f3e:	463b      	mov	r3, r7
 8018f40:	4630      	mov	r0, r6
 8018f42:	4639      	mov	r1, r7
 8018f44:	f001 fd20 	bl	801a988 <__aeabi_dcmpun>
 8018f48:	4682      	mov	sl, r0
 8018f4a:	2800      	cmp	r0, #0
 8018f4c:	f040 80ad 	bne.w	80190aa <pow+0x1a2>
 8018f50:	4630      	mov	r0, r6
 8018f52:	4639      	mov	r1, r7
 8018f54:	2200      	movs	r2, #0
 8018f56:	2300      	movs	r3, #0
 8018f58:	f001 fce4 	bl	801a924 <__aeabi_dcmpeq>
 8018f5c:	b310      	cbz	r0, 8018fa4 <pow+0x9c>
 8018f5e:	2200      	movs	r2, #0
 8018f60:	2300      	movs	r3, #0
 8018f62:	4620      	mov	r0, r4
 8018f64:	4629      	mov	r1, r5
 8018f66:	f001 fcdd 	bl	801a924 <__aeabi_dcmpeq>
 8018f6a:	4683      	mov	fp, r0
 8018f6c:	2800      	cmp	r0, #0
 8018f6e:	d069      	beq.n	8019044 <pow+0x13c>
 8018f70:	2201      	movs	r2, #1
 8018f72:	2000      	movs	r0, #0
 8018f74:	2100      	movs	r1, #0
 8018f76:	4ba9      	ldr	r3, [pc, #676]	; (801921c <pow+0x314>)
 8018f78:	f8cd a028 	str.w	sl, [sp, #40]	; 0x28
 8018f7c:	e9cd 6704 	strd	r6, r7, [sp, #16]
 8018f80:	e9cd 4506 	strd	r4, r5, [sp, #24]
 8018f84:	e9cd 0108 	strd	r0, r1, [sp, #32]
 8018f88:	e9cd 2302 	strd	r2, r3, [sp, #8]
 8018f8c:	f1b8 0f00 	cmp.w	r8, #0
 8018f90:	d042      	beq.n	8019018 <pow+0x110>
 8018f92:	2300      	movs	r3, #0
 8018f94:	4ca2      	ldr	r4, [pc, #648]	; (8019220 <pow+0x318>)
 8018f96:	e9cd 3400 	strd	r3, r4, [sp]
 8018f9a:	e9dd 0100 	ldrd	r0, r1, [sp]
 8018f9e:	b00d      	add	sp, #52	; 0x34
 8018fa0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8018fa4:	e9dd 0100 	ldrd	r0, r1, [sp]
 8018fa8:	f000 ff82 	bl	8019eb0 <finite>
 8018fac:	4680      	mov	r8, r0
 8018fae:	2800      	cmp	r0, #0
 8018fb0:	f000 8098 	beq.w	80190e4 <pow+0x1dc>
 8018fb4:	e9dd 0100 	ldrd	r0, r1, [sp]
 8018fb8:	f04f 0a00 	mov.w	sl, #0
 8018fbc:	f04f 0b00 	mov.w	fp, #0
 8018fc0:	4652      	mov	r2, sl
 8018fc2:	465b      	mov	r3, fp
 8018fc4:	f001 fcae 	bl	801a924 <__aeabi_dcmpeq>
 8018fc8:	2800      	cmp	r0, #0
 8018fca:	d0e6      	beq.n	8018f9a <pow+0x92>
 8018fcc:	4630      	mov	r0, r6
 8018fce:	4639      	mov	r1, r7
 8018fd0:	f000 ff6e 	bl	8019eb0 <finite>
 8018fd4:	2800      	cmp	r0, #0
 8018fd6:	d0e0      	beq.n	8018f9a <pow+0x92>
 8018fd8:	4620      	mov	r0, r4
 8018fda:	4629      	mov	r1, r5
 8018fdc:	f000 ff68 	bl	8019eb0 <finite>
 8018fe0:	2800      	cmp	r0, #0
 8018fe2:	d0da      	beq.n	8018f9a <pow+0x92>
 8018fe4:	2004      	movs	r0, #4
 8018fe6:	2100      	movs	r1, #0
 8018fe8:	f999 3000 	ldrsb.w	r3, [r9]
 8018fec:	4a8b      	ldr	r2, [pc, #556]	; (801921c <pow+0x314>)
 8018fee:	2b02      	cmp	r3, #2
 8018ff0:	e9cd 6704 	strd	r6, r7, [sp, #16]
 8018ff4:	e9cd 4506 	strd	r4, r5, [sp, #24]
 8018ff8:	e9cd ab08 	strd	sl, fp, [sp, #32]
 8018ffc:	9002      	str	r0, [sp, #8]
 8018ffe:	910a      	str	r1, [sp, #40]	; 0x28
 8019000:	9203      	str	r2, [sp, #12]
 8019002:	d004      	beq.n	801900e <pow+0x106>
 8019004:	a802      	add	r0, sp, #8
 8019006:	f000 ff59 	bl	8019ebc <matherr>
 801900a:	2800      	cmp	r0, #0
 801900c:	d142      	bne.n	8019094 <pow+0x18c>
 801900e:	f001 ffa7 	bl	801af60 <__errno>
 8019012:	2322      	movs	r3, #34	; 0x22
 8019014:	6003      	str	r3, [r0, #0]
 8019016:	e03d      	b.n	8019094 <pow+0x18c>
 8019018:	a802      	add	r0, sp, #8
 801901a:	f000 ff4f 	bl	8019ebc <matherr>
 801901e:	2800      	cmp	r0, #0
 8019020:	f000 80b3 	beq.w	801918a <pow+0x282>
 8019024:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 8019026:	2b00      	cmp	r3, #0
 8019028:	d03a      	beq.n	80190a0 <pow+0x198>
 801902a:	f001 ff99 	bl	801af60 <__errno>
 801902e:	e9dd 3408 	ldrd	r3, r4, [sp, #32]
 8019032:	e9cd 3400 	strd	r3, r4, [sp]
 8019036:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 8019038:	6003      	str	r3, [r0, #0]
 801903a:	e9dd 0100 	ldrd	r0, r1, [sp]
 801903e:	b00d      	add	sp, #52	; 0x34
 8019040:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8019044:	4620      	mov	r0, r4
 8019046:	4629      	mov	r1, r5
 8019048:	f000 ff32 	bl	8019eb0 <finite>
 801904c:	2800      	cmp	r0, #0
 801904e:	d0a4      	beq.n	8018f9a <pow+0x92>
 8019050:	2200      	movs	r2, #0
 8019052:	2300      	movs	r3, #0
 8019054:	4620      	mov	r0, r4
 8019056:	4629      	mov	r1, r5
 8019058:	f001 fc6e 	bl	801a938 <__aeabi_dcmplt>
 801905c:	2800      	cmp	r0, #0
 801905e:	d09c      	beq.n	8018f9a <pow+0x92>
 8019060:	2101      	movs	r1, #1
 8019062:	4a6e      	ldr	r2, [pc, #440]	; (801921c <pow+0x314>)
 8019064:	f999 3000 	ldrsb.w	r3, [r9]
 8019068:	f8cd b028 	str.w	fp, [sp, #40]	; 0x28
 801906c:	e9cd 6704 	strd	r6, r7, [sp, #16]
 8019070:	e9cd 4506 	strd	r4, r5, [sp, #24]
 8019074:	e9cd 1202 	strd	r1, r2, [sp, #8]
 8019078:	2b00      	cmp	r3, #0
 801907a:	f000 808b 	beq.w	8019194 <pow+0x28c>
 801907e:	2000      	movs	r0, #0
 8019080:	4968      	ldr	r1, [pc, #416]	; (8019224 <pow+0x31c>)
 8019082:	2b02      	cmp	r3, #2
 8019084:	e9cd 0108 	strd	r0, r1, [sp, #32]
 8019088:	f040 8088 	bne.w	801919c <pow+0x294>
 801908c:	f001 ff68 	bl	801af60 <__errno>
 8019090:	2321      	movs	r3, #33	; 0x21
 8019092:	6003      	str	r3, [r0, #0]
 8019094:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 8019096:	b11b      	cbz	r3, 80190a0 <pow+0x198>
 8019098:	f001 ff62 	bl	801af60 <__errno>
 801909c:	9b0a      	ldr	r3, [sp, #40]	; 0x28
 801909e:	6003      	str	r3, [r0, #0]
 80190a0:	e9dd 3408 	ldrd	r3, r4, [sp, #32]
 80190a4:	e9cd 3400 	strd	r3, r4, [sp]
 80190a8:	e777      	b.n	8018f9a <pow+0x92>
 80190aa:	2200      	movs	r2, #0
 80190ac:	2300      	movs	r3, #0
 80190ae:	4620      	mov	r0, r4
 80190b0:	4629      	mov	r1, r5
 80190b2:	f001 fc37 	bl	801a924 <__aeabi_dcmpeq>
 80190b6:	2800      	cmp	r0, #0
 80190b8:	f43f af6f 	beq.w	8018f9a <pow+0x92>
 80190bc:	2001      	movs	r0, #1
 80190be:	2200      	movs	r2, #0
 80190c0:	4b57      	ldr	r3, [pc, #348]	; (8019220 <pow+0x318>)
 80190c2:	4956      	ldr	r1, [pc, #344]	; (801921c <pow+0x314>)
 80190c4:	f1b8 0f02 	cmp.w	r8, #2
 80190c8:	f8cd b028 	str.w	fp, [sp, #40]	; 0x28
 80190cc:	e9cd 6704 	strd	r6, r7, [sp, #16]
 80190d0:	e9cd 4506 	strd	r4, r5, [sp, #24]
 80190d4:	9002      	str	r0, [sp, #8]
 80190d6:	e9cd 2308 	strd	r2, r3, [sp, #32]
 80190da:	9103      	str	r1, [sp, #12]
 80190dc:	d19c      	bne.n	8019018 <pow+0x110>
 80190de:	e9cd 2300 	strd	r2, r3, [sp]
 80190e2:	e75a      	b.n	8018f9a <pow+0x92>
 80190e4:	4630      	mov	r0, r6
 80190e6:	4639      	mov	r1, r7
 80190e8:	f000 fee2 	bl	8019eb0 <finite>
 80190ec:	2800      	cmp	r0, #0
 80190ee:	f43f af61 	beq.w	8018fb4 <pow+0xac>
 80190f2:	4620      	mov	r0, r4
 80190f4:	4629      	mov	r1, r5
 80190f6:	f000 fedb 	bl	8019eb0 <finite>
 80190fa:	2800      	cmp	r0, #0
 80190fc:	f43f af5a 	beq.w	8018fb4 <pow+0xac>
 8019100:	e9dd 2300 	ldrd	r2, r3, [sp]
 8019104:	4610      	mov	r0, r2
 8019106:	4619      	mov	r1, r3
 8019108:	f001 fc3e 	bl	801a988 <__aeabi_dcmpun>
 801910c:	f999 a000 	ldrsb.w	sl, [r9]
 8019110:	2800      	cmp	r0, #0
 8019112:	d168      	bne.n	80191e6 <pow+0x2de>
 8019114:	2303      	movs	r3, #3
 8019116:	f8df c104 	ldr.w	ip, [pc, #260]	; 801921c <pow+0x314>
 801911a:	900a      	str	r0, [sp, #40]	; 0x28
 801911c:	4629      	mov	r1, r5
 801911e:	4620      	mov	r0, r4
 8019120:	9302      	str	r3, [sp, #8]
 8019122:	2200      	movs	r2, #0
 8019124:	4b40      	ldr	r3, [pc, #256]	; (8019228 <pow+0x320>)
 8019126:	e9cd 4506 	strd	r4, r5, [sp, #24]
 801912a:	e9cd 6704 	strd	r6, r7, [sp, #16]
 801912e:	f8cd c00c 	str.w	ip, [sp, #12]
 8019132:	f001 f98f 	bl	801a454 <__aeabi_dmul>
 8019136:	4604      	mov	r4, r0
 8019138:	460d      	mov	r5, r1
 801913a:	f1ba 0f00 	cmp.w	sl, #0
 801913e:	d134      	bne.n	80191aa <pow+0x2a2>
 8019140:	f04f 4a60 	mov.w	sl, #3758096384	; 0xe0000000
 8019144:	f8df b0ec 	ldr.w	fp, [pc, #236]	; 8019234 <pow+0x32c>
 8019148:	2200      	movs	r2, #0
 801914a:	2300      	movs	r3, #0
 801914c:	4630      	mov	r0, r6
 801914e:	4639      	mov	r1, r7
 8019150:	e9cd ab08 	strd	sl, fp, [sp, #32]
 8019154:	f001 fbf0 	bl	801a938 <__aeabi_dcmplt>
 8019158:	2800      	cmp	r0, #0
 801915a:	f43f af53 	beq.w	8019004 <pow+0xfc>
 801915e:	4620      	mov	r0, r4
 8019160:	4629      	mov	r1, r5
 8019162:	f000 feb3 	bl	8019ecc <rint>
 8019166:	4622      	mov	r2, r4
 8019168:	462b      	mov	r3, r5
 801916a:	f001 fbdb 	bl	801a924 <__aeabi_dcmpeq>
 801916e:	2800      	cmp	r0, #0
 8019170:	d136      	bne.n	80191e0 <pow+0x2d8>
 8019172:	f04f 4260 	mov.w	r2, #3758096384	; 0xe0000000
 8019176:	4b2d      	ldr	r3, [pc, #180]	; (801922c <pow+0x324>)
 8019178:	f999 a000 	ldrsb.w	sl, [r9]
 801917c:	e9cd 2308 	strd	r2, r3, [sp, #32]
 8019180:	f1ba 0f02 	cmp.w	sl, #2
 8019184:	f43f af43 	beq.w	801900e <pow+0x106>
 8019188:	e73c      	b.n	8019004 <pow+0xfc>
 801918a:	f001 fee9 	bl	801af60 <__errno>
 801918e:	2321      	movs	r3, #33	; 0x21
 8019190:	6003      	str	r3, [r0, #0]
 8019192:	e747      	b.n	8019024 <pow+0x11c>
 8019194:	2300      	movs	r3, #0
 8019196:	2400      	movs	r4, #0
 8019198:	e9cd 3408 	strd	r3, r4, [sp, #32]
 801919c:	a802      	add	r0, sp, #8
 801919e:	f000 fe8d 	bl	8019ebc <matherr>
 80191a2:	2800      	cmp	r0, #0
 80191a4:	f47f af76 	bne.w	8019094 <pow+0x18c>
 80191a8:	e770      	b.n	801908c <pow+0x184>
 80191aa:	f04f 0b00 	mov.w	fp, #0
 80191ae:	f8df c088 	ldr.w	ip, [pc, #136]	; 8019238 <pow+0x330>
 80191b2:	2200      	movs	r2, #0
 80191b4:	2300      	movs	r3, #0
 80191b6:	4630      	mov	r0, r6
 80191b8:	4639      	mov	r1, r7
 80191ba:	e9cd bc08 	strd	fp, ip, [sp, #32]
 80191be:	f001 fbbb 	bl	801a938 <__aeabi_dcmplt>
 80191c2:	2800      	cmp	r0, #0
 80191c4:	d0dc      	beq.n	8019180 <pow+0x278>
 80191c6:	4620      	mov	r0, r4
 80191c8:	4629      	mov	r1, r5
 80191ca:	f000 fe7f 	bl	8019ecc <rint>
 80191ce:	4622      	mov	r2, r4
 80191d0:	462b      	mov	r3, r5
 80191d2:	f001 fba7 	bl	801a924 <__aeabi_dcmpeq>
 80191d6:	b918      	cbnz	r0, 80191e0 <pow+0x2d8>
 80191d8:	2200      	movs	r2, #0
 80191da:	4b12      	ldr	r3, [pc, #72]	; (8019224 <pow+0x31c>)
 80191dc:	e9cd 2308 	strd	r2, r3, [sp, #32]
 80191e0:	f999 a000 	ldrsb.w	sl, [r9]
 80191e4:	e7cc      	b.n	8019180 <pow+0x278>
 80191e6:	2201      	movs	r2, #1
 80191e8:	4b0c      	ldr	r3, [pc, #48]	; (801921c <pow+0x314>)
 80191ea:	f8cd 8028 	str.w	r8, [sp, #40]	; 0x28
 80191ee:	e9cd 6704 	strd	r6, r7, [sp, #16]
 80191f2:	e9cd 4506 	strd	r4, r5, [sp, #24]
 80191f6:	e9cd 2302 	strd	r2, r3, [sp, #8]
 80191fa:	f1ba 0f00 	cmp.w	sl, #0
 80191fe:	d0c9      	beq.n	8019194 <pow+0x28c>
 8019200:	2200      	movs	r2, #0
 8019202:	2300      	movs	r3, #0
 8019204:	4610      	mov	r0, r2
 8019206:	4619      	mov	r1, r3
 8019208:	f001 fa4e 	bl	801a6a8 <__aeabi_ddiv>
 801920c:	f1ba 0f02 	cmp.w	sl, #2
 8019210:	e9cd 0108 	strd	r0, r1, [sp, #32]
 8019214:	f43f af3a 	beq.w	801908c <pow+0x184>
 8019218:	e7c0      	b.n	801919c <pow+0x294>
 801921a:	bf00      	nop
 801921c:	0801af8c 	.word	0x0801af8c
 8019220:	3ff00000 	.word	0x3ff00000
 8019224:	fff00000 	.word	0xfff00000
 8019228:	3fe00000 	.word	0x3fe00000
 801922c:	c7efffff 	.word	0xc7efffff
 8019230:	200050b0 	.word	0x200050b0
 8019234:	47efffff 	.word	0x47efffff
 8019238:	7ff00000 	.word	0x7ff00000
 801923c:	00000000 	.word	0x00000000

08019240 <__ieee754_pow>:
 8019240:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8019244:	f023 4500 	bic.w	r5, r3, #2147483648	; 0x80000000
 8019248:	ea55 0402 	orrs.w	r4, r5, r2
 801924c:	b093      	sub	sp, #76	; 0x4c
 801924e:	d00d      	beq.n	801926c <__ieee754_pow+0x2c>
 8019250:	4e79      	ldr	r6, [pc, #484]	; (8019438 <__ieee754_pow+0x1f8>)
 8019252:	f021 4400 	bic.w	r4, r1, #2147483648	; 0x80000000
 8019256:	42b4      	cmp	r4, r6
 8019258:	460f      	mov	r7, r1
 801925a:	4682      	mov	sl, r0
 801925c:	dd0f      	ble.n	801927e <__ieee754_pow+0x3e>
 801925e:	f104 4440 	add.w	r4, r4, #3221225472	; 0xc0000000
 8019262:	f504 1480 	add.w	r4, r4, #1048576	; 0x100000
 8019266:	ea54 030a 	orrs.w	r3, r4, sl
 801926a:	d15b      	bne.n	8019324 <__ieee754_pow+0xe4>
 801926c:	2300      	movs	r3, #0
 801926e:	4c73      	ldr	r4, [pc, #460]	; (801943c <__ieee754_pow+0x1fc>)
 8019270:	e9cd 3400 	strd	r3, r4, [sp]
 8019274:	e9dd 0100 	ldrd	r0, r1, [sp]
 8019278:	b013      	add	sp, #76	; 0x4c
 801927a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 801927e:	4698      	mov	r8, r3
 8019280:	4694      	mov	ip, r2
 8019282:	d04e      	beq.n	8019322 <__ieee754_pow+0xe2>
 8019284:	42b5      	cmp	r5, r6
 8019286:	dcea      	bgt.n	801925e <__ieee754_pow+0x1e>
 8019288:	4e6b      	ldr	r6, [pc, #428]	; (8019438 <__ieee754_pow+0x1f8>)
 801928a:	42b5      	cmp	r5, r6
 801928c:	d066      	beq.n	801935c <__ieee754_pow+0x11c>
 801928e:	2f00      	cmp	r7, #0
 8019290:	468b      	mov	fp, r1
 8019292:	4681      	mov	r9, r0
 8019294:	e9cd 2302 	strd	r2, r3, [sp, #8]
 8019298:	db64      	blt.n	8019364 <__ieee754_pow+0x124>
 801929a:	2600      	movs	r6, #0
 801929c:	f1bc 0f00 	cmp.w	ip, #0
 80192a0:	d10e      	bne.n	80192c0 <__ieee754_pow+0x80>
 80192a2:	4b65      	ldr	r3, [pc, #404]	; (8019438 <__ieee754_pow+0x1f8>)
 80192a4:	429d      	cmp	r5, r3
 80192a6:	d046      	beq.n	8019336 <__ieee754_pow+0xf6>
 80192a8:	4b64      	ldr	r3, [pc, #400]	; (801943c <__ieee754_pow+0x1fc>)
 80192aa:	429d      	cmp	r5, r3
 80192ac:	f000 8097 	beq.w	80193de <__ieee754_pow+0x19e>
 80192b0:	f1b8 4f80 	cmp.w	r8, #1073741824	; 0x40000000
 80192b4:	f000 80b2 	beq.w	801941c <__ieee754_pow+0x1dc>
 80192b8:	4b61      	ldr	r3, [pc, #388]	; (8019440 <__ieee754_pow+0x200>)
 80192ba:	4598      	cmp	r8, r3
 80192bc:	f000 83d2 	beq.w	8019a64 <__ieee754_pow+0x824>
 80192c0:	4648      	mov	r0, r9
 80192c2:	4659      	mov	r1, fp
 80192c4:	f000 fdf0 	bl	8019ea8 <fabs>
 80192c8:	e9cd 0100 	strd	r0, r1, [sp]
 80192cc:	f1ba 0f00 	cmp.w	sl, #0
 80192d0:	d106      	bne.n	80192e0 <__ieee754_pow+0xa0>
 80192d2:	2c00      	cmp	r4, #0
 80192d4:	d060      	beq.n	8019398 <__ieee754_pow+0x158>
 80192d6:	4b59      	ldr	r3, [pc, #356]	; (801943c <__ieee754_pow+0x1fc>)
 80192d8:	f027 4240 	bic.w	r2, r7, #3221225472	; 0xc0000000
 80192dc:	429a      	cmp	r2, r3
 80192de:	d05b      	beq.n	8019398 <__ieee754_pow+0x158>
 80192e0:	0fff      	lsrs	r7, r7, #31
 80192e2:	3f01      	subs	r7, #1
 80192e4:	ea56 0307 	orrs.w	r3, r6, r7
 80192e8:	f000 8082 	beq.w	80193f0 <__ieee754_pow+0x1b0>
 80192ec:	4b55      	ldr	r3, [pc, #340]	; (8019444 <__ieee754_pow+0x204>)
 80192ee:	429d      	cmp	r5, r3
 80192f0:	f340 80b0 	ble.w	8019454 <__ieee754_pow+0x214>
 80192f4:	f103 7304 	add.w	r3, r3, #34603008	; 0x2100000
 80192f8:	429d      	cmp	r5, r3
 80192fa:	f340 844e 	ble.w	8019b9a <__ieee754_pow+0x95a>
 80192fe:	4b52      	ldr	r3, [pc, #328]	; (8019448 <__ieee754_pow+0x208>)
 8019300:	429c      	cmp	r4, r3
 8019302:	f340 8082 	ble.w	801940a <__ieee754_pow+0x1ca>
 8019306:	f1b8 0f00 	cmp.w	r8, #0
 801930a:	f340 8082 	ble.w	8019412 <__ieee754_pow+0x1d2>
 801930e:	a348      	add	r3, pc, #288	; (adr r3, 8019430 <__ieee754_pow+0x1f0>)
 8019310:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019314:	4610      	mov	r0, r2
 8019316:	4619      	mov	r1, r3
 8019318:	f001 f89c 	bl	801a454 <__aeabi_dmul>
 801931c:	e9cd 0100 	strd	r0, r1, [sp]
 8019320:	e7a8      	b.n	8019274 <__ieee754_pow+0x34>
 8019322:	b128      	cbz	r0, 8019330 <__ieee754_pow+0xf0>
 8019324:	4849      	ldr	r0, [pc, #292]	; (801944c <__ieee754_pow+0x20c>)
 8019326:	b013      	add	sp, #76	; 0x4c
 8019328:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 801932c:	f000 bdc8 	b.w	8019ec0 <nan>
 8019330:	42a5      	cmp	r5, r4
 8019332:	dcf7      	bgt.n	8019324 <__ieee754_pow+0xe4>
 8019334:	e7a8      	b.n	8019288 <__ieee754_pow+0x48>
 8019336:	f104 4340 	add.w	r3, r4, #3221225472	; 0xc0000000
 801933a:	f503 1380 	add.w	r3, r3, #1048576	; 0x100000
 801933e:	ea53 030a 	orrs.w	r3, r3, sl
 8019342:	d093      	beq.n	801926c <__ieee754_pow+0x2c>
 8019344:	4b40      	ldr	r3, [pc, #256]	; (8019448 <__ieee754_pow+0x208>)
 8019346:	429c      	cmp	r4, r3
 8019348:	f340 8396 	ble.w	8019a78 <__ieee754_pow+0x838>
 801934c:	f1b8 0f00 	cmp.w	r8, #0
 8019350:	db5f      	blt.n	8019412 <__ieee754_pow+0x1d2>
 8019352:	e9dd 3402 	ldrd	r3, r4, [sp, #8]
 8019356:	e9cd 3400 	strd	r3, r4, [sp]
 801935a:	e78b      	b.n	8019274 <__ieee754_pow+0x34>
 801935c:	f1bc 0f00 	cmp.w	ip, #0
 8019360:	d095      	beq.n	801928e <__ieee754_pow+0x4e>
 8019362:	e77c      	b.n	801925e <__ieee754_pow+0x1e>
 8019364:	4b3a      	ldr	r3, [pc, #232]	; (8019450 <__ieee754_pow+0x210>)
 8019366:	429d      	cmp	r5, r3
 8019368:	dc37      	bgt.n	80193da <__ieee754_pow+0x19a>
 801936a:	f1a3 7354 	sub.w	r3, r3, #55574528	; 0x3500000
 801936e:	429d      	cmp	r5, r3
 8019370:	dd93      	ble.n	801929a <__ieee754_pow+0x5a>
 8019372:	152b      	asrs	r3, r5, #20
 8019374:	f2a3 33ff 	subw	r3, r3, #1023	; 0x3ff
 8019378:	2b14      	cmp	r3, #20
 801937a:	f340 83ed 	ble.w	8019b58 <__ieee754_pow+0x918>
 801937e:	f1c3 0334 	rsb	r3, r3, #52	; 0x34
 8019382:	fa2c f203 	lsr.w	r2, ip, r3
 8019386:	fa02 f303 	lsl.w	r3, r2, r3
 801938a:	4563      	cmp	r3, ip
 801938c:	d185      	bne.n	801929a <__ieee754_pow+0x5a>
 801938e:	f002 0201 	and.w	r2, r2, #1
 8019392:	f1c2 0602 	rsb	r6, r2, #2
 8019396:	e781      	b.n	801929c <__ieee754_pow+0x5c>
 8019398:	f1b8 0f00 	cmp.w	r8, #0
 801939c:	da07      	bge.n	80193ae <__ieee754_pow+0x16e>
 801939e:	e9dd 2300 	ldrd	r2, r3, [sp]
 80193a2:	2000      	movs	r0, #0
 80193a4:	4925      	ldr	r1, [pc, #148]	; (801943c <__ieee754_pow+0x1fc>)
 80193a6:	f001 f97f 	bl	801a6a8 <__aeabi_ddiv>
 80193aa:	e9cd 0100 	strd	r0, r1, [sp]
 80193ae:	2f00      	cmp	r7, #0
 80193b0:	f6bf af60 	bge.w	8019274 <__ieee754_pow+0x34>
 80193b4:	f104 4440 	add.w	r4, r4, #3221225472	; 0xc0000000
 80193b8:	f504 1480 	add.w	r4, r4, #1048576	; 0x100000
 80193bc:	ea54 0306 	orrs.w	r3, r4, r6
 80193c0:	f000 8449 	beq.w	8019c56 <__ieee754_pow+0xa16>
 80193c4:	2e01      	cmp	r6, #1
 80193c6:	f47f af55 	bne.w	8019274 <__ieee754_pow+0x34>
 80193ca:	e9dd 4500 	ldrd	r4, r5, [sp]
 80193ce:	4622      	mov	r2, r4
 80193d0:	f105 4300 	add.w	r3, r5, #2147483648	; 0x80000000
 80193d4:	e9cd 2300 	strd	r2, r3, [sp]
 80193d8:	e74c      	b.n	8019274 <__ieee754_pow+0x34>
 80193da:	2602      	movs	r6, #2
 80193dc:	e75e      	b.n	801929c <__ieee754_pow+0x5c>
 80193de:	f1b8 0f00 	cmp.w	r8, #0
 80193e2:	f2c0 8355 	blt.w	8019a90 <__ieee754_pow+0x850>
 80193e6:	4648      	mov	r0, r9
 80193e8:	4659      	mov	r1, fp
 80193ea:	e9cd 0100 	strd	r0, r1, [sp]
 80193ee:	e741      	b.n	8019274 <__ieee754_pow+0x34>
 80193f0:	464a      	mov	r2, r9
 80193f2:	4648      	mov	r0, r9
 80193f4:	465b      	mov	r3, fp
 80193f6:	4659      	mov	r1, fp
 80193f8:	f000 fe74 	bl	801a0e4 <__aeabi_dsub>
 80193fc:	4602      	mov	r2, r0
 80193fe:	460b      	mov	r3, r1
 8019400:	f001 f952 	bl	801a6a8 <__aeabi_ddiv>
 8019404:	e9cd 0100 	strd	r0, r1, [sp]
 8019408:	e734      	b.n	8019274 <__ieee754_pow+0x34>
 801940a:	f1b8 0f00 	cmp.w	r8, #0
 801940e:	f6ff af7e 	blt.w	801930e <__ieee754_pow+0xce>
 8019412:	2300      	movs	r3, #0
 8019414:	2400      	movs	r4, #0
 8019416:	e9cd 3400 	strd	r3, r4, [sp]
 801941a:	e72b      	b.n	8019274 <__ieee754_pow+0x34>
 801941c:	464a      	mov	r2, r9
 801941e:	4648      	mov	r0, r9
 8019420:	465b      	mov	r3, fp
 8019422:	4659      	mov	r1, fp
 8019424:	f001 f816 	bl	801a454 <__aeabi_dmul>
 8019428:	e9cd 0100 	strd	r0, r1, [sp]
 801942c:	e722      	b.n	8019274 <__ieee754_pow+0x34>
 801942e:	bf00      	nop
 8019430:	8800759c 	.word	0x8800759c
 8019434:	7e37e43c 	.word	0x7e37e43c
 8019438:	7ff00000 	.word	0x7ff00000
 801943c:	3ff00000 	.word	0x3ff00000
 8019440:	3fe00000 	.word	0x3fe00000
 8019444:	41e00000 	.word	0x41e00000
 8019448:	3fefffff 	.word	0x3fefffff
 801944c:	0801af90 	.word	0x0801af90
 8019450:	433fffff 	.word	0x433fffff
 8019454:	f5b4 1f80 	cmp.w	r4, #1048576	; 0x100000
 8019458:	f280 839c 	bge.w	8019b94 <__ieee754_pow+0x954>
 801945c:	e9dd 0100 	ldrd	r0, r1, [sp]
 8019460:	2200      	movs	r2, #0
 8019462:	4bd1      	ldr	r3, [pc, #836]	; (80197a8 <__ieee754_pow+0x568>)
 8019464:	f000 fff6 	bl	801a454 <__aeabi_dmul>
 8019468:	f06f 0c34 	mvn.w	ip, #52	; 0x34
 801946c:	460c      	mov	r4, r1
 801946e:	e9cd 0100 	strd	r0, r1, [sp]
 8019472:	1523      	asrs	r3, r4, #20
 8019474:	4acd      	ldr	r2, [pc, #820]	; (80197ac <__ieee754_pow+0x56c>)
 8019476:	f3c4 0413 	ubfx	r4, r4, #0, #20
 801947a:	f2a3 33ff 	subw	r3, r3, #1023	; 0x3ff
 801947e:	f044 557f 	orr.w	r5, r4, #1069547520	; 0x3fc00000
 8019482:	eb03 010c 	add.w	r1, r3, ip
 8019486:	4294      	cmp	r4, r2
 8019488:	910d      	str	r1, [sp, #52]	; 0x34
 801948a:	f445 1540 	orr.w	r5, r5, #3145728	; 0x300000
 801948e:	f340 8373 	ble.w	8019b78 <__ieee754_pow+0x938>
 8019492:	4bc7      	ldr	r3, [pc, #796]	; (80197b0 <__ieee754_pow+0x570>)
 8019494:	429c      	cmp	r4, r3
 8019496:	f340 8400 	ble.w	8019c9a <__ieee754_pow+0xa5a>
 801949a:	2300      	movs	r3, #0
 801949c:	2400      	movs	r4, #0
 801949e:	2200      	movs	r2, #0
 80194a0:	e9cd 340a 	strd	r3, r4, [sp, #40]	; 0x28
 80194a4:	e9cd 3410 	strd	r3, r4, [sp, #64]	; 0x40
 80194a8:	2400      	movs	r4, #0
 80194aa:	4bc2      	ldr	r3, [pc, #776]	; (80197b4 <__ieee754_pow+0x574>)
 80194ac:	3101      	adds	r1, #1
 80194ae:	e9cd 2306 	strd	r2, r3, [sp, #24]
 80194b2:	910d      	str	r1, [sp, #52]	; 0x34
 80194b4:	f5a5 1580 	sub.w	r5, r5, #1048576	; 0x100000
 80194b8:	e9dd ab06 	ldrd	sl, fp, [sp, #24]
 80194bc:	e9dd 0100 	ldrd	r0, r1, [sp]
 80194c0:	4652      	mov	r2, sl
 80194c2:	465b      	mov	r3, fp
 80194c4:	4629      	mov	r1, r5
 80194c6:	4680      	mov	r8, r0
 80194c8:	f000 fe0c 	bl	801a0e4 <__aeabi_dsub>
 80194cc:	4652      	mov	r2, sl
 80194ce:	e9cd 0108 	strd	r0, r1, [sp, #32]
 80194d2:	465b      	mov	r3, fp
 80194d4:	4640      	mov	r0, r8
 80194d6:	4629      	mov	r1, r5
 80194d8:	e9cd ab06 	strd	sl, fp, [sp, #24]
 80194dc:	f000 fe04 	bl	801a0e8 <__adddf3>
 80194e0:	4602      	mov	r2, r0
 80194e2:	460b      	mov	r3, r1
 80194e4:	2000      	movs	r0, #0
 80194e6:	49b3      	ldr	r1, [pc, #716]	; (80197b4 <__ieee754_pow+0x574>)
 80194e8:	f001 f8de 	bl	801a6a8 <__aeabi_ddiv>
 80194ec:	460a      	mov	r2, r1
 80194ee:	4601      	mov	r1, r0
 80194f0:	4613      	mov	r3, r2
 80194f2:	e9cd 120e 	strd	r1, r2, [sp, #56]	; 0x38
 80194f6:	4602      	mov	r2, r0
 80194f8:	e9dd 0108 	ldrd	r0, r1, [sp, #32]
 80194fc:	f000 ffaa 	bl	801a454 <__aeabi_dmul>
 8019500:	460a      	mov	r2, r1
 8019502:	4683      	mov	fp, r0
 8019504:	4694      	mov	ip, r2
 8019506:	2300      	movs	r3, #0
 8019508:	4601      	mov	r1, r0
 801950a:	2000      	movs	r0, #0
 801950c:	e9cd bc00 	strd	fp, ip, [sp]
 8019510:	9300      	str	r3, [sp, #0]
 8019512:	106b      	asrs	r3, r5, #1
 8019514:	f043 5300 	orr.w	r3, r3, #536870912	; 0x20000000
 8019518:	f503 2300 	add.w	r3, r3, #524288	; 0x80000
 801951c:	e9cd 1204 	strd	r1, r2, [sp, #16]
 8019520:	e9dd ab00 	ldrd	sl, fp, [sp]
 8019524:	1919      	adds	r1, r3, r4
 8019526:	46a9      	mov	r9, r5
 8019528:	4604      	mov	r4, r0
 801952a:	460d      	mov	r5, r1
 801952c:	4602      	mov	r2, r0
 801952e:	460b      	mov	r3, r1
 8019530:	4650      	mov	r0, sl
 8019532:	4659      	mov	r1, fp
 8019534:	f000 ff8e 	bl	801a454 <__aeabi_dmul>
 8019538:	4602      	mov	r2, r0
 801953a:	460b      	mov	r3, r1
 801953c:	e9dd 0108 	ldrd	r0, r1, [sp, #32]
 8019540:	f000 fdd0 	bl	801a0e4 <__aeabi_dsub>
 8019544:	e9dd 2306 	ldrd	r2, r3, [sp, #24]
 8019548:	e9cd 0108 	strd	r0, r1, [sp, #32]
 801954c:	4620      	mov	r0, r4
 801954e:	4629      	mov	r1, r5
 8019550:	f000 fdc8 	bl	801a0e4 <__aeabi_dsub>
 8019554:	4602      	mov	r2, r0
 8019556:	460b      	mov	r3, r1
 8019558:	4640      	mov	r0, r8
 801955a:	4649      	mov	r1, r9
 801955c:	f000 fdc2 	bl	801a0e4 <__aeabi_dsub>
 8019560:	4652      	mov	r2, sl
 8019562:	465b      	mov	r3, fp
 8019564:	f000 ff76 	bl	801a454 <__aeabi_dmul>
 8019568:	4602      	mov	r2, r0
 801956a:	460b      	mov	r3, r1
 801956c:	e9dd 0108 	ldrd	r0, r1, [sp, #32]
 8019570:	f000 fdb8 	bl	801a0e4 <__aeabi_dsub>
 8019574:	e9dd 230e 	ldrd	r2, r3, [sp, #56]	; 0x38
 8019578:	f000 ff6c 	bl	801a454 <__aeabi_dmul>
 801957c:	e9dd 4504 	ldrd	r4, r5, [sp, #16]
 8019580:	e9cd 0106 	strd	r0, r1, [sp, #24]
 8019584:	4622      	mov	r2, r4
 8019586:	462b      	mov	r3, r5
 8019588:	4620      	mov	r0, r4
 801958a:	4629      	mov	r1, r5
 801958c:	f000 ff62 	bl	801a454 <__aeabi_dmul>
 8019590:	a373      	add	r3, pc, #460	; (adr r3, 8019760 <__ieee754_pow+0x520>)
 8019592:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019596:	4604      	mov	r4, r0
 8019598:	460d      	mov	r5, r1
 801959a:	f000 ff5b 	bl	801a454 <__aeabi_dmul>
 801959e:	a372      	add	r3, pc, #456	; (adr r3, 8019768 <__ieee754_pow+0x528>)
 80195a0:	e9d3 2300 	ldrd	r2, r3, [r3]
 80195a4:	f000 fda0 	bl	801a0e8 <__adddf3>
 80195a8:	4622      	mov	r2, r4
 80195aa:	462b      	mov	r3, r5
 80195ac:	f000 ff52 	bl	801a454 <__aeabi_dmul>
 80195b0:	a36f      	add	r3, pc, #444	; (adr r3, 8019770 <__ieee754_pow+0x530>)
 80195b2:	e9d3 2300 	ldrd	r2, r3, [r3]
 80195b6:	f000 fd97 	bl	801a0e8 <__adddf3>
 80195ba:	4622      	mov	r2, r4
 80195bc:	462b      	mov	r3, r5
 80195be:	f000 ff49 	bl	801a454 <__aeabi_dmul>
 80195c2:	a36d      	add	r3, pc, #436	; (adr r3, 8019778 <__ieee754_pow+0x538>)
 80195c4:	e9d3 2300 	ldrd	r2, r3, [r3]
 80195c8:	f000 fd8e 	bl	801a0e8 <__adddf3>
 80195cc:	4622      	mov	r2, r4
 80195ce:	462b      	mov	r3, r5
 80195d0:	f000 ff40 	bl	801a454 <__aeabi_dmul>
 80195d4:	a36a      	add	r3, pc, #424	; (adr r3, 8019780 <__ieee754_pow+0x540>)
 80195d6:	e9d3 2300 	ldrd	r2, r3, [r3]
 80195da:	f000 fd85 	bl	801a0e8 <__adddf3>
 80195de:	4622      	mov	r2, r4
 80195e0:	462b      	mov	r3, r5
 80195e2:	f000 ff37 	bl	801a454 <__aeabi_dmul>
 80195e6:	a368      	add	r3, pc, #416	; (adr r3, 8019788 <__ieee754_pow+0x548>)
 80195e8:	e9d3 2300 	ldrd	r2, r3, [r3]
 80195ec:	f000 fd7c 	bl	801a0e8 <__adddf3>
 80195f0:	4622      	mov	r2, r4
 80195f2:	4680      	mov	r8, r0
 80195f4:	4689      	mov	r9, r1
 80195f6:	462b      	mov	r3, r5
 80195f8:	4620      	mov	r0, r4
 80195fa:	4629      	mov	r1, r5
 80195fc:	f000 ff2a 	bl	801a454 <__aeabi_dmul>
 8019600:	4602      	mov	r2, r0
 8019602:	460b      	mov	r3, r1
 8019604:	4640      	mov	r0, r8
 8019606:	4649      	mov	r1, r9
 8019608:	f000 ff24 	bl	801a454 <__aeabi_dmul>
 801960c:	4604      	mov	r4, r0
 801960e:	460d      	mov	r5, r1
 8019610:	e9dd 0104 	ldrd	r0, r1, [sp, #16]
 8019614:	4652      	mov	r2, sl
 8019616:	465b      	mov	r3, fp
 8019618:	f000 fd66 	bl	801a0e8 <__adddf3>
 801961c:	e9dd 2306 	ldrd	r2, r3, [sp, #24]
 8019620:	f000 ff18 	bl	801a454 <__aeabi_dmul>
 8019624:	4622      	mov	r2, r4
 8019626:	462b      	mov	r3, r5
 8019628:	f000 fd5e 	bl	801a0e8 <__adddf3>
 801962c:	4652      	mov	r2, sl
 801962e:	4680      	mov	r8, r0
 8019630:	4689      	mov	r9, r1
 8019632:	465b      	mov	r3, fp
 8019634:	4650      	mov	r0, sl
 8019636:	4659      	mov	r1, fp
 8019638:	f000 ff0c 	bl	801a454 <__aeabi_dmul>
 801963c:	2200      	movs	r2, #0
 801963e:	4b5e      	ldr	r3, [pc, #376]	; (80197b8 <__ieee754_pow+0x578>)
 8019640:	e9cd 0108 	strd	r0, r1, [sp, #32]
 8019644:	f000 fd50 	bl	801a0e8 <__adddf3>
 8019648:	4642      	mov	r2, r8
 801964a:	464b      	mov	r3, r9
 801964c:	f000 fd4c 	bl	801a0e8 <__adddf3>
 8019650:	9800      	ldr	r0, [sp, #0]
 8019652:	460d      	mov	r5, r1
 8019654:	4604      	mov	r4, r0
 8019656:	462b      	mov	r3, r5
 8019658:	4622      	mov	r2, r4
 801965a:	4650      	mov	r0, sl
 801965c:	4659      	mov	r1, fp
 801965e:	f000 fef9 	bl	801a454 <__aeabi_dmul>
 8019662:	2200      	movs	r2, #0
 8019664:	4682      	mov	sl, r0
 8019666:	468b      	mov	fp, r1
 8019668:	4b53      	ldr	r3, [pc, #332]	; (80197b8 <__ieee754_pow+0x578>)
 801966a:	4620      	mov	r0, r4
 801966c:	4629      	mov	r1, r5
 801966e:	f000 fd39 	bl	801a0e4 <__aeabi_dsub>
 8019672:	e9dd 2308 	ldrd	r2, r3, [sp, #32]
 8019676:	f000 fd35 	bl	801a0e4 <__aeabi_dsub>
 801967a:	4602      	mov	r2, r0
 801967c:	460b      	mov	r3, r1
 801967e:	4640      	mov	r0, r8
 8019680:	4649      	mov	r1, r9
 8019682:	f000 fd2f 	bl	801a0e4 <__aeabi_dsub>
 8019686:	e9dd 2304 	ldrd	r2, r3, [sp, #16]
 801968a:	f000 fee3 	bl	801a454 <__aeabi_dmul>
 801968e:	4622      	mov	r2, r4
 8019690:	4680      	mov	r8, r0
 8019692:	4689      	mov	r9, r1
 8019694:	e9dd 0106 	ldrd	r0, r1, [sp, #24]
 8019698:	462b      	mov	r3, r5
 801969a:	f000 fedb 	bl	801a454 <__aeabi_dmul>
 801969e:	4602      	mov	r2, r0
 80196a0:	460b      	mov	r3, r1
 80196a2:	4640      	mov	r0, r8
 80196a4:	4649      	mov	r1, r9
 80196a6:	f000 fd1f 	bl	801a0e8 <__adddf3>
 80196aa:	4680      	mov	r8, r0
 80196ac:	4689      	mov	r9, r1
 80196ae:	4602      	mov	r2, r0
 80196b0:	460b      	mov	r3, r1
 80196b2:	4650      	mov	r0, sl
 80196b4:	4659      	mov	r1, fp
 80196b6:	f000 fd17 	bl	801a0e8 <__adddf3>
 80196ba:	a335      	add	r3, pc, #212	; (adr r3, 8019790 <__ieee754_pow+0x550>)
 80196bc:	e9d3 2300 	ldrd	r2, r3, [r3]
 80196c0:	9800      	ldr	r0, [sp, #0]
 80196c2:	460d      	mov	r5, r1
 80196c4:	4604      	mov	r4, r0
 80196c6:	f000 fec5 	bl	801a454 <__aeabi_dmul>
 80196ca:	4652      	mov	r2, sl
 80196cc:	e9cd 0104 	strd	r0, r1, [sp, #16]
 80196d0:	465b      	mov	r3, fp
 80196d2:	4620      	mov	r0, r4
 80196d4:	4629      	mov	r1, r5
 80196d6:	f000 fd05 	bl	801a0e4 <__aeabi_dsub>
 80196da:	4602      	mov	r2, r0
 80196dc:	460b      	mov	r3, r1
 80196de:	4640      	mov	r0, r8
 80196e0:	4649      	mov	r1, r9
 80196e2:	f000 fcff 	bl	801a0e4 <__aeabi_dsub>
 80196e6:	a32c      	add	r3, pc, #176	; (adr r3, 8019798 <__ieee754_pow+0x558>)
 80196e8:	e9d3 2300 	ldrd	r2, r3, [r3]
 80196ec:	f000 feb2 	bl	801a454 <__aeabi_dmul>
 80196f0:	a32b      	add	r3, pc, #172	; (adr r3, 80197a0 <__ieee754_pow+0x560>)
 80196f2:	e9d3 2300 	ldrd	r2, r3, [r3]
 80196f6:	4680      	mov	r8, r0
 80196f8:	4689      	mov	r9, r1
 80196fa:	4620      	mov	r0, r4
 80196fc:	4629      	mov	r1, r5
 80196fe:	f000 fea9 	bl	801a454 <__aeabi_dmul>
 8019702:	4602      	mov	r2, r0
 8019704:	460b      	mov	r3, r1
 8019706:	4640      	mov	r0, r8
 8019708:	4649      	mov	r1, r9
 801970a:	f000 fced 	bl	801a0e8 <__adddf3>
 801970e:	e9dd 2310 	ldrd	r2, r3, [sp, #64]	; 0x40
 8019712:	f000 fce9 	bl	801a0e8 <__adddf3>
 8019716:	4680      	mov	r8, r0
 8019718:	980d      	ldr	r0, [sp, #52]	; 0x34
 801971a:	4689      	mov	r9, r1
 801971c:	f000 fe30 	bl	801a380 <__aeabi_i2d>
 8019720:	4682      	mov	sl, r0
 8019722:	468b      	mov	fp, r1
 8019724:	e9dd 0104 	ldrd	r0, r1, [sp, #16]
 8019728:	4642      	mov	r2, r8
 801972a:	464b      	mov	r3, r9
 801972c:	f000 fcdc 	bl	801a0e8 <__adddf3>
 8019730:	e9dd 230a 	ldrd	r2, r3, [sp, #40]	; 0x28
 8019734:	f000 fcd8 	bl	801a0e8 <__adddf3>
 8019738:	4652      	mov	r2, sl
 801973a:	465b      	mov	r3, fp
 801973c:	f000 fcd4 	bl	801a0e8 <__adddf3>
 8019740:	9800      	ldr	r0, [sp, #0]
 8019742:	4652      	mov	r2, sl
 8019744:	465b      	mov	r3, fp
 8019746:	4604      	mov	r4, r0
 8019748:	460d      	mov	r5, r1
 801974a:	f000 fccb 	bl	801a0e4 <__aeabi_dsub>
 801974e:	e9dd 230a 	ldrd	r2, r3, [sp, #40]	; 0x28
 8019752:	f000 fcc7 	bl	801a0e4 <__aeabi_dsub>
 8019756:	e9dd 2304 	ldrd	r2, r3, [sp, #16]
 801975a:	e02f      	b.n	80197bc <__ieee754_pow+0x57c>
 801975c:	f3af 8000 	nop.w
 8019760:	4a454eef 	.word	0x4a454eef
 8019764:	3fca7e28 	.word	0x3fca7e28
 8019768:	93c9db65 	.word	0x93c9db65
 801976c:	3fcd864a 	.word	0x3fcd864a
 8019770:	a91d4101 	.word	0xa91d4101
 8019774:	3fd17460 	.word	0x3fd17460
 8019778:	518f264d 	.word	0x518f264d
 801977c:	3fd55555 	.word	0x3fd55555
 8019780:	db6fabff 	.word	0xdb6fabff
 8019784:	3fdb6db6 	.word	0x3fdb6db6
 8019788:	33333303 	.word	0x33333303
 801978c:	3fe33333 	.word	0x3fe33333
 8019790:	e0000000 	.word	0xe0000000
 8019794:	3feec709 	.word	0x3feec709
 8019798:	dc3a03fd 	.word	0xdc3a03fd
 801979c:	3feec709 	.word	0x3feec709
 80197a0:	145b01f5 	.word	0x145b01f5
 80197a4:	be3e2fe0 	.word	0xbe3e2fe0
 80197a8:	43400000 	.word	0x43400000
 80197ac:	0003988e 	.word	0x0003988e
 80197b0:	000bb679 	.word	0x000bb679
 80197b4:	3ff00000 	.word	0x3ff00000
 80197b8:	40080000 	.word	0x40080000
 80197bc:	f000 fc92 	bl	801a0e4 <__aeabi_dsub>
 80197c0:	460b      	mov	r3, r1
 80197c2:	4602      	mov	r2, r0
 80197c4:	4649      	mov	r1, r9
 80197c6:	4640      	mov	r0, r8
 80197c8:	f000 fc8c 	bl	801a0e4 <__aeabi_dsub>
 80197cc:	2200      	movs	r2, #0
 80197ce:	3e01      	subs	r6, #1
 80197d0:	ea56 0307 	orrs.w	r3, r6, r7
 80197d4:	bf14      	ite	ne
 80197d6:	4bda      	ldrne	r3, [pc, #872]	; (8019b40 <__ieee754_pow+0x900>)
 80197d8:	4bda      	ldreq	r3, [pc, #872]	; (8019b44 <__ieee754_pow+0x904>)
 80197da:	468b      	mov	fp, r1
 80197dc:	e9cd 2304 	strd	r2, r3, [sp, #16]
 80197e0:	e9dd 1202 	ldrd	r1, r2, [sp, #8]
 80197e4:	2300      	movs	r3, #0
 80197e6:	460e      	mov	r6, r1
 80197e8:	4617      	mov	r7, r2
 80197ea:	e9cd 6700 	strd	r6, r7, [sp]
 80197ee:	9300      	str	r3, [sp, #0]
 80197f0:	e9dd 6700 	ldrd	r6, r7, [sp]
 80197f4:	4682      	mov	sl, r0
 80197f6:	463b      	mov	r3, r7
 80197f8:	4608      	mov	r0, r1
 80197fa:	4611      	mov	r1, r2
 80197fc:	4632      	mov	r2, r6
 80197fe:	f000 fc71 	bl	801a0e4 <__aeabi_dsub>
 8019802:	4622      	mov	r2, r4
 8019804:	462b      	mov	r3, r5
 8019806:	f000 fe25 	bl	801a454 <__aeabi_dmul>
 801980a:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
 801980e:	4680      	mov	r8, r0
 8019810:	4689      	mov	r9, r1
 8019812:	4650      	mov	r0, sl
 8019814:	4659      	mov	r1, fp
 8019816:	f000 fe1d 	bl	801a454 <__aeabi_dmul>
 801981a:	4602      	mov	r2, r0
 801981c:	460b      	mov	r3, r1
 801981e:	4640      	mov	r0, r8
 8019820:	4649      	mov	r1, r9
 8019822:	f000 fc61 	bl	801a0e8 <__adddf3>
 8019826:	4680      	mov	r8, r0
 8019828:	4689      	mov	r9, r1
 801982a:	4632      	mov	r2, r6
 801982c:	463b      	mov	r3, r7
 801982e:	4620      	mov	r0, r4
 8019830:	4629      	mov	r1, r5
 8019832:	e9cd 8902 	strd	r8, r9, [sp, #8]
 8019836:	f000 fe0d 	bl	801a454 <__aeabi_dmul>
 801983a:	460b      	mov	r3, r1
 801983c:	4602      	mov	r2, r0
 801983e:	4606      	mov	r6, r0
 8019840:	460f      	mov	r7, r1
 8019842:	4640      	mov	r0, r8
 8019844:	4649      	mov	r1, r9
 8019846:	f000 fc4f 	bl	801a0e8 <__adddf3>
 801984a:	4bbf      	ldr	r3, [pc, #764]	; (8019b48 <__ieee754_pow+0x908>)
 801984c:	4604      	mov	r4, r0
 801984e:	4299      	cmp	r1, r3
 8019850:	460d      	mov	r5, r1
 8019852:	468a      	mov	sl, r1
 8019854:	f340 8126 	ble.w	8019aa4 <__ieee754_pow+0x864>
 8019858:	f101 433f 	add.w	r3, r1, #3204448256	; 0xbf000000
 801985c:	f503 03e0 	add.w	r3, r3, #7340032	; 0x700000
 8019860:	4303      	orrs	r3, r0
 8019862:	f040 81fe 	bne.w	8019c62 <__ieee754_pow+0xa22>
 8019866:	a3a2      	add	r3, pc, #648	; (adr r3, 8019af0 <__ieee754_pow+0x8b0>)
 8019868:	e9d3 2300 	ldrd	r2, r3, [r3]
 801986c:	e9dd 0102 	ldrd	r0, r1, [sp, #8]
 8019870:	f000 fc3a 	bl	801a0e8 <__adddf3>
 8019874:	4632      	mov	r2, r6
 8019876:	4680      	mov	r8, r0
 8019878:	4689      	mov	r9, r1
 801987a:	463b      	mov	r3, r7
 801987c:	4620      	mov	r0, r4
 801987e:	4629      	mov	r1, r5
 8019880:	f000 fc30 	bl	801a0e4 <__aeabi_dsub>
 8019884:	4602      	mov	r2, r0
 8019886:	460b      	mov	r3, r1
 8019888:	4640      	mov	r0, r8
 801988a:	4649      	mov	r1, r9
 801988c:	f001 f872 	bl	801a974 <__aeabi_dcmpgt>
 8019890:	2800      	cmp	r0, #0
 8019892:	f040 81e6 	bne.w	8019c62 <__ieee754_pow+0xa22>
 8019896:	46a8      	mov	r8, r5
 8019898:	f44f 1380 	mov.w	r3, #1048576	; 0x100000
 801989c:	2000      	movs	r0, #0
 801989e:	ea4f 5228 	mov.w	r2, r8, asr #20
 80198a2:	f2a2 32fe 	subw	r2, r2, #1022	; 0x3fe
 80198a6:	4113      	asrs	r3, r2
 80198a8:	4453      	add	r3, sl
 80198aa:	f3c3 520a 	ubfx	r2, r3, #20, #11
 80198ae:	4da7      	ldr	r5, [pc, #668]	; (8019b4c <__ieee754_pow+0x90c>)
 80198b0:	f2a2 32ff 	subw	r2, r2, #1023	; 0x3ff
 80198b4:	4115      	asrs	r5, r2
 80198b6:	f3c3 0413 	ubfx	r4, r3, #0, #20
 80198ba:	ea23 0105 	bic.w	r1, r3, r5
 80198be:	f1c2 0b14 	rsb	fp, r2, #20
 80198c2:	f444 1480 	orr.w	r4, r4, #1048576	; 0x100000
 80198c6:	f1ba 0f00 	cmp.w	sl, #0
 80198ca:	4602      	mov	r2, r0
 80198cc:	460b      	mov	r3, r1
 80198ce:	fa44 fb0b 	asr.w	fp, r4, fp
 80198d2:	4630      	mov	r0, r6
 80198d4:	4639      	mov	r1, r7
 80198d6:	bfb8      	it	lt
 80198d8:	f1cb 0b00 	rsblt	fp, fp, #0
 80198dc:	f000 fc02 	bl	801a0e4 <__aeabi_dsub>
 80198e0:	4606      	mov	r6, r0
 80198e2:	460f      	mov	r7, r1
 80198e4:	e9dd 0102 	ldrd	r0, r1, [sp, #8]
 80198e8:	4632      	mov	r2, r6
 80198ea:	463b      	mov	r3, r7
 80198ec:	f000 fbfc 	bl	801a0e8 <__adddf3>
 80198f0:	460d      	mov	r5, r1
 80198f2:	ea4f 5a0b 	mov.w	sl, fp, lsl #20
 80198f6:	2400      	movs	r4, #0
 80198f8:	a37f      	add	r3, pc, #508	; (adr r3, 8019af8 <__ieee754_pow+0x8b8>)
 80198fa:	e9d3 2300 	ldrd	r2, r3, [r3]
 80198fe:	4620      	mov	r0, r4
 8019900:	4629      	mov	r1, r5
 8019902:	f000 fda7 	bl	801a454 <__aeabi_dmul>
 8019906:	4632      	mov	r2, r6
 8019908:	4680      	mov	r8, r0
 801990a:	4689      	mov	r9, r1
 801990c:	463b      	mov	r3, r7
 801990e:	4620      	mov	r0, r4
 8019910:	4629      	mov	r1, r5
 8019912:	f000 fbe7 	bl	801a0e4 <__aeabi_dsub>
 8019916:	4602      	mov	r2, r0
 8019918:	460b      	mov	r3, r1
 801991a:	e9dd 0102 	ldrd	r0, r1, [sp, #8]
 801991e:	f000 fbe1 	bl	801a0e4 <__aeabi_dsub>
 8019922:	a377      	add	r3, pc, #476	; (adr r3, 8019b00 <__ieee754_pow+0x8c0>)
 8019924:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019928:	f000 fd94 	bl	801a454 <__aeabi_dmul>
 801992c:	a376      	add	r3, pc, #472	; (adr r3, 8019b08 <__ieee754_pow+0x8c8>)
 801992e:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019932:	4606      	mov	r6, r0
 8019934:	460f      	mov	r7, r1
 8019936:	4620      	mov	r0, r4
 8019938:	4629      	mov	r1, r5
 801993a:	f000 fd8b 	bl	801a454 <__aeabi_dmul>
 801993e:	4602      	mov	r2, r0
 8019940:	460b      	mov	r3, r1
 8019942:	4630      	mov	r0, r6
 8019944:	4639      	mov	r1, r7
 8019946:	f000 fbcf 	bl	801a0e8 <__adddf3>
 801994a:	4606      	mov	r6, r0
 801994c:	460f      	mov	r7, r1
 801994e:	4602      	mov	r2, r0
 8019950:	460b      	mov	r3, r1
 8019952:	4640      	mov	r0, r8
 8019954:	4649      	mov	r1, r9
 8019956:	f000 fbc7 	bl	801a0e8 <__adddf3>
 801995a:	4642      	mov	r2, r8
 801995c:	464b      	mov	r3, r9
 801995e:	4604      	mov	r4, r0
 8019960:	460d      	mov	r5, r1
 8019962:	f000 fbbf 	bl	801a0e4 <__aeabi_dsub>
 8019966:	4602      	mov	r2, r0
 8019968:	460b      	mov	r3, r1
 801996a:	4630      	mov	r0, r6
 801996c:	4639      	mov	r1, r7
 801996e:	f000 fbb9 	bl	801a0e4 <__aeabi_dsub>
 8019972:	4622      	mov	r2, r4
 8019974:	4680      	mov	r8, r0
 8019976:	4689      	mov	r9, r1
 8019978:	462b      	mov	r3, r5
 801997a:	4620      	mov	r0, r4
 801997c:	4629      	mov	r1, r5
 801997e:	f000 fd69 	bl	801a454 <__aeabi_dmul>
 8019982:	a363      	add	r3, pc, #396	; (adr r3, 8019b10 <__ieee754_pow+0x8d0>)
 8019984:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019988:	4606      	mov	r6, r0
 801998a:	460f      	mov	r7, r1
 801998c:	f000 fd62 	bl	801a454 <__aeabi_dmul>
 8019990:	a361      	add	r3, pc, #388	; (adr r3, 8019b18 <__ieee754_pow+0x8d8>)
 8019992:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019996:	f000 fba5 	bl	801a0e4 <__aeabi_dsub>
 801999a:	4632      	mov	r2, r6
 801999c:	463b      	mov	r3, r7
 801999e:	f000 fd59 	bl	801a454 <__aeabi_dmul>
 80199a2:	a35f      	add	r3, pc, #380	; (adr r3, 8019b20 <__ieee754_pow+0x8e0>)
 80199a4:	e9d3 2300 	ldrd	r2, r3, [r3]
 80199a8:	f000 fb9e 	bl	801a0e8 <__adddf3>
 80199ac:	4632      	mov	r2, r6
 80199ae:	463b      	mov	r3, r7
 80199b0:	f000 fd50 	bl	801a454 <__aeabi_dmul>
 80199b4:	a35c      	add	r3, pc, #368	; (adr r3, 8019b28 <__ieee754_pow+0x8e8>)
 80199b6:	e9d3 2300 	ldrd	r2, r3, [r3]
 80199ba:	f000 fb93 	bl	801a0e4 <__aeabi_dsub>
 80199be:	4632      	mov	r2, r6
 80199c0:	463b      	mov	r3, r7
 80199c2:	f000 fd47 	bl	801a454 <__aeabi_dmul>
 80199c6:	a35a      	add	r3, pc, #360	; (adr r3, 8019b30 <__ieee754_pow+0x8f0>)
 80199c8:	e9d3 2300 	ldrd	r2, r3, [r3]
 80199cc:	f000 fb8c 	bl	801a0e8 <__adddf3>
 80199d0:	4632      	mov	r2, r6
 80199d2:	463b      	mov	r3, r7
 80199d4:	f000 fd3e 	bl	801a454 <__aeabi_dmul>
 80199d8:	4602      	mov	r2, r0
 80199da:	460b      	mov	r3, r1
 80199dc:	4620      	mov	r0, r4
 80199de:	4629      	mov	r1, r5
 80199e0:	f000 fb80 	bl	801a0e4 <__aeabi_dsub>
 80199e4:	4606      	mov	r6, r0
 80199e6:	460f      	mov	r7, r1
 80199e8:	4602      	mov	r2, r0
 80199ea:	460b      	mov	r3, r1
 80199ec:	4620      	mov	r0, r4
 80199ee:	4629      	mov	r1, r5
 80199f0:	f000 fd30 	bl	801a454 <__aeabi_dmul>
 80199f4:	2200      	movs	r2, #0
 80199f6:	e9cd 0100 	strd	r0, r1, [sp]
 80199fa:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 80199fe:	4630      	mov	r0, r6
 8019a00:	4639      	mov	r1, r7
 8019a02:	f000 fb6f 	bl	801a0e4 <__aeabi_dsub>
 8019a06:	4602      	mov	r2, r0
 8019a08:	460b      	mov	r3, r1
 8019a0a:	e9dd 0100 	ldrd	r0, r1, [sp]
 8019a0e:	f000 fe4b 	bl	801a6a8 <__aeabi_ddiv>
 8019a12:	4642      	mov	r2, r8
 8019a14:	4606      	mov	r6, r0
 8019a16:	460f      	mov	r7, r1
 8019a18:	464b      	mov	r3, r9
 8019a1a:	4620      	mov	r0, r4
 8019a1c:	4629      	mov	r1, r5
 8019a1e:	f000 fd19 	bl	801a454 <__aeabi_dmul>
 8019a22:	4642      	mov	r2, r8
 8019a24:	464b      	mov	r3, r9
 8019a26:	f000 fb5f 	bl	801a0e8 <__adddf3>
 8019a2a:	4602      	mov	r2, r0
 8019a2c:	460b      	mov	r3, r1
 8019a2e:	4630      	mov	r0, r6
 8019a30:	4639      	mov	r1, r7
 8019a32:	f000 fb57 	bl	801a0e4 <__aeabi_dsub>
 8019a36:	4622      	mov	r2, r4
 8019a38:	462b      	mov	r3, r5
 8019a3a:	f000 fb53 	bl	801a0e4 <__aeabi_dsub>
 8019a3e:	4602      	mov	r2, r0
 8019a40:	460b      	mov	r3, r1
 8019a42:	2000      	movs	r0, #0
 8019a44:	493e      	ldr	r1, [pc, #248]	; (8019b40 <__ieee754_pow+0x900>)
 8019a46:	f000 fb4d 	bl	801a0e4 <__aeabi_dsub>
 8019a4a:	448a      	add	sl, r1
 8019a4c:	f5ba 1f80 	cmp.w	sl, #1048576	; 0x100000
 8019a50:	f2c0 8135 	blt.w	8019cbe <__ieee754_pow+0xa7e>
 8019a54:	4651      	mov	r1, sl
 8019a56:	e9dd 2304 	ldrd	r2, r3, [sp, #16]
 8019a5a:	f000 fcfb 	bl	801a454 <__aeabi_dmul>
 8019a5e:	e9cd 0100 	strd	r0, r1, [sp]
 8019a62:	e407      	b.n	8019274 <__ieee754_pow+0x34>
 8019a64:	2f00      	cmp	r7, #0
 8019a66:	f6ff ac2b 	blt.w	80192c0 <__ieee754_pow+0x80>
 8019a6a:	4648      	mov	r0, r9
 8019a6c:	4659      	mov	r1, fp
 8019a6e:	b013      	add	sp, #76	; 0x4c
 8019a70:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 8019a74:	f000 b956 	b.w	8019d24 <__ieee754_sqrt>
 8019a78:	f1b8 0f00 	cmp.w	r8, #0
 8019a7c:	f6bf acc9 	bge.w	8019412 <__ieee754_pow+0x1d2>
 8019a80:	e9dd 3402 	ldrd	r3, r4, [sp, #8]
 8019a84:	9300      	str	r3, [sp, #0]
 8019a86:	f104 4300 	add.w	r3, r4, #2147483648	; 0x80000000
 8019a8a:	9301      	str	r3, [sp, #4]
 8019a8c:	f7ff bbf2 	b.w	8019274 <__ieee754_pow+0x34>
 8019a90:	464a      	mov	r2, r9
 8019a92:	465b      	mov	r3, fp
 8019a94:	2000      	movs	r0, #0
 8019a96:	492a      	ldr	r1, [pc, #168]	; (8019b40 <__ieee754_pow+0x900>)
 8019a98:	f000 fe06 	bl	801a6a8 <__aeabi_ddiv>
 8019a9c:	e9cd 0100 	strd	r0, r1, [sp]
 8019aa0:	f7ff bbe8 	b.w	8019274 <__ieee754_pow+0x34>
 8019aa4:	4b2a      	ldr	r3, [pc, #168]	; (8019b50 <__ieee754_pow+0x910>)
 8019aa6:	f021 4800 	bic.w	r8, r1, #2147483648	; 0x80000000
 8019aaa:	4598      	cmp	r8, r3
 8019aac:	f340 80e9 	ble.w	8019c82 <__ieee754_pow+0xa42>
 8019ab0:	4b28      	ldr	r3, [pc, #160]	; (8019b54 <__ieee754_pow+0x914>)
 8019ab2:	440b      	add	r3, r1
 8019ab4:	4303      	orrs	r3, r0
 8019ab6:	d10a      	bne.n	8019ace <__ieee754_pow+0x88e>
 8019ab8:	4632      	mov	r2, r6
 8019aba:	463b      	mov	r3, r7
 8019abc:	f000 fb12 	bl	801a0e4 <__aeabi_dsub>
 8019ac0:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
 8019ac4:	f000 ff4c 	bl	801a960 <__aeabi_dcmpge>
 8019ac8:	2800      	cmp	r0, #0
 8019aca:	f43f aee5 	beq.w	8019898 <__ieee754_pow+0x658>
 8019ace:	a31a      	add	r3, pc, #104	; (adr r3, 8019b38 <__ieee754_pow+0x8f8>)
 8019ad0:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019ad4:	e9dd 0104 	ldrd	r0, r1, [sp, #16]
 8019ad8:	f000 fcbc 	bl	801a454 <__aeabi_dmul>
 8019adc:	a316      	add	r3, pc, #88	; (adr r3, 8019b38 <__ieee754_pow+0x8f8>)
 8019ade:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019ae2:	f000 fcb7 	bl	801a454 <__aeabi_dmul>
 8019ae6:	e9cd 0100 	strd	r0, r1, [sp]
 8019aea:	f7ff bbc3 	b.w	8019274 <__ieee754_pow+0x34>
 8019aee:	bf00      	nop
 8019af0:	652b82fe 	.word	0x652b82fe
 8019af4:	3c971547 	.word	0x3c971547
 8019af8:	00000000 	.word	0x00000000
 8019afc:	3fe62e43 	.word	0x3fe62e43
 8019b00:	fefa39ef 	.word	0xfefa39ef
 8019b04:	3fe62e42 	.word	0x3fe62e42
 8019b08:	0ca86c39 	.word	0x0ca86c39
 8019b0c:	be205c61 	.word	0xbe205c61
 8019b10:	72bea4d0 	.word	0x72bea4d0
 8019b14:	3e663769 	.word	0x3e663769
 8019b18:	c5d26bf1 	.word	0xc5d26bf1
 8019b1c:	3ebbbd41 	.word	0x3ebbbd41
 8019b20:	af25de2c 	.word	0xaf25de2c
 8019b24:	3f11566a 	.word	0x3f11566a
 8019b28:	16bebd93 	.word	0x16bebd93
 8019b2c:	3f66c16c 	.word	0x3f66c16c
 8019b30:	5555553e 	.word	0x5555553e
 8019b34:	3fc55555 	.word	0x3fc55555
 8019b38:	c2f8f359 	.word	0xc2f8f359
 8019b3c:	01a56e1f 	.word	0x01a56e1f
 8019b40:	3ff00000 	.word	0x3ff00000
 8019b44:	bff00000 	.word	0xbff00000
 8019b48:	408fffff 	.word	0x408fffff
 8019b4c:	000fffff 	.word	0x000fffff
 8019b50:	4090cbff 	.word	0x4090cbff
 8019b54:	3f6f3400 	.word	0x3f6f3400
 8019b58:	f1bc 0f00 	cmp.w	ip, #0
 8019b5c:	f040 809a 	bne.w	8019c94 <__ieee754_pow+0xa54>
 8019b60:	f1c3 0314 	rsb	r3, r3, #20
 8019b64:	fa45 f203 	asr.w	r2, r5, r3
 8019b68:	fa02 f303 	lsl.w	r3, r2, r3
 8019b6c:	42ab      	cmp	r3, r5
 8019b6e:	f000 80aa 	beq.w	8019cc6 <__ieee754_pow+0xa86>
 8019b72:	4666      	mov	r6, ip
 8019b74:	f7ff bb98 	b.w	80192a8 <__ieee754_pow+0x68>
 8019b78:	2300      	movs	r3, #0
 8019b7a:	2400      	movs	r4, #0
 8019b7c:	461a      	mov	r2, r3
 8019b7e:	4623      	mov	r3, r4
 8019b80:	e9cd 230a 	strd	r2, r3, [sp, #40]	; 0x28
 8019b84:	e9cd 2310 	strd	r2, r3, [sp, #64]	; 0x40
 8019b88:	2200      	movs	r2, #0
 8019b8a:	4b61      	ldr	r3, [pc, #388]	; (8019d10 <__ieee754_pow+0xad0>)
 8019b8c:	2400      	movs	r4, #0
 8019b8e:	e9cd 2306 	strd	r2, r3, [sp, #24]
 8019b92:	e491      	b.n	80194b8 <__ieee754_pow+0x278>
 8019b94:	f04f 0c00 	mov.w	ip, #0
 8019b98:	e46b      	b.n	8019472 <__ieee754_pow+0x232>
 8019b9a:	4b5e      	ldr	r3, [pc, #376]	; (8019d14 <__ieee754_pow+0xad4>)
 8019b9c:	429c      	cmp	r4, r3
 8019b9e:	f77f ac34 	ble.w	801940a <__ieee754_pow+0x1ca>
 8019ba2:	4b5b      	ldr	r3, [pc, #364]	; (8019d10 <__ieee754_pow+0xad0>)
 8019ba4:	429c      	cmp	r4, r3
 8019ba6:	f73f abae 	bgt.w	8019306 <__ieee754_pow+0xc6>
 8019baa:	e9dd 0100 	ldrd	r0, r1, [sp]
 8019bae:	2200      	movs	r2, #0
 8019bb0:	4b57      	ldr	r3, [pc, #348]	; (8019d10 <__ieee754_pow+0xad0>)
 8019bb2:	f000 fa97 	bl	801a0e4 <__aeabi_dsub>
 8019bb6:	a348      	add	r3, pc, #288	; (adr r3, 8019cd8 <__ieee754_pow+0xa98>)
 8019bb8:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019bbc:	4604      	mov	r4, r0
 8019bbe:	460d      	mov	r5, r1
 8019bc0:	f000 fc48 	bl	801a454 <__aeabi_dmul>
 8019bc4:	a346      	add	r3, pc, #280	; (adr r3, 8019ce0 <__ieee754_pow+0xaa0>)
 8019bc6:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019bca:	4682      	mov	sl, r0
 8019bcc:	468b      	mov	fp, r1
 8019bce:	4620      	mov	r0, r4
 8019bd0:	4629      	mov	r1, r5
 8019bd2:	f000 fc3f 	bl	801a454 <__aeabi_dmul>
 8019bd6:	2200      	movs	r2, #0
 8019bd8:	e9cd 0100 	strd	r0, r1, [sp]
 8019bdc:	4b4e      	ldr	r3, [pc, #312]	; (8019d18 <__ieee754_pow+0xad8>)
 8019bde:	4620      	mov	r0, r4
 8019be0:	4629      	mov	r1, r5
 8019be2:	f000 fc37 	bl	801a454 <__aeabi_dmul>
 8019be6:	4602      	mov	r2, r0
 8019be8:	460b      	mov	r3, r1
 8019bea:	a13f      	add	r1, pc, #252	; (adr r1, 8019ce8 <__ieee754_pow+0xaa8>)
 8019bec:	e9d1 0100 	ldrd	r0, r1, [r1]
 8019bf0:	f000 fa78 	bl	801a0e4 <__aeabi_dsub>
 8019bf4:	4622      	mov	r2, r4
 8019bf6:	462b      	mov	r3, r5
 8019bf8:	f000 fc2c 	bl	801a454 <__aeabi_dmul>
 8019bfc:	4602      	mov	r2, r0
 8019bfe:	460b      	mov	r3, r1
 8019c00:	2000      	movs	r0, #0
 8019c02:	4946      	ldr	r1, [pc, #280]	; (8019d1c <__ieee754_pow+0xadc>)
 8019c04:	f000 fa6e 	bl	801a0e4 <__aeabi_dsub>
 8019c08:	4622      	mov	r2, r4
 8019c0a:	4680      	mov	r8, r0
 8019c0c:	4689      	mov	r9, r1
 8019c0e:	462b      	mov	r3, r5
 8019c10:	4620      	mov	r0, r4
 8019c12:	4629      	mov	r1, r5
 8019c14:	f000 fc1e 	bl	801a454 <__aeabi_dmul>
 8019c18:	4602      	mov	r2, r0
 8019c1a:	460b      	mov	r3, r1
 8019c1c:	4640      	mov	r0, r8
 8019c1e:	4649      	mov	r1, r9
 8019c20:	f000 fc18 	bl	801a454 <__aeabi_dmul>
 8019c24:	a332      	add	r3, pc, #200	; (adr r3, 8019cf0 <__ieee754_pow+0xab0>)
 8019c26:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019c2a:	f000 fc13 	bl	801a454 <__aeabi_dmul>
 8019c2e:	4602      	mov	r2, r0
 8019c30:	460b      	mov	r3, r1
 8019c32:	e9dd 0100 	ldrd	r0, r1, [sp]
 8019c36:	f000 fa55 	bl	801a0e4 <__aeabi_dsub>
 8019c3a:	4602      	mov	r2, r0
 8019c3c:	460b      	mov	r3, r1
 8019c3e:	4680      	mov	r8, r0
 8019c40:	4689      	mov	r9, r1
 8019c42:	4650      	mov	r0, sl
 8019c44:	4659      	mov	r1, fp
 8019c46:	f000 fa4f 	bl	801a0e8 <__adddf3>
 8019c4a:	2000      	movs	r0, #0
 8019c4c:	460d      	mov	r5, r1
 8019c4e:	4604      	mov	r4, r0
 8019c50:	4652      	mov	r2, sl
 8019c52:	465b      	mov	r3, fp
 8019c54:	e5b2      	b.n	80197bc <__ieee754_pow+0x57c>
 8019c56:	e9dd 2300 	ldrd	r2, r3, [sp]
 8019c5a:	4610      	mov	r0, r2
 8019c5c:	4619      	mov	r1, r3
 8019c5e:	f7ff bbcb 	b.w	80193f8 <__ieee754_pow+0x1b8>
 8019c62:	a325      	add	r3, pc, #148	; (adr r3, 8019cf8 <__ieee754_pow+0xab8>)
 8019c64:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019c68:	e9dd 0104 	ldrd	r0, r1, [sp, #16]
 8019c6c:	f000 fbf2 	bl	801a454 <__aeabi_dmul>
 8019c70:	a321      	add	r3, pc, #132	; (adr r3, 8019cf8 <__ieee754_pow+0xab8>)
 8019c72:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019c76:	f000 fbed 	bl	801a454 <__aeabi_dmul>
 8019c7a:	e9cd 0100 	strd	r0, r1, [sp]
 8019c7e:	f7ff baf9 	b.w	8019274 <__ieee754_pow+0x34>
 8019c82:	4b26      	ldr	r3, [pc, #152]	; (8019d1c <__ieee754_pow+0xadc>)
 8019c84:	4598      	cmp	r8, r3
 8019c86:	f73f ae07 	bgt.w	8019898 <__ieee754_pow+0x658>
 8019c8a:	e9dd 2300 	ldrd	r2, r3, [sp]
 8019c8e:	4692      	mov	sl, r2
 8019c90:	4693      	mov	fp, r2
 8019c92:	e630      	b.n	80198f6 <__ieee754_pow+0x6b6>
 8019c94:	2600      	movs	r6, #0
 8019c96:	f7ff bb13 	b.w	80192c0 <__ieee754_pow+0x80>
 8019c9a:	a319      	add	r3, pc, #100	; (adr r3, 8019d00 <__ieee754_pow+0xac0>)
 8019c9c:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019ca0:	e9cd 230a 	strd	r2, r3, [sp, #40]	; 0x28
 8019ca4:	a318      	add	r3, pc, #96	; (adr r3, 8019d08 <__ieee754_pow+0xac8>)
 8019ca6:	e9d3 2300 	ldrd	r2, r3, [r3]
 8019caa:	e9cd 2310 	strd	r2, r3, [sp, #64]	; 0x40
 8019cae:	2200      	movs	r2, #0
 8019cb0:	4b1b      	ldr	r3, [pc, #108]	; (8019d20 <__ieee754_pow+0xae0>)
 8019cb2:	f44f 2480 	mov.w	r4, #262144	; 0x40000
 8019cb6:	e9cd 2306 	strd	r2, r3, [sp, #24]
 8019cba:	f7ff bbfd 	b.w	80194b8 <__ieee754_pow+0x278>
 8019cbe:	465a      	mov	r2, fp
 8019cc0:	f000 f992 	bl	8019fe8 <scalbn>
 8019cc4:	e6c7      	b.n	8019a56 <__ieee754_pow+0x816>
 8019cc6:	f002 0201 	and.w	r2, r2, #1
 8019cca:	f1c2 0602 	rsb	r6, r2, #2
 8019cce:	f7ff baeb 	b.w	80192a8 <__ieee754_pow+0x68>
 8019cd2:	bf00      	nop
 8019cd4:	f3af 8000 	nop.w
 8019cd8:	60000000 	.word	0x60000000
 8019cdc:	3ff71547 	.word	0x3ff71547
 8019ce0:	f85ddf44 	.word	0xf85ddf44
 8019ce4:	3e54ae0b 	.word	0x3e54ae0b
 8019ce8:	55555555 	.word	0x55555555
 8019cec:	3fd55555 	.word	0x3fd55555
 8019cf0:	652b82fe 	.word	0x652b82fe
 8019cf4:	3ff71547 	.word	0x3ff71547
 8019cf8:	8800759c 	.word	0x8800759c
 8019cfc:	7e37e43c 	.word	0x7e37e43c
 8019d00:	40000000 	.word	0x40000000
 8019d04:	3fe2b803 	.word	0x3fe2b803
 8019d08:	43cfd006 	.word	0x43cfd006
 8019d0c:	3e4cfdeb 	.word	0x3e4cfdeb
 8019d10:	3ff00000 	.word	0x3ff00000
 8019d14:	3feffffe 	.word	0x3feffffe
 8019d18:	3fd00000 	.word	0x3fd00000
 8019d1c:	3fe00000 	.word	0x3fe00000
 8019d20:	3ff80000 	.word	0x3ff80000

08019d24 <__ieee754_sqrt>:
 8019d24:	4a5f      	ldr	r2, [pc, #380]	; (8019ea4 <__ieee754_sqrt+0x180>)
 8019d26:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8019d2a:	438a      	bics	r2, r1
 8019d2c:	460d      	mov	r5, r1
 8019d2e:	4607      	mov	r7, r0
 8019d30:	f000 8094 	beq.w	8019e5c <__ieee754_sqrt+0x138>
 8019d34:	2900      	cmp	r1, #0
 8019d36:	460b      	mov	r3, r1
 8019d38:	4604      	mov	r4, r0
 8019d3a:	4606      	mov	r6, r0
 8019d3c:	dd54      	ble.n	8019de8 <__ieee754_sqrt+0xc4>
 8019d3e:	150f      	asrs	r7, r1, #20
 8019d40:	f000 80a7 	beq.w	8019e92 <__ieee754_sqrt+0x16e>
 8019d44:	f3c3 0313 	ubfx	r3, r3, #0, #20
 8019d48:	f2a7 37ff 	subw	r7, r7, #1023	; 0x3ff
 8019d4c:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
 8019d50:	0ff2      	lsrs	r2, r6, #31
 8019d52:	07f8      	lsls	r0, r7, #31
 8019d54:	eb02 0343 	add.w	r3, r2, r3, lsl #1
 8019d58:	ea4f 0546 	mov.w	r5, r6, lsl #1
 8019d5c:	d503      	bpl.n	8019d66 <__ieee754_sqrt+0x42>
 8019d5e:	0fed      	lsrs	r5, r5, #31
 8019d60:	eb05 0343 	add.w	r3, r5, r3, lsl #1
 8019d64:	00b5      	lsls	r5, r6, #2
 8019d66:	2600      	movs	r6, #0
 8019d68:	2216      	movs	r2, #22
 8019d6a:	46b4      	mov	ip, r6
 8019d6c:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
 8019d70:	107f      	asrs	r7, r7, #1
 8019d72:	eb0c 0001 	add.w	r0, ip, r1
 8019d76:	4298      	cmp	r0, r3
 8019d78:	ea4f 74d5 	mov.w	r4, r5, lsr #31
 8019d7c:	ea4f 0545 	mov.w	r5, r5, lsl #1
 8019d80:	dc03      	bgt.n	8019d8a <__ieee754_sqrt+0x66>
 8019d82:	1a1b      	subs	r3, r3, r0
 8019d84:	eb00 0c01 	add.w	ip, r0, r1
 8019d88:	440e      	add	r6, r1
 8019d8a:	3a01      	subs	r2, #1
 8019d8c:	eb04 0343 	add.w	r3, r4, r3, lsl #1
 8019d90:	ea4f 0151 	mov.w	r1, r1, lsr #1
 8019d94:	d1ed      	bne.n	8019d72 <__ieee754_sqrt+0x4e>
 8019d96:	4610      	mov	r0, r2
 8019d98:	4696      	mov	lr, r2
 8019d9a:	2420      	movs	r4, #32
 8019d9c:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
 8019da0:	e009      	b.n	8019db6 <__ieee754_sqrt+0x92>
 8019da2:	d052      	beq.n	8019e4a <__ieee754_sqrt+0x126>
 8019da4:	005b      	lsls	r3, r3, #1
 8019da6:	3c01      	subs	r4, #1
 8019da8:	eb03 73d5 	add.w	r3, r3, r5, lsr #31
 8019dac:	ea4f 0252 	mov.w	r2, r2, lsr #1
 8019db0:	ea4f 0545 	mov.w	r5, r5, lsl #1
 8019db4:	d037      	beq.n	8019e26 <__ieee754_sqrt+0x102>
 8019db6:	4563      	cmp	r3, ip
 8019db8:	eb02 010e 	add.w	r1, r2, lr
 8019dbc:	ddf1      	ble.n	8019da2 <__ieee754_sqrt+0x7e>
 8019dbe:	2900      	cmp	r1, #0
 8019dc0:	eb01 0e02 	add.w	lr, r1, r2
 8019dc4:	db0a      	blt.n	8019ddc <__ieee754_sqrt+0xb8>
 8019dc6:	46e0      	mov	r8, ip
 8019dc8:	eba3 030c 	sub.w	r3, r3, ip
 8019dcc:	42a9      	cmp	r1, r5
 8019dce:	bf88      	it	hi
 8019dd0:	f103 33ff 	addhi.w	r3, r3, #4294967295	; 0xffffffff
 8019dd4:	46c4      	mov	ip, r8
 8019dd6:	1a6d      	subs	r5, r5, r1
 8019dd8:	4410      	add	r0, r2
 8019dda:	e7e3      	b.n	8019da4 <__ieee754_sqrt+0x80>
 8019ddc:	f1be 0f00 	cmp.w	lr, #0
 8019de0:	dbf1      	blt.n	8019dc6 <__ieee754_sqrt+0xa2>
 8019de2:	f10c 0801 	add.w	r8, ip, #1
 8019de6:	e7ef      	b.n	8019dc8 <__ieee754_sqrt+0xa4>
 8019de8:	f021 4200 	bic.w	r2, r1, #2147483648	; 0x80000000
 8019dec:	4302      	orrs	r2, r0
 8019dee:	d029      	beq.n	8019e44 <__ieee754_sqrt+0x120>
 8019df0:	2900      	cmp	r1, #0
 8019df2:	d145      	bne.n	8019e80 <__ieee754_sqrt+0x15c>
 8019df4:	4601      	mov	r1, r0
 8019df6:	0aca      	lsrs	r2, r1, #11
 8019df8:	3b15      	subs	r3, #21
 8019dfa:	0549      	lsls	r1, r1, #21
 8019dfc:	2a00      	cmp	r2, #0
 8019dfe:	d0fa      	beq.n	8019df6 <__ieee754_sqrt+0xd2>
 8019e00:	f412 1080 	ands.w	r0, r2, #1048576	; 0x100000
 8019e04:	d001      	beq.n	8019e0a <__ieee754_sqrt+0xe6>
 8019e06:	e048      	b.n	8019e9a <__ieee754_sqrt+0x176>
 8019e08:	4630      	mov	r0, r6
 8019e0a:	0052      	lsls	r2, r2, #1
 8019e0c:	02d4      	lsls	r4, r2, #11
 8019e0e:	f100 0601 	add.w	r6, r0, #1
 8019e12:	d5f9      	bpl.n	8019e08 <__ieee754_sqrt+0xe4>
 8019e14:	f1c6 0420 	rsb	r4, r6, #32
 8019e18:	fa01 f606 	lsl.w	r6, r1, r6
 8019e1c:	40e1      	lsrs	r1, r4
 8019e1e:	1a1f      	subs	r7, r3, r0
 8019e20:	ea41 0302 	orr.w	r3, r1, r2
 8019e24:	e78e      	b.n	8019d44 <__ieee754_sqrt+0x20>
 8019e26:	432b      	orrs	r3, r5
 8019e28:	d124      	bne.n	8019e74 <__ieee754_sqrt+0x150>
 8019e2a:	0844      	lsrs	r4, r0, #1
 8019e2c:	1073      	asrs	r3, r6, #1
 8019e2e:	f103 537f 	add.w	r3, r3, #1069547520	; 0x3fc00000
 8019e32:	07f2      	lsls	r2, r6, #31
 8019e34:	f503 1300 	add.w	r3, r3, #2097152	; 0x200000
 8019e38:	bf48      	it	mi
 8019e3a:	f044 4400 	orrmi.w	r4, r4, #2147483648	; 0x80000000
 8019e3e:	eb03 5107 	add.w	r1, r3, r7, lsl #20
 8019e42:	4627      	mov	r7, r4
 8019e44:	4638      	mov	r0, r7
 8019e46:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8019e4a:	42a9      	cmp	r1, r5
 8019e4c:	d8aa      	bhi.n	8019da4 <__ieee754_sqrt+0x80>
 8019e4e:	2900      	cmp	r1, #0
 8019e50:	eb01 0e02 	add.w	lr, r1, r2
 8019e54:	dbc2      	blt.n	8019ddc <__ieee754_sqrt+0xb8>
 8019e56:	4698      	mov	r8, r3
 8019e58:	2300      	movs	r3, #0
 8019e5a:	e7bb      	b.n	8019dd4 <__ieee754_sqrt+0xb0>
 8019e5c:	4602      	mov	r2, r0
 8019e5e:	460b      	mov	r3, r1
 8019e60:	f000 faf8 	bl	801a454 <__aeabi_dmul>
 8019e64:	463a      	mov	r2, r7
 8019e66:	462b      	mov	r3, r5
 8019e68:	f000 f93e 	bl	801a0e8 <__adddf3>
 8019e6c:	4607      	mov	r7, r0
 8019e6e:	4638      	mov	r0, r7
 8019e70:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8019e74:	1c41      	adds	r1, r0, #1
 8019e76:	bf16      	itet	ne
 8019e78:	1c42      	addne	r2, r0, #1
 8019e7a:	3601      	addeq	r6, #1
 8019e7c:	0854      	lsrne	r4, r2, #1
 8019e7e:	e7d5      	b.n	8019e2c <__ieee754_sqrt+0x108>
 8019e80:	4602      	mov	r2, r0
 8019e82:	f000 f92f 	bl	801a0e4 <__aeabi_dsub>
 8019e86:	4602      	mov	r2, r0
 8019e88:	460b      	mov	r3, r1
 8019e8a:	f000 fc0d 	bl	801a6a8 <__aeabi_ddiv>
 8019e8e:	4607      	mov	r7, r0
 8019e90:	e7d8      	b.n	8019e44 <__ieee754_sqrt+0x120>
 8019e92:	461a      	mov	r2, r3
 8019e94:	4621      	mov	r1, r4
 8019e96:	463b      	mov	r3, r7
 8019e98:	e7b2      	b.n	8019e00 <__ieee754_sqrt+0xdc>
 8019e9a:	460e      	mov	r6, r1
 8019e9c:	2420      	movs	r4, #32
 8019e9e:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 8019ea2:	e7bb      	b.n	8019e1c <__ieee754_sqrt+0xf8>
 8019ea4:	7ff00000 	.word	0x7ff00000

08019ea8 <fabs>:
 8019ea8:	f021 4300 	bic.w	r3, r1, #2147483648	; 0x80000000
 8019eac:	4619      	mov	r1, r3
 8019eae:	4770      	bx	lr

08019eb0 <finite>:
 8019eb0:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
 8019eb4:	f501 1080 	add.w	r0, r1, #1048576	; 0x100000
 8019eb8:	0fc0      	lsrs	r0, r0, #31
 8019eba:	4770      	bx	lr

08019ebc <matherr>:
 8019ebc:	2000      	movs	r0, #0
 8019ebe:	4770      	bx	lr

08019ec0 <nan>:
 8019ec0:	2000      	movs	r0, #0
 8019ec2:	4901      	ldr	r1, [pc, #4]	; (8019ec8 <nan+0x8>)
 8019ec4:	4770      	bx	lr
 8019ec6:	bf00      	nop
 8019ec8:	7ff80000 	.word	0x7ff80000

08019ecc <rint>:
 8019ecc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8019ed0:	f3c1 570a 	ubfx	r7, r1, #20, #11
 8019ed4:	f2a7 35ff 	subw	r5, r7, #1023	; 0x3ff
 8019ed8:	2d13      	cmp	r5, #19
 8019eda:	b082      	sub	sp, #8
 8019edc:	4602      	mov	r2, r0
 8019ede:	460b      	mov	r3, r1
 8019ee0:	460c      	mov	r4, r1
 8019ee2:	ea4f 78d1 	mov.w	r8, r1, lsr #31
 8019ee6:	4606      	mov	r6, r0
 8019ee8:	dc32      	bgt.n	8019f50 <rint+0x84>
 8019eea:	2d00      	cmp	r5, #0
 8019eec:	db4d      	blt.n	8019f8a <rint+0xbe>
 8019eee:	493b      	ldr	r1, [pc, #236]	; (8019fdc <rint+0x110>)
 8019ef0:	4129      	asrs	r1, r5
 8019ef2:	ea03 0001 	and.w	r0, r3, r1
 8019ef6:	4310      	orrs	r0, r2
 8019ef8:	d025      	beq.n	8019f46 <rint+0x7a>
 8019efa:	0849      	lsrs	r1, r1, #1
 8019efc:	400b      	ands	r3, r1
 8019efe:	ea52 0603 	orrs.w	r6, r2, r3
 8019f02:	d00b      	beq.n	8019f1c <rint+0x50>
 8019f04:	f44f 2380 	mov.w	r3, #262144	; 0x40000
 8019f08:	2d13      	cmp	r5, #19
 8019f0a:	bf0c      	ite	eq
 8019f0c:	f04f 4600 	moveq.w	r6, #2147483648	; 0x80000000
 8019f10:	2600      	movne	r6, #0
 8019f12:	ea24 0101 	bic.w	r1, r4, r1
 8019f16:	fa43 f405 	asr.w	r4, r3, r5
 8019f1a:	430c      	orrs	r4, r1
 8019f1c:	4930      	ldr	r1, [pc, #192]	; (8019fe0 <rint+0x114>)
 8019f1e:	4623      	mov	r3, r4
 8019f20:	eb01 08c8 	add.w	r8, r1, r8, lsl #3
 8019f24:	e9d8 4500 	ldrd	r4, r5, [r8]
 8019f28:	4632      	mov	r2, r6
 8019f2a:	4620      	mov	r0, r4
 8019f2c:	4629      	mov	r1, r5
 8019f2e:	f000 f8db 	bl	801a0e8 <__adddf3>
 8019f32:	e9cd 0100 	strd	r0, r1, [sp]
 8019f36:	e9dd 0100 	ldrd	r0, r1, [sp]
 8019f3a:	4622      	mov	r2, r4
 8019f3c:	462b      	mov	r3, r5
 8019f3e:	f000 f8d1 	bl	801a0e4 <__aeabi_dsub>
 8019f42:	4602      	mov	r2, r0
 8019f44:	460b      	mov	r3, r1
 8019f46:	4610      	mov	r0, r2
 8019f48:	4619      	mov	r1, r3
 8019f4a:	b002      	add	sp, #8
 8019f4c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8019f50:	2d33      	cmp	r5, #51	; 0x33
 8019f52:	dd07      	ble.n	8019f64 <rint+0x98>
 8019f54:	f5b5 6f80 	cmp.w	r5, #1024	; 0x400
 8019f58:	d1f5      	bne.n	8019f46 <rint+0x7a>
 8019f5a:	f000 f8c5 	bl	801a0e8 <__adddf3>
 8019f5e:	4602      	mov	r2, r0
 8019f60:	460b      	mov	r3, r1
 8019f62:	e7f0      	b.n	8019f46 <rint+0x7a>
 8019f64:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
 8019f68:	f2a7 4713 	subw	r7, r7, #1043	; 0x413
 8019f6c:	40f9      	lsrs	r1, r7
 8019f6e:	4208      	tst	r0, r1
 8019f70:	d0e9      	beq.n	8019f46 <rint+0x7a>
 8019f72:	0849      	lsrs	r1, r1, #1
 8019f74:	4208      	tst	r0, r1
 8019f76:	d0d1      	beq.n	8019f1c <rint+0x50>
 8019f78:	f04f 4380 	mov.w	r3, #1073741824	; 0x40000000
 8019f7c:	ea20 0101 	bic.w	r1, r0, r1
 8019f80:	fa43 f707 	asr.w	r7, r3, r7
 8019f84:	ea41 0607 	orr.w	r6, r1, r7
 8019f88:	e7c8      	b.n	8019f1c <rint+0x50>
 8019f8a:	f021 4100 	bic.w	r1, r1, #2147483648	; 0x80000000
 8019f8e:	4301      	orrs	r1, r0
 8019f90:	d0d9      	beq.n	8019f46 <rint+0x7a>
 8019f92:	f3c3 0313 	ubfx	r3, r3, #0, #20
 8019f96:	431e      	orrs	r6, r3
 8019f98:	4273      	negs	r3, r6
 8019f9a:	4333      	orrs	r3, r6
 8019f9c:	0c64      	lsrs	r4, r4, #17
 8019f9e:	0b1b      	lsrs	r3, r3, #12
 8019fa0:	0464      	lsls	r4, r4, #17
 8019fa2:	f403 2300 	and.w	r3, r3, #524288	; 0x80000
 8019fa6:	ea43 0104 	orr.w	r1, r3, r4
 8019faa:	460b      	mov	r3, r1
 8019fac:	490c      	ldr	r1, [pc, #48]	; (8019fe0 <rint+0x114>)
 8019fae:	eb01 01c8 	add.w	r1, r1, r8, lsl #3
 8019fb2:	e9d1 4500 	ldrd	r4, r5, [r1]
 8019fb6:	4620      	mov	r0, r4
 8019fb8:	4629      	mov	r1, r5
 8019fba:	f000 f895 	bl	801a0e8 <__adddf3>
 8019fbe:	e9cd 0100 	strd	r0, r1, [sp]
 8019fc2:	e9dd 0100 	ldrd	r0, r1, [sp]
 8019fc6:	4622      	mov	r2, r4
 8019fc8:	462b      	mov	r3, r5
 8019fca:	f000 f88b 	bl	801a0e4 <__aeabi_dsub>
 8019fce:	f021 4100 	bic.w	r1, r1, #2147483648	; 0x80000000
 8019fd2:	4602      	mov	r2, r0
 8019fd4:	ea41 73c8 	orr.w	r3, r1, r8, lsl #31
 8019fd8:	e7b5      	b.n	8019f46 <rint+0x7a>
 8019fda:	bf00      	nop
 8019fdc:	000fffff 	.word	0x000fffff
 8019fe0:	0801af98 	.word	0x0801af98
 8019fe4:	00000000 	.word	0x00000000

08019fe8 <scalbn>:
 8019fe8:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8019fea:	f3c1 530a 	ubfx	r3, r1, #20, #11
 8019fee:	4604      	mov	r4, r0
 8019ff0:	460d      	mov	r5, r1
 8019ff2:	460e      	mov	r6, r1
 8019ff4:	4617      	mov	r7, r2
 8019ff6:	bb0b      	cbnz	r3, 801a03c <scalbn+0x54>
 8019ff8:	4603      	mov	r3, r0
 8019ffa:	f021 4600 	bic.w	r6, r1, #2147483648	; 0x80000000
 8019ffe:	4333      	orrs	r3, r6
 801a000:	d01b      	beq.n	801a03a <scalbn+0x52>
 801a002:	4b2f      	ldr	r3, [pc, #188]	; (801a0c0 <scalbn+0xd8>)
 801a004:	2200      	movs	r2, #0
 801a006:	f000 fa25 	bl	801a454 <__aeabi_dmul>
 801a00a:	4b2e      	ldr	r3, [pc, #184]	; (801a0c4 <scalbn+0xdc>)
 801a00c:	4604      	mov	r4, r0
 801a00e:	429f      	cmp	r7, r3
 801a010:	460d      	mov	r5, r1
 801a012:	db37      	blt.n	801a084 <scalbn+0x9c>
 801a014:	460e      	mov	r6, r1
 801a016:	f3c1 530a 	ubfx	r3, r1, #20, #11
 801a01a:	3b36      	subs	r3, #54	; 0x36
 801a01c:	f240 72fe 	movw	r2, #2046	; 0x7fe
 801a020:	443b      	add	r3, r7
 801a022:	4293      	cmp	r3, r2
 801a024:	dc1a      	bgt.n	801a05c <scalbn+0x74>
 801a026:	2b00      	cmp	r3, #0
 801a028:	dd11      	ble.n	801a04e <scalbn+0x66>
 801a02a:	f026 46ff 	bic.w	r6, r6, #2139095040	; 0x7f800000
 801a02e:	f426 06e0 	bic.w	r6, r6, #7340032	; 0x700000
 801a032:	ea46 5503 	orr.w	r5, r6, r3, lsl #20
 801a036:	4620      	mov	r0, r4
 801a038:	4629      	mov	r1, r5
 801a03a:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 801a03c:	f240 72ff 	movw	r2, #2047	; 0x7ff
 801a040:	4293      	cmp	r3, r2
 801a042:	d1eb      	bne.n	801a01c <scalbn+0x34>
 801a044:	4602      	mov	r2, r0
 801a046:	460b      	mov	r3, r1
 801a048:	f000 f84e 	bl	801a0e8 <__adddf3>
 801a04c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 801a04e:	f113 0f35 	cmn.w	r3, #53	; 0x35
 801a052:	da1d      	bge.n	801a090 <scalbn+0xa8>
 801a054:	f24c 3350 	movw	r3, #50000	; 0xc350
 801a058:	429f      	cmp	r7, r3
 801a05a:	dd0c      	ble.n	801a076 <scalbn+0x8e>
 801a05c:	a114      	add	r1, pc, #80	; (adr r1, 801a0b0 <scalbn+0xc8>)
 801a05e:	e9d1 0100 	ldrd	r0, r1, [r1]
 801a062:	4622      	mov	r2, r4
 801a064:	462b      	mov	r3, r5
 801a066:	f000 f831 	bl	801a0cc <copysign>
 801a06a:	a311      	add	r3, pc, #68	; (adr r3, 801a0b0 <scalbn+0xc8>)
 801a06c:	e9d3 2300 	ldrd	r2, r3, [r3]
 801a070:	f000 f9f0 	bl	801a454 <__aeabi_dmul>
 801a074:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 801a076:	4622      	mov	r2, r4
 801a078:	a10f      	add	r1, pc, #60	; (adr r1, 801a0b8 <scalbn+0xd0>)
 801a07a:	e9d1 0100 	ldrd	r0, r1, [r1]
 801a07e:	462b      	mov	r3, r5
 801a080:	f000 f824 	bl	801a0cc <copysign>
 801a084:	a30c      	add	r3, pc, #48	; (adr r3, 801a0b8 <scalbn+0xd0>)
 801a086:	e9d3 2300 	ldrd	r2, r3, [r3]
 801a08a:	f000 f9e3 	bl	801a454 <__aeabi_dmul>
 801a08e:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 801a090:	f026 46ff 	bic.w	r6, r6, #2139095040	; 0x7f800000
 801a094:	f426 06e0 	bic.w	r6, r6, #7340032	; 0x700000
 801a098:	3336      	adds	r3, #54	; 0x36
 801a09a:	ea46 5503 	orr.w	r5, r6, r3, lsl #20
 801a09e:	4620      	mov	r0, r4
 801a0a0:	4629      	mov	r1, r5
 801a0a2:	2200      	movs	r2, #0
 801a0a4:	4b08      	ldr	r3, [pc, #32]	; (801a0c8 <scalbn+0xe0>)
 801a0a6:	f000 f9d5 	bl	801a454 <__aeabi_dmul>
 801a0aa:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 801a0ac:	f3af 8000 	nop.w
 801a0b0:	8800759c 	.word	0x8800759c
 801a0b4:	7e37e43c 	.word	0x7e37e43c
 801a0b8:	c2f8f359 	.word	0xc2f8f359
 801a0bc:	01a56e1f 	.word	0x01a56e1f
 801a0c0:	43500000 	.word	0x43500000
 801a0c4:	ffff3cb0 	.word	0xffff3cb0
 801a0c8:	3c900000 	.word	0x3c900000

0801a0cc <copysign>:
 801a0cc:	f003 4300 	and.w	r3, r3, #2147483648	; 0x80000000
 801a0d0:	f021 4200 	bic.w	r2, r1, #2147483648	; 0x80000000
 801a0d4:	ea42 0103 	orr.w	r1, r2, r3
 801a0d8:	4770      	bx	lr
 801a0da:	bf00      	nop

0801a0dc <__aeabi_drsub>:
 801a0dc:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000
 801a0e0:	e002      	b.n	801a0e8 <__adddf3>
 801a0e2:	bf00      	nop

0801a0e4 <__aeabi_dsub>:
 801a0e4:	f083 4300 	eor.w	r3, r3, #2147483648	; 0x80000000

0801a0e8 <__adddf3>:
 801a0e8:	b530      	push	{r4, r5, lr}
 801a0ea:	ea4f 0441 	mov.w	r4, r1, lsl #1
 801a0ee:	ea4f 0543 	mov.w	r5, r3, lsl #1
 801a0f2:	ea94 0f05 	teq	r4, r5
 801a0f6:	bf08      	it	eq
 801a0f8:	ea90 0f02 	teqeq	r0, r2
 801a0fc:	bf1f      	itttt	ne
 801a0fe:	ea54 0c00 	orrsne.w	ip, r4, r0
 801a102:	ea55 0c02 	orrsne.w	ip, r5, r2
 801a106:	ea7f 5c64 	mvnsne.w	ip, r4, asr #21
 801a10a:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 801a10e:	f000 80e2 	beq.w	801a2d6 <__adddf3+0x1ee>
 801a112:	ea4f 5454 	mov.w	r4, r4, lsr #21
 801a116:	ebd4 5555 	rsbs	r5, r4, r5, lsr #21
 801a11a:	bfb8      	it	lt
 801a11c:	426d      	neglt	r5, r5
 801a11e:	dd0c      	ble.n	801a13a <__adddf3+0x52>
 801a120:	442c      	add	r4, r5
 801a122:	ea80 0202 	eor.w	r2, r0, r2
 801a126:	ea81 0303 	eor.w	r3, r1, r3
 801a12a:	ea82 0000 	eor.w	r0, r2, r0
 801a12e:	ea83 0101 	eor.w	r1, r3, r1
 801a132:	ea80 0202 	eor.w	r2, r0, r2
 801a136:	ea81 0303 	eor.w	r3, r1, r3
 801a13a:	2d36      	cmp	r5, #54	; 0x36
 801a13c:	bf88      	it	hi
 801a13e:	bd30      	pophi	{r4, r5, pc}
 801a140:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 801a144:	ea4f 3101 	mov.w	r1, r1, lsl #12
 801a148:	f44f 1c80 	mov.w	ip, #1048576	; 0x100000
 801a14c:	ea4c 3111 	orr.w	r1, ip, r1, lsr #12
 801a150:	d002      	beq.n	801a158 <__adddf3+0x70>
 801a152:	4240      	negs	r0, r0
 801a154:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 801a158:	f013 4f00 	tst.w	r3, #2147483648	; 0x80000000
 801a15c:	ea4f 3303 	mov.w	r3, r3, lsl #12
 801a160:	ea4c 3313 	orr.w	r3, ip, r3, lsr #12
 801a164:	d002      	beq.n	801a16c <__adddf3+0x84>
 801a166:	4252      	negs	r2, r2
 801a168:	eb63 0343 	sbc.w	r3, r3, r3, lsl #1
 801a16c:	ea94 0f05 	teq	r4, r5
 801a170:	f000 80a7 	beq.w	801a2c2 <__adddf3+0x1da>
 801a174:	f1a4 0401 	sub.w	r4, r4, #1
 801a178:	f1d5 0e20 	rsbs	lr, r5, #32
 801a17c:	db0d      	blt.n	801a19a <__adddf3+0xb2>
 801a17e:	fa02 fc0e 	lsl.w	ip, r2, lr
 801a182:	fa22 f205 	lsr.w	r2, r2, r5
 801a186:	1880      	adds	r0, r0, r2
 801a188:	f141 0100 	adc.w	r1, r1, #0
 801a18c:	fa03 f20e 	lsl.w	r2, r3, lr
 801a190:	1880      	adds	r0, r0, r2
 801a192:	fa43 f305 	asr.w	r3, r3, r5
 801a196:	4159      	adcs	r1, r3
 801a198:	e00e      	b.n	801a1b8 <__adddf3+0xd0>
 801a19a:	f1a5 0520 	sub.w	r5, r5, #32
 801a19e:	f10e 0e20 	add.w	lr, lr, #32
 801a1a2:	2a01      	cmp	r2, #1
 801a1a4:	fa03 fc0e 	lsl.w	ip, r3, lr
 801a1a8:	bf28      	it	cs
 801a1aa:	f04c 0c02 	orrcs.w	ip, ip, #2
 801a1ae:	fa43 f305 	asr.w	r3, r3, r5
 801a1b2:	18c0      	adds	r0, r0, r3
 801a1b4:	eb51 71e3 	adcs.w	r1, r1, r3, asr #31
 801a1b8:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 801a1bc:	d507      	bpl.n	801a1ce <__adddf3+0xe6>
 801a1be:	f04f 0e00 	mov.w	lr, #0
 801a1c2:	f1dc 0c00 	rsbs	ip, ip, #0
 801a1c6:	eb7e 0000 	sbcs.w	r0, lr, r0
 801a1ca:	eb6e 0101 	sbc.w	r1, lr, r1
 801a1ce:	f5b1 1f80 	cmp.w	r1, #1048576	; 0x100000
 801a1d2:	d31b      	bcc.n	801a20c <__adddf3+0x124>
 801a1d4:	f5b1 1f00 	cmp.w	r1, #2097152	; 0x200000
 801a1d8:	d30c      	bcc.n	801a1f4 <__adddf3+0x10c>
 801a1da:	0849      	lsrs	r1, r1, #1
 801a1dc:	ea5f 0030 	movs.w	r0, r0, rrx
 801a1e0:	ea4f 0c3c 	mov.w	ip, ip, rrx
 801a1e4:	f104 0401 	add.w	r4, r4, #1
 801a1e8:	ea4f 5244 	mov.w	r2, r4, lsl #21
 801a1ec:	f512 0f80 	cmn.w	r2, #4194304	; 0x400000
 801a1f0:	f080 809a 	bcs.w	801a328 <__adddf3+0x240>
 801a1f4:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 801a1f8:	bf08      	it	eq
 801a1fa:	ea5f 0c50 	movseq.w	ip, r0, lsr #1
 801a1fe:	f150 0000 	adcs.w	r0, r0, #0
 801a202:	eb41 5104 	adc.w	r1, r1, r4, lsl #20
 801a206:	ea41 0105 	orr.w	r1, r1, r5
 801a20a:	bd30      	pop	{r4, r5, pc}
 801a20c:	ea5f 0c4c 	movs.w	ip, ip, lsl #1
 801a210:	4140      	adcs	r0, r0
 801a212:	eb41 0101 	adc.w	r1, r1, r1
 801a216:	f411 1f80 	tst.w	r1, #1048576	; 0x100000
 801a21a:	f1a4 0401 	sub.w	r4, r4, #1
 801a21e:	d1e9      	bne.n	801a1f4 <__adddf3+0x10c>
 801a220:	f091 0f00 	teq	r1, #0
 801a224:	bf04      	itt	eq
 801a226:	4601      	moveq	r1, r0
 801a228:	2000      	moveq	r0, #0
 801a22a:	fab1 f381 	clz	r3, r1
 801a22e:	bf08      	it	eq
 801a230:	3320      	addeq	r3, #32
 801a232:	f1a3 030b 	sub.w	r3, r3, #11
 801a236:	f1b3 0220 	subs.w	r2, r3, #32
 801a23a:	da0c      	bge.n	801a256 <__adddf3+0x16e>
 801a23c:	320c      	adds	r2, #12
 801a23e:	dd08      	ble.n	801a252 <__adddf3+0x16a>
 801a240:	f102 0c14 	add.w	ip, r2, #20
 801a244:	f1c2 020c 	rsb	r2, r2, #12
 801a248:	fa01 f00c 	lsl.w	r0, r1, ip
 801a24c:	fa21 f102 	lsr.w	r1, r1, r2
 801a250:	e00c      	b.n	801a26c <__adddf3+0x184>
 801a252:	f102 0214 	add.w	r2, r2, #20
 801a256:	bfd8      	it	le
 801a258:	f1c2 0c20 	rsble	ip, r2, #32
 801a25c:	fa01 f102 	lsl.w	r1, r1, r2
 801a260:	fa20 fc0c 	lsr.w	ip, r0, ip
 801a264:	bfdc      	itt	le
 801a266:	ea41 010c 	orrle.w	r1, r1, ip
 801a26a:	4090      	lslle	r0, r2
 801a26c:	1ae4      	subs	r4, r4, r3
 801a26e:	bfa2      	ittt	ge
 801a270:	eb01 5104 	addge.w	r1, r1, r4, lsl #20
 801a274:	4329      	orrge	r1, r5
 801a276:	bd30      	popge	{r4, r5, pc}
 801a278:	ea6f 0404 	mvn.w	r4, r4
 801a27c:	3c1f      	subs	r4, #31
 801a27e:	da1c      	bge.n	801a2ba <__adddf3+0x1d2>
 801a280:	340c      	adds	r4, #12
 801a282:	dc0e      	bgt.n	801a2a2 <__adddf3+0x1ba>
 801a284:	f104 0414 	add.w	r4, r4, #20
 801a288:	f1c4 0220 	rsb	r2, r4, #32
 801a28c:	fa20 f004 	lsr.w	r0, r0, r4
 801a290:	fa01 f302 	lsl.w	r3, r1, r2
 801a294:	ea40 0003 	orr.w	r0, r0, r3
 801a298:	fa21 f304 	lsr.w	r3, r1, r4
 801a29c:	ea45 0103 	orr.w	r1, r5, r3
 801a2a0:	bd30      	pop	{r4, r5, pc}
 801a2a2:	f1c4 040c 	rsb	r4, r4, #12
 801a2a6:	f1c4 0220 	rsb	r2, r4, #32
 801a2aa:	fa20 f002 	lsr.w	r0, r0, r2
 801a2ae:	fa01 f304 	lsl.w	r3, r1, r4
 801a2b2:	ea40 0003 	orr.w	r0, r0, r3
 801a2b6:	4629      	mov	r1, r5
 801a2b8:	bd30      	pop	{r4, r5, pc}
 801a2ba:	fa21 f004 	lsr.w	r0, r1, r4
 801a2be:	4629      	mov	r1, r5
 801a2c0:	bd30      	pop	{r4, r5, pc}
 801a2c2:	f094 0f00 	teq	r4, #0
 801a2c6:	f483 1380 	eor.w	r3, r3, #1048576	; 0x100000
 801a2ca:	bf06      	itte	eq
 801a2cc:	f481 1180 	eoreq.w	r1, r1, #1048576	; 0x100000
 801a2d0:	3401      	addeq	r4, #1
 801a2d2:	3d01      	subne	r5, #1
 801a2d4:	e74e      	b.n	801a174 <__adddf3+0x8c>
 801a2d6:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 801a2da:	bf18      	it	ne
 801a2dc:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 801a2e0:	d029      	beq.n	801a336 <__adddf3+0x24e>
 801a2e2:	ea94 0f05 	teq	r4, r5
 801a2e6:	bf08      	it	eq
 801a2e8:	ea90 0f02 	teqeq	r0, r2
 801a2ec:	d005      	beq.n	801a2fa <__adddf3+0x212>
 801a2ee:	ea54 0c00 	orrs.w	ip, r4, r0
 801a2f2:	bf04      	itt	eq
 801a2f4:	4619      	moveq	r1, r3
 801a2f6:	4610      	moveq	r0, r2
 801a2f8:	bd30      	pop	{r4, r5, pc}
 801a2fa:	ea91 0f03 	teq	r1, r3
 801a2fe:	bf1e      	ittt	ne
 801a300:	2100      	movne	r1, #0
 801a302:	2000      	movne	r0, #0
 801a304:	bd30      	popne	{r4, r5, pc}
 801a306:	ea5f 5c54 	movs.w	ip, r4, lsr #21
 801a30a:	d105      	bne.n	801a318 <__adddf3+0x230>
 801a30c:	0040      	lsls	r0, r0, #1
 801a30e:	4149      	adcs	r1, r1
 801a310:	bf28      	it	cs
 801a312:	f041 4100 	orrcs.w	r1, r1, #2147483648	; 0x80000000
 801a316:	bd30      	pop	{r4, r5, pc}
 801a318:	f514 0480 	adds.w	r4, r4, #4194304	; 0x400000
 801a31c:	bf3c      	itt	cc
 801a31e:	f501 1180 	addcc.w	r1, r1, #1048576	; 0x100000
 801a322:	bd30      	popcc	{r4, r5, pc}
 801a324:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 801a328:	f045 41fe 	orr.w	r1, r5, #2130706432	; 0x7f000000
 801a32c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
 801a330:	f04f 0000 	mov.w	r0, #0
 801a334:	bd30      	pop	{r4, r5, pc}
 801a336:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 801a33a:	bf1a      	itte	ne
 801a33c:	4619      	movne	r1, r3
 801a33e:	4610      	movne	r0, r2
 801a340:	ea7f 5c65 	mvnseq.w	ip, r5, asr #21
 801a344:	bf1c      	itt	ne
 801a346:	460b      	movne	r3, r1
 801a348:	4602      	movne	r2, r0
 801a34a:	ea50 3401 	orrs.w	r4, r0, r1, lsl #12
 801a34e:	bf06      	itte	eq
 801a350:	ea52 3503 	orrseq.w	r5, r2, r3, lsl #12
 801a354:	ea91 0f03 	teqeq	r1, r3
 801a358:	f441 2100 	orrne.w	r1, r1, #524288	; 0x80000
 801a35c:	bd30      	pop	{r4, r5, pc}
 801a35e:	bf00      	nop

0801a360 <__aeabi_ui2d>:
 801a360:	f090 0f00 	teq	r0, #0
 801a364:	bf04      	itt	eq
 801a366:	2100      	moveq	r1, #0
 801a368:	4770      	bxeq	lr
 801a36a:	b530      	push	{r4, r5, lr}
 801a36c:	f44f 6480 	mov.w	r4, #1024	; 0x400
 801a370:	f104 0432 	add.w	r4, r4, #50	; 0x32
 801a374:	f04f 0500 	mov.w	r5, #0
 801a378:	f04f 0100 	mov.w	r1, #0
 801a37c:	e750      	b.n	801a220 <__adddf3+0x138>
 801a37e:	bf00      	nop

0801a380 <__aeabi_i2d>:
 801a380:	f090 0f00 	teq	r0, #0
 801a384:	bf04      	itt	eq
 801a386:	2100      	moveq	r1, #0
 801a388:	4770      	bxeq	lr
 801a38a:	b530      	push	{r4, r5, lr}
 801a38c:	f44f 6480 	mov.w	r4, #1024	; 0x400
 801a390:	f104 0432 	add.w	r4, r4, #50	; 0x32
 801a394:	f010 4500 	ands.w	r5, r0, #2147483648	; 0x80000000
 801a398:	bf48      	it	mi
 801a39a:	4240      	negmi	r0, r0
 801a39c:	f04f 0100 	mov.w	r1, #0
 801a3a0:	e73e      	b.n	801a220 <__adddf3+0x138>
 801a3a2:	bf00      	nop

0801a3a4 <__aeabi_f2d>:
 801a3a4:	0042      	lsls	r2, r0, #1
 801a3a6:	ea4f 01e2 	mov.w	r1, r2, asr #3
 801a3aa:	ea4f 0131 	mov.w	r1, r1, rrx
 801a3ae:	ea4f 7002 	mov.w	r0, r2, lsl #28
 801a3b2:	bf1f      	itttt	ne
 801a3b4:	f012 437f 	andsne.w	r3, r2, #4278190080	; 0xff000000
 801a3b8:	f093 4f7f 	teqne	r3, #4278190080	; 0xff000000
 801a3bc:	f081 5160 	eorne.w	r1, r1, #939524096	; 0x38000000
 801a3c0:	4770      	bxne	lr
 801a3c2:	f032 427f 	bics.w	r2, r2, #4278190080	; 0xff000000
 801a3c6:	bf08      	it	eq
 801a3c8:	4770      	bxeq	lr
 801a3ca:	f093 4f7f 	teq	r3, #4278190080	; 0xff000000
 801a3ce:	bf04      	itt	eq
 801a3d0:	f441 2100 	orreq.w	r1, r1, #524288	; 0x80000
 801a3d4:	4770      	bxeq	lr
 801a3d6:	b530      	push	{r4, r5, lr}
 801a3d8:	f44f 7460 	mov.w	r4, #896	; 0x380
 801a3dc:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 801a3e0:	f021 4100 	bic.w	r1, r1, #2147483648	; 0x80000000
 801a3e4:	e71c      	b.n	801a220 <__adddf3+0x138>
 801a3e6:	bf00      	nop

0801a3e8 <__aeabi_ul2d>:
 801a3e8:	ea50 0201 	orrs.w	r2, r0, r1
 801a3ec:	bf08      	it	eq
 801a3ee:	4770      	bxeq	lr
 801a3f0:	b530      	push	{r4, r5, lr}
 801a3f2:	f04f 0500 	mov.w	r5, #0
 801a3f6:	e00a      	b.n	801a40e <__aeabi_l2d+0x16>

0801a3f8 <__aeabi_l2d>:
 801a3f8:	ea50 0201 	orrs.w	r2, r0, r1
 801a3fc:	bf08      	it	eq
 801a3fe:	4770      	bxeq	lr
 801a400:	b530      	push	{r4, r5, lr}
 801a402:	f011 4500 	ands.w	r5, r1, #2147483648	; 0x80000000
 801a406:	d502      	bpl.n	801a40e <__aeabi_l2d+0x16>
 801a408:	4240      	negs	r0, r0
 801a40a:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 801a40e:	f44f 6480 	mov.w	r4, #1024	; 0x400
 801a412:	f104 0432 	add.w	r4, r4, #50	; 0x32
 801a416:	ea5f 5c91 	movs.w	ip, r1, lsr #22
 801a41a:	f43f aed8 	beq.w	801a1ce <__adddf3+0xe6>
 801a41e:	f04f 0203 	mov.w	r2, #3
 801a422:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 801a426:	bf18      	it	ne
 801a428:	3203      	addne	r2, #3
 801a42a:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 801a42e:	bf18      	it	ne
 801a430:	3203      	addne	r2, #3
 801a432:	eb02 02dc 	add.w	r2, r2, ip, lsr #3
 801a436:	f1c2 0320 	rsb	r3, r2, #32
 801a43a:	fa00 fc03 	lsl.w	ip, r0, r3
 801a43e:	fa20 f002 	lsr.w	r0, r0, r2
 801a442:	fa01 fe03 	lsl.w	lr, r1, r3
 801a446:	ea40 000e 	orr.w	r0, r0, lr
 801a44a:	fa21 f102 	lsr.w	r1, r1, r2
 801a44e:	4414      	add	r4, r2
 801a450:	e6bd      	b.n	801a1ce <__adddf3+0xe6>
 801a452:	bf00      	nop

0801a454 <__aeabi_dmul>:
 801a454:	b570      	push	{r4, r5, r6, lr}
 801a456:	f04f 0cff 	mov.w	ip, #255	; 0xff
 801a45a:	f44c 6ce0 	orr.w	ip, ip, #1792	; 0x700
 801a45e:	ea1c 5411 	ands.w	r4, ip, r1, lsr #20
 801a462:	bf1d      	ittte	ne
 801a464:	ea1c 5513 	andsne.w	r5, ip, r3, lsr #20
 801a468:	ea94 0f0c 	teqne	r4, ip
 801a46c:	ea95 0f0c 	teqne	r5, ip
 801a470:	f000 f8de 	bleq	801a630 <__aeabi_dmul+0x1dc>
 801a474:	442c      	add	r4, r5
 801a476:	ea81 0603 	eor.w	r6, r1, r3
 801a47a:	ea21 514c 	bic.w	r1, r1, ip, lsl #21
 801a47e:	ea23 534c 	bic.w	r3, r3, ip, lsl #21
 801a482:	ea50 3501 	orrs.w	r5, r0, r1, lsl #12
 801a486:	bf18      	it	ne
 801a488:	ea52 3503 	orrsne.w	r5, r2, r3, lsl #12
 801a48c:	f441 1180 	orr.w	r1, r1, #1048576	; 0x100000
 801a490:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
 801a494:	d038      	beq.n	801a508 <__aeabi_dmul+0xb4>
 801a496:	fba0 ce02 	umull	ip, lr, r0, r2
 801a49a:	f04f 0500 	mov.w	r5, #0
 801a49e:	fbe1 e502 	umlal	lr, r5, r1, r2
 801a4a2:	f006 4200 	and.w	r2, r6, #2147483648	; 0x80000000
 801a4a6:	fbe0 e503 	umlal	lr, r5, r0, r3
 801a4aa:	f04f 0600 	mov.w	r6, #0
 801a4ae:	fbe1 5603 	umlal	r5, r6, r1, r3
 801a4b2:	f09c 0f00 	teq	ip, #0
 801a4b6:	bf18      	it	ne
 801a4b8:	f04e 0e01 	orrne.w	lr, lr, #1
 801a4bc:	f1a4 04ff 	sub.w	r4, r4, #255	; 0xff
 801a4c0:	f5b6 7f00 	cmp.w	r6, #512	; 0x200
 801a4c4:	f564 7440 	sbc.w	r4, r4, #768	; 0x300
 801a4c8:	d204      	bcs.n	801a4d4 <__aeabi_dmul+0x80>
 801a4ca:	ea5f 0e4e 	movs.w	lr, lr, lsl #1
 801a4ce:	416d      	adcs	r5, r5
 801a4d0:	eb46 0606 	adc.w	r6, r6, r6
 801a4d4:	ea42 21c6 	orr.w	r1, r2, r6, lsl #11
 801a4d8:	ea41 5155 	orr.w	r1, r1, r5, lsr #21
 801a4dc:	ea4f 20c5 	mov.w	r0, r5, lsl #11
 801a4e0:	ea40 505e 	orr.w	r0, r0, lr, lsr #21
 801a4e4:	ea4f 2ece 	mov.w	lr, lr, lsl #11
 801a4e8:	f1b4 0cfd 	subs.w	ip, r4, #253	; 0xfd
 801a4ec:	bf88      	it	hi
 801a4ee:	f5bc 6fe0 	cmphi.w	ip, #1792	; 0x700
 801a4f2:	d81e      	bhi.n	801a532 <__aeabi_dmul+0xde>
 801a4f4:	f1be 4f00 	cmp.w	lr, #2147483648	; 0x80000000
 801a4f8:	bf08      	it	eq
 801a4fa:	ea5f 0e50 	movseq.w	lr, r0, lsr #1
 801a4fe:	f150 0000 	adcs.w	r0, r0, #0
 801a502:	eb41 5104 	adc.w	r1, r1, r4, lsl #20
 801a506:	bd70      	pop	{r4, r5, r6, pc}
 801a508:	f006 4600 	and.w	r6, r6, #2147483648	; 0x80000000
 801a50c:	ea46 0101 	orr.w	r1, r6, r1
 801a510:	ea40 0002 	orr.w	r0, r0, r2
 801a514:	ea81 0103 	eor.w	r1, r1, r3
 801a518:	ebb4 045c 	subs.w	r4, r4, ip, lsr #1
 801a51c:	bfc2      	ittt	gt
 801a51e:	ebd4 050c 	rsbsgt	r5, r4, ip
 801a522:	ea41 5104 	orrgt.w	r1, r1, r4, lsl #20
 801a526:	bd70      	popgt	{r4, r5, r6, pc}
 801a528:	f441 1180 	orr.w	r1, r1, #1048576	; 0x100000
 801a52c:	f04f 0e00 	mov.w	lr, #0
 801a530:	3c01      	subs	r4, #1
 801a532:	f300 80ab 	bgt.w	801a68c <__aeabi_dmul+0x238>
 801a536:	f114 0f36 	cmn.w	r4, #54	; 0x36
 801a53a:	bfde      	ittt	le
 801a53c:	2000      	movle	r0, #0
 801a53e:	f001 4100 	andle.w	r1, r1, #2147483648	; 0x80000000
 801a542:	bd70      	pople	{r4, r5, r6, pc}
 801a544:	f1c4 0400 	rsb	r4, r4, #0
 801a548:	3c20      	subs	r4, #32
 801a54a:	da35      	bge.n	801a5b8 <__aeabi_dmul+0x164>
 801a54c:	340c      	adds	r4, #12
 801a54e:	dc1b      	bgt.n	801a588 <__aeabi_dmul+0x134>
 801a550:	f104 0414 	add.w	r4, r4, #20
 801a554:	f1c4 0520 	rsb	r5, r4, #32
 801a558:	fa00 f305 	lsl.w	r3, r0, r5
 801a55c:	fa20 f004 	lsr.w	r0, r0, r4
 801a560:	fa01 f205 	lsl.w	r2, r1, r5
 801a564:	ea40 0002 	orr.w	r0, r0, r2
 801a568:	f001 4200 	and.w	r2, r1, #2147483648	; 0x80000000
 801a56c:	f021 4100 	bic.w	r1, r1, #2147483648	; 0x80000000
 801a570:	eb10 70d3 	adds.w	r0, r0, r3, lsr #31
 801a574:	fa21 f604 	lsr.w	r6, r1, r4
 801a578:	eb42 0106 	adc.w	r1, r2, r6
 801a57c:	ea5e 0e43 	orrs.w	lr, lr, r3, lsl #1
 801a580:	bf08      	it	eq
 801a582:	ea20 70d3 	biceq.w	r0, r0, r3, lsr #31
 801a586:	bd70      	pop	{r4, r5, r6, pc}
 801a588:	f1c4 040c 	rsb	r4, r4, #12
 801a58c:	f1c4 0520 	rsb	r5, r4, #32
 801a590:	fa00 f304 	lsl.w	r3, r0, r4
 801a594:	fa20 f005 	lsr.w	r0, r0, r5
 801a598:	fa01 f204 	lsl.w	r2, r1, r4
 801a59c:	ea40 0002 	orr.w	r0, r0, r2
 801a5a0:	f001 4100 	and.w	r1, r1, #2147483648	; 0x80000000
 801a5a4:	eb10 70d3 	adds.w	r0, r0, r3, lsr #31
 801a5a8:	f141 0100 	adc.w	r1, r1, #0
 801a5ac:	ea5e 0e43 	orrs.w	lr, lr, r3, lsl #1
 801a5b0:	bf08      	it	eq
 801a5b2:	ea20 70d3 	biceq.w	r0, r0, r3, lsr #31
 801a5b6:	bd70      	pop	{r4, r5, r6, pc}
 801a5b8:	f1c4 0520 	rsb	r5, r4, #32
 801a5bc:	fa00 f205 	lsl.w	r2, r0, r5
 801a5c0:	ea4e 0e02 	orr.w	lr, lr, r2
 801a5c4:	fa20 f304 	lsr.w	r3, r0, r4
 801a5c8:	fa01 f205 	lsl.w	r2, r1, r5
 801a5cc:	ea43 0302 	orr.w	r3, r3, r2
 801a5d0:	fa21 f004 	lsr.w	r0, r1, r4
 801a5d4:	f001 4100 	and.w	r1, r1, #2147483648	; 0x80000000
 801a5d8:	fa21 f204 	lsr.w	r2, r1, r4
 801a5dc:	ea20 0002 	bic.w	r0, r0, r2
 801a5e0:	eb00 70d3 	add.w	r0, r0, r3, lsr #31
 801a5e4:	ea5e 0e43 	orrs.w	lr, lr, r3, lsl #1
 801a5e8:	bf08      	it	eq
 801a5ea:	ea20 70d3 	biceq.w	r0, r0, r3, lsr #31
 801a5ee:	bd70      	pop	{r4, r5, r6, pc}
 801a5f0:	f094 0f00 	teq	r4, #0
 801a5f4:	d10f      	bne.n	801a616 <__aeabi_dmul+0x1c2>
 801a5f6:	f001 4600 	and.w	r6, r1, #2147483648	; 0x80000000
 801a5fa:	0040      	lsls	r0, r0, #1
 801a5fc:	eb41 0101 	adc.w	r1, r1, r1
 801a600:	f411 1f80 	tst.w	r1, #1048576	; 0x100000
 801a604:	bf08      	it	eq
 801a606:	3c01      	subeq	r4, #1
 801a608:	d0f7      	beq.n	801a5fa <__aeabi_dmul+0x1a6>
 801a60a:	ea41 0106 	orr.w	r1, r1, r6
 801a60e:	f095 0f00 	teq	r5, #0
 801a612:	bf18      	it	ne
 801a614:	4770      	bxne	lr
 801a616:	f003 4600 	and.w	r6, r3, #2147483648	; 0x80000000
 801a61a:	0052      	lsls	r2, r2, #1
 801a61c:	eb43 0303 	adc.w	r3, r3, r3
 801a620:	f413 1f80 	tst.w	r3, #1048576	; 0x100000
 801a624:	bf08      	it	eq
 801a626:	3d01      	subeq	r5, #1
 801a628:	d0f7      	beq.n	801a61a <__aeabi_dmul+0x1c6>
 801a62a:	ea43 0306 	orr.w	r3, r3, r6
 801a62e:	4770      	bx	lr
 801a630:	ea94 0f0c 	teq	r4, ip
 801a634:	ea0c 5513 	and.w	r5, ip, r3, lsr #20
 801a638:	bf18      	it	ne
 801a63a:	ea95 0f0c 	teqne	r5, ip
 801a63e:	d00c      	beq.n	801a65a <__aeabi_dmul+0x206>
 801a640:	ea50 0641 	orrs.w	r6, r0, r1, lsl #1
 801a644:	bf18      	it	ne
 801a646:	ea52 0643 	orrsne.w	r6, r2, r3, lsl #1
 801a64a:	d1d1      	bne.n	801a5f0 <__aeabi_dmul+0x19c>
 801a64c:	ea81 0103 	eor.w	r1, r1, r3
 801a650:	f001 4100 	and.w	r1, r1, #2147483648	; 0x80000000
 801a654:	f04f 0000 	mov.w	r0, #0
 801a658:	bd70      	pop	{r4, r5, r6, pc}
 801a65a:	ea50 0641 	orrs.w	r6, r0, r1, lsl #1
 801a65e:	bf06      	itte	eq
 801a660:	4610      	moveq	r0, r2
 801a662:	4619      	moveq	r1, r3
 801a664:	ea52 0643 	orrsne.w	r6, r2, r3, lsl #1
 801a668:	d019      	beq.n	801a69e <__aeabi_dmul+0x24a>
 801a66a:	ea94 0f0c 	teq	r4, ip
 801a66e:	d102      	bne.n	801a676 <__aeabi_dmul+0x222>
 801a670:	ea50 3601 	orrs.w	r6, r0, r1, lsl #12
 801a674:	d113      	bne.n	801a69e <__aeabi_dmul+0x24a>
 801a676:	ea95 0f0c 	teq	r5, ip
 801a67a:	d105      	bne.n	801a688 <__aeabi_dmul+0x234>
 801a67c:	ea52 3603 	orrs.w	r6, r2, r3, lsl #12
 801a680:	bf1c      	itt	ne
 801a682:	4610      	movne	r0, r2
 801a684:	4619      	movne	r1, r3
 801a686:	d10a      	bne.n	801a69e <__aeabi_dmul+0x24a>
 801a688:	ea81 0103 	eor.w	r1, r1, r3
 801a68c:	f001 4100 	and.w	r1, r1, #2147483648	; 0x80000000
 801a690:	f041 41fe 	orr.w	r1, r1, #2130706432	; 0x7f000000
 801a694:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
 801a698:	f04f 0000 	mov.w	r0, #0
 801a69c:	bd70      	pop	{r4, r5, r6, pc}
 801a69e:	f041 41fe 	orr.w	r1, r1, #2130706432	; 0x7f000000
 801a6a2:	f441 0178 	orr.w	r1, r1, #16252928	; 0xf80000
 801a6a6:	bd70      	pop	{r4, r5, r6, pc}

0801a6a8 <__aeabi_ddiv>:
 801a6a8:	b570      	push	{r4, r5, r6, lr}
 801a6aa:	f04f 0cff 	mov.w	ip, #255	; 0xff
 801a6ae:	f44c 6ce0 	orr.w	ip, ip, #1792	; 0x700
 801a6b2:	ea1c 5411 	ands.w	r4, ip, r1, lsr #20
 801a6b6:	bf1d      	ittte	ne
 801a6b8:	ea1c 5513 	andsne.w	r5, ip, r3, lsr #20
 801a6bc:	ea94 0f0c 	teqne	r4, ip
 801a6c0:	ea95 0f0c 	teqne	r5, ip
 801a6c4:	f000 f8a7 	bleq	801a816 <__aeabi_ddiv+0x16e>
 801a6c8:	eba4 0405 	sub.w	r4, r4, r5
 801a6cc:	ea81 0e03 	eor.w	lr, r1, r3
 801a6d0:	ea52 3503 	orrs.w	r5, r2, r3, lsl #12
 801a6d4:	ea4f 3101 	mov.w	r1, r1, lsl #12
 801a6d8:	f000 8088 	beq.w	801a7ec <__aeabi_ddiv+0x144>
 801a6dc:	ea4f 3303 	mov.w	r3, r3, lsl #12
 801a6e0:	f04f 5580 	mov.w	r5, #268435456	; 0x10000000
 801a6e4:	ea45 1313 	orr.w	r3, r5, r3, lsr #4
 801a6e8:	ea43 6312 	orr.w	r3, r3, r2, lsr #24
 801a6ec:	ea4f 2202 	mov.w	r2, r2, lsl #8
 801a6f0:	ea45 1511 	orr.w	r5, r5, r1, lsr #4
 801a6f4:	ea45 6510 	orr.w	r5, r5, r0, lsr #24
 801a6f8:	ea4f 2600 	mov.w	r6, r0, lsl #8
 801a6fc:	f00e 4100 	and.w	r1, lr, #2147483648	; 0x80000000
 801a700:	429d      	cmp	r5, r3
 801a702:	bf08      	it	eq
 801a704:	4296      	cmpeq	r6, r2
 801a706:	f144 04fd 	adc.w	r4, r4, #253	; 0xfd
 801a70a:	f504 7440 	add.w	r4, r4, #768	; 0x300
 801a70e:	d202      	bcs.n	801a716 <__aeabi_ddiv+0x6e>
 801a710:	085b      	lsrs	r3, r3, #1
 801a712:	ea4f 0232 	mov.w	r2, r2, rrx
 801a716:	1ab6      	subs	r6, r6, r2
 801a718:	eb65 0503 	sbc.w	r5, r5, r3
 801a71c:	085b      	lsrs	r3, r3, #1
 801a71e:	ea4f 0232 	mov.w	r2, r2, rrx
 801a722:	f44f 1080 	mov.w	r0, #1048576	; 0x100000
 801a726:	f44f 2c00 	mov.w	ip, #524288	; 0x80000
 801a72a:	ebb6 0e02 	subs.w	lr, r6, r2
 801a72e:	eb75 0e03 	sbcs.w	lr, r5, r3
 801a732:	bf22      	ittt	cs
 801a734:	1ab6      	subcs	r6, r6, r2
 801a736:	4675      	movcs	r5, lr
 801a738:	ea40 000c 	orrcs.w	r0, r0, ip
 801a73c:	085b      	lsrs	r3, r3, #1
 801a73e:	ea4f 0232 	mov.w	r2, r2, rrx
 801a742:	ebb6 0e02 	subs.w	lr, r6, r2
 801a746:	eb75 0e03 	sbcs.w	lr, r5, r3
 801a74a:	bf22      	ittt	cs
 801a74c:	1ab6      	subcs	r6, r6, r2
 801a74e:	4675      	movcs	r5, lr
 801a750:	ea40 005c 	orrcs.w	r0, r0, ip, lsr #1
 801a754:	085b      	lsrs	r3, r3, #1
 801a756:	ea4f 0232 	mov.w	r2, r2, rrx
 801a75a:	ebb6 0e02 	subs.w	lr, r6, r2
 801a75e:	eb75 0e03 	sbcs.w	lr, r5, r3
 801a762:	bf22      	ittt	cs
 801a764:	1ab6      	subcs	r6, r6, r2
 801a766:	4675      	movcs	r5, lr
 801a768:	ea40 009c 	orrcs.w	r0, r0, ip, lsr #2
 801a76c:	085b      	lsrs	r3, r3, #1
 801a76e:	ea4f 0232 	mov.w	r2, r2, rrx
 801a772:	ebb6 0e02 	subs.w	lr, r6, r2
 801a776:	eb75 0e03 	sbcs.w	lr, r5, r3
 801a77a:	bf22      	ittt	cs
 801a77c:	1ab6      	subcs	r6, r6, r2
 801a77e:	4675      	movcs	r5, lr
 801a780:	ea40 00dc 	orrcs.w	r0, r0, ip, lsr #3
 801a784:	ea55 0e06 	orrs.w	lr, r5, r6
 801a788:	d018      	beq.n	801a7bc <__aeabi_ddiv+0x114>
 801a78a:	ea4f 1505 	mov.w	r5, r5, lsl #4
 801a78e:	ea45 7516 	orr.w	r5, r5, r6, lsr #28
 801a792:	ea4f 1606 	mov.w	r6, r6, lsl #4
 801a796:	ea4f 03c3 	mov.w	r3, r3, lsl #3
 801a79a:	ea43 7352 	orr.w	r3, r3, r2, lsr #29
 801a79e:	ea4f 02c2 	mov.w	r2, r2, lsl #3
 801a7a2:	ea5f 1c1c 	movs.w	ip, ip, lsr #4
 801a7a6:	d1c0      	bne.n	801a72a <__aeabi_ddiv+0x82>
 801a7a8:	f411 1f80 	tst.w	r1, #1048576	; 0x100000
 801a7ac:	d10b      	bne.n	801a7c6 <__aeabi_ddiv+0x11e>
 801a7ae:	ea41 0100 	orr.w	r1, r1, r0
 801a7b2:	f04f 0000 	mov.w	r0, #0
 801a7b6:	f04f 4c00 	mov.w	ip, #2147483648	; 0x80000000
 801a7ba:	e7b6      	b.n	801a72a <__aeabi_ddiv+0x82>
 801a7bc:	f411 1f80 	tst.w	r1, #1048576	; 0x100000
 801a7c0:	bf04      	itt	eq
 801a7c2:	4301      	orreq	r1, r0
 801a7c4:	2000      	moveq	r0, #0
 801a7c6:	f1b4 0cfd 	subs.w	ip, r4, #253	; 0xfd
 801a7ca:	bf88      	it	hi
 801a7cc:	f5bc 6fe0 	cmphi.w	ip, #1792	; 0x700
 801a7d0:	f63f aeaf 	bhi.w	801a532 <__aeabi_dmul+0xde>
 801a7d4:	ebb5 0c03 	subs.w	ip, r5, r3
 801a7d8:	bf04      	itt	eq
 801a7da:	ebb6 0c02 	subseq.w	ip, r6, r2
 801a7de:	ea5f 0c50 	movseq.w	ip, r0, lsr #1
 801a7e2:	f150 0000 	adcs.w	r0, r0, #0
 801a7e6:	eb41 5104 	adc.w	r1, r1, r4, lsl #20
 801a7ea:	bd70      	pop	{r4, r5, r6, pc}
 801a7ec:	f00e 4e00 	and.w	lr, lr, #2147483648	; 0x80000000
 801a7f0:	ea4e 3111 	orr.w	r1, lr, r1, lsr #12
 801a7f4:	eb14 045c 	adds.w	r4, r4, ip, lsr #1
 801a7f8:	bfc2      	ittt	gt
 801a7fa:	ebd4 050c 	rsbsgt	r5, r4, ip
 801a7fe:	ea41 5104 	orrgt.w	r1, r1, r4, lsl #20
 801a802:	bd70      	popgt	{r4, r5, r6, pc}
 801a804:	f441 1180 	orr.w	r1, r1, #1048576	; 0x100000
 801a808:	f04f 0e00 	mov.w	lr, #0
 801a80c:	3c01      	subs	r4, #1
 801a80e:	e690      	b.n	801a532 <__aeabi_dmul+0xde>
 801a810:	ea45 0e06 	orr.w	lr, r5, r6
 801a814:	e68d      	b.n	801a532 <__aeabi_dmul+0xde>
 801a816:	ea0c 5513 	and.w	r5, ip, r3, lsr #20
 801a81a:	ea94 0f0c 	teq	r4, ip
 801a81e:	bf08      	it	eq
 801a820:	ea95 0f0c 	teqeq	r5, ip
 801a824:	f43f af3b 	beq.w	801a69e <__aeabi_dmul+0x24a>
 801a828:	ea94 0f0c 	teq	r4, ip
 801a82c:	d10a      	bne.n	801a844 <__aeabi_ddiv+0x19c>
 801a82e:	ea50 3401 	orrs.w	r4, r0, r1, lsl #12
 801a832:	f47f af34 	bne.w	801a69e <__aeabi_dmul+0x24a>
 801a836:	ea95 0f0c 	teq	r5, ip
 801a83a:	f47f af25 	bne.w	801a688 <__aeabi_dmul+0x234>
 801a83e:	4610      	mov	r0, r2
 801a840:	4619      	mov	r1, r3
 801a842:	e72c      	b.n	801a69e <__aeabi_dmul+0x24a>
 801a844:	ea95 0f0c 	teq	r5, ip
 801a848:	d106      	bne.n	801a858 <__aeabi_ddiv+0x1b0>
 801a84a:	ea52 3503 	orrs.w	r5, r2, r3, lsl #12
 801a84e:	f43f aefd 	beq.w	801a64c <__aeabi_dmul+0x1f8>
 801a852:	4610      	mov	r0, r2
 801a854:	4619      	mov	r1, r3
 801a856:	e722      	b.n	801a69e <__aeabi_dmul+0x24a>
 801a858:	ea50 0641 	orrs.w	r6, r0, r1, lsl #1
 801a85c:	bf18      	it	ne
 801a85e:	ea52 0643 	orrsne.w	r6, r2, r3, lsl #1
 801a862:	f47f aec5 	bne.w	801a5f0 <__aeabi_dmul+0x19c>
 801a866:	ea50 0441 	orrs.w	r4, r0, r1, lsl #1
 801a86a:	f47f af0d 	bne.w	801a688 <__aeabi_dmul+0x234>
 801a86e:	ea52 0543 	orrs.w	r5, r2, r3, lsl #1
 801a872:	f47f aeeb 	bne.w	801a64c <__aeabi_dmul+0x1f8>
 801a876:	e712      	b.n	801a69e <__aeabi_dmul+0x24a>

0801a878 <__gedf2>:
 801a878:	f04f 3cff 	mov.w	ip, #4294967295	; 0xffffffff
 801a87c:	e006      	b.n	801a88c <__cmpdf2+0x4>
 801a87e:	bf00      	nop

0801a880 <__ledf2>:
 801a880:	f04f 0c01 	mov.w	ip, #1
 801a884:	e002      	b.n	801a88c <__cmpdf2+0x4>
 801a886:	bf00      	nop

0801a888 <__cmpdf2>:
 801a888:	f04f 0c01 	mov.w	ip, #1
 801a88c:	f84d cd04 	str.w	ip, [sp, #-4]!
 801a890:	ea4f 0c41 	mov.w	ip, r1, lsl #1
 801a894:	ea7f 5c6c 	mvns.w	ip, ip, asr #21
 801a898:	ea4f 0c43 	mov.w	ip, r3, lsl #1
 801a89c:	bf18      	it	ne
 801a89e:	ea7f 5c6c 	mvnsne.w	ip, ip, asr #21
 801a8a2:	d01b      	beq.n	801a8dc <__cmpdf2+0x54>
 801a8a4:	b001      	add	sp, #4
 801a8a6:	ea50 0c41 	orrs.w	ip, r0, r1, lsl #1
 801a8aa:	bf0c      	ite	eq
 801a8ac:	ea52 0c43 	orrseq.w	ip, r2, r3, lsl #1
 801a8b0:	ea91 0f03 	teqne	r1, r3
 801a8b4:	bf02      	ittt	eq
 801a8b6:	ea90 0f02 	teqeq	r0, r2
 801a8ba:	2000      	moveq	r0, #0
 801a8bc:	4770      	bxeq	lr
 801a8be:	f110 0f00 	cmn.w	r0, #0
 801a8c2:	ea91 0f03 	teq	r1, r3
 801a8c6:	bf58      	it	pl
 801a8c8:	4299      	cmppl	r1, r3
 801a8ca:	bf08      	it	eq
 801a8cc:	4290      	cmpeq	r0, r2
 801a8ce:	bf2c      	ite	cs
 801a8d0:	17d8      	asrcs	r0, r3, #31
 801a8d2:	ea6f 70e3 	mvncc.w	r0, r3, asr #31
 801a8d6:	f040 0001 	orr.w	r0, r0, #1
 801a8da:	4770      	bx	lr
 801a8dc:	ea4f 0c41 	mov.w	ip, r1, lsl #1
 801a8e0:	ea7f 5c6c 	mvns.w	ip, ip, asr #21
 801a8e4:	d102      	bne.n	801a8ec <__cmpdf2+0x64>
 801a8e6:	ea50 3c01 	orrs.w	ip, r0, r1, lsl #12
 801a8ea:	d107      	bne.n	801a8fc <__cmpdf2+0x74>
 801a8ec:	ea4f 0c43 	mov.w	ip, r3, lsl #1
 801a8f0:	ea7f 5c6c 	mvns.w	ip, ip, asr #21
 801a8f4:	d1d6      	bne.n	801a8a4 <__cmpdf2+0x1c>
 801a8f6:	ea52 3c03 	orrs.w	ip, r2, r3, lsl #12
 801a8fa:	d0d3      	beq.n	801a8a4 <__cmpdf2+0x1c>
 801a8fc:	f85d 0b04 	ldr.w	r0, [sp], #4
 801a900:	4770      	bx	lr
 801a902:	bf00      	nop

0801a904 <__aeabi_cdrcmple>:
 801a904:	4684      	mov	ip, r0
 801a906:	4610      	mov	r0, r2
 801a908:	4662      	mov	r2, ip
 801a90a:	468c      	mov	ip, r1
 801a90c:	4619      	mov	r1, r3
 801a90e:	4663      	mov	r3, ip
 801a910:	e000      	b.n	801a914 <__aeabi_cdcmpeq>
 801a912:	bf00      	nop

0801a914 <__aeabi_cdcmpeq>:
 801a914:	b501      	push	{r0, lr}
 801a916:	f7ff ffb7 	bl	801a888 <__cmpdf2>
 801a91a:	2800      	cmp	r0, #0
 801a91c:	bf48      	it	mi
 801a91e:	f110 0f00 	cmnmi.w	r0, #0
 801a922:	bd01      	pop	{r0, pc}

0801a924 <__aeabi_dcmpeq>:
 801a924:	f84d ed08 	str.w	lr, [sp, #-8]!
 801a928:	f7ff fff4 	bl	801a914 <__aeabi_cdcmpeq>
 801a92c:	bf0c      	ite	eq
 801a92e:	2001      	moveq	r0, #1
 801a930:	2000      	movne	r0, #0
 801a932:	f85d fb08 	ldr.w	pc, [sp], #8
 801a936:	bf00      	nop

0801a938 <__aeabi_dcmplt>:
 801a938:	f84d ed08 	str.w	lr, [sp, #-8]!
 801a93c:	f7ff ffea 	bl	801a914 <__aeabi_cdcmpeq>
 801a940:	bf34      	ite	cc
 801a942:	2001      	movcc	r0, #1
 801a944:	2000      	movcs	r0, #0
 801a946:	f85d fb08 	ldr.w	pc, [sp], #8
 801a94a:	bf00      	nop

0801a94c <__aeabi_dcmple>:
 801a94c:	f84d ed08 	str.w	lr, [sp, #-8]!
 801a950:	f7ff ffe0 	bl	801a914 <__aeabi_cdcmpeq>
 801a954:	bf94      	ite	ls
 801a956:	2001      	movls	r0, #1
 801a958:	2000      	movhi	r0, #0
 801a95a:	f85d fb08 	ldr.w	pc, [sp], #8
 801a95e:	bf00      	nop

0801a960 <__aeabi_dcmpge>:
 801a960:	f84d ed08 	str.w	lr, [sp, #-8]!
 801a964:	f7ff ffce 	bl	801a904 <__aeabi_cdrcmple>
 801a968:	bf94      	ite	ls
 801a96a:	2001      	movls	r0, #1
 801a96c:	2000      	movhi	r0, #0
 801a96e:	f85d fb08 	ldr.w	pc, [sp], #8
 801a972:	bf00      	nop

0801a974 <__aeabi_dcmpgt>:
 801a974:	f84d ed08 	str.w	lr, [sp, #-8]!
 801a978:	f7ff ffc4 	bl	801a904 <__aeabi_cdrcmple>
 801a97c:	bf34      	ite	cc
 801a97e:	2001      	movcc	r0, #1
 801a980:	2000      	movcs	r0, #0
 801a982:	f85d fb08 	ldr.w	pc, [sp], #8
 801a986:	bf00      	nop

0801a988 <__aeabi_dcmpun>:
 801a988:	ea4f 0c41 	mov.w	ip, r1, lsl #1
 801a98c:	ea7f 5c6c 	mvns.w	ip, ip, asr #21
 801a990:	d102      	bne.n	801a998 <__aeabi_dcmpun+0x10>
 801a992:	ea50 3c01 	orrs.w	ip, r0, r1, lsl #12
 801a996:	d10a      	bne.n	801a9ae <__aeabi_dcmpun+0x26>
 801a998:	ea4f 0c43 	mov.w	ip, r3, lsl #1
 801a99c:	ea7f 5c6c 	mvns.w	ip, ip, asr #21
 801a9a0:	d102      	bne.n	801a9a8 <__aeabi_dcmpun+0x20>
 801a9a2:	ea52 3c03 	orrs.w	ip, r2, r3, lsl #12
 801a9a6:	d102      	bne.n	801a9ae <__aeabi_dcmpun+0x26>
 801a9a8:	f04f 0000 	mov.w	r0, #0
 801a9ac:	4770      	bx	lr
 801a9ae:	f04f 0001 	mov.w	r0, #1
 801a9b2:	4770      	bx	lr

0801a9b4 <__aeabi_d2iz>:
 801a9b4:	ea4f 0241 	mov.w	r2, r1, lsl #1
 801a9b8:	f512 1200 	adds.w	r2, r2, #2097152	; 0x200000
 801a9bc:	d215      	bcs.n	801a9ea <__aeabi_d2iz+0x36>
 801a9be:	d511      	bpl.n	801a9e4 <__aeabi_d2iz+0x30>
 801a9c0:	f46f 7378 	mvn.w	r3, #992	; 0x3e0
 801a9c4:	ebb3 5262 	subs.w	r2, r3, r2, asr #21
 801a9c8:	d912      	bls.n	801a9f0 <__aeabi_d2iz+0x3c>
 801a9ca:	ea4f 23c1 	mov.w	r3, r1, lsl #11
 801a9ce:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
 801a9d2:	ea43 5350 	orr.w	r3, r3, r0, lsr #21
 801a9d6:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 801a9da:	fa23 f002 	lsr.w	r0, r3, r2
 801a9de:	bf18      	it	ne
 801a9e0:	4240      	negne	r0, r0
 801a9e2:	4770      	bx	lr
 801a9e4:	f04f 0000 	mov.w	r0, #0
 801a9e8:	4770      	bx	lr
 801a9ea:	ea50 3001 	orrs.w	r0, r0, r1, lsl #12
 801a9ee:	d105      	bne.n	801a9fc <__aeabi_d2iz+0x48>
 801a9f0:	f011 4000 	ands.w	r0, r1, #2147483648	; 0x80000000
 801a9f4:	bf08      	it	eq
 801a9f6:	f06f 4000 	mvneq.w	r0, #2147483648	; 0x80000000
 801a9fa:	4770      	bx	lr
 801a9fc:	f04f 0000 	mov.w	r0, #0
 801aa00:	4770      	bx	lr
 801aa02:	bf00      	nop

0801aa04 <__aeabi_d2f>:
 801aa04:	ea4f 0241 	mov.w	r2, r1, lsl #1
 801aa08:	f1b2 43e0 	subs.w	r3, r2, #1879048192	; 0x70000000
 801aa0c:	bf24      	itt	cs
 801aa0e:	f5b3 1c00 	subscs.w	ip, r3, #2097152	; 0x200000
 801aa12:	f1dc 5cfe 	rsbscs	ip, ip, #532676608	; 0x1fc00000
 801aa16:	d90d      	bls.n	801aa34 <__aeabi_d2f+0x30>
 801aa18:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 801aa1c:	ea4f 02c0 	mov.w	r2, r0, lsl #3
 801aa20:	ea4c 7050 	orr.w	r0, ip, r0, lsr #29
 801aa24:	f1b2 4f00 	cmp.w	r2, #2147483648	; 0x80000000
 801aa28:	eb40 0083 	adc.w	r0, r0, r3, lsl #2
 801aa2c:	bf08      	it	eq
 801aa2e:	f020 0001 	biceq.w	r0, r0, #1
 801aa32:	4770      	bx	lr
 801aa34:	f011 4f80 	tst.w	r1, #1073741824	; 0x40000000
 801aa38:	d121      	bne.n	801aa7e <__aeabi_d2f+0x7a>
 801aa3a:	f113 7238 	adds.w	r2, r3, #48234496	; 0x2e00000
 801aa3e:	bfbc      	itt	lt
 801aa40:	f001 4000 	andlt.w	r0, r1, #2147483648	; 0x80000000
 801aa44:	4770      	bxlt	lr
 801aa46:	f441 1180 	orr.w	r1, r1, #1048576	; 0x100000
 801aa4a:	ea4f 5252 	mov.w	r2, r2, lsr #21
 801aa4e:	f1c2 0218 	rsb	r2, r2, #24
 801aa52:	f1c2 0c20 	rsb	ip, r2, #32
 801aa56:	fa10 f30c 	lsls.w	r3, r0, ip
 801aa5a:	fa20 f002 	lsr.w	r0, r0, r2
 801aa5e:	bf18      	it	ne
 801aa60:	f040 0001 	orrne.w	r0, r0, #1
 801aa64:	ea4f 23c1 	mov.w	r3, r1, lsl #11
 801aa68:	ea4f 23d3 	mov.w	r3, r3, lsr #11
 801aa6c:	fa03 fc0c 	lsl.w	ip, r3, ip
 801aa70:	ea40 000c 	orr.w	r0, r0, ip
 801aa74:	fa23 f302 	lsr.w	r3, r3, r2
 801aa78:	ea4f 0343 	mov.w	r3, r3, lsl #1
 801aa7c:	e7cc      	b.n	801aa18 <__aeabi_d2f+0x14>
 801aa7e:	ea7f 5362 	mvns.w	r3, r2, asr #21
 801aa82:	d107      	bne.n	801aa94 <__aeabi_d2f+0x90>
 801aa84:	ea50 3301 	orrs.w	r3, r0, r1, lsl #12
 801aa88:	bf1e      	ittt	ne
 801aa8a:	f04f 40fe 	movne.w	r0, #2130706432	; 0x7f000000
 801aa8e:	f440 0040 	orrne.w	r0, r0, #12582912	; 0xc00000
 801aa92:	4770      	bxne	lr
 801aa94:	f001 4000 	and.w	r0, r1, #2147483648	; 0x80000000
 801aa98:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 801aa9c:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 801aaa0:	4770      	bx	lr
 801aaa2:	bf00      	nop

0801aaa4 <__aeabi_frsub>:
 801aaa4:	f080 4000 	eor.w	r0, r0, #2147483648	; 0x80000000
 801aaa8:	e002      	b.n	801aab0 <__addsf3>
 801aaaa:	bf00      	nop

0801aaac <__aeabi_fsub>:
 801aaac:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000

0801aab0 <__addsf3>:
 801aab0:	0042      	lsls	r2, r0, #1
 801aab2:	bf1f      	itttt	ne
 801aab4:	ea5f 0341 	movsne.w	r3, r1, lsl #1
 801aab8:	ea92 0f03 	teqne	r2, r3
 801aabc:	ea7f 6c22 	mvnsne.w	ip, r2, asr #24
 801aac0:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 801aac4:	d06a      	beq.n	801ab9c <__addsf3+0xec>
 801aac6:	ea4f 6212 	mov.w	r2, r2, lsr #24
 801aaca:	ebd2 6313 	rsbs	r3, r2, r3, lsr #24
 801aace:	bfc1      	itttt	gt
 801aad0:	18d2      	addgt	r2, r2, r3
 801aad2:	4041      	eorgt	r1, r0
 801aad4:	4048      	eorgt	r0, r1
 801aad6:	4041      	eorgt	r1, r0
 801aad8:	bfb8      	it	lt
 801aada:	425b      	neglt	r3, r3
 801aadc:	2b19      	cmp	r3, #25
 801aade:	bf88      	it	hi
 801aae0:	4770      	bxhi	lr
 801aae2:	f010 4f00 	tst.w	r0, #2147483648	; 0x80000000
 801aae6:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 801aaea:	f020 407f 	bic.w	r0, r0, #4278190080	; 0xff000000
 801aaee:	bf18      	it	ne
 801aaf0:	4240      	negne	r0, r0
 801aaf2:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 801aaf6:	f441 0100 	orr.w	r1, r1, #8388608	; 0x800000
 801aafa:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 801aafe:	bf18      	it	ne
 801ab00:	4249      	negne	r1, r1
 801ab02:	ea92 0f03 	teq	r2, r3
 801ab06:	d03f      	beq.n	801ab88 <__addsf3+0xd8>
 801ab08:	f1a2 0201 	sub.w	r2, r2, #1
 801ab0c:	fa41 fc03 	asr.w	ip, r1, r3
 801ab10:	eb10 000c 	adds.w	r0, r0, ip
 801ab14:	f1c3 0320 	rsb	r3, r3, #32
 801ab18:	fa01 f103 	lsl.w	r1, r1, r3
 801ab1c:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 801ab20:	d502      	bpl.n	801ab28 <__addsf3+0x78>
 801ab22:	4249      	negs	r1, r1
 801ab24:	eb60 0040 	sbc.w	r0, r0, r0, lsl #1
 801ab28:	f5b0 0f00 	cmp.w	r0, #8388608	; 0x800000
 801ab2c:	d313      	bcc.n	801ab56 <__addsf3+0xa6>
 801ab2e:	f1b0 7f80 	cmp.w	r0, #16777216	; 0x1000000
 801ab32:	d306      	bcc.n	801ab42 <__addsf3+0x92>
 801ab34:	0840      	lsrs	r0, r0, #1
 801ab36:	ea4f 0131 	mov.w	r1, r1, rrx
 801ab3a:	f102 0201 	add.w	r2, r2, #1
 801ab3e:	2afe      	cmp	r2, #254	; 0xfe
 801ab40:	d251      	bcs.n	801abe6 <__addsf3+0x136>
 801ab42:	f1b1 4f00 	cmp.w	r1, #2147483648	; 0x80000000
 801ab46:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 801ab4a:	bf08      	it	eq
 801ab4c:	f020 0001 	biceq.w	r0, r0, #1
 801ab50:	ea40 0003 	orr.w	r0, r0, r3
 801ab54:	4770      	bx	lr
 801ab56:	0049      	lsls	r1, r1, #1
 801ab58:	eb40 0000 	adc.w	r0, r0, r0
 801ab5c:	f410 0f00 	tst.w	r0, #8388608	; 0x800000
 801ab60:	f1a2 0201 	sub.w	r2, r2, #1
 801ab64:	d1ed      	bne.n	801ab42 <__addsf3+0x92>
 801ab66:	fab0 fc80 	clz	ip, r0
 801ab6a:	f1ac 0c08 	sub.w	ip, ip, #8
 801ab6e:	ebb2 020c 	subs.w	r2, r2, ip
 801ab72:	fa00 f00c 	lsl.w	r0, r0, ip
 801ab76:	bfaa      	itet	ge
 801ab78:	eb00 50c2 	addge.w	r0, r0, r2, lsl #23
 801ab7c:	4252      	neglt	r2, r2
 801ab7e:	4318      	orrge	r0, r3
 801ab80:	bfbc      	itt	lt
 801ab82:	40d0      	lsrlt	r0, r2
 801ab84:	4318      	orrlt	r0, r3
 801ab86:	4770      	bx	lr
 801ab88:	f092 0f00 	teq	r2, #0
 801ab8c:	f481 0100 	eor.w	r1, r1, #8388608	; 0x800000
 801ab90:	bf06      	itte	eq
 801ab92:	f480 0000 	eoreq.w	r0, r0, #8388608	; 0x800000
 801ab96:	3201      	addeq	r2, #1
 801ab98:	3b01      	subne	r3, #1
 801ab9a:	e7b5      	b.n	801ab08 <__addsf3+0x58>
 801ab9c:	ea4f 0341 	mov.w	r3, r1, lsl #1
 801aba0:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 801aba4:	bf18      	it	ne
 801aba6:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 801abaa:	d021      	beq.n	801abf0 <__addsf3+0x140>
 801abac:	ea92 0f03 	teq	r2, r3
 801abb0:	d004      	beq.n	801abbc <__addsf3+0x10c>
 801abb2:	f092 0f00 	teq	r2, #0
 801abb6:	bf08      	it	eq
 801abb8:	4608      	moveq	r0, r1
 801abba:	4770      	bx	lr
 801abbc:	ea90 0f01 	teq	r0, r1
 801abc0:	bf1c      	itt	ne
 801abc2:	2000      	movne	r0, #0
 801abc4:	4770      	bxne	lr
 801abc6:	f012 4f7f 	tst.w	r2, #4278190080	; 0xff000000
 801abca:	d104      	bne.n	801abd6 <__addsf3+0x126>
 801abcc:	0040      	lsls	r0, r0, #1
 801abce:	bf28      	it	cs
 801abd0:	f040 4000 	orrcs.w	r0, r0, #2147483648	; 0x80000000
 801abd4:	4770      	bx	lr
 801abd6:	f112 7200 	adds.w	r2, r2, #33554432	; 0x2000000
 801abda:	bf3c      	itt	cc
 801abdc:	f500 0000 	addcc.w	r0, r0, #8388608	; 0x800000
 801abe0:	4770      	bxcc	lr
 801abe2:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 801abe6:	f043 40fe 	orr.w	r0, r3, #2130706432	; 0x7f000000
 801abea:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 801abee:	4770      	bx	lr
 801abf0:	ea7f 6222 	mvns.w	r2, r2, asr #24
 801abf4:	bf16      	itet	ne
 801abf6:	4608      	movne	r0, r1
 801abf8:	ea7f 6323 	mvnseq.w	r3, r3, asr #24
 801abfc:	4601      	movne	r1, r0
 801abfe:	0242      	lsls	r2, r0, #9
 801ac00:	bf06      	itte	eq
 801ac02:	ea5f 2341 	movseq.w	r3, r1, lsl #9
 801ac06:	ea90 0f01 	teqeq	r0, r1
 801ac0a:	f440 0080 	orrne.w	r0, r0, #4194304	; 0x400000
 801ac0e:	4770      	bx	lr

0801ac10 <__aeabi_ui2f>:
 801ac10:	f04f 0300 	mov.w	r3, #0
 801ac14:	e004      	b.n	801ac20 <__aeabi_i2f+0x8>
 801ac16:	bf00      	nop

0801ac18 <__aeabi_i2f>:
 801ac18:	f010 4300 	ands.w	r3, r0, #2147483648	; 0x80000000
 801ac1c:	bf48      	it	mi
 801ac1e:	4240      	negmi	r0, r0
 801ac20:	ea5f 0c00 	movs.w	ip, r0
 801ac24:	bf08      	it	eq
 801ac26:	4770      	bxeq	lr
 801ac28:	f043 4396 	orr.w	r3, r3, #1258291200	; 0x4b000000
 801ac2c:	4601      	mov	r1, r0
 801ac2e:	f04f 0000 	mov.w	r0, #0
 801ac32:	e01c      	b.n	801ac6e <__aeabi_l2f+0x2a>

0801ac34 <__aeabi_ul2f>:
 801ac34:	ea50 0201 	orrs.w	r2, r0, r1
 801ac38:	bf08      	it	eq
 801ac3a:	4770      	bxeq	lr
 801ac3c:	f04f 0300 	mov.w	r3, #0
 801ac40:	e00a      	b.n	801ac58 <__aeabi_l2f+0x14>
 801ac42:	bf00      	nop

0801ac44 <__aeabi_l2f>:
 801ac44:	ea50 0201 	orrs.w	r2, r0, r1
 801ac48:	bf08      	it	eq
 801ac4a:	4770      	bxeq	lr
 801ac4c:	f011 4300 	ands.w	r3, r1, #2147483648	; 0x80000000
 801ac50:	d502      	bpl.n	801ac58 <__aeabi_l2f+0x14>
 801ac52:	4240      	negs	r0, r0
 801ac54:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 801ac58:	ea5f 0c01 	movs.w	ip, r1
 801ac5c:	bf02      	ittt	eq
 801ac5e:	4684      	moveq	ip, r0
 801ac60:	4601      	moveq	r1, r0
 801ac62:	2000      	moveq	r0, #0
 801ac64:	f043 43b6 	orr.w	r3, r3, #1526726656	; 0x5b000000
 801ac68:	bf08      	it	eq
 801ac6a:	f1a3 5380 	subeq.w	r3, r3, #268435456	; 0x10000000
 801ac6e:	f5a3 0300 	sub.w	r3, r3, #8388608	; 0x800000
 801ac72:	fabc f28c 	clz	r2, ip
 801ac76:	3a08      	subs	r2, #8
 801ac78:	eba3 53c2 	sub.w	r3, r3, r2, lsl #23
 801ac7c:	db10      	blt.n	801aca0 <__aeabi_l2f+0x5c>
 801ac7e:	fa01 fc02 	lsl.w	ip, r1, r2
 801ac82:	4463      	add	r3, ip
 801ac84:	fa00 fc02 	lsl.w	ip, r0, r2
 801ac88:	f1c2 0220 	rsb	r2, r2, #32
 801ac8c:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 801ac90:	fa20 f202 	lsr.w	r2, r0, r2
 801ac94:	eb43 0002 	adc.w	r0, r3, r2
 801ac98:	bf08      	it	eq
 801ac9a:	f020 0001 	biceq.w	r0, r0, #1
 801ac9e:	4770      	bx	lr
 801aca0:	f102 0220 	add.w	r2, r2, #32
 801aca4:	fa01 fc02 	lsl.w	ip, r1, r2
 801aca8:	f1c2 0220 	rsb	r2, r2, #32
 801acac:	ea50 004c 	orrs.w	r0, r0, ip, lsl #1
 801acb0:	fa21 f202 	lsr.w	r2, r1, r2
 801acb4:	eb43 0002 	adc.w	r0, r3, r2
 801acb8:	bf08      	it	eq
 801acba:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 801acbe:	4770      	bx	lr

0801acc0 <__aeabi_fmul>:
 801acc0:	f04f 0cff 	mov.w	ip, #255	; 0xff
 801acc4:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 801acc8:	bf1e      	ittt	ne
 801acca:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 801acce:	ea92 0f0c 	teqne	r2, ip
 801acd2:	ea93 0f0c 	teqne	r3, ip
 801acd6:	d06f      	beq.n	801adb8 <__aeabi_fmul+0xf8>
 801acd8:	441a      	add	r2, r3
 801acda:	ea80 0c01 	eor.w	ip, r0, r1
 801acde:	0240      	lsls	r0, r0, #9
 801ace0:	bf18      	it	ne
 801ace2:	ea5f 2141 	movsne.w	r1, r1, lsl #9
 801ace6:	d01e      	beq.n	801ad26 <__aeabi_fmul+0x66>
 801ace8:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
 801acec:	ea43 1050 	orr.w	r0, r3, r0, lsr #5
 801acf0:	ea43 1151 	orr.w	r1, r3, r1, lsr #5
 801acf4:	fba0 3101 	umull	r3, r1, r0, r1
 801acf8:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 801acfc:	f5b1 0f00 	cmp.w	r1, #8388608	; 0x800000
 801ad00:	bf3e      	ittt	cc
 801ad02:	0049      	lslcc	r1, r1, #1
 801ad04:	ea41 71d3 	orrcc.w	r1, r1, r3, lsr #31
 801ad08:	005b      	lslcc	r3, r3, #1
 801ad0a:	ea40 0001 	orr.w	r0, r0, r1
 801ad0e:	f162 027f 	sbc.w	r2, r2, #127	; 0x7f
 801ad12:	2afd      	cmp	r2, #253	; 0xfd
 801ad14:	d81d      	bhi.n	801ad52 <__aeabi_fmul+0x92>
 801ad16:	f1b3 4f00 	cmp.w	r3, #2147483648	; 0x80000000
 801ad1a:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 801ad1e:	bf08      	it	eq
 801ad20:	f020 0001 	biceq.w	r0, r0, #1
 801ad24:	4770      	bx	lr
 801ad26:	f090 0f00 	teq	r0, #0
 801ad2a:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 801ad2e:	bf08      	it	eq
 801ad30:	0249      	lsleq	r1, r1, #9
 801ad32:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 801ad36:	ea40 2051 	orr.w	r0, r0, r1, lsr #9
 801ad3a:	3a7f      	subs	r2, #127	; 0x7f
 801ad3c:	bfc2      	ittt	gt
 801ad3e:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 801ad42:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 801ad46:	4770      	bxgt	lr
 801ad48:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 801ad4c:	f04f 0300 	mov.w	r3, #0
 801ad50:	3a01      	subs	r2, #1
 801ad52:	dc5d      	bgt.n	801ae10 <__aeabi_fmul+0x150>
 801ad54:	f112 0f19 	cmn.w	r2, #25
 801ad58:	bfdc      	itt	le
 801ad5a:	f000 4000 	andle.w	r0, r0, #2147483648	; 0x80000000
 801ad5e:	4770      	bxle	lr
 801ad60:	f1c2 0200 	rsb	r2, r2, #0
 801ad64:	0041      	lsls	r1, r0, #1
 801ad66:	fa21 f102 	lsr.w	r1, r1, r2
 801ad6a:	f1c2 0220 	rsb	r2, r2, #32
 801ad6e:	fa00 fc02 	lsl.w	ip, r0, r2
 801ad72:	ea5f 0031 	movs.w	r0, r1, rrx
 801ad76:	f140 0000 	adc.w	r0, r0, #0
 801ad7a:	ea53 034c 	orrs.w	r3, r3, ip, lsl #1
 801ad7e:	bf08      	it	eq
 801ad80:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 801ad84:	4770      	bx	lr
 801ad86:	f092 0f00 	teq	r2, #0
 801ad8a:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 801ad8e:	bf02      	ittt	eq
 801ad90:	0040      	lsleq	r0, r0, #1
 801ad92:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 801ad96:	3a01      	subeq	r2, #1
 801ad98:	d0f9      	beq.n	801ad8e <__aeabi_fmul+0xce>
 801ad9a:	ea40 000c 	orr.w	r0, r0, ip
 801ad9e:	f093 0f00 	teq	r3, #0
 801ada2:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 801ada6:	bf02      	ittt	eq
 801ada8:	0049      	lsleq	r1, r1, #1
 801adaa:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 801adae:	3b01      	subeq	r3, #1
 801adb0:	d0f9      	beq.n	801ada6 <__aeabi_fmul+0xe6>
 801adb2:	ea41 010c 	orr.w	r1, r1, ip
 801adb6:	e78f      	b.n	801acd8 <__aeabi_fmul+0x18>
 801adb8:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 801adbc:	ea92 0f0c 	teq	r2, ip
 801adc0:	bf18      	it	ne
 801adc2:	ea93 0f0c 	teqne	r3, ip
 801adc6:	d00a      	beq.n	801adde <__aeabi_fmul+0x11e>
 801adc8:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 801adcc:	bf18      	it	ne
 801adce:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 801add2:	d1d8      	bne.n	801ad86 <__aeabi_fmul+0xc6>
 801add4:	ea80 0001 	eor.w	r0, r0, r1
 801add8:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 801addc:	4770      	bx	lr
 801adde:	f090 0f00 	teq	r0, #0
 801ade2:	bf17      	itett	ne
 801ade4:	f090 4f00 	teqne	r0, #2147483648	; 0x80000000
 801ade8:	4608      	moveq	r0, r1
 801adea:	f091 0f00 	teqne	r1, #0
 801adee:	f091 4f00 	teqne	r1, #2147483648	; 0x80000000
 801adf2:	d014      	beq.n	801ae1e <__aeabi_fmul+0x15e>
 801adf4:	ea92 0f0c 	teq	r2, ip
 801adf8:	d101      	bne.n	801adfe <__aeabi_fmul+0x13e>
 801adfa:	0242      	lsls	r2, r0, #9
 801adfc:	d10f      	bne.n	801ae1e <__aeabi_fmul+0x15e>
 801adfe:	ea93 0f0c 	teq	r3, ip
 801ae02:	d103      	bne.n	801ae0c <__aeabi_fmul+0x14c>
 801ae04:	024b      	lsls	r3, r1, #9
 801ae06:	bf18      	it	ne
 801ae08:	4608      	movne	r0, r1
 801ae0a:	d108      	bne.n	801ae1e <__aeabi_fmul+0x15e>
 801ae0c:	ea80 0001 	eor.w	r0, r0, r1
 801ae10:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 801ae14:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 801ae18:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 801ae1c:	4770      	bx	lr
 801ae1e:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 801ae22:	f440 0040 	orr.w	r0, r0, #12582912	; 0xc00000
 801ae26:	4770      	bx	lr

0801ae28 <__aeabi_fdiv>:
 801ae28:	f04f 0cff 	mov.w	ip, #255	; 0xff
 801ae2c:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 801ae30:	bf1e      	ittt	ne
 801ae32:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 801ae36:	ea92 0f0c 	teqne	r2, ip
 801ae3a:	ea93 0f0c 	teqne	r3, ip
 801ae3e:	d069      	beq.n	801af14 <__aeabi_fdiv+0xec>
 801ae40:	eba2 0203 	sub.w	r2, r2, r3
 801ae44:	ea80 0c01 	eor.w	ip, r0, r1
 801ae48:	0249      	lsls	r1, r1, #9
 801ae4a:	ea4f 2040 	mov.w	r0, r0, lsl #9
 801ae4e:	d037      	beq.n	801aec0 <__aeabi_fdiv+0x98>
 801ae50:	f04f 5380 	mov.w	r3, #268435456	; 0x10000000
 801ae54:	ea43 1111 	orr.w	r1, r3, r1, lsr #4
 801ae58:	ea43 1310 	orr.w	r3, r3, r0, lsr #4
 801ae5c:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 801ae60:	428b      	cmp	r3, r1
 801ae62:	bf38      	it	cc
 801ae64:	005b      	lslcc	r3, r3, #1
 801ae66:	f142 027d 	adc.w	r2, r2, #125	; 0x7d
 801ae6a:	f44f 0c00 	mov.w	ip, #8388608	; 0x800000
 801ae6e:	428b      	cmp	r3, r1
 801ae70:	bf24      	itt	cs
 801ae72:	1a5b      	subcs	r3, r3, r1
 801ae74:	ea40 000c 	orrcs.w	r0, r0, ip
 801ae78:	ebb3 0f51 	cmp.w	r3, r1, lsr #1
 801ae7c:	bf24      	itt	cs
 801ae7e:	eba3 0351 	subcs.w	r3, r3, r1, lsr #1
 801ae82:	ea40 005c 	orrcs.w	r0, r0, ip, lsr #1
 801ae86:	ebb3 0f91 	cmp.w	r3, r1, lsr #2
 801ae8a:	bf24      	itt	cs
 801ae8c:	eba3 0391 	subcs.w	r3, r3, r1, lsr #2
 801ae90:	ea40 009c 	orrcs.w	r0, r0, ip, lsr #2
 801ae94:	ebb3 0fd1 	cmp.w	r3, r1, lsr #3
 801ae98:	bf24      	itt	cs
 801ae9a:	eba3 03d1 	subcs.w	r3, r3, r1, lsr #3
 801ae9e:	ea40 00dc 	orrcs.w	r0, r0, ip, lsr #3
 801aea2:	011b      	lsls	r3, r3, #4
 801aea4:	bf18      	it	ne
 801aea6:	ea5f 1c1c 	movsne.w	ip, ip, lsr #4
 801aeaa:	d1e0      	bne.n	801ae6e <__aeabi_fdiv+0x46>
 801aeac:	2afd      	cmp	r2, #253	; 0xfd
 801aeae:	f63f af50 	bhi.w	801ad52 <__aeabi_fmul+0x92>
 801aeb2:	428b      	cmp	r3, r1
 801aeb4:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 801aeb8:	bf08      	it	eq
 801aeba:	f020 0001 	biceq.w	r0, r0, #1
 801aebe:	4770      	bx	lr
 801aec0:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 801aec4:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 801aec8:	327f      	adds	r2, #127	; 0x7f
 801aeca:	bfc2      	ittt	gt
 801aecc:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 801aed0:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 801aed4:	4770      	bxgt	lr
 801aed6:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 801aeda:	f04f 0300 	mov.w	r3, #0
 801aede:	3a01      	subs	r2, #1
 801aee0:	e737      	b.n	801ad52 <__aeabi_fmul+0x92>
 801aee2:	f092 0f00 	teq	r2, #0
 801aee6:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 801aeea:	bf02      	ittt	eq
 801aeec:	0040      	lsleq	r0, r0, #1
 801aeee:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 801aef2:	3a01      	subeq	r2, #1
 801aef4:	d0f9      	beq.n	801aeea <__aeabi_fdiv+0xc2>
 801aef6:	ea40 000c 	orr.w	r0, r0, ip
 801aefa:	f093 0f00 	teq	r3, #0
 801aefe:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 801af02:	bf02      	ittt	eq
 801af04:	0049      	lsleq	r1, r1, #1
 801af06:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 801af0a:	3b01      	subeq	r3, #1
 801af0c:	d0f9      	beq.n	801af02 <__aeabi_fdiv+0xda>
 801af0e:	ea41 010c 	orr.w	r1, r1, ip
 801af12:	e795      	b.n	801ae40 <__aeabi_fdiv+0x18>
 801af14:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 801af18:	ea92 0f0c 	teq	r2, ip
 801af1c:	d108      	bne.n	801af30 <__aeabi_fdiv+0x108>
 801af1e:	0242      	lsls	r2, r0, #9
 801af20:	f47f af7d 	bne.w	801ae1e <__aeabi_fmul+0x15e>
 801af24:	ea93 0f0c 	teq	r3, ip
 801af28:	f47f af70 	bne.w	801ae0c <__aeabi_fmul+0x14c>
 801af2c:	4608      	mov	r0, r1
 801af2e:	e776      	b.n	801ae1e <__aeabi_fmul+0x15e>
 801af30:	ea93 0f0c 	teq	r3, ip
 801af34:	d104      	bne.n	801af40 <__aeabi_fdiv+0x118>
 801af36:	024b      	lsls	r3, r1, #9
 801af38:	f43f af4c 	beq.w	801add4 <__aeabi_fmul+0x114>
 801af3c:	4608      	mov	r0, r1
 801af3e:	e76e      	b.n	801ae1e <__aeabi_fmul+0x15e>
 801af40:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 801af44:	bf18      	it	ne
 801af46:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 801af4a:	d1ca      	bne.n	801aee2 <__aeabi_fdiv+0xba>
 801af4c:	f030 4200 	bics.w	r2, r0, #2147483648	; 0x80000000
 801af50:	f47f af5c 	bne.w	801ae0c <__aeabi_fmul+0x14c>
 801af54:	f031 4300 	bics.w	r3, r1, #2147483648	; 0x80000000
 801af58:	f47f af3c 	bne.w	801add4 <__aeabi_fmul+0x114>
 801af5c:	e75f      	b.n	801ae1e <__aeabi_fmul+0x15e>
 801af5e:	bf00      	nop

0801af60 <__errno>:
 801af60:	4b01      	ldr	r3, [pc, #4]	; (801af68 <__errno+0x8>)
 801af62:	6818      	ldr	r0, [r3, #0]
 801af64:	4770      	bx	lr
 801af66:	bf00      	nop
 801af68:	200050b4 	.word	0x200050b4
 801af6c:	5f633269 	.word	0x5f633269
 801af70:	74696e69 	.word	0x74696e69
 801af74:	6f6e000a 	.word	0x6f6e000a
 801af78:	6f662074 	.word	0x6f662074
 801af7c:	0a646e75 	.word	0x0a646e75
 801af80:	253d5400 	.word	0x253d5400
 801af84:	3d502066 	.word	0x3d502066
 801af88:	000a6425 	.word	0x000a6425
 801af8c:	00776f70 	.word	0x00776f70
	...

0801af98 <TWO52>:
 801af98:	00000000 43300000 00000000 c3300000     ......0C......0.

0801afa8 <_global_impure_ptr>:
 801afa8:	200050b8                                .P. 

Disassembly of section .plt:

0801b4ac <.plt>:
 801b4ac:	b500      	push	{lr}
 801b4ae:	f8df e008 	ldr.w	lr, [pc, #8]	; 801b4b8 <.plt+0xc>
 801b4b2:	44fe      	add	lr, pc
 801b4b4:	f85e ff08 	ldr.w	pc, [lr, #8]!
 801b4b8:	17fea034 	.word	0x17fea034

0801b4bc <_ZN4BIOS3DBG5PrintEPKcz@plt>:
 801b4bc:	f24a 0c30 	movw	ip, #41008	; 0xa030
 801b4c0:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801b4c4:	44fc      	add	ip, pc
 801b4c6:	f8dc f000 	ldr.w	pc, [ip]
 801b4ca:	bf00      	nop

0801b4cc <_ZN4BIOS3KEY6GetKeyEv@plt>:
 801b4cc:	f24a 0c24 	movw	ip, #40996	; 0xa024
 801b4d0:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801b4d4:	44fc      	add	ip, pc
 801b4d6:	f8dc f000 	ldr.w	pc, [ip]
 801b4da:	bf00      	nop

0801b4dc <_ZN4BIOS3LCD5ClearEt@plt>:
 801b4dc:	f24a 0c18 	movw	ip, #40984	; 0xa018
 801b4e0:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801b4e4:	44fc      	add	ip, pc
 801b4e6:	f8dc f000 	ldr.w	pc, [ip]
 801b4ea:	bf00      	nop

0801b4ec <_ZN4BIOS3SYS7DelayMsEi@plt>:
 801b4ec:	0000      	movs	r0, r0
 801b4ee:	0000      	movs	r0, r0
 801b4f0:	f24a 0c08 	movw	ip, #40968	; 0xa008
 801b4f4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801b4f8:	44fc      	add	ip, pc
 801b4fa:	f8dc f000 	ldr.w	pc, [ip]
 801b4fe:	bf00      	nop
