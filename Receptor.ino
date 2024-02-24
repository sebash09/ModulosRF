#include <RF24.h>

RF24 radio(2, 4);  // Pines D2 y D4 para CE y CSN respectivamente

struct DataPacket {
  float humidity;
  float temperature;
};

void setup() {
  Serial.begin(115200);
  
  if (!radio.begin()) {
    Serial.println("Error al iniciar la radio NRF24");
    while (1);
  }

  radio.openReadingPipe(1, 0xF0F0F0F0E1LL); // Dirección del canal de comunicación
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    DataPacket dataPacket;
    radio.read(&dataPacket, sizeof(dataPacket));

    Serial.print("Humedad recibida: ");
    Serial.print(dataPacket.humidity);
    Serial.print("%, Temperatura recibida: ");
    Serial.print(dataPacket.temperature);
    Serial.println("°C");
  }
}
