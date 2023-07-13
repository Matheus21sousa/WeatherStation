// MADE BY: Matheus Martins de Sousa //
//
// O presente projeto é aberto a correções
// Use com sabedoria!
// :)
//

// Bibliotecas utilizadas //
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include "RTClib.h"

// Define a pinagem de conexão com o arduíno;
#define DHTPIN 7
#define ampolad 2
#define ampolae 4
#define DHTTYPE DHT11
//#define DHTTYPE DHT22 (AM2302)
//#define DHTTYPE DHT21 (AM2301)

// Descrição das portas //
// DHT = Porta 07
// RTC = Portas I²C
//       SCL = Porta 01 I²C
//       SDA = Porta 02 I²C

// Inicializa o RTC //
RTC_DS3231 rtc;

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

// Definições de variáveis //
int b = 0; // Nº de basculadas //
float v = 1; // Volume da basculada //
int side = 0;
// lado = 0 - Início //
// lado = 1 - Esquerda //
// lado = 2 - Direita //

void setup() {
  pinMode(ampolae, INPUT_PULLUP); //Define pino de entrada
  pinMode(ampolad, INPUT_PULLUP); //Define pino de entrada
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);

  // RTC - Permite testar se está funcionando //
  if (! rtc.begin()) { 
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while (1); //SEMPRE ENTRE NO LOOP
  }
  if (rtc.lostPower()) { //Para inicializar o RTC pela primeira vez, utilize um dos dois códigos abaixo: //
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Captura do computador as informações de data e hora //
    // rtc.adjust(DateTime(2018, 9, 29, 15, 00, 45)); // Inserir manualmente [ano, mês, dia, hora, minutos, segundos] //
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
    //
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
    //
    else if (!digitalRead(ampolad) && side == 1) {
      b++;
      side++;
    }
    else if (!digitalRead(ampolae) && side == 2) {
      b++;
      side--;
    }
    tempoB = millis();
    comutarB = false;
  }
}

// Atividade 02 - Lê DHT e imprime: Data, Hora, Umidade, Pluviosidade //
void dados() {
  if (comutarD) {
    DateTime now = rtc.now(); // Chama função do RTC
    Serial.print("Data: "); 
    Serial.print(now.day(), DEC); 
    Serial.print('/'); 
    Serial.print(now.month(), DEC); 
    Serial.print('/'); 
    Serial.print(now.year(), DEC); 
    Serial.print(',');    
    Serial.print(" Horas: "); 
    Serial.print(now.hour(), DEC); 
    Serial.print(':'); 
    Serial.print(now.minute(), DEC);
    Serial.print(':'); 
    Serial.print(now.second(), DEC); 
    Serial.print(','); 

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.print(F("Erro ao ler a temperatura!"));
    }
    else {
      Serial.print(F(" Temperatura: "));
      Serial.print(event.temperature);
      Serial.print(F("°C"));
      Serial.print(','); 
    }

    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.print(F("Erro ao ler a umidade!"));
    }
    else {
      Serial.print(F(" Umidade: "));
      Serial.print(event.relative_humidity);
      Serial.print(F("%"));
      Serial.print(','); 
    }

    // Imprime variável chuva //
    float chuva = b * v;
    Serial.print(F(" Chuva atual: ")); Serial.print(chuva); Serial.print(F(" mm"));
    Serial.print(','); 
    Serial.println();

    // Comuta atividade 2 //
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
