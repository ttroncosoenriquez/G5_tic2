const int LED_PIN_1 = 9;  // Pin del primer LED
const int LED_PIN_2 = 10; // Pin del segundo LED
const int LED_PIN_3 = 11; // Pin del tercer LED
const int BUTTON_PIN = 2; // Pin del botón pulsador

int alive_cells =0;
int brightness = 0;
int fadeAmount = 5;
unsigned long lastUpdateTime = 0;
unsigned long buttonDebounceTime = 100;
int lastButtonState;
int u2 = 0;
void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_3, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
  lastButtonState = digitalRead(BUTTON_PIN);
}


void loop() {
   int buttonState = digitalRead(BUTTON_PIN);
    // Leer la cantidad de células vivas enviadas desde Python
    if (Serial.available() >= 4) {  // Se asegura de recibir 4 caracteres
        char buffer[5];  
        Serial.readBytes(buffer, 4);  
        buffer[4] = '\0';  

        // Transforma estos 4 caracteres a un numero
        alive_cells = atoi(buffer);  

    }



    int u1 = 0;
    // Evaluar las condiciones de estabilidad, subpoblación y sobrepoblación
    if (alive_cells < 870) {
      /*analogWrite(LED_PIN_3,255);
                analogWrite(LED_PIN_1,0);
            analogWrite(LED_PIN_2,0);*/
     u1 = 11;
     if(u1 != u2) {
       for (int i = 0; i <= 255; i++) {
         analogWrite(u1, i);
         analogWrite(u2, 255 - i);
         delay(10);  // Adjust delay for transition speed
        }
      }
      else{  analogWrite(LED_PIN_3,255);
                analogWrite(LED_PIN_1,0);
            analogWrite(LED_PIN_2,0); }
    } 
    else if ((alive_cells > 870) && (alive_cells < 1250)) {
      /*analogWrite(LED_PIN_2, 255);
                analogWrite(LED_PIN_3,0);
            analogWrite(LED_PIN_1,0);*/
      u1 = 10;
          if(u1 != u2) {
       for (int i = 0; i <= 255; i++) {
         analogWrite(u1, i);
         analogWrite(u2, 255 - i);
         delay(10);  // Adjust delay for transition speed
        }
      }
      else{  analogWrite(LED_PIN_3,0);
                analogWrite(LED_PIN_1,0);
            analogWrite(LED_PIN_2,255); }
    } else {
        /*analogWrite(LED_PIN_1,255);
          analogWrite(LED_PIN_3,0);
            analogWrite(LED_PIN_2,0); */
      u1 = 9;
           if(u1 != u2) {
       for (int i = 0; i <= 255; i++) {
         analogWrite(u1, i);
         analogWrite(u2, 255 - i);
         delay(10);  // Adjust delay for transition speed
        }
      }
      else{  analogWrite(LED_PIN_3,0);
                analogWrite(LED_PIN_1,255);
            analogWrite(LED_PIN_2,0); }
    }

    // Verificar si se ha presionado el botón pulsador para reiniciar el juego
    
    if ((buttonState == LOW && lastButtonState == HIGH)) {
      char message = "r";

      Serial.println("r");  // Add newline character as delimiter
    }
    lastButtonState = buttonState;
    u2 = u1;
}

