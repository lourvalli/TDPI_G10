/** @file dht.c
 *  @brief DHT22 library
 *
 */

/*==================[inclusions]=============================================*/
#include "chip.h"
#include "dht.h"
#include "sapi.h"        // <= Biblioteca sAPI

/*==================[external functions definition]==========================*/


byte dht22_pin_dat[5];

float humidity, temperature;

/** @brief Funcion que realiza un retardo en ms
 * @param time Cantidad de tiempo a retardar
 */
void dht22_delay_ms (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 20400 * time;

	for( i=delay; i>0; i-- );
}
/** @brief Funcion que realiza un retardo en microsegundos
 * @param time Cantidad de tiempo para retardar
 */
void dht22_delay_us (unsigned char time)
{
	volatile unsigned long i;
	volatile unsigned long delay;

	delay = 22 * time;

	for( i=delay; i>0; i-- );
}
/** @brief Funcion que lee 1 byte de datos del dht
 *El sensor devuelve 1s y 0s con el siguiente formato:
 *1 -> 50 uS en bajo y 70 uS en alto
 *0 -> 50 uS en bajo y 30 uS en alto
 @return un byte de datos de los 5 que devuelve el sensor
 */
byte read_dht_data(){
   int i = 0;
   byte result=0;
   int res[8]={0,0,0,0,0,0,0,0};
   for (i=0; i< 8; i++) {
      //We enter this during the first start bit (low for 50uS) of the byte

	  while(!gpioRead( DHT22 ));
      dht22_delay_us(30);
      if (gpioRead( DHT22 )){//Was: if(PINC * _BV(dht_PIN))
         result |=(1<<(7-i));
      }
      while (gpioRead( DHT22 ));

   }

   return result;
}
/** @brief funcion que lee los datos del sensor DHT22
 * primero realiza la inicializacion con los tiempos especificados
 *
 *luego lee 40 bits de datos que envia el sensor
 *
 *@param humidity parametro pasado como referencia donde se modificara el valor de la humedad
 *@param temperature parametro pasado como referencia donde se modificara el valor de la temperatura
 */
void read_dht22(float *humidity,float *temperature){
		bool_t dht_value;
	    int dht_pin_in;
	    float dhtH,dhtT;
	    int i;
	    byte dht22_dat[5],dht22_pin_checksum;
	    for(i=0;i<5;i++){
	    dht22_dat[i]=0;
	    }
	    //START SIGNAL
	    //Chip_SCU_PinMux(dht22_port,dht22_pin,SCU_MODE_PULLUP | SCU_MODE_ZIF_DIS,FUNC0); //dth22 como output
		//Chip_GPIO_SetDir(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin),!0);
	    gpioInit(DHT22, GPIO_OUTPUT);
	    delayInaccurateUs(20);
	    gpioWrite(DHT22, HIGH);
	    delayInaccurateUs(20);

	    //Chip_GPIO_ClearValue(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin));
	    gpioWrite(DHT22, LOW);
	    //dht_value = gpioRead(DHT22);

	    delayInaccurateUs(500);

	    //Chip_GPIO_SetValue(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin));
	    gpioWrite(DHT22, HIGH);
	    //dht_value = gpioRead(DHT22);

		//WAIT RESPONSE
	    //Chip_SCU_PinMux(dht22_port,dht22_pin,SCU_MODE_PULLUP  | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS,FUNC0);
	    //Chip_GPIO_SetDir(LPC_GPIO_PORT,dht22_gpioPort,(1<<dht22_gpioPin),!1);//dth22 como input
	    gpioInit(DHT22, GPIO_INPUT);

	    printf("Esperar respuesta del sensor.\n");

	    dht_value = gpioRead(DHT22);
		while(dht_value) {
			dht_value = gpioRead(DHT22);
		}
		printf("Respondio el sensor con 0.\n");

	    	    //dht_pin_in=Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin);
	    dht_value = gpioRead(DHT22);
	    while(!dht_value) {
		    dht_value = gpioRead(DHT22);
	    }
	    printf("Respondio el sensor con 1.\n");
	    //dht_pin_in=Chip_GPIO_GetPinState(LPC_GPIO_PORT,dht22_gpioPort,dht22_gpioPin);
	    delayInaccurateUs(70);
	    for(i=0;i<5;i++){
	    	dht22_dat[i]=read_dht_data();

	    }
	    dht22_pin_checksum= dht22_pin_dat[0]+dht22_pin_dat[1]+dht22_pin_dat[2]+dht22_pin_dat[3];
	    if(dht22_pin_dat[4]!=dht22_pin_checksum){
	         printf("\r\nDHT checksum error");
	    }

	    dhtH  = (dht22_dat[0]<<8) | dht22_dat[1];
	    dhtH  /= 10;
	    dhtT  = (dht22_dat[2]<<8) | dht22_dat[3];
	    dhtT /= 10;
	    *temperature = dhtT;
	    *humidity = dhtH;
}
