
output.elf:     file format elf32-littlearm


Disassembly of section .text:

08018000 <main>:
 8018000:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
 8018004:	f242 1004 	movw	r0, #8452	; 0x2104
 8018008:	f000 f8f6 	bl	80181f8 <_ZN4BIOS3LCD5ClearEt@plt>
 801800c:	481d      	ldr	r0, [pc, #116]	; (8018084 <main+0x84>)
 801800e:	f000 f8d3 	bl	80181b8 <_ZN4BIOS3DBG5PrintEPKcz@plt>
 8018012:	2100      	movs	r1, #0
 8018014:	f44f 5016 	mov.w	r0, #9600	; 0x2580
 8018018:	f000 f8e6 	bl	80181e8 <_ZN4BIOS4GPIO4UART5SetupEiNS1_7EConfigE@plt>
 801801c:	2140      	movs	r1, #64	; 0x40
 801801e:	2000      	movs	r0, #0
 8018020:	f000 f8fa 	bl	8018218 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>
 8018024:	2140      	movs	r1, #64	; 0x40
 8018026:	2001      	movs	r0, #1
 8018028:	f000 f8f6 	bl	8018218 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>
 801802c:	f000 f8fc 	bl	8018228 <_ZN4BIOS3SYS7GetTickEv@plt>
 8018030:	4604      	mov	r4, r0
 8018032:	4e15      	ldr	r6, [pc, #84]	; (8018088 <main+0x88>)
 8018034:	4f15      	ldr	r7, [pc, #84]	; (801808c <main+0x8c>)
 8018036:	f8df 805c 	ldr.w	r8, [pc, #92]	; 8018094 <main+0x94>
 801803a:	f000 f8cd 	bl	80181d8 <_ZN4BIOS3KEY6GetKeyEv@plt>
 801803e:	2806      	cmp	r0, #6
 8018040:	d01c      	beq.n	801807c <main+0x7c>
 8018042:	f000 f8f1 	bl	8018228 <_ZN4BIOS3SYS7GetTickEv@plt>
 8018046:	f504 737a 	add.w	r3, r4, #1000	; 0x3e8
 801804a:	4283      	cmp	r3, r0
 801804c:	4605      	mov	r5, r0
 801804e:	da09      	bge.n	8018064 <main+0x64>
 8018050:	4630      	mov	r0, r6
 8018052:	f000 f8b1 	bl	80181b8 <_ZN4BIOS3DBG5PrintEPKcz@plt>
 8018056:	4638      	mov	r0, r7
 8018058:	f000 f81e 	bl	8018098 <_Z11serialPrintPKc>
 801805c:	4640      	mov	r0, r8
 801805e:	f000 f8ab 	bl	80181b8 <_ZN4BIOS3DBG5PrintEPKcz@plt>
 8018062:	462c      	mov	r4, r5
 8018064:	4d0a      	ldr	r5, [pc, #40]	; (8018090 <main+0x90>)
 8018066:	f000 f89f 	bl	80181a8 <_ZN4BIOS4GPIO4UART9AvailableEv@plt>
 801806a:	2800      	cmp	r0, #0
 801806c:	d0e5      	beq.n	801803a <main+0x3a>
 801806e:	f000 f8cb 	bl	8018208 <_ZN4BIOS4GPIO4UART4ReadEv@plt>
 8018072:	4601      	mov	r1, r0
 8018074:	4628      	mov	r0, r5
 8018076:	f000 f89f 	bl	80181b8 <_ZN4BIOS3DBG5PrintEPKcz@plt>
 801807a:	e7f4      	b.n	8018066 <main+0x66>
 801807c:	2000      	movs	r0, #0
 801807e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
 8018082:	bf00      	nop
 8018084:	080180aa 	.word	0x080180aa
 8018088:	080180b1 	.word	0x080180b1
 801808c:	080180b8 	.word	0x080180b8
 8018090:	080180c4 	.word	0x080180c4
 8018094:	080180bf 	.word	0x080180bf

08018098 <_Z11serialPrintPKc>:
 8018098:	b510      	push	{r4, lr}
 801809a:	1e44      	subs	r4, r0, #1
 801809c:	f814 0f01 	ldrb.w	r0, [r4, #1]!
 80180a0:	b110      	cbz	r0, 80180a8 <_Z11serialPrintPKc+0x10>
 80180a2:	f000 f891 	bl	80181c8 <_ZN4BIOS4GPIO4UART5WriteEh@plt>
 80180a6:	e7f9      	b.n	801809c <_Z11serialPrintPKc+0x4>
 80180a8:	bd10      	pop	{r4, pc}
 80180aa:	7473      	.short	0x7473
 80180ac:	0a747261 	.word	0x0a747261
 80180b0:	65735b00 	.word	0x65735b00
 80180b4:	005d646e 	.word	0x005d646e
 80180b8:	6a6f6841 	.word	0x6a6f6841
 80180bc:	5b000a21 	.word	0x5b000a21
 80180c0:	005d6b6f 	.word	0x005d6b6f
 80180c4:	Address 0x00000000080180c4 is out of bounds.


Disassembly of section .plt:

08018198 <.plt>:
 8018198:	b500      	push	{lr}
 801819a:	f8df e008 	ldr.w	lr, [pc, #8]	; 80181a4 <.plt+0xc>
 801819e:	44fe      	add	lr, pc
 80181a0:	f85e ff08 	ldr.w	pc, [lr, #8]!
 80181a4:	17fecee4 	.word	0x17fecee4

080181a8 <_ZN4BIOS4GPIO4UART9AvailableEv@plt>:
 80181a8:	f64c 6ce0 	movw	ip, #52960	; 0xcee0
 80181ac:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80181b0:	44fc      	add	ip, pc
 80181b2:	f8dc f000 	ldr.w	pc, [ip]
 80181b6:	bf00      	nop

080181b8 <_ZN4BIOS3DBG5PrintEPKcz@plt>:
 80181b8:	f64c 6cd4 	movw	ip, #52948	; 0xced4
 80181bc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80181c0:	44fc      	add	ip, pc
 80181c2:	f8dc f000 	ldr.w	pc, [ip]
 80181c6:	bf00      	nop

080181c8 <_ZN4BIOS4GPIO4UART5WriteEh@plt>:
 80181c8:	f64c 6cc8 	movw	ip, #52936	; 0xcec8
 80181cc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80181d0:	44fc      	add	ip, pc
 80181d2:	f8dc f000 	ldr.w	pc, [ip]
 80181d6:	bf00      	nop

080181d8 <_ZN4BIOS3KEY6GetKeyEv@plt>:
 80181d8:	f64c 6cbc 	movw	ip, #52924	; 0xcebc
 80181dc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80181e0:	44fc      	add	ip, pc
 80181e2:	f8dc f000 	ldr.w	pc, [ip]
 80181e6:	bf00      	nop

080181e8 <_ZN4BIOS4GPIO4UART5SetupEiNS1_7EConfigE@plt>:
 80181e8:	f64c 6cb0 	movw	ip, #52912	; 0xceb0
 80181ec:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 80181f0:	44fc      	add	ip, pc
 80181f2:	f8dc f000 	ldr.w	pc, [ip]
 80181f6:	bf00      	nop

080181f8 <_ZN4BIOS3LCD5ClearEt@plt>:
 80181f8:	f64c 6ca4 	movw	ip, #52900	; 0xcea4
 80181fc:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018200:	44fc      	add	ip, pc
 8018202:	f8dc f000 	ldr.w	pc, [ip]
 8018206:	bf00      	nop

08018208 <_ZN4BIOS4GPIO4UART4ReadEv@plt>:
 8018208:	f64c 6c98 	movw	ip, #52888	; 0xce98
 801820c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018210:	44fc      	add	ip, pc
 8018212:	f8dc f000 	ldr.w	pc, [ip]
 8018216:	bf00      	nop

08018218 <_ZN4BIOS4GPIO7PinModeENS0_4EPinENS0_5EModeE@plt>:
 8018218:	f64c 6c8c 	movw	ip, #52876	; 0xce8c
 801821c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018220:	44fc      	add	ip, pc
 8018222:	f8dc f000 	ldr.w	pc, [ip]
 8018226:	bf00      	nop

08018228 <_ZN4BIOS3SYS7GetTickEv@plt>:
 8018228:	f64c 6c80 	movw	ip, #52864	; 0xce80
 801822c:	f2c1 7cfe 	movt	ip, #6142	; 0x17fe
 8018230:	44fc      	add	ip, pc
 8018232:	f8dc f000 	ldr.w	pc, [ip]
 8018236:	bf00      	nop
