#include <VirtualWire.h>

int projectors[] = {2,3,4,5,6,7,8};
int projectors_num = 7;

int receiver = 22;
int someone = 0;
int velocity = 0;

void setup() {
    //inicializa los pines de salida y los apaga
    for(int i=0; i<projectors_num; i++){
        pinMode(projectors[i], OUTPUT);
        digitalWrite(projectors[i], LOW);
    }

    //setup de comunicacion inalambrica
    vw_set_rx_pin(receiver);   
    vw_setup(2000); // Bits per sec
    vw_rx_start(); // Start the receiver PLL running  
}

void loop() {
    //comunicacion inalambrica
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)){
        someone = buf[0];
        velocity = buf[1];
    }

    if(someone == 1){
        for(int i=0; i<projectors_num; i++){
            analogWrite(projectors[i], velocity);
        }  
    }
}