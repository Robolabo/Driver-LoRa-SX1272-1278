/** @file application.c

	@author Alvaro Gutierrez
	@author Robolabo
	@date 2013/07/11 - Edici√≥n base.

	Implementation of functions defined in application.h file

    COPYRIGHT NOTICE:
    This software is property of Robolabo. Its reproduction,  total or  par-
    tial, by any means, in any form, permanent or temporary, is forbidden
    unless explicitly authorised by Robolabo.
    Any adaptation, amendment, translation or transformation, as well as
    the decompiling or disassembly of this software  product, shall only
    be performed with the explicit authorization of Robolabo.
    The user of the present software product shall be allowed  to make a
    backup copy  as long as it is necessary  for the utilization  of the
    same.
    The terms stated above shall be understood  affecting that stated in
    applicable Laws. */
		
// ************************************************************
// Includes
// ************************************************************
#include "application.h"






// ************************************************************
// Application_Config
// Initializate median, EEPROM and board taking into 
// account the board ID
// ************************************************************
void Application_Config_Task()
{
	/* Get hardware ID */
	applicationBoardID = HW_ReadID();
	/* Initialize median variables */
	Median_Init(applicationBoardID);
	/* Initialize EEPROM */
	EEPROM_Init();
	/* Initialize module */
	switch (applicationBoardID)
	{
		case ANALOG:
			Analog_Init();
			break;
			/*case THERMOCOUPLE:*/
			/*Therm_Init();*/
			break;
		case RTD:
			RTD_Init();
			break;
		case SAI:
			SAI_Init();
			break;
		default:
			break;
	}

	/* Delete this task */
	vTaskDelete(ApplicationConfigTask_Handle);
}

// ************************************************************
// MODBUS_Task
// Configure and starts the MODBUS transmission/reception            
// ************************************************************
void MODBUS_Task()                                                                                       
{
	uint8_t slave_address;

	/*Read switches*/
	slave_address = HW_ReadSwitches();                                                                     
	/*Init MODBUS*/
	SERIE_SetModbus();
	eMBInit(MB_RTU, slave_address, 1, 9600, MB_PAR_NONE);        
	eMBEnable();
	while(1)
	{
		eMBPoll();                                                             
	}
}

// ************************************************************
// ToggleLed_Task
// Change Led status every 100 miliseconds
// ************************************************************
void ToggleLed_Task()
{

	while(1)
	{
		switch (applicationBoardID)
		{
			case THERMOCOUPLE:
				HW_PinWrite(LED_H0, (BitAction)(1 - HW_PinRead(LED_H0)));
				//cout << "Test" << endl;

				HW_PinWrite(LED_H1, (BitAction)(1 - HW_PinRead(LED_H3)));

				break;
			case RTD:
				HW_PinWrite(LED_H2, (BitAction)(1 - HW_PinRead(LED_H2)));
				break;
			case ANALOG:
				HW_PinWrite(LED_H3, (BitAction)(1 - HW_PinRead(LED_H3)));
				break;
			case SAI:
				HW_PinWrite(LED_H0, (BitAction)(1 - HW_PinRead(LED_H0)));
				break;
			default:
				break;
		}
		vTaskDelay(500);
	}
}

// ************************************************************
// TestLoraTimer_Task
// Corresponde a un timer para cada un segundo
// ************************************************************
void TestLoraTimer_Task()
{
	contadorSeg_TX = 0;
	contadorSeg_RX = 0;

	while(1)
	{
		if(RADIO.statusRXTX == RADIO_TX)
		{
			contadorSeg_TX++;
		}
		else if(RADIO.statusRXTX == RADIO_RX)
		{
			contadorSeg_RX++;
		}

		HW_PinWrite(LED_H1, (BitAction)(1 - HW_PinRead(LED_H1)));

		contadorSegSensor++;					//Variable ˙til Multiples Lora

		vTaskDelay(1000);
	}

}
/*// *************************************************************/



// ************************************************************
// TestLoraTimer2_Task
// Desfase de tiempo para TX con aplicaciÛn punto-multipunto
// ************************************************************
void TestLoraTimer2_Task()
{
	contador200mS = 0;

	while(1)
	{
		contador200mS = contador200mS + 1;
		vTaskDelay(200);
	}
}
/*// *************************************************************/



// ************************************************************
// TestLora_Task
// Prueba para puesta en funcionamiento de LORA PRUEBA FUNCIONAMIENTO UNO A UNO......
//MASTER (punto a punto)
// ************************************************************
void TestLora_Task()
{

    //InicializaciÛn de variables de aplicaciÛn general
    char envioSolicitud[3];
    u1_t flagEnvio             = 0;
    u1_t segundosEsperaTX     = 2;
    u1_t segundosEsperaRX     = 5;
    u2_t contadorBytesRX     = 0;

    uint8_t     pruebaRx      = 0;
    uint8_t     pruebaTx      = 0;

    contadorSeg_TX = 0;
    contadorSeg_RX = 0;


    //InicializaciÛn de variables del driver
    RADIO.flagTx = 0;      //Inicializacion de variables
    RADIO.flagRx = 0;
    RADIO.crc      = 0;

    //Intervalo de frecuencia de 433.050 MHz to 434.790 MHz: BW: 1.74 MHz
	//ConfiguraciÛn de frecuencia
	RADIO.freq = 433800000;             //Frecuencias ISM para SX1278
	//RADIO.freq = 869525000;             //Frecuencias ISM para SX172

	//ConfiguraciÛn de potencia
	RADIO.txpow = 17;                   //Maxima TX power
	RADIO.imax = 100;                   //Por defecto 100mA   45mA <=Imax <= 240mA


	/*
	Aumento m·ximo de ganancia. LNA m·x. corriente 150%  LNA_RX_GAIN
	M·xima capacidad del Payload 64
	LORARegSymbTimeoutLsb tiempo Timeout de sÌmbolos es 0
	Palabra reservada para Lora networks LORARegSyncWord 0x34
	ConfiguraciÛn de Pines DIO0=RXDONE DIO3=PAYLOADCRCERROR
	Configura LoRa modem (RegModemConfig1, RegModemConfig2) valores de RADIO.sf RADIO.bw RADIO.cr
	Configura el Canal de acuerdo con la frecuencia de RADIO.freq en 3 registros
	Configura la potencia a Max potencia RADIO.txpow. Potencia limitada hasta 20dBm
	RADIO.sf = SF_6, SF_7, SF_8, SF_9, SF_10, SF_11, SF_12
	RADIO.cr = CR_4_5, CR_4_6, CR_4_7, CR_4_8
	RADIO.bw = BW7_8, BW10_4, BW15_6, BW20_8, BW31_25, BW41_7, BW62_5, BW125, BW250, BW500, BWrfu
	*/

    //Variables de configuraciÛn de la comunicaciÛn LoRa
    RADIO.sf = SF_9;                //ConfiguraciÛn del Spread factor
    RADIO.bw = BW500;               //ConfiguraciÛn del ancho de canal
    RADIO.cr = CR_4_5;              //Configuracion de Coding rate


    RADIO.statusRXTX = RADIO_RST;  //Se incorpora para modificar en el main*loop

    RADIO.statusRXTX = RADIO_RST;  //Se incorpora para modificar en el main*loop


     hal_init();                     //ConfiguraciÛn de puertos de SDIN
     radio_init();                   //ConfiguraciÛn de inicio para los LoRa


    envioSolicitud[0] = 'T';
    envioSolicitud[1] = 'x';
    envioSolicitud[2] = 'Y';

    while(1)
    {

        if((flagEnvio == 0) && (RADIO.statusRXTX == RADIO_RST))        // RADIO.flags != 0
        {
            flagEnvio = 1;
            RADIO.statusRXTX = RADIO_TX;                         //Flag para determinar donde esta el LORA-> esto se puede hacer desde dentro las librerias

            radio_buffer_to_frameTX (envioSolicitud,3);            //Carga datos a RADIO.frameTx y a RADIO.dataLen
            radio_mode(RADIO_TX);                                 //Configura la TX

            contadorSeg_TX=0;                         //Inicializa contador de seg TX
            contadorSeg_RX=0;                         //Inicializa contador de seg RX
            HW_PinWrite(LED_H2, (BitAction)(1 - HW_PinRead(LED_H2)));

            /*Timer1.initialize(1000000);               // Dispara cada 1000 ms*/
            /*Timer1.attachInterrupt(ISR_RETARDO);      //->Retardo*/

        }

        if((flagEnvio == 1)  && (RADIO.flagTx == 1))         //Ha recibido la confirmacion del modem por TX correcta
        {
            flagEnvio = 0;
            RADIO.flagTx = 0;
            /*Serial.println("Confirma TX \n");*/
            /*Timer1.initialize(1000000);             // Dispara cada 1000 ms*/
            RADIO.statusRXTX = RADIO_RX;             // Utilizado como flag coloca la variable de modo en RX
            radio_mode(RADIO_RX);                     //Pasa a MODO RX
            contadorSeg_RX=0;                         //Inicializa contador de seg RX
            contadorSeg_TX=0;                         //Inicializa contador de seg TX
            envioSolicitud[2] = 'N';                  //Nuevo dato
            //continua esperando
        }

        if ((contadorSeg_TX >= segundosEsperaTX) && (RADIO.statusRXTX == RADIO_TX))
        {
            /*Serial.println("Superado el tiempo de TX. Vuelve a TX \n");*/
            flagEnvio = 0;
            /*Timer1.initialize(1000000);             // Dispara cada 1000 ms*/
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
                    /*Serial.println("Superado el tiempo de RX. Vuelve para Solicitar ... \n");*/
                    envioSolicitud[2] = 'F';        //DATO ERRADO o NO RECIBIDO
                    flagEnvio = 0;
                    contadorSeg_RX = 0;                         //Inicializa contador de seg RX
                    contadorSeg_TX = 0;                         //Inicializa contador de seg TX
                    RADIO.statusRXTX = RADIO_RST;
                    /*Timer1.initialize(1000000);             // Dispara cada 1000 ms*/
                }
            }
            else                                   // RADIO.flagRx == 1  -> recibio los datos
            {
                RADIO.flagRx = 0;                              //Apaga flag RX
                /*Serial.print("Recibio ");*/
                /*Serial.print(RADIO.dataLen);*/
                /*Serial.println(" bytes: ");*/
                /*Serial.write(RADIO.frame, RADIO.dataLen);*/
                /*Serial.println();*/

                /*Serial.println("_____Calidad de se√±al_____");*/
                /*Serial.println("SNR= ");*/
                //Serial.print(RADIO.snr);
                // Serial.println(" ");
                /*Serial.print(RADIO.snr*4);*/
                //Serial.println(" ");

                /*Serial.println("\n RSSI= ");*/
                /*Serial.print(RADIO.rssi);*/
                /*Serial.println(" ");*/
                //    RADIO.snr  = readReg(LORARegPktSnrValue); // SNR [dB] * 4
                //    RADIO.rssi = readReg(LORARegPktRssiValue) - 125 + 64; // RSSI [dBm] (-196...+63)



                envioSolicitud[2] = 'N';        //NUEVO DATO
                if(RADIO.crc == 1)                             //ERROR
                {
                    /*Serial.print("Datos Errados!!!\n");*/
                    /*Serial.print("Solicita nuevamente !!!\n"); */
                    //HW_PinWrite(LED_H1, (BitAction)(1 - HW_PinRead(LED_H1)));
                    RADIO.crc = 0;
                    envioSolicitud[2] = 'F';        //DATO ERRADO o NO RECIBIDO
                }
                else
                {
                    contadorBytesRX = contadorBytesRX + RADIO.dataLenRX;
                    /*Serial.print("TOTAL ");*/
                    /*Serial.print(contadorBytesRX);*/
                    /*Serial.println(" Datos\n");*/
                }

                flagEnvio =0;
                RADIO.statusRXTX = RADIO_RST;
            }
        }

        vTaskDelay(1);
    }
}






// ************************************************************
// TestLoraMultiple_Task
// Prueba para puesta en funcionamiento de LORA   PRUEBA FUNCIONAMIENTO PUNTO A MULTIPUNTO......
//ESCLAVO 3
// ************************************************************
void TestLoraMultiple_Task()
{
	//contador200mS		-> INTRODUCIR a la variable  GLOBAL
	//contadorSegSensor	-> INTRODUCIR a la variable  GLOBAL

	u1_t segundosEsperaTX 	= 2;
	u1_t segundosEsperaRX 	= 5;

	u1_t NumeroMaestro = 0;            //Define el numero del maestro
	u1_t NumeroEsclavo = 3;            //Define el numero del esclavo
	u1_t VariableEsclavo = 0;            //Define la variable enviada por el esclavo
	u1_t tiempoTomaMuestraSensor = 30;   //Segundos de toma de muestra

	u1_t NumTiemDesfaseTX;


	u1_t flagEnvio             = 0;
	u2_t CONTADOR = 0x0000;
	u1_t SENSOR[4];
	u2_t contadorBytesRX = 0x0000;

	//Tramas utilizadas para enviar o recibir
	u1_t envioSensor[8] = {NumeroMaestro, NumeroEsclavo, VariableEsclavo, 0, 0, 0, 0, 0};              //los ultimos 4 bytes funcionan como dato tomado del sensor
	//envioSensor 0_NumeroMaestro{0}   1_NumeroEsclavo{1-3}   2_VariableEsclavo{0-255}   3-> Numero random de la trama      4-7-> Dato

	u1_t recibidoConfirmacion[4];
	//recibidoConfirmacion   0->Destino  1-> Fuente  2-> tipo sensor  3->Numero random de la trama

	//Almacenamiento TRAMAS
	u1_t tramaPendienteTX[200];
	u1_t cantidadPendienteTX;
	u1_t NumeroRandomTrama;

	u1_t i,j;						//uso general
	u2_t mult;						//Uso general

	//Inicializacion variable globales
	contadorSeg_TX = 0;
	contadorSeg_RX = 0;
	contador200mS = 0;
	contadorSegSensor = 0;




	//Configuracion de parametros
	RADIO.flagTx = 0;      //Inicializacion de variables
	RADIO.flagRx = 0;
	RADIO.crc 	 = 0;


	memset(tramaPendienteTX, 0, 200);
	cantidadPendienteTX = 0;

	//SX1278
	RADIO.freq = 433800000;   // Use a frequency in the g3 which allows 10% duty cycling.
	RADIO.txpow = 17;      // Maximum TX power
	RADIO.imax = 100;          //Por defecto 100mA   45mA <=Imax <= 240mA



	//{ CR_4_5, CR_4_6, CR_4_7, CR_4_8 };					//Usado para CR
	//{ SF_6, SF_7, SF_8, SF_9, SF_10, SF_11, SF_12 };		//Usado para SF
	//{ BW7_8, BW10_4, BW15_6, BW20_8, BW31_25, BW41_7, BW62_5, BW125, BW250, BW500, BWrfu };

	RADIO.sf = SF_9;              //Configuracion del Modem SF a baja velocidad
	RADIO.bw = BW500;              //Configuracion del ancho de banda reducido
	RADIO.cr = CR_4_5;              //Configuracion de Coding rate

	RADIO.statusRXTX = RADIO_RST;  //Se incorpora para modificar en el main*loop

	hal_init();						//inicia el hardware
	radio_init();					//inicia el radio


	CONTADOR = 1000*NumeroEsclavo;


	srand(&i);
	NumTiemDesfaseTX = (rand() % 50);					//Establece un randon entre 0-50

	//cargarSensor_TramaPendienteTX();        //incrementa la cantidad pendientes de tramas, ademas de Simula el valor del SENSOR
	contadorSegSensor = tiempoTomaMuestraSensor;			//FORZA que se entre a la parte final cargarSensor_TramaPendienteTX
	cantidadPendienteTX =0;
	contador200mS = 0;

	while(1)
	{

		if((cantidadPendienteTX>0) && (contador200mS > NumTiemDesfaseTX) && (flagEnvio == 0) && (RADIO.statusRXTX == RADIO_RST))        // RADIO.flags != 0
		{

			radio_mode(RADIO_RST);                                  //Detiene la RX antes
			flagEnvio = 1;
			contador200mS = 0;                                    //Coloca el contador de 200mS -> 0
			RADIO.flagTx = 0;
			RADIO.statusRXTX = RADIO_TX;                         //Flag para determinar donde esta el LORA-> esto se puede hacer desde dentro las librerias

			NumeroRandomTrama = tramaPendienteTX[0];
			envioSensor[3] = NumeroRandomTrama;               //Este numero se utilizara para comprobar que se TX

			radio_buffer_to_frameTX(envioSensor, 8);                  //Carga 8 bytes a la FIFO de LORa-> Activa TX

			radio_mode(RADIO_TX);     //Configura la TX


			contadorSeg_TX=0;                         //Inicializa contador de seg TX
			contadorSeg_RX=0;                         //Inicializa contador de seg RX
			/**/
			/*Timer1.initialize(1000000);               // Dispara cada 1000 ms*/
			/*Timer1.attachInterrupt(ISR_RETARDO_1S);      //->Retardo*/

			HW_PinWrite(LED_H2, (BitAction)(1 - HW_PinRead(LED_H2)));
		}


		if((flagEnvio == 1)  && (RADIO.flagTx == 1))         //Ha recibido la confirmacion del modem por TX correcta
		{
			flagEnvio = 0;
			RADIO.flagTx = 0;
			RADIO.flagRx = 0;

			//Timer1.initialize(1000000);             // Dispara cada 1000 ms
			radio_mode(RADIO_RX);                     //Pasa a MODO RX
			RADIO.statusRXTX = RADIO_RX;             // Utilizado como flag coloca la variable de modo en RX
			contadorSeg_RX=0;                         //Inicializa contador de seg RX
			contadorSeg_TX=0;                         //Inicializa contador de seg TX
		}

		if ((contadorSeg_TX >= segundosEsperaTX) && (RADIO.statusRXTX == RADIO_TX))
		{
			/*Serial.println("Superado el tiempo de TX. Vuelve a TX \n");*/
			flagEnvio = 0;
			/*Timer1.initialize(1000000);             // Dispara cada 1000 ms*/
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
					flagEnvio = 0;
					contadorSeg_RX = 0;                       //Inicializa contador de seg RX
					contadorSeg_TX=0;                         //Inicializa contador de seg TX
					RADIO.statusRXTX = RADIO_RST;

					NumTiemDesfaseTX = rand() % 50;         //Establece que el tiempo maximo de desfase sera 10 segundos
					contador200mS = 0;
				}
			}
			else                                   //recibio los datos
			{
				//Serial.print( "RX<<? \n");

				NumTiemDesfaseTX = rand() % 50;         //Establece que el tiempo maximo de desfase sera 10 segundos
				contador200mS = 0;

				flagEnvio = 0;                                  //Forza al estado de TX siempre y cuando se cumplan las condiciones
				RADIO.statusRXTX = RADIO_RST;                   //Forza al estado de TX siempre y cuando se cumplan las condiciones

				if(RADIO.crc == 1)                             //ERROR
				{
					//Serial.print("Datos Errados!!! sin acciones\n");
					RADIO.crc = 0;
				}
				else
				{
					if(RADIO.dataLenRX == 4)       //Nota, son 4 bytes de confirmacion
					{
						for (i=0; i< 4; i++)                        //Extrae los datos del  los datos de SENSOR a la TRAMA
						{
							recibidoConfirmacion[i] = RADIO.frameRX[i];
						}

						if( (recibidoConfirmacion[0] == NumeroEsclavo) && (recibidoConfirmacion[1] == NumeroMaestro) )
						{
							//Serial.print("Info para Esclavo!");

							if(recibidoConfirmacion[3] == NumeroRandomTrama)         //Se confirma que la ultima trama, la recibio el maestro
							{
								// Serial.print(" Trama OK!: ");

								radio_mode(RADIO_RST);          //Coloca el dispositivo a Dormir

								if(cantidadPendienteTX > 1)          //Tiene pendiente la TX de antiguas tramas?
								{//Es necesario desplazar/rotar tramaPendienteTX[]  5 posiciones
									cantidadPendienteTX--;
									//rotarTramaPendienteTX(); ->//Tarea de rotar vector de almacenamiento de tramas pendientes
									for (i=0; i<(cantidadPendienteTX*5);i++)
									{
										tramaPendienteTX[i]=tramaPendienteTX[i+5];
									}
									//Serial.print("LoRa-> Debe TX");
								}
								else
								{
									cantidadPendienteTX = 0;      //No tiene tramas pendientes por enviar
									//Serial.print("LoRa-> SLEEP!");
								}

							}


						}
					}
					/**/
					else
					{
						//Serial.print("Info NO Esclavo!");
					}

				}

			}

		}

		if(contadorSegSensor >= tiempoTomaMuestraSensor)             //Se cumplio el tiempo de tomar la muestra...se debe tomar la muestra del sensor
		{
			//Serial.print("\n Tiempo  Cumplido. Carga datos de Sensor! \n");
			contadorSegSensor = 0;
			//cargarSensor_TramaPendienteTX();        //incrementa la cantidad pendientes de tramas, ademas de Simula el valor del SENSOR


			//Finaliza Emular tomar dato de un sensor sensor

			cantidadPendienteTX++;

			if((cantidadPendienteTX > 0) && (cantidadPendienteTX < 31))
			{
				j=(cantidadPendienteTX -1)*5;                 //Solamente almacena el Byte Random y los 4 bytes de informacion
				tramaPendienteTX[j] = rand() % 255;           //establece el numero RANDOM de la Trama, como comprobacion

				//Emula tomar dato de un sensor sensor
				for(i=0; i<4; i++)				//Funcion Unidades, decenas, centenas miles....
				{
					switch(i)
					{
						case 0:
							mult = 1000;
							break;
						case 1:
							mult = 100;
							break;
						case 2:
							mult = 10;
							break;
						case 3:
							mult = 1;
							break;
					}
					
					/*mult = pow(10,(3-i));			//1000   100  10 1*/

					j++;                                       //ajuste, lo hace antes ajustes
					tramaPendienteTX[j] = (uint8_t) CONTADOR / mult;    //
					mult = CONTADOR % mult;
				}
				//finaliza Emular tomar dato de un sensor sensor
				CONTADOR++;

			}
			/*else*/
			/*{*/
			/*//Serial.println("Desbordamiento en Tramas por TX. Superado 30 min.");*/
			/*cantidadPendienteTX = 1;*/
			/*}*/

		}
		vTaskDelay(1);
	}
}











// ************************************************************
// Application_Start
// Configure the application and starts tasks
// ************************************************************
void Application_Start()
{
	//xTaskCreate(TestLora_Task, "TestLora_Task", TEST_LORA_TASK_STACK, NULL, TEST_LORA_TASK_PRIORITY, &TestLoraTask_Handle);		//-> TestLora_Task-> PRUEBA UNO A UNO

	xTaskCreate(TestLoraMultiple_Task, "TestLoraMultiple_Task", TEST_LORA_TASK_STACK, NULL, TEST_LORA_TASK_PRIORITY, &TestLoraMultipleTask_Handle);		//-> TestLora_Task-> PRUEBA UNO A UNO

	xTaskCreate(TestLoraTimer_Task, "TestLoraTimer_Task", TEST_LORA_TASK_STACK, NULL, TEST_LORA_TASK_PRIORITY, &TestLoraTimerTask_Handle);
	xTaskCreate(TestLoraTimer2_Task, "TestLoraTimer2_Task", TEST_LORA_TASK_STACK, NULL, TEST_LORA_TASK_PRIORITY, &TestLoraTimer2Task_Handle);


	//xTaskCreate(Application_Config_Task, "Application_Config_Task", APPLICATION_CONFIG_TASK_STACK, NULL, APPLICATION_CONFIG_TASK_PRIORITY, &ApplicationConfigTask_Handle);
	/*xTaskCreate(ToggleLed_Task, "Toggle_Led_Task", TOGGLE_LED_TASK_STACK, NULL, TOGGLE_LED_TASK_PRIORITY, &ToggleLedTask_Handle);*/
	//xTaskCreate(Median_PopulateT0_Task, "Median_PopulateT0_Task", MEDIAN_POPULATE_T0_TASK_STACK, NULL, MEDIAN_POPULATE_T0_TASK_PRIORITY, &Median_PopulateT0_Task_Handle);
	//xTaskCreate(Median_PopulateT1_Task, "Median_PopulateT1_Task", MEDIAN_POPULATE_T1_TASK_STACK, NULL, MEDIAN_POPULATE_T1_TASK_PRIORITY, &Median_PopulateT1_Task_Handle);
	//xTaskCreate(Median_PopulateT2_Task, "Median_PopulateT2_Task", MEDIAN_POPULATE_T2_TASK_STACK, NULL, MEDIAN_POPULATE_T2_TASK_PRIORITY, &Median_PopulateT2_Task_Handle);
	//xTaskCreate(Median_PopulateT3_Task, "Median_PopulateT3_Task", MEDIAN_POPULATE_T3_TASK_STACK, NULL, MEDIAN_POPULATE_T3_TASK_PRIORITY, &Median_PopulateT3_Task_Handle);
	//xTaskCreate(Median_PopulateT4_Task, "Median_PopulateT4_Task", MEDIAN_POPULATE_T4_TASK_STACK, NULL, MEDIAN_POPULATE_T4_TASK_PRIORITY, &Median_PopulateT4_Task_Handle);
	//xTaskCreate(MODBUS_Task, "MODBUS_Task", MODBUS_TASK_STACK, NULL, MODBUS_TASK_PRIORITY, &MODBUSTask_Handle);
}
