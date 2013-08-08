//pines
int person = A0;
int reed = 2; //sensor de presencia

int led1 = 3; //leds de indicacion de velocidad
int led2 = 5;
int led3 = 6;

int relay = 4; //relay para el alternador


//valores maximos y minimos de sensor de presencia
int person_min = 350;
int person_max = 450;

int vel_alternador = 120; //velocidad para conectar el alternador

//velocidad
volatile byte revolutions = 0; //guarda los giros
int rpm =0; //revoluciones por minuto

long debouncing = 20; //limpia lectura del reed
volatile unsigned long timeOld_debounce = 0; //ibid

//variables para los calculos
int radius = 26; //radio de la bici
float pi = 3.1416; //PI
int sample = 4; //cuantos imanes hay en la bici
int perimeter;

int distance = 0; //cuanta distancia se ha recorrido
int last_dist = 0;
int initime; //tiempo inicial
float vel; //velocidad

//para el calculo de tiempo
volatile unsigned long actualBang = 0;
unsigned long timeOld = 0;

void setup(){
	Serial.begin(9600); //inicializa la comunicacion serial
	Serial.println("Inicializando Bicigerador!");

	//declara pines de entrada
	pinMode(person, INPUT);
	pinMode(reed, INPUT);

	//declara pines de salida
	pinMode(led1, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led3, OUTPUT);
	pinMode(relay, OUTPUT);

	//activa la interrupcion en el pin 2
	attachInterrupt(0, rpm_sum, FALLING);

	//calcula el perimetro
	perimeter = pi*(radius*2);

	//toma el tiempo de inicio
	initime = millis();
}

//cuenta el numero de pasos del iman
void rpm_sum(){
	//elimina lecturas repetidas
	if((long)(micros() - timeOld_debounce) >= debouncing * 1000) {
	 	revolutions++; //aumenta las revoluciones
	 	distance += perimeter/sample; //calcula la distancia

    	timeOld_debounce = micros(); //actualiza el tiempo
  	}
}

void loop() {
	int person_read = analogRead(person); // lee el sensor de presencia
	
	//si hay alguien en la bici
	if(person_read >= person_max && person_read <= person_max){
		//calcula la velocidad
		if(millis() - timeOld >= 1000){
			vel = (distance - last_dist)/ 1.0 ;
			
			last_dist = distance;
			timeOld = millis();
		}

		//si la velocidad es buena, conecta el alternador para cargar
		//enciende el led
		if(vel >= vel_alternador){
			digitalWrite(led1, HIGH);
			digitalWrite(relay, HIGH);
		} else {
			digitalWrite(led1, LOW);
			digitalWrite(relay, LOW);
		}

		// indica diferentes niveles de velocidad
		if(vel > 160.0){
			digitalWrite(led2, HIGH);
		} else {
			digitalWrite(led2, LOW);
		}

		//ibid
		if(vel > 200.0){
			digitalWrite(led3, HIGH);
		} else {
			digitalWrite(led3, LOW);
		}
	} else {
		// si no hay nadie en la bici
		// resetea las variables
		vel = 0;
		distance = 0;

		// apaga el relay
		digitalWrite(relay, LOW);
	}
}