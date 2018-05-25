void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  MotorPasso_Configurar();
  pinMode(22, INPUT_PULLUP);

  
}

void loop() {
  if(Serial.available() > 0)
  {
    Serial.println("Comecando 500 passos");
    for(int i = 0; i < 500; i++)
    {
      MotorPasso_Girar(LOW);
      Serial.print("Passo: ");
      Serial.println(i);
    }
    Serial.println("Fim");
    while(Serial.available() > 0)
      Serial.read();
  }
  // put your main code here, to run repeatedly:
  
//  if(digitalRead(22) == LOW)
//  {
//    Serial.print("LOW");
//  }
//  else
//  {
//    Serial.print("HIGH");
//  }
//  delay(500);
}
