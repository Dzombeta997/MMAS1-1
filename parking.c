/*
 * Naziv projekta:
     Zadatak5
 * Konfiguracija:
     MCU:             P18F8520
     Dev.Board:       BigPIC 5
     Oscilator:      HS, 10.0 MHz
     Ext. Modules:    LCD2x16 module
     SW:              mikroC PRO for PIC

  * NOTES:
*/

// Podesavanje pinova LCD-a
sbit LCD_RS at RD2_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;
int brojac=0;
char txt[7];
bit flag1;
bit flag2;

void main(){

  ADCON1 |= 0x0F;
  CMCON  |= 7;

  TRISB0_bit = 1;                    // Podesavanje pina kao ulaznog
  TRISB1_bit = 1;


  Lcd_Init();                        // Inicijalizacija LCD-a
  Lcd_Cmd(_LCD_CLEAR);               // Brisanje podataka sa displeja
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Iskljucivanje kursora

  flag1=0;
  flag2=0;
          while(1) {                                // Beskonacna petlja

   if(PORTB.B0 && flag1 ){                   // Pritisak na taster detektovan
          if (brojac<14)  {
          brojac++;
          IntToStr(brojac,txt);             // Konverzija u string
          Lcd_Cmd(_LCD_CLEAR);              // Brisanje podataka sa displeja
          Lcd_Out(1,1,"Broj automobila");    // Ispis na LCD
          Lcd_Out(2,1,txt);
          }                     // Brojanje pritisaka tastera
          else{
          Lcd_Cmd(_LCD_CLEAR);              // Brisanje podataka sa displeja
          Lcd_Out(1,1,"Parking je pun");    // Ispis na LCD
          }
          flag1=0;                           // Resetovanje flag-a
   }
   if(!PORTB.B0)                            // Detektovano pustanje tastera
          flag1=1;                           // Setovanje flag-a



   if(PORTB.B1 && flag2 ){                   // Pritisak na taster detektovan
          if (brojac>1)  {
          brojac--;
          IntToStr(brojac,txt);             // Konverzija u string
          Lcd_Cmd(_LCD_CLEAR);              // Brisanje podataka sa displeja
          Lcd_Out(1,1,"Broj automobila");    // Ispis na LCD
          Lcd_Out(2,1,txt);
          }                     // Brojanje pritisaka tastera
          else{
          Lcd_Cmd(_LCD_CLEAR);              // Brisanje podataka sa displeja
          Lcd_Out(1,1,"Parking je prazan");    // Ispis na LCD
          }
          flag2=0;                           // Resetovanje flag-a
   }
   if(!PORTB.B1)                           // Detektovano pustanje tastera
          flag2=1;

}
}
