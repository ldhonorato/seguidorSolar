#include <Wire.h>
#include <DS3231.h>
#include <Time.h>

RTClib rtc_ds3231;

void RTC_Inicializar()
{
  Wire.begin();

  Serial.println("**Sincronizando com TimeLib...");
  setSyncProvider(RTC_MakeTimeSync);   // Função para ficar sincronizando o TimeLib
  if(timeStatus()!= timeSet) 
     Serial.println("**-Falha ao sincronizar RTC e TimeLib!");
  else
     Serial.println("**-RTC e TimeLib Sincronizados com sucesso!");
}

void RTC_ImprimirDiaHora(byte _u8QuantidadePrints)
{
  DateTime dateTime_stamp;
  for (int i = 0; i < _u8QuantidadePrints; i++) {
    dateTime_stamp = rtc_ds3231.now();
    
    Serial.print(dateTime_stamp.day(), DEC);
    Serial.print("-");
    Serial.print(dateTime_stamp.month(), DEC);
    Serial.print("-");
    Serial.print(dateTime_stamp.year(), DEC);
    Serial.print(" ");
    Serial.print(dateTime_stamp.hour(), DEC); //24-hr
    Serial.print(":");
    Serial.print(dateTime_stamp.minute(), DEC);
    Serial.print(":");
    Serial.println(dateTime_stamp.second(), DEC);
    Alarm.delay(1000);
  }
}

void RTC_AtualizarDataHora(int dia, int mes, int ano, int hora, int minuto, int segundo)
{
  DS3231 Clock;
  Clock.setYear(ano);
  Clock.setMonth(mes);
  Clock.setDate(dia);
  Clock.setHour(hora);
  Clock.setMinute(minuto);
  Clock.setSecond(segundo);
}


time_t RTC_MakeTimeSync()
{
  tmElements_t tm;
  DS3231 Clock;
  bool Century = true;
  bool h12, PM;
  tm.Year = Clock.getYear() + 2000 - 1970;
  tm.Month = Clock.getMonth(Century);
  tm.Day = Clock.getDate();
  tm.Hour = Clock.getHour(h12, PM);
  tm.Minute = Clock.getMinute();
  tm.Second = Clock.getSecond();
  tm.Wday = Clock.getDoW();
  
  return(makeTime(tm));
}

bool RTC_isSistemaAtivo()
{
  bool horaFuncionamento = false;
  
  byte hora = hour();

  if(hora >= HORA_LEVANTAR_PAINEL && hora < HORA_DESCANSAR_PAINEL)
  {
      horaFuncionamento = true;
  }
  
  return horaFuncionamento;
}
