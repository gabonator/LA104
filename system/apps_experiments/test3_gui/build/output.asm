
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08020000 <main>:
 8020000:	b507      	push	{r0, r1, r2, lr}
 8020002:	a801      	add	r0, sp, #4
 8020004:	f000 f808 	bl	8020018 <_ZN14CVisualMeasure7OnPaintEv>
 8020008:	f000 fc52 	bl	80208b0 <_ZN4BIOS3KEY6GetKeyEv@plt>
 802000c:	2800      	cmp	r0, #0
 802000e:	d0fb      	beq.n	8020008 <main+0x8>
 8020010:	2000      	movs	r0, #0
 8020012:	b003      	add	sp, #12
 8020014:	f85d fb04 	ldr.w	pc, [sp], #4

08020018 <_ZN14CVisualMeasure7OnPaintEv>:
 8020018:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
 802001c:	2400      	movs	r4, #0
 802001e:	b08b      	sub	sp, #44	; 0x2c
 8020020:	4bdf      	ldr	r3, [pc, #892]	; (80203a0 <_ZN14CVisualMeasure7OnPaintEv+0x388>)
 8020022:	a804      	add	r0, sp, #16
 8020024:	f241 0282 	movw	r2, #4226	; 0x1082
 8020028:	f244 2108 	movw	r1, #16904	; 0x4208
 802002c:	e9cd 4304 	strd	r4, r3, [sp, #16]
 8020030:	f000 fc2e 	bl	8020890 <_ZN3GUI10BackgroundER5CRectmm@plt>
 8020034:	4bdb      	ldr	r3, [pc, #876]	; (80203a4 <_ZN14CVisualMeasure7OnPaintEv+0x38c>)
 8020036:	a806      	add	r0, sp, #24
 8020038:	f244 2208 	movw	r2, #16904	; 0x4208
 802003c:	f24b 2108 	movw	r1, #45576	; 0xb208
 8020040:	9307      	str	r3, [sp, #28]
 8020042:	9406      	str	r4, [sp, #24]
 8020044:	f000 fc24 	bl	8020890 <_ZN3GUI10BackgroundER5CRectmm@plt>
 8020048:	4ad7      	ldr	r2, [pc, #860]	; (80203a8 <_ZN14CVisualMeasure7OnPaintEv+0x390>)
 802004a:	4bd8      	ldr	r3, [pc, #864]	; (80203ac <_ZN14CVisualMeasure7OnPaintEv+0x394>)
 802004c:	a808      	add	r0, sp, #32
 802004e:	f64f 71ff 	movw	r1, #65535	; 0xffff
 8020052:	e9cd 2308 	strd	r2, r3, [sp, #32]
 8020056:	f000 fc3b 	bl	80208d0 <_ZN3GUI6WindowERK5CRectt@plt>
 802005a:	4bd5      	ldr	r3, [pc, #852]	; (80203b0 <_ZN14CVisualMeasure7OnPaintEv+0x398>)
 802005c:	4622      	mov	r2, r4
 802005e:	9300      	str	r3, [sp, #0]
 8020060:	4621      	mov	r1, r4
 8020062:	f24b 5396 	movw	r3, #46486	; 0xb596
 8020066:	4620      	mov	r0, r4
 8020068:	f000 fc1a 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 802006c:	f8df 8374 	ldr.w	r8, [pc, #884]	; 80203e4 <_ZN14CVisualMeasure7OnPaintEv+0x3cc>
 8020070:	4621      	mov	r1, r4
 8020072:	f8cd 8000 	str.w	r8, [sp]
 8020076:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802007a:	f24b 5296 	movw	r2, #46486	; 0xb596
 802007e:	4605      	mov	r5, r0
 8020080:	f000 fc1e 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020084:	4405      	add	r5, r0
 8020086:	f108 0372 	add.w	r3, r8, #114	; 0x72
 802008a:	3508      	adds	r5, #8
 802008c:	9300      	str	r3, [sp, #0]
 802008e:	4621      	mov	r1, r4
 8020090:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020094:	f24b 5296 	movw	r2, #46486	; 0xb596
 8020098:	4628      	mov	r0, r5
 802009a:	f000 fc11 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 802009e:	4bc5      	ldr	r3, [pc, #788]	; (80203b4 <_ZN14CVisualMeasure7OnPaintEv+0x39c>)
 80200a0:	4405      	add	r5, r0
 80200a2:	9300      	str	r3, [sp, #0]
 80200a4:	4622      	mov	r2, r4
 80200a6:	4621      	mov	r1, r4
 80200a8:	f24b 5396 	movw	r3, #46486	; 0xb596
 80200ac:	4628      	mov	r0, r5
 80200ae:	f000 fbf7 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80200b2:	4405      	add	r5, r0
 80200b4:	f8cd 8000 	str.w	r8, [sp]
 80200b8:	4621      	mov	r1, r4
 80200ba:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80200be:	f24b 5296 	movw	r2, #46486	; 0xb596
 80200c2:	4628      	mov	r0, r5
 80200c4:	f000 fbfc 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 80200c8:	4bbb      	ldr	r3, [pc, #748]	; (80203b8 <_ZN14CVisualMeasure7OnPaintEv+0x3a0>)
 80200ca:	4405      	add	r5, r0
 80200cc:	3508      	adds	r5, #8
 80200ce:	9300      	str	r3, [sp, #0]
 80200d0:	4621      	mov	r1, r4
 80200d2:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80200d6:	f24b 5296 	movw	r2, #46486	; 0xb596
 80200da:	4628      	mov	r0, r5
 80200dc:	f000 fbe0 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80200e0:	4bb6      	ldr	r3, [pc, #728]	; (80203bc <_ZN14CVisualMeasure7OnPaintEv+0x3a4>)
 80200e2:	4428      	add	r0, r5
 80200e4:	9300      	str	r3, [sp, #0]
 80200e6:	4621      	mov	r1, r4
 80200e8:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80200ec:	f24b 5296 	movw	r2, #46486	; 0xb596
 80200f0:	3010      	adds	r0, #16
 80200f2:	f000 fbd5 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80200f6:	2202      	movs	r2, #2
 80200f8:	f9bd 0024 	ldrsh.w	r0, [sp, #36]	; 0x24
 80200fc:	f9bd 3020 	ldrsh.w	r3, [sp, #32]
 8020100:	fb90 f0f2 	sdiv	r0, r0, r2
 8020104:	fb93 f3f2 	sdiv	r3, r3, r2
 8020108:	f9bd 1022 	ldrsh.w	r1, [sp, #34]	; 0x22
 802010c:	4418      	add	r0, r3
 802010e:	4bac      	ldr	r3, [pc, #688]	; (80203c0 <_ZN14CVisualMeasure7OnPaintEv+0x3a8>)
 8020110:	4622      	mov	r2, r4
 8020112:	9300      	str	r3, [sp, #0]
 8020114:	3102      	adds	r1, #2
 8020116:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802011a:	3834      	subs	r0, #52	; 0x34
 802011c:	f000 fbc0 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020120:	f9bd 7022 	ldrsh.w	r7, [sp, #34]	; 0x22
 8020124:	f9bd 3020 	ldrsh.w	r3, [sp, #32]
 8020128:	f107 051a 	add.w	r5, r7, #26
 802012c:	f103 0608 	add.w	r6, r3, #8
 8020130:	9302      	str	r3, [sp, #8]
 8020132:	f108 03e4 	add.w	r3, r8, #228	; 0xe4
 8020136:	9300      	str	r3, [sp, #0]
 8020138:	4622      	mov	r2, r4
 802013a:	4629      	mov	r1, r5
 802013c:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020140:	4630      	mov	r0, r6
 8020142:	f000 fbbd 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020146:	f508 71ab 	add.w	r1, r8, #342	; 0x156
 802014a:	9100      	str	r1, [sp, #0]
 802014c:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020150:	4629      	mov	r1, r5
 8020152:	f44f 62fc 	mov.w	r2, #2016	; 0x7e0
 8020156:	4630      	mov	r0, r6
 8020158:	f000 fbb2 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 802015c:	4b99      	ldr	r3, [pc, #612]	; (80203c4 <_ZN14CVisualMeasure7OnPaintEv+0x3ac>)
 802015e:	4430      	add	r0, r6
 8020160:	9300      	str	r3, [sp, #0]
 8020162:	4629      	mov	r1, r5
 8020164:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020168:	f64f 72ff 	movw	r2, #65535	; 0xffff
 802016c:	3004      	adds	r0, #4
 802016e:	f000 fb97 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020172:	9802      	ldr	r0, [sp, #8]
 8020174:	f107 0a2a 	add.w	sl, r7, #42	; 0x2a
 8020178:	f100 0518 	add.w	r5, r0, #24
 802017c:	f508 73e4 	add.w	r3, r8, #456	; 0x1c8
 8020180:	f508 7b12 	add.w	fp, r8, #584	; 0x248
 8020184:	9300      	str	r3, [sp, #0]
 8020186:	4622      	mov	r2, r4
 8020188:	4651      	mov	r1, sl
 802018a:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802018e:	4628      	mov	r0, r5
 8020190:	f000 fb96 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020194:	4651      	mov	r1, sl
 8020196:	f8cd b000 	str.w	fp, [sp]
 802019a:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802019e:	f44f 4278 	mov.w	r2, #63488	; 0xf800
 80201a2:	4628      	mov	r0, r5
 80201a4:	f000 fb8c 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 80201a8:	eb05 0900 	add.w	r9, r5, r0
 80201ac:	4886      	ldr	r0, [pc, #536]	; (80203c8 <_ZN14CVisualMeasure7OnPaintEv+0x3b0>)
 80201ae:	f109 0903 	add.w	r9, r9, #3
 80201b2:	9000      	str	r0, [sp, #0]
 80201b4:	4622      	mov	r2, r4
 80201b6:	4651      	mov	r1, sl
 80201b8:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80201bc:	4648      	mov	r0, r9
 80201be:	f000 fb6f 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80201c2:	f508 7232 	add.w	r2, r8, #712	; 0x2c8
 80201c6:	4481      	add	r9, r0
 80201c8:	9203      	str	r2, [sp, #12]
 80201ca:	9200      	str	r2, [sp, #0]
 80201cc:	4651      	mov	r1, sl
 80201ce:	4622      	mov	r2, r4
 80201d0:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80201d4:	4648      	mov	r0, r9
 80201d6:	f000 fb73 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 80201da:	4a7c      	ldr	r2, [pc, #496]	; (80203cc <_ZN14CVisualMeasure7OnPaintEv+0x3b4>)
 80201dc:	4651      	mov	r1, sl
 80201de:	9200      	str	r2, [sp, #0]
 80201e0:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80201e4:	4622      	mov	r2, r4
 80201e6:	4448      	add	r0, r9
 80201e8:	f000 fb5a 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80201ec:	f107 093a 	add.w	r9, r7, #58	; 0x3a
 80201f0:	f508 73e4 	add.w	r3, r8, #456	; 0x1c8
 80201f4:	9300      	str	r3, [sp, #0]
 80201f6:	4622      	mov	r2, r4
 80201f8:	4649      	mov	r1, r9
 80201fa:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80201fe:	4628      	mov	r0, r5
 8020200:	f000 fb5e 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020204:	4649      	mov	r1, r9
 8020206:	f8cd b000 	str.w	fp, [sp]
 802020a:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802020e:	f44f 62fc 	mov.w	r2, #2016	; 0x7e0
 8020212:	4628      	mov	r0, r5
 8020214:	f000 fb54 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020218:	4428      	add	r0, r5
 802021a:	f100 0803 	add.w	r8, r0, #3
 802021e:	486a      	ldr	r0, [pc, #424]	; (80203c8 <_ZN14CVisualMeasure7OnPaintEv+0x3b0>)
 8020220:	4622      	mov	r2, r4
 8020222:	9000      	str	r0, [sp, #0]
 8020224:	4649      	mov	r1, r9
 8020226:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802022a:	4640      	mov	r0, r8
 802022c:	f000 fb38 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020230:	4480      	add	r8, r0
 8020232:	9803      	ldr	r0, [sp, #12]
 8020234:	4622      	mov	r2, r4
 8020236:	9000      	str	r0, [sp, #0]
 8020238:	4649      	mov	r1, r9
 802023a:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802023e:	4640      	mov	r0, r8
 8020240:	f000 fb3e 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020244:	4a61      	ldr	r2, [pc, #388]	; (80203cc <_ZN14CVisualMeasure7OnPaintEv+0x3b4>)
 8020246:	4649      	mov	r1, r9
 8020248:	9200      	str	r2, [sp, #0]
 802024a:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802024e:	4622      	mov	r2, r4
 8020250:	4440      	add	r0, r8
 8020252:	f000 fb25 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020256:	f107 084a 	add.w	r8, r7, #74	; 0x4a
 802025a:	f5ab 72b2 	sub.w	r2, fp, #356	; 0x164
 802025e:	9200      	str	r2, [sp, #0]
 8020260:	4641      	mov	r1, r8
 8020262:	4622      	mov	r2, r4
 8020264:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020268:	4630      	mov	r0, r6
 802026a:	f000 fb29 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 802026e:	f1ab 01f2 	sub.w	r1, fp, #242	; 0xf2
 8020272:	9100      	str	r1, [sp, #0]
 8020274:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020278:	4641      	mov	r1, r8
 802027a:	f44f 62fc 	mov.w	r2, #2016	; 0x7e0
 802027e:	4630      	mov	r0, r6
 8020280:	f000 fb1e 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020284:	4b52      	ldr	r3, [pc, #328]	; (80203d0 <_ZN14CVisualMeasure7OnPaintEv+0x3b8>)
 8020286:	4430      	add	r0, r6
 8020288:	9300      	str	r3, [sp, #0]
 802028a:	4641      	mov	r1, r8
 802028c:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020290:	f64f 72ff 	movw	r2, #65535	; 0xffff
 8020294:	3004      	adds	r0, #4
 8020296:	f000 fb03 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 802029a:	f107 095a 	add.w	r9, r7, #90	; 0x5a
 802029e:	f1ab 0380 	sub.w	r3, fp, #128	; 0x80
 80202a2:	9300      	str	r3, [sp, #0]
 80202a4:	4622      	mov	r2, r4
 80202a6:	4649      	mov	r1, r9
 80202a8:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80202ac:	4628      	mov	r0, r5
 80202ae:	f000 fb07 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 80202b2:	4649      	mov	r1, r9
 80202b4:	f8cd b000 	str.w	fp, [sp]
 80202b8:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80202bc:	221f      	movs	r2, #31
 80202be:	4628      	mov	r0, r5
 80202c0:	f000 fafe 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 80202c4:	4b43      	ldr	r3, [pc, #268]	; (80203d4 <_ZN14CVisualMeasure7OnPaintEv+0x3bc>)
 80202c6:	eb05 0800 	add.w	r8, r5, r0
 80202ca:	f108 0803 	add.w	r8, r8, #3
 80202ce:	9300      	str	r3, [sp, #0]
 80202d0:	4622      	mov	r2, r4
 80202d2:	4649      	mov	r1, r9
 80202d4:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80202d8:	4640      	mov	r0, r8
 80202da:	f000 fae1 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80202de:	4480      	add	r8, r0
 80202e0:	9803      	ldr	r0, [sp, #12]
 80202e2:	4622      	mov	r2, r4
 80202e4:	9000      	str	r0, [sp, #0]
 80202e6:	4649      	mov	r1, r9
 80202e8:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80202ec:	4640      	mov	r0, r8
 80202ee:	f000 fae7 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 80202f2:	4a36      	ldr	r2, [pc, #216]	; (80203cc <_ZN14CVisualMeasure7OnPaintEv+0x3b4>)
 80202f4:	4649      	mov	r1, r9
 80202f6:	9200      	str	r2, [sp, #0]
 80202f8:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80202fc:	4622      	mov	r2, r4
 80202fe:	4440      	add	r0, r8
 8020300:	f000 face 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020304:	f107 086a 	add.w	r8, r7, #106	; 0x6a
 8020308:	f1ab 0380 	sub.w	r3, fp, #128	; 0x80
 802030c:	9300      	str	r3, [sp, #0]
 802030e:	4622      	mov	r2, r4
 8020310:	4641      	mov	r1, r8
 8020312:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020316:	4628      	mov	r0, r5
 8020318:	f000 fad2 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 802031c:	4641      	mov	r1, r8
 802031e:	f8cd b000 	str.w	fp, [sp]
 8020322:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020326:	f240 72ff 	movw	r2, #2047	; 0x7ff
 802032a:	4628      	mov	r0, r5
 802032c:	f000 fac8 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020330:	4b29      	ldr	r3, [pc, #164]	; (80203d8 <_ZN14CVisualMeasure7OnPaintEv+0x3c0>)
 8020332:	4405      	add	r5, r0
 8020334:	3504      	adds	r5, #4
 8020336:	9300      	str	r3, [sp, #0]
 8020338:	4622      	mov	r2, r4
 802033a:	4641      	mov	r1, r8
 802033c:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020340:	4628      	mov	r0, r5
 8020342:	f000 faad 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020346:	4b25      	ldr	r3, [pc, #148]	; (80203dc <_ZN14CVisualMeasure7OnPaintEv+0x3c4>)
 8020348:	4622      	mov	r2, r4
 802034a:	9300      	str	r3, [sp, #0]
 802034c:	4641      	mov	r1, r8
 802034e:	f64f 031e 	movw	r3, #63518	; 0xf81e
 8020352:	4428      	add	r0, r5
 8020354:	f000 faa4 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 8020358:	f107 057a 	add.w	r5, r7, #122	; 0x7a
 802035c:	f5ab 72b2 	sub.w	r2, fp, #356	; 0x164
 8020360:	9200      	str	r2, [sp, #0]
 8020362:	4629      	mov	r1, r5
 8020364:	4622      	mov	r2, r4
 8020366:	f64f 73ff 	movw	r3, #65535	; 0xffff
 802036a:	4630      	mov	r0, r6
 802036c:	f000 faa8 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020370:	f1ab 01f2 	sub.w	r1, fp, #242	; 0xf2
 8020374:	9100      	str	r1, [sp, #0]
 8020376:	f64f 031e 	movw	r3, #63518	; 0xf81e
 802037a:	4629      	mov	r1, r5
 802037c:	f44f 62fc 	mov.w	r2, #2016	; 0x7e0
 8020380:	4630      	mov	r0, r6
 8020382:	f000 fa9d 	bl	80208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>
 8020386:	4b16      	ldr	r3, [pc, #88]	; (80203e0 <_ZN14CVisualMeasure7OnPaintEv+0x3c8>)
 8020388:	4430      	add	r0, r6
 802038a:	9300      	str	r3, [sp, #0]
 802038c:	4622      	mov	r2, r4
 802038e:	4629      	mov	r1, r5
 8020390:	f64f 73ff 	movw	r3, #65535	; 0xffff
 8020394:	3004      	adds	r0, #4
 8020396:	f000 fa83 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 802039a:	9802      	ldr	r0, [sp, #8]
 802039c:	e024      	b.n	80203e8 <_ZN14CVisualMeasure7OnPaintEv+0x3d0>
 802039e:	bf00      	nop
 80203a0:	00f00140 	.word	0x00f00140
 80203a4:	000e0140 	.word	0x000e0140
 80203a8:	00240014 	.word	0x00240014
 80203ac:	00dc012c 	.word	0x00dc012c
 80203b0:	08020734 	.word	0x08020734
 80203b4:	08020743 	.word	0x08020743
 80203b8:	0802074a 	.word	0x0802074a
 80203bc:	08020750 	.word	0x08020750
 80203c0:	08020756 	.word	0x08020756
 80203c4:	08020764 	.word	0x08020764
 80203c8:	08020772 	.word	0x08020772
 80203cc:	0802078b 	.word	0x0802078b
 80203d0:	0802078d 	.word	0x0802078d
 80203d4:	08020797 	.word	0x08020797
 80203d8:	080207a7 	.word	0x080207a7
 80203dc:	080207b4 	.word	0x080207b4
 80203e0:	080207b6 	.word	0x080207b6
 80203e4:	08020408 	.word	0x08020408
 80203e8:	4b06      	ldr	r3, [pc, #24]	; (8020404 <_ZN14CVisualMeasure7OnPaintEv+0x3ec>)
 80203ea:	4622      	mov	r2, r4
 80203ec:	9300      	str	r3, [sp, #0]
 80203ee:	f107 018a 	add.w	r1, r7, #138	; 0x8a
 80203f2:	f64f 031e 	movw	r3, #63518	; 0xf81e
 80203f6:	3014      	adds	r0, #20
 80203f8:	f000 fa52 	bl	80208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>
 80203fc:	b00b      	add	sp, #44	; 0x2c
 80203fe:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
 8020402:	bf00      	nop
 8020404:	080207c5 	.word	0x080207c5

08020408 <_ZL17CShapes_sel_right>:
 8020408:	2e2e2e0e 2e2e2e2e 2e2e2e2e 2e2e2e2e     ................
 8020418:	2e2e2e2e 2e2e2e2e 2e2e2e2e 2e2e202e     ............. ..
 8020428:	2e2e2e2e 2e2e2e2e 20202e2e 2e2e2e20     ..........   ...
 8020438:	2e2e2e2e 202e2e2e 20202020 20202020     .......         
 8020448:	20202020 20202020 20202020 20202020                     
 8020458:	20202020 20202020 20202020 20202020                     
 8020468:	20202020 20202020 20202020 20202020                     
 8020478:	                                          .

0802047a <_ZL16CShapes_sel_left>:
 802047a:	2020200e 20202020 20202020 20202020     .               
 802048a:	20202020 20202020 20202020 20202020                     
 802049a:	20202020 20202020 20202020 20202020                     
 80204aa:	20202020 20202020 2e202020 2e2e2e2e                .....
 80204ba:	2e2e2e2e 2020202e 2e2e2e2e 2e2e2e2e     .....   ........
 80204ca:	2e2e2e2e 2e2e2e20 2e2e2e2e 2e2e2e2e     .... ...........
 80204da:	2e2e2e2e 2e2e2e2e 2e2e2e2e 2e2e2e2e     ................
 80204ea:	                                         ..

080204ec <_ZL17CShapes_check_box>:
 80204ec:	2020200e 20202020 20202020 20202020     .               
 80204fc:	2e202020 2e2e2e2e 20202e2e 20202020        .......      
 802050c:	20202e20 2e202020 20202020 2e202020      .     .       .
 802051c:	20202020 20202e20 20202020 20202e20          .       .  
 802052c:	2e202020 20202020 2e202020 20202020        .       .    
 802053c:	20202e20 20202020 2e2e2e20 2e2e2e2e      .       .......
 802054c:	20202020 20202020 20202020 20202020                     
 802055c:	                                          .

0802055e <_ZL16CShapes_check_on>:
 802055e:	2020200e 20202020 20202e2e 20202020     .       ..      
 802056e:	20202020 2e202020 2020202e 20202020            ..       
 802057e:	20202020 2e2e2020 20202020 20202020           ..        
 802058e:	20202020 202e2e20 20202020 20202020          ..         
 802059e:	2e2e2e20 20202020 20202020 2e2e2e2e      ...        ....
 80205ae:	20202020 20202020 202e2e2e 20202020             ...     
 80205be:	20202020 20202020 20202020 20202020                     
 80205ce:	                                          .

080205d0 <_ZL14CShapes_dotout>:
 80205d0:	2020200e 2e2e2e20 20202e2e 20202020     .    .....      
 80205e0:	2e2e2020 2e2d2d2d 2020202e 2e202020       ..---..      .
 80205f0:	2d2d2d2e 2e2e2d2d 20202020 2d2d2e20     .-----..     .--
 8020600:	2d2d2d2d 20202e2d 2e202020 2d2d2d2d     -----.     .----
 8020610:	2e2d2d2d 20202020 2d2d2e20 2d2d2d2d     ---.     .------
 8020620:	20202e2d 2e202020 2d2d2d2e 2e2e2d2d     -.     ..-----..
 8020630:	20202020 2e2e2020 2e2d2d2d 2020202e           ..---..   
 8020640:	20202020 2e2e2e20 20202e2e 00202020          .....     .

08020650 <_ZL11CShapes_dot>:
 8020650:	2020200e 20202020 20202020 20202020     .               
 8020660:	20202020 202e2e2e 20202020 20202020         ...         
 8020670:	2e2e2e20 20202e2e 20202020 2e2e2020      .....        ..
 8020680:	2e2e2e2e 2020202e 20202020 2e2e2e2e     .....       ....
 8020690:	202e2e2e 20202020 2e2e2020 2e2e2e2e     ...       ......
 80206a0:	2020202e 20202020 2e2e2e20 20202e2e     .        .....  
 80206b0:	20202020 20202020 202e2e2e 20202020             ...     
 80206c0:	20202020 20202020 20202020 00202020                    .

080206d0 <_ZL14CShapes_degree>:
 80206d0:	2020200e 20202020 20202020 20202020     .               
 80206e0:	20202020 20202020 20202020 20202020                     
 80206f0:	20202020 20202020 20202020 2e2e2020                   ..
 8020700:	20202020 20202020 2e202020 202e2020                .  . 
 8020710:	20202020 20202020 20202e20 2020202e              .  .   
 8020720:	20202020 20202020 20202e2e 20202020             ..      
 8020730:	00202020 73695620 4d6c6175 75736165        . VisualMeasu
 8020740:	43006572 69666e6f 72470067 00687061     re.Config.Graph.
 8020750:	74617453 6f430073 6769666e 74617275     Stats.Configurat
 8020760:	006e6f69 57656e4f 20657269 50207461     ion.OneWire at P
 8020770:	30230031 30303030 30303030 44203a30     1.#0000000000: D
 8020780:	53383153 20203032 43003532 54484400     S18S20  25.C.DHT
 8020790:	20746120 54003250 65706d65 75746172      at P2.Temperatu
 80207a0:	203a6572 48003731 64696d75 3a797469     re: 17.Humidity:
 80207b0:	00353620 4d420025 74612050 20335020      65.%.BMP at P3 
 80207c0:	34502026 746f4e00 756f6620               & P4.Not found.

Disassembly of section .plt:

08020880 <.plt>:
 8020880:	b500      	push	{lr}
 8020882:	f8df e008 	ldr.w	lr, [pc, #8]	; 802088c <.plt+0xc>
 8020886:	44fe      	add	lr, pc
 8020888:	f85e ff08 	ldr.w	pc, [lr, #8]!
 802088c:	17fe67fc 	.word	0x17fe67fc

08020890 <_ZN3GUI10BackgroundER5CRectmm@plt>:
 8020890:	f246 7cf8 	movw	ip, #26616	; 0x67f8
 8020894:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020898:	44fc      	add	ip, pc
 802089a:	f8dc f000 	ldr.w	pc, [ip]
 802089e:	bf00      	nop

080208a0 <_ZN4BIOS3LCD5PrintEiittPKc@plt>:
 80208a0:	f246 7cec 	movw	ip, #26604	; 0x67ec
 80208a4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80208a8:	44fc      	add	ip, pc
 80208aa:	f8dc f000 	ldr.w	pc, [ip]
 80208ae:	bf00      	nop

080208b0 <_ZN4BIOS3KEY6GetKeyEv@plt>:
 80208b0:	f246 7ce0 	movw	ip, #26592	; 0x67e0
 80208b4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80208b8:	44fc      	add	ip, pc
 80208ba:	f8dc f000 	ldr.w	pc, [ip]
 80208be:	bf00      	nop

080208c0 <_ZN4BIOS3LCD4DrawEiittPKc@plt>:
 80208c0:	f246 7cd4 	movw	ip, #26580	; 0x67d4
 80208c4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80208c8:	44fc      	add	ip, pc
 80208ca:	f8dc f000 	ldr.w	pc, [ip]
 80208ce:	bf00      	nop

080208d0 <_ZN3GUI6WindowERK5CRectt@plt>:
 80208d0:	f246 7cc8 	movw	ip, #26568	; 0x67c8
 80208d4:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80208d8:	44fc      	add	ip, pc
 80208da:	f8dc f000 	ldr.w	pc, [ip]
 80208de:	bf00      	nop
