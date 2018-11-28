
static void initMain(){
  UART1_Init(9600);             
  Delay_ms(100);                  

  UART1_Write_Text("Start");     
}

void main() {
  int i;
  TRISD = 0xFF;
  char uart_rd;
  char value1;
  char value2;

  initMain();
  
  while (1) {                     
           if (UART1_Data_Ready()){                   
              uart_rd = UART1_Read();                  
                    }
              
           if(uart_rd =='1'){
              value1= 0x01;
                for(i = 0; i < 8; i++){
                  LATD = LATD | (value1<<i);
                  Delay_ms(1000);
                }     
           }
            else if(uart_rd =='2'){
                value2=0x80;
                 for (i = 0; i < 8; i++){
                    LATD = LATD | (value2>>i);
                  Delay_ms(1000);
                    }        
           }else {

                LATD = 0xFF;
                Delay_ms(1000);
                LATD = 0x00;
        }
    }
}