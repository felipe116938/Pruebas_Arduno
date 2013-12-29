   
    #define trigger1 10 //trigger1=10; // Pin de disparo del primer contador
    #define echo1 13 // Pin de medida del eco del primer contador
    
    #define trigger2 9 // Pin de disparo del segundo contador
    #define echo2 12 // Pin de medida del eco del segundo contador
    
    #define coche 1
    #define no_coche 0
    
    struct sensor_coche
    {
      int trigger_pin;
      int echo_pin;
      boolean estado; //0 - no hay coche  1 si hay
      byte filtro;  //para detectar flancos coche/no coche
      float umbral;  // distancia máxima que indica que hay coche
    };    
    
    struct sensor_coche s_coche1;
    struct sensor_coche s_coche2;
    
    void inicializa_sensor(struct sensor_coche * sc, int trigger_pin, int echo_pin)
    {
      sc->estado = 0;
      sc->filtro = 255;
      sc->umbral = 600.0; // Al imprlementar puede que haya que diferenciar un coche de otro
      sc->trigger_pin = trigger_pin;
      sc->echo_pin = echo_pin;
    }
    
    void setup()
    {
      Serial.begin(9600);
      pinMode(trigger1,OUTPUT);
      pinMode(echo1,INPUT);
      pinMode(trigger2,OUTPUT);
      pinMode(echo2,INPUT);
      inicializa_sensor(& s_coche1,trigger1,echo1);
      inicializa_sensor(& s_coche2,trigger2,echo2);
    }
    

    float leeSensor(struct sensor_coche * s_coche)
    { 
      digitalWrite(s_coche->trigger_pin,LOW);
      delayMicroseconds(5);
      // Comenzamos las mediciones. Enviamos una señal activando la salida trigger durante 10 microsegundos
      digitalWrite(s_coche->trigger_pin,HIGH);
      delayMicroseconds(10);
      digitalWrite(s_coche->trigger_pin,LOW);
      // distance=pulseIn(eco,HIGH); // Medimos el ancho del pulso (microsegundos) 
      return pulseIn(s_coche->echo_pin,HIGH); //*0.0001657;  distancia en metros  (*0.0001657)
    }
     
    void loop()
    {
      float distancia = leeSensor(& s_coche1);
      byte flag =  (distancia < s_coche1.umbral) ? 1 : 0;
      // Enviamos los datos medidos a traves del puerto serie      
      Serial.println(distancia);
      s_coche1.filtro <<= 1;
      s_coche1.filtro += flag;
      s_coche1.filtro &= 0x7;
      if(s_coche1.filtro  == 7)
        s_coche1.estado = coche;
      else if(s_coche1.filtro  == 0)
        s_coche1.estado = no_coche;

      Serial.print("Filtro: ");
      Serial.println(s_coche1.filtro); 
      Serial.print("Estado: ");
      Serial.println(s_coche1.estado);
//      Serial.println(leeSensor(& s_coche2));
      //delay(100);
      delay(1000);
    }
