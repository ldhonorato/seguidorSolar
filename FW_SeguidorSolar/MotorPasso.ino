#include <TimeLib.h>
#include <TimeAlarms.h>

#define MOTOR_PINO_PASSO              7
#define MOTOR_PINO_SENTIDO            6
#define MOTOR_PINO_ENABLE             5

#define MOTOR_PASSO_SENTIDO_HORARIO               LOW
#define MOTOR_PASSO_SENTIDO_ANTIHORARIO           HIGH

#define PERIODO_PULSO_MS              5 //quando menor, maior a velocidade do motor

#define QUANTIDADE_PASSOS_LEVANTAR_PAINEL   200
#define QUANTIDADE_PASSOS_AVANCO_HORA       50
#define QUANTIDADE_PASSOS_RETORNO_PAINEL    500

void MotorPasso_Configurar()
{
  pinMode(MOTOR_PINO_ENABLE, OUTPUT);
  pinMode(MOTOR_PINO_SENTIDO, OUTPUT);
  pinMode(MOTOR_PINO_PASSO, OUTPUT);

  Alarm.alarmRepeat(HORA_LEVANTAR_PAINEL,0,0, MotorPasso_LevantarPainel); //de 6h -> levar o painel para posição inicial
  Alarm.alarmRepeat(HORA_DESCANSAR_PAINEL,0,0, MotorPasso_PosicaoDescanso); //de 17h -> levar o painel para posição de descanso

  instalarProximoAlarmeMovimentacaoPainel();
}

void instalarProximoAlarmeMovimentacaoPainel()
{
  byte hora = hour();
  if( (hora >= HORA_LEVANTAR_PAINEL) && hora < (HORA_DESCANSAR_PAINEL - 1))
  {
    Alarm.alarmOnce(hora + 1, 0, 0, MotorPasso_ExecutarAvancoHora);
  }
}

void MotorPasso_Habilitar()
{
  digitalWrite(MOTOR_PINO_ENABLE, LOW);
}

void MotorPasso_Desabilitar()
{
  digitalWrite(MOTOR_PINO_ENABLE, HIGH);
}

void MotorPasso_ExecutarPasso(byte _u8Sentido)
{
  digitalWrite(MOTOR_PINO_SENTIDO, _u8Sentido);
  digitalWrite(MOTOR_PINO_PASSO, HIGH);
  Alarm.delay(PERIODO_PULSO_MS);
  digitalWrite(MOTOR_PINO_PASSO, LOW);
  Alarm.delay(PERIODO_PULSO_MS);
}

/*
 * Esta função realiza o avanço do painel, considerando que o mesmo parte da posição de descanso.
 */
void MotorPasso_LevantarPainel()
{
  Log_SystemLog("Levantando Painel");
  Serial.println("**Levantando Painel");
  MotorPasso_Habilitar();
  for(uint16_t i = 0; i < QUANTIDADE_PASSOS_LEVANTAR_PAINEL; i++)
  {
    MotorPasso_ExecutarPasso(MOTOR_PASSO_SENTIDO_HORARIO);
  }
  instalarProximoAlarmeMovimentacaoPainel();
}

/*
 * Esta função faz com que o painel retorne à posição de descanso.
 */
void MotorPasso_PosicaoDescanso()
{
  Log_SystemLog("Painel para posicao de descanso");
  Serial.println("**Painel para posicao de descanso");
  for(uint16_t i = 0; i < QUANTIDADE_PASSOS_RETORNO_PAINEL; i++)
  {
    MotorPasso_ExecutarPasso(MOTOR_PASSO_SENTIDO_ANTIHORARIO);
  }
  MotorPasso_Desabilitar();
}

/*
 * Esta função faz com que o painel incline x°, o que deve ser feito à cada hora.
 */
void MotorPasso_ExecutarAvancoHora()
{
  Log_SystemLog("Avancando painel");
  Serial.println(">Avancando painel");
  for(uint16_t i = 0; i < QUANTIDADE_PASSOS_AVANCO_HORA; i++)
  {
    MotorPasso_ExecutarPasso(MOTOR_PASSO_SENTIDO_HORARIO);
  }
  instalarProximoAlarmeMovimentacaoPainel();
}

/*
 * Esta função atualiza a posição do painel, para que o mesmo fique na inclinação correta após ser ligado
 */
void MotorPasso_AtualizarPosicao()
{
  Log_SystemLog("Atualizando posicao painel");
  Serial.println("**Atualizando posicao painel");
  if(RTC_isSistemaAtivo() == true)
  {
    byte hora = hour();
    MotorPasso_Habilitar();
    MotorPasso_LevantarPainel();
    for(byte i = HORA_LEVANTAR_PAINEL; i < hora; i++)
    {
      MotorPasso_ExecutarAvancoHora();
    }
    
  }
}

byte MotorPasso_GetSentidoAntiHorario()
{
  return MOTOR_PASSO_SENTIDO_ANTIHORARIO;
}

byte MotorPasso_GetSentidoHorario()
{
  return MOTOR_PASSO_SENTIDO_HORARIO;
}

