/*******************************************************************************
Definicion de funciones
 *******************************************************************************/

#ifndef _halLora_hpp_
#define _halLora_hpp_

#ifdef __cplusplus
extern "C"{
#endif

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



//Habilita la interrupcion por cambio de flanco ascendente en pin 2 (TXDONE/RXDONE-> DIO0 Lora)
void hal_enableIRQ_RXTXLORA (void);


//Utilizada en ardunio como debugger
void hal_failed (const char *file, u2_t line);  				//Se borra, unicamente para arduino se aplica para


//Inicializa los pines I/O y SPI
void hal_init (void); 					//Configuracion de PINES para comunicacicon digital y SPI con el LORA



#if defined(CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
void hal_TX_RX_en (u1_t estadoLora);						//Se utiliza para hacer seleccionar TX o RX con la version SX1278 del NICERF
#endif


#ifdef __cplusplus
} // extern "C"
#endif

#endif // _hal_hpp_

