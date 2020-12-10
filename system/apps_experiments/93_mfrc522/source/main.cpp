    
#include <library.h>
#include "../../os_host/source/framework/Console.h"
//#include "Adafruit_BMP085.h"
#include "mfrc522/MFRC522.h"

uint32_t* gpioStatus = nullptr;

void Serial_print(char *s) { CONSOLE::Print(s); }
void Serial_println(char *s) { CONSOLE::Print("%s\n", s); }
void Serial_print(const char *s) { CONSOLE::Print(s); }
void Serial_println(const char *s) { CONSOLE::Print("%s\n", s); }
void Serial_print(int n) { CONSOLE::Print("%d", n); }
void Serial_print(int n, int f) { CONSOLE::Print("%02x", n); }
void Serial_println(int n, int f) { CONSOLE::Print("%02x\n", n); }
void Serial_println() { CONSOLE::Print("\n"); }
long millis() { return BIOS::SYS::GetTick(); }
bool CWire::error() {
      int err = *gpioStatus;
      if (err)
      {
        CONSOLE::Color(RGB565(ffff00));
        CONSOLE::Print("<error %d>", err);
        CONSOLE::Color(RGB565(ffffff));        
        *gpioStatus = 0;
        return true;
      }
      return false;
}

using namespace BIOS;

//MFRC522_I2C dev = MFRC522_I2C(BIOS::GPIO::EPin::P3, 0x28);
MFRC522_SPI dev = MFRC522_SPI(0);
MFRC522 mfrc522 = MFRC522(&dev);


// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[][MFRC522::MF_KEY_SIZE] =  {
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF = factory default
    {0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5}, // A0 A1 A2 A3 A4 A5
    {0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5}, // B0 B1 B2 B3 B4 B5
    {0x4d, 0x3a, 0x99, 0xc3, 0x51, 0xdd}, // 4D 3A 99 C3 51 DD
    {0x1a, 0x98, 0x2c, 0x7e, 0x45, 0x9a}, // 1A 98 2C 7E 45 9A
    {0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7}, // D3 F7 D3 F7 D3 F7
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // AA BB CC DD EE FF
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // 00 00 00 00 00 00
};

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        CONSOLE::Print("%02x", buffer[i]);
    }
}

bool try_key(MFRC522::MIFARE_Key *key)
{
    bool result = false;
    byte buffer[18];
    byte block = 0;
    MFRC522::StatusCode status;
        CONSOLE::Print(F("Try key:"));
        dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
        CONSOLE::Print(F(" "));
    
    // http://arduino.stackexchange.com/a/14316
      if (!mfrc522.PICC_IsNewCardPresent()) {
        SYS::DelayMs(500);
        if (!mfrc522.PICC_IsNewCardPresent()) {
          CONSOLE::Print(F("not present\n"));
          return true;
        }
      }

/*
    if ( ! mfrc522.PICC_IsNewCardPresent())
{
        CONSOLE::Print(F("not present\n"));

        return false;
} */
    if ( ! mfrc522.PICC_ReadCardSerial())
{
        CONSOLE::Print(F("no serial\n"));

        return true;
}
    // Serial.println(F("Authenticating using key A..."));

//        CONSOLE::Print(F("\n"));
//        CONSOLE::Print(F("\n"));

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      CONSOLE::Print(F("Auth failed X: err:%s\n"), mfrc522.GetStatusCodeName(status));
        // Serial.print(F("PCD_Authenticate() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::STATUS_OK) {
      CONSOLE::Print(F("Auth failed Y: %02x\n"), mfrc522.GetStatusCodeName(status));
        // Serial.print(F("MIFARE_Read() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
//        CONSOLE::Print(F("Success with key:"));
//        dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
//        CONSOLE::Print(F("\n"));
        // Dump block data
        CONSOLE::Print(F("Success! Block %d:\n"), block);
        dump_byte_array(buffer, 16);
        CONSOLE::Print(F("\n"));
    }

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}

void TryKeys()
{
   // Try the known default keys
   MFRC522::MIFARE_Key key;
   for (byte k = 0; k < COUNT(knownKeys); k++) {
       // Copy the known key into the MIFARE_Key structure
       for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
           key.keyByte[i] = knownKeys[k][i];
       }
       // Try the key
       if (try_key(&key)) {
           // Found and reported on the key and block,
           // no need to try other keys for this PICC
           break;
       }
   }
   BIOS::SYS::DelayMs(1000);
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    gpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);
    CONSOLE::window.left = 8;
    CONSOLE::cursor = CONSOLE::window.TopLeft();

    CRect rcClient(0, 0, LCD::Width, LCD::Height);
    LCD::Bar(rcClient, RGB565(0000b0));
    
    CRect rc1(rcClient);
    rc1.bottom = 14;
    GUI::Background(rc1, RGB565(4040b0), RGB565(404040));    
    LCD::Print(8, rc1.top, RGB565(ffffff), RGBTRANS, "MFRC522 I2C test");

    CRect rc2(rcClient);
    rc2.top = rc2.bottom-14;
    GUI::Background(rc2, RGB565(404040), RGB565(202020));
    LCD::Print(8, rc2.top, RGB565(808080), RGBTRANS, "P1: SCL, P2: SDA, P3: Reset");

    KEY::EKey key;
    bool initOk = false;
    while ((key = KEY::GetKey()) != KEY::EKey::Escape)
    {
      if (!initOk)
      {
        char ver[64];
        mfrc522.PCD_Init();		// Init MFRC522
        mfrc522.PCD_DumpVersionToSerial(ver);	// Show details of PCD - MFRC522 Card Reader details
        if (strlen(ver) > 0)
        {
          CONSOLE::Print("%s\n", ver);
          CONSOLE::Print("Waiting for card...");
          initOk = true;
        } else
        {
          CONSOLE::Print("Not connected\n");
          BIOS::SYS::DelayMs(5000);
          continue;
        }
      }

      // Look for new cards
      if (!mfrc522.PICC_IsNewCardPresent()) {
//        EVERY(5000)
        {
          CONSOLE::Print(".");
        }
        SYS::DelayMs(20);
        continue; // no card in sight.
      }

      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) {
        CONSOLE::Print("\nBad read (was card removed too quickly?)\n");
        SYS::DelayMs(500);
        continue;
      };
      
      if ( mfrc522.uid.size == 0) {
        CONSOLE::Print("\nBad card read (size = 0)\n");
        SYS::DelayMs(500);
        continue;
      }

      BIOS::SYS::Beep(200);

      char buff[sizeof(mfrc522.uid.uidByte)* 5] = { 0 };
      for (int i = 0; i < mfrc522.uid.size; i++) {
        char tag[5]; // 3 digits, dash and \0.
        sprintf(tag, "%s%02x", i ? "-" : "", mfrc522.uid.uidByte[i]);
        strcat(buff, tag);
      };
      CONSOLE::Print("\nGood scan: %s\n", buff);
      // disengage with the card.
      //

      TryKeys();
      mfrc522.PICC_HaltA();


      SYS::DelayMs(20);
    }

    Wire.end();
    
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    CONSOLE::Color(RGB565(ffff00));
    CONSOLE::Print("Assertion failed in ");
    CONSOLE::Print(file);
    CONSOLE::Print(" [%d]: %s\n", line, cond);
#ifdef __APPLE__
    //kill(getpid(), SIGSTOP);
#endif
    while (1);
}

