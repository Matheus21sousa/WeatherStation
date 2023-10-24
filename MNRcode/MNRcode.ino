// MADE BY: Matheus Martins de Sousa //
//
// O presente projeto é aberto a correções
// Use com sabedoria!
// :)
//

// DESCRIÇÃO: Uso de sensores hall para determinar a pluviometria; 

// BIBLIOTECAS UTILIZADAS
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// DEFINIÇÃO DE PORTAS
#define DHTPIN 7
#define hallPinD A5 
#define hallPinE A4
// Portas hall (model 49e):
// 5V - Esquerda
// GND - Central
// DATA - Direita

// Selecione a seguir o sensor que está sendo utilizado, retirando as // no início da linha:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// CONFIGURAÇÃO SENSOR DHT - "TYPE"
DHT_Unified dht(DHTPIN, DHTTYPE);

// VARIÁVEL DE TEMPO
uint32_t delayMS;

// VARIÁVEIS GLOBAIS UTILIZADAS
int oldSignalE = 0;                      
int oldSignalD = 0;
int newSignalE = 0;
int newSignalD = 0;

int deltaE;
int ABSdE;
int deltaD;
int ABSdD;

int b_inst = 0;
int b_acu = 0;
float v = 13.2;
float chuva_inst = 0;
float chuva_acu = 0;
int side = 0; 
float last_refresh = 0;
float time_now = 0;
float delta_time = 0;

// ÍNDICE DE VARIÁVEIS:
// 1. "oldSignalX"      atribuir ao valor da leitura Hall distanciado do ímã
// 2. "newSignalX"      atribuir ao valor lido instantaneamente
// 3. "deltaX"          atribuir a diferença entre a leitura instantânea e a leitur base (newSignalX - oldSignalX) 
// 4. "ABSdX"           módulo do valor obtido em "deltaX"
// 5. "b"               número de basculadas
// 6. "v"               volume de água por basculada, em mL
// 7. "chuva_inst"      valor relativo á multiplicação do número de basculadas (b) * volume por basculada (v), em mL
// 8. "chuva_acu"       somatório de todas as basculadas ao longo do dia, em mm
// 8. "side"            variável que identifica o lado a qual a báscula está pendendo [1 = esquerda / 2 = direita] 
// 9. "last_refresh"    variável que armazena um valor em microsegundos a qual representa a hora da ultima atualização da variável "b"
// 10. "time_now"       variável responsável por armazenar o tempo adquirido por meio da função {milis()}; 
// 11. "delta_time"     variável responsável por receber o valor advindo da operação de subtração entre o "time_now" e "last_refresh"

// FUNÇÕES UTILIZADAS
void bascula();                   // Função que contabiliza basculadas 
void dados();                     // Função que imprime dados do DHT e da báscula 
void latencia();                  // Função que quantifica a diferênça de tempo entre um conjunto de dados e outro
void espera();                    // Função delay 

// COMUTAR EXECUÇÃO DE DIFERENTES FUNÇÕES
unsigned long tempoB=0;
unsigned long tempoD=0;
unsigned long tempoL=0;

bool comutarB = false;
bool comutarD = false;
bool comutarL = false;

// VOID SETUP
void setup() {
  pinMode(hallPinE, INPUT_PULLUP);
  pinMode(hallPinD, INPUT_PULLUP);
  Serial.begin(9600);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
}

// VOID LOOP
void loop(){
  bascula();
  dados();
  latencia();
  espera();
}

// FUNÇÃO 01 - Contabiliza basculadas
void bascula(){
  if(comutarB){
  
  // CALCULO
  chuva_inst = b_inst*v;
  chuva_acu = 10*b_acu*v/157.49;

  // INICIO:
    if (side == 0){
      newSignalE = analogRead(hallPinE);
      newSignalD = analogRead(hallPinD);
      oldSignalE = newSignalE;
      oldSignalD = newSignalD;
      side++;
    }

    //Sempre deve começar basculada para a esquerda
    else if (side == 1){ 
      newSignalD = analogRead(hallPinD); 
      deltaD = oldSignalD - newSignalD;
      ABSdD = abs(deltaD); 

      if (ABSdD >=30){
        side++;
        b_inst++;
        b_acu++;
        newSignalE = analogRead(hallPinE);
        oldSignalE = newSignalE;
        last_refresh = millis();
      }
    }

    //Bascula para o lado direito
    else if (side == 2){ 
      newSignalE = analogRead(hallPinE); 
      deltaE = oldSignalE - newSignalE;
      ABSdE = abs(deltaE); 

      if (ABSdE >=30){
        side--;
        b_inst++;
        b_acu++;
        newSignalD = analogRead(hallPinD);
        oldSignalD = newSignalD;
        last_refresh = millis();
      }
    }

  // COMUTA FUNÇÃO ACIMA
  tempoB=millis();
  comutarB = false;    
  }
}

// FUNÇÃO 02 - Lê DHT e imprimir dados de umidade, temperatura e chuva 
void dados(){
  if(comutarD){
    sensors_event_t event;
    Serial.print("{\"deviceId\":1,\"data\":");
    
    // ACCUMULATED RAIN
    Serial.print("{\"chuvaAcumulada\":");
    Serial.print(chuva_acu, 2);
    Serial.print(", ");

    // INSTANT RAIN
    Serial.print("\"chuvaInstantanea\":");
    Serial.print(chuva_inst, 2);
    Serial.print(", ");

    //Serial.print("side: ");
    //Serial.print(side);
    //Serial.print(", ");

    // TEMPERATURA
    dht.temperature().getEvent(&event);
    Serial.print("\"temperatura\":");
    Serial.print(event.temperature);
    Serial.print(", ");

    // HUMIDADE
    dht.humidity().getEvent(&event);
    Serial.print("\"umidade\":"); 
    Serial.print(event.relative_humidity);
    Serial.println("}}");

    // TIME
    //Serial.print(delta_time, 5);
    //Serial.print(" - "); 
    //Serial.println(time_now, 5);
    
    // COMUTA FUNÇÃO ACIMA
    tempoD=millis();
    comutarD = false;    
  }
}

// FUNÇÃO 3 - LATENCIA DE DADOS
void latencia(){
  if(comutarL){
    
    time_now = millis();
    delta_time = time_now - last_refresh;
    
    // CALCULA TEMPO ENTRE MEDIDAS: SE SUPERIOR A X, ZERA O NÚMERO DE BASCULADAS "b"
    if(delta_time > 20000){  // 20 segundos sem novos dados zera a leitura instantanea
      b_inst = 0;
      delta_time = 0; 
    }

    // COMUTA FUNÇÃO ACIMA
    tempoL=millis();
    comutarL = false;    
  }
}

// Definições de intervalo (delay) de cada atividade //
void espera(){
  // FUNÇÃO 01 - BASCULA
  if(!comutarB && millis() - tempoB>=200){
    comutarB = true;
  }
  // FUNÇÃO 02 - DHT E DADOS
  if(!comutarD && millis() - tempoD>=5000){
    comutarD = true;
  }
  // FUNÇÃO 03 - LATÊNCIA
  if(!comutarL && millis() - tempoL>=500){
    comutarL = true;
  }
}
