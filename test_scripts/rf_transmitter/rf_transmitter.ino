#include <VirtualWire.h>

int transmitter = 4;

//para guardar el mensaje a enviar inalambricamente
unsigned char msg[2] = {0,0}; 

void setup(){
    Serial.begin(9600); //inicializa la comunicacion serial

    //comunicacion inalambrica
    vw_set_tx_pin(transmitter);
    vw_setup(2000);  // Bits por segundo
}

void loop() {

    //envia los datos inalambricamente
    msg[0] = random(255);
    msg[1] = random(255);

    vw_send((uint8_t *)msg, 2);
    vw_wait_tx(); // Wait until the whole message is gone

    delay(1000);
}
