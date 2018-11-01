
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08018000 <main>:
 8018000:	b508      	push	{r3, lr}
 8018002:	f242 1004 	movw	r0, #8452	; 0x2104
 8018006:	f001 fa15 	bl	8019434 <_ZN4BIOS3LCD5ClearEt@plt+0xc>
 801800a:	4808      	ldr	r0, [pc, #32]	; (801802c <main+0x2c>)
 801800c:	f001 f9fa 	bl	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 8018010:	f001 f804 	bl	801901c <_Z5setupv>
 8018014:	4806      	ldr	r0, [pc, #24]	; (8018030 <main+0x30>)
 8018016:	f001 f9f5 	bl	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 801801a:	f001 f9fb 	bl	8019414 <_ZN4BIOS3KEY6GetKeyEv@plt+0xc>
 801801e:	2806      	cmp	r0, #6
 8018020:	d002      	beq.n	8018028 <main+0x28>
 8018022:	f001 f87b 	bl	801911c <_Z4loopv>
 8018026:	e7f8      	b.n	801801a <main+0x1a>
 8018028:	2000      	movs	r0, #0
 801802a:	bd08      	pop	{r3, pc}
 801802c:	08019186 	.word	0x08019186
 8018030:	0801918f 	.word	0x0801918f

08018034 <__aeabi_atexit>:
 8018034:	b508      	push	{r3, lr}
 8018036:	4613      	mov	r3, r2
 8018038:	460a      	mov	r2, r1
 801803a:	4601      	mov	r1, r0
 801803c:	4802      	ldr	r0, [pc, #8]	; (8018048 <__aeabi_atexit+0x14>)
 801803e:	f001 f9e1 	bl	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 8018042:	2001      	movs	r0, #1
 8018044:	bd08      	pop	{r3, pc}
 8018046:	bf00      	nop
 8018048:	0801916c 	.word	0x0801916c

0801804c <_ZN7Arduino5delayEi>:
 801804c:	f001 ba06 	b.w	801945c <_ZN4BIOS3SYS7GetTickEv@plt+0x4>

08018050 <_ZN7Arduino6millisEv>:
 8018050:	f001 ba0e 	b.w	8019470 <_ZN4BIOS3SYS7GetTickEv@plt+0x18>

08018054 <_ZN7Arduino15attachInterruptEiPFvvEi>:
 8018054:	4770      	bx	lr

08018056 <_ZN7Arduino15detachInterruptEi>:
 8018056:	4770      	bx	lr

08018058 <_ZN7Arduino7pinModeEii>:
 8018058:	4770      	bx	lr

0801805a <_ZN7Arduino5CWire5beginEv>:
 801805a:	b508      	push	{r3, lr}
 801805c:	2120      	movs	r1, #32
 801805e:	2000      	movs	r0, #0
 8018060:	f001 f9f2 	bl	8019448 <_ZN4BIOS3SYS7DelayMsEi@plt>
 8018064:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 8018068:	2120      	movs	r1, #32
 801806a:	2001      	movs	r0, #1
 801806c:	f001 b9ec 	b.w	8019448 <_ZN4BIOS3SYS7DelayMsEi@plt>

08018070 <_ZN7Arduino5CWire17beginTransmissionEh>:
 8018070:	4608      	mov	r0, r1
 8018072:	f001 b9a3 	b.w	80193bc <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt+0x4>

08018076 <_ZN7Arduino5CWire5writeEh>:
 8018076:	4608      	mov	r0, r1
 8018078:	f001 b9b2 	b.w	80193e0 <_ZN4BIOS4GPIO3I2C5WriteEh@plt+0x8>

0801807c <_ZN7Arduino5CWire15endTransmissionEv>:
 801807c:	b508      	push	{r3, lr}
 801807e:	f001 f9a5 	bl	80193cc <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt+0x4>
 8018082:	2000      	movs	r0, #0
 8018084:	bd08      	pop	{r3, pc}

08018086 <_ZN7Arduino5CWire11requestFromEhi>:
 8018086:	b510      	push	{r4, lr}
 8018088:	4614      	mov	r4, r2
 801808a:	4608      	mov	r0, r1
 801808c:	b2d1      	uxtb	r1, r2
 801808e:	f001 f9af 	bl	80193f0 <_ZN4BIOS4GPIO3I2C11RequestFromEhh@plt+0x8>
 8018092:	4b01      	ldr	r3, [pc, #4]	; (8018098 <_ZN7Arduino5CWire11requestFromEhi+0x12>)
 8018094:	601c      	str	r4, [r3, #0]
 8018096:	bd10      	pop	{r4, pc}
 8018098:	200050ac 	.word	0x200050ac

0801809c <_ZN7Arduino5CWire4readEv>:
 801809c:	b510      	push	{r4, lr}
 801809e:	f001 f9c1 	bl	8019424 <_ZN4BIOS4GPIO3I2C4ReadEv@plt+0xc>
 80180a2:	4a05      	ldr	r2, [pc, #20]	; (80180b8 <_ZN7Arduino5CWire4readEv+0x1c>)
 80180a4:	4604      	mov	r4, r0
 80180a6:	6813      	ldr	r3, [r2, #0]
 80180a8:	3b01      	subs	r3, #1
 80180aa:	6013      	str	r3, [r2, #0]
 80180ac:	b90b      	cbnz	r3, 80180b2 <_ZN7Arduino5CWire4readEv+0x16>
 80180ae:	f001 f98d 	bl	80193cc <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt+0x4>
 80180b2:	4620      	mov	r0, r4
 80180b4:	bd10      	pop	{r4, pc}
 80180b6:	bf00      	nop
 80180b8:	200050ac 	.word	0x200050ac

080180bc <_ZN7Arduino5CWire9availableEv>:
 80180bc:	4b03      	ldr	r3, [pc, #12]	; (80180cc <_ZN7Arduino5CWire9availableEv+0x10>)
 80180be:	6818      	ldr	r0, [r3, #0]
 80180c0:	2800      	cmp	r0, #0
 80180c2:	bfd4      	ite	le
 80180c4:	2000      	movle	r0, #0
 80180c6:	2001      	movgt	r0, #1
 80180c8:	4770      	bx	lr
 80180ca:	bf00      	nop
 80180cc:	200050ac 	.word	0x200050ac

080180d0 <_ZN7Arduino7CSerial7printlnEPKc>:
 80180d0:	4801      	ldr	r0, [pc, #4]	; (80180d8 <_ZN7Arduino7CSerial7printlnEPKc+0x8>)
 80180d2:	f001 b997 	b.w	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 80180d6:	bf00      	nop
 80180d8:	08019197 	.word	0x08019197

080180dc <_ZN7Arduino7CSerial7printlnEi>:
 80180dc:	4801      	ldr	r0, [pc, #4]	; (80180e4 <_ZN7Arduino7CSerial7printlnEi+0x8>)
 80180de:	f001 b991 	b.w	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 80180e2:	bf00      	nop
 80180e4:	0801919b 	.word	0x0801919b

080180e8 <_ZN7Arduino7CSerial7printlnEii>:
 80180e8:	4801      	ldr	r0, [pc, #4]	; (80180f0 <_ZN7Arduino7CSerial7printlnEii+0x8>)
 80180ea:	f001 b98b 	b.w	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 80180ee:	bf00      	nop
 80180f0:	0801919f 	.word	0x0801919f

080180f4 <_ZN7Arduino7CSerial5beginEi>:
 80180f4:	4770      	bx	lr

080180f6 <_ZN7Arduino7CSerial9availableEv>:
 80180f6:	2000      	movs	r0, #0
 80180f8:	4770      	bx	lr

080180fa <_ZN7Arduino7CSerial4readEv>:
 80180fa:	2000      	movs	r0, #0
 80180fc:	4770      	bx	lr

080180fe <_ZN7Arduino7CSerial5writeEPhi>:
 80180fe:	b570      	push	{r4, r5, r6, lr}
 8018100:	460c      	mov	r4, r1
 8018102:	4d05      	ldr	r5, [pc, #20]	; (8018118 <_ZN7Arduino7CSerial5writeEPhi+0x1a>)
 8018104:	188e      	adds	r6, r1, r2
 8018106:	42b4      	cmp	r4, r6
 8018108:	d005      	beq.n	8018116 <_ZN7Arduino7CSerial5writeEPhi+0x18>
 801810a:	f814 1b01 	ldrb.w	r1, [r4], #1
 801810e:	4628      	mov	r0, r5
 8018110:	f001 f978 	bl	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 8018114:	e7f7      	b.n	8018106 <_ZN7Arduino7CSerial5writeEPhi+0x8>
 8018116:	bd70      	pop	{r4, r5, r6, pc}
 8018118:	080191a3 	.word	0x080191a3

0801811c <_ZN7Arduino7CSerial5printEPKc>:
 801811c:	4608      	mov	r0, r1
 801811e:	f001 b971 	b.w	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>

08018122 <_ZN7Arduino7CSerial5printEi>:
 8018122:	4801      	ldr	r0, [pc, #4]	; (8018128 <_ZN7Arduino7CSerial5printEi+0x6>)
 8018124:	f001 b96e 	b.w	8019404 <_ZN4BIOS3DBG5PrintEPKcz@plt+0xc>
 8018128:	080191a6 	.word	0x080191a6

0801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>:
 801812c:	b570      	push	{r4, r5, r6, lr}
 801812e:	4606      	mov	r6, r0
 8018130:	460d      	mov	r5, r1
 8018132:	4c0a      	ldr	r4, [pc, #40]	; (801815c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3+0x30>)
 8018134:	2139      	movs	r1, #57	; 0x39
 8018136:	4620      	mov	r0, r4
 8018138:	f7ff ff9a 	bl	8018070 <_ZN7Arduino5CWire17beginTransmissionEh>
 801813c:	4631      	mov	r1, r6
 801813e:	4620      	mov	r0, r4
 8018140:	f7ff ff99 	bl	8018076 <_ZN7Arduino5CWire5writeEh>
 8018144:	4629      	mov	r1, r5
 8018146:	4620      	mov	r0, r4
 8018148:	f7ff ff95 	bl	8018076 <_ZN7Arduino5CWire5writeEh>
 801814c:	4620      	mov	r0, r4
 801814e:	f7ff ff95 	bl	801807c <_ZN7Arduino5CWire15endTransmissionEv>
 8018152:	fab0 f080 	clz	r0, r0
 8018156:	0940      	lsrs	r0, r0, #5
 8018158:	bd70      	pop	{r4, r5, r6, pc}
 801815a:	bf00      	nop
 801815c:	200050b0 	.word	0x200050b0

08018160 <_ZN17SparkFun_APDS9960C1Ev>:
 8018160:	2200      	movs	r2, #0
 8018162:	e9c0 2221 	strd	r2, r2, [r0, #132]	; 0x84
 8018166:	e9c0 2223 	strd	r2, r2, [r0, #140]	; 0x8c
 801816a:	e9c0 2225 	strd	r2, r2, [r0, #148]	; 0x94
 801816e:	e9c0 2227 	strd	r2, r2, [r0, #156]	; 0x9c
 8018172:	4770      	bx	lr

08018174 <_ZN17SparkFun_APDS9960D1Ev>:
 8018174:	4770      	bx	lr

08018176 <_ZN17SparkFun_APDS996022resetGestureParametersEv>:
 8018176:	2300      	movs	r3, #0
 8018178:	f8a0 3080 	strh.w	r3, [r0, #128]	; 0x80
 801817c:	e9c0 3321 	strd	r3, r3, [r0, #132]	; 0x84
 8018180:	e9c0 3323 	strd	r3, r3, [r0, #140]	; 0x8c
 8018184:	e9c0 3325 	strd	r3, r3, [r0, #148]	; 0x94
 8018188:	e9c0 3327 	strd	r3, r3, [r0, #156]	; 0x9c
 801818c:	4770      	bx	lr

0801818e <_ZN17SparkFun_APDS996018processGestureDataEv>:
 801818e:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8018192:	f890 1081 	ldrb.w	r1, [r0, #129]	; 0x81
 8018196:	2904      	cmp	r1, #4
 8018198:	d803      	bhi.n	80181a2 <_ZN17SparkFun_APDS996018processGestureDataEv+0x14>
 801819a:	2300      	movs	r3, #0
 801819c:	4618      	mov	r0, r3
 801819e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 80181a2:	1e4b      	subs	r3, r1, #1
 80181a4:	2b1f      	cmp	r3, #31
 80181a6:	f200 8084 	bhi.w	80182b2 <_ZN17SparkFun_APDS996018processGestureDataEv+0x124>
 80181aa:	4603      	mov	r3, r0
 80181ac:	1a1a      	subs	r2, r3, r0
 80181ae:	4291      	cmp	r1, r2
 80181b0:	ddf3      	ble.n	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 80181b2:	f813 5b01 	ldrb.w	r5, [r3], #1
 80181b6:	2d0a      	cmp	r5, #10
 80181b8:	d9f8      	bls.n	80181ac <_ZN17SparkFun_APDS996018processGestureDataEv+0x1e>
 80181ba:	f893 e01f 	ldrb.w	lr, [r3, #31]
 80181be:	f1be 0f0a 	cmp.w	lr, #10
 80181c2:	d9f3      	bls.n	80181ac <_ZN17SparkFun_APDS996018processGestureDataEv+0x1e>
 80181c4:	f893 603f 	ldrb.w	r6, [r3, #63]	; 0x3f
 80181c8:	2e0a      	cmp	r6, #10
 80181ca:	d9ef      	bls.n	80181ac <_ZN17SparkFun_APDS996018processGestureDataEv+0x1e>
 80181cc:	f893 705f 	ldrb.w	r7, [r3, #95]	; 0x5f
 80181d0:	2f0a      	cmp	r7, #10
 80181d2:	d9eb      	bls.n	80181ac <_ZN17SparkFun_APDS996018processGestureDataEv+0x1e>
 80181d4:	4401      	add	r1, r0
 80181d6:	f811 4d01 	ldrb.w	r4, [r1, #-1]!
 80181da:	2c0a      	cmp	r4, #10
 80181dc:	d90d      	bls.n	80181fa <_ZN17SparkFun_APDS996018processGestureDataEv+0x6c>
 80181de:	f891 8020 	ldrb.w	r8, [r1, #32]
 80181e2:	f1b8 0f0a 	cmp.w	r8, #10
 80181e6:	d908      	bls.n	80181fa <_ZN17SparkFun_APDS996018processGestureDataEv+0x6c>
 80181e8:	f891 c040 	ldrb.w	ip, [r1, #64]	; 0x40
 80181ec:	f1bc 0f0a 	cmp.w	ip, #10
 80181f0:	d903      	bls.n	80181fa <_ZN17SparkFun_APDS996018processGestureDataEv+0x6c>
 80181f2:	f891 3060 	ldrb.w	r3, [r1, #96]	; 0x60
 80181f6:	2b0a      	cmp	r3, #10
 80181f8:	d805      	bhi.n	8018206 <_ZN17SparkFun_APDS996018processGestureDataEv+0x78>
 80181fa:	4288      	cmp	r0, r1
 80181fc:	d1eb      	bne.n	80181d6 <_ZN17SparkFun_APDS996018processGestureDataEv+0x48>
 80181fe:	2300      	movs	r3, #0
 8018200:	469c      	mov	ip, r3
 8018202:	4698      	mov	r8, r3
 8018204:	461c      	mov	r4, r3
 8018206:	2264      	movs	r2, #100	; 0x64
 8018208:	eba4 0108 	sub.w	r1, r4, r8
 801820c:	4351      	muls	r1, r2
 801820e:	4444      	add	r4, r8
 8018210:	fb91 f4f4 	sdiv	r4, r1, r4
 8018214:	eba5 010e 	sub.w	r1, r5, lr
 8018218:	4351      	muls	r1, r2
 801821a:	4475      	add	r5, lr
 801821c:	fb91 f5f5 	sdiv	r5, r1, r5
 8018220:	1bf1      	subs	r1, r6, r7
 8018222:	1b64      	subs	r4, r4, r5
 8018224:	ebac 0503 	sub.w	r5, ip, r3
 8018228:	4351      	muls	r1, r2
 801822a:	4355      	muls	r5, r2
 801822c:	4463      	add	r3, ip
 801822e:	19f2      	adds	r2, r6, r7
 8018230:	fb95 f3f3 	sdiv	r3, r5, r3
 8018234:	fb91 f2f2 	sdiv	r2, r1, r2
 8018238:	f8d0 1084 	ldr.w	r1, [r0, #132]	; 0x84
 801823c:	1a9a      	subs	r2, r3, r2
 801823e:	f8d0 3088 	ldr.w	r3, [r0, #136]	; 0x88
 8018242:	4421      	add	r1, r4
 8018244:	4413      	add	r3, r2
 8018246:	2931      	cmp	r1, #49	; 0x31
 8018248:	f8c0 1084 	str.w	r1, [r0, #132]	; 0x84
 801824c:	f8c0 3088 	str.w	r3, [r0, #136]	; 0x88
 8018250:	dd38      	ble.n	80182c4 <_ZN17SparkFun_APDS996018processGestureDataEv+0x136>
 8018252:	2101      	movs	r1, #1
 8018254:	2b31      	cmp	r3, #49	; 0x31
 8018256:	f8c0 108c 	str.w	r1, [r0, #140]	; 0x8c
 801825a:	dd39      	ble.n	80182d0 <_ZN17SparkFun_APDS996018processGestureDataEv+0x142>
 801825c:	2301      	movs	r3, #1
 801825e:	f8d0 108c 	ldr.w	r1, [r0, #140]	; 0x8c
 8018262:	f8c0 3090 	str.w	r3, [r0, #144]	; 0x90
 8018266:	f104 0313 	add.w	r3, r4, #19
 801826a:	2900      	cmp	r1, #0
 801826c:	d143      	bne.n	80182f6 <_ZN17SparkFun_APDS996018processGestureDataEv+0x168>
 801826e:	f8d0 1090 	ldr.w	r1, [r0, #144]	; 0x90
 8018272:	2900      	cmp	r1, #0
 8018274:	d13f      	bne.n	80182f6 <_ZN17SparkFun_APDS996018processGestureDataEv+0x168>
 8018276:	2b26      	cmp	r3, #38	; 0x26
 8018278:	d88f      	bhi.n	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 801827a:	f102 0313 	add.w	r3, r2, #19
 801827e:	2b26      	cmp	r3, #38	; 0x26
 8018280:	d88b      	bhi.n	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 8018282:	bb5c      	cbnz	r4, 80182dc <_ZN17SparkFun_APDS996018processGestureDataEv+0x14e>
 8018284:	bb52      	cbnz	r2, 80182dc <_ZN17SparkFun_APDS996018processGestureDataEv+0x14e>
 8018286:	f8d0 3094 	ldr.w	r3, [r0, #148]	; 0x94
 801828a:	3301      	adds	r3, #1
 801828c:	f8c0 3094 	str.w	r3, [r0, #148]	; 0x94
 8018290:	f8d0 3094 	ldr.w	r3, [r0, #148]	; 0x94
 8018294:	2b09      	cmp	r3, #9
 8018296:	dd80      	ble.n	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 8018298:	f8d0 3098 	ldr.w	r3, [r0, #152]	; 0x98
 801829c:	2b01      	cmp	r3, #1
 801829e:	f77f af7c 	ble.w	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 80182a2:	2301      	movs	r3, #1
 80182a4:	bb04      	cbnz	r4, 80182e8 <_ZN17SparkFun_APDS996018processGestureDataEv+0x15a>
 80182a6:	2a00      	cmp	r2, #0
 80182a8:	f47f af78 	bne.w	801819c <_ZN17SparkFun_APDS996018processGestureDataEv+0xe>
 80182ac:	f8c0 309c 	str.w	r3, [r0, #156]	; 0x9c
 80182b0:	e774      	b.n	801819c <_ZN17SparkFun_APDS996018processGestureDataEv+0xe>
 80182b2:	2300      	movs	r3, #0
 80182b4:	469c      	mov	ip, r3
 80182b6:	4698      	mov	r8, r3
 80182b8:	461c      	mov	r4, r3
 80182ba:	461f      	mov	r7, r3
 80182bc:	461e      	mov	r6, r3
 80182be:	469e      	mov	lr, r3
 80182c0:	461d      	mov	r5, r3
 80182c2:	e7a0      	b.n	8018206 <_ZN17SparkFun_APDS996018processGestureDataEv+0x78>
 80182c4:	3131      	adds	r1, #49	; 0x31
 80182c6:	bfb4      	ite	lt
 80182c8:	f04f 31ff 	movlt.w	r1, #4294967295	; 0xffffffff
 80182cc:	2100      	movge	r1, #0
 80182ce:	e7c1      	b.n	8018254 <_ZN17SparkFun_APDS996018processGestureDataEv+0xc6>
 80182d0:	3331      	adds	r3, #49	; 0x31
 80182d2:	bfb4      	ite	lt
 80182d4:	f04f 33ff 	movlt.w	r3, #4294967295	; 0xffffffff
 80182d8:	2300      	movge	r3, #0
 80182da:	e7c0      	b.n	801825e <_ZN17SparkFun_APDS996018processGestureDataEv+0xd0>
 80182dc:	f8d0 3098 	ldr.w	r3, [r0, #152]	; 0x98
 80182e0:	3301      	adds	r3, #1
 80182e2:	f8c0 3098 	str.w	r3, [r0, #152]	; 0x98
 80182e6:	e7d3      	b.n	8018290 <_ZN17SparkFun_APDS996018processGestureDataEv+0x102>
 80182e8:	2a00      	cmp	r2, #0
 80182ea:	f43f af57 	beq.w	801819c <_ZN17SparkFun_APDS996018processGestureDataEv+0xe>
 80182ee:	2202      	movs	r2, #2
 80182f0:	f8c0 209c 	str.w	r2, [r0, #156]	; 0x9c
 80182f4:	e752      	b.n	801819c <_ZN17SparkFun_APDS996018processGestureDataEv+0xe>
 80182f6:	2b26      	cmp	r3, #38	; 0x26
 80182f8:	f63f af4f 	bhi.w	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 80182fc:	f102 0313 	add.w	r3, r2, #19
 8018300:	2b26      	cmp	r3, #38	; 0x26
 8018302:	f63f af4a 	bhi.w	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 8018306:	b92c      	cbnz	r4, 8018314 <_ZN17SparkFun_APDS996018processGestureDataEv+0x186>
 8018308:	b922      	cbnz	r2, 8018314 <_ZN17SparkFun_APDS996018processGestureDataEv+0x186>
 801830a:	f8d0 3094 	ldr.w	r3, [r0, #148]	; 0x94
 801830e:	3301      	adds	r3, #1
 8018310:	f8c0 3094 	str.w	r3, [r0, #148]	; 0x94
 8018314:	f8d0 3094 	ldr.w	r3, [r0, #148]	; 0x94
 8018318:	2b09      	cmp	r3, #9
 801831a:	f77f af3e 	ble.w	801819a <_ZN17SparkFun_APDS996018processGestureDataEv+0xc>
 801831e:	2300      	movs	r3, #0
 8018320:	e9c0 3323 	strd	r3, r3, [r0, #140]	; 0x8c
 8018324:	e9c0 3321 	strd	r3, r3, [r0, #132]	; 0x84
 8018328:	e738      	b.n	801819c <_ZN17SparkFun_APDS996018processGestureDataEv+0xe>

0801832a <_ZN17SparkFun_APDS996013decodeGestureEv>:
 801832a:	f8d0 309c 	ldr.w	r3, [r0, #156]	; 0x9c
 801832e:	2b01      	cmp	r3, #1
 8018330:	d104      	bne.n	801833c <_ZN17SparkFun_APDS996013decodeGestureEv+0x12>
 8018332:	2205      	movs	r2, #5
 8018334:	f8c0 20a0 	str.w	r2, [r0, #160]	; 0xa0
 8018338:	4618      	mov	r0, r3
 801833a:	4770      	bx	lr
 801833c:	2b02      	cmp	r3, #2
 801833e:	d104      	bne.n	801834a <_ZN17SparkFun_APDS996013decodeGestureEv+0x20>
 8018340:	2306      	movs	r3, #6
 8018342:	f8c0 30a0 	str.w	r3, [r0, #160]	; 0xa0
 8018346:	2301      	movs	r3, #1
 8018348:	e7f6      	b.n	8018338 <_ZN17SparkFun_APDS996013decodeGestureEv+0xe>
 801834a:	f8d0 308c 	ldr.w	r3, [r0, #140]	; 0x8c
 801834e:	1c59      	adds	r1, r3, #1
 8018350:	d104      	bne.n	801835c <_ZN17SparkFun_APDS996013decodeGestureEv+0x32>
 8018352:	f8d0 3090 	ldr.w	r3, [r0, #144]	; 0x90
 8018356:	b9b3      	cbnz	r3, 8018386 <_ZN17SparkFun_APDS996013decodeGestureEv+0x5c>
 8018358:	2303      	movs	r3, #3
 801835a:	e7f2      	b.n	8018342 <_ZN17SparkFun_APDS996013decodeGestureEv+0x18>
 801835c:	2b01      	cmp	r3, #1
 801835e:	d104      	bne.n	801836a <_ZN17SparkFun_APDS996013decodeGestureEv+0x40>
 8018360:	f8d0 3090 	ldr.w	r3, [r0, #144]	; 0x90
 8018364:	b9fb      	cbnz	r3, 80183a6 <_ZN17SparkFun_APDS996013decodeGestureEv+0x7c>
 8018366:	2304      	movs	r3, #4
 8018368:	e7eb      	b.n	8018342 <_ZN17SparkFun_APDS996013decodeGestureEv+0x18>
 801836a:	2b00      	cmp	r3, #0
 801836c:	d14b      	bne.n	8018406 <_ZN17SparkFun_APDS996013decodeGestureEv+0xdc>
 801836e:	f8d0 2090 	ldr.w	r2, [r0, #144]	; 0x90
 8018372:	2a01      	cmp	r2, #1
 8018374:	d101      	bne.n	801837a <_ZN17SparkFun_APDS996013decodeGestureEv+0x50>
 8018376:	2302      	movs	r3, #2
 8018378:	e7e3      	b.n	8018342 <_ZN17SparkFun_APDS996013decodeGestureEv+0x18>
 801837a:	3201      	adds	r2, #1
 801837c:	d1dc      	bne.n	8018338 <_ZN17SparkFun_APDS996013decodeGestureEv+0xe>
 801837e:	2301      	movs	r3, #1
 8018380:	f8c0 30a0 	str.w	r3, [r0, #160]	; 0xa0
 8018384:	e7d8      	b.n	8018338 <_ZN17SparkFun_APDS996013decodeGestureEv+0xe>
 8018386:	2b01      	cmp	r3, #1
 8018388:	d11d      	bne.n	80183c6 <_ZN17SparkFun_APDS996013decodeGestureEv+0x9c>
 801838a:	f8d0 3084 	ldr.w	r3, [r0, #132]	; 0x84
 801838e:	ea83 72e3 	eor.w	r2, r3, r3, asr #31
 8018392:	eba2 72e3 	sub.w	r2, r2, r3, asr #31
 8018396:	f8d0 3088 	ldr.w	r3, [r0, #136]	; 0x88
 801839a:	2b00      	cmp	r3, #0
 801839c:	bfb8      	it	lt
 801839e:	425b      	neglt	r3, r3
 80183a0:	429a      	cmp	r2, r3
 80183a2:	dcd9      	bgt.n	8018358 <_ZN17SparkFun_APDS996013decodeGestureEv+0x2e>
 80183a4:	e7e7      	b.n	8018376 <_ZN17SparkFun_APDS996013decodeGestureEv+0x4c>
 80183a6:	1c5a      	adds	r2, r3, #1
 80183a8:	d11d      	bne.n	80183e6 <_ZN17SparkFun_APDS996013decodeGestureEv+0xbc>
 80183aa:	f8d0 3084 	ldr.w	r3, [r0, #132]	; 0x84
 80183ae:	ea83 72e3 	eor.w	r2, r3, r3, asr #31
 80183b2:	eba2 72e3 	sub.w	r2, r2, r3, asr #31
 80183b6:	f8d0 3088 	ldr.w	r3, [r0, #136]	; 0x88
 80183ba:	2b00      	cmp	r3, #0
 80183bc:	bfb8      	it	lt
 80183be:	425b      	neglt	r3, r3
 80183c0:	429a      	cmp	r2, r3
 80183c2:	dcd0      	bgt.n	8018366 <_ZN17SparkFun_APDS996013decodeGestureEv+0x3c>
 80183c4:	e7db      	b.n	801837e <_ZN17SparkFun_APDS996013decodeGestureEv+0x54>
 80183c6:	3301      	adds	r3, #1
 80183c8:	d11d      	bne.n	8018406 <_ZN17SparkFun_APDS996013decodeGestureEv+0xdc>
 80183ca:	f8d0 3084 	ldr.w	r3, [r0, #132]	; 0x84
 80183ce:	ea83 72e3 	eor.w	r2, r3, r3, asr #31
 80183d2:	eba2 72e3 	sub.w	r2, r2, r3, asr #31
 80183d6:	f8d0 3088 	ldr.w	r3, [r0, #136]	; 0x88
 80183da:	2b00      	cmp	r3, #0
 80183dc:	bfb8      	it	lt
 80183de:	425b      	neglt	r3, r3
 80183e0:	429a      	cmp	r2, r3
 80183e2:	dcb9      	bgt.n	8018358 <_ZN17SparkFun_APDS996013decodeGestureEv+0x2e>
 80183e4:	e7cb      	b.n	801837e <_ZN17SparkFun_APDS996013decodeGestureEv+0x54>
 80183e6:	2b01      	cmp	r3, #1
 80183e8:	d10d      	bne.n	8018406 <_ZN17SparkFun_APDS996013decodeGestureEv+0xdc>
 80183ea:	f8d0 3084 	ldr.w	r3, [r0, #132]	; 0x84
 80183ee:	ea83 72e3 	eor.w	r2, r3, r3, asr #31
 80183f2:	eba2 72e3 	sub.w	r2, r2, r3, asr #31
 80183f6:	f8d0 3088 	ldr.w	r3, [r0, #136]	; 0x88
 80183fa:	2b00      	cmp	r3, #0
 80183fc:	bfb8      	it	lt
 80183fe:	425b      	neglt	r3, r3
 8018400:	429a      	cmp	r2, r3
 8018402:	ddb8      	ble.n	8018376 <_ZN17SparkFun_APDS996013decodeGestureEv+0x4c>
 8018404:	e7af      	b.n	8018366 <_ZN17SparkFun_APDS996013decodeGestureEv+0x3c>
 8018406:	2300      	movs	r3, #0
 8018408:	e796      	b.n	8018338 <_ZN17SparkFun_APDS996013decodeGestureEv+0xe>

0801840a <_ZN17SparkFun_APDS996021setGestureEnterThreshEh>:
 801840a:	20a0      	movs	r0, #160	; 0xa0
 801840c:	e68e      	b.n	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>

0801840e <_ZN17SparkFun_APDS996020setGestureExitThreshEh>:
 801840e:	20a1      	movs	r0, #161	; 0xa1
 8018410:	e68c      	b.n	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>

08018412 <_ZN17SparkFun_APDS996023setLightIntLowThresholdEt>:
 8018412:	b510      	push	{r4, lr}
 8018414:	2084      	movs	r0, #132	; 0x84
 8018416:	460c      	mov	r4, r1
 8018418:	b2c9      	uxtb	r1, r1
 801841a:	f7ff fe87 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 801841e:	b120      	cbz	r0, 801842a <_ZN17SparkFun_APDS996023setLightIntLowThresholdEt+0x18>
 8018420:	0a21      	lsrs	r1, r4, #8
 8018422:	2085      	movs	r0, #133	; 0x85
 8018424:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8018428:	e680      	b.n	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 801842a:	bd10      	pop	{r4, pc}

0801842c <_ZN17SparkFun_APDS996024setLightIntHighThresholdEt>:
 801842c:	b510      	push	{r4, lr}
 801842e:	2086      	movs	r0, #134	; 0x86
 8018430:	460c      	mov	r4, r1
 8018432:	b2c9      	uxtb	r1, r1
 8018434:	f7ff fe7a 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018438:	b120      	cbz	r0, 8018444 <_ZN17SparkFun_APDS996024setLightIntHighThresholdEt+0x18>
 801843a:	0a21      	lsrs	r1, r4, #8
 801843c:	2087      	movs	r0, #135	; 0x87
 801843e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8018442:	e673      	b.n	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018444:	bd10      	pop	{r4, pc}

08018446 <_ZN17SparkFun_APDS996027setProximityIntLowThresholdEh>:
 8018446:	2089      	movs	r0, #137	; 0x89
 8018448:	e670      	b.n	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>

0801844a <_ZN17SparkFun_APDS996019setProxIntLowThreshEh>:
 801844a:	f7ff bffc 	b.w	8018446 <_ZN17SparkFun_APDS996027setProximityIntLowThresholdEh>

0801844e <_ZN17SparkFun_APDS996028setProximityIntHighThresholdEh>:
 801844e:	208b      	movs	r0, #139	; 0x8b
 8018450:	e66c      	b.n	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>

08018452 <_ZN17SparkFun_APDS996020setProxIntHighThreshEh>:
 8018452:	f7ff bffc 	b.w	801844e <_ZN17SparkFun_APDS996028setProximityIntHighThresholdEh>

08018456 <_ZN17SparkFun_APDS996013wireWriteByteEh>:
 8018456:	b538      	push	{r3, r4, r5, lr}
 8018458:	460d      	mov	r5, r1
 801845a:	4c08      	ldr	r4, [pc, #32]	; (801847c <_ZN17SparkFun_APDS996013wireWriteByteEh+0x26>)
 801845c:	2139      	movs	r1, #57	; 0x39
 801845e:	4620      	mov	r0, r4
 8018460:	f7ff fe06 	bl	8018070 <_ZN7Arduino5CWire17beginTransmissionEh>
 8018464:	4629      	mov	r1, r5
 8018466:	4620      	mov	r0, r4
 8018468:	f7ff fe05 	bl	8018076 <_ZN7Arduino5CWire5writeEh>
 801846c:	4620      	mov	r0, r4
 801846e:	f7ff fe05 	bl	801807c <_ZN7Arduino5CWire15endTransmissionEv>
 8018472:	fab0 f080 	clz	r0, r0
 8018476:	0940      	lsrs	r0, r0, #5
 8018478:	bd38      	pop	{r3, r4, r5, pc}
 801847a:	bf00      	nop
 801847c:	200050b0 	.word	0x200050b0

08018480 <_ZN17SparkFun_APDS996017wireWriteDataByteEhh>:
 8018480:	b570      	push	{r4, r5, r6, lr}
 8018482:	460e      	mov	r6, r1
 8018484:	4615      	mov	r5, r2
 8018486:	4c0a      	ldr	r4, [pc, #40]	; (80184b0 <_ZN17SparkFun_APDS996017wireWriteDataByteEhh+0x30>)
 8018488:	2139      	movs	r1, #57	; 0x39
 801848a:	4620      	mov	r0, r4
 801848c:	f7ff fdf0 	bl	8018070 <_ZN7Arduino5CWire17beginTransmissionEh>
 8018490:	4631      	mov	r1, r6
 8018492:	4620      	mov	r0, r4
 8018494:	f7ff fdef 	bl	8018076 <_ZN7Arduino5CWire5writeEh>
 8018498:	4629      	mov	r1, r5
 801849a:	4620      	mov	r0, r4
 801849c:	f7ff fdeb 	bl	8018076 <_ZN7Arduino5CWire5writeEh>
 80184a0:	4620      	mov	r0, r4
 80184a2:	f7ff fdeb 	bl	801807c <_ZN7Arduino5CWire15endTransmissionEv>
 80184a6:	fab0 f080 	clz	r0, r0
 80184aa:	0940      	lsrs	r0, r0, #5
 80184ac:	bd70      	pop	{r4, r5, r6, pc}
 80184ae:	bf00      	nop
 80184b0:	200050b0 	.word	0x200050b0

080184b4 <_ZN17SparkFun_APDS996018wireWriteDataBlockEhPhj>:
 80184b4:	b570      	push	{r4, r5, r6, lr}
 80184b6:	460e      	mov	r6, r1
 80184b8:	480d      	ldr	r0, [pc, #52]	; (80184f0 <_ZN17SparkFun_APDS996018wireWriteDataBlockEhPhj+0x3c>)
 80184ba:	2139      	movs	r1, #57	; 0x39
 80184bc:	4614      	mov	r4, r2
 80184be:	461d      	mov	r5, r3
 80184c0:	f7ff fdd6 	bl	8018070 <_ZN7Arduino5CWire17beginTransmissionEh>
 80184c4:	4631      	mov	r1, r6
 80184c6:	480a      	ldr	r0, [pc, #40]	; (80184f0 <_ZN17SparkFun_APDS996018wireWriteDataBlockEhPhj+0x3c>)
 80184c8:	f7ff fdd5 	bl	8018076 <_ZN7Arduino5CWire5writeEh>
 80184cc:	4e08      	ldr	r6, [pc, #32]	; (80184f0 <_ZN17SparkFun_APDS996018wireWriteDataBlockEhPhj+0x3c>)
 80184ce:	4425      	add	r5, r4
 80184d0:	42ac      	cmp	r4, r5
 80184d2:	d005      	beq.n	80184e0 <_ZN17SparkFun_APDS996018wireWriteDataBlockEhPhj+0x2c>
 80184d4:	f814 1b01 	ldrb.w	r1, [r4], #1
 80184d8:	4630      	mov	r0, r6
 80184da:	f7ff fdc9 	bl	8018070 <_ZN7Arduino5CWire17beginTransmissionEh>
 80184de:	e7f7      	b.n	80184d0 <_ZN17SparkFun_APDS996018wireWriteDataBlockEhPhj+0x1c>
 80184e0:	4803      	ldr	r0, [pc, #12]	; (80184f0 <_ZN17SparkFun_APDS996018wireWriteDataBlockEhPhj+0x3c>)
 80184e2:	f7ff fdcb 	bl	801807c <_ZN7Arduino5CWire15endTransmissionEv>
 80184e6:	fab0 f080 	clz	r0, r0
 80184ea:	0940      	lsrs	r0, r0, #5
 80184ec:	bd70      	pop	{r4, r5, r6, pc}
 80184ee:	bf00      	nop
 80184f0:	200050b0 	.word	0x200050b0

080184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>:
 80184f4:	b570      	push	{r4, r5, r6, lr}
 80184f6:	4616      	mov	r6, r2
 80184f8:	f7ff ffad 	bl	8018456 <_ZN17SparkFun_APDS996013wireWriteByteEh>
 80184fc:	4604      	mov	r4, r0
 80184fe:	b170      	cbz	r0, 801851e <_ZN17SparkFun_APDS996016wireReadDataByteEhRh+0x2a>
 8018500:	2201      	movs	r2, #1
 8018502:	2139      	movs	r1, #57	; 0x39
 8018504:	4807      	ldr	r0, [pc, #28]	; (8018524 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh+0x30>)
 8018506:	f7ff fdbe 	bl	8018086 <_ZN7Arduino5CWire11requestFromEhi>
 801850a:	4d06      	ldr	r5, [pc, #24]	; (8018524 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh+0x30>)
 801850c:	4628      	mov	r0, r5
 801850e:	f7ff fdd5 	bl	80180bc <_ZN7Arduino5CWire9availableEv>
 8018512:	b120      	cbz	r0, 801851e <_ZN17SparkFun_APDS996016wireReadDataByteEhRh+0x2a>
 8018514:	4628      	mov	r0, r5
 8018516:	f7ff fdc1 	bl	801809c <_ZN7Arduino5CWire4readEv>
 801851a:	7030      	strb	r0, [r6, #0]
 801851c:	e7f6      	b.n	801850c <_ZN17SparkFun_APDS996016wireReadDataByteEhRh+0x18>
 801851e:	4620      	mov	r0, r4
 8018520:	bd70      	pop	{r4, r5, r6, pc}
 8018522:	bf00      	nop
 8018524:	200050b0 	.word	0x200050b0

08018528 <_ZN17SparkFun_APDS99607getModeEv>:
 8018528:	b507      	push	{r0, r1, r2, lr}
 801852a:	2180      	movs	r1, #128	; 0x80
 801852c:	f10d 0207 	add.w	r2, sp, #7
 8018530:	f7ff ffe0 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018534:	b120      	cbz	r0, 8018540 <_ZN17SparkFun_APDS99607getModeEv+0x18>
 8018536:	f89d 0007 	ldrb.w	r0, [sp, #7]
 801853a:	b003      	add	sp, #12
 801853c:	f85d fb04 	ldr.w	pc, [sp], #4
 8018540:	20ff      	movs	r0, #255	; 0xff
 8018542:	e7fa      	b.n	801853a <_ZN17SparkFun_APDS99607getModeEv+0x12>

08018544 <_ZN17SparkFun_APDS99607setModeEhh>:
 8018544:	b538      	push	{r3, r4, r5, lr}
 8018546:	460d      	mov	r5, r1
 8018548:	4614      	mov	r4, r2
 801854a:	f7ff ffed 	bl	8018528 <_ZN17SparkFun_APDS99607getModeEv>
 801854e:	28ff      	cmp	r0, #255	; 0xff
 8018550:	d01a      	beq.n	8018588 <_ZN17SparkFun_APDS99607setModeEhh+0x44>
 8018552:	2d06      	cmp	r5, #6
 8018554:	f004 0401 	and.w	r4, r4, #1
 8018558:	d80f      	bhi.n	801857a <_ZN17SparkFun_APDS99607setModeEhh+0x36>
 801855a:	2301      	movs	r3, #1
 801855c:	fa03 f105 	lsl.w	r1, r3, r5
 8018560:	b249      	sxtb	r1, r1
 8018562:	b240      	sxtb	r0, r0
 8018564:	b134      	cbz	r4, 8018574 <_ZN17SparkFun_APDS99607setModeEhh+0x30>
 8018566:	4308      	orrs	r0, r1
 8018568:	b2c0      	uxtb	r0, r0
 801856a:	4601      	mov	r1, r0
 801856c:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8018570:	2080      	movs	r0, #128	; 0x80
 8018572:	e5db      	b.n	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018574:	ea20 0001 	bic.w	r0, r0, r1
 8018578:	e7f6      	b.n	8018568 <_ZN17SparkFun_APDS99607setModeEhh+0x24>
 801857a:	2d07      	cmp	r5, #7
 801857c:	d1f5      	bne.n	801856a <_ZN17SparkFun_APDS99607setModeEhh+0x26>
 801857e:	2c00      	cmp	r4, #0
 8018580:	bf0c      	ite	eq
 8018582:	2000      	moveq	r0, #0
 8018584:	207f      	movne	r0, #127	; 0x7f
 8018586:	e7f0      	b.n	801856a <_ZN17SparkFun_APDS99607setModeEhh+0x26>
 8018588:	2000      	movs	r0, #0
 801858a:	bd38      	pop	{r3, r4, r5, pc}

0801858c <_ZN17SparkFun_APDS996011enablePowerEv>:
 801858c:	2201      	movs	r2, #1
 801858e:	2100      	movs	r1, #0
 8018590:	f7ff bfd8 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>

08018594 <_ZN17SparkFun_APDS996012disablePowerEv>:
 8018594:	2200      	movs	r2, #0
 8018596:	4611      	mov	r1, r2
 8018598:	f7ff bfd4 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>

0801859c <_ZN17SparkFun_APDS996018isGestureAvailableEv>:
 801859c:	b507      	push	{r0, r1, r2, lr}
 801859e:	21af      	movs	r1, #175	; 0xaf
 80185a0:	f10d 0207 	add.w	r2, sp, #7
 80185a4:	f7ff ffa6 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80185a8:	b130      	cbz	r0, 80185b8 <_ZN17SparkFun_APDS996018isGestureAvailableEv+0x1c>
 80185aa:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80185ae:	f000 0001 	and.w	r0, r0, #1
 80185b2:	b003      	add	sp, #12
 80185b4:	f85d fb04 	ldr.w	pc, [sp], #4
 80185b8:	2001      	movs	r0, #1
 80185ba:	e7fa      	b.n	80185b2 <_ZN17SparkFun_APDS996018isGestureAvailableEv+0x16>

080185bc <_ZN17SparkFun_APDS996016readAmbientLightERt>:
 80185bc:	2300      	movs	r3, #0
 80185be:	b537      	push	{r0, r1, r2, r4, r5, lr}
 80185c0:	460c      	mov	r4, r1
 80185c2:	800b      	strh	r3, [r1, #0]
 80185c4:	f10d 0207 	add.w	r2, sp, #7
 80185c8:	2194      	movs	r1, #148	; 0x94
 80185ca:	4605      	mov	r5, r0
 80185cc:	f7ff ff92 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80185d0:	b910      	cbnz	r0, 80185d8 <_ZN17SparkFun_APDS996016readAmbientLightERt+0x1c>
 80185d2:	2000      	movs	r0, #0
 80185d4:	b003      	add	sp, #12
 80185d6:	bd30      	pop	{r4, r5, pc}
 80185d8:	f89d 3007 	ldrb.w	r3, [sp, #7]
 80185dc:	f10d 0207 	add.w	r2, sp, #7
 80185e0:	8023      	strh	r3, [r4, #0]
 80185e2:	2195      	movs	r1, #149	; 0x95
 80185e4:	4628      	mov	r0, r5
 80185e6:	f7ff ff85 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80185ea:	2800      	cmp	r0, #0
 80185ec:	d0f1      	beq.n	80185d2 <_ZN17SparkFun_APDS996016readAmbientLightERt+0x16>
 80185ee:	8823      	ldrh	r3, [r4, #0]
 80185f0:	f89d 2007 	ldrb.w	r2, [sp, #7]
 80185f4:	eb03 2302 	add.w	r3, r3, r2, lsl #8
 80185f8:	8023      	strh	r3, [r4, #0]
 80185fa:	e7eb      	b.n	80185d4 <_ZN17SparkFun_APDS996016readAmbientLightERt+0x18>

080185fc <_ZN17SparkFun_APDS996012readRedLightERt>:
 80185fc:	2300      	movs	r3, #0
 80185fe:	b537      	push	{r0, r1, r2, r4, r5, lr}
 8018600:	460c      	mov	r4, r1
 8018602:	800b      	strh	r3, [r1, #0]
 8018604:	f10d 0207 	add.w	r2, sp, #7
 8018608:	2196      	movs	r1, #150	; 0x96
 801860a:	4605      	mov	r5, r0
 801860c:	f7ff ff72 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018610:	b910      	cbnz	r0, 8018618 <_ZN17SparkFun_APDS996012readRedLightERt+0x1c>
 8018612:	2000      	movs	r0, #0
 8018614:	b003      	add	sp, #12
 8018616:	bd30      	pop	{r4, r5, pc}
 8018618:	f89d 3007 	ldrb.w	r3, [sp, #7]
 801861c:	f10d 0207 	add.w	r2, sp, #7
 8018620:	8023      	strh	r3, [r4, #0]
 8018622:	2197      	movs	r1, #151	; 0x97
 8018624:	4628      	mov	r0, r5
 8018626:	f7ff ff65 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 801862a:	2800      	cmp	r0, #0
 801862c:	d0f1      	beq.n	8018612 <_ZN17SparkFun_APDS996012readRedLightERt+0x16>
 801862e:	8823      	ldrh	r3, [r4, #0]
 8018630:	f89d 2007 	ldrb.w	r2, [sp, #7]
 8018634:	eb03 2302 	add.w	r3, r3, r2, lsl #8
 8018638:	8023      	strh	r3, [r4, #0]
 801863a:	e7eb      	b.n	8018614 <_ZN17SparkFun_APDS996012readRedLightERt+0x18>

0801863c <_ZN17SparkFun_APDS996014readGreenLightERt>:
 801863c:	2300      	movs	r3, #0
 801863e:	b537      	push	{r0, r1, r2, r4, r5, lr}
 8018640:	460c      	mov	r4, r1
 8018642:	800b      	strh	r3, [r1, #0]
 8018644:	f10d 0207 	add.w	r2, sp, #7
 8018648:	2198      	movs	r1, #152	; 0x98
 801864a:	4605      	mov	r5, r0
 801864c:	f7ff ff52 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018650:	b910      	cbnz	r0, 8018658 <_ZN17SparkFun_APDS996014readGreenLightERt+0x1c>
 8018652:	2000      	movs	r0, #0
 8018654:	b003      	add	sp, #12
 8018656:	bd30      	pop	{r4, r5, pc}
 8018658:	f89d 3007 	ldrb.w	r3, [sp, #7]
 801865c:	f10d 0207 	add.w	r2, sp, #7
 8018660:	8023      	strh	r3, [r4, #0]
 8018662:	2199      	movs	r1, #153	; 0x99
 8018664:	4628      	mov	r0, r5
 8018666:	f7ff ff45 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 801866a:	2800      	cmp	r0, #0
 801866c:	d0f1      	beq.n	8018652 <_ZN17SparkFun_APDS996014readGreenLightERt+0x16>
 801866e:	8823      	ldrh	r3, [r4, #0]
 8018670:	f89d 2007 	ldrb.w	r2, [sp, #7]
 8018674:	eb03 2302 	add.w	r3, r3, r2, lsl #8
 8018678:	8023      	strh	r3, [r4, #0]
 801867a:	e7eb      	b.n	8018654 <_ZN17SparkFun_APDS996014readGreenLightERt+0x18>

0801867c <_ZN17SparkFun_APDS996013readBlueLightERt>:
 801867c:	2300      	movs	r3, #0
 801867e:	b537      	push	{r0, r1, r2, r4, r5, lr}
 8018680:	460c      	mov	r4, r1
 8018682:	800b      	strh	r3, [r1, #0]
 8018684:	f10d 0207 	add.w	r2, sp, #7
 8018688:	219a      	movs	r1, #154	; 0x9a
 801868a:	4605      	mov	r5, r0
 801868c:	f7ff ff32 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018690:	b910      	cbnz	r0, 8018698 <_ZN17SparkFun_APDS996013readBlueLightERt+0x1c>
 8018692:	2000      	movs	r0, #0
 8018694:	b003      	add	sp, #12
 8018696:	bd30      	pop	{r4, r5, pc}
 8018698:	f89d 3007 	ldrb.w	r3, [sp, #7]
 801869c:	f10d 0207 	add.w	r2, sp, #7
 80186a0:	8023      	strh	r3, [r4, #0]
 80186a2:	219b      	movs	r1, #155	; 0x9b
 80186a4:	4628      	mov	r0, r5
 80186a6:	f7ff ff25 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80186aa:	2800      	cmp	r0, #0
 80186ac:	d0f1      	beq.n	8018692 <_ZN17SparkFun_APDS996013readBlueLightERt+0x16>
 80186ae:	8823      	ldrh	r3, [r4, #0]
 80186b0:	f89d 2007 	ldrb.w	r2, [sp, #7]
 80186b4:	eb03 2302 	add.w	r3, r3, r2, lsl #8
 80186b8:	8023      	strh	r3, [r4, #0]
 80186ba:	e7eb      	b.n	8018694 <_ZN17SparkFun_APDS996013readBlueLightERt+0x18>

080186bc <_ZN17SparkFun_APDS996013readProximityERh>:
 80186bc:	2300      	movs	r3, #0
 80186be:	460a      	mov	r2, r1
 80186c0:	700b      	strb	r3, [r1, #0]
 80186c2:	219c      	movs	r1, #156	; 0x9c
 80186c4:	f7ff bf16 	b.w	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>

080186c8 <_ZN17SparkFun_APDS996019getProxIntLowThreshEv>:
 80186c8:	b507      	push	{r0, r1, r2, lr}
 80186ca:	2189      	movs	r1, #137	; 0x89
 80186cc:	f10d 0207 	add.w	r2, sp, #7
 80186d0:	f7ff ff10 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80186d4:	b908      	cbnz	r0, 80186da <_ZN17SparkFun_APDS996019getProxIntLowThreshEv+0x12>
 80186d6:	f88d 0007 	strb.w	r0, [sp, #7]
 80186da:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80186de:	b003      	add	sp, #12
 80186e0:	f85d fb04 	ldr.w	pc, [sp], #4

080186e4 <_ZN17SparkFun_APDS996020getProxIntHighThreshEv>:
 80186e4:	b507      	push	{r0, r1, r2, lr}
 80186e6:	218b      	movs	r1, #139	; 0x8b
 80186e8:	f10d 0207 	add.w	r2, sp, #7
 80186ec:	f7ff ff02 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80186f0:	b908      	cbnz	r0, 80186f6 <_ZN17SparkFun_APDS996020getProxIntHighThreshEv+0x12>
 80186f2:	f88d 0007 	strb.w	r0, [sp, #7]
 80186f6:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80186fa:	b003      	add	sp, #12
 80186fc:	f85d fb04 	ldr.w	pc, [sp], #4

08018700 <_ZN17SparkFun_APDS996011getLEDDriveEv>:
 8018700:	b507      	push	{r0, r1, r2, lr}
 8018702:	218f      	movs	r1, #143	; 0x8f
 8018704:	f10d 0207 	add.w	r2, sp, #7
 8018708:	f7ff fef4 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 801870c:	b128      	cbz	r0, 801871a <_ZN17SparkFun_APDS996011getLEDDriveEv+0x1a>
 801870e:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018712:	0980      	lsrs	r0, r0, #6
 8018714:	b003      	add	sp, #12
 8018716:	f85d fb04 	ldr.w	pc, [sp], #4
 801871a:	20ff      	movs	r0, #255	; 0xff
 801871c:	e7fa      	b.n	8018714 <_ZN17SparkFun_APDS996011getLEDDriveEv+0x14>

0801871e <_ZN17SparkFun_APDS996011setLEDDriveEh>:
 801871e:	b513      	push	{r0, r1, r4, lr}
 8018720:	460c      	mov	r4, r1
 8018722:	f10d 0207 	add.w	r2, sp, #7
 8018726:	218f      	movs	r1, #143	; 0x8f
 8018728:	f7ff fee4 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 801872c:	b158      	cbz	r0, 8018746 <_ZN17SparkFun_APDS996011setLEDDriveEh+0x28>
 801872e:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018732:	208f      	movs	r0, #143	; 0x8f
 8018734:	f001 013f 	and.w	r1, r1, #63	; 0x3f
 8018738:	ea41 1184 	orr.w	r1, r1, r4, lsl #6
 801873c:	b2c9      	uxtb	r1, r1
 801873e:	f88d 1007 	strb.w	r1, [sp, #7]
 8018742:	f7ff fcf3 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018746:	b002      	add	sp, #8
 8018748:	bd10      	pop	{r4, pc}

0801874a <_ZN17SparkFun_APDS996016getProximityGainEv>:
 801874a:	b507      	push	{r0, r1, r2, lr}
 801874c:	218f      	movs	r1, #143	; 0x8f
 801874e:	f10d 0207 	add.w	r2, sp, #7
 8018752:	f7ff fecf 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018756:	b130      	cbz	r0, 8018766 <_ZN17SparkFun_APDS996016getProximityGainEv+0x1c>
 8018758:	f89d 0007 	ldrb.w	r0, [sp, #7]
 801875c:	f3c0 0081 	ubfx	r0, r0, #2, #2
 8018760:	b003      	add	sp, #12
 8018762:	f85d fb04 	ldr.w	pc, [sp], #4
 8018766:	20ff      	movs	r0, #255	; 0xff
 8018768:	e7fa      	b.n	8018760 <_ZN17SparkFun_APDS996016getProximityGainEv+0x16>

0801876a <_ZN17SparkFun_APDS996016setProximityGainEh>:
 801876a:	b513      	push	{r0, r1, r4, lr}
 801876c:	460c      	mov	r4, r1
 801876e:	f10d 0207 	add.w	r2, sp, #7
 8018772:	218f      	movs	r1, #143	; 0x8f
 8018774:	f7ff febe 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018778:	b160      	cbz	r0, 8018794 <_ZN17SparkFun_APDS996016setProximityGainEh+0x2a>
 801877a:	f89d 1007 	ldrb.w	r1, [sp, #7]
 801877e:	00a4      	lsls	r4, r4, #2
 8018780:	f021 010c 	bic.w	r1, r1, #12
 8018784:	f004 040c 	and.w	r4, r4, #12
 8018788:	4321      	orrs	r1, r4
 801878a:	208f      	movs	r0, #143	; 0x8f
 801878c:	f88d 1007 	strb.w	r1, [sp, #7]
 8018790:	f7ff fccc 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018794:	b002      	add	sp, #8
 8018796:	bd10      	pop	{r4, pc}

08018798 <_ZN17SparkFun_APDS996019getAmbientLightGainEv>:
 8018798:	b507      	push	{r0, r1, r2, lr}
 801879a:	218f      	movs	r1, #143	; 0x8f
 801879c:	f10d 0207 	add.w	r2, sp, #7
 80187a0:	f7ff fea8 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80187a4:	b130      	cbz	r0, 80187b4 <_ZN17SparkFun_APDS996019getAmbientLightGainEv+0x1c>
 80187a6:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80187aa:	f000 0003 	and.w	r0, r0, #3
 80187ae:	b003      	add	sp, #12
 80187b0:	f85d fb04 	ldr.w	pc, [sp], #4
 80187b4:	20ff      	movs	r0, #255	; 0xff
 80187b6:	e7fa      	b.n	80187ae <_ZN17SparkFun_APDS996019getAmbientLightGainEv+0x16>

080187b8 <_ZN17SparkFun_APDS996019setAmbientLightGainEh>:
 80187b8:	b513      	push	{r0, r1, r4, lr}
 80187ba:	460c      	mov	r4, r1
 80187bc:	f10d 0207 	add.w	r2, sp, #7
 80187c0:	218f      	movs	r1, #143	; 0x8f
 80187c2:	f7ff fe97 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80187c6:	b158      	cbz	r0, 80187e0 <_ZN17SparkFun_APDS996019setAmbientLightGainEh+0x28>
 80187c8:	f89d 1007 	ldrb.w	r1, [sp, #7]
 80187cc:	f004 0403 	and.w	r4, r4, #3
 80187d0:	f021 0103 	bic.w	r1, r1, #3
 80187d4:	4321      	orrs	r1, r4
 80187d6:	208f      	movs	r0, #143	; 0x8f
 80187d8:	f88d 1007 	strb.w	r1, [sp, #7]
 80187dc:	f7ff fca6 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 80187e0:	b002      	add	sp, #8
 80187e2:	bd10      	pop	{r4, pc}

080187e4 <_ZN17SparkFun_APDS996011getLEDBoostEv>:
 80187e4:	b507      	push	{r0, r1, r2, lr}
 80187e6:	2190      	movs	r1, #144	; 0x90
 80187e8:	f10d 0207 	add.w	r2, sp, #7
 80187ec:	f7ff fe82 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80187f0:	b130      	cbz	r0, 8018800 <_ZN17SparkFun_APDS996011getLEDBoostEv+0x1c>
 80187f2:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80187f6:	f3c0 1001 	ubfx	r0, r0, #4, #2
 80187fa:	b003      	add	sp, #12
 80187fc:	f85d fb04 	ldr.w	pc, [sp], #4
 8018800:	20ff      	movs	r0, #255	; 0xff
 8018802:	e7fa      	b.n	80187fa <_ZN17SparkFun_APDS996011getLEDBoostEv+0x16>

08018804 <_ZN17SparkFun_APDS996011setLEDBoostEh>:
 8018804:	b513      	push	{r0, r1, r4, lr}
 8018806:	460c      	mov	r4, r1
 8018808:	f10d 0207 	add.w	r2, sp, #7
 801880c:	2190      	movs	r1, #144	; 0x90
 801880e:	f7ff fe71 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018812:	b160      	cbz	r0, 801882e <_ZN17SparkFun_APDS996011setLEDBoostEh+0x2a>
 8018814:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018818:	0124      	lsls	r4, r4, #4
 801881a:	f021 0130 	bic.w	r1, r1, #48	; 0x30
 801881e:	f004 0430 	and.w	r4, r4, #48	; 0x30
 8018822:	4321      	orrs	r1, r4
 8018824:	2090      	movs	r0, #144	; 0x90
 8018826:	f88d 1007 	strb.w	r1, [sp, #7]
 801882a:	f7ff fc7f 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 801882e:	b002      	add	sp, #8
 8018830:	bd10      	pop	{r4, pc}

08018832 <_ZN17SparkFun_APDS996021getProxGainCompEnableEv>:
 8018832:	b507      	push	{r0, r1, r2, lr}
 8018834:	219f      	movs	r1, #159	; 0x9f
 8018836:	f10d 0207 	add.w	r2, sp, #7
 801883a:	f7ff fe5b 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 801883e:	b130      	cbz	r0, 801884e <_ZN17SparkFun_APDS996021getProxGainCompEnableEv+0x1c>
 8018840:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018844:	f3c0 1040 	ubfx	r0, r0, #5, #1
 8018848:	b003      	add	sp, #12
 801884a:	f85d fb04 	ldr.w	pc, [sp], #4
 801884e:	20ff      	movs	r0, #255	; 0xff
 8018850:	e7fa      	b.n	8018848 <_ZN17SparkFun_APDS996021getProxGainCompEnableEv+0x16>

08018852 <_ZN17SparkFun_APDS996021setProxGainCompEnableEh>:
 8018852:	b513      	push	{r0, r1, r4, lr}
 8018854:	460c      	mov	r4, r1
 8018856:	f10d 0207 	add.w	r2, sp, #7
 801885a:	219f      	movs	r1, #159	; 0x9f
 801885c:	f7ff fe4a 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018860:	b160      	cbz	r0, 801887c <_ZN17SparkFun_APDS996021setProxGainCompEnableEh+0x2a>
 8018862:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018866:	0164      	lsls	r4, r4, #5
 8018868:	f021 0120 	bic.w	r1, r1, #32
 801886c:	f004 0420 	and.w	r4, r4, #32
 8018870:	4321      	orrs	r1, r4
 8018872:	209f      	movs	r0, #159	; 0x9f
 8018874:	f88d 1007 	strb.w	r1, [sp, #7]
 8018878:	f7ff fc58 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 801887c:	b002      	add	sp, #8
 801887e:	bd10      	pop	{r4, pc}

08018880 <_ZN17SparkFun_APDS996016getProxPhotoMaskEv>:
 8018880:	b507      	push	{r0, r1, r2, lr}
 8018882:	219f      	movs	r1, #159	; 0x9f
 8018884:	f10d 0207 	add.w	r2, sp, #7
 8018888:	f7ff fe34 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 801888c:	b130      	cbz	r0, 801889c <_ZN17SparkFun_APDS996016getProxPhotoMaskEv+0x1c>
 801888e:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018892:	f000 000f 	and.w	r0, r0, #15
 8018896:	b003      	add	sp, #12
 8018898:	f85d fb04 	ldr.w	pc, [sp], #4
 801889c:	20ff      	movs	r0, #255	; 0xff
 801889e:	e7fa      	b.n	8018896 <_ZN17SparkFun_APDS996016getProxPhotoMaskEv+0x16>

080188a0 <_ZN17SparkFun_APDS996016setProxPhotoMaskEh>:
 80188a0:	b513      	push	{r0, r1, r4, lr}
 80188a2:	460c      	mov	r4, r1
 80188a4:	f10d 0207 	add.w	r2, sp, #7
 80188a8:	219f      	movs	r1, #159	; 0x9f
 80188aa:	f7ff fe23 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80188ae:	b158      	cbz	r0, 80188c8 <_ZN17SparkFun_APDS996016setProxPhotoMaskEh+0x28>
 80188b0:	f89d 1007 	ldrb.w	r1, [sp, #7]
 80188b4:	f004 040f 	and.w	r4, r4, #15
 80188b8:	f021 010f 	bic.w	r1, r1, #15
 80188bc:	4321      	orrs	r1, r4
 80188be:	209f      	movs	r0, #159	; 0x9f
 80188c0:	f88d 1007 	strb.w	r1, [sp, #7]
 80188c4:	f7ff fc32 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 80188c8:	b002      	add	sp, #8
 80188ca:	bd10      	pop	{r4, pc}

080188cc <_ZN17SparkFun_APDS996021getGestureEnterThreshEv>:
 80188cc:	b507      	push	{r0, r1, r2, lr}
 80188ce:	21a0      	movs	r1, #160	; 0xa0
 80188d0:	f10d 0207 	add.w	r2, sp, #7
 80188d4:	f7ff fe0e 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80188d8:	b908      	cbnz	r0, 80188de <_ZN17SparkFun_APDS996021getGestureEnterThreshEv+0x12>
 80188da:	f88d 0007 	strb.w	r0, [sp, #7]
 80188de:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80188e2:	b003      	add	sp, #12
 80188e4:	f85d fb04 	ldr.w	pc, [sp], #4

080188e8 <_ZN17SparkFun_APDS996020getGestureExitThreshEv>:
 80188e8:	b507      	push	{r0, r1, r2, lr}
 80188ea:	21a1      	movs	r1, #161	; 0xa1
 80188ec:	f10d 0207 	add.w	r2, sp, #7
 80188f0:	f7ff fe00 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80188f4:	b908      	cbnz	r0, 80188fa <_ZN17SparkFun_APDS996020getGestureExitThreshEv+0x12>
 80188f6:	f88d 0007 	strb.w	r0, [sp, #7]
 80188fa:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80188fe:	b003      	add	sp, #12
 8018900:	f85d fb04 	ldr.w	pc, [sp], #4

08018904 <_ZN17SparkFun_APDS996014getGestureGainEv>:
 8018904:	b507      	push	{r0, r1, r2, lr}
 8018906:	21a3      	movs	r1, #163	; 0xa3
 8018908:	f10d 0207 	add.w	r2, sp, #7
 801890c:	f7ff fdf2 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018910:	b130      	cbz	r0, 8018920 <_ZN17SparkFun_APDS996014getGestureGainEv+0x1c>
 8018912:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018916:	f3c0 1041 	ubfx	r0, r0, #5, #2
 801891a:	b003      	add	sp, #12
 801891c:	f85d fb04 	ldr.w	pc, [sp], #4
 8018920:	20ff      	movs	r0, #255	; 0xff
 8018922:	e7fa      	b.n	801891a <_ZN17SparkFun_APDS996014getGestureGainEv+0x16>

08018924 <_ZN17SparkFun_APDS996014setGestureGainEh>:
 8018924:	b513      	push	{r0, r1, r4, lr}
 8018926:	460c      	mov	r4, r1
 8018928:	f10d 0207 	add.w	r2, sp, #7
 801892c:	21a3      	movs	r1, #163	; 0xa3
 801892e:	f7ff fde1 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018932:	b160      	cbz	r0, 801894e <_ZN17SparkFun_APDS996014setGestureGainEh+0x2a>
 8018934:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018938:	0164      	lsls	r4, r4, #5
 801893a:	f021 0160 	bic.w	r1, r1, #96	; 0x60
 801893e:	f004 0460 	and.w	r4, r4, #96	; 0x60
 8018942:	4321      	orrs	r1, r4
 8018944:	20a3      	movs	r0, #163	; 0xa3
 8018946:	f88d 1007 	strb.w	r1, [sp, #7]
 801894a:	f7ff fbef 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 801894e:	b002      	add	sp, #8
 8018950:	bd10      	pop	{r4, pc}

08018952 <_ZN17SparkFun_APDS996018getGestureLEDDriveEv>:
 8018952:	b507      	push	{r0, r1, r2, lr}
 8018954:	21a3      	movs	r1, #163	; 0xa3
 8018956:	f10d 0207 	add.w	r2, sp, #7
 801895a:	f7ff fdcb 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 801895e:	b130      	cbz	r0, 801896e <_ZN17SparkFun_APDS996018getGestureLEDDriveEv+0x1c>
 8018960:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018964:	f3c0 00c1 	ubfx	r0, r0, #3, #2
 8018968:	b003      	add	sp, #12
 801896a:	f85d fb04 	ldr.w	pc, [sp], #4
 801896e:	20ff      	movs	r0, #255	; 0xff
 8018970:	e7fa      	b.n	8018968 <_ZN17SparkFun_APDS996018getGestureLEDDriveEv+0x16>

08018972 <_ZN17SparkFun_APDS996018setGestureLEDDriveEh>:
 8018972:	b513      	push	{r0, r1, r4, lr}
 8018974:	460c      	mov	r4, r1
 8018976:	f10d 0207 	add.w	r2, sp, #7
 801897a:	21a3      	movs	r1, #163	; 0xa3
 801897c:	f7ff fdba 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018980:	b160      	cbz	r0, 801899c <_ZN17SparkFun_APDS996018setGestureLEDDriveEh+0x2a>
 8018982:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018986:	00e4      	lsls	r4, r4, #3
 8018988:	f021 0118 	bic.w	r1, r1, #24
 801898c:	f004 0418 	and.w	r4, r4, #24
 8018990:	4321      	orrs	r1, r4
 8018992:	20a3      	movs	r0, #163	; 0xa3
 8018994:	f88d 1007 	strb.w	r1, [sp, #7]
 8018998:	f7ff fbc8 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 801899c:	b002      	add	sp, #8
 801899e:	bd10      	pop	{r4, pc}

080189a0 <_ZN17SparkFun_APDS996018getGestureWaitTimeEv>:
 80189a0:	b507      	push	{r0, r1, r2, lr}
 80189a2:	21a3      	movs	r1, #163	; 0xa3
 80189a4:	f10d 0207 	add.w	r2, sp, #7
 80189a8:	f7ff fda4 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80189ac:	b130      	cbz	r0, 80189bc <_ZN17SparkFun_APDS996018getGestureWaitTimeEv+0x1c>
 80189ae:	f89d 0007 	ldrb.w	r0, [sp, #7]
 80189b2:	f000 0007 	and.w	r0, r0, #7
 80189b6:	b003      	add	sp, #12
 80189b8:	f85d fb04 	ldr.w	pc, [sp], #4
 80189bc:	20ff      	movs	r0, #255	; 0xff
 80189be:	e7fa      	b.n	80189b6 <_ZN17SparkFun_APDS996018getGestureWaitTimeEv+0x16>

080189c0 <_ZN17SparkFun_APDS996018setGestureWaitTimeEh>:
 80189c0:	b513      	push	{r0, r1, r4, lr}
 80189c2:	460c      	mov	r4, r1
 80189c4:	f10d 0207 	add.w	r2, sp, #7
 80189c8:	21a3      	movs	r1, #163	; 0xa3
 80189ca:	f7ff fd93 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 80189ce:	b158      	cbz	r0, 80189e8 <_ZN17SparkFun_APDS996018setGestureWaitTimeEh+0x28>
 80189d0:	f89d 1007 	ldrb.w	r1, [sp, #7]
 80189d4:	f004 0407 	and.w	r4, r4, #7
 80189d8:	f021 0107 	bic.w	r1, r1, #7
 80189dc:	4321      	orrs	r1, r4
 80189de:	20a3      	movs	r0, #163	; 0xa3
 80189e0:	f88d 1007 	strb.w	r1, [sp, #7]
 80189e4:	f7ff fba2 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 80189e8:	b002      	add	sp, #8
 80189ea:	bd10      	pop	{r4, pc}

080189ec <_ZN17SparkFun_APDS996023getLightIntLowThresholdERt>:
 80189ec:	2300      	movs	r3, #0
 80189ee:	b537      	push	{r0, r1, r2, r4, r5, lr}
 80189f0:	460c      	mov	r4, r1
 80189f2:	800b      	strh	r3, [r1, #0]
 80189f4:	f10d 0207 	add.w	r2, sp, #7
 80189f8:	2184      	movs	r1, #132	; 0x84
 80189fa:	4605      	mov	r5, r0
 80189fc:	f7ff fd7a 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018a00:	b910      	cbnz	r0, 8018a08 <_ZN17SparkFun_APDS996023getLightIntLowThresholdERt+0x1c>
 8018a02:	2000      	movs	r0, #0
 8018a04:	b003      	add	sp, #12
 8018a06:	bd30      	pop	{r4, r5, pc}
 8018a08:	f89d 3007 	ldrb.w	r3, [sp, #7]
 8018a0c:	f10d 0207 	add.w	r2, sp, #7
 8018a10:	8023      	strh	r3, [r4, #0]
 8018a12:	2185      	movs	r1, #133	; 0x85
 8018a14:	4628      	mov	r0, r5
 8018a16:	f7ff fd6d 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018a1a:	2800      	cmp	r0, #0
 8018a1c:	d0f1      	beq.n	8018a02 <_ZN17SparkFun_APDS996023getLightIntLowThresholdERt+0x16>
 8018a1e:	8823      	ldrh	r3, [r4, #0]
 8018a20:	f89d 2007 	ldrb.w	r2, [sp, #7]
 8018a24:	eb03 2302 	add.w	r3, r3, r2, lsl #8
 8018a28:	8023      	strh	r3, [r4, #0]
 8018a2a:	e7eb      	b.n	8018a04 <_ZN17SparkFun_APDS996023getLightIntLowThresholdERt+0x18>

08018a2c <_ZN17SparkFun_APDS996024getLightIntHighThresholdERt>:
 8018a2c:	2300      	movs	r3, #0
 8018a2e:	b537      	push	{r0, r1, r2, r4, r5, lr}
 8018a30:	460c      	mov	r4, r1
 8018a32:	800b      	strh	r3, [r1, #0]
 8018a34:	f10d 0207 	add.w	r2, sp, #7
 8018a38:	2186      	movs	r1, #134	; 0x86
 8018a3a:	4605      	mov	r5, r0
 8018a3c:	f7ff fd5a 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018a40:	b910      	cbnz	r0, 8018a48 <_ZN17SparkFun_APDS996024getLightIntHighThresholdERt+0x1c>
 8018a42:	2000      	movs	r0, #0
 8018a44:	b003      	add	sp, #12
 8018a46:	bd30      	pop	{r4, r5, pc}
 8018a48:	f89d 3007 	ldrb.w	r3, [sp, #7]
 8018a4c:	f10d 0207 	add.w	r2, sp, #7
 8018a50:	8023      	strh	r3, [r4, #0]
 8018a52:	2187      	movs	r1, #135	; 0x87
 8018a54:	4628      	mov	r0, r5
 8018a56:	f7ff fd4d 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018a5a:	2800      	cmp	r0, #0
 8018a5c:	d0f1      	beq.n	8018a42 <_ZN17SparkFun_APDS996024getLightIntHighThresholdERt+0x16>
 8018a5e:	8823      	ldrh	r3, [r4, #0]
 8018a60:	f89d 2007 	ldrb.w	r2, [sp, #7]
 8018a64:	eb03 2302 	add.w	r3, r3, r2, lsl #8
 8018a68:	8023      	strh	r3, [r4, #0]
 8018a6a:	e7eb      	b.n	8018a44 <_ZN17SparkFun_APDS996024getLightIntHighThresholdERt+0x18>

08018a6c <_ZN17SparkFun_APDS996027getProximityIntLowThresholdERh>:
 8018a6c:	2300      	movs	r3, #0
 8018a6e:	460a      	mov	r2, r1
 8018a70:	700b      	strb	r3, [r1, #0]
 8018a72:	2189      	movs	r1, #137	; 0x89
 8018a74:	f7ff bd3e 	b.w	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>

08018a78 <_ZN17SparkFun_APDS996028getProximityIntHighThresholdERh>:
 8018a78:	2300      	movs	r3, #0
 8018a7a:	460a      	mov	r2, r1
 8018a7c:	700b      	strb	r3, [r1, #0]
 8018a7e:	218b      	movs	r1, #139	; 0x8b
 8018a80:	f7ff bd38 	b.w	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>

08018a84 <_ZN17SparkFun_APDS996024getAmbientLightIntEnableEv>:
 8018a84:	b507      	push	{r0, r1, r2, lr}
 8018a86:	2180      	movs	r1, #128	; 0x80
 8018a88:	f10d 0207 	add.w	r2, sp, #7
 8018a8c:	f7ff fd32 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018a90:	b130      	cbz	r0, 8018aa0 <_ZN17SparkFun_APDS996024getAmbientLightIntEnableEv+0x1c>
 8018a92:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018a96:	f3c0 1000 	ubfx	r0, r0, #4, #1
 8018a9a:	b003      	add	sp, #12
 8018a9c:	f85d fb04 	ldr.w	pc, [sp], #4
 8018aa0:	20ff      	movs	r0, #255	; 0xff
 8018aa2:	e7fa      	b.n	8018a9a <_ZN17SparkFun_APDS996024getAmbientLightIntEnableEv+0x16>

08018aa4 <_ZN17SparkFun_APDS996024setAmbientLightIntEnableEh>:
 8018aa4:	b513      	push	{r0, r1, r4, lr}
 8018aa6:	460c      	mov	r4, r1
 8018aa8:	f10d 0207 	add.w	r2, sp, #7
 8018aac:	2180      	movs	r1, #128	; 0x80
 8018aae:	f7ff fd21 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018ab2:	b160      	cbz	r0, 8018ace <_ZN17SparkFun_APDS996024setAmbientLightIntEnableEh+0x2a>
 8018ab4:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018ab8:	0124      	lsls	r4, r4, #4
 8018aba:	f021 0110 	bic.w	r1, r1, #16
 8018abe:	f004 0410 	and.w	r4, r4, #16
 8018ac2:	4321      	orrs	r1, r4
 8018ac4:	2080      	movs	r0, #128	; 0x80
 8018ac6:	f88d 1007 	strb.w	r1, [sp, #7]
 8018aca:	f7ff fb2f 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018ace:	b002      	add	sp, #8
 8018ad0:	bd10      	pop	{r4, pc}

08018ad2 <_ZN17SparkFun_APDS996017enableLightSensorEb>:
 8018ad2:	b538      	push	{r3, r4, r5, lr}
 8018ad4:	460d      	mov	r5, r1
 8018ad6:	2101      	movs	r1, #1
 8018ad8:	4604      	mov	r4, r0
 8018ada:	f7ff fe6d 	bl	80187b8 <_ZN17SparkFun_APDS996019setAmbientLightGainEh>
 8018ade:	b190      	cbz	r0, 8018b06 <_ZN17SparkFun_APDS996017enableLightSensorEb+0x34>
 8018ae0:	b17d      	cbz	r5, 8018b02 <_ZN17SparkFun_APDS996017enableLightSensorEb+0x30>
 8018ae2:	2101      	movs	r1, #1
 8018ae4:	4620      	mov	r0, r4
 8018ae6:	f7ff ffdd 	bl	8018aa4 <_ZN17SparkFun_APDS996024setAmbientLightIntEnableEh>
 8018aea:	b160      	cbz	r0, 8018b06 <_ZN17SparkFun_APDS996017enableLightSensorEb+0x34>
 8018aec:	4620      	mov	r0, r4
 8018aee:	f7ff fd4d 	bl	801858c <_ZN17SparkFun_APDS996011enablePowerEv>
 8018af2:	b140      	cbz	r0, 8018b06 <_ZN17SparkFun_APDS996017enableLightSensorEb+0x34>
 8018af4:	2201      	movs	r2, #1
 8018af6:	4620      	mov	r0, r4
 8018af8:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8018afc:	4611      	mov	r1, r2
 8018afe:	f7ff bd21 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018b02:	4629      	mov	r1, r5
 8018b04:	e7ee      	b.n	8018ae4 <_ZN17SparkFun_APDS996017enableLightSensorEb+0x12>
 8018b06:	2000      	movs	r0, #0
 8018b08:	bd38      	pop	{r3, r4, r5, pc}

08018b0a <_ZN17SparkFun_APDS996018disableLightSensorEv>:
 8018b0a:	b510      	push	{r4, lr}
 8018b0c:	2100      	movs	r1, #0
 8018b0e:	4604      	mov	r4, r0
 8018b10:	f7ff ffc8 	bl	8018aa4 <_ZN17SparkFun_APDS996024setAmbientLightIntEnableEh>
 8018b14:	b130      	cbz	r0, 8018b24 <_ZN17SparkFun_APDS996018disableLightSensorEv+0x1a>
 8018b16:	4620      	mov	r0, r4
 8018b18:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8018b1c:	2200      	movs	r2, #0
 8018b1e:	2101      	movs	r1, #1
 8018b20:	f7ff bd10 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018b24:	bd10      	pop	{r4, pc}

08018b26 <_ZN17SparkFun_APDS996021getProximityIntEnableEv>:
 8018b26:	b507      	push	{r0, r1, r2, lr}
 8018b28:	2180      	movs	r1, #128	; 0x80
 8018b2a:	f10d 0207 	add.w	r2, sp, #7
 8018b2e:	f7ff fce1 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018b32:	b130      	cbz	r0, 8018b42 <_ZN17SparkFun_APDS996021getProximityIntEnableEv+0x1c>
 8018b34:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018b38:	f3c0 1040 	ubfx	r0, r0, #5, #1
 8018b3c:	b003      	add	sp, #12
 8018b3e:	f85d fb04 	ldr.w	pc, [sp], #4
 8018b42:	20ff      	movs	r0, #255	; 0xff
 8018b44:	e7fa      	b.n	8018b3c <_ZN17SparkFun_APDS996021getProximityIntEnableEv+0x16>

08018b46 <_ZN17SparkFun_APDS996021setProximityIntEnableEh>:
 8018b46:	b513      	push	{r0, r1, r4, lr}
 8018b48:	460c      	mov	r4, r1
 8018b4a:	f10d 0207 	add.w	r2, sp, #7
 8018b4e:	2180      	movs	r1, #128	; 0x80
 8018b50:	f7ff fcd0 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018b54:	b160      	cbz	r0, 8018b70 <_ZN17SparkFun_APDS996021setProximityIntEnableEh+0x2a>
 8018b56:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018b5a:	0164      	lsls	r4, r4, #5
 8018b5c:	f021 0120 	bic.w	r1, r1, #32
 8018b60:	f004 0420 	and.w	r4, r4, #32
 8018b64:	4321      	orrs	r1, r4
 8018b66:	2080      	movs	r0, #128	; 0x80
 8018b68:	f88d 1007 	strb.w	r1, [sp, #7]
 8018b6c:	f7ff fade 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018b70:	b002      	add	sp, #8
 8018b72:	bd10      	pop	{r4, pc}

08018b74 <_ZN17SparkFun_APDS996021enableProximitySensorEb>:
 8018b74:	b538      	push	{r3, r4, r5, lr}
 8018b76:	460d      	mov	r5, r1
 8018b78:	2102      	movs	r1, #2
 8018b7a:	4604      	mov	r4, r0
 8018b7c:	f7ff fdf5 	bl	801876a <_ZN17SparkFun_APDS996016setProximityGainEh>
 8018b80:	b1b8      	cbz	r0, 8018bb2 <_ZN17SparkFun_APDS996021enableProximitySensorEb+0x3e>
 8018b82:	2100      	movs	r1, #0
 8018b84:	4620      	mov	r0, r4
 8018b86:	f7ff fdca 	bl	801871e <_ZN17SparkFun_APDS996011setLEDDriveEh>
 8018b8a:	b190      	cbz	r0, 8018bb2 <_ZN17SparkFun_APDS996021enableProximitySensorEb+0x3e>
 8018b8c:	b17d      	cbz	r5, 8018bae <_ZN17SparkFun_APDS996021enableProximitySensorEb+0x3a>
 8018b8e:	2101      	movs	r1, #1
 8018b90:	4620      	mov	r0, r4
 8018b92:	f7ff ffd8 	bl	8018b46 <_ZN17SparkFun_APDS996021setProximityIntEnableEh>
 8018b96:	b160      	cbz	r0, 8018bb2 <_ZN17SparkFun_APDS996021enableProximitySensorEb+0x3e>
 8018b98:	4620      	mov	r0, r4
 8018b9a:	f7ff fcf7 	bl	801858c <_ZN17SparkFun_APDS996011enablePowerEv>
 8018b9e:	b140      	cbz	r0, 8018bb2 <_ZN17SparkFun_APDS996021enableProximitySensorEb+0x3e>
 8018ba0:	4620      	mov	r0, r4
 8018ba2:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8018ba6:	2201      	movs	r2, #1
 8018ba8:	2102      	movs	r1, #2
 8018baa:	f7ff bccb 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018bae:	4629      	mov	r1, r5
 8018bb0:	e7ee      	b.n	8018b90 <_ZN17SparkFun_APDS996021enableProximitySensorEb+0x1c>
 8018bb2:	2000      	movs	r0, #0
 8018bb4:	bd38      	pop	{r3, r4, r5, pc}

08018bb6 <_ZN17SparkFun_APDS996022disableProximitySensorEv>:
 8018bb6:	b510      	push	{r4, lr}
 8018bb8:	2100      	movs	r1, #0
 8018bba:	4604      	mov	r4, r0
 8018bbc:	f7ff ffc3 	bl	8018b46 <_ZN17SparkFun_APDS996021setProximityIntEnableEh>
 8018bc0:	b130      	cbz	r0, 8018bd0 <_ZN17SparkFun_APDS996022disableProximitySensorEv+0x1a>
 8018bc2:	4620      	mov	r0, r4
 8018bc4:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8018bc8:	2200      	movs	r2, #0
 8018bca:	2102      	movs	r1, #2
 8018bcc:	f7ff bcba 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018bd0:	bd10      	pop	{r4, pc}

08018bd2 <_ZN17SparkFun_APDS996019getGestureIntEnableEv>:
 8018bd2:	b507      	push	{r0, r1, r2, lr}
 8018bd4:	21ab      	movs	r1, #171	; 0xab
 8018bd6:	f10d 0207 	add.w	r2, sp, #7
 8018bda:	f7ff fc8b 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018bde:	b130      	cbz	r0, 8018bee <_ZN17SparkFun_APDS996019getGestureIntEnableEv+0x1c>
 8018be0:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018be4:	f3c0 0040 	ubfx	r0, r0, #1, #1
 8018be8:	b003      	add	sp, #12
 8018bea:	f85d fb04 	ldr.w	pc, [sp], #4
 8018bee:	20ff      	movs	r0, #255	; 0xff
 8018bf0:	e7fa      	b.n	8018be8 <_ZN17SparkFun_APDS996019getGestureIntEnableEv+0x16>

08018bf2 <_ZN17SparkFun_APDS996019setGestureIntEnableEh>:
 8018bf2:	b513      	push	{r0, r1, r4, lr}
 8018bf4:	460c      	mov	r4, r1
 8018bf6:	f10d 0207 	add.w	r2, sp, #7
 8018bfa:	21ab      	movs	r1, #171	; 0xab
 8018bfc:	f7ff fc7a 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018c00:	b160      	cbz	r0, 8018c1c <_ZN17SparkFun_APDS996019setGestureIntEnableEh+0x2a>
 8018c02:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018c06:	0064      	lsls	r4, r4, #1
 8018c08:	f021 0102 	bic.w	r1, r1, #2
 8018c0c:	f004 0402 	and.w	r4, r4, #2
 8018c10:	4321      	orrs	r1, r4
 8018c12:	20ab      	movs	r0, #171	; 0xab
 8018c14:	f88d 1007 	strb.w	r1, [sp, #7]
 8018c18:	f7ff fa88 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018c1c:	b002      	add	sp, #8
 8018c1e:	bd10      	pop	{r4, pc}

08018c20 <_ZN17SparkFun_APDS99604initEv>:
 8018c20:	b513      	push	{r0, r1, r4, lr}
 8018c22:	4604      	mov	r4, r0
 8018c24:	4868      	ldr	r0, [pc, #416]	; (8018dc8 <_ZN17SparkFun_APDS99604initEv+0x1a8>)
 8018c26:	f7ff fa18 	bl	801805a <_ZN7Arduino5CWire5beginEv>
 8018c2a:	f10d 0207 	add.w	r2, sp, #7
 8018c2e:	2192      	movs	r1, #146	; 0x92
 8018c30:	4620      	mov	r0, r4
 8018c32:	f7ff fc5f 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018c36:	b910      	cbnz	r0, 8018c3e <_ZN17SparkFun_APDS99604initEv+0x1e>
 8018c38:	2000      	movs	r0, #0
 8018c3a:	b002      	add	sp, #8
 8018c3c:	bd10      	pop	{r4, pc}
 8018c3e:	f89d 3007 	ldrb.w	r3, [sp, #7]
 8018c42:	2bab      	cmp	r3, #171	; 0xab
 8018c44:	d001      	beq.n	8018c4a <_ZN17SparkFun_APDS99604initEv+0x2a>
 8018c46:	2b9c      	cmp	r3, #156	; 0x9c
 8018c48:	d1f6      	bne.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018c4a:	2200      	movs	r2, #0
 8018c4c:	2107      	movs	r1, #7
 8018c4e:	4620      	mov	r0, r4
 8018c50:	f7ff fc78 	bl	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018c54:	2800      	cmp	r0, #0
 8018c56:	d0ef      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018c58:	21db      	movs	r1, #219	; 0xdb
 8018c5a:	2081      	movs	r0, #129	; 0x81
 8018c5c:	f7ff fa66 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018c60:	2800      	cmp	r0, #0
 8018c62:	d0e9      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018c64:	21f6      	movs	r1, #246	; 0xf6
 8018c66:	2083      	movs	r0, #131	; 0x83
 8018c68:	f7ff fa60 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018c6c:	2800      	cmp	r0, #0
 8018c6e:	d0e3      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018c70:	2187      	movs	r1, #135	; 0x87
 8018c72:	208e      	movs	r0, #142	; 0x8e
 8018c74:	f7ff fa5a 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018c78:	2800      	cmp	r0, #0
 8018c7a:	d0dd      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018c7c:	2100      	movs	r1, #0
 8018c7e:	209d      	movs	r0, #157	; 0x9d
 8018c80:	f7ff fa54 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018c84:	2800      	cmp	r0, #0
 8018c86:	d0d7      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018c88:	2100      	movs	r1, #0
 8018c8a:	209e      	movs	r0, #158	; 0x9e
 8018c8c:	f7ff fa4e 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018c90:	2800      	cmp	r0, #0
 8018c92:	d0d1      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018c94:	2160      	movs	r1, #96	; 0x60
 8018c96:	208d      	movs	r0, #141	; 0x8d
 8018c98:	f7ff fa48 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018c9c:	2800      	cmp	r0, #0
 8018c9e:	d0cb      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018ca0:	2100      	movs	r1, #0
 8018ca2:	4620      	mov	r0, r4
 8018ca4:	f7ff fd3b 	bl	801871e <_ZN17SparkFun_APDS996011setLEDDriveEh>
 8018ca8:	2800      	cmp	r0, #0
 8018caa:	d0c5      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018cac:	2102      	movs	r1, #2
 8018cae:	4620      	mov	r0, r4
 8018cb0:	f7ff fd5b 	bl	801876a <_ZN17SparkFun_APDS996016setProximityGainEh>
 8018cb4:	2800      	cmp	r0, #0
 8018cb6:	d0bf      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018cb8:	2101      	movs	r1, #1
 8018cba:	4620      	mov	r0, r4
 8018cbc:	f7ff fd7c 	bl	80187b8 <_ZN17SparkFun_APDS996019setAmbientLightGainEh>
 8018cc0:	2800      	cmp	r0, #0
 8018cc2:	d0b9      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018cc4:	2100      	movs	r1, #0
 8018cc6:	4620      	mov	r0, r4
 8018cc8:	f7ff fbbd 	bl	8018446 <_ZN17SparkFun_APDS996027setProximityIntLowThresholdEh>
 8018ccc:	2800      	cmp	r0, #0
 8018cce:	d0b3      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018cd0:	2132      	movs	r1, #50	; 0x32
 8018cd2:	4620      	mov	r0, r4
 8018cd4:	f7ff fbbb 	bl	801844e <_ZN17SparkFun_APDS996028setProximityIntHighThresholdEh>
 8018cd8:	2800      	cmp	r0, #0
 8018cda:	d0ad      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018cdc:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8018ce0:	4620      	mov	r0, r4
 8018ce2:	f7ff fb96 	bl	8018412 <_ZN17SparkFun_APDS996023setLightIntLowThresholdEt>
 8018ce6:	2800      	cmp	r0, #0
 8018ce8:	d0a6      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018cea:	2100      	movs	r1, #0
 8018cec:	4620      	mov	r0, r4
 8018cee:	f7ff fb9d 	bl	801842c <_ZN17SparkFun_APDS996024setLightIntHighThresholdEt>
 8018cf2:	2800      	cmp	r0, #0
 8018cf4:	d0a0      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018cf6:	2111      	movs	r1, #17
 8018cf8:	208c      	movs	r0, #140	; 0x8c
 8018cfa:	f7ff fa17 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018cfe:	2800      	cmp	r0, #0
 8018d00:	d09a      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d02:	2101      	movs	r1, #1
 8018d04:	2090      	movs	r0, #144	; 0x90
 8018d06:	f7ff fa11 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018d0a:	2800      	cmp	r0, #0
 8018d0c:	d094      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d0e:	2100      	movs	r1, #0
 8018d10:	209f      	movs	r0, #159	; 0x9f
 8018d12:	f7ff fa0b 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018d16:	2800      	cmp	r0, #0
 8018d18:	d08e      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d1a:	2128      	movs	r1, #40	; 0x28
 8018d1c:	4620      	mov	r0, r4
 8018d1e:	f7ff fb74 	bl	801840a <_ZN17SparkFun_APDS996021setGestureEnterThreshEh>
 8018d22:	2800      	cmp	r0, #0
 8018d24:	d088      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d26:	211e      	movs	r1, #30
 8018d28:	4620      	mov	r0, r4
 8018d2a:	f7ff fb70 	bl	801840e <_ZN17SparkFun_APDS996020setGestureExitThreshEh>
 8018d2e:	2800      	cmp	r0, #0
 8018d30:	d082      	beq.n	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d32:	2140      	movs	r1, #64	; 0x40
 8018d34:	20a2      	movs	r0, #162	; 0xa2
 8018d36:	f7ff f9f9 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018d3a:	2800      	cmp	r0, #0
 8018d3c:	f43f af7c 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d40:	2102      	movs	r1, #2
 8018d42:	4620      	mov	r0, r4
 8018d44:	f7ff fdee 	bl	8018924 <_ZN17SparkFun_APDS996014setGestureGainEh>
 8018d48:	2800      	cmp	r0, #0
 8018d4a:	f43f af75 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d4e:	2100      	movs	r1, #0
 8018d50:	4620      	mov	r0, r4
 8018d52:	f7ff fe0e 	bl	8018972 <_ZN17SparkFun_APDS996018setGestureLEDDriveEh>
 8018d56:	2800      	cmp	r0, #0
 8018d58:	f43f af6e 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d5c:	2101      	movs	r1, #1
 8018d5e:	4620      	mov	r0, r4
 8018d60:	f7ff fe2e 	bl	80189c0 <_ZN17SparkFun_APDS996018setGestureWaitTimeEh>
 8018d64:	2800      	cmp	r0, #0
 8018d66:	f43f af67 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d6a:	2100      	movs	r1, #0
 8018d6c:	20a4      	movs	r0, #164	; 0xa4
 8018d6e:	f7ff f9dd 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018d72:	2800      	cmp	r0, #0
 8018d74:	f43f af60 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d78:	2100      	movs	r1, #0
 8018d7a:	20a5      	movs	r0, #165	; 0xa5
 8018d7c:	f7ff f9d6 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018d80:	2800      	cmp	r0, #0
 8018d82:	f43f af59 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d86:	2100      	movs	r1, #0
 8018d88:	20a7      	movs	r0, #167	; 0xa7
 8018d8a:	f7ff f9cf 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018d8e:	2800      	cmp	r0, #0
 8018d90:	f43f af52 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018d94:	2100      	movs	r1, #0
 8018d96:	20a9      	movs	r0, #169	; 0xa9
 8018d98:	f7ff f9c8 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018d9c:	2800      	cmp	r0, #0
 8018d9e:	f43f af4b 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018da2:	21c9      	movs	r1, #201	; 0xc9
 8018da4:	20a6      	movs	r0, #166	; 0xa6
 8018da6:	f7ff f9c1 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018daa:	2800      	cmp	r0, #0
 8018dac:	f43f af44 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018db0:	2100      	movs	r1, #0
 8018db2:	20aa      	movs	r0, #170	; 0xaa
 8018db4:	f7ff f9ba 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018db8:	2800      	cmp	r0, #0
 8018dba:	f43f af3d 	beq.w	8018c38 <_ZN17SparkFun_APDS99604initEv+0x18>
 8018dbe:	2100      	movs	r1, #0
 8018dc0:	4620      	mov	r0, r4
 8018dc2:	f7ff ff16 	bl	8018bf2 <_ZN17SparkFun_APDS996019setGestureIntEnableEh>
 8018dc6:	e738      	b.n	8018c3a <_ZN17SparkFun_APDS99604initEv+0x1a>
 8018dc8:	200050b0 	.word	0x200050b0

08018dcc <_ZN17SparkFun_APDS996020clearAmbientLightIntEv>:
 8018dcc:	b507      	push	{r0, r1, r2, lr}
 8018dce:	21e7      	movs	r1, #231	; 0xe7
 8018dd0:	f10d 0207 	add.w	r2, sp, #7
 8018dd4:	f7ff fb8e 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018dd8:	b003      	add	sp, #12
 8018dda:	f85d fb04 	ldr.w	pc, [sp], #4

08018dde <_ZN17SparkFun_APDS996017clearProximityIntEv>:
 8018dde:	b507      	push	{r0, r1, r2, lr}
 8018de0:	21e5      	movs	r1, #229	; 0xe5
 8018de2:	f10d 0207 	add.w	r2, sp, #7
 8018de6:	f7ff fb85 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018dea:	b003      	add	sp, #12
 8018dec:	f85d fb04 	ldr.w	pc, [sp], #4

08018df0 <_ZN17SparkFun_APDS996014getGestureModeEv>:
 8018df0:	b507      	push	{r0, r1, r2, lr}
 8018df2:	21ab      	movs	r1, #171	; 0xab
 8018df4:	f10d 0207 	add.w	r2, sp, #7
 8018df8:	f7ff fb7c 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018dfc:	b130      	cbz	r0, 8018e0c <_ZN17SparkFun_APDS996014getGestureModeEv+0x1c>
 8018dfe:	f89d 0007 	ldrb.w	r0, [sp, #7]
 8018e02:	f000 0001 	and.w	r0, r0, #1
 8018e06:	b003      	add	sp, #12
 8018e08:	f85d fb04 	ldr.w	pc, [sp], #4
 8018e0c:	20ff      	movs	r0, #255	; 0xff
 8018e0e:	e7fa      	b.n	8018e06 <_ZN17SparkFun_APDS996014getGestureModeEv+0x16>

08018e10 <_ZN17SparkFun_APDS996014setGestureModeEh>:
 8018e10:	b513      	push	{r0, r1, r4, lr}
 8018e12:	460c      	mov	r4, r1
 8018e14:	f10d 0207 	add.w	r2, sp, #7
 8018e18:	21ab      	movs	r1, #171	; 0xab
 8018e1a:	f7ff fb6b 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018e1e:	b158      	cbz	r0, 8018e38 <_ZN17SparkFun_APDS996014setGestureModeEh+0x28>
 8018e20:	f89d 1007 	ldrb.w	r1, [sp, #7]
 8018e24:	f004 0401 	and.w	r4, r4, #1
 8018e28:	f021 0101 	bic.w	r1, r1, #1
 8018e2c:	4321      	orrs	r1, r4
 8018e2e:	20ab      	movs	r0, #171	; 0xab
 8018e30:	f88d 1007 	strb.w	r1, [sp, #7]
 8018e34:	f7ff f97a 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018e38:	b002      	add	sp, #8
 8018e3a:	bd10      	pop	{r4, pc}

08018e3c <_ZN17SparkFun_APDS996019enableGestureSensorEb>:
 8018e3c:	b538      	push	{r3, r4, r5, lr}
 8018e3e:	4604      	mov	r4, r0
 8018e40:	460d      	mov	r5, r1
 8018e42:	f7ff f998 	bl	8018176 <_ZN17SparkFun_APDS996022resetGestureParametersEv>
 8018e46:	21ff      	movs	r1, #255	; 0xff
 8018e48:	2083      	movs	r0, #131	; 0x83
 8018e4a:	f7ff f96f 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018e4e:	b368      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e50:	2189      	movs	r1, #137	; 0x89
 8018e52:	208e      	movs	r0, #142	; 0x8e
 8018e54:	f7ff f96a 	bl	801812c <_ZN17SparkFun_APDS996017wireWriteDataByteEhh.constprop.3>
 8018e58:	b340      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e5a:	2103      	movs	r1, #3
 8018e5c:	4620      	mov	r0, r4
 8018e5e:	f7ff fcd1 	bl	8018804 <_ZN17SparkFun_APDS996011setLEDBoostEh>
 8018e62:	b318      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e64:	b305      	cbz	r5, 8018ea8 <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x6c>
 8018e66:	2101      	movs	r1, #1
 8018e68:	4620      	mov	r0, r4
 8018e6a:	f7ff fec2 	bl	8018bf2 <_ZN17SparkFun_APDS996019setGestureIntEnableEh>
 8018e6e:	b1e8      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e70:	2101      	movs	r1, #1
 8018e72:	4620      	mov	r0, r4
 8018e74:	f7ff ffcc 	bl	8018e10 <_ZN17SparkFun_APDS996014setGestureModeEh>
 8018e78:	b1c0      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e7a:	4620      	mov	r0, r4
 8018e7c:	f7ff fb86 	bl	801858c <_ZN17SparkFun_APDS996011enablePowerEv>
 8018e80:	b1a0      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e82:	2201      	movs	r2, #1
 8018e84:	2103      	movs	r1, #3
 8018e86:	4620      	mov	r0, r4
 8018e88:	f7ff fb5c 	bl	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018e8c:	b170      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e8e:	2201      	movs	r2, #1
 8018e90:	2102      	movs	r1, #2
 8018e92:	4620      	mov	r0, r4
 8018e94:	f7ff fb56 	bl	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018e98:	b140      	cbz	r0, 8018eac <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x70>
 8018e9a:	4620      	mov	r0, r4
 8018e9c:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
 8018ea0:	2201      	movs	r2, #1
 8018ea2:	2106      	movs	r1, #6
 8018ea4:	f7ff bb4e 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018ea8:	4629      	mov	r1, r5
 8018eaa:	e7dd      	b.n	8018e68 <_ZN17SparkFun_APDS996019enableGestureSensorEb+0x2c>
 8018eac:	2000      	movs	r0, #0
 8018eae:	bd38      	pop	{r3, r4, r5, pc}

08018eb0 <_ZN17SparkFun_APDS996020disableGestureSensorEv>:
 8018eb0:	b510      	push	{r4, lr}
 8018eb2:	4604      	mov	r4, r0
 8018eb4:	f7ff f95f 	bl	8018176 <_ZN17SparkFun_APDS996022resetGestureParametersEv>
 8018eb8:	2100      	movs	r1, #0
 8018eba:	f7ff fe9a 	bl	8018bf2 <_ZN17SparkFun_APDS996019setGestureIntEnableEh>
 8018ebe:	b158      	cbz	r0, 8018ed8 <_ZN17SparkFun_APDS996020disableGestureSensorEv+0x28>
 8018ec0:	2100      	movs	r1, #0
 8018ec2:	4620      	mov	r0, r4
 8018ec4:	f7ff ffa4 	bl	8018e10 <_ZN17SparkFun_APDS996014setGestureModeEh>
 8018ec8:	b130      	cbz	r0, 8018ed8 <_ZN17SparkFun_APDS996020disableGestureSensorEv+0x28>
 8018eca:	4620      	mov	r0, r4
 8018ecc:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8018ed0:	2200      	movs	r2, #0
 8018ed2:	2106      	movs	r1, #6
 8018ed4:	f7ff bb36 	b.w	8018544 <_ZN17SparkFun_APDS99607setModeEhh>
 8018ed8:	2000      	movs	r0, #0
 8018eda:	bd10      	pop	{r4, pc}

08018edc <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj>:
 8018edc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8018ee0:	4690      	mov	r8, r2
 8018ee2:	461f      	mov	r7, r3
 8018ee4:	f7ff fab7 	bl	8018456 <_ZN17SparkFun_APDS996013wireWriteByteEh>
 8018ee8:	b918      	cbnz	r0, 8018ef2 <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj+0x16>
 8018eea:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
 8018eee:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8018ef2:	463a      	mov	r2, r7
 8018ef4:	2139      	movs	r1, #57	; 0x39
 8018ef6:	480b      	ldr	r0, [pc, #44]	; (8018f24 <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj+0x48>)
 8018ef8:	f7ff f8c5 	bl	8018086 <_ZN7Arduino5CWire11requestFromEhi>
 8018efc:	2500      	movs	r5, #0
 8018efe:	4e09      	ldr	r6, [pc, #36]	; (8018f24 <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj+0x48>)
 8018f00:	4630      	mov	r0, r6
 8018f02:	f7ff f8db 	bl	80180bc <_ZN7Arduino5CWire9availableEv>
 8018f06:	b2ec      	uxtb	r4, r5
 8018f08:	b148      	cbz	r0, 8018f1e <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj+0x42>
 8018f0a:	42a7      	cmp	r7, r4
 8018f0c:	f105 0501 	add.w	r5, r5, #1
 8018f10:	d9eb      	bls.n	8018eea <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj+0xe>
 8018f12:	4630      	mov	r0, r6
 8018f14:	f7ff f8c2 	bl	801809c <_ZN7Arduino5CWire4readEv>
 8018f18:	f808 0004 	strb.w	r0, [r8, r4]
 8018f1c:	e7f0      	b.n	8018f00 <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj+0x24>
 8018f1e:	4620      	mov	r0, r4
 8018f20:	e7e5      	b.n	8018eee <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj+0x12>
 8018f22:	bf00      	nop
 8018f24:	200050b0 	.word	0x200050b0

08018f28 <_ZN17SparkFun_APDS996011readGestureEv>:
 8018f28:	2300      	movs	r3, #0
 8018f2a:	b530      	push	{r4, r5, lr}
 8018f2c:	b0a3      	sub	sp, #140	; 0x8c
 8018f2e:	4604      	mov	r4, r0
 8018f30:	f88d 3006 	strb.w	r3, [sp, #6]
 8018f34:	f7ff fb32 	bl	801859c <_ZN17SparkFun_APDS996018isGestureAvailableEv>
 8018f38:	b918      	cbnz	r0, 8018f42 <_ZN17SparkFun_APDS996011readGestureEv+0x1a>
 8018f3a:	2200      	movs	r2, #0
 8018f3c:	4610      	mov	r0, r2
 8018f3e:	b023      	add	sp, #140	; 0x8c
 8018f40:	bd30      	pop	{r4, r5, pc}
 8018f42:	4620      	mov	r0, r4
 8018f44:	f7ff faf0 	bl	8018528 <_ZN17SparkFun_APDS99607getModeEv>
 8018f48:	f010 0f41 	tst.w	r0, #65	; 0x41
 8018f4c:	d0f5      	beq.n	8018f3a <_ZN17SparkFun_APDS996011readGestureEv+0x12>
 8018f4e:	201e      	movs	r0, #30
 8018f50:	f7ff f87c 	bl	801804c <_ZN7Arduino5delayEi>
 8018f54:	f10d 0207 	add.w	r2, sp, #7
 8018f58:	21af      	movs	r1, #175	; 0xaf
 8018f5a:	4620      	mov	r0, r4
 8018f5c:	f7ff faca 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018f60:	b908      	cbnz	r0, 8018f66 <_ZN17SparkFun_APDS996011readGestureEv+0x3e>
 8018f62:	22ff      	movs	r2, #255	; 0xff
 8018f64:	e7ea      	b.n	8018f3c <_ZN17SparkFun_APDS996011readGestureEv+0x14>
 8018f66:	f89d 3007 	ldrb.w	r3, [sp, #7]
 8018f6a:	07db      	lsls	r3, r3, #31
 8018f6c:	d543      	bpl.n	8018ff6 <_ZN17SparkFun_APDS996011readGestureEv+0xce>
 8018f6e:	f10d 0206 	add.w	r2, sp, #6
 8018f72:	21ae      	movs	r1, #174	; 0xae
 8018f74:	4620      	mov	r0, r4
 8018f76:	f7ff fabd 	bl	80184f4 <_ZN17SparkFun_APDS996016wireReadDataByteEhRh>
 8018f7a:	2800      	cmp	r0, #0
 8018f7c:	d0f1      	beq.n	8018f62 <_ZN17SparkFun_APDS996011readGestureEv+0x3a>
 8018f7e:	f89d 3006 	ldrb.w	r3, [sp, #6]
 8018f82:	2b00      	cmp	r3, #0
 8018f84:	d0e3      	beq.n	8018f4e <_ZN17SparkFun_APDS996011readGestureEv+0x26>
 8018f86:	009b      	lsls	r3, r3, #2
 8018f88:	aa02      	add	r2, sp, #8
 8018f8a:	21fc      	movs	r1, #252	; 0xfc
 8018f8c:	4620      	mov	r0, r4
 8018f8e:	f7ff ffa5 	bl	8018edc <_ZN17SparkFun_APDS996017wireReadDataBlockEhPhj>
 8018f92:	b2c0      	uxtb	r0, r0
 8018f94:	2803      	cmp	r0, #3
 8018f96:	d9da      	bls.n	8018f4e <_ZN17SparkFun_APDS996011readGestureEv+0x26>
 8018f98:	3003      	adds	r0, #3
 8018f9a:	f020 0003 	bic.w	r0, r0, #3
 8018f9e:	aa03      	add	r2, sp, #12
 8018fa0:	ab02      	add	r3, sp, #8
 8018fa2:	4410      	add	r0, r2
 8018fa4:	3304      	adds	r3, #4
 8018fa6:	4283      	cmp	r3, r0
 8018fa8:	d01a      	beq.n	8018fe0 <_ZN17SparkFun_APDS996011readGestureEv+0xb8>
 8018faa:	f894 2080 	ldrb.w	r2, [r4, #128]	; 0x80
 8018fae:	f813 1c04 	ldrb.w	r1, [r3, #-4]
 8018fb2:	f813 5c03 	ldrb.w	r5, [r3, #-3]
 8018fb6:	54a1      	strb	r1, [r4, r2]
 8018fb8:	18a1      	adds	r1, r4, r2
 8018fba:	f881 5020 	strb.w	r5, [r1, #32]
 8018fbe:	f813 5c02 	ldrb.w	r5, [r3, #-2]
 8018fc2:	3201      	adds	r2, #1
 8018fc4:	f881 5040 	strb.w	r5, [r1, #64]	; 0x40
 8018fc8:	f813 5c01 	ldrb.w	r5, [r3, #-1]
 8018fcc:	f881 5060 	strb.w	r5, [r1, #96]	; 0x60
 8018fd0:	f884 2080 	strb.w	r2, [r4, #128]	; 0x80
 8018fd4:	f894 2081 	ldrb.w	r2, [r4, #129]	; 0x81
 8018fd8:	3201      	adds	r2, #1
 8018fda:	f884 2081 	strb.w	r2, [r4, #129]	; 0x81
 8018fde:	e7e1      	b.n	8018fa4 <_ZN17SparkFun_APDS996011readGestureEv+0x7c>
 8018fe0:	4620      	mov	r0, r4
 8018fe2:	f7ff f8d4 	bl	801818e <_ZN17SparkFun_APDS996018processGestureDataEv>
 8018fe6:	b110      	cbz	r0, 8018fee <_ZN17SparkFun_APDS996011readGestureEv+0xc6>
 8018fe8:	4620      	mov	r0, r4
 8018fea:	f7ff f99e 	bl	801832a <_ZN17SparkFun_APDS996013decodeGestureEv>
 8018fee:	2300      	movs	r3, #0
 8018ff0:	f8a4 3080 	strh.w	r3, [r4, #128]	; 0x80
 8018ff4:	e7ab      	b.n	8018f4e <_ZN17SparkFun_APDS996011readGestureEv+0x26>
 8018ff6:	201e      	movs	r0, #30
 8018ff8:	f7ff f828 	bl	801804c <_ZN7Arduino5delayEi>
 8018ffc:	4620      	mov	r0, r4
 8018ffe:	f7ff f994 	bl	801832a <_ZN17SparkFun_APDS996013decodeGestureEv>
 8019002:	4620      	mov	r0, r4
 8019004:	f8d4 20a0 	ldr.w	r2, [r4, #160]	; 0xa0
 8019008:	f7ff f8b5 	bl	8018176 <_ZN17SparkFun_APDS996022resetGestureParametersEv>
 801900c:	e796      	b.n	8018f3c <_ZN17SparkFun_APDS996011readGestureEv+0x14>
 801900e:	bf00      	nop

08019010 <_Z16interruptRoutinev>:
 8019010:	2201      	movs	r2, #1
 8019012:	4b01      	ldr	r3, [pc, #4]	; (8019018 <_Z16interruptRoutinev+0x8>)
 8019014:	601a      	str	r2, [r3, #0]
 8019016:	4770      	bx	lr
 8019018:	200050b4 	.word	0x200050b4

0801901c <_Z5setupv>:
 801901c:	b508      	push	{r3, lr}
 801901e:	2100      	movs	r1, #0
 8019020:	2002      	movs	r0, #2
 8019022:	f7ff f819 	bl	8018058 <_ZN7Arduino7pinModeEii>
 8019026:	f44f 5116 	mov.w	r1, #9600	; 0x2580
 801902a:	4817      	ldr	r0, [pc, #92]	; (8019088 <_Z5setupv+0x6c>)
 801902c:	f7ff f862 	bl	80180f4 <_ZN7Arduino7CSerial5beginEi>
 8019030:	4916      	ldr	r1, [pc, #88]	; (801908c <_Z5setupv+0x70>)
 8019032:	4815      	ldr	r0, [pc, #84]	; (8019088 <_Z5setupv+0x6c>)
 8019034:	f7ff f872 	bl	801811c <_ZN7Arduino7CSerial5printEPKc>
 8019038:	4915      	ldr	r1, [pc, #84]	; (8019090 <_Z5setupv+0x74>)
 801903a:	4813      	ldr	r0, [pc, #76]	; (8019088 <_Z5setupv+0x6c>)
 801903c:	f7ff f848 	bl	80180d0 <_ZN7Arduino7CSerial7printlnEPKc>
 8019040:	4914      	ldr	r1, [pc, #80]	; (8019094 <_Z5setupv+0x78>)
 8019042:	4811      	ldr	r0, [pc, #68]	; (8019088 <_Z5setupv+0x6c>)
 8019044:	f7ff f844 	bl	80180d0 <_ZN7Arduino7CSerial7printlnEPKc>
 8019048:	4911      	ldr	r1, [pc, #68]	; (8019090 <_Z5setupv+0x74>)
 801904a:	480f      	ldr	r0, [pc, #60]	; (8019088 <_Z5setupv+0x6c>)
 801904c:	f7ff f840 	bl	80180d0 <_ZN7Arduino7CSerial7printlnEPKc>
 8019050:	2000      	movs	r0, #0
 8019052:	2201      	movs	r2, #1
 8019054:	4910      	ldr	r1, [pc, #64]	; (8019098 <_Z5setupv+0x7c>)
 8019056:	f7fe fffd 	bl	8018054 <_ZN7Arduino15attachInterruptEiPFvvEi>
 801905a:	4810      	ldr	r0, [pc, #64]	; (801909c <_Z5setupv+0x80>)
 801905c:	f7ff fde0 	bl	8018c20 <_ZN17SparkFun_APDS99604initEv>
 8019060:	b170      	cbz	r0, 8019080 <_Z5setupv+0x64>
 8019062:	490f      	ldr	r1, [pc, #60]	; (80190a0 <_Z5setupv+0x84>)
 8019064:	4808      	ldr	r0, [pc, #32]	; (8019088 <_Z5setupv+0x6c>)
 8019066:	f7ff f833 	bl	80180d0 <_ZN7Arduino7CSerial7printlnEPKc>
 801906a:	2101      	movs	r1, #1
 801906c:	480b      	ldr	r0, [pc, #44]	; (801909c <_Z5setupv+0x80>)
 801906e:	f7ff fee5 	bl	8018e3c <_ZN17SparkFun_APDS996019enableGestureSensorEb>
 8019072:	b138      	cbz	r0, 8019084 <_Z5setupv+0x68>
 8019074:	490b      	ldr	r1, [pc, #44]	; (80190a4 <_Z5setupv+0x88>)
 8019076:	4804      	ldr	r0, [pc, #16]	; (8019088 <_Z5setupv+0x6c>)
 8019078:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 801907c:	f7ff b828 	b.w	80180d0 <_ZN7Arduino7CSerial7printlnEPKc>
 8019080:	4909      	ldr	r1, [pc, #36]	; (80190a8 <_Z5setupv+0x8c>)
 8019082:	e7ef      	b.n	8019064 <_Z5setupv+0x48>
 8019084:	4909      	ldr	r1, [pc, #36]	; (80190ac <_Z5setupv+0x90>)
 8019086:	e7f6      	b.n	8019076 <_Z5setupv+0x5a>
 8019088:	200050b1 	.word	0x200050b1
 801908c:	08019195 	.word	0x08019195
 8019090:	080191a9 	.word	0x080191a9
 8019094:	080191ca 	.word	0x080191ca
 8019098:	08019011 	.word	0x08019011
 801909c:	200050b8 	.word	0x200050b8
 80190a0:	080191eb 	.word	0x080191eb
 80190a4:	08019239 	.word	0x08019239
 80190a8:	0801920d 	.word	0x0801920d
 80190ac:	08019257 	.word	0x08019257

080190b0 <_Z13handleGesturev>:
 80190b0:	b508      	push	{r3, lr}
 80190b2:	4811      	ldr	r0, [pc, #68]	; (80190f8 <_Z13handleGesturev+0x48>)
 80190b4:	f7ff fa72 	bl	801859c <_ZN17SparkFun_APDS996018isGestureAvailableEv>
 80190b8:	b1e0      	cbz	r0, 80190f4 <_Z13handleGesturev+0x44>
 80190ba:	480f      	ldr	r0, [pc, #60]	; (80190f8 <_Z13handleGesturev+0x48>)
 80190bc:	f7ff ff34 	bl	8018f28 <_ZN17SparkFun_APDS996011readGestureEv>
 80190c0:	3801      	subs	r0, #1
 80190c2:	2805      	cmp	r0, #5
 80190c4:	d814      	bhi.n	80190f0 <_Z13handleGesturev+0x40>
 80190c6:	e8df f000 	tbb	[pc, r0]
 80190ca:	0d0b      	.short	0x0d0b
 80190cc:	110f0903 	.word	0x110f0903
 80190d0:	490a      	ldr	r1, [pc, #40]	; (80190fc <_Z13handleGesturev+0x4c>)
 80190d2:	480b      	ldr	r0, [pc, #44]	; (8019100 <_Z13handleGesturev+0x50>)
 80190d4:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 80190d8:	f7fe bffa 	b.w	80180d0 <_ZN7Arduino7CSerial7printlnEPKc>
 80190dc:	4909      	ldr	r1, [pc, #36]	; (8019104 <_Z13handleGesturev+0x54>)
 80190de:	e7f8      	b.n	80190d2 <_Z13handleGesturev+0x22>
 80190e0:	4909      	ldr	r1, [pc, #36]	; (8019108 <_Z13handleGesturev+0x58>)
 80190e2:	e7f6      	b.n	80190d2 <_Z13handleGesturev+0x22>
 80190e4:	4909      	ldr	r1, [pc, #36]	; (801910c <_Z13handleGesturev+0x5c>)
 80190e6:	e7f4      	b.n	80190d2 <_Z13handleGesturev+0x22>
 80190e8:	4909      	ldr	r1, [pc, #36]	; (8019110 <_Z13handleGesturev+0x60>)
 80190ea:	e7f2      	b.n	80190d2 <_Z13handleGesturev+0x22>
 80190ec:	4909      	ldr	r1, [pc, #36]	; (8019114 <_Z13handleGesturev+0x64>)
 80190ee:	e7f0      	b.n	80190d2 <_Z13handleGesturev+0x22>
 80190f0:	4909      	ldr	r1, [pc, #36]	; (8019118 <_Z13handleGesturev+0x68>)
 80190f2:	e7ee      	b.n	80190d2 <_Z13handleGesturev+0x22>
 80190f4:	bd08      	pop	{r3, pc}
 80190f6:	bf00      	nop
 80190f8:	200050b8 	.word	0x200050b8
 80190fc:	08019288 	.word	0x08019288
 8019100:	200050b1 	.word	0x200050b1
 8019104:	0801928b 	.word	0x0801928b
 8019108:	08019290 	.word	0x08019290
 801910c:	08019295 	.word	0x08019295
 8019110:	0801929b 	.word	0x0801929b
 8019114:	080192a0 	.word	0x080192a0
 8019118:	080192a4 	.word	0x080192a4

0801911c <_Z4loopv>:
 801911c:	b508      	push	{r3, lr}
 801911e:	2000      	movs	r0, #0
 8019120:	f7fe ff99 	bl	8018056 <_ZN7Arduino15detachInterruptEi>
 8019124:	f7ff ffc4 	bl	80190b0 <_Z13handleGesturev>
 8019128:	2000      	movs	r0, #0
 801912a:	4b05      	ldr	r3, [pc, #20]	; (8019140 <_Z4loopv+0x24>)
 801912c:	2201      	movs	r2, #1
 801912e:	4905      	ldr	r1, [pc, #20]	; (8019144 <_Z4loopv+0x28>)
 8019130:	6018      	str	r0, [r3, #0]
 8019132:	f7fe ff8f 	bl	8018054 <_ZN7Arduino15attachInterruptEiPFvvEi>
 8019136:	e8bd 4008 	ldmia.w	sp!, {r3, lr}
 801913a:	2032      	movs	r0, #50	; 0x32
 801913c:	f7fe bf86 	b.w	801804c <_ZN7Arduino5delayEi>
 8019140:	200050b4 	.word	0x200050b4
 8019144:	08019011 	.word	0x08019011

08019148 <_GLOBAL__sub_I_apds>:
 8019148:	b510      	push	{r4, lr}
 801914a:	4c05      	ldr	r4, [pc, #20]	; (8019160 <_GLOBAL__sub_I_apds+0x18>)
 801914c:	4620      	mov	r0, r4
 801914e:	f7ff f807 	bl	8018160 <_ZN17SparkFun_APDS9960C1Ev>
 8019152:	4620      	mov	r0, r4
 8019154:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8019158:	4a02      	ldr	r2, [pc, #8]	; (8019164 <_GLOBAL__sub_I_apds+0x1c>)
 801915a:	4903      	ldr	r1, [pc, #12]	; (8019168 <_GLOBAL__sub_I_apds+0x20>)
 801915c:	f7fe bf6a 	b.w	8018034 <__aeabi_atexit>
 8019160:	200050b8 	.word	0x200050b8
 8019164:	200050a8 	.word	0x200050a8
 8019168:	08018175 	.word	0x08018175
 801916c:	74615f5f 	.word	0x74615f5f
 8019170:	74697865 	.word	0x74697865
 8019174:	3025203a 	.word	0x3025203a
 8019178:	25207838 	.word	0x25207838
 801917c:	20783830 	.word	0x20783830
 8019180:	78383025 	.word	0x78383025
 8019184:	735b000a 	.word	0x735b000a
 8019188:	70757465 	.word	0x70757465
 801918c:	5b000a5d 	.word	0x5b000a5d
 8019190:	706f6f6c 	.word	0x706f6f6c
 8019194:	0a5d      	.short	0x0a5d
 8019196:	00          	.byte	0x00
 8019197:	25          	.byte	0x25
 8019198:	25000a73 	.word	0x25000a73
 801919c:	25000a64 	.word	0x25000a64
 80191a0:	25000a78 	.word	0x25000a78
 80191a4:	64250063 	.word	0x64250063
 80191a8:	00          	.byte	0x00
 80191a9:	2d          	.byte	0x2d
 80191aa:	2d2d      	.short	0x2d2d
 80191ac:	2d2d2d2d 	.word	0x2d2d2d2d
 80191b0:	2d2d2d2d 	.word	0x2d2d2d2d
 80191b4:	2d2d2d2d 	.word	0x2d2d2d2d
 80191b8:	2d2d2d2d 	.word	0x2d2d2d2d
 80191bc:	2d2d2d2d 	.word	0x2d2d2d2d
 80191c0:	2d2d2d2d 	.word	0x2d2d2d2d
 80191c4:	2d2d2d2d 	.word	0x2d2d2d2d
 80191c8:	7053002d 	.word	0x7053002d
 80191cc:	466b7261 	.word	0x466b7261
 80191d0:	41206e75 	.word	0x41206e75
 80191d4:	2d534450 	.word	0x2d534450
 80191d8:	30363939 	.word	0x30363939
 80191dc:	47202d20 	.word	0x47202d20
 80191e0:	75747365 	.word	0x75747365
 80191e4:	65546572 	.word	0x65546572
 80191e8:	41007473 	.word	0x41007473
 80191ec:	2d534450 	.word	0x2d534450
 80191f0:	30363939 	.word	0x30363939
 80191f4:	696e6920 	.word	0x696e6920
 80191f8:	6c616974 	.word	0x6c616974
 80191fc:	74617a69 	.word	0x74617a69
 8019200:	206e6f69 	.word	0x206e6f69
 8019204:	706d6f63 	.word	0x706d6f63
 8019208:	6574656c 	.word	0x6574656c
 801920c:	6d6f5300 	.word	0x6d6f5300
 8019210:	69687465 	.word	0x69687465
 8019214:	7720676e 	.word	0x7720676e
 8019218:	20746e65 	.word	0x20746e65
 801921c:	6e6f7277 	.word	0x6e6f7277
 8019220:	75642067 	.word	0x75642067
 8019224:	676e6972 	.word	0x676e6972
 8019228:	44504120 	.word	0x44504120
 801922c:	39392d53 	.word	0x39392d53
 8019230:	69203036 	.word	0x69203036
 8019234:	2174696e 	.word	0x2174696e
 8019238:	73654700 	.word	0x73654700
 801923c:	65727574 	.word	0x65727574
 8019240:	6e657320 	.word	0x6e657320
 8019244:	20726f73 	.word	0x20726f73
 8019248:	6e207369 	.word	0x6e207369
 801924c:	7220776f 	.word	0x7220776f
 8019250:	696e6e75 	.word	0x696e6e75
 8019254:	5300676e 	.word	0x5300676e
 8019258:	74656d6f 	.word	0x74656d6f
 801925c:	676e6968 	.word	0x676e6968
 8019260:	6e657720 	.word	0x6e657720
 8019264:	72772074 	.word	0x72772074
 8019268:	20676e6f 	.word	0x20676e6f
 801926c:	69727564 	.word	0x69727564
 8019270:	6720676e 	.word	0x6720676e
 8019274:	75747365 	.word	0x75747365
 8019278:	73206572 	.word	0x73206572
 801927c:	6f736e65 	.word	0x6f736e65
 8019280:	6e692072 	.word	0x6e692072
 8019284:	00217469 	.word	0x00217469
 8019288:	44005055 	.word	0x44005055
 801928c:	004e574f 	.word	0x004e574f
 8019290:	5446454c 	.word	0x5446454c
 8019294:	47495200 	.word	0x47495200
 8019298:	4e005448 	.word	0x4e005448
 801929c:	00524145 	.word	0x00524145
 80192a0:	00524146 	.word	0x00524146
 80192a4:	454e4f4e 	.word	0x454e4f4e
	...

Disassembly of section .plt:

080193a8 <.plt>:
 80193a8:	b500      	push	{lr}
 80193aa:	f8df e008 	ldr.w	lr, [pc, #8]	; 80193b4 <.plt+0xc>
 80193ae:	44fe      	add	lr, pc
 80193b0:	f85e ff08 	ldr.w	pc, [lr, #8]!
 80193b4:	17febda8 	.word	0x17febda8

080193b8 <_ZN4BIOS4GPIO3I2C17BeginTransmissionEh@plt>:
 80193b8:	0000      	movs	r0, r0
 80193ba:	0000      	movs	r0, r0
 80193bc:	f64b 5ca0 	movw	ip, #48544	; 0xbda0
 80193c0:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80193c4:	44fc      	add	ip, pc
 80193c6:	Address 0x00000000080193c6 is out of bounds.


080193c8 <_ZN4BIOS4GPIO3I2C15EndTransmissionEv@plt>:
 80193c8:	f000 bf00 	b.w	801a1cc <_ZN4BIOS3SYS7GetTickEv@plt+0xd74>
 80193cc:	f64b 5c94 	movw	ip, #48532	; 0xbd94
 80193d0:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80193d4:	44fc      	add	ip, pc
 80193d6:	Address 0x00000000080193d6 is out of bounds.


080193d8 <_ZN4BIOS4GPIO3I2C5WriteEh@plt>:
 80193d8:	f000 bf00 	b.w	801a1dc <_ZN4BIOS3SYS7GetTickEv@plt+0xd84>
 80193dc:	0000      	movs	r0, r0
 80193de:	0000      	movs	r0, r0
 80193e0:	f64b 5c84 	movw	ip, #48516	; 0xbd84
 80193e4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe

080193e8 <_ZN4BIOS4GPIO3I2C11RequestFromEhh@plt>:
 80193e8:	44fc      	add	ip, pc
 80193ea:	f8dc f000 	ldr.w	pc, [ip]
 80193ee:	bf00      	nop
 80193f0:	f64b 5c78 	movw	ip, #48504	; 0xbd78
 80193f4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe

080193f8 <_ZN4BIOS3DBG5PrintEPKcz@plt>:
 80193f8:	44fc      	add	ip, pc
 80193fa:	f8dc f000 	ldr.w	pc, [ip]
 80193fe:	bf00      	nop
 8019400:	0000      	movs	r0, r0
 8019402:	0000      	movs	r0, r0
 8019404:	f64b 5c68 	movw	ip, #48488	; 0xbd68

08019408 <_ZN4BIOS3KEY6GetKeyEv@plt>:
 8019408:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801940c:	44fc      	add	ip, pc
 801940e:	f8dc f000 	ldr.w	pc, [ip]
 8019412:	bf00      	nop
 8019414:	f64b 5c5c 	movw	ip, #48476	; 0xbd5c

08019418 <_ZN4BIOS4GPIO3I2C4ReadEv@plt>:
 8019418:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801941c:	44fc      	add	ip, pc
 801941e:	f8dc f000 	ldr.w	pc, [ip]
 8019422:	bf00      	nop
 8019424:	f64b 5c50 	movw	ip, #48464	; 0xbd50

08019428 <_ZN4BIOS3LCD5ClearEt@plt>:
 8019428:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801942c:	44fc      	add	ip, pc
 801942e:	f8dc f000 	ldr.w	pc, [ip]
 8019432:	bf00      	nop
 8019434:	f64b 5c44 	movw	ip, #48452	; 0xbd44

08019438 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>:
 8019438:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 801943c:	44fc      	add	ip, pc
 801943e:	f8dc f000 	ldr.w	pc, [ip]
 8019442:	bf00      	nop
 8019444:	0000      	movs	r0, r0
	...

08019448 <_ZN4BIOS3SYS7DelayMsEi@plt>:
 8019448:	f64b 5c34 	movw	ip, #48436	; 0xbd34
 801944c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8019450:	44fc      	add	ip, pc
 8019452:	f8dc f000 	ldr.w	pc, [ip]
 8019456:	bf00      	nop

08019458 <_ZN4BIOS3SYS7GetTickEv@plt>:
 8019458:	0000      	movs	r0, r0
 801945a:	0000      	movs	r0, r0
 801945c:	f64b 5c24 	movw	ip, #48420	; 0xbd24
 8019460:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8019464:	44fc      	add	ip, pc
 8019466:	f8dc f000 	ldr.w	pc, [ip]
 801946a:	bf00      	nop
 801946c:	0000      	movs	r0, r0
 801946e:	0000      	movs	r0, r0
 8019470:	f64b 5c14 	movw	ip, #48404	; 0xbd14
 8019474:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8019478:	44fc      	add	ip, pc
 801947a:	f8dc f000 	ldr.w	pc, [ip]
 801947e:	bf00      	nop
