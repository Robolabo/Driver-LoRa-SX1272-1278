/*******************************************************************************
 * Prueba de inicio para comunicación entre dos SX1278
 * Aplicación Ping-Pong - Dispositivo Maestro
 * 29/07/16
 *******************************************************************************/

#include <radio.h>                    //Librería de alto nivel para configuración por software de los radio LoRa
#include <hal/halLora.h>              //Librería de bajo nivel para configuración de hardware radio LoRa (HAL)
#include <SPI.h>                      //Librería para manejo de comunicación SPI

#include <TimerOne.h>                 //Librería incluida para generar interrupciones por TIMER


//Definición de los pines de control de la tarjeta Arduino
const radio_pinmap radio_pins = {
    .nss = 10,
    .rxtx =RADIO_UNUSED_PIN,
    .rst = 9,
    .dio0 = 2,        
   
#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)       //Define la versión/fabricante del SX1278
    .dio3 = 5,    
#endif
#if defined(CFG_1278_NICERF1278)                                      //Selecciona el CHIP SX1278 de NICERF
    .tx_en = 3,
    .rx_en = 4,
#endif  
};


//Declaración de variables
int flagEnvio;
int contadorSeg_TX;
int contadorSeg_RX;

int segundosEsperaTX;
int segundosEsperaRX;

int CONTADOR = 0X0000;
int contadorBytesRX = 0x0000;

String ContadorString;
char BufferNumero[5];

char envioSolicitud[3];// = {'T', 'x', 'N'};              //El último Byte sera modificado para comprobar si se envio o no los datos



//Función Interrupción de timer cada segundo
void ISR_RETARDO()                                        //retardo por timer cada un segundo
{
  if(RADIO.statusRXTX == RADIO_TX)
  {
    contadorSeg_TX++;
    Serial.print(contadorSeg_TX);
  }
  else if(RADIO.statusRXTX == RADIO_RX)
  {
    contadorSeg_RX++;
  }
}




//Almacena los datos de buffer definido por el usuario al buffer RADIO.frameTX[...] utilizado por el driver
void buffer_to_frameTX( char *str, int datalen) 
{
  int i;
  RADIO.dataLenTX = datalen;
  for (i=0;i<datalen;i++)
  {
    RADIO.frameTX[i] = *str++;
  }
}



//Configuración de los dispositivos LoRa
void setup()        //Función de configuración inicial
{
    //Configuracion de comunicacion para realizar debugger 
    Serial.begin(115200);
    Serial.println("MASTER");     //Nombre del dispositivo
    delay(1000);          //Retardo
  
    #ifdef VCC_ENABLE
      pinMode(VCC_ENABLE, OUTPUT);
      digitalWrite(VCC_ENABLE, HIGH);
      delay(1000);
    #endif
     pinMode(LED_BUILTIN, OUTPUT);    //Configuración de pines



    //Inicialización de variables de aplicación general
    flagEnvio = 0;
    contadorSeg_TX = 0;
    contadorSeg_RX = 0;
    contadorBytesRX = 0;
    segundosEsperaTX = 3;       //Se debe aumentar de acuerdo a la configuración SF y BW
    segundosEsperaRX = 5;        //Se debe aumentar de acuerdo a la configuración SF y BW


    
    //Inicialización de variables del driver
    RADIO.flagTx = 0;      
    RADIO.flagRx =0;
    RADIO.crc = 0;



    //Intervalo de frecuencia de 433.050 MHz to 434.790 MHz: BW: 1.74 MHz
    //Configuración de frecuencia
    RADIO.freq = 433800000;             //Frecuencias ISM para SX1278
    //RADIO.freq = 869525000;             //Frecuencias ISM para SX172

    //Configuración de potencia 
    RADIO.txpow = 17;                   //Maxima TX power
    RADIO.imax = 100;                   //Por defecto 100mA   45mA <=Imax <= 240mA


/*
Aumento máximo de ganancia. LNA máx. corriente 150%  LNA_RX_GAIN
Máxima capacidad del Payload 64 
LORARegSymbTimeoutLsb tiempo Timeout de símbolos es 0 
Palabra reservada para Lora networks LORARegSyncWord 0x34
Configuración de Pines DIO0=RXDONE DIO3=PAYLOADCRCERROR
Configura LoRa modem (RegModemConfig1, RegModemConfig2) valores de RADIO.sf RADIO.bw RADIO.cr
Configura el Canal de acuerdo con la frecuencia de RADIO.freq en 3 registros 
Configura la potencia a Max potencia RADIO.txpow. Potencia limitada hasta 20dBm
RADIO.sf = SF_6, SF_7, SF_8, SF_9, SF_10, SF_11, SF_12 
RADIO.cr = CR_4_5, CR_4_6, CR_4_7, CR_4_8       
RADIO.bw = BW7_8, BW10_4, BW15_6, BW20_8, BW31_25, BW41_7, BW62_5, BW125, BW250, BW500, BWrfu 
*/

    //Variables de configuración de la comunicación LoRa
    RADIO.sf = SF_9;                //Configuración del Spread factor
    RADIO.bw = BW500;               //Configuración del ancho de canal 
    RADIO.cr = CR_4_5;              //Configuracion de Coding rate
    


    RADIO.statusRXTX = RADIO_RST;  //Se incorpora para modificar en el main*loop

    
    hal_init();                     //Configuración de puertos de arduino
    radio_init();                   //Configuración de inicio para los LoRa

  
    Serial.println("Inicia");
    Serial.flush();


     envioSolicitud[0] = 'T';
     envioSolicitud[1] = 'x';
     envioSolicitud[2] = 'Y';
    Serial.print("Version ");
    Serial.print(RADIO.version);

    if(RADIO.version != versionLora)            //Debugger comprobación de funcionamiento
    {
      Serial.print("LoRa no conectado \n");
    }
}


void loop() 
{
  
//------>Bucle ciclico <-------
  //tx_buffer
  if((flagEnvio == 0) && (RADIO.statusRXTX == RADIO_RST))        // RADIO.flags != 0
  {
      
      radio_mode(RADIO_RST);                                  //Detiene la RX antes
      flagEnvio = 1;
      RADIO.statusRXTX = RADIO_TX;                         //Flag para determinar donde esta el LORA-> esto se puede hacer desde dentro las librerias

      contadorSeg_TX=0;                         //Inicializa contador de seg TX
      contadorSeg_RX=0;                         //Inicializa contador de seg RX


      buffer_to_frameTX(envioSolicitud, 3);                  //trasnmite 3 bytes


    Serial.print("Envia ");
    Serial.print(RADIO.dataLenTX,DEC);
    
    radio_mode(RADIO_TX);     //Configura la TX
    
    Serial.println(" Cargo Datos al FIFO");
    Serial.write(RADIO.frameTX, RADIO.dataLenTX);


      contadorSeg_TX=0;                         //Inicializa contador de seg TX
      contadorSeg_RX=0;                         //Inicializa contador de seg RX
      Timer1.initialize(1000000);               // Dispara cada 1000 ms
      Timer1.attachInterrupt(ISR_RETARDO);      //->Retardo
  }

  if((flagEnvio == 1)  && (RADIO.flagTx == 1))         //Ha recibido la confirmacion del modem por TX correcta
  {
      flagEnvio = 0;
      RADIO.flagTx = 0;      
      Serial.println("Confirma TX \n");
      Timer1.initialize(1000000);             // Dispara cada 1000 ms
      radio_mode(RADIO_RX);                     //Pasa a MODO RX
      RADIO.statusRXTX = RADIO_RX;             // Utilizado como flag coloca la variable de modo en RX
      contadorSeg_RX=0;                         //Inicializa contador de seg RX
      contadorSeg_TX=0;                         //Inicializa contador de seg TX
      envioSolicitud[2] = 'N';                  //Nuevo dato
      //continua esperando
  }
  if ((contadorSeg_TX >= segundosEsperaTX) && (RADIO.statusRXTX == RADIO_TX))
  {
     Serial.println("Superado el tiempo de TX. Vuelve a TX \n");
     flagEnvio = 0;
     Timer1.initialize(1000000);             // Dispara cada 1000 ms
     RADIO.statusRXTX = RADIO_RST;           //<- RST
     contadorSeg_TX=0;
     contadorSeg_RX=0;
  }

  if(RADIO.statusRXTX == RADIO_RX)             //-> Configurado para recibir
  {
     if(RADIO.flagRx == 0)                   //Nota ha recibido los datos
     {
          if (contadorSeg_RX >= segundosEsperaRX)
          {
             Serial.println("Superado el tiempo de RX. Vuelve para Solicitar ... \n");
             envioSolicitud[2] = 'F';        //DATO ERRADO o NO RECIBIDO 
             flagEnvio = 0;
             contadorSeg_RX = 0;                         //Inicializa contador de seg RX
             contadorSeg_TX=0;                         //Inicializa contador de seg TX
             RADIO.statusRXTX = RADIO_RST;
             Timer1.initialize(1000000);             // Dispara cada 1000 ms
          }
     }
     else                                   //recibio los datos
     {
        RADIO.flagRx = 0;                              //Apaga flag RX
        Serial.print("Recibio ");
        Serial.print(RADIO.dataLenRX);
        Serial.println(" bytes: ");
        Serial.write(RADIO.frameRX, RADIO.dataLenRX);
        Serial.println();

        Serial.println("_____Calidad de señal_____");
        Serial.println("SNR= ");
        Serial.print(RADIO.snr);
             
        Serial.println("\n RSSI= ");
        Serial.print(RADIO.rssi);
        Serial.println(" ");
        
        envioSolicitud[2] = 'N';                        //NUEVO DATO
        if(RADIO.crc == 1)                             //ERROR
        {
          Serial.print("Datos Errados!!!\n");
          Serial.print("Solicita nuevamente !!!\n"); 
          RADIO.crc = 0;      
          envioSolicitud[2] = 'F';        //DATO ERRADO o NO RECIBIDO 
        }
        else
        {
          contadorBytesRX = contadorBytesRX + RADIO.dataLenRX;
            Serial.print("TOTAL ");
            Serial.print(contadorBytesRX);
            Serial.println(" Datos\n");
        }
        flagEnvio =0;
        RADIO.statusRXTX = RADIO_RST; 
     }
  }
}
