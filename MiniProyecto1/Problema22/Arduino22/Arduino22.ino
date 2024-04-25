#include <DHT.h> // Definimos el pin digital donde se conecta el sensor

#define DHTPIN 11 // Dependiendo del tipo de sensor

#define DHTTYPE DHT11 // Inicializamos el sensor DHT11 

DHT dht(DHTPIN, DHTTYPE);

const int LED_PIN_B = 2;  // Pin del primer LED
const int LED_PIN_G = 3; // Pin del segundo LED
const int LED_PIN_R = 4; // Pin del tercer LED

const int BUTTON_PIN1 = 7; // Pin del botón pulsador
const int BUTTON_PIN2 = 9; // Pin del botón pulsador
const int BUTTON_PIN3 = 10; // Pin del botón pulsador


int alive_cells =0;
int brightness = 0;
int fadeAmount = 5;
unsigned long tTemperatura;

int lastButtonState1;
int lastButtonState2;
int lastButtonState3;
int u2;

void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN_B, OUTPUT);
    pinMode(LED_PIN_G, OUTPUT);
    pinMode(LED_PIN_R, OUTPUT);
    pinMode(BUTTON_PIN1, INPUT_PULLUP);
    pinMode(BUTTON_PIN2, INPUT_PULLUP);
    pinMode(BUTTON_PIN3, INPUT_PULLUP);
  lastButtonState1 = digitalRead(BUTTON_PIN1);
  lastButtonState2 = digitalRead(BUTTON_PIN2);
  lastButtonState3 = digitalRead(BUTTON_PIN3);
  u2 = 0;
  tTemperatura = millis();
  dht.begin();
}


void loop() {


  unsigned long tBucle = millis();
  int buttonState1 = digitalRead(BUTTON_PIN1);
  int buttonState2 = digitalRead(BUTTON_PIN2);
  int buttonState3 = digitalRead(BUTTON_PIN3);


    // Lee la cantidad de células vivas enviadas desde Python
    if (Serial.available() >= 4) {  // Se asegura de recibir 4 caracteres
        char buffer[5];  
        Serial.readBytes(buffer, 4);  
        buffer[4] = '\0';  

        // Transforma estos 4 caracteres a un numero
        alive_cells = atoi(buffer);  

    }

// Esperamos x segundos entre medidas 
delay(500); 
// Leemos la temperatura en grados celsius 
float t = dht.readTemperature(); 
int tInt = int(t);
String tStr = "t_0" + String(tInt); // La convertimos en algo enviable a python

// Comprobamos si ha habido algún error en la lectura 
 //Serial.println(t);
  if (tBucle - tTemperatura >= 20000) {
    // Your data to sen
    
    tTemperatura = tBucle; // Update last send time
   Serial.println(tStr);
    if ( isnan(t)) {
   Serial.println("Error obteniendo los datos del sensor DHT11"); return; } 


}


    int u1 = 0;
    // Evaluar las condiciones de estabilidad, subpoblación y sobrepoblación
    if (alive_cells <4600) {

     u1 = LED_PIN_R;
     if(u1 != u2) {
       for (int i = 0; i <= 255; i++) {
         analogWrite(u1, i);
         analogWrite(u2, 255 - i);
         delay(10);  // Adjust delay for transition speed
        }
      }
      else{ analogWrite(LED_PIN_B,0);
            analogWrite(LED_PIN_G,0);
            analogWrite(LED_PIN_R,255); }
    } 
    else if ((alive_cells > 4600) && (alive_cells < 4800)) {
    
      u1 = LED_PIN_G;
          if(u1 != u2) {
       for (int i = 0; i <= 255; i++) {
         analogWrite(u1, i);
         analogWrite(u2, 255 - i);
         delay(10);  // Adjust delay for transition speed
        }
      }
       else{ analogWrite(LED_PIN_B,0);
            analogWrite(LED_PIN_G,255);
            analogWrite(LED_PIN_R,0); }
    } 
    else {

      u1 = LED_PIN_B;
           if(u1 != u2) {
       for (int i = 0; i <= 255; i++) {
         analogWrite(u1, i);
         analogWrite(u2, 255 - i);
         delay(10);  // Adjust delay for transition speed
        }
      }
      else{ analogWrite(LED_PIN_B,255);
            analogWrite(LED_PIN_G,0);
            analogWrite(LED_PIN_R,0); }
    }

    // Verificar si se ha presionado el botón pulsador para reiniciar el juego
    
    if ((buttonState3 == LOW && lastButtonState3 == HIGH) || (buttonState3 == HIGH && lastButtonState3 == LOW) ) {
      char message =  "r";

      Serial.println("r");  // Add newline character as delimiter
    }

    if ((buttonState2 == LOW && lastButtonState2 == HIGH) || (buttonState2 == HIGH && lastButtonState2 == LOW) ) {
      char message =  "b_c";

      Serial.println("b_c");  // Add newline character as delimiter
    }

    if ((buttonState1 == LOW && lastButtonState1 == HIGH) || (buttonState1 == HIGH && lastButtonState1 == LOW) ) {
      char message = "b_m";

      Serial.println("b_m");  // Add newline character as delimiter
    }
    lastButtonState1 = buttonState1;
    lastButtonState2 = buttonState2;
    lastButtonState3 = buttonState3;

    u2 = u1;
}

