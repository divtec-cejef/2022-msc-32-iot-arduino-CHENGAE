#include "DHT.h"
#include "timer.h"
#include <ArduinoLowPower.h>
#include <SigFox.h>

#define DHTPIN 5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

//Initialisation des constantes
const int PIN_BTN = 1;
int lastButtonState = 0;
Timer timer;


void setup() {
  Serial.begin(9600);

  // Définie le mode des boutons
  pinMode(PIN_BTN, INPUT);

  // Setup le timer
  timer.setInterval(3600000);
  timer.setCallback(sendTemperature);
  timer.start();

  dht.begin();
  SigFox.begin();
}


void loop() {
  bool currentButtonState = digitalRead(PIN_BTN);
  if (lastButtonState != currentButtonState && currentButtonState == HIGH) {
    sendTemperature();
  }
  lastButtonState = currentButtonState;

  timer.update(); // tick du timer
}

// Récupère la température et l'humidité puis envoie les données.
void sendTemperature() {
  float temperature = dht.readTemperature();
  float humidite = dht.readHumidity();

  SigFox.beginPacket();
  SigFox.write(temperature);
  SigFox.write(humidite);
  SigFox.endPacket();
}
