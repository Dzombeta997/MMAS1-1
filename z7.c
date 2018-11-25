# MMAS1
//Vezbe mmas1 FTN
#include "Zadatak7.h"
#define MASK 0xFF

static void initMain(){

  Lcd_Init();                        // Inicijalizacija LCD-a
  Lcd_Cmd(_LCD_CLEAR);               // Brisanje podataka sa displeja
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Iskljucivanje kursora
}

void main(){
  float adc,adc_t=0;
  char txt[15];
  int i;
  TRISD = 0;
  LATD = 0x00;

  initMain();

  Lcd_Out(1,1,"Napon je:");          // Ispis na LCD

  while(1) {                         // Beskonacna petlja
  
      adc = ADC_Read(2)*5.0/4096; 

      if(adc<adc_t)
         LATD &= ~(MASK << (int)(adc+0.5)+1);
      else
          LATD |= ~(MASK << (int)(adc+0.5));
      
                                     // Na taj nacin se obezbedjuje da se operacije izvrse kao float
      sprintf(txt,"%.2f V",adc);     // Konverzija iz float vrednsosti u string u formatu "x.xx V"
      Lcd_Out(2,1,txt);              // Ispis konvertovane vrednosti
      Delay_ms(500);
      adc_t=adc;
  }
}
