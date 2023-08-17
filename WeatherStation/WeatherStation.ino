/* ESTAÇÃO METEOROLÓGICA
    By: Matheus M. Sousa 
    O presente projeto é aberto a correções
    Use com sabedoria!
    :)
*/ 

// BIBLIOTECAS UTILIZADAS
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DFRobot_SHT20.h"
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>

// DEFINIÇÃO DE PORTAS
#define ampolae 2
#define ampolad 4
#define chipSelect 5
#define DHTPIN 7
#define DHTTYPE DHT11
//#define DHTTYPE DHT22 (AM2302)
//#define DHTTYPE DHT21 (AM2301)

// Descrição das portas //
// RTC:
//       SCL = Porta 01 I²C
//       SDA = Porta 02 I²C
// SHT:
//       SCL = Porta A5 I²C (Amarelo)
//       SDA = Porta A4 I²C (Verde)
// Leitor SD:
//       CS = Porta 05
//       SCK = Porta 13
//       MOSI = Porta 11
//       MISO = Porta 12

// Inicializa o RTC //
RTC_DS3231 rtc;

// Inicializa o SHT //
DFRobot_SHT20 sht20(&Wire, SHT20_I2C_ADDR);

// Define [nº de dias da semana] [nº de meses do ano] e nome dos dias da semana //
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

// Funções utilizadas //
void bascula();            // Função que contabiliza basculadas //
void dados();              // Função que imprime dados do DHT e da báscula //
void espera();             // Função delay //

unsigned long tempoB = 0;
unsigned long tempoD = 0;

// Comutando as funções //
bool comutarB = false;
bool comutarD = false;
// ComutarB = Função da báscula //
// ComutarD = Função de imprimir dados //

// Configuração do sensor DHT - pinagem e tipagem //
DHT_Unified dht(DHTPIN, DHTTYPE);

// Variável de delay //
uint32_t delayMS;

// VARIÁVEIS UTILIZADAS 
int b = 0;        // Nº de basculadas 
float v = 1;      // Volume da basculada - Deve ser calibrado
int side = 0;
// lado = 0 - Início 
// lado = 1 - Esquerda 
// lado = 2 - Direita 

void setup() {
  pinMode(ampolae, INPUT_PULLUP); //Define pino de entrada
  pinMode(ampolad, INPUT_PULLUP); //Define pino de entrada
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  // RTC - Permite testar se está funcionando //
  if (! rtc.begin()) { 
    Serial.println("RTC não encontrado"); // Imprime, em caso de erro
    while (1); // Fica em loop
  }
  if (rtc.lostPower()) { //Para inicializar o RTC pela primeira vez, utilize um dos dois códigos abaixo: //
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Captura do computador as informações de data e hora //
    // rtc.adjust(DateTime(2023, 7, 27, 23, 50, 00)); // Inserir manualmente [ano, mês, dia, hora, minutos, segundos] //
  }

  // SHT - Inicia o sensor
  sht20.initSHT20();
  sht20.checkSHT20();

  // Leitor SD - Permite testar se está funcionando //
  if (!SD.begin(chipSelect)){
    Serial.print("Error to acess the card!");
    Serial.print("Verify your card/conections and reset");
    return;
  }
}

// Execução de atividades simultâneas //
void loop() {
  bascula();
  dados();
  espera();
}

// Atividade 01 - Contabilização de basculadas //
void bascula() {
  if (comutarB) {
    // Início // Para lado == 0 //
    if (!digitalRead(ampolad) && side == 0) {
      b++;
      side++;
      side++;
    }
    else if (!digitalRead(ampolae) && side == 0) {
      b++;
      side++;
    }
    // Execução // Para lado != 0 //
    else if (!digitalRead(ampolad) && side == 1) {
      b++;
      side++;
    }
    else if (!digitalRead(ampolae) && side == 2) {
      b++;
      side--;
    }
    // Comuta a atividade 01 //
    tempoB = millis();
    comutarB = false;
  }
}

// Atividade 02 - Lê DHT e imprime: Data, Hora, Umidade, Pluviosidade //
void dados() {
  if (comutarD) {

    // Chama a função do RTC //
    DateTime now = rtc.now(); // Chama função do RTC
    Serial.print("Data: "); 
    Serial.print(now.day(), DEC); 
    Serial.print('/'); 
    Serial.print(now.month(), DEC); 
    Serial.print('/'); 
    Serial.print(now.year(), DEC); 
    Serial.print(',');    
    Serial.print(" Hora: "); 
    Serial.print(now.hour(), DEC); 
    Serial.print(':'); 
    Serial.print(now.minute(), DEC);
    Serial.print(':'); 
    Serial.print(now.second(), DEC); 
    Serial.print(','); 

    // Chama a função do DHT //
    sensors_event_t event;
    dht.temperature().getEvent(&event); // Lê temperatura do ar
    if (isnan(event.temperature)) {
      Serial.print(F("Erro ao ler a temperatura!"));
    }
    else {
      Serial.print(F(" Temp. do Ar: "));
      Serial.print(event.temperature);
      Serial.print(F("°C"));
      Serial.print(','); 
    }
    dht.humidity().getEvent(&event); // Lê umidade do ar
    if (isnan(event.relative_humidity)) {
      Serial.print(F("Erro ao ler a umidade!"));
    }
    else {
      Serial.print(F(" Umidade do Ar: "));
      Serial.print(event.relative_humidity);
      Serial.print(F("%"));
      Serial.print(','); 
    }

    // Chama o SHT 
    float soilHum = sht20.readHumidity();
    float soilTem = sht20.readTemperature();
    Serial.print(F(" Temp. do solo: ")); Serial.print(soilTem); Serial.print(F("°C,"));
    Serial.print(F(" Umidade do solo: ")); Serial.print(soilHum); Serial.print(F("%,"));

    // Imprime variável chuva //
    float chuva = b * v;
    Serial.print(F(" Chuva atual: ")); Serial.print(chuva); Serial.print(F(" mm"));
    Serial.print(','); 
    Serial.println();

    // Gravação de dados no SD //
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile){
      // Grava: Data e horário//
      dataFile.print("Data: "); 
      dataFile.print(now.day(), DEC); 
      dataFile.print('/'); 
      dataFile.print(now.month(), DEC); 
      dataFile.print('/'); 
      dataFile.print(now.year(), DEC); 
      dataFile.print(',');    
      dataFile.print(" Hora: "); 
      dataFile.print(now.hour(), DEC); 
      dataFile.print(':'); 
      dataFile.print(now.minute(), DEC);
      dataFile.print(':'); 
      dataFile.print(now.second(), DEC); 
      dataFile.print(',');

      // Grava: Temperatura do Ar //
      dht.temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        dataFile.print(F("Erro ao ler a temperatura!"));
      }
      else {
        dataFile.print(F(" Temp. do ar: ")); dataFile.print(event.temperature); dataFile.print(F("°C,"));
      }

      // Grava: Umidade do Ar //
      dht.humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        dataFile.print(F("Erro ao ler a umidade!"));
      }
      else {
        dataFile.print(F(" Umidade do ar: ")); dataFile.print(event.relative_humidity); dataFile.print(F("%,"));
      }

      // Grava: Temperatura do Solo //
      dataFile.print(F(" Temp. do solo: ")); dataFile.print(soilTem); dataFile.print(F("°C,"));

      // Grava: Umidade do Solo //
      dataFile.print(F(" Umidade do solo: ")); dataFile.print(soilHum); dataFile.print(F("%,"));

      // Grava: Pluviometria //
      dataFile.print(F(" Chuva atual: ")); dataFile.print(chuva); dataFile.print(F(" mm,"));
      dataFile.println();

      //Fecha o arquivo
      dataFile.close();
    }

    // Comuta atividade 02 //
    tempoD = millis();
    comutarD = false;
  }
}

// Definições de intervalo (delay) de cada atividade //
void espera() {
  if (!comutarB && millis() - tempoB >= 200) {
    comutarB = true;
  }
  if (!comutarD && millis() - tempoD >= 2000) {
    comutarD = true;
  }
}