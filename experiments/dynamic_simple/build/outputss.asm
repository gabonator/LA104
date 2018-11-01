
main.elf:     file format elf32-littlearm


Disassembly of section .text:

20005000 <main>:
20005000:	b508      	push	{r3, lr}
20005002:	f000 f86d 	bl	200050e0 <Test1@plt>
20005006:	f000 f863 	bl	200050d0 <Test2@plt>
2000500a:	f000 f879 	bl	20005100 <Test3@plt>
2000500e:	f000 f86f 	bl	200050f0 <Test4@plt>
20005012:	2001      	movs	r0, #1
20005014:	bd08      	pop	{r3, pc}

Disassembly of section .plt:

200050c0 <.plt>:
200050c0:	b500      	push	{lr}
200050c2:	f8df e008 	ldr.w	lr, [pc, #8]	; 200050cc <.plt+0xc>
200050c6:	44fe      	add	lr, pc
200050c8:	f85e ff08 	ldr.w	pc, [lr, #8]!
200050cc:	000000ec 	.word	0x000000ec

200050d0 <Test2@plt>:
200050d0:	f240 0ce8 	movw	ip, #232	; 0xe8
200050d4:	f2c0 0c00 	movt	ip, #0
200050d8:	44fc      	add	ip, pc
200050da:	f8dc f000 	ldr.w	pc, [ip]
200050de:	bf00      	nop

200050e0 <Test1@plt>:
200050e0:	f240 0cdc 	movw	ip, #220	; 0xdc
200050e4:	f2c0 0c00 	movt	ip, #0
200050e8:	44fc      	add	ip, pc
200050ea:	f8dc f000 	ldr.w	pc, [ip]
200050ee:	bf00      	nop

200050f0 <Test4@plt>:
200050f0:	f240 0cd0 	movw	ip, #208	; 0xd0
200050f4:	f2c0 0c00 	movt	ip, #0
200050f8:	44fc      	add	ip, pc
200050fa:	f8dc f000 	ldr.w	pc, [ip]
200050fe:	bf00      	nop

20005100 <Test3@plt>:
20005100:	f240 0cc4 	movw	ip, #196	; 0xc4
20005104:	f2c0 0c00 	movt	ip, #0
20005108:	44fc      	add	ip, pc
2000510a:	f8dc f000 	ldr.w	pc, [ip]
2000510e:	bf00      	nop
