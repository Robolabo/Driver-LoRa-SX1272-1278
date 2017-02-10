/*******************************************************************************
Definicion de funciones
*******************************************************************************
 */

#ifndef _halLora_hpp_
#define _halLora_hpp_

#include "project_config.h"
#include "radio.h"

//*********************************************************
//***************      	  SPI        *******************
//*********************************************************
#define LORA_SPI				SPI2
#define LORA_SPI_CLK			RCC_APB1Periph_SPI2
#define LORA_SPI_IRQn			SPI2_IRQn
#define LORA_SPI_PORT			GPIOB

// SPI CS/NSS
#define LORA_NSS_PIN			GPIO_Pin_12
#define LORA_NSS				LORA_SPI_PORT, LORA_NSS_PIN

// SPI SCK
#define LORA_SCK_PIN			GPIO_Pin_13
#define LORA_SCK				LORA_SPI_PORT, LORA_SCK_PIN

// SPI MISO
#define LORA_MISO_PIN			GPIO_Pin_14
#define LORA_MISO				LORA_SPI_PORT, LORA_MISO_PIN

// SPI MOSI
#define LORA_MOSI_PIN			GPIO_Pin_15
#define LORA_MOSI				LORA_SPI_PORT, LORA_MOSI_PIN


#define LORA_RST_PORT			GPIOA
#define LORA_RST_PIN			GPIO_Pin_7
#define LORA_RST				LORA_RST_PORT, LORA_RST_PIN
#define	LORA_RST_MODE			GPIO_Mode_Out_PP



#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)                        //DEFINE la version/fabricante del SX1278 de DORJI o SX1272 de SEMTECH

#define LORA_DIO3_PORT			GPIOA
#define LORA_DIO3_PIN			GPIO_Pin_6
#define LORA_DIO3				LORA_DIO3_PORT, LORA_DIO3_PIN
#define	LORA_DIO3_MODE			GPIO_Mode_IN_FLOATING;

#endif



#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF

#define LORA_TXEN_PORT			GPIOA									//Se configura el PIN A3 como salida-> TXEN
#define LORA_TXEN_PIN			GPIO_Pin_3
#define LORA_TXEN				LORA_TXEN_PORT, LORA_TXEN_PIN
#define	LORA_TXEN_MODE			GPIO_Mode_Out_PP;

#define LORA_RXEN_PORT			GPIOA									//Se configura el PIN A2 como salida-> RXEN
#define LORA_RXEN_PIN			GPIO_Pin_4
#define LORA_RXEN				LORA_RXEN_PORT, LORA_RXEN_PIN
#define	LORA_RXEN_MODE			GPIO_Mode_Out_PP;

#endif




#define LORA_DIO0_PORT			GPIOA
#define LORA_DIO0_PIN			GPIO_Pin_5
#define LORA_DIO0				LORA_DIO0_PORT, LORA_DIO0_PIN
#define	LORA_DIO0_MODE			GPIO_Mode_IN_FLOATING;
#define LORA_DIO0_EXTI 			EXTI_Line5
#define LORA_DIO0_PORT_SOURCE	GPIO_PortSourceGPIOA
#define LORA_DIO0_PIN_SOURCE	GPIO_PinSource5
#define LORA_DIO0_ISR_PRIORITY	8

//---Defincion de funciones de control por hardware--------------------------------------
// -----------------------------------------------------------------------------
//Inicio de configuracion de pines I/O-> NSS, RXTX, RST, DIO0, DIO3
//->NSS
//->RST
//<-DIO0
//<-DIO3
//<-RXTX No se usa
void hal_io_init (void);


//Se utiliza para hacer el reset en el Lora. 0/1 -> salida digital. 2-> entrada o pin flotante
void hal_pin_rst (u1_t val);


//Inicializa el SPI en arduino
void hal_spi_init (void);


//Habilita o no el bus SPI
//->val 0->habilita el bus    /1->Deshabilita el bus
void hal_pin_nss (u1_t val);


// Hace la tranferencia SPI con el radio. Tranfiere out, recibe/retorna res
//->out
//<-res
u1_t hal_spi (u1_t out);



//Hace retardos de ms segun el parametro time_delay
//-> time_delay
void hal_delay(u1_t time_delay);



//Se desarrolla la interrupcion por cambio de flanco ascendente por TXDONE o RXDONE
//Detecta si existio o no ErrorCRC comprobando bit activo en DIO3
void hal_ISR_check(void);


/*
//No es compatible
//Habilita la interrupcion por cambio de flanco ascendente en pin 2 (TXDONE/RXDONE-> DIO0 Lora)
void hal_enableIRQ_RXTXLORA (void);
*/

//Utilizada en ardunio como debugger
void hal_failed (const char *file, u2_t line);  				//Se borra, unicamente para arduino se aplica para


//Inicializa los pines I/O y SPI
void hal_init (void); 					//Configuracion de PINES para comunicacicon digital y SPI con el LORA



#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
void hal_TX_RX_en (u1_t estadoLora);						//Se utiliza para hacer seleccionar TX o RX con la version SX1278 del NICERF
#endif


#endif // _hal_hpp_
