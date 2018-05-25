#define MOTOR_PINO_PASSO              7
#define MOTOR_PINO_SENTIDO            6
#define MOTOR_PINO_ENABLE             5

#define SENTIDO_HORARIO               LOW
#define SENTIDO_ANTIHORARIO           HIGH

int velocidade = 5;

void MotorPasso_Configurar()
{
  pinMode(MOTOR_PINO_ENABLE, OUTPUT);
  pinMode(MOTOR_PINO_SENTIDO, OUTPUT);
  pinMode(MOTOR_PINO_PASSO, OUTPUT);

  pinMode(MOTOR_REED_SWITCH_FIM_CURSO, INPUT);

  digitalWrite(MOTOR_PINO_ENABLE, LOW);
}

void MotorPasso_Descansar()
{
//  while()
//  {
//    MotorPasso_Girar(SENTIDO_ANTIHORARIO);
//  }
}


void MotorPasso_Girar (byte _u8Sentido) {
  digitalWrite(MOTOR_PINO_SENTIDO, _u8Sentido);
  digitalWrite(MOTOR_PINO_PASSO, HIGH);
  delay(velocidade);
  digitalWrite(MOTOR_PINO_PASSO, LOW);
  delay(velocidade);
}
