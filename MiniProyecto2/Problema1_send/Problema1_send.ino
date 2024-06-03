#include <IRremote.h>
/*
IRsend irsend(3); // Define the IR sender on pin 9
IRrecv irrecv(2); // Define the IR receiver on pin 2
int sCommand;
*/
int IR_SEND_PIN = 3; 
int IR_RECEIVE_PIN = 2; 
uint32_t rawData; 
uint16_t sAddress_send = 0x0102; 
uint8_t sCommand_send = 0x34; 
uint16_t sAddress_receive; 
uint8_t sCommand_receive; 


const int pinLed = 5;
const int pinBuzzer = 6;


int lPunto  = 250;          // Largo del punto en morse (en ms)
int lLinea = lPunto * 3;    // Largo de la linea
int lLetras = lPunto * 3;  // Espacio entre letras
int lEspacio = lPunto * 7; // Espacio entre palabras

const char* CodigoMorse[][6] = {  // El diccionario con todas las letras y numeros:
  {".", "-", NULL},            // A
  {"-", ".", ".", ".", NULL},  // B
  {"-", ".", "-", ".", NULL},  // C
  {"-", ".", ".", NULL},       // D
  {".", NULL},                 // E
  {".", ".", "-", ".", NULL},  // F
  {"-", "-", ".", NULL},       // G
  {".", ".", ".", ".", NULL},  // H
  {".", ".", NULL},            // I
  {".", "-", "-", "-", NULL},  // J
  {"-", ".", "-", NULL},       // K
  {".", "-", ".", ".", NULL},  // L
  {"-", "-", NULL},            // M
  {"-", ".", NULL},            // N
  {"-", "-", "-", NULL},       // O
  {".", "-", "-", ".", NULL},  // P
  {"-", "-", ".", "-", NULL},  // Q
  {".", "-", ".", NULL},       // R
  {".", ".", ".", NULL},       // S
  {"-", NULL},                 // T
  {".", ".", "-", NULL},       // U
  {".", ".", ".", "-", NULL},  // V
  {".", "-", "-", NULL},       // W
  {"-", ".", ".", "-", NULL},  // X
  {"-", ".", "-", "-", NULL},  // Y
  {"-", "-", ".", ".", NULL},  // Z
  {"-", "-", "-", "-", "-", NULL},  // 0
  {".", "-", "-", "-", "-", NULL},  // 1
  {".", ".", "-", "-", "-", NULL},  // 2
  {".", ".", ".", "-", "-", NULL},  // 3
  {".", ".", ".", ".", "-", NULL},  // 4
  {".", ".", ".", ".", ".", NULL},  // 5
  {"-", ".", ".", ".", ".", NULL},  // 6
  {"-", "-", ".", ".", ".", NULL},  // 7
  {"-", "-", "-", ".", ".", NULL},  // 8
  {"-", "-", "-", "-", ".", NULL}  // 9
};



void setup() {
  Serial.begin(115200);
 IrSender.begin(IR_SEND_PIN, DISABLE_LED_FEEDBACK, 0); 
 IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); 

  pinMode(pinLed, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

}

void loop() {
  Serial.println("Ingrese un mensaje para ser enviado en Morse:");
  while (!Serial.available()) {
    // El codigo se queda esperando a que escribas tu mensaje
  }
  
  // Lee el mensaje
  String input = Serial.readStringUntil('\n');

  // Muestra lo que escribiste
  Serial.print("Se recibio: ");
  Serial.println(input);
  input.toUpperCase(); // Lo transforma a mayusculas para que el codigo lo pueda leer:

  int lArray = 150;
  const char* morseArray[lArray ]; // se define un array largo para almacenar el mensaje traducido en morse
  for (int i = 0; i < lArray; i++) {
  morseArray[i] = NULL;
  }

  // este for almacena el texto original en su formato morse:
  int index = 0;
    morseArray[index++] = "5";
  for (int i = 0; i < input.length(); i++) {
  char c = input.charAt(i); //revisa si el caracter es un numero o una letra 
  if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
    const char** morseChar;
    if (c >= 'A' && c <= 'Z') {
      
      morseChar = CodigoMorse[c - 'A']; // transforma las letras en su caracter de morse correspondiente
    } else {
      morseChar = CodigoMorse[c - '0' + 26]; // transforma los numeros en su letra de morse correspondiente
    }
    for (int j = 0; morseChar[j] != NULL; j++) {
      morseArray[index++] = morseChar[j];   /// Lee los elementos individuales del caracter y los separa a elementos individuales
    }
    // Separador entre caracteres
    morseArray[index++] = "2";
  } else if (c == ' ') {
    // Separador entre palabras
    morseArray[index++] = "3";
  }
  } 
  //indica el final del mensaje:
  morseArray[index++] = "4";
  morseArray[index++] = "5";

  // Imprime el mensaje en morse:
  Serial.println("Codigo Morse:");
  for (int i = 0; morseArray[i] != NULL; i++) {
    Serial.print(morseArray[i]);
  }
  Serial.println();
// Implementa un comando para cada caracter, para transmitir el mensaje en morse:


for (int i = 0; morseArray[i] != NULL; i++) { //elemento punto

 if (morseArray[i] == ".") {
       IrSender.sendNEC(0x0035, 0x01, 1);
         
  digitalWrite(pinLed, HIGH);
  digitalWrite(pinBuzzer, HIGH);
      delay(lPunto);
  digitalWrite(pinLed, LOW);
  digitalWrite(pinBuzzer, LOW);
    }

 if (morseArray[i] == "-") { //elemento raya
       IrSender.sendNEC(0x0035, 0x02, 1);

  digitalWrite(pinLed, HIGH);
  digitalWrite(pinBuzzer, HIGH);
      delay(lLinea);
  digitalWrite(pinLed, LOW);
  digitalWrite(pinBuzzer, LOW);
    }
    
 if (morseArray[i] == "2") { // esp. entre elementos
       IrSender.sendNEC(0x0035, 0x03, 1);
      delay(lLetras);
    }

 if (morseArray[i] == "3") { //esp. entre palabras
       IrSender.sendNEC(0x0035, 0x04, 1);
      delay(lEspacio);
    }

 if (morseArray[i] == "4") {
       IrSender.sendNEC(0x0035, 0x05, 1);
      delay(4*lPunto);
    }

 if (morseArray[i] == "5") {
       IrSender.sendNEC(0x0035, 0x06, 1);
      delay(lPunto);
    }

// Aquí Se auto recive la señal
/*  
  if (IrReceiver.decode()) { // Decodifica la señal IR
    sCommand_receive = IrReceiver.decodedIRData.command;
    Serial.println(sCommand_receive, HEX);
    IrReceiver.resume(); // Enable receiving of the next value
    delay(100);

  }
   */
  delay(lPunto);
}

}