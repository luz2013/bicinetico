#include <VirtualWire.h>

//pines
int person = A0;
int reed = 2;
int transmitter = 3;

//valores maximos y minimos de sensor de presencia
int person_min = 350;
int person_max = 450;

//velocidad
volatile byte revolutions;
int rpm;
float speed;
unsigned long timeOld;
long debouncing = 30;
volatile unsigned long timeOld_debounce;

void setup(){
	Serial.begin(9600); //inicializa la comunicacion serial
	Serial.println("Inicializando Bicigerador!");

	//declara pines de entrada
	pinMode(person, INPUT);
	pinMode(reed, INPUT);

	//activa la interrupcion en el pin 2
	attachInterrupt(0, rpm_sum, FALLING);
	//inicializa las variables
	revolutions = 0;
	rpm = 0;
	timeOld = 0;
	timeOld_debounce = 0;

	//comunicacion inalambrica
	vw_set_tx_pin(transmitter);
    vw_setup(2000);	 // Bits por segundo
}

//revisa el sensor de presencia y retorna true cuando hay alguien en la bici
boolean checkForPerson(){
	int sensor_read = analogRead(person);
	if(sensor_read > person_min && sensor_read < person_max){
		return true;
	} else {
		return false;
	}
}

//cuenta el numero de pasos del iman
void rpm_sum(){
	//elimina lecturas repetidas
	if((long)(micros() - timeOld_debounce) >= debouncing * 1000) {
	 	revolutions++;
    	timeOld_debounce = micros();
  	}
}

void loop() {
	if (millis()-timeOld >= 2000){
		detachInterrupt(0);//desabilita la interrupcion
		rpm = revolutions * 60; // convierte la frecuencia a RPM

		//debug
		 Serial.print("RPM: "); 
		 Serial.println(rpm); 
		 Serial.print("Hz: "); 
		 Serial.println(revolutions);

		//sendMsg(byte(checkForPerson()), byte(rpm));
		
 		revolutions = 0; //Reinicia contador
		timeOld = millis(); //actualiza el tiempo actual
		attachInterrupt(0, rpm_sum, FALLING); //habilita las interrupciones
	} 

	byte msg[3];
	msg[1] = byte(checkForPerson());
	msg[2] = rpm;

	vw_send((uint8_t *)msg, 3);
    vw_wait_tx(); // Wait until the whole message is gone	
}