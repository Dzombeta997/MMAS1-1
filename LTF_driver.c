#include "LTF_driver.h"


/**************************************************************************************************
* Funkcija Meter()
* -------------------------------------------------------------------------------------------------
* Opis: Funkcija prikazuje linearno ukljucenje LED dioda u zavisnosti od osvetljenja
* Ulaz: frekvencija
* Izlaz: Vrednost koji treba upisati u LATD registar
**************************************************************************************************/
char Meter(unsigned long f) {
  char temp = 0x00, i = 0x01;
  unsigned long level = 100;

  while (level <= 500) {
    if (f >= level) temp = temp | i;
    else temp = temp & (~i);
    level = level + 50;
    i = i << 1;
  }
  return temp;
}