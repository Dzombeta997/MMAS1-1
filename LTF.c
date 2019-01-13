/*
 * Naziv projekta:
     Light to Frequency
 * Konfiguracija:
     MCU:             PIC18F87K22
     Dev.Board:       EasyPIC PRO v7
     Oscilator:       HS, 16.0 MHz
     Ext. Modules:    UART, LightHz Click
     SW:              mikroC PRO for PIC
  * Opis:
  U ovom zadatku se vrsi ocitavanje sa senzora TSL230RD, koji konvertuje jacinu osvetljenja
  u frekvenciju. Pulsevi se broje pomocu RB0/INT interapta na uzlaznu ivicu. Timer1 generise
  interapt na svaku osminu sekunde tj. 0.125 s, nakon cega se vrsi proracun frekvencije.
  Rezultat se prikazuje linearno na LED PORTD i salju se posredstvom UART komunikacije na racunar.

  * NOTES:
    - Postaviti LightHz click u mikroBUS prvi konektor.
    - Ukljuciti PORTD LED (SW 6.4).
    - Omoguciti slanje podataka UART-om (SW 5.1 i SW 5.2).
*/

#include "LTF_driver.h"

/**************************************************************************************************
* Globalne promenljive
**************************************************************************************************/
unsigned long freq;
int timer_cnt,freq_read_done = 0;

/**************************************************************************************************
* Interrupt (prekidna) rutina
**************************************************************************************************/
void interrupt() {
  if (TMR0IF_bit) {                     // Provera Timer0 interrupt flag-a
    TMR0H = 0x0B;                       // Prvo upis viseg bajta u TMR0
    TMR0L = 0xDB;                       // Zatim upis nizeg bajta u TMR0
    TMR0IF_bit = 0;                     // Brisanje Timer0 interrupt flag-a
    freq_read_done=1;
  }
  if(INT0IF_bit){                       // Provera RB0/INT interrupt flag-a
    freq++;                              // Brojanje interrupt-a na RB0/INT pinu
    INT0IF_bit = 0;                     // Brisanje RB0/INT interrupt flag-a
  }
}

static void initMain(){
  ANCON0 = 0;                           // Podesavanje svih pinova kao digitalnih
  ANCON1 = 0;
  ANCON2 = 0;

  DIODES_direction = 0x00;
  OUT_direction = 1;                    // INT0 pin podesen kao ulazni

  DIODES = 0x00;

  UART1_Init(19200);                    // Inicijalizacija uarta
  Delay_ms(100);
  // Timer1 radi u 16-bitnom modu sa internim brojacem instrukcija i preskalerom 1:8
  // Ako je frekvencija 16 MHz onda ce frekvencija internih instrukcija biti 4 MHz
  // Dok ce preskaler 1:8 redukovati tu frekvenciju na 500 kHz
  // Posto 16 bitni tajmer nije dovoljno veliki da broji jednu sekundu ( od 0 do 500000)
  // Mora se brojati manji vremenski intervali, pa je u ovom slucaju mereno 1/8 sekunde
  // Pa je za 1/8 sekundu potrebno meriti 500000/8 = 62500 impulsa
  // Zbog toga je neophodno tajmer podesiti na 65536-62500 = 3036 (0x0BDC)
  // Prekoracenje TMR1 modula tj. generisanje interapta nastaje sa periodom 1/8 sekunde.

  TMR0H = 0x0B;                         // Upis viseg bajta u TMR1
  TMR0L = 0xDB;                         // Upis nizeg bajta u TMR1
  T0CON = 0x85;                         // Timer1 podesavanja (Fosc/4 kao ulaz i preskaler 1:8)
  TMR0IE_bit = 1;                       // Ukljuci prekoracenje TMR1
  INT0IE_bit = 1;
  INTEDG0_bit = 1;                      // Interrupt na RB0/INT0 pinu, okidanje na uzlaznu ivicu

  INTCON = 0xD0;                        // Setovanje GIE, PIE, INTE
}

/**************************************************************************************************
* Main
**************************************************************************************************/
void main() {
// Inicijalizacija promenljivih i registara
  unsigned long old_freq=0;
  char txt[12];
  
  freq = 0;
  old_freq  = 0;
  timer_cnt = 0;
  
  initMain();

  while(1){

    if (freq_read_done){
      GIE_bit = 0;                      // Iskljucenje interapta
      freq_read_done = 0;
      LongToStr(freq, txt);             // Konvertovanje freq (long integer) u string
      DIODES = Meter(freq);             // Prikaz rezultata na diodama PORTD
      UART1_Write_Text(txt);            // Slanje rezultata UART-om
      UART1_Write_Text("\r\n");

      old_freq  = freq;
      freq = 0;
      GIE_bit   = 1;                    // Ukljucenje interapta
      TMR1IE_bit = 1;
      INTEDG0_bit = 1;
    }
  }
}

/**************************************************************************************************
* End of File
**************************************************************************************************/