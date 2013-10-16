#include <VirtualWire.h>

/*
    Imprime por el puerto serial los datos que recibe via RF
*/

//variables para comunicacion inalambrica
int receiver = 1;

void setup(){
    Serial.begin(9600);

    //setup de comunicacion inalambrica
    vw_set_rx_pin(receiver);   
    vw_setup(2000); // Bits per sec
    vw_rx_start(); // Start the receiver PLL running
}


void loop(){

    //comunicacion inalambrica
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)){
        Serial.println("Alguien: " + buf[0]);
        Serial.println("Velocidad: "+ buf[1]);
    }

}

