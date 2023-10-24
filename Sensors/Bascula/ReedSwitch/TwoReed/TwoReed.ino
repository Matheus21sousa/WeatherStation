#define ampolad 2
#define ampolae 4

// Define número de basculadas para 0 ao iniciar //
int b = 0; 
int side = 0; 
// lado = 0 - Início //
// lado = 1 - Esquerda //
// lado = 2 - Direita //

// Define volume da basculada - MODIFICAR COM TESTES //
float v = 3.2;

void setup(){
  pinMode(ampolae, INPUT_PULLUP); //Define pino de entrada
  pinMode(ampolad, INPUT_PULLUP); //Define pino de entrada
  Serial.begin(9600); //Inicia porta serial
  Serial.println(F("------------------------------------"));
  Serial.println (F("Pluviômetro"));
  Serial.print (F("Chuva acumulada: ")); Serial.print(b*v); Serial.println(F(" mm"));
  Serial.println(F("------------------------------------"));
  }

void loop(){
  //
  // Início // Para lado == 0 //
  //
  if (!digitalRead(ampolad) && side==0){
    b++;
    side++;
    side++; 
    float chuva = b*v;
    Serial.print (F("Chuva atual: ")); Serial.print(chuva); Serial.print(F(" mm")); Serial.print(F(" Side: ")); Serial.println(side);
    delay(800);
  }
  else if (!digitalRead(ampolae) && side==0){
    b++;
    side++; 
    float chuva = b*v;
    Serial.print (F("Chuva atual: ")); Serial.print(chuva); Serial.print(F(" mm")); Serial.print(F(" Side: ")); Serial.println(side);
    delay(800);
  }
  //
  // Execução // Para lado != 0 //
  //
  else if (!digitalRead(ampolad) && side==1){
    b++;
    side++; 
    float chuva = b*v;
    Serial.print (F("Chuva atual: ")); Serial.print(chuva); Serial.print(F(" mm")); Serial.print(F(" Side: ")); Serial.println(side);
    delay(800);
  }
  else if (!digitalRead(ampolae) && side==2){
    b++;
    side--; 
    float chuva = b*v;
    Serial.print (F("Chuva atual: ")); Serial.print(chuva); Serial.print(F(" mm")); Serial.print(F(" Side: ")); Serial.println(side);
    delay(800);
  }
}
