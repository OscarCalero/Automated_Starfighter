/* ##############################################################

  Instrucciones por Serial ( sin ajuste de linea a 9600 ):

  Directorios:
  fX = directorio de reproducción donde X es número de 0 a 99
  Ejemplo: f1 es directorio: 01

  vX = volumen X donde X es de 1 a 30 pre-fijado en EEPROM para arranque
  Ejemplo: v20 es volumen 20 (66%)

  Asocial códigos IR:
  pX = programa el códigos IR a X
  Ejemplo: p5 + [madoIR] asocia boton de [mandoIR] a fichero 005????????.mp3

  Codigos especiales:
  p0 = Almacena cóigo para reproducción aleatoria
  p121 (pos EEPROM 484) = fichero de sonido con dos dígitos >9
  p122 (pos EEPROM 488) = APAGAR/ENCENDER LEDs
  p123 (pos EEPROM 492) = Fichero previo
  p124 (pos EEPROM 496) = Fichero siguiente
  p125 (pos EEPROM 500) = Volumen -
  p126 (pos EEPROM 504) = Volumen +

  Reproducir MP3/WAV:
  simplemente escribir numero. Por ejemplo:
  47 reproduce el fichero 047???????.mp3

  Programa diseñado para siguiente configuración gestionada en:  void receivedCode
  Sonido R2D2 - de fichero 001 a 003
  Conversaciones - de fichero 004 a 006
  Disparos laser - de fichero 007 a 0011
  Musica Star wars - fichero 012
  ##############################################################
*/



#include "SoftwareSerial.h" // Establece comunicación serie en otros pines

const int Rx = 3;
const int Tx = 4;
SoftwareSerial mySerial(Rx, Tx); // conectar Tx de Arduino con Rx del modulo y viceversa

/* Definimos las partes de la linea de comandos para la transmision serie
  7E FF 06 03 00 00 01 FF E6 EF (Ej: Select the first song played)

  7E --- START command
  FF --- Version Information
  06 --- Data length (not including parity)
  03 --- CMD (Representative No.)
  00 --- If need to acknowledge [0x01: need answering, 0x00: do not need to return the response]
  00 --- Tracks high byte [DH]
  01 --- Tracks low byte [DL], represented here is the first song played
  FF --- Checksum high byte
  E6 --- Checksum low byte
  EF --- END Command
*/
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
// CMD, Comando de control
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]
// High byte DH
// low byte DL
// Checksum high byte
// Checksum low byte
# define End_Byte 0xEF


// Definimos los comandos de control
# define PLAY_init 0x3F // iniciacion del modulo
# define REPEAT_play 0x11 // 1:start repeat, 2:stop play
# define PAUSE 0x0E
# define PLAY 0x0D // Reanuda despues de la pausa
# define DEF_folder 0x0F // Especifica carpeta de reproduccion 1-10
# define PLAY_next 0x01 // Pista siguiente
# define PLAY_prev 0x02 // Pista anterior
# define DEF_track 0x03 // Especifica pista 0-2999
# define DEF_mode 0x04 // 0-Repeat, 1-Folder repeat, 2-Single repeat, 3-Random
# define VOLUME_set 0x06 // Volumen entre 00 y 30
// ===================================================

# define ACTIVATED LOW // mp3 apagado inicialmente
boolean isPlaying = false;

#define FOLDERING 0
#define PLAYING 1
#define PROGRAMING 2
#define VOLUME 3
#define letraP 112  // program
#define letraF 102  // folder 1...99
#define letraV 118  //volumen 00 a 30
byte estatus = PLAYING;
byte folder  ;
byte volume ;  //volumen 00 a 30

// ********* IR CODES
#include <IRremote.h>
#define IR_Pin 2 // Pin 2 para el sourde del IR
IRrecv irrecv(IR_Pin);
decode_results IRresults;

// ********* EEPROM
#include <EEPROM.h>
unsigned int progCODE = 0;
boolean program;

// *********** LEDs
byte motores = HIGH;
byte laser = LOW;
byte r2d2 = LOW;
byte cabina = HIGH;
unsigned long tiempoLED ;
byte efecto = 255;
byte previous;
byte numeroAlto = 0;
#define ficheroInicialRandom 1
#define ficheroFinalRadom 11
#define totalNumeroFicheros 12
#define ficheroInicialEfecto1 1
#define ficheroFinalEfecto1 3
#define ficheroInicialEfecto2 7
#define ficheroFinalEfecto2 11
#define ficherosAlmacenables 120
#define VolumenMinimo 0
#define VolumenMaximo 30
#define pinLedMotores 5
#define pinLedLaser 6
#define pinLedR2D2 7
#define PinLedCabina 8
#define addressAleatorio 0
#define addressDecenas 484
#define addressLuces 488 
#define addressPrevio 492
#define addressSiguiente 496
#define addressVolumenUp 500
#define addressVolumenDown 504
#define addressFolder 508
#define addressVolume2 509



  
// #########################################################

void setup () {

  Serial.begin(9600);
  folder = EEPROM.read(addressFolder);
  volume =  EEPROM.read(addressVolume2);
  mySerial.begin (9600);
  delay(200);
  playFirst();
  isPlaying = true;

  // Lector IR
  irrecv.enableIRIn();
  //irrecv.blink13(true);

  // LEDs
  pinMode(pinLedMotores , OUTPUT);
  pinMode(pinLedLaser , OUTPUT);
  pinMode(pinLedR2D2 , OUTPUT);
  pinMode(PinLedCabina , OUTPUT);
  randomSeed(analogRead(A0));
  Serial.println("OK");
  tiempoLED = millis();
}
// ========================================

void loop () {

  gestionLEDs();

  if (mySerial.available() > 0) {
    // read the incoming byte:
    byte incomingByte = mySerial.read();
    //Serial.print(incomingByte, HEX);
    //Serial.print(" ");
    if (incomingByte == 239) { // Informa de finalización de sonido
      //Serial.println("");
      //Serial.print("answer: ");
      efecto = 0;
    }
  }

  if ( irrecv.decode(&IRresults)) {  // input IR code
    Serial.print("IR:");
    Serial.print( IRresults.value );
    if (IRresults.value == 4294967295) {  // codigo repetido
      Serial.print("***");
    }
    else if (estatus == PROGRAMING) {
      Serial.print("->pos:");
      Serial.print( progCODE );
      EEPROMWritelong( progCODE * 4 , IRresults.value );
      estatus = PLAYING;
    } else {
      receivedCode( IRresults.value );
    }
    Serial.println();
    irrecv.resume();
  }

  if (Serial.available() > 0) {
    delay(25); // tiempo para cargar 4 dígitos min. 5mS
    int number_ = 0;
    boolean numberExist = false;
    while ((Serial.available() > 0)) {
      byte incomingByte = Serial.read();
      switch ( incomingByte ) {
        case letraF:  // letra f = folder
          Serial.print("folder:");
          estatus = FOLDERING;
          break;
        case letraP:  // letra p = program
          estatus = PROGRAMING;
          Serial.print("prog :");
          break;
        case letraV:  // letra v = volume 0-30
          Serial.print("volume:");
          estatus = VOLUME;
          break;
        case 43: //+
          volume++;
          execute_CMD( VOLUME_set, 0, volume );
          break;
        case 45: //-
          volume--;
          execute_CMD( VOLUME_set, 0, volume );
          break;
        default:
          numberExist = true;
          number_ = number_ * 10 + incomingByte - 48;
          break;
      }
    }
    if (numberExist) {
      switch ( estatus ) {
        case FOLDERING:
          folder = number_;
          EEPROM.write(addressFolder, folder) ;
          Serial.println(folder);
          execute_CMD(DEF_folder, 0, 1);
          estatus = PLAYING;
          break;

        case PROGRAMING:
          progCODE = number_;
          Serial.print(progCODE);
          Serial.println(" (press IR code)");
          break;

        case PLAYING:
          if (folder == 0) {
            execute_CMD(DEF_track, 0, number_);
          } else {
            execute_CMD( 0x0F, folder, number_ );
          }
          break;
        case VOLUME:
          /*
            byte incomingByte = number_;
            EEPROM.write(addressVolume2, incomingByte) ;
            Serial.println(incomingByte);
            execute_CMD(VOLUME_set, 0, incomingByte);
            volume = incomingByte;
            estatus = PLAYING;
          */
          volume = number_;
          EEPROM.write(addressVolume2, volume) ;
          Serial.println(volume);
          execute_CMD(VOLUME_set, 0, volume);
          estatus = PLAYING;
          break;
      }
    }
  }
}
// ===========================================================


void gestionLEDs() {


  if ( millis() > tiempoLED ) {
    switch (efecto) {
      case 0:
        laser = LOW;
        r2d2 = LOW;
        break;
      case 1:
        tiempoLED = millis() + random(50, 300);
        if (r2d2 == LOW) {
          r2d2 = HIGH;
        } else {
          r2d2 = LOW;
        }
        break;
      case 2:
        tiempoLED = millis() + random(50, 200);
        if (laser == LOW) {
          laser = HIGH;
        } else {
          laser = LOW;
        }
        break;
      case 3:
        laser = LOW;
        r2d2 = LOW;
        motores = LOW;
        cabina = LOW;
        break;
      case 4:
        laser = HIGH;
        r2d2 = HIGH;
        motores = HIGH;
        cabina = HIGH;
        break;
    }
  }
  digitalWrite(pinLedMotores , motores);
  digitalWrite(pinLedLaser , laser);
  digitalWrite(pinLedR2D2 , r2d2);
  digitalWrite(PinLedCabina , cabina);
}

void receivedCode( unsigned long receivedCode_ ) {

  if (numeroAlto > 0) {
    for (int i = 0; i < 10; i++) {
      if ( EEPROMReadlong(i * 4) == receivedCode_ ) {
        if (numeroAlto == 1) {
          previous = i * 10;
          numeroAlto = 2;
        }
        else {
          previous = previous + i;
          numeroAlto = 0;
          execute_CMD( 0x0F, folder, previous );
        }
        break;
      }
    }
  }
  else if ( EEPROMReadlong(addressDecenas ) == receivedCode_  ) {  // número de fichero con 2 dígitos
    numeroAlto = 1;
  }
  else if ( EEPROMReadlong(addressLuces ) == receivedCode_  ) {
    if (cabina == HIGH ) {
      efecto = 3 ;
    } else {
      efecto = 4 ;
    }
  }
  else if ( EEPROMReadlong(addressPrevio ) == receivedCode_  && previous > 1 ) {  // Previous
    execute_CMD( 0x0F, folder, previous - 1 );
    Serial.println("Prev");
  }
  else if ( EEPROMReadlong(addressSiguiente ) == receivedCode_  && previous < ficherosAlmacenables +1 ) {  // Next
    execute_CMD( 0x0F, folder, previous + 1 );
    Serial.println("Next");
  }
  else if ( EEPROMReadlong(addressVolumenUp ) == receivedCode_  && volume > VolumenMinimo ) {   // volumen -
    volume--;
    execute_CMD( VOLUME_set, 0, volume );
    Serial.print(" ->Vol- ");
    Serial.println(volume);
  }
  else if ( EEPROMReadlong(addressVolumenDown ) == receivedCode_  && volume < VolumenMaximo ) {  // volumen +
    volume++;
    execute_CMD( VOLUME_set, 0, volume );
    Serial.print(" ->Vol+ ");
    Serial.println(volume);
  }
  else {
    for (int i = 0; i < ficherosAlmacenables +1 ; i++) {
      if ( EEPROMReadlong(i * 4) == receivedCode_ ) {
        execute_CMD( 0x0F, folder, i );
        break;
      }
    }
  }
}

void playFirst() {
  execute_CMD(PLAY_init, 0, 2); // 0x3F inicia dispositivo (1:U-Disk, 2:TF-Card, 4:PC, 8:Flash)
  delay(100);
  execute_CMD(VOLUME_set, 0, volume); // 0x06 Selecciona volumen (0x00~0x30)
  delay(100);
  // primero busca mp3 de la raiz y despues los de la carpeta
  execute_CMD(DEF_folder, 0, folder); // 0x0F Selecciona directorio /01 (max 99 folder)
  delay(100);
}

// =============================================================

void execute_CMD(byte CMD, byte Part1, byte Part2) {
  if ( CMD == 0x0F) {

    if (Part2 == 0 ) {  // Sonido aleatorio
      Part2 = random(ficheroInicialRandom , ficheroFinalRadom );
      Serial.println(" ->random ");
    }
    if (Part2 > ficheroInicialEfecto1 -1 && Part2 < ficheroFinalEfecto1 +1) {
      efecto = 1;
    }
    if (Part2 > ficheroInicialEfecto2 -1 && Part2 < ficheroFinalEfecto2 +1 ) {
      efecto = 2;
    }

    Serial.print(" ->folder:");
    Serial.print(Part1);
    Serial.print(" /track:");
    Serial.println(Part2);
    previous = Part2;
  }


  // Calcula el checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Part1 + Part2);
  // Construimos la linea de comandos
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                            Part1, Part2, highByte(checksum), lowByte(checksum), End_Byte
                          };
  //Serial.print(" >module: ");
  for (byte k = 0; k < 10; k++) { // Envia la linea de comandos al modulo
    mySerial.write( Command_line[k]);
    //Serial.print(Command_line[k], HEX); Serial.print(" ");
  }
  //Serial.println();
}

// ***************************************************************************
// ******************  UTILIDADES ********************************************
// ***************************************************************************


// ***** EEPROMWritelong *************
void EEPROMWritelong(int address, unsigned long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

// ***** EEPROMReadlong *************
unsigned long EEPROMReadlong(unsigned long address) {
  unsigned long four = EEPROM.read(address);
  unsigned long three = EEPROM.read(address + 1);
  unsigned long two = EEPROM.read(address + 2);
  unsigned long one = EEPROM.read(address + 3);
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
