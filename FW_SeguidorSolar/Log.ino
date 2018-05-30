#include <SD.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <max6675.h>

#define THERMO_DO    4
#define THERMO_CS    3
#define THERMO_CLK   2
MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_DO);

#define FOTODIODO_PINO            A3
#define FOTODIODO_FATOR_ESCALA    1.452

#define CS_SDCARD  53

bool Log_Inicializar()
{
  bool retorno = true;
  if (!SD.begin(CS_SDCARD)) {
    retorno = false;
  }

  Alarm.timerRepeat(INTERVALO_LOG_S, Log_RegistrarMedidas);
  return retorno;
}

void Log_RegistrarMedidas()
{
  float fotodioto, termopar;
  String dataString;
  if (RTC_isSistemaAtivo() == true)
  {
    termopar = thermocouple.readCelsius();
    fotodioto = analogRead(FOTODIODO_PINO) * FOTODIODO_FATOR_ESCALA; // não sei de onde veio este numero mágico...
    dataString =  String(day()) + "," + String(month()) + "," + String(year()) + ",";
    dataString += String(hour()) + "," + String(minute()) + "," + String(second()) + ",";
    dataString += String(termopar) + "," + String(fotodioto);

    Serial.print("> Registrando Medidas - ");
    Serial.println(dataString);
    
    File dataFile = SD.open("datalog.cvs", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
    }
  }
}

String Log_LeituraTermoparStr()
{
  return String(thermocouple.readCelsius());
}

String Log_LeituraFotodiodoStr()
{
  return String(analogRead(FOTODIODO_PINO) * FOTODIODO_FATOR_ESCALA);
}

void Log_SystemLog(String mensagemLog)
{
  String dataString;
  dataString =  String(day()) + "-" + String(month()) + "-" + String(year()) + "    ";
  dataString += String(hour()) + ":" + String(minute()) + ":" + String(second()) + "   |   ";
  File dataFile = SD.open("systemLog.log", FILE_WRITE);
    if (dataFile) {
      dataFile.print(dataString);
      dataFile.println(mensagemLog);
      dataFile.close();
    }
}

