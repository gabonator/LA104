# bez libm
set -e

mkdir -p build
cd build
# https://stackoverflow.com/questions/46160704/why-use-of-sqrtf-function-from-libgcc-a-causes-link-double-precision-function
arm-none-eabi-gcc -Wall -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -Wno-psabi -MD -DLA104 -DCIRCUITPY_ENABLE_MPY_NATIVE=0 -DCIRCUITPY_FULL_BUILD=0 -DMICROPY_ENABLE_COMPILER=1 -DMICROPY_ENABLE_EXTERNAL_IMPORT=0 -Wdouble-promotion -fno-math-errno -fsingle-precision-constant -Wdouble-promotion -c \
../source/circuitpython/py/mpstate.c \
../source/circuitpython/py/nlr.c \
../source/circuitpython/py/nlrx86.c \
../source/circuitpython/py/nlrx64.c \
../source/circuitpython/py/nlrthumb.c \
../source/circuitpython/py/nlrxtensa.c \
../source/circuitpython/py/nlrsetjmp.c \
../source/circuitpython/py/malloc.c \
../source/circuitpython/py/gc.c \
../source/circuitpython/py/pystack.c \
../source/circuitpython/py/qstr.c \
../source/circuitpython/py/vstr.c \
../source/circuitpython/py/mpprint.c \
../source/circuitpython/py/unicode.c \
../source/circuitpython/py/mpz.c \
../source/circuitpython/py/reader.c \
../source/circuitpython/py/lexer.c \
../source/circuitpython/py/parse.c \
../source/circuitpython/py/scope.c \
../source/circuitpython/py/compile.c \
../source/circuitpython/py/emitcommon.c \
../source/circuitpython/py/emitbc.c \
../source/circuitpython/py/asmbase.c \
../source/circuitpython/py/asmx64.c \
../source/circuitpython/py/emitnx64.c \
../source/circuitpython/py/asmx86.c \
../source/circuitpython/py/emitnx86.c \
../source/circuitpython/py/asmthumb.c \
../source/circuitpython/py/emitnthumb.c \
../source/circuitpython/py/emitinlinethumb.c \
../source/circuitpython/py/asmarm.c \
../source/circuitpython/py/emitnarm.c \
../source/circuitpython/py/asmxtensa.c \
../source/circuitpython/py/emitnxtensa.c \
../source/circuitpython/py/emitinlinextensa.c \
../source/circuitpython/py/formatfloat.c \
../source/circuitpython/py/parsenumbase.c \
../source/circuitpython/py/parsenum.c \
../source/circuitpython/py/emitglue.c \
../source/circuitpython/py/persistentcode.c \
../source/circuitpython/py/runtime.c \
../source/circuitpython/py/runtime_utils.c \
../source/circuitpython/py/scheduler.c \
../source/circuitpython/py/nativeglue.c \
../source/circuitpython/py/ringbuf.c \
../source/circuitpython/py/stackctrl.c \
../source/circuitpython/py/argcheck.c \
../source/circuitpython/py/warning.c \
../source/circuitpython/py/map.c \
../source/circuitpython/py/obj.c \
../source/circuitpython/py/objarray.c \
../source/circuitpython/py/objattrtuple.c \
../source/circuitpython/py/objbool.c \
../source/circuitpython/py/objboundmeth.c \
../source/circuitpython/py/objcell.c \
../source/circuitpython/py/objclosure.c \
../source/circuitpython/py/objcomplex.c \
../source/circuitpython/py/objdeque.c \
../source/circuitpython/py/objdict.c \
../source/circuitpython/py/objenumerate.c \
../source/circuitpython/py/objexcept.c \
../source/circuitpython/py/objfilter.c \
../source/circuitpython/py/objfloat.c \
../source/circuitpython/py/objfun.c \
../source/circuitpython/py/objgenerator.c \
../source/circuitpython/py/objgetitemiter.c \
../source/circuitpython/py/objint.c \
../source/circuitpython/py/objint_longlong.c \
../source/circuitpython/py/objint_mpz.c \
../source/circuitpython/py/objlist.c \
../source/circuitpython/py/objmap.c \
../source/circuitpython/py/objmodule.c \
../source/circuitpython/py/objobject.c \
../source/circuitpython/py/objpolyiter.c \
../source/circuitpython/py/objproperty.c \
../source/circuitpython/py/objnone.c \
../source/circuitpython/py/objnamedtuple.c \
../source/circuitpython/py/objrange.c \
../source/circuitpython/py/objreversed.c \
../source/circuitpython/py/objset.c \
../source/circuitpython/py/objsingleton.c \
../source/circuitpython/py/objslice.c \
../source/circuitpython/py/objstr.c \
../source/circuitpython/py/objstrunicode.c \
../source/circuitpython/py/objstringio.c \
../source/circuitpython/py/objtuple.c \
../source/circuitpython/py/objtype.c \
../source/circuitpython/py/objzip.c \
../source/circuitpython/py/opmethods.c \
../source/circuitpython/py/sequence.c \
../source/circuitpython/py/stream.c \
../source/circuitpython/py/binary.c \
../source/circuitpython/py/builtinimport.c \
../source/circuitpython/py/builtinevex.c \
../source/circuitpython/py/builtinhelp.c \
../source/circuitpython/py/modarray.c \
../source/circuitpython/py/modbuiltins.c \
../source/circuitpython/py/modcollections.c \
../source/circuitpython/py/modgc.c \
../source/circuitpython/py/modio.c \
../source/circuitpython/py/modmath.c \
../source/circuitpython/py/modcmath.c \
../source/circuitpython/py/modmicropython.c \
../source/circuitpython/py/modstruct.c \
../source/circuitpython/py/modsys.c \
../source/circuitpython/py/moduerrno.c \
../source/circuitpython/py/modthread.c \
../source/circuitpython/py/vm.c \
../source/circuitpython/py/bc.c \
../source/circuitpython/py/showbc.c \
../source/circuitpython/py/repl.c \
../source/circuitpython/py/smallint.c \
../source/circuitpython/py/frozenmod.c \
../source/circuitpython/py/gc_long_lived.c \
../source/circuitpython/py/proto.c \
../source/glue.c \
../source/exec.c \
../source/bindings/mini.c \
  -I../source/circuitpython \
  -I../source/

arm-none-eabi-g++ -Os -Werror -fno-common -mcpu=cortex-m3 -mthumb -msoft-float -fno-exceptions -fno-rtti -fno-threadsafe-statics -Wno-psabi -MD -D LA104 -D _ARM -D STM32F10X_HD -fno-math-errno -fsingle-precision-constant -Wdouble-promotion -c \
  ../source/main.cpp \
  ../source/assert.cpp \
  ../source/bindings/miniimpl.cpp \
  ../../../os_host/source/framework/Wnd.cpp \
  ../../../os_host/source/framework/Serialize.cpp \
  -I../source/ \
  -I../source/circuitpython \
  -I../../../os_library/include/ 

# -specs=nosys.specs
arm-none-eabi-gcc -fPIC -mcpu=cortex-m3 -mthumb -o output.elf -nostartfiles -T ../source/app.lds \
  mpstate.o \
  nlr.o \
  nlrx86.o \
  nlrx64.o \
  nlrthumb.o \
  nlrxtensa.o \
  nlrsetjmp.o \
  malloc.o \
  gc.o \
  pystack.o \
  qstr.o \
  vstr.o \
  mpprint.o \
  unicode.o \
  mpz.o \
  reader.o \
  lexer.o \
  parse.o \
  scope.o \
  compile.o \
  emitcommon.o \
  emitbc.o \
  asmbase.o \
  asmx64.o \
  emitnx64.o \
  asmx86.o \
  emitnx86.o \
  asmthumb.o \
  emitnthumb.o \
  emitinlinethumb.o \
  asmarm.o \
  emitnarm.o \
  asmxtensa.o \
  emitnxtensa.o \
  emitinlinextensa.o \
  formatfloat.o \
  parsenumbase.o \
  parsenum.o \
  emitglue.o \
  persistentcode.o \
  runtime.o \
  runtime_utils.o \
  scheduler.o \
  nativeglue.o \
  ringbuf.o \
  stackctrl.o \
  argcheck.o \
  warning.o \
  map.o \
  obj.o \
  objarray.o \
  objattrtuple.o \
  objbool.o \
  objboundmeth.o \
  objcell.o \
  objclosure.o \
  objcomplex.o \
  objdeque.o \
  objdict.o \
  objenumerate.o \
  objexcept.o \
  objfilter.o \
  objfloat.o \
  objfun.o \
  objgenerator.o \
  objgetitemiter.o \
  objint.o \
  objint_longlong.o \
  objint_mpz.o \
  objlist.o \
  objmap.o \
  objmodule.o \
  objobject.o \
  objpolyiter.o \
  objproperty.o \
  objnone.o \
  objnamedtuple.o \
  objrange.o \
  objreversed.o \
  objset.o \
  objsingleton.o \
  objslice.o \
  objstr.o \
  objstrunicode.o \
  objstringio.o \
  objtuple.o \
  objtype.o \
  objzip.o \
  opmethods.o \
  sequence.o \
  stream.o \
  binary.o \
  builtinimport.o \
  builtinevex.o \
  builtinhelp.o \
  modarray.o \
  modbuiltins.o \
  modcollections.o \
  modgc.o \
  modio.o \
  modmath.o \
  modcmath.o \
  modmicropython.o \
  modstruct.o \
  modsys.o \
  moduerrno.o \
  modthread.o \
  vm.o \
  bc.o \
  showbc.o \
  repl.o \
  smallint.o \
  gc_long_lived.o \
  proto.o \
  frozenmod.o \
  main.o \
  glue.o \
  exec.o \
  mini.o \
  miniimpl.o \
  assert.o \
  Wnd.o \
  Serialize.o \
  -lbios_la104 -lm -L../../../os_library/build

arm-none-eabi-objdump -d -S output.elf > output.asm

nm --print-size --size-sort -gC output.elf | grep " B " > symbols_ram.txt
nm --print-size --size-sort -gC output.elf | grep " T " > symbols_rom.txt
nm --print-size --size-sort -gC output.elf > symbols_all.txt

find . -type f -name '*.o' -delete
find . -type f -name '*.d' -delete

../../../../tools/elfstrip/elfstrip output.elf 113cirpy.elf
