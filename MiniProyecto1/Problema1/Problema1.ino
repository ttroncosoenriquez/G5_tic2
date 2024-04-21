// Funcion que me deja limpiar el puerto serial despues de correr el codigo
  void clearSerialBuffer() {
  while (Serial.available()) {
    Serial.read(); 
  }
  }
//Definimos los pins de la placa y a que estan conectados:

#include "pitches.h"

//Leds:
 #define ledPin1 2
 #define ledPin2 4
#define ledPin3 6
//Botones:
#define botonPin1  3
#define botonPin2  5
#define botonPin3  7
//Buzzers:
#define buzzerPin 8
int buzzer = buzzerPin;
// Variables de juego

int puntaje = 50;        // Puntuación inicial
int tiempoRonda = 5000;  // Tiempo de cada ronda (milisegundos)
int duracionTopo = 1000; // Tiempo que transcurre entre encendido y apagado de cada LED (milisegundos)
int golpes = 0;           // Número de golpes exitosos
int fallos = 0;          // Número de golpes errados
int dificultad = 1;      // Nivel de dificultad inicial

int pTotal = 50; // Puntuación sin resta


void setup() {
  // put your setup code here, to run once:

//Define que hace 
pinMode(ledPin1,OUTPUT);
pinMode(ledPin2,OUTPUT);
pinMode(ledPin3,OUTPUT);

pinMode(botonPin1, INPUT_PULLUP);
pinMode(botonPin2, INPUT_PULLUP);
pinMode(botonPin3, INPUT_PULLUP);

pinMode(buzzerPin, OUTPUT);


 delay(1000);
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  clearSerialBuffer(); //Limpia cualquier data que quede en el puerto serial




  Serial.print("\n\r Bienvenido al increible Juego de pegarle a los topos! \r\n \r\n");


  Serial.print("Primero que nada, ingresa una dificultad para el juego del 1 al 9\r\n Eso si, mientras juegues la dificultad va a ir aumentando y mas alla del 9");
  while (!Serial.available()  ) {
    // Hace que el codigo quede esperando a una respuesta por consola
  }


  // 
  if (Serial.available()) {
    // Aqui lee la dificultad ingresada 
    char entrada = Serial.read();
    int dificultad =  atoi(&entrada);

    Serial.print("\n\r e ingresaste: ");
    Serial.println(dificultad);

    if(dificultad > 9 || dificultad < 1 ){
      Serial.print("\n\r Dije que ingresaras un numero entre 1 y 9, por pesado te la pondre en 11\r\n \r\n");
      dificultad = 11;
    }

  } 

 while(puntaje > 0) {
    int tEncendido = 3000/sqrt(dificultad);
    int tRonda =   10*tEncendido;
    int delayBotones = 0.33*tEncendido;
    // Tiempo de duración de las rondas y cuanto esta encendido el led, talque cada ronda se encienden aproximadamente 25 veces los botones, ademas hay un tiempo de espera 
    // desde que presione un boton a que se encienda otro de 1/3 la duración del boton



    unsigned long tInicial = millis(); //Cuando empieza el Loop de la ronda
    while ((millis() - tInicial) < tRonda) {
      
      int cBoton = random(1, 4); //Elije uno de los 3 leds
      int cLed   = 2*cBoton; // ELije uno de los 3 botones
      int oldVal1 = digitalRead(3); //Define el estado actual del boton a usar
      int oldVal2 = digitalRead(5);
      int oldVal3 = digitalRead(7);

      bool vB1 = false; // La variable que nos va a decir si el boton fue presionado correctamente.
      bool vB2 = false; 
      bool vB3 = false; 

      // esta parte del codigo decide cual va a ser el boton a prender y 





      digitalWrite(cLed, HIGH);
      unsigned long tBoton = millis(); //registra cuanto tiempo tiene que estar prendido el boton

      while ((millis()-tBoton) < tEncendido){  // Corresponde a cada iteración del juego
        int val1 = digitalRead(3); 
        int val2 = digitalRead(5); 
        int val3 = digitalRead(7); 

        int bApretado = 0;

        if ((val1==LOW && oldVal1==HIGH) || (val1==HIGH && oldVal1==LOW ) ) { 
          vB1 = !vB1  ; // Si el boton es presionado, cambia el valor 
          bApretado = 1;
        }
        if ((val2==LOW && oldVal2==HIGH) || (val2==HIGH && oldVal2==LOW ) ) { 
          vB2 = !vB2  ; // Si el boton es presionado, cambia el valor 
          bApretado = 2;
        }
        if ((val3==LOW && oldVal3==HIGH) || (val3==HIGH && oldVal3==LOW ) ) { 
          vB3 = !vB3  ; // Si el boton es presionado, cambia el valor 
          bApretado = 3;
        }
    
       if ( ((vB1 == true) || (vB2 == true) || (vB3 == true) ) && (cBoton == bApretado ) )  { // Lograron apretar el boton correcto
         puntaje = puntaje + 10 ;
         pTotal = pTotal + 10;
         golpes = golpes + 1;
         tone(buzzerPin, 1000); // Tono afirmativo
         delay(200); // Delay for 200 milliseconds
         noTone(buzzerPin); // Stop the tone
        digitalWrite(cLed, LOW);
         break;
        }
        
       if ( ((vB1 == true) || (vB2 == true) || (vB3 == true) ) && (cBoton != bApretado ) )  { //apretaron el boton equivocado
         puntaje = puntaje - 5 ;
         fallos = fallos + 1;
         tone(buzzerPin, 150); // Tono Negativo
         delay(300); // Delay for 300 milliseconds
         noTone(buzzerPin); // Stop the tone
        digitalWrite(cLed, LOW);
         break;
        }
        
        oldVal1 = val1;
        oldVal2 = val2;
        oldVal3 = val3;

      }
     digitalWrite(cLed, LOW);
     if ( ((vB1 == false) && (vB2 == false) && (vB3 == false) )){
       tone(buzzerPin, 150);
      delay(300);
      noTone(buzzerPin);
      delay(200);
      tone(buzzerPin, 100);
      delay(150);
      noTone(buzzerPin);
        puntaje = puntaje - 5;
     }
    

    }

    if ( puntaje > 0){
      puntaje = puntaje + 20;
      pTotal = pTotal + 20;
      Serial.print("\r\n \r\n Felicitaciones, pasaste de nivel");
      tone(buzzerPin, 550);
      delay(300);
      tone(buzzerPin, 750);
      delay(300);
      noTone(buzzerPin);
      delay(200);
      tone(buzzerPin, 1000);
      delay(150);
      noTone(buzzerPin);
    }
   dificultad = dificultad + 1;

  }


Serial.print("\n\r \n\r Fin de la linea soldado! tus resultados fueron: \r\n");
  Serial.print("\n\r Puntaje: ");
  Serial.print(pTotal);
  Serial.print("\n\r Dificultad final: ");
  Serial.print(dificultad);
  Serial.print("\n\r Aciertos a los topos: ");
  Serial.print(golpes);
  Serial.print("\n\r Golpes equivocados: ");
  Serial.println(fallos);


// Cancion de Amongus:


 
  tone(buzzer, 1046); 
  delay(250);
  tone(buzzer, 1244);   
  delay(250); 
  tone(buzzer, 1400); 
  delay(250); 
  tone(buzzer,   1510); 
  delay(250);
  tone(buzzer, 1400); 
  delay(250);
  tone(buzzer,   1244); 
  delay(250); 
  tone(buzzer, 1046); 
  delay(250); 
  noTone(buzzer);   
  delay(500); 
  tone(buzzer, 932); 
  delay(125);
  tone(buzzer, 1174);   
  delay(125); 
  tone(buzzer, 1046); 
  delay(250);
  // end of first
   noTone(buzzer); 
  delay(500); 
  tone(buzzer, 780); 
  delay(250); 
   tone(buzzer, 525); 
  delay(250); 
  noTone(buzzer); 
  delay(250);
   //secont part
  tone(buzzer, 1046); 
  delay(250);
  tone(buzzer, 1244);   
  delay(250); 
  tone(buzzer, 1400); 
  delay(250); 
  tone(buzzer,   1510); 
  delay(250);
  tone(buzzer, 1400); 
  delay(250);
  tone(buzzer,   1244); 
  delay(250);
  tone(buzzer, 1400); 
  delay(250);
  noTone(buzzer);   
  delay(750);
  //fast part
  tone(buzzer, 1510); 
  delay(125);
   tone(buzzer, 1400); 
  delay(125);
  tone(buzzer, 1244); 
  delay(125);
   tone(buzzer, 1510); 
  delay(125);
  tone(buzzer, 1400); 
  delay(125);
   tone(buzzer, 1244); 
  delay(125);
  tone(buzzer, 1510); 
  delay(125);
   tone(buzzer, 1400); 
  delay(125);
  tone(buzzer, 1244); 
  delay(125);
   tone(buzzer, 1510); 
  delay(125);
  tone(buzzer, 1400); 
  delay(125);
   tone(buzzer, 1244); 
  delay(125);
   tone(buzzer, 1510); 
  delay(125);
   noTone(buzzer); 
  delay(500);
  
  Serial.print("\n\r Lamentablemente no puedo programar al arduino para que detenga el loop principal, así que tendras que jugar denuevo, en aproximadamente 10 segundos \n\r \n\r \n\r");

puntaje = 50;        // Puntuación inicial
duracionTopo = 1000; // Tiempo que transcurre entre encendido y apagado de cada LED (milisegundos)
golpes = 0;           // Número de golpes exitosos
fallos = 0;          // Número de golpes errados
dificultad = 1;      // Nivel de dificultad inicial
pTotal = 50; // Puntuación sin resta  
 delay(10000);
}
