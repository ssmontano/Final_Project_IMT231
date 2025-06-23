#include "Gol.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobotDFPlayerMini.h>
#include "config.h"

// Variables y objetos globales que este módulo necesita del archivo principal
extern LiquidCrystal_I2C lcd;
extern DFRobotDFPlayerMini mp3;
extern bool start, Partido;
extern unsigned long GoalTime;

// Función Aumentar_Puntuacion original
void Aumentar_Puntuacion(short &Contador, bool &Toco, short Equipo, bool &Gol) {
  Contador++;
  lcd.setCursor(0, 0); lcd.print("EQUIPO 1 ");
  lcd.setCursor(0, 1); lcd.print("EQUIPO 2");
  lcd.setCursor(14, Equipo - 1); lcd.print(Contador);
  Toco = false;
  start = true;
  Partido = false;
  Gol = true;
  GoalTime = millis();
  digitalWrite(Red,HIGH);
  digitalWrite(Green,HIGH);
  digitalWrite(Blue,HIGH);
  mp3.play(1);
}

// Función Falsa_Alarma original
void Falsa_Alarma(bool &Toco, unsigned long &TiempoDebounce) {
  Toco = false;
  TiempoDebounce = millis();
}

// Función Impulso_Detectado original
void Impulso_Detectado(bool &Toco, unsigned long &TiempoDebounce) {
  TiempoDebounce = millis();
  Toco = true;
}