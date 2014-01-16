/*
Conexiones del display a la placa
    Display    Arduino
    =======    =========
    CLK        Digital 2
    DIO        Digital 3
    VCC        +5V
    GND        GND
*/
#include "TM1637.h"

int8_t numDisp[] = {0x00,0x00,0x00,0x00}; // Dígitos a visualizar
byte a = 99; //Variables para almacenar el numero de vueltas
byte b = 99;

#define CLK 2 //definición de pines para el TM1637. Se pueden cambiar a otros puertos    
#define DIO 3 
TM1637 tm1637(CLK,DIO);

void setup()
{
  tm1637.set();
  tm1637.init();  
}
void loop()
{ 
  numDisp[0] = a/10;
  numDisp[1] = a % 10;
  numDisp[2] = b/10;
  numDisp[3] = b % 10;

  tm1637.display(numDisp);
  delay (1000);
  if(a > 0)
    a-=1;
  if(b > 0)
    b-=1;
}

