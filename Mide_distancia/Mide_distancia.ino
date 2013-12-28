   
    const int trigger1=10; // Pin de disparo del primer contador
    const int echo1=13; // Pin de medida del eco del primer contador
    
    const int trigger2=9; // Pin de disparo del segundo contador
    const int echo2=12; // Pin de medida del eco del segundo contador
     
    void setup(){
      Serial.begin(9600);
      pinMode(trigger1,OUTPUT);
      pinMode(echo1,INPUT);
      pinMode(trigger2,OUTPUT);
      pinMode(echo2,INPUT);
    }
    
    float leeSensor(const int sensor, const int eco)
    {
      float distance;
      digitalWrite(sensor,LOW);
      delayMicroseconds(5);
       // Comenzamos las mediciones
      // Enviamos una señal activando la salida trigger durante 10 microsegundos
      digitalWrite(sensor,HIGH);
      delayMicroseconds(10);
      digitalWrite(sensor,LOW);
    // Adquirimos los datos y convertimos la medida a metros
     distance=pulseIn(eco,HIGH); // Medimos el ancho del pulso (microsegundos)
     return distance; //*0.0001657;  distancia en metros  (*0.0001657)
    }
     
    void loop(){
//     unsigned long inicio=millis();
//Inicializamos el sensor
//    int *pp = (int*)malloc(sizeof(int));
//   struct
//    {
//      int m;
//      byte jj;
//    }lpij;
    
    
/*    
      digitalWrite(trigger1,LOW);
      delayMicroseconds(5);
    // Comenzamos las mediciones
    // Enviamos una señal activando la salida trigger durante 10 microsegundos
      digitalWrite(trigger1,HIGH);
      delayMicroseconds(10);
      digitalWrite(trigger1,LOW);
    // Adquirimos los datos y convertimos la medida a metros
     distance1=pulseIn(echo,HIGH); // Medimos el ancho del pulso
                                  // (Cuando la lectura del pin sea HIGH medira
                                  // el tiempo que transcurre hasta que sea LOW
      
     distance1=distance1*0.0001657;*/
    // Enviamos los datos medidos a traves del puerto serie y al display LCD
      Serial.println(leeSensor(trigger1, echo1)); 
      Serial.println(leeSensor(trigger2, echo2));
      delay(100);
 //     Serial.println(millis()-inicio);
    }
