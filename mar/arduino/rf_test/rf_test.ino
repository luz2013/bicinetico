#include <VirtualWire.h>

//pines
int pote = A0;
unsigned char msg[2] = {0,0}; 
int transmitter = 4;

void setup(){
    Serial.begin(9600); //inicializa la comunicacion serial
    Serial.println("Inicializando!");

    //comunicacion inalambrica
    vw_set_tx_pin(transmitter);
    vw_setup(2000);  // Bits por segundo
}

void loop() {
    int pote_read = analogRead(pote); // lee el sensor de presencia
    

    //envia los datos inalambricamente
    msg[0] = 1;
    msg[1] = pote_read/4;

    vw_send((uint8_t *)msg, 2);
    vw_wait_tx(); // Wait until the whole message is gone
}