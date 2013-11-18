const int interval = 10; //controla el tiempo del fade

//pines
const int pir = 22;
const int projectors[] = {2,3,4,5,6,7,8};
const int projectors_num = 7;

//flags
boolean fadingIn = false;
boolean fadeInDone = false;
boolean fadingOut = false;
boolean fadeOutDone = false;

//tiempo y valor de los leds
long time = 0;
int value = 0;


void setup() { 
  Serial.begin(9600);
  Serial.println("Inicializando MAR");
  
  //inicializa los pines de salida y los apaga
  for(int i=0; i<projectors_num; i++){
      pinMode(projectors[i], OUTPUT);
      digitalWrite(projectors[i], LOW);
  }
  
  pinMode(pir, INPUT);
  millis();
}

//funcion para escribir el valor actual en los pines
void writeValue(int val){
  for(int i=0; i<projectors_num; i++){
      analogWrite(projectors[i], val);
  }
}

void loop() { 
  int someone = digitalRead(3);
  
  if(someone == LOW){ // hay alguien
    
    if(!fadingIn && !fadeInDone){
      time = millis(); //toma el tiempo
      fadingIn = true; //inicia el fadein
      value = 0;
      Serial.println("Inicia fadeIn");
    }
    
    if(!fadeInDone && fadingIn){ //Hace el fade
      if(millis() - time > interval){ //aumenta el valor
        time = millis();
        value++;    
      }

      if(value >= 255){ //termina el fade y conserva value en rango
        Serial.println("fadeIn terminado");
        fadingIn = false;
        fadeInDone = true;
        
        value = 255;
      } else if (value < 0){
        value = 0;
      }
    }
    
    if(fadeInDone){ //mantiene los leds encendidos
       value = 255;
    }
    
    writeValue(value);
    
    //cambia el estado de las banderas
    fadingOut = false;
    fadeOutDone = false;
    
  } else if (someone == HIGH){
    //No hay nadie
     
    //cambia el estado para comenzar de nuevo
    fadeInDone = false;
    fadingIn = false;
    
    if(!fadingOut && !fadeOutDone){
      Serial.println("no hay nadie, iniciando fadeOut");
      time = millis(); //toma el tiempo
      fadingOut = true; //inicia fadeOut
    }
    
    if(fadingOut && !fadeOutDone){
      if(millis() - time > interval){
        time = millis();
        value--;    
      }

      if(value <= 0){
        Serial.println("fade out terminado");
        value = 0;
        fadingOut = false;
      } else if (value >= 255){
        value = 255;
      }
    }
    
    if(fadeOutDone){ //mantiene los leds encendidos
       value = 255;
    }   
    
    writeValue(value);
  }  
}
