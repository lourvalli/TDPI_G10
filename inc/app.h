/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/

#ifndef _APP_H_
#define _APP_H_

#define FAN_PIN			GPIO0
#define SPRINKLER_PIN	GPIO2
#define DHT22_PIN		GPIO4

void initGPIOs();
void pantallaPpal_a_string(void);
void printUart( char * msg);
void init ();
void sendStatus(float humidity, float temperature, unsigned int light, unsigned int sprinkler, unsigned int fan);

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
