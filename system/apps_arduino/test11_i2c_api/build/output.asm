
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08018000 <main>:
 8018000:	b5f0      	push	{r4, r5, r6, r7, lr}
 8018002:	f242 1004 	movw	r0, #8452	; 0x2104
 8018006:	b089      	sub	sp, #36	; 0x24
 8018008:	f000 fe4e 	bl	8018ca8 <_ZN4BIOS3LCD5ClearEt@plt+0x4>
 801800c:	4815      	ldr	r0, [pc, #84]	; (8018064 <main+0x64>)
 801800e:	f000 fe33 	bl	8018c78 <_ZN4BIOS3DBG5PrintEPKcz@plt+0x4>
 8018012:	4c15      	ldr	r4, [pc, #84]	; (8018068 <main+0x68>)
 8018014:	4f15      	ldr	r7, [pc, #84]	; (801806c <main+0x6c>)
 8018016:	f000 fe37 	bl	8018c88 <_ZN4BIOS3KEY6GetKeyEv@plt+0x4>
 801801a:	2806      	cmp	r0, #6
 801801c:	d01e      	beq.n	801805c <main+0x5c>
 801801e:	2103      	movs	r1, #3
 8018020:	a802      	add	r0, sp, #8
 8018022:	f000 f86c 	bl	80180fe <_ZN15Adafruit_BMP0855beginEh>
 8018026:	b918      	cbnz	r0, 8018030 <main+0x30>
 8018028:	4638      	mov	r0, r7
 801802a:	f000 fe25 	bl	8018c78 <_ZN4BIOS3DBG5PrintEPKcz@plt+0x4>
 801802e:	e7f2      	b.n	8018016 <main+0x16>
 8018030:	a802      	add	r0, sp, #8
 8018032:	f000 f8d2 	bl	80181da <_ZN15Adafruit_BMP08515readTemperatureEv>
 8018036:	4606      	mov	r6, r0
 8018038:	a802      	add	r0, sp, #8
 801803a:	f000 f908 	bl	801824e <_ZN15Adafruit_BMP08512readPressureEv>
 801803e:	4605      	mov	r5, r0
 8018040:	4630      	mov	r0, r6
 8018042:	f000 fabd 	bl	80185c0 <__aeabi_f2d>
 8018046:	9500      	str	r5, [sp, #0]
 8018048:	4602      	mov	r2, r0
 801804a:	460b      	mov	r3, r1
 801804c:	4620      	mov	r0, r4
 801804e:	f000 fe13 	bl	8018c78 <_ZN4BIOS3DBG5PrintEPKcz@plt+0x4>
 8018052:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
 8018056:	f000 fe39 	bl	8018ccc <_ZN4BIOS3SYS7DelayMsEi@plt+0x8>
 801805a:	e7dc      	b.n	8018016 <main+0x16>
 801805c:	2000      	movs	r0, #0
 801805e:	b009      	add	sp, #36	; 0x24
 8018060:	bdf0      	pop	{r4, r5, r6, r7, pc}
 8018062:	bf00      	nop
 8018064:	08018b2c 	.word	0x08018b2c
 8018068:	08018b3e 	.word	0x08018b3e
 801806c:	08018b33 	.word	0x08018b33

08018070 <_ZN7Arduino5delayEi>:
 8018070:	f000 be2c 	b.w	8018ccc <_ZN4BIOS3SYS7DelayMsEi@plt+0x8>

08018074 <_ZN15Adafruit_BMP0859computeB5El>:
 8018074:	8ac3      	ldrh	r3, [r0, #22]
 8018076:	1acb      	subs	r3, r1, r3
 8018078:	8a81      	ldrh	r1, [r0, #20]
 801807a:	4359      	muls	r1, r3
 801807c:	f9b0 300e 	ldrsh.w	r3, [r0, #14]
 8018080:	f9b0 0010 	ldrsh.w	r0, [r0, #16]
 8018084:	13c9      	asrs	r1, r1, #15
 8018086:	02db      	lsls	r3, r3, #11
 8018088:	4408      	add	r0, r1
 801808a:	fb93 f0f0 	sdiv	r0, r3, r0
 801808e:	4408      	add	r0, r1
 8018090:	4770      	bx	lr

08018092 <_ZN15Adafruit_BMP0855read8Eh>:
 8018092:	b510      	push	{r4, lr}
 8018094:	460c      	mov	r4, r1
 8018096:	2077      	movs	r0, #119	; 0x77
 8018098:	f000 fdcc 	bl	8018c34 <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt>
 801809c:	4620      	mov	r0, r4
 801809e:	f000 fddb 	bl	8018c58 <_ZN4BIOS4GPIO3I2C5WriteEh@plt+0x4>
 80180a2:	f000 fdd1 	bl	8018c48 <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt+0x4>
 80180a6:	2077      	movs	r0, #119	; 0x77
 80180a8:	f000 fdc4 	bl	8018c34 <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt>
 80180ac:	2101      	movs	r1, #1
 80180ae:	2077      	movs	r0, #119	; 0x77
 80180b0:	f000 fdda 	bl	8018c68 <_ZN4BIOS4GPIO3I2C11RequestFromEhh@plt+0x4>
 80180b4:	f000 fdf0 	bl	8018c98 <_ZN4BIOS4GPIO3I2C4ReadEv@plt+0x4>
 80180b8:	4604      	mov	r4, r0
 80180ba:	f000 fdc5 	bl	8018c48 <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt+0x4>
 80180be:	4620      	mov	r0, r4
 80180c0:	bd10      	pop	{r4, pc}

080180c2 <_ZN15Adafruit_BMP0856read16Eh>:
 80180c2:	b510      	push	{r4, lr}
 80180c4:	460c      	mov	r4, r1
 80180c6:	2077      	movs	r0, #119	; 0x77
 80180c8:	f000 fdb4 	bl	8018c34 <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt>
 80180cc:	4620      	mov	r0, r4
 80180ce:	f000 fdc3 	bl	8018c58 <_ZN4BIOS4GPIO3I2C5WriteEh@plt+0x4>
 80180d2:	f000 fdb9 	bl	8018c48 <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt+0x4>
 80180d6:	2077      	movs	r0, #119	; 0x77
 80180d8:	f000 fdac 	bl	8018c34 <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt>
 80180dc:	2102      	movs	r1, #2
 80180de:	2077      	movs	r0, #119	; 0x77
 80180e0:	f000 fdc2 	bl	8018c68 <_ZN4BIOS4GPIO3I2C11RequestFromEhh@plt+0x4>
 80180e4:	f000 fdd8 	bl	8018c98 <_ZN4BIOS4GPIO3I2C4ReadEv@plt+0x4>
 80180e8:	0200      	lsls	r0, r0, #8
 80180ea:	f400 447f 	and.w	r4, r0, #65280	; 0xff00
 80180ee:	f000 fdd3 	bl	8018c98 <_ZN4BIOS4GPIO3I2C4ReadEv@plt+0x4>
 80180f2:	4304      	orrs	r4, r0
 80180f4:	b2a4      	uxth	r4, r4
 80180f6:	f000 fda7 	bl	8018c48 <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt+0x4>
 80180fa:	4620      	mov	r0, r4
 80180fc:	bd10      	pop	{r4, pc}

080180fe <_ZN15Adafruit_BMP0855beginEh>:
 80180fe:	2903      	cmp	r1, #3
 8018100:	bf28      	it	cs
 8018102:	2103      	movcs	r1, #3
 8018104:	b510      	push	{r4, lr}
 8018106:	4604      	mov	r4, r0
 8018108:	7001      	strb	r1, [r0, #0]
 801810a:	2120      	movs	r1, #32
 801810c:	2000      	movs	r0, #0
 801810e:	f000 fdd3 	bl	8018cb8 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt+0x4>
 8018112:	2120      	movs	r1, #32
 8018114:	2001      	movs	r0, #1
 8018116:	f000 fdcf 	bl	8018cb8 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt+0x4>
 801811a:	21d0      	movs	r1, #208	; 0xd0
 801811c:	4620      	mov	r0, r4
 801811e:	f7ff ffb8 	bl	8018092 <_ZN15Adafruit_BMP0855read8Eh>
 8018122:	2855      	cmp	r0, #85	; 0x55
 8018124:	d138      	bne.n	8018198 <_ZN15Adafruit_BMP0855beginEh+0x9a>
 8018126:	21aa      	movs	r1, #170	; 0xaa
 8018128:	4620      	mov	r0, r4
 801812a:	f7ff ffca 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 801812e:	21ac      	movs	r1, #172	; 0xac
 8018130:	8060      	strh	r0, [r4, #2]
 8018132:	4620      	mov	r0, r4
 8018134:	f7ff ffc5 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018138:	21ae      	movs	r1, #174	; 0xae
 801813a:	80a0      	strh	r0, [r4, #4]
 801813c:	4620      	mov	r0, r4
 801813e:	f7ff ffc0 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018142:	21b0      	movs	r1, #176	; 0xb0
 8018144:	80e0      	strh	r0, [r4, #6]
 8018146:	4620      	mov	r0, r4
 8018148:	f7ff ffbb 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 801814c:	21b2      	movs	r1, #178	; 0xb2
 801814e:	8260      	strh	r0, [r4, #18]
 8018150:	4620      	mov	r0, r4
 8018152:	f7ff ffb6 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018156:	21b4      	movs	r1, #180	; 0xb4
 8018158:	82a0      	strh	r0, [r4, #20]
 801815a:	4620      	mov	r0, r4
 801815c:	f7ff ffb1 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018160:	21b6      	movs	r1, #182	; 0xb6
 8018162:	82e0      	strh	r0, [r4, #22]
 8018164:	4620      	mov	r0, r4
 8018166:	f7ff ffac 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 801816a:	21b8      	movs	r1, #184	; 0xb8
 801816c:	8120      	strh	r0, [r4, #8]
 801816e:	4620      	mov	r0, r4
 8018170:	f7ff ffa7 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018174:	21ba      	movs	r1, #186	; 0xba
 8018176:	8160      	strh	r0, [r4, #10]
 8018178:	4620      	mov	r0, r4
 801817a:	f7ff ffa2 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 801817e:	21bc      	movs	r1, #188	; 0xbc
 8018180:	81a0      	strh	r0, [r4, #12]
 8018182:	4620      	mov	r0, r4
 8018184:	f7ff ff9d 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018188:	21be      	movs	r1, #190	; 0xbe
 801818a:	81e0      	strh	r0, [r4, #14]
 801818c:	4620      	mov	r0, r4
 801818e:	f7ff ff98 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018192:	8220      	strh	r0, [r4, #16]
 8018194:	2001      	movs	r0, #1
 8018196:	bd10      	pop	{r4, pc}
 8018198:	2000      	movs	r0, #0
 801819a:	e7fc      	b.n	8018196 <_ZN15Adafruit_BMP0855beginEh+0x98>

0801819c <_ZN15Adafruit_BMP0856write8Ehh>:
 801819c:	b538      	push	{r3, r4, r5, lr}
 801819e:	460d      	mov	r5, r1
 80181a0:	4614      	mov	r4, r2
 80181a2:	2077      	movs	r0, #119	; 0x77
 80181a4:	f000 fd46 	bl	8018c34 <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt>
 80181a8:	4628      	mov	r0, r5
 80181aa:	f000 fd55 	bl	8018c58 <_ZN4BIOS4GPIO3I2C5WriteEh@plt+0x4>
 80181ae:	4620      	mov	r0, r4
 80181b0:	f000 fd52 	bl	8018c58 <_ZN4BIOS4GPIO3I2C5WriteEh@plt+0x4>
 80181b4:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 80181b8:	f000 bd46 	b.w	8018c48 <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt+0x4>

080181bc <_ZN15Adafruit_BMP08518readRawTemperatureEv>:
 80181bc:	b510      	push	{r4, lr}
 80181be:	4604      	mov	r4, r0
 80181c0:	21f4      	movs	r1, #244	; 0xf4
 80181c2:	222e      	movs	r2, #46	; 0x2e
 80181c4:	f7ff ffea 	bl	801819c <_ZN15Adafruit_BMP0856write8Ehh>
 80181c8:	2005      	movs	r0, #5
 80181ca:	f000 fd7f 	bl	8018ccc <_ZN4BIOS3SYS7DelayMsEi@plt+0x8>
 80181ce:	4620      	mov	r0, r4
 80181d0:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 80181d4:	21f6      	movs	r1, #246	; 0xf6
 80181d6:	f7ff bf74 	b.w	80180c2 <_ZN15Adafruit_BMP0856read16Eh>

080181da <_ZN15Adafruit_BMP08515readTemperatureEv>:
 80181da:	b510      	push	{r4, lr}
 80181dc:	4604      	mov	r4, r0
 80181de:	f7ff ffed 	bl	80181bc <_ZN15Adafruit_BMP08518readRawTemperatureEv>
 80181e2:	4601      	mov	r1, r0
 80181e4:	4620      	mov	r0, r4
 80181e6:	f7ff ff45 	bl	8018074 <_ZN15Adafruit_BMP0859computeB5El>
 80181ea:	3008      	adds	r0, #8
 80181ec:	1100      	asrs	r0, r0, #4
 80181ee:	f000 faf9 	bl	80187e4 <__aeabi_i2f>
 80181f2:	4902      	ldr	r1, [pc, #8]	; (80181fc <_ZN15Adafruit_BMP08515readTemperatureEv+0x22>)
 80181f4:	f000 fbfe 	bl	80189f4 <__aeabi_fdiv>
 80181f8:	bd10      	pop	{r4, pc}
 80181fa:	bf00      	nop
 80181fc:	41200000 	.word	0x41200000

08018200 <_ZN15Adafruit_BMP08515readRawPressureEv>:
 8018200:	b538      	push	{r3, r4, r5, lr}
 8018202:	4604      	mov	r4, r0
 8018204:	7802      	ldrb	r2, [r0, #0]
 8018206:	21f4      	movs	r1, #244	; 0xf4
 8018208:	0192      	lsls	r2, r2, #6
 801820a:	3234      	adds	r2, #52	; 0x34
 801820c:	f002 02fc 	and.w	r2, r2, #252	; 0xfc
 8018210:	f7ff ffc4 	bl	801819c <_ZN15Adafruit_BMP0856write8Ehh>
 8018214:	7823      	ldrb	r3, [r4, #0]
 8018216:	b98b      	cbnz	r3, 801823c <_ZN15Adafruit_BMP08515readRawPressureEv+0x3c>
 8018218:	2005      	movs	r0, #5
 801821a:	f000 fd57 	bl	8018ccc <_ZN4BIOS3SYS7DelayMsEi@plt+0x8>
 801821e:	21f6      	movs	r1, #246	; 0xf6
 8018220:	4620      	mov	r0, r4
 8018222:	f7ff ff4e 	bl	80180c2 <_ZN15Adafruit_BMP0856read16Eh>
 8018226:	21f8      	movs	r1, #248	; 0xf8
 8018228:	0205      	lsls	r5, r0, #8
 801822a:	4620      	mov	r0, r4
 801822c:	f7ff ff31 	bl	8018092 <_ZN15Adafruit_BMP0855read8Eh>
 8018230:	7823      	ldrb	r3, [r4, #0]
 8018232:	4328      	orrs	r0, r5
 8018234:	f1c3 0308 	rsb	r3, r3, #8
 8018238:	40d8      	lsrs	r0, r3
 801823a:	bd38      	pop	{r3, r4, r5, pc}
 801823c:	2b01      	cmp	r3, #1
 801823e:	d101      	bne.n	8018244 <_ZN15Adafruit_BMP08515readRawPressureEv+0x44>
 8018240:	2008      	movs	r0, #8
 8018242:	e7ea      	b.n	801821a <_ZN15Adafruit_BMP08515readRawPressureEv+0x1a>
 8018244:	2b02      	cmp	r3, #2
 8018246:	bf0c      	ite	eq
 8018248:	200e      	moveq	r0, #14
 801824a:	201a      	movne	r0, #26
 801824c:	e7e5      	b.n	801821a <_ZN15Adafruit_BMP08515readRawPressureEv+0x1a>

0801824e <_ZN15Adafruit_BMP08512readPressureEv>:
 801824e:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8018250:	4606      	mov	r6, r0
 8018252:	f7ff ffb3 	bl	80181bc <_ZN15Adafruit_BMP08518readRawTemperatureEv>
 8018256:	4605      	mov	r5, r0
 8018258:	4630      	mov	r0, r6
 801825a:	f7ff ffd1 	bl	8018200 <_ZN15Adafruit_BMP08515readRawPressureEv>
 801825e:	4629      	mov	r1, r5
 8018260:	4604      	mov	r4, r0
 8018262:	4630      	mov	r0, r6
 8018264:	f7ff ff06 	bl	8018074 <_ZN15Adafruit_BMP0859computeB5El>
 8018268:	f5a0 607a 	sub.w	r0, r0, #4000	; 0xfa0
 801826c:	fb00 f700 	mul.w	r7, r0, r0
 8018270:	f9b6 2006 	ldrsh.w	r2, [r6, #6]
 8018274:	133f      	asrs	r7, r7, #12
 8018276:	fb00 f302 	mul.w	r3, r0, r2
 801827a:	f9b6 2008 	ldrsh.w	r2, [r6, #8]
 801827e:	7835      	ldrb	r5, [r6, #0]
 8018280:	437a      	muls	r2, r7
 8018282:	1412      	asrs	r2, r2, #16
 8018284:	eb02 3263 	add.w	r2, r2, r3, asr #13
 8018288:	3202      	adds	r2, #2
 801828a:	8a73      	ldrh	r3, [r6, #18]
 801828c:	1092      	asrs	r2, r2, #2
 801828e:	f502 4200 	add.w	r2, r2, #32768	; 0x8000
 8018292:	4353      	muls	r3, r2
 8018294:	2204      	movs	r2, #4
 8018296:	0bd9      	lsrs	r1, r3, #15
 8018298:	f9b6 300a 	ldrsh.w	r3, [r6, #10]
 801829c:	435f      	muls	r7, r3
 801829e:	f9b6 3004 	ldrsh.w	r3, [r6, #4]
 80182a2:	4343      	muls	r3, r0
 80182a4:	f9b6 0002 	ldrsh.w	r0, [r6, #2]
 80182a8:	12db      	asrs	r3, r3, #11
 80182aa:	eb03 23e7 	add.w	r3, r3, r7, asr #11
 80182ae:	eb03 0380 	add.w	r3, r3, r0, lsl #2
 80182b2:	40ab      	lsls	r3, r5
 80182b4:	3302      	adds	r3, #2
 80182b6:	fb93 f3f2 	sdiv	r3, r3, r2
 80182ba:	f24c 3250 	movw	r2, #50000	; 0xc350
 80182be:	1ae4      	subs	r4, r4, r3
 80182c0:	fa22 f305 	lsr.w	r3, r2, r5
 80182c4:	4363      	muls	r3, r4
 80182c6:	bf55      	itete	pl
 80182c8:	005b      	lslpl	r3, r3, #1
 80182ca:	fbb3 f3f1 	udivmi	r3, r3, r1
 80182ce:	fbb3 f3f1 	udivpl	r3, r3, r1
 80182d2:	005b      	lslmi	r3, r3, #1
 80182d4:	1218      	asrs	r0, r3, #8
 80182d6:	fb00 f200 	mul.w	r2, r0, r0
 80182da:	f640 30de 	movw	r0, #3038	; 0xbde
 80182de:	4350      	muls	r0, r2
 80182e0:	4a04      	ldr	r2, [pc, #16]	; (80182f4 <_ZN15Adafruit_BMP08512readPressureEv+0xa6>)
 80182e2:	1400      	asrs	r0, r0, #16
 80182e4:	435a      	muls	r2, r3
 80182e6:	eb00 4022 	add.w	r0, r0, r2, asr #16
 80182ea:	f600 60cf 	addw	r0, r0, #3791	; 0xecf
 80182ee:	eb03 1020 	add.w	r0, r3, r0, asr #4
 80182f2:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 80182f4:	ffffe343 	.word	0xffffe343

080182f8 <__aeabi_drsub>:
 80182f8:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000
 80182fc:	e002      	b.n	8018304 <__adddf3>
 80182fe:	bf00      	nop

08018300 <__aeabi_dsub>:
 8018300:	f083 4300 	eor.w	r3, r3, #2147483648	; 0x80000000

08018304 <__adddf3>:
 8018304:	b530      	push	{r4, r5, lr}
 8018306:	ea4f 0441 	mov.w	r4, r1, lsl #1
 801830a:	ea4f 0543 	mov.w	r5, r3, lsl #1
 801830e:	ea94 0f05 	teq	r4, r5
 8018312:	bf08      	it	eq
 8018314:	ea90 0f02 	teqeq	r0, r2
 8018318:	bf1f      	itttt	ne
 801831a:	ea54 0c00 	orrsne.w	ip, r4, r0
 801831e:	ea55 0c02 	orrsne.w	ip, r5, r2
 8018322:	ea7f 5c64 	mvnsne.w	ip, r4, asr #21
 8018326:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 801832a:	f000 80e2 	beq.w	80184f2 <__adddf3+0x1ee>
 801832e:	ea4f 5454 	mov.w	r4, r4, lsr #21
 8018332:	ebd4 5555 	rsbs	r5, r4, r5, lsr #21
 8018336:	bfb8      	it	lt
 8018338:	426d      	neglt	r5, r5
 801833a:	dd0c      	ble.n	8018356 <__adddf3+0x52>
 801833c:	442c      	add	r4, r5
 801833e:	ea80 0202 	eor.w	r2, r0, r2
 8018342:	ea81 0303 	eor.w	r3, r1, r3
 8018346:	ea82 0000 	eor.w	r0, r2, r0
 801834a:	ea83 0101 	eor.w	r1, r3, r1
 801834e:	ea80 0202 	eor.w	r2, r0, r2
 8018352:	ea81 0303 	eor.w	r3, r1, r3
 8018356:	2d36      	cmp	r5, #54	; 0x36
 8018358:	bf88      	it	hi
 801835a:	bd30      	pophi	{r4, r5, pc}
 801835c:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 8018360:	ea4f 3101 	mov.w	r1, r1, lsl #12
 8018364:	f44f 1c80 	mov.w	ip, #1048576	; 0x100000
 8018368:	ea4c 3111 	orr.w	r1, ip, r1, lsr #12
 801836c:	d002      	beq.n	8018374 <__adddf3+0x70>
 801836e:	4240      	negs	r0, r0
 8018370:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 8018374:	f013 4f00 	tst.w	r3, #2147483648	; 0x80000000
 8018378:	ea4f 3303 	mov.w	r3, r3, lsl #12
 801837c:	ea4c 3313 	orr.w	r3, ip, r3, lsr #12
 8018380:	d002      	beq.n	8018388 <__adddf3+0x84>
 8018382:	4252      	negs	r2, r2
 8018384:	eb63 0343 	sbc.w	r3, r3, r3, lsl #1
 8018388:	ea94 0f05 	teq	r4, r5
 801838c:	f000 80a7 	beq.w	80184de <__adddf3+0x1da>
 8018390:	f1a4 0401 	sub.w	r4, r4, #1
 8018394:	f1d5 0e20 	rsbs	lr, r5, #32
 8018398:	db0d      	blt.n	80183b6 <__adddf3+0xb2>
 801839a:	fa02 fc0e 	lsl.w	ip, r2, lr
 801839e:	fa22 f205 	lsr.w	r2, r2, r5
 80183a2:	1880      	adds	r0, r0, r2
 80183a4:	f141 0100 	adc.w	r1, r1, #0
 80183a8:	fa03 f20e 	lsl.w	r2, r3, lr
 80183ac:	1880      	adds	r0, r0, r2
 80183ae:	fa43 f305 	asr.w	r3, r3, r5
 80183b2:	4159      	adcs	r1, r3
 80183b4:	e00e      	b.n	80183d4 <__adddf3+0xd0>
 80183b6:	f1a5 0520 	sub.w	r5, r5, #32
 80183ba:	f10e 0e20 	add.w	lr, lr, #32
 80183be:	2a01      	cmp	r2, #1
 80183c0:	fa03 fc0e 	lsl.w	ip, r3, lr
 80183c4:	bf28      	it	cs
 80183c6:	f04c 0c02 	orrcs.w	ip, ip, #2
 80183ca:	fa43 f305 	asr.w	r3, r3, r5
 80183ce:	18c0      	adds	r0, r0, r3
 80183d0:	eb51 71e3 	adcs.w	r1, r1, r3, asr #31
 80183d4:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 80183d8:	d507      	bpl.n	80183ea <__adddf3+0xe6>
 80183da:	f04f 0e00 	mov.w	lr, #0
 80183de:	f1dc 0c00 	rsbs	ip, ip, #0
 80183e2:	eb7e 0000 	sbcs.w	r0, lr, r0
 80183e6:	eb6e 0101 	sbc.w	r1, lr, r1
 80183ea:	f5b1 1f80 	cmp.w	r1, #1048576	; 0x100000
 80183ee:	d31b      	bcc.n	8018428 <__adddf3+0x124>
 80183f0:	f5b1 1f00 	cmp.w	r1, #2097152	; 0x200000
 80183f4:	d30c      	bcc.n	8018410 <__adddf3+0x10c>
 80183f6:	0849      	lsrs	r1, r1, #1
 80183f8:	ea5f 0030 	movs.w	r0, r0, rrx
 80183fc:	ea4f 0c3c 	mov.w	ip, ip, rrx
 8018400:	f104 0401 	add.w	r4, r4, #1
 8018404:	ea4f 5244 	mov.w	r2, r4, lsl #21
 8018408:	f512 0f80 	cmn.w	r2, #4194304	; 0x400000
 801840c:	f080 809a 	bcs.w	8018544 <__adddf3+0x240>
 8018410:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 8018414:	bf08      	it	eq
 8018416:	ea5f 0c50 	movseq.w	ip, r0, lsr #1
 801841a:	f150 0000 	adcs.w	r0, r0, #0
 801841e:	eb41 5104 	adc.w	r1, r1, r4, lsl #20
 8018422:	ea41 0105 	orr.w	r1, r1, r5
 8018426:	bd30      	pop	{r4, r5, pc}
 8018428:	ea5f 0c4c 	movs.w	ip, ip, lsl #1
 801842c:	4140      	adcs	r0, r0
 801842e:	eb41 0101 	adc.w	r1, r1, r1
 8018432:	f411 1f80 	tst.w	r1, #1048576	; 0x100000
 8018436:	f1a4 0401 	sub.w	r4, r4, #1
 801843a:	d1e9      	bne.n	8018410 <__adddf3+0x10c>
 801843c:	f091 0f00 	teq	r1, #0
 8018440:	bf04      	itt	eq
 8018442:	4601      	moveq	r1, r0
 8018444:	2000      	moveq	r0, #0
 8018446:	fab1 f381 	clz	r3, r1
 801844a:	bf08      	it	eq
 801844c:	3320      	addeq	r3, #32
 801844e:	f1a3 030b 	sub.w	r3, r3, #11
 8018452:	f1b3 0220 	subs.w	r2, r3, #32
 8018456:	da0c      	bge.n	8018472 <__adddf3+0x16e>
 8018458:	320c      	adds	r2, #12
 801845a:	dd08      	ble.n	801846e <__adddf3+0x16a>
 801845c:	f102 0c14 	add.w	ip, r2, #20
 8018460:	f1c2 020c 	rsb	r2, r2, #12
 8018464:	fa01 f00c 	lsl.w	r0, r1, ip
 8018468:	fa21 f102 	lsr.w	r1, r1, r2
 801846c:	e00c      	b.n	8018488 <__adddf3+0x184>
 801846e:	f102 0214 	add.w	r2, r2, #20
 8018472:	bfd8      	it	le
 8018474:	f1c2 0c20 	rsble	ip, r2, #32
 8018478:	fa01 f102 	lsl.w	r1, r1, r2
 801847c:	fa20 fc0c 	lsr.w	ip, r0, ip
 8018480:	bfdc      	itt	le
 8018482:	ea41 010c 	orrle.w	r1, r1, ip
 8018486:	4090      	lslle	r0, r2
 8018488:	1ae4      	subs	r4, r4, r3
 801848a:	bfa2      	ittt	ge
 801848c:	eb01 5104 	addge.w	r1, r1, r4, lsl #20
 8018490:	4329      	orrge	r1, r5
 8018492:	bd30      	popge	{r4, r5, pc}
 8018494:	ea6f 0404 	mvn.w	r4, r4
 8018498:	3c1f      	subs	r4, #31
 801849a:	da1c      	bge.n	80184d6 <__adddf3+0x1d2>
 801849c:	340c      	adds	r4, #12
 801849e:	dc0e      	bgt.n	80184be <__adddf3+0x1ba>
 80184a0:	f104 0414 	add.w	r4, r4, #20
 80184a4:	f1c4 0220 	rsb	r2, r4, #32
 80184a8:	fa20 f004 	lsr.w	r0, r0, r4
 80184ac:	fa01 f302 	lsl.w	r3, r1, r2
 80184b0:	ea40 0003 	orr.w	r0, r0, r3
 80184b4:	fa21 f304 	lsr.w	r3, r1, r4
 80184b8:	ea45 0103 	orr.w	r1, r5, r3
 80184bc:	bd30      	pop	{r4, r5, pc}
 80184be:	f1c4 040c 	rsb	r4, r4, #12
 80184c2:	f1c4 0220 	rsb	r2, r4, #32
 80184c6:	fa20 f002 	lsr.w	r0, r0, r2
 80184ca:	fa01 f304 	lsl.w	r3, r1, r4
 80184ce:	ea40 0003 	orr.w	r0, r0, r3
 80184d2:	4629      	mov	r1, r5
 80184d4:	bd30      	pop	{r4, r5, pc}
 80184d6:	fa21 f004 	lsr.w	r0, r1, r4
 80184da:	4629      	mov	r1, r5
 80184dc:	bd30      	pop	{r4, r5, pc}
 80184de:	f094 0f00 	teq	r4, #0
 80184e2:	f483 1380 	eor.w	r3, r3, #1048576	; 0x100000
 80184e6:	bf06      	itte	eq
 80184e8:	f481 1180 	eoreq.w	r1, r1, #1048576	; 0x100000
 80184ec:	3401      	addeq	r4, #1
 80184ee:	3d01      	subne	r5, #1
 80184f0:	e74e      	b.n	8018390 <__adddf3+0x8c>
 80184f2:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 80184f6:	bf18      	it	ne
 80184f8:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 80184fc:	d029      	beq.n	8018552 <__adddf3+0x24e>
 80184fe:	ea94 0f05 	teq	r4, r5
 8018502:	bf08      	it	eq
 8018504:	ea90 0f02 	teqeq	r0, r2
 8018508:	d005      	beq.n	8018516 <__adddf3+0x212>
 801850a:	ea54 0c00 	orrs.w	ip, r4, r0
 801850e:	bf04      	itt	eq
 8018510:	4619      	moveq	r1, r3
 8018512:	4610      	moveq	r0, r2
 8018514:	bd30      	pop	{r4, r5, pc}
 8018516:	ea91 0f03 	teq	r1, r3
 801851a:	bf1e      	ittt	ne
 801851c:	2100      	movne	r1, #0
 801851e:	2000      	movne	r0, #0
 8018520:	bd30      	popne	{r4, r5, pc}
 8018522:	ea5f 5c54 	movs.w	ip, r4, lsr #21
 8018526:	d105      	bne.n	8018534 <__adddf3+0x230>
 8018528:	0040      	lsls	r0, r0, #1
 801852a:	4149      	adcs	r1, r1
 801852c:	bf28      	it	cs
 801852e:	f041 4100 	orrcs.w	r1, r1, #2147483648	; 0x80000000
 8018532:	bd30      	pop	{r4, r5, pc}
 8018534:	f514 0480 	adds.w	r4, r4, #4194304	; 0x400000
 8018538:	bf3c      	itt	cc
 801853a:	f501 1180 	addcc.w	r1, r1, #1048576	; 0x100000
 801853e:	bd30      	popcc	{r4, r5, pc}
 8018540:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 8018544:	f045 41fe 	orr.w	r1, r5, #2130706432	; 0x7f000000
 8018548:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
 801854c:	f04f 0000 	mov.w	r0, #0
 8018550:	bd30      	pop	{r4, r5, pc}
 8018552:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 8018556:	bf1a      	itte	ne
 8018558:	4619      	movne	r1, r3
 801855a:	4610      	movne	r0, r2
 801855c:	ea7f 5c65 	mvnseq.w	ip, r5, asr #21
 8018560:	bf1c      	itt	ne
 8018562:	460b      	movne	r3, r1
 8018564:	4602      	movne	r2, r0
 8018566:	ea50 3401 	orrs.w	r4, r0, r1, lsl #12
 801856a:	bf06      	itte	eq
 801856c:	ea52 3503 	orrseq.w	r5, r2, r3, lsl #12
 8018570:	ea91 0f03 	teqeq	r1, r3
 8018574:	f441 2100 	orrne.w	r1, r1, #524288	; 0x80000
 8018578:	bd30      	pop	{r4, r5, pc}
 801857a:	bf00      	nop

0801857c <__aeabi_ui2d>:
 801857c:	f090 0f00 	teq	r0, #0
 8018580:	bf04      	itt	eq
 8018582:	2100      	moveq	r1, #0
 8018584:	4770      	bxeq	lr
 8018586:	b530      	push	{r4, r5, lr}
 8018588:	f44f 6480 	mov.w	r4, #1024	; 0x400
 801858c:	f104 0432 	add.w	r4, r4, #50	; 0x32
 8018590:	f04f 0500 	mov.w	r5, #0
 8018594:	f04f 0100 	mov.w	r1, #0
 8018598:	e750      	b.n	801843c <__adddf3+0x138>
 801859a:	bf00      	nop

0801859c <__aeabi_i2d>:
 801859c:	f090 0f00 	teq	r0, #0
 80185a0:	bf04      	itt	eq
 80185a2:	2100      	moveq	r1, #0
 80185a4:	4770      	bxeq	lr
 80185a6:	b530      	push	{r4, r5, lr}
 80185a8:	f44f 6480 	mov.w	r4, #1024	; 0x400
 80185ac:	f104 0432 	add.w	r4, r4, #50	; 0x32
 80185b0:	f010 4500 	ands.w	r5, r0, #2147483648	; 0x80000000
 80185b4:	bf48      	it	mi
 80185b6:	4240      	negmi	r0, r0
 80185b8:	f04f 0100 	mov.w	r1, #0
 80185bc:	e73e      	b.n	801843c <__adddf3+0x138>
 80185be:	bf00      	nop

080185c0 <__aeabi_f2d>:
 80185c0:	0042      	lsls	r2, r0, #1
 80185c2:	ea4f 01e2 	mov.w	r1, r2, asr #3
 80185c6:	ea4f 0131 	mov.w	r1, r1, rrx
 80185ca:	ea4f 7002 	mov.w	r0, r2, lsl #28
 80185ce:	bf1f      	itttt	ne
 80185d0:	f012 437f 	andsne.w	r3, r2, #4278190080	; 0xff000000
 80185d4:	f093 4f7f 	teqne	r3, #4278190080	; 0xff000000
 80185d8:	f081 5160 	eorne.w	r1, r1, #939524096	; 0x38000000
 80185dc:	4770      	bxne	lr
 80185de:	f032 427f 	bics.w	r2, r2, #4278190080	; 0xff000000
 80185e2:	bf08      	it	eq
 80185e4:	4770      	bxeq	lr
 80185e6:	f093 4f7f 	teq	r3, #4278190080	; 0xff000000
 80185ea:	bf04      	itt	eq
 80185ec:	f441 2100 	orreq.w	r1, r1, #524288	; 0x80000
 80185f0:	4770      	bxeq	lr
 80185f2:	b530      	push	{r4, r5, lr}
 80185f4:	f44f 7460 	mov.w	r4, #896	; 0x380
 80185f8:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 80185fc:	f021 4100 	bic.w	r1, r1, #2147483648	; 0x80000000
 8018600:	e71c      	b.n	801843c <__adddf3+0x138>
 8018602:	bf00      	nop

08018604 <__aeabi_ul2d>:
 8018604:	ea50 0201 	orrs.w	r2, r0, r1
 8018608:	bf08      	it	eq
 801860a:	4770      	bxeq	lr
 801860c:	b530      	push	{r4, r5, lr}
 801860e:	f04f 0500 	mov.w	r5, #0
 8018612:	e00a      	b.n	801862a <__aeabi_l2d+0x16>

08018614 <__aeabi_l2d>:
 8018614:	ea50 0201 	orrs.w	r2, r0, r1
 8018618:	bf08      	it	eq
 801861a:	4770      	bxeq	lr
 801861c:	b530      	push	{r4, r5, lr}
 801861e:	f011 4500 	ands.w	r5, r1, #2147483648	; 0x80000000
 8018622:	d502      	bpl.n	801862a <__aeabi_l2d+0x16>
 8018624:	4240      	negs	r0, r0
 8018626:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 801862a:	f44f 6480 	mov.w	r4, #1024	; 0x400
 801862e:	f104 0432 	add.w	r4, r4, #50	; 0x32
 8018632:	ea5f 5c91 	movs.w	ip, r1, lsr #22
 8018636:	f43f aed8 	beq.w	80183ea <__adddf3+0xe6>
 801863a:	f04f 0203 	mov.w	r2, #3
 801863e:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 8018642:	bf18      	it	ne
 8018644:	3203      	addne	r2, #3
 8018646:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 801864a:	bf18      	it	ne
 801864c:	3203      	addne	r2, #3
 801864e:	eb02 02dc 	add.w	r2, r2, ip, lsr #3
 8018652:	f1c2 0320 	rsb	r3, r2, #32
 8018656:	fa00 fc03 	lsl.w	ip, r0, r3
 801865a:	fa20 f002 	lsr.w	r0, r0, r2
 801865e:	fa01 fe03 	lsl.w	lr, r1, r3
 8018662:	ea40 000e 	orr.w	r0, r0, lr
 8018666:	fa21 f102 	lsr.w	r1, r1, r2
 801866a:	4414      	add	r4, r2
 801866c:	e6bd      	b.n	80183ea <__adddf3+0xe6>
 801866e:	bf00      	nop

08018670 <__aeabi_frsub>:
 8018670:	f080 4000 	eor.w	r0, r0, #2147483648	; 0x80000000
 8018674:	e002      	b.n	801867c <__addsf3>
 8018676:	bf00      	nop

08018678 <__aeabi_fsub>:
 8018678:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000

0801867c <__addsf3>:
 801867c:	0042      	lsls	r2, r0, #1
 801867e:	bf1f      	itttt	ne
 8018680:	ea5f 0341 	movsne.w	r3, r1, lsl #1
 8018684:	ea92 0f03 	teqne	r2, r3
 8018688:	ea7f 6c22 	mvnsne.w	ip, r2, asr #24
 801868c:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 8018690:	d06a      	beq.n	8018768 <__addsf3+0xec>
 8018692:	ea4f 6212 	mov.w	r2, r2, lsr #24
 8018696:	ebd2 6313 	rsbs	r3, r2, r3, lsr #24
 801869a:	bfc1      	itttt	gt
 801869c:	18d2      	addgt	r2, r2, r3
 801869e:	4041      	eorgt	r1, r0
 80186a0:	4048      	eorgt	r0, r1
 80186a2:	4041      	eorgt	r1, r0
 80186a4:	bfb8      	it	lt
 80186a6:	425b      	neglt	r3, r3
 80186a8:	2b19      	cmp	r3, #25
 80186aa:	bf88      	it	hi
 80186ac:	4770      	bxhi	lr
 80186ae:	f010 4f00 	tst.w	r0, #2147483648	; 0x80000000
 80186b2:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 80186b6:	f020 407f 	bic.w	r0, r0, #4278190080	; 0xff000000
 80186ba:	bf18      	it	ne
 80186bc:	4240      	negne	r0, r0
 80186be:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 80186c2:	f441 0100 	orr.w	r1, r1, #8388608	; 0x800000
 80186c6:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 80186ca:	bf18      	it	ne
 80186cc:	4249      	negne	r1, r1
 80186ce:	ea92 0f03 	teq	r2, r3
 80186d2:	d03f      	beq.n	8018754 <__addsf3+0xd8>
 80186d4:	f1a2 0201 	sub.w	r2, r2, #1
 80186d8:	fa41 fc03 	asr.w	ip, r1, r3
 80186dc:	eb10 000c 	adds.w	r0, r0, ip
 80186e0:	f1c3 0320 	rsb	r3, r3, #32
 80186e4:	fa01 f103 	lsl.w	r1, r1, r3
 80186e8:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 80186ec:	d502      	bpl.n	80186f4 <__addsf3+0x78>
 80186ee:	4249      	negs	r1, r1
 80186f0:	eb60 0040 	sbc.w	r0, r0, r0, lsl #1
 80186f4:	f5b0 0f00 	cmp.w	r0, #8388608	; 0x800000
 80186f8:	d313      	bcc.n	8018722 <__addsf3+0xa6>
 80186fa:	f1b0 7f80 	cmp.w	r0, #16777216	; 0x1000000
 80186fe:	d306      	bcc.n	801870e <__addsf3+0x92>
 8018700:	0840      	lsrs	r0, r0, #1
 8018702:	ea4f 0131 	mov.w	r1, r1, rrx
 8018706:	f102 0201 	add.w	r2, r2, #1
 801870a:	2afe      	cmp	r2, #254	; 0xfe
 801870c:	d251      	bcs.n	80187b2 <__addsf3+0x136>
 801870e:	f1b1 4f00 	cmp.w	r1, #2147483648	; 0x80000000
 8018712:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 8018716:	bf08      	it	eq
 8018718:	f020 0001 	biceq.w	r0, r0, #1
 801871c:	ea40 0003 	orr.w	r0, r0, r3
 8018720:	4770      	bx	lr
 8018722:	0049      	lsls	r1, r1, #1
 8018724:	eb40 0000 	adc.w	r0, r0, r0
 8018728:	f410 0f00 	tst.w	r0, #8388608	; 0x800000
 801872c:	f1a2 0201 	sub.w	r2, r2, #1
 8018730:	d1ed      	bne.n	801870e <__addsf3+0x92>
 8018732:	fab0 fc80 	clz	ip, r0
 8018736:	f1ac 0c08 	sub.w	ip, ip, #8
 801873a:	ebb2 020c 	subs.w	r2, r2, ip
 801873e:	fa00 f00c 	lsl.w	r0, r0, ip
 8018742:	bfaa      	itet	ge
 8018744:	eb00 50c2 	addge.w	r0, r0, r2, lsl #23
 8018748:	4252      	neglt	r2, r2
 801874a:	4318      	orrge	r0, r3
 801874c:	bfbc      	itt	lt
 801874e:	40d0      	lsrlt	r0, r2
 8018750:	4318      	orrlt	r0, r3
 8018752:	4770      	bx	lr
 8018754:	f092 0f00 	teq	r2, #0
 8018758:	f481 0100 	eor.w	r1, r1, #8388608	; 0x800000
 801875c:	bf06      	itte	eq
 801875e:	f480 0000 	eoreq.w	r0, r0, #8388608	; 0x800000
 8018762:	3201      	addeq	r2, #1
 8018764:	3b01      	subne	r3, #1
 8018766:	e7b5      	b.n	80186d4 <__addsf3+0x58>
 8018768:	ea4f 0341 	mov.w	r3, r1, lsl #1
 801876c:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 8018770:	bf18      	it	ne
 8018772:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 8018776:	d021      	beq.n	80187bc <__addsf3+0x140>
 8018778:	ea92 0f03 	teq	r2, r3
 801877c:	d004      	beq.n	8018788 <__addsf3+0x10c>
 801877e:	f092 0f00 	teq	r2, #0
 8018782:	bf08      	it	eq
 8018784:	4608      	moveq	r0, r1
 8018786:	4770      	bx	lr
 8018788:	ea90 0f01 	teq	r0, r1
 801878c:	bf1c      	itt	ne
 801878e:	2000      	movne	r0, #0
 8018790:	4770      	bxne	lr
 8018792:	f012 4f7f 	tst.w	r2, #4278190080	; 0xff000000
 8018796:	d104      	bne.n	80187a2 <__addsf3+0x126>
 8018798:	0040      	lsls	r0, r0, #1
 801879a:	bf28      	it	cs
 801879c:	f040 4000 	orrcs.w	r0, r0, #2147483648	; 0x80000000
 80187a0:	4770      	bx	lr
 80187a2:	f112 7200 	adds.w	r2, r2, #33554432	; 0x2000000
 80187a6:	bf3c      	itt	cc
 80187a8:	f500 0000 	addcc.w	r0, r0, #8388608	; 0x800000
 80187ac:	4770      	bxcc	lr
 80187ae:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 80187b2:	f043 40fe 	orr.w	r0, r3, #2130706432	; 0x7f000000
 80187b6:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 80187ba:	4770      	bx	lr
 80187bc:	ea7f 6222 	mvns.w	r2, r2, asr #24
 80187c0:	bf16      	itet	ne
 80187c2:	4608      	movne	r0, r1
 80187c4:	ea7f 6323 	mvnseq.w	r3, r3, asr #24
 80187c8:	4601      	movne	r1, r0
 80187ca:	0242      	lsls	r2, r0, #9
 80187cc:	bf06      	itte	eq
 80187ce:	ea5f 2341 	movseq.w	r3, r1, lsl #9
 80187d2:	ea90 0f01 	teqeq	r0, r1
 80187d6:	f440 0080 	orrne.w	r0, r0, #4194304	; 0x400000
 80187da:	4770      	bx	lr

080187dc <__aeabi_ui2f>:
 80187dc:	f04f 0300 	mov.w	r3, #0
 80187e0:	e004      	b.n	80187ec <__aeabi_i2f+0x8>
 80187e2:	bf00      	nop

080187e4 <__aeabi_i2f>:
 80187e4:	f010 4300 	ands.w	r3, r0, #2147483648	; 0x80000000
 80187e8:	bf48      	it	mi
 80187ea:	4240      	negmi	r0, r0
 80187ec:	ea5f 0c00 	movs.w	ip, r0
 80187f0:	bf08      	it	eq
 80187f2:	4770      	bxeq	lr
 80187f4:	f043 4396 	orr.w	r3, r3, #1258291200	; 0x4b000000
 80187f8:	4601      	mov	r1, r0
 80187fa:	f04f 0000 	mov.w	r0, #0
 80187fe:	e01c      	b.n	801883a <__aeabi_l2f+0x2a>

08018800 <__aeabi_ul2f>:
 8018800:	ea50 0201 	orrs.w	r2, r0, r1
 8018804:	bf08      	it	eq
 8018806:	4770      	bxeq	lr
 8018808:	f04f 0300 	mov.w	r3, #0
 801880c:	e00a      	b.n	8018824 <__aeabi_l2f+0x14>
 801880e:	bf00      	nop

08018810 <__aeabi_l2f>:
 8018810:	ea50 0201 	orrs.w	r2, r0, r1
 8018814:	bf08      	it	eq
 8018816:	4770      	bxeq	lr
 8018818:	f011 4300 	ands.w	r3, r1, #2147483648	; 0x80000000
 801881c:	d502      	bpl.n	8018824 <__aeabi_l2f+0x14>
 801881e:	4240      	negs	r0, r0
 8018820:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 8018824:	ea5f 0c01 	movs.w	ip, r1
 8018828:	bf02      	ittt	eq
 801882a:	4684      	moveq	ip, r0
 801882c:	4601      	moveq	r1, r0
 801882e:	2000      	moveq	r0, #0
 8018830:	f043 43b6 	orr.w	r3, r3, #1526726656	; 0x5b000000
 8018834:	bf08      	it	eq
 8018836:	f1a3 5380 	subeq.w	r3, r3, #268435456	; 0x10000000
 801883a:	f5a3 0300 	sub.w	r3, r3, #8388608	; 0x800000
 801883e:	fabc f28c 	clz	r2, ip
 8018842:	3a08      	subs	r2, #8
 8018844:	eba3 53c2 	sub.w	r3, r3, r2, lsl #23
 8018848:	db10      	blt.n	801886c <__aeabi_l2f+0x5c>
 801884a:	fa01 fc02 	lsl.w	ip, r1, r2
 801884e:	4463      	add	r3, ip
 8018850:	fa00 fc02 	lsl.w	ip, r0, r2
 8018854:	f1c2 0220 	rsb	r2, r2, #32
 8018858:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 801885c:	fa20 f202 	lsr.w	r2, r0, r2
 8018860:	eb43 0002 	adc.w	r0, r3, r2
 8018864:	bf08      	it	eq
 8018866:	f020 0001 	biceq.w	r0, r0, #1
 801886a:	4770      	bx	lr
 801886c:	f102 0220 	add.w	r2, r2, #32
 8018870:	fa01 fc02 	lsl.w	ip, r1, r2
 8018874:	f1c2 0220 	rsb	r2, r2, #32
 8018878:	ea50 004c 	orrs.w	r0, r0, ip, lsl #1
 801887c:	fa21 f202 	lsr.w	r2, r1, r2
 8018880:	eb43 0002 	adc.w	r0, r3, r2
 8018884:	bf08      	it	eq
 8018886:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 801888a:	4770      	bx	lr

0801888c <__aeabi_fmul>:
 801888c:	f04f 0cff 	mov.w	ip, #255	; 0xff
 8018890:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 8018894:	bf1e      	ittt	ne
 8018896:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 801889a:	ea92 0f0c 	teqne	r2, ip
 801889e:	ea93 0f0c 	teqne	r3, ip
 80188a2:	d06f      	beq.n	8018984 <__aeabi_fmul+0xf8>
 80188a4:	441a      	add	r2, r3
 80188a6:	ea80 0c01 	eor.w	ip, r0, r1
 80188aa:	0240      	lsls	r0, r0, #9
 80188ac:	bf18      	it	ne
 80188ae:	ea5f 2141 	movsne.w	r1, r1, lsl #9
 80188b2:	d01e      	beq.n	80188f2 <__aeabi_fmul+0x66>
 80188b4:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
 80188b8:	ea43 1050 	orr.w	r0, r3, r0, lsr #5
 80188bc:	ea43 1151 	orr.w	r1, r3, r1, lsr #5
 80188c0:	fba0 3101 	umull	r3, r1, r0, r1
 80188c4:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 80188c8:	f5b1 0f00 	cmp.w	r1, #8388608	; 0x800000
 80188cc:	bf3e      	ittt	cc
 80188ce:	0049      	lslcc	r1, r1, #1
 80188d0:	ea41 71d3 	orrcc.w	r1, r1, r3, lsr #31
 80188d4:	005b      	lslcc	r3, r3, #1
 80188d6:	ea40 0001 	orr.w	r0, r0, r1
 80188da:	f162 027f 	sbc.w	r2, r2, #127	; 0x7f
 80188de:	2afd      	cmp	r2, #253	; 0xfd
 80188e0:	d81d      	bhi.n	801891e <__aeabi_fmul+0x92>
 80188e2:	f1b3 4f00 	cmp.w	r3, #2147483648	; 0x80000000
 80188e6:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 80188ea:	bf08      	it	eq
 80188ec:	f020 0001 	biceq.w	r0, r0, #1
 80188f0:	4770      	bx	lr
 80188f2:	f090 0f00 	teq	r0, #0
 80188f6:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 80188fa:	bf08      	it	eq
 80188fc:	0249      	lsleq	r1, r1, #9
 80188fe:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 8018902:	ea40 2051 	orr.w	r0, r0, r1, lsr #9
 8018906:	3a7f      	subs	r2, #127	; 0x7f
 8018908:	bfc2      	ittt	gt
 801890a:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 801890e:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 8018912:	4770      	bxgt	lr
 8018914:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8018918:	f04f 0300 	mov.w	r3, #0
 801891c:	3a01      	subs	r2, #1
 801891e:	dc5d      	bgt.n	80189dc <__aeabi_fmul+0x150>
 8018920:	f112 0f19 	cmn.w	r2, #25
 8018924:	bfdc      	itt	le
 8018926:	f000 4000 	andle.w	r0, r0, #2147483648	; 0x80000000
 801892a:	4770      	bxle	lr
 801892c:	f1c2 0200 	rsb	r2, r2, #0
 8018930:	0041      	lsls	r1, r0, #1
 8018932:	fa21 f102 	lsr.w	r1, r1, r2
 8018936:	f1c2 0220 	rsb	r2, r2, #32
 801893a:	fa00 fc02 	lsl.w	ip, r0, r2
 801893e:	ea5f 0031 	movs.w	r0, r1, rrx
 8018942:	f140 0000 	adc.w	r0, r0, #0
 8018946:	ea53 034c 	orrs.w	r3, r3, ip, lsl #1
 801894a:	bf08      	it	eq
 801894c:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 8018950:	4770      	bx	lr
 8018952:	f092 0f00 	teq	r2, #0
 8018956:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 801895a:	bf02      	ittt	eq
 801895c:	0040      	lsleq	r0, r0, #1
 801895e:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 8018962:	3a01      	subeq	r2, #1
 8018964:	d0f9      	beq.n	801895a <__aeabi_fmul+0xce>
 8018966:	ea40 000c 	orr.w	r0, r0, ip
 801896a:	f093 0f00 	teq	r3, #0
 801896e:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 8018972:	bf02      	ittt	eq
 8018974:	0049      	lsleq	r1, r1, #1
 8018976:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 801897a:	3b01      	subeq	r3, #1
 801897c:	d0f9      	beq.n	8018972 <__aeabi_fmul+0xe6>
 801897e:	ea41 010c 	orr.w	r1, r1, ip
 8018982:	e78f      	b.n	80188a4 <__aeabi_fmul+0x18>
 8018984:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 8018988:	ea92 0f0c 	teq	r2, ip
 801898c:	bf18      	it	ne
 801898e:	ea93 0f0c 	teqne	r3, ip
 8018992:	d00a      	beq.n	80189aa <__aeabi_fmul+0x11e>
 8018994:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 8018998:	bf18      	it	ne
 801899a:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 801899e:	d1d8      	bne.n	8018952 <__aeabi_fmul+0xc6>
 80189a0:	ea80 0001 	eor.w	r0, r0, r1
 80189a4:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 80189a8:	4770      	bx	lr
 80189aa:	f090 0f00 	teq	r0, #0
 80189ae:	bf17      	itett	ne
 80189b0:	f090 4f00 	teqne	r0, #2147483648	; 0x80000000
 80189b4:	4608      	moveq	r0, r1
 80189b6:	f091 0f00 	teqne	r1, #0
 80189ba:	f091 4f00 	teqne	r1, #2147483648	; 0x80000000
 80189be:	d014      	beq.n	80189ea <__aeabi_fmul+0x15e>
 80189c0:	ea92 0f0c 	teq	r2, ip
 80189c4:	d101      	bne.n	80189ca <__aeabi_fmul+0x13e>
 80189c6:	0242      	lsls	r2, r0, #9
 80189c8:	d10f      	bne.n	80189ea <__aeabi_fmul+0x15e>
 80189ca:	ea93 0f0c 	teq	r3, ip
 80189ce:	d103      	bne.n	80189d8 <__aeabi_fmul+0x14c>
 80189d0:	024b      	lsls	r3, r1, #9
 80189d2:	bf18      	it	ne
 80189d4:	4608      	movne	r0, r1
 80189d6:	d108      	bne.n	80189ea <__aeabi_fmul+0x15e>
 80189d8:	ea80 0001 	eor.w	r0, r0, r1
 80189dc:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 80189e0:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 80189e4:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 80189e8:	4770      	bx	lr
 80189ea:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 80189ee:	f440 0040 	orr.w	r0, r0, #12582912	; 0xc00000
 80189f2:	4770      	bx	lr

080189f4 <__aeabi_fdiv>:
 80189f4:	f04f 0cff 	mov.w	ip, #255	; 0xff
 80189f8:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 80189fc:	bf1e      	ittt	ne
 80189fe:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 8018a02:	ea92 0f0c 	teqne	r2, ip
 8018a06:	ea93 0f0c 	teqne	r3, ip
 8018a0a:	d069      	beq.n	8018ae0 <__aeabi_fdiv+0xec>
 8018a0c:	eba2 0203 	sub.w	r2, r2, r3
 8018a10:	ea80 0c01 	eor.w	ip, r0, r1
 8018a14:	0249      	lsls	r1, r1, #9
 8018a16:	ea4f 2040 	mov.w	r0, r0, lsl #9
 8018a1a:	d037      	beq.n	8018a8c <__aeabi_fdiv+0x98>
 8018a1c:	f04f 5380 	mov.w	r3, #268435456	; 0x10000000
 8018a20:	ea43 1111 	orr.w	r1, r3, r1, lsr #4
 8018a24:	ea43 1310 	orr.w	r3, r3, r0, lsr #4
 8018a28:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 8018a2c:	428b      	cmp	r3, r1
 8018a2e:	bf38      	it	cc
 8018a30:	005b      	lslcc	r3, r3, #1
 8018a32:	f142 027d 	adc.w	r2, r2, #125	; 0x7d
 8018a36:	f44f 0c00 	mov.w	ip, #8388608	; 0x800000
 8018a3a:	428b      	cmp	r3, r1
 8018a3c:	bf24      	itt	cs
 8018a3e:	1a5b      	subcs	r3, r3, r1
 8018a40:	ea40 000c 	orrcs.w	r0, r0, ip
 8018a44:	ebb3 0f51 	cmp.w	r3, r1, lsr #1
 8018a48:	bf24      	itt	cs
 8018a4a:	eba3 0351 	subcs.w	r3, r3, r1, lsr #1
 8018a4e:	ea40 005c 	orrcs.w	r0, r0, ip, lsr #1
 8018a52:	ebb3 0f91 	cmp.w	r3, r1, lsr #2
 8018a56:	bf24      	itt	cs
 8018a58:	eba3 0391 	subcs.w	r3, r3, r1, lsr #2
 8018a5c:	ea40 009c 	orrcs.w	r0, r0, ip, lsr #2
 8018a60:	ebb3 0fd1 	cmp.w	r3, r1, lsr #3
 8018a64:	bf24      	itt	cs
 8018a66:	eba3 03d1 	subcs.w	r3, r3, r1, lsr #3
 8018a6a:	ea40 00dc 	orrcs.w	r0, r0, ip, lsr #3
 8018a6e:	011b      	lsls	r3, r3, #4
 8018a70:	bf18      	it	ne
 8018a72:	ea5f 1c1c 	movsne.w	ip, ip, lsr #4
 8018a76:	d1e0      	bne.n	8018a3a <__aeabi_fdiv+0x46>
 8018a78:	2afd      	cmp	r2, #253	; 0xfd
 8018a7a:	f63f af50 	bhi.w	801891e <__aeabi_fmul+0x92>
 8018a7e:	428b      	cmp	r3, r1
 8018a80:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 8018a84:	bf08      	it	eq
 8018a86:	f020 0001 	biceq.w	r0, r0, #1
 8018a8a:	4770      	bx	lr
 8018a8c:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 8018a90:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 8018a94:	327f      	adds	r2, #127	; 0x7f
 8018a96:	bfc2      	ittt	gt
 8018a98:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 8018a9c:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 8018aa0:	4770      	bxgt	lr
 8018aa2:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8018aa6:	f04f 0300 	mov.w	r3, #0
 8018aaa:	3a01      	subs	r2, #1
 8018aac:	e737      	b.n	801891e <__aeabi_fmul+0x92>
 8018aae:	f092 0f00 	teq	r2, #0
 8018ab2:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 8018ab6:	bf02      	ittt	eq
 8018ab8:	0040      	lsleq	r0, r0, #1
 8018aba:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 8018abe:	3a01      	subeq	r2, #1
 8018ac0:	d0f9      	beq.n	8018ab6 <__aeabi_fdiv+0xc2>
 8018ac2:	ea40 000c 	orr.w	r0, r0, ip
 8018ac6:	f093 0f00 	teq	r3, #0
 8018aca:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 8018ace:	bf02      	ittt	eq
 8018ad0:	0049      	lsleq	r1, r1, #1
 8018ad2:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 8018ad6:	3b01      	subeq	r3, #1
 8018ad8:	d0f9      	beq.n	8018ace <__aeabi_fdiv+0xda>
 8018ada:	ea41 010c 	orr.w	r1, r1, ip
 8018ade:	e795      	b.n	8018a0c <__aeabi_fdiv+0x18>
 8018ae0:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 8018ae4:	ea92 0f0c 	teq	r2, ip
 8018ae8:	d108      	bne.n	8018afc <__aeabi_fdiv+0x108>
 8018aea:	0242      	lsls	r2, r0, #9
 8018aec:	f47f af7d 	bne.w	80189ea <__aeabi_fmul+0x15e>
 8018af0:	ea93 0f0c 	teq	r3, ip
 8018af4:	f47f af70 	bne.w	80189d8 <__aeabi_fmul+0x14c>
 8018af8:	4608      	mov	r0, r1
 8018afa:	e776      	b.n	80189ea <__aeabi_fmul+0x15e>
 8018afc:	ea93 0f0c 	teq	r3, ip
 8018b00:	d104      	bne.n	8018b0c <__aeabi_fdiv+0x118>
 8018b02:	024b      	lsls	r3, r1, #9
 8018b04:	f43f af4c 	beq.w	80189a0 <__aeabi_fmul+0x114>
 8018b08:	4608      	mov	r0, r1
 8018b0a:	e76e      	b.n	80189ea <__aeabi_fmul+0x15e>
 8018b0c:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 8018b10:	bf18      	it	ne
 8018b12:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 8018b16:	d1ca      	bne.n	8018aae <__aeabi_fdiv+0xba>
 8018b18:	f030 4200 	bics.w	r2, r0, #2147483648	; 0x80000000
 8018b1c:	f47f af5c 	bne.w	80189d8 <__aeabi_fmul+0x14c>
 8018b20:	f031 4300 	bics.w	r3, r1, #2147483648	; 0x80000000
 8018b24:	f47f af3c 	bne.w	80189a0 <__aeabi_fmul+0x114>
 8018b28:	e75f      	b.n	80189ea <__aeabi_fmul+0x15e>
 8018b2a:	bf00      	nop
 8018b2c:	72617473 	.word	0x72617473
 8018b30:	6e000a74 	.word	0x6e000a74
 8018b34:	6620746f 	.word	0x6620746f
 8018b38:	646e756f 	.word	0x646e756f
 8018b3c:	3d54000a 	.word	0x3d54000a
 8018b40:	50206625 	.word	0x50206625
 8018b44:	0a64253d 	.word	0x0a64253d
	...

Disassembly of section .plt:

08018c24 <.plt>:
 8018c24:	b500      	push	{lr}
 8018c26:	f8df e008 	ldr.w	lr, [pc, #8]	; 8018c30 <.plt+0xc>
 8018c2a:	44fe      	add	lr, pc
 8018c2c:	f85e ff08 	ldr.w	pc, [lr, #8]!
 8018c30:	17fec45c 	.word	0x17fec45c

08018c34 <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt>:
 8018c34:	f24c 4c58 	movw	ip, #50264	; 0xc458
 8018c38:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018c3c:	44fc      	add	ip, pc
 8018c3e:	f8dc f000 	ldr.w	pc, [ip]
 8018c42:	bf00      	nop

08018c44 <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt>:
 8018c44:	0000      	movs	r0, r0
 8018c46:	0000      	movs	r0, r0
 8018c48:	f24c 4c48 	movw	ip, #50248	; 0xc448
 8018c4c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018c50:	44fc      	add	ip, pc
 8018c52:	Address 0x0000000008018c52 is out of bounds.


08018c54 <_ZN4BIOS4GPIO3I2C5WriteEh@plt>:
 8018c54:	f000 bf00 	b.w	8019a58 <_ZN4BIOS3SYS7DelayMsEi@plt+0xd94>
 8018c58:	f24c 4c3c 	movw	ip, #50236	; 0xc43c
 8018c5c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018c60:	44fc      	add	ip, pc
 8018c62:	Address 0x0000000008018c62 is out of bounds.


08018c64 <_ZN4BIOS4GPIO3I2C11RequestFromEhh@plt>:
 8018c64:	f000 bf00 	b.w	8019a68 <_ZN4BIOS3SYS7DelayMsEi@plt+0xda4>
 8018c68:	f24c 4c30 	movw	ip, #50224	; 0xc430
 8018c6c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018c70:	44fc      	add	ip, pc
 8018c72:	Address 0x0000000008018c72 is out of bounds.


08018c74 <_ZN4BIOS3DBG5PrintEPKcz@plt>:
 8018c74:	f000 bf00 	b.w	8019a78 <_ZN4BIOS3SYS7DelayMsEi@plt+0xdb4>
 8018c78:	f24c 4c24 	movw	ip, #50212	; 0xc424
 8018c7c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018c80:	44fc      	add	ip, pc
 8018c82:	Address 0x0000000008018c82 is out of bounds.


08018c84 <_ZN4BIOS3KEY6GetKeyEv@plt>:
 8018c84:	f000 bf00 	b.w	8019a88 <_ZN4BIOS3SYS7DelayMsEi@plt+0xdc4>
 8018c88:	f24c 4c18 	movw	ip, #50200	; 0xc418
 8018c8c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018c90:	44fc      	add	ip, pc
 8018c92:	Address 0x0000000008018c92 is out of bounds.


08018c94 <_ZN4BIOS4GPIO3I2C4ReadEv@plt>:
 8018c94:	f000 bf00 	b.w	8019a98 <_ZN4BIOS3SYS7DelayMsEi@plt+0xdd4>
 8018c98:	f24c 4c0c 	movw	ip, #50188	; 0xc40c
 8018c9c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018ca0:	44fc      	add	ip, pc
 8018ca2:	Address 0x0000000008018ca2 is out of bounds.


08018ca4 <_ZN4BIOS3LCD5ClearEt@plt>:
 8018ca4:	f000 bf00 	b.w	8019aa8 <_ZN4BIOS3SYS7DelayMsEi@plt+0xde4>
 8018ca8:	f24c 4c00 	movw	ip, #50176	; 0xc400
 8018cac:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018cb0:	44fc      	add	ip, pc
 8018cb2:	Address 0x0000000008018cb2 is out of bounds.


08018cb4 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>:
 8018cb4:	f000 bf00 	b.w	8019ab8 <_ZN4BIOS3SYS7DelayMsEi@plt+0xdf4>
 8018cb8:	f24c 3cf4 	movw	ip, #50164	; 0xc3f4
 8018cbc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018cc0:	44fc      	add	ip, pc
 8018cc2:	Address 0x0000000008018cc2 is out of bounds.


08018cc4 <_ZN4BIOS3SYS7DelayMsEi@plt>:
 8018cc4:	f000 bf00 	b.w	8019ac8 <_ZN4BIOS3SYS7DelayMsEi@plt+0xe04>
 8018cc8:	0000      	movs	r0, r0
 8018cca:	0000      	movs	r0, r0
 8018ccc:	f24c 3ce4 	movw	ip, #50148	; 0xc3e4
 8018cd0:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018cd4:	44fc      	add	ip, pc
 8018cd6:	f8dc f000 	ldr.w	pc, [ip]
 8018cda:	bf00      	nop
