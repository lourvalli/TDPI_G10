#include "app.h"         // <= Su propia cabecera
#include "sapi.h"        // <= Biblioteca sAPI
//#include "lcd.h"


char line1[16],line2[16];
static char uartBuff[100];
static char *  cons [2] = {"OFF","ON"};

void dht11ReadMock (float * humidity, float * temperature){
   if(*humidity < 90) *humidity +=10;
      else *humidity = 0;
   if(*temperature < 50) *temperature +=5;
      else *temperature = 0;
}


int main( void )
{
   init();

   float humidity=0, temperature=0;
   unsigned int light = 0;
   unsigned char sprinkler = 0;
   unsigned char fan = 0;

   while( TRUE ) {
      dht11Read(&humidity,&temperature);
      light = 100 - adcRead( CH1 )*100/1024;
      gpioWrite( GPIO4, ON );

      sendStatus(humidity,temperature,light,sprinkler,fan);
      
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

      //LCD_write_char('\f');
      //LCD_write_string(&line1);
      //LCD_write_char('\r');
      //LCD_write_string(&line2);
      
      delay(5000);
         
   }
   
   return 0;
}

void initGPIOs() {
	gpioInit(FAN_PIN, GPIO_OUTPUT);
	gpioInit(SPRINKLER_PIN, GPIO_OUTPUT);
}

void printUart( char * msg){
   uartWriteString( UART_232, msg);
}

void init(){
   // Inicializar y configurar la plataforma
   boardConfig();
   
   dht11Init(DHT_PIN);
   /* Inicializar UART_USB a 115200 baudios */
   uartConfig( UART_232, 115200 );

   initGPIOs();
   adcConfig( ADC_ENABLE ); /* ADC */
   
   /** @fn LCD_init initializes LCD*/
   //LCD_init(DISPLAY_8X5|_2_LINES,DISPLAY_ON|CURSOR_OFF|CURSOR_BLINK);
}

void pantallaPpal_a_string() {
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

void sendStatus(float humidity, float temperature, unsigned int light, unsigned int sprinkler, unsigned int fan){
   sprintf(uartBuff,"Humedad: %d%% | Temperatura: %d | Luz: %d%% | Aspersores: %s | Ventiladores: %s\n",(int) humidity,(int) temperature,light,cons[sprinkler],cons[fan]);
   printUart(uartBuff);
}


