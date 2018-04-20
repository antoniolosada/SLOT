/******************************************************************************
void AsignarVelocidad(int v)
*******************************************************************************/

#define MS_MEDIA_LECTURAS    2

#define MAX_VALORES_CORRECION    30
#define RETARDO_VALOR_CORRECCION 2 //Milis
#define PROPORCION_FRENADO  1

void AsignarVelocidad(int v)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  static long TopeGasTiempo = 0;
  static long tiempo;
  static int estado;
  static int v1 = 0;
  int rele, prog;
  static int CfgSelecFreno;
  static long TiempoRecta;
  int TiempoCfgRecta;
  int vc;

    if (v > MARGEN_INI_GAS)
    {
      //DEBUG("Desactivar Freno", "");
      //DEBUG("Rele ->", digitalRead(PIN_FRENO_RELE));
      digitalWrite(PIN_FRENO, LOW);      
      digitalWrite(PIN_FRENO_RELE, LOW);      
      if (v >= VALOR_TOPE_GAS)
      {
        digitalWrite(PIN_MOTOR, HIGH);
        if (TopeGasTiempo == 0)
          TopeGasTiempo = millis();
      }
      else
      {
          //Adaptamos la curva de potencia
          if ((SelecAceleracion != NO_SELEC) && (SelecAceleracion != 'L'))
          {
            //Normalizamos los valores según la curva de aceleración
            vc = AsignarCurvaAceleracion(v, SelecAceleracion);
            //SerialPrintValor("", v, vc, 17, 254);
          }
          else
            vc = v;
          analogWrite(PIN_MOTOR,vc);
      }
      estado = ETAPA_GAS;
      tiempo = 0;
      SalidaLed7s('g',g_punto, VER_GAS);
      
      if ((vc < VALOR_MIN_TOPE_GAS) && (TopeGasTiempo > 0))
      {
        TiempoRecta = millis()-TopeGasTiempo;
        TopeGasTiempo = 0;
      }
      CfgSelecFreno = NO_SELEC;
    }
    else if (v==0)//Etapa de frenado *********************************************************************************
    { 
      int i;
      digitalWrite(PIN_MOTOR, LOW);

      if (SelecFreno == 'A')
      { //Freno pilotado de selección automática
        if (TiempoRecta > 0)
        {
          CfgSelecFreno = 5; //Por defecto escoge la 5 en modo automatico
          DEBUG("Tiempo Recta=", TiempoRecta);
          for(i=0;i<5;i++)
          { //Buscamos configuraciones automáticas del 0..4
            TiempoCfgRecta = CfgFreno[i].TiempoRecta*50;
            DEBUG("T cdf recta=", TiempoCfgRecta);
            if ((TiempoCfgRecta > 0) && (TiempoRecta > TiempoCfgRecta))
            { //Buscamos del 0..4 configuraciones de freno con datos para tiempo de recta. Las configuraciones se ordenan de recta mas larga a mas corta
              CfgSelecFreno = i;
            }
          }
            
          DEBUG("Selec=", CfgSelecFreno);
          TiempoRecta = 0;
        }
        else if (CfgSelecFreno == NO_SELEC)
          CfgSelecFreno = 5;
      }
      else
        CfgSelecFreno = SelecFreno;
      
      if (SelecFreno == 'd')
      {
        if (TiempoRecta > 0)
        {
          DEBUG("Tiempo Recta=", TiempoRecta);
          estado = FRENO_PROGRESIVO;
          rele = LOW;
          prog = (TiempoRecta * 100L) / (FrenoDinamico.TiempoPotenciaMax*20); //El parámetro varía entre 0 y dos segundos, a partir de ahi potencia máxima
          if (prog > 100) prog = 100;
          FrenoProgresivo(prog);
          SalidaLed7s('d',g_punto, VER_GAS);
          TiempoRecta = 0;
        }
      }
      else if (CfgSelecFreno != NO_SELEC)
      {
        if  (CfgFreno[CfgSelecFreno].orden == 0)
        { //No hay información de freno en la configuración seleccionada
          SalidaLed7s(BORRAR_LED,g_punto, VER_GAS);
          return;          
        }
        if ((tiempo == 0) && (estado == ETAPA_GAS))
        {
          prog = 0;
          rele = LOW;
          tiempo = millis();
          if  (CfgFreno[CfgSelecFreno].orden == 'r')
          {
              estado = FRENO_RELE;
              rele = HIGH;
              prog = 0;
              SalidaLed7s('r',g_punto, VER_GAS);
          }
          else
          {
              estado = FRENO_PROGRESIVO;
              rele = LOW;
              prog = 100-CfgFreno[CfgSelecFreno].PotenciaFrenoProgresivo/2;
              
              DEBUG("Freno=", prog);
              
              SalidaLed7s('P',g_punto, VER_GAS);
          }
            DEBUG("1----------------rele ->", rele);
          digitalWrite(PIN_FRENO_RELE, rele);
          FrenoProgresivo(prog);      
        }
        if (estado == FRENO_RELE)
        {
              //Si se excede el tiempo se cambia de freno
              if ((millis()-tiempo >= CfgFreno[CfgSelecFreno].TiempoFrenoRele*30) && (CfgFreno[CfgSelecFreno].TiempoFrenoRele > 0)) // Si el tiempo = 0 es indefinido
              {
                rele = LOW;
                prog = 100-CfgFreno[CfgSelecFreno].PotenciaFrenoProgresivo/2;
                estado = ETAPA_FINAL_FRENO;
                
                //DEBUG("2-------------rele ->", rele);
                digitalWrite(PIN_FRENO_RELE, rele);      
                FrenoProgresivo(prog);
                SalidaLed7s('P',g_punto, VER_GAS);
              }
        }
        else if (estado == FRENO_PROGRESIVO)
        {
            if ((millis()-tiempo >= CfgFreno[CfgSelecFreno].TiempoFrenoRele*30) && (CfgFreno[CfgSelecFreno].TiempoFrenoRele > 0)) // Si el tiempo = 0 es indefinido
            {
              rele = HIGH;
              prog = 0;
              estado = ETAPA_FINAL_FRENO_RELE;
              tiempo = millis();
              
              DEBUG("3-------------rele ->", rele);
              digitalWrite(PIN_FRENO_RELE, rele);      
              FrenoProgresivo(prog);      
              SalidaLed7s('r',g_punto, VER_GAS);
            }
        }
        if ((estado == FRENO_RELE) || ((estado == ETAPA_FINAL_FRENO_RELE)))
        {
            if (millis()-tiempo >= MAX_TIEMPO_RELE)
            {
              //DEBUG("4-------------rele ->", 0);
              estado = ETAPA_FINAL_SIN_FRENO;
              digitalWrite(PIN_FRENO_RELE, LOW);      
              SalidaLed7s(BORRAR_LED,g_punto, VER_GAS);
            }
        }
      }
    }
    
}
#undef DEBUG


/******************************************************************************************************************************
int AsignarCurvaAceleracion(int &velocidad, int num_curva)
*******************************************************************************************************************************/

int AsignarCurvaAceleracion(int &velocidad, int num_curva)
{
#define N_DEBUG
  int v100, vtramo, vn;
  int i = 0;
  int s;
  int p1, p2;
  
  //Normalizamos la velocidad sobre 100
  v100 = (velocidad*100)/255;
  
  vtramo = CfgAceleracion[num_curva].Tramo[0].Gatillo;
  
  while((v100 >= vtramo) && (vtramo != 0) && (i<MAX_TRAMOS_ACEL))
  {
    vtramo = CfgAceleracion[num_curva].Tramo[++i].Gatillo;
  }
  
  if ((i == MAX_TRAMOS_ACEL) || (vtramo == 0))
  {//Estamos fuera del último tramo, el valor es mayor que el más alto configurado o el primer valor de gatillo es la sensibilidad => no hay curva
     s = CfgAceleracion[num_curva].Sensibilidad;
     p1 = s+CfgAceleracion[num_curva].Tramo[i-1].Potencia*(100-s)/100;
     p2 = 100;
     vn = map(v100, CfgAceleracion[num_curva].Tramo[i-1].Gatillo, 100, p1, p2);
  }
  else if (i == 0) //curva no definida
  {
    if (vtramo == 0)
    {//La curva de velocidad está vacía
       vn = v100;
    }
    //El valor del gatillo es menor que el primer valor
    s = CfgAceleracion[num_curva].Sensibilidad;
    p1 = s;
    p2 = s+CfgAceleracion[num_curva].Tramo[0].Potencia*(100-s)/100;
    vn = map(v100, 0, CfgAceleracion[num_curva].Tramo[0].Gatillo, p1, p2);
  }
  else
  { //Estamos entre tramos
    s = CfgAceleracion[num_curva].Sensibilidad;
    p1 = s+CfgAceleracion[num_curva].Tramo[i-1].Potencia*(100-s)/100;
    p2 = s+CfgAceleracion[num_curva].Tramo[i].Potencia*(100-s)/100;
    vn = map(v100, CfgAceleracion[num_curva].Tramo[i-1].Gatillo, CfgAceleracion[num_curva].Tramo[i].Gatillo, p1, p2);
  }
  #ifdef DEBUG
    SerialPrint("Velocidad ajustada a curva de potencia = ", vn, ",Vn100=", v100);
  #endif
  
  //Antes de normalizar la velocidad aplicamos la limitación de potencia
  vn = vn*(CfgAceleracion[num_curva].PotenciaMax+1)/100;
  
  //Volvemos a normalizar la velocidad en 0..255
  vn = vn*255/100;
  
  return vn;
#undef DEBUG  
}

void CorregirValorHallNormalizado(int posicion, int &velocidad, int algoritmo)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  static long ms = 0;
  static long acumulado = 0;
  static int media = 0, tmp = 0, valores = 0;
  static byte v[MAX_VALORES_CORRECION];
  static byte i = 0, j = 0;
  static byte lectura = 0;
  static long acumulado_p = 0, valores_p = 0, media_p = 0;
  //Recuperamos la información de un ms, calculamos la media y la entregamos el ms siguiente
  
  if (algoritmo == ALGO_MEDIA)
  {
    if ((millis() % AlgoMedia.Tiempo) == 0)
    {
      ms = millis();
      tmp = velocidad;
      media = acumulado/valores;
      acumulado = tmp;
      valores = 1;
    }
    else
    {
      valores++;
      acumulado += velocidad;
    }
    velocidad = media;
  }
  else if (algoritmo == ALGO_PROGRESIVO)
  {
    valores_p++;
    acumulado_p += velocidad;
    
    if (!AlgoProgresivo.Tiempo || (millis() % (AlgoProgresivo.Tiempo+1)) == 0)
    {
      if (!lectura || !AlgoProgresivo.Tiempo)
      {
        int Repeticion;
        lectura = 1;
        media_p = acumulado_p/valores_p;
        if (media_p < media)
          Repeticion = AlgoProgresivo.ProporcionFrenado;
        else
          Repeticion = 1;
          
        for (j=0; j<Repeticion; j++)
        {
          acumulado -= v[i];
          v[i] = media_p;
          acumulado += v[i];
          i = (i+1)%MAX_VALORES_CORRECION;
        }
        media = acumulado/MAX_VALORES_CORRECION;
        
        acumulado_p = 0;
        valores_p = 0;
      }
    }
    else
      lectura  = 0;
      
    velocidad = media;
    
    /*if (i == 0)
    {
      DEBUG("ms = ", millis()-ms);
      ms = millis();
    }*/
  }
}
#undef DEBUG

void FrenoProgresivo(int valor)
//define DEBUG(a,b) SerialPrint(a,b);
#define DEBUG(a,b) ;
{
  valor = valor*255/100;
  DEBUG("Freno prog=",valor)
  if (valor == 255)
    digitalWrite(PIN_FRENO, HIGH);
  else if (valor == 0)
    digitalWrite(PIN_FRENO, LOW);
  else
    analogWrite(PIN_FRENO, valor);
}
#undef DEBUG

void LecturaVoltimetro(int Seg)
{
  int valor;
    EscribirLed7s(8,0);

    Espera(0);
    while (!Espera(Seg*1000))
    {
      valor = analogRead(PIN_VOLTIMETRO);
      valor = ((3.34/142)*(valor-417)+10)*100;
      VerValorLed7s(valor);
    }
}
