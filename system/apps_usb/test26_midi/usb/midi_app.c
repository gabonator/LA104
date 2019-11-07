#include "midi_app.h"
#include "midi_desc.h"
#include "midi_conf.h"

typedef union {
  struct {
    u32 ALL;
  };
  struct {
    u8 cin_cable;
    u8 evnt0;
    u8 evnt1;
    u8 evnt2;
  };
  struct {
    u8 type:4;
    u8 cable:4;
    u8 chn:4; // mios32_midi_chn_t
    u8 event:4; // mios32_midi_event_t
    u8 value1;
    u8 value2;
  };

  // C++ doesn't allow to redefine names in anonymous unions
  // as a simple workaround, we rename these redundant names
  struct {
    u8 cin:4;
    u8 dummy1_cable:4;
    u8 dummy1_chn:4; // mios32_midi_chn_t 
    u8 dummy1_event:4; // mios32_midi_event_t 
    u8 note:8;
    u8 velocity:8;
  };
  struct {
    u8 dummy2_cin:4;
    u8 dummy2_cable:4;
    u8 dummy2_chn:4; // mios32_midi_chn_t 
    u8 dummy2_event:4; // mios32_midi_event_t 
    u8 cc_number:8;
    u8 value:8;
  };
  struct {
    u8 dummy3_cin:4;
    u8 dummy3_cable:4;
    u8 dummy3_chn:4; // mios32_midi_chn_t 
    u8 dummy3_event:4; // mios32_midi_event_t
    u8 program_change:8;
    u8 dummy3:8;
  };
} mios32_midi_package_t;

typedef enum {
  USB0 = 0x10
} mios32_midi_port_t;

const u8 mios32_midi_pcktype_num_bytes[16] = {
  0, // 0: invalid/reserved event
  0, // 1: invalid/reserved event
  2, // 2: two-byte system common messages like MTC, Song Select, etc.
  3, // 3: three-byte system common messages like SPP, etc.
  3, // 4: SysEx starts or continues
  1, // 5: Single-byte system common message or sysex sends with following single byte
  2, // 6: SysEx sends with following two bytes
  3, // 7: SysEx sends with following three bytes
  3, // 8: Note Off
  3, // 9: Note On
  3, // a: Poly-Key Press
  3, // b: Control Change
  2, // c: Program Change
  2, // d: Channel Pressure
  3, // e: PitchBend Change
  1  // f: single byte
};

char* _tohex(int);
char* _tox(int, int, int);

/*
s32 MIOS32_MIDI_SendPackageToRxCallback(mios32_midi_port_t port, mios32_midi_package_t midi_package)
{
dbgPrint("send: ");
  u8 buffer[3] = {midi_package.evnt0, midi_package.evnt1, midi_package.evnt2};
  int len = mios32_midi_pcktype_num_bytes[midi_package.cin];
  for(int i=0; i<len; ++i)
{
  dbgHex(buffer[i]);
}
  return 0;
}
*/
DEVICE_STATE bDeviceState;

void MIOS32_USB_MIDI_TxBufferHandler(void);
void MIOS32_USB_MIDI_RxBufferHandler(void);

void onReceive(uint_fast8_t);
bool onReceiveAvailable(int);
int onTransmitAvailable();
uint_fast8_t onTransmitPeek();
uint_fast8_t onTransmitGet();


//RingBuffer<uint32_t, 32> bufferReceive;
/*
// Rx buffer
u32 rx_buffer[MIOS32_USB_MIDI_RX_BUFFER_SIZE];
volatile u16 rx_buffer_tail = 0;
volatile u16 rx_buffer_head = 0;
volatile u16 rx_buffer_size = 0;
volatile u8 rx_buffer_new_data = 0;

// Tx buffer
u32 tx_buffer[MIOS32_USB_MIDI_TX_BUFFER_SIZE];
volatile u16 tx_buffer_tail = 0;
volatile u16 tx_buffer_head = 0;
volatile u16 tx_buffer_size = 0;
*/
// transfer possible?
static u8 transfer_possible = 0;
volatile u8 rx_buffer_new_data = 0;
volatile u8 tx_buffer_busy = 1;


s32 MIOS32_USB_MIDI_ChangeConnectionState(u8 connected)
{
  // in all cases: re-initialize USB MIDI driver
  // clear buffer counters and busy/wait signals again (e.g., so that no invalid data will be sent out)
//  rx_buffer_tail = rx_buffer_head = rx_buffer_size = 0;
  rx_buffer_new_data = 0; // no data received yet
//  tx_buffer_tail = tx_buffer_head = tx_buffer_size = 0;

  if( connected ) {
    dbgPrint("[connected]");
    transfer_possible = 1;
    tx_buffer_busy = 0; // buffer not busy anymore
  } else {
    dbgPrint("[disconnected]");
    // cable disconnected: disable transfers
    transfer_possible = 0;
    tx_buffer_busy = 1; // buffer busy
  }

  return 0; // no error
}

s32 MIOS32_USB_MIDI_CheckAvailable(void)
{
  return transfer_possible ? 1 : 0;
}

#define MEM8(addr)  (*((volatile u8  *)(addr)))
s32 MIOS32_SYS_SerialNumberGet(char *str)
{
  int i;

  // stored in the so called "electronic signature"
  for(i=0; i<24; ++i) {
    u8 b = MEM8(0x1ffff7e8 + (i/2));
    if( !(i & 1) )
      b >>= 4;
    b &= 0x0f;

    str[i] = ((b > 9) ? ('A'-10) : '0') + b;
  }
  str[i] = 0;

  return 0; // no error
}

static u32 nested_ctr;
static u32 prev_primask;

s32 MIOS32_IRQ_Disable(void)
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

  return 0; // no error
}

s32 MIOS32_IRQ_Enable(void)
{
  // check for nesting error
  if( nested_ctr == 0 )
    return -1; // nesting error

  // decrease nesting level
  --nested_ctr;

  // set back previous priority once nested level reached 0 again
  if( nested_ctr == 0 ) {
    __asm volatile ( \
		    "	msr primask, %0\n" \
		    :: "r" (prev_primask)  \
		    );
  }

  return 0; // no error
}

void MIOS32_USB_MIDI_TxBufferHandler(void)
{
  // send buffered packages if
  //   - last transfer finished
  //   - new packages are in the buffer
  //   - the device is configured

  // atomic operation to avoid conflict with other interrupts
  MIOS32_IRQ_Disable();
  if( !tx_buffer_busy && onTransmitAvailable() && transfer_possible ) {
//  if( onTransmitAvailable() ) {
    u32 *pma_addr = (u32 *)(PMAAddr + (MIOS32_USB_ENDP1_TXADDR<<1));
    // notify that new package is sent

    // copy into PMA buffer (16bit word with, only 32bit addressable)
    int sentSize = 0;
    do {
      while (onTransmitAvailable() && (onTransmitPeek() & 0xf0) == 0)
        onTransmitGet(); // ignore all

      if (!onTransmitAvailable())
        break;

      mios32_midi_package_t package;
      package.ALL = 0;
      package.evnt0 = onTransmitPeek();
      package.cin = package.evnt0 >> 4;
      int required = mios32_midi_pcktype_num_bytes[package.cin];
      if (onTransmitAvailable() < required)
        break;

//      dbgPrint("[");
      package.evnt0 = onTransmitGet();
//      dbgHex(package.evnt0);
      if (required >= 2)
      {
        package.evnt1 = onTransmitGet();
//        dbgHex(package.evnt1);
      }
      if (required >= 3)
      {
        package.evnt2 = onTransmitGet();
//        dbgHex(package.evnt2);
      }
//      dbgPrint("]");

//      package.cin_cable = package.evnt0 >> 4;

      *pma_addr++ = package.ALL & 0xffff;
      *pma_addr++ = (package.ALL>>16) & 0xffff;
      sentSize += 4;
    } while( sentSize < MIOS32_USB_MIDI_DATA_IN_SIZE - 4);

    if (sentSize > 0)
    {
      // send to IN pipe
      SetEPTxCount(ENDP1, sentSize);
      // send buffer
      SetEPTxValid(ENDP1);
      tx_buffer_busy = 1;
    }
  }

  MIOS32_IRQ_Enable();
}

void MIOS32_USB_MIDI_RxBufferHandler(void)
{
  s16 count;

  // atomic operation to avoid conflict with other interrupts
  MIOS32_IRQ_Disable();

  if( rx_buffer_new_data && (count=GetEPRxCount(ENDP2)/4) ) {

    // check if buffer is free
    if( onReceiveAvailable(count) ) {
      u32 *pma_addr = (u32 *)(PMAAddr + (MIOS32_USB_ENDP2_RXADDR<<1));
      // copy received packages into receive buffer
      // this operation should be atomic
      do {

	u16 pl = *pma_addr++;
	u16 ph = *pma_addr++;
	mios32_midi_package_t package;
	package.ALL = (ph << 16) | pl;

        u8 buffer[3] = {package.evnt0, package.evnt1, package.evnt2};
        int len = mios32_midi_pcktype_num_bytes[package.cin];
        for(int i=0; i<len; ++i)
          onReceive(buffer[i]);
      } while( --count > 0 );

      // notify, that data has been put into buffer
      rx_buffer_new_data = 0;

      // release OUT pipe
      SetEPRxValid(ENDP2);
    }
  }
  MIOS32_IRQ_Enable();
}

s32 MIOS32_USB_ForceSingleUSB(void)
{
  return 0;
}
