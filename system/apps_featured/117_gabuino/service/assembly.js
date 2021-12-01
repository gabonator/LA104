var objdumpasm = process.argv[2];

var fs = require("fs");
var input = fs.readFileSync(objdumpasm).toString().split("\n")
var regAddress = RegExp("^[0-9a-f]{8}")
var regHeading = RegExp("^[0-9a-f]{8} <(.*)>:$")
var regAssembly = RegExp("^[0-9a-f]{8}:")
var regMethod = RegExp("^(.*\\(.*\\).*):$")
var regReference = RegExp(".*:[0-9]+( \\(discriminator \\d+\\))?$")
var regSkip = RegExp("^Disassembly of section")

var regExtractReference = RegExp(".*/(.*?):([0-9]+)( \\(discriminator \\d+\\))?$")

var codeReference = null;

for (var i=5; i<input.length; i++)
{
  var l = input[i];
  if (l.length == 0)
    continue;
  if (l.match(regSkip))
    continue;
  if (l == "\t...")
    continue;

  if (l.match(regAddress))
  {
    if (l.match(regHeading))
    {
//      codeReference = l.match(regHeading)[1];
      continue;
    }
    if (l.match(regAssembly))
    {
      if (codeReference)
      {
        console.log(l + " ; " + codeReference);
        codeReference = null;
      } else
        console.log(l);
      continue;
    }
    throw "Unmatched address based"
  } else {
    if (l.match(regReference))
    {
      var reference = l.match(regExtractReference);
      if (!reference || reference.length < 3)
        throw "Cannot parse reference"
      if (reference[1] == "test.cpp")
//        codeReference = reference[2];
        codeReference = reference[1] + ":" + reference[2];
      continue;
    }
    if (l.match(regMethod))
    {
//      codeReference = l.match(regMethod)[1];
      continue;
    }
  }
//  console.log("line " + i + ":");
//  console.log(l);
  throw "Unmatched line"
}

/*

test.elf:     file format elf32-littlearm


Disassembly of section .text:

20005000 <main>:
main():
/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service/build/../source/test.cpp:9
20005000:	b5b0      	push	{r4, r5, r7, lr}
20005002:	b092      	sub	sp, #72	; 0x48
20005004:	af02      	add	r7, sp, #8
/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service/build/../source/test.cpp:10
20005006:	f107 0320 	add.w	r3, r7, #32
2000500a:	4618      	mov	r0, r3
2000500c:	f000 f8e6 	bl	200051dc <CRect::CRect()>
/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service/build/../source/test.cpp:11
20005010:	f107 0010 	add.w	r0, r7, #16
20005014:	23e2      	movs	r3, #226	; 0xe2
20005016:	9300      	str	r3, [sp, #0]
20005018:	f44f 73a0 	mov.w	r3, #320	; 0x140
2000501c:	220e      	movs	r2, #14
2000501e:	2100      	movs	r1, #0
20005020:	f000 f8f2 	bl	20005208 <CRect::CRect(int, int, int, int)>
/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service/build/../source/test.cpp:12
20005024:	f107 0330 	add.w	r3, r7, #48	; 0x30
20005028:	f107 0210 	add.w	r2, r7, #16
2000502c:	4611      	mov	r1, r2
2000502e:	4618      	mov	r0, r3
20005030:	f000 f903 	bl	2000523a <CRect::Center()>
20005034:	6b3b      	ldr	r3, [r7, #48]	; 0x30
20005036:	4a5a      	ldr	r2, [pc, #360]	; (200051a0 <main+0x1a0>)
20005038:	6013      	str	r3, [r2, #0]
/Users/gabrielvalky/Documents/git/LA104/system/apps/000_service/build/../source/test.cpp:13
2000503a:	f107 0338 	add.w	r3, r7, #56	; 0x38
2000503e:	f107 0210 	add.w	r2, r7, #16
20005042:	4611      	mov	r1, r2
20005044:	4618      	mov	r0, r3
*/