#include "BMP280.h"

int calculateTemp(uint32_t temp, uint16_t dig1, uint16_t dig2, uint16_t dig3){
  uint32_t var1, var2;

  var1 = ((((temp >> 3) - ((uint32_t)dig1 << 1))) *((uint32_t)dig2)) >>11;

  var2 = (((((temp >> 4) - ((uint32_t)dig1)) *((temp >> 4) - ((uint32_t)dig1))) >> 12) *
          ((uint32_t)dig3)) >> 14;

  uint32_t t_fine = var1 + var2;

  int T = (t_fine * 5 + 128) >> 8;
  
  return T;
}