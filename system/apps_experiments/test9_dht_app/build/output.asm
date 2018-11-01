
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08020000 <main>:
 8020000:	b51f      	push	{r0, r1, r2, r3, r4, lr}
 8020002:	2300      	movs	r3, #0
 8020004:	ac04      	add	r4, sp, #16
 8020006:	f804 3d10 	strb.w	r3, [r4, #-16]!
 802000a:	4620      	mov	r0, r4
 802000c:	f000 f932 	bl	8020274 <_ZN14CVisualMeasure7OnPaintEv>
 8020010:	f000 feca 	bl	8020da8 <_ZN4BIOS3KEY6GetKeyEv@plt>
 8020014:	2806      	cmp	r0, #6
 8020016:	d007      	beq.n	8020028 <main+0x28>
 8020018:	4620      	mov	r0, r4
 802001a:	f000 f85b 	bl	80200d4 <_ZN14CVisualMeasure9UpdateDhtEv>
 802001e:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
 8020022:	f000 f804 	bl	802002e <_ZN6CDelay7DelayMsEm>
 8020026:	e7f3      	b.n	8020010 <main+0x10>
 8020028:	2000      	movs	r0, #0
 802002a:	b004      	add	sp, #16
 802002c:	bd10      	pop	{r4, pc}

0802002e <_ZN6CDelay7DelayMsEm>:
 802002e:	f110 30ff 	adds.w	r0, r0, #4294967295	; 0xffffffff
 8020032:	d304      	bcc.n	802003e <_ZN6CDelay7DelayMsEm+0x10>
 8020034:	f642 63e1 	movw	r3, #12001	; 0x2ee1
 8020038:	3b01      	subs	r3, #1
 802003a:	d0f8      	beq.n	802002e <_ZN6CDelay7DelayMsEm>
 802003c:	e7fc      	b.n	8020038 <_ZN6CDelay7DelayMsEm+0xa>
 802003e:	4770      	bx	lr

08020040 <_ZN4CDHT5DelayEib>:
 8020040:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
 8020042:	4606      	mov	r6, r0
 8020044:	4617      	mov	r7, r2
 8020046:	2400      	movs	r4, #0
 8020048:	004d      	lsls	r5, r1, #1
 802004a:	42ac      	cmp	r4, r5
 802004c:	da06      	bge.n	802005c <_ZN4CDHT5DelayEib+0x1c>
 802004e:	7830      	ldrb	r0, [r6, #0]
 8020050:	f000 fe92 	bl	8020d78 <_ZN4BIOS4GPIO11DigitalReadENS0_4EPinE@plt>
 8020054:	4287      	cmp	r7, r0
 8020056:	d101      	bne.n	802005c <_ZN4CDHT5DelayEib+0x1c>
 8020058:	3403      	adds	r4, #3
 802005a:	e7f6      	b.n	802004a <_ZN4CDHT5DelayEib+0xa>
 802005c:	1eeb      	subs	r3, r5, #3
 802005e:	42a3      	cmp	r3, r4
 8020060:	bfd8      	it	le
 8020062:	462c      	movle	r4, r5
 8020064:	2002      	movs	r0, #2
 8020066:	fb94 f0f0 	sdiv	r0, r4, r0
 802006a:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}

0802006c <_ZN4CDHT4InitEv>:
 802006c:	b510      	push	{r4, lr}
 802006e:	4604      	mov	r4, r0
 8020070:	2102      	movs	r1, #2
 8020072:	7800      	ldrb	r0, [r0, #0]
 8020074:	f000 fea0 	bl	8020db8 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>
 8020078:	2100      	movs	r1, #0
 802007a:	7820      	ldrb	r0, [r4, #0]
 802007c:	f000 fe74 	bl	8020d68 <_ZN4BIOS4GPIO12DigitalWriteENS0_4EPinEb@plt>
 8020080:	2001      	movs	r0, #1
 8020082:	f7ff ffd4 	bl	802002e <_ZN6CDelay7DelayMsEm>
 8020086:	2109      	movs	r1, #9
 8020088:	7820      	ldrb	r0, [r4, #0]
 802008a:	f000 fe95 	bl	8020db8 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>
 802008e:	2101      	movs	r1, #1
 8020090:	7820      	ldrb	r0, [r4, #0]
 8020092:	f000 fe69 	bl	8020d68 <_ZN4BIOS4GPIO12DigitalWriteENS0_4EPinEb@plt>
 8020096:	2201      	movs	r2, #1
 8020098:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
 802009c:	4620      	mov	r0, r4
 802009e:	f7ff ffcf 	bl	8020040 <_ZN4CDHT5DelayEib>
 80200a2:	f5b0 7f7a 	cmp.w	r0, #1000	; 0x3e8
 80200a6:	d101      	bne.n	80200ac <_ZN4CDHT4InitEv+0x40>
 80200a8:	2000      	movs	r0, #0
 80200aa:	bd10      	pop	{r4, pc}
 80200ac:	2200      	movs	r2, #0
 80200ae:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
 80200b2:	4620      	mov	r0, r4
 80200b4:	f7ff ffc4 	bl	8020040 <_ZN4CDHT5DelayEib>
 80200b8:	f5b0 7f7a 	cmp.w	r0, #1000	; 0x3e8
 80200bc:	d0f4      	beq.n	80200a8 <_ZN4CDHT4InitEv+0x3c>
 80200be:	2201      	movs	r2, #1
 80200c0:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
 80200c4:	4620      	mov	r0, r4
 80200c6:	f7ff ffbb 	bl	8020040 <_ZN4CDHT5DelayEib>
 80200ca:	f5b0 707a 	subs.w	r0, r0, #1000	; 0x3e8
 80200ce:	bf18      	it	ne
 80200d0:	2001      	movne	r0, #1
 80200d2:	e7ea      	b.n	80200aa <_ZN4CDHT4InitEv+0x3e>

080200d4 <_ZN14CVisualMeasure9UpdateDhtEv>:
 80200d4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 80200d8:	b08d      	sub	sp, #52	; 0x34
 80200da:	4604      	mov	r4, r0
 80200dc:	e9d0 9702 	ldrd	r9, r7, [r0, #8]
 80200e0:	f7ff ffc4 	bl	802006c <_ZN4CDHT4InitEv>
 80200e4:	2300      	movs	r3, #0
 80200e6:	4680      	mov	r8, r0
 80200e8:	2800      	cmp	r0, #0
 80200ea:	f000 80af 	beq.w	802024c <_ZN14CVisualMeasure9UpdateDhtEv+0x178>
 80200ee:	f04f 0a28 	mov.w	sl, #40	; 0x28
 80200f2:	2507      	movs	r5, #7
 80200f4:	461e      	mov	r6, r3
 80200f6:	f04f 0b01 	mov.w	fp, #1
 80200fa:	7063      	strb	r3, [r4, #1]
 80200fc:	70a3      	strb	r3, [r4, #2]
 80200fe:	70e3      	strb	r3, [r4, #3]
 8020100:	7123      	strb	r3, [r4, #4]
 8020102:	7163      	strb	r3, [r4, #5]
 8020104:	2200      	movs	r2, #0
 8020106:	21c8      	movs	r1, #200	; 0xc8
 8020108:	4620      	mov	r0, r4
 802010a:	f7ff ff99 	bl	8020040 <_ZN4CDHT5DelayEib>
 802010e:	2201      	movs	r2, #1
 8020110:	9003      	str	r0, [sp, #12]
 8020112:	21c8      	movs	r1, #200	; 0xc8
 8020114:	4620      	mov	r0, r4
 8020116:	f7ff ff93 	bl	8020040 <_ZN4CDHT5DelayEib>
 802011a:	9b03      	ldr	r3, [sp, #12]
 802011c:	4283      	cmp	r3, r0
 802011e:	da05      	bge.n	802012c <_ZN14CVisualMeasure9UpdateDhtEv+0x58>
 8020120:	19a3      	adds	r3, r4, r6
 8020122:	7859      	ldrb	r1, [r3, #1]
 8020124:	fa0b f205 	lsl.w	r2, fp, r5
 8020128:	430a      	orrs	r2, r1
 802012a:	705a      	strb	r2, [r3, #1]
 802012c:	2d00      	cmp	r5, #0
 802012e:	f040 808a 	bne.w	8020246 <_ZN14CVisualMeasure9UpdateDhtEv+0x172>
 8020132:	2507      	movs	r5, #7
 8020134:	1c73      	adds	r3, r6, #1
 8020136:	b2de      	uxtb	r6, r3
 8020138:	f1ba 0a01 	subs.w	sl, sl, #1
 802013c:	d1e2      	bne.n	8020104 <_ZN14CVisualMeasure9UpdateDhtEv+0x30>
 802013e:	4b44      	ldr	r3, [pc, #272]	; (8020250 <_ZN14CVisualMeasure9UpdateDhtEv+0x17c>)
 8020140:	9300      	str	r3, [sp, #0]
 8020142:	f64f 72ff 	movw	r2, #65535	; 0xffff
 8020146:	f24b 5396 	movw	r3, #46486	; 0xb596
 802014a:	4639      	mov	r1, r7
 802014c:	4648      	mov	r0, r9
 802014e:	f000 fe23 	bl	8020d98 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020152:	f1b8 0f00 	cmp.w	r8, #0
 8020156:	d073      	beq.n	8020240 <_ZN14CVisualMeasure9UpdateDhtEv+0x16c>
 8020158:	4b3e      	ldr	r3, [pc, #248]	; (8020254 <_ZN14CVisualMeasure9UpdateDhtEv+0x180>)
 802015a:	f107 0810 	add.w	r8, r7, #16
 802015e:	9300      	str	r3, [sp, #0]
 8020160:	4648      	mov	r0, r9
 8020162:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020166:	f64f 72ff 	movw	r2, #65535	; 0xffff
 802016a:	4641      	mov	r1, r8
 802016c:	f000 fe14 	bl	8020d98 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020170:	eb09 0500 	add.w	r5, r9, r0
 8020174:	46a1      	mov	r9, r4
 8020176:	f8df b0f8 	ldr.w	fp, [pc, #248]	; 8020270 <_ZN14CVisualMeasure9UpdateDhtEv+0x19c>
 802017a:	f104 0a05 	add.w	sl, r4, #5
 802017e:	ae04      	add	r6, sp, #16
 8020180:	f819 2f01 	ldrb.w	r2, [r9, #1]!
 8020184:	4659      	mov	r1, fp
 8020186:	4630      	mov	r0, r6
 8020188:	f000 fe26 	bl	8020dd8 <_Z7sprintfPcPKcz@plt>
 802018c:	4628      	mov	r0, r5
 802018e:	9600      	str	r6, [sp, #0]
 8020190:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020194:	2200      	movs	r2, #0
 8020196:	4641      	mov	r1, r8
 8020198:	f000 fdde 	bl	8020d58 <_ZN4BIOS3LCD5PrintEiittPc@plt>
 802019c:	45ca      	cmp	sl, r9
 802019e:	f100 0004 	add.w	r0, r0, #4
 80201a2:	4405      	add	r5, r0
 80201a4:	d1ec      	bne.n	8020180 <_ZN14CVisualMeasure9UpdateDhtEv+0xac>
 80201a6:	78a2      	ldrb	r2, [r4, #2]
 80201a8:	7863      	ldrb	r3, [r4, #1]
 80201aa:	4641      	mov	r1, r8
 80201ac:	4413      	add	r3, r2
 80201ae:	78e2      	ldrb	r2, [r4, #3]
 80201b0:	4628      	mov	r0, r5
 80201b2:	4413      	add	r3, r2
 80201b4:	7922      	ldrb	r2, [r4, #4]
 80201b6:	4413      	add	r3, r2
 80201b8:	7962      	ldrb	r2, [r4, #5]
 80201ba:	b2db      	uxtb	r3, r3
 80201bc:	429a      	cmp	r2, r3
 80201be:	bf0c      	ite	eq
 80201c0:	4b25      	ldreq	r3, [pc, #148]	; (8020258 <_ZN14CVisualMeasure9UpdateDhtEv+0x184>)
 80201c2:	4b26      	ldrne	r3, [pc, #152]	; (802025c <_ZN14CVisualMeasure9UpdateDhtEv+0x188>)
 80201c4:	2200      	movs	r2, #0
 80201c6:	9300      	str	r3, [sp, #0]
 80201c8:	f24b 5396 	movw	r3, #46486	; 0xb596
 80201cc:	f000 fde4 	bl	8020d98 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80201d0:	7923      	ldrb	r3, [r4, #4]
 80201d2:	78e0      	ldrb	r0, [r4, #3]
 80201d4:	68a5      	ldr	r5, [r4, #8]
 80201d6:	eb03 2000 	add.w	r0, r3, r0, lsl #8
 80201da:	f000 fb25 	bl	8020828 <__aeabi_i2f>
 80201de:	4920      	ldr	r1, [pc, #128]	; (8020260 <_ZN14CVisualMeasure9UpdateDhtEv+0x18c>)
 80201e0:	f000 fb76 	bl	80208d0 <__aeabi_fmul>
 80201e4:	f000 fa0e 	bl	8020604 <__aeabi_f2d>
 80201e8:	4602      	mov	r2, r0
 80201ea:	460b      	mov	r3, r1
 80201ec:	4630      	mov	r0, r6
 80201ee:	491d      	ldr	r1, [pc, #116]	; (8020264 <_ZN14CVisualMeasure9UpdateDhtEv+0x190>)
 80201f0:	f000 fdf2 	bl	8020dd8 <_Z7sprintfPcPKcz@plt>
 80201f4:	f64f 72ff 	movw	r2, #65535	; 0xffff
 80201f8:	f107 0120 	add.w	r1, r7, #32
 80201fc:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020200:	4628      	mov	r0, r5
 8020202:	9600      	str	r6, [sp, #0]
 8020204:	f000 fda8 	bl	8020d58 <_ZN4BIOS3LCD5PrintEiittPc@plt>
 8020208:	78a3      	ldrb	r3, [r4, #2]
 802020a:	7860      	ldrb	r0, [r4, #1]
 802020c:	68a5      	ldr	r5, [r4, #8]
 802020e:	eb03 2000 	add.w	r0, r3, r0, lsl #8
 8020212:	f000 fb09 	bl	8020828 <__aeabi_i2f>
 8020216:	4912      	ldr	r1, [pc, #72]	; (8020260 <_ZN14CVisualMeasure9UpdateDhtEv+0x18c>)
 8020218:	f000 fb5a 	bl	80208d0 <__aeabi_fmul>
 802021c:	f000 f9f2 	bl	8020604 <__aeabi_f2d>
 8020220:	4602      	mov	r2, r0
 8020222:	460b      	mov	r3, r1
 8020224:	4630      	mov	r0, r6
 8020226:	4910      	ldr	r1, [pc, #64]	; (8020268 <_ZN14CVisualMeasure9UpdateDhtEv+0x194>)
 8020228:	f000 fdd6 	bl	8020dd8 <_Z7sprintfPcPKcz@plt>
 802022c:	9600      	str	r6, [sp, #0]
 802022e:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020232:	f64f 72ff 	movw	r2, #65535	; 0xffff
 8020236:	f107 0130 	add.w	r1, r7, #48	; 0x30
 802023a:	4628      	mov	r0, r5
 802023c:	f000 fd8c 	bl	8020d58 <_ZN4BIOS3LCD5PrintEiittPc@plt>
 8020240:	b00d      	add	sp, #52	; 0x34
 8020242:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8020246:	3d01      	subs	r5, #1
 8020248:	b2ed      	uxtb	r5, r5
 802024a:	e775      	b.n	8020138 <_ZN14CVisualMeasure9UpdateDhtEv+0x64>
 802024c:	4b07      	ldr	r3, [pc, #28]	; (802026c <_ZN14CVisualMeasure9UpdateDhtEv+0x198>)
 802024e:	e777      	b.n	8020140 <_ZN14CVisualMeasure9UpdateDhtEv+0x6c>
 8020250:	08020bff 	.word	0x08020bff
 8020254:	08020c1a 	.word	0x08020c1a
 8020258:	08020c37 	.word	0x08020c37
 802025c:	08020c40 	.word	0x08020c40
 8020260:	3dcccccd 	.word	0x3dcccccd
 8020264:	08020c4a 	.word	0x08020c4a
 8020268:	08020c5e 	.word	0x08020c5e
 802026c:	08020c0e 	.word	0x08020c0e
 8020270:	08020c32 	.word	0x08020c32

08020274 <_ZN14CVisualMeasure7OnPaintEv>:
 8020274:	b530      	push	{r4, r5, lr}
 8020276:	2400      	movs	r4, #0
 8020278:	b089      	sub	sp, #36	; 0x24
 802027a:	4b29      	ldr	r3, [pc, #164]	; (8020320 <_ZN14CVisualMeasure7OnPaintEv+0xac>)
 802027c:	4605      	mov	r5, r0
 802027e:	f241 0282 	movw	r2, #4226	; 0x1082
 8020282:	a802      	add	r0, sp, #8
 8020284:	f244 2108 	movw	r1, #16904	; 0x4208
 8020288:	e9cd 4302 	strd	r4, r3, [sp, #8]
 802028c:	f000 fd7c 	bl	8020d88 <_ZN3GUI10BackgroundER5CRectmm@plt>
 8020290:	4b24      	ldr	r3, [pc, #144]	; (8020324 <_ZN14CVisualMeasure7OnPaintEv+0xb0>)
 8020292:	a804      	add	r0, sp, #16
 8020294:	f244 2208 	movw	r2, #16904	; 0x4208
 8020298:	f24b 2108 	movw	r1, #45576	; 0xb208
 802029c:	9305      	str	r3, [sp, #20]
 802029e:	9404      	str	r4, [sp, #16]
 80202a0:	f000 fd72 	bl	8020d88 <_ZN3GUI10BackgroundER5CRectmm@plt>
 80202a4:	4a20      	ldr	r2, [pc, #128]	; (8020328 <_ZN14CVisualMeasure7OnPaintEv+0xb4>)
 80202a6:	4b21      	ldr	r3, [pc, #132]	; (802032c <_ZN14CVisualMeasure7OnPaintEv+0xb8>)
 80202a8:	a806      	add	r0, sp, #24
 80202aa:	f64f 71ff 	movw	r1, #65535	; 0xffff
 80202ae:	e9cd 2306 	strd	r2, r3, [sp, #24]
 80202b2:	f000 fd99 	bl	8020de8 <_ZN3GUI6WindowERK5CRectt@plt>
 80202b6:	4b1e      	ldr	r3, [pc, #120]	; (8020330 <_ZN14CVisualMeasure7OnPaintEv+0xbc>)
 80202b8:	4622      	mov	r2, r4
 80202ba:	9300      	str	r3, [sp, #0]
 80202bc:	4621      	mov	r1, r4
 80202be:	f24b 5396 	movw	r3, #46486	; 0xb596
 80202c2:	4620      	mov	r0, r4
 80202c4:	f000 fd68 	bl	8020d98 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80202c8:	4b1a      	ldr	r3, [pc, #104]	; (8020334 <_ZN14CVisualMeasure7OnPaintEv+0xc0>)
 80202ca:	4621      	mov	r1, r4
 80202cc:	9300      	str	r3, [sp, #0]
 80202ce:	f24b 5296 	movw	r2, #46486	; 0xb596
 80202d2:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80202d6:	f000 fd77 	bl	8020dc8 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 80202da:	2202      	movs	r2, #2
 80202dc:	f9bd 001c 	ldrsh.w	r0, [sp, #28]
 80202e0:	f9bd 3018 	ldrsh.w	r3, [sp, #24]
 80202e4:	fb90 f0f2 	sdiv	r0, r0, r2
 80202e8:	fb93 f3f2 	sdiv	r3, r3, r2
 80202ec:	f9bd 101a 	ldrsh.w	r1, [sp, #26]
 80202f0:	4418      	add	r0, r3
 80202f2:	4b11      	ldr	r3, [pc, #68]	; (8020338 <_ZN14CVisualMeasure7OnPaintEv+0xc4>)
 80202f4:	4622      	mov	r2, r4
 80202f6:	9300      	str	r3, [sp, #0]
 80202f8:	3102      	adds	r1, #2
 80202fa:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80202fe:	3854      	subs	r0, #84	; 0x54
 8020300:	f000 fd4a 	bl	8020d98 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020304:	f9bd 3018 	ldrsh.w	r3, [sp, #24]
 8020308:	4628      	mov	r0, r5
 802030a:	3308      	adds	r3, #8
 802030c:	60ab      	str	r3, [r5, #8]
 802030e:	f9bd 301a 	ldrsh.w	r3, [sp, #26]
 8020312:	331a      	adds	r3, #26
 8020314:	60eb      	str	r3, [r5, #12]
 8020316:	f7ff fedd 	bl	80200d4 <_ZN14CVisualMeasure9UpdateDhtEv>
 802031a:	b009      	add	sp, #36	; 0x24
 802031c:	bd30      	pop	{r4, r5, pc}
 802031e:	bf00      	nop
 8020320:	00f00140 	.word	0x00f00140
 8020324:	000e0140 	.word	0x000e0140
 8020328:	00420014 	.word	0x00420014
 802032c:	00be012c 	.word	0x00be012c
 8020330:	08020be2 	.word	0x08020be2
 8020334:	08020b70 	.word	0x08020b70
 8020338:	08020bec 	.word	0x08020bec

0802033c <__aeabi_drsub>:
 802033c:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000
 8020340:	e002      	b.n	8020348 <__adddf3>
 8020342:	bf00      	nop

08020344 <__aeabi_dsub>:
 8020344:	f083 4300 	eor.w	r3, r3, #2147483648	; 0x80000000

08020348 <__adddf3>:
 8020348:	b530      	push	{r4, r5, lr}
 802034a:	ea4f 0441 	mov.w	r4, r1, lsl #1
 802034e:	ea4f 0543 	mov.w	r5, r3, lsl #1
 8020352:	ea94 0f05 	teq	r4, r5
 8020356:	bf08      	it	eq
 8020358:	ea90 0f02 	teqeq	r0, r2
 802035c:	bf1f      	itttt	ne
 802035e:	ea54 0c00 	orrsne.w	ip, r4, r0
 8020362:	ea55 0c02 	orrsne.w	ip, r5, r2
 8020366:	ea7f 5c64 	mvnsne.w	ip, r4, asr #21
 802036a:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 802036e:	f000 80e2 	beq.w	8020536 <__adddf3+0x1ee>
 8020372:	ea4f 5454 	mov.w	r4, r4, lsr #21
 8020376:	ebd4 5555 	rsbs	r5, r4, r5, lsr #21
 802037a:	bfb8      	it	lt
 802037c:	426d      	neglt	r5, r5
 802037e:	dd0c      	ble.n	802039a <__adddf3+0x52>
 8020380:	442c      	add	r4, r5
 8020382:	ea80 0202 	eor.w	r2, r0, r2
 8020386:	ea81 0303 	eor.w	r3, r1, r3
 802038a:	ea82 0000 	eor.w	r0, r2, r0
 802038e:	ea83 0101 	eor.w	r1, r3, r1
 8020392:	ea80 0202 	eor.w	r2, r0, r2
 8020396:	ea81 0303 	eor.w	r3, r1, r3
 802039a:	2d36      	cmp	r5, #54	; 0x36
 802039c:	bf88      	it	hi
 802039e:	bd30      	pophi	{r4, r5, pc}
 80203a0:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 80203a4:	ea4f 3101 	mov.w	r1, r1, lsl #12
 80203a8:	f44f 1c80 	mov.w	ip, #1048576	; 0x100000
 80203ac:	ea4c 3111 	orr.w	r1, ip, r1, lsr #12
 80203b0:	d002      	beq.n	80203b8 <__adddf3+0x70>
 80203b2:	4240      	negs	r0, r0
 80203b4:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 80203b8:	f013 4f00 	tst.w	r3, #2147483648	; 0x80000000
 80203bc:	ea4f 3303 	mov.w	r3, r3, lsl #12
 80203c0:	ea4c 3313 	orr.w	r3, ip, r3, lsr #12
 80203c4:	d002      	beq.n	80203cc <__adddf3+0x84>
 80203c6:	4252      	negs	r2, r2
 80203c8:	eb63 0343 	sbc.w	r3, r3, r3, lsl #1
 80203cc:	ea94 0f05 	teq	r4, r5
 80203d0:	f000 80a7 	beq.w	8020522 <__adddf3+0x1da>
 80203d4:	f1a4 0401 	sub.w	r4, r4, #1
 80203d8:	f1d5 0e20 	rsbs	lr, r5, #32
 80203dc:	db0d      	blt.n	80203fa <__adddf3+0xb2>
 80203de:	fa02 fc0e 	lsl.w	ip, r2, lr
 80203e2:	fa22 f205 	lsr.w	r2, r2, r5
 80203e6:	1880      	adds	r0, r0, r2
 80203e8:	f141 0100 	adc.w	r1, r1, #0
 80203ec:	fa03 f20e 	lsl.w	r2, r3, lr
 80203f0:	1880      	adds	r0, r0, r2
 80203f2:	fa43 f305 	asr.w	r3, r3, r5
 80203f6:	4159      	adcs	r1, r3
 80203f8:	e00e      	b.n	8020418 <__adddf3+0xd0>
 80203fa:	f1a5 0520 	sub.w	r5, r5, #32
 80203fe:	f10e 0e20 	add.w	lr, lr, #32
 8020402:	2a01      	cmp	r2, #1
 8020404:	fa03 fc0e 	lsl.w	ip, r3, lr
 8020408:	bf28      	it	cs
 802040a:	f04c 0c02 	orrcs.w	ip, ip, #2
 802040e:	fa43 f305 	asr.w	r3, r3, r5
 8020412:	18c0      	adds	r0, r0, r3
 8020414:	eb51 71e3 	adcs.w	r1, r1, r3, asr #31
 8020418:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 802041c:	d507      	bpl.n	802042e <__adddf3+0xe6>
 802041e:	f04f 0e00 	mov.w	lr, #0
 8020422:	f1dc 0c00 	rsbs	ip, ip, #0
 8020426:	eb7e 0000 	sbcs.w	r0, lr, r0
 802042a:	eb6e 0101 	sbc.w	r1, lr, r1
 802042e:	f5b1 1f80 	cmp.w	r1, #1048576	; 0x100000
 8020432:	d31b      	bcc.n	802046c <__adddf3+0x124>
 8020434:	f5b1 1f00 	cmp.w	r1, #2097152	; 0x200000
 8020438:	d30c      	bcc.n	8020454 <__adddf3+0x10c>
 802043a:	0849      	lsrs	r1, r1, #1
 802043c:	ea5f 0030 	movs.w	r0, r0, rrx
 8020440:	ea4f 0c3c 	mov.w	ip, ip, rrx
 8020444:	f104 0401 	add.w	r4, r4, #1
 8020448:	ea4f 5244 	mov.w	r2, r4, lsl #21
 802044c:	f512 0f80 	cmn.w	r2, #4194304	; 0x400000
 8020450:	f080 809a 	bcs.w	8020588 <__adddf3+0x240>
 8020454:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 8020458:	bf08      	it	eq
 802045a:	ea5f 0c50 	movseq.w	ip, r0, lsr #1
 802045e:	f150 0000 	adcs.w	r0, r0, #0
 8020462:	eb41 5104 	adc.w	r1, r1, r4, lsl #20
 8020466:	ea41 0105 	orr.w	r1, r1, r5
 802046a:	bd30      	pop	{r4, r5, pc}
 802046c:	ea5f 0c4c 	movs.w	ip, ip, lsl #1
 8020470:	4140      	adcs	r0, r0
 8020472:	eb41 0101 	adc.w	r1, r1, r1
 8020476:	f411 1f80 	tst.w	r1, #1048576	; 0x100000
 802047a:	f1a4 0401 	sub.w	r4, r4, #1
 802047e:	d1e9      	bne.n	8020454 <__adddf3+0x10c>
 8020480:	f091 0f00 	teq	r1, #0
 8020484:	bf04      	itt	eq
 8020486:	4601      	moveq	r1, r0
 8020488:	2000      	moveq	r0, #0
 802048a:	fab1 f381 	clz	r3, r1
 802048e:	bf08      	it	eq
 8020490:	3320      	addeq	r3, #32
 8020492:	f1a3 030b 	sub.w	r3, r3, #11
 8020496:	f1b3 0220 	subs.w	r2, r3, #32
 802049a:	da0c      	bge.n	80204b6 <__adddf3+0x16e>
 802049c:	320c      	adds	r2, #12
 802049e:	dd08      	ble.n	80204b2 <__adddf3+0x16a>
 80204a0:	f102 0c14 	add.w	ip, r2, #20
 80204a4:	f1c2 020c 	rsb	r2, r2, #12
 80204a8:	fa01 f00c 	lsl.w	r0, r1, ip
 80204ac:	fa21 f102 	lsr.w	r1, r1, r2
 80204b0:	e00c      	b.n	80204cc <__adddf3+0x184>
 80204b2:	f102 0214 	add.w	r2, r2, #20
 80204b6:	bfd8      	it	le
 80204b8:	f1c2 0c20 	rsble	ip, r2, #32
 80204bc:	fa01 f102 	lsl.w	r1, r1, r2
 80204c0:	fa20 fc0c 	lsr.w	ip, r0, ip
 80204c4:	bfdc      	itt	le
 80204c6:	ea41 010c 	orrle.w	r1, r1, ip
 80204ca:	4090      	lslle	r0, r2
 80204cc:	1ae4      	subs	r4, r4, r3
 80204ce:	bfa2      	ittt	ge
 80204d0:	eb01 5104 	addge.w	r1, r1, r4, lsl #20
 80204d4:	4329      	orrge	r1, r5
 80204d6:	bd30      	popge	{r4, r5, pc}
 80204d8:	ea6f 0404 	mvn.w	r4, r4
 80204dc:	3c1f      	subs	r4, #31
 80204de:	da1c      	bge.n	802051a <__adddf3+0x1d2>
 80204e0:	340c      	adds	r4, #12
 80204e2:	dc0e      	bgt.n	8020502 <__adddf3+0x1ba>
 80204e4:	f104 0414 	add.w	r4, r4, #20
 80204e8:	f1c4 0220 	rsb	r2, r4, #32
 80204ec:	fa20 f004 	lsr.w	r0, r0, r4
 80204f0:	fa01 f302 	lsl.w	r3, r1, r2
 80204f4:	ea40 0003 	orr.w	r0, r0, r3
 80204f8:	fa21 f304 	lsr.w	r3, r1, r4
 80204fc:	ea45 0103 	orr.w	r1, r5, r3
 8020500:	bd30      	pop	{r4, r5, pc}
 8020502:	f1c4 040c 	rsb	r4, r4, #12
 8020506:	f1c4 0220 	rsb	r2, r4, #32
 802050a:	fa20 f002 	lsr.w	r0, r0, r2
 802050e:	fa01 f304 	lsl.w	r3, r1, r4
 8020512:	ea40 0003 	orr.w	r0, r0, r3
 8020516:	4629      	mov	r1, r5
 8020518:	bd30      	pop	{r4, r5, pc}
 802051a:	fa21 f004 	lsr.w	r0, r1, r4
 802051e:	4629      	mov	r1, r5
 8020520:	bd30      	pop	{r4, r5, pc}
 8020522:	f094 0f00 	teq	r4, #0
 8020526:	f483 1380 	eor.w	r3, r3, #1048576	; 0x100000
 802052a:	bf06      	itte	eq
 802052c:	f481 1180 	eoreq.w	r1, r1, #1048576	; 0x100000
 8020530:	3401      	addeq	r4, #1
 8020532:	3d01      	subne	r5, #1
 8020534:	e74e      	b.n	80203d4 <__adddf3+0x8c>
 8020536:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 802053a:	bf18      	it	ne
 802053c:	ea7f 5c65 	mvnsne.w	ip, r5, asr #21
 8020540:	d029      	beq.n	8020596 <__adddf3+0x24e>
 8020542:	ea94 0f05 	teq	r4, r5
 8020546:	bf08      	it	eq
 8020548:	ea90 0f02 	teqeq	r0, r2
 802054c:	d005      	beq.n	802055a <__adddf3+0x212>
 802054e:	ea54 0c00 	orrs.w	ip, r4, r0
 8020552:	bf04      	itt	eq
 8020554:	4619      	moveq	r1, r3
 8020556:	4610      	moveq	r0, r2
 8020558:	bd30      	pop	{r4, r5, pc}
 802055a:	ea91 0f03 	teq	r1, r3
 802055e:	bf1e      	ittt	ne
 8020560:	2100      	movne	r1, #0
 8020562:	2000      	movne	r0, #0
 8020564:	bd30      	popne	{r4, r5, pc}
 8020566:	ea5f 5c54 	movs.w	ip, r4, lsr #21
 802056a:	d105      	bne.n	8020578 <__adddf3+0x230>
 802056c:	0040      	lsls	r0, r0, #1
 802056e:	4149      	adcs	r1, r1
 8020570:	bf28      	it	cs
 8020572:	f041 4100 	orrcs.w	r1, r1, #2147483648	; 0x80000000
 8020576:	bd30      	pop	{r4, r5, pc}
 8020578:	f514 0480 	adds.w	r4, r4, #4194304	; 0x400000
 802057c:	bf3c      	itt	cc
 802057e:	f501 1180 	addcc.w	r1, r1, #1048576	; 0x100000
 8020582:	bd30      	popcc	{r4, r5, pc}
 8020584:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 8020588:	f045 41fe 	orr.w	r1, r5, #2130706432	; 0x7f000000
 802058c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
 8020590:	f04f 0000 	mov.w	r0, #0
 8020594:	bd30      	pop	{r4, r5, pc}
 8020596:	ea7f 5c64 	mvns.w	ip, r4, asr #21
 802059a:	bf1a      	itte	ne
 802059c:	4619      	movne	r1, r3
 802059e:	4610      	movne	r0, r2
 80205a0:	ea7f 5c65 	mvnseq.w	ip, r5, asr #21
 80205a4:	bf1c      	itt	ne
 80205a6:	460b      	movne	r3, r1
 80205a8:	4602      	movne	r2, r0
 80205aa:	ea50 3401 	orrs.w	r4, r0, r1, lsl #12
 80205ae:	bf06      	itte	eq
 80205b0:	ea52 3503 	orrseq.w	r5, r2, r3, lsl #12
 80205b4:	ea91 0f03 	teqeq	r1, r3
 80205b8:	f441 2100 	orrne.w	r1, r1, #524288	; 0x80000
 80205bc:	bd30      	pop	{r4, r5, pc}
 80205be:	bf00      	nop

080205c0 <__aeabi_ui2d>:
 80205c0:	f090 0f00 	teq	r0, #0
 80205c4:	bf04      	itt	eq
 80205c6:	2100      	moveq	r1, #0
 80205c8:	4770      	bxeq	lr
 80205ca:	b530      	push	{r4, r5, lr}
 80205cc:	f44f 6480 	mov.w	r4, #1024	; 0x400
 80205d0:	f104 0432 	add.w	r4, r4, #50	; 0x32
 80205d4:	f04f 0500 	mov.w	r5, #0
 80205d8:	f04f 0100 	mov.w	r1, #0
 80205dc:	e750      	b.n	8020480 <__adddf3+0x138>
 80205de:	bf00      	nop

080205e0 <__aeabi_i2d>:
 80205e0:	f090 0f00 	teq	r0, #0
 80205e4:	bf04      	itt	eq
 80205e6:	2100      	moveq	r1, #0
 80205e8:	4770      	bxeq	lr
 80205ea:	b530      	push	{r4, r5, lr}
 80205ec:	f44f 6480 	mov.w	r4, #1024	; 0x400
 80205f0:	f104 0432 	add.w	r4, r4, #50	; 0x32
 80205f4:	f010 4500 	ands.w	r5, r0, #2147483648	; 0x80000000
 80205f8:	bf48      	it	mi
 80205fa:	4240      	negmi	r0, r0
 80205fc:	f04f 0100 	mov.w	r1, #0
 8020600:	e73e      	b.n	8020480 <__adddf3+0x138>
 8020602:	bf00      	nop

08020604 <__aeabi_f2d>:
 8020604:	0042      	lsls	r2, r0, #1
 8020606:	ea4f 01e2 	mov.w	r1, r2, asr #3
 802060a:	ea4f 0131 	mov.w	r1, r1, rrx
 802060e:	ea4f 7002 	mov.w	r0, r2, lsl #28
 8020612:	bf1f      	itttt	ne
 8020614:	f012 437f 	andsne.w	r3, r2, #4278190080	; 0xff000000
 8020618:	f093 4f7f 	teqne	r3, #4278190080	; 0xff000000
 802061c:	f081 5160 	eorne.w	r1, r1, #939524096	; 0x38000000
 8020620:	4770      	bxne	lr
 8020622:	f032 427f 	bics.w	r2, r2, #4278190080	; 0xff000000
 8020626:	bf08      	it	eq
 8020628:	4770      	bxeq	lr
 802062a:	f093 4f7f 	teq	r3, #4278190080	; 0xff000000
 802062e:	bf04      	itt	eq
 8020630:	f441 2100 	orreq.w	r1, r1, #524288	; 0x80000
 8020634:	4770      	bxeq	lr
 8020636:	b530      	push	{r4, r5, lr}
 8020638:	f44f 7460 	mov.w	r4, #896	; 0x380
 802063c:	f001 4500 	and.w	r5, r1, #2147483648	; 0x80000000
 8020640:	f021 4100 	bic.w	r1, r1, #2147483648	; 0x80000000
 8020644:	e71c      	b.n	8020480 <__adddf3+0x138>
 8020646:	bf00      	nop

08020648 <__aeabi_ul2d>:
 8020648:	ea50 0201 	orrs.w	r2, r0, r1
 802064c:	bf08      	it	eq
 802064e:	4770      	bxeq	lr
 8020650:	b530      	push	{r4, r5, lr}
 8020652:	f04f 0500 	mov.w	r5, #0
 8020656:	e00a      	b.n	802066e <__aeabi_l2d+0x16>

08020658 <__aeabi_l2d>:
 8020658:	ea50 0201 	orrs.w	r2, r0, r1
 802065c:	bf08      	it	eq
 802065e:	4770      	bxeq	lr
 8020660:	b530      	push	{r4, r5, lr}
 8020662:	f011 4500 	ands.w	r5, r1, #2147483648	; 0x80000000
 8020666:	d502      	bpl.n	802066e <__aeabi_l2d+0x16>
 8020668:	4240      	negs	r0, r0
 802066a:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 802066e:	f44f 6480 	mov.w	r4, #1024	; 0x400
 8020672:	f104 0432 	add.w	r4, r4, #50	; 0x32
 8020676:	ea5f 5c91 	movs.w	ip, r1, lsr #22
 802067a:	f43f aed8 	beq.w	802042e <__adddf3+0xe6>
 802067e:	f04f 0203 	mov.w	r2, #3
 8020682:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 8020686:	bf18      	it	ne
 8020688:	3203      	addne	r2, #3
 802068a:	ea5f 0cdc 	movs.w	ip, ip, lsr #3
 802068e:	bf18      	it	ne
 8020690:	3203      	addne	r2, #3
 8020692:	eb02 02dc 	add.w	r2, r2, ip, lsr #3
 8020696:	f1c2 0320 	rsb	r3, r2, #32
 802069a:	fa00 fc03 	lsl.w	ip, r0, r3
 802069e:	fa20 f002 	lsr.w	r0, r0, r2
 80206a2:	fa01 fe03 	lsl.w	lr, r1, r3
 80206a6:	ea40 000e 	orr.w	r0, r0, lr
 80206aa:	fa21 f102 	lsr.w	r1, r1, r2
 80206ae:	4414      	add	r4, r2
 80206b0:	e6bd      	b.n	802042e <__adddf3+0xe6>
 80206b2:	bf00      	nop

080206b4 <__aeabi_frsub>:
 80206b4:	f080 4000 	eor.w	r0, r0, #2147483648	; 0x80000000
 80206b8:	e002      	b.n	80206c0 <__addsf3>
 80206ba:	bf00      	nop

080206bc <__aeabi_fsub>:
 80206bc:	f081 4100 	eor.w	r1, r1, #2147483648	; 0x80000000

080206c0 <__addsf3>:
 80206c0:	0042      	lsls	r2, r0, #1
 80206c2:	bf1f      	itttt	ne
 80206c4:	ea5f 0341 	movsne.w	r3, r1, lsl #1
 80206c8:	ea92 0f03 	teqne	r2, r3
 80206cc:	ea7f 6c22 	mvnsne.w	ip, r2, asr #24
 80206d0:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 80206d4:	d06a      	beq.n	80207ac <__addsf3+0xec>
 80206d6:	ea4f 6212 	mov.w	r2, r2, lsr #24
 80206da:	ebd2 6313 	rsbs	r3, r2, r3, lsr #24
 80206de:	bfc1      	itttt	gt
 80206e0:	18d2      	addgt	r2, r2, r3
 80206e2:	4041      	eorgt	r1, r0
 80206e4:	4048      	eorgt	r0, r1
 80206e6:	4041      	eorgt	r1, r0
 80206e8:	bfb8      	it	lt
 80206ea:	425b      	neglt	r3, r3
 80206ec:	2b19      	cmp	r3, #25
 80206ee:	bf88      	it	hi
 80206f0:	4770      	bxhi	lr
 80206f2:	f010 4f00 	tst.w	r0, #2147483648	; 0x80000000
 80206f6:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 80206fa:	f020 407f 	bic.w	r0, r0, #4278190080	; 0xff000000
 80206fe:	bf18      	it	ne
 8020700:	4240      	negne	r0, r0
 8020702:	f011 4f00 	tst.w	r1, #2147483648	; 0x80000000
 8020706:	f441 0100 	orr.w	r1, r1, #8388608	; 0x800000
 802070a:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
 802070e:	bf18      	it	ne
 8020710:	4249      	negne	r1, r1
 8020712:	ea92 0f03 	teq	r2, r3
 8020716:	d03f      	beq.n	8020798 <__addsf3+0xd8>
 8020718:	f1a2 0201 	sub.w	r2, r2, #1
 802071c:	fa41 fc03 	asr.w	ip, r1, r3
 8020720:	eb10 000c 	adds.w	r0, r0, ip
 8020724:	f1c3 0320 	rsb	r3, r3, #32
 8020728:	fa01 f103 	lsl.w	r1, r1, r3
 802072c:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 8020730:	d502      	bpl.n	8020738 <__addsf3+0x78>
 8020732:	4249      	negs	r1, r1
 8020734:	eb60 0040 	sbc.w	r0, r0, r0, lsl #1
 8020738:	f5b0 0f00 	cmp.w	r0, #8388608	; 0x800000
 802073c:	d313      	bcc.n	8020766 <__addsf3+0xa6>
 802073e:	f1b0 7f80 	cmp.w	r0, #16777216	; 0x1000000
 8020742:	d306      	bcc.n	8020752 <__addsf3+0x92>
 8020744:	0840      	lsrs	r0, r0, #1
 8020746:	ea4f 0131 	mov.w	r1, r1, rrx
 802074a:	f102 0201 	add.w	r2, r2, #1
 802074e:	2afe      	cmp	r2, #254	; 0xfe
 8020750:	d251      	bcs.n	80207f6 <__addsf3+0x136>
 8020752:	f1b1 4f00 	cmp.w	r1, #2147483648	; 0x80000000
 8020756:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 802075a:	bf08      	it	eq
 802075c:	f020 0001 	biceq.w	r0, r0, #1
 8020760:	ea40 0003 	orr.w	r0, r0, r3
 8020764:	4770      	bx	lr
 8020766:	0049      	lsls	r1, r1, #1
 8020768:	eb40 0000 	adc.w	r0, r0, r0
 802076c:	f410 0f00 	tst.w	r0, #8388608	; 0x800000
 8020770:	f1a2 0201 	sub.w	r2, r2, #1
 8020774:	d1ed      	bne.n	8020752 <__addsf3+0x92>
 8020776:	fab0 fc80 	clz	ip, r0
 802077a:	f1ac 0c08 	sub.w	ip, ip, #8
 802077e:	ebb2 020c 	subs.w	r2, r2, ip
 8020782:	fa00 f00c 	lsl.w	r0, r0, ip
 8020786:	bfaa      	itet	ge
 8020788:	eb00 50c2 	addge.w	r0, r0, r2, lsl #23
 802078c:	4252      	neglt	r2, r2
 802078e:	4318      	orrge	r0, r3
 8020790:	bfbc      	itt	lt
 8020792:	40d0      	lsrlt	r0, r2
 8020794:	4318      	orrlt	r0, r3
 8020796:	4770      	bx	lr
 8020798:	f092 0f00 	teq	r2, #0
 802079c:	f481 0100 	eor.w	r1, r1, #8388608	; 0x800000
 80207a0:	bf06      	itte	eq
 80207a2:	f480 0000 	eoreq.w	r0, r0, #8388608	; 0x800000
 80207a6:	3201      	addeq	r2, #1
 80207a8:	3b01      	subne	r3, #1
 80207aa:	e7b5      	b.n	8020718 <__addsf3+0x58>
 80207ac:	ea4f 0341 	mov.w	r3, r1, lsl #1
 80207b0:	ea7f 6c22 	mvns.w	ip, r2, asr #24
 80207b4:	bf18      	it	ne
 80207b6:	ea7f 6c23 	mvnsne.w	ip, r3, asr #24
 80207ba:	d021      	beq.n	8020800 <__addsf3+0x140>
 80207bc:	ea92 0f03 	teq	r2, r3
 80207c0:	d004      	beq.n	80207cc <__addsf3+0x10c>
 80207c2:	f092 0f00 	teq	r2, #0
 80207c6:	bf08      	it	eq
 80207c8:	4608      	moveq	r0, r1
 80207ca:	4770      	bx	lr
 80207cc:	ea90 0f01 	teq	r0, r1
 80207d0:	bf1c      	itt	ne
 80207d2:	2000      	movne	r0, #0
 80207d4:	4770      	bxne	lr
 80207d6:	f012 4f7f 	tst.w	r2, #4278190080	; 0xff000000
 80207da:	d104      	bne.n	80207e6 <__addsf3+0x126>
 80207dc:	0040      	lsls	r0, r0, #1
 80207de:	bf28      	it	cs
 80207e0:	f040 4000 	orrcs.w	r0, r0, #2147483648	; 0x80000000
 80207e4:	4770      	bx	lr
 80207e6:	f112 7200 	adds.w	r2, r2, #33554432	; 0x2000000
 80207ea:	bf3c      	itt	cc
 80207ec:	f500 0000 	addcc.w	r0, r0, #8388608	; 0x800000
 80207f0:	4770      	bxcc	lr
 80207f2:	f000 4300 	and.w	r3, r0, #2147483648	; 0x80000000
 80207f6:	f043 40fe 	orr.w	r0, r3, #2130706432	; 0x7f000000
 80207fa:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 80207fe:	4770      	bx	lr
 8020800:	ea7f 6222 	mvns.w	r2, r2, asr #24
 8020804:	bf16      	itet	ne
 8020806:	4608      	movne	r0, r1
 8020808:	ea7f 6323 	mvnseq.w	r3, r3, asr #24
 802080c:	4601      	movne	r1, r0
 802080e:	0242      	lsls	r2, r0, #9
 8020810:	bf06      	itte	eq
 8020812:	ea5f 2341 	movseq.w	r3, r1, lsl #9
 8020816:	ea90 0f01 	teqeq	r0, r1
 802081a:	f440 0080 	orrne.w	r0, r0, #4194304	; 0x400000
 802081e:	4770      	bx	lr

08020820 <__aeabi_ui2f>:
 8020820:	f04f 0300 	mov.w	r3, #0
 8020824:	e004      	b.n	8020830 <__aeabi_i2f+0x8>
 8020826:	bf00      	nop

08020828 <__aeabi_i2f>:
 8020828:	f010 4300 	ands.w	r3, r0, #2147483648	; 0x80000000
 802082c:	bf48      	it	mi
 802082e:	4240      	negmi	r0, r0
 8020830:	ea5f 0c00 	movs.w	ip, r0
 8020834:	bf08      	it	eq
 8020836:	4770      	bxeq	lr
 8020838:	f043 4396 	orr.w	r3, r3, #1258291200	; 0x4b000000
 802083c:	4601      	mov	r1, r0
 802083e:	f04f 0000 	mov.w	r0, #0
 8020842:	e01c      	b.n	802087e <__aeabi_l2f+0x2a>

08020844 <__aeabi_ul2f>:
 8020844:	ea50 0201 	orrs.w	r2, r0, r1
 8020848:	bf08      	it	eq
 802084a:	4770      	bxeq	lr
 802084c:	f04f 0300 	mov.w	r3, #0
 8020850:	e00a      	b.n	8020868 <__aeabi_l2f+0x14>
 8020852:	bf00      	nop

08020854 <__aeabi_l2f>:
 8020854:	ea50 0201 	orrs.w	r2, r0, r1
 8020858:	bf08      	it	eq
 802085a:	4770      	bxeq	lr
 802085c:	f011 4300 	ands.w	r3, r1, #2147483648	; 0x80000000
 8020860:	d502      	bpl.n	8020868 <__aeabi_l2f+0x14>
 8020862:	4240      	negs	r0, r0
 8020864:	eb61 0141 	sbc.w	r1, r1, r1, lsl #1
 8020868:	ea5f 0c01 	movs.w	ip, r1
 802086c:	bf02      	ittt	eq
 802086e:	4684      	moveq	ip, r0
 8020870:	4601      	moveq	r1, r0
 8020872:	2000      	moveq	r0, #0
 8020874:	f043 43b6 	orr.w	r3, r3, #1526726656	; 0x5b000000
 8020878:	bf08      	it	eq
 802087a:	f1a3 5380 	subeq.w	r3, r3, #268435456	; 0x10000000
 802087e:	f5a3 0300 	sub.w	r3, r3, #8388608	; 0x800000
 8020882:	fabc f28c 	clz	r2, ip
 8020886:	3a08      	subs	r2, #8
 8020888:	eba3 53c2 	sub.w	r3, r3, r2, lsl #23
 802088c:	db10      	blt.n	80208b0 <__aeabi_l2f+0x5c>
 802088e:	fa01 fc02 	lsl.w	ip, r1, r2
 8020892:	4463      	add	r3, ip
 8020894:	fa00 fc02 	lsl.w	ip, r0, r2
 8020898:	f1c2 0220 	rsb	r2, r2, #32
 802089c:	f1bc 4f00 	cmp.w	ip, #2147483648	; 0x80000000
 80208a0:	fa20 f202 	lsr.w	r2, r0, r2
 80208a4:	eb43 0002 	adc.w	r0, r3, r2
 80208a8:	bf08      	it	eq
 80208aa:	f020 0001 	biceq.w	r0, r0, #1
 80208ae:	4770      	bx	lr
 80208b0:	f102 0220 	add.w	r2, r2, #32
 80208b4:	fa01 fc02 	lsl.w	ip, r1, r2
 80208b8:	f1c2 0220 	rsb	r2, r2, #32
 80208bc:	ea50 004c 	orrs.w	r0, r0, ip, lsl #1
 80208c0:	fa21 f202 	lsr.w	r2, r1, r2
 80208c4:	eb43 0002 	adc.w	r0, r3, r2
 80208c8:	bf08      	it	eq
 80208ca:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 80208ce:	4770      	bx	lr

080208d0 <__aeabi_fmul>:
 80208d0:	f04f 0cff 	mov.w	ip, #255	; 0xff
 80208d4:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 80208d8:	bf1e      	ittt	ne
 80208da:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 80208de:	ea92 0f0c 	teqne	r2, ip
 80208e2:	ea93 0f0c 	teqne	r3, ip
 80208e6:	d06f      	beq.n	80209c8 <__aeabi_fmul+0xf8>
 80208e8:	441a      	add	r2, r3
 80208ea:	ea80 0c01 	eor.w	ip, r0, r1
 80208ee:	0240      	lsls	r0, r0, #9
 80208f0:	bf18      	it	ne
 80208f2:	ea5f 2141 	movsne.w	r1, r1, lsl #9
 80208f6:	d01e      	beq.n	8020936 <__aeabi_fmul+0x66>
 80208f8:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
 80208fc:	ea43 1050 	orr.w	r0, r3, r0, lsr #5
 8020900:	ea43 1151 	orr.w	r1, r3, r1, lsr #5
 8020904:	fba0 3101 	umull	r3, r1, r0, r1
 8020908:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 802090c:	f5b1 0f00 	cmp.w	r1, #8388608	; 0x800000
 8020910:	bf3e      	ittt	cc
 8020912:	0049      	lslcc	r1, r1, #1
 8020914:	ea41 71d3 	orrcc.w	r1, r1, r3, lsr #31
 8020918:	005b      	lslcc	r3, r3, #1
 802091a:	ea40 0001 	orr.w	r0, r0, r1
 802091e:	f162 027f 	sbc.w	r2, r2, #127	; 0x7f
 8020922:	2afd      	cmp	r2, #253	; 0xfd
 8020924:	d81d      	bhi.n	8020962 <__aeabi_fmul+0x92>
 8020926:	f1b3 4f00 	cmp.w	r3, #2147483648	; 0x80000000
 802092a:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 802092e:	bf08      	it	eq
 8020930:	f020 0001 	biceq.w	r0, r0, #1
 8020934:	4770      	bx	lr
 8020936:	f090 0f00 	teq	r0, #0
 802093a:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 802093e:	bf08      	it	eq
 8020940:	0249      	lsleq	r1, r1, #9
 8020942:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 8020946:	ea40 2051 	orr.w	r0, r0, r1, lsr #9
 802094a:	3a7f      	subs	r2, #127	; 0x7f
 802094c:	bfc2      	ittt	gt
 802094e:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 8020952:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 8020956:	4770      	bxgt	lr
 8020958:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 802095c:	f04f 0300 	mov.w	r3, #0
 8020960:	3a01      	subs	r2, #1
 8020962:	dc5d      	bgt.n	8020a20 <__aeabi_fmul+0x150>
 8020964:	f112 0f19 	cmn.w	r2, #25
 8020968:	bfdc      	itt	le
 802096a:	f000 4000 	andle.w	r0, r0, #2147483648	; 0x80000000
 802096e:	4770      	bxle	lr
 8020970:	f1c2 0200 	rsb	r2, r2, #0
 8020974:	0041      	lsls	r1, r0, #1
 8020976:	fa21 f102 	lsr.w	r1, r1, r2
 802097a:	f1c2 0220 	rsb	r2, r2, #32
 802097e:	fa00 fc02 	lsl.w	ip, r0, r2
 8020982:	ea5f 0031 	movs.w	r0, r1, rrx
 8020986:	f140 0000 	adc.w	r0, r0, #0
 802098a:	ea53 034c 	orrs.w	r3, r3, ip, lsl #1
 802098e:	bf08      	it	eq
 8020990:	ea20 70dc 	biceq.w	r0, r0, ip, lsr #31
 8020994:	4770      	bx	lr
 8020996:	f092 0f00 	teq	r2, #0
 802099a:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 802099e:	bf02      	ittt	eq
 80209a0:	0040      	lsleq	r0, r0, #1
 80209a2:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 80209a6:	3a01      	subeq	r2, #1
 80209a8:	d0f9      	beq.n	802099e <__aeabi_fmul+0xce>
 80209aa:	ea40 000c 	orr.w	r0, r0, ip
 80209ae:	f093 0f00 	teq	r3, #0
 80209b2:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 80209b6:	bf02      	ittt	eq
 80209b8:	0049      	lsleq	r1, r1, #1
 80209ba:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 80209be:	3b01      	subeq	r3, #1
 80209c0:	d0f9      	beq.n	80209b6 <__aeabi_fmul+0xe6>
 80209c2:	ea41 010c 	orr.w	r1, r1, ip
 80209c6:	e78f      	b.n	80208e8 <__aeabi_fmul+0x18>
 80209c8:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 80209cc:	ea92 0f0c 	teq	r2, ip
 80209d0:	bf18      	it	ne
 80209d2:	ea93 0f0c 	teqne	r3, ip
 80209d6:	d00a      	beq.n	80209ee <__aeabi_fmul+0x11e>
 80209d8:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 80209dc:	bf18      	it	ne
 80209de:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 80209e2:	d1d8      	bne.n	8020996 <__aeabi_fmul+0xc6>
 80209e4:	ea80 0001 	eor.w	r0, r0, r1
 80209e8:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 80209ec:	4770      	bx	lr
 80209ee:	f090 0f00 	teq	r0, #0
 80209f2:	bf17      	itett	ne
 80209f4:	f090 4f00 	teqne	r0, #2147483648	; 0x80000000
 80209f8:	4608      	moveq	r0, r1
 80209fa:	f091 0f00 	teqne	r1, #0
 80209fe:	f091 4f00 	teqne	r1, #2147483648	; 0x80000000
 8020a02:	d014      	beq.n	8020a2e <__aeabi_fmul+0x15e>
 8020a04:	ea92 0f0c 	teq	r2, ip
 8020a08:	d101      	bne.n	8020a0e <__aeabi_fmul+0x13e>
 8020a0a:	0242      	lsls	r2, r0, #9
 8020a0c:	d10f      	bne.n	8020a2e <__aeabi_fmul+0x15e>
 8020a0e:	ea93 0f0c 	teq	r3, ip
 8020a12:	d103      	bne.n	8020a1c <__aeabi_fmul+0x14c>
 8020a14:	024b      	lsls	r3, r1, #9
 8020a16:	bf18      	it	ne
 8020a18:	4608      	movne	r0, r1
 8020a1a:	d108      	bne.n	8020a2e <__aeabi_fmul+0x15e>
 8020a1c:	ea80 0001 	eor.w	r0, r0, r1
 8020a20:	f000 4000 	and.w	r0, r0, #2147483648	; 0x80000000
 8020a24:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 8020a28:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8020a2c:	4770      	bx	lr
 8020a2e:	f040 40fe 	orr.w	r0, r0, #2130706432	; 0x7f000000
 8020a32:	f440 0040 	orr.w	r0, r0, #12582912	; 0xc00000
 8020a36:	4770      	bx	lr

08020a38 <__aeabi_fdiv>:
 8020a38:	f04f 0cff 	mov.w	ip, #255	; 0xff
 8020a3c:	ea1c 52d0 	ands.w	r2, ip, r0, lsr #23
 8020a40:	bf1e      	ittt	ne
 8020a42:	ea1c 53d1 	andsne.w	r3, ip, r1, lsr #23
 8020a46:	ea92 0f0c 	teqne	r2, ip
 8020a4a:	ea93 0f0c 	teqne	r3, ip
 8020a4e:	d069      	beq.n	8020b24 <__aeabi_fdiv+0xec>
 8020a50:	eba2 0203 	sub.w	r2, r2, r3
 8020a54:	ea80 0c01 	eor.w	ip, r0, r1
 8020a58:	0249      	lsls	r1, r1, #9
 8020a5a:	ea4f 2040 	mov.w	r0, r0, lsl #9
 8020a5e:	d037      	beq.n	8020ad0 <__aeabi_fdiv+0x98>
 8020a60:	f04f 5380 	mov.w	r3, #268435456	; 0x10000000
 8020a64:	ea43 1111 	orr.w	r1, r3, r1, lsr #4
 8020a68:	ea43 1310 	orr.w	r3, r3, r0, lsr #4
 8020a6c:	f00c 4000 	and.w	r0, ip, #2147483648	; 0x80000000
 8020a70:	428b      	cmp	r3, r1
 8020a72:	bf38      	it	cc
 8020a74:	005b      	lslcc	r3, r3, #1
 8020a76:	f142 027d 	adc.w	r2, r2, #125	; 0x7d
 8020a7a:	f44f 0c00 	mov.w	ip, #8388608	; 0x800000
 8020a7e:	428b      	cmp	r3, r1
 8020a80:	bf24      	itt	cs
 8020a82:	1a5b      	subcs	r3, r3, r1
 8020a84:	ea40 000c 	orrcs.w	r0, r0, ip
 8020a88:	ebb3 0f51 	cmp.w	r3, r1, lsr #1
 8020a8c:	bf24      	itt	cs
 8020a8e:	eba3 0351 	subcs.w	r3, r3, r1, lsr #1
 8020a92:	ea40 005c 	orrcs.w	r0, r0, ip, lsr #1
 8020a96:	ebb3 0f91 	cmp.w	r3, r1, lsr #2
 8020a9a:	bf24      	itt	cs
 8020a9c:	eba3 0391 	subcs.w	r3, r3, r1, lsr #2
 8020aa0:	ea40 009c 	orrcs.w	r0, r0, ip, lsr #2
 8020aa4:	ebb3 0fd1 	cmp.w	r3, r1, lsr #3
 8020aa8:	bf24      	itt	cs
 8020aaa:	eba3 03d1 	subcs.w	r3, r3, r1, lsr #3
 8020aae:	ea40 00dc 	orrcs.w	r0, r0, ip, lsr #3
 8020ab2:	011b      	lsls	r3, r3, #4
 8020ab4:	bf18      	it	ne
 8020ab6:	ea5f 1c1c 	movsne.w	ip, ip, lsr #4
 8020aba:	d1e0      	bne.n	8020a7e <__aeabi_fdiv+0x46>
 8020abc:	2afd      	cmp	r2, #253	; 0xfd
 8020abe:	f63f af50 	bhi.w	8020962 <__aeabi_fmul+0x92>
 8020ac2:	428b      	cmp	r3, r1
 8020ac4:	eb40 50c2 	adc.w	r0, r0, r2, lsl #23
 8020ac8:	bf08      	it	eq
 8020aca:	f020 0001 	biceq.w	r0, r0, #1
 8020ace:	4770      	bx	lr
 8020ad0:	f00c 4c00 	and.w	ip, ip, #2147483648	; 0x80000000
 8020ad4:	ea4c 2050 	orr.w	r0, ip, r0, lsr #9
 8020ad8:	327f      	adds	r2, #127	; 0x7f
 8020ada:	bfc2      	ittt	gt
 8020adc:	f1d2 03ff 	rsbsgt	r3, r2, #255	; 0xff
 8020ae0:	ea40 50c2 	orrgt.w	r0, r0, r2, lsl #23
 8020ae4:	4770      	bxgt	lr
 8020ae6:	f440 0000 	orr.w	r0, r0, #8388608	; 0x800000
 8020aea:	f04f 0300 	mov.w	r3, #0
 8020aee:	3a01      	subs	r2, #1
 8020af0:	e737      	b.n	8020962 <__aeabi_fmul+0x92>
 8020af2:	f092 0f00 	teq	r2, #0
 8020af6:	f000 4c00 	and.w	ip, r0, #2147483648	; 0x80000000
 8020afa:	bf02      	ittt	eq
 8020afc:	0040      	lsleq	r0, r0, #1
 8020afe:	f410 0f00 	tsteq.w	r0, #8388608	; 0x800000
 8020b02:	3a01      	subeq	r2, #1
 8020b04:	d0f9      	beq.n	8020afa <__aeabi_fdiv+0xc2>
 8020b06:	ea40 000c 	orr.w	r0, r0, ip
 8020b0a:	f093 0f00 	teq	r3, #0
 8020b0e:	f001 4c00 	and.w	ip, r1, #2147483648	; 0x80000000
 8020b12:	bf02      	ittt	eq
 8020b14:	0049      	lsleq	r1, r1, #1
 8020b16:	f411 0f00 	tsteq.w	r1, #8388608	; 0x800000
 8020b1a:	3b01      	subeq	r3, #1
 8020b1c:	d0f9      	beq.n	8020b12 <__aeabi_fdiv+0xda>
 8020b1e:	ea41 010c 	orr.w	r1, r1, ip
 8020b22:	e795      	b.n	8020a50 <__aeabi_fdiv+0x18>
 8020b24:	ea0c 53d1 	and.w	r3, ip, r1, lsr #23
 8020b28:	ea92 0f0c 	teq	r2, ip
 8020b2c:	d108      	bne.n	8020b40 <__aeabi_fdiv+0x108>
 8020b2e:	0242      	lsls	r2, r0, #9
 8020b30:	f47f af7d 	bne.w	8020a2e <__aeabi_fmul+0x15e>
 8020b34:	ea93 0f0c 	teq	r3, ip
 8020b38:	f47f af70 	bne.w	8020a1c <__aeabi_fmul+0x14c>
 8020b3c:	4608      	mov	r0, r1
 8020b3e:	e776      	b.n	8020a2e <__aeabi_fmul+0x15e>
 8020b40:	ea93 0f0c 	teq	r3, ip
 8020b44:	d104      	bne.n	8020b50 <__aeabi_fdiv+0x118>
 8020b46:	024b      	lsls	r3, r1, #9
 8020b48:	f43f af4c 	beq.w	80209e4 <__aeabi_fmul+0x114>
 8020b4c:	4608      	mov	r0, r1
 8020b4e:	e76e      	b.n	8020a2e <__aeabi_fmul+0x15e>
 8020b50:	f030 4c00 	bics.w	ip, r0, #2147483648	; 0x80000000
 8020b54:	bf18      	it	ne
 8020b56:	f031 4c00 	bicsne.w	ip, r1, #2147483648	; 0x80000000
 8020b5a:	d1ca      	bne.n	8020af2 <__aeabi_fdiv+0xba>
 8020b5c:	f030 4200 	bics.w	r2, r0, #2147483648	; 0x80000000
 8020b60:	f47f af5c 	bne.w	8020a1c <__aeabi_fmul+0x14c>
 8020b64:	f031 4300 	bics.w	r3, r1, #2147483648	; 0x80000000
 8020b68:	f47f af3c 	bne.w	80209e4 <__aeabi_fmul+0x114>
 8020b6c:	e75f      	b.n	8020a2e <__aeabi_fmul+0x15e>
 8020b6e:	bf00      	nop

08020b70 <_ZL17CShapes_sel_right>:
 8020b70:	2e0e 2e2e 2e2e 2e2e 2e2e 2e2e 2e2e 2e2e     ................
 8020b80:	2e2e 2e2e 2e2e 2e2e 2e2e 2e2e 202e 2e2e     ............. ..
 8020b90:	2e2e 2e2e 2e2e 2e2e 2e2e 2020 2e20 2e2e     ..........   ...
 8020ba0:	2e2e 2e2e 2e2e 202e 2020 2020 2020 2020     .......         
 8020bb0:	2020 2020 2020 2020 2020 2020 2020 2020                     
 8020bc0:	2020 2020 2020 2020 2020 2020 2020 2020                     
 8020bd0:	2020 2020 2020 2020 2020 2020 2020 2020                     
 8020be0:	0020 4420 5448 7420 7365 0074 4844 2054      . DHT test.DHT 
 8020bf0:	656d 7361 7275 2065 6572 6f70 7472 4900     measure report.I
 8020c00:	696e 2074 6b6f 2021 2020 2020 0020 6e49     nit ok!      .In
 8020c10:	7469 6620 6961 656c 0064 4f52 3a4d 0020     it failed.ROM: .
 8020c20:	6572 6461 6620 6961 656c 2164 2020 2020     read failed!    
 8020c30:	0020 3025 7832 6300 6372 6f20 206b 0020      .%02x.crc ok  .
 8020c40:	7263 2063 7265 2072 0020 6554 706d 7265     crc err  .Temper
 8020c50:	7461 7275 3d65 6625 43f8 2020 0020 7548     ature=%f.C   .Hu
 8020c60:	696d 6964 7974 253d 2566 2025 2020           midity=%f%%   .

Disassembly of section .plt:

08020d48 <.plt>:
 8020d48:	b500      	push	{lr}
 8020d4a:	f8df e008 	ldr.w	lr, [pc, #8]	; 8020d54 <.plt+0xc>
 8020d4e:	44fe      	add	lr, pc
 8020d50:	f85e ff08 	ldr.w	pc, [lr, #8]!
 8020d54:	17fe6334 	.word	0x17fe6334

08020d58 <_ZN4BIOS3LCD5PrintEiittPc@plt>:
 8020d58:	f246 3c30 	movw	ip, #25392	; 0x6330
 8020d5c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020d60:	44fc      	add	ip, pc
 8020d62:	f8dc f000 	ldr.w	pc, [ip]
 8020d66:	bf00      	nop

08020d68 <_ZN4BIOS4GPIO12DigitalWriteENS0_4EPinEb@plt>:
 8020d68:	f246 3c24 	movw	ip, #25380	; 0x6324
 8020d6c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020d70:	44fc      	add	ip, pc
 8020d72:	f8dc f000 	ldr.w	pc, [ip]
 8020d76:	bf00      	nop

08020d78 <_ZN4BIOS4GPIO11DigitalReadENS0_4EPinE@plt>:
 8020d78:	f246 3c18 	movw	ip, #25368	; 0x6318
 8020d7c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020d80:	44fc      	add	ip, pc
 8020d82:	f8dc f000 	ldr.w	pc, [ip]
 8020d86:	bf00      	nop

08020d88 <_ZN3GUI10BackgroundER5CRectmm@plt>:
 8020d88:	f246 3c0c 	movw	ip, #25356	; 0x630c
 8020d8c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020d90:	44fc      	add	ip, pc
 8020d92:	f8dc f000 	ldr.w	pc, [ip]
 8020d96:	bf00      	nop

08020d98 <_ZN4BIOS3LCD5PrintEiittPKc@plt>:
 8020d98:	f246 3c00 	movw	ip, #25344	; 0x6300
 8020d9c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020da0:	44fc      	add	ip, pc
 8020da2:	f8dc f000 	ldr.w	pc, [ip]
 8020da6:	bf00      	nop

08020da8 <_ZN4BIOS3KEY6GetKeyEv@plt>:
 8020da8:	f246 2cf4 	movw	ip, #25332	; 0x62f4
 8020dac:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020db0:	44fc      	add	ip, pc
 8020db2:	f8dc f000 	ldr.w	pc, [ip]
 8020db6:	bf00      	nop

08020db8 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>:
 8020db8:	f246 2ce8 	movw	ip, #25320	; 0x62e8
 8020dbc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020dc0:	44fc      	add	ip, pc
 8020dc2:	f8dc f000 	ldr.w	pc, [ip]
 8020dc6:	bf00      	nop

08020dc8 <_ZN4BIOS3LCD4DrawEiittPKc@plt>:
 8020dc8:	f246 2cdc 	movw	ip, #25308	; 0x62dc
 8020dcc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020dd0:	44fc      	add	ip, pc
 8020dd2:	f8dc f000 	ldr.w	pc, [ip]
 8020dd6:	bf00      	nop

08020dd8 <_Z7sprintfPcPKcz@plt>:
 8020dd8:	f246 2cd0 	movw	ip, #25296	; 0x62d0
 8020ddc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020de0:	44fc      	add	ip, pc
 8020de2:	f8dc f000 	ldr.w	pc, [ip]
 8020de6:	bf00      	nop

08020de8 <_ZN3GUI6WindowERK5CRectt@plt>:
 8020de8:	f246 2cc4 	movw	ip, #25284	; 0x62c4
 8020dec:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020df0:	44fc      	add	ip, pc
 8020df2:	f8dc f000 	ldr.w	pc, [ip]
 8020df6:	bf00      	nop
