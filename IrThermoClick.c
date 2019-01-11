/*
 * Naziv projekta:
     IrThermo
 * Konfiguracija:
     MCU:             PIC18F87K22
     Dev.Board:       EasyPIC PRO v7
     Oscilator:       HS, 16.0 MHz
     Ext. Modules:    LCD, IRThermoClick
     SW:              mikroC PRO for PIC
  * Opis:
  U ovom zadatku se vrsi ocitavanje temperature sa Infra-crvenog senzora temperature
  MLX90614ESF-AAA. Temperatura je konvertovana u stepene Celzijusa i prikazuje se na LCD-u.
  * NOTES:
  	- Postaviti IrThermo click u mikroBUS prvi konektor.
  	- Postaviti kratkospojnik J5 u 5V poziciju.
  	- Iskljuciti PORTC LED (SW6.3).
  	- Ukljuciti pozadinsko osvetljenje (SW4.6).
*/


extern const _IR_THERMO_ADDR;
extern const _AMB_TEMP;
extern cons _OBJ_TEMP;

float ReadSensor(char Temp_Source);
void Display_Temperature(char Temp_Source, float temperature);
#endif

const _IR_THERMO_ADDR = 0x5A;
const _AMB_TEMP = 0x06;
const _OBJ_TEMP = 0x07;

float ReadSensor(char Temp_Source){
	unsigned int Temp_var;

	I2C1_Start();
	I2C1_Wr(_IR_THERMO_ADDR << 1);
	I2C1_Wr(Temp_Source);
	I2C1_Repeated_Start();
	I2C1_Wr(_IR_THERMO_ADDR<< 1);
	Temp_var= I2C1_Rd(0);
	Temp_var= (I2C1_Rd(0)<<8) + Temp_var;
	I2C1_Stop();

	return Temp_var;
}

void Display_Temperature(char Temp_Source, float temperature){
	char text[15];

	FloatToStr(temperature, text);
	if (text[1] == '.')
		text[4] = 0;
	if (text[2] == '.')
		text[5] = 0;
	if (text[3] == '.')
		text[6] == 0;
	strcat(text, "°C");

	if(Temp_Source == _AMB_TEMP)
	Lcd_Out(2,1,text);
	if(Temp_Source == _OBJ_TEMP)
	Lcd_Out(2,10, text);	
}

static void initMain(void){
	Lcd_Init();
	Lcd_Cmd(_LCD_CLEAR);
	Lcd_Cmd(_LCD_CURSOR_OFF);
	Lcd_Out(1,1,"Initialising I2C");
	
	I2C1_Init(50000);
	Delay_ms(1000);

	Lcd_Cmd(_LCD_CLEAR);
	Lcd_Out(1,1,"init OK");
	Delay_ms(1000);

	Lcd_Cmd(_LCD_CLEAR);
	Lcd_Out(1,1,"Amb. t: ");
  	Lcd_Out(1,9," Obj. t:");
}
void main(){
	float Temp;
	initMain();

	while(1){
		Temp=ReadSensor(_AMB_TEMP);
		Temp=(Temp * 0.02) - 273.15;
		Display_Temperature(_AMB_TEMP, Temp);

		Temp=ReadSensor(_OBJ_TEMP);
		Temp=(Temp*0.02) - 273.15;

		Display_Temperature(_OBJ_TEMP,Temp);
		Delay_ms(500);
	}

}
