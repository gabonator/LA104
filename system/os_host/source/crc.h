#if defined(LA104) || defined(DS213)
// https://stackoverflow.com/questions/21001659/crc32-algorithm-implementation-in-c-without-a-look-up-table-and-with-a-public-li

extern unsigned long _addressRomBegin;
extern unsigned long _addressRomEnd;

uint32_t crc32b(const uint8_t *message, int length) {
   uint32_t crc, mask;

   crc = 0xFFFFFFFF;
   while (length--) {
      crc = crc ^ *message++;
      for (int j = 7; j >= 0; j--) 
      {
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
   }
   return ~crc;
}
#endif
