/*
 * Naziv projekta:
     Zadatak7
 * Konfiguracija:
     MCU:             P18F87K22
     Dev.Board:       EasyPICv7
     Oscilator:       HS, 16.0 MHz
     Ext. Modules:    GLCD128x64 module
     SW:              mikroC PRO for PIC
  * Opis:
  U ovom zadatku se na GLCD displeju ispisuju se vrednosti ocitane sa 5 ADC kanala
  * NOTES:
   Neophodno je ukljuciti pozadinsko osvetljenje displeja pomocu prekidaca SW4.2 GLCD BCK.
*/


#include "Zadatak7.h"

static void initMain(){
  Glcd_Init();                                      // Inicijalizacija GLCD
  Glcd_Fill(0x00);                                  // Brisanje GLCD
  Glcd_Set_Font(FontSystem5x7_v2, 5, 7, 32);        // Podesavanje fonta
}

void main() {
  int i=0;
  float adc;
  char txt[20];
  initMain();

  while(1){
  adc = ADC_Read(2)*5.0/4096.0;
  Glcd_Fill(0x00);
 for(i =1; i < adc +0.5; i++){
     Glcd_Box(5+20*(i-1),50-10*(i-1), 20*i, 60, 1);
     /*     switch(i){
     case 1:
          Glcd_Box(5,50, 20, 60, 1);
          break;
     case 2:
          Glcd_Box(25,40, 40, 60, 1);
          break;
     case 3:
          Glcd_Box(45,30, 60, 60, 1);
          break;
     case 4:
          Glcd_Box(65,20, 80, 60, 1);
          break;
     case 5:
          Glcd_Box(85,10, 100, 60, 1);
          break;
     } */

     }
  Delay_ms(1000);
  }
}