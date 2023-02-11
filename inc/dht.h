#ifndef DHT22_H_
#define DHT22_H_

/** @file dht.h
 *  @brief DHT22 header file
 *
 */

#define dht22_pin	8
#define dht22_port	6
#define dht22_gpioPin	16
#define dht22_gpioPort	5

#define DHT22		GPIO4

typedef uint8_t byte;

void dht22_delay_ms (unsigned char time);
void dht22_delay_us (unsigned char time);
byte read_dht_data(void);
void read_dht22(float *,float *);

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
#endif /* DHT22_H_ */
