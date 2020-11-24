enum { LED5 = 8, LED1 = 0, LED2 = 1, LED3 = 3, LED4 = 2, SWITCH = 10};
int leds[] = {LED1, LED2, LED3, LED4, LED5};
 
// the setup function runs once when you press reset or power the board
void setup() {
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

}

void loop() 
{
  while (digitalRead(SWITCH) == 1);

  for (int i=0; i<5; i++)
    digitalWrite(leds[i], 1);
  delay(200);
  while (digitalRead(SWITCH) == 0);
  delay(100);
  
  while (digitalRead(SWITCH) == 1);
    for (int i=0; i<5; i++)
      digitalWrite(leds[i], 0);
   delay(200);
  while (digitalRead(SWITCH) == 0);
   delay(100);
}
