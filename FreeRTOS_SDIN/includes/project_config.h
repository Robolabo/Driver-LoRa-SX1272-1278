//*****************************************************************************
//
// project_config.h
//
//*****************************************************************************
//Se configura el uso del chip radio
/** 
 *	@file project_config.h

		@brief Main definitions of specific hardware

		@author Alvaro Gutierrez
		@author Robolabo
		@author www.robolabo.etsit.upm.es
		@date 2013/05/22 

    CREATIVE COMMONS PUBLIC LICENSE:
		
		THE WORK (AS DEFINED BELOW) IS PROVIDED UNDER THE TERMS OF THIS CREATIVE COMMONS PUBLIC LICENSE ("CCPL" OR "LICENSE"). 
		THE WORK IS PROTECTED BY COPYRIGHT AND/OR OTHER APPLICABLE LAW. 
		ANY USE OF THE WORK OTHER THAN AS AUTHORIZED UNDER THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.

		BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED HERE, YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS OF THIS LICENSE. 
		TO THE EXTENT THIS LICENSE MAY BE CONSIDERED TO BE A CONTRACT, 
		THE LICENSOR GRANTS YOU THE RIGHTS CONTAINED HERE IN CONSIDERATION OF YOUR ACCEPTANCE OF SUCH TERMS AND CONDITIONS.
*/

#include "stm32f10x.h"

#ifndef PROJECT_CONFIG_H_
#define PROJECT_CONFIG_H_

/** @brief Board Version */
#define	BOARD_ED_A
/** @brieg Software Version */
#define SOFTWARE_VERSION 	"SenseDin. Rev 1. 23-04-2013"


//------------------Configuracion de LORA----version a utilizar---------------

//#define CFG_1272_SEMTECH		1						//Selecciona el CHIP SX1272 de SEMTECH*/

//#define CFG_1278_DRF1278F		1						//Selecciona el CHIP SX1278 de DORJI version de tarjeta DRF1278F

#define CFG_1278_NICERF1278	1						//Selecciona el CHIP SX1278 de NICERF version de tarjeta LORA1278*/




//*********************************************************
//***************        OUTPUTS	    *******************
//*********************************************************

/* LEDs */
#define LED_H0_PORT			GPIOA
#define LED_H0_PIN			GPIO_Pin_12
#define LED_H0				LED_H0_PORT, LED_H0_PIN
#define	LED_H0_MODE			GPIO_Mode_Out_PP

#define LED_H1_PORT			GPIOA
#define LED_H1_PIN			GPIO_Pin_11
#define LED_H1				LED_H1_PORT, LED_H1_PIN
#define	LED_H1_MODE			GPIO_Mode_Out_PP

#define LED_H2_PORT			GPIOA
#define LED_H2_PIN			GPIO_Pin_8
#define LED_H2				LED_H2_PORT, LED_H2_PIN
#define	LED_H2_MODE			GPIO_Mode_Out_PP

#define LED_H3_PORT			GPIOB
#define LED_H3_PIN			GPIO_Pin_11
#define LED_H3				LED_H3_PORT, LED_H3_PIN
#define	LED_H3_MODE			GPIO_Mode_Out_PP

/* TRTs */
#define OUTPUT_TRT0_PORT		GPIOB
#define OUTPUT_TRT0_PIN			GPIO_Pin_12
#define OUTPUT_TRT0			OUTPUT_TRT0_PORT, OUTPUT_TRT0_PIN
#define	OUTPUT_TRT0_MODE		GPIO_Mode_Out_PP

#define OUTPUT_TRT1_PORT		GPIOB
#define OUTPUT_TRT1_PIN			GPIO_Pin_13
#define OUTPUT_TRT1			OUTPUT_TRT1_PORT, OUTPUT_TRT1_PIN
#define	OUTPUT_TRT1_MODE		GPIO_Mode_Out_PP

#define OUTPUT_TRT2_PORT		GPIOB
#define OUTPUT_TRT2_PIN			GPIO_Pin_14
#define OUTPUT_TRT2			OUTPUT_TRT2_PORT, OUTPUT_TRT2_PIN
#define	OUTPUT_TRT2_MODE		GPIO_Mode_Out_PP

#define OUTPUT_TRT3_PORT		GPIOB
#define OUTPUT_TRT3_PIN			GPIO_Pin_15
#define OUTPUT_TRT3			OUTPUT_TRT3_PORT, OUTPUT_TRT3_PIN
#define	OUTPUT_TRT3_MODE		GPIO_Mode_Out_PP

#define OUTPUT_TRT4_PORT		GPIOA
#define OUTPUT_TRT4_PIN			GPIO_Pin_1
#define OUTPUT_TRT4			OUTPUT_TRT4_PORT, OUTPUT_TRT4_PIN
#define	OUTPUT_TRT4_MODE		GPIO_Mode_Out_PP

#define OUTPUT_TRT5_PORT		GPIOA
#define OUTPUT_TRT5_PIN			GPIO_Pin_0
#define OUTPUT_TRT5			OUTPUT_TRT5_PORT, OUTPUT_TRT5_PIN
#define	OUTPUT_TRT5_MODE		GPIO_Mode_Out_PP

//*********************************************************
//***************      	  TIMERS        *******************
//*********************************************************
/*#define TIMER_BASE			TIM2*/
/*#define TIMER_BASE_IRQn		TIM2_IRQn*/
/*#define TIMER_BASE_CLK		RCC_APB1Periph_TIM2*/
/*#define TIMER_BASE_IRQHandler		TIM2_IRQHandler*/


//*********************************************************
//***************      UART CONFIG      *******************
//***  IMPORTANT! More configuration params in serie.h!
//*********************************************************
#define UART_NUM				1
#define UART_SPEED				9600
#define UART_DEFAULT_TIMEOUT			100

#define SERIE_ISR_PRIORITY			6

#define MODBUS_TIMER_ISR_PRIORITY		7
#define MODBUS_ADDRESS        			0x34
#define MODBUS_BAUDRATE       			9600
#define MODBUS_PARITY       			MB_PAR_NONE

//*********************************************************
//***************        INPUTS		    *******************
//*********************************************************

/* Switches */
#define INPUT_SWITCH0_PORT			GPIOC
#define INPUT_SWITCH0_PIN			GPIO_Pin_15
#define INPUT_SWITCH0				INPUT_SWITCH0_PORT, INPUT_SWITCH0_PIN
#define INPUT_SWITCH0_MODE			GPIO_Mode_IPU

#define INPUT_SWITCH1_PORT			GPIOC
#define INPUT_SWITCH1_PIN			GPIO_Pin_14
#define INPUT_SWITCH1				INPUT_SWITCH1_PORT, INPUT_SWITCH1_PIN
#define INPUT_SWITCH1_MODE			GPIO_Mode_IPU

#define INPUT_SWITCH2_PORT			GPIOB
#define INPUT_SWITCH2_PIN			GPIO_Pin_9
#define INPUT_SWITCH2				INPUT_SWITCH2_PORT, INPUT_SWITCH2_PIN
#define INPUT_SWITCH2_MODE			GPIO_Mode_IPU

#define INPUT_SWITCH3_PORT			GPIOB
#define INPUT_SWITCH3_PIN			GPIO_Pin_8
#define INPUT_SWITCH3				INPUT_SWITCH3_PORT, INPUT_SWITCH3_PIN
#define INPUT_SWITCH3_MODE			GPIO_Mode_IPU

#define INPUT_SWITCH4_PORT			GPIOB
#define INPUT_SWITCH4_PIN			GPIO_Pin_5
#define INPUT_SWITCH4				INPUT_SWITCH4_PORT, INPUT_SWITCH4_PIN
#define INPUT_SWITCH4_MODE			GPIO_Mode_IPU

/* ID */
#define INPUT_ID0_PORT				GPIOB
#define INPUT_ID0_PIN				GPIO_Pin_10
#define INPUT_ID0				INPUT_ID0_PORT, INPUT_ID0_PIN
#define INPUT_ID0_MODE				GPIO_Mode_IPU

#define INPUT_ID1_PORT				GPIOB
#define INPUT_ID1_PIN				GPIO_Pin_2
#define INPUT_ID1				INPUT_ID1_PORT, INPUT_ID1_PIN
#define INPUT_ID1_MODE				GPIO_Mode_IPU

#define INPUT_ID2_PORT				GPIOB
#define INPUT_ID2_PIN				GPIO_Pin_1
#define INPUT_ID2				INPUT_ID2_PORT, INPUT_ID2_PIN
#define INPUT_ID2_MODE				GPIO_Mode_IPU

#define INPUT_ID3_PORT				GPIOB
#define INPUT_ID3_PIN				GPIO_Pin_0
#define INPUT_ID3				INPUT_ID3_PORT, INPUT_ID3_PIN
#define INPUT_ID3_MODE				GPIO_Mode_IPU

/* ADCs */
#define INPUT_ADC0_PORT				GPIOA
#define INPUT_ADC0_PIN				GPIO_Pin_7
#define INPUT_ADC0				INPUT_ADC0_PORT, INPUT_ADC0_PIN
#define INPUT_ADC0_MODE				GPIO_Mode_AIN

#define INPUT_ADC1_PORT				GPIOA
#define INPUT_ADC1_PIN				GPIO_Pin_6
#define INPUT_ADC1				INPUT_ADC1_PORT, INPUT_ADC1_PIN
#define INPUT_ADC1_MODE				GPIO_Mode_AIN

#define INPUT_ADC2_PORT				GPIOA
#define INPUT_ADC2_PIN				GPIO_Pin_5
#define INPUT_ADC2				INPUT_ADC2_PORT, INPUT_ADC2_PIN
#define INPUT_ADC2_MODE				GPIO_Mode_AIN

#define INPUT_ADC3_PORT				GPIOA
#define INPUT_ADC3_PIN				GPIO_Pin_4
#define INPUT_ADC3				INPUT_ADC3_PORT, INPUT_ADC3_PIN
#define INPUT_ADC3_MODE				GPIO_Mode_AIN

#define INPUT_ADC4_PORT				GPIOA
#define INPUT_ADC4_PIN				GPIO_Pin_3
#define INPUT_ADC4				INPUT_ADC4_PORT, INPUT_ADC4_PIN
#define INPUT_ADC4_MODE				GPIO_Mode_AIN

#define INPUT_ADC5_PORT				GPIOA
#define INPUT_ADC5_PIN				GPIO_Pin_2
#define INPUT_ADC5				INPUT_ADC5_PORT, INPUT_ADC5_PIN
#define INPUT_ADC5_MODE				GPIO_Mode_AIN

#define INPUT_ADC6_PORT				GPIOA
#define INPUT_ADC6_PIN				GPIO_Pin_1
#define INPUT_ADC6				INPUT_ADC6_PORT, INPUT_ADC6_PIN
#define INPUT_ADC6_MODE				GPIO_Mode_AIN

//*********************************************************
//***************      	  SPI        *******************
//*********************************************************
#define EXT_ADC_SPI				    SPI2
#define EXT_ADC_SPI_CLK				RCC_APB1Periph_SPI2
#define EXT_ADC_SPI_IRQn			SPI2_IRQn
#define EXT_ADC_SPI_PORT			GPIOB

// SPI CS/NSS
#define EXT_ADC_NSS_PIN				GPIO_Pin_12
#define EXT_ADC_NSS				EXT_ADC_SPI_PORT, EXT_ADC_NSS_PIN

// SPI SCK
#define EXT_ADC_SCK_PIN				GPIO_Pin_13
#define EXT_ADC_SCK				EXT_ADC_SPI_PORT, EXT_ADC_SCK_PIN

// SPI MISO
#define EXT_ADC_MISO_PIN			GPIO_Pin_14
#define EXT_ADC_MISO				EXT_ADC_SPI_PORT, EXT_ADC_MISO_PIN

// SPI MOSI
#define EXT_ADC_MOSI_PIN			GPIO_Pin_15
#define EXT_ADC_MOSI				EXT_ADC_SPI_PORT, EXT_ADC_MOSI_PIN

//SPI RDY INTERRUPT - SAME AS MISO
#define EXT_ADC_DRDY_PORT 			GPIOB
#define EXT_ADC_DRDY_PIN  			GPIO_Pin_14
#define EXT_ADC_DRDY    			EXT_ADC_DRDY_PORT, EXT_ADC_DRDY_PIN
#define EXT_ADC_DRDY_EXTI 			EXTI_Line14
#define EXT_ADC_DRDY_PORT_SOURCE		GPIO_PortSourceGPIOB
#define EXT_ADC_DRDY_PIN_SOURCE			GPIO_PinSource14
#define EXTADC_DRDY_ISR_PRIORITY  		8

//*********************************************************
//***************          I2C	        *******************
//*********************************************************
#define I2C_PORT    				GPIOB

#define I2C_SCL_PIN 				GPIO_Pin_6
#define I2C_SCL 				I2C_PORT, I2C_SCL_PIN

#define I2C_SDA_PIN 				GPIO_Pin_7
#define I2C_SDA 				I2C_PORT, I2C_SDA_PIN

#define I2C_MODE 				GPIO_Mode_AF_OD

#define I2C                          		I2C1                                                            
#define I2C_CLK                      		RCC_APB1Periph_I2C1                                             
#define I2C_SCL_GPIO_CLK             		RCC_APB2Periph_GPIOB                                            
#define I2C_SDA_GPIO_CLK             		RCC_APB2Periph_GPIOB                                            

//*********************************************************
//***************  		QUEUES SIZE 	*******************
//*********************************************************

//*********************************************************
//***************      	  TIMERS        *******************
//*********************************************************

//*********************************************************
//***************  TASK's PRIO y STACKS *******************
//*********************************************************
/* Priority is higher as higher is its value 
 * It is important to check the configMAX_PRIORITIES value in 
 * includes/FreeRTOSConfig.h. Priorities should not go higher than
 * that value, because they will lowered to it */

#define TEST_LEDS_TASK_PRIORITY			1
#define TEST_LEDS_TASK_STACK			128

#define TEST_MODBUS_TASK_PRIORITY		2
#define TEST_MODBUS_TASK_STACK			512

#define TEST_MODBUS_2_TASK_PRIORITY		2
#define TEST_MODBUS_2_TASK_STACK		512

#define TEST_SWITCHES_TASK_PRIORITY		1
#define TEST_SWITCHES_TASK_STACK		128

#define TEST_ID_TASK_PRIORITY			1
#define TEST_ID_TASK_STACK			128

#define TEST_TRT_TASK_PRIORITY			1
#define TEST_TRT_TASK_STACK			128

#define TEST_ADC_TASK_PRIORITY			1
#define TEST_ADC_TASK_STACK			128

#define TEST_SERIAL_TASK_PRIORITY		1
#define TEST_SERIAL_TASK_STACK			128	

#define TEST_SAI_TASK_PRIORITY			1
#define TEST_SAI_TASK_STACK			128	

#define APPLICATION_CONFIG_TASK_PRIORITY		5
#define APPLICATION_CONFIG_TASK_STACK			128 

#define TOGGLE_LED_TASK_PRIORITY		1	
#define TOGGLE_LED_TASK_STACK			128 

#define TEST_LORA_TASK_PRIORITY		1	
#define TEST_LORA_TASK_STACK			256

#define MODBUS_TASK_PRIORITY			4	
#define MODBUS_TASK_STACK			256 

#define MEDIAN_POPULATE_T0_TASK_PRIORITY 	3
#define MEDIAN_POPULATE_T0_TASK_STACK 		128

#define MEDIAN_POPULATE_T1_TASK_PRIORITY 	2
#define MEDIAN_POPULATE_T1_TASK_STACK 		128

#define MEDIAN_POPULATE_T2_TASK_PRIORITY 	2
#define MEDIAN_POPULATE_T2_TASK_STACK 		128

#define MEDIAN_POPULATE_T3_TASK_PRIORITY 	2
#define MEDIAN_POPULATE_T3_TASK_STACK 		128

#define MEDIAN_POPULATE_T4_TASK_PRIORITY 	2
#define MEDIAN_POPULATE_T4_TASK_STACK 		128

//*********************************************************
//********************  ISR's PRIO  ***********************
//*********************************************************
/* Definicion de prioridades de interrupciones de perifericos */
/* Prioridad de la interrupcion 15 es la mas baja, 0 la mas alta */

#endif /*PROJECT_CONFIG_H_*/
