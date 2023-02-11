/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

// Inlcusiones

#include "app.h"         // <= Su propia cabecera
#include "sapi.h"        // <= Biblioteca sAPI
#include "dht.h"
//#include "lcd.h"

char line1[16],line2[16];

void pantallaPpal_a_string(void) {
	line1[0] = 'T';
	line1[1] = 'e';
	line1[2] = 'm';
	line1[3] = 'p';
	line1[4] = ':';
	line1[5] = ' ';
	line1[6] = '2';
	line1[7] = '2';
	line1[8] = '.';
	line1[9] = '2';
	line1[10] = ' ';
	line1[11] = '*';
	line1[12] = 'C';
	line1[13] = ' ';
	line1[14] = ' ';
	line1[15] = ' ';

	line2[0] = 'H';
	line2[1] = 'u';
	line2[2] = 'm';
	line2[3] = ':';
	line2[4] = ' ';
	line2[5] = '2';
	line2[6] = '2';
	line2[7] = '.';
	line2[8] = '2';
	line2[9] = ' ';
	line2[10] = '%';
	line2[11] = ' ';
	line2[12] = ' ';
	line2[13] = ' ';
	line2[14] = ' ';
	line2[15] = ' ';
}


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void )
{
   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_USB, 115200 );

   initGPIOs();
   adcConfig( ADC_ENABLE ); /* ADC */

   /** @fn LCD_init initializes LCD*/
   	//LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);

   	// Crear varias variables del tipo booleano
   bool_t buttonValue = OFF;
   bool_t ledValue    = OFF;

   bool_t sprinkler 	 = OFF;
   bool_t sprinklerValue = OFF;
   bool_t fan       	 = OFF;
   bool_t fanValue    	 = OFF;

   float humidity=0, temperature=0;

   /* Variable para almacenar el valor leido del ADC CH1 */
   uint16_t light1 = 0;
   uint16_t light2 = 0;
   uint16_t light3 = 0;
   uint16_t light4 = 0;

   /* Buffer */
   static char uartBuff[10];

   // Crear variable del tipo tick_t para contar tiempo
   tick_t timeCount   = 0;


   char* itoa(int value, char* result, int base) {
      // check that the base if valid
      if (base < 2 || base > 36) { *result = '\0'; return result; }

      char* ptr = result, *ptr1 = result, tmp_char;
      int tmp_value;

      do {
         tmp_value = value;
         value /= base;
         *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
      } while ( value );

      // Apply negative sign
      if (tmp_value < 0) *ptr++ = '-';
      *ptr-- = '\0';
      while(ptr1 < ptr) {
         tmp_char = *ptr;
         *ptr--= *ptr1;
         *ptr1++ = tmp_char;
      }
      return result;
   }


   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE ) {

      /* Retardo bloqueante durante 100ms */
      
      delay( 100 );
      
      /* Si pas� 1 segundo comienza a funcionar el programa que copia las
         acciones en BOTON al LED. Mientras espera titila el LED.  */
      
      timeCount++;      
      
      if( timeCount == 10 ){ // 100ms * 10 = 1s
         
         while( TRUE ) {
            
            /* Si se presiona CIAA_BOARD_BUTTON, enciende el CIAA_BOARD_LED */

            // Leer pin conectado al boton.
            buttonValue = gpioRead( CIAA_BOARD_BUTTON );
            // Invertir el valor leido, pues lee un 0 (OFF) con boton
            // presionado y 1 (ON) al liberarla.
            buttonValue = !buttonValue;
            // Escribir el valor leido en el LED correspondiente.
            gpioWrite( CIAA_BOARD_LED, buttonValue );

            /* Enviar a la salida estandar (UART_DEBUG) el estado del LED */
            
            // Leer el estado del pin conectado al led
            ledValue = gpioRead( CIAA_BOARD_LED );
            // Chequear si el valor leido es encedido
            if( ledValue == ON ) {
               // Si esta encendido mostrar por UART_USB "LED encendido."
               printf( "LED encendido.\r\n" );
            } else {
               // Si esta apagado mostrar por UART_USB "LED apagado."
               printf( "LED apagado.\r\n" );
            }


            // Esto no funciona
            read_dht22(&humidity,&temperature);
            if (temperature > 25) {
            	fan = ON;
            }
            if (temperature < 23) {
            	fan = OFF;
            }
            //gpioWrite( FAN, fan );

            if (humidity < 50) {
				sprinkler = ON;
			}
			if (humidity > 60) {
				sprinkler = OFF;
			}
			//gpioWrite( SPRINKLER, sprinkler );




            // vamos a suponer que el button TEC1 indica prender o apagar el ventilador
            // y el button TEC2 indica prender o apagar aspersor
            fan = gpioRead( FAN_BUTTON );
            fan = !fan;
            gpioWrite( FAN, fan );

            // Leer el estado del ventilador
			fanValue = gpioRead( FAN );
			// Chequear si el valor leido es encedido
			if( fanValue == ON ) {
			   // Si esta encendido mostrar por UART_USB "Ventilador encendido."
			   printf( "Ventilador encendido.\r\n" );
			} else {
			   // Si esta apagado mostrar por UART_USB "Ventilador apagado."
			   printf( "Ventilador apagado.\r\n" );
			}


            sprinkler = gpioRead( SPRINKLER_BUTTON );
            sprinkler = !sprinkler;
			gpioWrite( SPRINKLER, sprinkler );

			// Leer el estado del aspersor
			sprinklerValue = gpioRead( SPRINKLER );
			// Chequear si el valor leido es encedido
			if( sprinklerValue == ON ) {
			   // Si esta encendido mostrar por UART_USB "Aspersor encendido."
			   printf( "Aspersor encendido.\r\n" );
			} else {
			   // Si esta apagado mostrar por UART_USB "Aspersor apagado."
			   printf( "Aspersor apagado.\r\n" );
			}



			light1 = adcRead( CH1 );

			/* Envío la primer parte del mnesaje a la Uart */
			uartWriteString( UART_USB, "ADC CH1 value: " );

			/* Conversión de muestra entera a ascii con base decimal */
			itoa( light1, uartBuff, 10 ); /* 10 significa decimal */

			/* Enviar muestra y Enter */
			uartWriteString( UART_USB, uartBuff );
			uartWriteString( UART_USB, ";\r\n" );



	        //LCD_write_char('\f');
			//LCD_write_string(&line1);
			//LCD_write_char('\r');
			//LCD_write_string(&line2);



			delay( 250 );
            
         }
      } else {
         // Intercambiar el valor de CIAA_BOARD_LED
         gpioToggle(CIAA_BOARD_LED);
      }
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontrolador y no es llamado por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}


void initGPIOs() {
	gpioInit(FAN, GPIO_OUTPUT);
	gpioInit(SPRINKLER, GPIO_OUTPUT);
}
