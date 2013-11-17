/*

OBSERVACIONES

Aun hace falta incluir los 2 leds extra que indican velocidad 
baja o velocidad alta, no los vi en el circito

Esta biblioteca utiliza para mandar los datos el ISR, asi que 
hay que verificar que no se traslapen las Interrupciones. 
Si nada funciona esa puede ser la explicacion. Se sugiere 
mandar los datos via serial a travez del pin 4 en caso 
de que la comunicacion rf sea analogca. En caso de ser digital 
se sugiere cambiar la definicion de la interrupcion que calcula 
la velocidad.

Al el programa del frankitropo se manda el tiempo que hay 
entre cada interrupcion provocada por el iman dado el radio. 
De necesitar la distacia  se puede calcular como en el 
programa anterior y hacer un map para mandar el dato al 
frankitropo, pero se recomienda que este calculo se haga 
en el franquitropo

Verificar la conversion de datos en ambos programas, en el 
anterior se mandan datos float a una variable tipo uint8_f, 
mientras que en este se manda una unsigned long. Dado que el 
tiempo entre deteccion de cada iman es un numero menor a 100 la 
transmision deberia hacerse correctamente o en su defecto ajustar para
mandar el primer nible.

si el numero mandado (vel_time) es muy corto o se considera que 
tiene baja resolucion, se puede modificar cada una de las 
funciones millis () por micros () con la diferencia de que 
twentysecs debe reasignarse a 20 000 000 en lugar de 20 000 y 
de que sera necesario mandar el primer byte para la conversion 
de datos

*/

//Bibliotecas
#include <VirtualWire.h>

//Pines. Constantes
const int iredpin = A0;//Sensor de presencia
const int led1 = 3;//Led indicador de funcionamiento
const int led2 = 5;
const int led3 = 6;
const int relay_carga = 7;//relevador de carga de bateria
const int relay_franki = 8;//Relevador de activacion del frankitropo
const int tx = 4;//Pin de transmision

//Valores. Constantes
const int too_close = 100;//Si se sienta muy cerca
const int too_far = 900;//Si se sienta muy lejos
const unsigned long vel = 600;//tiempo entre interrupciones
const unsigned long twentysecs = 1000;//20 segundos para prender franki

//Timers-RAM
volatile unsigned long vel_time = 0;
volatile unsigned long vel_time_c = 0;
volatile unsigned long veinte = 0;
volatile unsigned long veinte_off = 0;

//Banderas-RAM
volatile byte waiter = false;//Indica si estamos contando
volatile byte franki = false;//indica si el frankitropo esta encendido

//Datos
int someone = 0;//Indica si hay alguien, inicialmente no hay nadie, debe ser int para enviarse uint8_t
unsigned char msg[2] = {0,0};

int sensor_ired = 500;

void setup ()
{
  //Inicia comunicacion serial
  Serial.begin (19200);//debug
  
  Serial.println ("Inicializando Bicigenerador!");//debug
  
  //Pinout
  pinMode(led1, OUTPUT);
  pinMode(relay_carga, OUTPUT);
  pinMode(relay_franki, OUTPUT);
  //Interrupciones
  attachInterrupt (0, velocidad, FALLING);
  //condiciones iniciales
  digitalWrite (led1, LOW);
  digitalWrite (relay_carga, LOW);
  digitalWrite (relay_franki, LOW);
  //Se incializa la transmision IMPORTANTE: SE USAN INTERRUPCIONES, VERIFICAR TRASLAPO DE PINES
  //vw_set_tx_pin (tx);
  //vw_setup(2000);
  //Se inicia el marcador de tiempo
  millis();
}

void loop ()
{
  //Primera condicion, que haya alguien sentado
 // int sensor_ired = analogRead (iredpin);//verificar lo que hay en el sensor

  
  if (sensor_ired >= too_close && sensor_ired <= too_far)//Verificamos que este cerca
  {
    someone = 1;//indicamos que hay alguien

    //Serial.println("Se ha detectado a alguien");//debug
    
    //Si la velocidad es buena iniciar la carga y esperar 20 segundos para alimentar el franki
    if (vel_time <= vel)//Si hay buena velocidad
    {

      Serial.println("Buena Velocidad!! ");

      //Secuencia de encendido del frankitropo, 20segundos
      if (waiter == false)//Si la velocidad no era suficiente iniciar la cuenta de encendido del frankitropo
      {
        Serial.println("encendiendo primer relay");

        //Activar secuencia de carga de bateria
        digitalWrite (led1, HIGH);//Encender led
        digitalWrite (relay_carga, HIGH);//Activar carga de bateria
        veinte = millis ();//Inicia cuenta de encendido
        waiter = true;//Activar cuenta solo una vez
      }
           
      if (millis () - veinte >= twentysecs && franki == false)//Solo si se ha superado la cuenta
      {
        digitalWrite(led2, HIGH);
        digitalWrite (relay_franki, HIGH);//Se activa el frankitropo
        Serial.println("ya se debio haber prendido el segundo rele");
        franki = true;
      } else {
        //Esperar
        Serial.print("Esperando 20:  ");
        Serial.println( millis() -veinte);
      }
    }
    else//Si la velocidad no es buena
    {
      Serial.println("Velocidad no es Buena, apaga relays");
      
      Serial.print("Velocidad ");  //debug
      Serial.println(vel_time);

      //Detener secuencia de carga de bateria
      digitalWrite (led1, LOW);//Se apaga el led
      digitalWrite (led2, LOW);
      digitalWrite (relay_carga, LOW);//Se apaga el relay de carga
      //Secuencia de apagagado del frankitropo, 20 segundos
      if (waiter == true)//Si la velocidad era suficiente iniciar, la cuenta de apagado del frankitropo
      {
        waiter = false;//Activar la cuenta solo una vez
        veinte_off = millis ();//Iniciar la cuenta de apagado
      }
      //Espera colocada fuera del if de presencia para asegurar apagado
    }
  }
  else//si nadie se sienta en el bicigenerador
  {
    
    Serial.println("Bicigenerador vacio");//debug
    
    //Si habia alguien, apagar todos los sistemas
    if (someone == 1)//Evitamos escribir a cada rato
    {
      someone = 0;//Indicar que no hay nadie sentado
      waiter = false;//Desactivar contador para frankitropo
      digitalWrite (led1, LOW);//Apagar led 1 por si estaba encendido
      digitalWrite (led2, LOW);
      digitalWrite (relay_carga, LOW);//Detener la carga de la bateria
/*    
      //Esta exepcion es una propuesta para corregir mal funcionamiento
      //provocado por si se bajan del bicigenerador muy rapido o por si
      //deja de detectar a la persona mientras la velocidad es optima
      if (waiter == true);
      {
        veinte_off = millis ();
      }
*/
    }
  }
  //Espera de apagado
  if (millis () - veinte_off >= twentysecs && franki == true)
  {
    digitalWrite (relay_franki, LOW);//Apagar frankitropo
    franki = false;//Bandera de frankitropo apagado
  }
  //envia los datos inalambricamente
 /* msg[0] = someone;//Se define lo que se va a mandar
  msg[1] = vel_time;//Se define lo que se va a mandar
  vw_send((uint8_t *)msg, 2);//Error de conversion en el ejemplo vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx();//Esperar que se mande el mensaje
  delay (200);*/
}

//Definir la velocidad
void velocidad ()
{
  vel_time = millis () - vel_time_c;//Se define por el tiempo entre cada interrupcion
  vel_time_c = millis ();
}
