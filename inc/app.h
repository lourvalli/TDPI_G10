/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/

#ifndef _APP_H_
#define _APP_H_

#define FAN_PIN			GPIO0
#define SPRINKLER_PIN	GPIO2
#define DHT_PIN         GPIO4

void init();
void initGPIOs();
void printUart( char * msg);
void writeMQTT(float humidity, float temperature, unsigned int light, unsigned int sprinkler, unsigned int fan);
void writeLCD(float humidity, float temperature, unsigned int light, unsigned int sprinkler, unsigned int fan);

/*==================[inclusiones]============================================*/

/*==================[c++]====================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================[macros]=================================================*/

/*==================[tipos de datos declarados por el usuario]===============*/

/*==================[declaraciones de datos externos]========================*/

/*==================[declaraciones de funciones externas]====================*/


/*==================[c++]====================================================*/
#ifdef __cplusplus
}
#endif

/*==================[end of file]============================================*/
#endif /* _APP_H_ */
