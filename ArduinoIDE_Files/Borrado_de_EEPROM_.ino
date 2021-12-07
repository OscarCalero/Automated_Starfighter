/*
  created by Oscar Calero
*/

// ********** Select your EEPROM ATmega168/ATmega328p ****
#include <EEPROM.h>
#define EEPROM_version 512  // ATmega168
//#define EEPROM_version 1024  // ATmega328p
long valorEEPROM_;


// ****************************************** SETUP ****************************************
void setup() {

  // RF & Serial
  Serial.begin(115200);

  Serial.print("Inicio Borrado versión " ) ;
  Serial.println( EEPROM_version ) ;
  
  for ( int i = 0 ; i < EEPROM_version ; i++ ) {
     EEPROM.write( i,255 ) ;
  }

  Serial.println("Done" ) ;
  Serial.println("" ) ;


  Serial.println("Códigos RF de 4 bytes" ) ;
  for ( int i = 0 ; i < 17 ; i++ ) {
    if ( EEPROM_version == 1024 || i < 7 ) {
      Serial.print("rf" ) ;
      Serial.print(i) ;
      Serial.print(" : " ) ;
      Serial.println(EEPROMReadlong(i * 4) ) ;
    }
  }

  if ( EEPROM_version == 1024 ) {
    Serial.print("Códigos secundarios 1=ON / 0=OFF : " ) ;
    Serial.println( EEPROM.read( 71 ) ) ;
  }

  Serial.println("" ) ;
  Serial.println("" ) ;
  Serial.print("Info bytes de direcciones EEPROM de  0 a " ) ;
  Serial.println(EEPROM_version ) ;

  for ( int i = 0 ; i < (EEPROM_version / 10) + 1 ; i++ ) {
    Serial.print("Address ") ;
    Serial.print(i * 10) ;
    Serial.print(" ... " ) ;
    for (int a = 0; a < 10; a++) {
      valorEEPROM_ = EEPROM.read( i * 10 + a ) ;
      if (i * 10 + a < EEPROM_version) {
        Serial.print(valorEEPROM_ ) ;
        Serial.print(", " ) ;
      }
    }
    Serial.println("") ;
  }
}

// ****************************************** LOOP ****************************************
void loop() {
  delay(1);
}

long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
