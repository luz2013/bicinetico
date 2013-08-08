/*
Este programa es para calibrar el sensor de presencia en los bicigeneradores.
Una vez obtenidos los datos minimos y maximos, se deben agregar al programa
'controller.ino' en las respectivas variables
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