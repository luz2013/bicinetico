int frankies[] = {2,3,4,5,6,7,8,9,10,11,12,13}; //lista con los pines de los frank
int frank_num = 12; //numero de franks 

void setup(){
	//inicializa los pines de salida y los apaga
	for(int i=0; i<frank_num; i++){
		pinMode(frankies[i], OUTPUT);
		digitalWrite(frankies[i], LOW);
	}
}

void loop(){
	for(int i=0; i<frank_num; i++){
		digitalWrite(frankies[i], HIGH);
	}
}

