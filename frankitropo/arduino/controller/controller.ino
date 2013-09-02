#include <VirtualWire.h>

int pot = A0; //pin del potenciometro
int frankies[] = {2,3,4,5,6,7,8,9,10,11,12,13}; //lista con los pines de los frank
int frank_num = 12; //numero de franks 
int state = LOW; //estado del LED
long preMillis = 0; //contador de tiempo
long mainCounter_prev = 0;
int machineState = 0;
int maxMachineState = 1;
int counter = 0; //cual frank enciende

//variables para comunicacion inalambrica
int receiver = 1;
int someone = 0;
int velocity = 0;
int vel_min = 0;
int vel_max = 255;
int change_percentage = 50; 

void setup(){
	//inicializa los pines de salida y los apaga
	for(int i=0; i<frank_num; i++){
		pinMode(frankies[i], OUTPUT);
		digitalWrite(frankies[i], LOW);
	}

	//setup de comunicacion inalambrica
	vw_set_rx_pin(receiver);   
	vw_setup(2000); // Bits per sec
	vw_rx_start(); // Start the receiver PLL running
}

//funcion que controla el encendido y apagado
void flash(int interval, int mode){
	long curMillis = millis();
	if(curMillis-preMillis > interval){
		preMillis = curMillis;

		if(state == LOW){
			state = HIGH;
			//cambia los modos de ejecucion del contador
			switch (mode) {
				//Cuenta hacia adelante
			    case 1:
			      	counter++;
					if(counter >= frank_num){
						counter = 0;
					}
			      break;
			    //Cuenta hacia atras
			    case 2:
			      	counter--;
					if(counter <= 0){
						counter = frank_num;
					}
			      break;
			    //Aleatorio
			    case 3:
			      counter = random(frank_num);
			      break;
			}
		} else {
			state = LOW;
		}
		digitalWrite(frankies[counter], state);
	}		
}


//enciende todos los pines
void allOn(){
	for(int i=0; i<frank_num; i++){
		digitalWrite(frankies[i], HIGH);
	}
}

//apaga todos los pines
void allOff(){
	for(int i=0; i<frank_num; i++){
		digitalWrite(frankies[i], LOW);
	}	
}


void loop(){
	//lee el potenciometro
	int pot_val = analogRead(pot);
	//convierte el valor del potenciometro en tiempo
	int spd = map(pot_val,0, 1023, 50, 800);

	//comunicacion inalambrica
	uint8_t buf[VW_MAX_MESSAGE_LEN];
	uint8_t buflen = VW_MAX_MESSAGE_LEN;
	if (vw_get_message(buf, &buflen)){
		someone = buf[0];
		velocity = buf[1];
	}

	//Si hay alguien en la bici
	if(someone == 1){
		//Enciende los franks dependiendo de la lectura del pot
		if(pot_val==0){
			allOn(); //los enciende todos
		} else if (pot_val==1023){
			allOff(); // los apaga todos		
		} else {
			//toma el tiempo actual 
			long mainCounter = millis();
			//Calculos para cambiar la velocidad de encendido
			//saca X % del valor de la velocidad dada por el potenciometro
			int percentage = (spd*change_percentage)/100;
			//Suma y resta el porcentaje a la lectura del potenciometro
			int spd_min = spd-percentage;
			int spd_max = spd+percentage;
			
			//Cambia la escala de la velocidad de acuerdo al dato que llega
			//por via inalambrica, y los valores actuales de modificacion
			int new_spd = map(velocity, vel_min, vel_max, spd_min, spd_max);
			
			// Contador para cambiar la maquina de estados
			// cambia cada 10 segundos
			if(mainCounter - mainCounter_prev > 10000){
				mainCounter_prev = mainCounter;
			   	machineState++;
				
				if(machineState > maxMachineState){
			        machineState = 0;
			    }   
			} 

			//Maqina de estados que controla el modo de parpadeo
			switch (machineState) {
			     case 1:
			       flash(new_spd, 1); //invoca el modo de parpadeo a la derecha
			       break;
			     case 2:
			       flash(new_spd, 2); //invoca el modo de parpadeo inverso
			       break;
			     case 3:
			     	flash(new_spd,3); //invoca el modo de parpadeo aleatorio
			     	break;
			 } 
		}	
	} else { //Si no hay nadie en la bici
		allOff(); //apaga todos los franks
	}
}

