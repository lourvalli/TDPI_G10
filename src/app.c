#include "app.h"         // <= Su propia cabecera
#include "sapi.h"        // <= Biblioteca sAPI

/*
void dht11ReadMock (float * humidity, float * temperature){
   if(*humidity < 90) *humidity +=10;
      else *humidity = 0;
   if(*temperature < 50) *temperature +=5;
      else *temperature = 0;
}
*/

int main( void )
{

   float humidity=0, temperature=0;
   unsigned int light = 0;
   unsigned char sprinkler = false, fan = false;

   init();

   while( true ) {
      // read light value from LDR
      light = 100 - adcRead( CH1 )*100/1024;

      // read humidity and temperature from DHT22      
      dht11Read(&humidity,&temperature);

      // control sprinkler
      if (humidity < 50 & !sprinkler) {
         sprinkler = true;
         gpioWrite( SPRINKLER_PIN, ON );
         printUart( "Aspersor encendido.\r\n" );
      }
      else if (humidity > 60 & sprinkler) {
         sprinkler = false;
         gpioWrite( SPRINKLER_PIN, OFF );
         printUart( "Aspersor apagado.\r\n" );
      }

      // control fan
      if (temperature > 25 & !fan) {
         fan = true;
         gpioWrite( FAN_PIN, ON );
         printUart( "Ventilador encendido.\r\n" );
      }
      else if (temperature < 23 & fan) {
         fan = false;
         gpioWrite( FAN_PIN, OFF );
         printUart( "Ventilador apagado.\r\n" );
      }

      sendStatus(humidity,temperature,light,sprinkler,fan);
      

      // Esperar durante 30 segundos
      delay(30000);
         
   }
   
   return 0;
}

void init(){
   // Inicializar y configurar la plataforma
   boardConfig();
   
   // Inicializar sensor DHT22
   dht11Init(DHT_PIN);

   // Inicializar puertos GPIO
   initGPIOs();

   adcConfig( ADC_ENABLE ); /* ADC */

   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_232, 115200 );
   
   /* Inicializar LCD de 16x2 (caracteres x lineas) con cada caracter de 5x8 pixeles */
   //lcdInit( 16, 2, 5, 8 );
   //lcdCursorSet( LCD_CURSOR_OFF ); // Apaga el cursor
   //lcdClear();                     // Borrar la pantalla
}

void initGPIOs() {
   gpioInit(FAN_PIN, GPIO_OUTPUT);
   gpioInit(SPRINKLER_PIN, GPIO_OUTPUT);
}

void printUart( char * msg){
   uartWriteString( UART_232, msg);
}
void sendStatus(float humidity, float temperature, unsigned int light, unsigned int sprinkler, unsigned int fan){
	writeMQTT( humidity,  temperature,  light, sprinkler, fan);
	//writeLCD( humidity,  temperature,  light, sprinkler, fan);
}

void writeMQTT(float humidity, float temperature, unsigned int light, unsigned int sprinkler, unsigned int fan){
   static char *status [2] = {"OFF","ON"};
   static char buffer[100];
   
   sprintf(buffer,"Humedad: %d%% | Temperatura: %d | Luz: %d%% | Aspersores: %s | Ventiladores: %s\n", (int) humidity, (int) temperature, light, status[sprinkler], status[fan]);
   
   // send data to MQTT through ESP8266
   printUart(buffer);
}

void writeLCD(float humidity, float temperature, unsigned int light, unsigned int sprinkler, unsigned int fan) {
   static char *status[2] = {"OFF","ON "};
   static char line1[16];
   static char line2[16];

   sprintf(line1,"T:%d H:%d%% L:%d%%", (int) temperature, (int) humidity, light);
   sprintf(line2,"  V:%s  A:%s  ", status[sprinkler], status[fan]);

   lcdGoToXY( 0, 0 ); // Poner cursor en 0, 0
   lcdSendStringRaw( line1 );

   lcdGoToXY( 0, 1 ); // Poner cursor en 0, 1
   lcdSendStringRaw( line2 );

}


