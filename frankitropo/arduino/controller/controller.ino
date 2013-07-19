int pot = A0; //pin del potenciometro
int frankies[] = {2,3,4,5,7,8,9,10,11}; //lista con los pines de los frank
int frank_num = 9; //numero de franks 
int state = LOW; //estado del LED
long preMillis = 0; //contador de tiempo
int counter = 0; //cual frank enciende

void setup(){
	Serial.begin(9600); //inicia la comunicacion serial

	//inicializa los pines de salida y los apaga
	for(int i=0; i<frank_num; i++){
		pinMode(frankies[i], OUTPUT);
		digitalWrite(frankies[i], LOW);
	}
}

//funcion que controla el encen
void flash(int interval){
	long curMillis = millis();
	if(curMillis-preMillis > interval){
		preMillis = curMillis;

		if(state == LOW){
			state = HIGH;
			counter++;
			if(counter > frank_num-1){
				counter = 0;
			}
		} else {
			state = LOW;
		}
		digitalWrite(frankies[counter], state);
	}	
			
}

void loop(){
	int pot_val = analogRead(pot);
	//TODO and vel range
	int spd = map(pot_val,0, 1023, 50, 800);
	Serial.println(spd);
	flash(spd);
}

