# Arduino_MP3_Player
Arduino MP3 player for Atmega168 and higher controlled by serial and/or IR controller

_Este es un ejemplo para reproducir ficheros MP3/WAV guardados en una SD desde Arduino. Los ficheros se pueden lanzar desde el puerto serie o desde un mando de Infrarrojos (IR).
El código se ha pensado para que corra desde un Atmega18 que tien muy poca memoria, por lo que se han eliminado muchas funciones superfluas_

Inicialmente desarrollé este código para hacer que una nave ( Starfighter de Obi-wan ) de STAR WARS encendiera las luces a modo de lásers y emitiera sonidos, por lo que el circuito incorpora además LEDs.
 
![Starfighter](https://github.com/OscarCalero/Automated_Starfighter/blob/main/Images/Portada2.png?raw=true)


## Instrucciones...

Link de YouTube con montaje, programación y funcionamiento :  ---

Necesitamos:

```
Nave en miniatura o dispositivo sobre el que introducimos en sonifo MP3 y LEDs
Reproductor MP3 DFPlayer-Mini (funciona por serial)
Componentes electrónica (LEDs, Altavoz, IR receiver, switch)
Una tarjeta SD, que en mi caso de de 4Gb
Un procesador (en mi caso tipo mini con ATmega168)
Mando a distancia
```
Este es el esquema electrónico que hay en el interior:

![Starfighter](https://github.com/OscarCalero/Automated_Starfighter/blob/main/Images/Circuit.png?raw=true)

Este es el esquema de archivos que se almacenan en la SD (tipo MP3 o WAV):

![Starfighter](https://github.com/OscarCalero/Automated_Starfighter/blob/main/Images/FilesOnSD.PNG?raw=true)

Recomiendo su montaje en 3 FASES de hardware:

```
1.- Montaje de circuito electrónico
2.- Proramación por puerto serie y con mando IR
3.- Inserción en maqueta
```

### Este es el resultado

Una miniatura que reproduce sonidos y música MP3 con el mando a distancia, y adicionalmente activa LEDs.

![Starfighter](https://github.com/OscarCalero/Automated_Starfighter/blob/main/Images/IR_controller.png?raw=true)


### Disfruta...

Oscar
