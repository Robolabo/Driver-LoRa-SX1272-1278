/*******************************************************************************
 * HAL compatible con arduino para comunicacion con el LORA
 *******************************************************************************/


#include <stdio.h>

#include "halLora.h"
#include "radio.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//Inicio de configuracion de pines I/O-> NSS, RXTX, RST, DIO0, DIO3
//->NSS
//->RST
//<-DIO0
//<-DIO3
//<-RXTX No se usa
void hal_io_init ()
{
	//Configuracion de pines de comuncionacion para el LoRa
	/* Create structs */
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	/* Init structs */
	GPIO_StructInit(&GPIO_InitStructure);
	/*NVIC_StructInit(&NVIC_InitStructure);*/
	EXTI_StructInit(&EXTI_InitStructure);
	
	/* Activate Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	/* Configure RST as OUTPUT */
	GPIO_InitStructure.GPIO_Pin 	= LORA_RST_PIN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= LORA_RST_MODE;
	GPIO_Init(LORA_RST_PORT, &GPIO_InitStructure);
	

#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)                        //DEFINE la version/fabricante del SX1278 de DORJI o SX1272 de SEMTECH

	// Configure DIO3 as INPUT
	GPIO_InitStructure.GPIO_Pin 	= LORA_DIO3_PIN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= LORA_DIO3_MODE;
	GPIO_Init(LORA_DIO3_PORT, &GPIO_InitStructure);
#endif

#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF

	/* Configure TXEN as OUTPUT */
	GPIO_InitStructure.GPIO_Pin 	= LORA_TXEN_PIN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= LORA_TXEN_MODE;
	GPIO_Init(LORA_TXEN_PORT, &GPIO_InitStructure);

	/* Configure RXEN as OUTPUT */
	GPIO_InitStructure.GPIO_Pin 	= LORA_RXEN_PIN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= LORA_RXEN_MODE;
	GPIO_Init(LORA_RXEN_PORT, &GPIO_InitStructure);
#endif



	
	/* Configure DIO0 as INPUT/INTERRUPT */
	GPIO_InitStructure.GPIO_Pin 	= LORA_DIO0_PIN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	= LORA_DIO0_MODE;
	GPIO_Init(LORA_DIO0_PORT, &GPIO_InitStructure);

	/* Interrupt on DIO0 */	
	GPIO_EXTILineConfig(LORA_DIO0_PORT_SOURCE, LORA_DIO0_PIN_SOURCE);

	EXTI_InitStructure.EXTI_Line 		= LORA_DIO0_EXTI;
	EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger 	= EXTI_Trigger_Rising; //EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd 	= ENABLE;	// It is initially disable
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel 	= EXTI9_5_IRQn; //External Line[15:10] Interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = LORA_DIO0_ISR_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //No importa, estamos en el grupo 4
	NVIC_InitStructure.NVIC_IRQChannelCmd 		  = ENABLE;
	NVIC_Init(&NVIC_InitStructure);



}

//Se utiliza para hacer el reset en el Lora. 0/1 -> salida digital. 2-> entrada o pin flotante
void hal_pin_rst (uint8_t val) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure RST as OUTPUT */
	GPIO_InitStructure.GPIO_Pin 	= LORA_RST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	if(val == 0 || val == 1)			// drive pin    0->SX1278    1->SX1272
	{
		GPIO_InitStructure.GPIO_Mode 	= LORA_RST_MODE;
		GPIO_Init(LORA_RST_PORT, &GPIO_InitStructure);
		HW_PinWrite(LORA_RST, val);
	}
	else
	{
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING ;
		GPIO_Init(LORA_RST_PORT, &GPIO_InitStructure);
	}
}

//Utilizada para configurar los pines de SPI
void hal_spi_init ()
{

	/* Create TypeDef vars */
	/* Create GPIOs struct */
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Create NVICs struct */
	/*NVIC_InitTypeDef NVIC_InitStructure;*/
	/* Create SPIs struct */
	SPI_InitTypeDef SPI_InitStructure;
	/* Create EXTIs struct */
	/*EXTI_InitTypeDef EXTI_InitStructure;*/

	/* Init TypeDefs */
	/* Init GPIOs struct */
	GPIO_StructInit(&GPIO_InitStructure);
	/* Init NVICs struct */
	/*NVIC_StructInit(&NVIC_InitStructure);*/
	/* Init SPIs struct */
	SPI_StructInit(&SPI_InitStructure);
	/* Init EXTIs struct */
	/*EXTI_StructInit(&EXTI_InitStructure);*/


	/* Activate Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* Config GPIOs */
	/* Configure SCK as Alternative Function PP */
	GPIO_InitStructure.GPIO_Pin = LORA_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(LORA_SPI_PORT, &GPIO_InitStructure);

	/* Configure MOSI as Alternative Function PP */
	GPIO_InitStructure.GPIO_Pin = LORA_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(LORA_SPI_PORT, &GPIO_InitStructure);
	
	/* Configure NSS as Alternative Function PP */
	GPIO_InitStructure.GPIO_Pin = LORA_NSS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LORA_SPI_PORT, &GPIO_InitStructure);
	
	/* Configure MISO as In Floating */
	GPIO_InitStructure.GPIO_Pin = LORA_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	/*GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;*/
	GPIO_Init(LORA_SPI_PORT, &GPIO_InitStructure);

	/* Config SPI */

	/* Config SPI Interrupt */
	/*NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);*/

	// Configure and enable SPI_MASTER interrupt -------------------------------
	/*NVIC_InitStructure.NVIC_IRQChannel = LORA_SPI_IRQn;*/
	/*NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;*/
	/*NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;*/
	/*NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;*/
	/*NVIC_Init(&NVIC_InitStructure);*/

	/* Config SPI mode */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	/*SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;*/
	/*SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(LORA_SPI, &SPI_InitStructure);

	hal_pin_nss(1);					//NSS disable

	/*SPI_BiDirectionalLineConfig(LORA_SPI, SPI_Direction_Tx); */
	/*SPI_I2S_ITConfig(LORA_SPI, SPI_I2S_IT_TXNE, ENABLE);*/
	/*SPI_I2S_ITConfig(LORA_SPI, SPI_I2S_IT_RXNE, ENABLE);*/
	SPI_Cmd(LORA_SPI, ENABLE);


}

//Habilita o no el bus SPI
//->val 0->habilita el bus    /1->Deshabilita el bus
void hal_pin_nss (uint8_t val) {
	HW_PinWrite(LORA_NSS, val);
}



// Hace la tranferencia SPI con el radio. Tranfiere out, recibe/retorna res
//->out
//<-res
uint8_t hal_spi (uint8_t out) {

	uint8_t res;		
/* Write communication register to allow a reading of the full-scale reg */
		while (SPI_I2S_GetFlagStatus(LORA_SPI, SPI_I2S_FLAG_TXE) == RESET);
		SPI_I2S_SendData(LORA_SPI, out);
		while (SPI_I2S_GetFlagStatus(LORA_SPI, SPI_I2S_FLAG_BSY) == SET);

		/* Clear trash in RX buffer*/	
		while (SPI_I2S_GetFlagStatus(LORA_SPI, SPI_I2S_FLAG_RXNE) == RESET);
		/*SPI_I2S_ReceiveData(LORA_SPI);*/
		/* A dummy write must be done in order to generate the clock and 
		 * allow the slave to put data in the MISO */
		/*while (SPI_I2S_GetFlagStatus(LORA_SPI, SPI_I2S_FLAG_TXE) == RESET);*/
		/*SPI_I2S_SendData(LORA_SPI, 0x00);*/
		/*while (SPI_I2S_GetFlagStatus(LORA_SPI, SPI_I2S_FLAG_RXNE) == RESET);*/
		res = SPI_I2S_ReceiveData(LORA_SPI);
	return res;
}

//Hace retardos de ms segun el parametro time_delay
//-> time_delay-> crea una tarea
void hal_delay(uint8_t time_delay)
{
	vTaskDelay(time_delay);
}


//Se desarrolla la interrupcion por cambio de flanco ascendente por TXDONE o RXDONE
//Detecta si existio o no ErrorCRC comprobando bit activo en DIO3
void hal_ISR_check()				//en esta funcion se desarrollara la interrupcion
{
    uint8_t errorCRC = 0;

#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)                        //DEFINE la version/fabricante del SX1278 de DORJI o SX1272 de SEMTECH

	if (HW_PinRead(LORA_DIO3) == 1 )
	{
		errorCRC = 1;
	}
#endif

	radio_irq_handler(errorCRC);
}



//Inicializa los pines I/O y SPI
void hal_init () 					//Configuracion de PINES para comunicaicon digital y SPI con el LORA
{
    hal_io_init();				//Configura los pines I/O control LORA
    hal_spi_init();				//Configura los pines de SPI LORA
}


//Habilitacion de funciones del SX12378 de NICERF
#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
void hal_TX_RX_en (u1_t estadoLora) {						//Se utiliza para hacer seleccionar TX o RX con la version SX1278 del NICERF
    if (estadoLora == TX_EN_OK)
    {
		HW_PinWrite(LORA_TXEN, 1);
		HW_PinWrite(LORA_RXEN, 0);
    }
    else if (estadoLora == RX_EN_OK)
    {
		HW_PinWrite(LORA_TXEN, 0);
		HW_PinWrite(LORA_RXEN, 1);
    }
    else							//Seleccionado TX_RX_EN_SLEEP
    {
		HW_PinWrite(LORA_TXEN, 0);
		HW_PinWrite(LORA_RXEN, 0);
    }
}
#endif



void EXTI9_5_IRQHandler(void)
{
	/*portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;*/

	/*// Vamos a comenzar a recibir un nuevo dato: reinicio el contador de bytes recibidos*/
	/**//*EXTADC_RestartNumBytes();*/

	/*// Desactivo la interrupción de DRDY*/
	/*ExtADC_DisableDRDYInt();*/

	/*// Activo el SPI*/
	/*EXT_ADC_SPI->DR = 0;*/
	/*SPI_I2S_ReceiveData(EXT_ADC_SPI);*/
	/*SPI_I2S_ClearITPendingBit(EXT_ADC_SPI, SPI_IT_CRCERR | SPI_I2S_IT_OVR);*/

	/**//*ExtADC_SpiOn();*/

	/* Toggle Led 0 */
	HW_PinWrite(LED_H0, (BitAction)(1 - HW_PinRead(LED_H0)));
	hal_ISR_check();
	/* limpio la interrupcion */
	EXTI_ClearFlag(EXTI_Line5);

	/*//Flag interrupt, should be changed by a Semaphore*/
	/*uExtADCReadyFlag = TRUE;*/

	/**//* Now the buffer is empty we can switch context if necessary. */
	/*if( xHigherPriorityTaskWoken ){*/
	/*portYIELD();*/
	/*}*/
	
}
