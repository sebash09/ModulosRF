#include <SPI.h>
#include <RF24.h>
#include <DHT.h>

#define DHT_PIN 6  // Pin de datos del sensor DHT11

DHT dht(DHT_PIN, DHT11);

RF24 radio(9, 10);  // Pines 9 y 10 para CE y CSN respectivamente

void setup() {
  Serial.begin(115200);
  
  if (!radio.begin()) {
    Serial.println("Error al iniciar la radio NRF24");
    while (1);
  }

  radio.openWritingPipe(0xF0F0F0F0E1LL); // Dirección del canal de comunicación
  dht.begin();
}

void loop() {
  // Lectura del sensor DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Verificar si la lectura del sensor fue exitosa
  if (!isnan(humidity) && !isnan(temperature)) {
    // Construir un paquete de datos
    struct DataPacket {
      float humidity;
      float temperature;
    };

    DataPacket dataPacket = {humidity, temperature};

    // Enviar el paquete de datos a través de la radio NRF24
    radio.write(&dataPacket, sizeof(dataPacket));

    // Mostrar la información en el monitor serial
    Serial.print("Humedad: ");
    Serial.print(humidity);
    Serial.print("%, Temperatura: ");
    Serial.print(temperature);
    Serial.println("°C");
  } else {
    Serial.println("Error al leer el sensor DHT11");
  }

  delay(1000);
}
