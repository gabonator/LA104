// DS203 LCD Benchmark — 4 WrCmd implementations, 10000 random rects each
// Measures DWT cycle count. F1=next, F2=exit

#include <stdint.h>

#define SYS_BIOS 0x08004200

typedef void     (*fn_ClearScreen)(uint16_t Color);
typedef void     (*fn_DisplayStr)(uint16_t x, uint16_t y,
                                  uint16_t Color, uint8_t Mode, char* s);
typedef uint32_t (*fn_Get)(uint8_t Object, uint32_t Value);
typedef void     (*fn_LCD_WR_Cmd)(uint16_t cmd);
typedef void     (*fn_DMA_Wait)(void);

#define Bios_ClearScreen   ((fn_ClearScreen) (SYS_BIOS + 0x15))
#define Bios_DisplayStr    ((fn_DisplayStr)  (SYS_BIOS + 0x49))
#define Bios_Get           ((fn_Get)         (SYS_BIOS + 0x55))
#define Bios_DMA_Wait      ((fn_DMA_Wait)    (SYS_BIOS + 0x21))
#define Bios_LCD_WR_Cmd    ((fn_LCD_WR_Cmd)  0x08004753)

#define GPIOD_BSRR  (*((volatile uint32_t*)0x40011410))
#define GPIOD_BRR   (*((volatile uint32_t*)0x40011414))
#define LCD_PORT    (*((volatile uint16_t*)0x60000000))

// DWT cycle counter
#define DEMCR       (*((volatile uint32_t*)0xE000EDFC))
#define DWT_CTRL    (*((volatile uint32_t*)0xE0001000))
#define DWT_CYCCNT  (*((volatile uint32_t*)0xE0001004))

#define KEY_STATUS  4
#define KEY1_STATUS 0x4000
#define KEY2_STATUS 0x2000
#define ALL_KEYS    (KEY1_STATUS | KEY2_STATUS | 0x0100 | 0x0200)

#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0x001F
#define GREEN   0x07E0
#define BLUE    0xF800
#define YELLOW  0x07FF
#define CYAN    0xFFE0
#define MAGENTA 0xF81F

static char msg[80];

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
static void u32Dec(uint32_t v, char* b) {
  char tmp[12];
  int i = 0;
  if (v == 0) { b[0]='0'; b[1]=0; return; }
  while (v) { tmp[i++] = '0' + (v % 10); v /= 10; }
  for (int j = 0; j < i; j++) b[j] = tmp[i-1-j];
  b[i] = 0;
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
static void displayDec(int line, uint16_t color,
                       const char* prefix, uint32_t val, const char* suffix) {
  strCpy(msg, prefix);
  char dec[12]; u32Dec(val, dec);
  strCat(msg, dec);
  strCat(msg, suffix);
  // pad to clear line
  int len = 0; for (char* p = msg; *p; p++) len++;
  while (len < 38) { msg[len++] = ' '; }
  msg[len] = 0;
  display(line, color, msg);
}

static uint32_t getKeys() { return ~Bios_Get(KEY_STATUS, 0); }

static bool waitKey() {
  while (getKeys() & ALL_KEYS);
  while (!(getKeys() & ALL_KEYS));
  bool f1 = !!(getKeys() & KEY1_STATUS);
  while (getKeys() & ALL_KEYS);
  return f1;
}

// ==========================================
// Method 1: BIOS internal LCD_WR_Cmd
// ==========================================
static void wrCmd_bios(uint16_t cmd) {
  Bios_LCD_WR_Cmd(cmd);
}

// ==========================================
// Method 2: ASM ldr from literal pool (BIOS clone)
// ==========================================
static void __attribute__((noinline, naked)) wrCmd_asm(uint16_t cmd) {
  __asm volatile(
    "push {r4, lr}\n"
    "ldr  r1, 1f\n"
    "movs r2, #0x10\n"
    "lsls r2, r2, #8\n"
    "str  r2, [r1]\n"           // GPIOD_BRR = 0x1000 (RS LOW)
    "ldr  r1, 2f\n"
    "strh r0, [r1]\n"           // LCD_PORT = cmd
    "ldr  r1, 3f\n"
    "str  r2, [r1]\n"           // GPIOD_BSRR = 0x1000 (RS HIGH)
    "pop  {r4, pc}\n"
    ".align 2\n"
    "1: .word 0x40011414\n"
    "2: .word 0x60000000\n"
    "3: .word 0x40011410\n"
  );
}

// ==========================================
// Method 3: D-bus stall (volatile flash read)
// ==========================================
static void __attribute__((noinline)) wrCmd_dbus(uint16_t cmd) {
  (void)(*(volatile uint32_t*)0x08000000);
  GPIOD_BRR = 0x1000;
  LCD_PORT = cmd;
  (void)(*(volatile uint32_t*)0x08000000);
  GPIOD_BSRR = 0x1000;
}

// ==========================================
// Method 4: 10 NOPs
// ==========================================
#define NOPS10 "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop"

static void __attribute__((noinline)) wrCmd_nop10(uint16_t cmd) {
  GPIOD_BRR = 0x1000;
  __asm volatile(NOPS10 ::: "memory");
  LCD_PORT = cmd;
  __asm volatile(NOPS10 ::: "memory");
  GPIOD_BSRR = 0x1000;
}

// ==========================================
// Rectangle drawing (shared by all methods)
// ==========================================
static void rectDraw(int x1, int y1, int x2, int y2, uint16_t color,
                     void (*wrCmd)(uint16_t)) {
  x2--; y2--;
  wrCmd(0x2A);
  LCD_PORT = (y1 >> 8);
  LCD_PORT = (y1 & 0xFF);
  LCD_PORT = (y2 >> 8);
  LCD_PORT = (y2 & 0xFF);
  wrCmd(0x2B);
  LCD_PORT = (x1 >> 8);
  LCD_PORT = (x1 & 0xFF);
  LCD_PORT = (x2 >> 8);
  LCD_PORT = (x2 & 0xFF);
  wrCmd(0x2C);
  int count = (x2+1 - x1) * (y2+1 - y1);
  for (int i = 0; i < count; i++)
    LCD_PORT = color;
}

// ==========================================
// PRNG
// ==========================================
static uint32_t rngState;
static uint32_t rng() {
  rngState ^= rngState << 13;
  rngState ^= rngState >> 17;
  rngState ^= rngState << 5;
  return rngState;
}

static const uint16_t colors[] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};

// ==========================================
// Benchmark: render N rects, return cycles
// ==========================================
static uint32_t benchmark(void (*wrCmd)(uint16_t), int count) {
  rngState = 0xDEADBEEF;

  DWT_CYCCNT = 0;
  uint32_t start = DWT_CYCCNT;

  for (int i = 0; i < count; i++) {
    int x1 = rng() % 380;
    int y1 = rng() % 220;
    int w = 10 + (rng() % 40);
    int h = 10 + (rng() % 40);
    int x2 = x1 + w; if (x2 > 400) x2 = 400;
    int y2 = y1 + h; if (y2 > 240) y2 = 240;
    uint16_t c = colors[rng() % 7];
    rectDraw(x1, y1, x2, y2, c, wrCmd);
  }

  uint32_t end = DWT_CYCCNT;
  return end - start;
}

static void initDWT() {
  DEMCR |= (1 << 24);    // TRCENA
  DWT_CYCCNT = 0;
  DWT_CTRL |= 1;         // CYCCNTENA
}

struct Result {
  const char* name;
  uint32_t cycles;
  uint32_t ms;
};

#define NUM_METHODS 4
#define NUM_RECTS 10000

extern "C" __attribute__((section(".entry")))
int _start() {
  initDWT();

  Result results[NUM_METHODS];

  void (*methods[NUM_METHODS])(uint16_t) = {
    wrCmd_bios, wrCmd_asm, wrCmd_dbus, wrCmd_nop10
  };
  const char* names[NUM_METHODS] = {
    "BIOS call", "ASM ldr", "D-bus stall", "10 NOPs"
  };

  // Run benchmarks
  for (int m = 0; m < NUM_METHODS; m++) {
    Bios_ClearScreen(BLACK);
    Bios_DMA_Wait();
    strCpy(msg, "Running: ");
    strCat(msg, names[m]);
    strCat(msg, "...                   ");
    display(0, YELLOW, msg);

    uint32_t cycles = benchmark(methods[m], NUM_RECTS);
    uint32_t ms = cycles / 72000;  // 72MHz

    results[m].name = names[m];
    results[m].cycles = cycles;
    results[m].ms = ms;
  }

  // Show results
  Bios_ClearScreen(BLACK);
  Bios_DMA_Wait();

  display(0, YELLOW, "=== LCD Benchmark: 10000 rects ===    ");
  display(1, WHITE,  "Method          Cycles      ms        ");

  for (int m = 0; m < NUM_METHODS; m++) {
    int line = 3 + m * 2;

    // name
    display(line, GREEN, names[m]);

    // cycles and ms
    char cyc[12]; u32Dec(results[m].cycles, cyc);
    char msv[12]; u32Dec(results[m].ms, msv);
    strCpy(msg, "  ");
    strCat(msg, cyc);
    strCat(msg, " cyc = ");
    strCat(msg, msv);
    strCat(msg, " ms");
    int len = 0; for (char* p = msg; *p; p++) len++;
    while (len < 38) { msg[len++] = ' '; }
    msg[len] = 0;
    display(line + 1, WHITE, msg);
  }

  // Find fastest
  uint32_t best = results[0].ms;
  int bestIdx = 0;
  for (int m = 1; m < NUM_METHODS; m++) {
    if (results[m].ms < best) { best = results[m].ms; bestIdx = m; }
  }

  strCpy(msg, "Fastest: ");
  strCat(msg, names[bestIdx]);
  strCat(msg, "                        ");
  display(12, CYAN, msg);

  display(14, WHITE, "F1=rerun  F2=exit                     ");

  while (1) {
    if (!waitKey()) return 0;
    // rerun — restart from top
    return _start();
  }

  return 0;
}
