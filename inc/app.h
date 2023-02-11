/*============================================================================
 * Licencia:
 * Autor:
 * Fecha:
 *===========================================================================*/

#ifndef _APP_H_
#define _APP_H_

#define FAN			GPIO0
#define SPRINKLER	GPIO2
#define DHT22		GPIO4

#define FAN_BUTTON			TEC1
#define SPRINKLER_BUTTON	TEC2

void initGPIOs();

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
