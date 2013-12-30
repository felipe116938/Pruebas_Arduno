   
    #define trigger1 10 //trigger1=10; // Pin de disparo del primer contador
    #define echo1 13 // Pin de medida del eco del primer contador
    
    #define trigger2 9 // Pin de disparo del segundo contador
    #define echo2 12 // Pin de medida del eco del segundo contador
    
    #define coche 1
    #define no_coche 0
    
    /****************************************/
    /* Estructura para controlar cada coche */
    /****************************************/
    struct sensor_coche
    {
      int trigger_pin;
      int echo_pin;
      boolean estado; //0 - no hay coche  1 si hay
      byte filtro;  //para detectar flancos coche/no coche
      float umbral;  // distancia máxima que indica que hay coche
      byte vueltas;
    };    
    
    struct sensor_coche s_coche1;
    struct sensor_coche s_coche2;
    
    /*************************************/
    /* Función para inicializar sensores */
    /*************************************/
    void inicializa_sensor(struct sensor_coche * sc, int trigger_pin, int echo_pin)
    {
      sc->estado = no_coche;
      sc->filtro = 255;
      sc->umbral = 600.0; // Al imprlementar puede que haya que diferenciar un coche de otro
      sc->trigger_pin = trigger_pin;
      sc->echo_pin = echo_pin;
      sc->vueltas = 20;
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
      inicializa_sensor(& s_coche1,trigger1,echo1);
      inicializa_sensor(& s_coche2,trigger2,echo2);
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
//      Serial.println(distancia);
      sensor->filtro <<= 1;
      sensor->filtro += flag;
      sensor->filtro &= 0x7;
          
      if((sensor->filtro  == 0) && (sensor->estado == coche))
      {
          sensor->vueltas -= 1;
          Serial.println(sensor->vueltas);
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
      detecta_coche(& s_coche1);
//      detecta_coche(& s_coche2);
      
 /*     
      Serial.print("Filtro 1: ");
      Serial.println(s_coche1.filtro);
      Serial.print("Estado 1: ");
      Serial.println(s_coche1.estado);
      Serial.print("Vueltas 1: ");
      Serial.println(s_coche1.vueltas);
 */     
      delay(10);
      //delay(2000);
    }
