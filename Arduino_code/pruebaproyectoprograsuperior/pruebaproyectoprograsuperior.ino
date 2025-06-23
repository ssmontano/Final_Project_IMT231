#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DFRobotDFPlayerMini.h>

#include "config.h"
#include "Brainwave.h"
#include "Gol.h"

// ----- Hardware Serial -----
HardwareSerial mp3Serial(0);
HardwareSerial headset1Serial(1);
HardwareSerial headset2Serial(2);

// ----- Variables Globales -----
short contador1 = 0, contador2 = 0;
unsigned long ultimoTiempoDebounce1 = 0, ultimoTiempoDebounce2 = 0;
unsigned long GoalTime = 0, TiempoConteo = 0, MostrarDelay = 0, TimeRGB = 0, WinTime = 0;
bool toco1 = false, toco2 = false, Partido = false, gol1 = false, gol2 = false, start = false;
short velo;

// ----- Objetos Globales -----
LiquidCrystal_I2C lcd(0x27, 16, 2);
DFRobotDFPlayerMini mp3;
Brainwave headset_izq, headset_der;
SemaphoreHandle_t motorMutex;

// ===== Tareas del RTOS =====
void Task_CascoIzq(void *parameter) {
  while (true) {
    if (Partido == true && start == false) {
      headset_izq.generating_res(headset1Serial, 1);
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void Task_CascoDer(void *parameter) {
  while (true) {
    if (Partido == true && start == false) {
      headset_der.generating_res(headset2Serial, 2);
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup() {
  headset1Serial.begin(BAUDRATE, SERIAL_8N1, 9, 10);
  headset2Serial.begin(BAUDRATE, SERIAL_8N1, 16, 17);
  mp3Serial.begin(9600, SERIAL_8N1, 12, 13);

  pinMode(ENABLE_PORT, OUTPUT);
  pinMode(MOTOR_WIRE_1, OUTPUT);
  pinMode(MOTOR_WIRE_2, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
  pinMode(Proximidad1, INPUT);
  pinMode(Proximidad2, INPUT);
  pinMode(Start, INPUT);
  pinMode(GOL_IZQ, INPUT);
  pinMode(GOL_DER, INPUT);

  ledcAttach(ENABLE_PORT, frecuencia, resolucion);
  ledcWrite(canal, 0); // Asegurarse de que el motor está apagado al inicio

  Wire.begin(21, 22);
  lcd.begin(16, 2); // Corregido según tu error anterior
  lcd.init();       // Buena práctica añadir `init()`
  lcd.backlight();

  lcd.setCursor(0, 0); lcd.print("EQUIPO 1 ");
  lcd.setCursor(14, 0); lcd.print(contador1);
  lcd.setCursor(0, 1); lcd.print("EQUIPO 2 ");
  lcd.setCursor(14, 1); lcd.print(contador2);

  if (!mp3.begin(mp3Serial, true, false)) {
    while (true);
  }
  mp3.volume(25);

  motorMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(Task_CascoIzq, "TaskIzq", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(Task_CascoDer, "TaskDer", 4096, NULL, 1, NULL, 0);
}

void loop() { // Toda esta lógica se ejecuta en el Núcleo 1
  if (start == true && Partido == false && gol1 == false && gol2 == false) 
  {
    TiempoConteo = millis();
    // Este bucle while bloqueante es parte de la lógica original.
    while(millis()-TiempoConteo <= 3100)
    {
      digitalWrite(Red,LOW);
      digitalWrite(Green,LOW);
      digitalWrite(Blue,HIGH);
      if(millis()-TiempoConteo >= 0 && millis()-TiempoConteo <=1000)
      {
        if (millis()-TiempoConteo == 0)lcd.clear();
        lcd.setCursor(8,0);
        lcd.print("3");
      }
      if(millis()-TiempoConteo > 1000 && millis()-TiempoConteo <=2000)
      {
        if (millis()-TiempoConteo ==1001)lcd.clear();
        lcd.setCursor(8,0);
        lcd.print("2");
      }
      if(millis()-TiempoConteo > 2000 && millis()-TiempoConteo <=3000)
      {
        if (millis()-TiempoConteo ==2001)lcd.clear();
        lcd.setCursor(8,0);
        lcd.print("1");
      }
    }
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("VAMOS");
    delay(500);
    digitalWrite(Red,LOW);
    digitalWrite(Green,HIGH);
    digitalWrite(Blue,LOW);
    Partido = true;
    start = false;
  }
  
  // Este bucle while bloqueante es parte de la lógica original.
  while (Partido == false && start == true) {
    if(contador1 == 2)
    {
      if(millis()-WinTime >= 5000)
      {
        lcd.clear();
        WinTime = millis();
      }
      lcd.setCursor(2,0);
      lcd.print("!!!GANA EL");
      lcd.setCursor(2,1);
      lcd.print("EQUIPO 1!!!");
    if(millis()-TimeRGB >= 20)
      {
        digitalWrite(Red,random(0,2));
        digitalWrite(Green,random(0,2));
        digitalWrite(Blue,1);
        TimeRGB = millis();
      }
    }
    if(contador2 == 2)
    {
      if(millis()-WinTime >= 5000)
      {
        lcd.clear();
        WinTime = millis();
      }
      lcd.setCursor(0,0);
      lcd.print("!!!GANA EL");
      lcd.setCursor(2,1);
      lcd.print("EQUIPO 2!!!");
      if(millis()-TimeRGB >= 20)
      {
        digitalWrite(Red,random(0,2));
        digitalWrite(Green,random(0,2));
        digitalWrite(Blue,1);
        TimeRGB = millis();
      }
    }
    if (gol1 == true) {
      digitalWrite(MOTOR_WIRE_1, HIGH);
      digitalWrite(MOTOR_WIRE_2, LOW);
      ledcWrite(ENABLE_PORT, 240);
      gol1 = false;
    }
    if (gol2 == true) {
      digitalWrite(MOTOR_WIRE_1, LOW);
      digitalWrite(MOTOR_WIRE_2, HIGH);
      ledcWrite(ENABLE_PORT, 240);
      gol2 = false;
    }
     if (digitalRead(Proximidad1) == LOW && digitalRead(Proximidad2) == LOW) {
      if(contador1 ==2 || contador2 == 2)
      {
        contador1 = 0;
        contador2 = 0;
      }
      ledcWrite(ENABLE_PORT, 0);
      digitalWrite(MOTOR_WIRE_1, LOW);
      digitalWrite(MOTOR_WIRE_2, LOW);
      start = false;
      Partido = false;
      gol1 = false;
      gol2 = false;
      // Salir del bucle while para reevaluar condiciones en loop()
      break; 
    }
  }

  if(Partido == true && start == false)
  {
    if(millis()-MostrarDelay >= 10000)
    {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("EQUIPO 1 ");
      lcd.setCursor(14, 0); lcd.print(contador1);
      lcd.setCursor(0, 1); lcd.print("EQUIPO 2 ");
      lcd.setCursor(14, 1); lcd.print(contador2);
      MostrarDelay = millis();
    }
  }
  
  if (digitalRead(Start) == HIGH)
  {
    start = true;
  }
  
  if(Partido == false && start == false)
  {
    digitalWrite(Red,HIGH);
    digitalWrite(Green,LOW);
    digitalWrite(Blue,LOW);
    if(millis()-MostrarDelay >= 10000)
    {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("EQUIPO 1 ");
      lcd.setCursor(14, 0); lcd.print(contador1);
      lcd.setCursor(0, 1); lcd.print("EQUIPO 2 ");
      lcd.setCursor(14, 1); lcd.print(contador2);
      MostrarDelay = millis();
    }
  }

  // Gol izquierdo
  if (!digitalRead(GOL_IZQ) && !toco1) Impulso_Detectado(toco1, ultimoTiempoDebounce1);
  if ((millis() - ultimoTiempoDebounce1 > debounceDelay) && digitalRead(GOL_IZQ) == LOW && (millis() - GoalTime > 1000))
    Aumentar_Puntuacion(contador1, toco1, 1, gol1);
  if ((millis() - ultimoTiempoDebounce1 > debounceDelay) && digitalRead(GOL_IZQ) == HIGH && toco1)
    Falsa_Alarma(toco1, ultimoTiempoDebounce1);

  // Gol derecho
  if (!digitalRead(GOL_DER) && !toco2) Impulso_Detectado(toco2, ultimoTiempoDebounce2);
  if ((millis() - ultimoTiempoDebounce2 > debounceDelay) && digitalRead(GOL_DER) == LOW && (millis() - GoalTime > 1000))
    Aumentar_Puntuacion(contador2, toco2, 2, gol2);
  if ((millis() - ultimoTiempoDebounce2 > debounceDelay) && digitalRead(GOL_DER) == HIGH && toco2)
    Falsa_Alarma(toco2, ultimoTiempoDebounce2);
}