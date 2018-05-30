#include <TimeLib.h>
#include <TimeAlarms.h>
#include <CLI.h>

CLI_COMMAND(MotorPassoFunc);
CLI_COMMAND(LevantarPainelFunc);
CLI_COMMAND(AvancarPainelFunc);
CLI_COMMAND(DescansarPainelFunc);
CLI_COMMAND(LeituraSensoresFunc);
CLI_COMMAND(HoraRTCFunc);
CLI_COMMAND(HoraTimeFunc);
CLI_COMMAND(helpFunc);
CLI_COMMAND(connectFunc);

#define INTERVALO_LOG_S         60   //intervalo em segundos do log
#define HORA_LEVANTAR_PAINEL    6   
#define HORA_DESCANSAR_PAINEL   17  //assumindo que o RTC e Time estarão sempre em 24h

void setup()
{
  Serial.begin(115200);
  CLI.setDefaultPrompt("> ");
  CLI.onConnect(connectFunc);
    
  CLI.addCommand("help", helpFunc);
  CLI.addCommand("motor", MotorPassoFunc);
  CLI.addCommand("levantar", LevantarPainelFunc);
  CLI.addCommand("descansar", DescansarPainelFunc);
  CLI.addCommand("avancar", AvancarPainelFunc);
  CLI.addCommand("ler", LeituraSensoresFunc);
  CLI.addCommand("rtc", HoraRTCFunc);
  CLI.addCommand("time", HoraTimeFunc);

  CLI.addClient(Serial);
  
  Serial.println("**Inicializando RTC...");
  RTC_Inicializar();
  RTC_ImprimirDiaHora(1);

  Serial.println("**Configurando Motor de Passo...");
  MotorPasso_Configurar();
  Serial.println("**Movendo painel para posicao da hora atual");
  //MotorPasso_AtualizarPosicao();

  Serial.println("**Inicializando Log - SD Card");
  if(Log_Inicializar())
    Serial.println("**-SD Card inicializado com sucesso!");
  else
    Serial.println("**-Falha ao inicializar SD Card");
}

void loop()
{
   CLI.process();
   Alarm.delay(1);
}

CLI_COMMAND(MotorPassoFunc) {
    if (argc != 3) {
        dev->println("Uso: motor <sentido> <n de passos> - sentido: 0-horario 1-anti-horario.");
        return 10;
    }
    byte sentidoMotor = MotorPasso_GetSentidoAntiHorario();
    int sentido = atoi(argv[1]);
    int passos = atoi(argv[2]);
    if(sentido == 0)
    {
      sentidoMotor = MotorPasso_GetSentidoHorario();
    }

    for(int i = 0; i < passos; i++)
    {
      MotorPasso_ExecutarPasso(sentidoMotor);
    }
    
    dev->println("OK");
    return 0;
}

CLI_COMMAND(LevantarPainelFunc)
{
  MotorPasso_LevantarPainel();
  dev->println("OK");
  return 0;
}

CLI_COMMAND(AvancarPainelFunc)
{
  MotorPasso_ExecutarAvancoHora();
  dev->println("OK");
  return 0;
}

CLI_COMMAND(DescansarPainelFunc)
{
  MotorPasso_PosicaoDescanso();
  dev->println("OK");
  return 0;
}

CLI_COMMAND(LeituraSensoresFunc)
{
  dev->print("Leitura termopar: ");
  dev->println(Log_LeituraTermoparStr());
  dev->print("Leitura fotodiodo: ");
  dev->println(Log_LeituraFotodiodoStr());
  return 0;
}

CLI_COMMAND(HoraRTCFunc)
{
  int dia, mes, ano, hora, minuto, segundo;
  int valorRetorno = 10;//inicializa com código de erro
  switch(argc)
  {
    case 1: //se não tem argumento, apenas imprime a hora armazenada
      RTC_ImprimirDiaHora(1);
      dev->println("OK");
      valorRetorno = 0;
      break;
    case 7://se tem todos os argumentos, faz o parser e atualiza a hora
      dia = atoi(argv[1]);
      mes = atoi(argv[2]);
      ano = atoi(argv[3]);
      hora = atoi(argv[4]);
      minuto = atoi(argv[5]);
      segundo = atoi(argv[6]);
      RTC_AtualizarDataHora(dia, mes, ano, hora, minuto, segundo);
      setTime(RTC_MakeTimeSync());
      dev->println("OK");    
      valorRetorno = 0;
      break;

     default://condição de erro
      dev->println("Uso: rtc <dia> <mes> <ano> <hora> <minuto> <segundo>. Digite help para mais detalhes"); 
  }

  return valorRetorno;
}

CLI_COMMAND(HoraTimeFunc)
{
  dev->print(day());
  dev->print("-");
  dev->print(month());
  dev->print("-");
  dev->print(year());
  dev->print(" | ");

  dev->print(hour());
  dev->print(":");
  dev->print(minute());
  dev->print(":");
  dev->print(second());
  return 0;
}

CLI_COMMAND(helpFunc) {
    dev->println("Comando disponiveis:");
    dev->println("motor <sentido> <n de passos> - sentido: 0-horario 1-anti-horario.");
    dev->println("levantar - partindo da posicao de descanso, traz o painel para posicao inicial.");
    dev->println("descansar - partindo da posicao do final do dia, retorna o painel para posicao de descanso.");
    dev->println("avancar - avanca o painel em x graus.");
    dev->println("ler - imprime a leitura do fotodiodo e do termopar.");
    dev->println("rtc <dia> <mes> <ano> <hora> <minuto> <segundo> - ajusta os dados d RTC ou imprime a hora armazenada (caso nao tenha parametros).");
    dev->println("time - imprime a hora da TimeLib.");
    return 0;
}

CLI_COMMAND(connectFunc) {
    dev->println("Bem vindo ao seguidor solar.");
    dev->println("Digite 'help' para lista de comandos.");
    dev->println();
    dev->printPrompt();
}

