#include "TM1637.h" // Controlador del display

#define trigger1 10 // Pin de disparo del primer contador
#define echo1 13 // Pin de medida del eco del primer contador
    
#define trigger2 9 // Pin de disparo del segundo contador
#define echo2 12 // Pin de medida del eco del segundo contador
    
#define coche 1 // Sensor detecta que está pasando uncoche
#define no_coche 0 // Sensor detecta que no está pasando uncoche

#define CLK 2 //definición de pines para el TM1637. Se pueden cambiar a otros puertos    
#define DIO 3 

TM1637 tm1637(CLK,DIO); // Inicialización del objeto tm1637
int8_t numDisp[] = {0x00,0x00,0x00,0x00}; // Dígitos a visualizar
boolean inicio = 0; // Controla comienzo de la carrera botón START

int start = 7; // Pulsador de START
int set1 = 4;
int set2 = 5;
int reset = 6;
    
/****************************************/
/* Estructura para controlar cada coche */
/****************************************/
struct sensor_coche
{
  int trigger_pin;
  int echo_pin;
  boolean estado; // 0 - no hay coche  1 si hay
  byte filtro;  // Para detectar flancos coche/no coche
  float umbral;  // Distancia máxima que indica que hay coche
  byte vueltas; // Almacena el número de vueltas restantes
  byte pista; // Número de pista por la que va el coche (distancia coche - sensor)
};    
    
struct sensor_coche s_coche1;
struct sensor_coche s_coche2;
    
/*************************************/
/* Función para inicializar sensores */
/*************************************/
void inicializa_sensor(struct sensor_coche * sc, int trigger_pin, int echo_pin, byte n_pista)
{
  sc->estado = no_coche;
  sc->filtro = 255;
  sc->umbral = 600.0; // Al implementar puede que haya que diferenciar un coche de otro
  sc->trigger_pin = trigger_pin;
  sc->echo_pin = echo_pin;
  sc->vueltas = 20;
  sc->pista = n_pista;
}
    
/***********/
/* Setup() */
/***********/
void setup()
{
  Serial.begin(9600);
  pinMode(trigger1,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(trigger2,OUTPUT);
  pinMode(echo2,INPUT);
  inicializa_sensor(& s_coche1,trigger1,echo1,1);
  inicializa_sensor(& s_coche2,trigger2,echo2,2);
  // Inicialización del contador
  numDisp[0] = (s_coche1.vueltas)/10;  
  numDisp[1] = (s_coche1.vueltas) % 10;
  numDisp[2] = (s_coche2.vueltas)/10;
  numDisp[3] = (s_coche2.vueltas) % 10;

  Serial.println(numDisp[0]);
  Serial.println(numDisp[1]);
  Serial.println(numDisp[2]);
  Serial.println(numDisp[3]);
  
  tm1637.set();
  tm1637.init(); 
}
    
/***********************/
/* Lectura de sensores */
/***********************/
float leeSensor(struct sensor_coche * s_coche)
{ 
  digitalWrite(s_coche->trigger_pin,LOW);
  delayMicroseconds(5);
  // Comenzamos las mediciones. Enviamos una señal activando la salida trigger durante 10 microsegundos
  digitalWrite(s_coche->trigger_pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(s_coche->trigger_pin,LOW);
  // dMedimos el ancho del pulso (microsegundos) 
  return pulseIn(s_coche->echo_pin,HIGH); //*0.0001657;  distancia en metros  (*0.0001657)
}
    
/*******************************/
/* Detecta el paso de un coche */
/*******************************/
void detecta_coche(struct sensor_coche * sensor)
{
  float distancia = leeSensor(sensor);  // OJO al puntero
  byte flag =  (distancia < sensor->umbral) ? 1 : 0; // 1 - está pasando un coche / 0 - no hay coche      sensor->filtro <<= 1;
  // Al valor del filtro, se le suma el flag
  // Serial.println(distancia);
  sensor->filtro <<= 1;
  sensor->filtro += flag;
  sensor->filtro &= 0x7;
          
  if((sensor->filtro  == 0) && (sensor->estado == coche))
  {
    if(sensor->vueltas > 0)
    {
      sensor->vueltas -= 1;
      if(sensor->pista == 1)
      {
          numDisp[0] = sensor->vueltas/10;
          numDisp[1] = sensor->vueltas % 10;
      }
      else
      {
          numDisp[2] = sensor->vueltas/10;
          numDisp[3] = sensor->vueltas % 10;
      }      
    }
//    Serial.println(sensor->vueltas);    
  }
  if(sensor->filtro  == 0)
    sensor->estado = no_coche; 
  else if(sensor->filtro  == 7)
    sensor->estado = coche;
}
     
/*********************/
/* Función principal */
/*********************/
void loop()
{
  if(digitalRead(start) == 1) // Botón de START pulsado
    inicio = 1;
  if(inicio == 1) // START activado
  {
    detecta_coche(& s_coche1);
    detecta_coche(& s_coche2);
    delay(10);    
  }
  tm1637.display(numDisp);
}
