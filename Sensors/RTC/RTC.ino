// Bibliotecas utilizadas //
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "RTClib.h"

// Descrição das portas //
// RTC = Portas I²C
//       SCL = Porta 01 I²C
//       SDA = Porta 02 I²C

// Inicializa o RTC //
RTC_DS3231 rtc;

// Define [nº de dias da semana] [nº de meses do ano] e nome dos dias da semana //
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

// Variável de delay //
uint32_t delayMS;

void setup() {
  Serial.begin(9600);

  // RTC - Permite testar se está funcionando //
  if (! rtc.begin()) { 
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while (1); //SEMPRE ENTRE NO LOOP
  }
  if (rtc.lostPower()) { //Para inicializar o RTC pela primeira vez, utilize um dos dois códigos abaixo: //
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Captura do computador as informações de data e hora //
    rtc.adjust(DateTime(2018, 9, 29, 15, 00, 45)); // Inserir manualmente [ano, mês, dia, hora, minutos, segundos] //
  }
}

// Execução de atividades simultâneas //
void loop() {
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

    delay(5000);
}