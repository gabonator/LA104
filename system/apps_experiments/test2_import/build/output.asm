
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08020000 <main>:
 8020000:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
 8020004:	2400      	movs	r4, #0
 8020006:	4f2a      	ldr	r7, [pc, #168]	; (80200b0 <main+0xb0>)
 8020008:	b086      	sub	sp, #24
 802000a:	2500      	movs	r5, #0
 802000c:	f1a4 03a0 	sub.w	r3, r4, #160	; 0xa0
 8020010:	fb03 f603 	mul.w	r6, r3, r3
 8020014:	f1a5 0278 	sub.w	r2, r5, #120	; 0x78
 8020018:	fb02 6202 	mla	r2, r2, r2, r6
 802001c:	ea07 2202 	and.w	r2, r7, r2, lsl #8
 8020020:	4629      	mov	r1, r5
 8020022:	f402 4278 	and.w	r2, r2, #63488	; 0xf800
 8020026:	4620      	mov	r0, r4
 8020028:	3501      	adds	r5, #1
 802002a:	f000 f8a9 	bl	8020180 <_ZN4BIOS3LCD8PutPixelEiit@plt>
 802002e:	2df0      	cmp	r5, #240	; 0xf0
 8020030:	d1f0      	bne.n	8020014 <main+0x14>
 8020032:	3401      	adds	r4, #1
 8020034:	f5b4 7fa0 	cmp.w	r4, #320	; 0x140
 8020038:	d1e7      	bne.n	802000a <main+0xa>
 802003a:	466c      	mov	r4, sp
 802003c:	f241 4acb 	movw	sl, #5323	; 0x14cb
 8020040:	2614      	movs	r6, #20
 8020042:	250a      	movs	r5, #10
 8020044:	f04f 0803 	mov.w	r8, #3
 8020048:	2702      	movs	r7, #2
 802004a:	4b1a      	ldr	r3, [pc, #104]	; (80200b4 <main+0xb4>)
 802004c:	f8df 906c 	ldr.w	r9, [pc, #108]	; 80200bc <main+0xbc>
 8020050:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 8020054:	330c      	adds	r3, #12
 8020056:	e884 0007 	stmia.w	r4, {r0, r1, r2}
 802005a:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
 802005e:	ab06      	add	r3, sp, #24
 8020060:	e903 0007 	stmdb	r3, {r0, r1, r2}
 8020064:	4c14      	ldr	r4, [pc, #80]	; (80200b8 <main+0xb8>)
 8020066:	f000 f893 	bl	8020190 <_ZN4BIOS3KEY6GetKeyEv@plt>
 802006a:	b9e0      	cbnz	r0, 80200a6 <main+0xa6>
 802006c:	fb09 4a0a 	mla	sl, r9, sl, r4
 8020070:	f3ca 020e 	ubfx	r2, sl, #0, #15
 8020074:	fb92 f3f8 	sdiv	r3, r2, r8
 8020078:	eb03 0343 	add.w	r3, r3, r3, lsl #1
 802007c:	1ad3      	subs	r3, r2, r3
 802007e:	a906      	add	r1, sp, #24
 8020080:	eb01 0383 	add.w	r3, r1, r3, lsl #2
 8020084:	f853 1c18 	ldr.w	r1, [r3, #-24]
 8020088:	f853 3c0c 	ldr.w	r3, [r3, #-12]
 802008c:	440d      	add	r5, r1
 802008e:	441e      	add	r6, r3
 8020090:	fb95 f5f7 	sdiv	r5, r5, r7
 8020094:	fb96 f6f7 	sdiv	r6, r6, r7
 8020098:	0052      	lsls	r2, r2, #1
 802009a:	4631      	mov	r1, r6
 802009c:	4628      	mov	r0, r5
 802009e:	f000 f86f 	bl	8020180 <_ZN4BIOS3LCD8PutPixelEiit@plt>
 80200a2:	3401      	adds	r4, #1
 80200a4:	e7df      	b.n	8020066 <main+0x66>
 80200a6:	2000      	movs	r0, #0
 80200a8:	b006      	add	sp, #24
 80200aa:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
 80200ae:	bf00      	nop
 80200b0:	07fff800 	.word	0x07fff800
 80200b4:	080200c0 	.word	0x080200c0
 80200b8:	002490f3 	.word	0x002490f3
 80200bc:	007df121 	.word	0x007df121
 80200c0:	0000000a 	.word	0x0000000a
 80200c4:	00000136 	.word	0x00000136
 80200c8:	0000008c 	.word	0x0000008c
 80200cc:	00000014 	.word	0x00000014
 80200d0:	00000028 	.word	0x00000028
 80200d4:	000000dc 	.word	0x000000dc

Disassembly of section .plt:

08020170 <.plt>:
 8020170:	b500      	push	{lr}
 8020172:	f8df e008 	ldr.w	lr, [pc, #8]	; 802017c <.plt+0xc>
 8020176:	44fe      	add	lr, pc
 8020178:	f85e ff08 	ldr.w	pc, [lr, #8]!
 802017c:	17fe3f0c 	.word	0x17fe3f0c

08020180 <_ZN4BIOS3LCD8PutPixelEiit@plt>:
 8020180:	f643 7c08 	movw	ip, #16136	; 0x3f08
 8020184:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020188:	44fc      	add	ip, pc
 802018a:	f8dc f000 	ldr.w	pc, [ip]
 802018e:	bf00      	nop

08020190 <_ZN4BIOS3KEY6GetKeyEv@plt>:
 8020190:	f643 6cfc 	movw	ip, #16124	; 0x3efc
 8020194:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8020198:	44fc      	add	ip, pc
 802019a:	f8dc f000 	ldr.w	pc, [ip]
 802019e:	bf00      	nop
