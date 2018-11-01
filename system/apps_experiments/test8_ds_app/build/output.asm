
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08020000 <main>:
 8020000:	b500      	push	{lr}
 8020002:	b08b      	sub	sp, #44	; 0x2c
 8020004:	2210      	movs	r2, #16
 8020006:	2100      	movs	r1, #0
 8020008:	4668      	mov	r0, sp
 802000a:	f000 fedd 	bl	8020dc8 <memset>
 802000e:	2300      	movs	r3, #0
 8020010:	4668      	mov	r0, sp
 8020012:	9304      	str	r3, [sp, #16]
 8020014:	f88d 301c 	strb.w	r3, [sp, #28]
 8020018:	f000 fa58 	bl	80204cc <_ZN14CVisualMeasure7OnPaintEv>
 802001c:	f001 f888 	bl	8021130 <_ZN4BIOS3KEY6GetKeyEv@plt+0x4>
 8020020:	2806      	cmp	r0, #6
 8020022:	d007      	beq.n	8020034 <main+0x34>
 8020024:	4668      	mov	r0, sp
 8020026:	f000 f8e3 	bl	80201f0 <_ZN14CVisualMeasure12UpdateDs1820Ev>
 802002a:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
 802002e:	f000 f824 	bl	802007a <_ZN6CDelay7DelayMsEm>
 8020032:	e7f3      	b.n	802001c <main+0x1c>
 8020034:	2000      	movs	r0, #0
 8020036:	b00b      	add	sp, #44	; 0x2c
 8020038:	f85d fb04 	ldr.w	pc, [sp], #4

0802003c <_ZN4CPin3LowEv>:
 802003c:	b510      	push	{r4, lr}
 802003e:	4604      	mov	r4, r0
 8020040:	2102      	movs	r1, #2
 8020042:	7800      	ldrb	r0, [r0, #0]
 8020044:	f001 f87c 	bl	8021140 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt+0x4>
 8020048:	7820      	ldrb	r0, [r4, #0]
 802004a:	2100      	movs	r1, #0
 802004c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8020050:	f001 b84e 	b.w	80210f0 <_ZN4BIOS4GPIO12DigitalWriteENS0_4EPinEb@plt+0x4>

08020054 <_ZN4CPin5FloatEv>:
 8020054:	b510      	push	{r4, lr}
 8020056:	4604      	mov	r4, r0
 8020058:	2109      	movs	r1, #9
 802005a:	7800      	ldrb	r0, [r0, #0]
 802005c:	f001 f870 	bl	8021140 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt+0x4>
 8020060:	7820      	ldrb	r0, [r4, #0]
 8020062:	2101      	movs	r1, #1
 8020064:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
 8020068:	f001 b842 	b.w	80210f0 <_ZN4BIOS4GPIO12DigitalWriteENS0_4EPinEb@plt+0x4>

0802006c <_ZN6CDelay7DelayUsEm>:
 802006c:	230c      	movs	r3, #12
 802006e:	4358      	muls	r0, r3
 8020070:	f110 30ff 	adds.w	r0, r0, #4294967295	; 0xffffffff
 8020074:	d300      	bcc.n	8020078 <_ZN6CDelay7DelayUsEm+0xc>
 8020076:	e7fb      	b.n	8020070 <_ZN6CDelay7DelayUsEm+0x4>
 8020078:	4770      	bx	lr

0802007a <_ZN6CDelay7DelayMsEm>:
 802007a:	f110 30ff 	adds.w	r0, r0, #4294967295	; 0xffffffff
 802007e:	d304      	bcc.n	802008a <_ZN6CDelay7DelayMsEm+0x10>
 8020080:	f642 63e1 	movw	r3, #12001	; 0x2ee1
 8020084:	3b01      	subs	r3, #1
 8020086:	d0f8      	beq.n	802007a <_ZN6CDelay7DelayMsEm>
 8020088:	e7fc      	b.n	8020084 <_ZN6CDelay7DelayMsEm+0xa>
 802008a:	4770      	bx	lr

0802008c <_ZN7CDS18204InitEv>:
 802008c:	b510      	push	{r4, lr}
 802008e:	4604      	mov	r4, r0
 8020090:	f7ff ffe0 	bl	8020054 <_ZN4CPin5FloatEv>
 8020094:	2032      	movs	r0, #50	; 0x32
 8020096:	f7ff ffe9 	bl	802006c <_ZN6CDelay7DelayUsEm>
 802009a:	4620      	mov	r0, r4
 802009c:	f7ff ffce 	bl	802003c <_ZN4CPin3LowEv>
 80200a0:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
 80200a4:	f7ff ffe2 	bl	802006c <_ZN6CDelay7DelayUsEm>
 80200a8:	4620      	mov	r0, r4
 80200aa:	f7ff ffd3 	bl	8020054 <_ZN4CPin5FloatEv>
 80200ae:	2064      	movs	r0, #100	; 0x64
 80200b0:	f7ff ffdc 	bl	802006c <_ZN6CDelay7DelayUsEm>
 80200b4:	7820      	ldrb	r0, [r4, #0]
 80200b6:	f001 f823 	bl	8021100 <_ZN4BIOS4GPIO11DigitalReadENS0_4EPinE@plt+0x4>
 80200ba:	b928      	cbnz	r0, 80200c8 <_ZN7CDS18204InitEv+0x3c>
 80200bc:	f44f 70c8 	mov.w	r0, #400	; 0x190
 80200c0:	f7ff ffd4 	bl	802006c <_ZN6CDelay7DelayUsEm>
 80200c4:	2001      	movs	r0, #1
 80200c6:	bd10      	pop	{r4, pc}
 80200c8:	2000      	movs	r0, #0
 80200ca:	e7fc      	b.n	80200c6 <_ZN7CDS18204InitEv+0x3a>

080200cc <_ZN7CDS18203CrcEi>:
 80200cc:	b570      	push	{r4, r5, r6, lr}
 80200ce:	2300      	movs	r3, #0
 80200d0:	4604      	mov	r4, r0
 80200d2:	1a22      	subs	r2, r4, r0
 80200d4:	4291      	cmp	r1, r2
 80200d6:	dd11      	ble.n	80200fc <_ZN7CDS18203CrcEi+0x30>
 80200d8:	2200      	movs	r2, #0
 80200da:	f814 6f01 	ldrb.w	r6, [r4, #1]!
 80200de:	fa46 f502 	asr.w	r5, r6, r2
 80200e2:	405d      	eors	r5, r3
 80200e4:	f015 0f01 	tst.w	r5, #1
 80200e8:	ea4f 0363 	mov.w	r3, r3, asr #1
 80200ec:	f102 0201 	add.w	r2, r2, #1
 80200f0:	bf18      	it	ne
 80200f2:	f083 038c 	eorne.w	r3, r3, #140	; 0x8c
 80200f6:	2a08      	cmp	r2, #8
 80200f8:	d1f1      	bne.n	80200de <_ZN7CDS18203CrcEi+0x12>
 80200fa:	e7ea      	b.n	80200d2 <_ZN7CDS18203CrcEi+0x6>
 80200fc:	fab3 f083 	clz	r0, r3
 8020100:	0940      	lsrs	r0, r0, #5
 8020102:	bd70      	pop	{r4, r5, r6, pc}

08020104 <_ZN7CDS18202InEv>:
 8020104:	b570      	push	{r4, r5, r6, lr}
 8020106:	4606      	mov	r6, r0
 8020108:	2508      	movs	r5, #8
 802010a:	2400      	movs	r4, #0
 802010c:	4630      	mov	r0, r6
 802010e:	f7ff ff95 	bl	802003c <_ZN4CPin3LowEv>
 8020112:	2005      	movs	r0, #5
 8020114:	f7ff ffaa 	bl	802006c <_ZN6CDelay7DelayUsEm>
 8020118:	4630      	mov	r0, r6
 802011a:	f7ff ff9b 	bl	8020054 <_ZN4CPin5FloatEv>
 802011e:	2005      	movs	r0, #5
 8020120:	f7ff ffa4 	bl	802006c <_ZN6CDelay7DelayUsEm>
 8020124:	7830      	ldrb	r0, [r6, #0]
 8020126:	0864      	lsrs	r4, r4, #1
 8020128:	f000 ffea 	bl	8021100 <_ZN4BIOS4GPIO11DigitalReadENS0_4EPinE@plt+0x4>
 802012c:	b110      	cbz	r0, 8020134 <_ZN7CDS18202InEv+0x30>
 802012e:	f064 047f 	orn	r4, r4, #127	; 0x7f
 8020132:	b2e4      	uxtb	r4, r4
 8020134:	2032      	movs	r0, #50	; 0x32
 8020136:	f7ff ff99 	bl	802006c <_ZN6CDelay7DelayUsEm>
 802013a:	3d01      	subs	r5, #1
 802013c:	d1e6      	bne.n	802010c <_ZN7CDS18202InEv+0x8>
 802013e:	4620      	mov	r0, r4
 8020140:	bd70      	pop	{r4, r5, r6, pc}

08020142 <_ZN7CDS18203OutEh>:
 8020142:	b570      	push	{r4, r5, r6, lr}
 8020144:	4606      	mov	r6, r0
 8020146:	460d      	mov	r5, r1
 8020148:	2408      	movs	r4, #8
 802014a:	4630      	mov	r0, r6
 802014c:	f7ff ff76 	bl	802003c <_ZN4CPin3LowEv>
 8020150:	07eb      	lsls	r3, r5, #31
 8020152:	d50d      	bpl.n	8020170 <_ZN7CDS18203OutEh+0x2e>
 8020154:	2005      	movs	r0, #5
 8020156:	f7ff ff89 	bl	802006c <_ZN6CDelay7DelayUsEm>
 802015a:	4630      	mov	r0, r6
 802015c:	f7ff ff7a 	bl	8020054 <_ZN4CPin5FloatEv>
 8020160:	2037      	movs	r0, #55	; 0x37
 8020162:	f7ff ff83 	bl	802006c <_ZN6CDelay7DelayUsEm>
 8020166:	3c01      	subs	r4, #1
 8020168:	ea4f 0555 	mov.w	r5, r5, lsr #1
 802016c:	d1ed      	bne.n	802014a <_ZN7CDS18203OutEh+0x8>
 802016e:	bd70      	pop	{r4, r5, r6, pc}
 8020170:	2037      	movs	r0, #55	; 0x37
 8020172:	f7ff ff7b 	bl	802006c <_ZN6CDelay7DelayUsEm>
 8020176:	4630      	mov	r0, r6
 8020178:	f7ff ff6c 	bl	8020054 <_ZN4CPin5FloatEv>
 802017c:	2005      	movs	r0, #5
 802017e:	e7f0      	b.n	8020162 <_ZN7CDS18203OutEh+0x20>

08020180 <_ZN7CDS18207ConvertEi>:
 8020180:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8020182:	4604      	mov	r4, r0
 8020184:	460d      	mov	r5, r1
 8020186:	f7ff ff81 	bl	802008c <_ZN7CDS18204InitEv>
 802018a:	b370      	cbz	r0, 80201ea <_ZN7CDS18207ConvertEi+0x6a>
 802018c:	21cc      	movs	r1, #204	; 0xcc
 802018e:	4620      	mov	r0, r4
 8020190:	f7ff ffd7 	bl	8020142 <_ZN7CDS18203OutEh>
 8020194:	2144      	movs	r1, #68	; 0x44
 8020196:	4620      	mov	r0, r4
 8020198:	f7ff ffd3 	bl	8020142 <_ZN7CDS18203OutEh>
 802019c:	2d28      	cmp	r5, #40	; 0x28
 802019e:	d107      	bne.n	80201b0 <_ZN7CDS18207ConvertEi+0x30>
 80201a0:	2102      	movs	r1, #2
 80201a2:	7820      	ldrb	r0, [r4, #0]
 80201a4:	f000 ffcc 	bl	8021140 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt+0x4>
 80201a8:	2101      	movs	r1, #1
 80201aa:	7820      	ldrb	r0, [r4, #0]
 80201ac:	f000 ffa0 	bl	80210f0 <_ZN4BIOS4GPIO12DigitalWriteENS0_4EPinEb@plt+0x4>
 80201b0:	f240 3002 	movw	r0, #770	; 0x302
 80201b4:	f7ff ff61 	bl	802007a <_ZN6CDelay7DelayMsEm>
 80201b8:	4620      	mov	r0, r4
 80201ba:	f7ff ff67 	bl	802008c <_ZN7CDS18204InitEv>
 80201be:	4605      	mov	r5, r0
 80201c0:	b198      	cbz	r0, 80201ea <_ZN7CDS18207ConvertEi+0x6a>
 80201c2:	21cc      	movs	r1, #204	; 0xcc
 80201c4:	4620      	mov	r0, r4
 80201c6:	f7ff ffbc 	bl	8020142 <_ZN7CDS18203OutEh>
 80201ca:	21be      	movs	r1, #190	; 0xbe
 80201cc:	4620      	mov	r0, r4
 80201ce:	f7ff ffb8 	bl	8020142 <_ZN7CDS18203OutEh>
 80201d2:	1c66      	adds	r6, r4, #1
 80201d4:	f104 070a 	add.w	r7, r4, #10
 80201d8:	4620      	mov	r0, r4
 80201da:	f7ff ff93 	bl	8020104 <_ZN7CDS18202InEv>
 80201de:	f806 0b01 	strb.w	r0, [r6], #1
 80201e2:	42be      	cmp	r6, r7
 80201e4:	d1f8      	bne.n	80201d8 <_ZN7CDS18207ConvertEi+0x58>
 80201e6:	4628      	mov	r0, r5
 80201e8:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
 80201ea:	2500      	movs	r5, #0
 80201ec:	e7fb      	b.n	80201e6 <_ZN7CDS18207ConvertEi+0x66>
	...

080201f0 <_ZN14CVisualMeasure12UpdateDs1820Ev>:
 80201f0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 80201f4:	f100 061c 	add.w	r6, r0, #28
 80201f8:	f100 0a1d 	add.w	sl, r0, #29
 80201fc:	4604      	mov	r4, r0
 80201fe:	b095      	sub	sp, #84	; 0x54
 8020200:	4630      	mov	r0, r6
 8020202:	f7ff ff43 	bl	802008c <_ZN7CDS18204InitEv>
 8020206:	2300      	movs	r3, #0
 8020208:	4680      	mov	r8, r0
 802020a:	f8c4 301d 	str.w	r3, [r4, #29]
 802020e:	f8ca 3004 	str.w	r3, [sl, #4]
 8020212:	f88a 3008 	strb.w	r3, [sl, #8]
 8020216:	2800      	cmp	r0, #0
 8020218:	f040 80f8 	bne.w	802040c <_ZN14CVisualMeasure12UpdateDs1820Ev+0x21c>
 802021c:	2700      	movs	r7, #0
 802021e:	463d      	mov	r5, r7
 8020220:	f04f 0b00 	mov.w	fp, #0
 8020224:	69a1      	ldr	r1, [r4, #24]
 8020226:	4b9c      	ldr	r3, [pc, #624]	; (8020498 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2a8>)
 8020228:	462a      	mov	r2, r5
 802022a:	9300      	str	r3, [sp, #0]
 802022c:	3120      	adds	r1, #32
 802022e:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020232:	6960      	ldr	r0, [r4, #20]
 8020234:	f000 ff74 	bl	8021120 <_ZN4BIOS3LCD5PrintEiittPKc@plt+0x4>
 8020238:	e9cd 6602 	strd	r6, r6, [sp, #8]
 802023c:	f10d 0910 	add.w	r9, sp, #16
 8020240:	9b02      	ldr	r3, [sp, #8]
 8020242:	4996      	ldr	r1, [pc, #600]	; (802049c <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2ac>)
 8020244:	f813 2f01 	ldrb.w	r2, [r3, #1]!
 8020248:	4648      	mov	r0, r9
 802024a:	9302      	str	r3, [sp, #8]
 802024c:	f000 ff88 	bl	8021160 <_Z7sprintfPcPKcz@plt+0x4>
 8020250:	e9d4 0105 	ldrd	r0, r1, [r4, #20]
 8020254:	3020      	adds	r0, #32
 8020256:	4458      	add	r0, fp
 8020258:	f8cd 9000 	str.w	r9, [sp]
 802025c:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020260:	462a      	mov	r2, r5
 8020262:	3120      	adds	r1, #32
 8020264:	f10b 0b12 	add.w	fp, fp, #18
 8020268:	f000 ff38 	bl	80210dc <_ZN4BIOS3LCD5PrintEiittPc@plt>
 802026c:	f1bb 0f90 	cmp.w	fp, #144	; 0x90
 8020270:	d1e6      	bne.n	8020240 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x50>
 8020272:	f1a7 0310 	sub.w	r3, r7, #16
 8020276:	b2db      	uxtb	r3, r3
 8020278:	2b18      	cmp	r3, #24
 802027a:	bf96      	itet	ls
 802027c:	4a88      	ldrls	r2, [pc, #544]	; (80204a0 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2b0>)
 802027e:	4b89      	ldrhi	r3, [pc, #548]	; (80204a4 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2b4>)
 8020280:	f852 3023 	ldrls.w	r3, [r2, r3, lsl #2]
 8020284:	4988      	ldr	r1, [pc, #544]	; (80204a8 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2b8>)
 8020286:	9300      	str	r3, [sp, #0]
 8020288:	093a      	lsrs	r2, r7, #4
 802028a:	f007 030f 	and.w	r3, r7, #15
 802028e:	4648      	mov	r0, r9
 8020290:	f000 ff66 	bl	8021160 <_Z7sprintfPcPKcz@plt+0x4>
 8020294:	69a1      	ldr	r1, [r4, #24]
 8020296:	f24b 5396 	movw	r3, #46486	; 0xb596
 802029a:	f8cd 9000 	str.w	r9, [sp]
 802029e:	462a      	mov	r2, r5
 80202a0:	3130      	adds	r1, #48	; 0x30
 80202a2:	6960      	ldr	r0, [r4, #20]
 80202a4:	f000 ff1a 	bl	80210dc <_ZN4BIOS3LCD5PrintEiittPc@plt>
 80202a8:	2300      	movs	r3, #0
 80202aa:	f8c4 301d 	str.w	r3, [r4, #29]
 80202ae:	f8ca 3004 	str.w	r3, [sl, #4]
 80202b2:	f88a 3008 	strb.w	r3, [sl, #8]
 80202b6:	f1b8 0f00 	cmp.w	r8, #0
 80202ba:	f000 80d1 	beq.w	8020460 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x270>
 80202be:	4639      	mov	r1, r7
 80202c0:	4630      	mov	r0, r6
 80202c2:	9302      	str	r3, [sp, #8]
 80202c4:	f7ff ff5c 	bl	8020180 <_ZN7CDS18207ConvertEi>
 80202c8:	4605      	mov	r5, r0
 80202ca:	2800      	cmp	r0, #0
 80202cc:	f000 80ce 	beq.w	802046c <_ZN14CVisualMeasure12UpdateDs1820Ev+0x27c>
 80202d0:	2109      	movs	r1, #9
 80202d2:	4630      	mov	r0, r6
 80202d4:	f7ff fefa 	bl	80200cc <_ZN7CDS18203CrcEi>
 80202d8:	4683      	mov	fp, r0
 80202da:	2800      	cmp	r0, #0
 80202dc:	f000 80cc 	beq.w	8020478 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x288>
 80202e0:	9b02      	ldr	r3, [sp, #8]
 80202e2:	2f00      	cmp	r7, #0
 80202e4:	f000 80cd 	beq.w	8020482 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x292>
 80202e8:	2f10      	cmp	r7, #16
 80202ea:	f040 80b1 	bne.w	8020450 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x260>
 80202ee:	f894 2024 	ldrb.w	r2, [r4, #36]	; 0x24
 80202f2:	2a10      	cmp	r2, #16
 80202f4:	f040 80cb 	bne.w	802048e <_ZN14CVisualMeasure12UpdateDs1820Ev+0x29e>
 80202f8:	7fa0      	ldrb	r0, [r4, #30]
 80202fa:	7f63      	ldrb	r3, [r4, #29]
 80202fc:	ea43 2300 	orr.w	r3, r3, r0, lsl #8
 8020300:	f343 034e 	sbfx	r3, r3, #1, #15
 8020304:	011b      	lsls	r3, r3, #4
 8020306:	f894 0023 	ldrb.w	r0, [r4, #35]	; 0x23
 802030a:	330c      	adds	r3, #12
 802030c:	1a18      	subs	r0, r3, r0
 802030e:	f000 fbb7 	bl	8020a80 <__aeabi_i2f>
 8020312:	f04f 5176 	mov.w	r1, #1031798784	; 0x3d800000
 8020316:	f000 fc07 	bl	8020b28 <__aeabi_fmul>
 802031a:	465d      	mov	r5, fp
 802031c:	4682      	mov	sl, r0
 802031e:	46d8      	mov	r8, fp
 8020320:	4a62      	ldr	r2, [pc, #392]	; (80204ac <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2bc>)
 8020322:	4963      	ldr	r1, [pc, #396]	; (80204b0 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2c0>)
 8020324:	4648      	mov	r0, r9
 8020326:	f000 ff1b 	bl	8021160 <_Z7sprintfPcPKcz@plt+0x4>
 802032a:	69a1      	ldr	r1, [r4, #24]
 802032c:	f8cd 9000 	str.w	r9, [sp]
 8020330:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020334:	f240 72ff 	movw	r2, #2047	; 0x7ff
 8020338:	3110      	adds	r1, #16
 802033a:	6960      	ldr	r0, [r4, #20]
 802033c:	f000 fece 	bl	80210dc <_ZN4BIOS3LCD5PrintEiittPc@plt>
 8020340:	b12d      	cbz	r5, 802034e <_ZN14CVisualMeasure12UpdateDs1820Ev+0x15e>
 8020342:	f1bb 0f00 	cmp.w	fp, #0
 8020346:	f240 75ff 	movw	r5, #2047	; 0x7ff
 802034a:	bf08      	it	eq
 802034c:	2500      	moveq	r5, #0
 802034e:	69a1      	ldr	r1, [r4, #24]
 8020350:	4b58      	ldr	r3, [pc, #352]	; (80204b4 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2c4>)
 8020352:	462a      	mov	r2, r5
 8020354:	9300      	str	r3, [sp, #0]
 8020356:	3140      	adds	r1, #64	; 0x40
 8020358:	f24b 5396 	movw	r3, #46486	; 0xb596
 802035c:	6960      	ldr	r0, [r4, #20]
 802035e:	f000 fedf 	bl	8021120 <_ZN4BIOS3LCD5PrintEiittPKc@plt+0x4>
 8020362:	f04f 0b00 	mov.w	fp, #0
 8020366:	9b03      	ldr	r3, [sp, #12]
 8020368:	494c      	ldr	r1, [pc, #304]	; (802049c <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2ac>)
 802036a:	f813 2f01 	ldrb.w	r2, [r3, #1]!
 802036e:	4648      	mov	r0, r9
 8020370:	9303      	str	r3, [sp, #12]
 8020372:	f000 fef5 	bl	8021160 <_Z7sprintfPcPKcz@plt+0x4>
 8020376:	e9d4 0105 	ldrd	r0, r1, [r4, #20]
 802037a:	3020      	adds	r0, #32
 802037c:	4458      	add	r0, fp
 802037e:	f8cd 9000 	str.w	r9, [sp]
 8020382:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020386:	462a      	mov	r2, r5
 8020388:	3140      	adds	r1, #64	; 0x40
 802038a:	f10b 0b12 	add.w	fp, fp, #18
 802038e:	f000 fea5 	bl	80210dc <_ZN4BIOS3LCD5PrintEiittPc@plt>
 8020392:	f1bb 0fa2 	cmp.w	fp, #162	; 0xa2
 8020396:	d1e6      	bne.n	8020366 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x176>
 8020398:	f1b8 0f00 	cmp.w	r8, #0
 802039c:	4650      	mov	r0, sl
 802039e:	f240 75ff 	movw	r5, #2047	; 0x7ff
 80203a2:	bf08      	it	eq
 80203a4:	2500      	moveq	r5, #0
 80203a6:	f000 fa59 	bl	802085c <__aeabi_f2d>
 80203aa:	4602      	mov	r2, r0
 80203ac:	460b      	mov	r3, r1
 80203ae:	4648      	mov	r0, r9
 80203b0:	4941      	ldr	r1, [pc, #260]	; (80204b8 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2c8>)
 80203b2:	f000 fed5 	bl	8021160 <_Z7sprintfPcPKcz@plt+0x4>
 80203b6:	69a1      	ldr	r1, [r4, #24]
 80203b8:	f8cd 9000 	str.w	r9, [sp]
 80203bc:	f24b 5396 	movw	r3, #46486	; 0xb596
 80203c0:	462a      	mov	r2, r5
 80203c2:	3150      	adds	r1, #80	; 0x50
 80203c4:	6960      	ldr	r0, [r4, #20]
 80203c6:	f000 fe89 	bl	80210dc <_ZN4BIOS3LCD5PrintEiittPc@plt>
 80203ca:	2f28      	cmp	r7, #40	; 0x28
 80203cc:	d11b      	bne.n	8020406 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x216>
 80203ce:	f894 3021 	ldrb.w	r3, [r4, #33]	; 0x21
 80203d2:	2b7f      	cmp	r3, #127	; 0x7f
 80203d4:	d017      	beq.n	8020406 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x216>
 80203d6:	4630      	mov	r0, r6
 80203d8:	f7ff fe58 	bl	802008c <_ZN7CDS18204InitEv>
 80203dc:	b198      	cbz	r0, 8020406 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x216>
 80203de:	21cc      	movs	r1, #204	; 0xcc
 80203e0:	4630      	mov	r0, r6
 80203e2:	f7ff feae 	bl	8020142 <_ZN7CDS18203OutEh>
 80203e6:	214e      	movs	r1, #78	; 0x4e
 80203e8:	4630      	mov	r0, r6
 80203ea:	f7ff feaa 	bl	8020142 <_ZN7CDS18203OutEh>
 80203ee:	2100      	movs	r1, #0
 80203f0:	4630      	mov	r0, r6
 80203f2:	f7ff fea6 	bl	8020142 <_ZN7CDS18203OutEh>
 80203f6:	2100      	movs	r1, #0
 80203f8:	4630      	mov	r0, r6
 80203fa:	f7ff fea2 	bl	8020142 <_ZN7CDS18203OutEh>
 80203fe:	217f      	movs	r1, #127	; 0x7f
 8020400:	4630      	mov	r0, r6
 8020402:	f7ff fe9e 	bl	8020142 <_ZN7CDS18203OutEh>
 8020406:	b015      	add	sp, #84	; 0x54
 8020408:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 802040c:	4630      	mov	r0, r6
 802040e:	f7ff fe3d 	bl	802008c <_ZN7CDS18204InitEv>
 8020412:	2800      	cmp	r0, #0
 8020414:	f43f af02 	beq.w	802021c <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2c>
 8020418:	2133      	movs	r1, #51	; 0x33
 802041a:	4630      	mov	r0, r6
 802041c:	f7ff fe91 	bl	8020142 <_ZN7CDS18203OutEh>
 8020420:	4655      	mov	r5, sl
 8020422:	f104 0725 	add.w	r7, r4, #37	; 0x25
 8020426:	4630      	mov	r0, r6
 8020428:	f7ff fe6c 	bl	8020104 <_ZN7CDS18202InEv>
 802042c:	f805 0b01 	strb.w	r0, [r5], #1
 8020430:	42af      	cmp	r7, r5
 8020432:	d1f8      	bne.n	8020426 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x236>
 8020434:	2108      	movs	r1, #8
 8020436:	4630      	mov	r0, r6
 8020438:	f7ff fe48 	bl	80200cc <_ZN7CDS18203CrcEi>
 802043c:	2800      	cmp	r0, #0
 802043e:	f43f aeed 	beq.w	802021c <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2c>
 8020442:	7f67      	ldrb	r7, [r4, #29]
 8020444:	f240 75ff 	movw	r5, #2047	; 0x7ff
 8020448:	2f00      	cmp	r7, #0
 802044a:	bf08      	it	eq
 802044c:	2500      	moveq	r5, #0
 802044e:	e6e7      	b.n	8020220 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x30>
 8020450:	2f28      	cmp	r7, #40	; 0x28
 8020452:	d11c      	bne.n	802048e <_ZN14CVisualMeasure12UpdateDs1820Ev+0x29e>
 8020454:	7fa3      	ldrb	r3, [r4, #30]
 8020456:	7f60      	ldrb	r0, [r4, #29]
 8020458:	ea40 2003 	orr.w	r0, r0, r3, lsl #8
 802045c:	b200      	sxth	r0, r0
 802045e:	e756      	b.n	802030e <_ZN14CVisualMeasure12UpdateDs1820Ev+0x11e>
 8020460:	46c3      	mov	fp, r8
 8020462:	4645      	mov	r5, r8
 8020464:	f04f 0a00 	mov.w	sl, #0
 8020468:	4a14      	ldr	r2, [pc, #80]	; (80204bc <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2cc>)
 802046a:	e75a      	b.n	8020322 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x132>
 802046c:	4683      	mov	fp, r0
 802046e:	4680      	mov	r8, r0
 8020470:	f04f 0a00 	mov.w	sl, #0
 8020474:	4a12      	ldr	r2, [pc, #72]	; (80204c0 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2d0>)
 8020476:	e754      	b.n	8020322 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x132>
 8020478:	4680      	mov	r8, r0
 802047a:	f04f 0a00 	mov.w	sl, #0
 802047e:	4a11      	ldr	r2, [pc, #68]	; (80204c4 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2d4>)
 8020480:	e74f      	b.n	8020322 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x132>
 8020482:	4605      	mov	r5, r0
 8020484:	f04f 0a00 	mov.w	sl, #0
 8020488:	46b8      	mov	r8, r7
 802048a:	4a0f      	ldr	r2, [pc, #60]	; (80204c8 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x2d8>)
 802048c:	e749      	b.n	8020322 <_ZN14CVisualMeasure12UpdateDs1820Ev+0x132>
 802048e:	465d      	mov	r5, fp
 8020490:	f04f 0a00 	mov.w	sl, #0
 8020494:	4698      	mov	r8, r3
 8020496:	e7f8      	b.n	802048a <_ZN14CVisualMeasure12UpdateDs1820Ev+0x29a>
 8020498:	08020f61 	.word	0x08020f61
 802049c:	08020f66 	.word	0x08020f66
 80204a0:	08020e5c 	.word	0x08020e5c
 80204a4:	08020f32 	.word	0x08020f32
 80204a8:	08020f6b 	.word	0x08020f6b
 80204ac:	08020f4c 	.word	0x08020f4c
 80204b0:	08020f85 	.word	0x08020f85
 80204b4:	08020f9e 	.word	0x08020f9e
 80204b8:	08020fa3 	.word	0x08020fa3
 80204bc:	08020f3a 	.word	0x08020f3a
 80204c0:	08020f43 	.word	0x08020f43
 80204c4:	08020f59 	.word	0x08020f59
 80204c8:	08020f50 	.word	0x08020f50

080204cc <_ZN14CVisualMeasure7OnPaintEv>:
 80204cc:	b530      	push	{r4, r5, lr}
 80204ce:	2400      	movs	r4, #0
 80204d0:	b089      	sub	sp, #36	; 0x24
 80204d2:	4b29      	ldr	r3, [pc, #164]	; (8020578 <_ZN14CVisualMeasure7OnPaintEv+0xac>)
 80204d4:	4605      	mov	r5, r0
 80204d6:	f241 0282 	movw	r2, #4226	; 0x1082
 80204da:	a802      	add	r0, sp, #8
 80204dc:	f244 2108 	movw	r1, #16904	; 0x4208
 80204e0:	e9cd 4302 	strd	r4, r3, [sp, #8]
 80204e4:	f000 fe14 	bl	8021110 <_ZN3GUI10BackgroundER5CRectmm@plt+0x4>
 80204e8:	4b24      	ldr	r3, [pc, #144]	; (802057c <_ZN14CVisualMeasure7OnPaintEv+0xb0>)
 80204ea:	a804      	add	r0, sp, #16
 80204ec:	f244 2208 	movw	r2, #16904	; 0x4208
 80204f0:	f24b 2108 	movw	r1, #45576	; 0xb208
 80204f4:	9305      	str	r3, [sp, #20]
 80204f6:	9404      	str	r4, [sp, #16]
 80204f8:	f000 fe0a 	bl	8021110 <_ZN3GUI10BackgroundER5CRectmm@plt+0x4>
 80204fc:	4a20      	ldr	r2, [pc, #128]	; (8020580 <_ZN14CVisualMeasure7OnPaintEv+0xb4>)
 80204fe:	4b21      	ldr	r3, [pc, #132]	; (8020584 <_ZN14CVisualMeasure7OnPaintEv+0xb8>)
 8020500:	a806      	add	r0, sp, #24
 8020502:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8020506:	e9cd 2306 	strd	r2, r3, [sp, #24]
 802050a:	f000 fe31 	bl	8021170 <_ZN3GUI6WindowERK5CRectt@plt+0x4>
 802050e:	4b1e      	ldr	r3, [pc, #120]	; (8020588 <_ZN14CVisualMeasure7OnPaintEv+0xbc>)
 8020510:	4622      	mov	r2, r4
 8020512:	9300      	str	r3, [sp, #0]
 8020514:	4621      	mov	r1, r4
 8020516:	f24b 5396 	movw	r3, #46486	; 0xb596
 802051a:	4620      	mov	r0, r4
 802051c:	f000 fe00 	bl	8021120 <_ZN4BIOS3LCD5PrintEiittPKc@plt+0x4>
 8020520:	4b1a      	ldr	r3, [pc, #104]	; (802058c <_ZN14CVisualMeasure7OnPaintEv+0xc0>)
 8020522:	4621      	mov	r1, r4
 8020524:	9300      	str	r3, [sp, #0]
 8020526:	f24b 5296 	movw	r2, #46486	; 0xb596
 802052a:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802052e:	f000 fe0f 	bl	8021150 <_ZN4BIOS3LCD4DrawEiittPKc@plt+0x4>
 8020532:	2202      	movs	r2, #2
 8020534:	f9bd 001c 	ldrsh.w	r0, [sp, #28]
 8020538:	f9bd 3018 	ldrsh.w	r3, [sp, #24]
 802053c:	fb90 f0f2 	sdiv	r0, r0, r2
 8020540:	fb93 f3f2 	sdiv	r3, r3, r2
 8020544:	f9bd 101a 	ldrsh.w	r1, [sp, #26]
 8020548:	4418      	add	r0, r3
 802054a:	4b11      	ldr	r3, [pc, #68]	; (8020590 <_ZN14CVisualMeasure7OnPaintEv+0xc4>)
 802054c:	4622      	mov	r2, r4
 802054e:	9300      	str	r3, [sp, #0]
 8020550:	3102      	adds	r1, #2
 8020552:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020556:	3854      	subs	r0, #84	; 0x54
 8020558:	f000 fde2 	bl	8021120 <_ZN4BIOS3LCD5PrintEiittPKc@plt+0x4>
 802055c:	f9bd 3018 	ldrsh.w	r3, [sp, #24]
 8020560:	4628      	mov	r0, r5
 8020562:	3308      	adds	r3, #8
 8020564:	616b      	str	r3, [r5, #20]
 8020566:	f9bd 301a 	ldrsh.w	r3, [sp, #26]
 802056a:	3302      	adds	r3, #2
 802056c:	61ab      	str	r3, [r5, #24]
 802056e:	f7ff fe3f 	bl	80201f0 <_ZN14CVisualMeasure12UpdateDs1820Ev>
 8020572:	b009      	add	sp, #36	; 0x24
 8020574:	bd30      	pop	{r4, r5, pc}
 8020576:	bf00      	nop
 8020578:	00f00140 	.word	0x00f00140
 802057c:	000e0140 	.word	0x000e0140
 8020580:	00420014 	.word	0x00420014
 8020584:	00be012c 	.word	0x00be012c
 8020588:	08020fba 	.word	0x08020fba
 802058c:	08020ec0 	.word	0x08020ec0
 8020590:	08020fc7 	.word	0x08020fc7

08020594 <__aeabi_drsub>:
 8020594:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000
 8020598:	e002      	b.n	80205a0 <__adddf3>
 802059a:	bf00      	nop

0802059c <__aeabi_dsub>:
 802059c:	f083 4300 	eor.w	r3, r3, #2147483648	; 0x80000000

080205a0 <__adddf3>:
 80205a0:	b530      	push	{r4, r5, lr}
 80205a2:	ea4f 0441 	mov.w	r4, r1, lsl #1
 80205a6:	ea4f 0543 	mov.w	r5, r3, lsl #1
 80205aa:	ea94 0f05 	teq	r4, r5
 80205ae:	bf08      	it	eq
 80205b0:	ea90 0f02 	teqeq	r0, r2
 80205b4:	bf1f      	itttt	ne
 80205b6:	ea54 0c00 	orrsne.w	ip, r4, r0
 80205ba:	ea55 0c02 	orrsne.w	ip, r5, r2
 80205be:	ea7f 5c64 	mvnsne.w	ip, r4, asr #21
 80205c2:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 80205c6:	f000 80e2 	beq.w	802078e <__adddf3+0x1ee>
 80205ca:	ea4f 5454 	mov.w	r4, r4, lsr #21
 80205ce:	ebd4 5555 	rsbs	r5, r4, r5, lsr #21
 80205d2:	bfb8      	it	lt
 80205d4:	426d      	neglt	r5, r5
 80205d6:	dd0c      	ble.n	80205f2 <__adddf3+0x52>
 80205d8:	442c      	add	r4, r5
 80205da:	ea80 0202 	eor.w	r2, r0, r2
 80205de:	ea81 0303 	eor.w	r3, r1, r3
 80205e2:	ea82 0000 	eor.w	r0, r2, r0
 80205e6:	ea83 0101 	eor.w	r1, r3, r1
 80205ea:	ea80 0202 	eor.w	r2, r0, r2
 80205ee:	ea81 0303 	eor.w	r3, r1, r3
 80205f2:	2d36      	cmp	r5, #54	; 0x36
 80205f4:	bf88      	it	hi
 80205f6:	bd30      	pophi	{r4, r5, pc}
 80205f8:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 80205fc:	ea4f 3101 	mov.w	r1, r1, lsl #12
 8020600:	f44f 1c80 	mov.w	ip, #1048576	; 0x100000
 8020604:	ea4c 3111 	orr.w	r1, ip, r1, lsr #12
 8020608:	d002      	beq.n	8020610 <__adddf3+0x70>
 802060a:	4240      	negs	r0, r0
 802060c:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 8020610:	f013 4f00 	tst.w	r3, #2147483648	; 0x80000000
 8020614:	ea4f 3303 	mov.w	r3, r3, lsl #12
 8020618:	ea4c 3313 	orr.w	r3, ip, r3, lsr #12
 802061c:	d002      	beq.n	8020624 <__adddf3+0x84>
 802061e:	4252      	negs	r2, r2
 8020620:	eb63 0343 	sbc.w	r3, r3, r3, lsl #1
 8020624:	ea94 0f05 	teq	r4, r5
 8020628:	f000 80a7 	beq.w	802077a <__adddf3+0x1da>
 802062c:	f1a4 0401 	sub.w	r4, r4, #1
 8020630:	f1d5 0e20 	rsbs	lr, r5, #32
 8020634:	db0d      	blt.n	8020652 <__adddf3+0xb2>
 8020636:	fa02 fc0e 	lsl.w	ip, r2, lr
 802063a:	fa22 f205 	lsr.w	r2, r2, r5
 802063e:	1880      	adds	r0, r0, r2
 8020640:	f141 0100 	adc.w	r1, r1, #0
 8020644:	fa03 f20e 	lsl.w	r2, r3, lr
 8020648:	1880      	adds	r0, r0, r2
 802064a:	fa43 f305 	asr.w	r3, r3, r5
 802064e:	4159      	adcs	r1, r3
 8020650:	e00e      	b.n	8020670 <__adddf3+0xd0>
 8020652:	f1a5 0520 	sub.w	r5, r5, #32
 8020656:	f10e 0e20 	add.w	lr, lr, #32
 802065a:	2a01      	cmp	r2, #1
 802065c:	fa03 fc0e 	lsl.w	ip, r3, lr
 8020660:	bf28      	it	cs
 8020662:	f04c 0c02 	orrcs.w	ip, ip, #2
 8020666:	fa43 f305 	asr.w	r3, r3, r5
 802066a:	18c0      	adds	r0, r0, r3
 802066c:	eb51 71e3 	adcs.w	r1, r1, r3, asr #31
 8020670:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 8020674:	d507      	bpl.n	8020686 <__adddf3+0xe6>
 8020676:	f04f 0e00 	mov.w	lr, #0
 802067a:	f1dc 0c00 	rsbs	ip, ip, #0
 802067e:	eb7e 0000 	sbcs.w	r0, lr, r0
 8020682:	eb6e 0101 	sbc.w	r1, lr, r1
 8020686:	f5b1 1f80 	cmp.w	r1, #1048576	; 0x100000
 802068a:	d31b      	bcc.n	80206c4 <__adddf3+0x124>
 802068c:	f5b1 1f00 	cmp.w	r1, #2097152	; 0x200000
 8020690:	d30c      	bcc.n	80206ac <__adddf3+0x10c>
 8020692:	0849      	lsrs	r1, r1, #1
 8020694:	ea5f 0030 	movs.w	r0, r0, rrx
 8020698:	ea4f 0c3c 	mov.w	ip, ip, rrx
 802069c:	f104 0401 	add.w	r4, r4, #1
 80206a0:	ea4f 5244 	mov.w	r2, r4, lsl #21
 80206a4:	f512 0f80 	cmn.w	r2, #4194304	; 0x400000
 80206a8:	f080 809a 	bcs.w	80207e0 <__adddf3+0x240>
 80206ac:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 80206b0:	bf08      	it	eq
 80206b2:	ea5f 0c50 	movseq.w	ip, r0, lsr #1
 80206b6:	f150 0000 	adcs.w	r0, r0, #0
 80206ba:	eb41 5104 	adc.w	r1, r1, r4, lsl #20
 80206be:	ea41 0105 	orr.w	r1, r1, r5
 80206c2:	bd30      	pop	{r4, r5, pc}
 80206c4:	ea5f 0c4c 	movs.w	ip, ip, lsl #1
 80206c8:	4140      	adcs	r0, r0
 80206ca:	eb41 0101 	adc.w	r1, r1, r1
 80206ce:	f411 1f80 	tst.w	r1, #1048576	; 0x100000
 80206d2:	f1a4 0401 	sub.w	r4, r4, #1
 80206d6:	d1e9      	bne.n	80206ac <__adddf3+0x10c>
 80206d8:	f091 0f00 	teq	r1, #0
 80206dc:	bf04      	itt	eq
 80206de:	4601      	moveq	r1, r0
 80206e0:	2000      	moveq	r0, #0
 80206e2:	fab1 f381 	clz	r3, r1
 80206e6:	bf08      	it	eq
 80206e8:	3320      	addeq	r3, #32
 80206ea:	f1a3 030b 	sub.w	r3, r3, #11
 80206ee:	f1b3 0220 	subs.w	r2, r3, #32
 80206f2:	da0c      	bge.n	802070e <__adddf3+0x16e>
 80206f4:	320c      	adds	r2, #12
 80206f6:	dd08      	ble.n	802070a <__adddf3+0x16a>
 80206f8:	f102 0c14 	add.w	ip, r2, #20
 80206fc:	f1c2 020c 	rsb	r2, r2, #12
 8020700:	fa01 f00c 	lsl.w	r0, r1, ip
 8020704:	fa21 f102 	lsr.w	r1, r1, r2
 8020708:	e00c      	b.n	8020724 <__adddf3+0x184>
 802070a:	f102 0214 	add.w	r2, r2, #20
 802070e:	bfd8      	it	le
 8020710:	f1c2 0c20 	rsble	ip, r2, #32
 8020714:	fa01 f102 	lsl.w	r1, r1, r2
 8020718:	fa20 fc0c 	lsr.w	ip, r0, ip
 802071c:	bfdc      	itt	le
 802071e:	ea41 010c 	orrle.w	r1, r1, ip
 8020722:	4090      	lslle	r0, r2
 8020724:	1ae4      	subs	r4, r4, r3
 8020726:	bfa2      	ittt	ge
 8020728:	eb01 5104 	addge.w	r1, r1, r4, lsl #20
 802072c:	4329      	orrge	r1, r5
 802072e:	bd30      	popge	{r4, r5, pc}
 8020730:	ea6f 0404 	mvn.w	r4, r4
 8020734:	3c1f      	subs	r4, #31
 8020736:	da1c      	bge.n	8020772 <__adddf3+0x1d2>
 8020738:	340c      	adds	r4, #12
 802073a:	dc0e      	bgt.n	802075a <__adddf3+0x1ba>
 802073c:	f104 0414 	add.w	r4, r4, #20
 8020740:	f1c4 0220 	rsb	r2, r4, #32
 8020744:	fa20 f004 	lsr.w	r0, r0, r4
 8020748:	fa01 f302 	lsl.w	r3, r1, r2
 802074c:	ea40 0003 	orr.w	r0, r0, r3
 8020750:	fa21 f304 	lsr.w	r3, r1, r4
 8020754:	ea45 0103 	orr.w	r1, r5, r3
 8020758:	bd30      	pop	{r4, r5, pc}
 802075a:	f1c4 040c 	rsb	r4, r4, #12
 802075e:	f1c4 0220 	rsb	r2, r4, #32
 8020762:	fa20 f002 	lsr.w	r0, r0, r2
 8020766:	fa01 f304 	lsl.w	r3, r1, r4
 802076a:	ea40 0003 	orr.w	r0, r0, r3
 802076e:	4629      	mov	r1, r5
 8020770:	bd30      	pop	{r4, r5, pc}
 8020772:	fa21 f004 	lsr.w	r0, r1, r4
 8020776:	4629      	mov	r1, r5
 8020778:	bd30      	pop	{r4, r5, pc}
 802077a:	f094 0f00 	teq	r4, #0
 802077e:	f483 1380 	eor.w	r3, r3, #1048576	; 0x100000
 8020782:	bf06      	itte	eq
 8020784:	f481 1180 	eoreq.w	r1, r1, #1048576	; 0x100000
 8020788:	3401      	addeq	r4, #1
 802078a:	3d01      	subne	r5, #1
 802078c:	e74e      	b.n	802062c <__adddf3+0x8c>
 802078e:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 8020792:	bf18      	it	ne
 8020794:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 8020798:	d029      	beq.n	80207ee <__adddf3+0x24e>
 802079a:	ea94 0f05 	teq	r4, r5
 802079e:	bf08      	it	eq
 80207a0:	ea90 0f02 	teqeq	r0, r2
 80207a4:	d005      	beq.n	80207b2 <__adddf3+0x212>
 80207a6:	ea54 0c00 	orrs.w	ip, r4, r0
 80207aa:	bf04      	itt	eq
 80207ac:	4619      	moveq	r1, r3
 80207ae:	4610      	moveq	r0, r2
 80207b0:	bd30      	pop	{r4, r5, pc}
 80207b2:	ea91 0f03 	teq	r1, r3
 80207b6:	bf1e      	ittt	ne
 80207b8:	2100      	movne	r1, #0
 80207ba:	2000      	movne	r0, #0
 80207bc:	bd30      	popne	{r4, r5, pc}
 80207be:	ea5f 5c54 	movs.w	ip, r4, lsr #21
 80207c2:	d105      	bne.n	80207d0 <__adddf3+0x230>
 80207c4:	0040      	lsls	r0, r0, #1
 80207c6:	4149      	adcs	r1, r1
 80207c8:	bf28      	it	cs
 80207ca:	f041 4100 	orrcs.w	r1, r1, #2147483648	; 0x80000000
 80207ce:	bd30      	pop	{r4, r5, pc}
 80207d0:	f514 0480 	adds.w	r4, r4, #4194304	; 0x400000
 80207d4:	bf3c      	itt	cc
 80207d6:	f501 1180 	addcc.w	r1, r1, #1048576	; 0x100000
 80207da:	bd30      	popcc	{r4, r5, pc}
 80207dc:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 80207e0:	f045 41fe 	orr.w	r1, r5, #2130706432	; 0x7f000000
 80207e4:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
 80207e8:	f04f 0000 	mov.w	r0, #0
 80207ec:	bd30      	pop	{r4, r5, pc}
 80207ee:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 80207f2:	bf1a      	itte	ne
 80207f4:	4619      	movne	r1, r3
 80207f6:	4610      	movne	r0, r2
 80207f8:	ea7f 5c65 	mvnseq.w	ip, r5, asr #21
 80207fc:	bf1c      	itt	ne
 80207fe:	460b      	movne	r3, r1
 8020800:	4602      	movne	r2, r0
 8020802:	ea50 3401 	orrs.w	r4, r0, r1, lsl #12
 8020806:	bf06      	itte	eq
 8020808:	ea52 3503 	orrseq.w	r5, r2, r3, lsl #12
 802080c:	ea91 0f03 	teqeq	r1, r3
 8020810:	f441 2100 	orrne.w	r1, r1, #524288	; 0x80000
 8020814:	bd30      	pop	{r4, r5, pc}
 8020816:	bf00      	nop

08020818 <__aeabi_ui2d>:
 8020818:	f090 0f00 	teq	r0, #0
 802081c:	bf04      	itt	eq
 802081e:	2100      	moveq	r1, #0
 8020820:	4770      	bxeq	lr
 8020822:	b530      	push	{r4, r5, lr}
 8020824:	f44f 6480 	mov.w	r4, #1024	; 0x400
 8020828:	f104 0432 	add.w	r4, r4, #50	; 0x32
 802082c:	f04f 0500 	mov.w	r5, #0
 8020830:	f04f 0100 	mov.w	r1, #0
 8020834:	e750      	b.n	80206d8 <__adddf3+0x138>
 8020836:	bf00      	nop

08020838 <__aeabi_i2d>:
 8020838:	f090 0f00 	teq	r0, #0
 802083c:	bf04      	itt	eq
 802083e:	2100      	moveq	r1, #0
 8020840:	4770      	bxeq	lr
 8020842:	b530      	push	{r4, r5, lr}
 8020844:	f44f 6480 	mov.w	r4, #1024	; 0x400
 8020848:	f104 0432 	add.w	r4, r4, #50	; 0x32
 802084c:	f010 4500 	ands.w	r5, r0, #2147483648	; 0x80000000
 8020850:	bf48      	it	mi
 8020852:	4240      	negmi	r0, r0
 8020854:	f04f 0100 	mov.w	r1, #0
 8020858:	e73e      	b.n	80206d8 <__adddf3+0x138>
 802085a:	bf00      	nop

0802085c <__aeabi_f2d>:
 802085c:	0042      	lsls	r2, r0, #1
 802085e:	ea4f 01e2 	mov.w	r1, r2, asr #3
 8020862:	ea4f 0131 	mov.w	r1, r1, rrx
 8020866:	ea4f 7002 	mov.w	r0, r2, lsl #28
 802086a:	bf1f      	itttt	ne
 802086c:	f012 437f 	andsne.w	r3, r2, #4278190080	; 0xff000000
 8020870:	f093 4f7f 	teqne	r3, #4278190080	; 0xff000000
 8020874:	f081 5160 	eorne.w	r1, r1, #939524096	; 0x38000000
 8020878:	4770      	bxne	lr
 802087a:	f032 427f 	bics.w	r2, r2, #4278190080	; 0xff000000
 802087e:	bf08      	it	eq
 8020880:	4770      	bxeq	lr
 8020882:	f093 4f7f 	teq	r3, #4278190080	; 0xff000000
 8020886:	bf04      	itt	eq
 8020888:	f441 2100 	orreq.w	r1, r1, #524288	; 0x80000
 802088c:	4770      	bxeq	lr
 802088e:	b530      	push	{r4, r5, lr}
 8020890:	f44f 7460 	mov.w	r4, #896	; 0x380
 8020894:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 8020898:	f021 4100 	bic.w	r1, r1, #2147483648	; 0x80000000
 802089c:	e71c      	b.n	80206d8 <__adddf3+0x138>
 802089e:	bf00      	nop

080208a0 <__aeabi_ul2d>:
 80208a0:	ea50 0201 	orrs.w	r2, r0, r1
 80208a4:	bf08      	it	eq
 80208a6:	4770      	bxeq	lr
 80208a8:	b530      	push	{r4, r5, lr}
 80208aa:	f04f 0500 	mov.w	r5, #0
 80208ae:	e00a      	b.n	80208c6 <__aeabi_l2d+0x16>

080208b0 <__aeabi_l2d>:
 80208b0:	ea50 0201 	orrs.w	r2, r0, r1
 80208b4:	bf08      	it	eq
 80208b6:	4770      	bxeq	lr
 80208b8:	b530      	push	{r4, r5, lr}
 80208ba:	f011 4500 	ands.w	r5, r1, #2147483648	; 0x80000000
 80208be:	d502      	bpl.n	80208c6 <__aeabi_l2d+0x16>
 80208c0:	4240      	negs	r0, r0
 80208c2:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 80208c6:	f44f 6480 	mov.w	r4, #1024	; 0x400
 80208ca:	f104 0432 	add.w	r4, r4, #50	; 0x32
 80208ce:	ea5f 5c91 	movs.w	ip, r1, lsr #22
 80208d2:	f43f aed8 	beq.w	8020686 <__adddf3+0xe6>
 80208d6:	f04f 0203 	mov.w	r2, #3
 80208da:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 80208de:	bf18      	it	ne
 80208e0:	3203      	addne	r2, #3
 80208e2:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 80208e6:	bf18      	it	ne
 80208e8:	3203      	addne	r2, #3
 80208ea:	eb02 02dc 	add.w	r2, r2, ip, lsr #3
 80208ee:	f1c2 0320 	rsb	r3, r2, #32
 80208f2:	fa00 fc03 	lsl.w	ip, r0, r3
 80208f6:	fa20 f002 	lsr.w	r0, r0, r2
 80208fa:	fa01 fe03 	lsl.w	lr, r1, r3
 80208fe:	ea40 000e 	orr.w	r0, r0, lr
 8020902:	fa21 f102 	lsr.w	r1, r1, r2
 8020906:	4414      	add	r4, r2
 8020908:	e6bd      	b.n	8020686 <__adddf3+0xe6>
 802090a:	bf00      	nop

0802090c <__aeabi_frsub>:
 802090c:	f080 4000 	eor.w	r0, r0, #2147483648	; 0x80000000
 8020910:	e002      	b.n	8020918 <__addsf3>
 8020912:	bf00      	nop

08020914 <__aeabi_fsub>:
 8020914:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000

08020918 <__addsf3>:
 8020918:	0042      	lsls	r2, r0, #1
 802091a:	bf1f      	itttt	ne
 802091c:	ea5f 0341 	movsne.w	r3, r1, lsl #1
 8020920:	ea92 0f03 	teqne	r2, r3
 8020924:	ea7f 6c22 	mvnsne.w	ip, r2, asr #24
 8020928:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 802092c:	d06a      	beq.n	8020a04 <__addsf3+0xec>
 802092e:	ea4f 6212 	mov.w	r2, r2, lsr #24
 8020932:	ebd2 6313 	rsbs	r3, r2, r3, lsr #24
 8020936:	bfc1      	itttt	gt
 8020938:	18d2      	addgt	r2, r2, r3
 802093a:	4041      	eorgt	r1, r0
 802093c:	4048      	eorgt	r0, r1
 802093e:	4041      	eorgt	r1, r0
 8020940:	bfb8      	it	lt
 8020942:	425b      	neglt	r3, r3
 8020944:	2b19      	cmp	r3, #25
 8020946:	bf88      	it	hi
 8020948:	4770      	bxhi	lr
 802094a:	f010 4f00 	tst.w	r0, #2147483648	; 0x80000000
 802094e:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8020952:	f020 407f 	bic.w	r0, r0, #4278190080	; 0xff000000
 8020956:	bf18      	it	ne
 8020958:	4240      	negne	r0, r0
 802095a:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 802095e:	f441 0100 	orr.w	r1, r1, #8388608	; 0x800000
 8020962:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 8020966:	bf18      	it	ne
 8020968:	4249      	negne	r1, r1
 802096a:	ea92 0f03 	teq	r2, r3
 802096e:	d03f      	beq.n	80209f0 <__addsf3+0xd8>
 8020970:	f1a2 0201 	sub.w	r2, r2, #1
 8020974:	fa41 fc03 	asr.w	ip, r1, r3
 8020978:	eb10 000c 	adds.w	r0, r0, ip
 802097c:	f1c3 0320 	rsb	r3, r3, #32
 8020980:	fa01 f103 	lsl.w	r1, r1, r3
 8020984:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 8020988:	d502      	bpl.n	8020990 <__addsf3+0x78>
 802098a:	4249      	negs	r1, r1
 802098c:	eb60 0040 	sbc.w	r0, r0, r0, lsl #1
 8020990:	f5b0 0f00 	cmp.w	r0, #8388608	; 0x800000
 8020994:	d313      	bcc.n	80209be <__addsf3+0xa6>
 8020996:	f1b0 7f80 	cmp.w	r0, #16777216	; 0x1000000
 802099a:	d306      	bcc.n	80209aa <__addsf3+0x92>
 802099c:	0840      	lsrs	r0, r0, #1
 802099e:	ea4f 0131 	mov.w	r1, r1, rrx
 80209a2:	f102 0201 	add.w	r2, r2, #1
 80209a6:	2afe      	cmp	r2, #254	; 0xfe
 80209a8:	d251      	bcs.n	8020a4e <__addsf3+0x136>
 80209aa:	f1b1 4f00 	cmp.w	r1, #2147483648	; 0x80000000
 80209ae:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 80209b2:	bf08      	it	eq
 80209b4:	f020 0001 	biceq.w	r0, r0, #1
 80209b8:	ea40 0003 	orr.w	r0, r0, r3
 80209bc:	4770      	bx	lr
 80209be:	0049      	lsls	r1, r1, #1
 80209c0:	eb40 0000 	adc.w	r0, r0, r0
 80209c4:	f410 0f00 	tst.w	r0, #8388608	; 0x800000
 80209c8:	f1a2 0201 	sub.w	r2, r2, #1
 80209cc:	d1ed      	bne.n	80209aa <__addsf3+0x92>
 80209ce:	fab0 fc80 	clz	ip, r0
 80209d2:	f1ac 0c08 	sub.w	ip, ip, #8
 80209d6:	ebb2 020c 	subs.w	r2, r2, ip
 80209da:	fa00 f00c 	lsl.w	r0, r0, ip
 80209de:	bfaa      	itet	ge
 80209e0:	eb00 50c2 	addge.w	r0, r0, r2, lsl #23
 80209e4:	4252      	neglt	r2, r2
 80209e6:	4318      	orrge	r0, r3
 80209e8:	bfbc      	itt	lt
 80209ea:	40d0      	lsrlt	r0, r2
 80209ec:	4318      	orrlt	r0, r3
 80209ee:	4770      	bx	lr
 80209f0:	f092 0f00 	teq	r2, #0
 80209f4:	f481 0100 	eor.w	r1, r1, #8388608	; 0x800000
 80209f8:	bf06      	itte	eq
 80209fa:	f480 0000 	eoreq.w	r0, r0, #8388608	; 0x800000
 80209fe:	3201      	addeq	r2, #1
 8020a00:	3b01      	subne	r3, #1
 8020a02:	e7b5      	b.n	8020970 <__addsf3+0x58>
 8020a04:	ea4f 0341 	mov.w	r3, r1, lsl #1
 8020a08:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 8020a0c:	bf18      	it	ne
 8020a0e:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 8020a12:	d021      	beq.n	8020a58 <__addsf3+0x140>
 8020a14:	ea92 0f03 	teq	r2, r3
 8020a18:	d004      	beq.n	8020a24 <__addsf3+0x10c>
 8020a1a:	f092 0f00 	teq	r2, #0
 8020a1e:	bf08      	it	eq
 8020a20:	4608      	moveq	r0, r1
 8020a22:	4770      	bx	lr
 8020a24:	ea90 0f01 	teq	r0, r1
 8020a28:	bf1c      	itt	ne
 8020a2a:	2000      	movne	r0, #0
 8020a2c:	4770      	bxne	lr
 8020a2e:	f012 4f7f 	tst.w	r2, #4278190080	; 0xff000000
 8020a32:	d104      	bne.n	8020a3e <__addsf3+0x126>
 8020a34:	0040      	lsls	r0, r0, #1
 8020a36:	bf28      	it	cs
 8020a38:	f040 4000 	orrcs.w	r0, r0, #2147483648	; 0x80000000
 8020a3c:	4770      	bx	lr
 8020a3e:	f112 7200 	adds.w	r2, r2, #33554432	; 0x2000000
 8020a42:	bf3c      	itt	cc
 8020a44:	f500 0000 	addcc.w	r0, r0, #8388608	; 0x800000
 8020a48:	4770      	bxcc	lr
 8020a4a:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 8020a4e:	f043 40fe 	orr.w	r0, r3, #2130706432	; 0x7f000000
 8020a52:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8020a56:	4770      	bx	lr
 8020a58:	ea7f 6222 	mvns.w	r2, r2, asr #24
 8020a5c:	bf16      	itet	ne
 8020a5e:	4608      	movne	r0, r1
 8020a60:	ea7f 6323 	mvnseq.w	r3, r3, asr #24
 8020a64:	4601      	movne	r1, r0
 8020a66:	0242      	lsls	r2, r0, #9
 8020a68:	bf06      	itte	eq
 8020a6a:	ea5f 2341 	movseq.w	r3, r1, lsl #9
 8020a6e:	ea90 0f01 	teqeq	r0, r1
 8020a72:	f440 0080 	orrne.w	r0, r0, #4194304	; 0x400000
 8020a76:	4770      	bx	lr

08020a78 <__aeabi_ui2f>:
 8020a78:	f04f 0300 	mov.w	r3, #0
 8020a7c:	e004      	b.n	8020a88 <__aeabi_i2f+0x8>
 8020a7e:	bf00      	nop

08020a80 <__aeabi_i2f>:
 8020a80:	f010 4300 	ands.w	r3, r0, #2147483648	; 0x80000000
 8020a84:	bf48      	it	mi
 8020a86:	4240      	negmi	r0, r0
 8020a88:	ea5f 0c00 	movs.w	ip, r0
 8020a8c:	bf08      	it	eq
 8020a8e:	4770      	bxeq	lr
 8020a90:	f043 4396 	orr.w	r3, r3, #1258291200	; 0x4b000000
 8020a94:	4601      	mov	r1, r0
 8020a96:	f04f 0000 	mov.w	r0, #0
 8020a9a:	e01c      	b.n	8020ad6 <__aeabi_l2f+0x2a>

08020a9c <__aeabi_ul2f>:
 8020a9c:	ea50 0201 	orrs.w	r2, r0, r1
 8020aa0:	bf08      	it	eq
 8020aa2:	4770      	bxeq	lr
 8020aa4:	f04f 0300 	mov.w	r3, #0
 8020aa8:	e00a      	b.n	8020ac0 <__aeabi_l2f+0x14>
 8020aaa:	bf00      	nop

08020aac <__aeabi_l2f>:
 8020aac:	ea50 0201 	orrs.w	r2, r0, r1
 8020ab0:	bf08      	it	eq
 8020ab2:	4770      	bxeq	lr
 8020ab4:	f011 4300 	ands.w	r3, r1, #2147483648	; 0x80000000
 8020ab8:	d502      	bpl.n	8020ac0 <__aeabi_l2f+0x14>
 8020aba:	4240      	negs	r0, r0
 8020abc:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 8020ac0:	ea5f 0c01 	movs.w	ip, r1
 8020ac4:	bf02      	ittt	eq
 8020ac6:	4684      	moveq	ip, r0
 8020ac8:	4601      	moveq	r1, r0
 8020aca:	2000      	moveq	r0, #0
 8020acc:	f043 43b6 	orr.w	r3, r3, #1526726656	; 0x5b000000
 8020ad0:	bf08      	it	eq
 8020ad2:	f1a3 5380 	subeq.w	r3, r3, #268435456	; 0x10000000
 8020ad6:	f5a3 0300 	sub.w	r3, r3, #8388608	; 0x800000
 8020ada:	fabc f28c 	clz	r2, ip
 8020ade:	3a08      	subs	r2, #8
 8020ae0:	eba3 53c2 	sub.w	r3, r3, r2, lsl #23
 8020ae4:	db10      	blt.n	8020b08 <__aeabi_l2f+0x5c>
 8020ae6:	fa01 fc02 	lsl.w	ip, r1, r2
 8020aea:	4463      	add	r3, ip
 8020aec:	fa00 fc02 	lsl.w	ip, r0, r2
 8020af0:	f1c2 0220 	rsb	r2, r2, #32
 8020af4:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 8020af8:	fa20 f202 	lsr.w	r2, r0, r2
 8020afc:	eb43 0002 	adc.w	r0, r3, r2
 8020b00:	bf08      	it	eq
 8020b02:	f020 0001 	biceq.w	r0, r0, #1
 8020b06:	4770      	bx	lr
 8020b08:	f102 0220 	add.w	r2, r2, #32
 8020b0c:	fa01 fc02 	lsl.w	ip, r1, r2
 8020b10:	f1c2 0220 	rsb	r2, r2, #32
 8020b14:	ea50 004c 	orrs.w	r0, r0, ip, lsl #1
 8020b18:	fa21 f202 	lsr.w	r2, r1, r2
 8020b1c:	eb43 0002 	adc.w	r0, r3, r2
 8020b20:	bf08      	it	eq
 8020b22:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 8020b26:	4770      	bx	lr

08020b28 <__aeabi_fmul>:
 8020b28:	f04f 0cff 	mov.w	ip, #255	; 0xff
 8020b2c:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 8020b30:	bf1e      	ittt	ne
 8020b32:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 8020b36:	ea92 0f0c 	teqne	r2, ip
 8020b3a:	ea93 0f0c 	teqne	r3, ip
 8020b3e:	d06f      	beq.n	8020c20 <__aeabi_fmul+0xf8>
 8020b40:	441a      	add	r2, r3
 8020b42:	ea80 0c01 	eor.w	ip, r0, r1
 8020b46:	0240      	lsls	r0, r0, #9
 8020b48:	bf18      	it	ne
 8020b4a:	ea5f 2141 	movsne.w	r1, r1, lsl #9
 8020b4e:	d01e      	beq.n	8020b8e <__aeabi_fmul+0x66>
 8020b50:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
 8020b54:	ea43 1050 	orr.w	r0, r3, r0, lsr #5
 8020b58:	ea43 1151 	orr.w	r1, r3, r1, lsr #5
 8020b5c:	fba0 3101 	umull	r3, r1, r0, r1
 8020b60:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 8020b64:	f5b1 0f00 	cmp.w	r1, #8388608	; 0x800000
 8020b68:	bf3e      	ittt	cc
 8020b6a:	0049      	lslcc	r1, r1, #1
 8020b6c:	ea41 71d3 	orrcc.w	r1, r1, r3, lsr #31
 8020b70:	005b      	lslcc	r3, r3, #1
 8020b72:	ea40 0001 	orr.w	r0, r0, r1
 8020b76:	f162 027f 	sbc.w	r2, r2, #127	; 0x7f
 8020b7a:	2afd      	cmp	r2, #253	; 0xfd
 8020b7c:	d81d      	bhi.n	8020bba <__aeabi_fmul+0x92>
 8020b7e:	f1b3 4f00 	cmp.w	r3, #2147483648	; 0x80000000
 8020b82:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 8020b86:	bf08      	it	eq
 8020b88:	f020 0001 	biceq.w	r0, r0, #1
 8020b8c:	4770      	bx	lr
 8020b8e:	f090 0f00 	teq	r0, #0
 8020b92:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 8020b96:	bf08      	it	eq
 8020b98:	0249      	lsleq	r1, r1, #9
 8020b9a:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 8020b9e:	ea40 2051 	orr.w	r0, r0, r1, lsr #9
 8020ba2:	3a7f      	subs	r2, #127	; 0x7f
 8020ba4:	bfc2      	ittt	gt
 8020ba6:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 8020baa:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 8020bae:	4770      	bxgt	lr
 8020bb0:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8020bb4:	f04f 0300 	mov.w	r3, #0
 8020bb8:	3a01      	subs	r2, #1
 8020bba:	dc5d      	bgt.n	8020c78 <__aeabi_fmul+0x150>
 8020bbc:	f112 0f19 	cmn.w	r2, #25
 8020bc0:	bfdc      	itt	le
 8020bc2:	f000 4000 	andle.w	r0, r0, #2147483648	; 0x80000000
 8020bc6:	4770      	bxle	lr
 8020bc8:	f1c2 0200 	rsb	r2, r2, #0
 8020bcc:	0041      	lsls	r1, r0, #1
 8020bce:	fa21 f102 	lsr.w	r1, r1, r2
 8020bd2:	f1c2 0220 	rsb	r2, r2, #32
 8020bd6:	fa00 fc02 	lsl.w	ip, r0, r2
 8020bda:	ea5f 0031 	movs.w	r0, r1, rrx
 8020bde:	f140 0000 	adc.w	r0, r0, #0
 8020be2:	ea53 034c 	orrs.w	r3, r3, ip, lsl #1
 8020be6:	bf08      	it	eq
 8020be8:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 8020bec:	4770      	bx	lr
 8020bee:	f092 0f00 	teq	r2, #0
 8020bf2:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 8020bf6:	bf02      	ittt	eq
 8020bf8:	0040      	lsleq	r0, r0, #1
 8020bfa:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 8020bfe:	3a01      	subeq	r2, #1
 8020c00:	d0f9      	beq.n	8020bf6 <__aeabi_fmul+0xce>
 8020c02:	ea40 000c 	orr.w	r0, r0, ip
 8020c06:	f093 0f00 	teq	r3, #0
 8020c0a:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 8020c0e:	bf02      	ittt	eq
 8020c10:	0049      	lsleq	r1, r1, #1
 8020c12:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 8020c16:	3b01      	subeq	r3, #1
 8020c18:	d0f9      	beq.n	8020c0e <__aeabi_fmul+0xe6>
 8020c1a:	ea41 010c 	orr.w	r1, r1, ip
 8020c1e:	e78f      	b.n	8020b40 <__aeabi_fmul+0x18>
 8020c20:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 8020c24:	ea92 0f0c 	teq	r2, ip
 8020c28:	bf18      	it	ne
 8020c2a:	ea93 0f0c 	teqne	r3, ip
 8020c2e:	d00a      	beq.n	8020c46 <__aeabi_fmul+0x11e>
 8020c30:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 8020c34:	bf18      	it	ne
 8020c36:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 8020c3a:	d1d8      	bne.n	8020bee <__aeabi_fmul+0xc6>
 8020c3c:	ea80 0001 	eor.w	r0, r0, r1
 8020c40:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 8020c44:	4770      	bx	lr
 8020c46:	f090 0f00 	teq	r0, #0
 8020c4a:	bf17      	itett	ne
 8020c4c:	f090 4f00 	teqne	r0, #2147483648	; 0x80000000
 8020c50:	4608      	moveq	r0, r1
 8020c52:	f091 0f00 	teqne	r1, #0
 8020c56:	f091 4f00 	teqne	r1, #2147483648	; 0x80000000
 8020c5a:	d014      	beq.n	8020c86 <__aeabi_fmul+0x15e>
 8020c5c:	ea92 0f0c 	teq	r2, ip
 8020c60:	d101      	bne.n	8020c66 <__aeabi_fmul+0x13e>
 8020c62:	0242      	lsls	r2, r0, #9
 8020c64:	d10f      	bne.n	8020c86 <__aeabi_fmul+0x15e>
 8020c66:	ea93 0f0c 	teq	r3, ip
 8020c6a:	d103      	bne.n	8020c74 <__aeabi_fmul+0x14c>
 8020c6c:	024b      	lsls	r3, r1, #9
 8020c6e:	bf18      	it	ne
 8020c70:	4608      	movne	r0, r1
 8020c72:	d108      	bne.n	8020c86 <__aeabi_fmul+0x15e>
 8020c74:	ea80 0001 	eor.w	r0, r0, r1
 8020c78:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 8020c7c:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 8020c80:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8020c84:	4770      	bx	lr
 8020c86:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 8020c8a:	f440 0040 	orr.w	r0, r0, #12582912	; 0xc00000
 8020c8e:	4770      	bx	lr

08020c90 <__aeabi_fdiv>:
 8020c90:	f04f 0cff 	mov.w	ip, #255	; 0xff
 8020c94:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 8020c98:	bf1e      	ittt	ne
 8020c9a:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 8020c9e:	ea92 0f0c 	teqne	r2, ip
 8020ca2:	ea93 0f0c 	teqne	r3, ip
 8020ca6:	d069      	beq.n	8020d7c <__aeabi_fdiv+0xec>
 8020ca8:	eba2 0203 	sub.w	r2, r2, r3
 8020cac:	ea80 0c01 	eor.w	ip, r0, r1
 8020cb0:	0249      	lsls	r1, r1, #9
 8020cb2:	ea4f 2040 	mov.w	r0, r0, lsl #9
 8020cb6:	d037      	beq.n	8020d28 <__aeabi_fdiv+0x98>
 8020cb8:	f04f 5380 	mov.w	r3, #268435456	; 0x10000000
 8020cbc:	ea43 1111 	orr.w	r1, r3, r1, lsr #4
 8020cc0:	ea43 1310 	orr.w	r3, r3, r0, lsr #4
 8020cc4:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 8020cc8:	428b      	cmp	r3, r1
 8020cca:	bf38      	it	cc
 8020ccc:	005b      	lslcc	r3, r3, #1
 8020cce:	f142 027d 	adc.w	r2, r2, #125	; 0x7d
 8020cd2:	f44f 0c00 	mov.w	ip, #8388608	; 0x800000
 8020cd6:	428b      	cmp	r3, r1
 8020cd8:	bf24      	itt	cs
 8020cda:	1a5b      	subcs	r3, r3, r1
 8020cdc:	ea40 000c 	orrcs.w	r0, r0, ip
 8020ce0:	ebb3 0f51 	cmp.w	r3, r1, lsr #1
 8020ce4:	bf24      	itt	cs
 8020ce6:	eba3 0351 	subcs.w	r3, r3, r1, lsr #1
 8020cea:	ea40 005c 	orrcs.w	r0, r0, ip, lsr #1
 8020cee:	ebb3 0f91 	cmp.w	r3, r1, lsr #2
 8020cf2:	bf24      	itt	cs
 8020cf4:	eba3 0391 	subcs.w	r3, r3, r1, lsr #2
 8020cf8:	ea40 009c 	orrcs.w	r0, r0, ip, lsr #2
 8020cfc:	ebb3 0fd1 	cmp.w	r3, r1, lsr #3
 8020d00:	bf24      	itt	cs
 8020d02:	eba3 03d1 	subcs.w	r3, r3, r1, lsr #3
 8020d06:	ea40 00dc 	orrcs.w	r0, r0, ip, lsr #3
 8020d0a:	011b      	lsls	r3, r3, #4
 8020d0c:	bf18      	it	ne
 8020d0e:	ea5f 1c1c 	movsne.w	ip, ip, lsr #4
 8020d12:	d1e0      	bne.n	8020cd6 <__aeabi_fdiv+0x46>
 8020d14:	2afd      	cmp	r2, #253	; 0xfd
 8020d16:	f63f af50 	bhi.w	8020bba <__aeabi_fmul+0x92>
 8020d1a:	428b      	cmp	r3, r1
 8020d1c:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 8020d20:	bf08      	it	eq
 8020d22:	f020 0001 	biceq.w	r0, r0, #1
 8020d26:	4770      	bx	lr
 8020d28:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 8020d2c:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 8020d30:	327f      	adds	r2, #127	; 0x7f
 8020d32:	bfc2      	ittt	gt
 8020d34:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 8020d38:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 8020d3c:	4770      	bxgt	lr
 8020d3e:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8020d42:	f04f 0300 	mov.w	r3, #0
 8020d46:	3a01      	subs	r2, #1
 8020d48:	e737      	b.n	8020bba <__aeabi_fmul+0x92>
 8020d4a:	f092 0f00 	teq	r2, #0
 8020d4e:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 8020d52:	bf02      	ittt	eq
 8020d54:	0040      	lsleq	r0, r0, #1
 8020d56:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 8020d5a:	3a01      	subeq	r2, #1
 8020d5c:	d0f9      	beq.n	8020d52 <__aeabi_fdiv+0xc2>
 8020d5e:	ea40 000c 	orr.w	r0, r0, ip
 8020d62:	f093 0f00 	teq	r3, #0
 8020d66:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 8020d6a:	bf02      	ittt	eq
 8020d6c:	0049      	lsleq	r1, r1, #1
 8020d6e:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 8020d72:	3b01      	subeq	r3, #1
 8020d74:	d0f9      	beq.n	8020d6a <__aeabi_fdiv+0xda>
 8020d76:	ea41 010c 	orr.w	r1, r1, ip
 8020d7a:	e795      	b.n	8020ca8 <__aeabi_fdiv+0x18>
 8020d7c:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 8020d80:	ea92 0f0c 	teq	r2, ip
 8020d84:	d108      	bne.n	8020d98 <__aeabi_fdiv+0x108>
 8020d86:	0242      	lsls	r2, r0, #9
 8020d88:	f47f af7d 	bne.w	8020c86 <__aeabi_fmul+0x15e>
 8020d8c:	ea93 0f0c 	teq	r3, ip
 8020d90:	f47f af70 	bne.w	8020c74 <__aeabi_fmul+0x14c>
 8020d94:	4608      	mov	r0, r1
 8020d96:	e776      	b.n	8020c86 <__aeabi_fmul+0x15e>
 8020d98:	ea93 0f0c 	teq	r3, ip
 8020d9c:	d104      	bne.n	8020da8 <__aeabi_fdiv+0x118>
 8020d9e:	024b      	lsls	r3, r1, #9
 8020da0:	f43f af4c 	beq.w	8020c3c <__aeabi_fmul+0x114>
 8020da4:	4608      	mov	r0, r1
 8020da6:	e76e      	b.n	8020c86 <__aeabi_fmul+0x15e>
 8020da8:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 8020dac:	bf18      	it	ne
 8020dae:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 8020db2:	d1ca      	bne.n	8020d4a <__aeabi_fdiv+0xba>
 8020db4:	f030 4200 	bics.w	r2, r0, #2147483648	; 0x80000000
 8020db8:	f47f af5c 	bne.w	8020c74 <__aeabi_fmul+0x14c>
 8020dbc:	f031 4300 	bics.w	r3, r1, #2147483648	; 0x80000000
 8020dc0:	f47f af3c 	bne.w	8020c3c <__aeabi_fmul+0x114>
 8020dc4:	e75f      	b.n	8020c86 <__aeabi_fmul+0x15e>
 8020dc6:	bf00      	nop

08020dc8 <memset>:
 8020dc8:	b4f0      	push	{r4, r5, r6, r7}
 8020dca:	0786      	lsls	r6, r0, #30
 8020dcc:	d043      	beq.n	8020e56 <memset+0x8e>
 8020dce:	1e54      	subs	r4, r2, #1
 8020dd0:	2a00      	cmp	r2, #0
 8020dd2:	d03e      	beq.n	8020e52 <memset+0x8a>
 8020dd4:	b2ca      	uxtb	r2, r1
 8020dd6:	4603      	mov	r3, r0
 8020dd8:	e002      	b.n	8020de0 <memset+0x18>
 8020dda:	f114 34ff 	adds.w	r4, r4, #4294967295	; 0xffffffff
 8020dde:	d338      	bcc.n	8020e52 <memset+0x8a>
 8020de0:	f803 2b01 	strb.w	r2, [r3], #1
 8020de4:	079d      	lsls	r5, r3, #30
 8020de6:	d1f8      	bne.n	8020dda <memset+0x12>
 8020de8:	2c03      	cmp	r4, #3
 8020dea:	d92b      	bls.n	8020e44 <memset+0x7c>
 8020dec:	b2cd      	uxtb	r5, r1
 8020dee:	ea45 2505 	orr.w	r5, r5, r5, lsl #8
 8020df2:	2c0f      	cmp	r4, #15
 8020df4:	ea45 4505 	orr.w	r5, r5, r5, lsl #16
 8020df8:	d916      	bls.n	8020e28 <memset+0x60>
 8020dfa:	f1a4 0710 	sub.w	r7, r4, #16
 8020dfe:	093f      	lsrs	r7, r7, #4
 8020e00:	f103 0620 	add.w	r6, r3, #32
 8020e04:	eb06 1607 	add.w	r6, r6, r7, lsl #4
 8020e08:	f103 0210 	add.w	r2, r3, #16
 8020e0c:	e942 5504 	strd	r5, r5, [r2, #-16]
 8020e10:	e942 5502 	strd	r5, r5, [r2, #-8]
 8020e14:	3210      	adds	r2, #16
 8020e16:	42b2      	cmp	r2, r6
 8020e18:	d1f8      	bne.n	8020e0c <memset+0x44>
 8020e1a:	f004 040f 	and.w	r4, r4, #15
 8020e1e:	3701      	adds	r7, #1
 8020e20:	2c03      	cmp	r4, #3
 8020e22:	eb03 1307 	add.w	r3, r3, r7, lsl #4
 8020e26:	d90d      	bls.n	8020e44 <memset+0x7c>
 8020e28:	461e      	mov	r6, r3
 8020e2a:	4622      	mov	r2, r4
 8020e2c:	3a04      	subs	r2, #4
 8020e2e:	2a03      	cmp	r2, #3
 8020e30:	f846 5b04 	str.w	r5, [r6], #4
 8020e34:	d8fa      	bhi.n	8020e2c <memset+0x64>
 8020e36:	1f22      	subs	r2, r4, #4
 8020e38:	f022 0203 	bic.w	r2, r2, #3
 8020e3c:	3204      	adds	r2, #4
 8020e3e:	4413      	add	r3, r2
 8020e40:	f004 0403 	and.w	r4, r4, #3
 8020e44:	b12c      	cbz	r4, 8020e52 <memset+0x8a>
 8020e46:	b2c9      	uxtb	r1, r1
 8020e48:	441c      	add	r4, r3
 8020e4a:	f803 1b01 	strb.w	r1, [r3], #1
 8020e4e:	429c      	cmp	r4, r3
 8020e50:	d1fb      	bne.n	8020e4a <memset+0x82>
 8020e52:	bcf0      	pop	{r4, r5, r6, r7}
 8020e54:	4770      	bx	lr
 8020e56:	4614      	mov	r4, r2
 8020e58:	4603      	mov	r3, r0
 8020e5a:	e7c5      	b.n	8020de8 <memset+0x20>

08020e5c <CSWTCH.41>:
 8020e5c:	0fdd 0802 0f32 0802 0f32 0802 0f32 0802     ....2...2...2...
 8020e6c:	0f32 0802 0f32 0802 0f32 0802 0f32 0802     2...2...2...2...
 8020e7c:	0f32 0802 0f32 0802 0f32 0802 0f32 0802     2...2...2...2...
 8020e8c:	0f32 0802 0f32 0802 0f32 0802 0f32 0802     2...2...2...2...
 8020e9c:	0f32 0802 0f32 0802 0fe5 0802 0f32 0802     2...2.......2...
 8020eac:	0f32 0802 0f32 0802 0f32 0802 0f32 0802     2...2...2...2...
 8020ebc:	0fec 0802                                   ....

08020ec0 <_ZL17CShapes_sel_right>:
 8020ec0:	2e0e 2e2e 2e2e 2e2e 2e2e 2e2e 2e2e 2e2e     ................
 8020ed0:	2e2e 2e2e 2e2e 2e2e 2e2e 2e2e 202e 2e2e     ............. ..
 8020ee0:	2e2e 2e2e 2e2e 2e2e 2e2e 2020 2e20 2e2e     ..........   ...
 8020ef0:	2e2e 2e2e 2e2e 202e 2020 2020 2020 2020     .......         
 8020f00:	2020 2020 2020 2020 2020 2020 2020 2020                     
 8020f10:	2020 2020 2020 2020 2020 2020 2020 2020                     
 8020f20:	2020 2020 2020 2020 2020 2020 2020 2020                     
 8020f30:	0020 6e55 6e6b 776f 006e 7245 3a72 6e49      .Unknown.Err:In
 8020f40:	7469 4500 7272 433a 6e6f 0076 6b4f 0021     it.Err:Conv.Ok!.
 8020f50:	7245 3a72 6554 706d 4500 7272 433a 6372     Err:Temp.Err:Crc
 8020f60:	5200 4d4f 003d 3025 7832 4600 6d61 6c69     .ROM=.%02x.Famil
 8020f70:	2079 6f63 6564 3d20 2820 7825 7825 2029     y code = (%x%x) 
 8020f80:	7325 2020 4f00 656e 6957 6572 7320 6174     %s  .OneWire sta
 8020f90:	7574 2073 203d 7325 2020 2020 0020 5053     tus = %s     .SP
 8020fa0:	3d44 5400 6d65 6570 6172 7574 6572 3d20     D=.Temperature =
 8020fb0:	2520 6632 43f8 2020 0020 4420 3153 3238      %2f.C   . DS182
 8020fc0:	2030 6574 7473 4400 3153 3238 2030 656d     0 test.DS1820 me
 8020fd0:	7361 7275 2065 6572 6f70 7472 4400 3153     asure report.DS1
 8020fe0:	5338 3032 4400 3153 3238 0032 5344 3831     8S20.DS1822.DS18
 8020ff0:	3242 0030                                   B20.

Disassembly of section .plt:

080210cc <.plt>:
 80210cc:	b500      	push	{lr}
 80210ce:	f8df e008 	ldr.w	lr, [pc, #8]	; 80210d8 <.plt+0xc>
 80210d2:	44fe      	add	lr, pc
 80210d4:	f85e ff08 	ldr.w	pc, [lr, #8]!
 80210d8:	17fe5fb0 	.word	0x17fe5fb0

080210dc <_ZN4BIOS3LCD5PrintEiittPc@plt>:
 80210dc:	f645 7cac 	movw	ip, #24492	; 0x5fac
 80210e0:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80210e4:	44fc      	add	ip, pc
 80210e6:	f8dc f000 	ldr.w	pc, [ip]
 80210ea:	bf00      	nop

080210ec <_ZN4BIOS4GPIO12DigitalWriteENS0_4EPinEb@plt>:
 80210ec:	0000      	movs	r0, r0
 80210ee:	0000      	movs	r0, r0
 80210f0:	f645 7c9c 	movw	ip, #24476	; 0x5f9c
 80210f4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80210f8:	44fc      	add	ip, pc
 80210fa:	Address 0x00000000080210fa is out of bounds.


080210fc <_ZN4BIOS4GPIO11DigitalReadENS0_4EPinE@plt>:
 80210fc:	f000 bf00 	b.w	8021f00 <_ZN3GUI6WindowERK5CRectt@plt+0xd94>
 8021100:	f645 7c90 	movw	ip, #24464	; 0x5f90
 8021104:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021108:	44fc      	add	ip, pc
 802110a:	Address 0x000000000802110a is out of bounds.


0802110c <_ZN3GUI10BackgroundER5CRectmm@plt>:
 802110c:	f000 bf00 	b.w	8021f10 <_ZN3GUI6WindowERK5CRectt@plt+0xda4>
 8021110:	f645 7c84 	movw	ip, #24452	; 0x5f84
 8021114:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021118:	44fc      	add	ip, pc
 802111a:	Address 0x000000000802111a is out of bounds.


0802111c <_ZN4BIOS3LCD5PrintEiittPKc@plt>:
 802111c:	f000 bf00 	b.w	8021f20 <_ZN3GUI6WindowERK5CRectt@plt+0xdb4>
 8021120:	f645 7c78 	movw	ip, #24440	; 0x5f78
 8021124:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021128:	44fc      	add	ip, pc
 802112a:	Address 0x000000000802112a is out of bounds.


0802112c <_ZN4BIOS3KEY6GetKeyEv@plt>:
 802112c:	f000 bf00 	b.w	8021f30 <_ZN3GUI6WindowERK5CRectt@plt+0xdc4>
 8021130:	f645 7c6c 	movw	ip, #24428	; 0x5f6c
 8021134:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021138:	44fc      	add	ip, pc
 802113a:	Address 0x000000000802113a is out of bounds.


0802113c <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>:
 802113c:	f000 bf00 	b.w	8021f40 <_ZN3GUI6WindowERK5CRectt@plt+0xdd4>
 8021140:	f645 7c60 	movw	ip, #24416	; 0x5f60
 8021144:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021148:	44fc      	add	ip, pc
 802114a:	Address 0x000000000802114a is out of bounds.


0802114c <_ZN4BIOS3LCD4DrawEiittPKc@plt>:
 802114c:	f000 bf00 	b.w	8021f50 <_ZN3GUI6WindowERK5CRectt@plt+0xde4>
 8021150:	f645 7c54 	movw	ip, #24404	; 0x5f54
 8021154:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021158:	44fc      	add	ip, pc
 802115a:	Address 0x000000000802115a is out of bounds.


0802115c <_Z7sprintfPcPKcz@plt>:
 802115c:	f000 bf00 	b.w	8021f60 <_ZN3GUI6WindowERK5CRectt@plt+0xdf4>
 8021160:	f645 7c48 	movw	ip, #24392	; 0x5f48
 8021164:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021168:	44fc      	add	ip, pc
 802116a:	Address 0x000000000802116a is out of bounds.


0802116c <_ZN3GUI6WindowERK5CRectt@plt>:
 802116c:	f000 bf00 	b.w	8021f70 <_ZN3GUI6WindowERK5CRectt@plt+0xe04>
 8021170:	f645 7c3c 	movw	ip, #24380	; 0x5f3c
 8021174:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8021178:	44fc      	add	ip, pc
 802117a:	f8dc f000 	ldr.w	pc, [ip]
 802117e:	bf00      	nop
