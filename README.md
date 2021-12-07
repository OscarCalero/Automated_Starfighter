# Arduino_MP3_Player
Arduino MP3 player for Atmega168 and higher controlled by serial and/or IR controller

_Este es un ejemplo para reproducir ficheros MP3/WAV guardados en una SD desde Arduino. Los ficheros se pueden lanzar desde el puerto serie o desde un mando de Infrarrojos (IR).
El código se ha pensado para que corra desde un Atmega18 que tien muy poca memoria, por lo que se han eliminado muchas funciones superfluas_

Inicialmente desarrollé este código para hacer que una nave ( Starfighter de Obi-wan ) de STAR WARS encendiera las luces a modo de lásers y emitiera sonidos, por lo que el circuito incorpora además LEDs.
 

![Starfighter](https://github.com/OscarCalero/Arduino_MP3_Player/blob/main/Imagenes/Portada2.png?raw=true)


## Instrucciones...

Link de YouTube con montaje, programación y funcionamiento :  https://www.youtube.com/watch?v=ZIpGoSoBYxE

Necesitamos:

```
TFT ILI9486 o similar
Controlador (librería) de TFT_eSPI de Bodmer
Una tarjeta SD, que en mi caso de de 8Gb
Un procesador ESP32, aunque otros pueden funcionar igualmente
```
[libreria TFT_eSPI Bodmer](https://github.com/Bodmer/TFT_eSPI)

Recomiendo su montaje en 3 FASES de hardware:

```
1.- conectar TFT
2.- Conectar pantalla táctil (touch) y calibrar grabando en SPIFFS
3.- Conectar la SD con imágenes JPG y su uso como HMI
```
![ILI9486_Montaje](https://github.com/OscarCalero/TFT_ILI9486/blob/main/Montaje.jpg?raw=true)
![ILI9486_Circuit](https://github.com/OscarCalero/TFT_ILI9486/blob/main/Circuit/ILI9486_L.jpg?raw=true)


### Contenidos que podreis ver

```
1.- Conexión de TFT
2.- Varios dispositivos SPI simultaneamente
3.- Uso de SPIFFS (memoria flash del ESP32)
4.- Uso de SD en ESP32
```

### Disfruta...

Oscar
