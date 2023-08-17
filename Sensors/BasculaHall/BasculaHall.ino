// MADE BY: Matheus Martins de Sousa //
//
// O presente projeto é aberto a correções
// Use com sabedoria!
// :)
//

// DESCRIÇÃO: Uso de sensores hall para determinar a pluviometria; 

// DEFINIÇÃO DE PORTAS
const int hallPinE = A7;
const int hallPinD = A14;
// Portas hall (model 49e):
// 5V - Esquerda
// GND - Central
// DATA - Direita

// VARIÁVEIS GLOBAIS UTILIZADAS
int oldSignalE = 0;                      
int oldSignalD = 0;
int newSignalE = 0;
int newSignalD = 0;

int deltaE;
int ABSdE;
int deltaD;
int ABSdD;

int b = 0;
int side = 0; 

// ÍNDICE DE VARIÁVEIS:
// 1. "oldSignalX"      atribuir ao valor da leitura Hall distanciado do ímã
// 2. "newSignalX"      atribuir ao valor lido instantaneamente
// 3. "deltaX"          atribuir a diferença entre a leitura instantânea e a leitur base (newSignalX - oldSignalX) 
// 4. "ABSdX"           módulo do valor obtido em "deltaX"
// 5. "b"               número de basculadas
// 6. "side"            variável que identifica o lado a qual a báscula está pendendo [1 = esquerda / 2 = direita]

// COMUTAR TEMPO
unsigned long tempoH=0; // Variavel para armazenar o tempo do Hall
unsigned long tempoD=0; // Variavel para armazenar o tempo do Dados
bool comutarH = false;
bool comutarD = false;

// VOID SETUP 
void setup(){
  Serial.begin(9600);
}

// VOID LOOP 
void loop(){
  hall();
  dados();
  espera();
}

// FUNÇÃO 01: Leitura dos sensores Hall
void hall(){
  if(comutarH){
    
    //Starting:
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
        b++;
        newSignalE = analogRead(hallPinE);
        oldSignalE = newSignalE;
      }
    }

    //Bascula para o lado direito
    else if (side == 2){ 
      newSignalE = analogRead(hallPinE); 
      deltaE = oldSignalE - newSignalE;
      ABSdE = abs(deltaE); 

      if (ABSdE >=30){
        side--;
        b++;
        newSignalD = analogRead(hallPinD);
        oldSignalD = newSignalD;
      }
    }
    
    //Comutando
    tempoH=millis();
    comutarH=false; 
  }
}

// FUNÇÃO 02: Printando dados para "checkagem"
void dados(){
  if(comutarD){
    Serial.print("hall E:");
    Serial.print(oldSignalE); 
    Serial.print("  ")/
    Serial.print(newSignalE);
    Serial.print(" // hall D:");
    Serial.print(oldSignalD); 
    Serial.print("  ");
    Serial.print(newSignalD);
    Serial.print(" // Delta:");
    Serial.print(ABSdE);
    Serial.print("  ");
    Serial.print(ABSdD);
    Serial.print(" // Basculadas:");
    Serial.print(b);
    Serial.print(" // Lado:");
    Serial.println(side);

    //Comutando
    tempoD=millis();
    comutarD=false;   
  }
}

// FUNÇÃO 03: Comuta a leitura dos sensores e o print na porta serial
void espera(){
  if(!comutarH && millis()-tempoH>=200){      //Comuta Hall
    comutarH = true;
  }
  
  if(!comutarD && millis()-tempoD>=1000){     //Comuta dados
    comutarD = true;
  }
}