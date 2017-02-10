/*******************************************************************************
 * HAL compatible con arduino para comunicacion con el LORA
 *******************************************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <stdio.h>
#include "../radio.h"
#include "halLora.h"


// Constante de comunicacion por SPI en arduino
static const SPISettings settings(10E6, MSBFIRST, SPI_MODE0);

// -----------------------------------------------------------------------------
//Inicio de configuracion de pines I/O-> NSS, RXTX, RST, DIO0, DIO3
//->NSS
//->RST
//<-DIO0
//<-DIO3
//<-RXTX No se usa
void hal_io_init ()
{
    // NSS, DIO0 y DIO3 SON REQUERIDOS, DIO1 requerido por LoRa, DIO2 por FSK //DIO1/2-> NO son USADOS
    ASSERT(radio_pins.nss != RADIO_UNUSED_PIN);
    ASSERT(radio_pins.dio0 != RADIO_UNUSED_PIN);

#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)                        //DEFINE la version/fabricante del SX1278 de DORJI o SX1272 de SEMTECH
    ASSERT(radio_pins.dio3 != RADIO_UNUSED_PIN);
#endif
#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
    ASSERT(radio_pins.tx_en != RADIO_UNUSED_PIN);   		//Aplica para la version de niceRF
    ASSERT(radio_pins.rx_en != RADIO_UNUSED_PIN);   		//Aplica para la version de niceRF
#endif

    pinMode(radio_pins.nss, OUTPUT);
    if (radio_pins.rxtx != RADIO_UNUSED_PIN)
    {
        pinMode(radio_pins.rxtx, INPUT);
    }
    if (radio_pins.rst != RADIO_UNUSED_PIN)
    {
        pinMode(radio_pins.rst, OUTPUT);
    }
    pinMode(radio_pins.dio0, INPUT);


#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)                        //DEFINE la version/fabricante del SX1278 de DORJI o SX1272 de SEMTECH
    pinMode(radio_pins.dio3, INPUT);
#endif
#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
    pinMode(radio_pins.tx_en, OUTPUT);						//Aplica para la version de niceRF
    pinMode(radio_pins.rx_en, OUTPUT);						//Aplica para la version de niceRF
#endif

}

//Se utiliza para hacer el reset en el Lora. 0/1 -> salida digital. 2-> entrada o pin flotante
void hal_pin_rst (u1_t val) {
    if (radio_pins.rst == RADIO_UNUSED_PIN)
        return;

    if(val == 0 || val == 1) { // drive pin    0->SX1278    1->SX1272
        pinMode(radio_pins.rst, OUTPUT);
        digitalWrite(radio_pins.rst, val);
    }
    else
    { //Entrada o Pin flotante
        pinMode(radio_pins.rst, INPUT);
    }
}
// -----------------------------------------------------------------------------



//Inicializa el SPI en arduino
void hal_spi_init () {
    SPI.begin();
}

//Habilita o no el bus SPI
//->val 0->habilita el bus    /1->Deshabilita el bus
void hal_pin_nss (u1_t val) {
    if (!val)
        SPI.beginTransaction(settings);
    else
        SPI.endTransaction();

    //Serial.println(val?">>":"<<");
    digitalWrite(radio_pins.nss, val);
}


// Hace la tranferencia SPI con el radio. Tranfiere out, recibe/retorna res
//->out
//<-res
u1_t hal_spi (u1_t out) {
    u1_t res = SPI.transfer(out);
    return res;
}

//Hace retardos de ms segun el parametro time_delay
//-> time_delay
void hal_delay(u1_t time_delay)
{
	delay(time_delay);
}


//Se desarrolla la interrupcion por cambio de flanco ascendente por TXDONE o RXDONE
//Detecta si existio o no ErrorCRC comprobando bit activo en DIO3
void hal_ISR_check()
{
	u1_t errorCRC;
    errorCRC =0;

#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)                        //DEFINE la version/fabricante del SX1278 de DORJI o SX1272 de SEMTECH
	if (digitalRead(radio_pins.dio3) == 1)				//detecta error por CRC_ERROR
	{
		errorCRC = 1;
		Serial.print(">>> ERROR <<<");
	}
#endif
	radio_irq_handler(errorCRC);
}


//Habilita la interrupcion por cambio de flanco ascendente en pin 2 (TXDONE/RXDONE-> DIO0 Lora)
void hal_enableIRQ_RXTXLORA ()
{
	u1_t NumeroInterrupcion;
#if defined(TimerOne_h_)       //Define <TimerOne.h>   -> Arduino UNO
	NumeroInterrupcion = 0;								//interrupcion ARDUINO UNO
#endif

#if defined(DueTimer_h)       //#include <DueTimer.h>  -> Arduino DUE
	NumeroInterrupcion = radio_pins.dio0;				////interrupcion ARDUINO DUE
#endif
	attachInterrupt( NumeroInterrupcion, hal_ISR_check, RISING);			//Definicion Interrupcion
}


// -----------------------------------------------------------------------------
#if defined(radio_PRINTF_TO)			//SOLO PARA DEBUGGER
static int uart_putchar (char c, FILE *)
{
    radio_PRINTF_TO.write(c) ;
    return 0 ;
}

void hal_printf_init()
{
    // create a FILE structure to reference our UART output function
    static FILE uartout;
    memset(&uartout, 0, sizeof(uartout));

    // fill in the UART file descriptor with pointer to writer.
    fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);

    // The uart is the standard output device STDOUT.
    stdout = &uartout ;
}
#endif // defined(radio_PRINTF_TO)



//Inicializa los pines I/O y SPI
void hal_init () 					//Configuracion de PINES para comunicacicon digital y SPI con el LORA
{
    hal_io_init();				//Configura los pines I/O control LORA
    hal_spi_init();				//Configura los pines de SPI LORA
}



//Habilitacion de funciones del SX12378 de NICERF
#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
void hal_TX_RX_en (u1_t estadoLora) {						//Se utiliza para hacer seleccionar TX o RX con la version SX1278 del NICERF
    if (estadoLora == TX_EN_OK)
    {
    	digitalWrite(radio_pins.tx_en, 1);
    	digitalWrite(radio_pins.rx_en, 0);
    }
    else if (estadoLora == RX_EN_OK)
    {
    	digitalWrite(radio_pins.tx_en, 0);
    	digitalWrite(radio_pins.rx_en, 1);
    }
    else							//Seleccionado TX_RX_EN_SLEEP
    {
    	digitalWrite(radio_pins.tx_en, 0);
    	digitalWrite(radio_pins.rx_en, 0);
    }
}
#endif








void hal_failed (const char *file, u2_t line)
{
#if defined(radio_FAILURE_TO)
    radio_FAILURE_TO.println("FAILURE ");
    radio_FAILURE_TO.print(file);
    radio_FAILURE_TO.print(':');
    radio_FAILURE_TO.println(line);
    radio_FAILURE_TO.flush();
#endif

    while(1);
}
