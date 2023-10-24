#define ampola 7

// Define número de basculadas para 0 ao iniciar //
int b = 0; 

// Define volume da basculada - MODIFICAR COM TESTES //
float v = 3.2;

void setup(){
  pinMode(ampola, INPUT_PULLUP); //Define pino de entrada
  Serial.begin(9600); //Inicia porta serial
  Serial.println(F("------------------------------------"));
  Serial.println (F("Pluviômetro"));
  Serial.print (F("Chuva acumulada: ")); Serial.print(b*v); Serial.println(F(" mm"));
  Serial.println(F("------------------------------------"));
  }

void loop(){
  if (!digitalRead(ampola)){
    b++;
    float chuva = b*v;
    Serial.print (F("Chuva atual: ")); Serial.print(chuva); Serial.println(F(" mm"));
    delay(800);
  }
}
