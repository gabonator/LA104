static u32 nested_ctr;
static u32 prev_primask;

void IRQ_Disable(void)
{
  // get current priority if nested level == 0
  if( !nested_ctr ) {
    __asm volatile (			   \
		    "	mrs %0, primask\n" \
		    : "=r" (prev_primask)  \
		    );
  }

  // disable interrupts
  __asm volatile ( \
		  "	mov r0, #1     \n" \
		  "	msr primask, r0\n" \
		  :::"r0"	 \
		  );

  ++nested_ctr;
}

void IRQ_Enable(void)
{
  // check for nesting error
  if( nested_ctr == 0 )
    return; // nesting error

  // decrease nesting level
  --nested_ctr;

  // set back previous priority once nested level reached 0 again
  if( nested_ctr == 0 ) {
    __asm volatile ( \
		    "	msr primask, %0\n" \
		    :: "r" (prev_primask)  \
		    );
  }
}
