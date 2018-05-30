# Seguidor Solar
Projeto de Firmware para um seguidor solar para painel fotovoltaico. O projeto é parte do mestrado do [Prof. MSc Baldoíno Sonildo da Nóbrega](http://lattes.cnpq.br/6474706000993108)

## Descrição do funciomaneto
O seguidor solar tem o objetivo de alterar a posição do painel solar durante o dia, seguindo a trajetória do sol. De forma resumida, o sistema se comporta da seguinte forma:
* Com o nascer do sol, por volta das 6h - irradiação suficiente para gerar energia, o painel solar é posicionado em 45° em relação ao solo.
* Ao longo do dia, o painel vai variando a posição em x°/h.
* A partir das 17h, com irradiação mínima, o painel solar é deslocado para posição de descanso.

Além disso, a cada x minutos, a temperatura do painel e a luminosidade sobre o mesmo são armazenadas em um SD card.

## Hardware utilizado
* Arduino Mega 2560
* Motor de passo ak23 15f6fn1.8
* Driver para motor de passo BL-TB6560-v2.0
* RTC DS3231
* Termopar MAX6675
* Sensor luminosidade (?)
* Módulo SD Card

### Configuração das chaves do BL-TB6560-v2.0
* Running Current - 3A
  * SW1 - ON
  * SW2 - ON
  * SW3 - ON
  * S1 - OFF
* Stop Current - 50%
  * S2 - ON
* Excitation Mode - whole
  * S3 - OFF
  * S4 - OFF
* Decay Setting - 0%
  * S5 - OFF
  * S6 - OFF

## Bibliotecas Utilizadas
* Biblioteca DS32231
* Biblioteca MAX6675
* Biblioteca SD
* [TimeLib](https://github.com/PaulStoffregen/Time)
* [TimeAlarms](https://github.com/PaulStoffregen/TimeAlarms)
* [CLI](https://github.com/PaulStoffregen/Time)

## Terminal serial
Utilizando a *Command Line stream interface for Arduino* (CLI), podem ser enviados os seguintes comandos pela porta serial:
* motor <sentido> <n de passos> - comando para operar o motor de passo que movimenta o painel
  *<sentido> - sentido de rotação do motor. Se igual a 0 (zero) o motor roda no sentido horário. Se diferente de 0 (zero) o motor roda em sentido anti-horário.
* levantar - partindo da posicao de descanso, traz o painel para posição inicial.
* descansar - partindo da posicao do final do dia, retorna o painel para posição de descanso.
* avancar - avanca o painel e x°.
* ler - imprime a leitura do fotodiodo e do termopar.
* rtc <dia> <mes> <ano> <hora> <minuto> <segundo> - ajusta os dados d RTC ou imprime a hora armazenada (caso nao tenha parametros).
* time - imprime a hora da TimeLib.

## Arquivos de Log
O sistema gera dois arquivos de Log, armazenados no SD Card:
### datalog.cvs 
Arquivo CSV, com o seguinte *header*:
* dia, mês, ano, hora, minuto, segundo, temperatura, fotodiodo
Estes dados são armazenados a cada 60 segundos.
### systemLog.log
Arquivo texto com todas as operações realizadas pelo firware.
