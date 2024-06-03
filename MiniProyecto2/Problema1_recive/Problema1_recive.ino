#include <IRremote.h>
/*
IRsend irsend(3); // Define the IR sender on pin 9
IRrecv irrecv(2); // Define the IR receiver on pin 2
int sCommand;
*/
int IR_SEND_PIN = 3; 
int IR_RECEIVE_PIN = 2; 
int pinBuzzer = 6;

uint32_t rawData; 
uint16_t sAddress_send = 0x0102; 
uint8_t sCommand_send = 0x34; 
uint16_t sAddress_receive; 
uint8_t sCommand_receive; 



int lPunto  = 250;          // Largo del punto en morse (en ms)

const char* CodigoMorse[37][6] = {  // El diccionario con todas las letras y numeros:
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
  {"-", "-", "-", "-", ".", NULL},  // 9
  {" ", " ", " ", " ", " ", NULL}  // espacio
};

const char* MensajeRecibido[40][6];


void setup() {
  Serial.begin(115200);
 IrSender.begin(IR_SEND_PIN, DISABLE_LED_FEEDBACK, 0); 
 IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); 
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {
// fuerza a que el recibidor de mensajes sea un vector nulo.
  for (int i = 0; i < 40; i++) { 
    for (int j = 0; j < 6; j++) { 
  MensajeRecibido[i][j] = NULL;
  }
  }
 // el que fuerza el proceso de lectura
int indLetra = 0;
int IndCaracter = 0;
Serial.println("empezo la recepción");
//Serial.println();
while(true){
  sCommand_receive = IrReceiver.decodedIRData.command;
  if (IrReceiver.decode()) { // Decodifica la señal IR
    if (sCommand_receive == 1) { 
    MensajeRecibido[indLetra][IndCaracter] = ".";
   IndCaracter++;
    }
    else if (sCommand_receive == 2) { 
    MensajeRecibido[indLetra][IndCaracter] = "-";
    IndCaracter++;
    }
    else if (sCommand_receive == 3) { 
    indLetra++;
    IndCaracter = 0;
    }
    else if (sCommand_receive == 4) { 
        for (int i = 0; i < 6; i++) {
       MensajeRecibido[indLetra][i] = " ";
        }
    indLetra++;
    IndCaracter = 0;
    }
    else if (sCommand_receive == 5) { 
      sCommand_receive = 5;
         break;
    }
    Serial.print(sCommand_receive);
    delay(lPunto);
    IrReceiver.resume();
  }
} // Termina El while
Serial.println();
Serial.println("El mensaje recibido en Morse:");
delay(lPunto);
 // Imprime la secuencia en morse:
 for (int i = 0; i < 40; i++) {
    // Loop through each column
    for (int j = 0; j < 6; j++) {
      // Print the element at current row and column
      if(MensajeRecibido[i][j] != NULL ){
      Serial.print(MensajeRecibido[i][j]);
     // Add a tab for formatting
      
    }

  }
       if(MensajeRecibido[i][0] != NULL ){
      Serial.print("|");}
 }
  Serial.println();
  Serial.println("Comienza la transcripción");

delay(lPunto);

  const char* MensajeTexto[40]; // se define un array largo para almacenar el mensaje traducido en morse
  for (int i = 0; i < 40; i++) {
 MensajeTexto[i] = NULL;
  }

delay(lPunto);

   int indTex = 0;
    for (int i = 0; i < 40 ; i++)  { // Revisa todos los caracterres de MensajeTexto[][]
    if (MensajeRecibido[i][0] == NULL ) { // Check if the first element of the string is NULL
      break; // Exit loop if encountered a NULL element
    }
    for (int j = 0; j < 37; j++)  { // Revisa el diccionario completo
      bool idPalabra = true;
    for (int k = 0; k < 5; k++) { // Revisa los caracteres no nulos del i esimo elemento de MensajeTexto[][]
          if (MensajeRecibido[i][k] != CodigoMorse[j][k]) {  // Revisa que el i esimo caracter del mensaje es o no el j esimo caracter en el diccionario, y de no serlo, trransforma a idPAlabra a falso
      idPalabra = false;
    }
    }
            if(idPalabra == true){ // Si idPalabra sigue siendo verdadero, quiere decir que nuestro caracter es igual al j-esimo del diccionario, y por lo tanto 
            if( 0 <= j <= 25 ){
            MensajeTexto[indTex] = char(j + 'A') ;
                  break;
            }
            else if( 25 < j <= 37 ){
            MensajeTexto[indTex] = char(j + '0') ;
                  break;
            }
            else{
            MensajeTexto[indTex] = " " ;
                  break;
            }
            indTex++;
          }
    }
  }

delay(lPunto);
Serial.print("termino la transcripción");

Serial.println();

  for (int i = 0; i < 40; i++) {
    Serial.print(MensajeTexto[i]);
  }
  Serial.println();

delay(lPunto);


  char texto;
  for (int i = 0; MensajeTexto[i] != NULL; i++) {
    texto += MensajeTexto[i];
  }
  
Serial.print(texto);


if (texto == "SOS" ) {   
  for (int i = 0; i < 15; i++) {
  tone(pinBuzzer, 500);
  delay(500);
  tone(pinBuzzer, 1000);
  delay(500);
  noTone(pinBuzzer);
    }
}

if (texto == "TIC" ) {
      tone(pinBuzzer, 550);
      delay(300);
      tone(pinBuzzer, 750);
      delay(300);
      noTone(pinBuzzer);
      delay(200);
      tone(pinBuzzer, 1000);
      delay(150);
      noTone(pinBuzzer);
    }


}