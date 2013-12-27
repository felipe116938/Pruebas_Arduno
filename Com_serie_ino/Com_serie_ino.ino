//Programa de comunicación bilateral Arduino-Visual Basic 6.0
//Mediante un pulsador enviamos información a visual y desde visual
//dependiendo del estado de ese pulsador mandamos una orden a arduino
//para encender un lez o apagarlo.
//Programa realizado por Regata
 
int boton=12; //pulsador conectado en pin 12
int led=13;   //led conectado en pin 13
 
void setup()
{
  Serial.begin(9600); //Configuración de puerto serie
  pinMode(led,OUTPUT); //Indicamos led como salida
  pinMode(boton,INPUT); //NOTA: no hace falta indicar el pulsador como entrada
}
 
void loop()
{
/*  int estado = digitalRead(boton);
  if(estado==0)
  {
    Serial.println("Botón pulsado");
    digitalWrite(led,HIGH);
  }
  else
  {
    Serial.println("Botón suelto");
    digitalWrite(led,LOW);
  }*/
  Serial.print(digitalRead(boton),DEC); //Enviamos por puerto serie el
  //estado del pulsador hacia VB
 
  if (Serial.available())  //Comprobamos si hay datos desde VB hacia Arduino
  {
    byte dato; //Variable donde guardaremos el dato recibido por puerto serie
    dato=Serial.read();  //Guardamos el dato recibido
    switch(dato)
    {
      case '0': //Apagamos el led si recibimos '0' por puerto serie
        digitalWrite(led,LOW);
        break;
      case '1': //Encendemos el led si recibimos '1' por puerto serie
        digitalWrite(led,HIGH);
        break;
    }
  }
  delay(30);
}
