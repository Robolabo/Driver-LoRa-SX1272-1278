/*******************************************************************************
 * Prueba de  comunicación punto - multipunto entre un maestro y tres esclavo utilizando SX1278
 * Aplicación red punto - multipunto - Dispositivo Esclavo "NumeroEsclavo"
 * 29/07/16
 *******************************************************************************/
 
#include <radio.h>                    //Libreria de alto nivel para configuración por software de los radio LoRa
#include <hal/halLora.h>              //Libreria de bajo nivel para configuración de hardware radio LoRa (HAL)
#include <SPI.h>                      //Libreria para manejo de comunicación SPI
           
#include <TimerOne.h>                 //Librería incluida para generar interrupciones por TIMER
#include <MsTimer2.h>                 //Librería incluida para generar interrupciones por TIMER


//Variable propias de la aplicación
int NumeroMaestro = 0;              //Define el número del maestro
int NumeroEsclavo = 2;              //Define el número del esclavo     <--- Este número se cambia para los diferentes esclavos
int VariableEsclavo = 0;            //Define la variable enviada por el esclavo
int tiempoTomaMuestraSensor = 30;   //Segundos de toma de muestra    (Tiempo de muestreo para la trasmisión de datos)



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
int NumTiemDesfaseTX;
int flagEnvio;
int contadorSeg_TX;
int contadorSeg_RX;
int contador200mS;
int contadorSegSensor;

int segundosEsperaTX;
int segundosEsperaRX;

int CONTADOR = 0xFFFF;
char SENSOR[4];           //simula el sensor

int contadorBytesRX = 0x0000;

String ContadorString;
                                //Tramas utilizadas para enviar o recibir
int envioSensor[8] = {NumeroMaestro, NumeroEsclavo, VariableEsclavo, 0, 0, 0, 0, 0};              //los últimos 4 bytes funcionan como dato tomado del sensor
//envioSensor 0_NumeroMaestro{0}   1_NumeroEsclavo{1-3}   2_VariableEsclavo{0-255}   3-> Número random de la trama      4-7-> Dato

int recibidoConfirmacion[4];
//recibidoConfirmacion   0->Destino  1-> Fuente  2-> tipo sensor  3->Número random de la trama

//Almacenamiento TRAMAS
int tramaPendienteTX[200];
int cantidadPendienteTX;
int NumeroRandomTrama;




//Función Interrupción de timer cada segundo                                   
void ISR_RETARDO_1S()                       //retardo por timer cada un segundo
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
  contadorSegSensor++;
}


//Función Interrupción de timer cada 200 milisegundos    
void ISR_RETARDO_200mS()                //retardo por timer cada 200 milisegundos
{
  contador200mS = contador200mS + 1;
}





//Almacena los datos de buffer definido por el usuario al buffer RADIO.frameTX[...] utilizado por el driver
void buffer_to_frameTX( int *str, int datalen) 
{
  int i;
  RADIO.dataLenTX = datalen;
  for (i=0;i<datalen;i++)
  {
    RADIO.frameTX[i] = *str++;
  }
}




//Cargar los datos del SENSOR a tramaPendienteTX
void cargarSensor_TramaPendienteTX()
{
  int i,j;
  char SENSOR_Bytes[4];
  
  //Emula tomar dato de un sensor sensor
  ContadorString = String(CONTADOR);
  ContadorString.toCharArray(SENSOR_Bytes,5);       //Datos del sensor a tipo CHAR
  CONTADOR++;
  //Finaliza Emular tomar dato de un sensor sensor

  cantidadPendienteTX++;
  
  if((cantidadPendienteTX > 0) && (cantidadPendienteTX < 31))
  {
      j=(cantidadPendienteTX -1)*5;                 //Solamente almacena el Byte Random y los 4 bytes de informacion           
      tramaPendienteTX[j] = random(255);           //establece el numero RANDOM de la Trama, como comprobacion

      Serial.print("CodigoRandom: ");
      Serial.print(tramaPendienteTX[j]);

      //Finaliza emulacion del SENSOR
      for (i=0; i< 4; i++)                        //Carga los datos de SENSOR a la TRAMA
      {
        j++;                                      //lo hace antes para 
        tramaPendienteTX[j] = SENSOR_Bytes[i] - 48;     //Convierte el dato de CHAR-> Decimal
      }
      
  }
  else
  {
    Serial.println("Desbordamiento en Tramas por TX. Superado 30 min.");
    cantidadPendienteTX = 1;
  }
}



//Tarea de rotar vector de almacenamiento de tramas pendientes
void rotarTramaPendienteTX()
{
  int i;
  for (i=0; i<(cantidadPendienteTX*5);i++)
  {
      tramaPendienteTX[i]=tramaPendienteTX[i+5];
  }
}



//Configuración de los dispositivos LoRa
void setup()        //Función de configuración inicial
{
    //Configuración de comunicacion para realizar debugger 
    Serial.begin(115200);  
    Serial.print("Inicia Esclavo: ");
    Serial.print(NumeroEsclavo);
    delay(1000);          //Retardo
  
    #ifdef VCC_ENABLE
      pinMode(VCC_ENABLE, OUTPUT);
      digitalWrite(VCC_ENABLE, HIGH);
      delay(1000);
    #endif
     pinMode(LED_BUILTIN, OUTPUT);    //Configuración de pines


    //Inicialización de variables de aplicación general
    flagEnvio = 0;
    NumTiemDesfaseTX=0;
    contadorSeg_TX = 0;
    contadorSeg_RX = 0;
    contador200mS = 0;
    contadorSegSensor = 0;
    contadorBytesRX = 0;
    memset(tramaPendienteTX, 0, 200);
    cantidadPendienteTX = 0;
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

    
    //Se emula el SENSOR Inicializa
    CONTADOR = 1000 * NumeroEsclavo;

    
    MsTimer2::set(200, ISR_RETARDO_200mS);      // Inicializa timer2 para contar cada 200ms
    MsTimer2::start();

    randomSeed(analogRead(0));
    NumTiemDesfaseTX = random(50);              //Establece que el tiempo máximo de desfase será de 10 segundos

 
    cantidadPendienteTX =0;
    cargarSensor_TramaPendienteTX();            //incrementa la cantidad pendientes de tramas, además de Simular el valor del SENSOR
}




void loop() 
{
  int i;

  if((cantidadPendienteTX>0) && (contador200mS > NumTiemDesfaseTX) && (flagEnvio == 0) && (RADIO.statusRXTX == RADIO_RST))        // RADIO.flags != 0
  {      
    
    radio_mode(RADIO_RST);                                //Detiene la RX antes
    flagEnvio = 1;
    contador200mS = 0;                                    //Coloca el contador de 200mS -> 0
    RADIO.flagTx = 0;
    RADIO.statusRXTX = RADIO_TX;                          //Flag para determinar donde esta el LORA-> esto se puede hacer desde dentro las líbrerias

    
    NumeroRandomTrama = tramaPendienteTX[0];
    envioSensor[3] = NumeroRandomTrama;                   //Este número se utilizara para comprobar que se TX


    Serial.print("\n");
    for (i=4; i< 8; i++)                                  //Carga los datos de TRAMAPENDIENTE a BUFFER DE TX
    {                                     
      envioSensor[i] = tramaPendienteTX[i-3];
    }

    buffer_to_frameTX(envioSensor, 8);                    //Carga 8 bytes a la FIFO de LORa-> Activa TX


    Serial.print("Envia ");
    Serial.print(RADIO.dataLenTX,DEC);
    
    radio_mode(RADIO_TX);                                   //Configura la TX
    Serial.println(" Datos-> FIFO: ");
    

    contadorSeg_TX=0;                                       //Inicializa contador de seg TX
    contadorSeg_RX=0;                                       //Inicializa contador de seg RX
    Timer1.initialize(1000000);                             // Dispara cada 1000 ms
    Timer1.attachInterrupt(ISR_RETARDO_1S);                 //->Retardo
  
     contadorSeg_TX=0;                                      //Inicializa contador de seg TX
  }

  if((flagEnvio == 1)  && (RADIO.flagTx == 1))              //Ha recibido la confirmacion del modem por TX correcta
  {
      flagEnvio = 0;
      RADIO.flagTx = 0;      
      RADIO.flagRx = 0; 
      Serial.println("TX>> OK! \n");
      Timer1.initialize(1000000);                     // Dispara cada 1000 ms
      radio_mode(RADIO_RX);                             //Pasa a MODO RX
      RADIO.statusRXTX = RADIO_RX;                    //Utilizado como flag coloca la variable de modo en RX
      contadorSeg_RX=0;                               //Inicializa contador de seg RX
      contadorSeg_TX=0;                               //Inicializa contador de seg TX
  }
  if ((contadorSeg_TX >= segundosEsperaTX) && (RADIO.statusRXTX == RADIO_TX))
  {
     Serial.println("Superado el tiempo de TX. Vuelve a TX \n");
     flagEnvio = 0;
     Timer1.initialize(1000000);             // Dispara cada 1000 ms
     RADIO.statusRXTX = RADIO_RST;           //<- RST
     contadorSeg_TX=0;
     contadorSeg_RX=0;
     contador200mS = NumTiemDesfaseTX;     //Forza que no exista Desfase 
  }

  if(RADIO.statusRXTX == RADIO_RX)             //-> Configurado para recibir
  {
     if(RADIO.flagRx == 0)                   //Nota ha recibido datos de confirmacion
     {
          if (contadorSeg_RX >= segundosEsperaRX)
          {
            Serial.println("Superado t_RX. T_desfase...sleep \n");
            
            flagEnvio = 0;
            contadorSeg_RX = 0;                       //Inicializa contador de seg RX
            contadorSeg_TX=0;                         //Inicializa contador de seg TX
            RADIO.statusRXTX = RADIO_RST;
            Timer1.initialize(1000000);               //Dispara cada 1000 ms
            
            MsTimer2::set(200, ISR_RETARDO_200mS);    //Cuenta cada 200ms
            MsTimer2::start();
            
            NumTiemDesfaseTX = random(50);            //Establece que el tiempo maximo de desfase sera 10 segundos
            contador200mS = 0; 
          }
     }
     else                                             //recibio los datos
     {
        Serial.print( "RX<<? \n");
        
        //Prepara el Timer2 para hacer un desfase y enviar nuevamente la Información
        MsTimer2::set(200, ISR_RETARDO_200mS);        //Cuenta cada 200ms
        MsTimer2::start();
        
        NumTiemDesfaseTX = random(50);         //Establece que el tiempo máximo de desfase sera 10 segundos

        contador200mS = 0; 
        //FIN Prepara el Timer2 para hacer un desfase y enviar nuevamente la Informacion

        flagEnvio = 0;                                  //Forza al estado de TX siempre y cuando se cumplan las condiciones
        RADIO.statusRXTX = RADIO_RST;                   //Forza al estado de TX siempre y cuando se cumplan las condiciones

        
        if(RADIO.crc == 1)                             //ERROR
        {
          Serial.print("Datos Errados!!! sin acciones\n");
          RADIO.crc = 0;      
        }
        else
        {
          if(RADIO.dataLenRX == 4)                      //Nota, son 4 bytes de confirmacion
          {
            for (i=0; i< 4; i++)                        //Extrae los datos del  los datos de SENSOR a la TRAMA
            {
              recibidoConfirmacion[i] = RADIO.frameRX[i];
            }
            
            if( (recibidoConfirmacion[0] == NumeroEsclavo) && (recibidoConfirmacion[1] == NumeroMaestro) )
            {
              Serial.print("Info para Esclavo!");


              if(recibidoConfirmacion[3] == NumeroRandomTrama)         //Se confirma que la última trama, la recibio el maestro
              {
                  Serial.print(" Trama OK!: ");
                                
                  radio_mode(RADIO_RST);                //Coloca el dispositivo a Dormir  
                  
                  if(cantidadPendienteTX > 1)          //Tiene pendiente la TX de antiguas tramas? 
                  {//Es necesario desplazar/rotar tramaPendienteTX[]  5 posiciones
                    cantidadPendienteTX--;
                    rotarTramaPendienteTX();
                    Serial.print("LoRa-> Debe TX");
                  }
                  else
                  {
                      cantidadPendienteTX = 0;          //No tiene tramas pendientes por enviar
                      Serial.print("LoRa-> SLEEP!");
                  } 
              }
            }
          }
          else
          {
            Serial.print("Info NO Esclavo!");
          }

        }

     }
     
  }

  if(contadorSegSensor >= tiempoTomaMuestraSensor)             //Se cumplio el tiempo de muestreo...se debe tomar la muestra del sensor
  {
    Serial.print("\n Tiempo  cumplido. Carga datos de Sensor! \n");
    contadorSegSensor = 0;
    cargarSensor_TramaPendienteTX();                          //incrementa la cantidad pendientes de tramas, ademas de Simula el valor del SENSOR    
  }
  
}
