#ifndef __DHT11_H__
#define __DHT11_H__
#include"stdint.h"


void microDelay (uint16_t delay);
uint8_t DHT11_Start (void);
uint8_t DHT11_Read (void);
void read_dht11(void);

#endif /* DHT11_H_ */
