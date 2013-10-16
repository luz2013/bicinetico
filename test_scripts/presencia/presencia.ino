/*
imprime por el puerto serial los datos recibidos por el sensor de presencia
*/

//Donde esta el sensor
int sensor = A0;

void setup() {
	//inicia la comunicacion serial
	Serial.begin(9600);

	//declara el pin de entrada
	pinMode(sensor, INPUT);
}

void loop() {
	//hace la lectura analoga
	int lectura = analogRead(sensor);

	//imprime los datos por el puerto serial
	Serial.print("El sensor: ");
	Serial.println(lectura);
}
