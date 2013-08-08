/*
Prueba la lectura del reed switch, imprimiendo el estado del reed, y la velocidad
actual.
*/

int reed = 2;

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
volatile unsigned long actualBang =0;
unsigned long timeOld = 0;


void setup() {
	//inicializa la comunicacion serial
	Serial.begin(9600);

	//declara el pin como entrada
	pinMode(reed, INPUT);

	//activa la interrupcion en el pin 2
	attachInterrupt(0, rpm_sum, FALLING);

	//calcula el perimetro
	perimeter = pi*(radius*2);

	//toma el tiempo actual
	initime = millis();
}

//cuenta el numero de pasos del iman
void rpm_sum(){
	//elimina lecturas repetidas
	if((long)(micros() - timeOld_debounce) >= debouncing * 1000) {
		Serial.println("!!! REED ON !!!!"); //avisa que el switch esta encendido

	 	revolutions++; //aumenta las revoluciones
	 	distance += perimeter/sample; //calcula la distancia

    	timeOld_debounce = micros(); //actualiza el tiempo
  	}
}

void loop() {
	//cada segundo
	if(millis() - timeOld >= 1000){
		//calcula la velocidad
		vel = (distance - last_dist)/ 1.0 ;
		
		//imprime por el puerto serial
		Serial.print("distance: ");
		Serial.println(distance - last_dist);	
		Serial.print("velocidad: ");
		Serial.println(vel);
		Serial.print("revoluciones: ");
		Serial.println(revolutions);
		Serial.print("dist: ");
		Serial.println(distance);

		//actualiza las variables
		last_dist = distance;
		timeOld = millis();
	}	
}