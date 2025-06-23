# Final_Project_IMT231
Un proyecto de la materia Programación Superior [IMT-231] del Departamento de Ingeniería Mecatrónica de la Universidad Católica Boliviana "San Pablo".

## 📜 Descripción del Proyecto
Brain Ball es un juego mecatrónico interactivo para dos jugadores controlado directamente por la mente. El sistema utiliza Interfaces Cerebro-Computadora (BCI) para traducir el estado cognitivo de los jugadores (específicamente su nivel de atención enfocada) en el movimiento físico de una pelota sobre una pista lineal.
Utilizando diademas comerciales de Electroencefalografía (EEG), el sistema cuantifica el nivel de atención de cada jugador. Un microcontrolador ESP32 actúa como unidad central, recibiendo los datos de atención de forma inalámbrica, ejecutando un algoritmo comparativo y generando señales PWM para controlar un motor DC. El jugador que demuestra un mayor nivel de atención sostenida gana el control, moviendo la pelota hacia la meta del oponente.
El proyecto integra un sistema completo de gestión de juego y retroalimentación, que incluye un marcador LCD, indicadores de gol audiovisuales y un mecanismo de reinicio automático de la pelota. Este trabajo demuestra con éxito una aplicación práctica y atractiva de la tecnología de neurofeedback y BCI en un entorno competitivo y no clínico.

## ⚙️ Cómo Funciona

1.  **Captura de Señal EEG**: Dos diademas NeuroSky MindWave Mobile 2 leen las ondas cerebrales de los jugadores y, mediante algoritmos propietarios, calculan un valor de "Atención" de 0 a 100.
2.  **Transmisión Inalámbrica**: Cada diadema envía su valor de "Atención" a un módulo Bluetooth HC-05 dedicado.
3.  **Procesamiento Central**: El ESP32 recibe los datos de ambos módulos HC-05 a través de dos puertos serie de hardware separados.
4.  **Lógica de Control Diferencial**: Un algoritmo en el ESP32 compara los dos valores de atención. El jugador con el valor más alto obtiene el control. La diferencia entre los valores determina la velocidad del movimiento.
5.  **Actuación Electromecánica**: El ESP32 genera una señal PWM que se envía a un driver de motor H-Bridge (L293D). Este driver controla la velocidad y dirección del motor DC, que a su vez mueve la pelota a lo largo de la pista.
6.  **Gestión de Estado**: Los sensores en la pista informan al ESP32 si se ha marcado un gol o si la pelota ha vuelto al centro, permitiendo que el sistema actualice el marcador, active las señales de feedback y reinicie la ronda.

## 🛠️ Componentes de Hardware
### Lista de Componentes

| **Categoría**               | **Componente**                          | **Cantidad** | **Detalle**                                         |
|----------------------------|-----------------------------------------|--------------|-----------------------------------------------------|
| **Control y Procesamiento**| ESP32 DEVKIT V1                         | 1            | Microcontrolador principal                          |
|                            | Diadema NeuroSky MindWave Mobile 2     | 2            | Adquisición de señales EEG                          |
|                            | Módulo Bluetooth HC-05                  | 2            | Comunicación inalámbrica                            |
| **Actuadores y Drivers**   | Driver Motor H-Bridge L293D             | 1            | Control de dirección y velocidad del motor          |
|                            | Motor DC con caja reductora            | 1            | Actuador principal del movimiento                   |
| **Sensores**               | Sensor de Proximidad Sharp GP2Y0A41     | 2            | Detección de la posición central de la pelota       |
|                            | Interruptor de Límite (Final de carrera)| 2            | Detección de goles                                  |
|                            | Pulsador                                | 1            | Botón de inicio del juego                           |
| **Interfaz y Feedback**    | Pantalla LCD 16x2 con módulo I2C        | 1            | Marcador y estado del juego                         |
|                            | Módulo DFPlayer Mini MP3                | 1            | Reproducción de audio                               |
|                            | Altavoz 3W 4-Ohm                        | 1            | Salida de audio                                     |
|                            | Tira LED RGB (o LED individual)         | 1            | Indicador visual de gol                             |
| **Alimentación y Otros**   | Fuente DC 12V 2A                        | 1            | Fuente de alimentación principal                    |
|                            | Regulador de Voltaje LM7805/LM2596      | 1            | Suministra 5V a los componentes lógicos             |
|                            | Transistores TIP41                      | 3            | Para controlar la tira LED RGB                      |
|                            | Resistencias y Capacitores   

## 📂 Arquitectura del Software
El software está desarrollado en el framework de Arduino para ESP32 y sigue una arquitectura modular para facilitar la legibilidad y el mantenimiento.

### Arduino_code/

│

├── pruebaproyectoprograsuperior.ino  # Archivo principal: Lógica de juego, inicialización y gestión de tareas RTOS

│

├── Config.h                          # Define pines, constantes y parámetros del sistema

│

├── Brainwave.h                       # Declaración de la clase y funciones para la librería Brainwave

├── Brainwave.cpp                     # Implementación: Lee y parsea datos del NeuroSky, controla el motor

│

├── Gol.h                             # Declaración de la clase y funciones para la librería Gol

├── Gol.cpp                           # Implementación: Gestiona eventos de gol y reseteo

-   **`pruebaproyectoprograsuperior.ino`**: Es el punto de entrada. Configura el hardware, inicializa los objetos y crea dos tareas separadas de FreeRTOS, una para cada jugador, garantizando una lectura de datos y control del motor concurrentes y responsivos.
-   **`Config.h`**: Centraliza todas las asignaciones de pines y constantes del sistema (frecuencia PWM, baud rates, etc.), haciendo que las modificaciones de hardware sean sencillas.
-   **`Brainwave` library**: Encapsula la lógica para leer el flujo de datos serie del HC-05, extraer el valor de "Atención" y traducirlo en comandos de control para el motor (velocidad y dirección) usando un mutex para evitar condiciones de carrera.
-   **`Gol` library**: Contiene la lógica para gestionar los eventos de fin de carrera. Cuando se detecta un gol, actualiza el marcador, activa el feedback audiovisual y gestiona las banderas de estado del juego.

## 🚀 Instalación y Puesta en Marcha
### Hardware
Ensamblar el circuito electrónico siguiendo el esquemático proporcionado. Construir la estructura mecánica con la pista, el carro para la pelota y el sistema de poleas y correa.
### Configuración del IDE
Instalar el IDE de Arduino.
### Añadir el soporte para tarjetas ESP32 en el Gestor de URLs Adicionales de Tarjetas: 
- https://dl.espressif.com/dl/package_esp32_index.json.
- Instalar la placa "ESP32 Dev Module" desde el Gestor de Tarjetas.
### Librerías
Instalar las siguientes librerías desde el Gestor de Librerías del IDE de Arduino:
- LiquidCrystal_I2C
- DFRobotDFPlayerMini
### Bluetooth
Antes de integrar los módulos HC-05 en el circuito final, es necesario configurarlos usando comandos AT para que operen en modo esclavo con una velocidad de 57600 baudios.
### Carga del Código
- Abrir el archivo pruebaproyectoprograsuperior.ino en el IDE de Arduino.
- Seleccionar la placa "ESP32 Dev Module" y el puerto COM correcto.
- Cargar el código en el ESP32.
## ▶️ Uso
1. Encender el sistema. La pantalla LCD mostrará un mensaje de bienvenida y el marcador inicial.
2. Los dos jugadores deben encender y colocarse sus respectivas diademas NeuroSky.
3. Presionar el botón de "Start" para comenzar la partida.
4. ¡A concentrarse! El jugador que mantenga un nivel de atención más alto moverá la pelota hacia el lado contrario.
5. El primer jugador en llevar la pelota hasta el final de la pista del oponente anota un punto. El sistema reproducirá un sonido de gol, mostrará una animación de luz y reiniciará la pelota en el centro para la siguiente ronda.
