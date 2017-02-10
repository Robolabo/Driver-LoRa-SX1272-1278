/*******************************************************************************
 * Prueba de  comunicación punto - multipunto entre un maestro y tres esclavo utilizando SX1278
 * Aplicación red punto - multipunto - Dispositivo Maestro
 * 29/07/16
 *******************************************************************************/
 
#include <radio.h>                    //Líbrería de alto nivel para configuración por software de los radio LoRa
#include <hal/halLora.h>              //Líbrería de bajo nivel para configuración de hardware radio LoRa (HAL)
#include <SPI.h>                      //Líbrería para manejo de comunicación SPI

#include <TimerOne.h>                 //Líbrería incluida para generar interrupciones por TIMER


//Variable propias de la aplicación
int NumeroMaestro = 0;                //Define el número del maestro, por defecto es "0"
int cantidadEsclavo = 3;              //Define la cantidad de dispositivos esclavos


//Definición de los pines de control de la tarjeta Arduino
const radio_pinmap radio_pins = {
    .nss = 10,
    .rxtx =RADIO_UNUSED_PIN,
    .rst = 9,
    .dio0 = 2,        
   
#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)       //Define la version/fabricante del SX1278
    .dio3 = 5,    
#endif
#if defined(CFG_1278_NICERF1278)                                      //Selecciona el CHIP SX1278 de NICERF
    .tx_en = 3,
    .rx_en = 4,
#endif  
};


//Declaración de variables
int FlagRecibir;
int contadorSeg_TX;
int contadorSeg_RX;

int segundosEsperaTX;
int segundosEsperaRX;

int CONTADOR = 0X0000;
int contadorBytesRX = 0x0000;

String ContadorString;
char BufferNumero[5];

char recibidoEsclavo[8];                  //los últimos 4 bytes funcionan como dato tomado del sensor
//envioSensor 0_NumeroMaestro{0}   1_NumeroEsclavo{1-3}   2_VariableEsclavo{0-255}   3-> Número random de la trama      4-7-> Dato
char trasmiteConfirmacion[4] = {0 , NumeroMaestro, 0, 0};              //los ultimos 4 bytes funcionan como dato tomado del sensor
//trasmiteConfirmacion   0->Destino  1-> Fuente  2-> tipo sensor  3->Número random de la trama



//Función Interrupción de timer cada segundo
void ISR_RETARDO()                                        //retardo por timer cada un segundo
{
  if(RADIO.statusRXTX == RADIO_TX)
  {
    contadorSeg_TX++;
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
    //Configuración de comunicación para realizar debugger 
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
    FlagRecibir = 0;
    contadorSeg_TX = 0;
    contadorSeg_RX = 0;  
    contadorBytesRX = 0;
    segundosEsperaTX = 3;       //Se debe aumentar de acuerdo a la configuración SF y BW
    segundosEsperaRX = 5;       //Se debe aumentar de acuerdo a la configuración SF y BW


    //Inicialización de variables del driver
    RADIO.flagTx = 0;      
    RADIO.flagRx =0;
    RADIO.crc = 0;



    //Intervalo de frecuencia de 433.050 MHz to 434.790 MHz: BW: 1.74 MHz
    //Configuración de frecuencia
    RADIO.freq = 433800000;             //Frecuencias ISM para SX1278
    //RADIO.freq = 869525000;           //Frecuencias ISM para SX172

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
    
    Serial.print("Version ");
    Serial.print(RADIO.version);
    Serial.print("\n");

    if(RADIO.version != versionLora)            //Debugger comprobación de funcionamiento
    {
      Serial.print("LoRa no conectado \n");
    }
 
}


void loop() 
{
  int i;
  
  if((FlagRecibir == 0) && (RADIO.statusRXTX == RADIO_RST))        // RADIO.flags != 0
  {
      FlagRecibir = 1;
      RADIO.flagRx = 0;      
      Serial.println("Espera dato RX \n");
      Timer1.initialize(1000000);               // Dispara cada 1000 ms
      radio_mode(RADIO_RX);                     //Pasa a MODO RX
      RADIO.statusRXTX = RADIO_RX;              // Utilizado como flag coloca la variable de modo en RX
      Timer1.attachInterrupt(ISR_RETARDO);      //->Retardo
  }


  if(RADIO.statusRXTX == RADIO_RX)              //-> Configurado para recibir
  {
     if(RADIO.flagRx == 0)                      //Nota ha recibido los datos
     {
          if (contadorSeg_RX >= segundosEsperaRX)
          {
             Serial.println("No ha recibido datos... \n");
             contadorSeg_RX = 0;                     //Inicializa contador de seg RX
             Timer1.initialize(1000000);             // Dispara cada 1000 ms
          }
     }
     else                                             //recibio los datos
     {
        RADIO.flagRx = 0;                             //Apaga flag RX
        
        RADIO.statusRXTX = RADIO_RST;                 //Forza al estado de TX siempre y cuando se cumplan las condiciones
        
        if(RADIO.crc == 1)                            //ERROR
        {
          Serial.print("Datos Errados!!! sin acciones\n");
          RADIO.crc = 0;      
        }
        else
        {
          if(RADIO.dataLenRX == 8)                   //Nota, son 8 bytes de confirmacion
          {
            
            if( (RADIO.frameRX[0] == NumeroMaestro) && (RADIO.frameRX[1]>0) && (RADIO.frameRX[1]<=cantidadEsclavo) )    
            {//identifica que la informacion sea de un esclavo
              Serial.print("Info Esclavo:");
              Serial.print(RADIO.frameRX[1]);
              
              Serial.print(" Trama OK!<<");
              Serial.write(RADIO.frameRX, RADIO.dataLenRX);
              Serial.println();
              
              trasmiteConfirmacion[0] = RADIO.frameRX[1];        //Almacena número de Esclavo
              trasmiteConfirmacion[2] =  RADIO.frameRX[2];       //Almacena tipo de variable del sensor
              trasmiteConfirmacion[3] =  RADIO.frameRX[3];       //Almacena Numero Random de la Comunicación
              

              RADIO.statusRXTX = RADIO_TX;                         //Flag para determinar donde esta el LORA-> esto se puede hacer desde dentro las líbrerias      
              buffer_to_frameTX(trasmiteConfirmacion, 4);          //trasnmite 4 bytes
              
              Serial.print("Envia>>");
              
              RADIO.flagTx = 0;
              radio_mode(RADIO_TX);                                 //Configura la TX
              
              Serial.println(">>FIFO ");
              Serial.write(RADIO.frameTX, RADIO.dataLenTX);

            }
            else
            {
              Serial.print("Info sin formato!");
            }
          }
          else
          {
            Serial.print("Exceso en Bytes!");
          }  
        }   
       
        contadorSeg_TX=0;                         //Inicializa contador de seg TX
        contadorSeg_RX=0;                         //Inicializa contador de seg TX
        Timer1.initialize(1000000);               //Dispara cada 1000 ms
     }
     
  }

  if(RADIO.flagTx == 1)                           //Ha recibido la confirmacion del modem por TX correcta
  {
      RADIO.flagTx = 0;      
      Serial.println("Confirma TX! \n");
                                                 //Inicializa contador de seg TX

      contadorBytesRX = contadorBytesRX + 8;
      Serial.print("TOTAL recibidos");
      Serial.print(contadorBytesRX);
      Serial.println(" Datos\n");
         
      Timer1.initialize(1000000);             //Dispara cada 1000 ms
      radio_mode(RADIO_RX);                   //Pasa a MODO RX
      RADIO.statusRXTX = RADIO_RX;            // Utilizado como flag coloca la variable de modo en RX
      contadorSeg_RX=0;                       //Inicializa contador de seg RX
      contadorSeg_TX=0;  

  }
  if ((contadorSeg_TX >= segundosEsperaTX) && (RADIO.statusRXTX == RADIO_TX))
  {
     Serial.println("Superado el tiempo de TX. Vuelve a TX \n");
     FlagRecibir = 0;
     Timer1.initialize(1000000);             // Dispara cada 1000 ms
     RADIO.statusRXTX = RADIO_RST;           //<- RST
     contadorSeg_TX=0;
     contadorSeg_RX=0;
  } 
}

