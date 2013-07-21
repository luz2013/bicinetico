//pines
int person = A0;
int reed = 2;

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

//calcula las revoluciones por minuto
void measure_rpm(){
	//cada segundo == 1hz
	if (millis()-timeOld == 1000){
		detachInterrupt(0);//desabilita la interrupcion
		rpm = revolutions * 60.0; // convierte la frecuencia a RPM

		//debug
		// Serial.print("RPM: "); 
		// Serial.println(rpm); 
		// Serial.print("Hz: "); 
		// Serial.println(revolutions);

 		revolutions = 0; //Reinicia contador
		timeOld = millis(); //actualiza el tiempo actual
		attachInterrupt(0, rpm_sum, FALLING); //habilita las interrupciones
	}
}

void loop() {
	boolean someone = checkForPerson();

	if(someone){
		//Serial.println("alguien esta en la bici");
		//Serial.println(rpm);
	}

	measure_rpm();
	
}