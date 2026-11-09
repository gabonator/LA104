// DS203 License Writer
//
// Reads STM32 unique device ID, computes license using both
// NEW and OLD algorithms, lets user choose, then erases the
// license page and writes the selected license.
//
// F1 = select/confirm, F2 = cancel/exit

#include <stdint.h>

#define SYS_BIOS 0x08004200

typedef void     (*fn_ClearScreen)(uint16_t Color);
typedef void     (*fn_DisplayStr)(uint16_t x, uint16_t y,
                                  uint16_t Color, uint8_t Mode, char* s);
typedef uint32_t (*fn_Get)(uint8_t Object, uint32_t Value);
typedef uint8_t  (*fn_FlashErase)(uint32_t Address);
typedef uint8_t  (*fn_FlashProg)(uint32_t Address, uint16_t Data);
typedef void     (*fn_FlashUnlock)(void);
typedef void     (*fn_FlashLock)(void);

#define Bios_ClearScreen  ((fn_ClearScreen) (SYS_BIOS + 0x15))
#define Bios_DisplayStr   ((fn_DisplayStr)  (SYS_BIOS + 0x49))
#define Bios_Get          ((fn_Get)         (SYS_BIOS + 0x55))
#define Bios_FlashErase   ((fn_FlashErase)  (SYS_BIOS + 0x71))
#define Bios_FlashProg    ((fn_FlashProg)   (SYS_BIOS + 0x75))
#define Bios_FlashUnlock  ((fn_FlashUnlock) (SYS_BIOS + 0x79))
#define Bios_FlashLock    ((fn_FlashLock)   (SYS_BIOS + 0x7D))

#define KEY_STATUS  4
#define KEY1_STATUS 0x4000
#define KEY2_STATUS 0x2000
#define ALL_KEYS    (KEY1_STATUS | KEY2_STATUS | 0x0100 | 0x0200)

#define LICENSE_PAGE  0x08003800
#define LICENSE_ADDR  0x08003804

// STM32F103 96-bit unique device ID
#define UID_WORD0  (*(volatile uint32_t*)0x1FFFF7E8)
#define UID_WORD1  (*(volatile uint32_t*)0x1FFFF7EC)
#define UID_WORD2  (*(volatile uint32_t*)0x1FFFF7F0)

#define CONST2 0xD5203002

// BGR565 (LCD has R/B swapped)
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0x001F
#define GREEN   0x07E0
#define YELLOW  0x07FF
#define CYAN    0xFFE0

static char msg[64];

static void strCpy(char* d, const char* s) {
  while (*s) *d++ = *s++; *d = 0;
}
static void strCat(char* d, const char* s) {
  while (*d) d++; while (*s) *d++ = *s++; *d = 0;
}
static void u32Hex(uint32_t v, char* b) {
  static const char h[] = "0123456789ABCDEF";
  b[0]='0'; b[1]='x';
  for (int i = 9; i >= 2; i--) { b[i] = h[v & 0xF]; v >>= 4; }
  b[10] = 0;
}

static void display(int line, uint16_t color, const char* text) {
  Bios_DisplayStr(0, 224 - line * 16, color, 0, (char*)text);
}
static void displayHex(int line, uint16_t color,
                       const char* prefix, uint32_t val) {
  strCpy(msg, prefix);
  char hex[12]; u32Hex(val, hex);
  strCat(msg, hex);
  display(line, color, msg);
}

static uint32_t getKeys() { return ~Bios_Get(KEY_STATUS, 0); }

// Wait for F1 or F2. Returns true=F1, false=F2.
static bool waitKey() {
  while (getKeys() & ALL_KEYS);
  while (!(getKeys() & ALL_KEYS));
  bool f1 = !!(getKeys() & KEY1_STATUS);
  while (getKeys() & ALL_KEYS);
  return f1;
}

// NEW algorithm (DFU >= V2.81): init=0xDEC0DE02, serial i=20..30, const2 i=57..70
static uint32_t compute_new(uint32_t serial) {
  uint32_t h = 0xDEC0DE02;
  for (int i = 20; i <= 30; i++) {
    uint8_t b = (serial >> ((i % 4) * 8)) & 0xFF;
    h = h * 31 + b;
  }
  for (int i = 57; i <= 70; i++) {
    uint8_t b = (CONST2 >> ((i % 4) * 8)) & 0xFF;
    h = h * 31 + b;
  }
  return h;
}

// OLD algorithm (DFU < V2.81): init=DEC0DE02, serial i=2..12, const=0x4013E5E1
static uint32_t compute_old(uint32_t serial) {
  uint32_t sp = 0;
  for (int i = 2; i <= 12; i++) {
    uint8_t b = (serial >> ((i % 4) * 8)) & 0xFF;
    sp = sp * 31 + b;
  }
  uint32_t p25 = 1, p14 = 1;
  for (int i = 0; i < 25; i++) p25 *= 31;
  for (int i = 0; i < 14; i++) p14 *= 31;
  return 0xDEC0DE02 * p25 + sp * p14 + 0x4013E5E1;
}

static bool writeLicense(uint32_t license) {
  __asm volatile ("cpsid i" ::: "memory");
  Bios_FlashUnlock();

  display(13, YELLOW, "Erasing page 0x08003800...   ");
  uint8_t ok = Bios_FlashErase(LICENSE_PAGE);
  if (!ok) {
    display(13, RED, "ERASE FAILED!                ");
    Bios_FlashLock();
    __asm volatile ("cpsie i" ::: "memory");
    return false;
  }

  // Verify erase
  if (*(volatile uint32_t*)LICENSE_ADDR != 0xFFFFFFFF) {
    display(13, RED, "Erase verify failed!         ");
    Bios_FlashLock();
    __asm volatile ("cpsie i" ::: "memory");
    return false;
  }

  display(13, YELLOW, "Programming...               ");
  uint16_t lo = (uint16_t)(license & 0xFFFF);
  uint16_t hi = (uint16_t)(license >> 16);

  ok = Bios_FlashProg(LICENSE_ADDR, lo);
  if (!ok) {
    display(13, RED, "Program low halfword FAILED! ");
    Bios_FlashLock();
    __asm volatile ("cpsie i" ::: "memory");
    return false;
  }

  ok = Bios_FlashProg(LICENSE_ADDR + 2, hi);
  if (!ok) {
    display(13, RED, "Program high halfword FAILED!");
    Bios_FlashLock();
    __asm volatile ("cpsie i" ::: "memory");
    return false;
  }

  Bios_FlashLock();
  __asm volatile ("cpsie i" ::: "memory");

  uint32_t written = *(volatile uint32_t*)LICENSE_ADDR;
  return (written == license);
}

extern "C" __attribute__((section(".entry")))
int _start() {
  Bios_ClearScreen(BLACK);
  display(0, YELLOW, "=== DS203 License Writer === ");

  // Read device unique ID
  uint32_t w0 = UID_WORD0;
  uint32_t w1 = UID_WORD1;
  uint32_t w2 = UID_WORD2;
  uint32_t serial = w0 + w1 + w2;

  displayHex(1, WHITE, "UID[0]: ", w0);
  displayHex(2, WHITE, "UID[1]: ", w1);
  displayHex(3, WHITE, "UID[2]: ", w2);
  displayHex(4, WHITE, "Serial: ", serial);

  uint32_t licNew = compute_new(serial);
  uint32_t licOld = compute_old(serial);

  displayHex(6, GREEN, "NEW: ", licNew);
  display(7, WHITE,    "  (DFU >= V2.81)             ");
  displayHex(8, CYAN,  "OLD: ", licOld);
  display(9, WHITE,    "  (DFU < V2.81)              ");

  uint32_t current = *(volatile uint32_t*)LICENSE_ADDR;
  displayHex(11, YELLOW, "Current @3804: ", current);

  // Let user choose algorithm
  display(13, WHITE, "F1=Write NEW  F2=Write OLD   ");
  bool useNew = waitKey();

  uint32_t license = useNew ? licNew : licOld;
  if (useNew)
    display(13, GREEN, "Selected: NEW algorithm      ");
  else
    display(13, CYAN,  "Selected: OLD algorithm      ");

  displayHex(12, WHITE, "Will write: ", license);
  display(14, WHITE, "F1=CONFIRM WRITE  F2=cancel  ");

  if (!waitKey()) {
    display(14, WHITE, "Cancelled.                   ");
    while(1);
  }

  // Write it
  if (writeLicense(license)) {
    uint32_t written = *(volatile uint32_t*)LICENSE_ADDR;
    displayHex(12, WHITE, "Written: ", written);
    display(13, GREEN, "SUCCESS!                     ");
    display(14, GREEN, "Power cycle the device.      ");
  } else {
    display(14, RED, "FAILED! Power cycle device.  ");
  }

  while(1);
  return 0;
}
