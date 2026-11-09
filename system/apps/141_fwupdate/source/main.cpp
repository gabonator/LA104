// DS203 Firmware Updater (two-pass)
//
// Pass 1: Reads Intel HEX file, verifies address ranges, shows summary.
// Pass 2: After button press, actually flashes the firmware.
//
// After disabling interrupts, this app ONLY calls SYS BIOS functions
// (jump table at 0x08004200, in SYS flash 0x08004000-0x0800BFFF).
// The program counter NEVER enters the os_host range (0x0800C000+).
//
// Uses raw SPI flash reads (__ReadDiskData) + minimal FAT12/16 parser.

#include <stdint.h>

// ============================================================
// Configuration
// ============================================================
// 8.3 FAT directory name for "SYSTEM.HEX"
static const char TARGET_NAME[11] = {
  'S','Y','S','T','E','M',' ',' ', 'H','E','X'
};

// Flash address safety bounds - never touch DFU or SYS BIOS
#define SAFE_FLASH_START  0x0800C000  // APP1 base (OS firmware)
#define SAFE_FLASH_END    0x08024000  // APP4 base (our flasher lives here)
#define FLASH_PAGE_SIZE   2048        // STM32F103 high-density page size

// ============================================================
// ARM Cortex-M3
// ============================================================
static inline void disable_irq() {
  __asm volatile ("cpsid i" ::: "memory");
}

static inline void dsb() {
  __asm volatile ("dsb" ::: "memory");
}

#define SCB_VTOR   (*(volatile uint32_t*)0xE000ED08)
#define SCB_AIRCR  (*(volatile uint32_t*)0xE000ED0C)

// ============================================================
// SYS BIOS function types
// Jump table at 0x08004200 in factory SYS flash.
// ============================================================
#define SYS_BIOS 0x08004200

typedef void     (*fn_ClearScreen)(uint16_t Color);
typedef void     (*fn_DisplayStr)(uint16_t x, uint16_t y,
                                  uint16_t Color, uint8_t Mode, char* s);
typedef uint8_t  (*fn_FlashErase)(uint32_t Address);
typedef uint8_t  (*fn_FlashProg)(uint32_t Address, uint16_t Data);
typedef void     (*fn_FlashUnlock)(void);
typedef void     (*fn_FlashLock)(void);
typedef uint8_t  (*fn_ReadDiskData)(uint8_t* pBuffer, uint32_t ReadAddr,
                                    uint16_t Length);
typedef uint32_t (*fn_Get)(uint8_t Object, uint32_t Value);

#define Bios_ClearScreen  ((fn_ClearScreen) (SYS_BIOS + 0x15))
#define Bios_DisplayStr   ((fn_DisplayStr)  (SYS_BIOS + 0x49))
#define Bios_FlashErase   ((fn_FlashErase)  (SYS_BIOS + 0x71))
#define Bios_FlashProg    ((fn_FlashProg)   (SYS_BIOS + 0x75))
#define Bios_FlashUnlock  ((fn_FlashUnlock) (SYS_BIOS + 0x79))
#define Bios_FlashLock    ((fn_FlashLock)   (SYS_BIOS + 0x7D))
#define Bios_ReadDiskData ((fn_ReadDiskData)(SYS_BIOS + 0x61))
#define Bios_Get          ((fn_Get)         (SYS_BIOS + 0x55))

// Key status bits (active low in hardware, inverted by GetKeys)
#define KEY_STATUS  4
#define KEY1_STATUS 0x4000
#define KEY2_STATUS 0x2000
#define KEY3_STATUS 0x0100
#define KEY4_STATUS 0x0200
#define ALL_KEYS    (KEY1_STATUS | KEY2_STATUS | KEY3_STATUS | KEY4_STATUS)

// ============================================================
// RGB565 colors
// ============================================================
#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0

// ============================================================
// Minimal string helpers (no libc)
// ============================================================
static void strCpy(char* d, const char* s) {
  while (*s) *d++ = *s++;
  *d = 0;
}

static void strCat(char* d, const char* s) {
  while (*d) d++;
  while (*s) *d++ = *s++;
  *d = 0;
}

static void u32Hex(uint32_t v, char* b) {
  static const char h[] = "0123456789ABCDEF";
  b[0]='0'; b[1]='x';
  for (int i = 9; i >= 2; i--) { b[i] = h[v & 0xF]; v >>= 4; }
  b[10] = 0;
}

static void intStr(int v, char* b) {
  if (v == 0) { b[0]='0'; b[1]=0; return; }
  char t[12]; int i = 0;
  if (v < 0) { *b++ = '-'; v = -v; }
  while (v > 0) { t[i++] = '0' + (v % 10); v /= 10; }
  int j = 0;
  while (i > 0) b[j++] = t[--i];
  b[j] = 0;
}

// ============================================================
// Display helpers (16px per text line)
// ============================================================
static char msg[52];

static void display(int line, uint16_t color, const char* text) {
  Bios_DisplayStr(0, line * 16, color, 0, (char*)text);
}

static void displayHex(int line, uint16_t color,
                       const char* prefix, uint32_t val) {
  strCpy(msg, prefix);
  char hex[12]; u32Hex(val, hex);
  strCat(msg, hex);
  display(line, color, msg);
}

static void displayInt(int line, uint16_t color,
                       const char* prefix, int val) {
  strCpy(msg, prefix);
  char num[12]; intStr(val, num);
  strCat(msg, num);
  display(line, color, msg);
}

// ============================================================
// Key reading
// ============================================================
static uint32_t getKeys() {
  return ~Bios_Get(KEY_STATUS, 0);
}

static void waitKeyRelease() {
  while (getKeys() & ALL_KEYS);
}

static void waitKeyPress() {
  while (!(getKeys() & ALL_KEYS));
}

// ============================================================
// Raw external flash read via SYS BIOS
// ============================================================
static uint8_t secBuf[512];

static void readFlash(uint8_t* buf, uint32_t byteAddr, uint16_t len) {
  Bios_ReadDiskData(buf, byteAddr, len);
}

// ============================================================
// Little-endian helpers
// ============================================================
static uint16_t rd16(const uint8_t* p) {
  return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t rd32(const uint8_t* p) {
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
         ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

// ============================================================
// FAT12/16 filesystem parser
// ============================================================
static struct {
  uint16_t bytesPerSec;
  uint8_t  secPerClus;
  uint16_t rsvdSecCnt;
  uint8_t  numFATs;
  uint16_t rootEntCnt;
  uint16_t FATSz16;
  uint32_t fatByteOff;
  uint32_t rootDirByteOff;
  uint32_t dataByteOff;
  uint32_t dataClus;
  bool     fat16;
} fat;

static bool parseBoot() {
  readFlash(secBuf, 0, 512);

  fat.bytesPerSec = rd16(secBuf + 0x0B);
  fat.secPerClus  = secBuf[0x0D];
  fat.rsvdSecCnt  = rd16(secBuf + 0x0E);
  fat.numFATs     = secBuf[0x10];
  fat.rootEntCnt  = rd16(secBuf + 0x11);
  fat.FATSz16     = rd16(secBuf + 0x16);

  uint16_t totSec16 = rd16(secBuf + 0x13);
  uint32_t totSec32 = rd32(secBuf + 0x20);

  if (fat.bytesPerSec == 0 || fat.secPerClus == 0) return false;
  if (fat.bytesPerSec != 512) return false;

  fat.fatByteOff = (uint32_t)fat.rsvdSecCnt * fat.bytesPerSec;
  fat.rootDirByteOff = fat.fatByteOff +
    (uint32_t)fat.numFATs * fat.FATSz16 * fat.bytesPerSec;

  uint32_t rootDirSectors =
    ((uint32_t)fat.rootEntCnt * 32 + fat.bytesPerSec - 1) / fat.bytesPerSec;
  fat.dataByteOff = fat.rootDirByteOff + rootDirSectors * fat.bytesPerSec;

  uint32_t totalSec = totSec16 ? totSec16 : totSec32;
  uint32_t dataSec = totalSec -
    (fat.rsvdSecCnt + (uint32_t)fat.numFATs * fat.FATSz16 + rootDirSectors);
  fat.dataClus = dataSec / fat.secPerClus;
  fat.fat16 = (fat.dataClus >= 4085);

  return true;
}

static uint16_t fileStartCluster;
static uint32_t fileSize;

static bool findFile() {
  uint32_t entries = fat.rootEntCnt;
  uint32_t offset = fat.rootDirByteOff;

  while (entries > 0) {
    readFlash(secBuf, offset, 512);
    int perSec = 512 / 32;
    if (perSec > (int)entries) perSec = entries;

    for (int i = 0; i < perSec; i++) {
      uint8_t* e = secBuf + i * 32;
      if (e[0] == 0x00) return false;
      if (e[0] == 0xE5) continue;
      if ((e[11] & 0x0F) == 0x0F) continue;
      if (e[11] & 0x08) continue;

      bool match = true;
      for (int j = 0; j < 11; j++) {
        if (e[j] != (uint8_t)TARGET_NAME[j]) { match = false; break; }
      }
      if (match) {
        fileStartCluster = rd16(e + 26);
        fileSize = rd32(e + 28);
        return true;
      }
    }

    offset += 512;
    entries -= perSec;
  }
  return false;
}

static void showDirEntries(int startLine, int maxLines) {
  uint32_t offset = fat.rootDirByteOff;
  int shown = 0;

  for (int sec = 0; sec < 4 && shown < maxLines; sec++) {
    readFlash(secBuf, offset + sec * 512, 512);

    for (int i = 0; i < 512 / 32 && shown < maxLines; i++) {
      uint8_t* e = secBuf + i * 32;
      if (e[0] == 0x00) return;
      if (e[0] == 0xE5) continue;
      if ((e[11] & 0x0F) == 0x0F) continue;
      if (e[11] & 0x08) continue;

      char name[16];
      int p = 0;
      for (int j = 0; j < 8; j++) name[p++] = (char)e[j];
      name[p++] = '.';
      for (int j = 8; j < 11; j++) name[p++] = (char)e[j];
      name[p] = 0;

      display(startLine + shown, WHITE, name);
      shown++;
    }
  }
}

static uint16_t nextCluster(uint16_t clus) {
  if (fat.fat16) {
    uint32_t off = fat.fatByteOff + (uint32_t)clus * 2;
    uint32_t secBase = off & ~(uint32_t)511;
    readFlash(secBuf, secBase, 512);
    return rd16(secBuf + (off - secBase));
  } else {
    uint32_t off = fat.fatByteOff + (uint32_t)clus + (uint32_t)clus / 2;
    uint32_t secBase = off & ~(uint32_t)511;
    readFlash(secBuf, secBase, 512);
    uint16_t idx = (uint16_t)(off - secBase);
    uint16_t val;
    if (idx < 511) {
      val = rd16(secBuf + idx);
    } else {
      val = secBuf[511];
      readFlash(secBuf, secBase + 512, 512);
      val |= (uint16_t)secBuf[0] << 8;
    }
    return (clus & 1) ? (val >> 4) : (val & 0x0FFF);
  }
}

static bool isEOC(uint16_t clus) {
  return fat.fat16 ? (clus >= 0xFFF8) : (clus >= 0xFF8);
}

// ============================================================
// Flash page buffer
// ============================================================
static uint8_t pageBuf[FLASH_PAGE_SIZE];
static uint32_t pageBase = 0xFFFFFFFF;
static bool     pageDirty = false;
static int      pagesFlashed = 0;
static bool     flashErr = false;

static void initPage(uint32_t base) {
  for (int i = 0; i < FLASH_PAGE_SIZE; i++) pageBuf[i] = 0xFF;
  pageBase = base;
  pageDirty = false;
}

static void flushPage() {
  if (!pageDirty || pageBase == 0xFFFFFFFF) return;

  Bios_FlashErase(pageBase);

  volatile uint16_t* fp = (volatile uint16_t*)pageBase;
  for (int i = 0; i < FLASH_PAGE_SIZE / 2; i++) {
    if (fp[i] != 0xFFFF) {
      displayHex(12, RED, "ERASE FAIL: ", pageBase + i * 2);
      flashErr = true;
      return;
    }
  }

  for (int i = 0; i < FLASH_PAGE_SIZE; i += 2) {
    uint16_t hw = (uint16_t)pageBuf[i] | ((uint16_t)pageBuf[i+1] << 8);
    if (hw != 0xFFFF) {
      Bios_FlashProg(pageBase + i, hw);
    }
  }

  for (int i = 0; i < FLASH_PAGE_SIZE / 2; i++) {
    uint16_t expect = (uint16_t)pageBuf[i*2] | ((uint16_t)pageBuf[i*2+1] << 8);
    if (fp[i] != expect) {
      displayHex(12, RED, "VERIFY FAIL: ", pageBase + i * 2);
      flashErr = true;
      return;
    }
  }

  pagesFlashed++;
  pageDirty = false;
}

// ============================================================
// Multi-pass mode control
// ============================================================
enum PassMode { PASS_VERIFY, PASS_FLASH, PASS_READBACK };
static PassMode passMode = PASS_VERIFY;
static uint32_t addrMin = 0xFFFFFFFF;
static uint32_t addrMax = 0x00000000;
static int      mismatchCount = 0;

static void storeByte(uint32_t addr, uint8_t data) {
  if (addr < SAFE_FLASH_START || addr >= SAFE_FLASH_END) {
    if (passMode == PASS_FLASH) {
      displayHex(12, RED, "ADDR REJECT: ", addr);
    }
    flashErr = true;
    return;
  }

  if (passMode == PASS_VERIFY) {
    if (addr < addrMin) addrMin = addr;
    if (addr > addrMax) addrMax = addr;
    return;
  }

  if (passMode == PASS_READBACK) {
    uint8_t actual = *(volatile uint8_t*)addr;
    if (actual != data) {
      if (mismatchCount == 0) {
        displayHex(12, RED, "MISMATCH AT: ", addr);
      }
      mismatchCount++;
    }
    return;
  }

  // PASS_FLASH
  uint32_t base = addr & ~(uint32_t)(FLASH_PAGE_SIZE - 1);
  if (base != pageBase) {
    flushPage();
    if (flashErr) return;
    initPage(base);
  }

  pageBuf[addr - pageBase] = data;
  pageDirty = true;
}

// ============================================================
// Intel HEX parser
// ============================================================
static uint32_t hexBase = 0;
static int      totalBytes = 0;
static bool     hexDone = false;
static char     lineBuf[80];
static int      linePos = 0;

static void resetHexParser() {
  hexBase = 0;
  totalBytes = 0;
  hexDone = false;
  linePos = 0;
  flashErr = false;
}

static uint8_t hexNib(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  return 0;
}

static uint8_t hexByte(const char* p) {
  return (hexNib(p[0]) << 4) | hexNib(p[1]);
}

static void processLine(const char* ln, int len) {
  if (hexDone || flashErr) return;
  if (len < 11 || ln[0] != ':') return;

  uint8_t count = hexByte(ln + 1);
  uint16_t addr = ((uint16_t)hexByte(ln + 3) << 8) | hexByte(ln + 5);
  uint8_t type  = hexByte(ln + 7);

  if (len < 11 + count * 2) return;

  switch (type) {
    case 0x00:  // Data
      for (int i = 0; i < count; i++) {
        uint8_t b = hexByte(ln + 9 + i * 2);
        storeByte(hexBase + addr + i, b);
        if (flashErr) return;
        totalBytes++;
      }
      break;

    case 0x01:  // EOF
      hexDone = true;
      break;

    case 0x02:  // Extended segment address
      if (count >= 2) {
        uint16_t seg = ((uint16_t)hexByte(ln+9) << 8) | hexByte(ln+11);
        hexBase = (uint32_t)seg << 4;
      }
      break;

    case 0x04:  // Extended linear address
      if (count >= 2) {
        uint16_t hi = ((uint16_t)hexByte(ln+9) << 8) | hexByte(ln+11);
        hexBase = (uint32_t)hi << 16;
      }
      break;
  }
}

static void parseChunk(const uint8_t* data, int len) {
  for (int i = 0; i < len && !hexDone && !flashErr; i++) {
    char c = (char)data[i];
    if (c == ':') {
      linePos = 0;
      lineBuf[linePos++] = c;
    } else if (c == '\r' || c == '\n') {
      if (linePos > 0) {
        lineBuf[linePos] = 0;
        processLine(lineBuf, linePos);
        linePos = 0;
      }
    } else if (linePos > 0 && linePos < (int)sizeof(lineBuf) - 1) {
      lineBuf[linePos++] = c;
    }
  }
}

// ============================================================
// Read entire file through the hex parser
// ============================================================
static int readAndParseFile(int progressLine) {
  uint16_t clus = fileStartCluster;
  uint32_t bytesLeft = fileSize;
  int sectorsRead = 0;

  while (!isEOC(clus) && clus >= 2 && bytesLeft > 0 &&
         !hexDone && !flashErr) {
    uint32_t clusBase = fat.dataByteOff +
      (uint32_t)(clus - 2) * fat.secPerClus * fat.bytesPerSec;

    for (int s = 0; s < fat.secPerClus && bytesLeft > 0 &&
         !hexDone && !flashErr; s++) {
      uint16_t toRead = (bytesLeft < 512) ? (uint16_t)bytesLeft : 512;
      readFlash(secBuf, clusBase + s * fat.bytesPerSec, toRead);
      parseChunk(secBuf, toRead);
      bytesLeft -= toRead;
      sectorsRead++;

      if ((sectorsRead & 0x7) == 0) {
        displayInt(progressLine, WHITE, "Sectors: ", sectorsRead);
      }
    }

    uint16_t next = nextCluster(clus);
    clus = next;
  }

  return sectorsRead;
}

// ============================================================
// Entry point
// ============================================================
extern "C" __attribute__((section(".entry")))
int _start() {
  Bios_ClearScreen(BLACK);
  display(0, YELLOW, "=== DS203 Firmware Updater ===");

  // ---- Parse FAT boot sector ----
  if (!parseBoot()) {
    display(2, RED,  "ERROR: Bad boot sector!      ");
    displayHex(3, WHITE, "BPS=", fat.bytesPerSec);
    displayHex(4, WHITE, "SPC=", fat.secPerClus);
    display(6, WHITE,"Power cycle to recover.      ");
    while(1);
  }

  strCpy(msg, fat.fat16 ? "FAT16 " : "FAT12 ");
  char num[12];
  strCat(msg, "bps=");
  intStr(fat.bytesPerSec, num); strCat(msg, num);
  strCat(msg, " spc=");
  intStr(fat.secPerClus, num); strCat(msg, num);
  display(1, WHITE, msg);

  // ---- Find firmware file ----
  display(2, WHITE, "Searching: SYSTEM.HEX        ");

  if (!findFile()) {
    display(2, RED,  "ERROR: SYSTEM.HEX not found! ");
    display(3, WHITE,"Files on disk:               ");
    showDirEntries(4, 5);
    display(10, WHITE,"Copy SYSTEM.HEX to device.  ");
    while(1);
  }

  strCpy(msg, "Found! size=");
  intStr(fileSize, num); strCat(msg, num);
  display(2, GREEN, msg);

  // ======================================================
  // PASS 1: Verify - parse HEX, check address ranges
  // ======================================================
  display(3, YELLOW, "Pass 1: Verifying...         ");

  passMode = PASS_VERIFY;
  addrMin = 0xFFFFFFFF;
  addrMax = 0x00000000;
  resetHexParser();

  int sectorsRead = readAndParseFile(4);

  displayInt(4, WHITE, "Sectors read: ", sectorsRead);
  displayInt(5, WHITE, "Data bytes: ", totalBytes);

  if (flashErr) {
    display(7, RED,  "VERIFY FAILED!               ");
    display(8, RED,  "Addresses outside safe range!");
    displayHex(9, WHITE, "Safe start: ", SAFE_FLASH_START);
    displayHex(10, WHITE, "Safe end:   ", SAFE_FLASH_END);
    display(12, WHITE,"Power cycle to recover.      ");
    while(1);
  }

  if (totalBytes == 0 || !hexDone) {
    display(7, RED,  "ERROR: No valid HEX data!    ");
    display(8, WHITE,"Check file format.           ");
    while(1);
  }

  // Show verified address range
  displayHex(6, GREEN, "Addr min: ", addrMin);
  displayHex(7, GREEN, "Addr max: ", addrMax);

  int pages = ((addrMax - (addrMin & ~(uint32_t)(FLASH_PAGE_SIZE-1)))
               / FLASH_PAGE_SIZE) + 1;
  displayInt(8, WHITE, "Pages to flash: ", pages);

  display(10, YELLOW,"Verify OK! Press btn to flash");
  display(11, WHITE, "Or power off to abort.       ");

  // Wait for button press
  waitKeyRelease();
  waitKeyPress();
  waitKeyRelease();

  // ======================================================
  // PASS 2: Actually flash
  // === POINT OF NO RETURN ===
  // Disable interrupts. From here, PC must never enter
  // os_host flash (0x0800C000+). Only SYS BIOS (0x08004xxx)
  // and our own code (0x08024xxx) are callable.
  // ======================================================
  disable_irq();
  dsb();
  SCB_VTOR = 0x08000000;

  Bios_ClearScreen(BLACK);
  display(0, YELLOW, "=== FLASHING - DO NOT POWER OFF ===");

  passMode = PASS_FLASH;
  resetHexParser();
  initPage(0xFFFFFFFF);
  pagesFlashed = 0;

  Bios_FlashUnlock();

  display(1, WHITE, "Reading and flashing...      ");
  sectorsRead = readAndParseFile(2);

  // Flush the last page
  if (!flashErr) flushPage();

  Bios_FlashLock();

  // ---- Final status ----
  displayInt(3, WHITE, "Sectors read: ", sectorsRead);
  displayInt(4, WHITE, "Data bytes: ", totalBytes);
  displayInt(5, WHITE, "Pages flashed: ", pagesFlashed);
  displayHex(6, WHITE, "Last page: ", pageBase);

  if (flashErr) {
    display(8, RED,  "!!! FLASH FAILED !!!         ");
    display(9, RED,  "Use DFU recovery mode.       ");
    display(10,WHITE,"Hold btn during power-on.    ");
    while(1);
  }

  if (pagesFlashed == 0) {
    display(8, RED,  "WARNING: Nothing flashed!    ");
    display(9, WHITE,"Check file name and format.  ");
    while(1);
  }

  display(8, GREEN, "Flash complete!              ");

  // ======================================================
  // PASS 3: Readback verification
  // ======================================================
  display(9, YELLOW, "Pass 3: Readback verify...   ");

  passMode = PASS_READBACK;
  mismatchCount = 0;
  resetHexParser();

  sectorsRead = readAndParseFile(10);

  displayInt(10, WHITE, "Sectors read: ", sectorsRead);
  displayInt(11, WHITE, "Data bytes checked: ", totalBytes);

  if (mismatchCount > 0) {
    displayInt(12, RED, "MISMATCHES: ", mismatchCount);
    display(13, RED,  "READBACK FAILED!             ");
    display(14, WHITE,"Use DFU recovery mode.       ");
    while(1);
  }

  display(12, GREEN, "Readback OK! All verified.   ");
  display(14, WHITE, "Resetting...                ");

  for (volatile int i = 0; i < 20000000; i++);

  SCB_AIRCR = (0x5FA << 16) | (1 << 2);
  while(1);
}
