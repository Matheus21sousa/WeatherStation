// DHT Temperature & Humidity Sensor
// Baseado na no repositório adafruit/DHT-sensor-library
// https://github.com/adafruit/DHT-sensor-library/blob/master/examples/DHT_Unified_Sensor/DHT_Unified_Sensor.ino

// São necessárias as seguintes bibliotecas:
// - DHT Sensor: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Sensor: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Define o pino de conexão com o arduíno; 
#define DHTPIN 7

// Selecione a seguir o sensor que está sendo utilizado, retirando as // no início da linha:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// Configuração do sensor DHT - pinagem e tipagem //
DHT_Unified dht(DHTPIN, DHTTYPE);

// Variável de delay //
uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  // Inicializa o dispositivo //
  dht.begin();
  // Irá mostrar as definições do sensor de temperatura //
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Sensor de temperatura"));
  Serial.print  (F("Tipo do sensor: ")); Serial.println(sensor.name);
  Serial.print  (F("Versão do driver:  ")); Serial.println(sensor.version);
  Serial.print  (F("ID único:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Malor máx.:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Valor min.:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolução:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  
  // Irá mostrar as definições do sensor de umidade //
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Sensor de umidade"));
  Serial.print  (F("Tipo do sensor: ")); Serial.println(sensor.name);
  Serial.print  (F("Versão do driver:  ")); Serial.println(sensor.version);
  Serial.print  (F("ID único:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Valor máx.:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Valor min.:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolução:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  
  // Instaura delay entre as leituras com base na determinação de delay minimo (1000 segundos) do sensor //
  // Para aumentar o delay, reduza o denominador //
  // Ex. 01: 1000"/50 -> delay de 20" // Ex. 02: 1000"/10 -> delay de 100" //
  delayMS = sensor.min_delay / 200; 
}

void loop() {
  // Chama o delay entre as definições e o início das leituras //
  delay(delayMS);
  // Realiza a leitura da temperatura e "printa" o valor //
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Erro ao ler a temperatura!"));
  }
  else {
    Serial.print(F("Temperatura: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Realiza a leitura da umidade e "printa" o valor //
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Erro ao ler a umidade!"));
  }
  else {
    Serial.print(F("Humidade: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}
