
output.elf:     file format elf32-littlearm


Disassembly of section .text:

20007000 <main>:
20007000:	b573      	push	{r0, r1, r4, r5, r6, lr}
20007002:	240a      	movs	r4, #10
20007004:	2500      	movs	r5, #0
20007006:	4e10      	ldr	r6, [pc, #64]	; (20007048 <main+0x48>)
20007008:	2201      	movs	r2, #1
2000700a:	210a      	movs	r1, #10
2000700c:	4610      	mov	r0, r2
2000700e:	f000 f85b 	bl	200070c8 <pinWrite>
20007012:	9500      	str	r5, [sp, #0]
20007014:	9b00      	ldr	r3, [sp, #0]
20007016:	42b3      	cmp	r3, r6
20007018:	d90d      	bls.n	20007036 <main+0x36>
2000701a:	2200      	movs	r2, #0
2000701c:	210a      	movs	r1, #10
2000701e:	2001      	movs	r0, #1
20007020:	f000 f852 	bl	200070c8 <pinWrite>
20007024:	9501      	str	r5, [sp, #4]
20007026:	9b01      	ldr	r3, [sp, #4]
20007028:	42b3      	cmp	r3, r6
2000702a:	d908      	bls.n	2000703e <main+0x3e>
2000702c:	3c01      	subs	r4, #1
2000702e:	d1eb      	bne.n	20007008 <main+0x8>
20007030:	4620      	mov	r0, r4
20007032:	b002      	add	sp, #8
20007034:	bd70      	pop	{r4, r5, r6, pc}
20007036:	9b00      	ldr	r3, [sp, #0]
20007038:	3301      	adds	r3, #1
2000703a:	9300      	str	r3, [sp, #0]
2000703c:	e7ea      	b.n	20007014 <main+0x14>
2000703e:	9b01      	ldr	r3, [sp, #4]
20007040:	3301      	adds	r3, #1
20007042:	9301      	str	r3, [sp, #4]
20007044:	e7ef      	b.n	20007026 <main+0x26>
20007046:	bf00      	nop
20007048:	000f423f 	.word	0x000f423f

2000704c <GetRegister>:
2000704c:	4b02      	ldr	r3, [pc, #8]	; (20007058 <GetRegister+0xc>)
2000704e:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
20007052:	ea40 0081 	orr.w	r0, r0, r1, lsl #2
20007056:	4770      	bx	lr
20007058:	200070e8 	.word	0x200070e8

2000705c <SetState>:
2000705c:	4b0b      	ldr	r3, [pc, #44]	; (2000708c <SetState+0x30>)
2000705e:	2907      	cmp	r1, #7
20007060:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
20007064:	f04f 030f 	mov.w	r3, #15
20007068:	bfc8      	it	gt
2000706a:	f001 0107 	andgt.w	r1, r1, #7
2000706e:	b510      	push	{r4, lr}
20007070:	bfc8      	it	gt
20007072:	3004      	addgt	r0, #4
20007074:	0089      	lsls	r1, r1, #2
20007076:	fa03 f401 	lsl.w	r4, r3, r1
2000707a:	6803      	ldr	r3, [r0, #0]
2000707c:	fa02 f101 	lsl.w	r1, r2, r1
20007080:	ea23 0304 	bic.w	r3, r3, r4
20007084:	4319      	orrs	r1, r3
20007086:	6001      	str	r1, [r0, #0]
20007088:	bd10      	pop	{r4, pc}
2000708a:	bf00      	nop
2000708c:	200070e8 	.word	0x200070e8

20007090 <SetPin>:
20007090:	4b05      	ldr	r3, [pc, #20]	; (200070a8 <SetPin+0x18>)
20007092:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
20007096:	2301      	movs	r3, #1
20007098:	fa03 f101 	lsl.w	r1, r3, r1
2000709c:	b10a      	cbz	r2, 200070a2 <SetPin+0x12>
2000709e:	6101      	str	r1, [r0, #16]
200070a0:	4770      	bx	lr
200070a2:	6141      	str	r1, [r0, #20]
200070a4:	4770      	bx	lr
200070a6:	bf00      	nop
200070a8:	200070e8 	.word	0x200070e8

200070ac <GetPin>:
200070ac:	4b05      	ldr	r3, [pc, #20]	; (200070c4 <GetPin+0x18>)
200070ae:	f853 2020 	ldr.w	r2, [r3, r0, lsl #2]
200070b2:	2301      	movs	r3, #1
200070b4:	fa03 f101 	lsl.w	r1, r3, r1
200070b8:	6893      	ldr	r3, [r2, #8]
200070ba:	4219      	tst	r1, r3
200070bc:	bf14      	ite	ne
200070be:	2001      	movne	r0, #1
200070c0:	2000      	moveq	r0, #0
200070c2:	4770      	bx	lr
200070c4:	200070e8 	.word	0x200070e8

200070c8 <pinWrite>:
200070c8:	b570      	push	{r4, r5, r6, lr}
200070ca:	4616      	mov	r6, r2
200070cc:	4604      	mov	r4, r0
200070ce:	460d      	mov	r5, r1
200070d0:	2202      	movs	r2, #2
200070d2:	f7ff ffc3 	bl	2000705c <SetState>
200070d6:	1c32      	adds	r2, r6, #0
200070d8:	4629      	mov	r1, r5
200070da:	4620      	mov	r0, r4
200070dc:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
200070e0:	bf18      	it	ne
200070e2:	2201      	movne	r2, #1
200070e4:	f7ff bfd4 	b.w	20007090 <SetPin>

200070e8 <arrGpioBase>:
200070e8:	0800 4001 0c00 4001 1000 4001 1400 4001     ...@...@...@...@
200070f8:	1800 4001 1c00 4001 2000 4001               ...@...@. .@
