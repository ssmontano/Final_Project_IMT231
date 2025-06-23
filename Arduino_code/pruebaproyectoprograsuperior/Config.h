#ifndef CONFIG_H
#define CONFIG_H

// ----- Pines hardware ESP32 -----
#define ENABLE_PORT     25
#define MOTOR_WIRE_1    26
#define MOTOR_WIRE_2    27
#define GOL_IZQ         18
#define GOL_DER         19
#define Proximidad1     32
#define Proximidad2     33
#define Start           15

// ----- Pines de las luces RGB -----
#define Red 14
#define Green 2
#define Blue 4

// ----- Constantes del Juego -----
const int BAUDRATE = 57600;
const int debounceDelay = 50;

// ----- PWM config -----
const int frecuencia = 5000;
const int resolucion = 8;
const int canal = 0;

#endif // CONFIG_H