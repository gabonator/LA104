enum { LED5 = 8, LED1 = 0, LED2 = 1, LED3 = 3, LED4 = 2, SWITCH = 10};
int leds[] = {LED1, LED2, LED3, LED4, LED5};

#define ledPin 10
uint8_t counter = 0;

volatile uint8_t level1 = 0;
volatile uint8_t level2 = 0;
volatile uint8_t level3 = 0;
volatile uint8_t level4 = 0;
volatile uint8_t level5 = 0;

void setupLowPower()
{
  #define BODS 7 //BOD Sleep bit in MCUCR
  #define BODSE 2 //BOD Sleep enable bit in MCUCR
  MCUCR |= _BV(BODS) | _BV(BODSE); //turn off the brown-out detector


  ADCSRA &= ~ bit(ADEN); // disable the ADC
  bitSet(PRR, PRADC); // power down the ADC
}

void setupTimer1() {
  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 1 Hz (1000000/((15624+1)*64))
  OCR1A = 2;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 64
  //TCCR1B |= (1 << CS11) | (1 << CS10);
  // Prescaler 1
  TCCR1B |= (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

ISR(TIM1_COMPA_vect) {
  uint8_t b = counter++;
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  uint8_t v = 0;
  v |= (b < level1) << 0;
  v |= (b < level2) << 1;
  v |= (b < level3) << 3;
  v |= (b < level4) << 2;
  PORTA = v;
  v = 1;
  v |= (b < level5) << 2;
  PORTB = v;
}

// the setup function runs once when you press reset or power the board
void setup() {
  setupLowPower();
  
  pinMode(SWITCH, INPUT);
  digitalWrite(SWITCH, 1);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
  digitalWrite(LED3, 0);
  digitalWrite(LED4, 0);
  digitalWrite(LED5, 0);
  
  setupTimer1();
}

void upd(volatile uint8_t& r, uint8_t p)
{
  int level = max(255-p, p);  
  level = max(0, level-190);
  r = level;
}

void wheel()
{
  static uint16_t posfix = 0;
  static int speed = 1<<8;
  static int dspeed = 50;

  posfix += speed;
  speed += dspeed;
  if (speed>(30<<8))
    dspeed = -dspeed;
  if (speed<-(30<<8))
    dspeed = -dspeed;
  
  uint8_t pos = posfix >> 8;
  upd(level1, pos-255*0/5);
  upd(level2, pos-255*1/5);
  upd(level3, pos-255*2/5);
  upd(level4, pos-255*3/5);
  upd(level5, pos-255*4/5);
}

void twinkle()
{
  static int l1 = 0, l2 = 0, l3 = 0, l4 = 0, l5 = 0;
  static int d1 = 23*3, d2 = 17*3, d3 = 13*3, d4 = 5*3, d5 = 4*3;
  constexpr int maxl = 128<<4;

  auto upd = [](int &l, int& d, volatile uint8_t& r)
  {
    l += d;
    if (l > maxl) { l = maxl; d = -d; }
    if (l < (0<<4)) { l = 0; d = -d; }
    
    int x = l >> 4;
    x = max(0, x-32-64);
    r = x;
  };

  upd(l1, d1, level1);
  upd(l2, d2, level2);
  upd(l3, d3, level3);
  upd(l4, d4, level4);
  upd(l5, d5, level5);
}

void blink()
{
  static uint8_t t = 0;
  constexpr int maxt = 50;

  uint8_t l = (t > maxt/2) ? 32 : 0;
  if (t++ >= maxt)
    t = 0;

  level1 = level2 = level3 = level4 = level5 = l;
}

void blank()
{
  if (level1 > 0) level1 --;
  if (level2 > 0) level2 --;
  if (level3 > 0) level3 --;
  if (level4 > 0) level4 --;
  if (level5 > 0) level5 --;
  if (level1 == 0 && level2 == 0 && level3 == 0 && level4 == 0 && level5 == 0)
  {
    // go to sleep
  }
}

void loop() 
{
  static uint8_t mode = 0;
  static uint8_t inhibit = 10;
  if (inhibit == 0)
  {
    if (digitalRead(SWITCH) == 0)
    {
      mode = (mode+1)%6;
      inhibit = 10;
    }
  } else 
  {
    if (digitalRead(SWITCH) == 0)
      inhibit = 10;
    else
      inhibit--;
  }
  
  switch (mode)
  {
    case 0: twinkle(); break;
    case 1: blank(); break;
    case 2: wheel(); break;    
    case 3: blank(); break;
    case 4: blink(); break;    
    case 5: blank(); break;
  }
  delay(5);
}
