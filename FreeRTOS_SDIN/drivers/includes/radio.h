//Definicion de tipos de variables
//Definicion de funciones
//Definicion de variables globales de radio
//Se definen el uso del CHIP SX1272 SX1278

#ifndef _radio_h_
#define _radio_h_


#include <stdint.h>
#include <project_config.h>

//================================================================================
typedef uint8_t            bit_t;
typedef uint8_t            u1_t;
typedef int8_t             s1_t;
typedef uint16_t           u2_t;
typedef int16_t            s2_t;
typedef uint32_t           u4_t;
typedef int32_t            s4_t;
typedef uint64_t           u8_t;
typedef int64_t            s8_t;
typedef unsigned int       uint;
typedef const char* str_t;

typedef              u1_t* xref2u1_t;


// ----------------------------------------
// Registers Mapping
#define RegFifo                                    0x00 // common
#define RegOpMode                                  0x01 // common
#define RegFrfMsb                                  0x06 // common
#define RegFrfMid                                  0x07 // common
#define RegFrfLsb                                  0x08 // common
#define RegPaConfig                                0x09 // Se utiliza a ahora para cambiar los parametros de
#define RegPaRamp                                  0x0A // common
#define RegOcp                                     0x0B // Se podria urtlizar para aumentar la corriente Imax
#define RegLna                                     0x0C // Cambian parametros -> HF LF
#define LORARegFifoAddrPtr                         0x0D
#define LORARegFifoTxBaseAddr                      0x0E
#define LORARegFifoRxBaseAddr                      0x0F
#define LORARegFifoRxCurrentAddr                   0x10
#define LORARegIrqFlagsMask                        0x11
#define LORARegIrqFlags                            0x12
#define LORARegRxNbBytes                           0x13
#define LORARegRxHeaderCntValueMsb                 0x14
#define LORARegRxHeaderCntValueLsb                 0x15
#define LORARegRxPacketCntValueMsb                 0x16
#define LORARegRxpacketCntValueLsb                 0x17
#define LORARegModemStat                           0x18	//Lo recomiendan usar para monitoreo del MODEM-> STATUS
#define LORARegPktSnrValue                         0x19
#define LORARegPktRssiValue                        0x1A
#define LORARegRssiValue                           0x1B	//Nueva formula de calculo
#define LORARegHopChannel                          0x1C
#define LORARegModemConfig1                        0x1D	//Cambian los valores de las posiciones de los bits-> actualizan
#define LORARegModemConfig2                        0x1E	//Cambian los valores de las posiciones de los bits-> actualizan
#define LORARegSymbTimeoutLsb                      0x1F
#define LORARegPreambleMsb                         0x20
#define LORARegPreambleLsb                         0x21
#define LORARegPayloadLength                       0x22
#define LORARegPayloadMaxLength                    0x23
#define LORARegHopPeriod                           0x24
#define LORARegFifoRxByteAddr                      0x25
#define LORARegModemConfig3                        0x26	//Actualizacion. Registro necesario para configuracion del MODEM
#define LORARegFeiMsb                              0x28
#define LORAFeiMib                                 0x29
#define LORARegFeiLsb                              0x2A
#define LORARegRssiWideband                        0x2C
#define LORARegDetectOptimize                      0x31

#define LORARegInvertIQ                            0x33
#define LORARegDetectionThreshold                  0x37
#define LORARegSyncWord                            0x39
#define RegDioMapping1                             0x40 // common
#define RegDioMapping2                             0x41 // common
#define RegVersion                                 0x42 // Cambia el valor
// #define RegAgcRef                                  0x43 // common
// #define RegAgcThresh1                              0x44 // common
// #define RegAgcThresh2                              0x45 // common
// #define RegAgcThresh3                              0x46 // common
// #define RegPllHop                                  0x4B // common
// #define RegTcxo                                    0x58 // common
#if defined(CFG_1272_SEMTECH)                          //Selecciona el CHIP SX1272
#define RegPaDac                                   0x5A // common			 Se utliza para la operacion a alta potencia +20dBm
#endif
#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1278_NICERF1278)    //Selecciona el CHIP SX1278
#define RegPaDac                                   0x4D // common			 Se utliza para la operacion a alta potencia +20dBm
#endif

// #define RegPll                                     0x5C // common
// #define RegPllLowPn                                0x5E // common
// #define RegFormerTemp                              0x6C // common
// #define RegBitRateFrac                             0x70 // common

// ----------------------------------------


#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1278_NICERF1278)    //Selecciona el CHIP SX1278
#define SX1278_MC2_RX_PAYLOAD_CRCON        0x04             //Bit 2 del LORARegModemConfig2
#define SX1278_MC3_LOW_DATA_RATE_OPTIMIZE  0x08				//Bit 3 del LORARegModemConfig3
#define SX1278_MC3_AGCAUTO                 0x04				//Bit 2 del LORARegModemConfig3
#endif


#if defined(CFG_1272_SEMTECH)                          			//Selecciona el CHIP SX1272
#define SX1272_MC1_RX_PAYLOAD_CRCON			0x02			//Bit 1 del LORARegModemConfig1
#define SX1272_MC1_LOW_DATA_RATE_OPTIMIZE	0x01			//Bit 0 del LORARegModemConfig1
#define SX1272_MC2_AGCAUTO_ON				0x04			//Bit 2 del LORARegModemConfig2
#endif




// transmit power configuration for RegPaConfig
#define SX1272_PAC_PA_SELECT_PA_BOOST 0x80
#define SX1272_PAC_PA_SELECT_RFIO_PIN 0x00


// preamble for lora networks (nibbles swapped)
#define LORA_MAC_PREAMBLE                  0x34


																		//SX1278 -> Registros de configuracion utilizados para Iniciar el modem (Solo testeo)
#define RXLORA_RXMODE_RSSI_REG_MODEM_CONFIG1 (0x70|0x02)				//BW=125Khz     Code rating=4/5    Explicit header
#define RXLORA_RXMODE_RSSI_REG_MODEM_CONFIG2     0x70   				//SF=7 -> 128chips/symbols   TxContinousMode=0-> Single Packet  RxPayloadCrcOn-> 0    SymbTimeout->00
#define RXLORA_RXMODE_RSSI_REG_MODEM_CONFIG3     0x04					//Low DatarateOptimize->0     AgcAutoOn->1  LNA gain set by internal AGC Loop*












// ----------------------------------------
// Constants for radio registers
#define OPMODE_LORA      0x80
#define OPMODE_MASK      0x07
#define OPMODE_SLEEP     0x00
#define OPMODE_STANDBY   0x01
//#define OPMODE_FSTX      0x02
#define OPMODE_TX        0x03
//#define OPMODE_FSRX      0x04
#define OPMODE_RX        0x05
//#define OPMODE_RX_SINGLE 0x06
//#define OPMODE_CAD       0x07

// ----------------------------------------
// Bits masking the corresponding IRQs from the radio
#define IRQ_LORA_RXTOUT_MASK 0x80
#define IRQ_LORA_RXDONE_MASK 0x40
#define IRQ_LORA_CRCERR_MASK 0x20
#define IRQ_LORA_HEADER_MASK 0x10
#define IRQ_LORA_TXDONE_MASK 0x08
#define IRQ_LORA_CDDONE_MASK 0x04
#define IRQ_LORA_FHSSCH_MASK 0x02
#define IRQ_LORA_CDDETD_MASK 0x01

#define IRQ_FSK1_MODEREADY_MASK         0x80
#define IRQ_FSK1_RXREADY_MASK           0x40
#define IRQ_FSK1_TXREADY_MASK           0x20
#define IRQ_FSK1_PLLLOCK_MASK           0x10
#define IRQ_FSK1_RSSI_MASK              0x08
#define IRQ_FSK1_TIMEOUT_MASK           0x04
#define IRQ_FSK1_PREAMBLEDETECT_MASK    0x02
#define IRQ_FSK1_SYNCADDRESSMATCH_MASK  0x01
#define IRQ_FSK2_FIFOFULL_MASK          0x80
#define IRQ_FSK2_FIFOEMPTY_MASK         0x40
#define IRQ_FSK2_FIFOLEVEL_MASK         0x20
#define IRQ_FSK2_FIFOOVERRUN_MASK       0x10
#define IRQ_FSK2_PACKETSENT_MASK        0x08
#define IRQ_FSK2_PAYLOADREADY_MASK      0x04
#define IRQ_FSK2_CRCOK_MASK             0x02
#define IRQ_FSK2_LOWBAT_MASK            0x01

// ----------------------------------------
// DIO function mappings                D0D1D2D3
#define MAP_DIO0_LORA_RXDONE   0x00  // 00------
#define MAP_DIO0_LORA_TXDONE   0x40  // 01------
#define MAP_DIO1_LORA_RXTOUT   0x00  // --00----
#define MAP_DIO1_LORA_NOP      0x30  // --11----
#define MAP_DIO2_LORA_NOP      0x0C  // ----11--


#define MAP_DIO0_FSK_READY     0x00  // 00------ (packet sent / payload ready)
#define MAP_DIO1_FSK_NOP       0x30  // --11----
#define MAP_DIO2_FSK_TXNOP     0x04  // ----01--
#define MAP_DIO2_FSK_TIMEOUT   0x08  // ----10--


#define MAP_DIO3_VALIDHEADER   0x01  	// ------01	//ValidHeader
#define MAP_DIO3_PAYLOADCRCERROR   0x02 // ------10	// PayloadCrcError
#define MAP_DIO3_LORA_NOP      0x03  	// ------11	//Sin uso


// FSK IMAGECAL defines
#define RF_IMAGECAL_AUTOIMAGECAL_MASK               0x7F
#define RF_IMAGECAL_AUTOIMAGECAL_ON                 0x80
#define RF_IMAGECAL_AUTOIMAGECAL_OFF                0x00  // Default

#define RF_IMAGECAL_IMAGECAL_MASK                   0xBF
#define RF_IMAGECAL_IMAGECAL_START                  0x40

#define RF_IMAGECAL_IMAGECAL_RUNNING                0x20
#define RF_IMAGECAL_IMAGECAL_DONE                   0x00  // Default


//Nuevo, esto funciona para el calculo del tiempo/simbolo
// sx1278
#define SX1278_BW_7_8              	7.8    //7.8Khz
#define SX1278_BW_10_4              10.4    //10.4Khz
#define SX1278_BW_15_6              15.6    //15.6Khz
#define SX1278_BW_20_8              20.8    //20.8Khz
#define SX1278_BW_31_25             31.25   //31.25Khz
#define SX1278_BW_41_7              41.7    //41.7Khz
#define SX1278_BW_62_5              62.5    //62.5Khz
#define SX1278_BW_125               125     //125Khz
#define SX1278_BW_250               250     //250Khz
#define SX1278_BW_500               500     //500Khz




#if defined(CFG_1272_SEMTECH)                          //Selecciona el CHIP SX1272
#define versionLora		0X22
#endif
#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1278_NICERF1278)    //Selecciona el CHIP SX1278
#define versionLora		0X12
#endif


#define LNA_RX_GAIN (0x20|0x3)						//antes SX1278 #define LNA_RX_GAIN (0x20|0x03)
//Se configura LnaBoostLf-> corriente por defecto   LnaBoostHf-> Boost on, 150% LNA current



#include <string.h>
#include "halLora.h"										//LLAMADO A LIBRERIA HAL.CPP



#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
enum { TX_RX_EN_SLEEP = 0, TX_EN_OK, RX_EN_OK};				//TX_RX_EN_SLEEP->Sleep Lora     TX_EN_OK->TX lora     RX_EN_OK-> RX lora
#endif


#define DEFINE_RADIO  struct radio_t RADIO
#define DECLARE_RADIO extern struct radio_t RADIO

//================================================================================
//Definicion de funciones


//Ingresa: RADIO.bw     RADIO.sf
//Calcula Ts
//Retorna 0 si Ts<16ms   1 si Ts>16ms
//Calculo del tiempo por simbolo (Ts) util para establecer si se debe habilitar el Bit de LowDataRateOptimize (Ts>16ms)
 u1_t tiempoPorSimbolo (void);					//Ts(ms)= 2^SF / (BW {hz}) * 1000


 /*
 Escribe en los registros del LORA
 Escribe sobre el registro addr el valor de data
 addr-> Direccion a escribir. En modo escritura, la escritura se habilita con el valor del registro BIT7->1
 data -> Valor a escribir
  */
 void writeReg (u1_t addr, u1_t data );


 /*
 Lee registros del LORA
 Lee el registro addr y retorna su contenido en val
 addr-> Direccion a leer.
 val -> Valor de la direccion
 */
 u1_t readReg (u1_t addr);


 /*
 Utilizada para escribir sobre la FIFO del LORA
 RegFiFO-> Direccion inicial FIFO
 RADIO.frameTX-> Vector referencia para leer y luego escribir sobre la FIFO
 RADIO.dataLenTX -> Longitud de datos a escribir
 */
 void writeBuf (u1_t addr, xref2u1_t buf, u1_t len);


 /*
 Utilizada para leer sobre la FIFO del LORA
 addr (RegFiFO{0})-> Direccion inicial FIFO
 RADIO.frameRX		-> Vector referencia donde se escribiron los datos
 RADIO.dataLenRX 	-> Longitud de datos a escribir
 Escribe en los registros del LORA
 Escribe sobre el registro addr el valor de data
 addr-> Direccion a escribir. En modo escritura, la escritura se habilita con el valor del registro BIT7->1
 data -> Valor a escribir
 */
 void readBuf (u1_t addr, xref2u1_t buf, u1_t len);		//Utilizada para leer los datos del la FIFO. addr-> DIRECCION inicial a leer     buf->VECTOR de alamcenamiento  len-> longitud de los datos a leer




 /*
 OPMODE_LORA     	0x80
 OPMODE_SLEEP    	0x00
 OPMODE_STANDBY  	0x01
 OPMODE_TX        	0x03
 OPMODE_RX        	0x05
 OPMODE_RX_SINGLE 	0x06
 Escribe el modo "mode" en el  registro RegOpMode{1}
 Enmascara los bits 7:3 y escribe el mode sobre 2:0
 */
 void opmode (u1_t mode);



 /*
 Escribe sobre el  RegOpMode{1} el  Modo Lora Bit7->1 y el modo SLEEP
  */
 void opmodeLora(void); 							//Se utiliza para escribir el modo LORA y modo SLEEP. Coloca el bit 7->1.




 /*
 sx1278 -> LORARegModemConfig1  -> 0x1D
  BW					  CR				ImplicitHeaderModeOn
  7:4				  3:1							0
 0000  7.8 kHz		001  4/5			0 -> Explicit Header mode*
 0001  10.4 kHz		010  4/6			1 -> Implicit Header mode
 0010  15.6 kHz		011  4/7
 0011  20.8kHz		100  4/8
 0100  31.25 kHz
 0101  41.7 kHz
 0110  62.5 kHz
 0111  125 kHz
 1000  250 kHz
 1001  500 kHz
 other values

 BW=125 kHz  0X70
 CR= 0X02 //CR = 4/5
 ImplicitHeaderModeOn= 0  0 -> Explicit Header mode*
  */

  /*
 sx12878 -> LORARegModemConfig2    0x1E

   SF (SpreadingFactor)		TxContinuousMode							RxPayloadCrcOn			SymbTimeout(9:8)
       7:4							3										2							1:0
 6 -> 64 chips / symbol		0 -> normal mode, single packet is sent*	0 -> CRC disable		SymbTimeout(9:8)
 7 -> 128 chips / symbol		1->continuous mode							1 -> CRC enable*		RX Time-Out MSB ->0**
 8 -> 256 chips / symbol
 9 -> 512 chips / symbol
 10 -> 1024 chips / symbol
 11 -> 2048 chips / symbol
 12 -> 4096 chips / symbol

 RXLORA_RXMODE_RSSI_REG_MODEM_CONFIG2
 SF = 0X90
 TxContinuousMode = 0 (Single Packet)
 RxPayloadCrcOn = 0 (CRC disable)


 SF (SpreadingFactor)		TxContinuousMode							AgcAutoOn
       7:4							3										2										1:0
 6 -> 64 chips / symbol		0 -> normal mode, single packet is sent*	0 -> LNA gain set by register LnaGain		SymbTimeout(9:8)
 7 -> 128 chips / symbol		1->continuous mode							1 -> LNA gain set by the internal AGC loop*	RX Time-Out MSB ->0**
 8 -> 256 chips / symbol
 9 -> 512 chips / symbol
 10 -> 1024 chips / symbol
 11 -> 2048 chips / symbol
 12 -> 4096 chips / symbol
 */

 /*
 sx12878 -> LORARegModemConfig3    0x26

 	Reservado		TxContinuousMode							AgcAutoOn								Reservado
 	   7:4					3										2										1:0
 				0 -> normal mode, single packet is sent*	0 -> LNA gain set by register LnaGain			00
 				1->continuous mode							1 -> LNA gain set by the internal AGC loop*

 */


/*
sx1272 -> LORARegModemConfig1  -> 0x1D						CR
 	BW				coding rate			ImplicitHeaderModeOn			RxPayloadCrcOn			LowDataRateOptimize
   7:6					5:3						2							1						0
  00->125K			001 -> 4/5			0 -> Explicit Header mode*		0 -> CRC disable		0 -> Disabled
  01->250K			010 -> 4/6			1 -> Implicit Header mode		1 -> CRC enable*		1 ->Enabled; mandated for SF11 and SF12 with BW = 125 kHz
  10->500K			011 -> 4/7
  11->reservado		100 -> 4/8
 */

  /*
sx1272 -> LORARegModemConfig2  -> 0x1E
         	SF				TxContinuousMode							AgcAutoOn
       		7:4						3										2										1:0
 6 -> 64 chips / symbol		0 -> normal mode, single packet is sent*	0 -> LNA gain set by register LnaGain		SymbTimeout(9:8)
 7 -> 128 chips / symbol		1->continuous mode							1 -> LNA gain set by the internal AGC loop*	RX Time-Out MSB ->0**
 8 -> 256 chips / symbol
 9 -> 512 chips / symbol
 10 -> 1024 chips / symbol
 11 -> 2048 chips / symbol
 12 -> 4096 chips / symbol
 */
 /*
 Toma los valores de RADIO.sf, RADIO.bw, RADIO.cr y los escribe en los registros
 LORARegModemConfig1{1D}-> LORA
 LORARegModemConfig2{1E}-> LORA
 LORARegModemConfig3{26}-> LORA
 RADIO.regModemConfig1->Mc
 RADIO.regModemConfig2->Mc
 RADIO.regModemConfig3->Mc
  */
 void configLoraModem (void);



 /*
 Configura el Canal de acuerdo con la frecuencia de RADIO.freq en 3 registros
 RADIO.freq
 Frecuencia OSC LORA (32MHZ)

 RegFrfMsb{06}
 RegFrfMid {07}
 RegFrfLsb{08}

 FQ = (FRF * 32 Mhz) / (2 ^ 19)
  * Ecuacion General
  * FR= frecuencia
  * FRF= precuencia de la portadora
  * 32MHz-> Frecuencia del OSCILADOR LORA
  */
 void configChannel (void);



 /*
 Configura la potencia a Max potencia RADIO.txpow.
 Potencia limitada hasta 20dBm.
 Configura PA ramp-up time (RADIO.PaRamp)
 Entrada->
 RADIO.txpow
 RADIO.PaRamp

 Escribe en el LORA
 RegPaConfig{09}
 RegPaRamp{0A}
 RegPaDac {4D}
 */
 void configPower (void);




 /*
 Configura el modo a modo TX
 Para configurar el lora siempre debe cambiarse de modo SLEEP{00} a Modo STANDBY{01}
 Carga los datos de RADIO.frameTX al FIFO del LORA. La cantidad de datos es de RADIO.dataLen

 Ingresa
 RADIO.frameTX			(Buffer de datos a TX)
 RADIO.dataLenTX  		(cantidad de satos a TX)

 Modifica
 LORARegFifoTxBaseAddr{0E}
 LORARegFifoAddrPtr{0D}
 LORARegPayloadLength{22}
 */
 void txlora (void);



/*
Configura el modo a modo RX
Para configurar el LORA siempre debe cambiarse de modo SLEEP{00} a Modo STANDBY{01}
Carga los datos de RADIO.frameRX al FIFO del LORA. La cantidad de datos es de RADIO.dataLen
*/
void rxlora (void);



/*
Corresponde a la funcion general que configura todos los parametros de inicio del MODEM LORA
Esta funcion solo se realiza una vez
La primera parte gnera 16 bytes aleatorios para el "calentamiento - warm up" el modem a partir de ruido RSSI
*/
void radio_init (void);


/*
//Atencion por Software al ISR
//Continuacion de la funcion hal_io_check (Int a nivel de Hardware)
*/
void radio_irq_handler (u1_t errorCRC); 				//Despues de una interrupcion por RX o TX -> El radio pasa de SLEEP/RXMODE/TXMODE-> STANDBY

/*
 Establece el modo de uso del radio para TX o RX
*/
void radio_mode (u1_t mode);


/*
 Inicia formalmente el uso del radio utilizando el pin de reset
*/
void radio_hal_init (void);

/*
 Funcion util para controbar si la comunicacion o conexion con el LoRa han sido correctas. Modo debugger
 */
void radio_Checkpoint_Version(void);


/*
Utilizada para trasnscribir los datos del buf  al vector utilizado por el Lora para TX
*/
void radio_buffer_to_frameTX (xref2u1_t buf, u1_t len);



//-------Finaliza definicion de funciones=================================



//UTILIZA LOS PARAMETROS para RADIO.sf   RADIO.bw    RADIO.cr

enum _cr_t { CR_4_5=1, CR_4_6, CR_4_7, CR_4_8 };					//Usado para CR
enum _sf_t { SF_6=6, SF_7, SF_8, SF_9, SF_10, SF_11, SF_12 };		//Usado para SF

#if defined(CFG_1272_SEMTECH)                          //Selecciona el CHIP SX1272
enum _bw_t { BW125=0, BW250, BW500, BWrfu };		//necesario ACTUALIZADO
#endif

#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1278_NICERF1278)    //Selecciona el CHIP SX1278
enum _bw_t { BW7_8=0, BW10_4, BW15_6, BW20_8, BW31_25, BW41_7, BW62_5, BW125, BW250, BW500, BWrfu };		//necesario ACTUALIZADO
#endif

enum { MAX_LEN_FRAME  = 64 };																				//Puede modificarse. Establece la cantidad de bytes de TX o RX como maximo


enum { RADIO_RST=0, RADIO_TX=1, RADIO_RX=2, RADIO_RXON=3 };

//-----Defincion de estructura de datos usados para configuracion---------------
struct radio_t {
    u4_t        freq;
    s1_t        rssi;
    s1_t        snr;

    u1_t        txpow;     		// dBm
    u1_t        imax;     		// mA -> Por defecto la corriente Imax es de 100mA ->       45mA <=Imax <= 240mA

    u1_t        crc;			//SE INCORPORA  para comprobar error CRC
    u1_t        flag;			//SE INCORPORA  para comprobar error FLAG
    u1_t        flagTx;			//SE INCORPORA  para comprobar  FLAGTX
    u1_t        flagRx;			//SE INCORPORA  para comprobar  FLAGRX
    u1_t		statusRXTX;		//Se incorpora para comprobar estados RST RX TX

    u1_t		sf;				//Se incorpora para	SF
    u1_t		bw;				//Se incorpora para el BW
    u1_t		cr;				//Se incorpora para el code rating
    u1_t		regModemConfig1;//Se incorpora para RegModemConfig1 1D
    u1_t		regModemConfig2;//Se incorpora para RegModemConfig2 1E

#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1278_NICERF1278)    //Selecciona el CHIP SX1278
    u1_t		regModemConfig3;//Se incorpora para RegModemConfig3 26. Aplica unicamnete para SX1278
#endif


    u1_t		PaRamp;			//Se incorpora para PaRamp 0A

    u1_t        dataLenTX;    	//Cantidad de datos a transmitir en el frameTX
    u1_t        dataLenRX;    	//Cantidad de datos recibidos en el frameRX

    u1_t        frameTX[MAX_LEN_FRAME];			//Se incorpora para Buffer de TX
    u1_t        frameRX[MAX_LEN_FRAME];			//Se incorpora para Buffer de RX
};
//struct radio_t RADIO


DECLARE_RADIO; //

#endif // _lmic_h_
