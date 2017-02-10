/*******************************************************************************
 * Configuracion de pines en arduino
 *******************************************************************************/
#ifndef _hal_halLora_h_
#define _hal_halLora_h_



struct radio_pinmap {
    u1_t nss;
    u1_t rxtx;
    u1_t rst;
    u1_t dio0;				//

#if (defined CFG_1278_DRF1278F)  ||  (defined CFG_1272_SEMTECH)                        //DEFINE la version/fabricante del SX1278 de DORJI o SX1272 de SEMTECH
    u1_t dio3;
#endif

#if defined (CFG_1278_NICERF1278)                          //Selecciona el CHIP SX1278 de NICERF
    u1_t tx_en;				//Aplica para la version de niceRF
    u1_t rx_en;				//Aplica para la version de niceRF
#endif
};

//utilizado para no definir pines
const u1_t RADIO_UNUSED_PIN = 0xff;

//Se hace la declaracion de los pines
extern const radio_pinmap radio_pins;

#endif // _hal_hal_h_
