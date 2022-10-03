/* 
 * File:   BMP280.h
 * Author: Compu Fire
 *
 * Created on 13 de septiembre de 2022, 1:16
 */

#ifndef BMP280_H
#define	BMP280_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdint.h>

int calculateTemp(uint32_t temp, uint16_t dig1, uint16_t dig2, uint16_t dig3);  // Prototipos de funciones

#endif	/* BMP280_H */

