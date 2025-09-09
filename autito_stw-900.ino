#include <NewPing.h>

// ----------------------------------------------------
// 1. Definición de Pines
// ----------------------------------------------------
// Nombres claros para cada componente conectado al Arduino.
// Luces de Freno (Rojas)
#define FRENO_IZQ_PIN 8
#define FRENO_DER_PIN 9
// Luces de Marcha (Verdes)
#define MARCHA_IZQ_PIN 10
#define MARCHA_DER_PIN 11

// Pines para el sensor de distancia (ultrasonido)
#define TRIG_PIN A0
#define ECHO_PIN A2
#define MAX_DISTANCE 200

// Configuraciones de velocidad del motor (no se usan en este código, pero están definidas)
#define MAX_SPEED 190 
#define MAX_SPEED_OFFSET 20

// Inicializa el sensor de ultrasonido
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

// Variables de estado del robot
boolean goesForward=false;
int distance = 100;
int speedSet = 0;
int IN1 = 7; //motor B
int IN2 = 6; //motor B
int IN3 = 5; //motor A
int IN4 = 4; //motor A

// ----------------------------------------------------
// 2. Función Setup: Se ejecuta una vez al iniciar
// ----------------------------------------------------
void setup() {
  // Realiza varias lecturas iniciales del sensor para estabilizar
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  
  // Configura los pines de los motores como SALIDAS
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN3, OUTPUT);
  
  // Configura los pines de las luces como SALIDAS
  pinMode(FRENO_IZQ_PIN, OUTPUT);
  pinMode(FRENO_DER_PIN, OUTPUT);
  pinMode(MARCHA_IZQ_PIN, OUTPUT);
  pinMode(MARCHA_DER_PIN, OUTPUT);
}

// ----------------------------------------------------
// 3. Función Loop: Se ejecuta repetidamente
// ----------------------------------------------------
void loop() {
  // Lógica principal: si hay un obstáculo cerca, detente y retrocede.
  if (distance <= 20) {
    moveStop();      // Detiene el robot y enciende las luces de freno
    delay(100);
    moveBackward();  // Hace que el robot retroceda y enciende las luces de freno
    delay(800);
    moveStop();      // Vuelve a detenerse y enciende las luces de freno
    delay(200);
  } else {
    moveForward();   // Si no hay obstáculos, avanza y enciende las luces de marcha
  }
  distance = readPing(); // Lee la distancia para la siguiente iteración
}

// ----------------------------------------------------
// 4. Funciones de Control del Robot
// ----------------------------------------------------

// Lee la distancia del sensor y devuelve el valor en cm
int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    // Si la lectura es 0, significa que no detectó nada (fuera de rango).
    // Asignamos un valor grande para que el robot siga avanzando.
    cm = 250;
  }
  return cm;
}

// Detiene el robot
void moveStop() {
  digitalWrite(IN4, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN1, LOW);
  
  // Enciende las luces de freno (rojas)
  digitalWrite(FRENO_IZQ_PIN, HIGH);
  digitalWrite(FRENO_DER_PIN, HIGH);

  // Apaga las luces de marcha (verdes)
  digitalWrite(MARCHA_IZQ_PIN, LOW);
  digitalWrite(MARCHA_DER_PIN, LOW);
}

// Mueve el robot hacia adelante
void moveForward() {
  if (!goesForward) {
    goesForward = true;
    digitalWrite(IN3, HIGH);
    digitalWrite(IN1, HIGH);
    delay(5);
  }
  
  // Apaga las luces de freno
  digitalWrite(FRENO_IZQ_PIN, LOW);
  digitalWrite(FRENO_DER_PIN, LOW);
  
  // Enciende las luces de marcha (verdes) en secuencia, simulando movimiento
  digitalWrite(MARCHA_IZQ_PIN, HIGH);
  digitalWrite(MARCHA_DER_PIN, LOW);
  delay(150);
  digitalWrite(MARCHA_IZQ_PIN, LOW);
  digitalWrite(MARCHA_DER_PIN, HIGH);
  delay(150);
}

// Mueve el robot hacia atrás
void moveBackward() {
  goesForward = false;
  digitalWrite(IN4, HIGH);
  digitalWrite(IN2, HIGH);
  delay(5);
  
  // Enciende las luces de freno para indicar que está retrocediendo
  digitalWrite(FRENO_IZQ_PIN, HIGH);
  digitalWrite(FRENO_DER_PIN, HIGH);
  
  // Apaga las luces de marcha
  digitalWrite(MARCHA_IZQ_PIN, LOW);
  digitalWrite(MARCHA_DER_PIN, LOW);
}
